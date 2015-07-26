#include "ArgParserLector.h"
#include "BrokerMsg.h"
#include "ipc-keys.h"
#include <IPC/SIGINT_Handler.h>
#include <IPC/SignalHandler.h>
#include <museo/MuseoMSG.h>
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
//	sigs->registrarHandler (SIGINT, &intHandler);

	LOG("COMPONENTE_LECTOR: Conectando a la cola con la interfaz");
	Cola<IPuertaMsg> colaMsg(ARCHIVO_COLA, LETRA_COLA);

	long idPuerta = atoi(argv[1]);
	MensajeGenerico brokerMsg;

	long rtype = atoi(argv[3]);	


	LOG("COMPONENTE_LECTOR: Inicializando conexión con broker...");
// DESCOMENTAR
	cClientSocket brokerConn (sizeof brokerMsg);
	brokerConn.tcp_adopt_connection (atoi(argv[2]));

	brokerMsg.mtype = 0;
	brokerMsg.id = idPuerta;

//DESCOMENTAR
	brokerConn.tcp_send (reinterpret_cast<char*> (&brokerMsg));

	LOG("COMPONENTE_LECTOR: Enviado. Aguardando respuestas a las solicitudes...");
	LOG("COMPONENTE_LECTOR: RTYPE %d", rtype);
	while (/*intHandler.getGracefulQuit () == 0*/true) {
		try {
			int err = brokerConn.tcp_recv (reinterpret_cast<char*> (&brokerMsg));
			if (err == 0) { /* EOF: conexión cerrada... */
				LOG ("COMPONENTE_LECTOR: Conexión cerrada por el host remoto."
					" Saliendo...");
				break;
			}
			System::check (err);
	LOG("COMPONENTE_LECTOR: RECIBI MENSAJE");
			IPuertaMsg msg;
			msg.mtype = rtype;
			msg.op = static_cast<IPuertaOp> (brokerMsg.msg.op);
			switch (msg.op) {
				case NOTIF_ENTRADA_PERSONA:
					msg.msg.nep.res = (ResultadoOperacionEntrada)brokerMsg.msg.param_a;
					break;
				case NOTIF_ENTRADA_INVESTIGADOR:
					msg.msg.nei.res = (ResultadoOperacionEntrada)brokerMsg.msg.param_a;
					msg.msg.nei.numeroLocker = brokerMsg.msg.param_b;
					break;
				case NOTIF_SALIDA_PERSONA:
					msg.msg.nsp.res = (ResultadoOperacionSalida)brokerMsg.msg.param_a;
					break;
				case NOTIF_SALIDA_INVESTIGADOR:
					msg.msg.nsi.res = (ResultadoOperacionSalida)brokerMsg.msg.param_a;
					msg.msg.nsi.pertenencias = brokerMsg.msg.param_b;
					break;
				case NOTIF_ECHAR_PERSONA:
					LOG("COMPONENTE LECTOR: RECIBI NOTIFICACION DE MUSEO CERRADO");
					LOG("PERSONA ECHADA");
					kill(getppid(),SIGINT);
					kill(rtype,SIGINT);
					exit(0);
					break;
				default:
					continue;
			}
		colaMsg.escribir(msg);
	LOG("COMPONENTE_LECTOR: MANDO A LA COLA");
		} catch (std::exception& e) {
			LOG("Error: %s.", e.what ());
			// TODO: log, eintr, etc
		}
	}

	sigs->destruir ();
}
