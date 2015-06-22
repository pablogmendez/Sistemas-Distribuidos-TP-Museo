#include "PersonaNormal.h"
#include "../Utils/ParserConfiguracion.h"
#include "../Utils/Utils.h"
#include <iostream>
#include <unistd.h>
#include "../Logger/Logger.h"

using std::string;

PersonaNormal::PersonaNormal() {
	// TODO Auto-generated constructor stub

}

PersonaNormal::~PersonaNormal() {
	// TODO Auto-generated destructor stub
}

void PersonaNormal::iniciarRecorrido() {
	ParserConfiguracion parser;
	int puertaIngreso, puertaEgreso;
	string numeroPuertaEntrada, numeroPuertaSalida;

	LOG("PERSONA NORMAL %d: INICIANDO RECORRIDO", getpid());
	parser.cargarConfiguracion(configuracion);
	srand(time(NULL));
	puertaIngreso = (rand() % Utils::string2int(parser.getValor("puertas"))) + 1;
	puertaEgreso = (rand() % Utils::string2int(parser.getValor("puertas"))) + 1;
	LOG("PERSONA NORMAL %d: PUERTA DE ENTRADA %d Y DE SALIDA %d", getpid(), puertaIngreso, puertaEgreso);
	numeroPuertaEntrada = Utils::int2String(puertaIngreso);
	numeroPuertaSalida = Utils::int2String(puertaEgreso);
	iniciar(tipoNormal, puertaIngreso, puertaEgreso);
	LOG("PERSONA NORMAL %d: TERMINANDO RECORRIDO", getpid());
}
