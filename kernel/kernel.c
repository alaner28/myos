#include <myos/tty/tty.h>
#include <myos/arch/gdt.h>
#include <myos/arch/idt.h>
void _kernel_init(void* info_table) {
    // TODO
    _init_gdt();
    _init_idt();
}

void _kernel_main(void* info_table) {
    // TODO
    tty_set_theme(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    tty_put_str("Hello kernel world!\nThis is second line.");
    
    int a = 1/0;
}