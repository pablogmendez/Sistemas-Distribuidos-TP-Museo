#ifndef ICOMMON_RESULTADO_H
#define ICOMMON_RESULTADO_H

enum ResultadoOperacionEntrada
{
	ENTRO         = 1,
	NO_HAY_LOCKER    ,
	CERRADO
};

static inline const char* strResultadoOperacionEntrada
		(ResultadoOperacionEntrada op)
{
	#define RET_OP_STR(o) case o : return "ResultadoOperacionEntrada::"#o
	switch (op) {
		RET_OP_STR (ENTRO);
		RET_OP_STR (NO_HAY_LOCKER);
		RET_OP_STR (CERRADO);
		default: return "desconocida";
	}
	#undef RET_OP_STR
}

enum ResultadoOperacionSalida
{
	SALIO             = 1,
	PUERTA_INCORRECTA
};

static inline const char* strResultadoOperacionSalida
		(ResultadoOperacionSalida op)
{
	#define RET_OP_STR(o) case o : return "ResultadoOperacionSalida::"#o
	switch (op) {
		RET_OP_STR (SALIO);
		RET_OP_STR (PUERTA_INCORRECTA);
		default: return "desconocida";
	}
	#undef RET_OP_STR
}
#endif
