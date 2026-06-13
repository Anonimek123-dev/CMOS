#pragma once

#include <stdint.h>
#include <stddef.h>

void pmm_init(void);
void pmm_info(void);
void* pmm_alloc_page(void);
void  pmm_free_page(void* page);
uint64_t pmm_total_pages(void);
uint64_t pmm_free_pages(void);

static inline void bitmap_set(uint64_t bit);
static inline void bitmap_clear(uint64_t bit);
static inline int bitmap_test(uint64_t bit);
