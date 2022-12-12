#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_ext.h"


game game_new_ext(uint nb_rows, uint nb_cols, square* squares, bool wrapping, bool unique){
    game e;
    return e;
}


game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping, bool unique){

    game e;
    return e;
}


uint game_nb_rows(cgame g){
    return 1;
}



uint game_nb_cols(cgame g){
    return 1;
}

bool game_is_wrapping(cgame g){
    return true;
}


bool game_is_unique(cgame g){
    return true;
}


void game_undo(game g){


}


void game_redo(game g){

}
