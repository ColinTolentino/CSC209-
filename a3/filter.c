#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "eratosthenes.h"
#include <sys/wait.h>



int filter(int m, int readfd, int writefd){
	int curr;
	while(read(readfd, &curr, sizeof(int)) > 0){
		if(curr % m != 0){
			if(write(writefd, &curr, sizeof(int)) == -1){
				break;
			}
		}
	}
	return 0;
}