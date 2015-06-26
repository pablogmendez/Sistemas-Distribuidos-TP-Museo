AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = -Wall -Wextra -Wconversion
AM_CPPFLAGS = -I$(srcdir)/../src/include -I$(srcdir)/../src

EXTRA_DIST = \
	ipuerta/ipuerta.mq

check_PROGRAMS = \
	ipuerta/mock_comp \
	ipuerta/test_ipuerta \
	sockets/client \
	sockets/server \
	sockets/serverproto

ipuerta_mock_comp_SOURCES = \
	ipuerta/mock_comp.cpp
ipuerta_mock_comp_LDADD = \
	../src/ipuerta/libipuerta.la

ipuerta_test_ipuerta_SOURCES = \
	ipuerta/main.cpp
ipuerta_test_ipuerta_LDADD = \
	../src/ipuerta/libipuerta.la

sockets_client_SOURCES = \
	sockets/client.cpp
sockets_client_LDADD = \
	../src/sockets/libsockets.la

sockets_server_SOURCES = \
	sockets/server.cpp
sockets_server_LDADD = \
	../src/sockets/libsockets.la

sockets_serverproto_SOURCES = \
	sockets/serverproto.cpp
sockets_serverproto_LDADD = \
	../src/sockets/libsockets.la
