#ifndef vgah
#define vgah
void vga_init();
void vga_printc(char c);
void vga_prints(char* str, int c);
void vga_error(int i, char* c);
int vga_enabled();
#endif