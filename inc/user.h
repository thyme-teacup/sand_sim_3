#ifndef USER_H_
#define USER_H_

#include "raylib.h"
#include "grid.h"
#include <stdint.h>
#include <stdlib.h>

extern struct Brush
{
    uint8_t size;
    uint8_t value;

} brush;

extern void brush_init();
extern void user_ctrl();

#endif // USER_H_
