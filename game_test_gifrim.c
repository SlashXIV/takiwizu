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
    
    if(game_check_move(g,0,1,S_ZERO))
        return false;


    if(!game_check_move(g,5,0,S_ZERO))
        return false;
    


    game_delete(g);
    return true;
}
    
bool test_game_has_error(){

    game g = game_default();

    game_set_square(g, 2, 0, S_ONE);
    game_set_square(g, 2, 2, S_ONE);

    if (game_has_error(g, 4, 2) == 0 || game_has_error(g, 3, 2) == 0 || game_has_error(g, 2, 2) == 0  || game_has_error(g, 5, 1) != 0) 
        return false;
    
    game_delete(g);
    return true;

}


    bool test_game_is_immutable(){

        // immutable coords = (0,1), (0,2), (2,1), (2,4), (3,1), (3,2), (4,2), (4,5), (5,5)
        // we have to try if we can't play on this coords

        game g = game_default();

        if(!game_is_immutable(g,0,1))
            return false;

        if(!game_is_immutable(g,0,2))
            return false;
        
        if(!game_is_immutable(g,2,1))
            return false;
        
        if(!game_is_immutable(g,2,4))
            return false;
        
        if(!game_is_immutable(g,3,1))
            return false;

        if(!game_is_immutable(g,3,2))
            return false;

        if(!game_is_immutable(g,4,2))
            return false;
        
        if(!game_is_immutable(g,4,5))
            return false;
        
        if(!game_is_immutable(g,5,5))
            return false; 
        
        
        game_delete(g);
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
    
    game_set_square(g,5,0,S_ZERO);
    if(game_is_empty(g,5,0))
        return false;
    
    
    game_delete(g);
    return true;

    
}


bool test_game_get_number(){

    //-1 = empty W=0 B=1 
    // (0,0) is an empty square, (0,1) is an immutable square, (5,1) gonna be an occuped square 
    // lets try the function on thoses examples !
    game g = game_default();


    game_play_move(g,5,1,S_ZERO);

    if(game_get_number(g,0,0)!=-1)
        return false;
    
    else if(game_get_number(g,0,1)!=1)
        return false;

    else if(game_get_number(g,5,1)!=0)
       return false; 


    game_delete(g);
    return true;

    

}

bool test_game_get_next_number(){

    game g = game_default();

    

    if(game_get_next_number(g,0,1,DOWN, 2)!=0)
        return false;

    else if (game_get_next_number(g,0,1,RIGHT,2)!=-1)
        return false;
    
    else if(game_get_next_number(g,4,1,DOWN,1)==0)
            return false;

    game_play_move(g,5,1,S_ZERO);

     if(game_get_next_number(g,4,1,DOWN,1)!=0)
            return false;
    
    game_delete(g);
    return true;
    
    
}


bool test_game_get_next_square(){

    game g = game_default();

    if(game_get_next_square(g,0,1,DOWN, 2)!=S_IMMUTABLE_ZERO)
        return false;

    else if (game_get_next_square(g,0,1,RIGHT,2)!=S_EMPTY)
        return false;
    
    else if(game_get_next_square(g,4,1,DOWN,1)!=S_EMPTY)
        return false;

    game_play_move(g,5,1,S_ZERO);

    if(game_get_next_square(g,4,1,DOWN,1)!=S_ZERO)
        return false;

    game_delete(g);
    return true;
}

bool test_game_restart(){

    game g1 = game_new_empty();

    //We place squares in the game g1:

    game_set_square(g1,2,0,S_ONE);
    game_set_square(g1,1,2,S_IMMUTABLE_ONE);
    game_set_square(g1,4,2,S_ZERO);
    game_set_square(g1,2,2,S_IMMUTABLE_ZERO);

    //We restart the game g1:

    game_restart(g1);


    if(game_get_square(g1,2,2)!=S_IMMUTABLE_ZERO)
        return false;
    else if(game_get_square(g1,4,2)!=S_EMPTY)
        return false;
    else if(game_get_square(g1,1,2)!=S_IMMUTABLE_ONE)
        return false;
    else if(game_get_square(g1,2,0)!=S_EMPTY)
        return false;

    
    
    game_delete(g1);
    return true;
}



int main(int argc, char *argv[]){

    bool ok = test_dummy();

    if (argc == 1 || argc > 2){
        usage_gifrim(argc);
    }

    
    if(!strcmp(argv[1],"dummy")){

        ok = test_dummy();

    }

    else if(!strcmp(argv[1],"game_check_move")){
        ok = test_game_check_move();

    }

    else if(!strcmp(argv[1],"game_has_error")){
        ok = test_game_has_error();

    }

    else if(!strcmp(argv[1],"game_is_immutable")){
        ok = test_game_is_immutable();

    }

    else if(!strcmp(argv[1],"game_is_empty")){
        ok = test_game_is_empty();

    }


    else if(!strcmp(argv[1],"game_get_number")){
        ok = test_game_get_number();

    }
    else if(!strcmp(argv[1],"game_get_next_number")){
        ok = test_game_get_next_number();

    }

    else if(!strcmp(argv[1],"game_get_next_square")){
        ok = test_game_get_next_square();

    }
    else if(!strcmp(argv[1],"game_restart")){
        ok = test_game_restart();
    }

    else {
        fprintf(stderr, "=> ERROR : test \"%s\" not found !\n", argv[1]);
        exit(EXIT_FAILURE);
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