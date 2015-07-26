#ifndef __IMUSEO_H__
#define __IMUSEO_H__

#include <stdint.h>
#include <IPC/Cola.h>
#include "IMuseoMsg.h"

namespace Entrada {
enum Resultado
{
	ENTRO      ,
	CERRADO    ,
	LLENO
};
}

namespace Salida {
enum Resultado
{
	SALIO         ,
	SALIO_Y_LLENO
};
}

class IMuseo {
private:
	Cola<IMuseoMsg> mqComp;
	pid_t child_pid;
	void lanzarComponente ();
	void terminarComponente ();

public:

IMuseo();

~IMuseo();

Entrada::Resultado entrarPersona();

Salida::Resultado sacarPersona();

bool abrirMuseo(uint32_t capacidad);

bool cerrarMuseo();

};


#endif /*__IMUSEO_H__*/
