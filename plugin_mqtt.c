/* $Id$
 * $URL$
 *
 * MQTT subscriber plugin
 *
 * Copyright (C) 2026 Alejandro Mora <amd989@users.noreply.github.com>
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
 * int plugin_init_mqtt (void)
 *  adds MQTT subscriber functions
 *
 */


#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>

#include <mosquitto.h>

#include "debug.h"
#include "plugin.h"
#include "hash.h"
#include "cfg.h"
#include "thread.h"
#include "qprintf.h"


#define MQTT_MAX_TOPICS      64
#define MQTT_MAX_TOPIC_LEN   256
#define MQTT_MAX_PAYLOAD_LEN 768

static char Section[] = "Plugin:MQTT";

typedef struct {
    char topic[MQTT_MAX_TOPIC_LEN];
    char payload[MQTT_MAX_PAYLOAD_LEN];
    long tv_sec;
    long tv_usec;
    int active;
} MQTT_SLOT;

typedef struct {
    int count;
    int connected;
    MQTT_SLOT slots[MQTT_MAX_TOPICS];
} MQTT_SHM;

/* IPC resources */
static int mqtt_mutex = 0;
static int mqtt_shmid = -1;
static pid_t mqtt_pid = -1;
static MQTT_SHM *mqtt_shm = NULL;
static HASH MQTT;

/* Configuration */
static char mqtt_broker[256];
static int mqtt_port;
static char mqtt_user[128];
static char mqtt_pass[128];
static char mqtt_clientid[128];
static char mqtt_cafile[512];
static char mqtt_certfile[512];
static char mqtt_keyfile[512];
static char *mqtt_topics[MQTT_MAX_TOPICS];
static int mqtt_topic_count = 0;


static int configure_mqtt(void)
{
    char *s;
    int i;
    char key[16];

    s = cfg_get(Section, "broker", "localhost");
    strncpy(mqtt_broker, s, sizeof(mqtt_broker) - 1);
    mqtt_broker[sizeof(mqtt_broker) - 1] = '\0';
    free(s);

    if (cfg_number(Section, "port", 1883, 1, 65535, &mqtt_port) < 0) {
        info("[MQTT] no '%s.port' entry from %s, using default 1883", Section, cfg_source());
        mqtt_port = 1883;
    }

    s = cfg_get(Section, "user", "");
    strncpy(mqtt_user, s, sizeof(mqtt_user) - 1);
    mqtt_user[sizeof(mqtt_user) - 1] = '\0';
    free(s);

    s = cfg_get(Section, "password", "");
    strncpy(mqtt_pass, s, sizeof(mqtt_pass) - 1);
    mqtt_pass[sizeof(mqtt_pass) - 1] = '\0';
    free(s);

    s = cfg_get(Section, "clientid", "");
    strncpy(mqtt_clientid, s, sizeof(mqtt_clientid) - 1);
    mqtt_clientid[sizeof(mqtt_clientid) - 1] = '\0';
    free(s);

    s = cfg_get(Section, "cafile", "");
    strncpy(mqtt_cafile, s, sizeof(mqtt_cafile) - 1);
    mqtt_cafile[sizeof(mqtt_cafile) - 1] = '\0';
    free(s);

    s = cfg_get(Section, "certfile", "");
    strncpy(mqtt_certfile, s, sizeof(mqtt_certfile) - 1);
    mqtt_certfile[sizeof(mqtt_certfile) - 1] = '\0';
    free(s);

    s = cfg_get(Section, "keyfile", "");
    strncpy(mqtt_keyfile, s, sizeof(mqtt_keyfile) - 1);
    mqtt_keyfile[sizeof(mqtt_keyfile) - 1] = '\0';
    free(s);

    /* Read topics: topic1, topic2, ... topicN */
    mqtt_topic_count = 0;
    for (i = 0; i < MQTT_MAX_TOPICS; i++) {
        qprintf(key, sizeof(key), "topic%d", i + 1);
        s = cfg_get(Section, key, NULL);
        if (s == NULL || *s == '\0') {
            if (s)
                free(s);
            break;
        }
        mqtt_topics[i] = s;
        mqtt_topic_count++;
    }

    info("[MQTT] broker=%s:%d topics=%d", mqtt_broker, mqtt_port, mqtt_topic_count);
    return 0;
}


/* Mosquitto callbacks — run in child process */

static void on_connect(struct mosquitto *mosq, void *userdata, int rc)
{
    MQTT_SHM *shm = (MQTT_SHM *) userdata;
    int i;

    if (rc != 0) {
        error("[MQTT] connection refused: %s", mosquitto_connack_string(rc));
        return;
    }

    info("[MQTT] connected to %s:%d", mqtt_broker, mqtt_port);

    /* Subscribe to all configured topics */
    for (i = 0; i < mqtt_topic_count; i++) {
        if (mosquitto_subscribe(mosq, NULL, mqtt_topics[i], 0) != MOSQ_ERR_SUCCESS) {
            error("[MQTT] failed to subscribe to '%s'", mqtt_topics[i]);
        } else {
            info("[MQTT] subscribed to '%s'", mqtt_topics[i]);
        }
    }

    mutex_lock(mqtt_mutex);
    shm->connected = 1;
    mutex_unlock(mqtt_mutex);
}


static void on_disconnect(struct mosquitto *mosq __attribute__((unused)), void *userdata, int rc)
{
    MQTT_SHM *shm = (MQTT_SHM *) userdata;

    if (rc != 0) {
        info("[MQTT] unexpected disconnect (rc=%d), reconnecting...", rc);
    } else {
        info("[MQTT] disconnected");
    }

    mutex_lock(mqtt_mutex);
    shm->connected = 0;
    mutex_unlock(mqtt_mutex);
}


static void on_message(struct mosquitto *mosq __attribute__((unused)), void *userdata,
                        const struct mosquitto_message *msg)
{
    MQTT_SHM *shm = (MQTT_SHM *) userdata;
    struct timeval now;
    int i, slot;
    int payload_len;

    if (msg->topic == NULL)
        return;

    gettimeofday(&now, NULL);

    mutex_lock(mqtt_mutex);

    /* Find existing slot for this topic */
    slot = -1;
    for (i = 0; i < shm->count; i++) {
        if (shm->slots[i].active && strcmp(shm->slots[i].topic, msg->topic) == 0) {
            slot = i;
            break;
        }
    }

    /* Allocate new slot if not found */
    if (slot < 0) {
        if (shm->count >= MQTT_MAX_TOPICS) {
            mutex_unlock(mqtt_mutex);
            error("[MQTT] slot table full, dropping message on '%s'", msg->topic);
            return;
        }
        slot = shm->count;
        shm->count++;
        shm->slots[slot].active = 1;
        strncpy(shm->slots[slot].topic, msg->topic, MQTT_MAX_TOPIC_LEN - 1);
        shm->slots[slot].topic[MQTT_MAX_TOPIC_LEN - 1] = '\0';
    }

    /* Copy payload */
    payload_len = msg->payloadlen;
    if (payload_len > MQTT_MAX_PAYLOAD_LEN - 1)
        payload_len = MQTT_MAX_PAYLOAD_LEN - 1;

    if (msg->payload != NULL && payload_len > 0) {
        memcpy(shm->slots[slot].payload, msg->payload, payload_len);
    }
    shm->slots[slot].payload[payload_len] = '\0';

    /* Record timestamp */
    shm->slots[slot].tv_sec = now.tv_sec;
    shm->slots[slot].tv_usec = now.tv_usec;

    mutex_unlock(mqtt_mutex);
}


/* Background thread — runs mosquitto event loop in child process */

static void mqtt_thread(void *data)
{
    MQTT_SHM *shm = (MQTT_SHM *) data;
    struct mosquitto *mosq;

    mosquitto_lib_init();

    mosq = mosquitto_new(mqtt_clientid[0] ? mqtt_clientid : NULL, true, shm);
    if (mosq == NULL) {
        error("[MQTT] mosquitto_new() failed: %s", strerror(errno));
        return;
    }

    /* Set credentials if configured */
    if (mqtt_user[0]) {
        mosquitto_username_pw_set(mosq, mqtt_user, mqtt_pass[0] ? mqtt_pass : NULL);
    }

    /* Set TLS if CA file is configured */
    if (mqtt_cafile[0]) {
        int tls_rc = mosquitto_tls_set(mosq,
                                        mqtt_cafile,
                                        NULL,
                                        mqtt_certfile[0] ? mqtt_certfile : NULL,
                                        mqtt_keyfile[0] ? mqtt_keyfile : NULL,
                                        NULL);
        if (tls_rc != MOSQ_ERR_SUCCESS) {
            error("[MQTT] TLS configuration failed: %s", mosquitto_strerror(tls_rc));
        }
    }

    /* Set callbacks */
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_disconnect_callback_set(mosq, on_disconnect);
    mosquitto_message_callback_set(mosq, on_message);

    /* Automatic reconnection: 1s initial, 5s max, no exponential backoff */
    mosquitto_reconnect_delay_set(mosq, 1, 5, false);

    /* Connect to broker */
    if (mosquitto_connect(mosq, mqtt_broker, mqtt_port, 60) != MOSQ_ERR_SUCCESS) {
        error("[MQTT] initial connection to %s:%d failed: %s", mqtt_broker, mqtt_port, strerror(errno));
        /* mosquitto_loop_forever will retry */
    }

    /* Block forever — handles reconnection automatically */
    mosquitto_loop_forever(mosq, -1, 1);

    /* Only reached on fatal error */
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}


/* Transfer a topic's value from SHM into the local HASH table */

static int mqtt_sync(const char *topic)
{
    int i, age;

    age = hash_age(&MQTT, topic);

    /* Rate-limit: skip SHM read if less than 10ms since last sync */
    if (age > 0 && age <= 10)
        return 0;

    mutex_lock(mqtt_mutex);
    for (i = 0; i < mqtt_shm->count; i++) {
        if (mqtt_shm->slots[i].active && strcmp(mqtt_shm->slots[i].topic, topic) == 0) {
            hash_put(&MQTT, topic, mqtt_shm->slots[i].payload);
            mutex_unlock(mqtt_mutex);
            return 0;
        }
    }
    mutex_unlock(mqtt_mutex);

    /* Topic not found in SHM — may not have received a message yet */
    if (age < 0) {
        hash_put(&MQTT, topic, "");
    }

    return 0;
}


/* Plugin functions — called by the expression evaluator in the parent process */

static void my_mqtt_topic(RESULT * result, RESULT * arg1)
{
    char *topic = R2S(arg1);
    char *val;

    mqtt_sync(topic);

    val = hash_get(&MQTT, topic, NULL);
    if (val == NULL)
        val = "";

    SetResult(&result, R_STRING, val);
}


static void my_mqtt_age(RESULT * result, RESULT * arg1)
{
    char *topic = R2S(arg1);
    double age;

    mqtt_sync(topic);

    age = (double) hash_age(&MQTT, topic);

    SetResult(&result, R_NUMBER, &age);
}


static void my_mqtt_connected(RESULT * result)
{
    double val;

    mutex_lock(mqtt_mutex);
    val = (double) mqtt_shm->connected;
    mutex_unlock(mqtt_mutex);

    SetResult(&result, R_NUMBER, &val);
}


int plugin_init_mqtt(void)
{
    configure_mqtt();

    if (mqtt_topic_count == 0) {
        info("[MQTT] no topics configured, plugin inactive");
        return 0;
    }

    hash_create(&MQTT);

    /* Create IPC resources */
    mqtt_mutex = mutex_create();

    mqtt_shmid = shm_create((void **) &mqtt_shm, sizeof(MQTT_SHM));
    if (mqtt_shmid < 0) {
        error("[MQTT] shared memory allocation failed");
        mutex_destroy(mqtt_mutex);
        return -1;
    }
    memset(mqtt_shm, 0, sizeof(MQTT_SHM));

    /* Initialize mosquitto library before fork */
    mosquitto_lib_init();

    /* Create background thread (fork) for MQTT event loop */
    mqtt_pid = thread_create("mqtt-sub", mqtt_thread, mqtt_shm);
    if (mqtt_pid < 0) {
        error("[MQTT] failed to create subscriber thread");
        shm_destroy(mqtt_shmid, mqtt_shm);
        mutex_destroy(mqtt_mutex);
        return -1;
    }

    AddFunction("mqtt::topic", 1, my_mqtt_topic);
    AddFunction("mqtt::age", 1, my_mqtt_age);
    AddFunction("mqtt::connected", 0, my_mqtt_connected);

    return 0;
}


void plugin_exit_mqtt(void)
{
    int i;

    if (mqtt_pid > 0) {
        thread_destroy(mqtt_pid);
        mqtt_pid = -1;
    }

    if (mqtt_mutex != 0) {
        mutex_destroy(mqtt_mutex);
        mqtt_mutex = 0;
    }

    if (mqtt_shm != NULL) {
        shm_destroy(mqtt_shmid, mqtt_shm);
        mqtt_shm = NULL;
        mqtt_shmid = -1;
    }

    hash_destroy(&MQTT);

    for (i = 0; i < mqtt_topic_count; i++) {
        if (mqtt_topics[i]) {
            free(mqtt_topics[i]);
            mqtt_topics[i] = NULL;
        }
    }
    mqtt_topic_count = 0;

    mosquitto_lib_cleanup();
}
