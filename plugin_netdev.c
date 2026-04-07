/* $Id$
 * $URL$
 *
 * plugin for /proc/net/dev parsing
 *
 * Copyright (C) 2003 Michael Reinelt <michael@reinelt.co.at>
 * Copyright (C) 2004 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
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
 * int plugin_init_netdev (void)
 *  adds functions to access /proc/net/dev
 *
 */


#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#ifdef __FreeBSD__
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <ifaddrs.h>
#endif

#include "debug.h"
#include "plugin.h"
#include "qprintf.h"
#include "hash.h"


static HASH NetDev;
static char *DELIMITER = " :|\t\n";

#ifndef __FreeBSD__
static FILE *Stream = NULL;
#endif


#ifdef __FreeBSD__

/* FreeBSD: populate NetDev hash from getifaddrs() AF_LINK entries.
 *
 * The stored string matches the /proc/net/dev column layout so that
 * hash_get_delta() works identically on both platforms:
 *
 *   col  0  Rx_face        (interface name)
 *   col  1  Rx_bytes       ifi_ibytes
 *   col  2  Rx_packets     ifi_ipackets
 *   col  3  Rx_errs        ifi_ierrors
 *   col  4  Rx_drop        ifi_iqdrops
 *   col  5  Rx_fifo        (unsupported, 0)
 *   col  6  Rx_frame       (unsupported, 0)
 *   col  7  Rx_compressed  (unsupported, 0)
 *   col  8  Rx_multicast   ifi_imcasts
 *   col  9  Tx_bytes       ifi_obytes
 *   col 10  Tx_packets     ifi_opackets
 *   col 11  Tx_errs        ifi_oerrors
 *   col 12  Tx_drop        ifi_oqdrops
 *   col 13  Tx_fifo        (unsupported, 0)
 *   col 14  Tx_colls       ifi_collisions
 *   col 15  Tx_carrier     (unsupported, 0)
 *   col 16  Tx_compressed  (unsupported, 0)
 */
static int parse_netdev(void)
{
    int age;
    struct ifaddrs *ifap, *ifa;

    /* reread every 10 msec only */
    age = hash_age(&NetDev, NULL);
    if (age > 0 && age <= 10)
        return 0;

    if (getifaddrs(&ifap) < 0) {
        error("getifaddrs() failed: %s", strerror(errno));
        return -1;
    }

    for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {
        struct if_data *stats;
        char buffer[256];

        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_LINK)
            continue;
        if (ifa->ifa_data == NULL)
            continue;

        stats = (struct if_data *)ifa->ifa_data;

        snprintf(buffer, sizeof(buffer),
                 "%s %llu %llu %llu %llu 0 0 0 %llu %llu %llu %llu %llu 0 %llu 0 0",
                 ifa->ifa_name,
                 (unsigned long long)stats->ifi_ibytes,
                 (unsigned long long)stats->ifi_ipackets,
                 (unsigned long long)stats->ifi_ierrors,
                 (unsigned long long)stats->ifi_iqdrops,
                 (unsigned long long)stats->ifi_imcasts,
                 (unsigned long long)stats->ifi_obytes,
                 (unsigned long long)stats->ifi_opackets,
                 (unsigned long long)stats->ifi_oerrors,
                 (unsigned long long)stats->ifi_oqdrops,
                 (unsigned long long)stats->ifi_collisions);

        hash_put_delta(&NetDev, ifa->ifa_name, buffer);
    }

    freeifaddrs(ifap);
    return 0;
}

#else

static int parse_netdev(void)
{
    int age;
    int row, col;
    static int first_time = 1;

    /* reread every 10 msec only */
    age = hash_age(&NetDev, NULL);
    if (age > 0 && age <= 10)
        return 0;

    if (Stream == NULL)
        Stream = fopen("/proc/net/dev", "r");
    if (Stream == NULL) {
        error("fopen(/proc/net/dev) failed: %s", strerror(errno));
        return -1;
    }

    rewind(Stream);
    row = 0;

    while (!feof(Stream)) {
        char buffer[256];
        char dev[16];
        char *beg, *end;
        unsigned int len;

        if (fgets(buffer, sizeof(buffer), Stream) == NULL)
            break;

        switch (++row) {

        case 1:
            /* skip row 1 */
            continue;

        case 2:
            /* row 2 used for headers */
            if (first_time) {
                char *RxTx = strrchr(buffer, '|');
                first_time = 0;
                col = 0;
                beg = buffer;
                while (beg) {
                    char key[32];

                    while (strchr(DELIMITER, *beg))
                        beg++;
                    if ((end = strpbrk(beg, DELIMITER)) != NULL)
                        *end = '\0';
                    qprintf(key, sizeof(key), "%s_%s", beg < RxTx ? "Rx" : "Tx", beg);
                    hash_set_column(&NetDev, col++, key);
                    beg = end ? end + 1 : NULL;
                }
            }
            continue;

        default:
            /* fetch interface name (1st column) as key */
            beg = buffer;
            while (*beg && *beg == ' ')
                beg++;
            end = beg + 1;
            while (*end && *end != ':')
                end++;
            len = end - beg;
            if (len >= sizeof(dev))
                len = sizeof(dev) - 1;
            strncpy(dev, beg, len);
            dev[len] = '\0';

            hash_put_delta(&NetDev, dev, buffer);
        }
    }

    return 0;
}

#endif

static void my_netdev(RESULT * result, RESULT * arg1, RESULT * arg2, RESULT * arg3)
{
    char *dev, *key;
    int delay;
    double value;

    if (parse_netdev() < 0) {
        SetResult(&result, R_STRING, "");
        return;
    }

    dev = R2S(arg1);
    key = R2S(arg2);
    delay = R2N(arg3);

    value = hash_get_regex(&NetDev, dev, key, delay);

    SetResult(&result, R_NUMBER, &value);
}

static void my_netdev_fast(RESULT * result, RESULT * arg1, RESULT * arg2, RESULT * arg3)
{
    char *dev, *key;
    int delay;
    double value;

    if (parse_netdev() < 0) {
        SetResult(&result, R_STRING, "");
        return;
    }

    dev = R2S(arg1);
    key = R2S(arg2);
    delay = R2N(arg3);

    value = hash_get_delta(&NetDev, dev, key, delay);

    SetResult(&result, R_NUMBER, &value);
}


int plugin_init_netdev(void)
{
    hash_create(&NetDev);
    hash_set_delimiter(&NetDev, " :|\t\n");

#ifdef __FreeBSD__
    /* On FreeBSD, column headers are set up statically here instead of
     * being parsed from /proc/net/dev at runtime.  The order matches the
     * Linux /proc/net/dev layout so that the same netdev() calls work on
     * both platforms without any change to themes or config files. */
    hash_set_column(&NetDev,  0, "Rx_face");
    hash_set_column(&NetDev,  1, "Rx_bytes");
    hash_set_column(&NetDev,  2, "Rx_packets");
    hash_set_column(&NetDev,  3, "Rx_errs");
    hash_set_column(&NetDev,  4, "Rx_drop");
    hash_set_column(&NetDev,  5, "Rx_fifo");
    hash_set_column(&NetDev,  6, "Rx_frame");
    hash_set_column(&NetDev,  7, "Rx_compressed");
    hash_set_column(&NetDev,  8, "Rx_multicast");
    hash_set_column(&NetDev,  9, "Tx_bytes");
    hash_set_column(&NetDev, 10, "Tx_packets");
    hash_set_column(&NetDev, 11, "Tx_errs");
    hash_set_column(&NetDev, 12, "Tx_drop");
    hash_set_column(&NetDev, 13, "Tx_fifo");
    hash_set_column(&NetDev, 14, "Tx_colls");
    hash_set_column(&NetDev, 15, "Tx_carrier");
    hash_set_column(&NetDev, 16, "Tx_compressed");
#endif

    AddFunction("netdev", 3, my_netdev);
    AddFunction("netdev::fast", 3, my_netdev_fast);
    return 0;
}

void plugin_exit_netdev(void)
{
#ifndef __FreeBSD__
    if (Stream != NULL) {
        fclose(Stream);
        Stream = NULL;
    }
#endif
    hash_destroy(&NetDev);
}
