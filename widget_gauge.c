/*
 * gauge widget handling
 *
 * Renders a circular arc gauge that fills proportionally to an expression
 * value. Uses GD's gdImageFilledArc() for drawing arcs.
 *
 * Copyright (C) 2025 lcd4linux-ax206 contributors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */


#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#ifdef HAVE_GD_GD_H
#include <gd/gd.h>
#else
#ifdef HAVE_GD_H
#include <gd.h>
#else
#error "gd.h not found!"
#error "cannot compile gauge widget"
#endif
#endif

#if GD2_VERS != 2
#error "lcd4linux requires libgd version 2"
#error "cannot compile gauge widget"
#endif

#include "debug.h"
#include "cfg.h"
#include "property.h"
#include "rgb.h"
#include "drv_generic.h"
#include "drv_generic_graphic.h"
#include "timer_group.h"
#include "widget.h"
#include "widget_gauge.h"

#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif


static int getcolorint(PROPERTY *colorprop, void *Image)
{
    char          *colorstr;
    int            colorint;
    char          *e;
    unsigned long  l;
    unsigned char  r, g, b, a;

    colorstr = P2S(colorprop);

    if (strlen(colorstr) == 8) {
        l = strtoul(colorstr, &e, 16);
        r = (l >> 24) & 0xff;
        g = (l >> 16) & 0xff;
        b = (l >> 8) & 0xff;
        a = (l & 0xff) / 2;

        colorint = gdImageColorAllocateAlpha(Image, r, g, b, a);
    } else {
        l = strtoul(colorstr, &e, 16);
        r = (l >> 16) & 0xff;
        g = (l >> 8) & 0xff;
        b = l & 0xff;

        colorint = gdImageColorAllocate(Image, r, g, b);
    }

    return colorint;
}


static void widget_gauge_render(const char *Name, WIDGET_GAUGE *Gauge)
{
    int x, y;
    int inverted;
    gdImagePtr gdImage;
    int colorbg, colorfg;
    double dmin, dmax, dval, scaled;
    int start_angle, sweep_angle, thickness;
    int cx, cy, diameter;
    int i;

    /* get min/max */
    dmin = property_valid(&Gauge->expr_min) ? P2N(&Gauge->expr_min) : 0.0;
    dmax = property_valid(&Gauge->expr_max) ? P2N(&Gauge->expr_max) : 100.0;
    if (dmin == dmax)
        dmax += 1;

    /* get value and scale to 0..1 */
    dval = P2N(&Gauge->value);
    scaled = (dval - dmin) / (dmax - dmin);
    if (scaled < 0.0) scaled = 0.0;
    if (scaled > 1.0) scaled = 1.0;

    /* get arc parameters */
    start_angle = property_valid(&Gauge->start) ? (int)P2N(&Gauge->start) : 135;
    sweep_angle = property_valid(&Gauge->sweep) ? (int)P2N(&Gauge->sweep) : 270;
    thickness = property_valid(&Gauge->thickness) ? (int)P2N(&Gauge->thickness) : 0;

    /* clear bitmap */
    if (Gauge->bitmap) {
        Gauge->oldheight = Gauge->xsize;
        for (i = 0; i < Gauge->ysize * Gauge->xsize; i++) {
            RGBA empty = {.R = 0x00, .G = 0x00, .B = 0x00, .A = 0xff};
            Gauge->bitmap[i] = empty;
        }
    }

    /* free previous image */
    if (Gauge->gdImage) {
        gdImageDestroy(Gauge->gdImage);
        Gauge->gdImage = NULL;
    }

    Gauge->gdImage = gdImageCreateTrueColor(Gauge->xsize, Gauge->ysize);
    gdImageSaveAlpha(Gauge->gdImage, 1);

    if (Gauge->gdImage == NULL) {
        error("Warning: Gauge %s: Create failed!", Name);
        return;
    }

    gdImage = Gauge->gdImage;

    /* fill with transparent background */
    int transparent = gdImageColorAllocateAlpha(gdImage, 0, 0, 0, 127);
    gdImageFill(gdImage, 0, 0, transparent);

    /* center and diameter of the arc */
    cx = Gauge->xsize / 2;
    cy = Gauge->ysize / 2;
    diameter = (Gauge->xsize < Gauge->ysize) ? Gauge->xsize : Gauge->ysize;

    /* allocate colors */
    colorbg = getcolorint(&Gauge->background, gdImage);
    colorfg = getcolorint(&Gauge->color, gdImage);

    /* apply color thresholds based on current value */
    int has_low = property_valid(&Gauge->valuelow) && property_valid(&Gauge->colorlow);
    int has_high = property_valid(&Gauge->valuehigh) && property_valid(&Gauge->colorhigh);

    if (has_high && dval > P2N(&Gauge->valuehigh)) {
        colorfg = getcolorint(&Gauge->colorhigh, gdImage);
    } else if (has_low && dval < P2N(&Gauge->valuelow)) {
        colorfg = getcolorint(&Gauge->colorlow, gdImage);
    }

    /* determine fill direction */
    char *dir_str = P2S(&Gauge->direction);
    int is_ccw = (dir_str && (strcasecmp(dir_str, "CCW") == 0));

    /* calculate value arc angles based on direction */
    int value_degrees = (int)(scaled * sweep_angle + 0.5);
    int val_start, val_end, full_start, full_end;

    if (is_ccw) {
        /* counter-clockwise: value grows backward from start */
        full_start = start_angle - sweep_angle;
        full_end = start_angle;
        val_start = start_angle - value_degrees;
        val_end = start_angle;
    } else {
        /* clockwise: value grows forward from start */
        full_start = start_angle;
        full_end = start_angle + sweep_angle;
        val_start = start_angle;
        val_end = start_angle + value_degrees;
    }

    /* check reverse mode */
    int is_reverse = (int)P2N(&Gauge->reverse);

    if (is_reverse) {
        /* reverse fill: foreground shows the remaining portion */
        gdImageFilledArc(gdImage, cx, cy, diameter - 1, diameter - 1,
                         full_start, full_end, colorfg, gdArc);
        if (val_end > val_start) {
            gdImageFilledArc(gdImage, cx, cy, diameter - 1, diameter - 1,
                             val_start, val_end, colorbg, gdArc);
        }
    } else {
        /* normal fill: foreground shows the value portion */
        gdImageFilledArc(gdImage, cx, cy, diameter - 1, diameter - 1,
                         full_start, full_end, colorbg, gdArc);
        if (val_end > val_start) {
            gdImageFilledArc(gdImage, cx, cy, diameter - 1, diameter - 1,
                             val_start, val_end, colorfg, gdArc);
        }
    }

    /* if thickness is set, punch out the inner circle to create a ring */
    if (thickness > 0) {
        int inner_diameter = diameter - 2 * thickness;
        if (inner_diameter > 0) {
            gdImageFilledEllipse(gdImage, cx, cy, inner_diameter, inner_diameter, transparent);
        }
    }

    /* allocate bitmap if needed */
    if (Gauge->bitmap == NULL && Gauge->ysize > 0 && Gauge->xsize > 0) {
        int size = Gauge->ysize * Gauge->xsize * sizeof(Gauge->bitmap[0]);
        Gauge->bitmap = malloc(size);
        if (Gauge->bitmap == NULL) {
            error("Warning: Gauge malloc failed");
            return;
        }
        for (i = 0; i < Gauge->ysize * Gauge->xsize; i++) {
            RGBA empty = {.R = 0x00, .G = 0x00, .B = 0x00, .A = 0x00};
            Gauge->bitmap[i] = empty;
        }
    }

    /* copy gdImage pixels to bitmap with alpha conversion */
    inverted = P2N(&Gauge->inverted);
    if (P2N(&Gauge->visible)) {
        for (x = 0; x < gdImage->sx; x++) {
            for (y = 0; y < gdImage->sy; y++) {
                int p = gdImageGetTrueColorPixel(gdImage, x, y);
                int a = gdTrueColorGetAlpha(p);
                int idx = x * Gauge->ysize + y;
                Gauge->bitmap[idx].R = gdTrueColorGetRed(p);
                Gauge->bitmap[idx].G = gdTrueColorGetGreen(p);
                Gauge->bitmap[idx].B = gdTrueColorGetBlue(p);
                /* GD's alpha is 0 (opaque) to 127 (transparent) */
                /* our alpha is 0 (transparent) to 255 (opaque) */
                Gauge->bitmap[idx].A = (a == 127) ? 0 : 255 - 2 * a;
                if (inverted) {
                    Gauge->bitmap[idx].R = 255 - Gauge->bitmap[idx].R;
                    Gauge->bitmap[idx].G = 255 - Gauge->bitmap[idx].G;
                    Gauge->bitmap[idx].B = 255 - Gauge->bitmap[idx].B;
                }
            }
        }
    }
}


static void widget_gauge_update(void *Self)
{
    WIDGET *W = (WIDGET *) Self;
    WIDGET_GAUGE *Gauge = W->data;

    /* process the parent only */
    if (W->parent == NULL) {

        /* evaluate properties */
        property_eval(&Gauge->value);
        property_eval(&Gauge->expr_min);
        property_eval(&Gauge->expr_max);
        property_eval(&Gauge->color);
        property_eval(&Gauge->colorlow);
        property_eval(&Gauge->valuelow);
        property_eval(&Gauge->colorhigh);
        property_eval(&Gauge->valuehigh);
        property_eval(&Gauge->background);
        property_eval(&Gauge->start);
        property_eval(&Gauge->sweep);
        property_eval(&Gauge->thickness);
        property_eval(&Gauge->reverse);
        property_eval(&Gauge->direction);
        property_eval(&Gauge->update);
        property_eval(&Gauge->reload);
        property_eval(&Gauge->visible);

        /* render image into bitmap */
        widget_gauge_render(W->name, Gauge);
    }

    /* finally, draw it! */
    if (W->class->draw)
        W->class->draw(W);

    /* add a new one-shot timer */
    if (P2N(&Gauge->update) > 0) {
        timer_add_widget(widget_gauge_update, Self, P2N(&Gauge->update), 1);
    }
}


int widget_gauge_init(WIDGET *Self)
{
    char *section;
    WIDGET_GAUGE *Gauge;

    /* re-use the parent if one exists */
    if (Self->parent == NULL) {

        /* prepare config section */
        section = malloc(strlen(Self->name) + 8);
        strcpy(section, "Widget:");
        strcat(section, Self->name);

        Gauge = malloc(sizeof(WIDGET_GAUGE));
        memset(Gauge, 0, sizeof(WIDGET_GAUGE));

        /* initial state */
        Gauge->bitmap = NULL;

        /* load properties */
        property_load(section, "min",        NULL,       &Gauge->expr_min);
        property_load(section, "max",        NULL,       &Gauge->expr_max);
        property_load(section, "width",      NULL,       &Gauge->prop_width);
        property_load(section, "height",     NULL,       &Gauge->prop_height);
        property_load(section, "update",     "1000",     &Gauge->update);
        property_load(section, "reload",     "0",        &Gauge->reload);
        property_load(section, "visible",    "1",        &Gauge->visible);
        property_load(section, "inverted",   "0",        &Gauge->inverted);
        property_load(section, "center",     "0",        &Gauge->center);
        property_load(section, "expression", NULL,       &Gauge->value);
        property_load(section, "color",      "00ff00",   &Gauge->color);
        property_load(section, "colorlow",   NULL,       &Gauge->colorlow);
        property_load(section, "valuelow",   NULL,       &Gauge->valuelow);
        property_load(section, "colorhigh",  NULL,       &Gauge->colorhigh);
        property_load(section, "valuehigh",  NULL,       &Gauge->valuehigh);
        property_load(section, "background", "333333",   &Gauge->background);
        property_load(section, "start",      "135",      &Gauge->start);
        property_load(section, "sweep",      "270",      &Gauge->sweep);
        property_load(section, "thickness",  "0",        &Gauge->thickness);
        property_load(section, "reverse",    "0",        &Gauge->reverse);
        property_load(section, "direction",  "CW",       &Gauge->direction);

        /* sanity checks */
        if (!property_valid(&Gauge->value)) {
            error("Warning: widget %s has no expression", section);
        }
        if (!property_valid(&Gauge->prop_width)) {
            error("Warning: widget %s has no width", section);
            free(section);
            free(Gauge);
            return 1;
        }
        if (!property_valid(&Gauge->prop_height)) {
            error("Warning: widget %s has no height", section);
            free(section);
            free(Gauge);
            return 1;
        }

        property_eval(&Gauge->prop_width);
        property_eval(&Gauge->prop_height);

        Gauge->xsize = P2N(&Gauge->prop_width);
        Gauge->ysize = P2N(&Gauge->prop_height);

        free(section);
        Self->data = Gauge;
        Self->x2 = Self->col + Gauge->xsize - 1;
        Self->y2 = Self->row + Gauge->ysize - 1;

    } else {
        /* re-use the parent */
        Self->data = Self->parent->data;
    }

    /* initial render */
    widget_gauge_update(Self);

    return 0;
}


int widget_gauge_quit(WIDGET *Self)
{
    if (Self) {
        /* do not deallocate child widget! */
        if (Self->parent == NULL) {
            if (Self->data) {
                WIDGET_GAUGE *Gauge = Self->data;
                if (Gauge->gdImage) {
                    gdImageDestroy(Gauge->gdImage);
                    Gauge->gdImage = NULL;
                }
                free(Gauge->bitmap);
                property_free(&Gauge->expr_min);
                property_free(&Gauge->expr_max);
                property_free(&Gauge->prop_width);
                property_free(&Gauge->prop_height);
                property_free(&Gauge->update);
                property_free(&Gauge->reload);
                property_free(&Gauge->visible);
                property_free(&Gauge->inverted);
                property_free(&Gauge->center);
                property_free(&Gauge->value);
                property_free(&Gauge->color);
                property_free(&Gauge->colorlow);
                property_free(&Gauge->valuelow);
                property_free(&Gauge->colorhigh);
                property_free(&Gauge->valuehigh);
                property_free(&Gauge->background);
                property_free(&Gauge->start);
                property_free(&Gauge->sweep);
                property_free(&Gauge->thickness);
                property_free(&Gauge->reverse);
                property_free(&Gauge->direction);

                free(Self->data);
                Self->data = NULL;
            }
        }
    }

    return 0;
}


WIDGET_CLASS Widget_Gauge = {
    .name = "gauge",
    .type = WIDGET_TYPE_XY,
    .init = widget_gauge_init,
    .draw = NULL,
    .quit = widget_gauge_quit,
};
