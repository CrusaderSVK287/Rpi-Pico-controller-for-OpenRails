#include "pico/stdlib.h"
#include <pico/stdio.h>
#include "blink.h"
#include <stdio.h>
#include <stdexcept>
#include <iostream>

#define LED_PIN 25
#define TIMEOUT 1000

void init_led() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    stdio_init_all();
}

void blink_led() {
    static int num = 0;

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

    num++;
}

