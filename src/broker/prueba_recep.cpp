#include "ConexionMsg.h"
#include "MensajeGenerico.h"

#include "../IPC/Cola.h"

#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include <iostream>

#define COLA_MAESTRA ".colamaestra.mq"
#define COLA_CONEXIONES ".colaconexiones.mq"


int main(int argc, char* argv[]){

        Cola<ConexionMsg>* conexion = new Cola<ConexionMsg>(COLA_CONEXIONES,0);

        ConexionMsg msg;
        msg.mtype = 1;
        msg.cola = 2;
        conexion->escribir(msg);

        Cola<MensajeGenerico>* recibir = new Cola<MensajeGenerico>(COLA_CONEXIONES,2);
        MensajeGenerico mensaje;
        recibir->leer(2,&mensaje);
	
	std::cout <<"RECIBI: "<< mensaje.mensaje << std::endl;

}

