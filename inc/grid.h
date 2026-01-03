#ifndef GRID_H_
#define GRID_H_

#include <stdint.h>
#include <assert.h>

#define CHUNK_SIZE 8
static_assert((CHUNK_SIZE&(CHUNK_SIZE-1)) == 0, "Chunk size must be a power of 2.");
#define WID 1024
#define HEI 512
static_assert(WID%CHUNK_SIZE == 0 && HEI%CHUNK_SIZE == 0, "Window size must be divisible by chunk size.");
#define CHORZ (WID/CHUNK_SIZE)
#define CVERT (HEI/CHUNK_SIZE)

enum tile : uint8_t
{
    empty = 0,
    water = 1,
    sand  = 2,
    stone = 3,
    TILE_MAX,
};

extern tile grid[CVERT][CHORZ][CHUNK_SIZE][CHUNK_SIZE];
extern uint32_t dead_chunk_ticks[CVERT][CHORZ];

// Functions
extern void grid_update();
extern void set_cell(uint32_t x, uint32_t y, tile value);

#endif // GRID_H_
