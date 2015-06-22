#include "Puerta.h"
#include "../IPC/SignalHandler.h"
#include "../Logger/Logger.h"
#include "../Utils/Utils.h"
#include "Constantes.h"

Puerta::Puerta(int numero) {
	capacidad = 0;
	mtxContMuseo = mtxMuseoAbierto = NULL;
	mtxVisitante = mtxPuerta = NULL;
	mtxContNorm = mtxContEsp = NULL;
	colaNorm = colaEsp = NULL;
	parserConfiguracion.cargarConfiguracion(configuracion);
	pid = getpid();
	this->numero = numero;
}
	
Puerta::~Puerta() {
}

int Puerta::tomarIPC() {
	LOG("PUERTA %d: INICIALIZANDO RECURSOS", numero);
	SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
	LOG("PUERTA %d: %s CREADO", numero, "SIGNAL HANDLER");
	shmMuseoAbierto.crear(recShmMuseoAbierto, 'm');
	LOG("PUERTA %d: %s CREADO", numero, "SHM shmMuseoAbierto");
	shmContMuseo.crear(recShmContMuseo, 'm');
	LOG("PUERTA %d: %s CREADO", numero, "SHM shmContadorMuseo");
	shmContNorm.crear(recShmContNorm, numero);
	LOG("PUERTA %d: %s CREADO", numero, "SHM shmContNorm");
	shmContEsp.crear(recShmContEsp, numero);
	LOG("PUERTA %d: %s CREADO", numero, "SHM shmContEsp");

	LOG("PUERTA %d: Creando mtxContMuseo", numero);
	mtxContMuseo = new Semaforo(recMtxContMuseo, 'a', -1);
	LOG("PUERTA %d: Creando mtxMuseoAbierto", numero);
	mtxMuseoAbierto = new Semaforo(recMtxMuseoAbierto, 'a', -1);
	LOG("PUERTA %d: Creando mtxVisitante", numero);
	mtxVisitante = new Semaforo(recMtxVisitante, numero, -1);
	LOG("PUERTA %d: Creando mtxPuerta", numero);
	mtxPuerta = new Semaforo(recMtxPuerta, numero, -1);
	LOG("PUERTA %d: Creando mtxContNorm", numero);
	mtxContNorm = new Semaforo(recMtxContNorm, numero, -1);
	LOG("PUERTA %d: Creando mtxContEsp", numero);
	mtxContEsp = new Semaforo(recMtxContEsp, numero, -1);

	colaNorm = new Cola<persona_t>(recColaNorm, 'a');
	LOG("PUERTA %d: Creando colaNorm", numero);
	colaEsp = new Cola<persona_t>(recColaEsp, 'a');
	LOG("PUERTA %d: Creando colaNorm", numero);
	LOG("PUERTA %d: INICIALIZADA", numero);
	return 0;
}
	
int Puerta::liberarIpc() {
	LOG("PUERTA %d: LIBERANDO RECURSOS", numero);
	LOG("PUERTA %d: Liberando SignalHandler", numero);
	SignalHandler::destruir();

	shmMuseoAbierto.liberar();
	LOG("PUERTA %d: Liberando shmMuseoAbierto", numero);
	shmContMuseo.liberar();
	LOG("PUERTA %d: Liberando shmContMuseo", numero);
	shmContNorm.liberar();
	LOG("PUERTA %d: Liberando shmContNorm", numero);
	shmContEsp.liberar();
	LOG("PUERTA %d: Liberando shmContEsp", numero);

	delete mtxContMuseo;
	LOG("PUERTA %d: Liberando mtxContMuseo", numero);
	delete mtxMuseoAbierto;
	LOG("PUERTA %d: Liberando mtxMuseoAbierto", numero);
	delete mtxVisitante;
	LOG("PUERTA %d: Liberando mtxVisitante", numero);
	delete mtxPuerta;
	LOG("PUERTA %d: Liberando mtxPuerta", numero);
	delete mtxContNorm;
	LOG("PUERTA %d: Liberando mtxContNorm", numero);
	delete mtxContEsp;
	LOG("PUERTA %d: Liberando mtxContEsp", numero);

	delete colaNorm;
	LOG("PUERTA %d: Liberando colaNorm", numero);
	delete colaEsp;
	LOG("PUERTA %d: Liberando colaEsp", numero);

	return 0;
}
	
void Puerta::abrir() {
	ParserConfiguracion parserConfiguracion;
	queue<persona_t> colaAuxiliar;
	persona_t persona;
	bool museoAbierto;
	int cantPersonas;

	tomarIPC();
	parserConfiguracion.cargarConfiguracion(configuracion);
	srand((unsigned)time(NULL));
	LOG("PUERTA %d: ABRIENDO PUERTA", numero);
	int capacidad = Utils::string2int(parserConfiguracion.getValor("capacidad"));

	while(sigint_handler.getGracefulQuit() == 0) {
		LOG("PUERTA %d: Destrabando mtxVisitante", numero);
		mtxVisitante->v();
		LOG("PUERTA %d: Trabando mtxPuerta", numero);
		mtxPuerta->p();
		LOG("PUERTA %d: Trabando mtxMuseoAbierto", numero);
		mtxMuseoAbierto->p();
		LOG("PUERTA %d: Trabando mtxContMuseo", numero);
		mtxContMuseo->p();

		LOG("PUERTA %d: Trabando mtxContEsp", numero);
		mtxContEsp->p(); // Leo el cont de invest
		if(shmContEsp.leer()) {
			shmContEsp.escribir(shmContEsp.leer() - 1);
			colaEsp->leer(numero, &persona);
			persona.mtype = persona.pid;
			persona.accion = 0;
			colaEsp->escribir(persona);
			LOG("PUERTA %d: Sale Investigador %d", numero, persona.pid);
			entrarColaAux(colaAuxiliar);
		}
		else { // Si no hay nada, leo del cont normal
			mtxContNorm->p();
			if(shmContNorm.leer()) {
				shmContNorm.escribir(shmContNorm.leer() - 1);
				colaNorm->leer(numero, &persona);
				if(persona.accion == 1) { // Entra
					museoAbierto = shmMuseoAbierto.leer();
					cantPersonas = shmContMuseo.leer();
					if(museoAbierto) {
						if(cantPersonas < capacidad) {
							if(entrarColaAux(colaAuxiliar) == 0) { // Entre uno en espera
								colaAuxiliar.push(persona);
							}
							else { // Entro al corriente
								LOG("PUERTA %d: PERSONA %d - ACCION INGRESAR", numero, persona.pid);
								persona.mtype = persona.pid;
								persona.accion = 0;
								colaNorm->escribir(persona);
								LOG("PUERTA %d: Entra Persona %d", numero, persona.pid);
							}
							shmContMuseo.escribir(shmContMuseo.leer() + 1);
						}
						else { // Si esta lleno, lo pongo en la cola de espera
							colaAuxiliar.push(persona);
						}
					}
					else { // Si el museo esta cerrado
						persona.mtype = persona.pid;
						persona.accion = 1;
						colaNorm->escribir(persona);
						LOG("PUERTA %d: MUSEO CERRADO. No ntra Persona %d", numero, persona.pid);
					}
				}
				else { // Si la accion es salir (Normal)
					shmContMuseo.escribir(shmContMuseo.leer() - 1);
					persona.mtype = persona.pid;
					persona.accion = 0;
					colaNorm->escribir(persona);
					LOG("PUERTA %d: Sale Persona normal %d", numero, persona.pid);
					entrarColaAux(colaAuxiliar); // Trato de entrar uno de la cola auxiliar
				}
			}
			mtxContNorm->v();
		}
		mtxContEsp->v();
		mtxContMuseo->v();
		mtxMuseoAbierto->v();

	}
	vaciarMuseo();
	liberarIpc();
}

int Puerta::entrarColaAux(queue<persona_t> &colaAuxiliar) {
	persona_t persona;
	if(colaAuxiliar.empty()) {
		LOG("PUERTA %d: COLA AUXILIAR VACIA", numero);
		return 1;
	}
	else {
		persona = colaAuxiliar.front();
		colaAuxiliar.pop();
		persona.mtype = persona.pid;
		persona.accion = 0;
		colaNorm->escribir(persona);
		LOG("PUERTA %d: MUSEO LLENO. Se deja a la persona %d en espera", numero, persona.pid);
		return 0;
	}
}

void Puerta::vaciarMuseo() {
	LOG("PUERTA %d: Vaciando museo...", numero);
	mtxContMuseo->p();
	shmContMuseo.escribir(0);
	mtxContMuseo->v();
	LOG("PUERTA %d: VACIADO", numero);
}
