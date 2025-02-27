#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include <stdio.h>

// Definição dos parâmetros do display OLED
#define SSD1306_WIDTH 128 // Largura do display
#define SSD1306_HEIGHT 64 // Altura do display
#define SSD1306_ADDR 0x3C // Endereço do display
#define I2C_PORT i2c1 // Porta I2C
#define WIDTH 128 // Largura
#define HEIGHT 64 // Altura
#define SDA_PIN 14 // Pino SDA
#define SCL_PIN 15 // Pino SCL

typedef enum {
  SET_CONTRAST = 0x81,
  SET_ENTIRE_ON = 0xA4,
  SET_NORM_INV = 0xA6,
  SET_DISP = 0xAE,
  SET_MEM_ADDR = 0x20,
  SET_COL_ADDR = 0x21,
  SET_PAGE_ADDR = 0x22,
  SET_DISP_START_LINE = 0x40,
  SET_SEG_REMAP = 0xA0,
  SET_MUX_RATIO = 0xA8,
  SET_COM_OUT_DIR = 0xC0,
  SET_DISP_OFFSET = 0xD3,
  SET_COM_PIN_CFG = 0xDA,
  SET_DISP_CLK_DIV = 0xD5,
  SET_PRECHARGE = 0xD9,
  SET_VCOM_DESEL = 0xDB,
  SET_CHARGE_PUMP = 0x8D
} ssd1306_command_t; // Comandos do display OLED SSD1306

// Estrutura para armazenar as configurações do display OLED SSD1306 
typedef struct {
  uint8_t width, height, pages, address; // Parâmetros do display
  i2c_inst_t *i2c_port; // Porta I2C
  bool external_vcc; // Tensão de alimentação
  uint8_t *ram_buffer; // Buffer de memória
  size_t bufsize; // Tamanho do buffer
  uint8_t port_buffer[2]; // Buffer de porta
} ssd1306_t; 

void ssd1306_init(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c); // Inicializa o display OLED SSD1306
void ssd1306_config(ssd1306_t *ssd);  // Configura o display OLED SSD1306
void ssd1306_command(ssd1306_t *ssd, uint8_t command); // Envia um comando para o display OLED SSD1306
void ssd1306_send_data(ssd1306_t *ssd); // Envia dados para o display OLED SSD1306
void ssd1306_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value); // Desenha um pixel no display OLED SSD1306
void ssd1306_fill(ssd1306_t *ssd, bool value); // Preenche o display OLED SSD1306
void ssd1306_rect(ssd1306_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value, bool fill); // Desenha um retângulo no display OLED SSD1306
void ssd1306_line(ssd1306_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value); // Desenha uma linha no display OLED SSD1306
void ssd1306_hline(ssd1306_t *ssd, uint8_t x0, uint8_t x1, uint8_t y, bool value); // Desenha uma linha horizontal no display OLED SSD1306
void ssd1306_vline(ssd1306_t *ssd, uint8_t x, uint8_t y0, uint8_t y1, bool value); // Desenha uma linha vertical no display OLED SSD1306
void ssd1306_draw_char(ssd1306_t *ssd, char c, uint8_t x, uint8_t y); // Desenha um caractere no display OLED SSD1306
void ssd1306_draw_string(ssd1306_t *ssd, const char *str, uint8_t x, uint8_t y); // Desenha uma string no display OLED SSD1306
void display_init(ssd1306_t *ssd); // Inicializa o display OLED SSD1306
void about_display(ssd1306_t *ssd); // Exibe a tela de sobre
void clear_display(ssd1306_t *ssd); // Limpa o display OLED SSD1306
void option_1_selected(ssd1306_t *ssd); // Exibe a tela de opção 1 selecionada
void option_2_selected(ssd1306_t *ssd); // Exibe a tela de opção 2 selecionada
void option_3_selected(ssd1306_t *ssd); // Exibe a tela de opção 3 selecionada
void navigation_status(ssd1306_t *ssd, uint8_t sector, int capacity, int missing); // Exibe o status da navegação