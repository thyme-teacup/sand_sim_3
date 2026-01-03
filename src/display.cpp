#include "display.h"

namespace display
{

// Definitions
uint32_t width, height;
Color    color_table[256];
uint8_t  *screen_buffer;
const uint8_t  RENDER_CHUNKED = 0x00000001;

// New variables
static Texture2D scr_buf_tx;
static Image     scr_buf_img;
static bool      alloc_flag;

// Functions
void spawn_window(uint32_t w, uint32_t h, const char* title, void *buf)
{
    width = w;
    height = h;

    InitWindow(width, height, title);
    scr_buf_img = GenImageColor(width, height, (Color){0, 0, 0, 255});
    scr_buf_tx  = LoadTextureFromImage(scr_buf_img);

    if(buf == 0)
    {
        alloc_flag = true;
        screen_buffer = (uint8_t*)malloc(sizeof(uint8_t)*width*height);
    }
    else
    {
        alloc_flag = false;
        screen_buffer = (uint8_t*)buf;
    }

    memset(color_table, 0, sizeof(Color)*256);
    memset(screen_buffer, 0, sizeof(uint8_t)*width*height); // For if the draw_window() called before the values
                                                            // in screen_buffer are set.
}

void draw_window(uint8_t render_flag, const int32_t chunk_size)
{
    if(!(render_flag&RENDER_CHUNKED))
    {
        for(int i = 0; i < height; ++i)
        for(int j = 0; j < width;  ++j)
        {
            ImageDrawPixel(&scr_buf_img, j, i, color_table[screen_buffer[i*width+j]]);
        }
    }
    else // Read chunked
    {
        assert(chunk_size > 0);

        uint32_t chunk_pos;
        uint32_t offset;

        for(int i = 0; i < height/chunk_size; ++i)
        for(int j = 0; j < width/chunk_size;  ++j)
        {
            chunk_pos = (i*width/chunk_size) + j;
            offset = chunk_pos*chunk_size*chunk_size;
        
            for(int e = 0; e < chunk_size; ++e)
            for(int f = 0; f < chunk_size; ++f)
            {
                ImageDrawPixel(&scr_buf_img, j*chunk_size+f, i*chunk_size+e,
                               color_table[screen_buffer[offset + e*chunk_size + f]]);
            }
        }
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

    if(alloc_flag == true) free(screen_buffer);
}

bool should_quit()
{
    return WindowShouldClose();
}

}
