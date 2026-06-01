#ifndef PIC_H
#define PIC_H

void pic_remap(int offset1, int offset2);
void pic_eoi(unsigned char irq);

#endif