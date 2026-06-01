#pragma once

#include "H/HAL/IO/print.h"
#include <stdarg.h>

#define INT_MAX 2147483647
#define INT_MIN -2147483647
#define UINT_MAX 4294967295U
#define LLONG_MAX  9223372036854775807LL
#define LLONG_MIN -9223372036854775807LL - 1LL

void putchar(char c);
void puts(const char* str);
void set_text_color(uint8_t fg, uint8_t bg);
void itoa(int value, char* buffer);
void itoa_base(long value, char* buffer, int base);
void test(const char* fmt, ...);
int printf(const char* fmt, ...);
int printf_color(uint8_t fg, uint8_t bg, const char* fmt, ...);
int vprintf(const char* fmt, va_list args);
int snprintf(char* buffer, size_t size, const char* fmt, ...);
int vsprintf(char* buffer, const char* fmt, va_list args);
int sprintf(char* buffer, const char* fmt, ...);
int vsnprintf(char* buffer, size_t size, const char* fmt, va_list args);
int str_len(const char* str);
void print_padding(int current_len, int width, char pad);
void buffer_putchar(char* buffer,size_t size, int* pos, char c);
void buffer_padding(char* buffer, size_t size, int* pos, int current_len, int width, char pad);
void buffer_putstr(char* buffer, size_t size, int* pos, const char* str);
void format_string(char* buffer, size_t size, int* pos, const char* str, int len, int width, int left_align, int zero_pad);
char getchar();
int scanf(const char* fmt, ...);
int vscanf(const char* fmt, va_list args);
int vsscanf(const char* buffer, const char* fmt, va_list args);