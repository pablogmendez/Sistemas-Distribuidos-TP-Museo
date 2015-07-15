#include "ConexionMsg.h"
#include "MensajeGenerico.h"

#include "../IPC/Cola.h"
#include "../sockets/cClientSocket.h"

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
	if(socket.tcp_open_activo("localhost",5000)){
		std::cout << "ERROR OPENING CONNECTION" << std::endl;
	}

	mensaje.mtype = 1;
	mensaje.id = 2;
	strncpy(mensaje.mensaje, "hola",4);
	
	socket.tcp_send((char*) &mensaje);
	std::cout << "ENVIE AL DESTINO 1";
}
