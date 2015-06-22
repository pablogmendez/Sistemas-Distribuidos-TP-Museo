#ifndef INVESTIGADOR_H_
#define INVESTIGADOR_H_

#include "Persona.h"

class Investigador: public Persona {
public:
	Investigador();
	virtual ~Investigador();
	virtual void iniciarRecorrido();
};

#endif /* INVESTIGADOR_H_ */
