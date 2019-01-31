#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>

#include "eratosthenes.h"


int main(int argc, char *argv[]) {
    // Turning off sigpipe
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    // Your solution below ...
    int factor1 = 0;
    int factor2 = 0;
    int m = 2;
    int n;
    int fd[2];
    int status;
    int make;
    int counter = 0;
    char set[] = "1234567890";
    int input_check;

    if(argc != 2){
    	fprintf(stderr, "Usage:\n\tpfact n\n");
    	exit(1);
    }
    input_check = strspn(argv[1],set);

    if(input_check != strlen(argv[1])){
        fprintf(stderr, "Usage:\n\tpfact n\n");
        exit(1);
    }

    //Gets n and makes sure it is a positive integer.
    n = strtol(argv[1],NULL,10);
    if(n < 2){
    	fprintf(stderr, "Usage:\n\tpfact n\n");
    	exit(1);
    }

    if(pipe(fd) == -1){
        exit(2);
    }
    //Fork once
    int r = fork();
    //
    
  	if(r == -1){
    	exit(2);
  	}

  	//Child process
  	if(r == 0){ 
        close(fd[1]); //Close the first fd from original parent to child.
		while(m <= sqrt(n)){
            //Code to check and store possible values of m that could be a factor
            if(n%m == 0){
                if(factor1 == 0){
                    factor1 = m;
                    if((n/factor1)%factor1==0){
                        if(m == 2){
                            close(fd[1]);
                        }
                        close(fd[0]);
                        if(factor1*factor1 == n){
                            factor2 = m;
                            printf("%d %d %d\n", n, factor1, factor2);
                            exit(0);
                        }else{
                            printf("%d is not the product of two primes\n", n);
                            exit(0);  
                        }
                    }      
                }else{//That means we found another factor that is a prime and is less the sqrt(n)
                    printf("%d is not the product of two primes\n", n);
                    exit(0);
                }
            }
            //Just coninoue with the seive
            int fds[2];
            int *fds_ptr = fds;
            counter++;
            make = make_stage(m,fd[0],&fds_ptr);
            if(make == 0){//We are dealing with the child process
                if(read((fds)[0], &m, sizeof(int)) == -1){
                    exit(2);
                }
                fd[0] = fds[0];
                fd[1] = fds[1];
            }else if(make > 0) {
                if(wait(&status) != -1){
                    if(WIFEXITED(status)){
                        exit(1 + WEXITSTATUS(status));
                    }
                }
                //Child exited abnormally.
                exit(2);
            }else{
                exit(2);
            }
        }
        close(fd[1]); //Since what is left is only reading from the pipe
        //Exits while loop means we are at the last pipe.
        if(n%m == 0){
            if(factor1 == 0){
                if(m == n){
                    printf("%d is prime\n", n);
                    close(fd[0]);
                    exit(0); 
                }else{
                    close(fd[0]);
                    printf("%d is not the product of two primes\n", n);
                    exit(0); 
                }
            }else{
                factor2 = m;
                if(factor1*factor2 == n){
                    printf("%d %d %d\n", n, factor1, factor2);
                    exit(0);
                }
            }
        }
        //Go through remaining numbers from pipe.
        while(read(fd[0], &m, sizeof(int)) > 0){
            if(factor1*m == n){
                factor2 = m;
                printf("%d %d %d\n", n, factor1, factor2);
                close(fd[0]);
                exit(0);
            }
        }
        close(fd[0]);


        //Check if we found no factors then
        if(factor1 == 0 && factor2 == 0){
            printf("%d is prime\n", n);
            exit(0);
        }
        
        //If we get to this point then we haven't found a second factor 
        printf("%d is not the product of two primes\n", n);
        exit(0); 
    }//End of child process
    //Parent process
    if(r > 0){
        close(fd[0]);
        for(int i = 2; i <= n; i++){
            if(write(fd[1], &i, sizeof(int)) == -1){
                break;  
            }
        }
        close(fd[1]);
    }
    if(wait(&status) != -1){
        if(WIFEXITED(status)){
            printf("Number of filters = %d\n",WEXITSTATUS(status));
        }
    }
	return 0;
}
