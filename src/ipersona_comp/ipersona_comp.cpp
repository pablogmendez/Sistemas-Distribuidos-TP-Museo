#include "ArgParser.h"
#include <broker/Constantes.h>
#include <broker/MensajeGenerico.h>
#include <cassert>
#include <id-server/IIdClient.h>
#include <iostream>
#include "ipc-keys.h"
#include <IPC/SIGINT_Handler.h>
#include <IPC/SignalHandler.h>
#include "IPCManager.h"
#include <ipersona/IPersona.h>
#include <libgen.h>
#include <Logger/Logger.h>
#include <museo/MuseoMSG.h>
#include <sockets/cClientSocket.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string.h>
#include <unistd.h>
#include <utils/EnvParam.h>
#include <utils/System.h>

#define LOG_IPCMP(fmt, ...) \
	LOG("IPERSONA_COMP [%d] - " fmt, getpid (),##__VA_ARGS__)

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

std::string calcularPathLector (const char* arg0)
{
	char* tmp = strdup (arg0);
	std::string ruta = dirname (tmp);
	ruta.append ("/ipersona_comp_lector");
	free (tmp);
	return ruta;
}

void run_loop (
		long idPuerta,
		IPCManager& ipcman,
		cClientSocket& sockBroker,
		SIGINT_Handler& intHandler);

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

		IPCManagerCleaner ipcmc (args.idLocal (), pathInterfaz, pathColas);
		IPCManager& ipcman = ipcmc.get ();
		ipcman.inicializar ();

		IIdClient idClient (args.idServer ().c_str ());
		IdGrabber idg (idClient, IIdClient::R_PUERTA);
		long idPuerta = idg.get ();

		LOG_IPCMP("Se obtuvo el identificador %ld", idPuerta);

		{
			LOG_IPCMP("Conectando con el puerto para lectores del broker.");

			cClientSocket connDeLector (sizeof (MensajeGenerico));
			int fdLector = connDeLector.tcp_open_activo (
					args.broker ().c_str (),
					BROKER_READERS_PORT);
			System::check (fdLector);
			fdLector = connDeLector.getFD ();
			// ... el proceso lector inicializa la conexión
			// enviando el identificador.

			std::ostringstream oss;
			oss << idPuerta;
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

		cClientSocket connDeEscritor (sizeof (MensajeGenerico));
		int err_sock = connDeEscritor.tcp_open_activo (
				args.broker ().c_str (),
				BROKER_WRITERS_PORT);
		System::check (err_sock);

		LOG_IPCMP("Conectado. Iniciando loop principal...");
		run_loop (idPuerta, ipcman, connDeEscritor, intHandler);
	} catch (std::exception& e) {
		LOG_IPCMP ("Error: %s", e.what ());
		err = 1;
	}

	if (lector != -1) {
		kill (lector, SIGINT);
	}

	return err;
}

void run_loop (
		long idPuerta,
		IPCManager& ipcman,
		cClientSocket& sockBroker,
		SIGINT_Handler& intHandler)
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

			LOG_IPCMP("Se recibió solicitud desde la interfaz.\n"
					  "Aguardando operación desde el broker...");

			// Ahora se espera a que arribe una operación desde el broker
			msgOp = ipcman.leerOperacionConPrioridad ();
			// TODO: validar mensaje de operacion...
			long dstId;
			switch (msgOp.op) {
				case OP_SOLIC_ENTRAR_MUSEO_PERSONA:
					dstId = msgOp.msg.osemp.idOrigen;
					break;
				case OP_SOLIC_ENTRAR_MUSEO_INVESTIGADOR:
					dstId = msgOp.msg.osemi.idOrigen;
					break;
				case OP_SOLIC_SALIR_MUSEO_PERSONA:
					dstId = msgOp.msg.ossmp.idOrigen;
					break;
				case OP_SOLIC_SALIR_MUSEO_INVESTIGADOR:
					dstId = msgOp.msg.ossmp.idOrigen;
					break;
				case OP_NOTIFICAR_CIERRE_MUSEO:
					dstId = msgOp.msg.osemp.idOrigen;
					break;
				default:
					assert (false);
			}

			LOG_IPCMP("Se recibió operación: %d (%s).\n"
					  "Enviando operación a interfaz...",
					  msgOp.op,
					  strIPersonaOp (msgOp.op));

			// Se devuelve la operación a la interfaz
			msgOp.mtype = msgInt.msg.spo.rtype;
			err = ipcman.interfaz->escribir (msgOp);
			System::check (err);

			// Luego se aguarda su respuesta
			err = ipcman.interfaz->leer (myMTYPE, &msgInt);
			System::check (err);

			LOG_IPCMP("Se obtuvo respuesta de interfaz: %d (%s).",
					msgInt.op,
					strIPersonaOp (msgInt.op));

			// TODO: validar el mensaje recibido...
			long param_a;
			long param_b;
			switch (msgInt.op) {
				case NOTIF_ENTRADA_PERSONA:
					param_a = msgInt.msg.nep.res;
					param_b = 0;
					break;
				case NOTIF_ENTRADA_INVESTIGADOR:
					param_a = msgInt.msg.nei.res;
					param_b = msgInt.msg.nei.numeroLocker;
					break;
				case NOTIF_SALIDA_PERSONA:
					param_a = msgInt.msg.nsp.res;
					param_b = 0;
					break;
				case NOTIF_SALIDA_INVESTIGADOR:
					param_a = msgInt.msg.nsi.res;
					param_b = msgInt.msg.nsi.pertenencias;
					break;
				default:
					assert (false);
			}

			MensajeGenerico msgBroker;
			msgBroker.mtype = dstId;
			msgBroker.id = idPuerta;
			msgBroker.msg.op = static_cast<MuseoMSG::OP> (msgInt.op);
			msgBroker.msg.param_a = param_a;
			msgBroker.msg.param_b = param_b;

			LOG_IPCMP("Enviando respuesta al broker:\n"
					"\tdstId  : %ld\n"
					"\tsrcId  : %ld\n"
					"\top     : %d (%s)\n"
					"\tparam_a: %ld\n"
					"\tparam_b: %ld",
					msgBroker.mtype,
					msgBroker.id,
					msgBroker.msg.op,
					strMuseoMSGOP (msgBroker.msg.op),
					msgBroker.msg.param_a,
					msgBroker.msg.param_b);

			sockBroker.tcp_send (reinterpret_cast<char*> (&msgBroker));
		} catch (SystemErrorException& e) {
			LOG_IPCMP ("Error (%d): %s", e.number (), e.what ());

			if (e.number () == EINTR) {
				// volver al inicio a chequear la bandera
				continue;
			}
			// TODO: qué hago acá?
			throw e;
		}
	}
}
