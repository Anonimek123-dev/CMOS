extern isr_handler

section .text

global isr_common_stub

; =====================================================
; EXPORTY
; =====================================================

%assign i 0
%rep 32
global isr%+i
%assign i i+1
%endrep

%assign i 32
%rep 16
global irq%+i
%assign i i+1
%endrep

; =====================================================
; ISR BEZ ERROR CODE
; =====================================================

%macro ISR_NOERR 1
isr%+%1:
    push 0              ; fake error code
    push %1             ; vector
    jmp isr_common_stub
%endmacro

; =====================================================
; ISR Z ERROR CODE
; CPU już wrzucił error_code
; =====================================================

%macro ISR_ERR 1
isr%+%1:
    push %1             ; vector
    jmp isr_common_stub
%endmacro

; =====================================================
; IRQ
; =====================================================

%macro IRQ_STUB 1
irq%+%1:
    push 0
    push %1
    jmp isr_common_stub
%endmacro

; =====================================================
; EXCEPTIONS
; =====================================================

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7

ISR_ERR   8

ISR_NOERR 9

ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14

ISR_NOERR 15
ISR_NOERR 16

ISR_ERR   17

ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20

ISR_ERR   21

ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

; =====================================================
; IRQ 32-47
; =====================================================

IRQ_STUB 32
IRQ_STUB 33
IRQ_STUB 34
IRQ_STUB 35
IRQ_STUB 36
IRQ_STUB 37
IRQ_STUB 38
IRQ_STUB 39
IRQ_STUB 40
IRQ_STUB 41
IRQ_STUB 42
IRQ_STUB 43
IRQ_STUB 44
IRQ_STUB 45
IRQ_STUB 46
IRQ_STUB 47

; =====================================================
; COMMON STUB
; =====================================================

isr_common_stub:

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

    ; rdi = vector
    mov rdi, [rsp + 15*8]

    ; rsi = error_code
    mov rsi, [rsp + 16*8]

    ; rdx = interrupt_frame_t*
    lea rdx, [rsp + 17*8]

    call isr_handler

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

    add rsp, 16         ; vector + error code

    iretq