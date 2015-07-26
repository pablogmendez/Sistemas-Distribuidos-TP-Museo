#include "../IPC/Cola.h"
#include "MensajeGenerico.h"
#include "../sockets/cSocket.h"
#include "Constantes.h"
#include "Proceso.h"
#include <fcntl.h>


#define COLA_MAESTRA ".colamaestra.mq"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char* argv[]){
        int socketCli = atoi(argv[1]);
       
        Cola<MensajeGenerico>* colaMaestra = new Cola<MensajeGenerico>(COLA_MAESTRA,0);

      
        std::cout << "Socket Ciente:" << socketCli << std::endl;

        cSocket socket(sizeof(MensajeGenerico),socketCli);

	MensajeGenerico msg;
	socket.tcp_recv((char*) &msg);
	// MY ID
	long id = msg.id;
	
	//Registro en archivo de conectados
	int fd = open(BROKER_PIDS_FILE, O_WRONLY |O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
	struct flock fl;
	fl.l_type=F_WRLCK;
    	fl.l_whence=SEEK_SET;
    	fl.l_start=0;
    	fl.l_len=0;	
	fl.l_type=F_WRLCK;
    	fcntl(fd,F_SETLKW,&fl);
	struct Proceso p;
	p.pid_escritor = getpid();
	p.id = id;
	if(id < 200){
		p.tipo = PUERTA;
	}else if(id <300){
		p.tipo = MUSEO;
	}else{
		p.tipo = PERSONA;
	}
	p.borrado = 0;
	write(fd,&p,sizeof(p));
	fl.l_type=F_UNLCK;
    	fcntl(fd,F_SETLK,&fl);	
	
        while(1){
                // Recibir y ponerlo en la cola maestra
                MensajeGenerico msg;
                colaMaestra->leer(id,&msg);
                std::cout << "RECIBO MENSAJE DE LA COLA MAESTRA: "  << std::endl;

                socket.tcp_send((char*) &msg);
                std::cout << "REDIRECCIONO MENSAJE " << std::endl;
        }
}

