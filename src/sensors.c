#include "sensors.h"
#include <hardware/gpio.h>
#include <stdint.h>
#include "hardware/adc.h"

uint16_t read_analog_pin(uint8_t pin) 
{
    /*
     * direction    = pin 26 = ADC0 
     * throttle     = pin 27 = ADC1
     * engine break = pin 28 = ADC2
     */
    uint8_t input = pin == APIN_DIRECTION       ? 0 :
                    pin == APIN_THROTTLE        ? 1 :
                    pin == APIN_ENGINE_BREAK    ? 2 :
                    255;
    adc_select_input(input);
    return adc_read();
}

void sensors_report(struct sensor_data *data)
{
    if (data == NULL)
        return;

    /* Switches */
    data->pause         = gpio_get(PIN_PAUSE);
    data->track_monitor = gpio_get(PIN_TRACK_MONITOR);
    data->next_station  = gpio_get(PIN_NEXT_STATION);
    data->headlights    = gpio_get(PIN_HEADLIGHTS);
    data->pantograph1   = gpio_get(PIN_PANTOGRAPH1);
    data->pantograph2   = gpio_get(PIN_PANTOGRAPH2);

    /* 3-state switch */
    data->view = gpio_get(PIN1_VIEW) ? 1 :
                 gpio_get(PIN2_VIEW) ? 2 :
                                       0 ;
    /* 4-state switch */
    // No, the pins arent in the wrong order
    // 1. poloha - všetky kontakty sú rozopnuté
    // 2. poloha - zopnuté kontakty 0-1-2
    // 3, poloha - zopnuté kontakty 0-1-4
    // 4. poloha - zopnuté kontakty 0-1-3
    data->train_break = gpio_get(PIN2_TRAIN_BRAKE) ? 1 :
                        gpio_get(PIN4_TRAIN_BRAKE) ? 2 :
                        gpio_get(PIN3_TRAIN_BRAKE) ? 3 :
                                                     0 ;
    /* Analogs */
    data->throttle      = read_analog_pin(APIN_THROTTLE);
    data->direction     = read_analog_pin(APIN_DIRECTION);
    data->engine_break  = read_analog_pin(APIN_ENGINE_BREAK);
}
