#include <argp.h>
#include <cstdlib>
#include <limits>
#include <sys/types.h>
#include <unistd.h>

#include "ArgParser.h"

static const unsigned NUM_ARGS = 2;
int parserFunc (int key, char *arg, struct argp_state *state)
{
	ArgParser* argParser = static_cast<ArgParser*> (state->input);
	switch (key) {
		case ARGP_KEY_ARG:
			if (state->arg_num == 0) {
				char* endptr;
				long offPuertas = strtol (arg, &endptr, 10);
				if (!(*arg != 0 && *endptr == 0) || offPuertas <= 0) {
					argp_failure (state, 1, 0,
							"el offset de la primer puerta debe "
							"ser mayor a 0.");
				}
				argParser->_offsetPuertas = offPuertas;
			} else if (state->arg_num == 1) {
				char* endptr;
				long offPersonas = strtol (arg, &endptr, 10);
				if (!(*arg != 0 && *endptr == 0) || offPersonas <= 0) {
					argp_failure (state, 1, 0,
							"el offset de la primer persona debe "
							"ser mayor a 0.");
				}
				argParser->_offsetPersonas = offPersonas;
			}
			break;
		case ARGP_KEY_END:
			if (state->arg_num < NUM_ARGS) {
				argp_failure (state, 1, 0, "no hay suficientes argumentos");
			} else if (state->arg_num > NUM_ARGS) {
				argp_failure (state, 1, 0, "demasiados argumentos");
			}
			argParser->validar (state);
			break;
	}
	return 0;
}

static struct argp_option options[] = {
	{0, 0, 0, 0, 0, 0}
};

static const char* const argNames = "OFFSET_PUERTAS OFFSET_PERSONAS";
static const char* const doc =
		" "
		"\v"
		"Argumentos:\n"
		"\tOFFSET_PUERTAS : Primer identificador del rango asignado a\n"
		"\t                 las puertas.\n"
		"\tOFFSET_PERSONAS: Primer identificador del rango asignado a\n"
		"\t                 las personas e investigadores.\n"
		"\n"
		"OFFSET_PUERTAS debe ser menor a OFFSET_PERSONAS.\n"
		"El número de puertas que pueden registrarse está dado por\n"
		"la diferencia entre OFFSET_PERSONAS y OFFSET_PUERTAS.\n"
		"\n";

static struct argp optionParser = {options, parserFunc, argNames, doc, 0, 0, 0};

ArgParser::ArgParser ()
	: _offsetPuertas (1)
	, _offsetPersonas (101)
{
}

ArgParser::~ArgParser ()
{
}

ArgParser& ArgParser::getInstance ()
{
	static ArgParser parser;
	return parser;
}

void ArgParser::parse (int argc, char** argv)
{
	argp_parse (&optionParser, argc, argv, 0, 0, this);
}

void ArgParser::validar (struct argp_state *state)
{
	if (_offsetPersonas <= _offsetPuertas) {
		argp_failure (state, 1, 0,
				"OFFSET_PUERTAS debe ser menor a OFFSET_PERSONAS.");
	}
}
