#ifndef GHOSTAI_H
#define GHOSTAI_H
#include <stdint.h>
#include "util.h"
#include "game.h"
#define MAX_PEN_TIME   80
#define PINKY_PEN_TIME 48
#define INKY_PEN_TIME  72
#define CLYDE_PEN_TIME 100
#define TIME_FRIGHTENED 700
#define GRID_WIDTH 8
#define PEN_TOPLEFT_X 88
#define PEN_TOPLEFT_Y 104
#define BLINKY_PEN_X (PEN_TOPLEFT_X + (GRID_WIDTH * 3))
#define BLINKY_PEN_Y (PEN_TOPLEFT_Y - (GRID_WIDTH * 1.5))
#define INKY_PEN_X (PEN_TOPLEFT_X + (GRID_WIDTH * 1))
#define INKY_PEN_Y (PEN_TOPLEFT_Y + (GRID_WIDTH * 1.5))
#define PINKY_PEN_X (PEN_TOPLEFT_X + (GRID_WIDTH * 3))
#define PINKY_PEN_Y (PEN_TOPLEFT_Y + (GRID_WIDTH * 1.5))
#define CLYDE_PEN_X (PEN_TOPLEFT_X + (GRID_WIDTH * 5))
#define CLYDE_PEN_Y (PEN_TOPLEFT_Y + (GRID_WIDTH * 1.5))

/*Definition for the four scatter points for blinky*/
#define BLINKY_CORNER_X ((PIXEL_WIDTH - 1)/8)
#define BLINKY_CORNER_Y 0

#define PINKY_CORNER_X 0
#define PINKY_CORNER_Y 0

#define CLYDE_CORNER_X 0
#define CLYDE_CORNER_Y ((PIXEL_HEIGHT - 1)/8)

#define INKY_CORNER_X ((PIXEL_WIDTH - 1)/8)
#define INKY_CORNER_Y ((PIXEL_HEIGHT - 1)/8)

/*Definition for the four scatter points for pinky*/

/*Definition for the four scatter points for inky*/

/*Definition for the four scatter points for clyde*/

typedef struct{
  int x;
  int y;
} coordinates;

void init_ghosts(game*);
void change_state(uint8_t, game*, ghost*,ghost_state);
void scatter_all(game*);
void chase_all(game*);
void frightened_all(game*);
void eaten_all(game*);
void pen_all(game*);
void shortest_path_helper(game*);
void shortest_path(ghost*,uint8_t*, int x, int y);
coordinates scatter_generate(uint8_t, uint8_t, ghost*);

void blinky_ai(game*, ghost*, int*, int*);
void pinky_ai(game*, ghost*, int*, int*);
void inky_ai(game*, ghost*, int*, int*);
void clyde_ai(game*, ghost*, int*, int*);
uint8_t legal_tile(int, int,int, int, uint8_t*, uint8_t);
void get_ghost_board_coord(ghost*, int*, int*);
void set_speed(uint8_t, game*, ghost*, float);
void speed_setter(ghost* ghost_ptr);

#endif
