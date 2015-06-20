/*
Autor: Marelli Ricardo Guido
Padron: 79134

Descripcion: Define una clase bara intercambio de informacion entre sockets.
*/

#ifndef __C_SOCKET_H__
#define __C_SOCKET_H__

#include <errno.h>
#include <unistd.h>

class cSocket
{
   public:
      cSocket(int buffer_size, int sockfd=-1);
      int tcp_recv(register char* buffer);
      int tcp_send(register char* buffer);
      virtual ~cSocket(); 
   protected:
      int sockfd;
   private:
      int size;  
};

#endif

