#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "include/led_rgb.h"

#define BUZZER_A_PIN 10 // Define o pino do buzzer A
#define BUZZER_B_PIN 21 // Define o pino do buzzer B

void buzzer_init(uint gpio); // Inicializa o buzzer
void buzzer_init_all(); // Inicializa todos os buzzers
void set_buzzer_frequency(uint pin, uint frequency); // Configura a frequência do buzzer
void play_buzzer(uint pin, uint frequency, uint duration_ms); // Toca o buzzer
void play_denied_sound(); // Toca o som de negação
void play_success_sound(); // Toca o som de sucesso
void play_delivery_sound(); // Toca o som de entrega de objetivo

// Frequências das notas musicais (em Hz)
enum NotasMusicais {
    DO = 2640, // Dó
    RE = 2970, // Ré
    MI = 3300, // Mi
    FA = 3520, // Fá
    SOL = 3960, // Sol
    LA = 4400, // Lá
    SI = 4950  // Si
};

#endif