#ifndef IPC_KEYS_H_
#define IPC_KEYS_H_

// Según `man ftok` se usan solo 8 bits
const int PROJ_ID_MAX = (1 << 8) - 1;

// FIXME: unificar con constantes de IPersona
// Parámetros de ftok para cola de comunicación con interfaz
const char* const PATH_INT = "/ipersona.mq";
const char        MQ_INT   = 1;

static inline int int_calc_proj_id (int id_local, char recurso)
{
	return (id_local - 1) + recurso;
}
////////////////////////////////////

// Parámetros de ftok para colas de entradas y salidas
const char* const PATH_MQS = "/ipersona_comp.ipc";
// Recursos compartidos por todas las puertas
// que corren en la misma máquina.
const char        MQ_SESP  = 1;
const char        MQ_SEST  = 2;
const char        MQ_ENTR  = 3;
// Recursos que se asignan por puerta y son
// propios de ellas.
const char        SHM_MQS  = 4;
const char        MTX_MQS  = 5;
const char        SEM_HAY  = 6;
// Mantener actualizados MQS_MIN y MQS_MAX con
// los extremos del rango de recursos propios de
// cada puerta.
const int         COMP_MIN  = SHM_MQS;
const int         COMP_MAX  = SEM_HAY;
const int         COMP_SIZE = (COMP_MAX - COMP_MIN + 1);

static inline int comp_calc_proj_id (int id_local, char recurso)
{
	const int offset_x_id = (id_local - 1) * COMP_SIZE;
	const int offset_x_recursos_comp = COMP_MIN;
	const int numero_recurso_propio = (recurso - COMP_MIN);
	return offset_x_recursos_comp + offset_x_id + numero_recurso_propio;
}

#endif /* IPC_KEYS_H_ */
