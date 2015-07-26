#ifndef LOCKERRACK_H_
#define LOCKERRACK_H_

#include "Locker.h"

#include <list>
#include <vector>

class LockerRack
{
private:
	int tamanio;
	std::list<long> libres;
	std::vector<Locker> rack;
public:
	static const long NO_HAY_LUGAR = -1;
	static const long LOCKER_LIBRE = -2;

	LockerRack (int tamanio);

	long guardar (long duenio, long pertenencias);
	long duenio (long locker);
	Locker retirar (long locker);
	void clear ();
};

#endif /* LOCKERRACK_H_ */
