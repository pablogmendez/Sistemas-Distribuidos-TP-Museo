#include <cstdlib>
#include "Logger/Logger.h"
#include <unistd.h>
#include "Museo/PersonaNormal.h"
#include "Museo/Investigador.h"

int main(int argc, char **argv) {
int tipo = atoi(argv[1]);

if(tipo == 1) {
	LOG("PERSONA NORMAL: INICIANDO UNA NUEVA INSTANCIA CON PID:%d", getpid());
	PersonaNormal personaNormal;
	personaNormal.iniciarRecorrido();
}
else {
	LOG("INVESTIGADOR: INICIANDO UNA NUEVA INSTANCIA CON PID:%d", getpid());
	Investigador investigador;
	investigador.iniciarRecorrido();
}
return 0;
}
