#include "H/HAL/IO/stdio.h"

void putchar(char c) {
    print_char(c);
}

void puts(const char* str) {
    print_str((char*)str);
}

void set_text_color(uint8_t fg, uint8_t bg) {
    print_set_color(fg, bg);
}

void test(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int x = va_arg(args, int);
    print_int(x);
    va_end(args);
}

int str_len(const char* str) {
    int len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

void print_padding(int current_len, int width, char pad) {
    while (current_len < width) {
        print_char(pad);
        current_len++;
    }
}

int printf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int result = vprintf(fmt, args);
    va_end(args);
    return result;
}

int printf_color(uint8_t fg, uint8_t bg, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    uint8_t old_fg = WHITE;
    uint8_t old_bg = BLACK;
    set_text_color(fg, bg);
    int result = vprintf(fmt, args);
    set_text_color(old_fg, old_bg);
    va_end(args);
    return result;
}

int vprintf(const char* fmt, va_list args) {
    char buffer[1024];
    int result = vsnprintf(buffer, sizeof(buffer), fmt, args);
    print_str(buffer);
    return result;
}

int snprintf(char* buffer, size_t size, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int result = vsnprintf(buffer, size, fmt, args);
    va_end(args);
    return result;
}

int vsprintf(char* buffer, const char* fmt, va_list args) {
    return vsnprintf(buffer, INT_MAX, fmt, args);
}

int sprintf(char* buffer, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int result = vsprintf(buffer, fmt, args);
    va_end(args);
    return result;
}

void buffer_putchar(char* buffer, size_t size, int* pos, char c) {
    if (size > 0 && (size_t)(*pos) < size - 1) {
        buffer[*pos] = c;
    }
    (*pos)++;
}

void buffer_padding(char* buffer, size_t size, int* pos, int current_len, int width, char pad) {
    while (current_len < width) {
        buffer_putchar(buffer, size, pos, pad);
        current_len++;
    }
}

void buffer_putstr(char* buffer, size_t size, int* pos, const char* str) {
    int i = 0;

    while (str[i]) {
        buffer_putchar(buffer, size, pos, str[i]);
        i++;
    }
}

void format_string(char* buffer, size_t size, int* pos, const char* str, int len, int width, int left_align, int zero_pad) {
    char pad = zero_pad ? '0' : ' ';

    if (left_align) {
        buffer_putstr(buffer, size, pos, str);
        buffer_padding(buffer, size, pos, len, width, ' ');
    } else {
        buffer_padding(buffer, size, pos, len, width, pad);
        buffer_putstr(buffer, size, pos, str);
    }
}

int vsnprintf(char* buffer, size_t size, const char* fmt, va_list args) {
    int pos = 0;

    while (*fmt) {
        if (*fmt != '%') {
            buffer_putchar(buffer, size, &pos, *fmt);
            fmt++;
            continue;
        }

        if (*fmt == '%') {
            fmt++;
            int zero_pad = 0;
            int width = 0; 
            int long_flag = 0;
            int longlong_flag = 0;
            int left_align = 0;
            
            if (*fmt == '-') {
                left_align = 1;
                fmt++;
            }

            if (*fmt == '0') {
                zero_pad = 1;
                fmt++;
            }
            
            while (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt - '0');
                fmt++;
            }               

            if (*fmt == 'l') {
            fmt++;

            if (*fmt == 'l') {
            longlong_flag = 1;
            fmt++;          

    } 
    else {
        long_flag = 1;
    }
}
            switch (*fmt) {

                case 'd': {
                    long long value;
                    
                    if (longlong_flag) {
                        value = va_arg(args, long long);
                    } else if (long_flag) {
                        value = va_arg(args, long);
                    } else {
                        value = va_arg(args, int);
                    }

                    char temp[64];
                    itoa_base(value, temp, 10);
                    int len = str_len(temp);

                    if (left_align) {
                        buffer_putstr(buffer, size, &pos, temp);
                        buffer_padding(buffer, size, &pos, len, width, ' ');
                    } else {
                        if (zero_pad && temp[0] == '-') {
                            buffer_putchar(buffer, size, &pos, '-');
                            buffer_padding(buffer, size, &pos, len - 1, width - 1, '0');
                            buffer_putstr(buffer, size, &pos, temp + 1);
                        } else {
                            if (zero_pad)
                                buffer_padding(buffer, size, &pos, len, width, '0');
                            else
                                buffer_padding(buffer, size, &pos, len, width, ' ');
                            buffer_putstr(buffer, size, &pos, temp);
                        }
                    }                     
                    break;
                }

                case 'x': {
                    unsigned int value = va_arg(args, unsigned int);
                    char temp[32];
                    itoa_base(value, temp, 16);
                    int len = str_len(temp);
                    format_string(buffer, size, &pos, temp, len, width, left_align, zero_pad);
                    break;
                }

                case 's': {
                    char* str = va_arg(args, char*);
                    if (!str)
                        str = "(null)";
                    int len = str_len(str);
                    format_string(buffer, size, &pos, str, len, width, left_align, zero_pad);
                    break;
                }

                case 'c': {
                    char c = (char)va_arg(args, int);
                    int len = 1;
                    if (left_align) {
                        buffer_putchar(buffer, size, &pos, c);
                        buffer_padding(buffer, size, &pos, len, width, ' ');
                    } else {
                        if (zero_pad)
                            buffer_padding(buffer, size, &pos, len, width, '0');
                        else
                            buffer_padding(buffer, size, &pos, len, width, ' ');
                        buffer_putchar(buffer, size, &pos, c);
                    }
                    break;
                }

                case '%': {
                    int len = 1;
                    if (left_align) {
                        buffer_putchar(buffer, size, &pos, '%');
                        buffer_padding(buffer, size, &pos, len, width, ' ');
                    } else {
                        if (zero_pad)
                            buffer_padding(buffer, size, &pos, len, width, '0');
                        else
                            buffer_padding(buffer, size, &pos, len, width, ' ');
                        buffer_putchar(buffer, size, &pos, '%');
                    }
                    break;
                }

                case 'b': {
                    unsigned int value = va_arg(args, unsigned int);
                    char temp[64];
                    itoa_base(value, temp, 2);
                    int len = str_len(temp);
                    format_string(buffer, size, &pos, temp, len, width, left_align, zero_pad);
                    break;
                }

               case 'o': {
                    unsigned int value = va_arg(args, unsigned int);
                    char temp[64];
                    itoa_base(value, temp, 8);
                    int len = str_len(temp);
                    format_string(buffer, size, &pos, temp, len, width, left_align, zero_pad);
                    break;
                }

                case 'u': {
                    unsigned int value = va_arg(args, unsigned int);
                    char temp[64];
                    itoa_base(value, temp, 10);
                    int len = str_len(temp);
                    format_string(buffer, size, &pos, temp, len, width, left_align, zero_pad);
                    break;
                }

                case 'p': {
                    unsigned long long value = (unsigned long long)(uintptr_t)va_arg(args, void*);
                    char temp[64];
                    itoa_base(value, temp, 16);
                    int len = str_len(temp) + 2; 

                    if (left_align) {
                        buffer_putchar(buffer, size, &pos, '0');
                        buffer_putchar(buffer, size, &pos, 'x');
                        buffer_putstr(buffer, size, &pos, temp);
                        buffer_padding(buffer, size, &pos, len, width, ' ');
                    } else {
                        char pad = zero_pad ? '0' : ' ';
                        buffer_padding(buffer, size, &pos, len, width, pad);
                        buffer_putchar(buffer, size, &pos, '0');
                        buffer_putchar(buffer, size, &pos, 'x');
                        buffer_putstr(buffer, size, &pos, temp);
                    }
                    break;
                }

                default: {
                    buffer_putchar(buffer, size, &pos, '%');
                    buffer_putchar(buffer, size, &pos, *fmt);
                    break;
                }
            }
        }
        fmt++;        
    }

    if (size > 0) {
        if (pos >= (int)size)
            buffer[size - 1] = '\0';
        else
            buffer[pos] = '\0';
    }
    return pos; 
}

char getchar() {
    int c = -1;
    while (c == -1) {
        c = kb_getchar();
    }
    return (char)c;
}

int scanf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int result = vscanf(fmt, args);
    va_end(args);
    return result;
}

int vscanf(const char* fmt, va_list args) {
    char* line = readline();
    return vsscanf(line, fmt, args);
}

int vsscanf(const char* buffer, const char* fmt, va_list args) {
    int assigned = 0;

    while (*fmt) {

        if (*fmt == ' ') {
            while (*buffer == ' ' || *buffer == '\t' || *buffer == '\n')
            {
                buffer++;
            }
            fmt++;
            continue;
        }

        if (*fmt != '%') {
            if (*fmt != *buffer)
                return assigned;

            fmt++;
            buffer++;
            continue;
        }
        fmt++;

        int width = 0;

        while (*fmt >= '0' && *fmt <= '9') {
            width = width * 10 + (*fmt - '0');
            fmt++;
        }

        int long_flag = 0;
        int longlong_flag = 0;

        if (*fmt == 'l') {
            fmt++;

            if (*fmt == 'l') {
                longlong_flag = 1;
                fmt++;
            } else {
                long_flag = 1;
            }
        }

        switch (*fmt) {

            case 's': {
                char* dst = va_arg(args, char*);

                while (*buffer == ' ' || *buffer == '\t' || *buffer == '\n') {
                    buffer++;
                }
                
                if (*buffer == '\0')
                    return assigned;

                int count = 0;

                while (*buffer && *buffer != ' ' && *buffer != '\n' && *buffer != '\t') {
                    if (width == 0 || count < width) {
                        *dst++ = *buffer;
                        count++;
                    }
                    buffer++;
                }

                *dst = '\0';
                assigned++;
                break;
            }

            case 'd': {
                long long value = 0;

                while (*buffer == ' ' || *buffer == '\t' || *buffer == '\n') {
                    buffer++;
                }

                int sign = 1;

                if (*buffer == '-') {
                    sign = -1;
                    buffer++;
                }

                else if (*buffer == '+') {
                    buffer++;
                }

                int found_digit = 0;

                while (*buffer >= '0' && *buffer <= '9') {
                    found_digit = 1;
                    int digit = *buffer - '0';

                    if (sign == 1) {

                        if (value > (LLONG_MAX - digit) / 10)
                            return assigned;

                        value = value * 10 + digit;
                    }
                    else {

                        if (value < (LLONG_MIN + digit) / 10)
                            return assigned;

                        value = value * 10 - digit;
                    }
                    buffer++;
                }

                if (!found_digit)
                    return assigned;

                if (longlong_flag) {
                    long long* dst = va_arg(args, long long*);
                    *dst = value;
                }
                else if (long_flag) {
                    long* dst = va_arg(args, long*);
                    *dst = (long)value;
                }
                else {
                    int* dst = va_arg(args, int*);
                    *dst = (int)value;
                }

                assigned++;
                break;
            }

            case 'c': {
                char* dst = va_arg(args, char*);

                if (*buffer == '\0')
                    return assigned;

                *dst = *buffer;
                buffer++;
                assigned++;
                break;
            }

            case '%': {
                if (*buffer != '%')
                    return assigned;

                buffer++;
                break;
            }

            case 'x': {
                unsigned int* dst = va_arg(args, unsigned int*);

                while (*buffer == ' ' || *buffer == '\t' || *buffer == '\n')
                    buffer++;

                unsigned int value = 0;
                int found_digit = 0;

                while (1) {
                    int digit;

                    if (*buffer >= '0' && *buffer <= '9')
                        digit = *buffer - '0';
                    else if (*buffer >= 'a' && *buffer <= 'f')
                        digit = *buffer - 'a' + 10;
                    else if (*buffer >= 'A' && *buffer <= 'F')
                        digit = *buffer - 'A' + 10;
                    else
                        break;

                    found_digit = 1;

                    if (value > (UINT_MAX - digit) / 16)
                        return assigned;

                    value = value * 16 + digit;
                    buffer++;
                }

                if (!found_digit)
                    return assigned;

                *dst = value;
                assigned++;
                break;
            }

            case 'b': {
                int* dst = va_arg(args, int*);

                while (*buffer == ' ' || *buffer == '\t' || *buffer == '\n')
                    buffer++;

                int value = 0;
                int found_digit = 0;

                while (*buffer == '0' || *buffer == '1') {
                    found_digit = 1;
                    value = (value << 1) + (*buffer - '0');
                    buffer++;
                }

                if (!found_digit)
                    return assigned;

                *dst = value;
                assigned++;
                break;
            }

            case 'u': {
                unsigned int* dst = va_arg(args, unsigned int*);

                while (*buffer == ' ' || *buffer == '\t' || *buffer == '\n')
                    buffer++;

                unsigned int value = 0;
                int found_digit = 0;

                while (*buffer >= '0' && *buffer <= '9') {
                    found_digit = 1;

                    unsigned int digit = *buffer - '0';

                    if (value > (UINT_MAX - digit) / 10)
                        return assigned;

                    value = value * 10 + digit;
                    buffer++;
                }

                if (!found_digit)
                    return assigned;

                *dst = value;
                assigned++;
                break;
            }

            case 'o': {
                int* dst = va_arg(args, int*);

                while (*buffer == ' ' || *buffer == '\t' || *buffer == '\n')
                    buffer++;

                int value = 0;
                int found_digit = 0;

                while (*buffer >= '0' && *buffer <= '7') {
                    found_digit = 1;

                    value = value * 8 + (*buffer - '0');
                    buffer++;
                }

                if (!found_digit)
                    return assigned;

                *dst = value;
                assigned++;
                break;
            }

            case 'p': {
                void** dst = va_arg(args, void**);

                while (*buffer == ' ' || *buffer == '\t' || *buffer == '\n')
                    buffer++;

                if (buffer[0] == '0' &&
                    (buffer[1] == 'x' || buffer[1] == 'X'))
                {
                    buffer += 2;
                }

                uintptr_t value = 0;
                int found_digit = 0;

                while (1) {
                    int digit;

                    if (*buffer >= '0' && *buffer <= '9')
                        digit = *buffer - '0';
                    else if (*buffer >= 'a' && *buffer <= 'f')
                        digit = *buffer - 'a' + 10;
                    else if (*buffer >= 'A' && *buffer <= 'F')
                        digit = *buffer - 'A' + 10;
                    else
                        break;

                    found_digit = 1;
                    value = value * 16 + digit;
                    buffer++;
                }

                if (!found_digit)
                    return assigned;

                *dst = (void*)value;
                assigned++;
                break;
            }
        }
        fmt++;
    }
    return assigned;
}