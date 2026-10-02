#include <stdint.h>
#include "drivers/timer.h"
#include "drivers/serial.h"
#include "arch/x86_64/idt.h"
#include "arch/x86_64/pic.h"
#include "arch/x86_64/io.h"

static volatile uint64_t ticks = 0;

__attribute__((interrupt))
static void timer_handler(struct interrupt_frame *frame) {
    (void)frame;

    ticks++;
    if (ticks % 100 == 0) {
        serial_print("Seconds since boot: ");
        serial_print_num(ticks / 100);
        serial_print("\n");
    }

    pic_send_eoi();
}

void timer_init(void) {
    uint16_t divisor = 1193182 / 100;

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);

    idt_set(32, timer_handler);
}