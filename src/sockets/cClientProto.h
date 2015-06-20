/*
Autor: Marelli Ricardo Guido
Padron: 79134

Descripcion: Define una clase para manejar el protocolo del lado del servidor
*/

#ifndef __C_CLIENTPROTO_H__
#define __C_CLIENTPROTO_H__

#include "cDispatcher.h"
#include <errno.h>

class cClientProto
{
   public:
      cClientProto();
      virtual ~cClientProto();
      static int method_reply(unsigned int size, char* data, void* userdata);
      int reset();
      unsigned int getSeqn();
      unsigned int getHisSeqn();      
      unsigned int nextSeqn();
      char* getData();
      unsigned int getDataSize();
   private:
      char data[__TCP_MSG_LEN__];
      unsigned int size;
      unsigned int my_seqn;
      unsigned int his_seqn;
};

#endif

