# Official changelog for `CMOS pre-alpha version 0.0.4`

## WHAT'S NEW????

### 1. Complete stdio subsystem rewrite

* Reworked large parts of `stdio.c`.
* Added a proper formatted output layer similar to the C standard library.
* Introduced wrapper functions for easier kernel-side text output.
* Improved separation between low-level VGA printing and high-level formatting logic.

New functions include:

* `printf()`
* `vprintf()`
* `sprintf()`
* `vsprintf()`
* `snprintf()`
* `vsnprintf()`
* `scanf()`
* `vscanf()`
* `vsscanf()`
* `printf_color()`

This update significantly improves debugging capabilities and general kernel usability.

---

### 2. Added formatted printing support

Implemented support for multiple format specifiers:

* `%d`
* `%ld`
* `%lld`
* `%u`
* `%x`
* `%o`
* `%b`
* `%p`
* `%s`
* `%c`
* `%%`

Added support for:

* field width
* left alignment (`%-`)
* zero padding (`%0`)
* signed and unsigned integer formatting

This is the first major step toward a libc-like environment inside CMOS.

---

### 3. Added formatted input system

Implemented the first version of a scanning subsystem.

Supported specifiers:

* `%d`
* `%ld`
* `%lld`
* `%u`
* `%x`
* `%o`
* `%b`
* `%s`
* `%c`
* `%%`

Features:

* whitespace handling
* width limits
* signed integer parsing
* hexadecimal parsing
* binary parsing
* octal parsing
* pointer parsing

This allows CMOS to receive structured user input for future shell development.

---

### 4. Added integer conversion library

Created:

* `itoa.c`
* `itoa.h`

Features:

* decimal conversion
* hexadecimal conversion
* octal conversion
* binary conversion
* signed integer support
* unsigned integer support

The entire formatted output system now relies on internal conversion routines instead of hardcoded printing logic.

---

### 5. Added first string library

Created:

* `string.c`
* `string.h`

Current functions:

* `strcmp()`

This introduces the foundation for a future CMOS libc implementation.

---

### 6. Added kernel panic screen

Implemented the first real kernel panic system.

Features:

* full-screen panic display
* automatic screen clearing
* red panic background
* exception reporting
* register dumping
* system halt after fatal errors

Displayed information:

* Exception Name
* Error Code
* RIP
* CS
* RFLAGS
* RSP
* CR2

This dramatically improves debugging of crashes and memory faults.

---

### 7. Added exception handling framework

Created:

* `panic.c`
* `panic.h`
* `exceptions.c`
* `exceptions.h`

Added:

* exception name table
* centralized exception dispatcher
* integration with the panic screen

The architecture is now prepared for advanced exception diagnostics.

---

### 8. Improved kernel boot output

`kernel_init()` now uses:

```c
printf_color(YELLOW, BLACK,
    "CMOS booted successfully!\n");
```

instead of older direct printing methods.

Benefits:

* cleaner code
* unified output API
* easier future logging integration

---

### 9. Internal cleanup

* Improved code organization.
* Added new Core utility modules.
* Expanded low-level helper functions.
* Simplified future development of memory management and shell subsystems.
* Reduced duplicated printing logic throughout the kernel.

---

## NEXT TARGET

The next major milestone is:

* Physical Memory Manager (PMM)
* Heap allocator (`kmalloc`, `kfree`)

---

**Author:** CMOS Kernel Team
**Version:** pre-alpha 0.0.4
**Last Updated:** 1 June 2026