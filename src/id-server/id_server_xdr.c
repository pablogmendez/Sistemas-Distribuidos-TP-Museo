/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "id_server.h"

bool_t
xdr_ids_response_type (XDR *xdrs, ids_response_type *objp)
{
	register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ids_response (XDR *xdrs, ids_response *objp)
{
	register int32_t *buf;

	 if (!xdr_ids_response_type (xdrs, &objp->type))
		 return FALSE;
	switch (objp->type) {
	case IDS_SUCCESS:
		 if (!xdr_long (xdrs, &objp->ids_response_u.id))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_ids_rango (XDR *xdrs, ids_rango *objp)
{
	register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}
