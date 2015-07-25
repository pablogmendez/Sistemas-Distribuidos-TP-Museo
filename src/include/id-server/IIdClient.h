#ifndef ID_SERVER_IIDCLIENT_H
#define ID_SERVER_IIDCLIENT_H

#include <stdexcept>

class IIdClient
{
	private:
		class Impl;
		Impl* pImpl;
	public:
		enum Rangos
		{
			R_PUERTA  ,
			R_IMUSEO  ,
			R_PERSONA
		};

		IIdClient (const char* server_name) throw (std::runtime_error);
		~IIdClient ();
		long obtenerId (Rangos rango) throw (std::runtime_error);
		void devolverId (long id) throw (std::runtime_error);
};

#endif /* ID_SERVER_IIDCLIENT_H */
