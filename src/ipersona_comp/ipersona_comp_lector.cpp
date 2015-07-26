#include "ArgParserLector.h"
#include <broker/Constantes.h>
#include <broker/MensajeGenerico.h>
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
	MensajeGenerico brokerMsg;

	LOG_IPCL("Creando socket con mensaje de tamaño %llu.",
			static_cast<unsigned long long> (sizeof brokerMsg));

	cClientSocket brokerConn (sizeof brokerMsg);
	brokerConn.tcp_adopt_connection (args.fdBroker ());

	brokerMsg.mtype = 0;
	brokerMsg.id = idPuerta;

	LOG_IPCL("Paquete de inicialización:\n"
			"\tdstId  : %ld\n"
			"\tsrcId  : %ld",
			brokerMsg.mtype,
			brokerMsg.id);

	brokerConn.tcp_send (reinterpret_cast<char*> (&brokerMsg));

	LOG_IPCL("Enviado. Aguardando operaciones...");

	while (intHandler.getGracefulQuit () == 0) {
		try {
			int err = brokerConn.tcp_recv (reinterpret_cast<char*> (&brokerMsg));
			if (err == 0) { /* EOF: conexión cerrada... */
				LOG_IPCL ("Conexión cerrada por el host remoto. Saliendo...");
				break;
			}
			System::check (err);

			LOG_IPCL ("Se recibió paquete desde el broker:\n"
					  "\tdstId        : %ld\n"
					  "\tsrcId        : %ld\n"
					  "\tmsg.op       : %d (%s)\n"
					  "\tmsg.param_a  : %ld\n"
					  "\tmsg.param_b  : %ld\n"
					  "\tshm.abierto  : %d\n"
					  "\tshm.capacidad: %d\n"
					  "\tshm.personas : %d",
					  brokerMsg.mtype,
					  brokerMsg.id,
					  brokerMsg.msg.op,
					  strMuseoMSGOP (brokerMsg.msg.op),
					  brokerMsg.msg.param_a,
					  brokerMsg.msg.param_b,
					  brokerMsg.shmem.abierto,
					  brokerMsg.shmem.capacidad,
					  brokerMsg.shmem.personas);

			IPersonaMsg msg;
			msg.op = static_cast<IPersonaOp> (brokerMsg.msg.op);
			switch (brokerMsg.msg.op) {
				case MuseoMSG::SOLIC_ENTRAR_MUSEO_PERSONA:
					msg.msg.osemp.idOrigen = brokerMsg.id;
					break;
				case MuseoMSG::SOLIC_ENTRAR_MUSEO_INVESTIGADOR:
					msg.msg.osemi.idOrigen = brokerMsg.id;
					msg.msg.osemi.pertenencias = brokerMsg.msg.param_a;
					break;
				case MuseoMSG::SOLIC_SALIR_MUSEO_PERSONA:
					msg.msg.ossmp.idOrigen = brokerMsg.id;
					break;
				case MuseoMSG::SOLIC_SALIR_MUSEO_INVESTIGADOR:
					msg.msg.ossmi.idOrigen = brokerMsg.id;
					msg.msg.ossmi.numeroLocker = brokerMsg.msg.param_a;
					break;
				case MuseoMSG::NOTIFICAR_CIERRE_MUSEO:
				case MuseoMSG::INDICAR_MUSEO_NO_LLENO:
					break;
				default:
					LOG_IPCL("Se recibió operación inválida: %d.",
							brokerMsg.msg.op);
					continue;
			}

			ipcman.ponerOperacion (msg);
		} catch (std::exception& e) {
			LOG_IPCL("Error (%d): %s.", errno, e.what ());
			// TODO: log, eintr, etc
		}
	}

	sigs->destruir ();
}
