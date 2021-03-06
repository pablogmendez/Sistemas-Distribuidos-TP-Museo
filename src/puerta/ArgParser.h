#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <argp.h>
#include <string>
#include <utils/NonCopyable.h>

class ArgParser : private NonCopyable
{
	private:
		int _idLocal;
		std::string _sesion;

		ArgParser();
		~ArgParser();

		friend int parserFunc (int key, char *arg, struct argp_state *state);

	public:
		static ArgParser& getInstance ();

		void parse (int argc, char **argv);

		int idLocal () const { return _idLocal; }
		const std::string& sesion () const { return _sesion; }
};

#endif
