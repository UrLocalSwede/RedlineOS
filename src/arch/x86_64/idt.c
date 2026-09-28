#include <stdint.h>
#include "arch/x86_64/idt.h"
#include "drivers/serial.h"

// One slot in the emergency contact list (16 bytes)
struct idt_entry {
    uint16_t address_low;    // handler address, bits 0-15
    uint16_t selector;       // which GDT badge the handler runs with
    uint8_t  ist;            // extra stack option, not used yet
    uint8_t  flags;          // "present, kernel-only, interrupt"
    uint16_t address_mid;    // handler address, bits 16-31
    uint32_t address_high;   // handler address, bits 32-63
    uint32_t reserved;       // must be 0
} __attribute__((packed));

// Tells the CPU where the list is, same idea as for the GDT
struct idt_pointer {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_pointer idtr;

// Put a handler into slot number `number`
static void idt_set(int number, void *handler) {
    uint64_t address = (uint64_t)handler;

    idt[number].address_low  = address & 0xFFFF;
    idt[number].selector     = 0x08;
    idt[number].ist          = 0;
    idt[number].flags        = 0x8E;
    idt[number].address_mid  = (address >> 16) & 0xFFFF;
    idt[number].address_high = address >> 32;
    idt[number].reserved     = 0;
}

// What the CPU tells the handler about where the problem happened
struct interrupt_frame {
    uint64_t rip;       // address of the instruction that failed
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

__attribute__((interrupt))
static void divide_error_handler(struct interrupt_frame *frame) {
    serial_print("\n!!! CPU EXCEPTION: Division by zero\n");
    serial_print("    at address ");
    serial_print_hex(frame->rip);
    serial_print("\n");

    while (1) {
        asm volatile ("cli; hlt");
    }
}

void idt_load(void) {
    idt_set(0, divide_error_handler);

    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint64_t)idt;
    asm volatile ("lidt %0" : : "m"(idtr));
}