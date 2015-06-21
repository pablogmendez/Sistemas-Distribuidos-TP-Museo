#include <ipuerta/IPuertaCli.h>
#include <iostream>
#include <stdlib.h>

int main (int argc, char** argv)
{
	/* Establece la variable de entrono para que se utilice el
	 * componente provisto por este test. */
	setenv(IPuertaCli::ENV_IPUERTA_COMP, "./mock_comp", 1);

	std::cout << "Iniciando recorrido simple." << std::endl;

	IPuertaCli ipuerta;
	ipuerta.recorrerMuseo(1, 1, 1000);

	std::cout << "Finalizado recorrido simple." << std::endl;
}
