#ifndef GDT_IDT_H
#define GDT_IDT_H
#include "types.h"

// GDT 结构
struct gdt_entry_struct {
    uint16_t limit_low; uint16_t base_low; uint8_t base_middle;
    uint8_t access; uint8_t granularity; uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr_struct {
    uint16_t limit; uint32_t base;
} __attribute__((packed));

// IDT 结构
struct idt_entry_struct {
    uint16_t base_low; uint16_t sel; uint8_t always0;
    uint8_t flags; uint16_t base_high;
} __attribute__((packed));

struct idt_ptr_struct {
    uint16_t limit; uint32_t base;
} __attribute__((packed));

extern gdt_entry_struct gdt_entries[3];
extern gdt_ptr_struct   gdt_ptr;
extern idt_entry_struct idt_entries[256];
extern idt_ptr_struct   idt_ptr;


// 这里只告诉编译器“这些变量在别处有”，不分配空间
void init_gdt();
void init_idt();

// 声明外部函数
extern "C" void gdt_flush(uint32_t);
extern "C" void keyboard_handler(); // 汇编里的入口
extern "C" void divide_by_zero_handler();

#endif