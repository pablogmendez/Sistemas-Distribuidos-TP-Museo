#include "ArgParser.h"
#include <fstream>
#include <iostream>
#include <ipersona_comp/ipc-keys.h>
#include <libgen.h>
#include <signal.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <utils/System.h>

std::string calcularRutaMQ (const char* arg0, const char* mq)
{
	char* tmp = strdup (arg0);
	char* dir = dirname (tmp);
	std::string ruta = dir;
	ruta.append ("/../../resources");
	ruta.append (mq);
	free (tmp);
	return ruta;
}

int esta_corriendo (const std::string& id, const std::string& spid)
{
	pid_t pid;
	std::istringstream iss (spid);
	if (!(iss >> pid)) {
		std::cerr << "esta_corriendo: recibió pid '"
			<< spid << "' inválido." << std::endl;
		// Ante errores se asume que está corriendo.
		return 1;
	}

	if (kill (pid, 0) == 0) {
		std::cerr << "La puerta con id local '"
			<< id << "' aún está corriendo con pid '"
			<< spid << "'." << std::endl;
		return 1;
	}

	if (errno != ESRCH) {
		SystemErrorException e;
		std::cerr << "esta_corriendo: error al detectar proceso"
			<< "\n\t" << e.what () << std::endl;
		return 1;
	}

	// No está corriendo
	return 0;
}

void mqrm (const std::string& path, char letra)
{
	try {
		System::mqrm (path, letra);
	} catch (SystemErrorException& e) {
		char hex[5] = "";
		sprintf (hex, "0x%X", letra);
		std::cerr << "mqrm (" << path << ", " << hex << ")\n  "
			<< e.what () << std::endl;
	}
}

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	int corriendo = 0;
	
	std::string id;
	std::ifstream ifs;
	ifs.open (args.sesion ().c_str ());
	if (ifs.fail ()) {
		std::cerr << "Error al abrir el archivo: "
			<< args.sesion () << std::endl;
		return 1;
	}
	
	while (getline (ifs, id, ':')) {
		std::string pid;
		if (!getline (ifs, pid)) {
			std::cerr << "Archivo de sesion inválido.\n\t"
				<< "No se encontro pid para el id " << id
				<< std::endl;
			return 1;
		}
		corriendo += esta_corriendo (id, pid);
	}
	
	ifs.close ();

	if (corriendo > 0) {
		std::cerr << "No se pueden remover IPCs: hay puertas corriendo."
			<< std::endl;
		return 1;
	}

	std::string mqint = calcularRutaMQ (argv[0], PATH_INT);
	std::string mqcomp = calcularRutaMQ (argv[0], PATH_MQS);

	mqrm (mqint, MQ_INT);
	mqrm (mqcomp, MQ_SESP);
	mqrm (mqcomp, MQ_SEST);
	mqrm (mqcomp, MQ_ENTR);
	return 0;
}
