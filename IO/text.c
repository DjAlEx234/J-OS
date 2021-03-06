#include <stdint.h>
#include <stddef.h>
#include "text.h"
uint16_t* video_memory;
int row, column;
static const int width = 80;
static const int height = 25;
int text_get_row()
{
    return row;
}
int text_get_column()
{
    return column;
}
void text_setpos(int new_row, int new_column)
{
    row = new_row;
    column = new_column;
}
void text_scroll()
{
    for (int a = 0; a < height; a++)
        for (int b = 0; b < width; b++)
            video_memory[a * width + b] = video_memory[(a + 1) * width + b];
}
uint16_t text_getchar(unsigned char c, uint8_t color)
{
    return (uint16_t) c | (uint16_t) color << 8;
}
uint8_t text_getcolor(enum colors fg, enum colors bg)
{
    return fg | bg << 4;
}
void text_clear(enum colors color)
{
    for (int a = 0; a < height; a++)
        for (int b = 0; b < width; b++)
            video_memory[a * width + b] = text_getchar('\0', text_getcolor(0, color));
    row = 0;
    column = 0;
}
enum colors fg, bg;
void text_setfgbg(int new_fg, int new_bg)
{
    fg = new_fg;
    bg = new_bg;
}
void text_printc(char c)
{
    if (c == '\0')
        return;
    if (c == '\b')
    {
        column--;
        video_memory[row * width + column] = text_getchar(0, text_getcolor(fg, bg));
        return;
    }
    else if (c == '\n')
    {
        int space = 80 - column;
        for (int i = 0; i != space; i++)
           text_printc(' ');
        return;
    }
    else
        video_memory[row * width + column] = text_getchar(c, text_getcolor(fg, bg));
    if (++column == width)
    {
        column = 0;
        if (++row == height)
        {
            text_scroll();
            row = 24;
        }
    }
}
void text_prints(char* s, enum colors new_fg)
{
    fg = new_fg;
    for (int i = 0; i < string_len(s); i++)
        text_printc(s[i]);
}
void text_init(void)
{
    video_memory = (uint16_t*)0xB8000;
    text_clear(0);
}