#ifndef IPCMANAGER_H_
#define IPCMANAGER_H_

#include "contador-msg.h"
#include <IPC/Cola.h>
#include <IPC/MemoriaCompartida.h>
#include <IPC/Semaforo.h>
#include <ipersona/IPersonaMsg.h>
#include <string>
#include <utils/NonCopyable.h>

class IPCManager : private NonCopyable
{
public:
	// TODO: ver que pasa si hay varias puertas corriendo en
	// la misma maquina.
	static const long MTYPE = 1;

	Cola<IPersonaMsg>*              interfaz;
	Cola<IPersonaMsg>*              ingresos;
	Cola<IPersonaMsg>*              egresoSimple;
	Cola<IPersonaMsg>*              egresoPreferencial;
	MemoriaCompartida<ContadorMsg>* pendientes;
	Semaforo*                       lock;
	Semaforo*                       hay;

	IPCManager (
			const std::string& pathInterfaz,
			const std::string& pathColas);
	~IPCManager ();

	void inicializar ();
	void destruir ();

	IPersonaMsg leerOperacionConPrioridad ();
};

#endif /* IPCMANAGER_H_ */
