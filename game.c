#include "game.h"
#include "ghost_sprite.h"
#include "buttons.h"
#include "print_string.h"
#include <string.h>

/*TODO free game ptr*/
game* init_game(uint8_t* board) {
  game* game_ptr  = (game *) malloc(sizeof(game));
  game_ptr->score = 0;
  game_ptr->level = 1;
  game_ptr->lives = 3;
  game_ptr->pacman_ptr = init_pacman();
  game_ptr->board = board;
  game_ptr->fruit_set = 0;
  return game_ptr;
}

uint8_t *init_game_board() {
  uint8_t *game_board = (uint8_t *) calloc(GAME_BOARD_X * GAME_BOARD_Y, sizeof(uint8_t));
  reset_game_board(game_board);
  return game_board;
}

void reset_game_board(uint8_t *game_board) {
   uint8_t initial_board[GAME_BOARD_X * GAME_BOARD_Y] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,2,2,2,2,2,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,2,2,2,2,2,0,
    0,2,0,0,0,0,2,0,0,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,0,0,2,0,
    0,3,0,1,1,0,2,0,1,1,1,0,2,0,0,2,0,1,1,1,0,2,0,1,1,0,3,0,
    0,2,0,0,0,0,2,0,0,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,0,0,2,0,
    0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
    0,2,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,0,2,0,
    0,2,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,0,2,0,
    0,2,2,2,2,2,2,0,0,2,2,2,2,0,0,2,2,2,2,0,0,2,2,2,2,2,2,0,
    0,0,0,0,0,0,2,0,0,0,0,0,1,0,0,1,0,0,0,0,0,2,0,0,0,0,0,0,
    0,0,0,0,0,0,2,0,0,0,0,0,1,0,0,1,0,0,0,0,0,2,0,0,0,0,0,0,
    0,0,0,0,0,0,2,0,0,1,1,1,1,1,1,1,1,1,1,0,0,2,0,0,0,0,0,0,
    0,0,0,0,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,
    0,0,0,0,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,
    1,1,1,1,1,1,2,1,1,1,0,1,1,1,1,1,1,0,1,1,1,2,1,1,1,1,1,1,
    0,0,0,0,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,
    0,0,0,0,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,
    0,0,0,0,0,0,2,0,0,1,1,1,1,1,1,1,1,1,1,0,0,2,0,0,0,0,0,0,
    0,0,0,0,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,
    0,0,0,0,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,
    0,2,2,2,2,2,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,2,2,2,2,2,0,
    0,2,0,0,0,0,2,0,0,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,0,0,2,0,
    0,2,0,0,0,0,2,0,0,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,0,0,2,0,
    0,3,2,2,0,0,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,0,0,2,2,3,0,
    0,0,0,2,0,0,2,0,0,2,0,0,0,0,0,0,0,0,2,0,0,2,0,0,2,0,0,0,
    0,0,0,2,0,0,2,0,0,2,0,0,0,0,0,0,0,0,2,0,0,2,0,0,2,0,0,0,
    0,2,2,2,2,2,2,0,0,2,2,2,2,0,0,2,2,2,2,0,0,2,2,2,2,2,2,0,
    0,2,0,0,0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,2,0,
    0,2,0,0,0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,2,0,
    0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
   memcpy(game_board, initial_board, GAME_BOARD_X * GAME_BOARD_Y * sizeof(uint8_t));
}

uint8_t get_board_value(int x, int y, uint8_t *board) {
  return *(board + x + (y * GAME_BOARD_X));
}

void set_board_value(int x, int y, int val, uint8_t *board) {
  *(board + x + (y * GAME_BOARD_X)) = val;
}

uint8_t got_power_up(game *game_ptr) {
  int bx = 0;
  int by = 0;
  get_pacman_board_coord(game_ptr->pacman_ptr, &bx, &by);
  if(get_board_value(bx, by, game_ptr->board) == POWER_UP) {
    return 1;
  }
  return 0;
}


void update_board(uint8_t *board, pacman *pacman_ptr, game *game_ptr) {
  for (int y = 0; y < GAME_BOARD_Y; ++y) {
    for (int x = 0; x < GAME_BOARD_X; ++x) {
      if(get_board_value(x, y, board) == PACMAN) {
        set_board_value(x, y, EMPTY, board);
      }
    }
  }
  int bx = 0;
  int by = 0;
  get_pacman_board_coord(pacman_ptr, &bx, &by);
  if(get_board_value(bx + 1, by, board) == PACMAN) {
    set_board_value(bx + 1, by, EMPTY, board);
  } else if(get_board_value(bx - 1, by, board) == PACMAN) {
    set_board_value(bx - 1, by, EMPTY, board);
  } else if(get_board_value(bx, by + 1, board) == PACMAN) {
    set_board_value(bx, by + 1, EMPTY, board);
  } else if(get_board_value(bx, by - 1, board) == PACMAN) {
    set_board_value(bx, by - 1, EMPTY, board);
  }

  uint8_t prev_tile_value = get_board_value(bx, by, board);
  if(prev_tile_value == FOOD) {
    game_ptr->score += 10;
    game_ptr->pacman_ptr->dots++;
  } else if(prev_tile_value == POWER_UP) {
    game_ptr->score += 50;
  } else if(prev_tile_value == FRUIT) {
    int level = game_ptr->level;
    int fruit_score = 0;
    if(level == 1) {
      fruit_score = 100;
    } else if(level <= 2) {
      fruit_score = 300;
    } else if(level <= 4) {
      fruit_score = 500;
    } else if(level <= 6) {
      fruit_score = 700;
    } else if(level <= 8) {
      fruit_score = 1000;
    } else if(level <=10) {
      fruit_score = 2000;
    } else if(level <= 12) {
      fruit_score = 3000;
    } else {
      fruit_score = 5000;
    }
    game_ptr->score += fruit_score;
    game_ptr->fruit_set++;
  }
  set_board_value(bx, by, PACMAN, board);
}

uint8_t game_won(uint8_t *board) {
  for(int i = 0; i < (GAME_BOARD_X * GAME_BOARD_Y); ++i) {
    if(board[i] == FOOD || board[i] == POWER_UP) {
      return 0;
    }
  }
  return 1;
}

int detect_collision(game *game_ptr, ghost **ghosts_array, int offset, volatile unsigned char *fb) {
  int ghost_bx, ghost_by, pacman_bx, pacman_by;

  for(int i = 0; i < 4; ++i) {
    get_pacman_board_coord(game_ptr->pacman_ptr, &pacman_bx, &pacman_by);
    get_ghost_board_coord(ghosts_array[i], &ghost_bx, &ghost_by);

    ghost *cur_ghost = ghosts_array[i];

    if(ghost_bx == pacman_bx && ghost_by == pacman_by) {
      if(cur_ghost->state == FRIGHTENED) {
        change_state(0, NULL, cur_ghost, EATEN);
        int num_still_scared = 0;
        for (int i = 0; i < NUM_GHOSTS; ++i) {
          if(ghosts_array[i]->state == FRIGHTENED) {
            num_still_scared++;
          }
        }
        int ghost_point = 200;
        ghost_point <<= (NUM_GHOSTS - (num_still_scared + 1));
        game_ptr->score += ghost_point;
        draw_ghost_score(cur_ghost, game_ptr->pacman_ptr, ghost_point,  SCREEN_HEIGHT - offset, fb);
        for (volatile int i = 0; i < 2000000; ++i) {}
      } else if(cur_ghost->state == SCATTER || cur_ghost->state == CHASE) {
        if(game_ptr->lives - 1 == 0) {
          display_end_screen("GAME OVER!", "PRESS ANY BUTTON TO RESTART", offset, fb);
        }
        return 1;
      }
    }
  }
  return 0;
}

void display_end_screen(char *message1, char *message2, int offset, volatile unsigned char *fb) {
  int wait_count = 0;
  while(!poll_controller() || wait_count < 1000) {
    print_string(message1, get_centre_x(strlen(message1) * 8), 220, SCREEN_HEIGHT - offset, fb);
    print_string(message2, get_centre_x(strlen(message2) * 8), 240, SCREEN_HEIGHT - offset, fb);
    if(wait_count < 1000) {
      wait_count += 1;
    }
  }
}

void init_ghosts_sprite(ghost *ghosts[]) {
  for (int i = 0; i < NUM_GHOSTS; ++i) {
    ghosts[i]->ghost_sprite = (Sprite *) malloc(sizeof(Sprite));
    ghosts[i]->ghost_sprite->height = GHOST_SIZE;
    ghosts[i]->ghost_sprite->width = GHOST_SIZE;
  }
  ghosts[0]->ghost_sprite->bitmap = blinky_left;
  ghosts[1]->ghost_sprite->bitmap = pinky_down;
  ghosts[2]->ghost_sprite->bitmap = inky_up;;
  ghosts[3]->ghost_sprite->bitmap = clyde_up;
}

void change_ghost_animation(ghost *ghosts[]) {
  uint16_t *blinky_animation[] = {blinky_up, blinky_right, blinky_down, blinky_left};
  uint16_t *pinky_animation[]  = {pinky_up, pinky_right, pinky_down, pinky_left};
  uint16_t *inky_animation[]   = {inky_up, inky_right, inky_down, inky_left};
  uint16_t *clyde_animation[]  = {clyde_up, clyde_right, clyde_down, clyde_left};
  uint16_t** ghost_animations[] = {blinky_animation, pinky_animation, inky_animation, clyde_animation};
  uint16_t *eaten_animation[] = {ghost_eyes_up, ghost_eyes_right, ghost_eyes_down, ghost_eyes_left};

  for (int i = 0; i < NUM_GHOSTS; ++i) {
    if(ghosts[i]->state == FRIGHTENED) {
      if(ghosts[i]->nearly_ending) {
        ghosts[i]->ghost_sprite->bitmap = ghost_scared2;
      } else {
        ghosts[i]->ghost_sprite->bitmap = ghost_scared1;
      }
    } else {
      uint16_t **animation = ghost_animations[i];
      if(ghosts[i]->state == EATEN) {
        animation = eaten_animation;
      }
      if(ghosts[i]->vec.y == -1) {
        ghosts[i]->ghost_sprite->bitmap = animation[0];
      } else if(ghosts[i]->vec.x == 1) {
        ghosts[i]->ghost_sprite->bitmap = animation[1];
      } else if(ghosts[i]->vec.y == 1) {
        ghosts[i]->ghost_sprite->bitmap = animation[2];
      } else if(ghosts[i]->vec.x == -1) {
        ghosts[i]->ghost_sprite->bitmap = animation[3];
      }
    }
  }
}

void control_ghost_speed(ghost *ghost_ptr) {
  ghost_ptr->temp_x += ghost_ptr->speed * ghost_ptr->vec.x * BASE_SPEED;
  ghost_ptr->temp_y += ghost_ptr->speed * ghost_ptr->vec.y * BASE_SPEED;
  ghost_ptr->x = (int) (ghost_ptr->temp_x);
  ghost_ptr->y = (int) (ghost_ptr->temp_y);
}
