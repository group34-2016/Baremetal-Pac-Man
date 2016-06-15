#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

typedef struct{
  int x;
  int y;
} vector;

int dist(int, int, int, int);
uint64_t timer_tick(void);
void timer_start(uint64_t *timer);
void timer_wait(uint64_t wait);
uint64_t timer_get_relative(uint64_t timer);
uint32_t get_centre_x(uint32_t);
#endif
