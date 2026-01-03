#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "raylib.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

namespace display
{

extern uint32_t width, height;
extern Color    color_table[256];
extern uint8_t  *screen_buffer;

extern const uint8_t RENDER_CHUNKED;

extern void spawn_window(uint32_t w, uint32_t h, const char* title, void *buf = 0);
// NOTE: spawn_window ASSUMES correct size of the buffer!
extern void draw_window(uint8_t render_flag = 0, const int32_t chunk_size = -1);
extern void kill_window();
extern bool should_quit();

}

#endif // DISPLAY_H_
