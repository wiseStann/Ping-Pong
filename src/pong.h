/*
   ######################################################
   #                                                    #
   #                         |                          #
   #                         |                          #
   #                         |                          #
   #          N              |               N          #
   #          N              @               N          #
   #          N              |               N          #
   #                         |                          #
   #                         |                          #
   #                         |                          #
   #                                                    #
   ######################################################
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#define FIELD_WIDTH     80
#define FIELD_HEIGHT    25

#define GAME_WIDTH      (FIELD_WIDTH + 2)
#define GAME_HEIGHT     (FIELD_HEIGHT + 2)

#define RACQUET1_X      (FIELD_WIDTH / 4 - 10)
#define RACQUET2_X      (FIELD_WIDTH - FIELD_WIDTH / 4 + 10)

#define DEFAULT_BALL_X  (FIELD_WIDTH / 2)
#define DEFAULT_BALL_Y  (GAME_HEIGHT / 2 + 1)

#define DEFAULT_RAC_Y   (DEFAULT_BALL_Y + 1)

#define PLAYER1_SCORE_X 2
#define PLAYER2_SCORE_X (FIELD_WIDTH - 20)

#define WIN_LABEL_X     (GAME_WIDTH / 3 + 6)


void show_menu();

char get_user_input();

void show_scores(unsigned, unsigned);

// x, y, ball x, ball y, rac1 y, rac2 y
void show_field_part(unsigned, unsigned, unsigned, unsigned, unsigned, unsigned);

void show_start_hints();
void show_initial_field();
void show_current_field();
void show_win_field();

// ball direction (0 - up right, 1 - up left, 2 - down left, 3 - down right)
int start_game();
