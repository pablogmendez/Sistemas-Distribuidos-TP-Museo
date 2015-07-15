#include <id-server/IIdClient.h>
#include "id_server.h"

class IIdClient::Impl
{
	public:
		CLIENT* clnt;

		Impl (const char* host)
		{
			clnt = clnt_create (host, ID_SERVER, ID_SERVER_VERSION, "udp");
			if (clnt == NULL) {
				char* err = clnt_spcreateerror (host);
				throw std::runtime_error (err);
			}
		}

		~Impl ()
		{
			clnt_destroy (clnt);
		}
};

IIdClient::IIdClient (const char* server_name) throw (std::runtime_error)
{
	pImpl = new Impl (server_name);
}

IIdClient::~IIdClient ()
{
	delete pImpl;
}

long IIdClient::obtenerId (Rangos rango) throw (std::runtime_error)
{
	ids_response* result_1;
	ids_rango obtener_id_1_arg = static_cast<ids_rango> (rango);
	result_1 = obtener_id_1(&obtener_id_1_arg, pImpl->clnt);
	if (result_1 == (ids_response *) NULL) {
		char* err = clnt_sperror (pImpl->clnt,
				"IIdClient::obtenerId: falló RPC");
		throw std::runtime_error (err);
	}

	if (result_1->type == IDS_SUCCESS) {
		return result_1->ids_response_u.id;
	}

	throw std::runtime_error ("El servidor no pudo asignar identificador");
}

void IIdClient::devolverId (long id) throw (std::runtime_error)
{
	void  *result_2;

	result_2 = devolver_id_1(&id, pImpl->clnt);
	if (result_2 == (void *) NULL) {
		char* err = clnt_sperror (pImpl->clnt,
				"IIdClient::devolverId: falló RPC");
		throw std::runtime_error (err);
	}
}
