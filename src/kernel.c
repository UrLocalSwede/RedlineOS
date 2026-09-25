#include <stdint.h>
#include "limine.h"

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;


__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static void serial_putc(char c) {
    while (!(inb(0x3F8 + 5) & 0x20)) {
    }
    outb(0x3F8, c);
}

static void serial_print(const char *s) {
    while (*s) {
        serial_putc(*s);
        s ++;
    }
}

static void serial_print_num(uint64_t n) {
    if (n == 0) {
        serial_putc('0');
        return;
    }

    char buf[20];
    int i = 0;

    while (n > 0) {
        buf[i] = '0' + n % 10;
        i++;
        n = n / 10;
    }

    while (i > 0) {
        i--;
        serial_putc(buf[i]);
    }
}

void kmain(void) {
    serial_print("hello from RedlineOS!\n");
    serial_print_num(305);
    while (1) {
        asm volatile ("hlt");
    }
}