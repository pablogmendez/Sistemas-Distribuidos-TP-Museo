#include "../sockets/cServerSocket.h"

#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include <sstream>
#include <iostream>

#define EJECUTABLE_SALIDA "./broker_salida"

int lanzarSalida(int);

int main(int argc, char* argv[]){
	
	cServerSocket serverSocket;
	int cliSocket;
	serverSocket.tcp_open_pasivo(5001,5);
	while((cliSocket = serverSocket.tcp_accept()) != -1){
		std::cout << "ACEPTE CONEXION" << std::endl;
		lanzarSalida(cliSocket);
		close(cliSocket);
	}
	serverSocket.tcp_close();
}


int lanzarSalida(int socketCli){
        std::stringstream ss;
        ss << socketCli;
        int pidEntrada;
        if ((pidEntrada = fork()) < 0) {/* se crea el proceso hijo */
                perror("Error en el fork");
                exit(1);
        } else if (pidEntrada == 0) {
                // PROCESO HIJO (child)
                execlp(EJECUTABLE_SALIDA, "Salida", ss.str().c_str(), (char *)0);
                /* si sigue es que no se ejecuto correctamente el comando execlp */
                perror("Error al lanzar el programa Salida Broker");
                exit(3);
        }
}

