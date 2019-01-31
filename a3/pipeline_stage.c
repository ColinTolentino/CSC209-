#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "eratosthenes.h"

pid_t make_stage(int m, int read_fd, int **fds){
	int *fd = *fds;
	if(pipe(fd) == -1){
		exit(-1);
	}
	//Creates new process
	int r = fork();
	//Error check if fork is successful
	if(r == -1){
		exit(-1);
	}

	if(r == 0){
		close(fd[1]);
		return 0;
	}

	close(fd[0]);
	filter(m,read_fd,fd[1]);
	close(fd[1]);
	close(read_fd);
	return getpid();
}