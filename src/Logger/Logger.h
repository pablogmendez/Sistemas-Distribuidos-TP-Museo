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

inline ssize_t locked_write (int fd, char* buf, size_t size)
{
	ssize_t err;
	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	err = fcntl (fd, F_SETLKW, &fl);
	if (err == 0) {
		err = write (fd, buf, size);
		fl.l_type = F_UNLCK;
		fcntl (fd, F_SETLK, &fl);
	}
	return err;
}

#define LOG(...) { \
	size_t evento_len; \
	ssize_t err; \
	char evento[1024]; \
	sprintf(evento, __VA_ARGS__); \
	evento_len = strlen(evento); \
	evento[evento_len] = '\n'; \
	err = locked_write(fileno(stdout), evento, evento_len + 1); \
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
