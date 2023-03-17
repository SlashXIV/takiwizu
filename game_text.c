#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "annex_funcs.h"
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"

void list_error_each_square(cgame g) {
  for (uint i = 0; i < game_nb_rows(g); i++)
    for (uint j = 0; j < game_nb_cols(g); j++)
      if (game_has_error(g, i, j)) printf("Error at square (%d, %d)\n", i, j);
}

void play_on_square(game g, int i, int j, int move) {
  char pion;

  switch (move) {
    case S_ZERO:
      pion = 'w';
      break;

    case S_ONE:
      pion = 'b';
      break;

    case S_EMPTY:
      pion = 'e';
      break;
  }

  printf("\naction : play move '%c' into square (%d, %d)\n", pion, i, j);

  if (game_check_move(g, i, j, move)) {
    game_play_move(g, i, j, move);
  } else {
    printf("sorry, wrong move :(\n");
  }
}

void display_help() {
  printf("- press 'w <i> <j>' to put a zero/white at square (i,j)\n");
  printf("- press 'b' <i> <j>' to put a one/black at square (i,j)\n");
  printf("- press 'e' <i> <j>' to empty square (i,j)\n");
  printf("- press 'r' to restart\n");
  printf("- press 'q' to quit\n");
  printf("- press 'z' to undo\n");
  printf("- press 'r' to redo\n");
  printf("- press 's' <filename> to save current game\n");
}

int main(int argc, char *argv[]) {
  game g;

  if (argc == 2) {
    g = game_load(argv[1]);
    check(g != NULL, "file not found, try again!");
    printf("game from \"%s\" successfully loaded !\n", argv[1]);
  } else {
    g = game_default();
  }

  while (!game_is_over(g)) {
    printf("\n");
    game_print(g);

    list_error_each_square(g);

    printf("\n? [h for help]\n");

    char input_char;

    int s =
        scanf(" %c", &input_char);  // space before format specifier implies not
                                    // consedirating any spaces & line breaks

    if (s == EOF) return EXIT_FAILURE;

    switch (input_char) {
      case 'h':
        printf("\naction : help\n");
        display_help();
        break;

      case 'r':
        printf("\naction : restart\n");
        game_restart(g);
        break;

      case 'q':
        printf("\naction : quit\nwhat a shame, you gave up ;-;\n");
        game_delete(g);
        return EXIT_SUCCESS;
        break;

      case 'z':
        printf("\naction : undo\n");
        game_undo(g);
        break;

      case 'y':
        printf("\naction : redo\n");
        game_redo(g);
        break;

      case 's':
        printf("\naction : save game\n");
        char filename_input[255];
        int s2 = scanf(" %s", filename_input);
        if (s2 == EOF) return EXIT_FAILURE;
        if (strlen(filename_input) == 0) {
          printf("filename required, please retry !\n");
          break;
        }
        game_save(g, filename_input);
        break;

      // PLAY
      case 'w':
      case 'b':
      case 'e':;

        // si la valeur du scanf vaut 999 alors on a un problème

        // ON POSITION
        uint i, j;
        int s3 = scanf("%d %d", &i, &j);
        if (s3 == EOF) return EXIT_FAILURE;
        switch (input_char) {
          // WHITE
          case 'w':
            play_on_square(g, i, j, S_ZERO);
            break;

          // BLACK
          case 'b':
            play_on_square(g, i, j, S_ONE);
            break;

          // EMPTY
          case 'e':
            play_on_square(g, i, j, S_EMPTY);
            break;
        }
    }
  }

  // WIN CONDITION (OUT OF WHILE LOOP)
  game_print(g);
  printf("congratulation, gg wp, you did it ;] !\n");
  return EXIT_SUCCESS;
}
