#include "Lanzador.h"
#include <unistd.h>
#include "../IPC/SignalHandler.h"
#include "../Logger/Logger.h"
#include "../Utils/ParserConfiguracion.h"
#include "../Utils/Utils.h"
#include "Constantes.h"
#include <fstream>
#include <string>
#include <iostream>

Lanzador::Lanzador(): mtxContMuseo(NULL), mtxMuseoAbierto(NULL), colaEsp(NULL), colaNorm(NULL) {
};
	
Lanzador::~Lanzador() {
}

int Lanzador::tomarIPC() {
	LOG("LANZADOR: CREANDO RECURSOS");
	// Creo la MC mcMuseoAbierto
	shmMuseoAbierto.crear(recShmMuseoAbierto, 'm');
	shmMuseoAbierto.escribir(false);
	LOG("LANZADOR: shmMuseoAbierto CREADA");
	
	// Creo  la MC mcContadorMuseo
	shmContadorMuseo.crear(recShmContMuseo, 'm');
	shmContadorMuseo.escribir(0);
	LOG("LANZADOR: shmContadorMuseo CREADA");
	
	// Creo el mutex mutexContadorMuseo
	mtxContMuseo = new Semaforo(recMtxContMuseo.c_str(), 'a', 1);
	LOG("LANZADOR: mtxContMuseo CREADO");
	
	// Creo el mutex mutexMuseoAbierto
	mtxMuseoAbierto= new Semaforo(recMtxMuseoAbierto.c_str(), 'a', 1);
	LOG("LANZADOR: mtxMuseoAbierto CREADO");
	
	colaNorm = new Cola<persona_t>(recColaNorm, 'a');
	LOG("LANZADOR: colaNorm CREADA");
	colaEsp = new Cola<persona_t>(recColaEsp, 'a');
	LOG("LANZADOR: colaEsp CREADA");

	ParserConfiguracion parser;
	parser.cargarConfiguracion(configuracion);

	LOG("LANZADOR: CREANDO IPC's exclusivos de las puertas");

	for(int i = 1; i <= Utils::string2int(parser.getValor("puertas")); ++i) {
		LOG("LANZADOR: CREANDO IPC's exclusivos de la puerta %d", i);
		Semaforo *mtxPuerta = new Semaforo(recMtxPuerta, i, 0);
		LOG("LANZADOR: CREANDO mtxPuerta");
		Semaforo *mtxVisitante = new Semaforo(recMtxVisitante, i, 0);
		LOG("LANZADOR: CREANDO mtxVisitante");
		Semaforo *mtxContNorm = new Semaforo(recMtxContNorm, i, 1);
		LOG("LANZADOR: CREANDO mtxContNorm");
		Semaforo *mtxContEsp = new Semaforo(recMtxContEsp, i, 1);
		LOG("LANZADOR: CREANDO mtxContEsp");
		MemoriaCompartida<int> *shmContNorm = new MemoriaCompartida<int>();
		shmContNorm->crear(recShmContNorm, i);
		LOG("LANZADOR: CREANDO shmContNorm");
		MemoriaCompartida<int> *shmContEsp = new MemoriaCompartida<int>();
		shmContEsp->crear(recShmContEsp, i);
		LOG("LANZADOR: CREANDO shmContEsp");

		colaMtx.push(mtxPuerta);
		colaMtx.push(mtxVisitante);
		colaMtx.push(mtxContNorm);
		colaMtx.push(mtxContEsp);

		colaShm.push(shmContEsp);
		colaShm.push(shmContNorm);
	}

	return 0;
}

int Lanzador::liberarIPC() {
	LOG("LANZADOR: LIBERANDO RECURSOS");

	shmMuseoAbierto.liberar();
	LOG("LANZADOR: shmMuseoAbierto LIBERADA");

	shmMuseoAbierto.liberar();
	LOG("LANZADOR: shmMuseoAbierto LIBERADA");

	delete mtxContMuseo;
	LOG("LANZADOR: mtxContMuseo LIBERADO");

	delete mtxMuseoAbierto;
	LOG("LANZADOR: mtxMuseoAbierto LIBERADO");

	delete colaEsp;
	LOG("LANZADOR: colaEsp LIBERADA");
	delete colaNorm;
	LOG("LANZADOR: colaNorm LIBERADA");

	LOG("LANZADOR: LIBERANDO los IPC's exclusivos de las puertas");
	while(!colaShm.empty()) {
		colaShm.front()->liberar();
		delete colaShm.front();
		colaShm.pop();
	}

	LOG("LANZADOR: SHM LIBERADAS");

	while(!colaMtx.empty()) {
		delete colaMtx.front();
		colaMtx.pop();
	}

	LOG("LANZADOR: MTX LIBERADOS");

	return 0;
}

int Lanzador::iniciarProcesos() {
	ParserConfiguracion parserConfiguracion;
	std::ofstream ofs(perDat.c_str(), std::ofstream::out);
	
	parserConfiguracion.cargarConfiguracion(configuracion);
	
	srand((unsigned)time(NULL));
	sleep(5);

	LOG("LANZADOR: Creando PERSONAS ...");
	// Lanzador de personas
	int personasNormales = Utils::string2int(parserConfiguracion.getValor("personasNormales"));
	int investigadores = Utils::string2int(parserConfiguracion.getValor("investigadores"));
	int tiempoAdormir = Utils::string2int(parserConfiguracion.getValor("tiempoEntreIngresos"));
	int personasNormalesIngresadas = 0;
	int investigadoresIngresados = 0;
	
	LOG("LANZADOR: PERSONAS NORMALES configuradas: %d", personasNormales);
	LOG("LANZADOR: INVESTIGADORES configurados: %d", investigadores);
	while((personasNormalesIngresadas < personasNormales) || (investigadoresIngresados < investigadores)) {
		LOG("LANZADOR: Se inicia una nueva instancia de creacion de personas");
		int tipoAingresar = ((rand()%2) + 1);
		if((tipoAingresar == 1) && (personasNormalesIngresadas < personasNormales)) { // Es una persona normal
			LOG("LANZADOR: PERSONAS NORMALES ingresadas hasta el momento: %d", personasNormalesIngresadas);
			int aCrear = (rand()%(personasNormales - personasNormalesIngresadas)) + 1;
			int creadas = 0;
			LOG("LANZADOR: PERSONAS NORMALES  a crear: %d", aCrear);
			crearPersonas(aCrear, creadas, tipoAingresar, ofs);
			personasNormalesIngresadas += aCrear;
		}
		else if((tipoAingresar == 2) && (investigadoresIngresados < investigadores)) { // Es un investigador
			LOG("LANZADOR: INVESTIGADORES ingresados hasta el momento: %d", investigadoresIngresados);
			int aCrear = (rand()%(investigadores - investigadoresIngresados)) + 1;
			int creadas = 0;
			LOG("LANZADOR: INVESTIGADORES a crear: %d", aCrear);
			crearPersonas(aCrear, creadas, tipoAingresar, ofs);
			investigadoresIngresados += aCrear;		
		}
		LOG("LANZADOR: Durmiendo %d segundos.", tiempoAdormir);
		sleep(tiempoAdormir);
	}
	LOG("LANZADOR: Finaliza creacion de %d Personas Normales y de %d Investigadores", personasNormales, investigadores);
	ofs.close();
	return 0;
}

int Lanzador::crearPersonas(int aCrear, int creadas, int tipo, std::ofstream &ofs) {
	pid_t pidPersona;
	LOG("LANZADOR: Creando %d visitantes", aCrear);
	while(creadas < aCrear) {
		if ((pidPersona = fork()) < 0) {/* se crea el proceso hijo */
			perror("Error en el fork");
			exit(1);
		}
		else if (pidPersona == 0) {
			// PROCESO HIJO (child)
			execlp(ejecutablePersona.c_str(), "Persona", (Utils::int2String(tipo)).c_str(), (char *)0);
			/* si sigue es que no se ejecuto correctamente el comando execlp */
			perror("Error al lanzar el programa PERSONA");
			exit(3);
		}
		ofs << pidPersona << std::endl;
		++creadas;
	}
	LOG("LANZADOR: %d ingresantes creados", aCrear);
	return 0;
}
