#include <IPC/Cola.h>
#include <ipuerta/IPuertaMsg.h>
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
	ruta.append("/ipuerta.mq");
	free (tmp);
	return ruta;
}

int main (int argc, char** argv)
{
	std::cout << "Iniciando componente..." << std::endl;

	Cola<IPuertaMsg> mqComp (calcularRutaMQ (argv[0]), 'A');
	long mtype = getpid ();
	IPuertaMsg msg = {};
	IPuertaMsg res = {};
	int err;

	std::cout << "Aceptando mensajes..." << std::endl;

	while (true) {
		err = mqComp.leer (mtype, &msg);
		System::check (err);

		switch (msg.op) {
		case SOLIC_ENTRAR_MUSEO_PERSONA:
			// Este componente siempre deja entrar las personas
			std::cout << "Persona entrando por puerta "
				<< msg.msg.semp.idPuerta << std::endl;
			res.mtype = msg.msg.semp.rtype;
			res.op = NOTIFICACION_ENTRADA_PERSONA;
			res.msg.nep.res = ENTRO;
			err = mqComp.escribir (res);
			System::check (err);
			break;
		case SOLIC_ENTRAR_MUSEO_INVESTIGADOR:
			// Este componente guarda las pertenencias en el numero
			// de locker.
			std::cout << "Investigador entrando por puerta "
					<< msg.msg.semi.idPuerta
					<< " con pertenencias "
					<< msg.msg.semi.pertenencias << std::endl;
			res.mtype = msg.msg.semi.rtype;
			res.op = NOTIFICACION_ENTRADA_INVESTIGADOR;
			res.msg.nei.res = ENTRO;
			res.msg.nei.numeroLocker = msg.msg.semi.pertenencias;
			err = mqComp.escribir (res);
			System::check (err);
			break;
		case SOLIC_SALIR_MUSEO_PERSONA:
			// Responde que salió
			std::cout << "Persona saliendo por puerta "
					<< msg.msg.ssmp.idPuerta << std::endl;
			res.mtype = msg.msg.ssmp.rtype;
			res.op = NOTIFICACION_SALIDA_PERSONA;
			res.msg.nsp.res = SALIO;
			err = mqComp.escribir (res);
			System::check (err);
			break;
		case SOLIC_SALIR_MUSEO_INVESTIGADOR:
			// Devuelve las pertenencias que guardo en el
			// numero de locker.
			// No checkea puerta correcta
			std::cout << "Investigador saliendo por puerta "
					<< msg.msg.ssmi.idPuerta
					<< " con numero de locker "
					<< msg.msg.ssmi.numeroLocker << std::endl;
			res.mtype = msg.msg.ssmi.rtype;
			res.op = NOTIFICACION_SALIDA_INVESTIGADOR;
			res.msg.nsi.res = SALIO;
			res.msg.nsi.pertenencias = msg.msg.ssmi.numeroLocker;
			err = mqComp.escribir (res);
			System::check (err);
			break;
		default:
			std::cerr << "Componente recibio mensaje inválido: "
				<< msg.op << std::endl;
		}
	}

	/* not reached */
	return 0;
}
