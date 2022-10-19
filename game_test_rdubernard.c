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

    // game init simulation
    game g = game_default();
    game_play_move(g, 0, 0, S_ONE);
    game_play_move(g, 0, 5, S_ZERO);

    game_print(g);
    // game_default() each total square on this basic configuration
    unsigned int total_empty = 25;
    unsigned int total_immutable_white = 6;
    unsigned int total_immutable_black = 3;
    unsigned int total_white = 1;
    unsigned int total_black = 1;

    // cpt of each square type
    unsigned int cpt_empty = 0;
    unsigned int cpt_immutable_white = 0;
    unsigned int cpt_immutable_black = 0;
    unsigned int cpt_black = 0;
    unsigned int cpt_white = 0;

    
    // counting each square type on default config
    for (int x = 0; x < DEFAULT_SIZE ; x++){
        for (int y = 0; y < DEFAULT_SIZE; y++){
            square actual_square = game_get_square(g, x, y);
            switch (actual_square){
            
            // actual square type => empty
            case S_EMPTY:
                cpt_empty++;
                break;

            // actual square type => immutable black
            case S_IMMUTABLE_ONE:
                cpt_immutable_black++;
                break;

            // actual square type => immutable white
            case S_IMMUTABLE_ZERO:
                cpt_immutable_white++;
                break;
            
            // actual square type => black
            case S_ONE:
                // printf("there is a black square at (%d, %d)\n",x, y);
                cpt_black = cpt_black + 1;
                break;
            
            // actual square type => white
            case S_ZERO:
                cpt_white++;
                break;

            default: 
                // printf("zero ?\n");
                break;
            }
        }    
    }
    // printf("counted :\n");

    // printf("\tI white =%d\n", cpt_immutable_white);
    // printf("\tI black =%d\n", cpt_immutable_black);
  
    // printf("\n");

    // printf("\twhite = %d\n", cpt_white);
    // printf("\tblack = %d\n", cpt_black);
    // printf("\tempty = %d\n", cpt_empty);

    // verifying that the values match 
    bool values_matches = (
        cpt_white == total_white &&
        cpt_empty == total_empty &&
        cpt_black == total_black &&
        cpt_immutable_black == total_immutable_black &&
        cpt_immutable_white == total_immutable_white
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
