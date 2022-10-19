#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#include"game.h"
#include"game_aux.h"

/* [====== USAGE ======] */ 
void usage(char * cmd){
    fprintf(stderr, "Usage %s <testname> [<...>]\n", cmd);
    exit(EXIT_FAILURE);
}

/* [====== DUMMY TESTS ======] */ 

// -> DUMMY 
bool test_dummy(void){
    return true;
}

/* [====== GAME UNIT-TESTS ======] */

// ISSUE #7 -> game_get_square()    
bool test_get_square(void){
    
    // game_default() each total square on basic configuration
    unsigned int total_empty = 27;
    unsigned int total_white = 6;
    unsigned int total_black = 3;

    // cpt of each square type
    int cpt_empty, cpt_white, cpt_black = 0;

    // game init simulation
    game g = game_default();
    
    // counting each square type on default config
    for (int x = 0; x < DEFAULT_SIZE ; x++){
        for (int y = 0; y < DEFAULT_SIZE; y++){
            switch (game_get_square(g, x, y)){
            
            // actual square type => empty
            case S_EMPTY:
                cpt_empty++;
                break;

            // actual square type => black
            case S_IMMUTABLE_ONE:
                cpt_black++;
                break;

            // actual square type => white
            case S_IMMUTABLE_ZERO:
                cpt_white++;
                break;
            
            default :
                break;
            }
        }    
    }

    // verifying that the values match 
    bool values_matches = (
        cpt_white == total_white ||
        cpt_empty == total_empty ||
        cpt_black == total_black
    );

    // découle de l'utilisation itérée de game_get_square()
    return values_matches;
}

// -> game_set_square()
// -> game_delete()        
// -> game_equal()         
// -> game_copy()          
// -> game_new_empty()     
// -> game_new()           


/* [====== MAIN ROUTINE ======] */
// MAIN ROUTINE
int main(int argc, char *argv[]){
    // CHECKING FOR 1 ARG ONLY
    if (argc == 1 || argc > 2){
        usage(argv[0]);
    }
    
    // TEST START
    fprintf(stderr, "=> Start test : \"%s\"\n", argv[1]);    
    bool ok = false;

    /* [===== STR COMPARE OF ARG 1 =====] */
    
    // -> dummy
    if(!strcmp("dummy", argv[1])){
        ok = test_dummy();
    }
    
    // -> get_square
    else if (!strcmp("get_square", argv[1])){
        ok = test_get_square();
    }
    
    else {
        // INVALID : "?"
        fprintf(stderr, "=> ERROR : test \"%s\" not found !\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // TEST RESULTS
    if (ok){
        fprintf(stderr, "=> Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "=> Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_SUCCESS;
    }
}
