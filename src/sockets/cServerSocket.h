/*
Autor: Marelli Ricardo Guido
Padron: 79134

Descripcion:
    Define una clase socket para servidores
*/

#ifndef __C_SERVERSOCKET_H__
#define __C_SERVERSOCKET_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

class cServerSocket
{
   public:
      cServerSocket();
      int tcp_open_pasivo(int port, int backlog);
      int tcp_accept();
      virtual ~cServerSocket();
      int tcp_close();
   private:
      int sockfd;
};

#endif

