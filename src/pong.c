#include "pong.h"

void show_menu() {
  printf("\n\n\n\n");
  printf("    @       @@    @                      @\n");
  printf("  @ @      @  @   @                      @\n");
  printf(" @  @       @   @@@@@    @@@   @ @@@@  @@@@@      @@@    @@@   @@@ "
         "@@    @@@\n");
  printf("    @        @    @     @  @   @@        @       @   @  @  @   @  @  "
         "@  @   @\n");
  printf("    @         @   @     @  @   @         @       @   @  @  @   @  @  "
         "@  @@@@@\n");
  printf("    @      @ @    @ @   @  @   @         @ @     @   @  @  @   @  @  "
         "@  @\n");
  printf("    @ O     @      @     @@ @  @          @       @@@@   @@ @  @  @  "
         "@   @@@\n");
  printf("                                                     @\n");
  printf("                                                 @@@@\n");
  printf("   @@@     @@@@@         @    @\n");
  printf("  @   @    @                  @\n");
  printf("      @    @      @   @  @  @@@@@      @@@    @@@   @@@ @@    @@@\n");
  printf(
      "     @     @@@@@   @ @   @    @       @   @  @  @   @  @  @  @   @\n");
  printf(
      "    @      @        @    @    @       @   @  @  @   @  @  @  @@@@@\n");
  printf("   @       @       @ @   @    @ @     @   @  @  @   @  @  @  @\n");
  printf("  @@@@@ O  @@@@@  @   @  @     @       @@@@   @@ @  @  @  @   @@@\n");
  printf("                                          @\n");
  printf("                                      @@@@\n");
}

char get_user_input() {
  char buf = 0;
  struct termios old = {0};
  fflush(stdout);
  if (tcgetattr(0, &old) < 0)
    perror("tcsetattr()");
  old.c_lflag &= ~(ICANON | ECHO);
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0)
    perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0)
    perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0)
    perror("tcsetattr ~ICANON");
  return buf;
}

void show_scores(unsigned score_1, unsigned score_2) {
  for (int x = 0; x < GAME_WIDTH; x++)
    printf(" ");
  printf("\n");
  for (int x = 0; x < GAME_WIDTH; x++) {
    if (x == PLAYER1_SCORE_X)
      printf("Player1: %d", score_1);
    else if (x == PLAYER2_SCORE_X)
      printf("Player2: %d", score_2);
    else
      printf(" ");
  }
  printf("\n");
  for (int x = 0; x < GAME_WIDTH; x++)
    printf(" ");
  printf("\n");
}

void show_field_part(unsigned x, unsigned y, unsigned ball_x, unsigned ball_y,
                     unsigned racquet1_y, unsigned racquet2_y) {
  if (y == 0 || y == GAME_HEIGHT || x == 0 || x == GAME_WIDTH) {
    printf("#");
  } else if (x > 0 && x < GAME_WIDTH) {
    if (x == ball_x && y == ball_y) {
      printf("@");
    } else if (x == FIELD_WIDTH / 2 && y > 0 && y < GAME_HEIGHT) {
      printf("|");
    } else if ((x == RACQUET1_X && y <= racquet1_y && y >= racquet1_y - 2) ||
               (x == RACQUET2_X && y <= racquet2_y && y >= racquet2_y - 2)) {
      printf("N");
    } else if (y > 0 && y < GAME_HEIGHT) {
      printf(" ");
    }
  }
}

void show_start_hints() {
  printf("\n\n\t\t\t   'q'     - exit the game\n");
  printf("\n\t\t\t   'space' - start playing\n");
}

void show_initial_field(unsigned score_1, unsigned score_2) {
  show_scores(score_1, score_2);
  for (int y = 0; y <= GAME_HEIGHT; y++) {
    for (int x = 0; x <= GAME_WIDTH; x++)
      show_field_part(x, y, DEFAULT_BALL_X, DEFAULT_BALL_Y, DEFAULT_RAC_Y,
                      DEFAULT_RAC_Y);
    printf("\n");
  }
}

void show_current_field(unsigned ball_x, unsigned ball_y, unsigned racquet1_y,
                        unsigned racquet2_y, unsigned score_1,
                        unsigned score_2) {
  // draw field
  show_scores(score_1, score_2);
  for (int y = 0; y <= GAME_HEIGHT; y++) {
    if (score_1 == 21 || score_2 == 21)
      break;
    for (int x = 0; x <= GAME_WIDTH; x++)
      show_field_part(x, y, ball_x, ball_y, racquet1_y, racquet2_y);
    printf("\n");
  }
}

void show_win_field(unsigned player_id) {
  for (int y = 0; y <= GAME_HEIGHT; y++) {
    for (int x = 0; x <= GAME_WIDTH; x++) {
      if (y == 0 || y == GAME_HEIGHT || x == 0 || x == GAME_WIDTH)
        printf("#");
      if (x > 0 && x < GAME_WIDTH && y > 0 && y < GAME_HEIGHT)
        printf(" ");
      if (x == WIN_LABEL_X && y == GAME_HEIGHT / 2) {
        printf("Player%d wins!", player_id);
        x += 12;
      }
    }
    printf("\n");
  }
}

int start_game() {
  // set ball initial coordinates
  unsigned ball_x = DEFAULT_BALL_X, ball_y = DEFAULT_BALL_Y;

  // ball direction axices
  int ball_speed_x = 1, ball_speed_y = 1;

  // set racquets initial coordinates
  unsigned racquet1_y, racquet2_y;
  racquet2_y = racquet1_y = DEFAULT_RAC_Y;
  // players score
  unsigned player1_score, player2_score;
  player2_score = player1_score = 0;

  while (1) {
    system("clear");
    unsigned player1_old_score = player1_score,
             player2_old_score = player2_score;

    ball_x += ball_speed_x;
    ball_y += ball_speed_y;
    if (ball_x <= RACQUET1_X - 2) {
      player2_score++;
    } else if (ball_x >= RACQUET2_X + 2) {
      player1_score++;
      // ball hits the racquets
    } else if ((ball_x == RACQUET1_X + 1 && ball_y <= racquet1_y &&
                ball_y >= racquet1_y - 2) ||
               (ball_x == RACQUET2_X - 1 && ball_y <= racquet2_y &&
                ball_y >= racquet2_y - 2)) {
      // ball hits vertical/horizontal bounds also
      if (ball_y == GAME_HEIGHT - 1 || ball_y == 1) {
        ball_speed_x *= -1;
        ball_speed_y *= -1;
      } else {
        ball_speed_x *= -1;
      }
      // ball hits the vertical/horizontal bounds
    } else if (ball_y == GAME_HEIGHT - 1 || ball_y == 1) {
      ball_speed_y *= -1;
      // ball hits the end of the racquet 1
    } else if (ball_x == RACQUET1_X) {
      if (ball_y == racquet1_y - 3 || ball_y == racquet1_y + 1)
        ball_speed_y *= -1;
      // ball hits the end of the racquet 2
    } else if (ball_x == RACQUET2_X) {
      if (ball_y == racquet2_y - 3 || ball_y == racquet2_y + 1)
        ball_speed_y *= -1;
      // ball hits the corners of the racquet 1
    } else if (ball_x == RACQUET1_X + 1) {
      if ((ball_y == racquet1_y + 1 && ball_speed_y == -1 &&
           ball_speed_x == -1) ||
          (ball_y == racquet1_y - 3 && ball_speed_y == 1 &&
           ball_speed_x == -1)) {
        ball_speed_x *= -1;
        ball_speed_y *= -1;
      }
      // ball hits the corners of the racquet 2
    } else if (ball_x == RACQUET2_X - 1) {
      if ((ball_y == racquet2_y - 3 && ball_speed_y == 1 &&
           ball_speed_x == 1) ||
          (ball_y == racquet2_y + 1 && ball_speed_y == -1 &&
           ball_speed_x == 1)) {
        ball_speed_x *= -1;
        ball_speed_y *= -1;
      }
    }

    show_current_field(ball_x, ball_y, racquet1_y, racquet2_y, player1_score,
                       player2_score);
    char c;
    while (1) {
      c = get_user_input();
      if (c == 'a' && racquet1_y > 3) {
        racquet1_y--;
        break;
      } else if (c == 'z' && racquet1_y < GAME_HEIGHT - 1) {
        racquet1_y++;
        break;
      } else if (c == 'k' && racquet2_y > 3) {
        racquet2_y--;
        break;
      } else if (c == 'm' && racquet2_y < GAME_HEIGHT - 1) {
        racquet2_y++;
        break;
      } else if (c == ' ') {
        break;
      } else if (c == 'q') {
        return 1;
      }
    }

    if (player1_score == 21 || player2_score == 21) {
      system("clear");
      show_win_field((player1_score == 21) ? 1 : 2);
      break;
    } else if (player1_score > player1_old_score ||
               player2_score > player2_old_score) {
      system("clear");
      show_initial_field(player1_score, player2_score);
      ball_x = DEFAULT_BALL_X;
      ball_y = DEFAULT_BALL_Y;
      racquet1_y = racquet2_y = DEFAULT_RAC_Y;
      if (player1_score > player1_old_score) {
        ball_speed_x = -1;
        ball_speed_y = -1;
      } else if (player2_score > player2_old_score) {
        ball_speed_x = 1;
        ball_speed_y = 1;
      }
      get_user_input();
    }
    // delay for realistic interaction
    usleep(80000);
  }

  return 0;
}

int skip_to_new_line() {
  int spaces_only = 1, ch;
  while ((ch = getchar()) != '\n') {
    if (!isspace(ch))
      spaces_only = 0;
  }
  return spaces_only;
}

int main() {
  system("clear");
  show_menu();
  int choice, flag = 1;
  while (flag) {
    printf("Choose an option: ");
    int res = scanf("%d", &choice);
    int spaces_only = skip_to_new_line();
    if (res != 1 || !spaces_only) {
      printf("Invalid option, try again!\n");
    } else if (choice != 1 && choice != 2) {
      printf("Invalid option, try again!\n");
    } else if (choice == 1) {
      system("clear");
      show_initial_field(0, 0);
      show_start_hints();
      while (1) {
        char c = get_user_input();
        if (c == ' ') {
          break;
        } else if (c == 'q') {
          system("clear");
          printf("Exiting game\n");
          sleep(1);
          return 0;
        }
      }
      srand(time(0));
      int status = start_game();
      if (status == 1) {
        system("clear");
        printf("Exiting game\n");
      }
      sleep(1);
      flag = 0;
    } else if (choice == 2) {
      flag = 0;
    }
  }

  return 0;
}
