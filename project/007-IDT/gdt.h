#ifndef GDT_H
#define GDT_H

#include "types.h"

struct gdt_entry_struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// 声明函数，让 kernel.cpp 知道它们存在
void init_gdt();
extern "C" void gdt_flush(uint32_t);

#endif