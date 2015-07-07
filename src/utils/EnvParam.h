#ifndef ICOMMON_ENVPARAM_H
#define ICOMMON_ENVPARAM_H

#include <map>
#include <string>

class EnvParam
{
	private:
		std::string param;
	public:
		EnvParam (
				const std::string& variable,
				const std::string& deflt);
		const std::string& get () const { return param; }
};

#endif
