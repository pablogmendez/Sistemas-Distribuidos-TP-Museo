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
};

struct Operacion_SolicEntrarMuseoInvestigador
{
	long idOrigen;
	long pertenencias;
};

struct Operacion_SolicSalirMuseoPersona
{
	long idOrigen;
};

struct Operacion_SolicSalirMuseoInvestigador
{
	long idOrigen;
	long numeroLocker;
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

#endif
