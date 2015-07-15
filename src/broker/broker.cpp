#include "ConexionMsg.h"
#include "MensajeGenerico.h"

#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <sstream>

#define CONEXION_RQ 1

#define EJECUTABLE_ENTRADA "./server_entrada"
#define EJECUTABLE_SALIDA "./server_salida"

int lanzarServerEntrada();
int lanzarServerSalida();


int main(int argc, char* argv[]){

	//Lanzar server entrada
	lanzarServerEntrada();
	//Lanzar proceso salida
	lanzarServerSalida();
	
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

}
