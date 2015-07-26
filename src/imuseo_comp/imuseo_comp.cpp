#include <Logger/Logger.h>
#include <cassert>
#include <id-server/IIdClient.h>
#include <iostream>
#include <IPC/SIGINT_Handler.h>
#include <IPC/SignalHandler.h>
#include <IPC/Cola.h>
#include <broker/Constantes.h>
#include <broker/MensajeGenerico.h>
#include <imuseo/IMuseo.h>
#include <libgen.h>
#include <Logger/Logger.h>
#include <sockets/cClientSocket.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string.h>
#include <unistd.h>
#include <utils/EnvParam.h>
#include <utils/System.h>
#include <ipuerta/IPuertaMsg.h>
#include <museo/MuseoMSG.h>

static const char* DFLT_IMUSEO_BROKER   = "broker";
static const char* DFLT_IMUSEO_IDSERVER = "id-server";
static const std::string ARCHIVO_COLA="/etc/hosts";
static const char LETRA_COLA='B';

void run_loop (
		long idPersona,
		Cola<IMuseoMsg> &colaMsg,
		cClientSocket& connDeEscritor,
		cClientSocket& connDeLector,
		SIGINT_Handler& intHandler);

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
	LOG("COMPONENTE_MUSEO: Iniciando componente ----> Escritor");
	SIGINT_Handler intHandler;
	SignalHandler* sigs = SignalHandler::getInstance ();
	sigs->registrarHandler (SIGINT, &intHandler);
		LOG("COMPONENTE_MUSEO: Conectando a la cola de comunicaciones de la interfaz ...");
		Cola<IMuseoMsg> colaMsg(ARCHIVO_COLA, LETRA_COLA);
// DESCOMENTAR
////		IIdClient idClient (DFLT_IPUERTA_IDSERVER);
//		IdGrabber idg (idClient, IIdClient::R_PUERTA);
		long idMuseo = 101;//idg.get ();

		LOG("COMPONENTE_MUSEO: Se obtuvo el identificador %ld", idMuseo);
		
		LOG("COMPONENTE_MUSEO: Conectando con el puerto para lectores del broker.");
		cClientSocket connDeLector (sizeof (MensajeGenerico));
		int fdLector = connDeLector.tcp_open_activo (
				DFLT_IMUSEO_BROKER,
				BROKER_READERS_PORT);
		System::check(fdLector);

		LOG("COMPONENTE_MUSEO: Conectando a puerto para escritores del broker.");
		cClientSocket connDeEscritor (sizeof (MensajeGenerico));
		int err_sock = connDeEscritor.tcp_open_activo (
				DFLT_IMUSEO_BROKER,
				BROKER_WRITERS_PORT);
		System::check (err_sock);

		LOG("COMPONENTE_MUSEO: Conectado. Iniciando loop principal...");
		run_loop (idMuseo, colaMsg, connDeEscritor, connDeLector, intHandler);

		return err;
}

void run_loop (
		long idMuseo,
		Cola<IMuseoMsg> &colaMsg,
		cClientSocket& connDeEscritor,
		cClientSocket& connDeLector,
		SIGINT_Handler& intHandler)
{
	long myMTYPE = getpid ();
	long MTYPEdeMiPapa = getppid();
	uint32_t retval;
	IMuseoMsg msg;
	MensajeGenerico brokerMsg;

	while (intHandler.getGracefulQuit () != 1) {
		try {
			int err;

			int error = colaMsg.leer(myMTYPE, &msg);

			System::check(error);

			LOG("COMPONENTE_MUSEO: Se recibió solicitud desde la interfaz");
			LOG("COMPONENTE_MUSEO: mtype ---> %ld", msg.mtype);
			brokerMsg.id = idMuseo;
			brokerMsg.mtype = 1; // Dame la SHM
			switch(msg.op) {
				case 1: LOG("COMPONENTE_MUSEO: Operacion 1 ---> ENTRAR PERSONA");
					break;
				case 2:	LOG("COMPONENTE_MUSEO: Operacion 2 ---> SACAR PERSONA");
					break;
				case 3: LOG("COMPONENTE_MUSEO: Operacion 3 ---> ABRIR MUSEO");
					break;
				case 4:	LOG("COMPONENTE_MUSEO: Operacion 4 ---> CERRAR MUSEO");
					break;
				default: LOG("COMPONENTE_MUSEO: MANDASTE CUALQUIERA");
					break;
			}

		LOG("COMPONENTE_MUSEO: Enviando peticion al BROKER");
		LOG("COMPONENTE_MUSEO: brokerMsg.id ----> %ld", idMuseo);
		LOG("COMPONENTE_MUSEO: brokerMsg.mtype ----> %ld", 1);
		connDeEscritor.tcp_send (reinterpret_cast<char*> (&brokerMsg));
		} catch (SystemErrorException& e) {
			std::cerr << "Error (" << e.number ()
					  << "): " << e.what () << std::endl;
		}
		LOG("COMPONENTE_MUSEO: Esperando respuesta del BROKER");
		int err = connDeLector.tcp_recv (reinterpret_cast<char*> (&brokerMsg));
		System::check (err);
		LOG("COMPONENTE_MUSEO: Recibi respuesta del BROKER");
		LOG("COMPONENTE_MUSEO: Preparando respuesta para la interfaz");
		msg.mtype = MTYPEdeMiPapa;
		brokerMsg.mtype = 2;
		switch(msg.op) {
                                case 1: if(brokerMsg.shmem.personas < brokerMsg.shmem.capacidad) {
						brokerMsg.shmem.personas++;
						msg.op = 5;
					}
					else {
						msg.op = 6;
					}
                                        break;
                                case 2: if(brokerMsg.shmem.personas > 0) {
                                                brokerMsg.shmem.personas--;
                                                msg.op = 5;
                                        }
                                        else {
                                                msg.op = 6;
                                        }
                                        break;
                                case 3: if(brokerMsg.shmem.abierto == 0) {
                                            brokerMsg.shmem.abierto = 1;
					    brokerMsg.shmem.capacidad = msg.param_1;
                                            msg.op = 5;
                                        }
                                        else {
                                                msg.op = 6;
                                        }
                                        break;
                                case 4: if(brokerMsg.shmem.abierto == 1) {
                                            brokerMsg.shmem.abierto = 0;
                                            brokerMsg.shmem.capacidad = 0;
                                            msg.op = 5;
                                        }
                                        else {
                                                msg.op = 6;
                                        }
                                        break;
                                default: LOG("COMPONENTE_MUSEO: MANDASTE CUALQUIERA");
                                        break;
                        }

			LOG("COMPONENTE_MUSEO: Enviando respuesta al broker:\n"
					"\tmtype       : %ld\n"
					"\tid          : %ld\n"
					"\tabierto     : %ld\n"
					"\tcapacidad   : %ld\n"
					"\tpersonas    : %ld",
					brokerMsg.mtype,
					brokerMsg.id,
					brokerMsg.shmem.abierto,
					brokerMsg.shmem.capacidad,
					brokerMsg.shmem.personas);
// DESCOMENTAR
		try {
			connDeEscritor.tcp_send (reinterpret_cast<char*> (&brokerMsg));
			LOG("COMPONENTE_MUSEO: Respuesta enviada");
		} catch (SystemErrorException& e) {
			std::cerr << "Error (" << e.number ()
					  << "): " << e.what () << std::endl;
		}
		LOG("COMPONENTE_MUSEO: Enviando respuesta a la interfaz");
		LOG("COMPONENTE_MUSEO: mtype ----> %d", msg.mtype);
		LOG("COMPONENTE_MUSEO: op    ----> %d", msg.op);
		colaMsg.escribir(msg);
	}
}

