#ifndef IPUERTA_IPUERTA_H
#define IPUERTA_IPUERTA_H

class IPuerta
{
private:
	class Impl;
	Impl* pImpl;
public:
	static const char* const ENV_IPUERTA_COMP;
	static const char* const ENV_IPUERTA_PARAMS;

	IPuerta ();
	~IPuerta ();

	/**
	 * Utilizado por las personas.
	 *
	 * Entra al museo por la puerta cuyo identificador
	 * es 'puerta'
	 */
	void entrar (long puerta);

	/**
	 * Utilizado por los investigadores.
	 *
	 * Entra al museo por la puerta cuyo identificador
	 * es 'puerta' entregando las pertenencias dadas.
	 *
	 * Devuelve el número de locker donde se almacenaron
	 * las pertenencias.
	 */
	long entrar (long puerta, long pertenencias);

	/**
	 * Utilizado por las personas.
	 *
	 * Sale del museo por la puerta cuyo identificador
	 * es 'puerta'
	 */
	void salir (long puerta);

	/**
	 * Utilizado por los investigadores.
	 *
	 * Sale del museo por la puerta cuyo identificador
	 * es 'puerta'.
	 *
	 * Devuelve las pertenencias almacenadas en el locker
	 * cuyo número es 'numeroLocker'.
	 */
	long salir (long puerta, long numeroLocker);
};

#endif
