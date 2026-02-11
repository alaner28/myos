#include <myos/arch/multiboot.h>

.section .multiboot
    .long MB_MAGIC
    .long MB_ALIGNED_4K_MEM_MAP
    .long CHECKSUM(MB_ALIGNED_4K_MEM_MAP)

.section .bss
    /* According to System V ABI, the stack must be aligned at 16 bytes boundary */
    .align 16
    stack_bottom:
        .skip 16318, 0
    stack_top:
     /* 这里只是一个临时栈，在_hhk_init里面我们会初始化内核专用栈 */

.section .hhk_init
    /*此处grub交接，GRUB使用平坦内存模型，基地址为0，线性地址等于逻辑地址*/
    .global start_
    .type start_, @function
    start_:    
        movl $stack_top, %esp


        call _init_hhk

        
        pushl $_kernel_entry
        ret









        