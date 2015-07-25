#ifndef __IMUSEOMSG_H__ 
#define __IMUSEOMSG_H__ 
#include <stdint.h>
struct IMuseoMsg {
	long mtype;
	uint32_t op; // 1: Entrar Persona - 2: Sacar Persona - 3: Abrir Museo - 4: Cerrar Museo - 5: Op OK - 6: Op ERROR
	uint32_t param_1;
};

#endif /*__IMUSEOMSG_H__*/
