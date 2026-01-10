#include <myos/arch/idt.h>
void interrupt_dispatcher(isr_param* param){
    switch (param->vector)
    {
    case FAULT_DIVISION_ERROR:
        isr0();
        break;
    
    default:
        break;
    }
}