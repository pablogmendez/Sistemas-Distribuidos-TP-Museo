/*
Autor: Marelli Ricardo Guido
Padron: 79134

*/


#include "cSocket.h"
#include "cDispatcher.h"
#include "cServerProto.h"
#include "common.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void main_proc(int sockfd)
{
   cSocket osocket(__TCP_MSG_LEN__,sockfd);
   char mensaje[__TCP_MSG_LEN__];
   /* preparo el dispatcher */
   cServerProto protocol;
   cDispatcher dispatcher(__CLIENT_ADDR__);
   dispatcher.setDestAddr(__SERVER_ADDR__);
   dispatcher.registerMethod(__PROTO_OP_INIT__,&protocol.method_init,&protocol);
   dispatcher.registerMethod(__PROTO_OP_FIN__,&protocol.method_fin,&protocol);
   dispatcher.registerMethod(__USER_OP_REVERT__,&protocol.method_revert,&protocol);
   dispatcher.registerMethod(__USER_OP_STATS__,&protocol.method_stats,&protocol); 

   int salir=0;
   int retval=-1;
   while(!salir) {
      if(osocket.tcp_recv(mensaje) == -1) {
          printmsg("Error receving msg: %s",strerror(errno));
          exit(1);           
      }
      retval = dispatcher.dispathMessage(protocol.getHisSeqn(),mensaje);
      if(retval < 0) {
          if(retval == __PROTO_BAD_NSEQ__) {
            printmsg("Client protocol violation!");
            exit(1);     
	  }
	  else {
            printmsg("Client protocol violation!");
            exit(1);     
	  }    
      }
      else {
         if(protocol.fin()==1) {
            dispatcher.makeMessage(mensaje,protocol.getSeqn(),__PROTO_OP_REPLY__,protocol.getDataSize(),protocol.getData());
            if(osocket.tcp_send(mensaje) == -1) {
               printmsg("Error sending msg: %s",strerror(errno));
               exit(1);     
            }
	    salir=1;
	 }
      }
   }

}

int main(int argc,char** argv)
{  
  
   if(signal(SIGPIPE,SIG_IGN)==SIG_ERR) {
       printmsg("DIE! signal(SIGPIPE)");
       exit(1);
   }

   if (argc > 1) {
       int sockfd = atoi(argv[1]);
       if(sockfd > 0) {
           main_proc(sockfd); 
       }
       else {
           printmsg("Panic! Invalid socket");
           exit(1);
       }
   }
   else {
       printmsg("Panic! No socket receive");
       exit(1);
   }
}
