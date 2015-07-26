#include "ArgParser.h"
#include <imuseo/IMuseo.h>
#include <IPC/SIGINT_Handler.h>
#include <IPC/SignalHandler.h>
#include <ipersona/IPersona.h>
#include <iostream>
#include <libgen.h>
#include "LockerRack.h"
#include <Logger/Logger.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <utils/System.h>

std::string calcularRutaComp (const char* arg0)
{
	char* tmp = strdup (arg0);
	char* dir = dirname (tmp);
	std::string ruta = dir;
	ruta.append("/../ipersona_comp/ipersona_comp");
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

int write_id_local (const std::string& file, int id)
{
	int err;
	int fd = -1;
	try {
		std::ostringstream oss;
		std::string sid;

		err = fd = open (file.c_str (), O_CREAT | O_WRONLY | O_APPEND, 0664);
		System::check (err, "[open]");
		oss << id << ':' << getpid () << '\n';
		sid = oss.str ();
		ssize_t errw = write (fd, sid.c_str (), sid.size ());
		System::check (errw, "[write]");
		err = 0;
	} catch (SystemErrorException& e) {
		std::cout << "error al escribir id local: " << e.what () << std::endl;
	}
	if (fd != -1) {
		close (fd);
	}
	return err;
}

#define LOG_PUERTA(fmt, ...) \
	LOG("PUERTA [%d] - " fmt, getpid (),##__VA_ARGS__)

int main (int argc, char** argv) try
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	SIGINT_Handler intHandler;
	SignalHandler* sigs = SignalHandler::getInstance ();
	sigs->registrarHandler (SIGINT, &intHandler);

	LOG_PUERTA("Iniciando puerta con id local %d.", args.idLocal ());

	// TODO: agregar parámetro para usar interfaz con defaults
	// o con esta configuración especial.
	configurar_para_correr_en_arbol_fuentes (argv[0]);

	// TODO: agregar parámetro para determinar el numero de
	// lockers en esta puerta.
	const int NUM_LOCKERS = 100;
	LOG_PUERTA ("Creando %d lockers...", NUM_LOCKERS);
	LockerRack rack (NUM_LOCKERS);

	LOG_PUERTA ("Creando interfaz IPersona...");
	IPersona ipersona (args.idLocal ());

	LOG_PUERTA ("Creando interfaz IMuseo...");
	IMuseo imuseo;

	LOG_PUERTA ("Escribiendo id local en archivo de sesion.");
	write_id_local (args.sesion (), args.idLocal ());

	LOG_PUERTA ("Iniciando ciclo principal...");

	int capacidad = 2;
	int personas = 0;

	while (true) {
		Operacion op = ipersona.leerProximaOperacion ();
		switch (op.tipo) {
			case SOLIC_ENTRAR_MUSEO_PERSONA:
			{
				LOG_PUERTA ("Persona %ld solicita entrar al museo.",
						op.op.semp.idOrigen);

				Entrada::Resultado res = imuseo.entrarPersona ();

				LOG_PUERTA ("Se recibio resultado del museo: %d (%s)",
					res, Entrada::strResultado (res));

				if (res == Entrada::ENTRO) {
					ipersona.notificarEntrada (op, ENTRO);
				} else if (res == Entrada::LLENO) {
					LOG_PUERTA ("Museo lleno.");
					set_lleno (op, true);
					ipersona.notificarEntrada (op,
						static_cast<ResultadoOperacionEntrada> (0));
				} else { /* res == Entrada::CERRADO */
					LOG_PUERTA ("Museo cerrado.");
					ipersona.notificarEntrada (op, CERRADO);
				}
				break;
			}
			case SOLIC_ENTRAR_MUSEO_INVESTIGADOR:
			{
				long idOrigen = op.op.semi.idOrigen;
				long pertenencias = op.op.semi.pertenencias;

				LOG_PUERTA ("Investigador %ld solicita entrar al museo"
						    " con pertenencias %ld.", idOrigen,
						    pertenencias);

				long locker = rack.guardar (idOrigen, pertenencias);

				if (locker == LockerRack::NO_HAY_LUGAR) {
					LOG_PUERTA ("No hay lockers...");
					ipersona.notificarEntrada (op, NO_HAY_LOCKER, 0);
					break;
				}

				Entrada::Resultado res = imuseo.entrarPersona ();

				LOG_PUERTA ("Se recibio resultado del museo: %d (%s)",
					res, Entrada::strResultado (res));

				if (res == Entrada::ENTRO) {
					ipersona.notificarEntrada (op, ENTRO, locker);
				} else if (res == Entrada::CERRADO) {
					rack.retirar (locker);
					ipersona.notificarEntrada (op, CERRADO, 0);
				} else { /* res == Entrada::LLENO */
					LOG_PUERTA ("Museo lleno");
					rack.retirar (locker);
					set_lleno (op, true);
					ipersona.notificarEntrada (op,
						static_cast<ResultadoOperacionEntrada> (0), 0);
				}
				break;
			}
			case SOLIC_SALIR_MUSEO_PERSONA:
			{
				LOG_PUERTA ("La persona %ld solicita salir del museo.",
						op.op.ssmp.idOrigen);
				Salida::Resultado res = imuseo.sacarPersona ();

				LOG_PUERTA ("Se recibio resultado del museo: %d (%s)",
					res, Salida::strResultado (res));

				set_lleno (op, res == Salida::SALIO_Y_LLENO);
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
				} else {
					Salida::Resultado res = imuseo.sacarPersona ();
					LOG_PUERTA ("Se recibio resultado del museo: %d (%s)",
						res, Salida::strResultado (res));
					set_lleno (op, res == Salida::SALIO_Y_LLENO);
					Locker locker = rack.retirar (lockerId);
					ipersona.notificarSalida (op, SALIO, locker.pertenencias);
				}
				break;
			}
			case NOTIFICAR_CIERRE_MUSEO:
			{
				LOG_PUERTA ("El administrador inició el cierre del museo...");
				// Acá se reinicializan los lockers para que no queden en
				// uso...
				rack.clear ();
				break;
			}
			default:
				LOG_PUERTA ("Recibí operación erronea: tipo=%d.", op.tipo);
				return 1;
		}
	}
	return 0;
} catch (SystemErrorException& e) {
	LOG_PUERTA ("Error (%d): %s", e.number (), e.what ());
	SignalHandler::getInstance ()->destruir ();
	if (e.number () == EINTR) {
		return 0;
	}
	return 1;
} catch (std::exception& e) {
	LOG_PUERTA ("Error: %s", e.what ());
	SignalHandler::getInstance ()->destruir ();
	return 1;
}
