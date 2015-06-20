/*
Autor: Marelli Ricardo Guido
Padron: 79134

Descripcion: 
    Se encarga de armar mensajes y de despacharlos a la funcion correspondiente
    Quizas seria mas correcto llamarlo cProtocolo...
*/

#ifndef __C_DISPATCHER_H__
#define __C_DISPATCHER_H__

#include "common.h"

class cDispatcher
{
   public:
      cDispatcher(unsigned int source_addr);  
      /* No necesariamente conozco dest_addr al crear al objeto */
      void setDestAddr(unsigned int dest_addr);
      int getDestAddr(unsigned int dest_addr);      
      
      int makeMessage(char* buffer, unsigned int nseq, char op, unsigned int size, char* data);
      // agregar un nuevo metodo a la tabla
      int registerMethod(char op, int (* method)(unsigned int,char*,void*),void* userdata);
      // procesar un mensaje 
      int dispathMessage(unsigned int nseq, char* mensaje);
      virtual ~cDispatcher(); 
   private:
      unsigned int source_addr;
      unsigned int dest_addr;
      //metodos invocados de acuerdo al campo (op) operacion del mensaje
      // Los parametros que toman son:
      // size = longitud en bytes de data 
      // data = campo data extraido del mensaje
      // userdata = un puntero del usuario 

      int (* opmethods[__MAX_OP_NUMBER__])(unsigned int,char*,void*);  
      void* methods_userdata[__MAX_OP_NUMBER__];
      /*
         Metodo que es invocada por defecto cuando
         no se ha registrado uno especifico para la operacion.
      */
      static int default_method(unsigned int size, char* data, void* userdata);
};

#endif

