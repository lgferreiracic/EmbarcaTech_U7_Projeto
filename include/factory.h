#ifndef FACTORY_H
#define FACTORY_H

#include "include/matrix.h"
#include "include/joystick.h"
#include "include/buzzer.h"
#include <stdlib.h>

#define NUM_SECTORS 9
#define COLS 5
#define ROWS 5
#define LOAD_CAPACITY 2
#define NUM_LOADS 5

typedef struct {
    uint8_t x;
    uint8_t y;
}Position;

typedef uint8_t Sector[NUM_PIXELS];

typedef struct{
    int sector;
    Position position;
}Robot;

typedef struct {
    Sector sectors[NUM_SECTORS];
    Robot robot;
}Factory;

typedef struct {
    int from_sector, to_sector;
    Position from_pos, to_pos;
} SectorConnection;
void draw_factory(Factory *factory, uint8_t *sector);
void move_up(Factory *factory, uint8_t *sector);
void move_down(Factory *factory, uint8_t *sector);
void move_left(Factory *factory, uint8_t *sector);
void move_right(Factory *factory, uint8_t *sector);
void change_to_right_sector(Factory *factory, uint8_t *sector);
void change_to_left_sector(Factory *factory, uint8_t *sector);
void change_to_up_sector(Factory *factory, uint8_t *sector);
void change_to_down_sector(Factory *factory, uint8_t *sector);
void movimentation_in_sector_0(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y);
void movimentation_in_sector_1(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y);
void movimentation_in_sector_2(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y);
void movimentation_in_sector_3(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y);
void movimentation_in_sector_4(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y);
void movimentation_in_sector_5(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y);
void movimentation_in_sector_6(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y);
void movimentation_in_sector_7(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y);
void movimentation_in_sector_8(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y);
void manual_mode_movimentation(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y);
void automatic_mode_movimentation(Robot* path, int path_length, Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[]);
void find_path(Robot goal, Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[]);
bool is_valid_position(int x, int y);
bool bfs(Factory *factory, Robot start, Robot goal, Robot **came_from);
void reconstruct_path(Robot *came_from, Robot start, Robot goal, Robot **path, int *path_length);
void get_global_coordinates(Robot coordinate, int *globalX, int *globalY);
int manhattan_distance(Robot a, Robot b);
void randomize_objectives(Robot objectives[], Factory *factory);
void show_destination(Factory *factory);
void calculate_distances(int distances[], Robot objectives[], Factory *factory);
void insertion_sort(int arr[], int n, bool aux[], Robot objectives[]);
void solve_capacitated_vrp(Factory *factory, Robot objectives[], int distances[], bool delivered[], uint8_t *sector);
void show_delivarables(Factory *factory, uint8_t sector, bool delivered[], Robot objectives[]);
bool verify_delivered(bool delivered[], int n);


#endif