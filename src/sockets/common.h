/*
Autor: Marelli Ricardo Guido
Padron: 79134

*/

#ifndef __COMMON_H__
#define __COMMON_H__

#define __SERVER_ADDR__ 7070
#define __CLIENT_ADDR__ 8080

#define __COMMON_PORT__ 5000
#define __TCP_MSG_LEN__ 3000
#define __TCP_MSG_DATA_LEN__ 3000
#define __PRINT_MSG_LEN__ 3000

/* 
PROTOCOL DEFINITION
*/
#define __MAX_OP_NUMBER__ 50

/* proto errors */
#define __PROTO_NO_OP__ -1000
#define __PROTO_BAD_NSEQ__ -1010
#define __PROTO_BAD_OP__ -1020
#define __PROTO_ERR__ -1030

/* protocol operations */
#define __PROTO_OP_INIT__ 0
#define __PROTO_OP_FIN__ 1
#define __PROTO_OP_REPLY__ 2

/* user operations */
#define __USER_OP_REVERT__ 10
#define __USER_OP_STATS__ 11

int printmsg(const char* format,...);

#endif
