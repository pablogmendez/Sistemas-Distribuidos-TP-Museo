#ifndef BROKERMSG_H_
#define BROKERMSG_H_

#include <stdint.h>

const int BROKER_READERS_PORT = 10001;
const int BROKER_WRITERS_PORT = 10002;

enum BrokerMsgOp
{
	// Mensajes de IPersona, desde y hacia las puertas
	BMO_SOLIC_PROXIMA_OPERACION            = 1   ,
	BMO_OP_SOLIC_ENTRAR_MUSEO_PERSONA            ,
	BMO_OP_SOLIC_ENTRAR_MUSEO_INVESTIGADOR       ,
	BMO_OP_SOLIC_SALIR_MUSEO_PERSONA             ,
	BMO_OP_SOLIC_SALIR_MUSEO_INVESTIGADOR        ,
	BMO_OP_NOTIFICAR_CIERRE_MUSEO                ,
	BMO_NOTIF_ENTRADA_PERSONA                    ,
	BMO_NOTIF_ENTRADA_INVESTIGADOR               ,
	BMO_NOTIF_SALIDA_PERSONA                     ,
	BMO_NOTIF_SALIDA_INVESTIGADOR                ,

	// Mensajes hacia el broker
	// Lector envía su id al broker
	BMO_BROKER_SET_ID                      = 1000
};

struct BrokerMsg
{
	int32_t dstId;
	int32_t srcId;
	int32_t op;
	int32_t param_a;
	int32_t param_b;
};

#endif /* BROKERMSG_H_ */
