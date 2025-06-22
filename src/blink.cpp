#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <pico/stdio.h>
#include "blink.h"
#include <pico/time.h>
#include <stdio.h>
//#include <stdexcept>
//#include <iostream>

#define LED_PIN 25
#define TIMEOUT 1000

void init_led() {
    gpio_init(LED_PIN);
    //gpio_set_dir(LED_PIN, GPIO_OUT);


    // init serial output
    stdio_init_all();
    // init adc hardware
    adc_init();

    // Select ADC input 0 (GPIO26)
    adc_gpio_init(26);
    adc_select_input(0);
    gpio_put(LED_PIN, 1);

    sleep_ms(3000);
    printf("Setup complete\n");
}

void blink_led() {
    /*static int num = 0;

    gpio_put(LED_PIN, 1);
    printf("LED on\n");
    sleep_ms(TIMEOUT);

    try {
        throw std::runtime_error("An error occurred during riskyOperation");
    } catch (const std::runtime_error& e) {
        printf(e.what());
    }

    std::cout << "lol" << std::endl;

    gpio_put(LED_PIN, 0);
    printf("LED off\n");
    sleep_ms(TIMEOUT);

    num++;*/

    uint16_t result = adc_read(); // 12-bit result: 0–4095
    printf("ADC Value: %u\tVoltage: %fV\n", result, result * 3.3f / 4095.0f);
    sleep_ms(500);
}

