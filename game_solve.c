#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "annex_funcs.h"
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"

int main(int argc, char *argv[]) {
  check((argc == 3 || argc == 4), "wrong argument composition");
  bool output_desired = false;

  char *option = argv[1];
  char *input = argv[2];
  if (argc == 4) {
    output_desired = true;
    char *output = argv[3];
    FILE *f = fopen(output, "w");
  }

  game g = game_load(input);

  switch (option) {
    case "-s":
      game_solve(g);
      if (output_desired)
        game_save(g, output);
      else
        game_print(g);
      break;

    case "-c":
      int nb_solutions = game_nb_solutions(g);
      if (output_desired)
        fprintf(output, "%d", nb_solutions);
      else
        printf("%d", nb_solutions);
      break;

    default:
      fprintf(stderr, "option saisie invalide %s\n", option);
      break;
  }

  return EXIT_SUCCESS;
}