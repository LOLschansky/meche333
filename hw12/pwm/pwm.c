/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

//#define LED_PIN 15
//#define BUTTON_PIN 18
#define PWM 0 // the built in LED on the Pico//

float min_duty = 0.022;
float max_duty = 0.13;

void set_angle(int theta);
void angle_init();

void main(){
  angle_init();
  int theta = 1;
  int speed = 1;
  while (1) {
    if (theta == 0 || theta == 180){
      speed = speed * -1;
    }
    theta = theta + speed;
    set_angle(theta);
    sleep_ms(11);
  }
}


void angle_init(){
  gpio_set_function(PWM, GPIO_FUNC_PWM); // Set the LED Pin to be PWM
  uint slice_num = pwm_gpio_to_slice_num(PWM); // Get PWM slice number
  float div = 128; // must be between 1-255
  pwm_set_clkdiv(slice_num, div); // divider
  uint16_t wrap = 19531; // when to rollover, must be less than 65535
  pwm_set_wrap(slice_num, wrap);
  pwm_set_enabled(slice_num, true); // turn on the PWM
  pwm_set_gpio_level(PWM, wrap / 8); // set the servo to 0 degrees... set the duty cycle to roughly 2%
 // pwm_set_gpio_level(PWM, wrap / 8); // // set the servo to 180 degrees ... set the duty cycle to roughly 12.5%
}

void set_angle(int theta){
  uint16_t wrap = 19531; // when to rollover, must be less than 65535
  float duty = ((max_duty- min_duty) / 180) * theta + min_duty;
  pwm_set_gpio_level(PWM, wrap * duty);
}