#include "H/Core/kernel/kernel.h"
#include "H/Memory/mmp.h"
#include "H/Memory/pmm.h"
#include "H/Core/arch/x86_64/paging.h"
#include "H/Memory/vmm.h"
#include "H/Memory/heap.h"

// Main kernel function
void kernel_main(uint64_t multiboot_addr) {
    print_clr();    
    hardwaresetup();
    memorysetup(multiboot_addr);
    kernel_init();
    while (1) {
        kernel_update();
    }
}

// Called once to set up interrupts + devices
void hardwaresetup(void) {   
    idt_init();                     // 1) Initialize IDT (sets up interrupt gates)
    pic_remap(0x20, 0x28);          // 2) Remap PIC so IRQs 0..15 map to vectors 0x20..0x2F
    kb_init();                      // 3) Initialize PS/2 keyboard driver (buffers, state)
    tm_init();                      // 4) Initialize timer
    enable_irq();                   // 5) Enable interrupts globally       
}

// Called once to configure all aspects of memory management
void memorysetup(uint64_t multiboot_addr)
{
    mmap_init(multiboot_addr);      // 1) Initialize Memory Map Parser
    pmm_init();                     // 2) Initialize Physical Memory Manager
    paging_init();                  // 3) Initialize Paging
    paging_enable();                // 4) Loading the PML4 address into CR3
    vmm_init();                     // 5) Initialize Virtual Memory Manager
    heap_init();                    // 6) Initialize Heap
}