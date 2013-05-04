#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.6)

#The following line has the program name, the program version, and the bug report address.
AC_INIT([crimesquad], [4.06.3],[jonathansfox@users.sourceforge.net])
AC_CONFIG_SRCDIR([src/cursesgraphics.cpp])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE
# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

# Defines
# TODO: Conditionally set these depending on build system.
AC_DEFINE([UNIX],[],[Defined if on a UNIX based system])
AC_DEFINE([Linux],[],[Defined if running on Linux])
AC_DEFINE([NCURSES],[],[Defined if we use NCURSES (always true on linux)])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h unistd.h ncurses.h ncursesw/ncurses.h])

# Check for ncurses libraries, preferring wide character ones:
for lib in ncurses ncursesw; do
   if test "$lib" = ncursesw; then
      name="NCURSES_W"
      header="$ac_cv_header_ncursesw_ncurses_h"
   else
      name="NCURSES"
      header="$ac_cv_header_ncurses_h"
   fi

   if test "$header" = "yes"; then
      AC_CHECK_LIB($lib, addch, [
         AC_CHECK_LIB($lib, add_wch,
            [wide_ncurses="$wide_ncurses $lib"],
            [plain_ncurses="$plain_ncurses $lib"]
         )
      ])
   fi
done

AC_SEARCH_LIBS(initscr, [$wide_ncurses $plain_ncurses], [],
   [AC_MSG_ERROR([You need ncurses!])])

for lib in $wide_ncurses; do
   if test "$ac_cv_search_initscr" = "-l$lib"; then
      AC_DEFINE(HAVE_WIDE_NCURSES, [],
         [Define if you have wide character support in your ncurses.])
   fi
done

# Tell the headers which one to use.
if test "$ac_cv_search_initscr" = "-lncurses"; then
   AC_DEFINE(USE_NCURSES, [], [Define to use libncurses.])
elif test "$ac_cv_search_initscr" = "-lncursesw"; then
   AC_DEFINE(USE_NCURSES_W, [], [Define to use libncursesw.])
fi

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
