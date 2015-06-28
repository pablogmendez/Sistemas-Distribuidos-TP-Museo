#ifndef IPUERTA_IPUERTAMSG_H
#define IPUERTA_IPUERTAMSG_H

#include <icommon/resultado.h>

enum IPuertaOp
{
	// Interfaz --> Componente
	SOLIC_ENTRAR_MUSEO_PERSONA       = 1,
	SOLIC_ENTRAR_MUSEO_INVESTIGADOR     ,
	SOLIC_SALIR_MUSEO_PERSONA           ,
	SOLIC_SALIR_MUSEO_INVESTIGADOR      ,
	// Componente --> Interfaz
	NOTIFICACION_ENTRADA_PERSONA        ,
	NOTIFICACION_ENTRADA_INVESTIGADOR   ,
	NOTIFICACION_SALIDA_PERSONA         ,
	NOTIFICACION_SALIDA_INVESTIGADOR
};

struct IPuertaMsg_SolicEntrarMuseoPersona
{
	long rtype;
	long idPuerta;
};

struct IPuertaMsg_SolicEntrarMuseoInvestigador
{
	long rtype;
	long idPuerta;
	long pertenencias;
};

struct IPuertaMsg_SolicSalirMuseoPersona
{
	long rtype;
	long idPuerta;
};

struct IPuertaMsg_SolicSalirMuseoInvestigador
{
	long rtype;
	long idPuerta;
	long numeroLocker;
};

struct IPuertaMsg_NotificacionEntradaPersona
{
	enum ResultadoOperacionEntrada res;
};

struct IPuertaMsg_NotificacionEntradaInvestigador
{
	enum ResultadoOperacionEntrada res;
	long numeroLocker;
};

struct IPuertaMsg_NotificacionSalidaPersona
{
	enum ResultadoOperacionSalida res;
};

struct IPuertaMsg_NotificacionSalidaInvestigador
{
	enum ResultadoOperacionSalida res;
	long pertenencias;
};

struct IPuertaMsg
{
	long mtype;
	enum IPuertaOp op;
	union {
		struct IPuertaMsg_SolicEntrarMuseoPersona semp;
		struct IPuertaMsg_SolicEntrarMuseoInvestigador semi;
		struct IPuertaMsg_SolicSalirMuseoPersona ssmp;
		struct IPuertaMsg_SolicSalirMuseoInvestigador ssmi;
		struct IPuertaMsg_NotificacionEntradaPersona nep;
		struct IPuertaMsg_NotificacionEntradaInvestigador nei;
		struct IPuertaMsg_NotificacionSalidaPersona nsp;
		struct IPuertaMsg_NotificacionSalidaInvestigador nsi;
	} msg;
};

#endif
