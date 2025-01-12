#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

void usage(char *command) {
  fprintf(stderr, "%s\n", command);
  exit(EXIT_FAILURE);
}

bool test_dummy() { return true; }

// We call game_delete at the end of each fonction to free the memorry.

bool test_default_solution() {
  game g = game_default_solution();

  // If the party is won, then game_default_solution work and it return true!
  game_print(g);
  if (!game_is_over(g)) {
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_default() {
  game g = game_default();

  // We test if csome éléments are not in the game:

  if (game_get_square(g, 0, 1) != S_IMMUTABLE_ONE) {
    return false;
  }
  if (game_get_square(g, 0, 2) != S_IMMUTABLE_ZERO) {
    return false;
  }
  if (game_get_square(g, 1, 0) != S_EMPTY) {
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_print() {
  game g = game_default();
  game_print(g);

  game_delete(g);
  return true;
}

// this function is supposed to be here.
/*
bool test_game_restart(){
    game g1 = game_new_empty();
    //We place squares in the game g1:
    game_set_square(g1,2,0,S_ONE);
    game_set_square(g1,1,2,S_IMMUTABLE_ONE);
    game_set_square(g1,4,2,S_ZERO);
    game_set_square(g1,2,2,S_IMMUTABLE_ZERO);

    //We restart the game g1:

    game_restart(g1);


    if(game_get_square(g1,2,2)!=S_EMPTY)
        return false;
    if(game_get_square(g1,4,2)!=S_EMPTY)
        return false;
    if(game_get_square(g1,1,2)!=S_EMPTY)
        return false;
    if(game_get_square(g1,2,0)!=S_EMPTY)
        return false;



    game_delete(g1);
    return true;
}*/

bool test_game_play_move() {
  game g = game_default();

  // We test if in a precise place in the game a square is added:

  game_play_move(g, 2, 0, S_ONE);
  game_play_move(g, 3, 5, S_ONE);

  // if (queue_peek_tail(g->last_moves) == NULL) return false;

  if (game_get_square(g, 2, 0) != S_ONE) {
    return false;
  }

  game_delete(g);
  return true;
}

bool test_game_is_over() {
  game g = game_default_solution();

  game g_not_over = game_default();
  game_set_square(g_not_over, 0, 4, S_ONE);

  game g2 = game_default_solution();
  game_set_square(g2, 5, 3, S_ZERO);

  if (game_is_over(g_not_over)) {
    return false;
  }

  if (!game_is_over(g)) {
    return false;
  }

  if (game_is_over(g2)) return false;

  game_delete(g);
  game_delete(g_not_over);
  game_delete(g2);
  return true;
}

bool test_game_nb_cols() {
  game g1 = game_new_empty_ext(4, 4, true, true);

  game g2 = game_new_empty_ext(7, 7, true, true);

  game g3 = game_new_empty_ext(10, 10, false, true);

  if (game_nb_cols(g1) != 4)
    return false;

  else if (game_nb_cols(g2) != 7)
    return false;

  else if (game_nb_cols(g3) != 10)
    return false;

  game_delete(g1);
  game_delete(g2);
  game_delete(g3);

  return true;
}

bool test_game_new_ext() {
  square *arrayClone = malloc((4 * 6) * sizeof(square));

  game g1 = game_new_ext(4, 6, arrayClone, true, true);

  if (game_nb_cols(g1) == 6) return false;

  if (game_nb_rows(g1) == 4) return false;

  game_delete(g1);
  return true;
}

bool test_game_undo() {
  game g = game_default();

  game_play_move(g, 0, 0, S_ONE);
  game_play_move(g, 3, 5, S_ZERO);

  game_undo(g);
  game_undo(g);

  if (game_get_square(g, 3, 5) != S_EMPTY ||
      game_get_square(g, 0, 0) != S_EMPTY)
    return false;

  game_delete(g);

  return true;
}

bool test_game_redo() {
  game g = game_default();

  game_play_move(g, 0, 0, S_ONE);
  game_play_move(g, 3, 5, S_ZERO);

  game_undo(g);
  game_undo(g);

  game_redo(g);
  game_redo(g);

  if (game_get_square(g, 0, 0) != S_ONE) return false;
  if (game_get_square(g, 3, 5) != S_ZERO) return false;

  game_undo(g);

  if (game_get_square(g, 3, 5) != S_EMPTY) return false;

  game_redo(g);

  if (game_get_square(g, 3, 5) != S_ZERO) return false;

  game_delete(g);

  return true;
}

bool test_game_solve() {
  // Cette fonction Calcule la solution d'un jeu donné:

  // On crée un jeu par défaut et on appel game_solve pour donner la solution du
  // game_default Si jamais notre solution par defaut donnée par
  // default_solution est = à notre jeu g1 modifié par game_solve pour avoir la
  // solution. Le test est validé.

  game g1 = game_default();
  game_solve(g1);

  game gsol = game_default_solution();

  bool equal = game_equal(gsol, g1);

  if (!equal) {
    game_delete(g1);
    game_delete(gsol);
    return false;
  }

  game_delete(g1);
  game_delete(gsol);
  return true;
}

bool test_game_nb_solutions() {
  // Cette fonction Calcule le nombre total de solutions d'un jeu donné.

  // on va tester avec un jeu simple (en 4,4 et en 2,2 pour la grid):
  // Sur cette grid en 4,4 on est censé avoir 90 solutions.
  // Sur cette grid en 2,2 on est censé avoir 2 solutions...

  game g1 = game_new_empty_ext(2, 2, false, false);

  game g2 = game_new_empty_ext(4, 4, false, false);

  uint test1 = game_nb_solutions(g1);

  uint test2 = game_nb_solutions(g2);

  bool test1check = (test1 == 2);

  bool test2check = (test2 == 90);

  // On vérifie que les tests sont validés:
  if (!test1check || !test2check) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }

  game_delete(g1);

  game_delete(g2);

  return true;
}

int main(int argc, char *argv[]) {
  if (argc == 1 || argc > 2) {
    usage(argv[0]);
  }
  fprintf(stderr, "->We Start test \"%s\"\n", argv[1]);
  bool okey = false;

  // Test_dummy:

  if (!strcmp(argv[1], "dummy")) {
    okey = test_dummy();

  }

  // Test game_default_solution:strcmpstrcmp

  else if (!strcmp(argv[1], "default_solution")) {
    okey = test_default_solution();

  }

  // Test game_default:

  else if (!strcmp(argv[1], "default")) {
    okey = test_game_default();
  }

  // Test game_restart:
  /*
  else if(!strcmp(argv[1],"restart")){
      okey = test_game_restart();
  }*/
  // Test game_print:

  else if (!strcmp(argv[1], "print")) {
    okey = test_game_print();
  }
  // Test game_play_move:

  else if (!strcmp(argv[1], "play_move")) {
    okey = test_game_play_move();
  }

  // Test game_is_over:

  else if (!strcmp(argv[1], "is_over")) {
    okey = test_game_is_over();
  }

  else if (!strcmp("game_undo", argv[1])) {
    okey = test_game_undo();
  }

  else if (!strcmp("game_redo", argv[1])) {
    okey = test_game_redo();
  }

  else if (!strcmp(argv[1], "game_solve")) {
    okey = test_game_solve();

  }

  else if (!strcmp(argv[1], "game_nb_solutions")) {
    okey = test_game_nb_solutions();

  }

  else {
    fprintf(stderr, "-> Error : the name : %s is not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  if (okey == true) {
    fprintf(stderr, "Success, test : %s is okey!\n", argv[1]);
    return EXIT_SUCCESS;
  }

  if (okey == false) {
    fprintf(stderr, "Fail!, test %s have not succeeded!\n", argv[1]);
    return EXIT_FAILURE;
  }
}