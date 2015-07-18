#include <argp.h>
#include <cstdlib>
#include <limits>
#include <sys/types.h>
#include <unistd.h>

#include "ArgParser.h"

static const unsigned NUM_ARGS = 1;
int parserFunc (int key, char *arg, struct argp_state *state)
{
	ArgParser* argParser = static_cast<ArgParser*> (state->input);
	switch (key) {
		case ARGP_KEY_ARG:
			if (state->arg_num == 0) {
				argParser->_sesion = arg;
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
	{0, 0, 0, 0, 0, 0}
};

static const char* const argNames = "ARCHIVO_SESION";
static const char* const doc = \
	" "
	"\v"
	"Argumentos:\n"
	"\tARCHIVO_SESION: archivo desde donde se obtienen las\n"
	"\t                puertas iniciadas en este equipo.\n"
	"\n";

static struct argp optionParser = {options, parserFunc, argNames, doc, 0, 0, 0};

ArgParser::ArgParser ()
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
