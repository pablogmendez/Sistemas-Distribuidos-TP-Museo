#ifndef LANZADOR_H_
#define LANZADOR_H_

#include <iostream>
#include "../IPC/MemoriaCompartida.h"
#include "../IPC/Semaforo.h"
#include "../IPC/Cola.h"
#include "Constantes.h"
#include <queue>
#include <fstream>

using std::queue;

class Lanzador {
private:
	MemoriaCompartida<bool> shmMuseoAbierto;
	MemoriaCompartida<unsigned> shmContadorMuseo;
	Semaforo *mtxContMuseo;
	Semaforo *mtxMuseoAbierto;
	Cola<persona_t> *colaNorm, *colaEsp;
	queue<Semaforo*> colaMtx;
	queue<MemoriaCompartida<int>*> colaShm;

	int crearPersonas(int aCrear, int creadas, int tipo, std::ofstream &ofs);

public:

	Lanzador();
	
	~Lanzador();

	int tomarIPC();

	int liberarIPC();
	
	int iniciarProcesos();
};


#endif /* LANZADOR_H_ */
