#ifndef __IMUSEO_H__
#define __IMUSEO_H__

#include <stdint.h>
#include <IPC/Cola.h>
#include "IMuseoMsg.h"

enum ResultadoEntrada
{
	ENTRO      ,
	CERRADO    ,
	LLENO
};

enum ResultadoSalida
{
	SALIO         ,
	SALIO_Y_LLENO
}

class IMuseo {
private:
	Cola<IMuseoMsg> mqComp;
	pid_t child_pid;

public:

IMuseo();

~IMuseo();

bool entrarPersona();

bool sacarPersona();

void abrirMuseo(uint32_t capacidad);

void cerrarMuseo();

};


#endif /*__IMUSEO_H__*/
