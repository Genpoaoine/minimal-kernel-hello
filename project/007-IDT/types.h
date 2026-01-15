#ifndef TYPES_H
#define TYPES_H

// 自己定义，不再依赖 stdint.h
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

struct idt_entry_struct {
    uint16_t base_low;    // 中断处理函数地址的低16位
    uint16_t sel;         // 目标段选择子（我们在GDT里设的0x08）
    uint8_t  always0;     // 始终为 0
    uint8_t  flags;       // 标志位（特权级、存在位等）
    uint16_t base_high;   // 中断处理函数地址的高16位
} __attribute__((packed));

struct idt_ptr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

idt_entry_struct idt_entries[256];
idt_ptr_struct   idt_ptr;

#endif