/*
Autor: Marelli Ricardo Guido
Padron: 79134

*/

#include "cClientProto.h"
#include "cClientSocket.h"
#include "cDispatcher.h"
#include "common.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
   char op;
   char data[__TCP_MSG_LEN__];
   int size;
} sOperacion;

int cargar_archivo(sOperacion* ops)
{
   char mensaje[__TCP_MSG_LEN__];
   int totalops=0;

   /* leo las operaciones del archivo */
   FILE* f;
   
   f=stdin;
   while(!feof(f)) {
       if(fgets(mensaje,__TCP_MSG_LEN__,f)==NULL) {
          printmsg("Error reading file");
          exit(1);
       }
       int size = strlen(mensaje) - 1; /* op|data|\n */
       if(mensaje[size] == '\n') { 
           mensaje[size] = '\0';
           size--;
       }
       ops[totalops].size = size;
       ops[totalops].op = mensaje[0]-38;
       strcpy(ops[totalops].data,&mensaje[1]);   
       totalops++;
   } 
   fclose(f);
   return totalops;
}

void main_proc(const char* server_name, int port, sOperacion* ops, int totalops)
{
   cClientSocket cliente(__TCP_MSG_LEN__);  
   if(cliente.tcp_open_activo(server_name,port) == -1) {
       printmsg("DIE! Could not connect to the server: %s",strerror(errno));
       exit(1);   
   }

   /* preparo el dispatcher */
   cDispatcher dispatcher(__CLIENT_ADDR__);
   dispatcher.setDestAddr(__SERVER_ADDR__);
   cClientProto protocol;
   dispatcher.registerMethod(__PROTO_OP_REPLY__,protocol.method_reply,&protocol);

   char mensaje[__TCP_MSG_LEN__];
  
   int salir=0;
   while(!salir) {
      /* envio inicio de la secuencia */
      dispatcher.makeMessage(mensaje,protocol.getSeqn(),__PROTO_OP_INIT__,0,NULL);
      if(cliente.tcp_send(mensaje) == -1) {
          printmsg("Error sending msg: %s",strerror(errno));
          exit(1);     
      }
      printmsg("send: init");

      for(int i=0;i<totalops;i++) {
          /* envio las operaciones */   
	 dispatcher.makeMessage(mensaje,protocol.nextSeqn(),ops[i].op,ops[i].size,ops[i].data);
         if(cliente.tcp_send(mensaje) == -1) {
            printmsg("Error sending msg: %s",strerror(errno));
            exit(1);     
         }
         printmsg("send: op=%d, data=%s",(int)ops[i].op,ops[i].data);	 
      }

      /* envio fin de la secuencia */      
      dispatcher.makeMessage(mensaje,protocol.nextSeqn(),__PROTO_OP_FIN__,0,NULL);
      if(cliente.tcp_send(mensaje) == -1) {
         printmsg("Error sending msg: %s",strerror(errno));
         exit(1);     
      }
      printmsg("send: fin");
      /* recibo la respuesta */
      if(cliente.tcp_recv(mensaje) == -1) {
         printmsg("Error receving msg: %s",strerror(errno));
         exit(1);           
      }
   
      int retval = dispatcher.dispathMessage(protocol.getHisSeqn(),mensaje);
      if (retval > 0) {
         if(retval == __PROTO_OP_REPLY__) {
            char* p = protocol.getData();
	    printmsg("size: %d",protocol.getDataSize());
	    for(int i=0;i<totalops;i++) {
	        if(ops[i].op == __USER_OP_REVERT__) {
	            char buffer[__TCP_MSG_LEN__];
		    strncpy(buffer,p,ops[i].size);
		    buffer[ops[i].size]='\0';
		    p+=ops[i].size;
		    printmsg("recv result op%d: %s",i,buffer);
		}
		else if(ops[i].op == __USER_OP_STATS__) {
                    int v; int c; int l;
		    memcpy(&v,p,sizeof(int));
		    p+=sizeof(int);
		    memcpy(&c,p,sizeof(int));
		    p+=sizeof(int);
		    memcpy(&l,p,sizeof(int));
		    p+=sizeof(int);		    		
		    printmsg("recv result op%d: vocales=%d, consonantes=%d, letras=%d",i,v,c,l);	
		}
	    }
	    salir=1;
         }
	 else {
            printmsg("Error!");
	    exit(1);	 
	 }
      }
      else {
         printmsg("Protocol error!");
         exit(1);              
      }
   }

}

int main(int argc,char** argv)
{
   const char* server_name;
   int port =__COMMON_PORT__;
   if (argc > 2) {
	port = atoi(argv[2]);
	if(port <= 0) {
           printmsg("DIE! Invalid port");
           exit(1);
	}
   }
   
   if (argc > 1) {
       server_name = argv[1];
   }
   else {
       server_name = "localhost";
   }
   
   if(signal(SIGPIPE,SIG_IGN)==SIG_ERR) {
       printmsg("DIE! signal(SIGPIPE)");
       exit(1);
   }

   sOperacion ops[500];
   int totalops = cargar_archivo(&ops[0]);

   main_proc(server_name,port,ops,totalops);
   
}
