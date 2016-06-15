#ifndef PRINT_STRING_H
#define PRINT_STRING_H

#include <stdio.h>
#include "print_number.h"

void print_string(char *, uint32_t, uint32_t, uint32_t, volatile unsigned char *);
void print_string_colour(char *, uint16_t, uint32_t, uint32_t, uint32_t, volatile unsigned char *);
void print_string_colour_delay(char *, uint16_t, uint32_t, uint32_t, uint32_t, volatile unsigned char *);
#endif
