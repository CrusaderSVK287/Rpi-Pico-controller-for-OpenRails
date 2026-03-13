#include "pico/stdlib.h"
#include "pico/time.h"
#include "bsp/board.h"
#include "hardware/adc.h"
#include "tusb.h"
#include "class/hid/hid.h"
#include <string.h>

/* 
 * ################################################
 * # This file contains unused callback that must #
 * # be defined but, at least currently, don't    #
 * # serve any purpouse, therefore they would     #
 * # just cause boilerplate and visual clutter    #
 * # among the acutually needed and usefull code  #
 * ################################################
 */

void tud_unmount_cb(void) {
    // Device unmounted
}

void tud_suspend_cb(bool remote_wakeup_en) {
    // USB suspended
}

void tud_resume_cb(void) {
    // USB resumed
}

