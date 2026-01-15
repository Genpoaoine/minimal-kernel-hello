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
    mov 4(%esp), %eax
    lgdt (%eax)
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ljmp $0x08, $.flush
.flush:
    ret

.global keyboard_handler
.extern keyboard_handler_main
keyboard_handler:
    pusha                # 压入所有寄存器
    call keyboard_handler_main  # 调用 gdt_idt.cpp 里的逻辑函数
    popa                 # 弹出所有寄存器
    iret                 # 中断返回 (核心指令！)