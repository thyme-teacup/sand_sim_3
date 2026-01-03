#include "raylib.h"
#include "display.h"
#include "grid.h"
#include <stdio.h>

void set_colors();
void print_FPS();

int main()
{
    display::spawn_window(WID, HEI, "Sandsim", (void*)grid);

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

    while(!display::should_quit())
    {
        display::draw_window(display::RENDER_CHUNKED, CHUNK_SIZE);

        grid_update();

        if(IsKeyDown(KEY_F3)) print_FPS();
    }

    display::kill_window();

    return 0;
}

void set_colors()
{
    display::set_color(empty, BROWN);
    display::set_color(water, BLUE);
    display::set_color(sand,  YELLOW);
    display::set_color(stone, GRAY);
}

void print_FPS()
{
    printf("FPS: %d\n", GetFPS());
}
