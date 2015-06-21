AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = -Wall -Wextra -Wconversion
AM_CPPFLAGS = -I$(srcdir)/../src/include -I$(srcdir)/../src

EXTRA_DIST = \
	ipuerta/ipuerta.mq

check_PROGRAMS = \
	ipuerta/mock_comp \
	ipuerta/test_ipuerta

ipuerta_mock_comp_SOURCES = \
	ipuerta/mock_comp.cpp
ipuerta_mock_comp_LDADD = \
	../src/libipuerta.la

ipuerta_test_ipuerta_SOURCES = \
	ipuerta/main.cpp
ipuerta_test_ipuerta_LDADD = \
	../src/libipuerta.la
