#include "display.h"

namespace display
{

// Definitions
uint32_t width, height;
Color    color_table[256];
uint8_t  *screen_buffer;

// New variables
static Texture2D scr_buf_tx;
static Image     scr_buf_img;

// Functions
void spawn_window(uint32_t w, uint32_t h, const char* title)
{
    width = w;
    height = h;

    InitWindow(width, height, title);
    scr_buf_img = GenImageColor(width, height, (Color){0, 0, 0, 255});
    scr_buf_tx  = LoadTextureFromImage(scr_buf_img);

    screen_buffer = (uint8_t*)malloc(sizeof(uint8_t)*width*height);

    memset(color_table, 0, sizeof(Color)*256);
    memset(screen_buffer, 0, sizeof(uint8_t)*width*height);
}

void draw_window()
{
    for(int i = 0; i < height; ++i)
        for(int j = 0; j < width;  ++j)
        {
            ImageDrawPixel(&scr_buf_img, j, i, color_table[screen_buffer[i*width+j]]);
        }

    UpdateTexture(scr_buf_tx, scr_buf_img.data);

    BeginDrawing();
    DrawTexture(scr_buf_tx, 0, 0, WHITE);
    EndDrawing();
}

void kill_window()
{
    UnloadTexture(scr_buf_tx);
    UnloadImage(scr_buf_img);
    CloseWindow();

    free(screen_buffer);
}

}
