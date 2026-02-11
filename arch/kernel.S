.section .text
        
    global _kernel_entry
    .type _kernel_entry, @function    
    _kernel_entry:

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
        pushl %ebx  //multiboot info pointer which is saved in ebx by GRUB
        call _kernel_main

        cli
    j_:
        hlt