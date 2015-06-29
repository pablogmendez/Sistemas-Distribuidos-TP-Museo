#include "ConexionMsg.h"
#include "MensajeGenerico.h"

#include "../IPC/Cola.h"

#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>

#define COLA_MAESTRA ".colamaestra.mq"
#define COLA_CONEXIONES ".colaconexiones.mq"

#define CONEXION_RQ 1

#define EJECUTABLE_ENTRADA "./broker_entrada"
#define EJECUTABLE_SALIDA "./broker_salida"

int lanzarEntrada(int);
int lanzarSalida(int);


int main(int argc, char* argv[]){
	// Cola para recibir conexiones, cambiar por sockets
	Cola<ConexionMsg>* conexiones = new Cola<ConexionMsg>(COLA_CONEXIONES,0);
	
	// Cola maestra del broker
	Cola<MensajeGenerico>* colaMaestra =  new Cola<MensajeGenerico>(COLA_MAESTRA,0);

	while(1){
		ConexionMsg dato;
		std::cout << "Leo de la cola de conexiones";
		std::cout.flush();
		conexiones->leer(CONEXION_RQ,&dato);
		
		int cola = dato.cola;

		//Lanzar proceso entrada
		lanzarEntrada(cola);
		//Lanzar proceso salida
		lanzarSalida(cola);
	}
	
	return 0;
}

int lanzarEntrada(int cola){
	std::stringstream ss;
        ss << cola;
	int pidEntrada;
	if ((pidEntrada = fork()) < 0) {/* se crea el proceso hijo */
        	perror("Error en el fork");
                exit(1);
     	} else if (pidEntrada == 0) {
          	// PROCESO HIJO (child)
          	execlp(EJECUTABLE_ENTRADA, "Entrada", ss.str().c_str(), (char *)0);
                /* si sigue es que no se ejecuto correctamente el comando execlp */
                perror("Error al lanzar el programa Entrada Broker");
                exit(3);
        }
}

int lanzarSalida(int cola){
	std::stringstream ss;
        ss << cola;
	int pidSalida;
        if ((pidSalida = fork()) < 0) {/* se crea el proceso hijo */
                perror("Error en el fork");
                exit(1);
        } else if (pidSalida == 0) {
                // PROCESO HIJO (child)
                execlp(EJECUTABLE_SALIDA, "Salida", ss.str().c_str(), (char *)0);
                /* si sigue es que no se ejecuto correctamente el comando execlp */
                perror("Error al lanzar el programa Salida Broker");
                exit(3);
        }

}
