#include "util.h"

#define SYSTEM_TIMER_L  0x20003004

int dist(int x1, int y1, int x2, int y2) {
  return (((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
}

uint64_t timer_tick(void) {
  return *((uint64_t *) SYSTEM_TIMER_L);
}

void timer_start(uint64_t *timer) {
  *timer = timer_tick();
}

void timer_wait(uint64_t wait) {
  uint64_t time_stamp;
  timer_start(&time_stamp);
  while((wait  - (timer_tick() - time_stamp)));
}

uint64_t timer_get_relative(uint64_t timer) {
  return *((uint64_t *) SYSTEM_TIMER_L) - timer;
}

uint32_t get_centre_x(uint32_t width) {
  return ((640 - width) / 2);
}
