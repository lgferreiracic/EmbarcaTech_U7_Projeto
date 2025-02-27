#ifndef LED_RGB_H
#define LED_RGB_H

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define LED_GREEN_PIN 11 // Define o pino do LED verde
#define LED_BLUE_PIN 12 // Define o pino do LED azul
#define LED_RED_PIN 13 // Define o pino do LED vermelho

void led_init(uint led_pin); // Inicializa o LED
void led_init_all(); // Inicializa todos os LEDs
void red(); // Acende o LED vermelho
void green(); // Acende o LED verde
void blue(); // Acende o LED azul
void yellow(); //Combinação de vermelho e verde
void cyan(); //Combinação de verde e azul
void magenta(); //Combinação de vermelho e azul
void white(); //Combinação de vermelho, verde e azul
void black(); // Apaga todos os LEDs

#endif