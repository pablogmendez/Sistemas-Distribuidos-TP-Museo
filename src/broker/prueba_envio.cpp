#include "ConexionMsg.h"
#include "MensajeGenerico.h"

#include "../IPC/Cola.h"

#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#define COLA_MAESTRA ".colamaestra.mq"
#define COLA_CONEXIONES ".colaconexiones.mq"


int main(int argc, char* argv[]){

	Cola<ConexionMsg>* conexion = new Cola<ConexionMsg>(COLA_CONEXIONES,0);
	
	ConexionMsg msg;
	msg.mtype = 1;
	msg.cola = 1;
	conexion->escribir(msg);

	Cola<MensajeGenerico>* enviar = new Cola<MensajeGenerico>(COLA_CONEXIONES,1);
	MensajeGenerico mensaje;
	mensaje.mtype = 1;
	mensaje.destino = 2;
	strncpy(mensaje.mensaje, "hola",4);
	
	enviar->escribir(mensaje);

}
