#include "joystick.h"

void joystick_init(uint x_pin, uint y_pin){
    adc_init();
    adc_gpio_init(x_pin);
    adc_gpio_init(y_pin);
}

uint joystick_read_x(uint x_pin){
    adc_select_input(x_pin);
    return adc_read();
}

uint joystick_read_y(uint y_pin){
    adc_select_input(y_pin);
    return adc_read();
}