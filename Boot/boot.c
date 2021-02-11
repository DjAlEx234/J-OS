#include "keyboard.h"
#include "terminal.h"
#include "inoutb.h"
#include "text.h"
#include "int.h"
#include "vga.h"
#include "cmd.h"
char input = -1;
int todo = 0;
void bootopt(int i)
{
    input = i;
}
void bootloop()
{
    while (input == -1){keyboard_set(bootopt);};
    switch (input)
        {
            case 59:
                todo = 1;
                return;

            case 60:
                todo = 2;
                return;
    
            default:
                input = -1;
                bootloop();
                break;
    }
}
void entry(void)
{
    interrupt_install();
    outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
    text_init();
    text_setfgbg(3, 0);
    text_prints("Welcome to J-OS! Boot options below:\n");
    text_prints("F1 - Boot into text mode\n");
    text_prints("F2 - Boot into VGA mode\n");
    keyboard_init();
    bootloop();
    if (todo == 1)
        text_prints("\nLaunching text mode...\n");
    else if (todo == 2)
        vga_init();
    cmd_vga();
    keyboard_set(0);
    terminal_init();
    while (1);
}