#include <iostream>
#include <IPC/Cola.h>
#include <Logger/Logger.h>
#include <imuseo/IMuseoMsg.h>

using namespace std;

static const std::string ARCHIVO_COLA="/etc/hosts";
static const char LETRA_COLA='B';

int main(int argc, char ** argv) {
        LOG("DESTRUCTOR_ADMINISTRADOR: Destruyendo IPC Cola");
        Cola<IMuseoMsg> colaMsg(ARCHIVO_COLA, LETRA_COLA);
        colaMsg.destruir();
        LOG("DESTRUCTOR_ADMINISTRADOR: Cola destruida");
return 0;
}
