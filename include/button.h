#ifndef BUTTON_H
#define BUTTON_H

#include "stdio.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define JOYSTICK_BUTTON_PIN 22

void button_init(uint gpio);
void button_init_all();
bool debounce(volatile uint32_t *last_time);
void irq_handler(uint gpio, uint32_t events);

#endif