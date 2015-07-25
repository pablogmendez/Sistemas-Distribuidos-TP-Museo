AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = -Wall -Wextra -Wconversion
AM_CPPFLAGS = -I$(srcdir)/../src/include -I$(srcdir)/../src

EXTRA_DIST = \
	ipuerta/ipuerta.mq

TESTS = \
	id-server/test-id-server \
	ipersona/test_ipersona

check_PROGRAMS = \
	ipersona/mock_comp \
	ipersona/test_ipersona \
	sockets/client \
	sockets/server \
	sockets/serverproto \
	id-server/test-id-server

ipersona_mock_comp_SOURCES = \
	ipersona/mock_comp.cpp
ipersona_mock_comp_LDADD = \
	../src/IPC/libIPC.la \
	../src/utils/libutils.la

ipersona_test_ipersona_SOURCES = \
	ipersona/main.cpp
ipersona_test_ipersona_LDADD = \
	../src/ipersona/libipersona.la

ipuerta_mock_comp_SOURCES = \
	ipuerta/mock_comp.cpp
ipuerta_mock_comp_LDADD = \
	../src/IPC/libIPC.la \
	../src/utils/libutils.la

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

id_server_test_id_server_SOURCES = \
	id-server/main.cpp
id_server_test_id_server_LDADD = \
	../src/id-server/libidclient.la \
	../src/utils/libutils.la
