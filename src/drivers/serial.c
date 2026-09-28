#include "drivers/serial.h"
#include "arch/x86_64/io.h"

static void serial_putc(char c) {
    while (!(inb(0x3F8 + 5) & 0x20)) {
    }
    outb(0x3F8, c);
}

// Prints a String
void serial_print(const char *s) {
    while (*s) {
        serial_putc(*s);
        s ++;
    }
}

// Prints an Integer
void serial_print_num(uint64_t n) {
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

// Prints a Hexadecimal
void serial_print_hex(uint64_t n) {
    serial_print("0x");

    if (n == 0) {
        serial_putc('0');
        return;
        
    }

    char buf[16];
    int i = 0;

    while (n > 0) {
        int digit = n % 16;
        if (digit < 10) {
            buf[i] = '0' + digit;
        } else {
            buf[i] = 'a' + (digit - 10);
        }
        i++;
        n = n / 16;
    }

    while (i > 0) {
        i--;
        serial_putc(buf[i]);
    }
}