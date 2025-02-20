#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"   
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "include/button.h"
#include "include/buzzer.h"
#include "include/joystick.h"
#include "include/led_rgb.h"
#include "include/matrix.h"
#include "include/ssd1306.h"
#include "ws2812.pio.h"

// Definição dos pinos correspondentes aos componentes
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define WS2812_PIN 7
#define BUZZER_A_PIN 10
#define LED_GREEN_PIN 11
#define LED_BLUE_PIN 12
#define LED_RED_PIN 13
#define SDA_PIN 14
#define SCL_PIN 15
#define BUZZER_B_PIN 21
#define JOYSTICK_BUTTON_PIN 22
#define JOYSTICK_X_PIN 26
#define JOYSTICK_Y_PIN 27

// Definição dos parâmetros do display OLED
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_ADDR 0x3C
#define I2C_PORT i2c1

int main(){
    stdio_init_all();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
