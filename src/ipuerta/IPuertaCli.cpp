#include <ipuerta/IPuertaCli.h>
#include "IPuertaMsg.h"

#include <errno.h>
#include <error.h>
#include <IPC/Cola.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <utils/System.h>
#include <vector>

class IPuertaCli::Impl
{
public:
	Cola<IPuertaMsg> mqComp;
	pid_t pidComp;

	#define RESOURCES_DIR "."
	Impl ()
		: mqComp (RESOURCES_DIR "/ipuerta.mq", 'A')
		, pidComp (-1)
		{}

	~Impl ()
		{
			mqComp.destruir ();
		}

	void lanzarComponente ();
	void terminarComponente ();
};

const char* IPuertaCli::ENV_IPUERTA_COMP = "ipuerta_comp";
static const char* DFLT_IPUERTA_COMP = "ipuerta_comp";

void IPuertaCli::Impl::lanzarComponente ()
{
	const char* comp_path = getenv (IPuertaCli::ENV_IPUERTA_COMP);
	if (comp_path == NULL) {
		comp_path = DFLT_IPUERTA_COMP;
	}

	std::vector<const char*> args(2);
	args.push_back (comp_path);
	args.push_back (NULL);
	pidComp = System::spawn(comp_path, args);
	System::check (pidComp);
}

void IPuertaCli::Impl::terminarComponente ()
{
	kill (pidComp, SIGINT);
}

IPuertaCli::IPuertaCli ()
{
	pImpl = new Impl;
	pImpl->lanzarComponente ();
}

IPuertaCli::~IPuertaCli ()
{
	pImpl->terminarComponente ();
	delete pImpl;
}

void IPuertaCli::recorrerMuseo (
		long puertaEntrada,
		long puertaSalida,
		long tiempoRecorrido)
{
	int err;
	struct IPuertaMsg msg = {};
	long rtype = getpid ();

	msg.msg.rm.rtype = rtype;
	msg.msg.rm.idPuertaEntrada = puertaEntrada;
	msg.msg.rm.idPuertaSalida = puertaSalida;
	msg.msg.rm.tiempoRecorrido = tiempoRecorrido;

	msg.mtype = pImpl->pidComp;
	msg.op = RECORRER_MUSEO;
	err = pImpl->mqComp.escribir (msg);
	if (err == -1) {
		error (1, errno, "IPuertaCli::recorrerMuseo");
		throw "not reached";
	}

	err = pImpl->mqComp.leer (rtype, &msg);
	if (err == -1) {
		error (1, errno, "IPuertaCli::recorrerMuseo");
		throw "not reached";
	}
}

void IPuertaCli::iniciarRecorridoEspecial (
		long puerta,
		long tiempoRecorrido/*,
		TODO: pertenencias */)
{
	int err;
	struct IPuertaMsg msg = {};
	long rtype = getpid ();

	msg.msg.re.rtype = rtype;
	msg.msg.re.idPuerta = puerta;
	msg.msg.re.tiempoRecorrido = tiempoRecorrido;

	msg.mtype = pImpl->pidComp;
	msg.op = INICIAR_RECORRIDO_ESPECIAL;
	err = pImpl->mqComp.escribir (msg);
	if (err == -1) {
		error (1, errno, "IPuertaCli::iniciarRecorridoEspecial");
		throw "not reached";
	}

	err = pImpl->mqComp.leer (rtype, &msg);
	if (err == -1) {
		error (1, errno, "IPuertaCli::iniciarRecorridoEspecial");
		throw "not reached";
	}
}
