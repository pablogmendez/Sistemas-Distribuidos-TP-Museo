#include "MensajeGenerico.h"
#include "../IPC/Semaforo.h"
#include "../IPC/Cola.h"
#include "../IPC/MemoriaCompartida.h"

#include <iostream>

#define COLA_MAESTRA ".colamaestra.mq"
#define SEMAFORO_SH_MEM ".semaforo_shmem"
#define SH_MEM ".shmem"
#define ID_SHMEM_P 1
#define ID_SHMEM_V 2

int main(int argc, char* argv[]){
	Cola<MensajeGenerico>* colaMaestra = new Cola<MensajeGenerico>(COLA_MAESTRA,0);
	Semaforo* sem = new Semaforo(SEMAFORO_SH_MEM,0); 
	MemoriaCompartida<int>* shMem = new MemoriaCompartida<int>();
	if(shMem->crear(SH_MEM,0)){
		std::cout << "ERROR EN LA SH MEM" << std::endl;
	}
	MensajeGenerico msg;

	
	MensajeGenerico msg2;
	while(1){
		// RECIBO SH MEM
		colaMaestra->leer(2,&msg);
		std::cout << "DEVOLVIO LA SH MEM: " << msg.id << std::endl;
		// ESCRIBO LA SH MEM
		shMem->escribir(msg.shmem);
		// PERMITO OTRO PEDIDO
		sem->v();
	}

	return 0;
}
