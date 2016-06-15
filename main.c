#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pacman.h"
#include "game.h"
#include "render.h"
#include "rpi-mailbox-interface.h"
#include "background.h"
#include "food_sprite.h"
#include "buttons.h"
#include "print_number.h"
#include "print_string.h"
#include "title_screen.h"

/** Main function - we'll never return from here */
void kernel_main( unsigned int r0, unsigned int r1, unsigned int atags )
{
  volatile unsigned char* fb = NULL;

  rpi_mailbox_property_t* mp;

  /* Ensure the ARM is running at it's maximum rate */
  RPI_PropertyInit();
  RPI_PropertyAddTag( TAG_SET_CLOCK_RATE, TAG_CLOCK_ARM, mp->data.buffer_32[1] );
  RPI_PropertyProcess();

  mp = RPI_PropertyGet( TAG_GET_CLOCK_RATE );

  /* Initialise a framebuffer */
  RPI_PropertyInit();
  RPI_PropertyAddTag( TAG_ALLOCATE_BUFFER );
  RPI_PropertyAddTag( TAG_SET_PHYSICAL_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT );
  RPI_PropertyAddTag( TAG_SET_VIRTUAL_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT << 1);
  RPI_PropertyAddTag( TAG_SET_VIRTUAL_OFFSET, 0, 0 );
  RPI_PropertyAddTag( TAG_SET_DEPTH, SCREEN_DEPTH );
  RPI_PropertyProcess();

  if( ( mp = RPI_PropertyGet( TAG_ALLOCATE_BUFFER ) ) ) {
    fb = (unsigned char*)mp->data.buffer_32[0];
  }

  int offset = SCREEN_HEIGHT;

  Sprite *background  = (Sprite *) malloc(sizeof(Sprite));
  background->bitmap  = background_data;
  background->width   = 224;
  background->height  = 288;

  Sprite *food_sprite = (Sprite *) malloc(sizeof(Sprite));
  food_sprite->bitmap = food;
  food_sprite->width  = 8;
  food_sprite->height = 8;

  Sprite *power_up_sprite = (Sprite *) malloc(sizeof(Sprite));
  power_up_sprite->bitmap = power_up;
  power_up_sprite->width  = 8;
  power_up_sprite->height = 8;

  Sprite *fruit_sprite = (Sprite *) malloc(sizeof(Sprite));
  fruit_sprite->bitmap = cherry_sprite;
  fruit_sprite->width  = 16;
  fruit_sprite->height = 16;

  for (int y = 0; y < SCREEN_HEIGHT << 1; ++y) {
    for (int x = 0; x < SCREEN_WIDTH; ++x) {
      draw_pixel(x, y, fb, 0);
    }
  }

  draw_title_screen(fb, 0);
  while(!poll_controller()){ /*do nothing */ }

  uint8_t *game_board;

  INIT: game_board = init_game_board();
  game *game_ptr = init_game(game_board);
  uint8_t  game_pace = 0;
  uint64_t score_for_next_life = SCORE_FOR_LIFE;

  NEW_LEVEL: for (int y = 0; y < SCREEN_HEIGHT ; ++y) {
    for (int x = 0; x < SCREEN_WIDTH; ++x) {
      draw_pixel(x, y, fb, 0);
    }
  }

  reset_game_board(game_board);
  game_ptr->pacman_ptr = init_pacman();
  if(game_ptr->level == 1) {
    fruit_sprite->bitmap = cherry_sprite;
  } else if(game_ptr->level <= 2) {
    fruit_sprite->bitmap = strawberry_sprite;
  } else if(game_ptr->level <= 4) {
    fruit_sprite->bitmap = orange_sprite;
  } else if(game_ptr->level <= 6) {
    fruit_sprite->bitmap = apple_sprite;
  } else if(game_ptr->level <= 8) {
    fruit_sprite->bitmap = melon_sprite;
  } else if(game_ptr->level <= 10) {
    fruit_sprite->bitmap = bell_sprite;
  } else if(game_ptr->level <= 12) {
    fruit_sprite->bitmap = galaxian_sprite;
  } else {
    fruit_sprite->bitmap = key_sprite;
  }

  NEW_LIFE: init_ghosts(game_ptr);

  for (int y = 0; y < SCREEN_HEIGHT << 1; ++y) {
    for (int x = 0; x < SCREEN_WIDTH; ++x) {
      draw_pixel(x, y, fb, 0);
    }
  }

  draw_background(background, 0, fb);
  draw_background(background, SCREEN_HEIGHT, fb);

  int pacman_bx = 0;
  int pacman_by = 0;
  int ghost_bx  = 0;
  int ghost_by  = 0;

  int curr_direction    = 0;
  int next_direction    = 0;
  int animation_counter = 0;
  int power_up_counter  = 0;

  ghost* ghosts[] = { game_ptr->ghosts_ptr->blinky,
                      game_ptr->ghosts_ptr->pinky,
                      game_ptr->ghosts_ptr->inky,
                      game_ptr->ghosts_ptr->clyde };
  /*TODO NEED TO ACTUALLY FREE*/
  init_ghosts_sprite(ghosts);

  uint64_t main_timer;
  uint64_t frame_timer;
  uint64_t game_timer;
  uint64_t frightened_timer;
  uint64_t fruit_timer;
  uint64_t game_plan[3][7] = {{7000000, 20000000, 7000000, 20000000, 5000000, 20000000, 5000000},
                              {7000000, 20000000, 7000000, 20000000, 5000000, 1033000000, 16667},
                              {5000000, 20000000, 5000000, 20000000, 5000000, 1037000000, 16667}};
  uint8_t  game_tick = 0;
  uint8_t  frightened_mode = 0;
  uint8_t  frightened_mode_counter = 0;
  uint32_t prev_score = 0;
  timer_start(&main_timer);
  timer_start(&game_timer);
  change_state(1, game_ptr, NULL, SCATTER);

  while( 1 ) {
    timer_start(&frame_timer);
    if(game_plan[game_pace][game_tick] < timer_get_relative(game_timer) && game_tick != 7 && !frightened_mode) {
      timer_start(&game_timer);
      game_tick++;
      if(!(game_tick % 2)) {
        change_state(1, game_ptr, NULL, SCATTER);
      } else {
        change_state(1, game_ptr, NULL, CHASE);
      }
    }
    if(got_power_up(game_ptr)) {
      timer_start(&frightened_timer);
      frightened_mode = 1;
      for (int i = 0; i < NUM_GHOSTS; ++i) {
        ghosts[i]->nearly_ending = 0;
      }
      change_state(1, game_ptr, NULL, FRIGHTENED);
    }

    if(frightened_mode) {
      if(timer_get_relative(frightened_timer) > 5500000) {
        if(frightened_mode_counter == 15) {
          for (int i = 0; i < NUM_GHOSTS; ++i) {
            ghosts[i]->nearly_ending = 1;
          }
        }
        if(frightened_mode_counter == 0) {
          for (int i = 0; i < NUM_GHOSTS; ++i) {
            ghosts[i]->nearly_ending = 0;
          }
        }
        frightened_mode_counter++;
        frightened_mode_counter %= 30;
      }

      if(timer_get_relative(frightened_timer) > 8000000) {
        frightened_mode = 0;
        game_timer += timer_get_relative(frightened_timer);
        for (int i = 0; i < NUM_GHOSTS; ++i) {
          ghosts[i]->nearly_ending = 0;
        }
        if(!(game_tick % 2)) {
          change_state(1, game_ptr, NULL, SCATTER);
        } else {
          change_state(1, game_ptr, NULL, CHASE);
        }
      }
    }

    uint8_t input = poll_controller();
    game_ptr->pacman_ptr->prev_vec = game_ptr->pacman_ptr->vec;
    if(input == 1) {
      game_ptr->pacman_ptr->vec.x = 1;
      game_ptr->pacman_ptr->vec.y = 0;
      next_direction = PACMAN_RIGHT;
    } else if(input == 2) {
      game_ptr->pacman_ptr->vec.x = -1;
      game_ptr->pacman_ptr->vec.y = 0;
      next_direction = PACMAN_LEFT;
    } else if(input == 4) {
      game_ptr->pacman_ptr->vec.x = 0;
      game_ptr->pacman_ptr->vec.y = 1;
      next_direction = PACMAN_DOWN;
    } else if(input == 8) {
      game_ptr->pacman_ptr->vec.x = 0;
      game_ptr->pacman_ptr->vec.y = -1;
      next_direction = PACMAN_UP;
    }
    is_valid_vector(game_board, game_ptr->pacman_ptr);
    if(curr_direction != next_direction) {
      change_pacman_animation(game_ptr->pacman_ptr);
      curr_direction = next_direction;
    } else {
      animation_counter += 1;
      if(animation_counter > 3 && (game_ptr->pacman_ptr->vec.x || game_ptr->pacman_ptr->vec.y)) {
        change_pacman_animation(game_ptr->pacman_ptr);
        animation_counter = 0;
      }
    }
    change_ghost_animation(ghosts);
    is_valid_vector(game_board, game_ptr->pacman_ptr);
    update_board(game_board, game_ptr->pacman_ptr, game_ptr);
    /* TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO set fruits */
    if(game_ptr->pacman_ptr->dots == 70) {
      /*first fruit*/
      timer_start(&fruit_timer);
      game_ptr->fruit_set = 1;
      set_board_value(13, 17, FRUIT, game_ptr->board);
    }
    if(game_ptr->pacman_ptr->dots == 170) {
      /*second fruit*/
      timer_start(&fruit_timer);
      game_ptr->fruit_set = 1;
      set_board_value(13, 17, FRUIT, game_ptr->board);
    }
    if((timer_get_relative(fruit_timer) > 10000000 && game_ptr->fruit_set) || game_ptr->fruit_set > 60) {
      /*remove fruit*/
      set_board_value(13, 17, EMPTY, game_ptr->board);
      game_ptr->fruit_set = 0;
      remove_fruit(fb);
    }
    /* TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO set fruits */

    print_string("LEVEL: ", X_OFFSET_GAME, Y_OFFSET_GAME - 19, offset, fb);
    print_number(game_ptr->level, X_OFFSET_GAME + (17 * 8), Y_OFFSET_GAME - 19, fb, offset);
    print_string("SCORE: ", X_OFFSET_GAME, Y_OFFSET_GAME - 10, offset, fb);
    print_number(game_ptr->score, X_OFFSET_GAME + (17 * 8), Y_OFFSET_GAME - 10, fb, offset);

    if(detect_collision(game_ptr, ghosts, offset, fb)) {
      game_ptr->lives -= 1;
      if(!game_ptr->lives) {
        goto INIT;
      } else {
        //reset pacmans pos
        int prev_dots = game_ptr->pacman_ptr->dots;
        game_ptr->pacman_ptr = init_pacman();
        game_ptr->pacman_ptr->dots = prev_dots;
        goto NEW_LIFE;
      }
    }
/*==============================GRAPHICS==========================*/
    get_pacman_board_coord(game_ptr->pacman_ptr, &pacman_bx, &pacman_by);
    draw_background_16_box(background, pacman_bx, pacman_by, offset, fb);
    for (int i = 0; i < NUM_GHOSTS; ++i) {
      get_ghost_board_coord(ghosts[i], &ghost_bx, &ghost_by);
      draw_background_16_box(background, ghost_bx, ghost_by, offset, fb);
    }

    draw_background_teleport(background, offset, fb);
    draw_lives(game_ptr->lives, offset, fb);
    if(game_ptr->fruit_set) {
      if(game_ptr->fruit_set == 1) {
        draw_fruit(fruit_sprite, offset, fb);
      } else {
        /*draw fruit score*/
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
        draw_fruit_score(fruit_score, offset, fb);
        game_ptr->fruit_set++;
      }
    }

    draw_food(food_sprite, game_board, offset, fb);
    draw_power_up(power_up_sprite, game_board, offset, &(power_up_counter), fb);
    draw_pacman(game_ptr->pacman_ptr, offset, fb);

    for (int i = 0; i < NUM_GHOSTS; ++i) {
      draw_ghost(ghosts[i], offset, fb);
    }

    shortest_path_helper(game_ptr);
    if(!(game_ptr->score - prev_score == 10)) {
      /*skip frame otherwise*/
      control_pacman_speed(game_ptr->pacman_ptr);
    }
    prev_score = game_ptr->score;
    for (int i = 0; i < NUM_GHOSTS; ++i) {
      control_ghost_speed(ghosts[i]);
    }

    // switch between front and back buffer
    RPI_PropertyInit();
    if(!offset) {
      offset = SCREEN_HEIGHT;
      RPI_PropertyAddTag( TAG_SET_VIRTUAL_OFFSET, 0, 0 );
      RPI_PropertyProcess();
    } else {
      offset = 0;
      RPI_PropertyAddTag( TAG_SET_VIRTUAL_OFFSET, 0, SCREEN_HEIGHT);
      RPI_PropertyProcess();
    }

    if(game_won(game_board)) {
      display_end_screen("YOU WIN!", "PRESS ANY BUTTON FOR NEXT LEVEL", offset, fb);
      game_ptr->level += 1;
      if(game_ptr->level < 5){
        game_pace = 1;
      } else {
        game_pace = 2;
      }
      goto NEW_LEVEL;
    }

    if(game_ptr->score >= score_for_next_life){
      score_for_next_life += SCORE_FOR_LIFE;
      if(game_ptr->lives < MAX_LIVES) {
        game_ptr->lives += 1;
      }
    }

    /*keep constant 60 fps*/
    while(timer_get_relative(frame_timer) < 16667);
  }
}
