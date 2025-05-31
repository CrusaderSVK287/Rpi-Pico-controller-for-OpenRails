#include "pico/stdlib.h"
#include "blink.h"

int main() {
    init_led();

    while (1 == 1) {
        blink_led();
    }
}

