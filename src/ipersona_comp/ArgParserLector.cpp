#include <argp.h>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>

#include "ArgParserLector.h"

static const unsigned NUM_ARGS = 4;
int parserFunc (int key, char *arg, struct argp_state *state)
{
	(void) arg;

	ArgParserLector* argParser = static_cast<ArgParserLector*> (state->input);
	switch (key) {
		case ARGP_KEY_ARG:
			if (state->arg_num == 0) {
				argParser->_mqInterfaz = arg;
			} else if (state->arg_num == 1) {
				argParser->_broker = arg;
			} else if (state->arg_num == 2) {
				argParser->_recursos = arg;
			} else if (state->arg_num == 3) {
				char* endptr;
				long idPuerta = strtol (arg, &endptr, 10);
				if (!(*arg != 0 && *endptr == 0) || idPuerta <= 0) {
					argp_failure (state, 1, 0,
							"el id debe ser mayor a 0.");
				}
				argParser->_idPuerta = idPuerta;
			}
			break;
		case ARGP_KEY_END:
			if (state->arg_num < NUM_ARGS) {
				argp_failure (state, 1, 0,
						"no hay suficientes argumentos");
			} else if (state->arg_num > NUM_ARGS) {
				argp_failure (state, 1, 0,
						"demasiados argumentos");
			}
			break;
	}
	return 0;
}

static struct argp_option options[] = {
	{0, 0, 0, 0, 0, 0}
};

static const char* const argNames = "MQ_INTERFAZ BROKER DIR_RECURSOS ID";
static const char* const doc =
		" "
		"\v"
		"Argumentos:\n"
		"\tMQ_INTERFAZ : ruta utilizada para conectar a la cola de la\n"
		"\t              interfaz.\n"
		"\tBROKER      : nombre del equipo que aloja al broker.\n"
		"\tDIR_RECURSOS: directorios donde se encuentran los recursos para\n"
		"\t              crear los IPCs.\n"
		"\tID          : id de la puerta utilizado para comunicarse con el\n"
		"\t              broker.\n"
		"\n";

static struct argp optionParser = {options, parserFunc, argNames, doc, 0, 0, 0};

ArgParserLector::ArgParserLector ()
	: _idPuerta (0)
{
}

ArgParserLector::~ArgParserLector ()
{
}

ArgParserLector& ArgParserLector::getInstance ()
{
	static ArgParserLector parser;
	return parser;
}

void ArgParserLector::parse (int argc, char** argv)
{
	argp_parse (&optionParser, argc, argv, 0, 0, this);
}
