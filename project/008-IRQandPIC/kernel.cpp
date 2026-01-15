#include "types.h"
#include "screen.h"
#include "gdt_idt.h"

extern "C" void kmain() {
    init_gdt();
    init_idt();

    clear_screen();
    kprint("STRATA OS Revision 1.0.5\n");
    kprint("System Core: Sentry Kernel Active\n");
    kprint("-----------------------------------\n");
    kprint("Waiting for keyboard input...\n");

    while(1);
}