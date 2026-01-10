#ifndef __IDT_H__
#define __IDT_H__

#include "../.vscode/intellisense_helpers.h"

#define IDT_SIZE 256
#define IDT_PRESENT(x) ((x) << 15) // ISR所在的段是否存在
#define IDT_DPL0(x)   ((x) << 13) // ISR ring
#define IDT_D(x)     ((x) << 11) // ISR位宽，0表示16位，1表示32位

#define IDT_FLAGS(x) (IDT_PRESENT(1) | IDT_DPL0(x) | IDT_D(1) | ((0x6)<<8))

#define IDT_OFFSET_L(x)   ((x) & 0x0FFFF) // ISR偏移量低16位
#define IDT_OFFSET_H(x)   ((x) & 0xFFFF0000) // ISR偏移量高16位
#define IDT_SELECTOR(x)   ((x) << 16) // ISR所在的段选择子

#define FAULT_DIVISION_ERROR 0x0
#define FAULT_TRAP_DEBUG_EXCEPTION 0x1
#define FAULT_NMI_ 0x2
#define FAULT_BREAKPOINT 0x3
#define FAULT_OVERFLOW 0x4
#define FAULT_BOUND_RANGE_EXCEEDED 0x5
#define FAULT_INVALID_OPCODE 0x6
#define FAULT_NO_MATH_PROCESSOR 0x7
#define FAULT_DOUBLE_FAULT 0x8
#define FAULT_RESERVED_0 0x9
#define FAULT_INVALID_TSS 0xA
#define FAULT_SEGMENT_NOT_PRESENT 0xB
#define FAULT_STACK_SEGMENT_FAULT 0xC
#define FAULT_GENERAL_PROTECTION 0xD
#define FAULT_PAGE_FAULT 0xE
#define FAULT_RESERVED_1 0xF
#define FAULT_FPU_FLOATING_POINT_ERROR 0x10
#define FAULT_ALIGNMENT_CHECK 0x11
#define FAULT_MACHINE_CHECK 0x12
#define FAULT_SIMD_FLOATING_POINT_EXCEPTION 0x13
#define FAULT_VIRTUALIZATION_EXCEPTION 0x14
#define FAULT_CONTROL_PROTECTION 0x15

#ifndef __ASSEMBLER__
#include <stdint.h>

void _init_idt();

#pragma pack(push, 1) // 取消结构体对齐
typedef struct {
    uint32_t vector;
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} isr_param;
#pragma pack(pop) // 恢复结构体对齐

void interrupt_dispatcher(isr_param* param);

void isr0();
extern void _asm_isr0();

#endif /* __ASSEMBLER__ */

#endif /* __IDT_H__ */