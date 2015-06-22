#include "Administrador.h"
#include "Constantes.h"
#include "../Logger/Logger.h"
#include "../Utils/Utils.h"
#include <fstream>
#include <sys/wait.h>
#include "../Utils/ParserConfiguracion.h"

#define PROCESS_NAME "ADMINISTRADOR"

Administrador::Administrador(): mtxMuseoAbierto(NULL) {
}
	
Administrador::~Administrador() {
}

void Administrador::abrirMuseo() {
	tomarIPC();
	LOG("ADMINISTRADOR: Abriendo museo ...");
	mtxMuseoAbierto->p();
	shmMuseoAbierto.escribir(true);
	mtxMuseoAbierto->v();
	LOG("ADMINISTRADOR: Museo abierto");
	
	// CREACION DE PUERTAS 
	pid_t pidPuerta;
        int cantidadPuertas;
        int puertasCreadas = 1;
        ParserConfiguracion parserConfiguracion;
        std::ofstream ofs(procesos.c_str(), std::ofstream::out);

        parserConfiguracion.cargarConfiguracion(configuracion);
        cantidadPuertas = Utils::string2int(parserConfiguracion.getValor("puertas"));

        LOG("ADMINISTRADOR: Cantidad de PUERTAS configuradas: %d", cantidadPuertas);
        LOG("ADMINISTRADOR: Creando PUERTAS ...");

        srand((unsigned)time(NULL));
        while(puertasCreadas <= cantidadPuertas) {
                if ((pidPuerta = fork()) < 0) {/* se crea el proceso hijo */
                        perror("Error en el fork");
                        exit(1);
                }
                else if (pidPuerta == 0) {
                        // PROCESO HIJO (child)
                        execlp(ejecutablePuerta.c_str(), "Puerta", (Utils::int2String(puertasCreadas)).c_str(), (char *)0);
                        /* si sigue es que no se ejecuto correctamente el comando execlp */
                        perror("Error al lanzar el programa PUERTA");
                        exit(3);
                }
                LOG("ADMINISTRADOR: PUERTA numero: %d ---> CREADA", puertasCreadas);
                LOG("ADMINISTRADOR: pid de PUERTA: %d ---> %d", puertasCreadas, pidPuerta);
                ++puertasCreadas;
                ofs << pidPuerta  << std::endl;
        }

	liberarIPC();
}
	
void Administrador::cerrarMuseo() {
	std::ifstream ifs;
        std::string strpid;
        int pid;

	tomarIPC();
	LOG("ADMINISTRADOR: Cerrando museo ...");
	mtxMuseoAbierto->p();
	shmMuseoAbierto.escribir(false);
	mtxMuseoAbierto->v();
	LOG("ADMINISTRADOR: Museo cerrado");

	LOG("ADMINISTRADOR: Esperando a que salgan las personas que estan adentro ...");
        ifs.open(perDat.c_str(), std::ifstream::in);
	while(ifs >> strpid) {
                pid = Utils::string2int(strpid);
                LOG("ADMINISTRADOR: Esperando a que salga persona: %d ...", pid);
                waitpid(pid, NULL, 0);
                LOG("ADMINISTRADOR: pid: %d ---> TERMINADO", pid);
        }
	ifs.close();


        LOG("ADMINISTRADOR: Deteniendo procesos puerta...");

        ifs.open(procesos.c_str(), std::ifstream::in);
        while(ifs >> strpid) {
                pid = Utils::string2int(strpid);
                LOG("ADMINISTRADOR: Deteniendo pid PUERTA: %d ...", pid);
                kill(pid, SIGINT);
                waitpid(pid, NULL, 0);
                LOG("ADMINISTRADOR: pid: %d ---> DETENIDO", pid);
        }
        ifs.close();


	liberarIPC();
}

int Administrador::tomarIPC() {
	LOG("ADMINISTRADOR: INICIALIZANDO RECURSOS");
	shmMuseoAbierto.crear(recShmMuseoAbierto, 'm');
	LOG("ADMINISTRADOR: %s CREADO", "SHM MuseoAbierto");
	mtxMuseoAbierto = new Semaforo(recMtxMuseoAbierto, 'a', -1);
	LOG("ADMINISTRADOR: %s CREADO", "MUTEX MuseoAbierto");
	return 0;
}

int Administrador::liberarIPC() {
	LOG("ADMINISTRADOR: LIBERANDO RECURSOS");
	delete mtxMuseoAbierto;
	LOG("ADMINISTRADOR: %s LIBERADO", "MUTEX MuseoAbierto");
	shmMuseoAbierto.liberar();
	LOG("ADMINISTRADOR: %s LIBERADA", "SHM MuseoAbierto");
	return 0;
}
