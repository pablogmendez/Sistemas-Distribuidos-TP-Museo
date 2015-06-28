#include <stdexcept>
#include <stdlib.h>
#include <utils/EnvParams.h>

// TODO: la idea es que parsee pares clave/valor de la variable,
// por ahora solo sirve para definir el archivo a partir del que
// se crea la cola.

EnvParams::EnvParams (
		const ParamDefaults& defaults,
		const std::string& variable)
{
	const char* tmp = getenv (variable.c_str ());
	if (tmp != NULL) {
		params["MQ"] = tmp;
	} else {
		// FIXME: esta arrojando excepcion cuando no hay default
		params["MQ"] = defaults.dflts.at ("MQ");
	}
}

const std::string& EnvParams::get_param (const std::string& key) const
{
	if (key == "MQ") {
		return params.at (key);
	}
	throw std::logic_error ("FIXME: Solo el parámetro MQ es soportado.");
}
