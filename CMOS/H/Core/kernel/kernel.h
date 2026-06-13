#pragma once

#include "H/HAL/IO/stdio.h"
#include <stdint.h>

void kernel_init(void);
void hardwaresetup(void);
void kernel_update(void);
void idt_init(void);            
void pic_remap(int offset1, int offset2);      
void kb_init(void);                                    
void tm_init(void);   
void enable_irq(void);
void kb_update(void);
void memorysetup(uint64_t multiboot_addr);