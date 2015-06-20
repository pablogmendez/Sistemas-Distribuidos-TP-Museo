/*
Autor: Marelli Ricardo Guido
Padron: 79134

Implementa una clase cSocket para servidores
*/

#include "cServerSocket.h"
#include "common.h"

cServerSocket::cServerSocket()
{
    this->sockfd = -1;
}

cServerSocket::~cServerSocket()
{
    if(this->sockfd != -1) close(this->sockfd);
}

int cServerSocket::tcp_open_pasivo(int port, int backlog)
{
    struct sockaddr_in serv_addr;
    if(this->sockfd != -1) return -10;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons((u_short)port);
    if( (this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }
 
    if (bind(this->sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        this->sockfd=-1;
        return -1;
    }

    if(listen(this->sockfd, backlog) == -1) {
        this->sockfd=-1;
	return -1;
    }
    return 0;
}

int cServerSocket::tcp_accept()
{
    struct sockaddr_in cli_addr;
    unsigned clilen = sizeof(cli_addr);
    int clientsock = accept(this->sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if(clientsock < 0) {
        return -1;
    }
    return clientsock;
}

int cServerSocket::tcp_close()
{
    if(this->sockfd != -1) {
        close(this->sockfd);
        this->sockfd=-1;
    }
    return 0;
}
