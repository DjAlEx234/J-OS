#include "keyboard.h"
#include "terminal.h"
#include "inoutb.h"
#include "serial.h"
#include "text.h"
#include "int.h"
#include "vga.h"
#include "cmd.h"
char input = -1;
int todo = 0;
void bootopt(int i)
{
    input = i;
    if (i == 59)
        todo = 1;
    else if (i == 60)
        todo = 2;
}
static void bootloop()
{
    while (todo == 0)
        keyboard_set(bootopt);
    keyboard_set(0);
}
void entry(void)
{
    interrupt_install();
    outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
    text_init();
    keyboard_init();
#ifdef DEBUG
    text_prints("Enable serial debugging?\n", 10);
    text_prints("(Will only output COM1)\n", 10);
    text_prints("F1 - Yes\n", 10);
    text_prints("F2 - No\n", 10);
    bootloop();
    if (todo == 1)
    {
        serial_enable();
        serial_outs("\nWelcome to J-OS serial output");
    }
    todo = 0;
#endif
    text_clear(0);
    text_prints("Welcome to J-OS! Boot options below:\n", 10);
    text_prints("F1 - Boot into text mode\n", 10);
    text_prints("F2 - Boot into VGA mode\n", 10);
    bootloop();
    text_setfgbg(3, 0);
    if (todo == 1)
    {
        text_prints("\nLaunching text mode...\n\n", 10);
        cmd_handle(text_prints, 3);
    }
    else if (todo == 2)
    {
        vga_init();
        cmd_handle(vga_prints, 3);
    }
    keyboard_set(0);
    terminal_init();
    while (1);
}