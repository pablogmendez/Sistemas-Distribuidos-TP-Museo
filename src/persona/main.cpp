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
	long puerta, pertenencias, locker;

	IPuerta ipuerta;
		//mostrarOpciones();
		cout << "Ingresar Tipo Persona  a entrar Normal(n)/Investigador(i) ";
		cin >> op;
		switch(op) {
			case 'n': cout << "Ingresar numero de puerta:";
				  cin >> puerta;
				  ipuerta.entrar(puerta);
				  cout << "Ingresar puerta por donde salir:";
  				  cin >> puerta;
				  ipuerta.salir(puerta);
				  break;
			case 'i': cout << "Ingresar numero de puerta y cantidad de pertenencias:";
    				  cin >> puerta >> pertenencias;
				  ipuerta.entrar(puerta, pertenencias); 
				  cout << "Ingresar puerta por donde salir y locker:";
  				  cin >> puerta >> locker;
				  ipuerta.salir(puerta, locker);
				  break;
			default:  cout << "Parametro invalido" << endl;
		}
}
