// 裸机环境下，显存起始地址是 0xB8000
// extern "C": 需要告诉 C++ 编译器：“这个函数请按照 C 语言的方式处理，不要乱改它的名字”。
extern "C" void kmain() {
	const char *str = "STRATA OS: Sentry Kernel Active!";
	char *video_memory = (char *)0xB8000;

	// 清屏并将文字显示在屏幕左上角
	for (int i = 0; i < 80 * 25 * 2; i++)
		video_memory[i] = 0;

	for (int i = 0; str[i] != '\0'; i++) {
		video_memory[i * 2] = str[i];      // 字符
		video_memory[i * 2 + 1] = 0x07;    // 颜色 (浅灰色)
	}

	// 运行到这里后死循环，防止 CPU 跑飞
	while (1);
}