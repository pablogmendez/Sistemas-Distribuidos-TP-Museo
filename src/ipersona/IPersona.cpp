#include <ipersona/IPersona.h>
#include <ipersona/Operacion.h>
#include <ipersona_comp/ipc-keys.h>
#include "IPersonaMsg.h"
#include <errno.h>
#include <error.h>
#include <IPC/Cola.h>
#include <libgen.h>
#include <Logger/Logger.h>
#include <signal.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <utils/EnvParam.h>
#include <utils/System.h>
#include <vector>

#define LOG_IP(fmt, ...) \
	LOG("IPUERTA [%d]: " fmt, getpid (),##__VA_ARGS__)

// TODO: mover a .h de constantes
static const char* DFLT_IPERSONA_COMP     = "ipersona_comp";
static const char* DFLT_IPERSONA_MQ       = "./ipersona.mq";
static const char* DFLT_IPERSONA_BROKER   = "broker";
static const char* DFLT_IPERSONA_IDSERVER = "id-server";
/////////////////////////////////

class IPersona::Impl
{
public:
	EnvParam mqConf;
	EnvParam compBin;
	EnvParam brokerHost;
	EnvParam idServerHost;
	int id_local;
	Cola<IPersonaMsg> mqComp;
	pid_t pidComp;

	Impl (int id_local)
		: mqConf (IPersona::ENV_IPERSONA_MQ, DFLT_IPERSONA_MQ)
		, compBin (IPersona::ENV_IPERSONA_COMP, DFLT_IPERSONA_COMP)
		, brokerHost (IPersona::ENV_IPERSONA_BROKER, DFLT_IPERSONA_BROKER)
		, idServerHost (IPersona::ENV_IPERSONA_IDSERVER,
				DFLT_IPERSONA_IDSERVER)
		, id_local (id_local)
		, mqComp (mqConf.get (), MQ_INT)
		, pidComp (-1)
		{}

	~Impl ()
		{
		}

		void lanzarComponente ();
		void terminarComponente ();

		void enviar (const IPersonaMsg& msg, const char* prg)
		{
			LOG_IP ("Enviando mensaje a componente:\n"
					"\tOP: %d (%s).", msg.op,
					strIPersonaOp (msg.op));
			int err = mqComp.escribir (msg);
			System::check (err, prg);
		}
};

const char* const IPersona::ENV_IPERSONA_COMP     = "ipersona_comp";
const char* const IPersona::ENV_IPERSONA_MQ       = "ipersona_mq";
const char* const IPersona::ENV_IPERSONA_BROKER   = "ipersona_broker";
const char* const IPersona::ENV_IPERSONA_IDSERVER = "ipersona_idserver";

static std::string dir_recursos (const char* mq)
{
	char* tmp = strdup (mq);
	std::string res = dirname (tmp);
	free (tmp);
	return res;
}

void IPersona::Impl::lanzarComponente ()
{
	const std::string& comp_path = compBin.get ();
	std::string res_dir = dir_recursos (mqConf.get ().c_str ());
	std::ostringstream oss;
	oss << "--id-local=" << id_local;
	std::string param_id_local = oss.str ();

	std::vector<const char*> args;
	args.push_back (comp_path.c_str ());
	args.push_back (param_id_local.c_str ());
	args.push_back (idServerHost.get ().c_str ());
	args.push_back (brokerHost.get ().c_str ());
	args.push_back (res_dir.c_str ());
	args.push_back (NULL);
	pidComp = System::spawn(comp_path.c_str (), args);
	System::check (pidComp);
}

void IPersona::Impl::terminarComponente ()
{
	if (pidComp != -1) {
		kill (pidComp, SIGINT);
	}
}

IPersona::IPersona (int id_local)
{
	pImpl = new Impl (id_local);
	pImpl->lanzarComponente ();
}

IPersona::~IPersona ()
{
	pImpl->terminarComponente ();
	delete pImpl;
}

Operacion IPersona::leerProximaOperacion ()
{
	int err;
	struct IPersonaMsg msg = {};
	long rtype = getpid ();

	msg.msg.spo.rtype = rtype;
	msg.op = SOLIC_PROXIMA_OPERACION;
	msg.mtype = pImpl->pidComp;

	err = pImpl->mqComp.escribir (msg);
	System::check (err, "IPersona::leerProximaOperacion");

	err = pImpl->mqComp.leer(rtype, &msg);
	System::check (err, "IPersona::leerProximaOperacion");

	LOG_IP ("Se recibió la operación desde el componente:\n"
			"\tOP: %d (%s)", msg.op,
			strIPersonaOp (msg.op));

	struct Operacion op = {};
	switch (msg.op) {
	case OP_SOLIC_ENTRAR_MUSEO_PERSONA:
		op.tipo = SOLIC_ENTRAR_MUSEO_PERSONA;
		op.op.semp.idOrigen = msg.msg.osemp.idOrigen;
		break;
	case OP_SOLIC_ENTRAR_MUSEO_INVESTIGADOR:
		op.tipo = SOLIC_ENTRAR_MUSEO_INVESTIGADOR;
		op.op.semi.idOrigen = msg.msg.osemi.idOrigen;
		op.op.semi.pertenencias = msg.msg.osemi.pertenencias;
		break;
	case OP_SOLIC_SALIR_MUSEO_PERSONA:
		op.tipo = SOLIC_SALIR_MUSEO_PERSONA;
		op.op.ssmp.idOrigen = msg.msg.ossmp.idOrigen;
		break;
	case OP_SOLIC_SALIR_MUSEO_INVESTIGADOR:
		op.tipo = SOLIC_SALIR_MUSEO_INVESTIGADOR;
		op.op.ssmi.idOrigen = msg.msg.ossmi.idOrigen;
		op.op.ssmi.numeroLocker = msg.msg.ossmi.numeroLocker;
		break;
	case OP_NOTIFICAR_CIERRE_MUSEO:
		op.tipo = NOTIFICAR_CIERRE_MUSEO;
		break;
	default:
		error (0, 0, "IPersona::leerProximaOperacion: operación inválida: %d", msg.op);
		return leerProximaOperacion ();
	}
	return op;
}

void IPersona::notificarEntrada (Operacion op, ResultadoOperacionEntrada res)
{
	struct IPersonaMsg msg = {};
	msg.op = NOTIF_ENTRADA_PERSONA;
	msg.msg.nep.res = res;
	msg.msg.nep.lleno = op.op.semp.lleno;
	msg.mtype = pImpl->pidComp;
	pImpl->enviar (msg, "IPersona::notificarEntrada[persona]");
}

void IPersona::notificarEntrada (Operacion op,
		ResultadoOperacionEntrada res, long numeroLocker)
{
	struct IPersonaMsg msg = {};
	msg.op = NOTIF_ENTRADA_INVESTIGADOR;
	msg.msg.nei.res = res;
	msg.msg.nei.numeroLocker = numeroLocker;
	msg.msg.nei.lleno = op.op.semi.lleno;
	msg.mtype = pImpl->pidComp;
	pImpl->enviar (msg, "IPersona::notificarEntrada[investigador]");
}

void IPersona::notificarSalida (Operacion op, ResultadoOperacionSalida res)
{
	struct IPersonaMsg msg = {};
	msg.op = NOTIF_SALIDA_PERSONA;
	msg.msg.nsp.res = res;
	msg.msg.nsp.lleno = op.op.ssmp.lleno;
	msg.mtype = pImpl->pidComp;
	pImpl->enviar (msg, "IPersona::notificarSalida[persona]");
}

void IPersona::notificarSalida (Operacion op,
		ResultadoOperacionSalida res, long pertenencias)
{
	struct IPersonaMsg msg = {};
	msg.op = NOTIF_SALIDA_INVESTIGADOR;
	msg.msg.nsi.res = res;
	msg.msg.nsi.pertenencias = pertenencias;
	msg.msg.nsi.lleno = op.op.ssmi.lleno;
	msg.mtype = pImpl->pidComp;
	pImpl->enviar (msg, "IPersona::notificarSalida[investigador]");
}
