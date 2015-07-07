#include "ParserConfiguracion.h"
#include <fstream>

using std::ifstream;
using std::string;
using std::map;

ParserConfiguracion::ParserConfiguracion() {
}

ParserConfiguracion::~ParserConfiguracion() {
}

int ParserConfiguracion::cargarConfiguracion(std::string pathConfiguracion) {
	std::string parametro;
	std::ifstream ifs;
	ifs.open(pathConfiguracion.c_str(), std::ifstream::in);
	while(!ifs.eof()) {
		ifs >> parametro;
		configuracion[parametro.substr(0, parametro.find_first_of('='))] = parametro.substr(parametro.find_first_of('=') + 1);
	}
	ifs.close();
	return 0;
}
	
string ParserConfiguracion::getValor(std::string clave) {
	return configuracion.find(clave)->second;
}
