#include "H/Core/kernel/kernel.h"

// Main kernel function
void kernel_main() {
    print_clr();    
    hardwaresetup(); 
    kernel_init(); 
    while (1) {
        kernel_update();
    }
}

// Called once to set up interrupts + devices
void hardwaresetup(void) {   
    idt_init();                                     // 1) Initialize IDT (sets up interrupt gates)
    pic_remap(0x20, 0x28);                          // 2) Remap PIC so IRQs 0..15 map to vectors 0x20..0x2F
    kb_init();                                      // 3) Initialize PS/2 keyboard driver (buffers, state)
    tm_init();                                      // 4) Initialize timer
    enable_irq();                                   // 5) Enable interrupts globally       
}
