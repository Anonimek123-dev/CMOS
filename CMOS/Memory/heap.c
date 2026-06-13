#include "H/Memory/heap.h"
#include "H/Memory/vmm.h"
#include "H/HAL/IO/stdio.h"
#include "H/Core/string.h"

typedef struct heap_block
{
    uint64_t size;
    uint8_t free;

    struct heap_block* next;
    struct heap_block* prev;

} heap_block_t;

static heap_block_t* heap_head = 0;

static void split_block(heap_block_t* block, uint64_t size);
static void merge_block(heap_block_t* block);

static uint64_t heap_total = 0;
static uint64_t heap_used = 0;

static uint64_t align8(uint64_t size)
{
    return (size + 7) & ~7ULL;
}

void heap_init(void)
{
    void* heap_mem = vmm_alloc_pages(16);
    heap_head = (heap_block_t*)heap_mem;
    heap_head->size = (16 * 4096) - sizeof(heap_block_t);
    heap_head->free = 1;
    heap_head->next = 0;
    heap_head->prev = 0;
    heap_total = 16 * 4096;
}

void* kmalloc(uint64_t size)
{
    size = align8(size);
    heap_block_t* current = heap_head;

    while(current)
    {
        if(current->free && current->size >= size)
        {
            if(current->size > size + sizeof(heap_block_t) + 8)
            {
                split_block(current, size);
            }
            current->free = 0;
            heap_used += current->size;
            return (void*) ((uint8_t*)current + sizeof(heap_block_t));
        }
        current = current->next;
    }
    return 0;
}

void kfree(void* ptr)
{
    if(!ptr)
        return;

    heap_block_t* block = (heap_block_t*) ((uint8_t*)ptr - sizeof(heap_block_t));

    if(block->free)
        return;

    block->free = 1;

    if(heap_used >= block->size)
        heap_used -= block->size;

    merge_block(block);
}

void* kcalloc(uint64_t count, uint64_t size)
{
    uint64_t total = count * size;
    void* ptr = kmalloc(total);
    if(!ptr)
        return 0;
    memset(ptr, 0, total);
    return ptr;
}

void heap_info(void)
{
    printf("\n");

    print_set_color(LIGHT_CYAN, BLACK);
    printf("+--------------------------------+\n");
    printf("|           HEAP INFO            |\n");
    printf("+--------------------------------+\n");

    print_set_color(YELLOW, BLACK);
    printf("Heap Base: ");
    print_hex((uint64_t)heap_head);
    printf("\n");

    printf("Heap Size: ");
    print_int(heap_total / 1024);
    printf(" KB\n");


    printf("Heap Used : ");
    print_int(heap_used / 1024);
    printf(" KB\n");

    printf("Heap Free : ");
    print_int((heap_total - heap_used) / 1024);
    printf(" KB\n");

    printf("Heap State: ");
    printf("\nBlocks:\n");

    heap_block_t* current = heap_head;
    int id = 0;

    while(current)
    {
        printf("[");
        print_int(id);
        printf("] ");

        if(current->free)
            printf_color(LIGHT_GREEN, BLACK, "FREE ");
        else
            printf_color(LIGHT_RED, BLACK, "USED ");

        printf("Size: ");
        print_int(current->size);
        printf(" bytes\n");
        current = current->next;
        id++;
    }

    printf("\n");
    printf_color(LIGHT_CYAN, BLACK, "+--------------------------------+");
}

static void split_block(heap_block_t* block, uint64_t size)
{
    heap_block_t* new_block = (heap_block_t*) ((uint8_t*)block + sizeof(heap_block_t) + size);
    new_block->size = block->size - size - sizeof(heap_block_t);
    new_block->free = 1;
    new_block->next = block->next;
    new_block->prev = block;
    if(new_block->next)
        new_block->next->prev = new_block;
    block->next = new_block;
    block->size = size;
}

static void merge_block(heap_block_t* block)
{
    if(block->next && block->next->free)
    {
        block->size += sizeof(heap_block_t) + block->next->size;
        block->next = block->next->next;
        if(block->next)
            block->next->prev = block;
    }

    if(block->prev && block->prev->free)
    {
        merge_block(block->prev);
    }
}