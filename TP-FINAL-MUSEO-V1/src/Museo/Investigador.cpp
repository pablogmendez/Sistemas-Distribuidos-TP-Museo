#include "Investigador.h"
#include "../Logger/Logger.h"
#include <unistd.h>
#include "../Utils/Utils.h"
#include "../Utils/ParserConfiguracion.h"

Investigador::Investigador() {
	// TODO Auto-generated constructor stub

}

Investigador::~Investigador() {
	// TODO Auto-generated destructor stub
}


void Investigador::iniciarRecorrido() {
	ParserConfiguracion parser;
	int puerta;
	string numeroPuerta;

	LOG("INVESTIGADOR %d: INICIANDO RECORRIDO", getpid());
	parser.cargarConfiguracion(configuracion);
	srand(time(NULL));
	puerta = (rand() % Utils::string2int(parser.getValor("puertas"))) + 1;
	numeroPuerta = Utils::int2String(puerta);
	LOG("INVESTIGADOR %d: PUERTA DE ENTRADA Y DE SALIDA: %d", getpid(), puerta);
	iniciar(tipoInvest, puerta, puerta);
	LOG("INVESTIGADOR %d: TERMINANDO RECORRIDO", getpid());
}
