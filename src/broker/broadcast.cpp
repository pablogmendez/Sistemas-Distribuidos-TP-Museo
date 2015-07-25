#include "MensajeGenerico.h"
#include "../IPC/Cola.h"
#include "../IPC/Semaforo.h"
#include "../IPC/MemoriaCompartida.h"
#include "Constantes.h"
#include "Proceso.h"
#include <iostream>
#include <fcntl.h>


#define COLA_MAESTRA ".colamaestra.mq"
#define SEMAFORO_SH_MEM ".semaforo_shmem"
#define SH_MEM ".shmem"
#define ID_SHMEM_P 1
#define ID_SHMEM_V 2

int main(int argc, char* argv[]){
	Cola<MensajeGenerico>* colaMaestra = new Cola<MensajeGenerico>(COLA_MAESTRA,0);
	MensajeGenerico msg;
	while(1){
		// RECIBO PEDIDO DE BROADCAST
		colaMaestra->leer(BROKER_BROADCAST_ID, &msg);
		std::cout << "RECIBI UN PEDIDO DE BROADCAST DE: " << msg.id << std::endl;
		
		// MANDO EL BROADCAST
		// ABRO ARCHIVO DE PROCESOS
		int fd = open(BROKER_PIDS_FILE, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
		// TOMO LOCK
		struct flock fl;
		fl.l_type=F_RDLCK;
    		fl.l_whence=SEEK_SET;
    		fl.l_start=0;
    		fl.l_len=0;
		fcntl(fd,F_SETLKW,&fl);

		Proceso p;
		while(read(fd,&p,sizeof(p)) > 0){
			if(!p.borrado && p.tipo == PUERTA){
				long id = p.id;
				msg.mtype = id;
				colaMaestra->escribir(msg);
				std::cout << "MANDO MENSAJE DE BROADCAST A: " << msg.mtype << std::endl;
			}
		}
		// DEVUELVO LOCK
		fl.l_type=F_UNLCK;
    		fcntl(fd,F_SETLK,&fl);
	}

	return 0;
}
