# Welcome to CMOS Documentation!

## How do you use CMOS?

CMOS is still in its early stages of development, but it already contains several useful subsystems. The operating system currently provides VGA text output, PS/2 keyboard input, timer functionality, a basic string library, formatted I/O, and a kernel panic system.

Although CMOS is still far from being feature-complete, many of the foundations required for future development are already in place.

---

# Input and Output (I/O)

The I/O subsystem is one of the most developed parts of CMOS at the moment. It allows displaying information on the screen, changing colours, reading user input, and formatting data similarly to the standard C library.

## Low-Level Output Functions

### `print_char()`

Displays a single character on the screen.

Example:

```c
print_char('A');
```

---

### `print_str()`

Displays an entire null-terminated string.

Example:

```c
print_str("Hello World!");
```

---

### `print_set_color()`

Changes the current text and background colour.

Example:

```c
print_set_color(YELLOW, BLACK);
```

The first parameter specifies the text colour and the second specifies the background colour.

---

### `print_clr()`

Clears the entire screen and resets the cursor position.

Example:

```c
print_clr();
```

This function is also used internally by the CMOS kernel panic system.

---

# Formatted Output

CMOS now includes a basic formatted output library similar to the standard C library.

## `printf()`

Prints formatted text directly to the screen.

Example:

```c
printf("Value = %d\n", 123);
```

---

## `printf_color()`

Works similarly to `printf()` but allows specifying text and background colours.

Example:

```c
printf_color(YELLOW, BLACK,
             "CMOS booted successfully!\n");
```

---

## `sprintf()`

Writes formatted text into a buffer.

Example:

```c
char buffer[64];

sprintf(buffer,
        "Score: %d",
        100);
```

---

## `snprintf()`

Safe version of `sprintf()` that limits the number of written characters.

Example:

```c
char buffer[64];

snprintf(buffer,
         sizeof(buffer),
         "Value: %d",
         123);
```

---

# Supported Format Specifiers

CMOS currently supports:

| Specifier | Description       |
| --------- | ----------------- |
| `%d`      | Signed integer    |
| `%ld`     | Long integer      |
| `%lld`    | Long long integer |
| `%u`      | Unsigned integer  |
| `%x`      | Hexadecimal       |
| `%o`      | Octal             |
| `%b`      | Binary            |
| `%p`      | Pointer           |
| `%s`      | String            |
| `%c`      | Character         |
| `%%`      | Percent sign      |

Example:

```c
printf("Dec: %d\n", 42);
printf("Hex: %x\n", 42);
printf("Bin: %b\n", 42);
```

---

# Input Functions

CMOS also contains a basic input subsystem.

## `getchar()`

Reads a single character from the keyboard.

Example:

```c
char c = getchar();
```

---

## `scanf()`

Reads formatted input.

Example:

```c
int value;

scanf("%d", &value);
```

---

# String Library

CMOS includes the beginning of its own string library.

## `strcmp()`

Compares two strings.

Example:

```c
if (strcmp("hello", "hello") == 0)
{
    printf("Equal");
}
```

---

# Time Measurement

CMOS currently uses the Programmable Interval Timer (PIT) for timekeeping.

## `tm_uptime_ms()`

Returns system uptime in milliseconds.

Example:

```c
uint64_t uptime = tm_uptime_ms();
```

---

## `tm_print_uptime()`

Prints uptime in:

```text
HH:MM:SS.ms
```

format.

---

## `tm_sleep()`

Pauses execution for a specified number of milliseconds.

Example:

```c
tm_sleep(1000);
```

---

## `set_timeout()`

Schedules a function to be executed after a specified time.

Example:

```c
set_timeout(hello, 45000);
```

This will execute `hello()` 45 seconds after system startup.

---

# Kernel Panic System

CMOS includes a built-in kernel panic system for debugging fatal errors.

When an exception occurs, CMOS displays:

* Exception name
* Error code
* RIP
* RSP
* RFLAGS
* CR2 (for page faults)

and immediately halts the system.

This greatly simplifies kernel debugging.

---

# Summary

CMOS is still in active development, but it already includes:

* VGA text output
* Formatted printing
* Formatted input
* PS/2 keyboard support
* Timer subsystem
* String library
* Kernel panic screen
* CPU exception handling

Future releases will introduce memory management, a heap allocator, multitasking, and the CMOS shell.
