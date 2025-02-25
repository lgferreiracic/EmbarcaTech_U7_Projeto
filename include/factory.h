#ifndef FACTORY_H
#define FACTORY_H

#include "include/matrix.h"

#define NUM_SECTORS 9
#define COLS 5
#define ROWS 5

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

bool is_valid_position(int x, int y);
bool bfs(Factory *factory, Robot start, Robot goal, Robot **came_from);
void reconstruct_path(Robot *came_from, Robot start, Robot goal, Robot **path, int *path_length);


#endif