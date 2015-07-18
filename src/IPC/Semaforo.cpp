#include "Semaforo.h"
#include "../utils/System.h"

Semaforo::Semaforo (const std::string& nombre, int key, const int valorInicial)
{
	key_t clave = ftok (nombre.c_str(), key);
	System::check (clave);
	this->id = semget (clave, 1, 0666 | IPC_CREAT );
	System::check (id);
	this->inicializar (valorInicial);
}

Semaforo::Semaforo (const std::string& nombre, int key)
{
	key_t clave = ftok (nombre.c_str(), key);
	System::check (clave);
	this->id = semget (clave, 1, 0666 | IPC_CREAT );
	System::check (id);
}

Semaforo::~Semaforo() {
}

int Semaforo::inicializar (int valorInicial) const {

	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	semnum init;
	init.val = valorInicial;
	int resultado = semctl ( this->id,0,SETVAL,init );
	return resultado;
}

int Semaforo :: p () const {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = -1;	// restar 1 al semaforo
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop ( this->id,&operacion,1 );
	return resultado;
}

int Semaforo :: v () const {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = 1;	// sumar 1 al semaforo
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop ( this->id,&operacion,1 );
	return resultado;
}

void Semaforo :: eliminar () const {
	semctl ( this->id,0,IPC_RMID );
}
