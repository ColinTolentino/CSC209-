#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include <sys/time.h>


// Message to print in the signal handling function. 
#define MESSAGE "%ld reads were done in %ld seconds.\n"


/* Global variables to store number of read operations and seconds elapsed.
 * These have to be global so that signal handler to be written will have
 * access.
 */
long num_reads = 0, seconds;
int ready = 0;

//From pcrs example video 2.
void handler(int code){

    if(seconds != 0){
        seititimer(ITIMER_REAL,&timer,NULL);
        ready = 1;

    }else{
        exit(0);
    }
    
}


int main(int argc, char ** argv) {

    if (argc != 2) {
        fprintf(stderr, "Usage: time_reads s filename\n");
        exit(1);
    }
    seconds = strtol(argv[1], NULL, 10);


    //Creating a sigaction handler
    struct sigaction newact;
    newact.sa_handler = handler;
    newact.sa_flags = 0;
    sigemptyset(&newact.sa_mask);
    sigaction(SIGALRM, &newact, NULL);

    //Creating the timer
    alarm(1);



    /* In an infinite loop, read an int from a random location in the file
     * and print it to stderr.
     */
    for (;;) {
        if(ready == 1){
            printf("%ld\n", seconds);
            seconds--;
            ready = 0;
            alarm(1);
        }
    }

    return 1;  //something is wrong if we ever get here!
}

