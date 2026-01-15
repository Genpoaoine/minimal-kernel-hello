// 屏幕常量
const int VIDEO_WIDTH = 80;
const int VIDEO_HEIGHT = 25;
const char DEFAULT_COLOR = 0x07; // 灰底白字

// 记录当前光标位置
int cursor_x = 0;
int cursor_y = 0;

// 清屏函数
void clear_screen() {
    char *video_memory = (char *)0xB8000;
    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT * 2; i++) {
        video_memory[i] = 0;
    }
    cursor_x = 0;
    cursor_y = 0;
}

// 打印单个字符
void put_char(char c) {
    char *video_memory = (char *)0xB8000;

    // 处理换行符
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        // 计算显存偏移量：(y * 宽度 + x) * 2
        int offset = (cursor_y * VIDEO_WIDTH + cursor_x) * 2;
        video_memory[offset] = c;
        video_memory[offset + 1] = DEFAULT_COLOR;
        cursor_x++;
    }

    // 检查是否需要自动换行
    if (cursor_x >= VIDEO_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    // 如果到底了，暂时简单回到顶部（进阶版需要实现滚屏）
    if (cursor_y >= VIDEO_HEIGHT) {
        cursor_y = 0; 
    }
}

// 打印字符串
void kprint(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        put_char(str[i]);
    }
}

extern "C" void kmain() {
    clear_screen();
    
    kprint("STRATA OS Kernel Mode\n");
    kprint("---------------------\n");
    kprint("Initializing Video Driver... [OK]\n");
    kprint("Welcome to your own operating system!");
    
    while (1);
}