#ifndef BUTTONS_H
#define BUTTONS_H
#include <stdint.h>
uint8_t poll_controller();
#define GPIO_F 0x20200000
#define GPIO_S 0x20200004
#define GPLEVN 0x20200034
#endif
