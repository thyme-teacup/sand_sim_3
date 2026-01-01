#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "raylib.h"
#include <stdint.h>

namespace display
{

extern uint32_t width, height;
extern uint32_t color_table[256];
extern uint8_t  **screen_buffer;

void spawn_window(uint32_t w, uint32_t h, const char* title);
void draw_window();
void kill_window();

}

#endif // DISPLAY_H_
