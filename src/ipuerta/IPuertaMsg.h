#ifndef IPUERTA_IPUERTAMSG_H
#define IPUERTA_IPUERTAMSG_H

enum IPuertaOp
{
	RECORRER_MUSEO             = 1,
	INICIAR_RECORRIDO_ESPECIAL = 2,
	RECORRIDO_COMPLETADO       = 3
};

struct IPuertaMsg_RecorrerMuseo
{
	long rtype;
	long idPuertaEntrada;
	long idPuertaSalida;
	long tiempoRecorrido;
};

struct IPuertaMsg_IniciarRecorridoEspecial
{
	long rtype;
	long idPuerta;
	long tiempoRecorrido;
	/* TODO: pertenencias */
};

struct IPuertaMsg_RecorridoCompletado { };

struct IPuertaMsg
{
	long mtype;
	enum IPuertaOp op;
	union {
		struct IPuertaMsg_RecorrerMuseo rm;
		struct IPuertaMsg_IniciarRecorridoEspecial re;
		struct IPuertaMsg_RecorridoCompletado rc;
	} msg;
};

#endif
