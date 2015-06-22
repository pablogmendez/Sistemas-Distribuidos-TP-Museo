#ifndef DESTRUCTOR_H_
#define DESTRUCTOR_H_

#include <iostream>
#include "../IPC/MemoriaCompartida.h"
#include "../IPC/Semaforo.h"
#include "../IPC/Cola.h"
#include "../Logger/Logger.h"
#include "Constantes.h"
#include <queue>

using std::queue;

class Destructor {
private:
	MemoriaCompartida<bool> shmMuseoAbierto;
	MemoriaCompartida<unsigned> shmContadorMuseo;
	Semaforo *mtxContMuseo;
	Semaforo *mtxMuseoAbierto;
	Cola<persona_t> *colaNorm, *colaEsp;
	queue<Semaforo*> colaMtx;
	queue<MemoriaCompartida<int>*> colaShm;


public:

	Destructor();
	
	~Destructor();
	
	int liberarIPC();
	
	int tomarIPC();

	int pararProcesos();

};


#endif /* DESTRUCTOR_H_ */
