#ifndef MATRIX_H
#define MATRIX_H

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"

#define NUM_PIXELS 25
#define FRAME_DELAY 200

// Definição da estrutura RGB
typedef struct {
    double R;
    double G;
    double B;
} RGB;

//Cores 
const RGB RED = {1, 0, 0};
const RGB GREEN = {0, 1, 0};
const RGB BLUE = {0, 0, 1};
const RGB YELLOW = {1, 1, 0};
const RGB CYAN = {0, 1, 1};
const RGB MAGENTA = {1, 0, 1};
const RGB WHITE = {1, 1, 1};
const RGB BLACK = {0, 0, 0};

uint32_t matrix_rgb(double r, double g, double b);
void set_leds(PIO pio, uint sm, double r, double g, double b);
int getIndex(int x, int y);
void desenho_pio(RGB pixels[NUM_PIXELS], PIO pio, uint sm);

#endif