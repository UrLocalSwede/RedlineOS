#include <stdint.h>
#include "limine.h"

#include "drivers/serial.h"
#include "arch/x86_64/gdt.h"
#include "arch/x86_64/idt.h"

#include "drivers/timer.h"
#include "arch/x86_64/pic.h"

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(4);

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;


__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;



void kmain(void) {
    gdt_load();
    idt_load();
    pic_init();
    timer_init();
    asm volatile ("sti");

    serial_print("Welcome to RedlineOS\n");

    while (1) {
        asm volatile ("hlt");
    }
}