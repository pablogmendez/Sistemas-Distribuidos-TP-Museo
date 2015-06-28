#include <IPC/Cola.h>
#include <ipersona/IPersonaMsg.h>
#include <iostream>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utils/System.h>

std::string calcularRutaMQ (const char* arg0)
{
	char* tmp = strdup (arg0);
	char* dir = dirname (tmp);
	std::string ruta = dir;
	ruta.append("/ipersona.mq");
	free (tmp);
	return ruta;
}

int main (int argc, char** argv) try
{
	std::cout << "Iniciando componente..." << std::endl;

	int err;
	Cola<IPersonaMsg> mqComp (calcularRutaMQ (argv[0]), 'A');
	long mtype = getpid ();

	IPersonaMsg msg;
	IPersonaMsg res;

	// Bloqueo en la cola hasta que haya una solicitud de operación
	err = mqComp.leer (mtype, &msg);
	System::check (err);

	std::cout << "[COMPONENTE] Se obtuvo solicitud: op "
			<< msg.op << std::endl;

	// Acá el componente lee de la red una petición a esta puerta
	// Simulamos que una persona quiere entrar
	// Enviamos la operación de solicitud de entrada
	res.mtype = msg.msg.spo.rtype;
	res.op = OP_SOLIC_ENTRAR_MUSEO_PERSONA;
	// Id de la persona que quiere entrar como se recibió de la red
	res.msg.osemp.idOrigen = 12345;
	err = mqComp.escribir (res);
	System::check (err);

	// Luego esperamos que responda la interfaz
	err = mqComp.leer (mtype, &msg);
	System::check (err);

	if (msg.op != NOTIF_ENTRADA_PERSONA) {
		std::cerr << "[COMPONENTE] Interfaz respondio con"
				" operación incorrecta " << msg.op
				<< std::endl;
		goto out;
	}

	std::cout << "[COMPONENTE] La interfaz respondio: op "
				<< msg.op << std::endl;
	std::cout << "[COMPONENTE] La interfaz respondio: res "
					<< msg.msg.nep.res << std::endl;

	// Acá el componente debería responder por la red al origen
	System::millisleep (2000);

	// Y despues volver al inicio
	// Aquí simulamos que la persona quiere salir

	// Espera por la próxima solicitud de la interfaz
	err = mqComp.leer (mtype, &msg);
	System::check (err);

	// Simulamos que una persona quiere salir

	// Enviamos la operación de solicitud de salida
	res.mtype = msg.msg.spo.rtype;
	res.op = OP_SOLIC_SALIR_MUSEO_PERSONA;
	// Id de la persona que quiere salir como se recibió de la red
	res.msg.ossmp.idOrigen = 12345;
	err = mqComp.escribir (res);
	System::check (err);

	// Luego esperamos que responda la interfaz
	err = mqComp.leer (mtype, &msg);
	System::check (err);

	if (msg.op != NOTIF_SALIDA_PERSONA) {
		std::cerr << "[COMPONENTE] Interfaz respondio con"
				" operación incorrecta " << msg.op
				<< std::endl;
		goto out;
	}

	std::cout << "[COMPONENTE] La interfaz respondio: op "
				<< msg.op << std::endl;
	std::cout << "[COMPONENTE] La interfaz respondio: res "
					<< msg.msg.nsp.res << std::endl;

	// Acá el componente debería responder por la red al origen
	System::millisleep (2000);

	// Fin de la simulación
	// Enviamos el cierre para que el test de la interfaz
	// termine.

out:
	// Envía notificación de cierre
	err = mqComp.leer (mtype, &msg);
	System::check (err);

	res.mtype = msg.msg.spo.rtype;
	res.op = OP_NOTIFICAR_CIERRE_MUSEO;
	err = mqComp.escribir (res);
	System::check (err);
	return 0;
} catch (...) {
	// try-catch para que se corran los destructores
	std::cerr << "[COMPONENTE] Error al correr la simulación."
			<< std::endl;
}
