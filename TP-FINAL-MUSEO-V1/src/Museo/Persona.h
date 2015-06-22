#ifndef PERSONA_H_
#define PERSONA_H_

#include <iostream>
#include "../IPC/Cola.h"
#include "../IPC/SIGINT_Handler.h"
#include "../IPC/Semaforo.h"
#include "../IPC/MemoriaCompartida.h"
#include "Constantes.h"

using std::string;

class Persona {
private:
	Cola<persona_t> *colaEsp, *colaNorm;

	Semaforo *mtxVisitante, *mtxPuerta, *mtxCont;
	
	MemoriaCompartida<int> *shmCont;
	
	int numero;

protected:

	void iniciarIPC(int tipo, int puertaIngreso, int puertaEgreso);

	void liberarIPC();
	
	void iniciar(int tipo, int puertaIngreso, int puertaEgreso);

public:
	Persona();
	
	virtual ~Persona();
	
	virtual void iniciarRecorrido() = 0;
};

#endif /* PERSONA_H_ */
