#include <stdbool.h>

#include "game.h"

// verifies if the expression is true, else it abort on error_code
void check(bool expr, const char* error_code);

// returns a dynamic-allocated square array identical to the first parameter
square* grid_copy(square* squares_primal, uint nb_squares);

// returns true if both squares are equals, false otherwise
bool identical_squares(square S1, square S2);

// returns true if both game dimensions are equals, false otherwise
bool identical_game_dimension(cgame g1, cgame g2);

// returns true if the square is a one (immutable or not), false otherwise
bool one_square(square s);

// returns true if the square is a zero (immutable or not), false otherwise
bool zero_square(square s);

// returns true if empty square, false otherwise
bool empty_square(square s);

// returns true if IMMUTABLE square, false otherwise
bool immutable_square(square s);

// returns true if there is 3 consecutives equal squares in the game, false
// otherwise
bool three_identical_consecutives_squares_on_pos(cgame g, uint i, uint j);

// verifies that there is the same amount of one and zero in the lines
bool parity_lines(cgame g, uint i, uint j);

// return true if there is two equals lines in the game
bool unicity_disrespected(cgame g, uint i, uint j);

// return true if the parameter is even, false otherwise
bool is_even(uint n);

// generate the first solution found for the current game given
void generate_first_solution(game solution, game g, int pos, int len,
                             uint* count, bool solved, int nb_rows,
                             int nb_cols);

// generate all the solutions for the current game given
uint generate_all_solutions(int pos, int len, game g, uint nb_cols);

// Replace all the squares by the squares of an other game
void game_replace(game g, game g2);

// Clear all game to empty squares
void game_clear(game g);

game game_custom1(void);