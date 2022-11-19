#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"

struct game_s{
    square * ArrayOfSquare;

};

//We have to implement the functions now:

game game_new(square * squares){ // robs
    if (squares == NULL){
        fprintf(stderr, "ERROR -> game_new(square * squares) : invalid  squares parameter; pointing on nothing...");
        exit(EXIT_FAILURE);
    }

    game new_game = malloc(sizeof(game));
    new_game->ArrayOfSquare = squares;
    return new_game;
}

game game_new_empty(void){ // robs
    uint nb_cases = DEFAULT_SIZE * DEFAULT_SIZE;
    
    // automatically generate an empty square array with calloc (0 <=> S_EMPTY)
    square * squares_empty = calloc(nb_cases, sizeof(square)); 

    // using game_new with our empty squares ==> creation of empty game
    game new_empty_game = game_new(squares_empty);

    return new_empty_game;
}

game game_copy(cgame g){ // robs
    if (g == NULL){
        fprintf(stderr, "ERROR -> game_copy(cgame g) : invalid g parameter; pointing on nothing...");
        exit(EXIT_FAILURE);
    } 

    // sending back a new game with the actual state of squares of g
    return game_new(g->ArrayOfSquare);
}

bool game_equal(cgame g1, cgame g2){ // robs
    if (g1 == NULL || g2 == NULL){
        fprintf(stderr, "ERROR -> game_equal(cgame g1, cgame g2 : one of the two games are pointing on nothing...");
        exit(EXIT_FAILURE);
    }

    // traverse and compare every square of both games
    for (int x = 0; x < DEFAULT_SIZE; x++){
        for (int y = 0; y < DEFAULT_SIZE; y++){
            square g1_actual_square = game_get_square(g1, x, y);
            square g2_actual_square = game_get_square(g2, x, y);
            if (g1_actual_square != g2_actual_square){
                // unequal squares on same pos
                return false;
            }
        }
    }
    // out of the loop -> identical games 
    return true;
}

void game_delete(game g){ // robs
    free(g->ArrayOfSquare);
    free(g);
}

void game_set_square(game g, uint i, uint j, square s){ // robs
    if (g == NULL){ 
        fprintf(stderr, "ERROR on game_set_square(game g, uint i, uint j, square s) : invalid parameters; g pointing on nothing...");
        exit(EXIT_FAILURE);
    } else if (i > DEFAULT_SIZE || j > DEFAULT_SIZE) {
        fprintf(stderr, "ERROR on game_set_square(game g, uint i, uint j, square s) : invalid parameters; i or j aren't under DEFAULT_SIZE (%d) ...", DEFAULT_SIZE);
        exit(EXIT_FAILURE);
    } else if (s < S_EMPTY || s > S_IMMUTABLE_ONE) { // s in [0;4]
        fprintf(stderr, "ERROR on game_set_square(game g, uint i, uint j, square s) : invalid parameters; the square s (%u) isn't a square ...", s);
        exit(EXIT_FAILURE);
    }

    // CIBLE VERROUILÉE ...
    uint index_to_set = (i * DEFAULT_SIZE) + j;

    // FIRE (LET THAT F** SQUARE IN) 
    g->ArrayOfSquare[index_to_set] = s;
}

square game_get_square(cgame g, uint i, uint j){ // robs
    if (g == NULL){ 
        fprintf(stderr, "ERROR on game_get_square(game g, uint i, uint j : invalid parameters; g pointing on nothing...");
        exit(EXIT_FAILURE);
    } else if (i > DEFAULT_SIZE || j > DEFAULT_SIZE) {
        fprintf(stderr, "ERROR on game_get_square(game g, uint i, uint j, : invalid parameters; i (%u) or j (%u) over DEFAULT_SIZE (%d) ...", i, j, DEFAULT_SIZE);
        exit(EXIT_FAILURE);
    }

    // CIBLE VERROUILLÉE ...
    uint index_to_get = (i * DEFAULT_SIZE) + j;

    // GET THAT SQUARE AND GIVE IT BACK
    return g->ArrayOfSquare[index_to_get];
}

int game_get_number(cgame g, uint i, uint j){ //gab

    int findInArray = (i*6)+j;

    return g->ArrayOfSquare[findInArray];
}

int game_get_next_square(cgame g, uint i, uint j, direction dir, uint dist){ //gab

    if(dir == UP)
        i-=(6*dist);
    
    if(dir == DOWN)
        i+=(6*dist);
    
    if(dir == LEFT)
        j-=dist;
    
    if(dir == RIGHT)
        j+=dist;

    return game_get_square(g,i,j);
}

int game_get_next_number(cgame g, uint i, uint j, direction dir, uint dist){ //gab

    if(dir == UP)
        i-=(6*dist);
    
    if(dir == DOWN)
        i+=(6*dist);
    
    if(dir == LEFT)
        j-=dist;
    
    if(dir == RIGHT)
        j+=dist;

    return game_get_number(g,i,j);
}

bool game_is_empty(cgame g, uint i, uint j){ //gab

    int findInArray = (i*6)+j;
    

    return g->ArrayOfSquare[findInArray]==S_EMPTY;
}

bool game_is_immutable(cgame g, uint i, uint j){ //gab

    int findInArray = (i*6)+j;


    if(g->ArrayOfSquare[findInArray] ==S_IMMUTABLE_ONE)
        return true;
    
    else if (g->ArrayOfSquare[findInArray] ==S_IMMUTABLE_ONE)
        return true;
    
    else
        return false;

    
}

int game_has_error(cgame g, uint i, uint j){ //gab

    return 1;
}

bool game_check_move(cgame g, uint i, uint j, square s){ //gab

    return true;
}

void game_play_move(game g, uint i, uint j, square s){ //ilisa

    //return nothing
}

bool game_is_over(cgame g){ //ilisa

    return true;
}

void game_restart(game g){ //ilisa

    //return nothing
}