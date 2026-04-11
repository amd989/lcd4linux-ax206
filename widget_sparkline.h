/*
 * sparkline widget handling
 *
 * Copyright (C) 2025-2026 Alejandro Mora <amd989@users.noreply.github.com>
 *
 * This file is part of LCD4Linux.
 *
 * LCD4Linux is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * LCD4Linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */


#ifndef _WIDGET_SPARKLINE_H_
#define _WIDGET_SPARKLINE_H_

#include "property.h"
#include "widget.h"
#include "rgb.h"

typedef struct WIDGET_SPARKLINE {
    /* gdImage to center must be in this order to match WIDGET_IMAGE */
    void *gdImage;              /* raw gd image */
    RGBA *bitmap;               /* image bitmap */
    int ysize, xsize;           /* widget size (ysize=length, xsize=width) */
    int oldheight;              /* height of the image before */
    PROPERTY expr_min;          /* explicit minimum value */
    PROPERTY expr_max;          /* explicit maximum value */
    PROPERTY width;             /* widget width in pixels */
    PROPERTY length;            /* widget length in pixels */
    PROPERTY update;            /* update interval (msec) */
    PROPERTY reload;            /* reload on update? */
    PROPERTY visible;           /* widget visible? */
    PROPERTY inverted;          /* widget inverted? */
    PROPERTY center;            /* widget centered? (padding, not used) */
    /* From here onward can be custom to this widget */
    PROPERTY value;             /* expression to sample */
    PROPERTY color;             /* line color (default / mid range) */
    PROPERTY colorlow;          /* line color below valuelow threshold */
    PROPERTY valuelow;          /* threshold below which to use colorlow */
    PROPERTY colorhigh;         /* line color above valuehigh threshold */
    PROPERTY valuehigh;         /* threshold above which to use colorhigh */
    PROPERTY background;        /* background color */
    PROPERTY samples;           /* optional max ring-buffer size (default: length) */
    PROPERTY thickness;         /* line width in pixels (default 1) */
    PROPERTY smooth;            /* 1 = Catmull-Rom smooth curve, 0 = straight segments */
    PROPERTY smoothsteps;       /* subdivisions per segment when smooth (default 12) */
    PROPERTY stroke;            /* solid | dashed | dotted */
    PROPERTY valueblur;         /* binomial smooth passes on values (0=off); softens "square" I/O */
    PROPERTY fillunder;         /* 1 = gradient fill from line color toward transparent toward min value */
    PROPERTY fillalpha;         /* GD alpha at curve for fillunder (0..127, lower=stronger, default 44) */
    double *history;            /* ring buffer of sampled values */
    int nsamples;               /* size of ring buffer (<= length unless samples set larger) */
    int head;                   /* next write index */
    int count;                  /* number of valid samples */
    double min;                 /* current min for auto-scaling */
    double max;                 /* current max for auto-scaling */
} WIDGET_SPARKLINE;


extern WIDGET_CLASS Widget_Sparkline;

#endif
