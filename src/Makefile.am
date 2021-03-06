AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS = -Wall -Wextra -Wconversion
AM_CXXFLAGS = -Wall -Wextra -Wconversion
AM_CPPFLAGS = -I$(srcdir)/include -I$(srdir)

EXTRA_DIST =
bin_PROGRAMS =

noinst_HEADERS = \
	include/museo/MuseoMSG.h \
	include/museo/MuseoSHM.h

noinst_LTLIBRARIES = \
	IPC/libIPC.la \
	sockets/libsockets.la \
	utils/libutils.la \
	Logger/libLogger.la \
	ipuerta/libipuerta.la \
	ipersona/libipersona.la \
	imuseo/libimuseo.la 

IPC_libIPC_la_SOURCES = \
	IPC/Cola.h \
	IPC/EventHandler.h \
	IPC/Fifo.cpp \
	IPC/Fifo.h \
	IPC/FifoEscritura.cpp \
	IPC/FifoEscritura.h \
	IPC/FifoLectura.cpp \
	IPC/FifoLectura.h \
	IPC/MemoriaCompartida.h \
	IPC/Pipe.cpp \
	IPC/Pipe.h \
	IPC/SIGINT_Handler.h \
	IPC/Semaforo.cpp \
	IPC/Semaforo.h \
	IPC/SignalHandler.cpp \
	IPC/SignalHandler.h

IPC_libIPC_la_LIBADD = \
	utils/libutils.la

sockets_libsockets_la_SOURCES = \
	sockets/cClientSocket.cpp \
	sockets/cClientSocket.h \
	sockets/cDispatcher.cpp \
	sockets/cDispatcher.h \
	sockets/cServerSocket.cpp \
	sockets/cServerSocket.h \
	sockets/cSocket.cpp \
	sockets/cSocket.h \
	\
	sockets/common.cpp \
	sockets/common.h \
	sockets/cClientProto.cpp \
	sockets/cClientProto.h \
	sockets/cServerProto.cpp \
	sockets/cServerProto.h

utils_libutils_la_SOURCES = \
	utils/EnvParam.cpp \
	utils/EnvParam.h \
	utils/NonCopyable.h \
	utils/ParserConfiguracion.cpp \
	utils/ParserConfiguracion.h \
	utils/System.cpp \
	utils/System.h

Logger_libLogger_la_SOURCES = \
	Logger/MensajeLog.cpp \
	Logger/Lock.h \
	Logger/Lock.cpp \
	Logger/MensajeLog.h \
	Logger/Logger.cpp \
	Logger/Logger.h

ipuerta_libipuerta_la_SOURCES = \
	include/icommon/resultado.h \
	include/ipuerta/IPuerta.h \
	ipuerta/IPuerta.cpp \
	ipuerta/IPuertaMsg.h
ipuerta_libipuerta_la_LIBADD = IPC/libIPC.la utils/libutils.la

ipersona_libipersona_la_SOURCES = \
	include/icommon/resultado.h \
	include/ipersona/IPersona.h \
	include/ipersona/Operacion.h \
	ipersona/IPersona.cpp \
	ipersona/IPersonaMsg.h
ipersona_libipersona_la_LIBADD = IPC/libIPC.la utils/libutils.la

imuseo_libimuseo_la_SOURCES = \
	include/icommon/resultado.h \
	imuseo/IMuseo.h \
	imuseo/IMuseo.cpp \
	imuseo/IMuseoMsg.h
imuseo_libimuseo_la_LIBADD = IPC/libIPC.la utils/libutils.la

############ Generacion de fuentes con rpcgen #############
# A MANO !!!! con script gen_rpc.sh
#
#id-server/id_server.h: id-server/id_server.x
#	rpcgen -h -o $@ $<
#
#id-server/id_server_xdr.c: id-server/id_server.x
#	rpcgen -c -o $@ $<
#
#id-server/id_server_clnt.c: id-server/id_server.x
#	rpcgen -l -o $@ $<
#
#id-server/id_server_svc.c: id-server/id_server.x
#	rpcgen -s udp -s tcp -o $@ $<
#
###########################################################
EXTRA_DIST += id-server/id_server.x id-server/gen_rpc.sh
bin_PROGRAMS += id-server/id-server
id_server_id_server_SOURCES = \
	id-server/ArgParser.cpp \
	id-server/ArgParser.h \
	id-server/id_rango.h \
	id-server/id_server_svc.c \
	id-server/id_server_xdr.c \
	id-server/id_server.h \
	id-server/id_server_impl.cpp \
	id-server/id_server_main.cpp
# La siguiente linea evita error:
#   Objects ‘created with both libtool and without’
id_server_id_server_CFLAGS = $(AM_CFLAGS)

noinst_LTLIBRARIES += id-server/libidclient.la
id_server_libidclient_la_SOURCES = \
	id-server/id_server_clnt.c \
	id-server/id_server_xdr.c \
	id-server/id_server.h \
	id-server/IIdClient.cpp \
	include/id-server/IIdClient.h

########## Broker ##############
# Acá se listan los archivos que no son fuentes o encabezados
# y que necesita el programa
EXTRA_DIST += \
	broker/.colamaestra.mq \
	broker/.colaconexiones.mq \
	broker/.semaforo_shmem \
	broker/.shmem

noinst_HEADERS += \
	broker/Constantes.h \
	broker/Proceso.h

# Se agrega el binario
bin_PROGRAMS += \
	broker/broker \
	broker/broker_entrada \
	broker/broker_salida \
	broker/server_entrada \
	broker/server_salida \
	broker/shmem_get \
	broker/shmem_return \
	broker/prueba_envio \
	broker/prueba_recep \
	broker/prueba_get_shmem \
	broker/broadcast \
	broker/broadcast_personas
	
# Acá se listan los fuentes y encabezados con los que
# se compila el programa
broker_broker_SOURCES = \
	broker/broker.cpp

broker_broker_LDADD = \
	IPC/libIPC.la

broker_broker_entrada_SOURCES = \
	broker/broker_entrada.cpp \
	broker/MensajeGenerico.h

# Linkea con la biblioteca de IPCs
broker_broker_entrada_LDADD = \
	IPC/libIPC.la \
	sockets/libsockets.la

broker_broker_salida_SOURCES = \
	broker/broker_salida.cpp \
	broker/MensajeGenerico.h

broker_broker_salida_LDADD = \
	IPC/libIPC.la \
	sockets/libsockets.la

broker_server_entrada_SOURCES = \
	broker/server_entrada.cpp \
	broker/MensajeGenerico.h

broker_server_entrada_LDADD = \
	IPC/libIPC.la \
	sockets/libsockets.la

broker_server_salida_SOURCES = \
	broker/server_salida.cpp \
	broker/MensajeGenerico.h

broker_server_salida_LDADD = \
	IPC/libIPC.la \
	sockets/libsockets.la

broker_shmem_get_SOURCES = \
	broker/shmem_get.cpp \
	broker/MensajeGenerico.h

broker_shmem_get_LDADD = \
	IPC/libIPC.la \
	sockets/libsockets.la

broker_shmem_return_SOURCES = \
	broker/shmem_return.cpp \
	broker/MensajeGenerico.h

broker_shmem_return_LDADD = \
	IPC/libIPC.la \
	sockets/libsockets.la

broker_prueba_envio_SOURCES = \
	broker/prueba_envio.cpp \
	broker/ConexionMsg.h \
	broker/MensajeGenerico.h

broker_prueba_envio_LDADD = \
	sockets/libsockets.la

broker_prueba_recep_SOURCES = \
	broker/prueba_recep.cpp \
	broker/MensajeGenerico.h

broker_prueba_recep_LDADD = \
	sockets/libsockets.la

broker_prueba_get_shmem_SOURCES = \
	broker/prueba_get_shmem.cpp \
	broker/MensajeGenerico.h

broker_prueba_get_shmem_LDADD = \
	sockets/libsockets.la

broker_broadcast_SOURCES = \
	broker/broadcast.cpp
	broker/MensajeGenerico.h

broker_broadcast_LDADD = \
        IPC/libIPC.la \
        sockets/libsockets.la

broker_broadcast_personas_SOURCES = \
	broker/broadcast_personas.cpp \
	broker/MensajeGenerico.h

broker_broadcast_personas_LDADD = \
        IPC/libIPC.la \
        sockets/libsockets.la

########## Puerta #############
bin_PROGRAMS += puerta/puerta

puerta_puerta_SOURCES = \
	puerta/ArgParser.cpp \
	puerta/ArgParser.h \
	puerta/Locker.h \
	puerta/LockerRack.cpp \
	puerta/LockerRack.h \
	puerta/main.cpp

puerta_puerta_LDADD = \
	ipersona/libipersona.la \
	imuseo/libimuseo.la

bin_PROGRAMS += finalizador-puertas/finalizador-puertas

finalizador_puertas_finalizador_puertas_SOURCES = \
	finalizador-puertas/ArgParser.cpp \
	finalizador-puertas/ArgParser.h \
	finalizador-puertas/main.cpp

finalizador_puertas_finalizador_puertas_LDADD = \
	utils/libutils.la
###############################

##### Componente IPersona #####
bin_PROGRAMS += \
	ipersona_comp/ipersona_comp \
	ipersona_comp/ipersona_comp_lector

ipersona_comp_ipersona_comp_SOURCES = \
	ipersona_comp/ArgParser.cpp \
	ipersona_comp/ArgParser.h \
	ipersona_comp/contador-msg.h \
	ipersona_comp/ipc-keys.h \
	ipersona_comp/IPCManager.cpp \
	ipersona_comp/IPCManager.h \
	ipersona_comp/ipersona_comp.cpp

ipersona_comp_ipersona_comp_LDADD = \
	id-server/libidclient.la \
	IPC/libIPC.la \
	ipersona/libipersona.la \
	sockets/libsockets.la

ipersona_comp_ipersona_comp_lector_SOURCES = \
	ipersona_comp/ArgParserLector.cpp \
	ipersona_comp/ArgParserLector.h \
	ipersona_comp/contador-msg.h \
	ipersona_comp/ipc-keys.h \
	ipersona_comp/IPCManager.cpp \
	ipersona_comp/IPCManager.h \
	ipersona_comp/ipersona_comp_lector.cpp

ipersona_comp_ipersona_comp_lector_LDADD = \
	IPC/libIPC.la \
	utils/libutils.la \
	sockets/libsockets.la
###############################

##### Componente IPuerta #####
bin_PROGRAMS += \
	ipuerta_comp/ipuerta_comp \
	ipuerta_comp/ipuerta_comp_lector

ipuerta_comp_ipuerta_comp_SOURCES = \
	ipuerta_comp/ArgParser.cpp \
	ipuerta_comp/ArgParser.h \
	ipuerta_comp/contador-msg.h \
	ipuerta_comp/ipc-keys.h \
	ipuerta_comp/IPCManager.cpp \
	ipuerta_comp/IPCManager.h \
	ipuerta_comp/ipuerta_comp.cpp

ipuerta_comp_ipuerta_comp_LDADD = \
	id-server/libidclient.la \
	IPC/libIPC.la \
	ipuerta/libipuerta.la \
	sockets/libsockets.la

ipuerta_comp_ipuerta_comp_lector_SOURCES = \
	ipuerta_comp/ArgParserLector.cpp \
	ipuerta_comp/ArgParserLector.h \
	ipuerta_comp/contador-msg.h \
	ipuerta_comp/ipc-keys.h \
	ipuerta_comp/IPCManager.cpp \
	ipuerta_comp/IPCManager.h \
	ipuerta_comp/ipuerta_comp_lector.cpp

ipuerta_comp_ipuerta_comp_lector_LDADD = \
	IPC/libIPC.la \
	utils/libutils.la \
	sockets/libsockets.la \
	Logger/libLogger.la
###############################

########## Persona #############
bin_PROGRAMS += persona/persona
persona_persona_SOURCES = \
	persona/main.cpp

persona_persona_LDADD = \
	ipuerta/libipuerta.la
###############################

########## Lanzador Persona #############
bin_PROGRAMS += persona/personaLanzador
persona_personaLanzador_SOURCES = \
	persona/persona_lanzador.cpp

persona_personaLanzador_LDADD = \
	ipuerta/libipuerta.la
###############################

########## Destructor Persona #############
bin_PROGRAMS += persona/personaDestructor
persona_personaDestructor_SOURCES = \
	persona/persona_destructor.cpp

persona_personaDestructor_LDADD = \
	ipuerta/libipuerta.la
###############################

##### Componente IMuseo #####
bin_PROGRAMS += \
	imuseo_comp/imuseo_comp

imuseo_comp_imuseo_comp_SOURCES = \
	imuseo_comp/imuseo_comp.cpp

imuseo_comp_imuseo_comp_LDADD = \
	id-server/libidclient.la \
	IPC/libIPC.la \
	imuseo/libimuseo.la \
	sockets/libsockets.la \
	Logger/libLogger.la
###############################

########## ADMINISTRADOR #############
bin_PROGRAMS += administrador/administrador
administrador_administrador_SOURCES = \
	administrador/administrador.cpp

administrador_administrador_LDADD = \
	imuseo/libimuseo.la
#####################################
########## Destructor administrador #############
bin_PROGRAMS += administrador/administradorDestructor
administrador_administradorDestructor_SOURCES = \
	administrador/administrador_destructor.cpp

administrador_administradorDestructor_LDADD = \
	imuseo/libimuseo.la
###############################


