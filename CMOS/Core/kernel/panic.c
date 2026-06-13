#include "H/Core/kernel/panic.h"
#include "H/HAL/IO/stdio.h"

void kernel_panic(const char* reason, interrupt_frame_t* frame, uint64_t error_code)
{
    uint64_t cr2 = 0;

    asm volatile("mov %%cr2, %0" : "=r"(cr2));

    print_set_color(WHITE, RED);
    print_clr();    

    printf("\n");
    printf("+--------------------------------------------+\n");
    printf("|              CMOS KERNEL PANIC             |\n");
    printf("+--------------------------------------------+\n\n");

    printf(" Reason     : %s\n", reason);

    printf(" Error Code : ");
    print_hex(error_code);
    printf("\n");

    printf(" RIP        : ");
    print_hex(frame->rip);
    printf("\n");

    printf(" CS         : ");
    print_hex(frame->cs);
    printf("\n");

    printf(" RFLAGS     : ");
    print_hex(frame->rflags);
    printf("\n");

    printf(" RSP        : ");
    print_hex(frame->rsp);
    printf("\n");

    printf(" CR2        : ");
    print_hex(cr2);
    printf("\n");

    // Page Fault decode
    if(error_code || cr2)
    {
        printf("\n");
        printf("+--------------------------------------------+\n");
        printf("|                 PAGE FAULT                 |\n");
        printf("+--------------------------------------------+\n");

        printf(" Present    : ");
        printf((error_code & 1) ? "YES" : "NO");
        printf("\n");

        printf(" Write      : ");
        printf((error_code & 2) ? "YES" : "NO");
        printf("\n");

        printf(" User       : ");
        printf((error_code & 4) ? "YES" : "NO");
        printf("\n");

        printf(" Reserved   : ");
        printf((error_code & 8) ? "YES" : "NO");
        printf("\n");

        printf(" InstrFetch : ");
        printf((error_code & 16) ? "YES" : "NO");
        printf("\n");
    }

    printf("\n");
    printf("System halted...");

    while(1)
    {
        asm volatile("cli : hlt");
    }
}