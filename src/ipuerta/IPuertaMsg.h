#ifndef IPUERTA_IPUERTAMSG_H
#define IPUERTA_IPUERTAMSG_H

#include <icommon/resultado.h>

enum IPuertaOp
{
	OP_SOLIC_ENTRAR_MUSEO_PERSONA      = 1,
	OP_SOLIC_ENTRAR_MUSEO_INVESTIGADOR    ,
	OP_SOLIC_SALIR_MUSEO_PERSONA          ,
	OP_SOLIC_SALIR_MUSEO_INVESTIGADOR     ,
	OP_NOTIFICAR_CIERRE_MUSEO             ,

	NOTIF_ENTRADA_PERSONA                 ,
	NOTIF_ENTRADA_INVESTIGADOR            ,
	NOTIF_SALIDA_PERSONA                  ,
	NOTIF_SALIDA_INVESTIGADOR             ,
	NOTIF_DESCONEXION		      ,
	NOTIF_ECHAR_PERSONA		      
};

static inline const char* strIPuertaOp (IPuertaOp op)
{
	#define RET_OP_STR(o) case o : return "IPuertaOp::"#o
	switch (op) {
		RET_OP_STR (OP_SOLIC_ENTRAR_MUSEO_PERSONA);
		RET_OP_STR (OP_SOLIC_ENTRAR_MUSEO_INVESTIGADOR);
		RET_OP_STR (OP_SOLIC_SALIR_MUSEO_PERSONA);
		RET_OP_STR (OP_SOLIC_SALIR_MUSEO_INVESTIGADOR);
		RET_OP_STR (OP_NOTIFICAR_CIERRE_MUSEO);
		RET_OP_STR (NOTIF_ENTRADA_PERSONA);
		RET_OP_STR (NOTIF_ENTRADA_INVESTIGADOR);
		RET_OP_STR (NOTIF_SALIDA_PERSONA);
		RET_OP_STR (NOTIF_SALIDA_INVESTIGADOR);
		default: return "desconocida";
	}
	#undef RET_OP_STR
}

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

#define IPUERTAMSG_INITIALIZER {0, (IPuertaOp) 0, {0, 0}}

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
