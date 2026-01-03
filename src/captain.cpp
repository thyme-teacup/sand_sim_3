#include "raylib.h"
#include "display.h"
#include "grid.h"

int main()
{
    display::spawn_window(WID, HEI, "Sandsim", (void*)grid);

    while(!display::should_quit())
    {
        display::draw_window(display::RENDER_CHUNKED, CHUNK_SIZE);
    }

    display::kill_window();

    return 0;
}
