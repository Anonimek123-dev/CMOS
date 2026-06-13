#include "H/Memory/pmm.h"
#include "H/Memory/vmm.h"
#include "H/Core/arch/x86_64/paging.h"
#include "H/HAL/IO/stdio.h"

static uint64_t vmm_next_free = 0x100000000;
static uint64_t vmm_base = 0x100000000ULL;
static uint64_t vmm_allocated_pages = 0;

typedef struct
{
    uint64_t base;
    uint64_t pages;
} vmm_region_t;

static vmm_region_t free_regions[128];
static uint32_t free_region_count = 0;

void vmm_init(void)
{
    vmm_base = 0x100000000ULL;
    vmm_next_free = vmm_base;
    vmm_allocated_pages = 0;

    free_region_count = 0;
}

void* vmm_alloc_pages(uint64_t pages)
{
    uint64_t virt = 0;

    for(uint32_t i = 0; i < free_region_count; i++)
    {
        if(free_regions[i].pages >= pages)
        {
            virt = free_regions[i].base;

            if(free_regions[i].pages == pages)
            {
                free_regions[i] = free_regions[free_region_count - 1];
                free_region_count--;
            }
            else
            {
                free_regions[i].base += pages * 4096;
                free_regions[i].pages -= pages;
            }
            break;
        }
    }

    if(!virt)
    {
        virt = vmm_next_free;
        vmm_next_free += pages * 4096;
    }

    for(uint64_t i = 0; i < pages; i++)
    {
        void* phys = pmm_alloc_page();

        if(!phys)
        {
            vmm_free_pages((void*)virt, i);
            return 0;
        }

        map_page(virt + i * 4096,(uint64_t)phys, PAGE_WRITABLE);
    }

    vmm_allocated_pages += pages;
    return (void*)virt;
}

void vmm_free_pages(void* addr, uint64_t pages)
{
    uint64_t virt = (uint64_t)addr;

    for(uint64_t i = 0; i < pages; i++)
    {
        uint64_t current = virt + i * 4096;
        uint64_t phys = virt_to_phys(current);

        if(phys)
        {
            unmap_page(current);
            pmm_free_page((void*)phys);

            if(vmm_allocated_pages)
                vmm_allocated_pages--;
        }
    }

    if(free_region_count < 128)
    {
        free_regions[free_region_count].base = virt;
        free_regions[free_region_count].pages = pages;
        free_region_count++;
        vmm_merge_regions();
    }
}

void vmm_info(void)
{
    print_set_color(LIGHT_CYAN, BLACK);

    printf("\n");
    printf("+--------------------------------------------+\n");
    printf("|                 VMM INFO                   |\n");
    printf("+--------------------------------------------+\n");

    print_set_color(WHITE, BLACK);

    printf(" Base Address : ");
    print_hex(vmm_base);
    printf("\n");

    printf(" Next Free VA : ");
    print_hex(vmm_next_free);
    printf("\n");

    printf(" Alloc Pages  : ");
    print_int(vmm_allocated_pages);
    printf("\n");

    printf(" Alloc Memory : ");
    print_int((vmm_allocated_pages * 4096) / 1024);
    printf(" KB\n");

    printf(" Free Regions : ");
    print_int(free_region_count);
    printf("\n");

    print_set_color(LIGHT_CYAN, BLACK);

    printf("+--------------------------------------------+\n");

    print_set_color(WHITE, BLACK);
}

void vmm_merge_regions(void)
{
    for(uint32_t i = 0; i < free_region_count; i++)
    {
        for(uint32_t j = i + 1; j < free_region_count; j++)
        {
            uint64_t i_end = free_regions[i].base + free_regions[i].pages * 4096;

            uint64_t j_end = free_regions[j].base + free_regions[j].pages * 4096;

            if(i_end == free_regions[j].base)
            {
                free_regions[i].pages += free_regions[j].pages;
                free_regions[j] = free_regions[free_region_count - 1];
                free_region_count--;
                j--;
            }

            else if(j_end == free_regions[i].base)
            {
                free_regions[i].base = free_regions[j].base;
                free_regions[i].pages += free_regions[j].pages;
                free_regions[j] = free_regions[free_region_count - 1];
                free_region_count--;
                j--;
            }
        }
    }
}