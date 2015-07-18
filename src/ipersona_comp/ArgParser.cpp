#include <argp.h>
#include <cstdlib>
#include <limits>
#include <sys/types.h>
#include <unistd.h>

#include "ArgParser.h"

static const unsigned NUM_ARGS = 3;
int parserFunc (int key, char *arg, struct argp_state *state)
{
	(void) arg;

	ArgParser* argParser = static_cast<ArgParser*> (state->input);
	switch (key) {
		case 'i':
		{
			char* endptr;
			long idLocal = strtol (arg, &endptr, 10);
			if (!(*arg != 0 && *endptr == 0) || idLocal <= 0) {
				argp_failure (state, 1, 0,
						"el id local debe ser mayor a 0.");
			}
			if (idLocal > std::numeric_limits<int>::max ()) {
				argp_failure (state, 1, 0,
						"el id local debe ser menor a %d.",
						std::numeric_limits<int>::max ());
			}
			argParser->_idLocal = static_cast<int> (idLocal);
			break;
		}
		case ARGP_KEY_ARG:
			if (state->arg_num == 0) {
				argParser->_idServer = arg;
			} else if (state->arg_num == 1) {
				argParser->_broker = arg;
			} else if (state->arg_num == 2) {
				argParser->_recursos = arg;
			}
			break;
		case ARGP_KEY_END:
			if (state->arg_num < NUM_ARGS) {
				argp_failure (state, 1, 0, "no hay suficientes argumentos");
			} else if (state->arg_num > NUM_ARGS) {
				argp_failure (state, 1, 0, "demasiados argumentos");
			}
			break;
	}
	return 0;
}

static struct argp_option options[] = {
	{"id-local", 'i', "ID_LOCAL", 0,
		"Id local de la puerta que utiliza este componente.", 0},
	{0, 0, 0, 0, 0, 0}
};

static const char* const argNames = "ID_SERVER BROKER DIR_RECURSOS";
static const char* const doc =
		" "
		"\v"
		"Argumentos:\n"
		"\tID_SERVER   : nombre del equipo que aloja al servidor de\n"
		"\t              identificadores.\n"
		"\tBROKER      : nombre del equipo que aloja al broker.\n"
		"\tDIR_RECURSOS: directorios donde se encuentran los recursos para\n"
		"\t              crear los IPCs.\n"
		"\n";

static struct argp optionParser = {options, parserFunc, argNames, doc, 0, 0, 0};

ArgParser::ArgParser ()
	: _idLocal (1)
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
