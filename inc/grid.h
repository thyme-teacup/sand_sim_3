#ifndef GRID_H_
#define GRID_H_

#include "raylib.h"
#include <stdint.h>
#include <assert.h>

namespace grid
{

enum tile : uint8_t
{
    empty = 0,
    water,
    sand,
    stone,
};

extern tile *grid;
extern uint32_t width, height;
extern const uint32_t chunk_size;

// Functions

void init(uint32_t w, uint32_t h, tile *buffer, Color *color_table); // Assumes the buffer has w*h*sizeof(tile) bytes
void generate();
void update();

}

#endif // GRID_H_
