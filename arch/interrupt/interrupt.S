#include <myos/arch/idt.h>

// 该宏用于定义中断服务例程（ISR）,在汇编器中展开
.macro isr_temp vector, no_error_code = 1
    .global _asm_isr\vector
    .type _asm_isr\vector, @function
    _asm_isr\vector:
        .if \no_error_code
            push $0
        .endif
        push $\vector
        jmp interrupt_wrapper
.endm

.section .text
    isr_temp 0
    interrupt_wrapper:
        //这里需要手动传参，将esp的值传递给dispatcher函数，所以需要保存esp的值
        //并且要16字节对齐，因为ABI要求call函数前栈要16字节对齐
        movl %esp, %eax
        andl $0xFFFFFFF0, %esp
        //保持16字节对齐的时候不能使用push/pop 寄存器，因为寄存器数据为四字节32位
        subl $16, %esp
        movl %eax, (%esp) // 将原esp值存储在栈顶，作为参数传递给dispatcher
        call interrupt_dispatcher
        pop %eax
        movl %eax, %esp
        add $8, %esp  //只需要弹出中断号和错误码，其余的由iret指令弹出
        
        iret