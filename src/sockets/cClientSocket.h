/*
Autor: Marelli Ricardo Guido
Padron: 79134

Descripcion: Define una clase socket para clientes
*/

#ifndef __C_CLIENTSOCKET_H__
#define __C_CLIENTSOCKET_H__

#include "cSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

class cClientSocket : public cSocket
{
   public:
      cClientSocket(int buffer_size);
      int tcp_open_activo(const char* server, int port);
      int tcp_adopt_connection (int fd);
      int getFD () const { return sockfd; }
};

#endif

