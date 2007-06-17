#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.6)

#The following line has the program name, the program version, and the bug report address.
AC_INIT([crimesquad], [3.11.1],[jonathansfox@users.sourceforge.net])
AC_CONFIG_SRCDIR([src/cursesgraphics.cpp])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Defines
# TODO: Conditionally set these depending on build system.
AC_DEFINE([UNIX],[],[Defined if on a UNIX based system])
AC_DEFINE([Linux],[],[Defined if running on Linux])
AC_DEFINE([NCURSES],[],[Defined if we use NCURSES (always true on linux)])


AC_CHECK_LIB(ncurses, initscr, [], [AC_MSG_ERROR([You need ncurses!])], [])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h unistd.h ncurses.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_MALLOC
AC_TYPE_SIGNAL
AC_FUNC_STAT



AC_CHECK_FUNCS([memset mkdir strchr])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
