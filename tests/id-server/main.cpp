#include <id-server/IIdClient.h>
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <utils/System.h>
#include <vector>

int main (int argc, char** argv)
{
	pid_t srv_pid;
	std::vector<const char*> srv_args;
	const char* srv_bin = "../../src/id-server/id-server";
	srv_args.push_back (srv_bin);
	srv_args.push_back ("1001");
	srv_args.push_back (NULL);
	srv_pid = System::spawn (srv_bin, srv_args);
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
