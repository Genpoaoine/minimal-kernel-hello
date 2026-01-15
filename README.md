# Strata: Minimal x86 Kernel

**Strata** 是一个从零开始、使用 C++ 和汇编语言编写的最小化 x86 架构内核。本项目旨在探索操作系统底层的运行机制，通过在 QEMU 模拟器中从引导到执行简单的命令，展示 OSDev 的核心基础。

> **项目名称**: Strata **存储库**: `minimal-kernel-hello` **运行环境**: QEMU (i386)

------

## 🚀 开发进度与功能集

目前项目已经实现了内核启动的关键里程碑：

- [x] **001-002: 内核引导与基础输出** - 实现了从引导加载程序进入内核，并构建了基础的 `kprint` 打印函数。
- [x] **003-005: 屏幕控制** - 支持自定义字符样式（amiStyle）、光标移动控制以及屏幕滚动处理。
- [x] **006: 全局描述符表 (GDT)** - 初始化并重新加载 GDT，定义了内核态与用户态的分段模型。
- [x] **007-008: 中断系统 (IDT & PIC)** - 成功配置中断描述符表（IDT）并重新映射 8259 PIC，支持外设硬件中断。
- [x] **009: ASCII 与键盘输入** - 实现了硬件扫描码到 ASCII 的映射转换。
- [x] **010: 交互初步** - 支持退格（Backspace）处理及简单的命令解析执行框架。

------

## 🛠 技术栈

- **语言**: C++ (用于内核逻辑), x86 Assembly (用于引导和底层 CPU 交互)
- **构建工具**: `Makefile`, `ld` (链接器)
- **模拟器**: QEMU
- **目标架构**: x86 (32-bit protected mode)

------

## 📖 快速开始

## 🛠 开发环境与构建

本项目开发于 **Windows 10/11 (WSL2)** 环境下的 **Visual Studio Code**。使用交叉编译工具链以确保生成的二进制文件符合 x86 裸机（Bare Metal）要求。

### 前置要求

确保你的 WSL2 系统中已安装以下交叉编译器：

- `i686-linux-gnu-gcc` / `i686-linux-gnu-g++`
- `i686-linux-gnu-as`
- `qemu-system-i386`

### 🏗 手动构建步骤

在项目根目录下按顺序执行以下命令（此处仅截止到文件夹`010-backspaceAndRunCommand`时的操作，之后可能有所不同）：

**1. 编译引导汇编代码**

Bash

```bash
i686-linux-gnu-as boot.s -o boot.o
```

**2. 编译内核 C++ 核心组件**

Bash

```bash
# 屏幕驱动与字符显示
i686-linux-gnu-g++ -c screen.cpp -o screen.o -ffreestanding -O2 -I.

# 全局描述符表 (GDT) 与 中断描述符表 (IDT)
i686-linux-gnu-g++ -c gdt_idt.cpp -o gdt_idt.o -ffreestanding -O2 -I.

# 内核入口主逻辑
i686-linux-gnu-g++ -c kernel.cpp -o kernel.o -ffreestanding -O2 -I.
```

**3. 链接生成内核映像**

Bash

```bash
i686-linux-gnu-gcc -T linker.ld -o strata_kernel.bin \
    -ffreestanding -O2 -nostdlib \
    boot.o screen.o gdt_idt.o kernel.o \
    -lgcc -no-pie
```

### 🖥 运行

使用 QEMU 启动生成的内核：

Bash

```bash
qemu-system-i386 -kernel strata_kernel.bin
```

------

## 🗺 路线图 (Roadmap)

- [ ] 内存管理器 (Physical Memory Allocation)
- [ ] 分页机制 (Paging)
- [ ] 多任务/进程调度
- [ ] 系统调用接口

------

## 🤝 贡献与学习

本项目主要用于个人学习和教育目的。如果你对操作系统开发感兴趣，欢迎提交 Issue 或 Pull Request 来共同完善 Strata。
