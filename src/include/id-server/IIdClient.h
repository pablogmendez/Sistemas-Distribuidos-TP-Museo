#ifndef ID_SERVER_IIDCLIENT_H
#define ID_SERVER_IIDCLIENT_H

#include <stdexcept>

class IIdClient
{
	private:
		class Impl;
		Impl* pImpl;
	public:
		IIdClient (const char* server_name) throw (std::runtime_error);
		~IIdClient ();
		long obtenerId () throw (std::runtime_error);
		void devolverId (long id) throw (std::runtime_error);
};

#endif /* ID_SERVER_IIDCLIENT_H */
