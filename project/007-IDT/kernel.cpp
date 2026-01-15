#include "gdt.h"
#include "types.h"
// 屏幕常量
const int VIDEO_WIDTH = 80;
const int VIDEO_HEIGHT = 25;
const char AMI_BIOS_COLOR = 0x17; 

int cursor_x = 0;
int cursor_y = 0;

// --- 1. 先定义全局变量 ---
// 这样下面的函数才能看到这两个变量
gdt_entry_struct gdt_entries[3];
gdt_ptr_struct   gdt_ptr;

// --- 2. 然后定义使用这些变量的函数 ---

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = ((limit >> 16) & 0x0F);

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}

void init_gdt() {
    gdt_ptr.limit = (sizeof(gdt_entry_struct) * 3) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data
}

// IDT
extern "C" void divide_by_zero_handler(); 
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags   = flags; 
}

void init_idt() {
    idt_ptr.limit = sizeof(idt_entry_struct) * 256 - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    // 清空 IDT，防止乱码
    for(int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // 现在编译器认识它了
    idt_set_gate(0, (uint32_t)divide_by_zero_handler, 0x08, 0x8E);

    asm volatile("lidt (%0)" : : "r" (&idt_ptr));
}

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

// 滚屏功能
void scroll() {
    char *video_memory = (char *)0xB8000;

    // 1. 将第 1-24 行的内容向上移动一行 (每一行 80*2 个字节)
    for (int i = 0; i < (VIDEO_HEIGHT - 1) * VIDEO_WIDTH * 2; i++) {
        video_memory[i] = video_memory[i + VIDEO_WIDTH * 2];
    }

    // 2. 最后一行填充空格
    for (int i = (VIDEO_HEIGHT - 1) * VIDEO_WIDTH * 2; i < VIDEO_HEIGHT * VIDEO_WIDTH * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = AMI_BIOS_COLOR;
    }

    // 3. 光标留在最后一行行首
    cursor_y = VIDEO_HEIGHT - 1;
}

// 修改后的 put_char
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

    // 如果水平溢出，自动换行
    if (cursor_x >= VIDEO_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    // 如果垂直溢出，触发滚屏
    if (cursor_y >= VIDEO_HEIGHT) {
        scroll();
    }

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

// 定义int
void kprint_int(int n) {
    if (n == 0) {
        put_char('0', AMI_BIOS_COLOR);
        return;
    }

    char buf[12]; // 足够存放 32 位整数
    int i = 0;
    
    // 提取每一位数字
    while (n > 0) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }

    // 反转并打印
    for (int j = i - 1; j >= 0; j--) {
        put_char(buf[j], AMI_BIOS_COLOR);
    }
}

// --- 3. 最后是内核入口 ---

extern "C" void gdt_flush(uint32_t);

extern "C" void divide_by_zero_handler() {
    // 0x4F 是红底白字
    kprint_highlight(" !!! FATAL ERROR: DIVISION BY ZERO !!! ");
    while(1); // 停在这里，不让系统崩溃重启
}

extern "C" void kmain() {
    init_gdt();
    gdt_flush((uint32_t)&gdt_ptr);
    init_idt();

    clear_screen();
    kprint("IDT Loaded. Testing exception...\n");

    // 加入 volatile 防止优化
    volatile int a = 10;
    volatile int b = 0;
    volatile int c = a / b; 

    // 为了确保万无一失，可以强行打印一下 c (虽然跑不到这里)
    if (c == 42) kprint("Lucky number");

    kprint("This line will never be printed.");
    while(1);
}