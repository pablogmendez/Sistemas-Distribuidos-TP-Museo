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
	// My ID
	int id = atoi(argv[1]);	

	cClientSocket socketEntrada(sizeof(MensajeGenerico));
	MensajeGenerico mensaje;
	if(socketEntrada.tcp_open_activo("localhost",5000)){
		std::cout << "ERROR OPENING CONNECTION" << std::endl;
	}
	mensaje.mtype = 1;
	mensaje.id = id;
	// Pido la sh mem
	socketEntrada.tcp_send((char*) &mensaje);
	std::cout << "PEDI SH MEM" << std::endl;
	
	cClientSocket socketSalida(sizeof(MensajeGenerico));
	if(socketSalida.tcp_open_activo("localhost",5001)){
		std::cout << "ERROR OPENING CONNECTION" << std::endl;
	}
	// Paso mi identificador al proceso de salida en el broker
	mensaje.id = id;
	socketSalida.tcp_send((char*) &mensaje);
	// Espero la SH mem
	socketSalida.tcp_recv((char*) &mensaje);
	std::cout << "RECIBI SH MEM. VALOR: " << mensaje.shmem << std::endl;

	int nuevoValor;
	std::cin >> nuevoValor;
	
	MensajeGenerico resp;
	resp.mtype = 2;
	resp.id = id;
	resp.shmem = nuevoValor;	
	socketEntrada.tcp_send((char*) &resp);
	std::cout << "DEVOLVI LA SH MEM CON VALOR: " << resp.shmem;

	return 0;
}
