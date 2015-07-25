#!/bin/sh
if [ "x$1" = xclobber ]; then
	FILES="id_server.h id_server_xdr.c id_server_clnt.c id_server_svc.c"
	for file in $FILES; do rm $file; done
fi

rpcgen -h -o id_server.h id_server.x
rpcgen -c -o id_server_xdr.c id_server.x
rpcgen -l -o id_server_clnt.c id_server.x
rpcgen -m -o id_server_svc.c id_server.x
# para generar este ultimo con los main:
# rpcgen -s udp -s tcp -o id_server_svc.c id_server.x
