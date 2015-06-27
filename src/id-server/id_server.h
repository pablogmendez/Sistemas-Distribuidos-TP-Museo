/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _ID_SERVER_H_RPCGEN
#define _ID_SERVER_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


enum ids_response_type {
	IDS_SUCCESS = 0,
	IDS_ERROR = 1,
};
typedef enum ids_response_type ids_response_type;

struct ids_response {
	ids_response_type type;
	union {
		long id;
	} ids_response_u;
};
typedef struct ids_response ids_response;

#define ID_SERVER 0x2E8A16D6
#define ID_SERVER_VERSION 1

#if defined(__STDC__) || defined(__cplusplus)
#define OBTENER_ID 1
extern  ids_response * obtener_id_1(void *, CLIENT *);
extern  ids_response * obtener_id_1_svc(void *, struct svc_req *);
#define DEVOLVER_ID 2
extern  void * devolver_id_1(long *, CLIENT *);
extern  void * devolver_id_1_svc(long *, struct svc_req *);
extern int id_server_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define OBTENER_ID 1
extern  ids_response * obtener_id_1();
extern  ids_response * obtener_id_1_svc();
#define DEVOLVER_ID 2
extern  void * devolver_id_1();
extern  void * devolver_id_1_svc();
extern int id_server_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_ids_response_type (XDR *, ids_response_type*);
extern  bool_t xdr_ids_response (XDR *, ids_response*);

#else /* K&R C */
extern bool_t xdr_ids_response_type ();
extern bool_t xdr_ids_response ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_ID_SERVER_H_RPCGEN */
