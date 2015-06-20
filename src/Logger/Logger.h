/*
 * Logger.h
 *
 *  Created on: 03/04/2014
 *      Author: Emmanuel
 */


#ifndef LOGGER_H_
#define LOGGER_H_

#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Lock.h"

#define LOG(...) { \
	char evento[100]; \
	char salto[10]; \
	sprintf(evento, __VA_ARGS__); \
	write(fileno(stdout), evento, strlen(evento)); \
	sprintf(salto, "\n"); \
	write(fileno(stdout), salto, strlen(salto)); \
}

class Logger {

private:
    static Logger* instance;
    Lock* lockLog;
    Lock* lockSynchro;
    Lock* lockDebug;


    Logger ( void );
    void log(std::string event);
    void debug(std::string event);
    void synchro(std::string event);
    void print(std::string event);

    void setLogFileName();
    std::string getString(int value);
    std::string getHeader();
    std::string getTimeStamp();

    bool modeDebug;
    bool modeLog;
    bool modeSynchro;
    bool modeCout;

    std::string processName;

public:
    static Logger* getInstance ();
    void destroit ();
    void loggearLog(std::string event);
    void loggearDebug(std::string event);
    void loggearCout(std::string event);
    void loggearSynchro(std::string event);

    void setFlags(bool modeDebug, bool modeLog, bool modeSynchro, bool modeCout);
    void setProcessName(std::string name);

};

#endif /* LOGGER_H_ */
