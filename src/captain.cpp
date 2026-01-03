#include "raylib.h"
#include "display.h"
#include "grid.h"

void set_colors();

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

    while(!display::should_quit())
    {
        display::draw_window(display::RENDER_CHUNKED, CHUNK_SIZE);

        grid_update();
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
