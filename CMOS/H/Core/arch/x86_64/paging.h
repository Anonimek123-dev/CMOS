#pragma once

#include <stdint.h>

typedef uint64_t pte_t;
typedef uint64_t pde_t;
typedef uint64_t pdpt_t;
typedef uint64_t pml4e_t;

#define PAGE_PRESENT    (1ULL << 0)
#define PAGE_WRITABLE   (1ULL << 1)
#define PAGE_USER       (1ULL << 2)
#define PAGE_HUGE       (1ULL << 7)

void paging_init(void);
void paging_info(void);
void paging_enable(void);

void map_page(uint64_t virt, uint64_t phys, uint64_t flags);
void unmap_page(uint64_t virt);

static void paging_load(uint64_t* pml4);
static void page_zero(uint64_t* table);

uint64_t virt_to_phys(uint64_t virt);