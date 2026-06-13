#pragma once

#include <stdint.h>

void vmm_init(void);
void* vmm_alloc_pages(uint64_t pages);
void vmm_free_pages(void* addr, uint64_t pages);
void vmm_info(void);