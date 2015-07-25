#ifndef IPUERTA_IPUERTA_H
#define IPUERTA_IPUERTA_H
#include <IPC/Cola.h>
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
#include <ipuerta/IPuertaMsg.h>

class IPuerta
{
private:
	Cola<IPuertaMsg> mqComp;
	pid_t child_pid;

	void lanzarComponente ();
	void terminarComponente ();

public:
	IPuerta ();
	~IPuerta ();
	void entrar (long puerta);
	long entrar (long puerta, long pertenencias);
	void salir (long puerta);
	long salir (long puerta, long numeroLocker);
};

#endif
