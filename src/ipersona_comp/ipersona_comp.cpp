#include "ArgParser.h"

#include <cassert>
#include <id-server/IIdClient.h>
#include <iostream>
#include "ipc-keys.h"
#include <IPC/SIGINT_Handler.h>
#include <IPC/SignalHandler.h>
#include "IPCManager.h"
#include <ipersona/IPersona.h>
#include <libgen.h>
#include <Logger/Logger.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string.h>
#include <unistd.h>
#include <utils/EnvParam.h>
#include <utils/System.h>

#define LOG_IPCMP(fmt, ...) \
	LOG("IPERSONA_COMP [%d] - " fmt, getpid (),##__VA_ARGS__)

std::string calcularPathColas (ArgParser& args)
{
	std::string path = args.recursos ();
	path.append (PATH_MQS);
	return path;
}

std::string calcularPathInterfaz (ArgParser& args)
{
	std::string dfltPath = args.recursos ();
	dfltPath.append (PATH_INT);
	EnvParam param (IPersona::ENV_IPERSONA_MQ, dfltPath);
	return param.get ();
}

std::string calcularPathLector (const char* arg0)
{
	char* tmp = strdup (arg0);
	std::string ruta = dirname (tmp);
	ruta.append ("/ipersona_comp_lector");
	free (tmp);
	return ruta;
}

void run_loop (IPCManager& ipcman, SIGINT_Handler& intHandler);

class IPCManagerCleaner
{
		IPCManager ipcman;
public:
		IPCManagerCleaner (
				int baseId,
				const std::string& pathInt,
				const std::string& pathColas)
			: ipcman (baseId, pathInt, pathColas) {}
		~IPCManagerCleaner ()
		{
			ipcman.destruir ();
		}

		IPCManager& get () { return ipcman; }
};

class IdGrabber
{
		IIdClient& idc;
		long id;
public:
		IdGrabber (IIdClient& idc, IIdClient::Rangos tipo)
			: idc (idc)
			, id (idc.obtenerId (tipo)) {}

		~IdGrabber ()
		{
			idc.devolverId (id);
		}

		long get () { return id; }
};

int main (int argc, char** argv)
{
	int err = 0;
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	LOG_IPCMP("Argumentos:\n"
			  "\tID_SERVER   : %s\n"
			  "\tBROKER      : %s\n"
			  "\tDIR_RECURSOS: %s",
			  args.idServer ().c_str (),
			  args.broker ().c_str (),
			  args.recursos ().c_str ());

	SIGINT_Handler intHandler;
	SignalHandler* sigs = SignalHandler::getInstance ();
	sigs->registrarHandler (SIGINT, &intHandler);

	try {
		std::string pathColas = calcularPathColas (args);
		std::string pathInterfaz = calcularPathInterfaz (args);

		{
			char wd[1024];
			char *wd_err = getcwd (wd, sizeof wd);
			if (wd_err == NULL) {
				strcpy (wd, "[error al obtener directorio de trabajo]");
			}
			LOG_IPCMP("Conectando a recursos IPC:\n"
					"\tCWD          : %s\n"
					"\tPATH_INTERFAZ: %s\n"
					"\tPATH_COLAS   : %s",
					wd,
					pathInterfaz.c_str (),
					pathColas.c_str ());
		}

		IPCManagerCleaner ipcmc (args.idLocal (), pathInterfaz, pathColas);
		IPCManager& ipcman = ipcmc.get ();
		ipcman.inicializar ();

		IIdClient idClient (args.idServer ().c_str ());
		IdGrabber idg (idClient, IIdClient::R_PUERTA);
		long idPuerta = idg.get ();

		LOG_IPCMP("Se obtuvo el identificador %ld", idPuerta);

		pid_t lector = -1;
		{
			std::ostringstream oss;
			oss << idPuerta;
			std::string strIdPuerta = oss.str ();

			oss.str ("");
			oss << "--id-local=" << args.idLocal ();
			std::string strIdLocal = oss.str ();

			std::string path_lector = calcularPathLector (argv[0]);
			std::vector<const char*> args_lector;
			args_lector.push_back (path_lector.c_str ());
			args_lector.push_back (strIdLocal.c_str ());
			args_lector.push_back (pathInterfaz.c_str ());
			args_lector.push_back (args.broker ().c_str ());
			args_lector.push_back (args.recursos ().c_str ());
			args_lector.push_back (strIdPuerta.c_str ());
			args_lector.push_back (NULL);
			lector = System::spawn (path_lector.c_str (), args_lector);
			if (lector == -1) {
				SystemErrorException e;
				throw e;
			}
		}

		// TODO: socket para responder al broker...

		run_loop (ipcman, intHandler);
		if (lector != -1) {
			kill (lector, SIGINT);
		}
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what () << std::endl;
		err = 1;
	}

	return err;
}

void run_loop (IPCManager& ipcman, SIGINT_Handler& intHandler)
{
	long myMTYPE = getpid ();
	while (intHandler.getGracefulQuit () != 1) {
		try {
			int err;
			IPersonaMsg msgInt;
			IPersonaMsg msgOp;

			// Se espera a que la aplicación solicite una operación.
			err = ipcman.interfaz->leer (myMTYPE, &msgInt);
			System::check (err);
			// TODO: validar el mensaje recibido...
			assert (msgInt.op == SOLIC_PROXIMA_OPERACION);

			// Ahora se espera a que arribe una operación desde el broker
			msgOp = ipcman.leerOperacionConPrioridad ();
			// TODO: validar mensaje de operacion...
			switch (msgOp.op) {
				case OP_SOLIC_ENTRAR_MUSEO_PERSONA:
				case OP_SOLIC_ENTRAR_MUSEO_INVESTIGADOR:
				case OP_SOLIC_SALIR_MUSEO_PERSONA:
				case OP_SOLIC_SALIR_MUSEO_INVESTIGADOR:
				case OP_NOTIFICAR_CIERRE_MUSEO:
					break;
				default:
					assert (false);
			}

			// Se devuelve la operación a la interfaz
			msgOp.mtype = msgInt.msg.spo.rtype;
			err = ipcman.interfaz->escribir (msgOp);
			System::check (err);

			// Luego se aguarda su respuesta
			err = ipcman.interfaz->leer (myMTYPE, &msgInt);
			System::check (err);
			// TODO: validar el mensaje recibido...
			switch (msgInt.op) {
				case NOTIF_ENTRADA_PERSONA:
				case NOTIF_ENTRADA_INVESTIGADOR:
				case NOTIF_SALIDA_PERSONA:
				case NOTIF_SALIDA_INVESTIGADOR:
					break;
				default:
					assert (false);
			}

			// TODO: convertir mensaje y meterlo en el socket hacia el
			// broker.
		} catch (SystemErrorException& e) {
			std::cerr << "Error (" << e.number ()
					  << "): " << e.what () << std::endl;
			if (e.number () == EINTR) {
				// volver al inicio a chequear la bandera
				continue;
			}
			// TODO: qué hago acá?
			throw e;
		}
	}
}
