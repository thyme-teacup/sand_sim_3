#ifndef GRID_H_
#define GRID_H_

#include <stdint.h>
#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h> // TODO : remove this

#define CHUNK_SIZE 8
static_assert((CHUNK_SIZE&(CHUNK_SIZE-1)) == 0, "Chunk size must be a power of 2.");
#define WID 1536
#define HEI 768
static_assert(WID%CHUNK_SIZE == 0 && HEI%CHUNK_SIZE == 0, "Window size must be divisible by chunk size.");
#define CHORZ (WID/CHUNK_SIZE)
#define CVERT (HEI/CHUNK_SIZE)
#define THREAD_COUNT 4
static_assert(CHORZ%THREAD_COUNT == 0, "Amount of chunks horizontally must be divisible by the number of threads.");

#define tile uint8_t
#define empty 0
#define water 1
#define sand  2
#define stone 3
#define TILE_MAX 4

extern tile grid[CVERT][CHORZ][CHUNK_SIZE][CHUNK_SIZE] __attribute__((aligned(64)));
extern atomic_int dead_chunk_ticks[CVERT][CHORZ];

// Functions
extern void grid_jumpstart();
extern void grid_update();
extern void grid_kill();
extern void set_cell(uint32_t x, uint32_t y, tile value);

#endif // GRID_H_
