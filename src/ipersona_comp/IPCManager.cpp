#include "IPCManager.h"
#include "ipc-keys.h"
#include <stdexcept>
#include <utils/System.h>

IPCManager::IPCManager (
		int baseId,
		const std::string& pathInterfaz,
		const std::string& pathColas)
	: MTYPE (baseId)
{
	if (baseId <= 0) {
		throw std::logic_error ("El id local debe ser mayor a 0.");
	}

	if (comp_calc_proj_id (baseId, COMP_MAX) > PROJ_ID_MAX) {
		throw std::runtime_error (
				"El sistema no soporta puertas con este id.");
	}

	interfaz = new Cola<IPersonaMsg> (pathInterfaz,
			int_calc_proj_id (baseId, MQ_INT));
	ingresos = new Cola<IPersonaMsg> (pathColas, MQ_ENTR);
	egresoSimple = new Cola<IPersonaMsg> (pathColas, MQ_SEST);
	egresoPreferencial = new Cola<IPersonaMsg> (pathColas, MQ_SESP);
	pendientes = new MemoriaCompartida<ContadorMsg> ();
	pendientes->crear (pathColas, comp_calc_proj_id (baseId, SHM_MQS));
	lock = new Semaforo (pathColas, comp_calc_proj_id (baseId, MTX_MQS));
	hay = new Semaforo (pathColas, comp_calc_proj_id (baseId, SEM_HAY));
}

IPCManager::~IPCManager ()
{
	pendientes->liberar ();
	delete interfaz;
	delete ingresos;
	delete egresoSimple;
	delete egresoPreferencial;
	delete pendientes;
	delete lock;
	delete hay;
}

void IPCManager::inicializar ()
{
	ContadorMsg dato;
	dato.entradas = 0;
	dato.esperando = 0;
	dato.salidasEspeciales = 0;
	dato.salidasEstandar = 0;
	pendientes->escribir (dato);

	lock->inicializar (1);
	hay->inicializar (0);
}

void IPCManager::destruir ()
{
	lock->eliminar ();
	hay->eliminar ();
	ingresos->destruir ();
	egresoPreferencial->destruir ();
	egresoSimple->destruir ();
}

IPersonaMsg IPCManager::leerOperacionConPrioridad ()
{
	int err;
	IPersonaMsg msg;

	err = lock->p ();
	System::check (err);

	ContadorMsg cont = pendientes->leer ();
	while (cont.entradas == 0
			&& cont.salidasEspeciales == 0
			&& cont.salidasEstandar == 0) {
		cont.esperando++;
		pendientes->escribir (cont);

		err = lock->v ();
		System::check (err);

		err = hay->p ();
		System::check (err);

		err = lock->p ();
		System::check (err);
		cont = pendientes->leer ();
	}

	if (cont.salidasEspeciales > 0) {
		cont.salidasEspeciales--;

		err = egresoPreferencial->leer (MTYPE, &msg);
	} else if (cont.salidasEstandar > 0) {
		cont.salidasEstandar--;
		err = egresoSimple->leer (MTYPE, &msg);
	} else /* if (cont.entradas > 0) */ {
		cont.entradas--;
		err = ingresos->leer (MTYPE, &msg);
	}

	// Se guarda errno para evitar que el uso del semáforo lo pise.
	int number = errno;
	pendientes->escribir (cont);
	lock->v ();
	errno = number;
	System::check (err);
	return msg;
}
