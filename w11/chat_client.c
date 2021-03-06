#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "socket.h"


#ifndef PORT
  #define PORT 56295
#endif
#define BUF_SIZE 128

int main(void) {

    int sock_fd = connect_to_server(PORT, "127.0.0.1");
    printf("Enter Username:\n");
    char username_prompt[BUF_SIZE+1];
    int username_read = read(STDIN_FILENO,username_prompt, BUF_SIZE);
    if (username_read < 0){
        exit(1);
    }
    username_prompt[username_read] = '\0';
    write(sock_fd, username_prompt, username_read);
    // Read input from the user, send it to the server, and then accept the
    //Get input from stdin

    // echo that returns. Exit when stdin is closed.
    char buf[BUF_SIZE + 1];
    fd_set all_fds, listen_fds;
    FD_ZERO(&all_fds);
    FD_SET(sock_fd, &all_fds);
    FD_SET(STDIN_FILENO, &all_fds);

    while (1) {
        // select updates the fd_set it receives, so we always use a copy and retain the original.
        listen_fds = all_fds;
        int num_read;

        int nready = select(sock_fd + 1, &listen_fds, NULL, NULL, NULL);
        if (nready == -1) {
            perror("server: select");
            exit(1);
        }

        if(FD_ISSET(STDIN_FILENO,&listen_fds)){
            num_read = read(STDIN_FILENO, buf, BUF_SIZE);
            if (num_read == 0) {
                break;
            }
            buf[num_read] = '\0';         // Just because I'm paranoid

            int num_written = write(sock_fd, buf, num_read);
            if (num_written != num_read) {
                perror("client: write");
                close(sock_fd);
                exit(1);
            }
        }

        if(FD_ISSET(sock_fd, &listen_fds)){
            num_read = read(sock_fd, buf, BUF_SIZE);
            buf[num_read] = '\0';
            printf("%s", buf);
        }
    }

    close(sock_fd);
    return 0;
}
