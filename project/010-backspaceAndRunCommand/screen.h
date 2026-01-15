#ifndef SCREEN_H
#define SCREEN_H
#include "types.h"

void clear_screen();
void kprint(const char *str);
void kprint_highlight(const char *str);
void kprint_int(int n);
void put_char(char c, char color);
void update_cursor(int x, int y);

#endif