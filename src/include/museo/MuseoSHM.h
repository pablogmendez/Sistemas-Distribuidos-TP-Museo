#ifndef MUSEO_SHM_H
#define MUSEO_SHM_H

#include <iostream>

struct MuseoSHM
{
	int abierto;
	int capacidad;
	int personas;	
};

static inline
std::ostream& operator << (std::ostream& os, const MuseoSHM& m)
{
	os << "{ "
		<< m.abierto
		<< ", " << m.capacidad
		<< ", " << m.personas << "}";
	return os;
}

#endif /* MUSEO_SHM_H */
