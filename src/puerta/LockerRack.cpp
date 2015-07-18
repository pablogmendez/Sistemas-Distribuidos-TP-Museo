#include "LockerRack.h"

#include <algorithm>
#include <cstddef>
#include <sstream>
#include <stdexcept>

LockerRack::LockerRack (int tamanio)
{
	if (tamanio <= 0) {
		throw std::logic_error (
				"LockerRack: tamanio debe ser mayor a 0.");
	}

	std::size_t n = static_cast<std::size_t>(tamanio);
	rack.reserve (n);
	for (std::size_t i = 0; i < n; i++) {
		libres.push_back (i);
	}
}

long LockerRack::guardar (long duenio, long pertenencias)
{
	if (libres.size () == 0) {
		return LockerRack::NO_HAY_LUGAR;
	}

	long lockerIdx = libres.front ();
	libres.pop_front ();

	if (rack[lockerIdx].duenio != 0) {
		std::ostringstream oss;
		oss << "LockerRack::guardar:"
			<< " el locker " << lockerIdx
			<< " aún está en uso por el investigador "
			<< rack[lockerIdx].duenio
			<< " y se lo ingresó a la lista de libres.";
		throw std::logic_error (oss.str ());
	}

	rack[lockerIdx].duenio = duenio;
	rack[lockerIdx].pertenencias = pertenencias;
	return lockerIdx;
}

long LockerRack::duenio (long lockerIdx)
{
	if (lockerIdx < 0
			|| static_cast<size_t> (lockerIdx) >= rack.capacity ()) {
		std::ostringstream oss;
		oss << "LockerRack::duenio: "
			<< "locker " << lockerIdx
			<< " no pertenece al rack.";
		throw std::logic_error (oss.str ());
	}

	long id = rack[lockerIdx].duenio;
	if (id == 0) {
		return LockerRack::LOCKER_LIBRE;
	}
	return id;
}

Locker LockerRack::retirar (long lockerIdx)
{
	if (lockerIdx < 0
			|| static_cast<size_t> (lockerIdx) >= rack.capacity ()) {
		std::ostringstream oss;
		oss << "LockerRack::retirar: "
			<< "locker " << lockerIdx
			<< " no pertenece al rack.";
		throw std::logic_error (oss.str ());
	}

	Locker tmp;
	std::swap (tmp, rack[lockerIdx]);
	libres.push_front (lockerIdx);
	return tmp;
}
