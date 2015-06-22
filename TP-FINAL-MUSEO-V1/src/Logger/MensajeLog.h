#ifndef MENSAJELOG_H_
#define MENSAJELOG_H_

#include <sstream>

class MensajeLog {
	std::stringstream mensaje;

public:
	MensajeLog();

	virtual ~MensajeLog();

	void agregar(std::string cadena);

	void agregar(int entero);

	void agregar(double doble);

	std::string getStringMensaje();
};

#endif /* MENSAJELOG_H_ */
