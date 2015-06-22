#ifndef PUERTA_H_
#define PUERTA_H_

#include <iostream>
#include "../IPC/MemoriaCompartida.h"
#include "../IPC/Semaforo.h"
#include "../IPC/Cola.h"
#include "../IPC/SIGINT_Handler.h"
#include "../Utils/ParserConfiguracion.h"
#include "Constantes.h"
#include <queue>

using std::queue;
using std::string;

class Puerta {
private:
	MemoriaCompartida<bool> shmMuseoAbierto;
	MemoriaCompartida<unsigned> shmContMuseo;
	MemoriaCompartida<int> shmContNorm, shmContEsp;
	Semaforo *mtxContMuseo;
	Semaforo *mtxMuseoAbierto;
	Semaforo *mtxVisitante, *mtxPuerta;
	Semaforo *mtxContNorm, *mtxContEsp;
	SIGINT_Handler sigint_handler;
	Cola<persona_t> *colaNorm;
	Cola<persona_t> *colaEsp;

	ParserConfiguracion parserConfiguracion;
	
	int numero, pid;

	string strNumero;
		
	int capacidad;

	int tomarIPC();
	
	int liberarIpc();
	
	void vaciarMuseo();
	
	int entrarColaAux(queue<persona_t> &colaAuxiliar);

public:
	Puerta(int numero);
	
	~Puerta();
	
	void abrir();

};

#endif /* PUERTA_H_ */
