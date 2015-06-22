/*
 * Logger.cpp
 *
 *  Created on: 03/04/2014
 *      Author: Emmanuel
 */

#include "Logger.h"
#include <cstdio>

using namespace std;

Logger* Logger::instance = NULL;

static const std::string FILE_LOG	= "Log/logger.log";
static const std::string FILE_SYNCHRO	= "Log/synchro.log";
static const std::string FILE_DEBUG  	= "Log/debug.log";

Logger :: Logger() {
    lockLog     = new Lock((FILE_LOG).c_str());
    lockSynchro = new Lock((FILE_SYNCHRO).c_str());
    lockDebug   = new Lock((FILE_DEBUG).c_str());

    this->setFlags(true, true, true, true);
}

Logger* Logger :: getInstance () {
	if ( instance == NULL ){
			instance = new Logger ();
	}
	return instance;
}

void Logger :: destroit () {
	if ( instance != NULL ) {
        delete(lockLog);
        delete(lockSynchro);
        delete(lockDebug);
        delete ( instance );
        instance = NULL;
	}
}

std::string Logger::getString(int value) {
   char texto[32];
   sprintf(texto, "%d", value);
   string message = texto;
   return message;
}

std::string Logger::getHeader(){
    std::string message = "(" + getString(getppid()) + ")(" + getString(getpid()) + ")   ";
	return message;
}

std::string Logger::getTimeStamp() {
	char buff[20];
	time_t now = time(NULL);
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
	return buff;
}

void Logger::log(std::string event){
	if (this->modeLog) {
	    std::string message = getTimeStamp() + getHeader() + event + "\n";
	    //Se convierte a char*
	    char * mens = (char *)strdup(message.c_str());
	    lockLog->takeLock();
	    lockLog->writeLock(mens,message.length());
	    lockLog->freeLock();
	    free(mens);
	}

}

void Logger::debug(std::string event){
	if (this->modeDebug){
		std::string message = getTimeStamp() + getHeader() + event + "\n";
	    //Se convierte a char*
	    char * mens = (char *)strdup(message.c_str());
	    lockDebug->takeLock();
	    lockDebug->writeLock(mens,message.length());
	    lockDebug->freeLock();
	    free(mens);
	}
}

void Logger::synchro(std::string event){
	if (this->modeSynchro){
		std::string message = getTimeStamp() + getHeader() + event + "\n";
	    //Se convierte a char*
	    char * mens = (char *)strdup(message.c_str());
	    lockSynchro->takeLock();
	    lockSynchro->writeLock(mens,message.length());
	    lockSynchro->freeLock();
	    free(mens);
	}
}

void Logger::print(std::string event){

	if (this->modeCout){
		std::string message = getTimeStamp() + getHeader() + event + "\n";
		cout << message << endl;
	}
}

void Logger::setFlags(bool modeDebug, bool modeLog, bool modeSynchro, bool modeCout){
	this->modeDebug = modeDebug;
	this->modeLog   = modeLog;
	this->modeSynchro = modeSynchro;
	this->modeCout  = modeCout;
}

void Logger::loggearLog(std::string event){
	log(event);
	debug(event);
	print(event);
}
void Logger::loggearDebug(std::string event){
	debug(event);
	print(event);
}

void Logger::loggearSynchro(std::string event){
	synchro(event);
	debug(event);
	print(event);
}

void Logger::loggearCout(std::string event){
	print(event);
}



//void Logger::log(std::string event) {
//	std::string name = processName;
//	std::string msg = name + ": " + event;
//
//	loggearDebug(msg);
//}


void Logger::setProcessName(std::string name) {
	processName = name;
}
