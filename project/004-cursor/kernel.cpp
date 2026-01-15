// 屏幕常量
const int VIDEO_WIDTH = 80;
const int VIDEO_HEIGHT = 25;
const char AMI_BIOS_COLOR = 0x17; 

int cursor_x = 0;
int cursor_y = 0;

// --- 1. 先定义底层硬件操作函数 ---

// 向 I/O 端口写入一个字节
void outb(unsigned short port, unsigned char val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// 更新硬件光标位置
void update_cursor(int x, int y) {
    unsigned short pos = y * VIDEO_WIDTH + x;
    outb(0x3D4, 0x0E); // 寄存器 0x0E: 光标位置高8位
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
    outb(0x3D4, 0x0F); // 寄存器 0x0F: 光标位置低8位
    outb(0x3D5, (unsigned char)(pos & 0xFF));
}

// --- 2. 再定义高层显示函数 ---

void clear_screen() {
    char *video_memory = (char *)0xB8000;
    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = AMI_BIOS_COLOR;
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor(cursor_x, cursor_y);
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

    if (cursor_x >= VIDEO_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    // 现在编译器认识 update_cursor 了
    update_cursor(cursor_x, cursor_y);
}

void kprint(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        put_char(str[i], AMI_BIOS_COLOR);
    }
}

void kprint_highlight(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        put_char(str[i], 0x70); 
    }
}

// --- 3. 最后是内核入口 ---

extern "C" void kmain() {
    clear_screen();
    
    kprint(" STRATA OS Revision 1.0.4 \n");
    kprint(" System Core: Sentry Kernel Active \n");
    kprint("-----------------------------------\n");
    kprint(" Hardware Cursor... [Enabled]\n");
    kprint("\n\n");
    
    kprint_highlight(" SYSTEM READY. ");
    
    while (1);
}