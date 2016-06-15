#ifndef RENDER_H
#define RENDER_H value
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "game.h"
typedef struct {
  int x;
  int y;
} Point2D;

#define TUNNEL_LEFT1(x, y)  ((x == 8)               && (y == 148))
#define TUNNEL_LEFT2(x, y)  ((x == 8)               && (y == 149))
#define TUNNEL_RIGHT1(x, y) ((x == PIXEL_WIDTH - 8) && (y == 148))
#define TUNNEL_RIGHT2(x, y) ((x == PIXEL_WIDTH - 8) && (y == 149))


void draw_pixel(int, int, volatile unsigned char *, uint16_t);
void draw_background_teleport(Sprite *sprite, int offset, volatile unsigned char *fb);
void draw_pacman(pacman *, int offset, volatile unsigned char *fb);
void draw_ghost(ghost* ghost_ptr, int offset, volatile unsigned char *fb);
void draw_ghost_score(ghost* ghost_ptr, pacman *pacman_ptr, int point, int offset, volatile unsigned char *fb);
void draw_sprite(Sprite *sprite, int x_pos, int y_pos, volatile unsigned char *fb);
void draw_food(Sprite *, uint8_t *,int, volatile unsigned char *);
void draw_fruit(Sprite *sprite, int offset, volatile unsigned char *fb);
void draw_fruit_score(int point, int offset, volatile unsigned char *fb);
void remove_fruit(volatile unsigned char *fb);
void draw_power_up(Sprite *, uint8_t *, int, int *, volatile unsigned char *);
void draw_background(Sprite *sprite, int offset, volatile unsigned char *fb);
void draw_background_16_box(Sprite *sprite, int x, int y, int offset, volatile unsigned char *fb);
void draw_lives(int lives, int offset, volatile unsigned char *fb);
void render_line(Point2D, Point2D, volatile unsigned char *, uint16_t);

int min(int a, int b);
int max(int a, int b);
int min3(int a, int b, int c);
int max3(int a, int b, int c);
void sub_point(Point2D a, Point2D b, Point2D *p);
float dot_product(Point2D a, Point2D b);
void get_barycentric(Point2D p, Point2D a, Point2D b, Point2D c, float *u, float *v, float *w);
void render_triangle(Point2D v0, Point2D v1, Point2D v2, volatile unsigned char *fb, uint16_t colour);
#endif
