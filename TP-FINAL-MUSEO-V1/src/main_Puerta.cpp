#include "Museo/Puerta.h"
#include <cstdlib>
#include "Logger/Logger.h"
#include "Utils/Utils.h"

int main(int argc, char **argv) {
	Puerta puerta(atoi(argv[1]));
	puerta.abrir();
	return 0;
}
