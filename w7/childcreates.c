#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv) {

	int i;
	int n;
	int num_kids;
	int status;
	pid_t pid;

	if (argc != 2) {
		fprintf(stderr, "Usage: childcreates <numkids>\n");
		exit(1);
	}

	num_kids = strtol(argv[1], NULL, 10);

	for (i = 0; i < num_kids; i++) {
		n = fork();
		//Check for child if there is then we print its info and go to next iteration.
		if(n == 0){
 			printf("pid = %d, ppid = %d, i = %d\n", getpid(), getppid(), i);
 			//Goes to next iteration so that the loop increments on each call of fork.
 			continue;
		}
		if (n < 0) {
			perror("fork");
			exit(1);
		}
		//makes the parent slow down to wait until the child processes finish.
        if( (pid = wait(&status)) == -1) { //Checking if wait works. And makes sure the parent doesnt execute before the children executes.
            perror("wait");
        }
        // This exit(0) is what is important, it will stop the loop from making more children.
 		exit(0);
	}

	return 0;
}
