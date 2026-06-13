# Official changelog for `CMOS pre-alpha version 0.0.5`

## WHAT'S NEW????

### 1. Added Memory Map Manager (MMM)

Implemented the first Memory Map Manager responsible for processing memory information provided by the bootloader.

Features:

* memory map parsing
* usable memory detection
* reserved region detection
* memory statistics
* integration with PMM initialization

The MMM now serves as the foundation for all future memory management systems inside CMOS.

---

### 2. Added Physical Memory Manager (PMM)

Implemented a fully functional page-based physical memory allocator.

Features:

* 4 KB page allocation
* page freeing
* free page tracking
* used page tracking
* memory usage statistics

New functions:

* `pmm_init()`
* `pmm_alloc_page()`
* `pmm_free_page()`
* `pmm_info()`

This is the first real dynamic memory subsystem in CMOS.

---

### 3. Added x86_64 Paging System

Implemented a complete paging subsystem for long mode memory management.

Features:

* PML4 support
* PDPT support
* Page Directory support
* Page Table support
* dynamic page table creation
* page mapping
* page unmapping
* address translation

New functions:

* `paging_init()`
* `paging_enable()`
* `map_page()`
* `unmap_page()`
* `virt_to_phys()`
* `paging_info()`

The paging system is now capable of dynamically expanding virtual memory space.

---

### 4. Added Virtual Memory Manager (VMM)

Implemented CMOS Virtual Memory Manager.

Features:

* virtual page allocation
* virtual page freeing
* virtual-to-physical integration
* rollback protection on allocation failure
* free region tracking
* free region reuse
* free region merging

New functions:

* `vmm_init()`
* `vmm_alloc_pages()`
* `vmm_free_pages()`
* `vmm_info()`

The VMM now sits directly above PMM and Paging, creating a complete memory management stack.

---

### 5. Added Kernel Heap Allocator

Implemented the first dynamic kernel heap.

Features:

* dynamic memory allocation
* dynamic memory freeing
* block splitting
* block merging
* free block reuse
* heap statistics
* aligned allocations

New functions:

* `heap_init()`
* `kmalloc()`
* `kfree()`
* `kcalloc()`
* `heap_info()`

The kernel can now allocate memory dynamically during runtime.

---

### 6. Expanded String Library

Added several new utility functions to `string.c`.

New functions:

* `strcmp()`
* `strlen()`
* `strcpy()`
* `strncpy()`
* `memcpy()`
* `memset()`

These functions are now used internally by multiple CMOS subsystems including heap allocation.

---

### 7. Added Unsigned Integer Conversion Support

Extended the integer conversion subsystem.

New functions:

* `utoa()`
* `utoa_base()`

Supported bases:

* decimal
* hexadecimal
* octal
* binary

This improves formatted output support and future debugging tools.

---

### 8. Improved Memory Debugging

Added detailed information screens for:

* MMM
* PMM
* Paging
* VMM
* Heap

Each subsystem can now report:

* memory usage
* allocation statistics
* internal state
* diagnostic information

This greatly improves kernel debugging.

---

### 9. Major Memory Architecture Milestone

CMOS now contains a complete memory stack:

```text
Memory Map Manager
        ↓
Physical Memory Manager
        ↓
Paging
        ↓
Virtual Memory Manager
        ↓
Kernel Heap
```

This is the first major architecture milestone of the project.

---

### 10. Internal Cleanup

* Improved code organization.
* Added memory subsystem separation.
* Removed temporary allocation logic.
* Improved page management architecture.
* Improved allocator reliability.
* Added rollback protection for failed allocations.
* Reduced memory leaks during allocation failures.

---

## NEXT TARGET

### pre-alpha 0.0.6

* [ ] PSF 1
* [ ] New folder structure
* [ ] Code Page 437 etc.
* [ ] set_window_size() function
* [ ] Other possible issues relating to fonts
* [ ] String Utils [Part II]

---

**Author:** CMOS Team
**Module:** 'CHANGELOG.md'
**Last Updated:** 13 June 2026
