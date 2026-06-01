#include "H/Core/arch/x86_64/idt.h"
#include "H/Core/kernel/kernel.h"

#define KERNEL_CS 0x08
#define INT_GATE  0x8E

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

// ISR stubs (0–31)
extern void isr0(),  isr1(),  isr2(),  isr3(),  isr4(),  isr5(),  isr6(),  isr7();
extern void isr8(),  isr9(),  isr10(), isr11(), isr12(), isr13(), isr14(), isr15();
extern void isr16(), isr17(), isr18(), isr19(), isr20(), isr21(), isr22(), isr23();
extern void isr24(), isr25(), isr26(), isr27(), isr28(), isr29(), isr30(), isr31();

// IRQ stubs (32–47)
extern void irq32(), irq33(), irq34(), irq35(), irq36(), irq37(), irq38(), irq39();
extern void irq40(), irq41(), irq42(), irq43(), irq44(), irq45(), irq46(), irq47();

static void (*const isr_table[32])() = {
    isr0,  isr1,  isr2,  isr3,  isr4,  isr5,  isr6,  isr7,
    isr8,  isr9,  isr10, isr11, isr12, isr13, isr14, isr15,
    isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
    isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
};

static void (*const irq_table[16])() = {
    irq32, irq33, irq34, irq35, irq36, irq37, irq38, irq39,
    irq40, irq41, irq42, irq43, irq44, irq45, irq46, irq47
};

void set_gate(int vec, uint64_t handler, uint8_t flags) {
    idt[vec] = (struct idt_entry){
        .offset_low  = handler & 0xFFFF,
        .selector    = KERNEL_CS,
        .ist         = 0,
        .type_attr   = flags,
        .offset_mid  = (handler >> 16) & 0xFFFF,
        .offset_high = (handler >> 32) & 0xFFFFFFFF,
        .zero        = 0
    };
}

static void load_ex(void) {
    for (int i = 0; i < 32; i++) {
        set_gate(i, (uint64_t)isr_table[i], INT_GATE);
    }
}

static void load_irqs(void) {
    for (int i = 0; i < 16; i++) {
        set_gate(i + 32, (uint64_t)irq_table[i], INT_GATE);
    }
}

void idt_init(void) {
    for (int i = 0; i < IDT_ENTRIES; i++) {
        set_gate(i, 0, 0);
    }

    load_ex();
    load_irqs();

    idtp = (struct idt_ptr){
        .limit = sizeof(idt) - 1,
        .base  = (uint64_t)&idt
    };

    __asm__ volatile ("lidt %0" : : "m"(idtp));
}