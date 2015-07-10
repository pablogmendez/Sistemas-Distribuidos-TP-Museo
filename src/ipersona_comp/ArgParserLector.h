#ifndef ARGPARSERLECTOR_H
#define ARGPARSERLECTOR_H

#include <argp.h>
#include <string>
#include <utils/NonCopyable.h>

class ArgParserLector : private NonCopyable
{
	private:
		std::string _mqInterfaz;
		std::string _broker;
		std::string _recursos;
		long        _idPuerta;
		int         _idLocal;

		ArgParserLector();
		~ArgParserLector();

		friend int parserFunc (int key, char *arg, struct argp_state *state);

	public:
		static ArgParserLector& getInstance();

		void parse (int argc, char **argv);

		const std::string& mqInterfaz () const { return _mqInterfaz; }
		const std::string& broker () const { return _broker; }
		const std::string& recursos () const { return _recursos; }
		long idPuerta () const { return _idPuerta; }
		int idLocal () const { return _idLocal; }
};

#endif
