#pragma once

#include <stdint.h>

void heap_init(void);
void* kmalloc(uint64_t size);
void kfree(void* ptr);
void heap_info(void);
void* kcalloc(uint64_t count, uint64_t size);