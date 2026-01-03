#include "grid.h"

tile grid[CVERT][CHORZ][CHUNK_SIZE][CHUNK_SIZE];
uint32_t dead_chunk_ticks[CVERT][CHORZ] = {0};

void set_cell(uint32_t x, uint32_t y, tile value)
{
    assert(x >= 0 && x < WID && y >= 0 && y < HEI);
    assert(value < TILE_MAX && value >= 0);

    uint32_t i, j, e, f;

    i = y;
    j = x;
    uint32_t tmp = CHUNK_SIZE;
    while(tmp != 1)
    {
        tmp >>= 1;
        i >>= 1;
        j >>= 1;
    }

    e = y&(CHUNK_SIZE-1);
    f = x&(CHUNK_SIZE-1);

    grid[i][j][e][f] = value;

    dead_chunk_ticks[i][j] = 0;
}

static bool toggle_switch()
{
    static bool ts = true;

    return ts = !ts;
}

static tile get_cell(uint32_t x, uint32_t y)
{
    if(x < 0 || x >= WID || y < 0 || y >= HEI) return stone;

    uint32_t i, j, e, f;

    i = y;
    j = x;
    uint32_t tmp = CHUNK_SIZE;
    while(tmp != 1)
    {
        tmp >>= 1;
        i >>= 1;
        j >>= 1;
    }

    e = y&(CHUNK_SIZE-1);
    f = x&(CHUNK_SIZE-1);

    return grid[i][j][e][f];
}

static void swap_cells(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2)
{
    assert(x1 >= 0 && x1 < WID);
    assert(y1 >= 0 && y1 < HEI);
    assert(x2 >= 0 && x2 < WID);
    assert(y2 >= 0 && y2 < HEI);

    uint32_t i1, j1, e1, f1, i2, j2, e2, f2;

    i1 = y1;
    j1 = x1;
    uint32_t tmp = 1;
    while(tmp != CHUNK_SIZE)
    {
        tmp <<= 1;
        i1 >>= 1;
        j1 >>= 1;
    }

    e1 = y1&(CHUNK_SIZE-1);
    f1 = x1&(CHUNK_SIZE-1);

    i2 = y2;
    j2 = x2;
    tmp = 1;
    while(tmp != CHUNK_SIZE)
    {
        tmp <<= 1;
        i2 >>= 1;
        j2 >>= 1;
    }

    e2 = y2&(CHUNK_SIZE-1);
    f2 = x2&(CHUNK_SIZE-1);

    grid[i1][j1][e1][f1] = (tile)(grid[i1][j1][e1][f1]^grid[i2][j2][e2][f2]);
    grid[i2][j2][e2][f2] = (tile)(grid[i1][j1][e1][f1]^grid[i2][j2][e2][f2]);
    grid[i1][j1][e1][f1] = (tile)(grid[i1][j1][e1][f1]^grid[i2][j2][e2][f2]);
    
    dead_chunk_ticks[i1][j1] = 0;
    dead_chunk_ticks[i2][j2] = 0;
}

static void wake_up_adjacents(uint32_t i, uint32_t j)
{
    if(dead_chunk_ticks[i][j] != 0) return;

    // Upwards
    for(int f = 0; f < CHUNK_SIZE; ++f)
        if(grid[i][j][0][f] == empty || grid[i][j][0][f] == water)
        {
            if(i!=0) dead_chunk_ticks[i-1][j] = 0;
            break;
        }

    // Leftwards
    for(int e = 0; e < CHUNK_SIZE; ++e)
        if(grid[i][j][e][0] == empty || grid[i][j][e][0] == water)
        {
            if(j!=0) dead_chunk_ticks[i][j-1] = 0;
            break;
        }

    // Rightwards
    for(int e = 0; e < CHUNK_SIZE; ++e)
        if(grid[i][j][e][CHUNK_SIZE-1] == empty || grid[i][j][e][CHUNK_SIZE-1] == water)
        {
            if(j!=CHORZ-1) dead_chunk_ticks[i][j+1] = 0;
            break;
        }

    // Left corner
    if(grid[i][j][0][0] == empty || grid[i][j][0][0] == water)
        if(i!=0 && j!=0) dead_chunk_ticks[i-1][j-1] = 0;

    // Right corner
    if(grid[i][j][0][CHUNK_SIZE-1] == empty || grid[i][j][0][CHUNK_SIZE-1] == water)
        if(i!=0 && j!=CHORZ-1) dead_chunk_ticks[i-1][j+1] = 0;
}

static void update_chunk(uint32_t i, uint32_t j)
{
    ++dead_chunk_ticks[i][j];

    uint32_t x, y;
    for(int e = CHUNK_SIZE-1; e >= 0; --e)
        for(int f = 0; f < CHUNK_SIZE; ++f)
        {
            // global coords
            y = i*CHUNK_SIZE + e;
            x = (y&1) == 0 ? j*CHUNK_SIZE+f : (j+1)*CHUNK_SIZE-f-1; // For alternating order

            if(get_cell(x, y) == empty) continue;
            if(get_cell(x, y) == stone) continue;
    
            // Sand
            if(get_cell(x, y) == sand)
            {
                // Down
                if(get_cell(x, y+1) == empty || get_cell(x, y+1) == water)
                {
                    swap_cells(x, y, x, y+1);
                    continue;
                }
    
                // Diagonal randomness switch
                if((get_cell(x-1, y+1) == empty || get_cell(x-1, y+1) == water)
                && (get_cell(x+1, y+1) == empty || get_cell(x+1, y+1) == water))
                {
                    if(toggle_switch())
                    {
                        swap_cells(x, y, x-1, y+1);
                        continue;
                    }
                    else
                    {
                        swap_cells(x, y, x+1, y+1);
                        continue;
                    }
                }
    
                // Diagonal left
                if(get_cell(x-1, y+1) == empty || get_cell(x-1, y+1) == water)
                {
                    swap_cells(x, y, x-1, y+1);
                    continue;
                }
    
                // Diagonal right
                if(get_cell(x+1, y+1) == empty || get_cell(x+1, y+1) == water)
                {
                    swap_cells(x, y, x+1, y+1);
                    continue;
                }
            }

            // Water
            if(get_cell(x, y) == water)
            {
                // Down
                if(get_cell(x, y+1) == empty)
                {
                    swap_cells(x, y, x, y+1);
                    continue;
                }
                
                // Diagonal randomness switch
                if(get_cell(x-1, y+1) == empty && get_cell(x+1, y+1) == empty)
                {
                    if(toggle_switch())
                    {
                        swap_cells(x, y, x-1, y+1);
                        continue;
                    }
                    else
                    {
                        swap_cells(x, y, x+1, y+1);
                        continue;
                    }
                }
    
                // Diagonal left
                if(get_cell(x-1, y+1) == empty)
                {
                    swap_cells(x, y, x-1, y+1);
                    continue;
                }
    
                // Diagonal right
                if(get_cell(x+1, y+1) == empty)
                {
                    swap_cells(x, y, x+1, y+1);
                    continue;
                }
    
                // Horizontal randomness switch
                if(get_cell(x-1, y) == empty && get_cell(x+1, y) == empty)
                {
                    if(toggle_switch())
                    {
                        swap_cells(x, y, x-1, y);
                        continue;
                    }
                    else
                    {
                        swap_cells(x, y, x+1, y);
                        continue;
                    }
                }
    
                // Horizontal left
                if(get_cell(x-1, y) == empty)
                {
                    swap_cells(x, y, x-1, y);
                    continue;
                }
    
                // Horizontal right
                if(get_cell(x+1, y) == empty)
                {
                    swap_cells(x, y, x+1, y);
                    continue;
                }
            }
        }

    wake_up_adjacents(i, j);
}

void grid_update()
{
    for(int i = CVERT-1; i >= 0; --i)
    for(int j = 0; j < CHORZ; ++j)
    {
        if(dead_chunk_ticks[i][j] < 5) update_chunk(i, j);
    }
}
