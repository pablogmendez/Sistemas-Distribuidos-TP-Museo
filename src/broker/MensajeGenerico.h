#ifndef MENSAJE_GENERICO_H
#define MENSAJE_GENERICO_H

#include <museo/MuseoMSG.h>
#include <museo/MuseoSHM.h>

typedef struct {
	long     mtype;
	long     id;
	MuseoMSG msg;
	MuseoSHM shmem;
} MensajeGenerico;

#endif /* MENSAJE_GENERICO_H */
