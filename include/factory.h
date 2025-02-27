#ifndef FACTORY_H
#define FACTORY_H

#include "include/matrix.h"
#include "include/joystick.h"
#include "include/buzzer.h"
#include "include/ssd1306.h"
#include <stdlib.h>

#define NUM_SECTORS 9
#define COLS 5
#define ROWS 5
#define LOAD_CAPACITY 2
#define NUM_LOADS 5

// Estrutura para armazenar uma coordenada
typedef struct {
    uint8_t x; // Coordenada x
    uint8_t y; /// Coordenada y
}Position;

typedef uint8_t Sector[NUM_PIXELS]; // Definição de um setor, possuindo 25 posições

// Estrutura para armazenar um robô
typedef struct{
    int sector; // Setor atual
    Position position; // Posição atual
}Robot;

// Estrutura para armazenar a fábrica
typedef struct {
    Sector sectors[NUM_SECTORS]; // Vetor de setores 
    Robot robot; // Robô
}Factory;

// Estrutura para armazenar uma conexão entre setores
typedef struct {
    int from_sector, to_sector; // Setores de origem e destino
    Position from_pos, to_pos; // Posições de origem e destino
} SectorConnection;

void draw_factory(Factory *factory, uint8_t *sector); // Função para desenhar a fábrica
void move_up(Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[]); // Função para mover o robô para cima
void move_down(Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[]); // Função para mover o robô para baixo
void move_left(Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[]); // Função para mover o robô para a esquerda
void move_right(Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[]); // Função para mover o robô para a direita
void change_to_right_sector(Factory *factory, uint8_t *sector); // Função para mudar para o setor à direita
void change_to_left_sector(Factory *factory, uint8_t *sector); // Função para mudar para o setor à esquerda
void change_to_up_sector(Factory *factory, uint8_t *sector); // Função para mudar para o setor acima
void change_to_down_sector(Factory *factory, uint8_t *sector); // Função para mudar para o setor abaixo
void movimentation_in_sector_0(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]); // Função para movimentação no setor 0
void movimentation_in_sector_1(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]); // Função para movimentação no setor 1
void movimentation_in_sector_2(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]); // Função para movimentação no setor 2
void movimentation_in_sector_3(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]); // Função para movimentação no setor 3
void movimentation_in_sector_4(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]); // Função para movimentação no setor 4
void movimentation_in_sector_5(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]); // Função para movimentação no setor 5
void movimentation_in_sector_6(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]); // Função para movimentação no setor 6
void movimentation_in_sector_7(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]); // Função para movimentação no setor 7
void movimentation_in_sector_8(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]); // Função para movimentação no setor 8
void manual_mode_movimentation(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, ssd1306_t *ssd, bool delivered[], Robot objectives[]); // Função para movimentação manual
void automatic_mode_movimentation(Robot* path, int path_length, Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[], ssd1306_t *ssd); // Função para movimentação automática
void find_path(Robot goal, Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[], ssd1306_t *ssd); // Função para encontrar o caminho
bool is_valid_position(int x, int y); // Função para verificar se a posição é válida
bool bfs(Factory *factory, Robot start, Robot goal, Robot **came_from); // Função para realizar a busca em largura
void reconstruct_path(Robot *came_from, Robot start, Robot goal, Robot **path, int *path_length); // Função para reconstruir o caminho
void get_global_coordinates(Robot coordinate, int *globalX, int *globalY); // Função para obter as coordenadas globais
int manhattan_distance(Robot a, Robot b); // Função para calcular a distância de Manhattan
void randomize_objectives(Robot objectives[], Factory *factory); // Função para randomizar os objetivos
void show_destination(Factory *factory); // Função para mostrar o destino
void calculate_distances(int distances[], Robot objectives[], Factory *factory); // Função para calcular as distâncias
void insertion_sort(int arr[], int n, bool aux[], Robot objectives[]); // Função para ordenar as distâncias
void solve_capacitated_vrp(Factory *factory, Robot objectives[], int distances[], bool delivered[], uint8_t *sector, ssd1306_t *ssd); // Função para resolver o problema do VRP
void show_delivarables(Factory *factory, uint8_t sector, bool delivered[], Robot objectives[]); // Função para mostrar os objetivos entregáveis
bool verify_delivered(bool delivered[], int n); // Função para verificar se todos os objetivos foram entregues
void reset_delivered(bool delivered[]); // Função para resetar os objetivos entregues

#endif