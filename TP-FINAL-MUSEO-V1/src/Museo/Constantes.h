#ifndef CONSTANTES_H_
#define CONSTANTES_H_

#include <iostream>

// IPC Comunes
static const std::string recShmMuseoAbierto = "recursos/shmMuseoAbierto";
static const std::string recShmContMuseo    = "recursos/shmContMuseo";
static const std::string recMtxMuseoAbierto = "recursos/mtxMuseoAbierto";
static const std::string recMtxContMuseo    = "recursos/mtxContMuseo";
static const std::string recColaNorm        = "recursos/colaNorm";
static const std::string recColaEsp         = "recursos/colaEsp";

// IPC Exclusivos para las puertas y sus visitantes
static const std::string recMtxPuerta       = "recursos/mtxPuerta";
static const std::string recMtxVisitante    = "recursos/mtxVisitante";
static const std::string recMtxContNorm     = "recursos/mtxContNorm";
static const std::string recMtxContEsp      = "recursos/mtxContEsp";
static const std::string recShmContNorm     = "recursos/shmContNorm";
static const std::string recShmContEsp      = "recursos/shmContEsp";


static const std::string ejecutablePuerta  = "Debug/Puerta";
static const std::string ejecutablePersona = "Debug/Persona";

static const std::string configuracion = "conf/museo.cfg";
static const std::string procesos      = "tmp/procesos.dat";
static const std::string perDat        = "tmp/personas.dat";

static const int tipoNormal = 1;
static const int tipoInvest = 2;

typedef struct {
	long mtype;   // Puerta por la que ingresa/egresa
	unsigned pid; // Process Id de la persona
	int accion;   // Persona: 1 Entra - 0 Sale. Museo: 0 OK - 1 ERROR
	int tipo;    // Tipo de persona: 0 Normal - 1 Investigador
} persona_t;

#endif /* CONSTANTES_H_ */
