#ifndef CONTADOR_MSG_H_
#define CONTADOR_MSG_H_

#include <cstddef>

struct ContadorMsg
{
	std::size_t salidasEspeciales;
	std::size_t salidasEstandar;
	std::size_t entradas;
	std::size_t esperando;
	bool        museoLleno;
};

#endif /* CONTADOR_MSG_H_ */
