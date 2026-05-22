#pragma once

#include <stddef.h>

void kb_update(void);
int kb_getchar(void);
void draw_cursor(int start_col, int cursor_pos, int row, int blink_state);

extern void enable_irq(void);
extern size_t row; 
extern size_t col;   

#define LINE_BUF_SIZE 128
#define HISTORY_SIZE 16
#define TAB_SIZE 4
#define BLINK_THRESHOLD 20000