/*Pacman movement utilities*/
#include "game.h"
#include "pacman_sprite.h"

pacman* init_pacman(void) {
  pacman *pacman_ptr = (pacman *) malloc(sizeof(pacman));
  vector vec;
  vec.x = 0;
  vec.y = 0;
  pacman_ptr->vec = vec;
  pacman_ptr->prev_vec = vec;
  pacman_ptr->x = 112;
  pacman_ptr->y = 188;
  pacman_ptr->temp_x = pacman_ptr->x;
  pacman_ptr->temp_y = pacman_ptr->y;
  pacman_ptr->speed = 0.8;
  pacman_ptr->animation_state = 1;
  pacman_ptr->dots = 0;
  pacman_ptr->pacman_sprite = (Sprite *) malloc(sizeof(Sprite));
  pacman_ptr->pacman_sprite->bitmap = pacman_closed;
  pacman_ptr->pacman_sprite->width = PACMAN_SIZE;
  pacman_ptr->pacman_sprite->height = PACMAN_SIZE;
  return pacman_ptr;
}

void get_pacman_board_coord(pacman *pacman_ptr, int *bx, int *by) {
  int x = pacman_ptr->x;
  int y = pacman_ptr->y;
  *bx = x / 8;
  *by = y / 8;
}

void is_valid_vector(uint8_t *board, pacman *pacman_ptr) {
  int vx = pacman_ptr->vec.x;
  int vy = pacman_ptr->vec.y;
  int cx = 0;
  int cy = 0;
  get_pacman_board_coord(pacman_ptr, &cx, &cy);
  if(vx == 1) {
    if(get_board_value(cx + 1, cy, board) == WALL) {
      if(pacman_ptr->prev_vec.x > 0) {
        pacman_ptr->vec.x = 0;
      } else {
        pacman_ptr->vec = pacman_ptr->prev_vec;
      }
    }
  } else if(vx == -1) {
    if(get_board_value(cx - 1, cy, board) == WALL) {
      if(pacman_ptr->prev_vec.x < 0) {
        pacman_ptr->vec.x = 0;
      } else {
        pacman_ptr->vec = pacman_ptr->prev_vec;
      }
    }
  } else if(vy == 1) {
    if(get_board_value(cx, cy + 1, board) == WALL) {
      if(pacman_ptr->prev_vec.y > 0) {
        pacman_ptr->vec.y = 0;
      } else {
        pacman_ptr->vec = pacman_ptr->prev_vec;
      }
    }
  } else if(vy == -1) {
    if(get_board_value(cx, cy - 1, board) == WALL) {
      if(pacman_ptr->prev_vec.y < 0) {
        pacman_ptr->vec.y = 0;
      } else {
        pacman_ptr->vec = pacman_ptr->prev_vec;
      }
    }
  }
}

void pacman_animation_right(pacman *pacman_ptr){
  int animation = pacman_ptr->animation_state;
  pacman_ptr->animation_state = (animation + 1) % 3;
  switch(animation) {
    case 0:
      pacman_ptr->pacman_sprite->bitmap = pacman_closed;
      break;
    case 1:
      pacman_ptr->pacman_sprite->bitmap = pacman_right_open1;
      break;
    case 2:
      pacman_ptr->pacman_sprite->bitmap = pacman_right_open2;
      break;
  }
}

void pacman_animation_left(pacman *pacman_ptr){
  int animation = pacman_ptr->animation_state;
  pacman_ptr->animation_state = (animation + 1) % 3;
  switch(animation) {
    case 0:
      pacman_ptr->pacman_sprite->bitmap = pacman_closed;
      break;
    case 1:
      pacman_ptr->pacman_sprite->bitmap = pacman_left_open1;
      break;
    case 2:
      pacman_ptr->pacman_sprite->bitmap = pacman_left_open2;
      break;
  }
}

void pacman_animation_down(pacman *pacman_ptr){
  int animation = pacman_ptr->animation_state;
  pacman_ptr->animation_state = (animation + 1) % 3;
  switch(animation) {
    case 0:
      pacman_ptr->pacman_sprite->bitmap = pacman_closed;
      break;
    case 1:
      pacman_ptr->pacman_sprite->bitmap = pacman_down_open1;
      break;
    case 2:
      pacman_ptr->pacman_sprite->bitmap = pacman_down_open2;
      break;
  }
}

void pacman_animation_up(pacman *pacman_ptr){
  int animation = pacman_ptr->animation_state;
  pacman_ptr->animation_state = (animation + 1) % 3;
  switch(animation) {
    case 0:
      pacman_ptr->pacman_sprite->bitmap = pacman_closed;
      break;
    case 1:
      pacman_ptr->pacman_sprite->bitmap = pacman_up_open1;
      break;
    case 2:
      pacman_ptr->pacman_sprite->bitmap = pacman_up_open2;
      break;
  }
}

void change_pacman_animation(pacman *pacman_ptr) {
  int vx = pacman_ptr->vec.x;
  int vy = pacman_ptr->vec.y;
  if(vx == 1) {
    pacman_animation_right(pacman_ptr);
  }
  if(vx == -1) {
    pacman_animation_left(pacman_ptr);
  }
  if(vy == -1) {
    pacman_animation_up(pacman_ptr);
  }
  if(vy == 1) {
    pacman_animation_down(pacman_ptr);
  }
}

void control_pacman_speed(pacman *pacman_ptr) {
  pacman_ptr->temp_x += pacman_ptr->speed * pacman_ptr->vec.x * BASE_SPEED;
  pacman_ptr->temp_y += pacman_ptr->speed * pacman_ptr->vec.y * BASE_SPEED;
  pacman_ptr->x = (int) (pacman_ptr->temp_x);
  pacman_ptr->y = (int) (pacman_ptr->temp_y);

  if(pacman_ptr->vec.y) {
    pacman_ptr->x = ((pacman_ptr->x >> 3) << 3) + 3;
    pacman_ptr->temp_x = pacman_ptr->x;
  } if(pacman_ptr->vec.x) {
    pacman_ptr->y = ((pacman_ptr->y >> 3) << 3) + 3;
    pacman_ptr->temp_y = pacman_ptr->y;
  }
}

