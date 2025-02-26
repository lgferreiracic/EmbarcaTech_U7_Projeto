#ifndef BUTTON_H
#define BUTTON_H

#include "stdio.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#define BUTTON_A_PIN 5 // Define o pino do botão A
#define BUTTON_B_PIN 6 // Define o pino do botão B
#define JOYSTICK_BUTTON_PIN 22 // Define o pino do botão do joystick

void button_init(uint gpio); // Inicializa o botão
void button_init_all(); // Inicializa todos os botões
bool debounce(volatile uint32_t *last_time); // Função para debouncing
void irq_handler(uint gpio, uint32_t events); // Função para gerenciar a interrupção dos botões

#endif