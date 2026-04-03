/* $Id$
 * $URL$
 *
 * plugin for statfs() syscall
 *
 * Copyright (C) 2005 Michael Reinelt <michael@reinelt.co.at>
 * Copyright (C) 2005 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
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

/* 
 * exported functions:
 *
 * int plugin_init_statfs (void)
 *  adds statfs() functions
 *
 */


#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifdef HAVE_SYS_VFS_H
#include <sys/vfs.h>
#else
#include <sys/param.h>
#include <sys/mount.h>
#endif

#include "debug.h"
#include "plugin.h"



static void my_statfs(RESULT * result, RESULT * arg1, RESULT * arg2)
{
    struct statfs buf;
    char *path, *key;
    double value;

    path = R2S(arg1);
    key = R2S(arg2);

    if (statfs(path, &buf) != 0) {
        error("statfs(%s) failed: %s", path, strerror(errno));
        SetResult(&result, R_STRING, "");
        return;
    }

    if (strcasecmp(key, "type") == 0) {
        value = buf.f_type;
    } else if (strcasecmp(key, "bsize") == 0) {
        value = buf.f_bsize;
    } else if (strcasecmp(key, "blocks") == 0) {
        value = buf.f_blocks;
    } else if (strcasecmp(key, "bfree") == 0) {
        value = buf.f_bfree;
    } else if (strcasecmp(key, "bavail") == 0) {
        value = buf.f_bavail;
    } else if (strcasecmp(key, "files") == 0) {
        value = buf.f_files;
    } else if (strcasecmp(key, "ffree") == 0) {
        value = buf.f_ffree;
#if 0
    } else if (strcasecmp(key, "fsid") == 0) {
        value = buf.f_fsid;
#endif
    } else if (strcasecmp(key, "namelen") == 0) {
#ifdef __FreeBSD__
        value = buf.f_namemax;
#else
        value = buf.f_namelen;
#endif
    } else {
        error("statfs: unknown field '%s'", key);
        value = -1;
    }

    SetResult(&result, R_NUMBER, &value);
}


int plugin_init_statfs(void)
{
    AddFunction("statfs", 2, my_statfs);
    return 0;
}

void plugin_exit_statfs(void)
{
}
