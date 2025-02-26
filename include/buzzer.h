#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "include/led_rgb.h"

#define BUZZER_A_PIN 10
#define BUZZER_B_PIN 21

void buzzer_init(uint gpio);
void buzzer_init_all();
void set_buzzer_frequency(uint pin, uint frequency);
void play_buzzer(uint pin, uint frequency, uint duration_ms);
void play_denied_sound();
void play_success_sound();
void play_delivery_sound();

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