#include <ipuerta/IPuertaServ.h>
#include "IPuertaMsg.h"

#include <errno.h>
#include <error.h>
#include <IPC/Cola.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

class IPuertaServ::Impl
{
public:
	Cola<IPuertaMsg> mqComp;

	#define RESOURCES_DIR "."
	Impl ()
		: mqComp (RESOURCES_DIR "/ipuerta.mq", 'A')
		{}

	~Impl ()
		{
			mqComp.destruir ();
		}
};

IPuertaServ::IPuertaServ ()
{
	pImpl = new Impl;
}

IPuertaServ::~IPuertaServ ()
{
	delete pImpl;
}

Operacion IPuertaServ::leerProximaOperacion ()
{
	int err;
	struct IPuertaMsg msg = {};

	err = pImpl->mqComp.leer(getpid (), &msg);
	if (err == -1) {
		error (1, errno, "IPuertaSev::leerProximaOperacion");
		throw "not reached";
	}

	if (msg.op == RECORRER_MUSEO) {
		long rtype = msg.msg.rm.rtype;
		long puertaEntrada = msg.msg.rm.idPuertaEntrada;
		long puertaSalida = msg.msg.rm.idPuertaSalida;
		long tiempoRecorrido = msg.msg.rm.tiempoRecorrido;
		return Operacion::crearRecorridoSimple(
				rtype, puertaEntrada, puertaSalida, tiempoRecorrido);
	}

	if (msg.op == INICIAR_RECORRIDO_ESPECIAL) {
		long rtype = msg.msg.re.rtype;
		long puerta = msg.msg.re.idPuerta;
		long tiempoRecorrido = msg.msg.rm.tiempoRecorrido;
		return Operacion::crearRecorridoEspecial (
				rtype, puerta, tiempoRecorrido);
	}

	error (0, 0, "IPuertaServ::leerProximaOperacion: operación inválida: %d", msg.op);
	return leerProximaOperacion ();
}

void IPuertaServ::responderOperacion(Operacion op)
{
	int err;
	struct IPuertaMsg msg = {};

	msg.mtype = op.getRType ();
	msg.op = RECORRIDO_COMPLETADO;

	err = pImpl->mqComp.escribir(msg);
	if (err == -1) {
		error (1, errno, "IPuertaServ::responderOperacion");
		throw "not reached";
	}
}
