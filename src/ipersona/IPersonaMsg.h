#ifndef IPERSONA_IPERSONAMSG_H
#define IPERSONA_IPERSONAMSG_H

#include <icommon/resultado.h>

enum IPersonaOp
{
	// Mantener los valores de acuerdo a MuseoMSG::OP
	OP_SOLIC_ENTRAR_MUSEO_PERSONA      = 1,
	OP_SOLIC_ENTRAR_MUSEO_INVESTIGADOR    ,
	OP_SOLIC_SALIR_MUSEO_PERSONA          ,
	OP_SOLIC_SALIR_MUSEO_INVESTIGADOR     ,
	OP_NOTIFICAR_CIERRE_MUSEO             ,

	NOTIF_ENTRADA_PERSONA                 ,
	NOTIF_ENTRADA_INVESTIGADOR            ,
	NOTIF_SALIDA_PERSONA                  ,
	NOTIF_SALIDA_INVESTIGADOR             ,
	///////////////////////////////////////

	SOLIC_PROXIMA_OPERACION            = 1000,
};

static inline const char* strIPersonaOp (IPersonaOp op)
{
	#define RET_OP_STR(o) case o : return "IPersonaOp::"#o
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
		RET_OP_STR (SOLIC_PROXIMA_OPERACION);
		default: return "desconocida";
	}
	#undef RET_OP_STR
}

struct IPersonaMsg_SolicProximaOperacion
{
	long rtype;
};

struct IPersonaMsg_OpSolicEntrarMuseoPersona
{
	long idOrigen;
};

struct IPersonaMsg_OpSolicEntrarMuseoInvestigador
{
	long idOrigen;
	long pertenencias;
};

struct IPersonaMsg_OpSolicSalirMuseoPersona
{
	long idOrigen;
};

struct IPersonaMsg_OpSolicSalirMuseoInvestigador
{
	long idOrigen;
	long numeroLocker;
};

struct IPersonaMsg_OpNotificarCierre
{

};

struct IPersonaMsg_NotifEntradaPersona
{
	enum ResultadoOperacionEntrada res;
};

struct IPersonaMsg_NotifEntradaInvestigador
{
	enum ResultadoOperacionEntrada res;
	long numeroLocker;
};

struct IPersonaMsg_NotifSalidaPersona
{
	enum ResultadoOperacionSalida res;
};

struct IPersonaMsg_NotifSalidaInvestigador
{
	enum ResultadoOperacionSalida res;
	long pertenencias;
};

struct IPersonaMsg
{
	long mtype;
	enum IPersonaOp op;
	union {
		// solicitudes
		struct IPersonaMsg_SolicProximaOperacion spo;

		// operaciones obtenidas
		struct IPersonaMsg_OpSolicEntrarMuseoPersona osemp;
		struct IPersonaMsg_OpSolicEntrarMuseoInvestigador osemi;
		struct IPersonaMsg_OpSolicSalirMuseoPersona ossmp;
		struct IPersonaMsg_OpSolicSalirMuseoInvestigador ossmi;
		struct IPersonaMsg_OpNotificarCierre onc;

		// resultados de las operaciones
		struct IPersonaMsg_NotifEntradaPersona nep;
		struct IPersonaMsg_NotifEntradaInvestigador nei;
		struct IPersonaMsg_NotifSalidaPersona nsp;
		struct IPersonaMsg_NotifSalidaInvestigador nsi;
	} msg;
};

#endif
