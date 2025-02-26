#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
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

void irq_handler(uint gpio, uint32_t events){
    if (gpio == BUTTON_A_PIN && debounce(&last_time_button_a)){
        clear_matrix();
        clear_display(&ssd);
        reset_usb_boot(0,0);
    }
    else if (gpio == BUTTON_B_PIN && debounce(&last_time_button_b)){
        printf("Botão B pressionado\n");
    }
    else if (gpio == JOYSTICK_BUTTON_PIN && debounce(&last_time_joystick_button)){
        printf("Botão do joystick pressionado\n");
    }
}

int main(){
    stdio_init_all();
    buzzer_init_all();
    matrix_init();
    joystick_init();
    matrix_init();
    led_init_all();
    button_init_all();
    display_init(&ssd);
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &irq_handler);
    gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &irq_handler);

    start_display(&ssd);

    randomize_objectives(objectives, &factory);
    solve_capacitated_vrp(&factory, objectives, distances, delivered, &sector);
    while (true) {
        show_destination(&factory);
        draw_factory(&factory, &sector);
        manual_mode_movimentation(&factory, &sector, joystick_x, joystick_y, &ssd);  
        
    }
    return 0;
}
