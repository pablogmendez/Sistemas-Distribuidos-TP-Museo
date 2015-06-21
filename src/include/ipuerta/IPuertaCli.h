#ifndef IPUERTA_IPUERTACLI_H
#define IPUERTA_IPUERTACLI_H

class IPuertaCli
{
private:
	class Impl;
	Impl* pImpl;
public:
	static const char* ENV_IPUERTA_COMP;

	IPuertaCli ();
	~IPuertaCli ();

	void recorrerMuseo (
		long puertaEntrada,
		long puertaSalida,
		long tiempoRecorrido);

	void iniciarRecorridoEspecial (
		long puerta,
		long tiempoRecorrido/*,
		TODO: pertenencias */);
};

#endif
