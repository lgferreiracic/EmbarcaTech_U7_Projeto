#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define WS2812_PIN 7 // Define o pino do LED RGB
#define NUM_PIXELS 25 // Define o número de LEDs RGB
#define FRAME_DELAY 200 // Define o atraso entre os frames

// Definição da estrutura RGB para representar as cores
typedef struct {
    double R; // Intensidade da cor vermelha
    double G; // Intensidade da cor verde
    double B; // Intensidade da cor azul
} RGB;

//Cores 
extern const RGB RED; // Vermelho
extern const RGB GREEN; // Verde
extern const RGB BLUE; // Azul
extern const RGB YELLOW; // Amarelo
extern const RGB CYAN; // Ciano
extern const RGB MAGENTA; // Magenta
extern const RGB WHITE; // Branco
extern const RGB BLACK; // Preto

uint matrix_init(); // Inicializa a matriz de LEDs RGB
uint32_t matrix_rgb(double r, double g, double b); // Função para converter as intensidades de cor para um valor RGB
void set_leds(PIO pio, uint sm, double r, double g, double b); // Função para definir as cores dos LEDs
int getIndex(int x, int y); // Função para obter o índice do LED RGB
void getCoordinates(int index, int *x, int *y); // Função para obter as coordenadas do LED RGB
void desenho_pio(RGB pixels[NUM_PIXELS], PIO pio, uint sm); // Função para desenhar a matriz de LEDs RGB
uint coordenates_to_index(int x, int y); // Função para converter as coordenadas para o índice do LED RGB
void clear_matrix(); // Função para apagar a matriz de LEDs RGB

#endif