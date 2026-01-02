#include "grid.h"

namespace grid
{

// Definitions
tile *grid = 0;
uint32_t width = 0, height = 0;

const uint32_t chunk_size = 8; // 8x8 chunks so they fit into single cache lines
static_assert((chunk_size&(chunk_size-1)) == 0, "Chunk size must be a power of 2");

// Functions

void init(uint32_t w, uint32_t h, tile *buffer, Color *color_table)
{
    assert(w % chunk_size == 0);
    assert(h % chunk_size == 0);

    width = w;
    height = h;
    grid = buffer;

    color_table[empty] = Color({172, 107, 83, 255});
    color_table[water] = Color({14, 135, 204, 255});
    color_table[sand]  = Color({149, 140, 129, 255});
    color_table[stone] = Color({226, 202, 118, 255});
}

static inline tile get_cell(uint32_t chx, uint32_t chy, uint32_t x, uint32_t y)
{
    if(chx < 0 || chx >= width/chunk_size)   return stone;
    if(chy < 0 || chy >= height/chunk_size)  return stone;
    if(x   < 0 || x   >= chunk_size)         return stone;
    if(y   < 0 || y   >= chunk_size)         return stone;

    uint32_t offset = (chy*(width/chunk_size)+chx)*chunk_size*chunk_size;
    return grid[offset + y*chunk_size+x];
}

static inline void set_cell(uint32_t chx, uint32_t chy, uint32_t x, uint32_t y, tile tl)
{
    assert(chx >= 0 && chx < width/chunk_size);
    assert(chy >= 0 && chy < height/chunk_size);
    assert(x   >= 0 && x   < chunk_size);
    assert(y   >= 0 && y   < chunk_size);

    
    uint32_t offset = (chy*(width/chunk_size)+chx)*chunk_size*chunk_size;
    grid[offset + y*chunk_size+x] = tl;
}

void generate()
{
    uint32_t w = width;
    uint32_t h = height;

    for(int i = 0; i < height/chunk_size/2; ++i)
    for(int j = 0; j < width/chunk_size/2;  ++j)
    for(int e = 0; e < chunk_size; ++e)
    for(int f = 0; f < chunk_size; ++f)
    {
        set_cell(j, i, f, e, sand);
    }
    
}

static void update_chunk(uint32_t i, uint32_t j)
{
}

void update()
{
    for(int i = height/chunk_size-1; i >= 0; --i)
    for(int j = 0; j < width/chunk_size; ++j)
    {
        update_chunk(i, j);
    }
}

}
