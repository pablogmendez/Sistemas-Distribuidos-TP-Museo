#include "ArgParser.h"
#include <Logger/Logger.h>
#include <cassert>
#include <id-server/IIdClient.h>
#include <iostream>
#include "ipc-keys.h"
#include <IPC/SIGINT_Handler.h>
#include <IPC/SignalHandler.h>
#include <IPC/Cola.h>
#include <broker/Constantes.h>
#include <broker/MensajeGenerico.h>
#include <ipuerta/IPuerta.h>
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

static const char* DFLT_IPUERTA_BROKER   = "broker";
static const char* DFLT_IPUERTA_IDSERVER = "id-server";

#define LOG_IPCMP(fmt, ...) \
	LOG("IPERSONA_COMP [%d] - " fmt, getpid (),##__VA_ARGS__)

static const std::string ARCHIVO_COLA="/etc/hosts";
static const char LETRA_COLA='A';
static const char* DFLT_IPUERTA_COMP_LECTOR="../ipuerta_comp/ipuerta_comp_lector";

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
	EnvParam param ("/etc/hosts", dfltPath);
	return param.get ();
}

std::string calcularPathLector (const char* arg0)
{
	char* tmp = strdup (arg0);
	std::string ruta = dirname (tmp);
	ruta.append ("/ipuerta_comp_lector");
	free (tmp);
	return ruta;
}

void run_loop (
		long idPersona,
		Cola<IPuertaMsg> &colaMsg,
		cClientSocket& sockBroker,
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
	pid_t lector = -1;
	LOG("COMPONENTE_ESCRITOR: Iniciando componente ----> Escritor");
	SIGINT_Handler intHandler;
	SignalHandler* sigs = SignalHandler::getInstance ();
	sigs->registrarHandler (SIGINT, &intHandler);
		LOG("COMPONENTE_ESCRITOR: Conectando a la cola de comunicaciones de la interfaz ...");
		Cola<IPuertaMsg> colaMsg(ARCHIVO_COLA, LETRA_COLA);
// DESCOMENTAR
		IIdClient idClient (DFLT_IPUERTA_IDSERVER);
		IdGrabber idg (idClient, IIdClient::R_PERSONA);
		long idPersona = idg.get ();

		LOG("COMPONENTE_ESCRITOR: Se obtuvo el identificador %ld", idPersona);

		
			LOG("COMPONENTE_ESCRITOR: Conectando con el puerto para lectores del broker.");


			cClientSocket connDeLector (sizeof (MensajeGenerico));
			int fdLector = connDeLector.tcp_open_activo (
					DFLT_IPUERTA_BROKER,
					BROKER_READERS_PORT);
			System::check (fdLector);
			fdLector = connDeLector.getFD ();

			std::ostringstream oss;
			oss << idPersona;
			std::string strIdPuerta = oss.str ();

			oss.str ("");
			oss << fdLector;
			std::string strFdLector = oss.str ();

			oss.str ("");
			oss << getppid();
			std::string strPpid = oss.str ();

	LOG("Proceso %d: Lanzando componente lector", getpid());
          lector = fork();
           if(lector == 0) {
	        execlp(DFLT_IPUERTA_COMP_LECTOR, "ipuerta_comp_lector", strIdPuerta.c_str(), strFdLector.c_str(), strPpid.c_str(), NULL);
		exit(2);              
           }
           else if(lector == -1) {
               std::cout << "Error: Could not fork" << std::endl;
           }
	LOG("Proceso %d: Componente lanzado", getpid());

		LOG("COMPONENTE_ESCRITOR: Conectando a puerto para escritores del broker.");

		cClientSocket connDeEscritor (sizeof (MensajeGenerico));
//DESCOMENTAR		

		int err_sock = connDeEscritor.tcp_open_activo (
				DFLT_IPUERTA_BROKER,
				BROKER_WRITERS_PORT);
		System::check (err_sock);

		LOG("COMPONENTE_ESCRITOR: Conectado. Iniciando loop principal...");
		run_loop (idPersona, colaMsg, connDeEscritor, intHandler);

	//MANDO MENSAJE DE DESCONEXION
	MensajeGenerico descMsg;
	descMsg.id = idPersona;
	descMsg.msg.op = MuseoMSG::NOTIF_DESCONEXION;
	connDeEscritor.tcp_send((char*) &descMsg);
	
	if (lector != -1) {
		kill (lector, SIGINT);
	}

	return err;
}

void run_loop (
		long idPersona,
		Cola<IPuertaMsg> &colaMsg,
		cClientSocket& sockBroker,
		SIGINT_Handler& intHandler)
{
	long myMTYPE = getpid ();
	while (intHandler.getGracefulQuit () != 1) {
		try {
			int err;
			IPuertaMsg msgOp;

			int error = colaMsg.leer(myMTYPE, &msgOp);

			System::check (error);

			LOG("COMPONENTE_ESCRITOR: Se recibió solicitud desde la interfaz.\n"
					  "Convirtiendo la solicitud en un formato aceptado por el broker...");

			LOG("COMPONENTE_ESCRITOR: mtype ---> %ld", msgOp.mtype);
			LOG("COMPONENTE_ESCRITOR: op    ---> %d", msgOp.op);

			MensajeGenerico brokerMsg;
			brokerMsg.id = idPersona;
			switch (msgOp.op) {
				case OP_SOLIC_ENTRAR_MUSEO_PERSONA:
					brokerMsg.mtype = msgOp.msg.semp.idPuerta;
					brokerMsg.msg.op = static_cast<MuseoMSG::OP> (MuseoMSG::SOLIC_ENTRAR_MUSEO_PERSONA);
					brokerMsg.msg.param_a = 0;
					brokerMsg.msg.param_b = 0;
					break;
				case OP_SOLIC_ENTRAR_MUSEO_INVESTIGADOR:
					brokerMsg.mtype = msgOp.msg.semi.idPuerta;
					brokerMsg.msg.op = static_cast<MuseoMSG::OP> (MuseoMSG::SOLIC_ENTRAR_MUSEO_INVESTIGADOR);
					brokerMsg.msg.param_a = msgOp.msg.semi.pertenencias;
					brokerMsg.msg.param_b = 0;
					break;
				case OP_SOLIC_SALIR_MUSEO_PERSONA:
					brokerMsg.mtype = msgOp.msg.ssmp.idPuerta;
					brokerMsg.msg.op = static_cast<MuseoMSG::OP> (MuseoMSG::SOLIC_SALIR_MUSEO_PERSONA);
					brokerMsg.msg.param_a = 0;
					brokerMsg.msg.param_b = 0;
					break;
				case OP_SOLIC_SALIR_MUSEO_INVESTIGADOR:
					brokerMsg.mtype = msgOp.msg.ssmi.idPuerta;
					brokerMsg.msg.op = static_cast<MuseoMSG::OP> (MuseoMSG::SOLIC_SALIR_MUSEO_INVESTIGADOR);
					brokerMsg.msg.param_a = msgOp.msg.ssmi.numeroLocker;
					brokerMsg.msg.param_b = 0;
					break;
				case OP_NOTIFICAR_CIERRE_MUSEO:
					brokerMsg.mtype = msgOp.msg.semp.idPuerta;
					brokerMsg.msg.op = static_cast<MuseoMSG::OP> (MuseoMSG::NOTIFICAR_CIERRE_MUSEO);
					brokerMsg.msg.param_a = msgOp.msg.ssmi.numeroLocker;
					brokerMsg.msg.param_b = 0;
					break;
				default:
					assert (false);
			}


			LOG("COMPONENTE_ESCRITOR: Enviando respuesta al broker:\n"
					"\tdstId  : %ld\n"
					"\tsrcId  : %ld\n"
					"\top     : %ld\n"
					"\tparam_a: %ld\n"
					"\tparam_b: %ld",
					brokerMsg.mtype,
					brokerMsg.id,
					brokerMsg.msg.op,
					brokerMsg.msg.param_a,
					brokerMsg.msg.param_b);
// DESCOMENTAR
			sockBroker.tcp_send (reinterpret_cast<char*> (&brokerMsg));
		} catch (SystemErrorException& e) {
			std::cerr << "Error (" << e.number ()
					  << "): " << e.what () << std::endl;
		}
	}
}
