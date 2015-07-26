/*
Autor: Marelli Ricardo Guido
Padron: 79134

Implementa una clase cSocket
*/

#include "cSocket.h"
#include "common.h"

cSocket::cSocket(int buffer_size, int sockfd) 
{
    this->sockfd = sockfd;
    this->size = buffer_size;
}

cSocket::~cSocket()
{
    if(this->sockfd != -1) close(this->sockfd);
}

int cSocket::tcp_recv(register char* buffer)
{
    int total=0;
    if(this->sockfd == -1) return -1;
    while(total < this->size) { 
       int leidos = read(this->sockfd, &buffer[total], this->size - total);
       if(leidos == -1 /*ERROR*/) {
           return -1;
       }
	   if (leidos == 0 /*EOF*/) {
           return 0;
       }
       total+=leidos;
    }
    return 1;
}

int cSocket::tcp_send(register char* buffer)
{
    int total=0;
    if(this->sockfd == -1) return -1;
    while(total < this->size) { 
        int escritos = write(this->sockfd, &buffer[total], this->size - total);
        if(escritos == -1) {
            return -1;
	}
	total+=escritos;
    }
    return 0;
}
