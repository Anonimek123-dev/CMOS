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

# Memory Management

CMOS now includes the first complete memory management stack.

Current architecture:

```text
PMM
 ↓
Paging
 ↓
VMM
 ↓
Heap
 ↓
kmalloc / kcalloc / kfree
```

This architecture allows CMOS to manage physical memory, virtual memory, and dynamic kernel allocations.

---

# Physical Memory Manager (PMM)

The Physical Memory Manager is responsible for managing physical RAM pages.

Current page size:

```text
4096 bytes (4 KB)
```

## PMM Functions

### `pmm_alloc_page()`

Allocates a single physical page.

Example:

```c
void* page = pmm_alloc_page();
```

Returns a pointer to a free physical page.

---

### `pmm_free_page()`

Frees a previously allocated physical page.

Example:

```c
pmm_free_page(page);
```

---

### `pmm_info()`

Displays PMM statistics.

Example:

```c
pmm_info();
```

Information displayed:

```text
Total Memory
Free Pages
Used Pages
Reserved Pages
```

---

# Paging System

CMOS includes a dynamic x86_64 paging subsystem.

Features:

* PML4 creation
* PDPT creation
* PD creation
* PT creation
* Dynamic page mapping
* Dynamic page unmapping
* TLB invalidation

---

## `map_page()`

Maps a virtual address to a physical address.

Example:

```c
map_page(
    virt_addr,
    phys_addr,
    PAGE_WRITABLE
);
```

---

## `unmap_page()`

Removes an existing page mapping.

Example:

```c
unmap_page(virt_addr);
```

---

## `virt_to_phys()`

Translates a virtual address into a physical address.

Example:

```c
uint64_t phys =
    virt_to_phys(virt_addr);
```

Returns:

```c
0
```

if the page is not mapped.

---

## `paging_info()`

Displays paging statistics.

Example:

```c
paging_info();
```

Information displayed:

```text
PML4 Address
PDPT Address
PD Address
PT Address
Mapped Pages
Mapped Memory
Paging State
```

---

# Virtual Memory Manager (VMM)

The Virtual Memory Manager sits above Paging.

Responsibilities:

* Virtual address allocation
* Page mapping
* Page freeing
* Free-region reuse
* Region merging

---

## `vmm_alloc_pages()`

Allocates multiple virtual pages.

Example:

```c
void* ptr =
    vmm_alloc_pages(4);
```

Allocates:

```text
4 pages = 16 KB
```

---

## `vmm_free_pages()`

Frees previously allocated virtual pages.

Example:

```c
vmm_free_pages(ptr, 4);
```

---

## `vmm_info()`

Displays VMM statistics.

Example:

```c
vmm_info();
```

Information displayed:

```text
Base Address
Next Free Address
Allocated Pages
Allocated Memory
Free Regions
```

---

# Heap Allocator

CMOS includes a kernel heap allocator built on top of VMM.

The heap currently uses:

```text
Doubly Linked Free List
```

Features:

* Block splitting
* Block merging
* Free block reuse
* 8-byte alignment
* Dynamic allocation
* Dynamic freeing

---

## `kmalloc()`

Allocates memory from the kernel heap.

Example:

```c
void* ptr =
    kmalloc(128);
```

Allocates:

```text
128 bytes
```

Returns a pointer to usable memory.

---

## `kfree()`

Frees memory allocated with kmalloc().

Example:

```c
kfree(ptr);
```

After freeing:

* adjacent blocks are merged
* fragmentation is reduced

---

## `kcalloc()`

Allocates memory and automatically fills it with zeros.

Example:

```c
uint8_t* buffer =
    kcalloc(256, 1);
```

Equivalent to:

```c
kmalloc(256);
memset(buffer, 0, 256);
```

---

## `heap_info()`

Displays heap statistics.

Example:

```c
heap_info();
```

Information displayed:

```text
Heap Base
Heap Size
Heap Used
Heap Free
```

Additionally displays heap blocks:

```text
[0] USED Size: 32
[1] FREE Size: 64
[2] USED Size: 128
```

This is useful for debugging fragmentation and allocator behavior.

---

# Memory Allocation Flow

Example allocation sequence:

```c
void* a = kmalloc(32);
void* b = kmalloc(64);

kfree(a);

void* c = kmalloc(16);
```

Flow:

```text
kmalloc()
 ↓
Heap
 ↓
VMM
 ↓
Paging
 ↓
PMM
 ↓
Physical RAM
```
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
* Memory Managment