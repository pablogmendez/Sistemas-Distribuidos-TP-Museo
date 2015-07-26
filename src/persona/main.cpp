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

	IPuerta ipuerta;
		//mostrarOpciones();
		cout << "Ingresar Tipo Persona  a entrar Normal(n)/Investigador(i) ";
		cin >> op;
		switch(op) {
			case 'n': cout << "Ingresar numero de puerta:";
				  cin >> puerta;
				  retval = ipuerta.entrar(puerta);
				  if(retval == 1) break;
				  cout << "Ingresar puerta por donde salir:";
  				  cin >> puerta;
				  ipuerta.salir(puerta);
				  break;
			case 'i': cout << "Ingresar numero de puerta y cantidad de pertenencias:";
    				  cin >> puerta >> pertenencias;
				  retval = ipuerta.entrar(puerta, pertenencias); 
				  if(retval == -1) break;

				  cout << "La puerta guardó las pertenencias en el locker: " << retval << endl;

				  long res;
				  do {
				    cout << "Ingresar puerta por donde salir y locker:";
				    cin >> puerta >> locker;
				    cout << "Saliendo por puerta " << puerta << " con locker " << locker << endl;
				    res = ipuerta.salir(puerta, locker);
				    if (res == -1) {
				      cout << "Puerta o locker incorrecto. Intente de nuevo."
				           << endl;
				    }
				   } while (res == -1);
				  cout << "Salí. La puerta devolvió las pertenencias " << res << endl;
				  break;
			default:  cout << "Parametro invalido" << endl;
		}
}
