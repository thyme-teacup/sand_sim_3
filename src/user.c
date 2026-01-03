#include "user.h"

struct Brush brush;

static void brush_resize(uint8_t d) // Yes, overflow is intentional
{
    brush.size += d;
}

static void brush_set(uint8_t newset)
{
    assert(newset < TILE_MAX && newset >= 0);
    brush.value = newset;
}

static void brush_use(uint32_t x, uint32_t y)
{
    uint32_t l, r, t, b;

    l = ((int32_t)x-brush.size < 0)  ? 0        : x-brush.size;
    r = (x+brush.size >= WID)        ? WID - 1  : x+brush.size;
    t = ((int32_t)y-brush.size < 0)  ? 0        : y-brush.size;
    b = (y+brush.size >= HEI)        ? HEI - 1  : y+brush.size;

    uint32_t sq_dist;
    for(int i = t; i <= b; ++i)
    for(int j = l; j <= r; ++j)
    {
        sq_dist = (x-j)*(x-j) + (y-i)*(y-i);

        if(sq_dist > brush.size*brush.size) continue;

        if(rand()%100 > (float)sq_dist/(brush.size*brush.size)*100)
            set_cell(j, i, brush.value);
    }
}

void user_ctrl()
{
    brush_resize(GetMouseWheelMove());

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        brush_use(GetMouseX(), GetMouseY());

    if(IsKeyPressed(KEY_SPACE))
    {
        switch(brush.value)
        {
            case sand:
                brush_set(water);
            break;

            case water:
                brush_set(sand);
            break;
        }
    }
}
