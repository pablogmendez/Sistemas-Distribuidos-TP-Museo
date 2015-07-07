#include <ipersona/IPersona.h>
#include <iostream>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>

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
	ruta.append("/ipersona.mq");
	free (tmp);
	return ruta;
}

int main (int argc, char** argv)
{
	/* Establece la variable de entrono para que se utilice el
	 * componente provisto por este test. */
	setenv(IPersona::ENV_IPERSONA_COMP,
			calcularRutaComp (argv[0]).c_str (), 1);
	/* Establece la variable de entrono para que se pueda encontrar
	 * el recurso para crear la cola que comunica interfaz y
	 * componente. */
	setenv(IPersona::ENV_IPERSONA_MQ,
			calcularRutaMQ (argv[0]).c_str (), 1);

	std::cout << "Iniciando prueba de IPersona." << std::endl;

	IPersona ipersona;

	// Lo que sigue es basicamente el loop principal
	// de los procesos puerta.

	Operacion op = ipersona.leerProximaOperacion ();
	while (op.tipo != NOTIFICAR_CIERRE_MUSEO) {
		std::cout << "Recibí operacion de tipo "
				<< op.tipo << std::endl;

		switch (op.tipo) {
		case SOLIC_ENTRAR_MUSEO_PERSONA:
		{
			std::cout << "Persona "
				<< op.op.semp.idOrigen << " solicita entrar al museo"
				<< std::endl;
			// Acá la puerta se comunicaría con el museo
			// Luego notifica que entró
			ipersona.notificarEntrada (op, ENTRO);
			break;
		}
		case SOLIC_ENTRAR_MUSEO_INVESTIGADOR:
		{
			long locker = op.op.semi.pertenencias;
			std::cout << "Investigador "
					<< op.op.semi.idOrigen << " solicita entrar al museo"
					<< " con pertenencias " << locker
					<< std::endl;
			// Acá la puerta se comunicaría con el museo
			// Luego notifica que entró, pasando el locker asignado
			ipersona.notificarEntrada (op, ENTRO, locker);
			break;
		}
		case SOLIC_SALIR_MUSEO_PERSONA:
		{
			std::cout << "Persona "
					<< op.op.ssmp.idOrigen << " solicita salir del museo"
					<< std::endl;
			// Acá la puerta se comunicaría con el museo
			// Luego notifica que la persona salió
			ipersona.notificarSalida (op, SALIO);
			break;
		}
		case SOLIC_SALIR_MUSEO_INVESTIGADOR:
		{
			long pert = op.op.ssmi.numeroLocker;
			std::cout << "Investigador "
					<< op.op.ssmi.idOrigen << " solicita salir del museo"
					<< " con locker " << pert
					<< std::endl;
			// Acá la puerta se comunicaría con el museo
			// Luego notifica que la persona salió, pasando las
			// pertenencias.
			ipersona.notificarSalida (op, SALIO, pert);
			break;
		}
		default:
			std::cerr << "Recibí operacion erronea "
				<< op.tipo << std::endl;
			return 1;
		}

		op = ipersona.leerProximaOperacion ();
	}

	return 0;
}
