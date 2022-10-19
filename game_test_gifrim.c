#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>


void usage_gifrim(int argc) {
    fprintf(stderr,"maybe not enough arguments or maybe more ! you have %d args where 2 are expected\n",argc );
    exit(EXIT_FAILURE);
}

bool test_dummy() {
    return true;
}


int main(int argc, char *argv[]){

    bool ok = test_dummy();

    if(argc < 2 || argc > 2)
        usage_gifrim(argc);
    
    if(strcmp("dummy",argv[1])){
        ok=false;
    }


    // TEST RESULTS
    if (ok){
        fprintf(stderr, "=> Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "=> Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }


}