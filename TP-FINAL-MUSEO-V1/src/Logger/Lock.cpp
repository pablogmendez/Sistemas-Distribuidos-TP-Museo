/*
 * Lock.cpp
 *
 *  Created on: 04/04/2014
 *      Author: Emmanuel
 */

#include "Lock.h"
Lock:: Lock (const char* name ) {

	strcpy ( this->name,name );

	this->fl.l_type = F_WRLCK;
	this->fl.l_whence = SEEK_SET;
	this->fl.l_start = 0;
	this->fl.l_len = 0;
	this->fl.l_pid = getpid ();
	this->fd = open ( this->name,O_CREAT|O_WRONLY,0777 );
}

int Lock :: takeLock() {
	this->fl.l_type = F_WRLCK;
	int result = fcntl ( this->fd,F_SETLKW,&(this->fl) );
	return result;
}

int Lock :: freeLock () {
	this->fl.l_type = F_UNLCK;
	int result = fcntl ( this->fd,F_SETLK,&(this->fl) );
	return result;
}

int Lock :: writeLock ( char* buffer,int buffsize ) {
	lseek ( this->fd,0,SEEK_END );
	int result = write ( this->fd,buffer,buffsize );
	return result;
}

Lock :: ~Lock () {
	close ( this->fd );
}
