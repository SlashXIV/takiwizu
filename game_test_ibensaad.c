#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "game_aux.h"
#include "game.h"


void usage(char * command){
    fprintf(stderr,"%s\n",command);
    exit(EXIT_FAILURE);
}

bool test_dummy(){
    return true;
}

bool test_default_solution(){

    game g = game_default_solution();
    //If the party is won, then game_default_solution work and it return true!
   
    return game_is_over(g);
    
}

bool test_game_default(){
    game g = game_default();

    //On test si certains élément ne sont pas dans la grille du jeu:

    if(game_get_square(g,0,1)!=S_IMMUTABLE_ONE){
        return false;
    }
    if(game_get_square(g,0,2)!=S_IMMUTABLE_ZERO){
        return false;
    }
    if(game_get_square(g,1,0)!=S_EMPTY){
        return false;
    }
    return true;
}

int main(int argc, char *argv[]){


    if(argc == 1 || argc > 2){
        usage(argv[0]);
    }
    fprintf(stderr, "->We Start test \"%s\"\n", argv[1]);
    bool okey = false;

//Test_dummy:

    if(!strcmp(argv[1],"dummy")){

        okey = test_dummy();

    }

//Test game_default_solution:

    else if(!strcmp(argv[1],"default_solution")){
        okey = test_default_solution();

    }

//Test game_default:

    else if(!strcmp(argv[1],"default")){
        okey = test_game_default();
    }

    else {

        fprintf(stderr,"-> Error : the name : %s is not found!\n",argv[1]);
        exit(EXIT_FAILURE);

    }



    if(okey == true){
        fprintf(stderr,"Success, test : %s is okey!\n",argv[1]);
        return EXIT_SUCCESS;
    }

    if(okey == false){
        fprintf(stderr,"Fail!, test %s have not succeeded!\n",argv[1]);
        return EXIT_FAILURE;
    }


    /* game game_default_solution(void);
       game game_default(void);
        void game_print(cgame g);
        void game_restart(game g);
        void game_play_move(game g, uint i, uint j, square s);
        bool game_is_over(cgame g);
    */

}