#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"   
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "./include/button.h"
#include "./include/buzzer.h"
#include "./include/joystick.h"
#include "./include/led_rgb.h"
#include "./include/matrix.h"
#include "./include/ssd1306.h"
#include "./include/factory.h"

ssd1306_t ssd;
volatile uint32_t last_time_button_a = 0;
volatile uint32_t last_time_button_b = 0;
volatile uint32_t last_time_joystick_button = 0;
uint16_t joystick_x, joystick_y;
uint8_t sector = 0;
Robot objectives[NUM_LOADS];
int distances[NUM_LOADS];
bool delivered[NUM_LOADS] = {false, false, false, false, false};

Factory factory = {
    .sectors[0] = {
        2, 2, 2, 2, 2,
        2, 0, 0, 1, 2,
        2, 0, 0, 0, 0,
        2, 0, 0, 0, 2,
        2, 2, 0, 2, 2
    },
    .sectors[1] = {
        2, 2, 2, 2, 2,
        2, 0, 0, 0, 2,
        0, 0, 0, 0, 0,
        2, 0, 0, 0, 2,
        2, 2, 0, 2, 2
    },
    .sectors[2] = {
        2, 2, 2, 2, 2,
        2, 0, 0, 0, 2,
        0, 0, 0, 0, 2,
        2, 0, 0, 0, 2,
        2, 2, 0, 2, 2
    },
    .sectors[3] = {
        2, 2, 0, 2, 2,
        2, 0, 0, 0, 2,
        2, 0, 0, 0, 0,
        2, 0, 0, 0, 2,
        2, 2, 0, 2, 2
    },
    .sectors[4] = {
        2, 2, 0, 2, 2,
        2, 0, 0, 0, 2,
        0, 0, 0, 0, 0,
        2, 0, 0, 0, 2,
        2, 2, 0, 2, 2
    },
    .sectors[5] = {
        2, 2, 0, 2, 2,
        2, 0, 0, 0, 2,
        0, 0, 0, 0, 2,
        2, 0, 0, 0, 2,
        2, 2, 0, 2, 2
    },
    .sectors[6] = {
        2, 2, 0, 2, 2,
        2, 0, 0, 0, 2,
        2, 0, 0, 0, 0,
        2, 0, 0, 0, 2,
        2, 2, 2, 2, 2
    },
    .sectors[7] = {
        2, 2, 0, 2, 2,
        2, 0, 0, 0, 2,
        0, 0, 0, 0, 0,
        2, 0, 0, 0, 2,
        2, 2, 4, 2, 2
    },
    .sectors[8] = {
        2, 2, 0, 2, 2,
        2, 0, 0, 0, 2,
        0, 0, 0, 0, 2,
        2, 0, 0, 0, 2,
        2, 2, 2, 2, 2
    },
    .robot = {
        .position = {
            .x = 1,
            .y = 3
        },
        .sector = 0
    }
};

int main(){
    stdio_init_all();
    buzzer_init_all();
    matrix_init();
    joystick_init();
    matrix_init();
    display_init(&ssd);
    randomize_objectives(objectives, &factory);
    sleep_ms(10000);
    solve_capacitated_vrp(&factory, objectives, distances, delivered, &sector);
    while (true) {
        show_destination(&factory);
        draw_factory(&factory, &sector);
        manual_mode_movimentation(&factory, &sector, joystick_x, joystick_y);  
        
    }
    return 0;
}
