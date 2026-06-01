#include "H/Core/kernel/kernel.h"
#include "H/HAL/IO/stdio.h"

// Start
void kernel_init() {    
    printf_color(YELLOW, BLACK, "CMOS booted successfully!\n");
}

// Update per frame
void kernel_update(void) {
    kb_update();
}