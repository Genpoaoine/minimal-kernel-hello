#include "types.h"
#include "screen.h"
#include "gdt_idt.h"

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