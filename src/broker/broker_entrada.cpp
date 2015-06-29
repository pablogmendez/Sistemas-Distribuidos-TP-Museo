#include "../IPC/Cola.h"
#include "MensajeGenerico.h"

#define COLA_MAESTRA "./colamaestra.mq"
#define COLA_CONEXIONES "./colaconexiones.mq"

#include <iostream>
#include <stdlib.h>
#include <string.h>

int main(int argc,char* argv[]){
	int cola = atoi(argv[1]);
	std::cout << cola << std::endl;

	while(1){
		// Recibir y ponerlo en la cola maestra
		Cola<MensajeGenerico>* entrada = new Cola<MensajeGenerico>(COLA_CONEXIONES,cola);
		MensajeGenerico msg;
		std::cout << "RECIBO MENSAJE DE LA COLA DE CONEXIONES "<< cola << std::endl;
		entrada->leer(1,&msg);
	
		MensajeGenerico msg2;
		msg2.mtype = msg.destino;
		strncpy(msg2.mensaje, msg.mensaje,strlen(msg.mensaje));
	
		std::cout << "REDIRECCIONO MENSAJE CON DESTINO " << msg.destino << std::endl;
		Cola<MensajeGenerico>* colaMaestra = new Cola<MensajeGenerico>(COLA_MAESTRA,0);
		colaMaestra->escribir(msg2);
	}
}
