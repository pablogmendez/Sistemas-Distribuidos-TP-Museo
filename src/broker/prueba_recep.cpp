#include "ConexionMsg.h"
#include "MensajeGenerico.h"

#include "../IPC/Cola.h"
#include "../sockets/cClientSocket.h"
#include <broker/Constantes.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include <iostream>

int main(int argc, char* argv[]){
	cClientSocket socket(sizeof(MensajeGenerico));
        MensajeGenerico mensaje;
        socket.tcp_open_activo("localhost", BROKER_READERS_PORT);
	mensaje.id=1;
	socket.tcp_send((char*)&mensaje);
	socket.tcp_recv((char*)&mensaje);
	
	std::cout <<"RECIBI: \n"
		<< "\tshmem.abierto  : " << mensaje.shmem.abierto << '\n'
		<< "\tshmem.capacidad: " << mensaje.shmem.capacidad << '\n'
		<< "\tshmem.personas : " << mensaje.shmem.personas
		<< std::endl;
}

