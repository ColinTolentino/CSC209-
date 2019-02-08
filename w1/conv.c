Last login: Fri Mar 30 17:18:06 on console
Colins-MacBook-Pro:~ colintolentino$ ssh danielm6@ug150.eecg.utoronto.ca
The authenticity of host 'ug150.eecg.utoronto.ca (128.100.13.150)' can't be established.
ECDSA key fingerprint is SHA256:dUNfIrVOxLvJhg1CXPkLpNIWfS0/uNOvMGQhzHGCj2g.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added 'ug150.eecg.utoronto.ca,128.100.13.150' (ECDSA) to the list of known hosts.
danielm6@ug150.eecg.utoronto.ca's password: 
Linux ug150 4.9.0-6-amd64 #1 SMP Debian 4.9.82-1+deb9u3 (2018-03-02) x86_64

The programs included with the Debian GNU/Linux system are free software;
the exact distribution terms for each program are described in the
individual files in /usr/share/doc/*/copyright.

Debian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent
permitted by applicable law.
Last login: Mon Mar 26 02:29:23 2018 from cpe84948c1ec6c1-cm84948c1ec6c0.cpe.net.cable.rogers.com
ug150:~% cd ece361
ug150:~/ece361% ls
exercises_lab2.txt  lab3  lab4
ug150:~/ece361% cd lab4
ug150:~/ece361/lab4% ls
lab4  submit
ug150:~/ece361/lab4% cd submit
ug150:~/ece361/lab4/submit% ls
client.c  explanations_lab4.txt  lab4.tar.gz  server.c
ug150:~/ece361/lab4/submit% vim server.c

        }

        // run through the existing connections looking for data to read
        for (i = 0; i <= fdmax; i++) {
            //printf("we out here\n");
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == listener) {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                            (struct sockaddr *) &remoteaddr,
                            &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        memset(buf,'\0',strlen(buf));
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) { // keep track of the max
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on "
                                "socket %d\n",
                                inet_ntop(remoteaddr.ss_family,
                                get_in_addr((struct sockaddr*) &remoteaddr),
                                remoteIP, INET6_ADDRSTRLEN),
                                newfd);
                        //add some shit here 
                        memset(buf,'\0',strlen(buf));
                        int messageSize = recv(newfd,buf,sizeof buf,0);
                        char *pos;
                        //printf("this weird char is %d\n",buf[sizeof(buf)+1]);   
                        int ctr;
                        for (ctr =0;ctr<sizeof(buf);++ctr){
                            //printf("char is %c\n",buf[ctr]);
                            if ((buf[ctr]>'z')||(buf[ctr]<'A'))
                                break;
                        }
                        buf[ctr]='\0';
                        char * userName = buf;

                        //printf("we tryna add: %s  to %d\n",buf,newfd);
                        printf("name is %s\n",buf);
                        for (ctr =0;ctr<60;++ctr){
                            if (strcmp(users[ctr],buf)==0){
                                printf("mandems exist already, give a diff name\n");
                                //close(newfd);
                                //FD_CLR(listener, &master);
                                foundFlag=1;
                                break;
                            }
                            else {
                                foundFlag=0;
                            }
                        }
                                                                                                                                                                   190,27        46%
