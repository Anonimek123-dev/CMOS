#include "H/Core/kernel/kernel.h"

// Start
void kernel_init() {    
    print_set_color(YELLOW, BLACK);
    print_str("CMOS booted successfully!\n");
}

// Update per frame
void kernel_update(void) {
    kb_update();
}