enum type { PUERTA, PERSONA, MUSEO};

struct Proceso{
	long pid_escritor;
	long id;
	type tipo;
	int borrado;
}; 
