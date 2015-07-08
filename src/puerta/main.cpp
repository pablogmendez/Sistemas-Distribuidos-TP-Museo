#include <ipersona/IPersona.h>
#include <iostream>
#include <libgen.h>
#include "LockerRack.h"
#include <Logger/Logger.h>
#include <stdlib.h>
#include <string.h>

std::string calcularRutaComp (const char* arg0)
{
	char* tmp = strdup (arg0);
	char* dir = dirname (tmp);
	std::string ruta = dir;
	ruta.append("/../ipersona/ipersona_comp");
	free (tmp);
	return ruta;
}

std::string calcularRutaMQ (const char* arg0)
{
	char* tmp = strdup (arg0);
	char* dir = dirname (tmp);
	std::string ruta = dir;
	ruta.append("/../../resources/ipersona.mq");
	free (tmp);
	return ruta;
}

void configurar_para_correr_en_arbol_fuentes (const char* arg0)
{
	std::string comp = calcularRutaComp (arg0);
	std::string mq = calcularRutaMQ (arg0);

	/* Establece la variable de entrono para que se utilice el
	 * componente provisto por este test. */
	setenv(IPersona::ENV_IPERSONA_COMP, comp.c_str (), 1);
	/* Establece la variable de entrono para que se pueda encontrar
	 * el recurso para crear la cola que comunica interfaz y
	 * componente. */
	setenv(IPersona::ENV_IPERSONA_MQ, mq.c_str (), 1);
}

#define LOG_PUERTA(fmt, ...) \
	LOG("PUERTA [%d] - " fmt, getpid (),##__VA_ARGS__)

int main (int argc, char** argv)
{
	LOG_PUERTA("Iniciando puerta...");

	// TODO: agregar parámetro para usar interfaz con defaults
	// o con esta configuración especial.
	configurar_para_correr_en_arbol_fuentes (argv[0]);

	// TODO: agregar parámetro para determinar el numero de
	// lockers en esta puerta.
	const int NUM_LOCKERS = 100;
	LOG_PUERTA ("Creando %d lockers...", NUM_LOCKERS);
	LockerRack rack (NUM_LOCKERS);

	LOG_PUERTA ("Creando interfaz...");
	IPersona ipersona;

	LOG_PUERTA ("Iniciando ciclo principal...");

	Operacion op = ipersona.leerProximaOperacion ();
	while (op.tipo != NOTIFICAR_CIERRE_MUSEO) {
		switch (op.tipo) {
			case SOLIC_ENTRAR_MUSEO_PERSONA:
			{
				LOG_PUERTA ("Persona %ld solicita entrar al museo.",
						op.op.semp.idOrigen);

				// TODO: Usar interfaz IMuseo para contabilizar la
				// entrada y verificar si hay lugar.
				// Por ahora se deja entrar a cualquiera...

				ipersona.notificarEntrada (op, ENTRO);
				break;
			}
			case SOLIC_ENTRAR_MUSEO_INVESTIGADOR:
			{
				long idOrigen = op.op.semi.idOrigen;
				long pertenencias = op.op.semi.pertenencias;

				LOG_PUERTA ("Investigador %ld solicita entrar al museo"
						    " con pertenencias %ld.", idOrigen,
						    pertenencias);

				// TODO: Usar interfaz IMuseo para contabilizar la
				// entrada y verificar si hay lugar.
				long locker = rack.guardar (idOrigen, pertenencias);

				if (locker == LockerRack::NO_HAY_LUGAR) {
					ipersona.notificarEntrada (op, NO_HAY_LOCKER, 0);
				} else {
					ipersona.notificarEntrada (op, ENTRO, locker);
				}
				break;
			}
			case SOLIC_SALIR_MUSEO_PERSONA:
			{
				LOG_PUERTA ("La persona %ld solicita salir del museo.",
						op.op.ssmp.idOrigen);

				// TODO: Usar interfaz IMuseo para contabilizar la
				// salida de la persona.

				ipersona.notificarSalida (op, SALIO);
				break;
			}
			case SOLIC_SALIR_MUSEO_INVESTIGADOR:
			{
				long idOrigen = op.op.ssmi.idOrigen;
				long lockerId = op.op.ssmi.numeroLocker;

				LOG_PUERTA ("Investigador %ld solicita salir del museo"
						    " con locker %ld.", idOrigen, lockerId);

				long duenio = rack.duenio (lockerId);
				if (duenio == LockerRack::LOCKER_LIBRE
						|| duenio != idOrigen) {
					LOG_PUERTA ("El investigador no entró por esta puerta."
							    " Dueño actual: %ld.", duenio);
					ipersona.notificarSalida (op, PUERTA_INCORRECTA, 0);
				}

				// TODO: Usar interfaz IMuseo para contabilizar la
				// salida de la persona.
				Locker locker = rack.retirar (lockerId);
				ipersona.notificarSalida (op, SALIO, locker.pertenencias);
				break;
			}
			default:
				LOG_PUERTA ("Recibí operación erronea: tipo=%d.", op.tipo);
				return 1;
		}

		op = ipersona.leerProximaOperacion ();
	}

	LOG_PUERTA ("El administrador inició el cierre del museo...");
	// FIXME: echar a la gente y esperar que salgan...
	return 0;
}
