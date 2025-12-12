#ifndef __SENSORS__
#define __SENSORS__

#include "tusb.h"
#include <stdint.h>

#define APIN_DIRECTION       26
#define APIN_THROTTLE        27
#define APIN_ENGINE_BREAK    28

#define PIN_PANTOGRAPH1     0
#define PIN_PANTOGRAPH2     1
#define PIN_HEADLIGHTS      2
#define PIN_TRACK_MONITOR   3
#define PIN_NEXT_STATION    4
#define PIN_PAUSE           8

#define PIN1_VIEW           6
#define PIN2_VIEW           7

// there is PIN1_TRAIN_BRAKE on the physical switch but its not used
#define PIN2_TRAIN_BRAKE    13
#define PIN3_TRAIN_BRAKE    14
#define PIN4_TRAIN_BRAKE    15

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

