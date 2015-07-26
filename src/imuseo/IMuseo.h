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

static inline const char* strResultado (Resultado r)
{
	#define RET_OP_STR(o) case o : return "Entrada::Resultado::"#o
	switch (r) {
		RET_OP_STR (ENTRO);
		RET_OP_STR (CERRADO);
		RET_OP_STR (LLENO);
		default: return "desconocida";
	}
	#undef RET_OP_STR
}

}

namespace Salida {
enum Resultado
{
	SALIO         ,
	SALIO_Y_LLENO
};

static inline const char* strResultado (Resultado r)
{
	#define RET_OP_STR(o) case o : return "Salida::Resultado::"#o
	switch (r) {
		RET_OP_STR (SALIO);
		RET_OP_STR (SALIO_Y_LLENO);
		default: return "desconocida";
	}
	#undef RET_OP_STR
}
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
