#include "../IPC/Cola.h"
#include "MensajeGenerico.h"

#define COLA_MAESTRA ".colamaestra.mq"
#define COLA_CONEXIONES ".colaconexiones.mq"

#include <iostream>
#include <stdlib.h>
#include <string.h>

int main(int argc,char* argv[]){
        int id = atoi(argv[1]);
        std::cout << id;

        // Sacar de la cola y mandarlo
	while(1){
		MensajeGenerico msg;
                Cola<MensajeGenerico>* colaMaestra = new Cola<MensajeGenerico>(COLA_MAESTRA,0);
                std::cout << "RECIBO MENSAJE DE LA COLA MAESTRA con id:" << id << std::endl;
		colaMaestra->leer(id,&msg);
		
		MensajeGenerico msg2;
                msg2.mtype = 2;
                strncpy(msg2.mensaje, msg.mensaje,strlen(msg.mensaje));

		Cola<MensajeGenerico>* entrada = new Cola<MensajeGenerico>(COLA_CONEXIONES,id);
                std::cout << "LO MANDO POR LA COLA DE CONEXIONES "<< id << std::endl;
                entrada->escribir(msg2);
        }

}

