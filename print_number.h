#ifndef PRINT_NUMBER_H
#define PRINT_NUMBER_H
#include <stdint.h>
#include "render.h"
#define base 10
#define y_coord 20
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8
void print_number(uint32_t, uint32_t, uint32_t, volatile unsigned char*, uint32_t);
void print_character(uint16_t *, uint32_t, uint32_t, volatile unsigned char*);
void print_character_colour(uint16_t *, uint16_t, uint32_t, uint32_t, volatile unsigned char *);
#endif
