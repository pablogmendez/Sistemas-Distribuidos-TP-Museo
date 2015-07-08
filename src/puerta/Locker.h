#ifndef LOCKER_H_
#define LOCKER_H_

struct Locker
{
	long duenio;
	long pertenencias;

	Locker ()
		: duenio (0)
		, pertenencias (0) { }
};

#endif /* LOCKER_H_ */
