#ifndef COLA_H_
#define COLA_H_

#include <errno.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <string.h>

template <class T> class Cola {
	private:
		key_t	clave;
		int		id;

	public:
		Cola ( const std::string& archivo,const int letra );
		~Cola();
		int escribir ( const T& dato ) const;
		int leer ( const long tipo,T* buffer ) const;
		int destruir () const;
};

template <class T> Cola<T> :: Cola ( const std::string& archivo,const int letra ) {
	this->clave = ftok ( archivo.c_str(),letra );
	if ( this->clave == -1 ) {
		int err = errno;
		perror ( "Error en ftok" );
		throw std::runtime_error (strerror (err));
	}

	this->id = msgget ( this->clave,0777|IPC_CREAT );
	if ( this->id == -1 ) {
		int err = errno;
		perror ( "Error en msgget" );
		throw std::runtime_error (strerror (err));
	}
}

template <class T> Cola<T> :: ~Cola () {
}

template <class T> int Cola<T> :: destruir () const {
	int resultado = msgctl ( this->id,IPC_RMID,NULL );
	return resultado;
}

template <class T> int Cola<T> :: escribir ( const T& dato ) const {
	int resultado = msgsnd ( this->id,static_cast<const void*>(&dato),sizeof(T)-sizeof(long),0 );
	return resultado;
}

template <class T> int Cola<T> :: leer ( const long tipo,T* buffer ) const {
	int resultado = msgrcv ( this->id,static_cast<void *>(buffer),sizeof(T)-sizeof(long),tipo,0 );
	return resultado;
}

#endif /* COLA_H_ */
