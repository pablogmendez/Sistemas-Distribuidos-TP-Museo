#include "../IPC/Cola.h"
#include "MensajeGenerico.h"
#include "../sockets/cSocket.h"

#define COLA_MAESTRA ".colamaestra.mq"

#include <iostream>
#include <stdlib.h>
#include <string.h>

int main(int argc,char* argv[]){
        int socketCli = atoi(argv[1]);
       
        Cola<MensajeGenerico>* colaMaestra = new Cola<MensajeGenerico>(COLA_MAESTRA,0);

      
        std::cout << "Socket Ciente:" << socketCli << std::endl;

        cSocket socket(sizeof(MensajeGenerico),socketCli);

	MensajeGenerico msg;
	socket.tcp_recv((char*) &msg);
	// MY ID
	int id = msg.id;
        while(1){
                // Recibir y ponerlo en la cola maestra
                MensajeGenerico msg;
                colaMaestra->leer(id,&msg);
                std::cout << "RECIBO MENSAJE DE LA COLA MAESTRA: "  << std::endl;

                socket.tcp_send((char*) &msg);
                std::cout << "REDIRECCIONO MENSAJE " << std::endl;
        }
}

