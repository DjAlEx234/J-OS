#include <stddef.h>
#include "keyboard.h"
#include "serial.h"
#include "text.h"
#include "vga.h"
#include "cmd.h"
void (*printc)(char c);
void terminal_prints(char* str)
{
    for (int i = 0; i < string_len(str); i++)
        printc(str[i]);
}
int terminal_bufptr = 0;
static char terminal_buffer[99];
void terminal_keybd(char in)
{
    char c = keyboard_getkey(in);
    if (c == '\n')
    {
        terminal_buffer[terminal_bufptr++] = '\0';
        terminal_bufptr = 0;
        cmd_run(terminal_buffer);
        return;
    }
    if (terminal_bufptr == 100)
        return;
    if (c == '\b')
    {
        if (terminal_bufptr == 0)
            return;
        printc('\b');
        terminal_bufptr--;
        terminal_buffer[terminal_bufptr] = 0;
        return;
    }
    terminal_buffer[terminal_bufptr] = c;
    printc(c);
    terminal_bufptr++;
#ifdef DEBUG
    char* test = 0;
    text_itoa(terminal_bufptr, test, 10);
    serial_outc('\n');
    serial_outs(test);
    serial_outc('\n');
    serial_outs(terminal_buffer);
#endif
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