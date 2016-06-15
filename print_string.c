#include "print_string.h"
#include "font_sprite.h"
void print_string(char *str, uint32_t x, uint32_t y, uint32_t offset, volatile unsigned char *fb) {
  while (*str != '\0') {
    if ((*str >= ' ') && (*str <= 'Z')) {
      print_character(&font_sprite_array[((uint8_t)(*str) - (uint8_t)(' ')) * 64], x, y + offset, fb);
      x += 8;
    }
    ++str;
  }
}

void print_string_colour(char *str, uint16_t colour, uint32_t x, uint32_t y, uint32_t offset, volatile unsigned char *fb) {
  while (*str != '\0') {
    if ((*str >= ' ') && (*str <= 'Z')) { 
      print_character_colour(&font_sprite_array[((uint8_t)(*str) - (uint8_t)(' ')) * 64], 
                             colour, x, y + offset, fb);
      x += 8;
    }
    ++str;
  }
}

void wait_small() {
  for (volatile int i = 0; i < 100000; ++i);
}


void print_string_colour_delay(char *str, uint16_t colour, uint32_t x, uint32_t y, uint32_t offset, volatile unsigned char *fb) {
  while (*str != '\0') {
    if ((*str >= ' ') && (*str <= 'Z')) { 
      print_character_colour(&font_sprite_array[((uint8_t)(*str) - (uint8_t)(' ')) * 64], 
                             colour, x, y + offset, fb);
      x += 8;
    }
    ++str;
    wait_small();
  }
}
