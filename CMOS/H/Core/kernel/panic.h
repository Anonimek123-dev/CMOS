#pragma once

#include <stdint.h>

typedef struct interrupt_frame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} interrupt_frame_t;

void kernel_panic(const char* reason, interrupt_frame_t* frame, uint64_t error_code);