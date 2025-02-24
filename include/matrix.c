#include "matrix.h"

//Cores 
const RGB RED = {1, 0, 0};
const RGB GREEN = {0, 1, 0};
const RGB BLUE = {0, 0, 1};
const RGB YELLOW = {1, 1, 0};
const RGB CYAN = {0, 1, 1};
const RGB MAGENTA = {1, 0, 1};
const RGB WHITE = {1, 1, 1};
const RGB BLACK = {0, 0, 0};

PIO pio;
uint sm;

//rotina para inicialização da matrix de leds - ws2812b
uint matrix_init() {
   //Configurações da PIO
   pio = pio0; 
   uint offset = pio_add_program(pio, &pio_matrix_program);
   sm = pio_claim_unused_sm(pio, true);
   pio_matrix_program_init(pio, sm, offset, WS2812_PIN);
}

//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double r, double g, double b){
  unsigned char R, G, B;
  R = r * 55;
  G = g * 55;
  B = b * 55;
  return (G << 24) | (R << 16) | (B << 8);
}

//rotina para acionar a matrix de leds - ws2812b
void set_leds(PIO pio, uint sm, double r, double g, double b) {
    uint32_t valor_led;
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(r, g, b);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

// Função para converter a posição do matriz para uma posição do vetor.
int getIndex(int x, int y) {
    // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
    // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
    if (y % 2 == 0) {
        return 24-(y * 5 + x); // Linha par (esquerda para direita).
    } else {
        return 24-(y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
    }
}

void getCoordinates(int index, int *x, int *y) {
    *x = (index % 5);
    *y = (index / 5);
}

uint coordenates_to_index(int x, int y) {
    return x * 5 + y;
}

//rotina para acionar a matrix de leds - ws2812b
void desenho_pio(RGB pixels[NUM_PIXELS], PIO pio, uint sm) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        int x = i % 5;
        int y = i / 5;
        int index = getIndex(x, y);
        pio_sm_put_blocking(pio, sm, matrix_rgb(pixels[index].R, pixels[index].G, pixels[index].B));
    }
}

// Função para reproduzir a animação 0, que é um coração pulsante
void play_animation_0() {
    
    RGB frame1[NUM_PIXELS] = {
        BLACK, RED, BLACK, RED, BLACK,
        RED, RED, RED, RED, RED,
        RED, RED, RED, RED, RED,
        BLACK, RED, RED, RED, BLACK,
        BLACK, BLACK, RED, BLACK, BLACK
    };

    RGB frame2[NUM_PIXELS] = {
        BLACK, MAGENTA, BLACK, MAGENTA, BLACK,
        MAGENTA, RED, RED, RED, MAGENTA,
        MAGENTA, RED, RED, RED, MAGENTA,
        BLACK, RED, RED, RED, BLACK,
        BLACK, BLACK, MAGENTA, BLACK, BLACK
    };

    RGB frame3[NUM_PIXELS] = {
        BLACK, MAGENTA, BLACK, MAGENTA, BLACK,
        MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA,
        MAGENTA, MAGENTA, RED, MAGENTA, MAGENTA,
        BLACK, MAGENTA, MAGENTA, MAGENTA, BLACK,
        BLACK, BLACK, MAGENTA, BLACK, BLACK
    };
       
    RGB frame4[NUM_PIXELS] = {
        BLACK, MAGENTA, BLACK, MAGENTA, BLACK,
        MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA,
        MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA,
        BLACK, MAGENTA, MAGENTA, MAGENTA, BLACK,
        BLACK, BLACK, MAGENTA, BLACK, BLACK
    };

    RGB frame5[NUM_PIXELS] = {
        BLACK, RED, BLACK, RED, BLACK,
        RED, RED, RED, RED, RED,
        RED, RED, RED, RED, RED,
        BLACK, RED, RED, RED, BLACK,
        BLACK, BLACK, RED, BLACK, BLACK
    };
    
    RGB *frames[5] = {frame1, frame2, frame3, frame4, frame5};

    for(int i = 0; i < 5; i++){
        for (int i = 0; i < 5; i++) {
            desenho_pio(frames[i], pio, sm);
            sleep_ms(FRAME_DELAY);
        }
    }
    set_leds(pio, sm, 0, 0, 0); // Desliga todos os LEDs
}

