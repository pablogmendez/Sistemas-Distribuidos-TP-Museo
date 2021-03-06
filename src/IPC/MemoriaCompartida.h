#ifndef MEMORIACOMPARTIDA_H_
#define MEMORIACOMPARTIDA_H_

#define SHM_OK			 0
#define	ERROR_FTOK		-1
#define ERROR_SHMGET	-2
#define	ERROR_SHMAT		-3

#include "../Logger/Logger.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <unistd.h>

template <class T> class MemoriaCompartida {

private:
	int shmId;
	T*	ptrDatos;

	shmatt_t cantidadProcesosAdosados () const;

public:
	MemoriaCompartida ();
	~MemoriaCompartida ();
	int crear (const std::string& archivo, const int letra);
	void liberar ();
	void escribir ( const T& dato );
	T leer () const;

};

template <class T> MemoriaCompartida<T> :: MemoriaCompartida() : shmId(0), ptrDatos(NULL) {
}

template <class T> MemoriaCompartida<T> :: ~MemoriaCompartida() {
}

template <class T>
int MemoriaCompartida<T>::crear (const std::string& archivo, const int letra)
{

	// generacion de la clave
	key_t clave = ftok ( archivo.c_str(),letra );
	if ( clave == -1 )
		return ERROR_FTOK;
	else {
		// creacion de la memoria compartida
		this->shmId = shmget ( clave,sizeof(T),0644|IPC_CREAT );

		if ( this->shmId == -1 )
			return ERROR_SHMGET;
		else {
			// attach del bloque de memoria al espacio de direcciones del proceso
			void* ptrTemporal = shmat ( this->shmId,NULL,0 );

			if ( ptrTemporal == (void *) -1 ) {
				return ERROR_SHMAT;
			} else {
				this->ptrDatos = static_cast<T*> (ptrTemporal);
				LOG ("SHM [proc %d] - Creada con id %d.", getpid (), shmId);
				return SHM_OK;
			}
		}
	}
}


template <class T> void MemoriaCompartida<T> :: liberar () {
	// detach del bloque de memoria
	shmdt ( static_cast<void*> (this->ptrDatos) );

	shmatt_t procAdosados = this->cantidadProcesosAdosados ();

	if ( procAdosados == 0 ) {
		shmctl ( this->shmId,IPC_RMID,NULL );
	}
	const char* op = (procAdosados == 0 ? "Removida" : "Liberada");
	LOG ("SHM [proc %d] - %s. Adosados: %llu.", getpid (), op,
		static_cast<unsigned long long> (procAdosados));
}

template <class T> void MemoriaCompartida<T> :: escribir ( const T& dato ) {
	* (this->ptrDatos) = dato;
}

template <class T> T MemoriaCompartida<T> :: leer () const {
	return ( *(this->ptrDatos) );
}

template <class T> shmatt_t MemoriaCompartida<T> :: cantidadProcesosAdosados () const {
	shmid_ds estado;
	shmctl ( this->shmId,IPC_STAT,&estado );
	return estado.shm_nattch;
}


#endif /* MEMORIACOMPARTIDA_H_ */
