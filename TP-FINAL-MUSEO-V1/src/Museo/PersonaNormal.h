#ifndef PERSONANORMAL_H_
#define PERSONANORMAL_H_

#include "Persona.h"

class PersonaNormal: public Persona {
public:
	PersonaNormal();

	virtual void iniciarRecorrido();

	virtual ~PersonaNormal();
};

#endif /* PERSONANORMAL_H_ */
