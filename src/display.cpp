#include "display.h"

using namespace display;

static Texture2D scr_buf_tx;
static Image     scr_buf_img;

// Functions
void spawn_window(uint32_t w, uint32_t h, const char* title)
{
    InitWindow(w, h, title);
    scr_buf_img = GenImageColor(width, height, (Color){0, 0, 0, 255});
    scr_buf_tx  = LoadTextureFromImage(scr_buf_img);

    memset(color_table, 0, sizeof(Color)*256);
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
    CloseWindow();
    UnloadTexture(scr_buf_tx);
    UnloadImage(scr_buf_img);
}
