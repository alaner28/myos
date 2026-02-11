#include<myos/arch/gdt.h>
#include <stdint.h>
#include "../.vscode/intellisense_helpers.h"
#define GDT_ENTRY_NUM 6 // 全局描述符表项数
/*全局描述符表，根据链接器，作为数据段加载到内存地址0x100000 */

uint64_t _gdt[GDT_ENTRY_NUM]; 
uint16_t _gdt_limit = sizeof(_gdt) - 1; // 全局描述符表界限

void _set_gdt_entry(int index, uint32_t base, uint32_t limit, uint32_t flags)
{
    _gdt[index] = SD_BASE_H(base)| flags | SD_LIMIT_H(limit) | SD_BASE_M(base);
    _gdt[index] <<= 32;
    _gdt[index] |= SD_BASE_L(base) | SD_LIMIT_L(limit);
}

void _init_gdt()
{
    // 空描述符
    _set_gdt_entry(0, 0, 0, 0);
    // 内核代码段
    _set_gdt_entry(1, 0, 0xFFFFF, SEG_R0_CODE);
    // 内核数据段
    _set_gdt_entry(2, 0, 0xFFFFF, SEG_R0_DATA);
    // 用户代码段
    _set_gdt_entry(3, 0, 0xFFFFF, SEG_R3_CODE);
    // 用户数据段
    _set_gdt_entry(4, 0, 0xFFFFF, SEG_R3_DATA);

}