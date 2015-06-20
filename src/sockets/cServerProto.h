/*
Autor: Marelli Ricardo Guido
Padron: 79134

Descripcion: Define una clase para manejar el protocolo del lado del servidor
*/

#ifndef __C_SERVERPROTO_H__
#define __C_SERVERPROTO_H__

#include "cDispatcher.h"
#include <errno.h>

/* macros a utilizar por la maquina de estados del protocolo */
#define __WAITING_INIT__ 1 /* inicio del protocolo */
#define __WAITING_OP__ 2 /* esperando OP */
#define __WAITING_FIN__ 3 /* esperando FIN o OP */
#define __WAITING_NOTHING__ 4 /* fin del protocolo */

class cServerProto
{
   public:
      cServerProto();
      virtual ~cServerProto();
      static int method_init(unsigned int size, char* data, void* userdata);
      static int method_fin(unsigned int size, char* data, void* userdata);
      static int method_revert(unsigned int size, char* data, void* userdata);
      static int method_stats(unsigned int size, char* data, void* userdata);      
      static int method_fry(unsigned int size, char* data, void* userdata);      
      int reset();
      unsigned int getSeqn();
      unsigned int getHisSeqn();      
      unsigned int nextSeqn();
      int fin();
      char* getData();
      unsigned int getDataSize();
   private:
      char data[__TCP_MSG_LEN__];
      unsigned int size;
      unsigned int my_seqn;
      unsigned int his_seqn;
      int waiting; /* proximo mensaje esperado */ 
};

#endif
