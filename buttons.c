/* GPIO PINS 17, 18, 22, 23 are being used for the four buttons.
 * GPIO 17 -> UP
 * GPIO 18 -> DOWN
 * GPIO 23 -> LEFT
 * GPIO 22-> RIHGT
 * OUTPUT FORMAT :
 * UP   |DOWN   |LEFT   |RIGHT
 */

#include "buttons.h"
uint8_t poll_controller(){
  uint32_t* gplevn_ptr = (uint32_t *) GPLEVN;
  /*GPIO Pins to cycle through, to avoid magic numbers.*/
  int pins[] = {17, 18, 23, 22};
  unsigned int output = 0;
  for (int i = 0; i < 4; ++i) {
    output |= (*gplevn_ptr >> (pins[i])) & 1;
    output <<= 1;
  }
  return output >> 1;
}
