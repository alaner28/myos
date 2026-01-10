#define SD_TYPE(x)        ((x) << 8) // 段类型
#define SD_DPL(x)        ((x) << 13) // 描述符特权级
#define SD_S(x)            (x << 12) // 描述符类型，0表示系统段，1表示代码段或数据段
#define SD_P(x)            (x << 15) // 段是否存在
#define SD_AVL(x)         (x << 20) // 系统自行定义，置0
#define SD_L(x)            (x << 21) //仅在IA-32e模式下有意义，标记段是否为64位代码段。
#define SD_DB(x)           (x << 22) // 默认操作大小。对于代码段，0表示16位段，1表示32位段；对于数据段，0表示16位段，1表示32位段或扩展至32位段。
#define SD_G(x)            (x << 23) // 粒度标志。0表示段界限以字节为单位，1表示段界限以4KB为单位。

#define SD_LIMIT_L(x)    (x & 0x0FFFF) // 段界限低16位
#define SD_LIMIT_H(x)    (x & 0xF0000) // 段界限高4位
#define SD_BASE_L(x)    ((x & 0x0FFFF)<<16) // 段基址低16位
#define SD_BASE_M(x)    (x & 0x00FF0000)
#define SD_BASE_H(x)    (x & 0xFF000000) // 段基址高8位

#define SEG_DATA_RD        0x00 // 可读数据段
#define SEG_DATA_RDA    0x01 // 可读已访问数据段
#define SEG_DATA_RDWR    0x02 // 可读写数据段
#define SEG_DATA_RDWRA    0x03 // 可读写已访问数据段    
#define SEG_DATA_RDEXPD    0x04 // 可读向下扩展数据段
#define SEG_DATA_RDEXPDA 0x05 // 可读向下扩展已访问数据段
#define SEG_DATA_RDWREXP 0x06 // 可读写向下扩展数据段
#define SEG_DATA_RDWREXPA 0x07 // 可读写向下扩展已访问数据段
#define SEG_CODE_EX        0x08 // 可执行代码段
#define SEG_CODE_EXA    0x09 // 可执行已访问代码段
#define SEG_CODE_EXRD    0x0A // 可执行可读代码段
#define SEG_CODE_EXRDA    0x0B // 可执行可读已访问代码段
#define SEG_CODE_EXC    0x0C // 可执行一致代码段
#define SEG_CODE_EXCA    0x0D // 可执行一致已访问代码段
#define SEG_CODE_EXRDC    0x0E // 可执行可读一致代码段
#define SEG_CODE_EXRDCA 0x0F // 可执行可读一致已访问代码段


#define SEG_R0_CODE    SD_TYPE(SEG_CODE_EXRD) | SD_S(1) | SD_DPL(0) | SD_P(1) | SD_DB(1) | SD_G(1) | \
                        SD_AVL(0) | SD_L(0)
#define SEG_R0_DATA    SD_TYPE(SEG_DATA_RDWR) | SD_S(1) | SD_DPL(0) | SD_P(1) | SD_DB(1) | SD_G(1) | \
                        SD_AVL(0) | SD_L(0)
#define SEG_R3_CODE    SD_TYPE(SEG_CODE_EXRD) | SD_S(1) | SD_DPL(3) | SD_P(1) | SD_DB(1) | SD_G(1) | \
                        SD_AVL(0) | SD_L(0)
#define SEG_R3_DATA    SD_TYPE(SEG_DATA_RDWR) | SD_S(1) | SD_DPL(3) | SD_P(1) | SD_DB(1) | SD_G(1) | \
                        SD_AVL(0) | SD_L(0)


void _init_gdt();