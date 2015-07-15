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
		

	std::cout << "Socket CLiente:" << socketCli << std::endl;
	
	cSocket socket(sizeof(MensajeGenerico),socketCli);

	while(1){
		// Recibir y ponerlo en la cola maestra
		MensajeGenerico msg;
		socket.tcp_recv((char*)&msg);
		std::cout << "RECIBO MENSAJE DEL SOCKET: "  << std::endl;
		
		colaMaestra->escribir(msg);
		std::cout << "REDIRECCIONO MENSAJE CON DESTINO " << msg.mtype << std::endl;
	}
}
