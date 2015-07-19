#include <iostream>
#include <IPC/Cola.h>
#include <Logger/Logger.h>
#include <ipuerta/IPuertaMsg.h>

using namespace std;

static const std::string ARCHIVO_COLA="/etc/hosts";
static const char LETRA_COLA='A';

int main(int argc, char ** argv) {
	LOG("LANZADOR: Iniciando IPC Cola");
	Cola<IPuertaMsg> colaMsg(ARCHIVO_COLA, LETRA_COLA);
	LOG("LANZADOR: Cola inicializada");
return 0;
}
