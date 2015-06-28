#ifndef ICOMMON_ENVPARAMS_H
#define ICOMMON_ENVPARAMS_H

#include <cstdarg>
#include <map>
#include <string>

struct ParamDefaults
{
	std::map<std::string, std::string> dflts;

	ParamDefaults (const char* first...)
	{
		va_list args;
		const char* key = first;
		va_start(args, first);
		while (key != NULL) {
			const char* value = va_arg(args, char*);
			dflts[key] = value;
			key = va_arg(args, char*);
		}
		va_end(args);
	}
};

class EnvParams
{
	private:
		std::map<std::string, std::string> params;
	public:
		EnvParams (
				const ParamDefaults& defaults,
				const std::string& variable);
		const std::string& get_param (const std::string& key) const;
};

#endif
