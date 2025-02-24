#ifndef LED_RGB_H
#define LED_RGB_H

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define LED_GREEN_PIN 11
#define LED_BLUE_PIN 12
#define LED_RED_PIN 13

void led_init(uint led_pin);
void led_init_all();

#endif