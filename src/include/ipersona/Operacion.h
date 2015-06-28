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
