#include "gdt_idt.h"
#include "screen.h"

gdt_entry_struct gdt_entries[3];
gdt_ptr_struct   gdt_ptr;
idt_entry_struct idt_entries[256];
idt_ptr_struct   idt_ptr;

extern char key_buffer[];
extern int buffer_idx;
extern void execute_command();

unsigned char kbd_us[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',   
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,      
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt_entries[num].access = access;
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}

void pic_remap() {
    outb(0x20, 0x11); outb(0xA0, 0x11);
    outb(0x21, 0x20); outb(0xA1, 0x28);
    outb(0x21, 0x04); outb(0xA1, 0x02);
    outb(0x21, 0x01); outb(0xA1, 0x01);
    outb(0x21, 0xFF); outb(0xA1, 0xFF);
}

void init_gdt() {
    gdt_ptr.limit = (sizeof(gdt_entry_struct) * 3) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;
    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    gdt_flush((uint32_t)&gdt_ptr);
}

void init_idt() {
    idt_ptr.limit = sizeof(idt_entry_struct) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;
    for(int i = 0; i < 256; i++) idt_set_gate(i, 0, 0, 0);

    pic_remap();
    idt_set_gate(0, (uint32_t)divide_by_zero_handler, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)keyboard_handler, 0x08, 0x8E);

    asm volatile("lidt (%0)" : : "r" (&idt_ptr));
    outb(0x21, 0xFD); // 开启键盘 IRQ1
    asm volatile("sti");
}

// 异常与中断处理逻辑
extern "C" void divide_by_zero_handler() {
    kprint_highlight("\n !!! FATAL ERROR: DIVISION BY ZERO !!! ");
    while(1);
}

extern "C" void keyboard_handler_main() {
    uint8_t scancode = inb(0x60);
    if (!(scancode & 0x80)) {
        unsigned char c = kbd_us[scancode];
        
        if (c == '\n') {
            execute_command(); 
        } 
        // 关键：这里要识别出退格键
        else if (c == '\b') {
            if (buffer_idx > 0) {
                buffer_idx--;           // 缓冲区减 1
                put_char('\b', 0x17);   // 屏幕驱动回退 1
            }
        }
        else if (c > 0 && buffer_idx < 255) {
            key_buffer[buffer_idx++] = (char)c;
            char str[2] = {(char)c, '\0'};
            kprint(str);
        }
    }
    outb(0x20, 0x20);
}