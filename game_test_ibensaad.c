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

//bool test_game_print la fonction renvoie juste true et ne test rien askip (à vérifier sur la fiche du TD5) 
//Attention, certaines fonctions sont faciles à tester et d'autres sont plus difficiles, et encore pour
// certaines fonctions, le test n'est pas vraiment réalisable sans outils externes, comme par exemple game_print()
// ou game_delete(). Dans ce cas, on se contentera juste d'appeler la fonction pour vérifier qu'elle ne provoque pas 
//d'erreur grave (comme segmentation fault).

bool test_game_restart(){
    game g1 = game_default();
    game g2 = game_default();
    //We play a move in the game g1:
    game_play_move(g1,2,0,S_ONE);

    //We restart the game g1:

    game_restart(g1);

    //We test if the restarted game g1 is like the game g2 in it's initial state:
    if(!game_equal(g1,g2)){
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

    else if(!strcmp(argv[1],"restart")){
        okey = test_game_restart();
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


    /* game game_default_solution(void); ok
       game game_default(void); ok
        void game_print(cgame g);
        void game_restart(game g); ok
        void game_play_move(game g, uint i, uint j, square s);
        bool game_is_over(cgame g);
    */

}