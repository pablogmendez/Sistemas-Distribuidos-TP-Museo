#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <argp.h>
#include <string>
#include <utils/NonCopyable.h>

class ArgParser : private NonCopyable
{
	private:
		long _offsetPuertas;
		long _offsetPersonas;

		ArgParser();
		~ArgParser();

		friend int parserFunc (int key, char *arg, struct argp_state *state);

		void validar (struct argp_state *state);
	public:
		static ArgParser& getInstance();

		void parse (int argc, char **argv);

		long offsetPuertas () const { return _offsetPuertas; }
		long offsetPersonas () const { return _offsetPersonas; }
};

#endif
