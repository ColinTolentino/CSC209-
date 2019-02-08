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

//From pcrs example video 2.
void handler(int code){
    fprintf(stderr,"Signal %d caught\n", code);
    fprintf(stderr,MESSAGE, num_reads, seconds);
    exit(0);
}


int main(int argc, char ** argv) {
    int random_loc;
    int number_found;
    if (argc != 3) {
        fprintf(stderr, "Usage: time_reads s filename\n");
        exit(1);
    }
    seconds = strtol(argv[1], NULL, 10);

    FILE *fp;
    if ((fp = fopen(argv[2], "r+")) == NULL) {    // Read+Write for later ...
      perror("fopen");
      exit(1);
    }

    //Creating a sigaction handler
    struct sigaction newact;
    newact.sa_handler = handler;
    newact.sa_flags = 0;
    sigemptyset(&newact.sa_mask);
    sigaction(SIGPROF, &newact, NULL);

    //Creating the timer
    struct itimerval timer;
    timer.it_value.tv_sec = seconds;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_PROF, &timer, NULL);


    /* In an infinite loop, read an int from a random location in the file
     * and print it to stderr.
     */
    for (;;) {
      random_loc = sizeof(int)*rand()%100;
      fseek(fp, random_loc, SEEK_SET);
      fread(&number_found, sizeof(int),1,fp);
      num_reads++;
      printf("%d\n", number_found);
    }

    return 1;  //something is wrong if we ever get here!
}

