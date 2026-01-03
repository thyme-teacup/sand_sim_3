#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "raylib.h"
#include "grid.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

extern uint32_t width, height;
extern uint8_t  *screen_buffer;

extern const uint8_t RENDER_CHUNKED;

extern void display_spawn_window(uint32_t w, uint32_t h, const char* title, void *buf);
// NOTE: spawn_window ASSUMES correct size of the buffer!
extern void display_draw_window(uint8_t render_flag, const int32_t chunk_size);
extern void display_kill_window();
extern void display_set_color(uint8_t val, Color color);

#endif // DISPLAY_H_
