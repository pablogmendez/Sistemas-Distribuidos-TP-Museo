/*
Autor: Marelli Ricardo Guido
Padron: 79134

Implementa la clase cDispatcher
*/

#include "cDispatcher.h"
#include <string.h>
#include <stdlib.h>

cDispatcher::cDispatcher(unsigned int source_addr)
{
   for(int i=0;i<=__MAX_OP_NUMBER__;i++) {
       this->opmethods[i] = &default_method;
       this->methods_userdata[i] = NULL;
   }
   this->source_addr = source_addr;
   this->dest_addr = 0;
}

void cDispatcher::setDestAddr(unsigned int dest_addr)
{
   this->dest_addr = dest_addr;
}

int cDispatcher::getDestAddr(unsigned int dest_addr)
{
   return this->dest_addr;
}

int cDispatcher::default_method(unsigned int size, char* data, void* userdata)
{
   return __PROTO_NO_OP__;
}

int cDispatcher::registerMethod(char op, int (* method)(unsigned int,char*,void*),void* userdata)
{
   int method_op = (int)op;
   if(method_op > __MAX_OP_NUMBER__) return -1;
   this->opmethods[method_op] = method;   
   this->methods_userdata[method_op] = userdata;      
   return 0;
}


int cDispatcher::dispathMessage(unsigned int nseq, char* mensaje)
{
   /* mensaje = |nseq|source|dest|op|size|data|*/
   unsigned int mes_nseq=0;
   unsigned int mes_source=0;
   unsigned int mes_dest=0;
   unsigned int mes_size=0;
   int op=0;
   char* data;

   memcpy(&mes_nseq,mensaje,sizeof(unsigned int));
    mensaje+=sizeof(unsigned int);
   
   if(nseq != mes_nseq) {
       return __PROTO_BAD_NSEQ__;
   }
   
   memcpy(&mes_source,mensaje,sizeof(unsigned int));
   mensaje+=sizeof(unsigned int);   
   memcpy(&mes_dest,mensaje,sizeof(unsigned int));
   mensaje+=sizeof(unsigned int);            
   op=mensaje[0];
   mensaje++;
   
   if(op > __MAX_OP_NUMBER__) {
       return __PROTO_BAD_OP__;
   }   
   
   memcpy(&mes_size,mensaje,sizeof(unsigned int));
   mensaje+=sizeof(unsigned int);              

   if(mes_size > 0) {
      data = mensaje;
   }
   else {
      data = NULL;
   }
   return this->opmethods[op](mes_size,data,this->methods_userdata[op]);
}

int cDispatcher::makeMessage(char* buffer, unsigned int nseq, char op, unsigned int size, char* data)
{
   /* mensaje = |nseq|source|dest|op|size|data|*/
   char* p = buffer;
   if(p == NULL || ((data == NULL) && (size > 0))) {
      printmsg("Die! Invalid parameters");
      exit(1);
   }
   if((int)op > __MAX_OP_NUMBER__) {
      printmsg("Die! Invalid parameters");
      exit(1);   
   }
   memcpy(p,&nseq,sizeof(unsigned int));
   p+=sizeof(unsigned int);
   memcpy(p,&this->source_addr,sizeof(unsigned int));
   p+=sizeof(unsigned int);
   memcpy(p,&this->dest_addr,sizeof(unsigned int));
   p+=sizeof(unsigned int);
   p[0] = op;
   p+=sizeof(char);
   memcpy(p,&size,sizeof(unsigned int));
   if(size > 0) {
      p+=sizeof(unsigned int);
      memcpy(p,data,size);
   }
   return 0;
}

cDispatcher::~cDispatcher()
{

}

