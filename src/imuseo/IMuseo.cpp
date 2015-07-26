#include <imuseo/IMuseo.h>
#include <Logger/Logger.h>
#include <errno.h>
#include <error.h>
#include <IPC/Cola.h>
#include <signal.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <utils/EnvParam.h>
#include <utils/System.h>
#include <vector>

// TODO: mover a .h de constantes
static const char* DFLT_IMUSEO_COMP = "../imuseo_comp/imuseo_comp";
static const char* DFLT_IMUSEO_MQ   = "/etc/hosts";
static const char* DFLT_IMUSEO_BROKER   = "broker";
static const char* DFLT_IMUSEO_IDSERVER = "id-server";
/////////////////////////////////

void IMuseo::lanzarComponente ()
{
	LOG("IMUSEO: Lanzando componente");
	std::ostringstream oss;
	oss << getpid();
           child_pid = fork();
           if(child_pid == 0) {
	        execlp(DFLT_IMUSEO_COMP, "comp_puerta", oss.str().c_str(), NULL);
		exit(2);              
           }
           else if(child_pid == -1) {
              LOG("Error: Could not fork");
           }
	LOG("IMUSEO: Componente lanzado");
}

void IMuseo::terminarComponente ()
{
	LOG("IMUSEO: Terminando componente");
	if (child_pid != -1) {
		LOG("KILLING");
		kill (child_pid, SIGINT);
	}
}

IMuseo::IMuseo(): mqComp(DFLT_IMUSEO_MQ, 'B')
{
	LOG("IMUSEO: Iniciando interfaz puerta ...");
	lanzarComponente ();
}

IMuseo::~IMuseo(){
	LOG("IMUSEO: Terminando interfaz puerta ...");
	terminarComponente ();
//	mqComp.destruir(); // mata a la puerta también
}

Entrada::Resultado IMuseo::entrarPersona() {
	IMuseoMsg msg;

	msg.mtype = child_pid;	
	msg.op = 1;
	int e;
	mqComp.escribir(msg);
	mqComp.leer(getpid(), &msg);
	if (e == -1) {
		error (1, errno, "IPuerta::entrar[persona]");
		throw "not reached";
	}
	if(msg.op == 5) {
		return Entrada::ENTRO;
	}
	else if (msg.op == 6) {
		return Entrada::CERRADO;
	}
	else if (msg.op == 7) {
		return Entrada::LLENO;
	}
}

Salida::Resultado IMuseo::sacarPersona() {
	IMuseoMsg msg;

	msg.mtype = child_pid;	
	msg.op = 2;
	int e;
	mqComp.escribir(msg);
	mqComp.leer(getpid(), &msg);
	if (e == -1) {
		error (1, errno, "IPuerta::entrar[persona]");
		throw "not reached";
	}
	if(msg.op == 5) {
		return Salida::SALIO;
	}
	else if(msg.op == 8) {
		return Salida::SALIO_Y_LLENO;
	}

}

bool IMuseo::abrirMuseo(uint32_t capacidad) {
	IMuseoMsg msg;

	msg.mtype = child_pid;	
	msg.op = 3;
	msg.param_1 = capacidad;

	mqComp.escribir(msg);
	mqComp.leer(getpid(), &msg);
	int e;
	if (e == -1) {
		error (1, errno, "IPuerta::entrar[persona]");
		throw "not reached";
	}
	if(msg.op == 5)
		return true;
	else 
		return false;

}

bool IMuseo::cerrarMuseo() {
	IMuseoMsg msg;

	msg.mtype = child_pid;	
	msg.op = 4;
	int e;
	mqComp.escribir(msg);
	mqComp.leer(getpid(), &msg);
	if (e == -1) {
		error (1, errno, "IPuerta::entrar[persona]");
		throw "not reached";
	}
	if(msg.op == 5)
		return true;
	else 
		return false;

}

