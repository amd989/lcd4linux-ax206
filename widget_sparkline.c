/*
 * sparkline widget handling
 *
 * Renders a line graph of historical values sampled from an expression.
 * Stores samples in a ring buffer and draws
 * connected line segments using GD's gdImageLine().
 *
 * Copyright (C) 2025-2026 Alejandro Mora <amd989@users.noreply.github.com>
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
#include <math.h>
#include <strings.h>

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


typedef struct {
    double x, y;
} SparkPt;


static int spark_rgb_from_prop(PROPERTY *prop, unsigned char *r, unsigned char *g, unsigned char *b)
{
    char          *colorstr;
    char          *e;
    unsigned long  l;

    colorstr = P2S(prop);
    if (colorstr == NULL || strlen(colorstr) < 6)
        return -1;
    if (strlen(colorstr) == 8) {
        l = strtoul(colorstr, &e, 16);
        *r = (unsigned char)((l >> 24) & 0xff);
        *g = (unsigned char)((l >> 16) & 0xff);
        *b = (unsigned char)((l >> 8) & 0xff);
    } else {
        l = strtoul(colorstr, &e, 16);
        *r = (unsigned char)((l >> 16) & 0xff);
        *g = (unsigned char)((l >> 8) & 0xff);
        *b = (unsigned char)(l & 0xff);
    }
    return 0;
}


/* Leftmost x on the polyline at height iy (GD coords: x = value axis, y = time). */
static double spark_min_x_at_y(SparkPt *pts, int n, double iy)
{
    double xmin = 1e300;
    int    j;
    int    any = 0;

    for (j = 0; j < n - 1; j++) {
        double x0 = pts[j].x, x1 = pts[j + 1].x;
        double y0 = pts[j].y, y1 = pts[j + 1].y;

        if (fabs(y1 - y0) < 1e-9) {
            if (fabs(iy - y0) < 0.5) {
                double xa = x0 < x1 ? x0 : x1;

                if (xa < xmin) {
                    xmin = xa;
                    any = 1;
                }
            }
            continue;
        }
        if ((iy >= y0 && iy <= y1) || (iy >= y1 && iy <= y0)) {
            double x = x0 + (x1 - x0) * (iy - y0) / (y1 - y0);

            if (x < xmin) {
                xmin = x;
                any = 1;
            }
        }
    }
    return any ? xmin : -1.0;
}


/* Horizontal gradient: strong at the curve, transparent toward x = xsize-1 (min value).
 * Interpolate the left boundary per scanline when the spline wiggles in y (some rows miss
 * ray hits), avoiding vertical cracks from sparse scanline sampling. */
static void spark_fill_gradient_under(gdImagePtr im, SparkPt *pts, int n, int xsize, int ysize, unsigned char r,
                                      unsigned char g, unsigned char b, int alpha_curve)
{
    double *xcrow;
    int     iy, ix;
    int     xlast = xsize - 1;
    int     k;

    if (n < 2 || xsize < 2)
        return;

    xcrow = malloc((size_t)ysize * sizeof(double));
    if (xcrow == NULL)
        return;

    for (iy = 0; iy < ysize; iy++)
        xcrow[iy] = spark_min_x_at_y(pts, n, (double)iy + 0.5);

    for (iy = 0; iy < ysize; iy++) {
        if (xcrow[iy] >= 0.0)
            continue;
        k = iy - 1;
        while (k >= 0 && xcrow[k] < 0.0)
            k--;
        if (k >= 0) {
            int k2 = iy + 1;

            while (k2 < ysize && xcrow[k2] < 0.0)
                k2++;
            if (k2 < ysize)
                xcrow[iy] = xcrow[k] + (xcrow[k2] - xcrow[k]) * (double)(iy - k) / (double)(k2 - k);
            else
                xcrow[iy] = xcrow[k];
        } else {
            k = iy + 1;
            while (k < ysize && xcrow[k] < 0.0)
                k++;
            if (k < ysize)
                xcrow[iy] = xcrow[k];
            else
                xcrow[iy] = -1.0;
        }
    }

    gdImageAlphaBlending(im, 1);

    for (iy = 0; iy < ysize; iy++) {
        double xc = xcrow[iy];
        double span;
        double frac;
        int    a;

        if (xc < 0.0)
            continue;
        if (xc > (double)xlast)
            continue;

        span = (double)xlast - xc;
        if (span < 1.0)
            span = 1.0;

        for (ix = (int)floor(xc); ix <= xlast; ix++) {
            if (ix < 0)
                continue;
            frac = ((double)ix - xc) / span;
            if (frac < 0.0)
                frac = 0.0;
            if (frac > 1.0)
                frac = 1.0;
            /* GD alpha: 0 = opaque, 127 = transparent. Start ~44 at the curve so the fill
             * stays softer than full-opacity; ramp to fully transparent at the far edge. */
            {
                int amax = 127 - alpha_curve;
                a = alpha_curve + (int)((double)amax * frac + 0.5);
            }
            if (a > 127)
                a = 127;
            gdImageSetPixel(im, ix, iy, gdTrueColorAlpha(r, g, b, a));
        }
    }

    free(xcrow);
}

#define SPARK_SMOOTH_CAP 256

#define SPARK_STROKE_SOLID   0
#define SPARK_STROKE_DASHED  1
#define SPARK_STROKE_DOTTED  2

static int sparkline_parse_stroke(const char *s)
{
    if (s == NULL)
        return SPARK_STROKE_SOLID;
    if (!strcasecmp(s, "dashed") || !strcasecmp(s, "dash"))
        return SPARK_STROKE_DASHED;
    if (!strcasecmp(s, "dotted") || !strcasecmp(s, "dot"))
        return SPARK_STROKE_DOTTED;
    return SPARK_STROKE_SOLID;
}

/* Endpoints: reflect phantom points so adjacent spline pieces are C1 (no kinks at samples). */
/* 3-tap binomial low-pass along the time series (reduces step / square-wave look). */
static void spark_value_smooth(double *data, double *tmp, int n, int passes)
{
    int p, i;

    for (p = 0; p < passes; p++) {
        if (n < 2)
            return;
        tmp[0] = data[0];
        tmp[n - 1] = data[n - 1];
        for (i = 1; i < n - 1; i++)
            tmp[i] = 0.25 * data[i - 1] + 0.5 * data[i] + 0.25 * data[i + 1];
        memcpy(data, tmp, (size_t)n * sizeof(double));
    }
}

static void spark_catmull_controls(const SparkPt *ctrl, int n, int seg, SparkPt *p0, SparkPt *p1, SparkPt *p2,
                                   SparkPt *p3)
{
    *p1 = ctrl[seg];
    *p2 = ctrl[seg + 1];
    if (seg == 0) {
        p0->x = 2.0 * ctrl[0].x - ctrl[1].x;
        p0->y = 2.0 * ctrl[0].y - ctrl[1].y;
    } else {
        *p0 = ctrl[seg - 1];
    }
    if (seg + 2 >= n) {
        p3->x = 2.0 * ctrl[n - 1].x - ctrl[n - 2].x;
        p3->y = 2.0 * ctrl[n - 1].y - ctrl[n - 2].y;
    } else {
        *p3 = ctrl[seg + 2];
    }
}

/* Catmull-Rom segment from P1 to P2 with controls P0..P3 (uniform parameterization). */
static void spark_catmull_rom(double t, SparkPt p0, SparkPt p1, SparkPt p2, SparkPt p3, SparkPt *out)
{
    double t2 = t * t;
    double t3 = t2 * t;

    out->x = 0.5 * ((2.0 * p1.x) +
                    (-p0.x + p2.x) * t +
                    (2.0 * p0.x - 5.0 * p1.x + 4.0 * p2.x - p3.x) * t2 +
                    (-p0.x + 3.0 * p1.x - 3.0 * p2.x + p3.x) * t3);
    out->y = 0.5 * ((2.0 * p1.y) +
                    (-p0.y + p2.y) * t +
                    (2.0 * p0.y - 5.0 * p1.y + 4.0 * p2.y - p3.y) * t2 +
                    (-p0.y + 3.0 * p1.y - 3.0 * p2.y + p3.y) * t3);
}


static int spark_build_full_polyline(SparkPt *ctrl, int count, int do_smooth, int steps, SparkPt *segbuf,
                                     SparkPt *out, int out_max)
{
    int seg, j, fn;

    fn = 0;
    for (seg = 0; seg < count - 1; seg++) {
        int seg_n;

        if (!do_smooth) {
            segbuf[0] = ctrl[seg];
            segbuf[1] = ctrl[seg + 1];
            seg_n = 2;
        } else {
            SparkPt p0, p1, p2, p3;

            spark_catmull_controls(ctrl, count, seg, &p0, &p1, &p2, &p3);
            for (j = 0; j <= steps; j++) {
                double t = (double)j / (double)steps;

                spark_catmull_rom(t, p0, p1, p2, p3, &segbuf[j]);
            }
            seg_n = steps + 1;
        }
        {
            int st = (seg == 0) ? 0 : 1;

            for (j = st; j < seg_n; j++) {
                if (fn >= out_max)
                    return fn;
                out[fn++] = segbuf[j];
            }
        }
    }
    return fn;
}


static void spark_draw_polyline_solid(gdImagePtr im, SparkPt *pts, int n, int color, int ox, int oy)
{
    int j;

    if (n < 2)
        return;
    for (j = 0; j < n - 1; j++) {
        gdImageLine(im,
                    (int)(pts[j].x + 0.5) + ox, (int)(pts[j].y + 0.5) + oy,
                    (int)(pts[j + 1].x + 0.5) + ox, (int)(pts[j + 1].y + 0.5) + oy,
                    color);
    }
}

/* Walk the polyline, drawing only dash segments (on/off lengths in pixels).
 * dash_rem / dash_drawing carry phase across consecutive calls so dashes are not
 * restarted every sparkline sample segment (each ~1–3px — far shorter than one dash).
 */
static void spark_draw_polyline_dashed(gdImagePtr im, SparkPt *pts, int n, int color, int ox, int oy,
                                       double dash_on, double dash_gap,
                                       double *dash_rem, int *dash_drawing)
{
    int    i;
    int    drawing;
    double rem;
    double cx, cy;

    if (n < 2)
        return;

    i = 0;
    cx = pts[0].x;
    cy = pts[0].y;
    rem = *dash_rem;
    drawing = *dash_drawing;

    while (i < n - 1) {
        double tx = pts[i + 1].x;
        double ty = pts[i + 1].y;
        double dx = tx - cx;
        double dy = ty - cy;
        double elen = sqrt(dx * dx + dy * dy);

        if (elen < 1e-9) {
            i++;
            cx = pts[i].x;
            cy = pts[i].y;
            continue;
        }

        while (elen > 1e-9) {
            double step = rem < elen ? rem : elen;
            double nx = cx + (dx / elen) * step;
            double ny = cy + (dy / elen) * step;

            if (drawing) {
                gdImageLine(im,
                            (int)(cx + 0.5) + ox, (int)(cy + 0.5) + oy,
                            (int)(nx + 0.5) + ox, (int)(ny + 0.5) + oy,
                            color);
            }
            cx = nx;
            cy = ny;
            dx = tx - cx;
            dy = ty - cy;
            elen = sqrt(dx * dx + dy * dy);
            rem -= step;
            if (rem < 1e-9) {
                drawing = !drawing;
                rem = drawing ? dash_on : dash_gap;
            }
        }
        i++;
        cx = pts[i].x;
        cy = pts[i].y;
    }

    *dash_rem = rem;
    *dash_drawing = drawing;
}

static void spark_dash_lengths(int thickness, int stroke, double *dash_on, double *dash_gap)
{
    double scale = (thickness > 1) ? (0.65 * (double)thickness + 0.35) : 1.0;

    if (stroke == SPARK_STROKE_DOTTED) {
        *dash_on = 3.0 * scale;
        *dash_gap = 6.0 * scale;
    } else {
        /* Longer gaps than dashes so thick gdImageLine caps do not bridge gaps (looks solid). */
        *dash_on = 14.0 * scale;
        *dash_gap = 14.0 * scale;
    }
}

static void spark_draw_polyline_styled(gdImagePtr im, SparkPt *pts, int n, int color, int thickness,
                                       int stroke, int ox, int oy, double *dash_rem, int *dash_drawing)
{
    double dash_on, dash_gap;
    double local_rem;
    int    local_draw;
    int    draw_thick;

    if (n < 2)
        return;

    if (stroke == SPARK_STROKE_SOLID) {
        gdImageSetThickness(im, thickness);
        spark_draw_polyline_solid(im, pts, n, color, ox, oy);
    } else {
        /* Thick strokes merge dash gaps; draw dashes slightly thinner than the main solid line. */
        draw_thick = thickness;
        if (thickness > 2)
            draw_thick = thickness / 2;
        if (draw_thick < 1)
            draw_thick = 1;
        gdImageSetThickness(im, draw_thick);
        spark_dash_lengths(thickness, stroke, &dash_on, &dash_gap);
        if (dash_rem == NULL) {
            local_rem = dash_on;
            local_draw = 1;
            dash_rem = &local_rem;
            dash_drawing = &local_draw;
        }
        spark_draw_polyline_dashed(im, pts, n, color, ox, oy, dash_on, dash_gap, dash_rem, dash_drawing);
    }

    gdImageSetThickness(im, 1);
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
    if (Spark->gdImage == NULL) {
        error("Warning: Sparkline %s: Create failed!", Name);
        return;
    }
    gdImageSaveAlpha(Spark->gdImage, 1);

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

    {
        int            thickness = (int)(P2N(&Spark->thickness) + 0.5);
        int            do_smooth = (int)(P2N(&Spark->smooth) + 0.5) != 0;
        int            steps = (int)(P2N(&Spark->smoothsteps) + 0.5);
        int            stroke = sparkline_parse_stroke(P2S(&Spark->stroke));
        /* Bare word like "dashed" without quotes is treated as a variable by
         * the expression evaluator, resolving to "".  Fall back to the raw
         * expression string so unquoted enum values still work. */
        if (stroke == SPARK_STROKE_SOLID && Spark->stroke.expression != NULL) {
            int fb = sparkline_parse_stroke(Spark->stroke.expression);
            if (fb != SPARK_STROKE_SOLID)
                stroke = fb;
        }
        int            blurpasses = (int)(P2N(&Spark->valueblur) + 0.5);
        int            do_fillunder = (int)(P2N(&Spark->fillunder) + 0.5) != 0;
        int            fillalpha = (int)(P2N(&Spark->fillalpha) + 0.5);
        SparkPt       *ctrl = NULL;
        double        *vbuf = NULL;
        double        *vtmp = NULL;
        SparkPt        segbuf[SPARK_SMOOTH_CAP + 1];
        int            seg_n;
        int            oldest;
        int            seg;

        if (thickness < 1)
            thickness = 1;
        if (thickness > 32)
            thickness = 32;
        if (steps < 2)
            steps = 2;
        if (steps > SPARK_SMOOTH_CAP)
            steps = SPARK_SMOOTH_CAP;
        if (blurpasses < 0)
            blurpasses = 0;
        if (blurpasses > 16)
            blurpasses = 16;
        if (fillalpha < 0)
            fillalpha = 0;
        if (fillalpha > 127)
            fillalpha = 127;

        if (Spark->count >= 2) {
            ctrl = malloc(Spark->count * sizeof(SparkPt));
            vbuf = malloc(Spark->count * sizeof(double));
            if (ctrl == NULL || vbuf == NULL) {
                error("Warning: Sparkline %s: malloc failed", Name);
                free(ctrl);
                free(vbuf);
            } else {

                if (Spark->count < Spark->nsamples) {
                    oldest = 0;
                } else {
                    oldest = Spark->head % Spark->nsamples;
                }

                for (i = 0; i < Spark->count; i++) {
                    int idx = (oldest + i) % Spark->nsamples;

                    vbuf[i] = Spark->history[idx];
                }

                if (blurpasses > 0) {
                    vtmp = malloc(Spark->count * sizeof(double));
                    if (vtmp == NULL) {
                        error("Warning: Sparkline %s: malloc failed", Name);
                    } else {
                        spark_value_smooth(vbuf, vtmp, Spark->count, blurpasses);
                    }
                }

                /* In portrait mode, GD x-axis = screen vertical, GD y-axis = screen horizontal.
                 * xsize = width (short/vertical), ysize = length (long/horizontal).
                 * Time samples span ysize (horizontal on screen), values span xsize (vertical).
                 * Fractional ctrl[].y avoids stepped horizontal bands that read as “square waves”. */

                for (i = 0; i < Spark->count; i++) {
                    double val = vbuf[i];
                    double scaled;

                    if (Spark->count > 1) {
                        ctrl[i].y = (double)i * (double)(Spark->ysize - 1) / (double)(Spark->count - 1);
                    } else {
                        ctrl[i].y = 0.0;
                    }

                    scaled = (val - dmin) / (dmax - dmin);
                    if (scaled < 0.0)
                        scaled = 0.0;
                    if (scaled > 1.0)
                        scaled = 1.0;
                    ctrl[i].x = (1.0 - scaled) * (Spark->xsize - 1);
                }

                if (do_fillunder) {
                    unsigned char ru, gu, bu;

                    if (spark_rgb_from_prop(&Spark->color, &ru, &gu, &bu) == 0) {
                        int      full_max = do_smooth ? (Spark->count - 1) * steps + 1 : Spark->count;
                        SparkPt *full = malloc((size_t)full_max * sizeof(SparkPt));

                        if (full != NULL) {
                            int fn = spark_build_full_polyline(ctrl, Spark->count, do_smooth, steps, segbuf, full,
                                                                full_max);

                            if (fn >= 2)
                                spark_fill_gradient_under(gdImage, full, fn, Spark->xsize, Spark->ysize, ru, gu, bu,
                                                          fillalpha);
                            free(full);
                        }
                    }
                }

                if (stroke == SPARK_STROKE_SOLID) {
                    for (seg = 0; seg < Spark->count - 1; seg++) {
                        double midval = (vbuf[seg] + vbuf[seg + 1]) / 2.0;
                        int    segcolor;

                        if (has_high && midval > dhigh) {
                            segcolor = colorhigh_gd;
                        } else if (has_low && midval < dlow) {
                            segcolor = colorlow_gd;
                        } else {
                            segcolor = colorline;
                        }

                        if (!do_smooth) {
                            segbuf[0] = ctrl[seg];
                            segbuf[1] = ctrl[seg + 1];
                            seg_n = 2;
                        } else {
                            SparkPt p0, p1, p2, p3;
                            int     j;

                            spark_catmull_controls(ctrl, Spark->count, seg, &p0, &p1, &p2, &p3);

                            for (j = 0; j <= steps; j++) {
                                double t = (double)j / (double)steps;

                                spark_catmull_rom(t, p0, p1, p2, p3, &segbuf[j]);
                            }
                            seg_n = steps + 1;
                        }

                        spark_draw_polyline_styled(gdImage, segbuf, seg_n, segcolor, thickness, stroke, 0, 0,
                                                   NULL, NULL);
                    }
                } else {
                    /* Dashed/dotted: phase must continue across all sample segments (each is ~1–3px). */
                    double dash_on, dash_gap;
                    double drem;
                    int    ddraw;

                    spark_dash_lengths(thickness, stroke, &dash_on, &dash_gap);
                    drem = dash_on;
                    ddraw = 1;
                    for (seg = 0; seg < Spark->count - 1; seg++) {
                        double midval = (vbuf[seg] + vbuf[seg + 1]) / 2.0;
                        int    segcolor;

                        if (has_high && midval > dhigh) {
                            segcolor = colorhigh_gd;
                        } else if (has_low && midval < dlow) {
                            segcolor = colorlow_gd;
                        } else {
                            segcolor = colorline;
                        }

                        if (!do_smooth) {
                            segbuf[0] = ctrl[seg];
                            segbuf[1] = ctrl[seg + 1];
                            seg_n = 2;
                        } else {
                            SparkPt p0, p1, p2, p3;
                            int     j;

                            spark_catmull_controls(ctrl, Spark->count, seg, &p0, &p1, &p2, &p3);

                            for (j = 0; j <= steps; j++) {
                                double t = (double)j / (double)steps;

                                spark_catmull_rom(t, p0, p1, p2, p3, &segbuf[j]);
                            }
                            seg_n = steps + 1;
                        }

                        spark_draw_polyline_styled(gdImage, segbuf, seg_n, segcolor, thickness, stroke,
                                                   0, 0, &drem, &ddraw);
                    }
                }

                free(vtmp);
                free(vbuf);
                free(ctrl);
            }
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
        property_eval(&Spark->thickness);
        property_eval(&Spark->smooth);
        property_eval(&Spark->smoothsteps);
        property_eval(&Spark->stroke);
        property_eval(&Spark->valueblur);
        property_eval(&Spark->fillunder);
        property_eval(&Spark->fillalpha);
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
        property_load(section, "samples",    NULL,       &Spark->samples);
        property_load(section, "thickness",  "1",        &Spark->thickness);
        property_load(section, "smooth",     "0",        &Spark->smooth);
        property_load(section, "smoothsteps", "24",      &Spark->smoothsteps);
        property_load(section, "stroke",     "solid",    &Spark->stroke);
        property_load(section, "valueblur",  "0",        &Spark->valueblur);
        property_load(section, "fillunder",  "0",        &Spark->fillunder);
        property_load(section, "fillalpha",  "44",       &Spark->fillalpha);

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

        /* Ring buffer size: optional `samples N` caps history; default is one per pixel along length */
        if (property_valid(&Spark->samples)) {
            property_eval(&Spark->samples);
            Spark->nsamples = (int)(P2N(&Spark->samples) + 0.5);
            if (Spark->nsamples < 1)
                Spark->nsamples = Spark->ysize;
        } else {
            Spark->nsamples = Spark->ysize;
        }
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
                property_free(&Spark->samples);
                property_free(&Spark->thickness);
                property_free(&Spark->smooth);
                property_free(&Spark->smoothsteps);
                property_free(&Spark->stroke);
                property_free(&Spark->valueblur);
                property_free(&Spark->fillunder);
                property_free(&Spark->fillalpha);

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
