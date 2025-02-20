#ifndef LED_RGB_H
#define LED_RGB_H

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

void led_rgb_init(uint led_red_pin, uint led_green_pin, uint led_blue_pin);

#endif