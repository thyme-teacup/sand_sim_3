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
};

extern tile grid[CVERT][CHORZ][CHUNK_SIZE][CHUNK_SIZE];

// Functions
extern void grid_update();

#endif // GRID_H_
