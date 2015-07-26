#include <ipuerta/IPuerta.h>
#include "IPuertaMsg.h"

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
static const char* DFLT_IPUERTA_COMP = "../ipuerta_comp/ipuerta_comp";
static const char* DFLT_IPUERTA_MQ   = "/etc/hosts";
static const char* DFLT_IPUERTA_BROKER   = "broker";
static const char* DFLT_IPUERTA_IDSERVER = "id-server";
/////////////////////////////////


void IPuerta::lanzarComponente ()
{
	LOG("Proceso %d: Lanzando componente", getpid());
           child_pid = fork();
           if(child_pid == 0) {
	        execlp(DFLT_IPUERTA_COMP, "comp_puerta", NULL);
		exit(2);              
           }
           else if(child_pid == -1) {
               std::cout << "Error: Could not fork" << std::endl;
           }
	LOG("Proceso %d: Componente lanzado", getpid());
}

void IPuerta::terminarComponente ()
{
	LOG("Proceso %d: Terminando componente", getpid());
	if (child_pid != -1) {
		kill (child_pid, SIGINT);
	}
}

IPuerta::IPuerta (): mqComp(DFLT_IPUERTA_MQ, 'A')
{
	LOG("Proceso %d: Iniciando interfaz puerta ...", getpid());
	lanzarComponente ();
}

IPuerta::~IPuerta ()
{
	terminarComponente ();
}

long IPuerta::entrar (long puerta)
{
	int err;
	struct IPuertaMsg msg;
	long rtype = getpid ();

	LOG("Proceso %d: Intentando ingresar al museo por la puerta %d", getpid(), puerta);
	msg.msg.semp.rtype = rtype;
	msg.msg.semp.idPuerta = puerta;
	msg.mtype = child_pid;
	msg.op = OP_SOLIC_ENTRAR_MUSEO_PERSONA;

	LOG("Se ingresan en la cola los siguientes parametros:");
	LOG("msg.msg.semp.rtype ---> %d", rtype);
	LOG("msg.msg.semp.idPuerta ---> %d", puerta);
	LOG("msg.mtype ---> %d", child_pid);
	LOG("msg.op ---> OP_SOLIC_ENTRAR_MUSEO_PERSONA = %d", OP_SOLIC_ENTRAR_MUSEO_PERSONA);

	err = mqComp.escribir (msg);
	if (err == -1) {
		error (1, errno, "IPuerta::entrar[persona]");
		throw "not reached";
	}
	LOG("IPUERTA: RTYPE %d", rtype);
	err = mqComp.leer (rtype, &msg);
	if (err == -1) {
		error (1, errno, "IPuerta::entrar[persona]");
		throw "not reached";
	}

	if (msg.op == NOTIF_ENTRADA_PERSONA) {
		if(msg.msg.nep.res == 3) {	
		LOG("Museo Cerrado");
		return 1;			
		}
		// TODO: ver si no estaba cerrado el museo
		return 0;
	}

	std::ostringstream oss;
	oss << "IPuerta::entrar[persona]: ";
	oss << "se recibio operacion incorrecta: " << msg.op;
	error (1, 0, "%s", oss.str ().c_str ());
	throw "not reached";
}

long IPuerta::entrar (long puerta, long pertenencias)
{
	int err;
	IPuertaMsg msg;
	long rtype = getpid ();

	LOG("Proceso %d: Intentando ingresar al museo por la puerta %d, con pertenencias %d", getpid(), puerta, pertenencias);
	msg.msg.semi.rtype = rtype;
	msg.msg.semi.idPuerta = puerta;
	msg.msg.semi.pertenencias = pertenencias;
	msg.mtype = child_pid;
	msg.op = OP_SOLIC_ENTRAR_MUSEO_INVESTIGADOR;

	LOG("Se ingresan en la cola los siguientes parametros:");
	LOG("msg.msg.semi.rtype ---> %d", rtype);
	LOG("msg.msg.semi.idPuerta ---> %d", puerta);
	LOG("msg.msg.semi.pertenencias ---> %d", pertenencias);
	LOG("msg.mtype ---> %d", child_pid);
	LOG("msg.op ---> OP_SOLIC_ENTRAR_MUSEO_INVESTIGADOR = %d", OP_SOLIC_ENTRAR_MUSEO_PERSONA);

	err = mqComp.escribir (msg);
	if (err == -1) {
		error (1, errno, "IPuerta::entrar[investigador]");
		throw "not reached";
	}

	err = mqComp.leer (rtype, &msg);
	if (err == -1) {
		error (1, errno, "IPuerta::entrar[investigador]");
		throw "not reached";
	}

	if (msg.op == NOTIF_ENTRADA_INVESTIGADOR) {
		if(msg.msg.nep.res == 3) {	
		LOG("Museo Cerrado");
		return 1;			
		}
		// TODO: ver si no estaba cerrado el museo
		return 0;
	}

	std::ostringstream oss;
	oss << "IPuerta::entrar[investigador]: ";
	oss << "se recibio operacion incorrecta: " << msg.op;
	error (1, 0, "%s", oss.str ().c_str ());
	throw "not reached";
}

long IPuerta::salir (long puerta)
{
	int err;
	struct IPuertaMsg msg = {};
	long rtype = getpid ();

	LOG("Proceso %d: Intentando salir del museo por la puerta %d", getpid(), puerta);
	msg.msg.ssmp.rtype = rtype;
	msg.msg.ssmp.idPuerta = puerta;
	msg.mtype = child_pid;
	msg.op = OP_SOLIC_SALIR_MUSEO_PERSONA;

	LOG("Se ingresan en la cola los siguientes parametros:");
	LOG("msg.msg.ssmp.rtype ---> %d", rtype);
	LOG("msg.msg.ssmp.idPuerta ---> %d", puerta);
	LOG("msg.mtype ---> %d", child_pid);
	LOG("msg.op ---> OP_SOLIC_SALIR_MUSEO_PERSONA = %d", OP_SOLIC_SALIR_MUSEO_PERSONA);

	err = mqComp.escribir (msg);
	if (err == -1) {
		error (1, errno, "IPuerta::salir[persona]");
		throw "not reached";
	}

	err = mqComp.leer (rtype, &msg);
	if (err == -1) {
		error (1, errno, "IPuerta::salir[persona]");
		throw "not reached";
	}

	if (msg.op == NOTIF_SALIDA_PERSONA) {
	LOG("IPUERTA: SALIO LA PERSONA");
		return 0;
	}

	std::ostringstream oss;
	oss << "IPuerta::salir[persona]: ";
	oss << "se recibio operacion incorrecta: " << msg.op;
	error (1, 0, "%s", oss.str ().c_str ());
	throw "not reached";
}

long IPuerta::salir (long puerta, long numeroLocker)
{
	int err;
	struct IPuertaMsg msg;
	long rtype = getpid ();

	LOG("Proceso %d: Intentando ingresar al museo por la puerta %d, con locker %d", getpid(), puerta, numeroLocker);
	msg.msg.ssmi.rtype = rtype;
	msg.msg.ssmi.idPuerta = puerta;
	msg.msg.ssmi.numeroLocker = numeroLocker;
	msg.mtype = child_pid;
	msg.op = OP_SOLIC_SALIR_MUSEO_INVESTIGADOR;

	LOG("Se ingresan en la cola los siguientes parametros:");
	LOG("msg.msg.ssmi.rtype ---> %d", rtype);
	LOG("msg.msg.ssmi.idPuerta ---> %d", puerta);
	LOG("msg.msg.ssmi.numeroLocker ---> %d", numeroLocker);
	LOG("msg.mtype ---> %d", child_pid);
	LOG("msg.op ---> OP_SOLIC_SALIR_MUSEO_INVESTIGADOR = %d", OP_SOLIC_SALIR_MUSEO_INVESTIGADOR);

	err = mqComp.escribir (msg);
	if (err == -1) {
		error (1, errno, "IPuerta::salir[investigador]");
		throw "not reached";
	}

	err = mqComp.leer (rtype, &msg);
	if (err == -1) {
		error (1, errno, "IPuerta::salir[investigador]");
		throw "not reached";
	}

	if (msg.op == NOTIF_SALIDA_INVESTIGADOR) {
		if (msg.msg.nsi.res == SALIO) {
			return msg.msg.nsi.pertenencias;
		}

		std::ostringstream oss;
		oss << "IPuerta::salir[investigador]: ";
		oss << "se recibio error: puerta incorrecta.";
		error (1, 0, "%s", oss.str ().c_str ());
		throw "not reached";
	}

	std::ostringstream oss;
	oss << "IPuerta::salir[investigador]: ";
	oss << "se recibio operacion incorrecta: " << msg.op;
	error (1, 0, "%s", oss.str ().c_str ());
	throw "not reached";
}
