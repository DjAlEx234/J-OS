#include <stddef.h>
#include "inoutb.h"
#include "text.h"
#define PORT 0x3F8
int serial = 0;
int serial_enabled()
{
    return serial;
}
void serial_enable()
{
   outb(PORT + 1, 0x00);
   outb(PORT + 3, 0x80);
   outb(PORT + 0, 0x03);
   outb(PORT + 1, 0x00);
   outb(PORT + 3, 0x03);
   outb(PORT + 2, 0xC7);
   //outb(PORT + 4, 0x0B); re-enable interrupts maybe so no
}
int serial_transmit_empty()
{
    return inb(PORT + 5) & 0x20;
}
void serial_outc(char c)
{
    while (serial_transmit_empty() == 0);
    outb(PORT, c);
}
void serial_outs(char *s)
{
    for (int i = 0; i < string_len(s); i++)
        serial_outc(s[i]);
}