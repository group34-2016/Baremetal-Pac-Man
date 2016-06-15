#include "render.h"
#include "print_number.h"
#include "score_sprite.h"
#include "fruit_score_sprite.h"

void draw_pixel(int x, int y, volatile unsigned char *fb, uint16_t colour) {
  int pixel_offset = (x + y * 640) << 1;
  *(uint16_t *)&fb[pixel_offset] = colour;
}

void draw_background_teleport(Sprite *sprite, int offset, volatile unsigned char *fb) {
  int padding_y = 24;
  int width = sprite->width;
  int y_start = 108;
  int y_end   = 140;
  int x_start = 0;
  int x_end   = 24;
  for (int y = y_start + padding_y; y < y_end + padding_y; ++y) {
    for (int x = x_start; x < x_end; ++x) {
      draw_pixel(x + X_OFFSET, y + Y_OFFSET + offset, fb, *(sprite->bitmap + x + (y * width)));
    }
  }
  x_start = (GAME_BOARD_X - 3) << 3;
  x_end   = GAME_BOARD_X << 3;
  for (int y = y_start + padding_y; y < y_end + padding_y; ++y) {
    for (int x = x_start; x < x_end; ++x) {
      draw_pixel(x + X_OFFSET, y + Y_OFFSET + offset, fb, *(sprite->bitmap + x + (y * width)));
    }
  }
}

//if x and y are at the tunnel, move them to the other tunnel
void tunnel_teleport(uint32_t *x, float *fx) {
  if(*x <= 9) {
    *x = PIXEL_WIDTH - 10;
    *fx = *x;
   } else if(*x >= PIXEL_WIDTH - 9) {
     *x = 10;
     *fx = *x;
   }
}

void draw_pacman(pacman *pacman_ptr, int offset, volatile unsigned char *fb) {
  tunnel_teleport(&pacman_ptr->x, &pacman_ptr->temp_x);
  int x_pos = pacman_ptr->x;
  int y_pos = pacman_ptr->y + offset;
  x_pos -= 6;
  y_pos -= 6;
  int width = pacman_ptr->pacman_sprite->width;
  int height = pacman_ptr->pacman_sprite->height;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if(*(pacman_ptr->pacman_sprite->bitmap + x + (y * width))) {
        draw_pixel(x + x_pos + X_OFFSET_GAME, y + y_pos + Y_OFFSET_GAME, fb, *(pacman_ptr->pacman_sprite->bitmap + x + (y * width)));
      }
    }
  }
}

void draw_ghost(ghost* ghost_ptr, int offset, volatile unsigned char *fb) {
  tunnel_teleport(&(ghost_ptr->x), &ghost_ptr->temp_x);
  int x_pos = ghost_ptr->x;
  int y_pos = ghost_ptr->y + offset;
  x_pos -= 7;
  y_pos -= 7;
  int width = ghost_ptr->ghost_sprite->width;
  int height = ghost_ptr->ghost_sprite->height;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if(*(ghost_ptr->ghost_sprite->bitmap + x + (y * width))) {
        draw_pixel(x + x_pos + X_OFFSET_GAME, y + y_pos + Y_OFFSET_GAME, fb, *(ghost_ptr->ghost_sprite->bitmap + x + (y * width)));
      }
    }
  }
}

void draw_score_sprite(int point, int offset, ghost *ghost_ptr, volatile unsigned char *fb) {
  Sprite *score = (Sprite *) malloc(sizeof(Sprite));
  score->height = 16;
  score->width = 16;
  switch(point) {
    case 200:
      score->bitmap = points_200;
      break;
    case 400:
      score->bitmap = points_400;
      break;
    case 800:
      score->bitmap = points_800;
      break;
    case 1600:
      score->bitmap = points_1600;
      break;
  }
  int x_pos = ghost_ptr->x;
  int y_pos = ghost_ptr->y + offset;
  x_pos -= 9;
  y_pos -= 9;
  int width = score->width;
  int height = score->height;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if(*(score->bitmap + x + (y * width))) {
        draw_pixel(x + x_pos + X_OFFSET_GAME, y + y_pos + Y_OFFSET_GAME, fb, *(score->bitmap + x + (y * width)));
      }
    }
  }
  free(score);
}


void draw_ghost_score(ghost* ghost_ptr, pacman *pacman_ptr, int point, int offset, volatile unsigned char *fb) {
  int x_pos = ghost_ptr->x;
  int y_pos = ghost_ptr->y + offset;
  x_pos -= (7 + ghost_ptr->vec.x);
  y_pos -= (7 + ghost_ptr->vec.x);
  int width = ghost_ptr->ghost_sprite->width;
  int height = ghost_ptr->ghost_sprite->height;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if(*(ghost_ptr->ghost_sprite->bitmap + x + (y * width))) {
        draw_pixel(x + x_pos + X_OFFSET_GAME, y + y_pos + Y_OFFSET_GAME, fb, 0);
      }
    }
  }
  x_pos = pacman_ptr->x;
  y_pos = pacman_ptr->y + offset;
  x_pos -= (6 + pacman_ptr->vec.x);
  y_pos -= (6 + pacman_ptr->vec.y);
  width = pacman_ptr->pacman_sprite->width;
  height = pacman_ptr->pacman_sprite->height;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if(*(pacman_ptr->pacman_sprite->bitmap + x + (y * width))) {
        draw_pixel(x + x_pos + X_OFFSET_GAME, y + y_pos + Y_OFFSET_GAME, fb, 0);
      }
    }
  }
  draw_score_sprite(point, offset, ghost_ptr, fb);
}


void draw_sprite(Sprite *sprite, int x_pos, int y_pos, volatile unsigned char *fb) {
  x_pos -= 7;
  y_pos -= 7;
  int width = sprite->width;
  int height = sprite->height;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if(*(sprite->bitmap + x + (y * width))) {
        draw_pixel(x + x_pos + X_OFFSET_GAME, y + y_pos + Y_OFFSET_GAME, fb, *(sprite->bitmap + x + (y * width)));
      }
    }
  }
}

void draw_tile(Sprite *sprite, int x_pos, int y_pos, volatile unsigned char *fb) {
  int width = sprite->width;
  int height = sprite->height;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      draw_pixel(x + x_pos + X_OFFSET_GAME, y + y_pos + Y_OFFSET_GAME, fb, *(sprite->bitmap + x + (y * width)));
    }
  }
}

void draw_blank_tile(int x_pos, int y_pos, volatile unsigned char *fb) {
  for (int y = 0; y < 8; ++y) {
    for (int x = 0; x < 8; ++x) {
      draw_pixel(x + x_pos + X_OFFSET_GAME, y + y_pos + Y_OFFSET_GAME, fb, 0);
    }
  }
}

void draw_food(Sprite *sprite, uint8_t *board, int offset, volatile unsigned char *fb) {
  for(int y = 0; y < GAME_BOARD_Y; ++y) {
    for(int x = 0; x < GAME_BOARD_X; ++x) {
      if(get_board_value(x, y, board) == FOOD) {
        draw_tile(sprite, (x << 3), (y << 3) + offset, fb);
      }
    }
  }
}

void draw_fruit(Sprite *sprite, int offset, volatile unsigned char *fb) {
  int x_pos = 13 * 8 - 4;
  int y_pos = 17 * 8 - 4 + offset;
  int height = sprite->height;
  int width = sprite->width;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      draw_pixel(x + x_pos + X_OFFSET_GAME, y + y_pos + Y_OFFSET_GAME, fb, *(sprite->bitmap + x + (y * width)));
    }
  }
}

void draw_fruit_score(int point, int offset, volatile unsigned char *fb) {
  Sprite *score = (Sprite *) malloc(sizeof(Sprite));
  score->height = 16;
  score->width = 16;
  switch(point) {
    case 100:
      score->bitmap = fruit_score_100;
      break;
    case 300:
      score->bitmap = fruit_score_300;
      break;
    case 500:
      score->bitmap = fruit_score_500;
      break;
    case 700:
      score->bitmap = fruit_score_700;
      break;
    case 1000:
      score->bitmap = fruit_score_1000;
      break;
    case 3000:
      score->bitmap = fruit_score_3000;
      break;
    case 5000:
      score->bitmap = fruit_score_5000;
      break;
  }
  int x_pos = 13 * 8 - 4;
  int y_pos = 17 * 8 - 4 + offset;
  int height = score->height;
  int width = score->width;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      draw_pixel(x + x_pos + X_OFFSET_GAME, y + y_pos + Y_OFFSET_GAME, fb, *(score->bitmap + x + (y * width)));
    }
  }
  free(score);
}

void remove_fruit(volatile unsigned char *fb) {
  int x_pos = 13 * 8 - 4;
  int y_pos = 17 * 8 - 4;
  int height = 16;
  int width = 16;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      draw_pixel(x + x_pos + X_OFFSET_GAME, y + y_pos + Y_OFFSET_GAME, fb, 0);
      draw_pixel(x + x_pos + X_OFFSET_GAME, y + y_pos + SCREEN_HEIGHT + Y_OFFSET_GAME, fb, 0);
    }
  }
}


void draw_power_up(Sprite *sprite, uint8_t *board, int offset, int *counter, volatile unsigned char *fb) {
  int xs[4] = {1, 26, 1, 26};
  int ys[4] = {3, 3, 23, 23};
  if(*counter < 10) {
    for (int i = 0; i < sizeof(xs) / sizeof(int); ++i) {
      if(get_board_value(xs[i], ys[i], board) == POWER_UP) {
        draw_tile(sprite, (xs[i] << 3), (ys[i] << 3) + offset, fb);
      }
    }
  } else {
    for (int i = 0; i < sizeof(xs) / sizeof(int); ++i) {
      draw_blank_tile((xs[i] << 3), (ys[i] << 3) + offset, fb);
    }
  }
  if(*counter >= 20) {
    *counter = 0;
  } else {
    *counter += 1;
  }
}


void draw_background(Sprite *sprite, int offset, volatile unsigned char *fb) {
  int width = sprite->width;
  int height = sprite->height;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      draw_pixel(x + X_OFFSET, y + Y_OFFSET + offset, fb, *(sprite->bitmap + x + (y * width)));
    }
  }
}

void draw_background_16_box(Sprite *sprite, int x, int y, int offset, volatile unsigned char *fb) {
  int x_start = x - 1;
  int y_start = y - 1;
  int x_end = x + 2;
  int y_end = y + 2;
  int padding_y = 24;
  int x_pos = 2;
  int y_pos = 2;
  int x_neg = -2;
  int y_neg = -2;

  if(x_start <= 0) {
    x_start = 0;
    x_neg = 0;
  }
  if(y_start <= 0) {
    y_start = 0;
    y_neg = 0;
  }
  if(x_end >= GAME_BOARD_X) {
    x_end = GAME_BOARD_X;
    x_pos = 0;
  }
  if(y_end >= GAME_BOARD_Y) {
    y_end = GAME_BOARD_Y;
    y_pos = 0;
  }
  /*multiply by 8 as each box is 8 pixel*/
  x_start <<= 3;
  y_start <<= 3;
  x_end <<= 3;
  y_end <<= 3;
  x_start += x_neg;
  y_start += y_neg;
  x_end   += x_pos;
  y_end   += y_pos;
  int width = sprite->width;
  for (int y = y_start + padding_y; y < y_end + padding_y; ++y) {
    for (int x = x_start; x < x_end; ++x) {
      draw_pixel(x + X_OFFSET, y + Y_OFFSET + offset, fb, *(sprite->bitmap + x + (y * width)));
    }
  }
}

void draw_lives(int lives, int offset, volatile unsigned char *fb) {
  uint16_t pacman_life[169] = {0x0000,0x0000,0x0000,0x0000,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0x0000,0x0000,0x0000,0x0000
  ,0x0000,0x0000,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0x0000,0x0000
  ,0x0000,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0x0000
  ,0x0000,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0x0000
  ,0x0000,0x0000,0x0000,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0
  ,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0
  ,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffe0,0xffe0,0xffe0,0xffe0
  ,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0
  ,0x0000,0x0000,0x0000,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0
  ,0x0000,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0x0000
  ,0x0000,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0x0000
  ,0x0000,0x0000,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0x0000,0x0000
  ,0x0000,0x0000,0x0000,0x0000,0xffe0,0xffe0,0xffe0,0xffe0,0xffe0,0x0000,0x0000,0x0000,0x0000
  };
  Sprite pacman;
  pacman.bitmap = pacman_life;
  pacman.height = PACMAN_SIZE;
  pacman.width  = PACMAN_SIZE;

  //draw black
  /*for(int y = PIXEL_HEIGHT + Y_OFFSET_GAME; y <= PIXEL_HEIGHT + Y_OFFSET_GAME + 13; ++y) {
    for(int x = X_OFFSET_GAME + 8; x <= X_OFFSET_GAME + 56; ++x) {
      draw_pixel(x, y + offset, fb, 0);
    }
  }*/

  for(int life = 0; life < lives - 1; ++life) {
    draw_sprite(&(pacman), 8 + (16 * life), PIXEL_HEIGHT - 32 + offset, fb);
  }
}

/*uses Bresenham's line algorithm */
void render_line(Point2D a, Point2D b, volatile unsigned char *fb, uint16_t colour) {
  int dx, dy;
  int step_x, step_y;
  int x, y;
  /*checks for simple cases first*/
  /*render diagonal line*/
  if(b.x > a.x) {
    dx = b.x - a.x;
    step_x = 1;
  } else {
    dx = a.x - b.x;
    step_x = -1;
  }
  if(b.y > a.y) {
    dy = b.y - a.y;
    step_y = 1;
  } else {
    dy = a.y - b.y;
    step_y = -1;
  }
  /*checks for simple cases first*/
  if(a.x == b.x) {
    x = a.x;
    y = a.y;
    for(y = a.y; y != b.y; y += step_y) {
      draw_pixel(x, y, fb, colour);
    }
    return;
  }
  if(a.y == b.y) {
    x = a.x;
    y = a.y;
    for(x = a.x; x != b.x; x += step_x) {
      draw_pixel(x, y, fb, colour);
    }
    return;
  }
  int error = dx - dy;
  x = a.x;
  y = a.y;

  while(1) {
    if(x == b.x + step_x) {
      break;
    }
    if(y == b.y + step_y) {
      break;
    }
    draw_pixel(x, y, fb, colour);
    if((error << 1) >= -dy) {
      x += step_x;
      error -= dy;
    }
    if((error << 1) <= dx) {
      y += step_y;
      error += dx;
    }
  }
}

int min(int a, int b) {
  return a <= b ? a : b;
}

int max(int a, int b) {
  return a >= b ? a : b;
}

int min3(int a, int b, int c) {
  if(a <= b && a <= c) {
    return a;
  }
  if(b <= a && b <= c) {
    return b;
  }
  return c;
}

int max3(int a, int b, int c) {
  if(a >= b && a >= c) {
    return a;
  }
  if(b >= a && b >= c) {
    return b;
  }
  return c;
}


void sub_point(Point2D a, Point2D b, Point2D *p) {
  p->x = a.x - b.x;
  p->y = a.y - b.y;
}

float dot_product(Point2D a, Point2D b) {
  return (float) (a.x * b.x + a.y * b.y);
}

void get_barycentric(Point2D p, Point2D a, Point2D b, Point2D c,
    float *u, float *v, float *w) {
  Point2D v0;
  Point2D v1;
  Point2D v2;
  sub_point(b, a, &v0);
  sub_point(c, a, &v1);
  sub_point(p, a, &v2);

  float d00 = dot_product(v0, v0);
  float d01 = dot_product(v0, v1);
  float d11 = dot_product(v1, v1);
  float d20 = dot_product(v2, v0);
  float d21 = dot_product(v2, v1);

  *v = (float) ((d11 * d20 - d01 * d21) / (d00 * d11 - d01 * d01));
  *w = (float) ((d00 * d21 - d01 * d20) / (d00 * d11 - d01 * d01));
  *u = 1 - *v - *w;
}

/*software rasterizer TODO could be more efficient*/
void render_triangle(Point2D v0, Point2D v1, Point2D v2,
    volatile unsigned char *fb, uint16_t colour) {
  int min_x = min3(v0.x, v1.x, v2.x);
  int min_y = min3(v0.y, v1.y, v2.y);
  int max_x = max3(v0.x, v1.x, v2.x);
  int max_y = max3(v0.y, v1.y, v2.y);

  Point2D p;
  for (p.y = min_y; p.y <= max_y; p.y++) {
    for (p.x = min_x; p.x <= max_x; p.x++) {
      float w0, w1, w2;
      get_barycentric(p, v0, v1, v2, &w0, &w1, &w2);
      // If p is on or inside all edges, render pixel.
      if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
          /*TODO render nicely*/
        draw_pixel(p.x, p.y, fb, colour);
      }
    }
  }
}
