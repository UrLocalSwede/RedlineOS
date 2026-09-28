#pragma once

#include <stdint.h>

void serial_print(const char *s);
void serial_print_num(uint64_t n);
void serial_print_hex(uint64_t n);