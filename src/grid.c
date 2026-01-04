#include "grid.h"

tile grid[CVERT][CHORZ][CHUNK_SIZE][CHUNK_SIZE] __attribute__((aligned(64)));

void set_cell(uint32_t x, uint32_t y, tile value)
{
    assert(x >= 0 && x < WID && y >= 0 && y < HEI);
    assert(value < TILE_MAX && value >= 0);

    uint32_t i, j, e, f;

    i = y;
    j = x;
    uint32_t tmp = 1;
    while(tmp != CHUNK_SIZE)
    {
        tmp <<= 1;
        i >>= 1;
        j >>= 1;
    }

    e = y&(CHUNK_SIZE-1);
    f = x&(CHUNK_SIZE-1);

    grid[i][j][e][f] = value;

    atomic_store(&(dead_chunk_ticks[i][j]), 0);
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
    
    atomic_store(&(dead_chunk_ticks[i1][j1]), 0);
    atomic_store(&(dead_chunk_ticks[i2][j2]), 0);
}

static void wake_up_adjacents(uint32_t i, uint32_t j)
{
    if(atomic_load(&(dead_chunk_ticks[i][j])) != 0) return;

    // Upwards
    for(int f = 0; f < CHUNK_SIZE; ++f)
        if(grid[i][j][0][f] == empty || grid[i][j][0][f] == water)
        {
            if(i!=0) atomic_store(&(dead_chunk_ticks[i-1][j]), 0);
            break;
        }

    // Leftwards
    for(int e = 0; e < CHUNK_SIZE; ++e)
        if(grid[i][j][e][0] == empty || grid[i][j][e][0] == water)
        {
            if(j!=0) atomic_store(&(dead_chunk_ticks[i][j-1]), 0);
            break;
        }

    // Rightwards
    for(int e = 0; e < CHUNK_SIZE; ++e)
        if(grid[i][j][e][CHUNK_SIZE-1] == empty || grid[i][j][e][CHUNK_SIZE-1] == water)
        {
            if(j!=CHORZ-1) atomic_store(&(dead_chunk_ticks[i][j+1]), 0);
            break;
        }

    // Left corner
    if(grid[i][j][0][0] == empty || grid[i][j][0][0] == water)
        if(i!=0 && j!=0) atomic_store(&(dead_chunk_ticks[i-1][j-1]), 0);

    // Right corner
    if(grid[i][j][0][CHUNK_SIZE-1] == empty || grid[i][j][0][CHUNK_SIZE-1] == water)
        if(i!=0 && j!=CHORZ-1) atomic_store(&(dead_chunk_ticks[i-1][j+1]), 0);
}

static void update_chunk(uint32_t i, uint32_t j)
{
    atomic_store(&(dead_chunk_ticks[i][j]), atomic_load(&(dead_chunk_ticks[i][j]))+1);

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
                    if(y&1)
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
                    if(y&1)
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
                    if(y&1)
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

/* MULTITHREADING PART */

// Threads stuff
static atomic_bool cooked[CVERT][CHORZ]; // Whether chunk has been already processed this frame
static atomic_bool kill_switch = ATOMIC_VAR_INIT(false);
static atomic_bool frame_launch[THREAD_COUNT]; // Start signal for each frame
static pthread_t   threads[THREAD_COUNT];
static atomic_bool taken[CVERT][CHORZ]; // Whether the chunk is being currently processed
atomic_int dead_chunk_ticks[CVERT][CHORZ];

static bool seize_chunk(int32_t i, int32_t j)
{
    if(i < 0 || i >= CVERT || j < 0 || j >= CHORZ) return true;

    if(atomic_load(&(taken[i][j])) == true) return false;

    atomic_store(&(taken[i][j]), true);
    return true;
}

static void release_chunk(int32_t i, int32_t j)
{
    if(i < 0 || i >= CVERT || j < 0 || j >= CHORZ) return;

    atomic_store(&(taken[i][j]), false);
}

static void* worker(void *arg)
{
    uint32_t id = *((int*)arg);
    uint32_t l, r;

    while(atomic_load(&kill_switch) == false)
    {
        while(atomic_load(&(frame_launch[id])) != true); // We wait

        // We started
        l = id*CHORZ;
        r = (id+1)*CHORZ - 1;

        for(int i = CVERT-1; i >= 0; --i)
        for(int j = l; j <= r; ++j)
        {
            // We need the chunk to the left of us to be cooked
            while(true) {if(j == 0 || atomic_load(&(cooked[i][j-1])) == true) break;}

            // We need to claim 3x3 area of chunks around us
            while(true) if(seize_chunk(i-1, j-1)) break;
            while(true) if(seize_chunk(i-1, j)) break;
            while(true) if(seize_chunk(i-1, j+1)) break;
            while(true) if(seize_chunk(i,   j-1)) break;
            while(true) if(seize_chunk(i,   j)) break;
            while(true) if(seize_chunk(i,   j+1)) break;
            while(true) if(seize_chunk(i+1, j-1)) break;
            while(true) if(seize_chunk(i+1, j)) break;
            while(true) if(seize_chunk(i+1, j+1)) break;

            // Now we can work in peace
            if(atomic_load(&(dead_chunk_ticks[i][j])) < 5)
            {
                update_chunk(i, j);
            }
            atomic_store(&(cooked[i][j]), true);

            // We can let go of the chunks now
            release_chunk(i-1, j-1);
            release_chunk(i-1, j);
            release_chunk(i-1, j+1);
            release_chunk(i,   j-1);
            release_chunk(i,   j);
            release_chunk(i,   j+1);
            release_chunk(i+1, j-1);
            release_chunk(i+1, j);
            release_chunk(i+1, j+1);
        }

        atomic_store(&(frame_launch[id]), false);
    }

    return 0;
}

void grid_jumpstart()
{
    static uint32_t id[THREAD_COUNT];

    for(int i = 0; i < CVERT; ++i)
        for(int j = 0; j < CHORZ; ++j)
        {
            atomic_store(&(dead_chunk_ticks[i][j]), 0);
            atomic_store(&(taken[i][j]), false);
        }

    for(int i = 0; i < THREAD_COUNT; ++i)
    {
        id[i] = i;
        pthread_create(&threads[i], NULL, worker, &(id[i]));
    }

    for(int i = 0; i < THREAD_COUNT; ++i)
    {
        atomic_store(&(frame_launch[i]), true);
    }
}

void grid_update()
{
    // Clean the flags for our boys
    for(int i = CVERT-1; i >= 0; --i)
        for(int j = 0; j < CHORZ; ++j)
        {
            atomic_store(&(cooked[i][j]), false);
            atomic_store(&(taken[i][j]), false);
        }

    // Ready... Start!
    for(int i = 0; i < THREAD_COUNT; ++i)
    {
        atomic_store(&(frame_launch[i]), true);
    }

    // Now we're waiting for our boys to finish...
    while(true)
    {
        for(int i = 0; i < THREAD_COUNT; ++i)
        {
            if(atomic_load(&(frame_launch[i])) == true) // The thread hasn't finished its work yet
                goto nope;
        }

        break;

        nope:
        continue;
    }
}

void grid_kill()
{
    atomic_store(&kill_switch, true);

    for(int i = 0; i < THREAD_COUNT; ++i)
        pthread_join(threads[i], NULL);
}
