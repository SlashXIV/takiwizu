#include <stdlib.h>
#include <stdio.h>
#include "game_tools.h"
#include "game.h"

#include "game_ext.h"
#include "game_struct.h"

game game_load(char * filename);

void game_save(cgame g, char * filename){

    //Creation d'un fichier de sauvegarde

    FILE * fgame = fopen(filename, "w");



    //Sauvegarde de la taille du plateau (nb_rows nb_cols wrapping unique\n)

    fprintf(fgame, "%d %d %d %d\n", game_nb_rows(g), game_nb_cols(g), g->wrapping, g->unique);

    //Sauvegarde du plateau de jeu ligne par ligne

    for(int i = 0; i < game_nb_rows(g); i++){

        for(int j = 0; j < game_nb_cols(g); j++){

            fprintf(fgame, "%d", g->grid[i][j]);

        }

        fprintf(fgame, "\n");

    }



}


