#ifndef IPUERTA_OPERACION_H
#define IPUERTA_OPERACION_H

enum Tipo
{
	RECORRIDO_SIMPLE,  /* El que realizan las personas. */
	RECORRIDO_ESPECIAL /* El que realizan los investigadores. */
};

class Operacion
{
private:
	enum Tipo tipo;
	long rtype;
	long puertaEntrada;
	long puertaSalida;
	long tiempoRecorrido;
	/* TODO: pertenencias */

	Operacion () {}
public:
	static Operacion crearRecorridoEspecial (
			long rtype,
			long puerta,
			long tiempoRecorrido)
	{
		Operacion op;
		op.tipo = RECORRIDO_ESPECIAL;
		op.rtype = rtype;
		op.puertaEntrada = puerta;
		op.puertaSalida = puerta;
		op.tiempoRecorrido = tiempoRecorrido;
		return op;
	}

	static Operacion crearRecorridoSimple (
			long rtype,
			long puertaEntrada,
			long puertaSalida,
			long tiempoRecorrido)
	{
		Operacion op;
		op.tipo = RECORRIDO_SIMPLE;
		op.rtype = rtype;
		op.puertaEntrada = puertaEntrada;
		op.puertaSalida = puertaSalida;
		op.tiempoRecorrido = tiempoRecorrido;
		return op;
	}

	long getRType () const { return rtype; }
	long getPuertaEntrada () const { return puertaEntrada; }
	long getPuertaSalida () const { return puertaSalida; }
	long getTiempoRecorrido () const { return tiempoRecorrido; }
};

#endif
