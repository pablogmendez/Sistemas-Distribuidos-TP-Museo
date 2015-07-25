#include "ConexionMsg.h"
#include "MensajeGenerico.h"

#include "../IPC/Cola.h"
#include "../sockets/cClientSocket.h"
#include "Constantes.h"
#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>


int main(int argc, char* argv[]){
	
	cClientSocket socket(sizeof(MensajeGenerico));
	MensajeGenerico mensaje;
	if(socket.tcp_open_activo("localhost", BROKER_WRITERS_PORT)){
		std::cout << "ERROR OPENING CONNECTION" << std::endl;
	}

	mensaje.mtype = atoi(argv[1]);
	mensaje.id = atoi(argv[2]);
	mensaje.shmem.abierto = 1;
	mensaje.shmem.capacidad = 2;
	mensaje.shmem.personas = 3;
	
	socket.tcp_send((char*) &mensaje);
	std::cout << "ENVIE AL DESTINO 1";

	// Mando desconexion
	mensaje.msg.op = MuseoMSG::NOTIF_DESCONEXION;
	socket.tcp_send((char*) &mensaje);
}
