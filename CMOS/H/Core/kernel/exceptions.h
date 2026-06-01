#pragma once

#include <stdint.h>
#include "H/Core/kernel/panic.h"

void exception_handler(int vector, interrupt_frame_t* frame, uint64_t error_code);