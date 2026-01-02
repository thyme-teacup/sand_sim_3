#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "raylib.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace display
{

extern uint32_t width, height;
extern Color    color_table[256];
extern uint8_t  *screen_buffer;

extern void spawn_window(uint32_t w, uint32_t h, const char* title);
extern void draw_window();
extern void kill_window();

}

#endif // DISPLAY_H_
