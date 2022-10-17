#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

void usage(char cmd){
    fprintf(stderr, "Usage %s <testname> [<...>]\n", cmd);
    exit(EXIT_FAILURE);
}

bool test_dummy(){
    return true;
}

int main(int argc, char *argv[]){
    // CMD CONTAIN ONLY EXECUTABLE (0 ARGS)
    if (argc == 1){
        usage(argv[0]);
    }
    
    // TEST START
    fprintf(stderr, "=> Start test : \"%s=\"\n", argv[1]);    
    bool ok = false;

    // STRING COMPARE ARG N°1
    if(strcmp("dummy", argv[1])){
        // VALID : "dummy"
        ok = test_dummy1();
    } else {
        // INVALID : "?"
        fprintf(stderr, "ERROR : test \"%s\" not found !\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // TEST RESULTS
    if (ok){
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_SUCCESS;
    }
}