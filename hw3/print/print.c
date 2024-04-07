/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define LED_PIN 15
#define BUTTON_PIN 18

int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    //gpio setup
    gpio_init(LED_PIN);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    //adc setup
    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    while(true){
    printf("Press the button to start!\n");

    //Turn on LED, turn off when button pushed
    while (gpio_get(BUTTON_PIN)) {
        gpio_put(LED_PIN,1);
        // sleep_ms(250);
    }
    gpio_put(LED_PIN,0);

    //Prompt user to determine # of analog samples to take

    int analog_samples;
    printf("Input desired number of analog samples: \n");
    scanf("%d", &analog_samples);
    printf("Number of analog samples: %d\r\n",analog_samples);

    //Read voltage on ADC0 analog_samples number of times @ 100Hz
    for (int i = 1; i<=analog_samples; i++){
        uint16_t result = adc_read();
        const float conversion_factor = 3.3f / (1 << 12);
        printf("%f V\n", result, result * conversion_factor);
        sleep_ms(1);
    }
    }
}
