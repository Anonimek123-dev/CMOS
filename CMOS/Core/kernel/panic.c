#include "H/Core/kernel/panic.h"
#include "H/HAL/IO/stdio.h"
#include "H/Core/string.h"

void kernel_panic(const char* reason, interrupt_frame_t* frame, uint64_t error_code) {
    uint64_t cr2 = 0;

    __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));

    set_text_color(WHITE, RED);
    print_clr();

    printf("\n");
    printf("========================================\n");
    printf("           CMOS KERNEL PANIC            \n");
    printf("========================================\n\n");

    printf("Exception     : %s\n", reason);
    printf("Error Code    : 0x%llx\n", error_code);

    printf("RIP           : %p\n", (void*)frame->rip);
    printf("CS            : %p\n", (void*)frame->cs);
    printf("RFLAGS        : %p\n", (void*)frame->rflags);
    printf("RSP           : %p\n", (void*)frame->rsp);

    printf("CR2           : %p\n", (void*)cr2);

    if (reason && strcmp(reason, "Page Fault") == 0) {

    printf("\nCause:\n");

    if (error_code & 1)
        printf(" - Protection Violation\n");
    else
        printf(" - Non-Present Page\n");

    if (error_code & 2)
        printf(" - Write\n");
    else
        printf(" - Read\n");

    if (error_code & 4)
        printf(" - User Mode\n");
    else
        printf(" - Kernel Mode\n");

    if (error_code & 8)
        printf(" - Reserved Bit Violation\n");

    if (error_code & 16)
        printf(" - Instruction Fetch\n");
    }

    printf("\nSystem halted.\n");

    while (1) {
        __asm__ volatile("cli");
        __asm__ volatile("hlt");
    }
}