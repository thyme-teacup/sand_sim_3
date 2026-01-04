#include "raylib.h"
#include "display.h"
#include "grid.h"
#include "user.h"
#include <stdio.h>

void set_colors();
void show_diagnostics();

int main()
{
    display_spawn_window(WID, HEI, "Sandsim", (void*)grid);

    set_colors();

    for(int i = 10; i < 15; ++i)
        for(int j = 10; j < 15; ++j)
        {
            for(int e = 0; e < CHUNK_SIZE; ++e)
            for(int f = 0; f < CHUNK_SIZE; ++f)
            {
                grid[i][j][e][f] = sand;
            }
        }

    
    for(int i = 16; i < 20; ++i)
        for(int j = 10; j < 16; ++j)
        {
            for(int e = 0; e < CHUNK_SIZE; ++e)
            for(int f = 0; f < CHUNK_SIZE; ++f)
            {
                grid[i][j][e][f] = water;
            }
        }

    brush_init();


    while(!WindowShouldClose())
    {
        BeginDrawing();

        display_draw_window(RENDER_CHUNKED, CHUNK_SIZE);

        grid_update();

        user_ctrl();

        if(IsKeyDown(KEY_F3)) show_diagnostics();

        EndDrawing();
    }

    display_kill_window();

    return 0;
}

void set_colors()
{
    display_set_color(empty, BROWN);
    display_set_color(water, BLUE);
    display_set_color(sand,  YELLOW);
    display_set_color(stone, GRAY);
}

void show_diagnostics()
{
    printf("FPS: %d\t|\t", GetFPS());

    uint32_t lcc = 0;
    for(int i = 0; i < CVERT; ++i)
        for(int j = 0; j < CHORZ; ++j)
        {
            if(dead_chunk_ticks[i][j] == 0)
            {
                ++lcc;
            }
        }

    printf("LCC: %d\n", lcc);
}
