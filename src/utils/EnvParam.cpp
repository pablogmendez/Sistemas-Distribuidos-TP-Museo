#include <stdexcept>
#include <stdlib.h>
#include <utils/EnvParam.h>

EnvParam::EnvParam (
		const std::string& variable,
		const std::string& deflt)
{
	const char* tmp = getenv (variable.c_str ());
	if (tmp != NULL) {
		param = tmp;
	} else {
		param = deflt;
	}
}
