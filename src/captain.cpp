#include "raylib.h"
#include "display.h"

#define WID 1024
#define HEI 512

int main()
{
    display::spawn_window(WID, HEI, "Sand-and-Water Simulator");

    while(!WindowShouldClose())
    {
        display::draw_window();
    }

    display::kill_window();

    return 0;
}
