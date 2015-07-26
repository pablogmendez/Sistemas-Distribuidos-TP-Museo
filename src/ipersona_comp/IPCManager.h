#ifndef IPCMANAGER_H_
#define IPCMANAGER_H_

#include "contador-msg.h"
#include <IPC/Cola.h>
#include <IPC/MemoriaCompartida.h>
#include <IPC/Semaforo.h>
#include <ipersona/IPersonaMsg.h>
#include <string>
#include <utils/NonCopyable.h>
#include <vector>

class IPCManager : private NonCopyable
{
public:
	const long MTYPE;

	Cola<IPersonaMsg>*              interfaz;
	Cola<IPersonaMsg>*              ingresos;
	Cola<IPersonaMsg>*              egresoSimple;
	Cola<IPersonaMsg>*              egresoPreferencial;
	MemoriaCompartida<ContadorMsg>* pendientes;
	Semaforo*                       lock;
	Semaforo*                       hay;
	std::vector<IPersonaMsg>        devueltas;

	IPCManager (
			/* id local de la puerta; para soportar varias puertas
			 * en una misma máquina.
			 */
			int baseId,
			const std::string& pathInterfaz,
			const std::string& pathColas);
	~IPCManager ();

	void inicializar ();
	void destruir ();

	//- API para la parte "escritora" del componente
	IPersonaMsg leerOperacionConPrioridad ();
	void devolverOperacion (IPersonaMsg msg);
	void vaciarColasPorCierre ();
	//- API para la parte "lectora" del componente
	void ponerOperacion (IPersonaMsg msg);
};

#endif /* IPCMANAGER_H_ */
