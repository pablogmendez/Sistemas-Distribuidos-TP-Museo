#ifndef IPUERTA_IPUERTASERV_H
#define IPUERTA_IPUERTASERV_H

#include "Operacion.h"

class IPuertaServ
{
private:
	class Impl;
	Impl* pImpl;
public:
	IPuertaServ ();
	~IPuertaServ ();

	Operacion leerProximaOperacion ();
	void responderOperacion(Operacion op);
};

#endif
