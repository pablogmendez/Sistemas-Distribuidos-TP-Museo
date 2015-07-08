#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])
AC_INIT([tp-museo], [1.0.0], [])
AC_CONFIG_SRCDIR([src/utils/System.cpp])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
LT_INIT

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_MODE_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([strerror error])

#AC_CHECK_FUNC([argp_parse], ,
#	AC_MSG_ERROR([argp library not found.]))

AC_CHECK_FUNC([semget], ,
	AC_MSG_ERROR([semget is required]))

AC_CHECK_FUNC([shmget], ,
	AC_MSG_ERROR([shmget is required]))

AC_CHECK_FUNC([msgget], ,
	AC_MSG_ERROR([msgget is required]))

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 tests/Makefile])
AC_CONFIG_LINKS([
		tests/ipuerta/ipuerta.mq:tests/ipuerta/ipuerta.mq
		tests/sockets/ops.txt:tests/sockets/ops.txt
		tests/sockets/README:tests/sockets/README
		src/broker/.colamaestra.mq:src/broker/.colamaestra.mq
		src/broker/.colaconexiones.mq:src/broker/.colaconexiones.mq
		resources/ipersona.mq:resources/ipersona.mq])

AC_OUTPUT
