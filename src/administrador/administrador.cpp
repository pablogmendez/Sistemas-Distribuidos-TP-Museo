#include <imuseo/IMuseo.h>
#include <iostream>
#include <cstdlib>
using namespace std;

void mostrarAyuda() {
	cout << "Administrador ayuda ..." << endl;
	cout << "./administrador {op} <capacidad>" << endl;
	cout << "op: a - abrir museo" << endl; 
	cout << "op: c - cerrar museo" << endl; 
}

int main(int argc, char **argv) {

if(argc == 1) {
	mostrarAyuda();
	return 1;
}
IMuseo imuseo;
bool retval;
if(*argv[1] == 'a' ){
	retval = imuseo.abrirMuseo(atoi(argv[2]));
	if(retval) {
		cout << "Museo Abierto" << endl;
	}
	else {
		cout << "Error: El museo ya se encuentra abierto" << endl;
	}
}
else {
	retval = imuseo.cerrarMuseo();
	if(retval) {
                cout << "Museo Cerrado" << endl;
        }
        else {
                cout << "Error: El museo ya se encuentra cerrado" << endl;
        }
}
return 0;
}
