#include "Destructor.h"
#include "../Logger/Logger.h"
#include "../Utils/ParserConfiguracion.h"
#include "../Utils/Utils.h"
#include "../IPC/SignalHandler.h"
#include "Constantes.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>

Destructor::Destructor(): mtxContMuseo(NULL), mtxMuseoAbierto(NULL){
	colaEsp = NULL;
	colaNorm = NULL;
}
	
Destructor::~Destructor() {
}
	
int Destructor::tomarIPC() {
	LOG("DESTRUCTOR: INICIALIZANDO RECURSOS");
	LOG("LANZADOR: INICIALIZANDO RECURSOS");
		// Creo la MC mcMuseoAbierto
		shmMuseoAbierto.crear(recShmMuseoAbierto, 'm');
		LOG("DESTRUCTOR: SHM MuseoAbierto CREADA");

		// Creo  la MC mcContadorMuseo
		shmContadorMuseo.crear(recShmContMuseo, 'm');
		LOG("DESTRUCTOR: SHM ContadorMuseo CREADA");

		// Creo el mutex mutexContadorMuseo
		mtxContMuseo = new Semaforo(recMtxContMuseo.c_str(), 'a', -1);
		LOG("DESTRUCTOR: MUTEX ContadorMuseo CREADO");

		// Creo el mutex mutexMuseoAbierto
		mtxMuseoAbierto= new Semaforo(recMtxMuseoAbierto.c_str(), 'a', -1);
		LOG("DESTRUCTOR: MUTEX MuseoAbierto CREADO");

		colaNorm = new Cola<persona_t>(recColaNorm, 'a');
		LOG("DESTRUCTOR: ColaNormal CREADA");
		colaEsp = new Cola<persona_t>(recColaEsp, 'a');
		LOG("DESTRUCTOR: ColaEspecial CREADA");

		ParserConfiguracion parser;
		parser.cargarConfiguracion(configuracion);

		LOG("DESTRUCTOR: Creando IPC's exclusivos para las puertas");

		for(int i = 1; i <= Utils::string2int(parser.getValor("puertas")); ++i) {
			LOG("DESTRUCTOR: Creando IPC's para la puerta %d", i);
			Semaforo *mtxPuerta = new Semaforo(recMtxPuerta, i, -1);
			LOG("DESTRUCTOR: mtxPuerta CREADO");
			Semaforo *mtxVisitante = new Semaforo(recMtxVisitante, i, -1);
			LOG("DESTRUCTOR: mtxVisitante CREADO");
			Semaforo *mtxContNorm = new Semaforo(recMtxContNorm, i, -1);
			LOG("DESTRUCTOR: mtxContNorm CREADO");
			Semaforo *mtxContEsp = new Semaforo(recMtxContEsp, i, -1);
			LOG("DESTRUCTOR: mtxContEsp CREADO");
			MemoriaCompartida<int> *shmContNorm = new MemoriaCompartida<int>();
			shmContNorm->crear(recShmContNorm, i);
			LOG("DESTRUCTOR: shmContNorm CREADA");
			MemoriaCompartida<int> *shmContEsp = new MemoriaCompartida<int>();
			shmContEsp->crear(recShmContEsp, i);
			LOG("DESTRUCTOR: shmContEsp CREADA");

			colaMtx.push(mtxPuerta);
			colaMtx.push(mtxVisitante);
			colaMtx.push(mtxContNorm);
			colaMtx.push(mtxContEsp);

			colaShm.push(shmContEsp);
			colaShm.push(shmContNorm);
		}

		return 0;
}

int Destructor::liberarIPC() {

	LOG("DESTRUCTOR: DESTRUYENDO RECURSOS");

	shmMuseoAbierto.liberar();
	LOG("DESTRUCTOR: shmMuseoAbierto DESTRUIDA");

	shmContadorMuseo.liberar();
	LOG("DESTRUCTOR: shmContadorMuseo DESTRUIDA");

	mtxContMuseo->eliminar();
	delete mtxContMuseo;
	LOG("DESTRUCTOR: mtxContMuseo DESTRUIDO");

	mtxMuseoAbierto->eliminar();
	delete mtxMuseoAbierto;
	LOG("DESTRUCTOR: mtxMuseoAbierto DESTRUIDO");

	colaEsp->destruir();
	colaNorm->destruir();
	delete colaEsp;
	LOG("DESTRUCTOR: colaEspecial DESTRUIDA");
	delete colaNorm;
	LOG("DESTRUCTOR: colaNormal DESTRUIDA");

	LOG("DESTRUCTOR: DESTRUYENDO IPC's exclusivos de las puertas");

	while(!colaShm.empty()) {
		colaShm.front()->liberar();
		delete colaShm.front();
		colaShm.pop();
	}

	LOG("DESTRUCTOR: SHM DESTRUIDAS");

	while(!colaMtx.empty()) {
		colaMtx.front()->eliminar();
		delete colaMtx.front();
		colaMtx.pop();
	}
	LOG("DESTRUCTOR: MTX DESTRUIDOS");
	return 0;
}
	
int Destructor::pararProcesos() {
	std::ifstream ifs;
	std::string strpid;
	int pid;
	LOG("DESTRUCTOR: Deteniendo procesos ...");

	ifs.open(perDat.c_str(), std::ifstream::in);
	while(ifs >> strpid) {
		pid = Utils::string2int(strpid);
		LOG("DESTRUCTOR: Deteniendo pid PERSONA: %d ...", pid);
		kill(pid, SIGINT);
		waitpid(pid, NULL, 0);
		LOG("DESTRUCTOR: pid: %d ---> DETENIDO", pid);
	}
	ifs.close();

	return 0;
}
