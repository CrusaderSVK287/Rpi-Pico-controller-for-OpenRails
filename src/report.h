#ifndef __REPORT__
#define __REPORT__

/*
 * #############################################
 * # This file contains code for HID reports   #
 * # and device descriptor, mostly boilerplate #
 * # code that simply has to be defined        #
 * #############################################
 */

#include "pico/stdlib.h"
#include "pico/time.h"
#include "bsp/board.h"
#include "hardware/adc.h"
#include "tusb.h"
#include "class/hid/hid.h"
#include <string.h>

#define DESC_MANUFACTURER   "Lukas Devices"
#define DESC_PRODUCT        "Pico HID Controller"
#define DESC_SERIALNUMBER   "123456"
#endif

