#include "Persona.h"
#include "Puerta.h"
#include "../IPC/SIGINT_Handler.h"
#include "../Utils/ParserConfiguracion.h"
#include "../Utils/Utils.h"
#include "../Logger/Logger.h"

Persona::Persona(): colaEsp(NULL), colaNorm(NULL),
mtxCont(NULL), mtxPuerta(NULL), mtxVisitante(NULL),
shmCont(NULL),
numero(getpid()) {
}
	
Persona::~Persona() {
}

void Persona::iniciarIPC(int tipo, int puertaIngreso, int puertaEgreso) {
	LOG("PERSONA %d: INICIALIZANDO IPC's", numero);

	colaNorm = new Cola<persona_t>(recColaNorm, 'a');
	LOG("PERSONA %d: CREANDO colaNorm", numero);
	colaEsp = new Cola<persona_t>(recColaEsp, 'a');
	LOG("PERSONA %d: CREANDO colaEsp", numero);


	mtxVisitante = new Semaforo(recMtxVisitante, puertaEgreso, -1);
	LOG("PERSONA %d: CREANDO mtxVisitante", numero);
	mtxPuerta = new Semaforo(recMtxPuerta, puertaEgreso, -1);
	LOG("PERSONA %d: CREANDO mtxPuerta", numero);

	if(tipo == 1) {
		mtxCont = new Semaforo(recMtxContNorm, puertaEgreso, -1);
		LOG("PERSONA %d: CREANDO mtxContNorm", numero);
		shmCont = new MemoriaCompartida<int>();
		shmCont->crear(recShmContNorm, puertaEgreso);
		LOG("PERSONA %d: CREANDO shmContNorm", numero);
	}
	else {
		mtxCont = new Semaforo(recMtxContEsp, puertaEgreso, -1);
		LOG("PERSONA %d: CREANDO mtxContEsp", numero);
		shmCont = new MemoriaCompartida<int>();
		shmCont->crear(recShmContEsp, puertaEgreso);
		LOG("PERSONA %d: CREANDO shmContEsp", numero);
	}
}
	
void Persona::liberarIPC() {
	LOG("PERSONA %d: LIBERANDO IPC's", numero);
	delete colaNorm;
	LOG("PERSONA %d: colaNorm LIBERADA", numero);
	delete colaEsp;
	LOG("PERSONA %d: colaEsp LIBERADA", numero);
	delete mtxVisitante;
	LOG("PERSONA %d: mtxVisitante LIBERADO", numero);
	delete mtxPuerta;
	LOG("PERSONA %d: mtxPuerta LIBERADO", numero);
	delete mtxCont;
	LOG("PERSONA %d: mtxCont LIBERADO", numero);
	shmCont->liberar();
	delete shmCont;
	LOG("PERSONA %d: shmCont LIBERADA", numero);
	LOG("PERSONA %d: LIBERANDO IPC's", numero);
}
	
	
void Persona::iniciar(int tipo, int puertaIngreso, int puertaEgreso) {
	persona_t persona;
	ParserConfiguracion parser;

	iniciarIPC(tipo, puertaIngreso, puertaEgreso);
	parser.cargarConfiguracion(configuracion);
	srand((unsigned)time(NULL));
	int tiempoEnMuseo = (rand()%Utils::string2int(parser.getValor("tiempoEnMuseo")))+1;
	LOG("PERSONA %d: TIEMPO EN MUSEO: %d", numero, tiempoEnMuseo);
	persona.mtype = puertaIngreso;
	persona.pid = numero;
	persona.accion = 1;
	persona.tipo = tipo;
	colaNorm->escribir(persona);
	LOG("PERSONA %d: Enviando peticion de entrada", numero);
	mtxCont->p();
	shmCont->escribir(shmCont->leer() + 1);
	mtxCont->v();
	mtxVisitante->p();
	mtxPuerta->v();
	colaNorm->leer(numero, &persona);
	LOG("PERSONA %d: Respuesta recibida", numero);
	if(persona.accion == 0) {
		LOG("PERSONA %d: Peticion aceptada", numero);
		LOG("PERSONA %d: INGRESO A MUSEO", numero);
		LOG("PERSONA %d: Recorriendo MUSEO", numero);
		sleep(tiempoEnMuseo);
		persona.mtype = puertaEgreso;
		persona.pid = numero;
		persona.accion = 0;
		if(tipo == 1) {
			colaNorm->escribir(persona);
		}
		else {
			colaEsp->escribir(persona);
		}
		LOG("PERSONA %d: ENVIANDO PETICION DE SALIDA", numero);
		LOG("PERSONA %d: ENVIANDO SEÑAL A LA shmCont de la puerta", numero);
		mtxCont->p();
		shmCont->escribir(shmCont->leer() + 1);
		mtxCont->v();
		LOG("PERSONA %d: Destrabando mtxPuerta", numero);
		mtxPuerta->v();
		LOG("PERSONA %d: Trabando mtxVisitante", numero);
		mtxVisitante->p();

		if(tipo == 1) {
			colaNorm->leer(numero, &persona);
		}
		else {
			colaEsp->leer(numero, &persona);
		}
		LOG("PERSONA %d: SALE DE MUSEO", numero);
	} else {
		LOG("PERSONA %d: NO PUDO INGRESAR A MUSEO - CERRADO", numero);
	}
	liberarIPC();
}
