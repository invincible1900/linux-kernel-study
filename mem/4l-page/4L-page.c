/*****************************************************************
 * physical memory 256 T:
 * ----------------  page-0 4k
 * phy addr
 *
 * 0x0               
 * 0x1
 * ...
 * 0x8
 * ...
 * 0xff8
 * ...
 * ----------------  page-1 4k
 * 0x1000
 * 0x1001
 * ..
 * 0x1008
 * ...
 * 0x1ff8
 *... 
 * ----------------  page-268435455 4k (hex(268435455) == 0xfffffff)
 * ...
 * 0xffffffffff (10个f)
 *
 * --------------------------------------------------------
 *
 * Index 所有的 memory unit(1 bytes) 需要多少个地址，地址需要多少 bits ?
 *
 * 256 T == 1 << 40 Bytes 
 *
 * 共需要 (1 << 40) 个地址: 0 ~ ((1<<40) -1)
 *
 * 至少 40 bits
 *
 * --------------------------------------------------------
 *
 * Kernel Addresses
 * 0xFFFF800000000000 ~ 0xFFFFFFFFFFFFFFFF  (128 TB, 11 个 0, 16 个 F)
 *
 * User Addresses
 * 0x0000000000000000 ~ 0x00007FFFFFFFFFFF ( 128 TB, 11 个 F)
 *
 * Empty Space
 * 0x0000800000000000 ~ 0xFFFF7FFFFFFFFFFF
 *
 * 一个 虚拟地址 映射到一个 物理地址，需要少数内存保存这个映射?
 *
 *
 * --------------------------------------------------------
 *
 * 4K 一个 page 的话，256T 内存一共 268435456 个 page
 *
 * 每个物理 page 对应一个虚拟 page 地址 ,占 8 字节,  一共需要多少内存？
 *
 * #define B (1<< 0)
 * #define K (1<<10)
 * #define M (1<<20)
 * #define G (1<<30)
 * #define T (1<<40)
 *
 * #define PAGE (4 * K)
 *
 * mem = ((256 * T)/PAGE) * (8 * B)  == 64 * G;
 * --------------------------------------------------------
 * 使用四级页表需要占据多少内存？
 *
 * PML4 Table -> PDP Table -> PD Table -> PT
 *
 * PML4 Table address 占据的内存: mem1 = 1 * (8 * B) ;
 * PDP Table address 占据的内存:  mem2 = 512 * (8 * B) == 4 * K;
 * PD Table address 占据的内存: mem3 = 512 * 512 * (8 * B) == 2 * M;
 * PT address 占据的内存: mem4 = 512 * 512 * 512 * (8 * B) == 1 * G;
 * Total: 1G + 2M + 4K + 8B;
 *
 * 四级页表可以 index 多少内存？
 * PML4 = 1;
 * PDP = PML4 * 512;
 * PD = PDP * 512;
 * PT = PD * 512;
 *
 * PT * 512 * PAGE  == 256 * T;
 * 
 *
 * 
 * *****************************************************************/
#include <stdio.h>

typedef uint64_t x86_64_pageentry_t;

typedef struct x86_64_pagetable {
    x86_64_pageentry_t entry[512];
} x86_64_pagetable;



void init_pg_table(){
}

void print_bin(unsigned char value)
{
    for (int i = sizeof(char) * 7; i >= 0; i--)
        printf("%d", (value & (1 << i)) >> i );
    putc('\n', stdout);
}


int main(){

}

//Chickadee’s k-init.cc::cpustate::init_cpu_hardware()
//Load segment descriptor tables
asm volatile("lgdt %0; ltr %1; lidt %2"
             :
             : "m" (gdt.limit),
               "r" ((uint16_t) SEGSEL_TASKSTATE),
               "m" (idt.limit)
             : "memory", "cc");

//Initialize segments, including `%gs`, which
//points at this cpustate
asm volatile("movw %%ax, %%fs; movw %%ax, %%gs"
             : : "a" ((uint16_t) SEGSEL_KERN_DATA));
wrmsr(MSR_IA32_GS_BASE, reinterpret_cast<uint64_t>(this));










