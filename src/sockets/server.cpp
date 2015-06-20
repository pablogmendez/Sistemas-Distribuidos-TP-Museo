/*
Autor: Marelli Ricardo Guido
Padron: 79134

*/


#include "cServerSocket.h"
#include "common.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

void sig_chld(int n)
{
    int status;
    while (waitpid(0,&status, WNOHANG) >= 0);
}


int main(int argc,char** argv)
{  
   cServerSocket server;
   int port =__COMMON_PORT__;
   if (argc > 1) {
	port = atoi(argv[1]);
	if(port <= 0) {
           printmsg("DIE! Invalid port");
           exit(1);
	}
   }
   
   if(signal(SIGCHLD,&sig_chld)==SIG_ERR) {
       printmsg("DIE! signal(SIGCHLD)");
       exit(1);
   }

   if(server.tcp_open_pasivo(port,20) != 0) {
       printmsg("DIE! Could not create server: %s",strerror(errno));
       exit(1);
   }
   printmsg("Server is running");
   while(1) {
       int sockfd = server.tcp_accept();
       pid_t child_pid=-1;
       if(sockfd == -1) {
           printmsg("DIE! Could not accept client: %s",strerror(errno));
           exit(1);
        }
        else {
           child_pid = fork();
           if(child_pid == 0) {
                char s_sockfd[15];
                printmsg("Client accepted");
	        server.tcp_close();
		sprintf(s_sockfd,"%d",sockfd);
	        execlp("./serverproto", "serverproto", s_sockfd, NULL);
                printmsg("Error: Could not exec serverproto: %s",strerror(errno));
		exit(2);              
           }
           else if(child_pid == -1) {
               printmsg("Error: Could not fork: %s",strerror(errno));
           }
           else {
               close(sockfd);
           }
       }
   }
}
