#include "types.h"
#include "screen.h"
#include "gdt_idt.h"

char key_buffer[256];
int buffer_idx = 0;

// 简单的字符串比较函数（因为我们没有标准库）
bool str_equal(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] == s2[i]) {
        if (s1[i] == '\0') return true;
        i++;
    }
    return false;
}

void execute_command() {
    key_buffer[buffer_idx] = '\0'; // 结束符
    kprint("\n");

    if (str_equal(key_buffer, "hi")) {
        kprint("Hello, user! Welcome to STRATA OS.\n");
    } 
    else if (str_equal(key_buffer, "clear")) {
        clear_screen();
    }
    else if (str_equal(key_buffer, "help")) {
        kprint("Available: hi, clear, help\n");
    }
    else if (buffer_idx > 0) {
        kprint("Unknown command: ");
        kprint(key_buffer);
        kprint("\n");
    }

    kprint("root@strata:/# ");
    buffer_idx = 0; // 重置缓冲区
}
extern "C" void kmain() {
    init_gdt();
    init_idt();

    clear_screen();
    kprint("American Megatrends (R) - STRATA OS\n");
    kprint("Core 0: Sentry Kernel Ready.\n");
    kprint("-----------------------------------\n");
    kprint("root@strata:/# "); // 模拟一个命令行提示符

    while(1) {
        // 内核现在就停在这里，所有的工作都交给“中断”去驱动
        // 这种模式叫“中断驱动编程”
    }
}