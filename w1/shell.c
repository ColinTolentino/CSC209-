#include <stdio.h>
#include <stdlib.h>

int helper(int **fds){
	printf("%d\n",(*fds)[0]);
	return 0;
}

int main(int argc, char** argv){
	int fd[2] = {1,2};
	int *fds = fd;
	printf("%d\n", fd[0]);
	helper(&fds);
	return 0;
}