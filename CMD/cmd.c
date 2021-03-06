#include <stdint.h>
#include "interface.h"
#include "serial.h"
#include "power.h"
#include "text.h"
#include "vga.h"
int vga = 0;
static char buffer[10][20];
void cmd_flush()
{
    buffer[0][0] = 0;
    buffer[1][0] = 0;
    buffer[2][0] = 0;
    buffer[3][0] = 0;
    buffer[4][0] = 0;
    buffer[5][0] = 0;
    buffer[6][0] = 0;
    buffer[7][0] = 0;
    buffer[8][0] = 0;
    buffer[9][0] = 0;
}
void (*cmd_string)(char s[], int c);
int col = 0;
void cmd_handle(void *handle, int co)
{
    cmd_flush();
    cmd_string = handle;
    col = co;
    cmd_string("J-OS>", co);
}
int cmd_cmp(char a[], char b[])
{
    int len = string_len(b) + 1;
#ifdef DEBUG
    serial_outc('\n');
    serial_outs(a);
    serial_outc('\n');
    serial_outs(b);
#endif
    for (int c = 0; c != len; c++)
        if (a[c] != b[c])
            return 0;
    return 1;
}
void cmd_parse(char cmd[])
{
    int d = 0, r = 0, c = 0;
    d = string_len(cmd);
    for (int a = 0; a <= d; a++)
    {
        if (cmd[a] == ' ' || cmd[a] == '\n')
        {
            buffer[r][c] = 0;
            r++;
            c = 0;
        }
        else
        {
            buffer[r][c] = cmd[a];
            c++;
        }
        if (r == 11)
            break;
    }
}
void cmd_run(char* cmd)
{
    cmd_parse(cmd);
#ifdef DEBUG
    serial_outc('\n');
    serial_outs(cmd);
#endif
    cmd_string("\n", col);
    if (cmd_cmp(buffer[0], "reboot\0"))
    {
#ifdef DEBUG
        serial_outc('\n');
        serial_outs("Rebooting!");
#endif
        reboot();
    }
    else if (cmd_cmp(buffer[0], "echo\0"))
    {

    }
    else if (cmd_cmp(buffer[0], "list\0"))
    {
        cmd_string("Commands:\n", col);
        cmd_string("Echo - Sends your message\n", col);
        cmd_string("List - Lists commands\n", col);
        cmd_string("Reboot - Reboots PC\n", col);
        cmd_string("UI - Opens graphical interface", col);
    }
    else if (cmd_cmp(buffer[0], "ui\0"))
        ui_init();
    else
    {
        cmd_string("Command \"", col);
        cmd_string(buffer[0], col);
        cmd_string("\" not found. Use \"list\" for help.", col);
    }
    cmd_string("\nJ-OS>", col);
    cmd_flush();
}