#include <stdint.h>
#include "limine.h"

#include "drivers/serial.h"
#include "arch/x86_64/gdt.h"

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;


__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;



void kmain(void) {
    serial_print("kmain is at ");
    serial_print_hex((uint64_t)kmain);
    serial_print("\n");
    while (1) {
        asm volatile ("hlt");
    }
}