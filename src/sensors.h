#ifndef __SENSORS__
#define __SENSORS__

#include "tusb.h"
#include <stdint.h>

/* Structure to help with reading the sensor data */
struct __packed sensor_data {
    /* switches */
    uint8_t track_monitor;
    uint8_t next_station;
    uint8_t pantograph1;
    uint8_t pantograph2;
    uint8_t headlights;
    uint8_t pause;

    uint8_t view; // 3-state switch
    uint8_t train_break; // 4-state switch

    /* Analogs */
    uint16_t direction;
    uint16_t throttle;
    uint16_t engine_break;
};

/* Function that collects data from sensors and fills up data with them */
void sensors_report(struct sensor_data *data);

#endif

