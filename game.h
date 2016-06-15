#ifndef GAME_H
#define GAME_H
#include <stdint.h>
#include <stdlib.h>
#include "util.h"

#define PIXEL_WIDTH 224
#define PIXEL_HEIGHT 288
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define SCREEN_DEPTH    16
#define X_OFFSET        208
#define Y_OFFSET        96
#define X_OFFSET_GAME   208
#define Y_OFFSET_GAME   120

#define GAME_BOARD_X 28
#define GAME_BOARD_Y 31

#define WALL     0
#define EMPTY    1
#define FOOD     2
#define POWER_UP 3
#define PACMAN   4
#define BLINKY   5
#define PINKY    6
#define INKY     7
#define CLYDE    8
#define FRUIT    9

#define PACMAN_SIZE 13
#define GHOST_SIZE  14

#define NUM_GHOSTS 4
#define BASE_SPEED 1.33

#define MAX_LIVES 5
#define SCORE_FOR_LIFE 10000

typedef struct {
  uint16_t *bitmap;
  int height;
  int width;
} Sprite;

/*Struct to the protagonist Mr.Pac Man*/
typedef struct {
  /*There are 244 dots*/
  uint16_t dots;
  vector vec;
  vector prev_vec;
  uint32_t x;
  uint32_t y;
  float temp_x;
  float temp_y;
  float speed;
  uint8_t animation_state;
  Sprite *pacman_sprite;
} pacman;

#include "pacman.h"

typedef enum {none, CHASE, SCATTER, FRIGHTENED, EATEN, PEN} ghost_state;

/*4 types of ghosts.*/
typedef struct{
  uint8_t turn;
  uint32_t x;
  uint32_t y;
  vector vec;
  float temp_x;
  float temp_y;
  float speed;
  ghost_state state;
  ghost_state prev_state;
  Sprite *ghost_sprite;
  uint8_t nearly_ending;
} ghost;

/*Struct that points to the four ghosts
 * TODO: SET SPEEDS BASED ON LEVEL SOMEWHERE. THIS IS UNINITIALISED*/
typedef struct{
  ghost* blinky;
  ghost* inky;
  ghost* pinky;
  ghost* clyde;
  uint32_t norm_speed;
  uint32_t fright_speed;
  uint32_t tunnel_speed;
} ghosts;

/*Game struct*/
typedef struct {
  uint32_t score;
  uint32_t lives;
  uint32_t level;
  ghosts* ghosts_ptr;
  pacman* pacman_ptr;
  uint8_t* board;
  uint16_t fruit_set;
} game;

#include "ghost_ai.h"

game* init_game(uint8_t*);
uint8_t *init_game_board(void);
void reset_game_board(uint8_t *);
uint8_t get_board_value(int, int, uint8_t *);
void set_board_value(int, int, int, uint8_t *);
void is_valid_vector(uint8_t *board, pacman *pacman_ptr);
uint8_t got_power_up(game *game_ptr);
void update_board(uint8_t *board, pacman *pacman_ptr, game *game_ptr);
uint8_t game_won(uint8_t *board);
int detect_collision(game* game_ptr, ghost **ghosts_array, int offset, volatile unsigned char *fb);
void display_end_screen(char *message1, char *message2, int offset, volatile unsigned char *fb);
void init_ghosts_sprite(ghost *ghosts[]);
void change_ghost_animation(ghost *ghosts[]);
void control_ghost_speed(ghost *ghost);
#endif
