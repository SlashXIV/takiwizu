#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"game.h"
#include"game_aux.h"



void usage_gifrim(int argc) {
    fprintf(stderr,"maybe not enough arguments or maybe more ! you have %d args where 2 are expected\n",argc );
    exit(EXIT_FAILURE);
}

bool test_dummy() {
    return true;
}

bool test_game_check_move(){

    game g = game_default(); 

    if(!game_check_move(g,0,4,S_ZERO))
        return false;
    

    if(!game_check_move(g,5,0,S_ZERO))
        return false;

    return true;
}

bool test_game_has_error(){

    game g = game_default();

    int first_test = game_has_error(g,0,0);
    int second_test = game_has_error(g,1,1);

    if(first_test=!0) //we test if this move don't have error
        return false;

    if(second_test==0) //we test if this one have an error
        return false;

    return true;

}


bool test_game_is_immutable(){

    // immutable coords = (0,1), (0,2), (2,1), (2,4), (3,1), (3,2), (4,2), (4,5), (5,5)
    // we have to try if we can't play on this coords

    game g = game_default;
    if(!game_is_immutable(g,0,1))
        return false;

    else if(!game_is_immutable(g,0,2))
        return false;
    
    else if(!game_is_immutable(g,2,1))
        return false;
    
    else if(!game_is_immutable(g,2,4))
        return false;
    
    else if(!game_is_immutable(g,3,1))
        return false;

    else if(!game_is_immutable(g,3,2))
        return false;

    else if(!game_is_immutable(g,4,2))
        return false;
    
    else if(!game_is_immutable(g,4,5))
        return false;
    
    else if(!game_is_immutable(g,5,5))
        return false;
    
    else
        return true;
}


bool test_game_is_empty(){

    game g = game_default();

    // some empty squares (in the default game) that I will check (0,0), (1,1), (1,2)
    // some occuped squares (in the default game) that I will check (0,1), (2,1), (5,5)  

    if(!game_is_empty(g,0,0))
        return false;
    
    else if(!game_is_empty(g,1,1))
        return false;

    else if(!game_is_empty(g,1,2))
        return false;
    
    else if(game_is_empty(g,0,1))
        return false;
    
    else if(game_is_empty(g,2,1))
        return false;
    
    else if(game_is_empty(g,5,5))
        return false;
    
    else
        return true;
}



int main(int argc, char *argv[]){

    bool ok = test_dummy();

    if (argc == 1 || argc > 2){
        usage_gifrim(argv[0]);
    }

    
    if(!strcmp(argv[1],"dummy")){

        ok = test_dummy();

    }

    else if(!strcmp(argv[1],"game_check_move")){
        ok = test_game_check_move;

    }

    else if(!strcmp(argv[1],"game_has_error")){
        ok = test_game_has_error;

    }

    else if(!strcmp(argv[1],"game_is_immutable")){
        ok = test_game_is_immutable;

    }

    else if(!strcmp(argv[1],"game_is_empty")){
        ok = test_game_is_empty;

    }


    // TEST RESULTS
    if (ok){
        fprintf(stderr, "=> Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "=> Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }


}