#include <myos/arch/idt.h>
#include <myos/tty/tty.h>
void isr0(){
    tty_clear();
    tty_put_str("Division By Zero Error Exception!\n");
}