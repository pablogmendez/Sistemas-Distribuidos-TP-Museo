#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <argp.h>
#include <string>
#include <utils/NonCopyable.h>

class ArgParser : private NonCopyable
{
	private:
		std::string _idServer;
		std::string _broker;
		std::string _recursos;

		ArgParser();
		~ArgParser();

		friend int parserFunc (int key, char *arg, struct argp_state *state);

	public:
		static ArgParser& getInstance();

		void parse (int argc, char **argv);

		const std::string& idServer () const { return _idServer; }
		const std::string& broker () const { return _broker; }
		const std::string& recursos () const { return _recursos; }
};

#endif
