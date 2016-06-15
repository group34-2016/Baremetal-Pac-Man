#include "print_number.h"
#include "char_sprite.h"


void print_character(uint16_t *colour_ptr, uint32_t x, uint32_t y, volatile unsigned char *fb) {
  for (int i = 0; i < CHAR_WIDTH; ++i) {
    for (int j = 0; j < CHAR_HEIGHT; ++j) {
      draw_pixel(x + j, y + i, fb, colour_ptr[j + (i * CHAR_WIDTH)]);
    }
  }
}

void print_character_colour(uint16_t *colour_ptr, uint16_t user_colour, uint32_t x, uint32_t y, volatile unsigned char *fb) {
  for (int i = 0; i < CHAR_WIDTH; ++i) {
    for (int j = 0; j < CHAR_HEIGHT; ++j) {
      if(colour_ptr[j + (i * CHAR_WIDTH)]) {
        draw_pixel(x + j, y + i, fb, user_colour);
      }
    }
  }
}

uint32_t power(uint32_t num_base, uint32_t power) {

  if (power == 0) {
    return 1;
  }

  uint32_t result = num_base;

  for (uint32_t i = 1; i < power; ++i) {
    result *= num_base;
  }
  return result;
}




void print_number(uint32_t number, uint32_t x, uint32_t y, volatile unsigned char *fb, uint32_t offset) {

  uint8_t non_zero_met = 0;
  uint32_t division;
  uint32_t current;

  for (int i = 10; i >= 0; --i) {
    current    = power(10, i);
    division   = number / current;
    number     = number % current;
    if (division != 0) {
      non_zero_met = 1;
    }
    if (i == 1) {
      non_zero_met = 1;
    }
    if (non_zero_met) {
      print_character(&char_sprite_array[division * 64], x + 80 - (8 * i), y + offset, fb);
    }
  }
}
