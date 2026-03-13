#include "bsp/board.h"
#include "hardware/adc.h"
#include "tusb.h"

#include "sensors.h"
#include <hardware/gpio.h>
#include <stdint.h>

void send_input_report(const struct sensor_data *data)
{
    if (!tud_hid_ready()) return;
    if (!data) return;

    uint8_t report[8];

    // Analog axes (X, Y, Z)
    report[0] = (uint8_t)(data->throttle & 0xFF);        // low byte
    report[1] = (uint8_t)((data->throttle >> 8) & 0xFF); // high byte
    /* direction */
    report[2] = (uint8_t)(data->direction & 0xFF);       
    report[3] = (uint8_t)((data->direction >> 8) & 0xFF);
    /* brake (engine_break) */
    report[4] = (uint8_t)(data->engine_break & 0xFF);
    report[5] = (uint8_t)((data->engine_break >> 8) & 0xFF);

    // Digital byte 1: button + 5 two-state switches
    report[6] = (data->pause << 0) 
              | (data->track_monitor << 1)
              | (data->next_station << 2)
              | (data->headlights << 3)
              | (data->pantograph1 << 4)
              | (data->pantograph2 << 5);
    // Bits 6-7 are padding, left as 0

    // Digital byte 2: 3-state and 4-state switches
    // 3-state switch (bits 0-1), 4-state switch (bits 2-3)
    report[7] = (data->view & 0x03)                 // 3-state switch
              | ((data->train_break & 0x03) << 2);  // 4-state switch
    // Bits 4-7 are padding, left as 0

    tud_hid_report(0, report, sizeof(report));
}

// Required TinyUSB callback – called on setup
void tud_mount_cb(void) {
    // Device mounted
    if (tud_hid_ready()) {
        struct sensor_data data;
        sensors_report(&data);
        send_input_report(&data);       
    }
}

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
    struct sensor_data data;
    sensors_report(&data);
    send_input_report(&data);       
}

int main(void)
{
    board_init();
    tusb_init();

    // analog pin setup
    adc_gpio_init(APIN_THROTTLE);
    adc_gpio_init(APIN_DIRECTION);
    adc_gpio_init(APIN_ENGINE_BREAK);
    
    // digital pins setup
    // there is no PIN1_TRAIN_BRAKE
    const uint8_t pins[] = { PIN_PANTOGRAPH1, PIN_PANTOGRAPH2, PIN_HEADLIGHTS, PIN_TRACK_MONITOR, 
        PIN_NEXT_STATION,  PIN_PAUSE, PIN1_VIEW, PIN2_VIEW, PIN2_TRAIN_BRAKE,  
        PIN3_TRAIN_BRAKE,  PIN4_TRAIN_BRAKE   
    };
    const size_t pinCount = sizeof(pins) / sizeof(pins[0]);

    for (size_t i = 0; i < pinCount; i++) {
        uint8_t pin = pins[i];
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_down(pin);
    }

    // initialisite the analog to digital converter
    adc_init();

    while (1)
    {
        tud_task();
    }
}

