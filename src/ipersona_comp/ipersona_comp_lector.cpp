#include "ArgParserLector.h"
#include "ipc-keys.h"
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

	std::string pathColas = calcularPathColas (args);
	IPCManager ipcman (args.mqInterfaz (), pathColas);

	// TODO: crear socket hacia el broker...
	// TODO: enviar id de puerta al broker

	while (1) {
		// TODO: leer paquete del broker
		// TODO: convertir paquete a IPersonaMsg
		IPersonaMsg msg;
		// TODO: colocar paquete en cola adecuada
	}
}
