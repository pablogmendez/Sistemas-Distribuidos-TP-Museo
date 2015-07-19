#include <iostream>
#include <IPC/Cola.h>
#include <Logger/Logger.h>
#include <ipuerta/IPuertaMsg.h>

using namespace std;

static const std::string ARCHIVO_COLA="/etc/hosts";
static const char LETRA_COLA='A';

int main(int argc, char ** argv) {
	LOG("LANZADOR: Destruyendo IPC Cola");
	Cola<IPuertaMsg> colaMsg(ARCHIVO_COLA, LETRA_COLA);
	colaMsg.destruir();
	LOG("LANZADOR: Cola destruida");
return 0;
}
