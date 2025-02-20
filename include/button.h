#ifndef BUTTON_H
#define BUTTON_H

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"

void button_init(uint gpio);
bool debounce(volatile uint32_t *last_time);

#endif