#include "led_rgb.h"

void led_rgb_init(uint led_red_pin, uint led_green_pin, uint led_blue_pin){
    gpio_init(led_red_pin);
    gpio_set_dir(led_red_pin, GPIO_OUT);

    gpio_init(led_green_pin);
    gpio_set_dir(led_green_pin, GPIO_OUT);
    
    gpio_init(led_blue_pin);
    gpio_set_dir(led_blue_pin, GPIO_OUT);
}