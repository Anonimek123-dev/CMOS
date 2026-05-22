; isr.asm - compact ISR/IRQ stubs for x86_64 (bare-metal)
extern isr_handler

section .text
global isr_common_stub

; Export all ISR/IRQ labels
%assign i 0
%rep 48
%if i < 32
    global isr%+i
%else
    global irq%+i
%endif
%assign i i+1
%endrep

; Macro for saving registers and jumping to common stub
%macro ISR_STUB 1
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rdi
    push rsi
    push rbp
    push rbx
    push rdx
    push rcx
    push rax
    push %1
    jmp isr_common_stub
%endmacro

; Generate all ISR/IRQ stubs
%assign i 0
%rep 48
%if i < 32
isr%+i: ISR_STUB i
%else
irq%+i: ISR_STUB i
%endif
%assign i i+1
%endrep

; Common handler stub
isr_common_stub:
    mov rdi, [rsp]      ; vector -> rdi
    call isr_handler
    add rsp, 8          ; remove vector
    pop rax
    pop rcx
    pop rdx
    pop rbx
    pop rbp
    pop rsi
    pop rdi
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    iretq