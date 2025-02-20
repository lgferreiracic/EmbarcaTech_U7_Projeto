#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

void buzzer_init(uint gpio);
void set_buzzer_frequency(uint pin, uint frequency);
void play_buzzer(uint pin, uint frequency, uint duration_ms);

#endif