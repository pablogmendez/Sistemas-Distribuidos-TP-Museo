#include "ArgParserLector.h"
#include "BrokerMsg.h"
#include "ipc-keys.h"
#include <IPC/SIGINT_Handler.h>
#include <IPC/SignalHandler.h>
#include "IPCManager.h"
#include <ipersona/IPersonaMsg.h>
#include <sockets/cClientSocket.h>
#include <stdint.h>
#include <utils/System.h>

std::string calcularPathColas (ArgParserLector& args)
{
	std::string path = args.recursos ();
	path.append (PATH_MQS);
	return path;
}

#define LOG_IPCL(fmt, ...) \
	LOG("IPERSONA_COMP_LECTOR [%d] - " fmt, getpid (),##__VA_ARGS__)

int main (int argc, char** argv)
{
	ArgParserLector& args = ArgParserLector::getInstance ();
	args.parse (argc, argv);

	if (args.fdBroker () == -1) {
		LOG_IPCL(
			"TODO: verificar si el lector podría abrir"
			" la conexión hacia el broker; ver ordenamiento"
			" de connects..");
		return 1;
	}

	LOG_IPCL("Estableciendo manejador de señal SIGINT...");

	SIGINT_Handler intHandler;
	SignalHandler* sigs = SignalHandler::getInstance ();
	sigs->registrarHandler (SIGINT, &intHandler);

	LOG_IPCL("Conectando a IPCs...");

	std::string pathColas = calcularPathColas (args);
	IPCManager ipcman (args.idLocal (), args.mqInterfaz (), pathColas);

	LOG_IPCL("Inicializando conexión con broker...");

	long idPuerta = args.idPuerta ();
	BrokerMsg brokerMsg;

	LOG_IPCL("Creando socket con mensaje de tamaño %d.",
			(sizeof brokerMsg));

	cClientSocket brokerConn (sizeof brokerMsg);
	brokerConn.tcp_adopt_connection (args.fdBroker ());

	brokerMsg.dstId = 0;
	brokerMsg.srcId = idPuerta;
	brokerMsg.op = BMO_BROKER_SET_ID;
	brokerMsg.param_a = idPuerta;
	brokerMsg.param_b = 0;

	LOG_IPCL("Paquete de inicialización:\n"
			"\tdstId  : %d\n"
			"\tsrcId  : %d\n"
			"\top     : %d\n"
			"\tparam_a: %d\n"
			"\tparam_b: %d",
			brokerMsg.dstId,
			brokerMsg.srcId,
			brokerMsg.op,
			brokerMsg.param_a,
			brokerMsg.param_b);

	brokerConn.tcp_send (reinterpret_cast<char*> (&brokerMsg));

	LOG_IPCL("Enviado. Aguardando operaciones...");

	while (intHandler.getGracefulQuit () == 0) {
		try {
			int err = brokerConn.tcp_recv (reinterpret_cast<char*> (&brokerMsg));
			System::check (err);

			IPersonaMsg msg;
			msg.op = static_cast<IPersonaOp> (brokerMsg.op);
			switch (brokerMsg.op) {
				case BMO_OP_SOLIC_ENTRAR_MUSEO_PERSONA:
					msg.msg.osemp.idOrigen = brokerMsg.srcId;
					break;
				case BMO_OP_SOLIC_ENTRAR_MUSEO_INVESTIGADOR:
					msg.msg.osemi.idOrigen = brokerMsg.srcId;
					msg.msg.osemi.pertenencias = brokerMsg.param_a;
					break;
				case BMO_OP_SOLIC_SALIR_MUSEO_PERSONA:
					msg.msg.ossmp.idOrigen = brokerMsg.srcId;
					break;
				case BMO_OP_SOLIC_SALIR_MUSEO_INVESTIGADOR:
					msg.msg.ossmi.idOrigen = brokerMsg.srcId;
					msg.msg.ossmi.numeroLocker = brokerMsg.param_a;
					break;
				case BMO_OP_NOTIFICAR_CIERRE_MUSEO:
					break;
				default:
					LOG_IPCL("Se recibió operación inválida: %d.",
							brokerMsg.op);
					continue;
			}

			ipcman.ponerOperacion (msg);
		} catch (std::exception& e) {
			LOG_IPCL("Error: %s.", e.what ());
			// TODO: log, eintr, etc
		}
	}

	sigs->destruir ();
}
