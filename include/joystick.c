#include "joystick.h"

void joystick_init(){
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);
}

uint joystick_read_x(){
    adc_select_input(1);
    return adc_read();
}

uint joystick_read_y(){
    adc_select_input(0);
    return adc_read();
}

// Função para leitura do joystick, verificando se o joystick está posicionado para cima, baixo, esquerda, direita ou no centro.
void reading_joystick(uint16_t *x, uint16_t *y){
    *x = joystick_read_x();
    *y = joystick_read_y();
}
