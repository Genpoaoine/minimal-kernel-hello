.set ALIGN,    1<<0             
.set MEMINFO,  1<<1             
.set FLAGS,    ALIGN | MEMINFO  
.set MAGIC,    0x1BADB002       
.set CHECKSUM, -(MAGIC + FLAGS) 

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
_start:
	mov $stack_top, %esp
	call kmain
	cli
1:	hlt
	jmp 1b

.global gdt_flush
gdt_flush:
    mov 4(%esp), %eax    # 获取参数（gdt_ptr 地址）
    lgdt (%eax)          # 加载新 GDT

    mov $0x10, %ax       # 0x10 是数据段在 GDT 中的偏移 (2 * 8)
    mov %ax, %ds         # 更新所有数据段寄存器
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    
    # 绝杀：长跳转刷新 CS 寄存器
    # 0x08 是代码段偏移 (1 * 8)
    ljmp $0x08, $.flush
.flush:
    ret