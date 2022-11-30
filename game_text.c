#include<stdio.h>
#include<stdlib.h>
#include"game.h"
#include"game_aux.h"


void play_on_square(game g, int i, int j, int move){
    char pion;
    switch (move){
        case S_ZERO:
            pion = 'w';
            break;
    
        case S_ONE:
            pion = 'b';
            break;

        case S_EMPTY:
            pion = 'e';
            break;
    }

     printf("\naction : play move '%c' into square (%d, %d)\n", pion, i, j);

    if (game_check_move(g, i, j, move)){
                game_play_move(g, i, j, move);
            } else {
                printf("sorry, wrong move :(\n");
            }
    
}
 
void display_help(){
	printf("\naction : help\n");
	printf("- press 'w <i> <j>' to put a zero/white at square (i,j)\n");
	printf("- press 'b' <i> <j>' to put a one/black at square (i,j)\n");
	printf("- press 'e' <i> <j>' to empty square (i,j)\n");
	printf("- press 'r' to restart\n");
	printf("- press 'q' to quit\n");
}

int main(void){

	// GAME START
	game g = game_default();
	while(!game_is_over(g)){
    
        // GAME PRINT
        printf("\n");
        game_print(g);

        game_set_square(g,2,2,S_ZERO);
        game_set_square(g,2,3,S_ZERO);
        
        // LIST ERROR ON EACH SQUARE

        for (uint i = 0; i < DEFAULT_SIZE; i++){
            for (uint j = 0; j < DEFAULT_SIZE; j++){
                if (game_has_error(g, i, j)){
                    printf("Error at square (%d, %d)\n", i, j);
                }
            }
        }

        // READ INPUT CHAR
        printf("\n? [h for help]\n");
        char input_char;
        scanf(" %c", &input_char); // space before format specifier implies not consedirating spaces & line breaks 
        
        // INPUT CHAR CONSEQUENCE
        switch (input_char){
        
            // HELP
            case 'h' :
                printf("\naction : help\n");
                display_help();
                break;
                
            // RESTART
            case 'r' :
                printf("\naction : restart\n");
                game_restart(g);
                break;
                
            // QUIT
            case 'q' :
                printf("\naction : quit\nwhat a shame, you gave up ;-;\n");
                game_delete(g);
                return EXIT_SUCCESS;
                break;
            
            // PLAY
            case 'w' :
            case 'b' :
            case 'e' : ;
            
                // ON POSITION
                uint i, j;
                scanf("%d %d", &i, &j);
                switch (input_char){
                
                    // WHITE
                    case 'w' :
                        play_on_square(g, i, j, S_ZERO);
                        break;
                        
                    // BLACK
                    case 'b' :
                        play_on_square(g, i, j, S_ONE);
                        break;
                        
                    // EMPTY
                    case 'e' :
                        play_on_square(g, i, j, S_EMPTY);
                        break;

                break;
                }
        }
    }
        
	// WIN CONDITION (OUT OF WHILE LOOP)
    game_print(g);
	printf("congratulation, gg wp, you did it ;] !\n");
	return EXIT_SUCCESS;
}