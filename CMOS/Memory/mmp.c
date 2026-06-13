#include "H/Memory/mmp.h"
#include "H/HAL/IO/stdio.h"

memory_region_t memory_regions[128];
uint32_t memory_region_count = 0;

uint64_t total_usable_ram = 0;
uint64_t largest_region_base = 0;
uint64_t largest_region_size = 0;

typedef struct {
uint32_t type;
uint32_t size;
} multiboot_tag_t;

typedef struct {
uint32_t type;
uint32_t size;
uint32_t entry_size;
uint32_t entry_version;
} multiboot_tag_mmap_t;

typedef struct {
uint64_t addr;
uint64_t len;
uint32_t type;
uint32_t zero;
} multiboot_memory_map_t;

void mmap_init(uint64_t multiboot_addr)
{
    memory_region_count = 0;
    total_usable_ram = 0;
    largest_region_base = 0;
    largest_region_size = 0;

    multiboot_tag_t* tag = (multiboot_tag_t*)(multiboot_addr + 8);

    while (tag->type != 0)
    {
        if (tag->type == 6)
        {
            multiboot_tag_mmap_t* mmap = (multiboot_tag_mmap_t*)tag;

            multiboot_memory_map_t* entry = (multiboot_memory_map_t*) ((uint8_t*)mmap + sizeof(multiboot_tag_mmap_t));

            while ((uint8_t*)entry < ((uint8_t*)mmap + mmap->size))
            {
                if (memory_region_count < 128)
                {
                    memory_regions[memory_region_count].base =
                        entry->addr;

                    memory_regions[memory_region_count].length =
                        entry->len;

                    memory_regions[memory_region_count].type =
                        entry->type;

                    memory_region_count++;
                }

                if (entry->type == 1)
                {
                    total_usable_ram += entry->len;

                    if (entry->len > largest_region_size)
                    {
                        largest_region_size = entry->len;
                        largest_region_base = entry->addr;
                    }
                }

                entry = (multiboot_memory_map_t*) ((uint8_t*)entry + mmap->entry_size);
            }

            return;
        }

        tag = (multiboot_tag_t*) ((uint8_t*)tag + ((tag->size + 7) & ~7));
    }
}

void mmap_info() {
    print_set_color(LIGHT_CYAN, BLACK);

    printf("\n");
    printf("+--------------------------------------------+\n");
    printf("|               CMOS MEMORY MAP              |\n");
    printf("+--------------------------------------------+\n");

    for(uint32_t i = 0; i < memory_region_count; i++)
    {
        memory_region_t* region = &memory_regions[i];

        switch(region->type)
        {
            case 1:
                printf_color(LIGHT_GREEN, BLACK, "[USABLE   ] ");
                break;

            case 2:
                printf_color(YELLOW, BLACK, "[RESERVED ] ");
                break;

            case 3:
                printf_color(LIGHT_CYAN, BLACK, "[ACPI     ] ");
                break;

            case 4:
                printf_color(MAGENTA, BLACK, "[ACPI NVS ] ");
                break;

            case 5:
                printf_color(LIGHT_RED, BLACK, "[BAD      ] ");
                break;

            default:
                printf_color(DARK_GRAY, BLACK, "[UNKNOWN  ] ");
                break;
        }

        print_set_color(WHITE, BLACK);

        print_hex((unsigned long)region->base);

        printf(" -> ");

        print_hex((unsigned long)(region->base + region->length));

        printf("\n");
    }
    print_set_color(LIGHT_CYAN, BLACK);

    printf("+--------------------------------------------+\n");
    printf("|                 SUMMARY                    |\n");
    printf("+--------------------------------------------+\n");

    print_set_color(WHITE, BLACK);

    printf(" Total RAM          : ");
    print_int(total_usable_ram / 1024 / 1024);
    printf(" MB\n");

    printf(" Largest Region     : ");
    print_hex((unsigned long)largest_region_base);
    printf("\n");

    printf(" Largest Region Size: ");
    print_int(largest_region_size / 1024 / 1024);
    printf(" MB\n");

    printf(" Regions Found      : ");
    print_hex((unsigned long)memory_region_count);
    printf("\n");

    print_set_color(LIGHT_CYAN, BLACK);

    printf("+--------------------------------------------+\n");

    print_set_color(WHITE, BLACK);

}