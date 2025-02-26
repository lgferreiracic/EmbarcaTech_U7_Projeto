#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "stdio.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define JOYSTICK_X_PIN 26
#define JOYSTICK_Y_PIN 27

void joystick_init();
uint joystick_read_x();
uint joystick_read_y();
void reading_joystick(uint16_t *x, uint16_t *y);

#endif