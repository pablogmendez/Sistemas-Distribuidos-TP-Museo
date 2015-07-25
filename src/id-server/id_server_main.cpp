#include "ArgParser.h"
#include "id_rango.h"
#include "id_server.h"
#include <limits>
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <sys/socket.h>
#include <netinet/in.h>

void inicializar_servicio (int argc, char **argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	long puertas = args.offsetPuertas ();
	long imuseos = args.offsetIMuseo ();
	long personas = args.offsetPersonas ();

	rangos[R_PUERTA].first = puertas;
	rangos[R_PUERTA].next = puertas;
	rangos[R_PUERTA].max = imuseos;

	rangos[R_IMUSEO].first = imuseos;
	rangos[R_IMUSEO].next = imuseos;
	rangos[R_IMUSEO].max = personas;

	rangos[R_PERSONA].first = personas;
	rangos[R_PERSONA].next = personas;
	rangos[R_PERSONA].max = std::numeric_limits<long>::max ();

	if (puertas == 1) {
		fprintf (stderr,
			"WARNING: no hay rango de identificadores reservados.\n");
	}

	printf ("Servidor iniciado con los siguientes rangos:\n");
	printf ("\tR_PUERTA : %ld:%ld (%ld)\n",
		rangos[R_PUERTA].first,
		rangos[R_PUERTA].max,
		rangos[R_PUERTA].max - rangos[R_PUERTA].first);
	printf ("\tR_IMUSEO : %ld:%ld (%ld)\n",
		rangos[R_IMUSEO].first,
		rangos[R_IMUSEO].max,
		rangos[R_IMUSEO].max - rangos[R_IMUSEO].first);
	printf ("\tR_PERSONA: %ld:%ld (%ld)\n",
		rangos[R_PERSONA].first,
		rangos[R_PERSONA].max,
		rangos[R_PERSONA].max - rangos[R_PERSONA].first);
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
