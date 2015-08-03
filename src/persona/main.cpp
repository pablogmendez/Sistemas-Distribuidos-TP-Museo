#include <ipuerta/IPuerta.h>
#include <iostream>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <utils/System.h>
#include <cstring>

using namespace std;

void mostrarOpciones() {
//system("clear");
cout << "================" << endl;
cout << "OPCIONES PERSONA" << endl;
cout << "================" << endl << endl;
cout << "n  [Puerta]                Ingreso de persona normal por la puerta indicada" << endl;
cout << "i  [Puerta] [Pertenencias] Ingreso de Investigador por la puerta indicada con pertenencias" << endl;
cout << "r  [Puerta]                Salida de persona normal por la puerta indicada" << endl;
cout << "s  [Puerta] [Locker] 	    Salida de Investigador por la puerta indicada con numero de locker" << endl;
cout << "q		 	    Terminar aplicacion" << endl;
}

int main (int argc, char** argv)
{
	char op;
	long puerta, pertenencias, locker, retval;
srand(time(NULL));
rand();
rand();
rand();
rand();
	IPuerta ipuerta;
		//mostrarOpciones();
		op = *argv[1];
		switch(op) {
			case 'n': //cout << "Ingresar numero de puerta:";
				  //cin >> puerta;
				  puerta = rand()%2 +100 ;
				  retval = ipuerta.entrar(puerta);
				  if(retval == 1) break;
				  sleep(rand()%20 + 1);
				  ipuerta.salir(rand()%2 +100);
				  break;
			case 'i': {
				  //cout << "Ingresar numero de puerta y cantidad de pertenencias:";
    				  //cin >> puerta >> pertenencias;
				puerta = rand()%2 +100 ;
				  retval = ipuerta.entrar(puerta, rand()%2 +100); 
				  if(retval == -1) break;

				  cout << "La puerta guardó las pertenencias en el locker: " << retval << endl;
				  sleep(rand()%5 + 1);
				  long res, puertaEntrada = puerta, lockerUsado = retval;
				  do {
				    cout << "Saliendo por puerta " << puerta << " con locker " << locker << endl;
				    res = ipuerta.salir(puerta, retval);
				    if (res == -1) {
				      cout << "Puerta o locker incorrecto. Intente de nuevo."
				           << endl;
				      cout << "Recuerde que entro por puerta " << puertaEntrada
				           << " y que su locker es " << lockerUsado
				           << '.' << endl;
				    }
				   } while (res == -1);
				  cout << "Salí. La puerta devolvió las pertenencias " << res << endl;
				  break;
			}
			default:  cout << "Parametro invalido" << endl;
		}
}
