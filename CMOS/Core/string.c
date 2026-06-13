#include "H/Core/string.h"

void* memset(void* ptr, int value, uint64_t size)
{
    unsigned char* p = (unsigned char*)ptr;

    while(size--)
    {
        *p++ = (unsigned char)value;
    }

    return ptr;
}

void* memcpy(void* dest, const void* src, uint64_t size)
{
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;

    while(size--)
        *d++ = *s++;

    return dest;
}

int memcmp(const void* a, const void* b, uint64_t size)
{
    const uint8_t* p1 = (const uint8_t*)a;
    const uint8_t* p2 = (const uint8_t*)b;

    while(size--)
    {
        if(*p1 != *p2)
            return *p1 - *p2;

        p1++;
        p2++;
    }
    return 0;
}

uint64_t strlen(const char* str)
{
    uint64_t len = 0;

    while(str[len])
        len++;

    return len;
}

char* strcpy(char* dest, const char* src)
{
    char* start = dest;
    while((*dest++ = *src++))
        ;
    return start;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}