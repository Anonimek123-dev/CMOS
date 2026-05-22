#include "H/Core/arch/x86_64/TIMER/pit.h"
#include "H/Core/arch/x86_64/port.h"
#include "H/HAL/IO/print.h"

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43
#define PIT_FREQUENCY 1193182 // Hz

// PIT initialisation at a specified frequency
void pit_init(uint32_t frequency) {
    uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);
    outb(PIT_COMMAND, 0x36);            // channel 0, lo/hi byte, mode 3 (square wave)
    outb(PIT_CHANNEL0, divisor & 0xFF); // low byte
    outb(PIT_CHANNEL0, divisor >> 8);   // high byte

    print_set_color(LIGHT_CYAN, BLACK); print_str("[PIT]"); print_set_color(WHITE, BLACK); print_str(" initialized at "); print_int(frequency); print_str(" Hz\n");
}