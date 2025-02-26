#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "stdio.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define JOYSTICK_X_PIN 26 // Define o pino do eixo x do joystick
#define JOYSTICK_Y_PIN 27 // Define o pino do eixo y do joystick

void joystick_init(); // Inicializa o joystick
uint joystick_read_x(); // Lê o valor do eixo x do joystick
uint joystick_read_y(); // Lê o valor do eixo y do joystick
void reading_joystick(uint16_t *x, uint16_t *y); // Lê o valor do joystick

#endif