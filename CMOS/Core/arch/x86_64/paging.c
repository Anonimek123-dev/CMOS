#include "H/Core/arch/x86_64/paging.h"
#include "H/Memory/pmm.h"
#include "H/HAL/IO/stdio.h"

static uint64_t* kernel_pml4 = 0;
static uint64_t* kernel_pdpt = 0;
static uint64_t* kernel_pd   = 0;
static uint64_t* kernel_pt   = 0;
static uint8_t paging_enabled = 0;
static uint64_t mapped_pages = 0;

void paging_init(void)
{
    kernel_pml4 = pmm_alloc_page();
    kernel_pdpt = pmm_alloc_page();
    kernel_pd   = pmm_alloc_page();
    kernel_pt   = pmm_alloc_page();

    for(uint64_t i = 0; i < 512; i++)
    {
        kernel_pml4[i] = 0;
        kernel_pdpt[i] = 0;
        kernel_pd[i]   = 0;
        kernel_pt[i]   = 0;
    }

    kernel_pml4[0] = ((uint64_t)kernel_pdpt) | PAGE_PRESENT | PAGE_WRITABLE;
    kernel_pdpt[0] = ((uint64_t)kernel_pd)   | PAGE_PRESENT | PAGE_WRITABLE;
    kernel_pd[0]   = ((uint64_t)kernel_pt)   | PAGE_PRESENT | PAGE_WRITABLE;

    for(uint64_t i = 0; i < 512; i++)
    {
        map_page(i * 0x1000, i * 0x1000, PAGE_WRITABLE);
    }
}

void paging_info(void)
{
    print_set_color(LIGHT_CYAN, BLACK);

    printf("\n");
    printf("+--------------------------------------------+\n");
    printf("|               PAGING INFO                  |\n");
    printf("+--------------------------------------------+\n");

    printf_color(LIGHT_GREEN, BLACK, "PML4 : ");
    print_hex((uint64_t)kernel_pml4);
    printf("\n");

    printf_color(LIGHT_GREEN, BLACK, "PDPT : ");
    print_hex((uint64_t)kernel_pdpt);
    printf("\n");

    printf_color(LIGHT_GREEN, BLACK, "PD   : ");
    print_hex((uint64_t)kernel_pd);
    printf("\n");

    printf_color(LIGHT_GREEN, BLACK, "PT   : ");
    print_hex((uint64_t)kernel_pt);
    printf("\n");

    uint64_t mapped_bytes = mapped_pages * 4096;

    printf_color(RED, BLACK, "Mapped Pages : ");
    print_int(mapped_pages);
    printf("\n");

    printf_color(RED, BLACK, "Mapped Memory: ");
    print_int(mapped_bytes / 1024 / 1024);
    printf(" MB\n");

    printf_color(LIGHT_GREEN, BLACK, "CR3 Loaded : ");
    if(paging_enabled)
        printf("YES");
    else
        printf("NO");
    printf("\n");

    printf_color(LIGHT_CYAN, BLACK, "+--------------------------------------------+\n");
}

static void paging_load(uint64_t* pml4)
{
    asm volatile("mov %0, %%cr3" : : "r"(pml4) : "memory");
}

void map_page(uint64_t virt, uint64_t phys, uint64_t flags)
{
    uint64_t pml4_index = (virt >> 39) & 0x1FF;
    uint64_t pdpt_index = (virt >> 30) & 0x1FF;
    uint64_t pd_index   = (virt >> 21) & 0x1FF;
    uint64_t pt_index   = (virt >> 12) & 0x1FF;

    // Create PDPT if missing
    if(!(kernel_pml4[pml4_index] & PAGE_PRESENT))
    {
        uint64_t* new_pdpt = pmm_alloc_page();
        if(!new_pdpt)
            return;
        page_zero(new_pdpt);
        kernel_pml4[pml4_index] = ((uint64_t)new_pdpt) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    uint64_t* pdpt = (uint64_t*)(kernel_pml4[pml4_index] & ~0xFFFULL);

    // Create PD if missing
    if(!(pdpt[pdpt_index] & PAGE_PRESENT))
    {
        uint64_t* new_pd = pmm_alloc_page();
        if(!new_pd)
            return;
        page_zero(new_pd);
        pdpt[pdpt_index] = ((uint64_t)new_pd) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    uint64_t* pd = (uint64_t*)(pdpt[pdpt_index] & ~0xFFFULL);

    // Create PT if missing
    if(!(pd[pd_index] & PAGE_PRESENT))
    {
        uint64_t* new_pt = pmm_alloc_page();
        if(!new_pt)
            return;
        page_zero(new_pt);
        pd[pd_index] = ((uint64_t)new_pt) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    uint64_t* pt = (uint64_t*)(pd[pd_index] & ~0xFFFULL);

    // Map page
    if(!(pt[pt_index] & PAGE_PRESENT))
    {
        mapped_pages++;
    }

    pt[pt_index] = (phys & ~0xFFFULL) | flags | PAGE_PRESENT;
    asm volatile("invlpg (%0)" : : "r"(virt) : "memory");
}

void unmap_page(uint64_t virt)
{
    uint64_t pml4_index = (virt >> 39) & 0x1FF;
    uint64_t pdpt_index = (virt >> 30) & 0x1FF;
    uint64_t pd_index   = (virt >> 21) & 0x1FF;
    uint64_t pt_index   = (virt >> 12) & 0x1FF;

    if(!(kernel_pml4[pml4_index] & PAGE_PRESENT))
        return;

    uint64_t* pdpt = (uint64_t*)(kernel_pml4[pml4_index] & ~0xFFFULL);

    if(!(pdpt[pdpt_index] & PAGE_PRESENT))
        return;

    uint64_t* pd = (uint64_t*)(pdpt[pdpt_index] & ~0xFFFULL);

    if(!(pd[pd_index] & PAGE_PRESENT))
        return;

    uint64_t* pt = (uint64_t*)(pd[pd_index] & ~0xFFFULL);

    if(pt[pt_index] & PAGE_PRESENT)
    {
        pt[pt_index] = 0;
        if(mapped_pages)
            mapped_pages--;
    }

    asm volatile("invlpg (%0)" : : "r"(virt) : "memory");
}

uint64_t virt_to_phys(uint64_t virt)
{
    uint64_t pml4_index = (virt >> 39) & 0x1FF;
    uint64_t pdpt_index = (virt >> 30) & 0x1FF;
    uint64_t pd_index   = (virt >> 21) & 0x1FF;
    uint64_t pt_index   = (virt >> 12) & 0x1FF;

    if(!(kernel_pml4[pml4_index] & PAGE_PRESENT))
        return 0;

    uint64_t* pdpt = (uint64_t*)(kernel_pml4[pml4_index] & ~0xFFFULL);

    if(!(pdpt[pdpt_index] & PAGE_PRESENT))
        return 0;

    uint64_t* pd = (uint64_t*)(pdpt[pdpt_index] & ~0xFFFULL);

    if(!(pd[pd_index] & PAGE_PRESENT))
        return 0;

    uint64_t* pt = (uint64_t*)(pd[pd_index] & ~0xFFFULL);

    if(!(pt[pt_index] & PAGE_PRESENT))
        return 0;

    uint64_t phys = (pt[pt_index] & ~0xFFFULL) | (virt & 0xFFFULL);
    return phys;
}

void paging_enable(void)
{
    paging_load(kernel_pml4);
    paging_enabled = 1;
}

static void page_zero(uint64_t* table)
{
    for(uint64_t i = 0; i < 512; i++)
        table[i] = 0;
}