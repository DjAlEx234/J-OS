#include <stdint.h>
#include "power.h"
#include "text.h"
#include "vga.h"
int vga = 0;
void cmd_string(char* send)
{
    if (vga == 1)
        vga_prints(send, 10);
    else
        text_prints(send);
}
void cmd_vga()
{
    vga = vga_enabled();
    cmd_string("J-OS>");
}
void cmd_run(char* cmd)
{
    if (text_cmp(cmd, "reboot"))
        reboot();
    cmd_string("\nCommand \"");
    cmd_string(cmd);
    cmd_string("\" not found.\n");
    cmd_string("J-OS>");
}