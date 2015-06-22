#include "Museo/Destructor.h"
#include "Logger/Logger.h"

int main(int argc, char **argv) {
Destructor destructor;

LOG("DESTRUCTOR: INICIANDO");
destructor.tomarIPC();
destructor.pararProcesos();
destructor.liberarIPC();
LOG("DESTRUCTOR: FINALIZADO");
return 0;
}
