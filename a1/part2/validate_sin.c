#include <stdlib.h>
#include <stdio.h>
#include "sin_helpers.c"

int populate_array(int sin, int *sin_array);
int check_sin(int *sin_array);


int main(int argc, char **argv) {
	int sin_array[10];
	int sin_number;
	// TODO: Verify that command line arguments are valid.
	if (argc != 2) {
        return 1;
    }
    // TODO: Parse arguments and then call the two helpers in sin_helpers.c
    // to verify the SIN given as a command line argument.
    sin_number = strtol(argv[1],NULL,10);
    //populates sin_array and checks if integer is 9.
    if(populate_array(sin_number,sin_array) == 0){
        if(check_sin(sin_array)==0){
            printf("Valid SIN\n");
            return 0;
        }
    } 
    printf("Invalid SIN\n");
    return 0;
}
