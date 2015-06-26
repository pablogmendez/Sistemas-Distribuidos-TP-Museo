AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = -Wall -Wextra -Wconversion
AM_CPPFLAGS = -I$(srcdir)/include -I$(srdir)

noinst_LTLIBRARIES = \
	IPC/libIPC.la \
	sockets/libsockets.la \
	utils/libutils.la \
	ipuerta/libipuerta.la

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
	utils/System.cpp \
	utils/System.h

ipuerta_libipuerta_la_SOURCES = \
	include/ipuerta/IPuertaCli.h \
	include/ipuerta/IPuertaServ.h \
	include/ipuerta/Operacion.h \
	ipuerta/IPuertaCli.cpp \
	ipuerta/IPuertaMsg.h \
	ipuerta/IPuertaServ.cpp
ipuerta_libipuerta_la_LIBADD = IPC/libIPC.la utils/libutils.la
