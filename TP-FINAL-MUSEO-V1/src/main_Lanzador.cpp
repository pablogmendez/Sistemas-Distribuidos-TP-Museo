#include "Museo/Lanzador.h"
#include "Logger/Logger.h"

int main(int argc, char **argv) {
Lanzador lanzador;

LOG("LANZADOR: INICIANDO");

lanzador.tomarIPC();
lanzador.iniciarProcesos();
lanzador.liberarIPC();

LOG("LANZADOR: FINALIZADO");
return 0;
}
