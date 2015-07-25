#include "../IPC/Cola.h"
#include "MensajeGenerico.h"
#include "../sockets/cSocket.h"
#include "Proceso.h"
#include "Constantes.h"

#define COLA_MAESTRA ".colamaestra.mq"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <signal.h> 
#include <fcntl.h>

int main(int argc,char* argv[]){
	int socketCli = atoi(argv[1]);
	
	Cola<MensajeGenerico>* colaMaestra = new Cola<MensajeGenerico>(COLA_MAESTRA,0);
		

	std::cout << "Socket CLiente:" << socketCli << std::endl;
	
	cSocket socket(sizeof(MensajeGenerico),socketCli);

	MensajeGenerico msg;
	while(1){
		// Recibir y ponerlo en la cola maestra
		socket.tcp_recv((char*)&msg);
		std::cout << "RECIBO MENSAJE DEL SOCKET: "  << std::endl;
	
		if(msg.msg.op == MuseoMSG::NOTIF_DESCONEXION){
			break;
		}	
		colaMaestra->escribir(msg);
		std::cout << "REDIRECCIONO MENSAJE CON DESTINO " << msg.mtype << std::endl;
	}

	// ABRO ARCHIVO DE PROCESOS
        int fd = open(BROKER_PIDS_FILE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        // TOMO LOCK
        struct flock fl;
        fl.l_type=F_WRLCK;
        fl.l_whence=SEEK_SET;
        fl.l_start=0;
        fl.l_len=0;
	fcntl(fd,F_SETLKW,&fl);
	
	// Busco el pid del escritor para matarlo
	Proceso p;
        while(read(fd,&p,sizeof(p)) > 0){
        	if(!p.borrado && p.id == msg.id){
                	kill(p.pid_escritor,SIGINT);
                        std::cout << "MATO PROCESO ESCRITOR DE ID:" << p.id << std::endl;
			break;
                }
	}
	// Marco como borrado
	p.borrado = 1;
	lseek(fd, -sizeof(p), SEEK_CUR);
	write(fd,&p,sizeof(p));
        // DEVUELVO LOCK
        fl.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&fl);

}
