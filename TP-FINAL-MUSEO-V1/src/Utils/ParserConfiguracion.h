#ifndef PARSERCONFIGURACION_H_
#define PARSERCONFIGURACION_H_

#include <iostream>
#include <map>

class ParserConfiguracion {

private:
	std::map<std::string, std::string> configuracion;

public:

	ParserConfiguracion();

	~ParserConfiguracion();

	int cargarConfiguracion(std::string pathConfiguracion);
	
	std::string getValor(std::string clave);
};

#endif /* PARSERCONFIGURACION_H_ */
