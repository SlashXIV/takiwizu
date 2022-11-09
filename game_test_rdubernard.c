#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#include"game.h"
#include"game_aux.h"

#define ASSERT(expr)                                                                  \
  do                                                                                  \
  {                                                                                   \
    if ((expr) == 0)                                                                  \
    {                                                                                 \
      fprintf(stderr, "[%s:%d] Assertion '%s' failed!\n", __FILE__, __LINE__, #expr); \
      abort();                                                                        \
    }                                                                                 \
  } while (0)


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
bool test_game_get_square(void){

    // game init simulation
    game g = game_default();
    game_play_move(g, 0, 0, S_ONE);
    game_play_move(g, 0, 5, S_ZERO);

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

    // verifying that the values match 
    bool values_matches = (
        cpt_white == total_white &&
        cpt_empty == total_empty &&
        cpt_black == total_black &&
        cpt_immutable_black == total_immutable_black &&
        cpt_immutable_white == total_immutable_white
    );

    game_delete(g);

    // découle de l'utilisation itérée de game_get_square()
    return values_matches;
}

// ISSUE #6 -> game_set_square()
bool test_game_set_square(void){
    // GENERATE EMPTY GAME
    game g = game_new_empty();
    ASSERT(g);

    // FILLING 'b' then 'w' MID
    for (int x = 0; x < DEFAULT_SIZE; x++){
        for (int y = 0; y < DEFAULT_SIZE/2; y++){
            game_set_square(g, x, y, S_ONE);
        }
        for (int y = DEFAULT_SIZE/2; y < DEFAULT_SIZE; y++){
            game_set_square(g, x, y, S_ZERO);
        }
    }
    
    game_print(g);
    
    // VERIF
    for (int x = 0; x < DEFAULT_SIZE; x++){
        for (int y = 0; y < DEFAULT_SIZE/2; y++){
            printf("%d",game_get_square(g, x, y) == S_ONE);
            if (game_get_square(g, x, y) != S_ONE){
                game_delete(g);
                return false;
            }
        }
        for (int y = DEFAULT_SIZE/2; y < DEFAULT_SIZE; y++){
            printf("%d",game_get_square(g, x, y) == S_ZERO);
            if (game_get_square(g, x, y) != S_ZERO){
                game_delete(g);
                return false;
            }
        }
        printf("\n");
    }
    
    // CLEAR
    for (int x = 0; x < DEFAULT_SIZE; x++){
        for (int y = 0; y < DEFAULT_SIZE; y++){
            game_set_square(g, x, y, S_EMPTY);
        }
    }

    game_print(g);
    
    // FILLING EACH SQUARE AT RANDOM POS 
    game_set_square(g, 2, 4, S_IMMUTABLE_ONE);
    game_set_square(g, 0, 1, S_IMMUTABLE_ZERO);
    game_set_square(g, 0, 2, S_ONE);
    game_set_square(g, 0, 3, S_ZERO);
    game_set_square(g, 0, 4, S_EMPTY);
    game_set_square(g, 3, 2, S_ONE);
    game_set_square(g, 5, 1, S_ZERO);
    game_set_square(g, 3, 4, S_IMMUTABLE_ONE);
    game_set_square(g, 0, 3, S_EMPTY);

    game_print(g);

    // AND CHECKING IF IT WORKED
    bool values_matches = 
        game_get_square(g, 3, 2) == S_ONE &&
        game_get_square(g, 5, 1) == S_ZERO &&
        game_get_square(g, 3, 4) == S_IMMUTABLE_ONE &&
        game_get_next_square(g, 2, 3, RIGHT, 1) == S_IMMUTABLE_ONE &&
        game_get_square(g, 0, 1) == S_IMMUTABLE_ZERO &&
        game_get_square(g, 0, 2) == S_ONE &&
        game_get_square(g, 0, 3) == S_EMPTY &&
        game_get_square(g, 0, 4) == S_EMPTY;


    game_set_square(g, 4, 2, S_ONE);
    if (game_get_square(g, 4, 2) != S_ONE){
        game_delete(g);
        return false;
    }

    game_set_square(g, 4, 2, S_EMPTY);
    if (game_get_square(g, 4, 2) != S_EMPTY){
        game_delete(g);
        return false;
    }

    game_set_square(g, 1, 5, S_ZERO);
    if (game_get_square(g, 1, 5) != S_ZERO){
        game_delete(g);
        return false;
    }

    game_set_square(g, 2, 1, S_IMMUTABLE_ONE);
    if (game_get_square(g, 2, 4) != S_IMMUTABLE_ONE){
        game_delete(g);
        return false;
    }   

    game_set_square(g, 3, 4, S_IMMUTABLE_ZERO);
    if (game_get_square(g, 3, 4) != S_IMMUTABLE_ZERO){
        game_delete(g);
        return false;
    }

    game_delete(g);

    if (!values_matches){
        return false;
    }

    return true;
}

// ISSUE #5 -> game_delete()
bool test_game_delete(){
    game g = game_new_empty();
    game_delete(g);
    return true;
}        

// ISSUE #4 -> game_equal()
bool test_game_equal(){
    game g_empty = game_new_empty();
    game g_default = game_default();
    game g_end = game_default_solution();


    game g1 = game_new_empty();
    game g2= game_new_empty();

    game g_zero = game_new_empty();
    game g_immutable_zero = game_new_empty();
    game g_one = game_new_empty();
    game g_immutable_one = game_new_empty();

    /*if (!game_equal(g_end, g_end)){
        return false;
    }

    if (game_equal(g_empty, g_default)){
        return false;
    }

    if (!game_equal(g_empty, g_empty)){
        return false;
    }

    if (game_equal(g_end, g_default)){
        return false;
    }*/

    game_set_square(g_zero,3,3,S_ZERO);
    game_set_square(g_immutable_zero,3,3,S_IMMUTABLE_ZERO);
    game_set_square(g_one,2,3,S_ONE);
    game_set_square(g_immutable_one,2,3,S_IMMUTABLE_ONE);

    if(game_equal(g_zero,g_immutable_zero)){
        return false;
    }
    if(game_equal(g_one,g_immutable_one)){
        return false;
    }

    if(!game_equal(g1,g2)){
        return false;
    }

    game_set_square(g1,2,2,S_ONE);

    if(game_equal(g1,g2)){
        return false;
    }
    game_set_square(g2,2,2,S_ONE);
    if(!game_equal(g1,g2)){
        return false;
    }



    game_delete(g_empty);
    game_delete(g_default);
    game_delete(g_end);
    game_delete(g_zero);
    game_delete(g_immutable_zero);
    game_delete(g_one);
    game_delete(g_immutable_one);
    game_delete(g1);
    game_delete(g2);

    return true;
}

// ISSUE 3 -> game_copy()
bool test_game_copy(){
    game g_default_primal = game_default();
    game g_default_clone = game_copy(g_default_primal);

    if(!game_equal(g_default_clone, g_default_primal)){
        return false;
    } 
    
    game_delete(g_default_clone);
    game_delete(g_default_primal);

    return true;
}

// ISSUE 2 -> game_new_empty()
bool test_game_new_empty(){
    game g = game_new_empty();

    for (int x = 0; x < DEFAULT_SIZE; x++){
        for (int y = 0; y < DEFAULT_SIZE ; y++){
            if (game_get_square(g, x, y) != S_EMPTY){
                return false;
            }
        }
    }
    return true;
    game_delete(g);
}
 
// ISSUE 1 -> game_new()
bool test_game_new(){
    uint nb_cases = DEFAULT_SIZE * DEFAULT_SIZE;
    square * square_array = calloc(nb_cases, sizeof(square));
    game g = game_new(square_array);

    game_print(g);
    game_delete(g);
    return true;
}  


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
    else if (!strcmp("game_get_square", argv[1])){
        ok = test_game_get_square();
    }
    
    // -> set_square
    else if (!strcmp("game_set_square", argv[1])){
        ok = test_game_set_square();
    }

    // -> game_delete
    else if (!strcmp("game_delete", argv[1])){
        ok = test_game_delete();
    }

    // -> game_equal
    else if (!strcmp("game_equal", argv[1])){
        ok = test_game_equal();
    }

    // -> game_copy
    else if (!strcmp("game_copy", argv[1])){
        ok = test_game_copy();
    }

    // -> game_new
    else if (!strcmp("game_new", argv[1])){
        ok = test_game_new();
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
