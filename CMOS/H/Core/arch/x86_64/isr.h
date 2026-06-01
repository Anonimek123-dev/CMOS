#pragma once

#include <stdint.h>
#include "H/Core/kernel/panic.h"

void isr_handler(uint64_t vector, uint64_t error_code, interrupt_frame_t* frame);