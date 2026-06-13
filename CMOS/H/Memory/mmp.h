#pragma once

#include <stdint.h>

typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
} memory_region_t;

extern memory_region_t memory_regions[128];
extern uint32_t memory_region_count;

extern uint64_t total_usable_ram;
extern uint64_t largest_region_base;
extern uint64_t largest_region_size;

void mmap_init(uint64_t multiboot_addr);
void mmap_info(void);;