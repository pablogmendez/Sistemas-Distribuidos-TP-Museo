#include "Museo/Administrador.h"
#include <cstdio>
#include <cstring>
#include "Logger/Logger.h"
#include "Utils/Utils.h"

int main(int argc, char **argv) {
Administrador administrador;
int retval = 0;

LOG("ADMINISTRADOR: INICIANDO");
LOG("ADMINISTRADOR: Opcion seleccionada: %s", argv[1]);


if(argc == 1) {
	LOG("ADMINISTRADOR: Cantidad de parametros invalida.");
	LOG("ADMINISTRADOR: a --> Abrir Museo");
	LOG("ADMINISTRADOR: c --> Cerrar Museo");
	retval = 1;
} else if(strcmp(argv[1], "a") == 0) {
	LOG("ADMINISTRADOR: Abrir Museo");
	administrador.abrirMuseo();
} else if(strcmp(argv[1], "c") == 0) {
	LOG("ADMINISTRADOR: Cerrar Museo");
	administrador.cerrarMuseo();
} else {
	LOG("ADMINISTRADOR: Opcion invalida. Las opciones posibles son:");
	LOG("ADMINISTRADOR: a --> Abrir Museo");
	LOG("ADMINISTRADOR: c --> Cerrar Museo");
	retval = 1;
}
LOG("ADMINISTRADOR: FINALIZADO");
return retval;
}
