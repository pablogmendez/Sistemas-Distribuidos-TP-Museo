#ifndef __IMUSEO_H__
#define __IMUSEO_H__

#include <stdint.h>
#include <IPC/Cola.h>
#include <imuseo/IMuseoMsg.h>

class IMuseo {
private:
	Cola<IMuseoMsg> mqComp;
	pid_t child_pid;

void lanzarComponente (); 

void terminarComponente ();


public:

IMuseo();

~IMuseo();

bool entrarPersona();

bool sacarPersona();

bool abrirMuseo(uint32_t capacidad);

bool cerrarMuseo();

};


#endif /*__IMUSEO_H__*/
