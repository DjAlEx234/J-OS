#ifndef stringh
#define stringh
void string_split(char* in, char* first, char* second, char split);
void string_itoa(int conv, char text[], int base);
int string_len(const char* string);
int string_cmp(char a[], char b[]);
void string_reverse(char text[]);
#endif