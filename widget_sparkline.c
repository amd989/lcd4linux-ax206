/*
 * sparkline widget handling
 *
 * Renders a line graph of historical values sampled from an expression.
 * Stores samples in a ring buffer (one per pixel column) and draws
 * connected line segments using GD's gdImageLine().
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

#ifdef HAVE_GD_GD_H
#include <gd/gd.h>
#else
#ifdef HAVE_GD_H
#include <gd.h>
#else
#error "gd.h not found!"
#error "cannot compile sparkline widget"
#endif
#endif

#if GD2_VERS != 2
#error "lcd4linux requires libgd version 2"
#error "cannot compile sparkline widget"
#endif

#include "debug.h"
#include "cfg.h"
#include "property.h"
#include "rgb.h"
#include "drv_generic.h"
#include "drv_generic_graphic.h"
#include "timer_group.h"
#include "widget.h"
#include "widget_sparkline.h"

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


static void widget_sparkline_render(const char *Name, WIDGET_SPARKLINE *Spark)
{
    int x, y;
    int inverted;
    gdImagePtr gdImage;
    int colorbackground;
    int colorline;
    double dmin, dmax;
    int i;

    /* determine Y-axis range */
    if (property_valid(&Spark->expr_min)) {
        dmin = P2N(&Spark->expr_min);
    } else {
        dmin = Spark->min;
    }

    if (property_valid(&Spark->expr_max)) {
        dmax = P2N(&Spark->expr_max);
    } else {
        dmax = Spark->max;
    }

    if (dmin == dmax)
        dmax += 1;

    /* clear bitmap */
    if (Spark->bitmap) {
        Spark->oldheight = Spark->xsize;
        for (i = 0; i < Spark->ysize * Spark->xsize; i++) {
            RGBA empty = {.R = 0x00, .G = 0x00, .B = 0x00, .A = 0xff};
            Spark->bitmap[i] = empty;
        }
    }

    /* free previous image */
    if (Spark->gdImage) {
        gdImageDestroy(Spark->gdImage);
        Spark->gdImage = NULL;
    }

    Spark->gdImage = gdImageCreateTrueColor(Spark->xsize, Spark->ysize);
    gdImageSaveAlpha(Spark->gdImage, 1);

    if (Spark->gdImage == NULL) {
        error("Warning: Sparkline %s: Create failed!", Name);
        return;
    }

    gdImage = Spark->gdImage;

    /* fill background */
    colorbackground = getcolorint(&Spark->background, gdImage);
    gdImageFill(gdImage, 0, 0, colorbackground);

    /* draw the sparkline */
    colorline = getcolorint(&Spark->color, gdImage);

    /* prepare threshold colors if configured */
    int has_low = property_valid(&Spark->valuelow) && property_valid(&Spark->colorlow);
    int has_high = property_valid(&Spark->valuehigh) && property_valid(&Spark->colorhigh);
    double dlow = has_low ? P2N(&Spark->valuelow) : 0;
    double dhigh = has_high ? P2N(&Spark->valuehigh) : 0;
    int colorlow_gd = has_low ? getcolorint(&Spark->colorlow, gdImage) : colorline;
    int colorhigh_gd = has_high ? getcolorint(&Spark->colorhigh, gdImage) : colorline;

    if (Spark->count >= 2) {
        int oldest;
        int px, py, cx, cy;
        double pval;

        /* oldest sample index in ring buffer */
        if (Spark->count < Spark->nsamples) {
            oldest = 0;
        } else {
            oldest = Spark->head % Spark->nsamples;
        }

        /* In portrait mode, GD x-axis = screen vertical, GD y-axis = screen horizontal.
         * xsize = width (short/vertical), ysize = length (long/horizontal).
         * Time samples span ysize (horizontal on screen), values span xsize (vertical). */

        for (i = 0; i < Spark->count; i++) {
            int idx = (oldest + i) % Spark->nsamples;
            double val = Spark->history[idx];
            double scaled;

            /* map sample index to GD y-axis (horizontal on screen) */
            if (Spark->count > 1) {
                cy = (i * (Spark->ysize - 1)) / (Spark->count - 1);
            } else {
                cy = 0;
            }

            /* map value to GD x-axis (vertical on screen, inverted: max at top) */
            scaled = (val - dmin) / (dmax - dmin);
            if (scaled < 0.0) scaled = 0.0;
            if (scaled > 1.0) scaled = 1.0;
            cx = (int)((1.0 - scaled) * (Spark->xsize - 1) + 0.5);

            if (i > 0) {
                /* pick color based on the segment's midpoint value */
                double midval = (pval + val) / 2.0;
                int segcolor;

                if (has_high && midval > dhigh) {
                    segcolor = colorhigh_gd;
                } else if (has_low && midval < dlow) {
                    segcolor = colorlow_gd;
                } else {
                    segcolor = colorline;
                }

                gdImageLine(gdImage, px, py, cx, cy, segcolor);
            }

            px = cx;
            py = cy;
            pval = val;
        }
    }

    /* allocate bitmap if needed */
    if (Spark->bitmap == NULL && Spark->ysize > 0 && Spark->xsize > 0) {
        int size = Spark->ysize * Spark->xsize * sizeof(Spark->bitmap[0]);
        Spark->bitmap = malloc(size);
        if (Spark->bitmap == NULL) {
            error("Warning: Sparkline malloc failed");
            return;
        }
        for (i = 0; i < Spark->ysize * Spark->xsize; i++) {
            RGBA empty = {.R = 0x00, .G = 0x00, .B = 0x00, .A = 0x00};
            Spark->bitmap[i] = empty;
        }
    }

    /* copy gdImage pixels to bitmap with alpha conversion */
    inverted = P2N(&Spark->inverted);
    if (P2N(&Spark->visible)) {
        for (x = 0; x < gdImage->sx; x++) {
            for (y = 0; y < gdImage->sy; y++) {
                int p = gdImageGetTrueColorPixel(gdImage, x, y);
                int a = gdTrueColorGetAlpha(p);
                int idx = x * Spark->ysize + y;
                Spark->bitmap[idx].R = gdTrueColorGetRed(p);
                Spark->bitmap[idx].G = gdTrueColorGetGreen(p);
                Spark->bitmap[idx].B = gdTrueColorGetBlue(p);
                /* GD's alpha is 0 (opaque) to 127 (transparent) */
                /* our alpha is 0 (transparent) to 255 (opaque) */
                Spark->bitmap[idx].A = (a == 127) ? 0 : 255 - 2 * a;
                if (inverted) {
                    Spark->bitmap[idx].R = 255 - Spark->bitmap[idx].R;
                    Spark->bitmap[idx].G = 255 - Spark->bitmap[idx].G;
                    Spark->bitmap[idx].B = 255 - Spark->bitmap[idx].B;
                }
            }
        }
    }
}


static void widget_sparkline_update(void *Self)
{
    WIDGET *W = (WIDGET *) Self;
    WIDGET_SPARKLINE *Spark = W->data;

    /* process the parent only */
    if (W->parent == NULL) {
        double val;

        /* evaluate properties */
        property_eval(&Spark->value);
        property_eval(&Spark->expr_min);
        property_eval(&Spark->expr_max);
        property_eval(&Spark->color);
        property_eval(&Spark->colorlow);
        property_eval(&Spark->valuelow);
        property_eval(&Spark->colorhigh);
        property_eval(&Spark->valuehigh);
        property_eval(&Spark->background);
        property_eval(&Spark->update);
        property_eval(&Spark->reload);
        property_eval(&Spark->visible);

        /* sample the value into the ring buffer */
        val = P2N(&Spark->value);

        if (Spark->history != NULL && Spark->nsamples > 0) {
            Spark->history[Spark->head % Spark->nsamples] = val;
            Spark->head++;
            if (Spark->count < Spark->nsamples)
                Spark->count++;

            /* update auto-scale bounds from the current buffer contents */
            if (!property_valid(&Spark->expr_min) || !property_valid(&Spark->expr_max)) {
                int i, start;
                double bmin, bmax;

                if (Spark->count < Spark->nsamples)
                    start = 0;
                else
                    start = Spark->head % Spark->nsamples;

                bmin = bmax = Spark->history[start];
                for (i = 1; i < Spark->count; i++) {
                    int idx = (start + i) % Spark->nsamples;
                    double v = Spark->history[idx];
                    if (v < bmin) bmin = v;
                    if (v > bmax) bmax = v;
                }

                if (!property_valid(&Spark->expr_min))
                    Spark->min = bmin;
                if (!property_valid(&Spark->expr_max))
                    Spark->max = bmax;
            }
        }

        /* render image into bitmap */
        widget_sparkline_render(W->name, Spark);
    }

    /* finally, draw it! */
    if (W->class->draw)
        W->class->draw(W);

    /* add a new one-shot timer */
    if (P2N(&Spark->update) > 0) {
        timer_add_widget(widget_sparkline_update, Self, P2N(&Spark->update), 1);
    }
}


int widget_sparkline_init(WIDGET *Self)
{
    char *section;
    WIDGET_SPARKLINE *Spark;

    /* re-use the parent if one exists */
    if (Self->parent == NULL) {

        /* prepare config section */
        section = malloc(strlen(Self->name) + 8);
        strcpy(section, "Widget:");
        strcat(section, Self->name);

        Spark = malloc(sizeof(WIDGET_SPARKLINE));
        memset(Spark, 0, sizeof(WIDGET_SPARKLINE));

        /* initial state */
        Spark->bitmap = NULL;
        Spark->history = NULL;
        Spark->head = 0;
        Spark->count = 0;
        Spark->min = 0;
        Spark->max = 0;

        /* load properties */
        property_load(section, "min",        NULL,       &Spark->expr_min);
        property_load(section, "max",        NULL,       &Spark->expr_max);
        property_load(section, "width",      NULL,       &Spark->width);
        property_load(section, "length",     NULL,       &Spark->length);
        property_load(section, "update",     "1000",     &Spark->update);
        property_load(section, "reload",     "0",        &Spark->reload);
        property_load(section, "visible",    "1",        &Spark->visible);
        property_load(section, "inverted",   "0",        &Spark->inverted);
        property_load(section, "center",     "0",        &Spark->center);
        property_load(section, "expression", NULL,       &Spark->value);
        property_load(section, "color",      "00ff00",   &Spark->color);
        property_load(section, "colorlow",   NULL,       &Spark->colorlow);
        property_load(section, "valuelow",   NULL,       &Spark->valuelow);
        property_load(section, "colorhigh",  NULL,       &Spark->colorhigh);
        property_load(section, "valuehigh",  NULL,       &Spark->valuehigh);
        property_load(section, "background", "000000",   &Spark->background);

        /* sanity checks */
        if (!property_valid(&Spark->value)) {
            error("Warning: widget %s has no expression", section);
        }
        if (!property_valid(&Spark->length)) {
            error("Warning: widget %s has no length", section);
            free(section);
            free(Spark);
            return 1;
        }
        if (!property_valid(&Spark->width)) {
            error("Warning: widget %s has no width", section);
            free(section);
            free(Spark);
            return 1;
        }

        property_eval(&Spark->length);
        property_eval(&Spark->width);

        Spark->xsize = P2N(&Spark->width);
        Spark->ysize = P2N(&Spark->length);

        /* allocate ring buffer (one sample per pixel along the length axis) */
        Spark->nsamples = Spark->ysize;
        if (Spark->nsamples > 0) {
            Spark->history = calloc(Spark->nsamples, sizeof(double));
        }

        free(section);
        Self->data = Spark;
        Self->x2 = Self->col + Spark->xsize - 1;
        Self->y2 = Self->row + Spark->ysize - 1;

    } else {
        /* re-use the parent */
        Self->data = Self->parent->data;
    }

    /* initial render */
    widget_sparkline_update(Self);

    return 0;
}


int widget_sparkline_quit(WIDGET *Self)
{
    if (Self) {
        /* do not deallocate child widget! */
        if (Self->parent == NULL) {
            if (Self->data) {
                WIDGET_SPARKLINE *Spark = Self->data;
                if (Spark->gdImage) {
                    gdImageDestroy(Spark->gdImage);
                    Spark->gdImage = NULL;
                }
                free(Spark->bitmap);
                free(Spark->history);
                property_free(&Spark->expr_min);
                property_free(&Spark->expr_max);
                property_free(&Spark->width);
                property_free(&Spark->length);
                property_free(&Spark->update);
                property_free(&Spark->reload);
                property_free(&Spark->visible);
                property_free(&Spark->inverted);
                property_free(&Spark->center);
                property_free(&Spark->value);
                property_free(&Spark->color);
                property_free(&Spark->colorlow);
                property_free(&Spark->valuelow);
                property_free(&Spark->colorhigh);
                property_free(&Spark->valuehigh);
                property_free(&Spark->background);

                free(Self->data);
                Self->data = NULL;
            }
        }
    }

    return 0;
}


WIDGET_CLASS Widget_Sparkline = {
    .name = "sparkline",
    .type = WIDGET_TYPE_XY,
    .init = widget_sparkline_init,
    .draw = NULL,
    .quit = widget_sparkline_quit,
};
