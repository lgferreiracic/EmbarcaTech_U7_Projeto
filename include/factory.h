#ifndef FACTORY_H
#define FACTORY_H

#include "include/matrix.h"

#define NUM_SECTORS 9

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

extern Factory factory;

#endif