#include "H/Memory/pmm.h"
#include "H/Memory/mmp.h"
#include "H/HAL/IO/stdio.h"

static uint8_t* bitmap = 0;
static uint64_t total_pages = 0;
static uint64_t free_pages = 0;

void pmm_init(void)
{
    total_pages = total_usable_ram / 4096;
    free_pages = total_pages;
    extern char kernel_end[];
    uint64_t bitmap_start = ((uint64_t)kernel_end + 4095) & ~4095ULL;
    uint64_t bitmap_size = (total_pages + 7) / 8;
    uint64_t bitmap_pages = (bitmap_size + 4095) / 4096;
    bitmap = (uint8_t*)bitmap_start;
    uint64_t kernel_start = 0x100000;
    uint64_t kernel_end_addr = (uint64_t)kernel_end;
    uint64_t kernel_pages = (kernel_end_addr - kernel_start + 4095) / 4096;
    uint64_t bitmap_page_start = (bitmap_start - 0x100000) / 4096;

    for (uint64_t i = 0; i < bitmap_size; i++)
    {
        bitmap[i] = 0;
    }

    for(uint64_t i = 0; i < kernel_pages; i++)
    {
        bitmap_set(i);
        free_pages--;
    }

    for(uint64_t i = 0; i < bitmap_pages; i++)
    {
        bitmap_set(bitmap_page_start + i);
        free_pages--;
    }
}

static inline void bitmap_set(uint64_t bit) {
    bitmap[bit / 8] |= (1 << (bit % 8));
}

static inline void bitmap_clear(uint64_t bit) {
    bitmap[bit / 8] &= ~(1 << (bit % 8));
}

static inline int bitmap_test(uint64_t bit) {
    return bitmap[bit / 8] & (1 << (bit % 8));
}

void* pmm_alloc_page(void)
{
    uint64_t page_index = 0;

    for(uint32_t region = 0; region < memory_region_count; region++)
    {
        memory_region_t* r = &memory_regions[region];

        if(r->type != 1)
            continue;

        if(r->base < 0x100000)
            continue;

        uint64_t region_pages = r->length / 4096;

        for(uint64_t i = 0; i < region_pages; i++)
        {
            if(!bitmap_test(page_index))
            {
                bitmap_set(page_index);

                if(free_pages)
                    free_pages--;

                return (void*)(r->base + (i * 4096));
            }

            page_index++;
        }
    }

    return 0;
}

void pmm_free_page(void* page) {
    uint64_t addr = (uint64_t)page;

    uint64_t page_index = 0;

    for(uint32_t region = 0; region < memory_region_count; region++)
    {
        memory_region_t* r = &memory_regions[region];

        if(r->type != 1)
            continue;

        uint64_t region_start = r->base;
        uint64_t region_end = r->base + r->length;

        if(addr >= region_start && addr < region_end)
        {
            uint64_t local_page =
                (addr - region_start) / 4096;

            bitmap_clear(page_index + local_page);

            free_pages++;

            return;
        }

        page_index += r->length / 4096;
    }
}

uint64_t pmm_total_pages(void) {
    return total_pages;
}

uint64_t pmm_free_pages(void) {
    return free_pages;
}

void pmm_info(void)
{
    uint64_t bitmap_size = (total_pages + 7) / 8;
    uint64_t bitmap_pages = (bitmap_size + 4095) / 4096;

    print_set_color(LIGHT_CYAN, BLACK);

    printf("\n");
    printf("+--------------------------------------------+\n");
    printf("|                 PMM INFO                   |\n");
    printf("+--------------------------------------------+\n");

    print_set_color(WHITE, BLACK);

    printf(" Total Pages : ");
    print_int((int)total_pages);
    printf("\n");

    printf(" Free Pages  : ");
    print_int((int)free_pages);
    printf("\n");

    printf(" Used Pages  : ");
    print_int((int)(total_pages - free_pages));
    printf("\n");

    printf(" Bitmap Base : ");
    print_hex((unsigned long)bitmap);
    printf("\n");

    printf(" Bitmap Size : ");
    print_int((int)bitmap_size);
    printf(" bytes\n");

    printf(" Bitmap Pages: ");
    print_int((int)bitmap_pages);
    printf("\n");

    print_set_color(LIGHT_CYAN, BLACK);
    printf("+--------------------------------------------+\n");

    print_set_color(WHITE, BLACK);
}