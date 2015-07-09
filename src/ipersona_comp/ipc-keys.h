#ifndef IPC_KEYS_H_
#define IPC_KEYS_H_

// FIXME: unificar con constantes de IPersona
// Parámetros de ftok para cola de comunicación con interfaz
const char* const PATH_INT = "/ipersona.mq";
const char        MQ_INT   = 'A';
////////////////////////////////////

// Parámetros de ftok para colas de entradas y salidas
const char* const PATH_MQS = "/ipersona_comp.ipc";
const char        SHM_MQS  = 'A';
const char        MTX_MQS  = 'B';
const char        SEM_HAY  = 'C';
const char        MQ_SESP  = 'D';
const char        MQ_SEST  = 'E';
const char        MQ_ENTR  = 'F';

#endif /* IPC_KEYS_H_ */
