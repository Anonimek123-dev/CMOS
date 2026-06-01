#include "H/HAL/IO/print.h"
#include "H/HAL/Drivers/PS2/keyboard.h"
#include "H/HAL/IO/input.h"
#include <stdbool.h>

static char buffer[LINE_BUF_SIZE];
static int line_len = 0;
static int cursor_pos = 0;
static int start_col;

static char history[HISTORY_SIZE][LINE_BUF_SIZE];
static int history_len = 0;   // History Total Length
static int history_pos = -1;  // Current Position in History

static int blink_state = 0;   // Blinker State
static int blink_counter = 0; // "Ticks" counter

static bool line_ready = false;
static bool input_enabled = false;

static inline void sanit_cursor() {
    if (line_len < 0) line_len = 0;
    if (line_len > LINE_BUF_SIZE - 1) line_len = LINE_BUF_SIZE - 1;
    if (cursor_pos < 0) cursor_pos = 0;
    if (cursor_pos > line_len) cursor_pos = line_len;
}

void kb_update() {
    if (!input_enabled)
        return;

    int ci = kb_getchar();
    if (ci < 0) {
        blink_counter++;
        if (blink_counter > BLINK_THRESHOLD) {
            blink_counter = 0;
            blink_state = !blink_state;
            draw_cursor(start_col, cursor_pos, row, blink_state);
        }
        return;
    }

    int c = ci & 0xFF;

    // ENTER
    if (c == '\n') {
        buffer[line_len] = '\0';
        line_ready = true;

        // Add to history
        if (line_len > 0) {
            if (history_len < HISTORY_SIZE) {
                for (int i = 0; i <= line_len; i++)
                    history[history_len][i] = buffer[i];
                history_len++;
            } else {
                // Move history up
                for (int h = 1; h < HISTORY_SIZE; h++)
                    for (int i = 0; i <= LINE_BUF_SIZE; i++)
                        history[h-1][i] = history[h][i];
                for (int i = 0; i <= line_len; i++)
                    history[HISTORY_SIZE-1][i] = buffer[i];
            }
        }
        history_pos = -1;

        col = 0;
        row++;
        start_col = 0;
        sanit_cursor();
    }

    // BACKSPACE
    else if (c == '\b') {
        if (cursor_pos > 0) {
            for (int i = cursor_pos - 1; i < line_len - 1; i++)
                buffer[i] = buffer[i + 1];

            cursor_pos--;
            line_len--;
            buffer[line_len] = '\0';
            sanit_cursor();

            print_set_cursor(start_col + cursor_pos, row);
            for (int i = cursor_pos; i < line_len; i++)
                print_char(buffer[i]);
            print_char(' ');
            print_set_cursor(start_col + cursor_pos, row);
            draw_cursor(start_col, cursor_pos, row, blink_state);
        }
    }

    // LEFT ARROW
    else if (ci == KEY_LEFT) {
        cursor_pos--;
        sanit_cursor();
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }

    // RIGHT ARROW
    else if (ci == KEY_RIGHT) {
        cursor_pos++;
        sanit_cursor();
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }

    // UP ARROW (history)
    else if (ci == KEY_UP) {
        if (history_len > 0) {
            if (history_pos == -1)
                history_pos = history_len - 1;
            else if (history_pos > 0)
                history_pos--;

            int old_len = line_len;

            for (int i = 0; i <= LINE_BUF_SIZE; i++)
                buffer[i] = history[history_pos][i];
            line_len = 0;
            while (buffer[line_len] != '\0') line_len++;
            cursor_pos = line_len;
            sanit_cursor();

            print_set_cursor(start_col, row);
            for (int i = 0; i < old_len; i++) print_char(' ');
            print_set_cursor(start_col, row);
            for (int i = 0; i < line_len; i++) print_char(buffer[i]);
            print_set_cursor(start_col + cursor_pos, row);
            draw_cursor(start_col, cursor_pos, row, blink_state);
        }
    }

    // DOWN ARROW (history)
    else if (ci == KEY_DOWN) {
        if (history_pos != -1) {
            int old_len = line_len;

            if (history_pos < history_len - 1)
                history_pos++;
            else
                history_pos = -1;

            if (history_pos == -1) {
                line_len = 0;
                cursor_pos = 0;
                buffer[0] = '\0';
            } else {
                for (int i = 0; i <= LINE_BUF_SIZE; i++)
                    buffer[i] = history[history_pos][i];
                line_len = 0;
                while (buffer[line_len] != '\0') line_len++;
                cursor_pos = line_len;
            }
            sanit_cursor();

            print_set_cursor(start_col, row);
            for (int i = 0; i < old_len; i++) print_char(' ');
            print_set_cursor(start_col, row);
            for (int i = 0; i < line_len; i++) print_char(buffer[i]);
            print_set_cursor(start_col + cursor_pos, row);
            draw_cursor(start_col, cursor_pos, row, blink_state);
        }
    }

    // DELETE
    else if (ci == KEY_DELETE) {
        if (cursor_pos >= line_len) return;  // nic do usunięcia
        for (int i = cursor_pos; i < line_len - 1; i++)
            buffer[i] = buffer[i + 1];
        line_len--;
        buffer[line_len] = '\0';
        sanit_cursor();

        print_set_cursor(start_col + cursor_pos, row);
        for (int i = cursor_pos; i < line_len; i++)
            print_char(buffer[i]);
        print_char(' ');
        print_set_cursor(start_col + cursor_pos, row);
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }

    // HOME
    else if (ci == KEY_HOME) {
        cursor_pos = 0;
        sanit_cursor();
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }

    // END
    else if (ci == KEY_END) {
        cursor_pos = line_len;
        sanit_cursor();
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }

    // TAB 
    else if (c == '\t') {
    int spaces = TAB_SIZE;
    if (line_len + spaces >= LINE_BUF_SIZE)
        spaces = LINE_BUF_SIZE - 1 - line_len;

    if (spaces <= 0) return;

    // text ->
    for (int i = line_len - 1; i >= cursor_pos; i--)
        buffer[i + spaces] = buffer[i];

    // Spaces
    for (int s = 0; s < spaces; s++)
        buffer[cursor_pos + s] = ' ';

    line_len += spaces;
    cursor_pos += spaces;
    buffer[line_len] = '\0';
    sanit_cursor();

    // Refresh line
    print_set_cursor(start_col, row);
    for (int i = 0; i < line_len; i++)
        print_char(buffer[i]);
    print_char(' ');
    print_set_cursor(start_col + cursor_pos, row);
    draw_cursor(start_col, cursor_pos, row, blink_state);
    return;
}


// NORMAL CHARACTERS
else {     
    if (line_len < LINE_BUF_SIZE - 1) {
        for (int i = line_len; i > cursor_pos; i--)
            buffer[i] = buffer[i - 1];

        buffer[cursor_pos] = (char)c;
        line_len++;
        cursor_pos++;
        sanit_cursor();

        print_set_cursor(start_col + cursor_pos - 1, row);
        for (int i = cursor_pos - 1; i < line_len; i++)
            print_char(buffer[i]);

        print_set_cursor(start_col + cursor_pos, row);
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }
}

    // BLINK
    blink_counter++;
    if (blink_counter > BLINK_THRESHOLD) {
        blink_counter = 0;
        blink_state = !blink_state;
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }
}

bool input_ready() {
    return line_ready;
}

char* input_getline() {
    static char line[LINE_BUF_SIZE];
    for (int i = 0; i <= line_len; i++)
        line[i] = buffer[i];
    line_ready = false;
    line_len = 0;
    cursor_pos = 0;
    buffer[0] = '\0';
    return line;
}

char* readline() {
    start_col = col;    
    input_enable();
    while (!input_ready()) {
        kb_update();
    }
    input_disable();
    return input_getline();
}

void input_enable() {
    input_enabled = true;
}

void input_disable() {
    input_enabled = false;
}