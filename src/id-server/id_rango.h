#ifndef ID_RANGO_H
#define ID_RANGO_H

#include "id_server.h"
#include <map>
#include <vector>

struct estado_rango
{
	// Lista de identificadores libres
	std::vector<long> libres;
	// Primer identificador válido
	long first;
	// Proximo identificador a asignar si no hay libres
	long next;
	// Ultimo identificador (exclusivo)
	long max;
};

extern std::map<ids_rango, estado_rango> rangos;

#endif
