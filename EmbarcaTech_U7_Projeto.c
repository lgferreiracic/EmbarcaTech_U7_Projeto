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

ssd1306_t ssd; // Variável para o display OLED SSD1306
volatile uint32_t last_time_button_a = 0; // Variável para debounce do botão A
volatile uint32_t last_time_button_b = 0; // Variável para debounce do botão B
volatile uint32_t last_time_joystick_button = 0; // Variável para debounce do botão do joystick
volatile int option_selected = 0; // Variável para armazenar a opção selecionada
volatile int option = 1; // Variável para armazenar a opção atual
uint16_t joystick_x, joystick_y; // Variáveis para armazenar a leitura do joystick em relação aos eixos x e y
uint8_t sector = 0; // Variável para armazenar o setor atual
Robot objectives[NUM_LOADS]; // Vetor para armazenar os objetivos do robô
int distances[NUM_LOADS]; // Vetor para armazenar as distâncias entre o robô e os objetivos
bool delivered[NUM_LOADS] = {false, false, false, false, false}; // Vetor para armazenar se os objetivos foram entregues

// Estrutura responsável por armazenar a fábrica e o robô
Factory factory = {
    .sectors[0] = {
        2, 2, 2, 2, 2,
        2, 1, 0, 0, 2,
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
            .y = 1
        },
        .sector = 0
    }
};

// Função para gerenciar a interrupção dos botões
void irq_handler(uint gpio, uint32_t events){
    if (gpio == BUTTON_A_PIN && debounce(&last_time_button_a)){
        clear_matrix();
        clear_display(&ssd);
        reset_usb_boot(0,0);
    }
    else if (gpio == BUTTON_B_PIN && debounce(&last_time_button_b)){
        option_selected = option;
        option = 0;
        if(option_selected == 1 || option_selected == 2){
            randomize_objectives(objectives, &factory);
            reset_delivered(delivered);
        }
    }
    else if (gpio == JOYSTICK_BUTTON_PIN && debounce(&last_time_joystick_button)){
        option = option_selected;
        option_selected = 0;
        clear_matrix();
        reset_delivered(delivered);
        for(int i = 0; i < NUM_SECTORS; i++){
            for(int j = 0; j < NUM_PIXELS; j++){
                if(factory.sectors[i][j] == 3){
                    factory.sectors[i][j] = 0;
                }
            }
        }
    }
}

int main(){
    //Rotina de inicialização
    stdio_init_all(); 
    buzzer_init_all();
    matrix_init();
    joystick_init();
    matrix_init();
    led_init_all();
    button_init_all();
    display_init(&ssd);

    //Configuração dos pinos de interrupção
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &irq_handler);
    gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &irq_handler);

    while (true) {
        reading_joystick(&joystick_x, &joystick_y); // Leitura do joystick

        if(joystick_y < 1000 && option_selected == 0){ //Se o joystick for pressionado para baixo e nenhuma opção estiver selecionada
            option++;
            if(option > 3){
                option = 1;
            }
        }
        else if(joystick_y > 3000 && option_selected == 0){ //Se o joystick for pressionado para cima e nenhuma opção estiver selecionada
            option--;
            if(option < 1){
                option = 3;
            }
        }

        //Seleção da opção
        switch(option){
            case 1:
                option_1_selected(&ssd);
                break;
            case 2:
                option_2_selected(&ssd);
                break;
            case 3:
                option_3_selected(&ssd);
                break;
            default:
                break;
        }

        //Seleção da opção selecionada
        switch(option_selected){
            case 1:
                manual_mode_movimentation(&factory, &sector, joystick_x, joystick_y, &ssd, delivered, objectives);
                break;
            case 2:
                clear_display(&ssd);
                clear_matrix();
                solve_capacitated_vrp(&factory, objectives, distances, delivered, &sector, &ssd);
                option_selected = 1;
                break;
            case 3:
                about_display(&ssd);
                option = 1;
                option_selected = 0;
                break;
            default:
                break;
        }
        sleep_ms(200);
    }
    return 0;
}
