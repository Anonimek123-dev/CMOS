#include "H/HAL/IO/stdio.h"

void itoa(int value, char* buffer) {

    char temp[32];
    int i = 0;

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    int negative = 0;

    if (value < 0) {
        negative = 1;
        value = -value;
    }

    while (value > 0) {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }

    int pos = 0;

    if (negative) {
        buffer[pos++] = '-';
    }

    while (i > 0) {
        buffer[pos++] = temp[--i];
    }

    buffer[pos] = '\0';
}

void itoa_base(long value, char* buffer, int base) {

    char digits[] = "0123456789ABCDEF";
    char temp[65];
    int i = 0;
    int negative = 0;

    if (base < 2 || base > 16) {
        buffer[0] = '\0';
        return;
    }

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    if (base == 10 && value < 0) {
        negative = 1;
        value = -value;
    }

    while (value > 0) {
        int remainder = value % base;
        temp[i++] = digits[remainder];
        value /= base;
    }

    if (negative) {
        temp[i++] = '-';
    }

    int j = 0;

    while (i > 0) {
        buffer[j++] = temp[--i];
    }

    buffer[j] = '\0';
}