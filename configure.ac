#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.65)

#The following line has the program name, the program version, and the bug report address.
AC_INIT([crimesquad], [4.10.0],[kamaljalals@gmail.com])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR([config.aux])
AC_CONFIG_MACRO_DIRS([config.m4])
AM_INIT_AUTOMAKE([subdir-objects foreign 1.11 -Wno-syntax -Wall])
AM_SILENT_RULES([yes])
AC_CANONICAL_HOST

# Checks for programs.
AM_PROG_AR
LT_INIT([disable-shared])
AC_PROG_CC
AC_PROG_CXX
AC_LANG(C++)

old_cxxflags=$CXXFLAGS
AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])
AM_CXXFLAGS=$CXXFLAGS
AC_SUBST([AM_CXXFLAGS])
CXXFLAGS=$old_cxxflags

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks for library functions.
#
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_CHECK_FUNCS([memset mkdir strchr])

AC_CHECK_HEADER([langinfo.h],
                [AC_DEFINE([HAVE_LANGINFO_H], [1],
                    [Define is you have <lamginfo.h>.])])

dnl do this until libSDL is fully integrated into the project
AC_DEFINE(DONT_INCLUDE_SDL, [], [Define to avoid including SDL2 and SDL2_mixer.])

# Some special handling for WIndows builds
AS_CASE([$host_os],
        [*mingw32],[build_windows=true])
AM_CONDITIONAL([BUILD_WIN32],[test x$build_windows = xtrue])

AC_CONFIG_SUBDIRS([lib/libsdl2])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([lib/Makefile])
AC_CONFIG_FILES([lib/pdcurses/Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_OUTPUT
