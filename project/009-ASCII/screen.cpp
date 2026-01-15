#include "screen.h"

const int VIDEO_WIDTH = 80;
const int VIDEO_HEIGHT = 25;
const char AMI_BIOS_COLOR = 0x17;

int cursor_x = 0;
int cursor_y = 0;

void update_cursor(int x, int y) {
    uint16_t pos = y * VIDEO_WIDTH + x;
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
}

void scroll() {
    char *video_memory = (char *)0xB8000;
    for (int i = 0; i < (VIDEO_HEIGHT - 1) * VIDEO_WIDTH * 2; i++) {
        video_memory[i] = video_memory[i + VIDEO_WIDTH * 2];
    }
    for (int i = (VIDEO_HEIGHT - 1) * VIDEO_WIDTH * 2; i < VIDEO_HEIGHT * VIDEO_WIDTH * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = AMI_BIOS_COLOR;
    }
    cursor_y = VIDEO_HEIGHT - 1;
}

void put_char(char c, char color) {
    char *video_memory = (char *)0xB8000;
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        int offset = (cursor_y * VIDEO_WIDTH + cursor_x) * 2;
        video_memory[offset] = c;
        video_memory[offset + 1] = color;
        cursor_x++;
    }
    if (cursor_x >= VIDEO_WIDTH) { cursor_x = 0; cursor_y++; }
    if (cursor_y >= VIDEO_HEIGHT) scroll();
    update_cursor(cursor_x, cursor_y);
}

void kprint(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) put_char(str[i], AMI_BIOS_COLOR);
}

void kprint_highlight(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) put_char(str[i], 0x70);
}

void clear_screen() {
    char *video_memory = (char *)0xB8000;
    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = AMI_BIOS_COLOR;
    }
    cursor_x = 0; cursor_y = 0;
    update_cursor(0, 0);
}