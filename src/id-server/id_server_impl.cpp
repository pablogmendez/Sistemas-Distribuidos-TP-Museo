#include "id_server.h"
#include "id_rango.h"
#include <algorithm>
#include <limits>
#include <stdexcept>

std::map<ids_rango, estado_rango> rangos;

ids_response *
obtener_id_1_svc(ids_rango *argp, struct svc_req *rqstp)
{
	static ids_response  result;

	try {
		estado_rango& st = rangos.at (*argp);

		if (!st.libres.empty()) {
			result.type = IDS_SUCCESS;
			result.ids_response_u.id = st.libres.back();
			st.libres.pop_back();
		} else if (st.next < st.max) {
			result.type = IDS_SUCCESS;
			result.ids_response_u.id = st.next++;
		} else {
			result.type = IDS_ERROR;
		}
	} catch (std::exception& e) {
		result.type = IDS_ERROR;
	}

	return &result;
}

void *
devolver_id_1_svc(long *argp, struct svc_req *rqstp)
{
	static char * result;
	long id = *argp;

	std::map<ids_rango, estado_rango>::iterator it = rangos.begin ();
	for (; it != rangos.end (); ++it) {
		estado_rango& st = it->second;
		// No hay que poner basura en la lista de libres
		if (id >= st.first && id < st.max
				&& std::find (st.libres.begin (), st.libres.end (), id)
					== st.libres.end ()) {
			st.libres.push_back (id);
			break;
		}
	}

	return (void *) &result;
}
