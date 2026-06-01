#include "H/Core/kernel/kernel.h"
#include "H/Core/arch/x86_64/pic.h"
#include "H/HAL/Drivers/PS2/keyboard.h"
#include "H/Core/arch/x86_64/TIMER/pit.h"
#include "H/Core/arch/x86_64/TIMER/timer.h"
#include "H/Core/kernel/exceptions.h"
#include "H/Core/kernel/panic.h"

// Forward declare PIC send EOI and keyboard handler
extern void pic_eoi(unsigned char irq);
extern void kb_irq_handler(void);

// C handler called from assembly with vector in rdi
void isr_handler(uint64_t vector, uint64_t error_code, interrupt_frame_t* frame) {
    if (vector <= 31) {
        exception_handler(vector, frame, error_code);
        return;
    }

    if (vector >= 32 && vector <= 47) {
        unsigned char irq = (unsigned char)(vector - 32);
        // dispatch common IRQs here
        switch (irq) {
            case 0:  // PIT timer IRQ0
                tm_tick();
                break;
            case 1: // keyboard
                kb_irq_handler();
                break;
            default:
                // other IRQs we don't handle yet
                break;
        }

        // send EOI to PICs
        pic_eoi(irq);
        return;
    }

    // unexpected vector
    print_str("Unhandled vector: ");
    print_int((int)vector);
    print_str("\n");
}

// enable interrupts (wrapper)
void enable_irq(void) {
    __asm__ volatile ("sti");
}