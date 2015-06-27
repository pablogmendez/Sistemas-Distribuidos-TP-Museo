#include "id_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Parámetros del servicio de identificadores
extern long first;
extern long next;
/////////////////////

void inicializar_servicio (int argc, char **argv)
{
	first = 1;
	if (argc > 1) {
		char* endptr;
		first = strtol (argv[1], &endptr, 10);
		if (!(*argv[1] != 0 && *endptr == 0) || first <= 0) {
			fprintf (stderr, "El primer identificador asignable debe ser"
							 " un número mayor a 0.\n");
			exit (1);
		}
	} else {
		fprintf (stderr, "WARNING: el servidor se inicio sin parámetros,"
				" se entregaran identificadores a partir de 1.\n");
	}
	next = first;
}

// Funciones registradas por el main
extern "C" {
	void
	id_server_1(struct svc_req *rqstp, register SVCXPRT *transp);
}
/////////////////////

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	inicializar_servicio (argc, argv);

	pmap_unset (ID_SERVER, ID_SERVER_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s\n", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, ID_SERVER, ID_SERVER_VERSION, id_server_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s\n", "unable to register (ID_SERVER, ID_SERVER_VERSION, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s\n", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, ID_SERVER, ID_SERVER_VERSION, id_server_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s\n", "unable to register (ID_SERVER, ID_SERVER_VERSION, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s\n", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
