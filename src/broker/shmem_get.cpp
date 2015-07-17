#include "MensajeGenerico.h"
#include "../IPC/Cola.h"
#include "../IPC/Semaforo.h"
#include "../IPC/MemoriaCompartida.h"
#include <broker/Constantes.h>
#include <iostream>
#include <museo/MuseoSHM.h>

#define COLA_MAESTRA ".colamaestra.mq"
#define SEMAFORO_SH_MEM ".semaforo_shmem"
#define SH_MEM ".shmem"
#define ID_SHMEM_P 1
#define ID_SHMEM_V 2

int main(int argc, char* argv[]){
	Cola<MensajeGenerico>* colaMaestra = new Cola<MensajeGenerico>(COLA_MAESTRA,0);
	Semaforo* sem = new Semaforo(SEMAFORO_SH_MEM,0); 
	MemoriaCompartida<MuseoSHM>* shMem = new MemoriaCompartida<MuseoSHM>();
	if(shMem->crear(SH_MEM,0)){
		std::cout << "ERROR EN LA SH MEM" << std::endl;
	}
	MensajeGenerico msg;

	{
		MuseoSHM tmp = {0, 0, 0};
		shMem->escribir(tmp);
	}
	MensajeGenerico msg2;
	while(1){
		// RECIBO PEDIDO DE SH MEM
		colaMaestra->leer(BROKER_SHMGET_ID, &msg);
		std::cout << "RECIBI UN PEDIDO DE: " << msg.id << std::endl;
		// MANDO LA SH MEM
		msg2.mtype = msg.id;
		msg2.shmem = shMem->leer();
		colaMaestra->escribir(msg2);
		std::cout << "MANDO LA SH MEM A: " << msg2.mtype << std::endl;
		// ESPERO A QUE LA DEVUELVAN
		sem->p();
	}

	return 0;
}
