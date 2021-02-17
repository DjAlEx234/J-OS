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
    else if (text_cmp(cmd, "list"))
    {
        cmd_string("\nCommands:\n");
        cmd_string("List - Lists commands\n");
        cmd_string("Reboot - Reboots PC");
    }
    else
    {
        cmd_string("\nCommand \"");
        cmd_string(cmd);
        cmd_string("\" not found. Use \"List\" for help.");
    }
    cmd_string("\nJ-OS>");
}