#include "ArgParser.h"
#include "BrokerMsg.h"
#include <cassert>
#include <id-server/IIdClient.h>
#include <iostream>
#include "ipc-keys.h"
#include <IPC/SIGINT_Handler.h>
#include <IPC/SignalHandler.h>
#include <IPC/Cola.h>
//#include "IPCManager.h"
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

#define LOG_IPCMP(fmt, ...) \
	LOG("IPERSONA_COMP [%d] - " fmt, getpid (),##__VA_ARGS__)

static const std::string ARCHIVO_COLA="/etc/hosts";
static const char LETRA_COLA='A';

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
	EnvParam param (IPuerta::ENV_IPUERTA_MQ, dfltPath);
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
/*
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
*/
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

//		IPCManagerCleaner ipcmc (args.idLocal (), pathInterfaz, pathColas);
//		IPCManager& ipcman = ipcmc.get ();
//		ipcman.inicializar ();
		
		Cola<IPuertaMsg> colaMsg(ARCHIVO_COLA, LETRA_COLA);

		IIdClient idClient (args.idServer ().c_str ());
		IdGrabber idg (idClient, IIdClient::R_PUERTA);
		long idPersona = idg.get ();

		LOG_IPCMP("Se obtuvo el identificador %ld", idPersona);

		{
			LOG_IPCMP("Conectando con el puerto para lectores del broker.");

			cClientSocket connDeLector (sizeof (BrokerMsg));
			int fdLector = connDeLector.tcp_open_activo (
					args.broker ().c_str (),
					BROKER_READERS_PORT);
			System::check (fdLector);
			fdLector = connDeLector.getFD ();
			// ... el proceso lector inicializa la conexión
			// enviando el identificador.

			std::ostringstream oss;
			oss << idPersona;
			std::string strIdPuerta = oss.str ();

			oss.str ("");
			oss << "--id-local=" << args.idLocal ();
			std::string strIdLocal = oss.str ();

			oss.str ("");
			oss << "--fd-broker=" << fdLector;
			std::string strFdLector = oss.str ();

			std::string path_lector = calcularPathLector (argv[0]);
			std::vector<const char*> args_lector;
			args_lector.push_back (path_lector.c_str ());
			args_lector.push_back (strIdLocal.c_str ());
			args_lector.push_back (strFdLector.c_str ());
			args_lector.push_back (pathInterfaz.c_str ());
			args_lector.push_back (args.broker ().c_str ());
			args_lector.push_back (args.recursos ().c_str ());
			args_lector.push_back (strIdPuerta.c_str ());
			args_lector.push_back (NULL);

			oss.str ("");
			oss << "Lanzando lector:\n\t" << path_lector;
			for (size_t i = 0; i < args_lector.size () - 1; i++)
				oss << " " << args_lector[i];
			LOG_IPCMP("%s", oss.str ().c_str ());

			lector = System::spawn (path_lector.c_str (), args_lector);
			if (lector == -1) {
				SystemErrorException e;
				throw e;
			}
		}

		LOG_IPCMP("Conectando a puerto para escritores del broker.");

		cClientSocket connDeEscritor (sizeof (BrokerMsg));
		int err_sock = connDeEscritor.tcp_open_activo (
				args.broker ().c_str (),
				BROKER_WRITERS_PORT);
		System::check (err_sock);

		LOG_IPCMP("Conectado. Iniciando loop principal...");
		run_loop (idPersona, colaMsg, connDeEscritor, intHandler);
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what () << std::endl;
		err = 1;
	}

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
			IPuertaMsg msgInt;
			IPuertaMsg msgOp;

			// Se espera a que la aplicación solicite una operación.
//			err = ipcman.interfaz->leer (myMTYPE, &msgInt);
//			System::check (err);
			// TODO: validar el mensaje recibido...
//			assert (msgInt.op == SOLIC_PROXIMA_OPERACION);

			IPuertaMsg msg;

			colaMsg.leer(myMTYPE, &msg);

			LOG_IPCMP("Se recibió solicitud desde la interfaz.\n"
					  "Convirtiendo la solicitud en un formato aceptado por el broker...");


			struct BrokerMsg brokerMsg;
			brokerMsg.srcId = idPersona;
			switch (msgOp.op) {
				case OP_SOLIC_ENTRAR_MUSEO_PERSONA:
					brokerMsg.dstId = msgOp.msg.semp.idPuerta;
					brokerMsg.op = OP_SOLIC_ENTRAR_MUSEO_PERSONA;
					brokerMsg.param_a = 0;
					brokerMsg.param_b = 0;
					break;
				case OP_SOLIC_ENTRAR_MUSEO_INVESTIGADOR:
					brokerMsg.dstId = msgOp.msg.semi.idPuerta;
					brokerMsg.op = BMO_OP_SOLIC_ENTRAR_MUSEO_INVESTIGADOR;
					brokerMsg.param_a = msgOp.msg.semi.pertenencias;
					brokerMsg.param_b = 0;
					break;
				case OP_SOLIC_SALIR_MUSEO_PERSONA:
					brokerMsg.dstId = msgOp.msg.ssmp.idPuerta;
					brokerMsg.op = BMO_OP_SOLIC_SALIR_MUSEO_PERSONA;
					brokerMsg.param_a = 0;
					brokerMsg.param_b = 0;
					break;
				case OP_SOLIC_SALIR_MUSEO_INVESTIGADOR:
					brokerMsg.dstId = msgOp.msg.ssmp.idPuerta;
					brokerMsg.op = BMO_OP_SOLIC_SALIR_MUSEO_INVESTIGADOR;
					brokerMsg.param_a = 0;
					brokerMsg.param_b = 0;
					break;
				case OP_NOTIFICAR_CIERRE_MUSEO:
					brokerMsg.dstId = msgOp.msg.semp.idPuerta;
					brokerMsg.op = BMO_OP_NOTIFICAR_CIERRE_MUSEO;
					brokerMsg.param_a = msgOp.msg.ssmi.numeroLocker;
					brokerMsg.param_b = 0;
					break;
				default:
					assert (false);
			}


			LOG_IPCMP("Enviando mensaje al broker:\n"
					"\tdstId  : %d\n"
					"\tsrcId  : %d\n"
					"\top     : %d\n"
					"\tparam_a: %d\n"
					"\tparam_b: %d\n",
					brokerMsg.dstId,
					brokerMsg.srcId,
					brokerMsg.op,
					brokerMsg.param_a,
					brokerMsg.param_b);

			sockBroker.tcp_send (reinterpret_cast<char*> (&brokerMsg));
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
