#ifndef IPUERTA_IPERSONA_H
#define IPUERTA_IPERSONA_H

#include <icommon/resultado.h>
#include "Operacion.h"

class IPersona
{
private:
	class Impl;
	Impl* pImpl;
public:
	static const char* const ENV_IPERSONA_COMP;
	static const char* const ENV_IPERSONA_MQ;
	static const char* const ENV_IPERSONA_BROKER;
	static const char* const ENV_IPERSONA_IDSERVER;

	IPersona ();
	~IPersona ();

	Operacion leerProximaOperacion ();
	void notificarEntrada (Operacion op, ResultadoOperacionEntrada res);
	void notificarEntrada (Operacion op,
			ResultadoOperacionEntrada res, long numeroLocker);
	void notificarSalida (Operacion op, ResultadoOperacionSalida res);
	void notificarSalida (Operacion op,
			ResultadoOperacionSalida res, long pertenencias);
};

#endif
