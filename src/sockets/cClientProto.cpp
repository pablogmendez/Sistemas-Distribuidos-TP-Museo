/*
Autor: Marelli Ricardo Guido
Padron: 79134

Implementa la clase cServerProto
*/


#include "cClientProto.h"
#include <string.h>

cClientProto::cClientProto()
{
   this->reset();
}

int cClientProto::reset()
{
   this->my_seqn = 0;
   this->his_seqn = 0;
   this->data[0] = '\0';
   this->size=0;
   return 0;
}

char* cClientProto::getData()
{
    return this->data;
}

unsigned int cClientProto::getDataSize()
{
    return this->size;
}
      
unsigned int cClientProto::getSeqn()
{
    return this->my_seqn;
}

unsigned int cClientProto::nextSeqn()
{
    this->my_seqn++;
    return this->my_seqn;
}


unsigned int cClientProto::getHisSeqn()
{
    return this->his_seqn;
}

cClientProto::~cClientProto()
{

}

/* los siguientes metodos son todos estaticos! */

int cClientProto::method_reply(unsigned int size, char* data, void* userdata)
{
   cClientProto* c = (cClientProto*)userdata;
   memcpy(c->data,data,size);
   c->data[size] = '\0';
   c->size = size;
   return __PROTO_OP_REPLY__;
}

