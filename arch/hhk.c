#include <myos/mm/page.h>
#include <stdint.h>
//链接器提供
extern uint8_t __kernel_start;
extern uint8_t __kernel_end;
extern uint8_t __init_hhk_end;
extern uint8_t _k_stack;

//页表和页目录内存上连续分配
#define PT_ADDR(ptd, pte_index)        ((ptd_t*)ptd + (pte_index + 1) * 1024)// 获取页表地址
#define sym_val(sym)                   ((uintptr_t)(&sym))

//(a+b-1)/b   +0x1000-1 是为了除以4096(页大小)向上取整
#define KERNEL_PAGE_COUNT           ((sym_val(__kernel_end) - sym_val(__kernel_start) + 0x1000 - 1) >> 12);
#define HHK_PAGE_COUNT              ((sym_val(__init_hhk_end) - 0x100000 + 0x1000 - 1) >> 12)

void set_pde(ptd_t*ptd, uint32_t pde_index, uint32_t pde_value) {
    ptd[pde_index] = pde_value;
}
void set_pte(ptd_t*ptd, uint32_t pde_index, uint32_t pte_index, uint32_t pte_value) {
    pt_t* pt = (pt_t*)PT_ADDR(ptd, pde_index);// 获取页表地址
    pt[pte_index] = pte_value;
}

void _init_paging() {
    ptd_t* ptd_base = (ptd_t*)PTD_BASE_ADDR;
    SET_PDE(ptd_base, 0, PDE(ptd_base + PAGE_ENTRIES, PAGE_PRESENT));
    // 2.实现前1MB对等映射
    for (uint32_t addr = 0; addr < 0x100000; addr += PAGE_SIZE) {
        uint32_t pde_index = PDE_INDEX(addr);
        uint32_t pte_index = PTE_INDEX(addr);
        set_pte(ptd_base, pde_index, pte_index, PTE(addr, PAGE_PREM_RW));
    }
    // 3.对等映射init_hhk
    for (uint32_t i = 0; i < HHK_PAGE_COUNT; i++) {
        uint32_t addr = 0x100000 + i * PAGE_SIZE;
        uint32_t pde_index = PDE_INDEX(addr);
        uint32_t pte_index = PTE_INDEX(addr);
        set_pte(ptd_base, pde_index, pte_index, PTE(addr, PAGE_PREM_RW));
    }


    // 4.kernel映射至高半核，kernel运行在分页之后
    //kernel页个数不直接使用count，而是多开辟出一部分页，这里开出5个页目录项的空间
    uint32_t kernel_page_count = KERNEL_PAGE_COUNT;//实际上内核用了多少页
    uint32_t kernel_pde_idx= PDE_INDEX(__kernel_start);
    uint32_t kernel_pte_idx= PTE_INDEX(__kernel_start);
    //操作系统分配给内核5个页目录项空间
    for(int i=kernel_pde_idx;i<kernel_pde_idx + PAGE_DICTIONARY_STACK_INDEX-PAGE_DICTIONARY_KERNEL_INDEX;i++){
        set_pde(ptd_base, i, PDE(ptd_base + PAGE_ENTRIES + (i - kernel_pde_idx + 1) * PAGE_ENTRIES, PAGE_PREM_RW));
    }
    // 首先，检查内核的大小是否够用
    if (kernel_page_count > (PAGE_DICTIONARY_STACK_INDEX-PAGE_DICTIONARY_KERNEL_INDEX) * PAGE_ENTRIES) {
        // ERROR: require more pages
        //  here should do something else other than head into blocking
        while (1);
    }
    // 页表映射
    uint32_t phys_kernel_start = VIRT_TO_PHYS(sym_val(__kernel_start));//映射为物理地址
    uint32_t pde_index = PAGE_DICTIONARY_KERNEL_INDEX;
    for(uint32_t i = 0; i < kernel_page_count; i++) {
        uint32_t addr = phys_kernel_start + i * PAGE_SIZE;// 块物理地址
        uint32_t pte_index = i % PAGE_ENTRIES;
        if(pte_index == 0) 
            pde_index++;
        set_pte(ptd_base, pde_index, pte_index, PTE(addr, PAGE_PREM_RW));
    }
    //设置循环映射
    set_pde(ptd_base, PAGE_ENTRIES - 1, PDE(ptd_base, T_SELF_REF_PERM));
}


void _init_hhk(ptd_t* ptd, uint32_t kpg_size){
    // 初始化 kpg 全为0
    uint8_t* kpg = (uint8_t*) ptd;
    for (uint32_t i = 0; i < kpg_size; i++)
    {
        *(kpg + i) = 0;
    }

    _init_paging();
}