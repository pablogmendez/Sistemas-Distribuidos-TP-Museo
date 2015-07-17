#ifndef MUSEO_MSG_H
#define MUSEO_MSG_H

struct MuseoMSG
{
	enum OP
	{
		// Mensajes hacia IPersona, desde las personas
		SOLIC_ENTRAR_MUSEO_PERSONA      = 1,
		SOLIC_ENTRAR_MUSEO_INVESTIGADOR    ,
		SOLIC_SALIR_MUSEO_PERSONA          ,
		SOLIC_SALIR_MUSEO_INVESTIGADOR     ,
		// Mensjes hacia IPersona, desde el administrador
		NOTIFICAR_CIERRE_MUSEO             ,
		// Mensajes hacia IPuerta, desde las puertas
		NOTIF_ENTRADA_PERSONA              ,
		NOTIF_ENTRADA_INVESTIGADOR         ,
		NOTIF_SALIDA_PERSONA               ,
		NOTIF_SALIDA_INVESTIGADOR          ,
	};

	OP op;
	long param_a;
	long param_b;
};

#endif /* MUSEO_MSG_H*/
