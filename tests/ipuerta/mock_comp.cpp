#include <ipuerta/IPuertaServ.h>
#include <iostream>
#include <unistd.h>
#include <utils/System.h>

int main (int argc, char** argv)
{
	std::cout << "Iniciando componente..." << std::endl;

	IPuertaServ ipuerta;

	while (true) {
		// Este componente solo espera el tiempo solicitado
		// Y confirma la operacion.
		Operacion op = ipuerta.leerProximaOperacion ();
		System::millisleep ((unsigned long)op.getTiempoRecorrido());
		ipuerta.responderOperacion(op);
	}

	/* not reached */
	return 0;
}
