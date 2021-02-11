#include <stddef.h>
#include "keyboard.h"
#include "text.h"
#include "vga.h"
void (*printc)(char c);
void terminal_prints(char* str)
{
    for (int i = 0; i < string_len(str); i++)
        printc(str[i]);
}
void terminal_keybd(char c)
{
    printc(keyboard_getkey(c));
}
void terminal_init()
{
    if (vga_enabled())
    {
        printc = *vga_printc;
        vga_prints("", 10);
    }
    else
        printc = *text_printc;
    keyboard_set(terminal_keybd);
}