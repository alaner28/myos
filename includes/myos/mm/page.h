#include <../.vscode/intellisense_helpers.h>
typedef uint32_t ptd_t; // 页目录项类型
typedef uint32_t pt_t;  // 页表项类型

#define PAGE_SIZE 4096
#define PAGE_MASK 0xFFFFF000
#define PAGE_SHIFT 12
#define PAGE_ENTRIES 1024

#define PAGE_OFFSET(addr) ((addr) & ~PAGE_MASK)// 页内偏移
#define PTE_INDEX(addr) (((addr) >> 12) & 0x3FF) // 页表项索引
#define PDE_INDEX(addr) (((addr) >> 22) & 0x3FF) // 页目录项索引
#define PAGE_ALIGN(addr) (((addr) + PAGE_SIZE - 1) & PAGE_MASK)// 地址向上对齐到页边界

#define PDE_ADDR(pde_index) ((pde_index) << 22)
#define PTE_ADDR(pte_index, pde_index) ((pte_index << 12) | (pde_index << 22))

#define PAGE_PRESENT              (0x1)// 页存在标志
#define PAGE_WRITE                (0x1 << 1)// 可写标志
#define PAGE_ALLOW_USER           (0x1 << 2)// 用户态可访问标志
#define PAGE_WRITE_THROUGHT       (1 << 3)// 写直达
#define PAGE_DISABLE_CACHE        (1 << 4)// 禁止缓存
#define PAGE_PDE_4MB              (1 << 7)// 页目录项4MB页标志

#define PAGE_DIRTY(pte)           ((pte& (1 << 6))>>6)// 页被写过标志
#define PAGE_ACCESSED(pte)        ((pte & (1 << 5))>>5)// 页被访问过标志

// 创建页目录项和页表项
#define PDE(addr,flags)         (((uint32_t)(addr) & PAGE_MASK) | (flags)& 0xFFF)
#define PTE(addr,flags)         (((uint32_t)(addr) & PAGE_MASK) | (flags)& 0xFFF)

//循环映射， 页目录和页表的物理地址为它本身最后一个表项的地址
#define PTD_BASE_ADDR 0xFFFFF000
#define PT_BASE_ADDR 0xFFC00000
// 获取页目录项虚拟地址
#define PT_VADDR(pd_offset)     (PT_BASE_ADDR | ((pd_offset) << 12))

#define PAGE_DICTIONARY_STACK_INDEX 6
#define PAGE_DICTIONARY_KERNEL_INDEX 1

//Virtual to Physical address
#define HIGHER_HLF_BASE         0xC0000000
#define VIRT_TO_PHYS(addr)      ((uintptr_t)(addr) - HIGHER_HLF_BASE)
#define PHYS_TO_VIRT(addr)      ((uintptr_t)(addr) + HIGHER_HLF_BASE)


// 用于对PD进行循环映射，因为我们可能需要对PD进行频繁操作，我们在这里禁用TLB缓存
#define PAGE_PREM_R              PAGE_PRESENT
#define PAGE_PREM_RW             PAGE_PRESENT | PAGE_WRITE
#define PAGE_PREM_UR             PAGE_PRESENT | PAGE_ALLOW_USER
#define PAGE_PREM_URW            PAGE_PRESENT | PAGE_WRITE | PAGE_ALLOW_USER
#define T_SELF_REF_PERM        PAGE_PREM_RW | PAGE_DISABLE_CACHE