#include <ipuerta/IPuerta.h>
#include "IPuertaMsg.h"

#include <errno.h>
#include <error.h>
#include <IPC/Cola.h>
#include <signal.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <utils/EnvParam.h>
#include <utils/System.h>
#include <vector>

// TODO: mover a .h de constantes
static const char* DFLT_IPUERTA_COMP = "ipuerta_comp";
static const char* DFLT_IPUERTA_MQ   = "/etc/hosts";
static const char* DFLT_IPERSONA_BROKER   = "broker";
static const char* DFLT_IPERSONA_IDSERVER = "id-server";
/////////////////////////////////

class IPuerta::Impl
{
public:
	EnvParam mqConf;
	EnvParam compBin;
	Cola<IPuertaMsg> mqComp;
	pid_t pidComp;

	Impl ()ect
		: mqConf (IPuerta::ENV_IPUERTA_MQ, DFLT_IPUERTA_MQ)
		, compBin (IPuerta::ENV_IPUERTA_COMP, DFLT_IPUERTA_COMP)
		, mqComp (mqConf.get (), 'A')
		, pidComp (-1)
		{}

	~Impl ()
		{
			mqComp.destruir ();
		}

	void lanzarComponente ();
	void terminarComponente ();
};

const char* const IPuerta::ENV_IPUERTA_COMP = "ipuerta_comp";
const char* const IPuerta::ENV_IPUERTA_MQ   = "/etc/hosts";

void IPuerta::Impl::lanzarComponente ()
{
	const std::string& comp_path = compBin.get ();
	std::vector<const char*> args;
	args.push_back (comp_path.c_str ());
	args.push_back (NULL);
	pidComp = System::spawn(comp_path.c_str (), args);
	System::check (pidComp);
}

void IPuerta::Impl::terminarComponente ()
{
	if (pidComp != -1) {
		kill (pidComp, SIGINT);
	}
}

IPuerta::IPuerta ()
{
	pImpl = new Impl;
	pImpl->lanzarComponente ();
}

IPuerta::~IPuerta ()
{
	pImpl->terminarComponente ();
	delete pImpl;
}

void IPuerta::entrar (long puerta)
{
	int err;
	struct IPuertaMsg msg = {};
	long rtype = getpid ();

	msg.msg.semp.rtype = rtype;
	msg.msg.semp.idPuerta = puerta;
	msg.mtype = pImpl->pidComp;
	msg.op = SOLIC_ENTRAR_MUSEO_PERSONA;
	err = pImpl->mqComp.escribir (msg);
	if (err == -1) {
		error (1, errno, "IPuerta::entrar[persona]");
		throw "not reached";
	}

	err = pImpl->mqComp.leer (rtype, &msg);
	if (err == -1) {
		error (1, errno, "IPuerta::entrar[persona]");
		throw "not reached";
	}

	if (msg.op == NOTIFICACION_ENTRADA_PERSONA) {
		// TODO: ver si no estaba cerrado el museo
		return;
	}

	std::ostringstream oss;
	oss << "IPuerta::entrar[persona]: ";
	oss << "se recibio operacion incorrecta: " << msg.op;
	error (1, 0, "%s", oss.str ().c_str ());
	throw "not reached";
}

long IPuerta::entrar (long puerta, long pertenencias)
{
	int err;
	struct IPuertaMsg msg = {};
	long rtype = getpid ();

	msg.msg.semi.rtype = rtype;
	msg.msg.semi.idPuerta = puerta;
	msg.msg.semi.pertenencias = pertenencias;
	msg.mtype = pImpl->pidComp;
	msg.op = SOLIC_ENTRAR_MUSEO_INVESTIGADOR;
	err = pImpl->mqComp.escribir (msg);
	if (err == -1) {
		error (1, errno, "IPuerta::entrar[investigador]");
		throw "not reached";
	}

	err = pImpl->mqComp.leer (rtype, &msg);
	if (err == -1) {
		error (1, errno, "IPuerta::entrar[investigador]");
		throw "not reached";
	}

	if (msg.op == NOTIFICACION_ENTRADA_INVESTIGADOR) {
		// TODO: ver si no estaba cerrado el museo
		return msg.msg.nei.numeroLocker;
	}

	std::ostringstream oss;
	oss << "IPuerta::entrar[investigador]: ";
	oss << "se recibio operacion incorrecta: " << msg.op;
	error (1, 0, "%s", oss.str ().c_str ());
	throw "not reached";
}

void IPuerta::salir (long puerta)
{
	int err;
	struct IPuertaMsg msg = {};
	long rtype = getpid ();

	msg.msg.ssmp.rtype = rtype;
	msg.msg.ssmp.idPuerta = puerta;
	msg.mtype = pImpl->pidComp;
	msg.op = SOLIC_SALIR_MUSEO_PERSONA;
	err = pImpl->mqComp.escribir (msg);
	if (err == -1) {
		error (1, errno, "IPuerta::salir[persona]");
		throw "not reached";
	}

	err = pImpl->mqComp.leer (rtype, &msg);
	if (err == -1) {
		error (1, errno, "IPuerta::salir[persona]");
		throw "not reached";
	}

	if (msg.op == NOTIFICACION_SALIDA_PERSONA) {
		return;
	}

	std::ostringstream oss;
	oss << "IPuerta::salir[persona]: ";
	oss << "se recibio operacion incorrecta: " << msg.op;
	error (1, 0, "%s", oss.str ().c_str ());
	throw "not reached";
}

long IPuerta::salir (long puerta, long numeroLocker)
{
	int err;
	struct IPuertaMsg msg = {};
	long rtype = getpid ();

	msg.msg.ssmi.rtype = rtype;
	msg.msg.ssmi.idPuerta = puerta;
	msg.msg.ssmi.numeroLocker = numeroLocker;
	msg.mtype = pImpl->pidComp;
	msg.op = SOLIC_SALIR_MUSEO_INVESTIGADOR;
	err = pImpl->mqComp.escribir (msg);
	if (err == -1) {
		error (1, errno, "IPuerta::salir[investigador]");
		throw "not reached";
	}

	err = pImpl->mqComp.leer (rtype, &msg);
	if (err == -1) {
		error (1, errno, "IPuerta::salir[investigador]");
		throw "not reached";
	}

	if (msg.op == NOTIFICACION_SALIDA_INVESTIGADOR) {
		if (msg.msg.nsi.res == SALIO) {
			return msg.msg.nsi.pertenencias;
		}

		std::ostringstream oss;
		oss << "IPuerta::salir[investigador]: ";
		oss << "se recibio error: puerta incorrecta.";
		error (1, 0, "%s", oss.str ().c_str ());
		throw "not reached";
	}

	std::ostringstream oss;
	oss << "IPuerta::salir[investigador]: ";
	oss << "se recibio operacion incorrecta: " << msg.op;
	error (1, 0, "%s", oss.str ().c_str ());
	throw "not reached";
}
