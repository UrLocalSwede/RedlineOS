#include <stdint.h>

#include "arch/x86_64/gdt.h"

// ---- GDT: the CPU's list of ID badges ----

// The badge list: empty badge, kernel code badge, kernel data badge
static uint64_t gdt[] = {
    0x0000000000000000,   // 0x00: empty (required by the CPU)
    0x00AF9A000000FFFF,   // 0x08: kernel code
    0x00CF92000000FFFF,   // 0x10: kernel data
};

// Tells the CPU where the list is and how big it is
struct gdt_pointer {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

static struct gdt_pointer gdtr;

// Hands the list to the CPU and puts on the new badges
void gdt_load(void) {
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (uint64_t)gdt;

    asm volatile ("lgdt %0" : : "m"(gdtr));

    asm volatile (
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        "pushq $0x08\n"
        "leaq 1f(%%rip), %%rax\n"
        "pushq %%rax\n"
        "lretq\n"
        "1:\n"
        : : : "rax", "memory"
    );
}