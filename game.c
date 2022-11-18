#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"


struct game_s{

    square  *ArrayOfSquare;

};

//We have to implement the functions now:

game game_new(square* squares){

    return NULL;
}

game game_new_empty(void){

    return NULL;
}

game game_copy(cgame g){

    return NULL;
}

bool game_equal(cgame g1, cgame g2){

    return true;
}

void game_delete(game g){

    //return nothing
}

void game_set_square(game g, uint i, uint j, square s){

    //return nothing
}

square game_get_square(cgame g, uint i, uint j){

    return S_EMPTY;
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