#ifndef IPERSONA_OPERACION_H
#define IPERSONA_OPERACION_H

enum Tipo
{
	SOLIC_ENTRAR_MUSEO_PERSONA       = 1,
	SOLIC_ENTRAR_MUSEO_INVESTIGADOR     ,
	SOLIC_SALIR_MUSEO_PERSONA           ,
	SOLIC_SALIR_MUSEO_INVESTIGADOR      ,
	NOTIFICAR_CIERRE_MUSEO
};

static inline const char* strOperacionTipo (Tipo op)
{
	#define RET_OP_STR(o) case o : return "OperacionTipo::"#o
	switch (op) {
		RET_OP_STR (SOLIC_ENTRAR_MUSEO_PERSONA);
		RET_OP_STR (SOLIC_ENTRAR_MUSEO_INVESTIGADOR);
		RET_OP_STR (SOLIC_SALIR_MUSEO_PERSONA);
		RET_OP_STR (SOLIC_SALIR_MUSEO_INVESTIGADOR);
		RET_OP_STR (NOTIFICAR_CIERRE_MUSEO);
		default: return "desconocida";
	}
	#undef RET_OP_STR
}

struct Operacion_SolicEntrarMuseoPersona
{
	long idOrigen;
	bool lleno;
};

struct Operacion_SolicEntrarMuseoInvestigador
{
	long idOrigen;
	long pertenencias;
	bool lleno;
};

struct Operacion_SolicSalirMuseoPersona
{
	long idOrigen;
	bool lleno;
};

struct Operacion_SolicSalirMuseoInvestigador
{
	long idOrigen;
	long numeroLocker;
	bool lleno;
};

struct Operacion_NotificarCierreMuseo
{

};

struct Operacion
{
	enum Tipo tipo;
	union {
		struct Operacion_SolicEntrarMuseoPersona semp;
		struct Operacion_SolicEntrarMuseoInvestigador semi;
		struct Operacion_SolicSalirMuseoPersona ssmp;
		struct Operacion_SolicSalirMuseoInvestigador ssmi;
		struct Operacion_NotificarCierreMuseo ncm;
	} op;
};

static inline
void set_lleno (Operacion& o, bool lleno)
{
	switch (o.tipo) {
		case SOLIC_ENTRAR_MUSEO_PERSONA:
			o.op.semp.lleno = lleno;
			break;
		case SOLIC_ENTRAR_MUSEO_INVESTIGADOR:
			o.op.semi.lleno = lleno;
			break;
		case SOLIC_SALIR_MUSEO_PERSONA:
			o.op.ssmp.lleno = lleno;
			break;
		case SOLIC_SALIR_MUSEO_INVESTIGADOR:
			o.op.ssmi.lleno = lleno;
			break;
		default:
			throw "set_lleno: tipo de operación inválida.";
	}
}

#endif
