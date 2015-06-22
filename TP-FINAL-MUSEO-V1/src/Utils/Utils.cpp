/*
 * Utils.cpp
 *
 *  Created on: 16/05/2015
 *      Author: pablo
 */

#include "Utils.h"
#include <sstream>
#include <iostream>
Utils::Utils() {
	// TODO Auto-generated constructor stub

}

Utils::~Utils() {
	// TODO Auto-generated destructor stub
}

int Utils::string2int(std::string str) {
	int result;
	std::istringstream ss(str);
	ss.imbue(std::locale::classic());
	ss >> result;
	return result;
}

std::string Utils::int2String ( int i )
{
	std::stringstream ss;
	ss << i;
	return ss.str();
}
