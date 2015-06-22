/*
 * Utils.h
 *
 *  Created on: 16/05/2015
 *      Author: pablo
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>

class Utils {
public:
	Utils();
	virtual ~Utils();
	static int string2int(std::string str);
	static std::string int2String (int i);
};

#endif /* UTILS_H_ */
