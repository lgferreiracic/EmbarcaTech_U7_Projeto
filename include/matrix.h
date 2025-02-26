#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define WS2812_PIN 7
#define NUM_PIXELS 25
#define FRAME_DELAY 200

// Definição da estrutura RGB
typedef struct {
    double R;
    double G;
    double B;
} RGB;

//Cores 
extern const RGB RED;
extern const RGB GREEN;
extern const RGB BLUE;
extern const RGB YELLOW;
extern const RGB CYAN;
extern const RGB MAGENTA;
extern const RGB WHITE;
extern const RGB BLACK;

uint matrix_init();
uint32_t matrix_rgb(double r, double g, double b);
void set_leds(PIO pio, uint sm, double r, double g, double b);
int getIndex(int x, int y);
void getCoordinates(int index, int *x, int *y);
void desenho_pio(RGB pixels[NUM_PIXELS], PIO pio, uint sm);
uint coordenates_to_index(int x, int y);
void clear_matrix();

#endif