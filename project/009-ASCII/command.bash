# 1. 编译汇编
i686-linux-gnu-as boot.s -o boot.o

# 2. 编译所有 CPP 文件
i686-linux-gnu-g++ -c screen.cpp -o screen.o -ffreestanding -O2 -I.
i686-linux-gnu-g++ -c gdt_idt.cpp -o gdt_idt.o -ffreestanding -O2 -I.
i686-linux-gnu-g++ -c kernel.cpp -o kernel.o -ffreestanding -O2 -I.

# 3. 链接在一起
i686-linux-gnu-gcc -T linker.ld -o strata_kernel.bin -ffreestanding -O2 -nostdlib \
    boot.o screen.o gdt_idt.o kernel.o -lgcc -no-pie