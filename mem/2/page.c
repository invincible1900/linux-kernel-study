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


/*
 * To make page tables concrete, we first describe a tiny, single-level page table 
 * for a machine with a one-byte address size, with just 64 meaningful addresses.
 *
 * First, we divide memory into aligned blocks called pages. 
 * On this machine, we’ll say the page size is 8-every page is 8 bytes of memory, 
 * starting at an address that’s a multiple of 8. 
 *
 * This splits memory addresses into two parts, the page index and the page offset. 
 *
 * The following is a memory address in this architecture:
 * bit[0:3]: 3bit offset 0~7
 * bit[3:6]: 3bit index  0~7
 *
 * Each of the architecture’s 8 memory pages comprises 8 bytes of memory, 
 * starting at an address that’s a multiple of 8. 
 *
 * Each page is identified by an index. 
 * Within a page, there are 8 bytes—8 different addresses—each with a different offset.
 *
 * A page table must provide enough information to map any virtual address 
 * to a corresponding physical address. 
 *
 * It does so by defining page table entries (PTEs) 
 * that map a specific virtual page which comprises a set of virtual addresses 
 * with the same virtual page index—to a physical page, 
 * as well as any permission bits to be used for that virtual page. 
 *
 * Here’s an example for this architecture:
 *
 * bit[0:3]: 3bit flag bits: U W P
 * bit[3:6]: 3bit physical page index
 *
 * In our 6-bit architecture, the lookup proceeds as follows 
 * (with virtual address va and access type at):
 *
 * 1. Start from physical address %cr3 (location of the page table)
 * 2. Access physical memory at %cr3[va >> 3]: this is the relevant page table entry. (va >> 3 is the page index)
 * 3. Check flag bits; maybe fault. 
 * 4. If the access is OK, return (%cr3[va >> 3] & 0b111000) | (va & 7) as the physical address.
 *
 * */

#include <stdio.h>

char *cr3;
char phy_mem[64];
char page_table[8];

void init_pg_table(){
    cr3 = page_table;

    page_table[0] = (0x0<<3) | 0x3;
    page_table[4] = (0x1<<3) | 0x2;
    page_table[3] = (0x2<<3) | 0x3;
    page_table[2] = (0x3<<3) | 0x3;
    page_table[1] = (0x4<<3) | 0x1;
    page_table[6] = (0x5<<3) | 0x2;
    page_table[7] = (0x6<<3) | 0x1;
    page_table[5] = (0x2<<3) | 0x3;
}

void print_bin(unsigned char value)
{
    for (int i = sizeof(char) * 7; i >= 0; i--)
        printf("%d", (value & (1 << i)) >> i );
    putc('\n', stdout);
}


int main(){
        unsigned char va, pa;

        init_pg_table();

        va = 0x1a;
        printf("va:\n");
        print_bin(va);

        printf("page table index: %x\n", va >> 3);
        printf("page table[%x] entry: 0x%x\n", va >> 3, cr3[ va >> 3 ] );

        printf("physical page index: %x\n", cr3[va >> 3] >> 3);
        printf("physical page offset: %x\n", va & 7);

        pa =  cr3[va >> 3] & 0b111000 | (va & 7);
        printf("physical page address: 0x%x, %d\n", pa, pa);

        va = 0x2a;
        printf("va:\n");
        print_bin(va);
        printf("page table index: %x\n", va >> 3);
        printf("page table[%x] entry: 0x%x\n", va >> 3, cr3[ va >> 3 ] );

        printf("physical page index: %x\n", cr3[va >> 3] >> 3);
        printf("physical page offset: %x\n", va & 7);

        pa =  cr3[va >> 3] & 0b111000 | (va & 7);
        printf("physical page address: 0x%x, %d\n", pa, pa);

        return 0;
}









