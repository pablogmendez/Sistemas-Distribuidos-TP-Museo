#include <ipuerta/IPuerta.h>
#include <iostream>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <utils/System.h>

std::string calcularRutaComp (const char* arg0)
{
	char* tmp = strdup (arg0);
	char* dir = dirname (tmp);
	std::string ruta = dir;
	ruta.append("/mock_comp");
	free (tmp);
	return ruta;
}

std::string calcularRutaMQ (const char* arg0)
{
	char* tmp = strdup (arg0);
	char* dir = dirname (tmp);
	std::string ruta = dir;
	ruta.append("/ipuerta.mq");
	free (tmp);
	return ruta;
}

int main (int argc, char** argv)
{
	/* Establece la variable de entrono para que se utilice el
	 * componente provisto por este test. */
	setenv(IPuerta::ENV_IPUERTA_COMP,
			calcularRutaComp (argv[0]).c_str (), 1);
	/* Establece la variable de entrono para que se pueda encontrar
	 * el recurso para crear la cola que comunica interfaz y
	 * componente. */
	setenv(IPuerta::ENV_IPUERTA_MQ,
			calcularRutaMQ (argv[0]).c_str (), 1);

	std::cout << "Iniciando recorrido simple." << std::endl;

	IPuerta ipuerta;
	std::cout << "Entrando por puerta 1." << std::endl;
	ipuerta.entrar (1);
	std::cout << "Esperando 1 segundo." << std::endl;
	System::millisleep (1000);
	std::cout << "Saliendo por puerta 2" << std::endl;
	ipuerta.salir (2);

	std::cout << "Finalizado recorrido simple." << std::endl;
}
