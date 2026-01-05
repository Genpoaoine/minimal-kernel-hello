// In a bare-metal environment, the starting address of video memory is 0xB8000
// extern "C": Used to inform the C++ compiler: "Process this function in the C language manner; do not alter its name".
extern "C" void kmain() {
	const char *str = "Example OS: Sentry Kernel Active!";
	char *video_memory = (char *)0xB8000;

	// Clear the screen and display text at the top-left corner of the screen
	for (int i = 0; i < 80 * 25 * 2; i++)
		video_memory[i] = 0;

	for (int i = 0; str[i] != '\0'; i++) {
		video_memory[i * 2] = str[i];      // Character
		video_memory[i * 2 + 1] = 0x07;    // Color (light gray)
	}

	// Enter an infinite loop here to prevent the CPU from running wild
	while (1);
}