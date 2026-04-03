/*
 * gauge widget handling
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


#ifndef _WIDGET_GAUGE_H_
#define _WIDGET_GAUGE_H_

#include "property.h"
#include "widget.h"
#include "rgb.h"

typedef struct WIDGET_GAUGE {
    /* gdImage to center must be in this order to match WIDGET_IMAGE */
    void *gdImage;              /* raw gd image */
    RGBA *bitmap;               /* image bitmap */
    int ysize, xsize;           /* widget size */
    int oldheight;              /* height of the image before */
    PROPERTY expr_min;          /* explicit minimum value */
    PROPERTY expr_max;          /* explicit maximum value */
    PROPERTY prop_width;        /* widget width in pixels */
    PROPERTY prop_height;       /* widget height in pixels */
    PROPERTY update;            /* update interval (msec) */
    PROPERTY reload;            /* reload on update? */
    PROPERTY visible;           /* widget visible? */
    PROPERTY inverted;          /* widget inverted? */
    PROPERTY center;            /* widget centered? (padding, not used) */
    /* From here onward can be custom to this widget */
    PROPERTY value;             /* expression to display */
    PROPERTY color;             /* arc color (default / mid range) */
    PROPERTY colorlow;          /* arc color below valuelow threshold */
    PROPERTY valuelow;          /* threshold below which to use colorlow */
    PROPERTY colorhigh;         /* arc color above valuehigh threshold */
    PROPERTY valuehigh;         /* threshold above which to use colorhigh */
    PROPERTY background;        /* background arc color */
    PROPERTY start;             /* start angle in degrees (0 = 3 o'clock) */
    PROPERTY sweep;             /* sweep angle in degrees */
    PROPERTY thickness;         /* ring thickness in pixels (0 = filled pie) */
    PROPERTY reverse;           /* reverse fill: 1 = show remaining, 0 = show value */
    PROPERTY direction;         /* fill direction: 'CW' (clockwise) or 'CCW' */
} WIDGET_GAUGE;


extern WIDGET_CLASS Widget_Gauge;

#endif
