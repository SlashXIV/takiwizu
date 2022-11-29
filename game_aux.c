#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"

void game_print(cgame g){ //ilisa

    int a;
    int b =0;
    int c = 0;
    // 0 = " "   1 = "w"   2="b"   3="W"   4="B"
    printf("   012345\n");
    printf("   ------\n");

    while (b!=5){

        printf("%d |",b);

            switch(a){
        case S_EMPTY:
            printf(" ");

        case S_ZERO:
            printf("w");

        case S_ONE:
            printf("b");

        case S_IMMUTABLE_ZERO:
            printf("W");

        case S_IMMUTABLE_ONE:
            printf("B");
        }

        while(c!=5){
            a = game_get_square(g,b,c);
            c++;
        }
        c=0;
        b++;
    }

    printf("   ------\n");



}

game game_default(void){ //ilisa


    square *tab = malloc((DEFAULT_SIZE*DEFAULT_SIZE)*sizeof(square));

    if(tab==NULL)
        exit(EXIT_FAILURE);

    tab[0] = S_EMPTY;
    tab[1] = S_IMMUTABLE_ONE;
    tab[2] = S_IMMUTABLE_ZERO;
    tab[3] =S_EMPTY;
    tab[4] =S_EMPTY;
    tab[5] =S_EMPTY;
    tab[6] =S_EMPTY;
    tab[7] =S_EMPTY;
    tab[8] =S_EMPTY;
    tab[9] =S_EMPTY;
    tab[10] =S_EMPTY;
    tab[11] =S_EMPTY;
    tab[12] =S_EMPTY;
    tab[13] =S_IMMUTABLE_ZERO;
    tab[14] =S_EMPTY;
    tab[15] =S_EMPTY;
    tab[16] =S_IMMUTABLE_ZERO;
    tab[17] =S_EMPTY;
    tab[18] =S_EMPTY;
    tab[19] =S_IMMUTABLE_ZERO;
    tab[20] =S_IMMUTABLE_ONE;
    tab[21] =S_EMPTY;
    tab[22] =S_EMPTY;
    tab[23] =S_EMPTY;
    tab[24] =S_EMPTY;
    tab[25] =S_EMPTY;
    tab[26] =S_IMMUTABLE_ONE;
    tab[27] =S_EMPTY;
    tab[28] =S_EMPTY;
    tab[29] =S_IMMUTABLE_ZERO;
    tab[30] =S_EMPTY;
    tab[31] =S_EMPTY;
    tab[32] =S_EMPTY;
    tab[33] =S_EMPTY;
    tab[34] =S_EMPTY;
    tab[35] =S_IMMUTABLE_ZERO;

    return game_new(tab);
}

game game_default_solution(void){ //ilisa

    square *tab = malloc((DEFAULT_SIZE*DEFAULT_SIZE)*sizeof(square));

    if(tab==NULL)
        exit(EXIT_FAILURE);

    tab[0] = S_ZERO;
    tab[1] = S_IMMUTABLE_ONE;
    tab[2] = S_IMMUTABLE_ZERO;
    tab[3] = S_ONE;
    tab[4] = S_ZERO;
    tab[5] = S_ONE;

    tab[6] = S_ZERO;
    tab[7] = S_ONE;
    tab[8] = S_ONE;
    tab[9] = S_ZERO;
    tab[10] = S_ONE;
    tab[11] = S_ZERO;

    tab[12] = S_ONE;
    tab[13] = S_IMMUTABLE_ZERO;
    tab[14] = S_ZERO;
    tab[15] = S_ONE;
    tab[16] =S_IMMUTABLE_ZERO;
    tab[17] = S_ONE;

    tab[18] = S_ONE;
    tab[19] = S_IMMUTABLE_ZERO;
    tab[20] = S_IMMUTABLE_ONE;
    tab[21] = S_ZERO;
    tab[22] = S_ZERO;
    tab[23] = S_ONE;

    tab[24] = S_ZERO;
    tab[25] = S_ONE;
    tab[26] =S_IMMUTABLE_ONE;
    tab[27] = S_ZERO;
    tab[28] = S_ONE;
    tab[29] = S_IMMUTABLE_ZERO;

    tab[30] = S_ONE;
    tab[31] = S_ZERO;
    tab[32] = S_ZERO;
    tab[33] = S_ONE;
    tab[34] = S_ONE;
    tab[35] =S_IMMUTABLE_ZERO;

    return game_new(tab);
}