AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS = -Wall -Wextra -Wconversion
AM_CXXFLAGS = -Wall -Wextra -Wconversion
AM_CPPFLAGS = -I$(srcdir)/include -I$(srdir)

EXTRA_DIST =
bin_PROGRAMS =

noinst_LTLIBRARIES = \
	IPC/libIPC.la \
	sockets/libsockets.la \
	utils/libutils.la \
	ipuerta/libipuerta.la \
	ipersona/libipersona.la

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
	utils/EnvParams.cpp \
	utils/EnvParams.h \
	utils/System.cpp \
	utils/System.h

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
