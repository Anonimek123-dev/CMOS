#pragma once

#include <stdint.h>

void* memset(void* ptr, int value, uint64_t size);
void* memcpy(void* dest, const void* src, uint64_t size);
int memcmp(const void* a, const void* b, uint64_t size);

uint64_t strlen(const char* str);
char* strcpy(char* dest, const char* src);
int strcmp(const char* a, const char* b);