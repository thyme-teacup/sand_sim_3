#include "raylib.h"
#include "display.h"
#include "grid.h"

#define WID 1024
#define HEI 512

int main()
{
    display::spawn_window(WID, HEI, "Sand-and-Water Simulator");
    grid::init(WID, HEI, (grid::tile*)display::screen_buffer, display::color_table);

    grid::generate();

    while(!WindowShouldClose())
    {
        display::draw_window(display::RENDER_CHUNKED, grid::chunk_size);
    }

    display::kill_window();

    return 0;
}
