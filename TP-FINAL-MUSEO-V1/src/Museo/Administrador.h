#ifndef ADMINSITRADOR_H_
#define ADMINSITRADOR_H_

#include <iostream>
#include "../IPC/MemoriaCompartida.h"
#include "../IPC/Semaforo.h"
#include "../IPC/SIGINT_Handler.h"

class Administrador {
private:
	MemoriaCompartida<bool> shmMuseoAbierto;
	Semaforo *mtxMuseoAbierto;
	
	int tomarIPC();
	
	int liberarIPC();
	
public:
	Administrador();
	
	~Administrador();

	void abrirMuseo();
	
	void cerrarMuseo();
};

#endif
