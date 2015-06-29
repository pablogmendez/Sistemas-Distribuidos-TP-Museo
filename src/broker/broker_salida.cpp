#include "../IPC/Cola.h"

#define COLA_MAESTRA "./colamaestra.mq"
#define COLA_CONEXIONES "./colaconexiones.mq"

#include <iostream>
#include <stdlib.h>

int main(int argc,char* argv[]){
        int cola = atoi(argv[1]);
        std::cout << cola;

        // Sacar de la cola y mandarlo
}

