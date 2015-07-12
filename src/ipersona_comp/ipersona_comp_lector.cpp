#include "ArgParserLector.h"
#include "ipc-keys.h"
#include <IPC/SIGINT_Handler.h>
#include <IPC/SignalHandler.h>
#include "IPCManager.h"
#include <ipersona/IPersonaMsg.h>

std::string calcularPathColas (ArgParserLector& args)
{
	std::string path = args.recursos ();
	path.append (PATH_MQS);
	return path;
}

int main (int argc, char** argv)
{
	ArgParserLector& args = ArgParserLector::getInstance ();
	args.parse (argc, argv);

	SIGINT_Handler intHandler;
	SignalHandler* sigs = SignalHandler::getInstance ();
	sigs->registrarHandler (SIGINT, &intHandler);

	std::string pathColas = calcularPathColas (args);
	IPCManager ipcman (args.idLocal (), args.mqInterfaz (), pathColas);

	// TODO: crear socket hacia el broker...
	// TODO: enviar id de puerta al broker

	while (intHandler.getGracefulQuit () == 0) {
		try {
			// TODO: leer paquete del broker
			// TODO: convertir paquete a IPersonaMsg
			IPersonaMsg msg;
			// TODO: colocar paquete en cola adecuada
		} catch (std::exception& e) {
			// TODO: log, eintr, etc
		}
	}

	sigs->destruir ();
}
