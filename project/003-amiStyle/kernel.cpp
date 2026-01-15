// 屏幕常量
const int VIDEO_WIDTH = 80;
const int VIDEO_HEIGHT = 25;

// AMI BIOS 配色：0x17 (1=蓝色背景, 7=灰色/白色文字)
// 如果想要更亮的白色，可以用 0x1F
const char AMI_BIOS_COLOR = 0x17; 

int cursor_x = 0;
int cursor_y = 0;

void clear_screen() {
    char *video_memory = (char *)0xB8000;
    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++) {
        video_memory[i * 2] = ' ';              // 用空格填充
        video_memory[i * 2 + 1] = AMI_BIOS_COLOR; // 背景涂蓝
    }
    cursor_x = 0;
    cursor_y = 0;
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

    // 简单的滚屏逻辑：如果到底了，直接清屏回到顶部
    if (cursor_y >= VIDEO_HEIGHT) {
        clear_screen();
    }
}

void kprint(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        put_char(str[i], AMI_BIOS_COLOR);
    }
}

// 专门用来打印高亮提示（比如黄底蓝字或者白底蓝字）
void kprint_highlight(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        put_char(str[i], 0x70); // 0x70 是灰底黑字，常用于 BIOS 底部的菜单栏
    }
}

extern "C" void kmain() {
    clear_screen();
    
    kprint(" American Megatrends (R) \n");
    kprint(" BIOS Revision 1.00.01 \n");
    kprint(" STRATA Kernel initialized successfully. \n");
    kprint("\n");
    kprint(" CPU  : i686 Family\n");
    kprint(" DRAM : 640 KB Base Memory\n");
    kprint("\n\n\n\n");
    
    kprint_highlight(" Press F2 to enter STRATA Setup, F12 for Boot Menu ");
    
    while (1);
}