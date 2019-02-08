#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256
#define MAX_PASSWORD 10

#define SUCCESS "Password verified\n"
#define INVALID "Invalid password\n"
#define NO_USER "No such user\n"

int main(void) {
  char user_id[MAXLINE];
  char password[MAXLINE];
  int status;
  int fd[2];
  printf("Enter a user ID: ");
  if(fgets(user_id, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
  printf("Enter password: ");
  if(fgets(password, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }

  //Create the pipe
  if(pipe(fd) == -1) {
    perror("pipe");
  }
  //Creates new process
  int r = fork();
  //Error check if fork is successful
  if(r == -1){
    perror("fork");
    exit(1);
  }
  //When the process is a child
  if(r == 0){
    //Error check if dup2 ran succesfully
    if(dup2(fd[0],fileno(stdin)) == -1){
      perror("dup2");
      close(fd[1]);
      exit(1);
    }
    close(fd[0]);
    execl("./validate","validate",(char *) 0);
  }

  //In the case of the parent
  //close
  close(fd[0]);
  write(fd[1],user_id,10);
  write(fd[1],password,10);
  close(fd[1]);

  if(wait(&status) < 0){
    perror("wait");
    exit(1);
  }
  //if the user id and password match
  if(WEXITSTATUS(status) == 0){
    printf(SUCCESS);
  }
  //if there is an error
  if(WEXITSTATUS(status) == 1){
    printf("Error in validate.c\n");
    exit(1);
  }
  //if the user id is found but the password does not match
  if(WEXITSTATUS(status) == 2){
    printf(INVALID);
  }
  //if the user id is not found in the password file.
  if(WEXITSTATUS(status) == 3){
    printf(NO_USER);
  }
  return 0;
}
