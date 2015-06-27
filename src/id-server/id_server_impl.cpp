#include "id_server.h"
#include <algorithm>
#include <limits>
#include <vector>

// Lista de identificadores libres
std::vector<long> libres;
// Primer identificador válido
long first;
// Proximo identificador a asignar si no hay libres
long next;

ids_response *
obtener_id_1_svc(void *argp, struct svc_req *rqstp)
{
	static ids_response  result;

	if (!libres.empty()) {
		result.type = IDS_SUCCESS;
		result.ids_response_u.id = libres.back();
		libres.pop_back();
	} else if (next < std::numeric_limits<long>::max()) {
		result.type = IDS_SUCCESS;
		result.ids_response_u.id = next++;
	} else {
		result.type = IDS_ERROR;
	}

	return &result;
}

void *
devolver_id_1_svc(long *argp, struct svc_req *rqstp)
{
	static char * result;
	long id = *argp;

	// No hay que poner basura en la lista de libres
	if (id >= first
			&& std::find (libres.begin (), libres.end (), id)
	              == libres.end ()) {
		libres.push_back(id);
	}

	return (void *) &result;
}
