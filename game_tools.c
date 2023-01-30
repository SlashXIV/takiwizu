#include <stdlib.h>
#include <stdio.h>
#include "game_tools.h"
#include "game.h"

game game_load(char * filename);

void game_save(cgame g, char * filename){

    //Creation d'un fichier de sauvegarde

    FILE * fgame = fopen(filename, "w");



    //Sauvegarde de la taille du plateau (nb_rows nb_cols wrapping unique\n)

    fprintf(fgame, "%d %d %d %d\n", g->nb_rows, g->nb_cols, g->wrapping, g->unique);

    //Sauvegarde du plateau de jeu ligne par ligne

    for(int i = 0; i < g->nb_rows; i++){

        for(int j = 0; j < g->nb_cols; j++){

            fprintf(fgame, "%d", g->board[i][j]);

        }

        fprintf(fgame, "\n");

    }



}


