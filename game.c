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
    if(g->ArrayOfSquare!=NULL)
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

    if(i<0 || i> DEFAULT_SIZE || j<0 || j>DEFAULT_SIZE || g ==NULL){

        fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
        exit(EXIT_FAILURE);
    }

    int findInArray = (i*6)+j;

    return g->ArrayOfSquare[findInArray];
}

int game_get_next_square(cgame g, uint i, uint j, direction dir, uint dist){ //gab


    if(i<0 || i> DEFAULT_SIZE || j<0 || j>DEFAULT_SIZE || g ==NULL){

        fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
        exit(EXIT_FAILURE);
    }

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

    if(i<0 || i> DEFAULT_SIZE || j<0 || j>DEFAULT_SIZE || g ==NULL){

        fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
        exit(EXIT_FAILURE);
    }

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

    if(i<0 || i> DEFAULT_SIZE || j<0 || j>DEFAULT_SIZE || g ==NULL){

        fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
        exit(EXIT_FAILURE);
    }

    int findInArray = (i*6)+j;
    

    return g->ArrayOfSquare[findInArray]==S_EMPTY;
}

bool game_is_immutable(cgame g, uint i, uint j){ //gab

    if(i<0 || i> DEFAULT_SIZE || j<0 || j>DEFAULT_SIZE || g ==NULL){

        fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
        exit(EXIT_FAILURE);
    }

    int findInArray = (i*6)+j;


    if(g->ArrayOfSquare[findInArray] ==S_IMMUTABLE_ONE)
        return true;
    
    else if (g->ArrayOfSquare[findInArray] ==S_IMMUTABLE_ONE)
        return true;
    
    else if(i<0 || i> DEFAULT_SIZE || j<0 || j>DEFAULT_SIZE)
        return false;
    
    else
        return false;

    
}

int game_has_error(cgame g, uint i, uint j){ //gab

    if(i<0 || i> DEFAULT_SIZE || j<0 || j>DEFAULT_SIZE || g ==NULL){

        fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
        exit(EXIT_FAILURE);
    }

    int whiteCol = 0;
    int whiteLine = 0;

    for(int h=0;h<DEFAULT_SIZE;h++){

        if(game_get_square(g,h,j)==S_ZERO ||game_get_square(g,h,j)== S_IMMUTABLE_ZERO )
            whiteLine++;
    }


    for(int w=0;w<DEFAULT_SIZE;w++){
        if(game_get_square(g,i,w)==S_ZERO ||game_get_square(g,i,w)== S_IMMUTABLE_ZERO )
            whiteCol++;
    }



    if (whiteLine>3 || whiteCol >3) //we check the parity of the squares if >3 mean that parity isnt respected
        return 1;     

    // LA FONCTION DOIT SEULEMENT RENVOYER UNE ERREUR ET NE RIEN PRINT DU TOUT, LE PRINT CE FAIT DANS LE GAME_TEXT
    // NE PAS AVOIR 3 CONSECUTIFS CAD trois a la suite => WWW BBB, W BBB

   
   
    square primaryCase = game_get_square(g,i,j);
    
    if(i*6+j>=34 || i*6+j>=1){
        if(game_get_square(g,i,j+1)==primaryCase && game_get_square(g,i,j-1)==primaryCase) //checking the width 
            return 1;
        
    }

    if((i*6)+6+j<=35 || (i*6)-6+j>=0){
        if(game_get_square(g,i+1,j)==primaryCase && game_get_square(g,i-1,j)==primaryCase) //checking the height 
            return 1;
    }

    if(i==5){
        if(game_get_square(g,i-1,j)==primaryCase && game_get_square(g,i-2,j)==primaryCase) //checking the height in case of corner
            return 1;
    }

    if(i==0){
        if(game_get_square(g,i+1,j)==primaryCase && game_get_square(g,i+2,j)==primaryCase) //checking the height in case of corner
            return 1;
    }

    if(j==5){
        if(game_get_square(g,i,j-1)==primaryCase && game_get_square(g,i,j-2)==primaryCase) //checking the height in case of corner
            return 1;
    }

    if(j==0){
        if(game_get_square(g,i,j+1)==primaryCase && game_get_square(g,i,j+2)==primaryCase) //checking the height in case of corner
            return 1;
    }

    


    return 0; //0 mean that there is no error
}

bool game_check_move(cgame g, uint i, uint j, square s){ //gab

    if(i<0 || i> DEFAULT_SIZE || j<0 || j>DEFAULT_SIZE || g ==NULL){

        fprintf(stderr, "g is null, or  wrong coordinates given :/\n");
        exit(EXIT_FAILURE);
    }


    square cas  = game_get_square(g,i,j);


    if(i<0 || i> DEFAULT_SIZE || j<0 || j>DEFAULT_SIZE)
        return false;
    
    if(cas == S_IMMUTABLE_ONE || cas == S_IMMUTABLE_ZERO)
        return false;
    
    if( cas != S_EMPTY || cas != S_ONE || cas != S_ZERO)
        return false;


    return true; 
}

void game_play_move(game g, uint i, uint j, square s){ //ilisa

    //return nothing
}

bool game_is_over(cgame g){ //ilisa

//We test if our pointer g is NULL, if it is the case we exit the programm.

    if(g==NULL){
        exit(EXIT_FAILURE);
    }
    /* rules:
    --> to win the game we need to have the same number of white and black square in each lines and columns 
    --> and not more than 3 black or white consecutives square for each lines and columns:
    

    uint nb_white_lines=0;
    uint nb_black_lines=0;
    uint nb_black_columns=0;
    uint nb_white_columns=0;

//We have created counters and we use them in a loop to count how many black and white square we have.

    //Lines:

    uint num_line=0;
    
    while(num_line <DEFAULT_SIZE){
        for(uint num_column=0; num_column<DEFAULT_SIZE; num_column++){

            square actual_square = game_get_square(g,num_column,num_line);

            if(actual_square==S_ONE || actual_square==S_IMMUTABLE_ONE){
                nb_black_lines++;
            }
            else if(actual_square==S_ZERO || actual_square==S_IMMUTABLE_ZERO){
                nb_white_lines++;
            }
        }
        if(nb_black_lines != nb_white_lines){ //Check if at least on line does not respect the rule of same number of white and black square.
            return false;
        }
        num_line++;
        nb_black_lines = 0;
        nb_white_lines = 0;
    }


    //Columns:   

    uint num_column=0;
    while(num_column <DEFAULT_SIZE){
        for(uint num_line; num_line<DEFAULT_SIZE; ,um_line++){
            square actual_square = game_get_square(g,num_column,num_line);

            if(actual_square==S_ONE || actual_square==S_IMMUTABLE_ONE){
                nb_black_columns++;
            }
            else if(actual_square==S_ZERO || actual_square==S_IMMUTABLE_ZERO){
                nb_white_columns++;
            }
        }
        if(nb_black_columns != nb_white_columns){ //Check if at least on line does not respect the rule of same number of white and black square.
            return false;
        }
        num_column++;
        nb_black_columns = 0;
        nb_white_columns = 0;
    }
    */

    for(int i=0; i<DEFAULT_SIZE; i++){
        for(int j=0; j<DEFAULT_SIZE; j++){
            if(game_has_error(g,i,j)){
                return false;
            }
        }
    }

    //In case we haven't returned false, all the rules are satisied and we return true ---> the game is won.
    return true;
}

void game_restart(game g){ //ilisa

    g = game_default();
}

//S_ONE = BLACK, Z_ZERO = WHITE.