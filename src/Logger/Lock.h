/*
 * Lock.h
 *
 *  Created on: 04/04/2014
 *      Author: Emmanuel
 */

#ifndef LOCK_H_
#define LOCK_H_

#include <unistd.h>
#include <fcntl.h>
#include <string.h>

class Lock {

private:

        struct flock fl;
        int fd;
        char name [ 255 ];

public:

        Lock (const char* name );
        virtual ~Lock();

        int takeLock ();
        int freeLock ();
        int writeLock ( char* buffer,int buffsize );
};

#endif /* LOCK_H_ */
