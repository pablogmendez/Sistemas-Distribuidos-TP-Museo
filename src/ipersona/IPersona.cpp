#include <ipersona/IPersona.h>
#include <ipersona/Operacion.h>
#include "IPersonaMsg.h"
#include <errno.h>
#include <error.h>
#include <IPC/Cola.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <utils/EnvParams.h>
#include <utils/System.h>
#include <vector>

class IPersona::Impl : private EnvParams
{
public:
	Cola<IPersonaMsg> mqComp;
	pid_t pidComp;

	Impl ()
		: EnvParams (
				ParamDefaults ("MQ", "./persona.mq", NULL),
				IPersona::ENV_IPERSONA_PARAMS)
		, mqComp (get_param ("MQ"), 'A')
		, pidComp (-1)
		{}

	~Impl ()
		{
			mqComp.destruir ();
		}

		void lanzarComponente ();
		void terminarComponente ();

		void enviar (const IPersonaMsg& msg, const char* prg)
		{
			int err = mqComp.escribir (msg);
			if (err == -1) {
				error (1, errno, "%s", prg);
				throw "not reached";
			}
		}
};

const char* const IPersona::ENV_IPERSONA_COMP = "ipersona_comp";
const char* const IPersona::ENV_IPERSONA_PARAMS = "ipersona_params";
static const char* DFLT_IPERSONA_COMP = "ipersona_comp";

void IPersona::Impl::lanzarComponente ()
{
	const char* comp_path = getenv (IPersona::ENV_IPERSONA_COMP);
	if (comp_path == NULL) {
		comp_path = DFLT_IPERSONA_COMP;
	}

	std::vector<const char*> args;
	args.push_back (comp_path);
	args.push_back (NULL);
	pidComp = System::spawn(comp_path, args);
	System::check (pidComp);
}

void IPersona::Impl::terminarComponente ()
{
	if (pidComp != -1) {
		kill (pidComp, SIGINT);
	}
}

IPersona::IPersona ()
{
	pImpl = new Impl;
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
	if (err == -1) {
		error (1, errno, "IPersona::leerProximaOperacion");
		throw "not reached";
	}

	err = pImpl->mqComp.leer(rtype, &msg);
	if (err == -1) {
		error (1, errno, "IPersona::leerProximaOperacion");
		throw "not reached";
	}

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
	msg.mtype = pImpl->pidComp;
	pImpl->enviar (msg, "IPersona::notificarEntrada[investigador]");
}

void IPersona::notificarSalida (Operacion op, ResultadoOperacionSalida res)
{
	struct IPersonaMsg msg = {};
	msg.op = NOTIF_SALIDA_PERSONA;
	msg.msg.nsp.res = res;
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
	msg.mtype = pImpl->pidComp;
	pImpl->enviar (msg, "IPersona::notificarSalida[investigador]");
}
