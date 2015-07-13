/*
Autor: Marelli Ricardo Guido
Padron: 79134

Implementa una clase cSocket para clientes
*/

#include "cClientSocket.h"
#include "common.h"

cClientSocket::cClientSocket(int buffer_size) : cSocket(buffer_size)
{

}

int cClientSocket::tcp_open_activo(const char* server, int port)
{
    struct sockaddr_in serv_addr;
    struct hostent *ptr_server; 
    if(this->sockfd != -1) return -10;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    ptr_server = gethostbyname(server);
    if ( ((char *)ptr_server) == NULL ) {
	return -2;
    }
    memcpy(&serv_addr.sin_addr, ptr_server->h_addr, ptr_server->h_length);

    if( (this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }

    if (connect(this->sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
	close(this->sockfd);
        return -1;
    }
    return 0;
}

int cClientSocket::tcp_adopt_connection (int fd)
{
	if (fd == -1 || sockfd != -1) {
		return -10;
	}
	sockfd = fd;
	return 0;
}
