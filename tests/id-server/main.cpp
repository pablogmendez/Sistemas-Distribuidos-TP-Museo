#include <id-server/IIdClient.h>
#include <iostream>
#include <libgen.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <utils/System.h>
#include <vector>

std::string calcularRutaServidor (const char* arg0)
{
	char* tmp = strdup (arg0);
	char* dir = dirname (tmp);
	std::string ruta = dir;
	ruta.append("/../../src/id-server/id-server");
	free (tmp);
	return ruta;
}

int main (int argc, char** argv)
{
	pid_t srv_pid;
	std::vector<const char*> srv_args;
	std::string srv_bin = calcularRutaServidor (argv[0]);
	srv_args.push_back (srv_bin.c_str ());
	srv_args.push_back ("100");
	srv_args.push_back ("300");
	srv_args.push_back ("1001");
	srv_args.push_back (NULL);
	srv_pid = System::spawn (srv_bin.c_str (), srv_args);
	System::check (srv_pid);

	// Espera "un rato" para que el server inicialice
	sleep (3);

	int err = 0;
	try {
		IIdClient ids ("localhost");
		long myId = ids.obtenerId (IIdClient::R_PERSONA);
		if (myId != 1001) {
			std::cerr << "Se obtuvo el id "
					<< myId << ". Se esperaba 1001"
					<< std::endl;
			err = 1;
			goto out;
		}
		ids.devolverId (myId);

		// Verificar duplicados
		ids.devolverId (myId);
		long idA = ids.obtenerId (IIdClient::R_PERSONA);
		long idB = ids.obtenerId (IIdClient::R_PERSONA);
		if (idA == idB) {
			std::cerr << "Se duplicó el id "
					<< idA << std::endl;
			err = 1;
			goto out;
		}
		ids.devolverId (idA);
		ids.devolverId (idB);

		// Verificar el rango puerta
		myId = ids.obtenerId (IIdClient::R_PUERTA);
		if (myId != 100) {
			std::cerr << "Se obtuvo el id "
					<< myId << ". Se esperaba 100"
					<< std::endl;
			err = 1;
			goto out;
		}
		ids.devolverId (myId);

		// Verificar duplicados
		ids.devolverId (myId);
		idA = ids.obtenerId (IIdClient::R_PUERTA);
		idB = ids.obtenerId (IIdClient::R_PUERTA);
		if (idA == idB) {
			std::cerr << "Se duplicó el id "
					<< idA << std::endl;
			err = 1;
			goto out;
		}
		ids.devolverId (idA);
		ids.devolverId (idB);

		// Verificar el rango imuseo
		myId = ids.obtenerId (IIdClient::R_IMUSEO);
		if (myId != 300) {
			std::cerr << "Se obtuvo el id "
					<< myId << ". Se esperaba 300"
					<< std::endl;
			err = 1;
			goto out;
		}
		ids.devolverId (myId);

		// Verificar duplicados
		ids.devolverId (myId);
		idA = ids.obtenerId (IIdClient::R_IMUSEO);
		idB = ids.obtenerId (IIdClient::R_IMUSEO);
		if (idA == idB) {
			std::cerr << "Se duplicó el id "
					<< idA << std::endl;
			err = 1;
			goto out;
		}
		ids.devolverId (idA);
		ids.devolverId (idB);

		// Rango Inválido
		try {
			myId = ids.obtenerId (static_cast<IIdClient::Rangos> (73737));
			std::cerr << "Se obtuvo el id " << myId
				<< " para el rango inválido 73737" << std::endl;
			err = 1;
			goto out;
		} catch (std::runtime_error& e) {
			// OK
		}
	} catch (std::exception& e) {
		std::cerr << "Error al invocar servicio: "
				<< e.what ()
				<< std::endl;
		err = 1;
	}

out:
	kill (srv_pid, SIGINT);
	waitpid (srv_pid, NULL, 0);
	return err;
}
