/*
Autor: Marelli Ricardo Guido
Padron: 79134

Implementa la clase cServerProto
*/


#include "cServerProto.h"
#include <string.h>

cServerProto::cServerProto()
{
   this->reset();
}

int cServerProto::reset()
{
   this->my_seqn = 0;
   this->his_seqn = 0;
   this->waiting = __WAITING_INIT__;
   this->data[0] = '\0';
   this->size=0;
   return 0;
}

char* cServerProto::getData()
{
    return this->data;
}

unsigned int cServerProto::getDataSize()
{
    return this->size;
}

      
unsigned int cServerProto::getSeqn()
{
    return this->my_seqn;
}

unsigned int cServerProto::nextSeqn()
{
    this->my_seqn++;
    return this->my_seqn;
}


unsigned int cServerProto::getHisSeqn()
{
    return this->his_seqn;
}

int cServerProto::fin()
{
   if(this->waiting == __WAITING_NOTHING__) return 1;
   return 0;
}

cServerProto::~cServerProto()
{

}

/* los siguientes metodos son todos estaticos! */

int cServerProto::method_init(unsigned int size, char* data, void* userdata)
{
   cServerProto* proto = (cServerProto*)userdata;
   proto->his_seqn++;
   if(proto->waiting != __WAITING_INIT__) {
       proto->reset();
       return __PROTO_ERR__;
   }
   proto->waiting = __WAITING_OP__;
   printmsg("recv: init -> waiting operations");
   return __PROTO_OP_INIT__;
}

int cServerProto::method_fin(unsigned int size, char* data, void* userdata)
{
   cServerProto* proto = (cServerProto*)userdata;
   proto->his_seqn++;
   if(proto->waiting != __WAITING_FIN__) {
       proto->reset();
       return __PROTO_ERR__;
   }
   proto->waiting = __WAITING_NOTHING__;
   printmsg("recv: fin -> sending reply");   
   return __PROTO_OP_FIN__;
}

int cServerProto::method_revert(unsigned int size, char* data, void* userdata)
{
   cServerProto* proto = (cServerProto*)userdata;
   proto->his_seqn++;
   if( (proto->waiting != __WAITING_FIN__)&&(proto->waiting != __WAITING_OP__ ) ){
       proto->reset();
       return __PROTO_ERR__;
   }  
   if(proto->size + size > __TCP_MSG_DATA_LEN__) {
       proto->reset();
       return __PROTO_ERR__;   
   }
   proto->waiting = __WAITING_FIN__;
   
   char* p = &proto->data[proto->size];  
   for(unsigned int i=0;i<size;i++) {
      p[i] = data[size-i-1];
   }
   p[size] = '\0';
   proto->size += size;
   printmsg("recv: op=10");   
   return __USER_OP_REVERT__;
}

int cServerProto::method_stats(unsigned int size, char* data, void* userdata)
{
   cServerProto* proto = (cServerProto*)userdata;
   proto->his_seqn++;
   if( (proto->waiting != __WAITING_FIN__)&&(proto->waiting != __WAITING_OP__ ) ){
       proto->reset();
       return __PROTO_ERR__;
   }  
   if(proto->size + 3*sizeof(int) > __TCP_MSG_DATA_LEN__) {
       proto->reset();
       return __PROTO_ERR__;   
   }
   proto->waiting = __WAITING_FIN__;
   
   char* p = &proto->data[proto->size];  
   int vocales=0;
   int consonantes=0;
   int letras=0;
   for(unsigned int i=0;i<size;i++) {
       if( (data[i] > 64 && data[i] < 91)||(data[i] > 96 && data[i] < 123) ) {
           letras++;
	   if(data[i]=='a' || data[i]=='e' ||
	      data[i]=='i' || data[i]=='o' || data[i]=='u' ||
	      data[i]=='A' || data[i]=='E' || 
	      data[i]=='I' || data[i]=='O' || data[i]=='U' ) 
           {
	      vocales++;      
	   }
	   else {
	      consonantes++;
	   }
       } 
   }
   memcpy(p,(void*)&vocales,sizeof(int));
   p+=sizeof(int);
   memcpy(p,(void*)&consonantes,sizeof(int));
   p+=sizeof(int);
   memcpy(p,(void*)&letras,sizeof(int));
   p+=sizeof(int);
   
   proto->size += 3*sizeof(int);
   printmsg("recv: op=11");   
   return __USER_OP_STATS__;
}

