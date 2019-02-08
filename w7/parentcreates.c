#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char **argv) {
	int i;
	int n;
	int num_kids;
	int status;
	pid_t pid;

	if (argc != 2) {
		fprintf(stderr, "Usage: parentcreates <numkids>\n");
		exit(1);
	}

	num_kids = strtol(argv[1], NULL, 10);

	for (i = 0; i < num_kids; i++) {
		n = fork();
		//Checks if the process is a child then prints and exits the process before it can make a new child.
		if(n == 0){
 			printf("pid = %d, ppid = %d, i = %d\n", getpid(), getppid(), i);
 			//Exit so that we do not make anymore children from the child processes.
 			exit(0);
		}
		if (n < 0) {
			perror("fork");
			exit(1);
		}
        if( (pid = wait(&status)) == -1) { //Checking if wait works. And makes sure the parent doesnt execute before the children executes.
            perror("wait");
        }
	}

	return 0;
}
