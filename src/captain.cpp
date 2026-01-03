#include "raylib.h"
#include "display.h"
#include "grid.h"

void set_colors();

int main()
{
    display::spawn_window(WID, HEI, "Sandsim", (void*)grid);

    set_colors();

    while(!display::should_quit())
    {
        display::draw_window(display::RENDER_CHUNKED, CHUNK_SIZE);
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
