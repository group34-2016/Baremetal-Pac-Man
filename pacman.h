#ifndef PACMAN_H
#define PACMAN_H
#include "game.h"

#define PACMAN_UP    0
#define PACMAN_RIGHT 1
#define PACMAN_DOWN  2
#define PACMAN_LEFT  3

pacman* init_pacman(void);
void get_pacman_board_coord(pacman *, int *, int *);
void is_valid_vector(uint8_t *board, pacman *pacman_ptr);
void change_pacman_animation(pacman *pacman_ptr);
void control_pacman_speed(pacman *pacman_ptr);
#endif
