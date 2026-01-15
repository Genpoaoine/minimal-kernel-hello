# WSL 编译汇编
i686-linux-gnu-as boot.s -o boot.o
# WSL 编译 C++
# 现在有了多个头文件，编译命令需要确保编译器能找到当前目录。在你的编译指令中加入 -I.（大写的 I，后面跟一个点，代表搜索当前路径）
i686-linux-gnu-g++ -c kernel.cpp -o kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -I.
# WSL 链接
i686-linux-gnu-gcc -T linker.ld -o strata_kernel.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc -no-pie