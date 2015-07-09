#include "ArgParser.h"

#include <cassert>
#include <iostream>
#include "ipc-keys.h"
#include <IPC/SIGINT_Handler.h>
#include <IPC/SignalHandler.h>
#include "IPCManager.h"
#include <ipersona/IPersona.h>
#include <stdexcept>
#include <string>
#include <utils/EnvParam.h>
#include <utils/System.h>

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

void run_loop (IPCManager& ipcman, SIGINT_Handler& intHandler);

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	SIGINT_Handler intHandler;
	SignalHandler* sigs = SignalHandler::getInstance ();
	sigs->registrarHandler (SIGINT, &intHandler);

	std::string pathColas = calcularPathColas (args);
	std::string pathInterfaz = calcularPathInterfaz (args);

	IPCManager ipcman (pathInterfaz, pathColas);
	ipcman.inicializar ();

	// TODO: lanzar lector de mensajes desde el broker
	pid_t lector = -1;

	// TODO: socket para responder al broker...

	int err = 0;
	try {
		run_loop (ipcman, intHandler);
		if (lector != -1) {
			kill (lector, SIGINT);
		}
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what () << std::endl;
		err = 1;
	}

	ipcman.destruir ();
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
			// TODO: qué hago acá? interrumpido u otro error?
			throw e;
		}
	}
}
