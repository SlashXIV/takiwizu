#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


void usage(char * command){
    fprintf(stderr,"%s\n",command);
    exit(EXIT_FAILURE);
}

bool test_dummy(){
    return true;
}

int main(int argc, char *argv[]){

    if(argc == 1 || argc > 2){
        usage(argv[0]);
    }
    fprintf(stderr, "->We Start test \"%s\"\n", argv[1]);
    bool okey = false;

    if(!strcmp(argv[1],"dummy")){

        okey = test_dummy();

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

}