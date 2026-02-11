#include<myos/arch/idt.h>
#include <stdint.h>
#include "../.vscode/intellisense_helpers.h"
/*中断描述符表，根据链接器，作为数据段加载到内存地址0x100000 */
uint64_t _idt[IDT_SIZE]; 
uint16_t _idt_limit = sizeof(_idt) - 1; // 中断描述符表界限

void _set_idt_entry(uint32_t index, uint16_t seg_selector, void(*isr)(), uint8_t dpl)
{
    uintptr_t offset = (uintptr_t)isr;
    _idt[index] = IDT_OFFSET_H(offset) | IDT_FLAGS(dpl);
    _idt[index] <<= 32;
    _idt[index] |= IDT_OFFSET_L(offset) | IDT_SELECTOR(seg_selector);
}

void _init_idt()
{
    //此处的asm_isr0是在汇编文件中定义的中断服务例程入口，与isr0区分开
    _set_idt_entry(FAULT_DIVISION_ERROR, 0x08, _asm_isr0, 0);
}