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
	srv_args.push_back ("1001");
	srv_args.push_back (NULL);
	srv_pid = System::spawn (srv_bin.c_str (), srv_args);
	System::check (srv_pid);

	// Espera "un rato" para que el server inicialice
	sleep (3);

	int err = 0;
	try {
		IIdClient ids ("localhost");
		long myId = ids.obtenerId ();
		if (myId != 1001) {
			std::cerr << "Se obtuvo el id "
					<< myId << ". Se esperaba 1001"
					<< std::endl;
			err = 1;
			goto out;
		}
		ids.devolverId (myId);
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
