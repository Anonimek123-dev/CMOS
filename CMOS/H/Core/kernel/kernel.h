#pragma once

#include "H/HAL/IO/print.h"
#include <stdint.h>

void kernel_init(void);
void hardwaresetup(void);
void kernel_update(void);
void enable_irq(void);
void kb_update(void);