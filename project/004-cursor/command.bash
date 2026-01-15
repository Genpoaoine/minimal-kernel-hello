# WSL 编译汇编
i686-linux-gnu-as boot.s -o boot.o
# WSL 编译 C++
i686-linux-gnu-g++ -c kernel.cpp -o kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
# WSL 链接
i686-linux-gnu-gcc -T linker.ld -o strata_kernel.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc -no-pie