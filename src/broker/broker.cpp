#include "../IPC/Semaforo.h"
#include "MensajeGenerico.h"
#include "Constantes.h"

#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>


#include <iostream>
#include <sstream>

#define CONEXION_RQ 1

#define EJECUTABLE_ENTRADA "./server_entrada"
#define EJECUTABLE_SALIDA "./server_salida"
#define EJECUTABLE_GET_SHMEM "./shmem_get"
#define EJECUTABLE_RET_SHMEM "./shmem_return"
#define EJECUTABLE_BROADCAST "./broadcast"
#define EJECUTABLE_BROADCAST_PERS "./broadcast_personas"

#define SEMAFORO_SH_MEM ".semaforo_shmem"

int lanzarServerEntrada();
int lanzarServerSalida();
int lanzarGetShMem();
int lanzarReturnShMem();
int lanzarBroadcast();
int lanzarBroadcastPersonas();

int main(int argc, char* argv[]){

	Semaforo* sem = new Semaforo(SEMAFORO_SH_MEM,0,0);
	
	//Crear archivo de procesos
	int fd = open(BROKER_PIDS_FILE,O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	// Lanzar procesos de SH MEM
	long pid_shmem = lanzarGetShMem();
	long pid_shmem2 = lanzarReturnShMem();

	//lanzar broadcasts
	long pid_broadcast = lanzarBroadcast();
	long pid_broadcast_personas = lanzarBroadcastPersonas();

	//Lanzar server entrada
	long pid_entrada = lanzarServerEntrada();
	//Lanzar proceso salida
	long pid_salida = lanzarServerSalida();
	
	int finish;
	std::cin >> finish;
	kill(pid_shmem,SIGINT);
	kill(pid_shmem2,SIGINT);   
	kill(pid_broadcast,SIGINT);   
	kill(pid_broadcast_personas,SIGINT);
	kill(pid_entrada,SIGINT);   
	kill(pid_salida,SIGINT);   
	return 0;
}

int lanzarServerEntrada(){
	int pidEntrada;
	if ((pidEntrada = fork()) < 0) {/* se crea el proceso hijo */
        	perror("Error en el fork");
                exit(1);
     	} else if (pidEntrada == 0) {
          	// PROCESO HIJO (child)
          	execlp(EJECUTABLE_ENTRADA, "Entrada", (char *)0);
                /* si sigue es que no se ejecuto correctamente el comando execlp */
                perror("Error al lanzar el programa Entrada Broker");
                exit(3);
        }
	return pidEntrada;
}

int lanzarServerSalida(){
	int pidSalida;
        if ((pidSalida = fork()) < 0) {/* se crea el proceso hijo */
                perror("Error en el fork");
                exit(1);
        } else if (pidSalida == 0) {
                // PROCESO HIJO (child)
                execlp(EJECUTABLE_SALIDA, "Salida", (char *)0);
                /* si sigue es que no se ejecuto correctamente el comando execlp */
                perror("Error al lanzar el programa Salida Broker");
                exit(3);
        }
	return pidSalida;
}

int lanzarGetShMem(){
	int pidSalida;
        if ((pidSalida = fork()) < 0) {/* se crea el proceso hijo */
                perror("Error en el fork");
                exit(1);
        } else if (pidSalida == 0) {
                // PROCESO HIJO (child)
                execlp(EJECUTABLE_GET_SHMEM, "Get SH MEM", (char *)0);
                /* si sigue es que no se ejecuto correctamente el comando execlp */
                perror("Error al lanzar el programa Get SH MEM");
                exit(3);
        }
	return pidSalida;
}

int lanzarReturnShMem(){
	int pidSalida;
        if ((pidSalida = fork()) < 0) {/* se crea el proceso hijo */
                perror("Error en el fork");
                exit(1);
        } else if (pidSalida == 0) {
                // PROCESO HIJO (child)
                execlp(EJECUTABLE_RET_SHMEM, "Return SH MEM", (char *)0);
                /* si sigue es que no se ejecuto correctamente el comando execlp */
                perror("Error al lanzar el programa Return SH MEM");
                exit(3);
        }
	return pidSalida;
}

int lanzarBroadcast(){
	int pidSalida;
	if((pidSalida = fork()) <0){
		perror("Error en el fork");
		exit(1);
	}else if(pidSalida==0){
		execlp(EJECUTABLE_BROADCAST, "Broadcast puertas", (char*)0);
		perror("Error al lanzar el programa Broadcast puertas");
		exit(3);
	}
	return pidSalida;
}

int lanzarBroadcastPersonas(){
	int pidSalida;
	if((pidSalida = fork()) <0){
		perror("Error en el fork");
		exit(1);
	}else if(pidSalida==0){
		execlp(EJECUTABLE_BROADCAST_PERS, "Broadcast personas", (char*)0);
		perror("Error al lanzar el programa Broadcast personas");
		exit(3);
	}
	return pidSalida;
}
