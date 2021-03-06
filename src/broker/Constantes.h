#ifndef BROKER_CONSTANTES_H
#define BROKER_CONSTANTES_H

const int  BROKER_READERS_PORT = 5000;
const int  BROKER_WRITERS_PORT = 5001;
const long BROKER_SHMGET_ID    = 1;
const long BROKER_SHMRET_ID    = 2;
const long BROKER_BROADCAST_ID = 3;
const long BROKER_BROADCAST_PERSONAS_ID = 4;

const char* BROKER_PIDS_FILE = "procesos"; 

#endif /* BROKER_CONSTANTES_H */
