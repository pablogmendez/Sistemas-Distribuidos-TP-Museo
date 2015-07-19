#include "ArgParserLector.h"
#include "BrokerMsg.h"
#include "ipc-keys.h"
#include <IPC/SIGINT_Handler.h>
#include <IPC/SignalHandler.h>
//#include "IPCManager.h"
#include <IPC/Cola.h>
#include <ipuerta/IPuertaMsg.h>
#include <sockets/cClientSocket.h>
#include <stdint.h>
#include <utils/System.h>
#include <broker/Constantes.h>
#include <broker/MensajeGenerico.h>
#include <Logger/Logger.h>

static const std::string ARCHIVO_COLA="/etc/hosts";
static const char LETRA_COLA='A';

std::string calcularPathColas (ArgParserLector& args)
{
	std::string path = args.recursos ();
	path.append (PATH_MQS);
	return path;
}


int main (int argc, char** argv)
{
	SIGINT_Handler intHandler;
	SignalHandler* sigs = SignalHandler::getInstance ();
	sigs->registrarHandler (SIGINT, &intHandler);

	LOG("COMPONENTE_LECTOR: Conectando a la cola con la interfaz");
	Cola<IPuertaMsg> colaMsg(ARCHIVO_COLA, LETRA_COLA);

	long idPuerta = atoi(argv[1]);
	BrokerMsg brokerMsg;

	LOG("COMPONENTE_LECTOR: Inicializando conexión con broker...");
// DESCOMENTAR
	cClientSocket brokerConn (sizeof brokerMsg);
	brokerConn.tcp_adopt_connection (atoi(argv[2]));

	brokerMsg.dstId = 0;
	brokerMsg.srcId = idPuerta;
	brokerMsg.op = BMO_BROKER_SET_ID;
	brokerMsg.param_a = idPuerta;
	brokerMsg.param_b = 0;

	LOG("COMPONENTE_LECTOR: Paquete de inicialización:\n"
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
//DESCOMENTAR
	brokerConn.tcp_send (reinterpret_cast<char*> (&brokerMsg));

	LOG("COMPONENTE_LECTOR: Enviado. Aguardando respuestas a las solicitudes...");

	while (intHandler.getGracefulQuit () == 0) {
		try {
			sleep(10);
			LOG("COMPONENTE_LECTOR: Dormi 10 segundos");
			int err = brokerConn.tcp_recv (reinterpret_cast<char*> (&brokerMsg));
			System::check (err);

			IPuertaMsg msg;
			msg.op = static_cast<IPuertaOp> (brokerMsg.op);
			switch (brokerMsg.op) {
				case BMO_NOTIF_ENTRADA_PERSONA:
					msg.msg.nep.res = brokerMsg.param_a;
					break;
				case BMO_NOTIF_ENTRADA_INVESTIGADOR:
					msg.msg.nei.res = brokerMsg.param_a;
					msg.msg.nei.numeroLocker = brokerMsg.param_b;
					break;
				case BMO_NOTIF_SALIDA_PERSONA:
					msg.msg.nsp.res = brokerMsg.param_a;
					break;
				case BMO_NOTIF_SALIDA_INVESTIGADOR:
					msg.msg.nsi.res = brokerMsg.param_a;
					msg.msg.nsi.pertenencias = brokerMsg.param_b;
					break;
				default:
					continue;
			}


		} catch (std::exception& e) {
			LOG("Error: %s.", e.what ());
			// TODO: log, eintr, etc
		}
	}

	sigs->destruir ();
}
