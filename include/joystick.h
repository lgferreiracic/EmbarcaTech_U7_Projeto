#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

void joystick_init(uint x_pin, uint y_pin);
uint joystick_read_x(uint x_pin);
uint joystick_read_y(uint y_pin);

#endif