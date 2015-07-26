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
		// Mensaje de desconexion
		NOTIF_DESCONEXION                  ,
		NOTIF_ECHAR_PERSONA	           ,
		// Mensajes hacia IPersona, desde otras puertas
		INDICAR_MUSEO_NO_LLENO            = 500,
	};

	OP op;
	long param_a;
	long param_b;
};

static inline const char* strMuseoMSGOP (MuseoMSG::OP op)
{
	#define RET_OP_STR(o) case o : return #o
	switch (op) {
		RET_OP_STR (MuseoMSG::SOLIC_ENTRAR_MUSEO_PERSONA);
		RET_OP_STR (MuseoMSG::SOLIC_ENTRAR_MUSEO_INVESTIGADOR);
		RET_OP_STR (MuseoMSG::SOLIC_SALIR_MUSEO_PERSONA);
		RET_OP_STR (MuseoMSG::SOLIC_SALIR_MUSEO_INVESTIGADOR);
		RET_OP_STR (MuseoMSG::NOTIFICAR_CIERRE_MUSEO);
		RET_OP_STR (MuseoMSG::NOTIF_ENTRADA_PERSONA);
		RET_OP_STR (MuseoMSG::NOTIF_ENTRADA_INVESTIGADOR);
		RET_OP_STR (MuseoMSG::NOTIF_SALIDA_PERSONA);
		RET_OP_STR (MuseoMSG::NOTIF_SALIDA_INVESTIGADOR);
		RET_OP_STR (MuseoMSG::NOTIF_DESCONEXION);
		RET_OP_STR (MuseoMSG::INDICAR_MUSEO_NO_LLENO);
		default: return "desconocida";
	}
	#undef RET_OP_STR
}

#endif /* MUSEO_MSG_H*/
