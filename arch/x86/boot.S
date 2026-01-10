#include "multiboot.h"

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

.section .text
    /*此处grub交接，GRUB使用平坦内存模型，基地址为0，线性地址等于逻辑地址*/
    .global start_
    .type start_, @function
    start_:    
        movl $stack_top, %esp
        /* 
            TODO: kernel init
                1. Load GDT
                2. Load IDT
                3. Enable paging
        */
        call _kernel_init

        //gdt&idt init
        subl $0x6,%esp
        movl $_gdt,2(%esp)    //取出地址值
        movw _gdt_limit,%ax  //内存引用，取出变量的值
        movw %ax,(%esp)
        lgdt (%esp)
        movl $_idt,2(%esp)    //lidt 同lgdt
        movw _idt_limit,%ax
        movw %ax,(%esp)
        lidt (%esp)
        sti
        addl $0x6,%esp


        movw $0x10,%cx
        movw %cx,%es
        movw %cx,%ds
        movw %cx,%fs
        movw %cx,%gs
        movw %cx,%ss


        pushw $0x08
        pushl $_after_gdt
        retf
        
    _after_gdt:
        pushl %ebx
        call _kernel_main

        cli
    j_:
        hlt