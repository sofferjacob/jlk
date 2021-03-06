// kheap.c -- Kernel heap functions, also provides
//            a placement malloc() for use before the heap is 
//            initialised.
//            Written for JamesM's kernel development tutorials.

#include "kheap.h"
#include <heap.h>
#include <hal.h>

// end is defined in the linker script.
extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;
extern heap_t* kheap;
extern page_directory_t* kernel_directory;

uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys)
{
    if (kheap != 0)
    {
        void *addr = alloc(sz, (uint8_t)align, kheap);
        if (phys != 0)
        {
            page_t *page = get_page((uint32_t)addr, 0, kernel_directory);
            *phys = page->frame * 0x1000 + (uint32_t)addr & 0xFFF;
        }
        return (uint32_t)addr;
    }
    else
    {
        if (align == 1 && (placement_address & 0xFFFFF000))
        {
            // Align the placement address;
            placement_address &= 0xFFFFF000;
            placement_address += 0x1000;
        }
        if (phys)
        {
            *phys = placement_address;
        }
        uint32_t tmp = placement_address;
        placement_address += sz;
        return tmp;
    }
}

void kfree(void *p)
{
    free(p, kheap);
}

uint32_t kmalloc_a(uint32_t sz)
{
    return kmalloc_int(sz, 1, 0);
}

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 0, phys);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 1, phys);
}

uint32_t kmalloc(uint32_t sz)
{
    return kmalloc_int(sz, 0, 0);
}

/*
uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys)
{
    if (kheap != 0) {
        void* ret = alloc(sz, align, kheap);
        if (phys != 0) {
            page_t* page = get_page((uint32_t)ret, 0, kernel_directory);
            *phys = page->frame*0x1000 + (uint32_t)ret&0xFFF;
        }
        return (uint32_t)ret;
        } else
        // This will eventually call malloc() on the kernel heap.
        // For now, though, we just assign memory at placement_address
        // and increment it by sz. Even when we've coded our kernel
        // heap, this will be useful for use before the heap is initialised.
        if (align == 1 && (placement_address & 0xFFFFF000) )
        {
            // Align the placement address;
            placement_address &= 0xFFFFF000;
            placement_address += 0x1000;
        }
        if (phys)
        {
            *phys = placement_address;
        }
        uint32_t tmp = placement_address;
        placement_address += sz;
        return tmp;
}

uint32_t kmalloc_a(uint32_t sz)
{
    return kmalloc_int(sz, 1, 0);
}

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 0, phys);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 1, phys);
}

uint32_t kmalloc(uint32_t sz)
{
    return kmalloc_int(sz, 0, 0);
}*/


