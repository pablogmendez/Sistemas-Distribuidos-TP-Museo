AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = -Wall -Wextra -Wconversion
AM_CPPFLAGS = -I$(srcdir)/include -I$(srdir)

noinst_LTLIBRARIES = \
	libIPC.la \
	libsockets.la \
	libutils.la \
	libipuerta.la

libIPC_la_SOURCES = \
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

libsockets_la_SOURCES = \
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

libutils_la_SOURCES = \
	utils/System.cpp \
	utils/System.h

libipuerta_la_SOURCES = \
	include/ipuerta/IPuertaCli.h \
	include/ipuerta/IPuertaServ.h \
	include/ipuerta/Operacion.h \
	ipuerta/IPuertaCli.cpp \
	ipuerta/IPuertaMsg.h \
	ipuerta/IPuertaServ.cpp
libipuerta_la_LIBADD = libIPC.la libutils.la
