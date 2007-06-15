######################################################################################
# Liberal Crime Squad Makefile
#
# Kevin Sadler 2004-06-30
#
# This Makefile is used to build LCS.
#
# As well as building with Microsoft Visual C++ (which doesn't use this makefile)
# the code can be built using g++ in the Cygwin environment (www.cygwin.com a 
# Linux-like environment for Windows) and also with the MinGW32 compiler 
# (www.mingw.org Minimalist GNU for Windows). It can probably be built on other UNIX 
# systems with little or no modification.
#
# LCS depends on curses and will either use the ncurses or the PDCurses/Xcurses
# library. Linux will probably have ncurses installed already. You may need to add
# the ncurses package to cygwin using the cygwin setup utility. For MinGW32 you will
# need to download PDCurses source and compile it using it's Makefile for MinGW32 
# (which you will need to edit to specify paths and such, you will probably need to
# rename the pdcurses.a to libpdcurses.a and move it and the curses.h file into the
# MinGW32 lib and include folders).
#
# For any platform 3 variables should be set:
#
# PLATFORM	This specifies the platform, code may get conditionally compiled
#		according to the platform.
#
# PFLAGS	This is added into the CFLAGS, options that are passed to the
#		compiler. It will define PLATFORM as a macro and also the type
#		of curses to use, which may be either:
#
#		-DNCURSES (Use ncurses)
#		-DXCURSES (Use PDCurses/Xcurses)
#		-DCURSES  (Use "normal" curses, perhaps for some UNIX systems)
#
#		There is also a -DUNIX macro that can be defined here which
#		avoids some Windows API specific things.
#
# CURSESLIB	This tells the linker which libraries (primarily the curses one)
#		to link with. It may need to specify a path to the lib directory.
#
# Examples for Linux (and cygwin) and MinGW32 (with MSYS, installed in the
# default locations) are included. Uncomment the lines for the Platform you
# want to build on.
#
# Commented out lines for debug compilation and static linking appear after the
# platform specific stuff.
#
######################################################################################
CC=g++
######################################################################################
# Linux and Cygwin with ncurses
# Cygwin is so "Linux-like" that it will compile with platform set to Linux.
# If you have another UNIX system try this first.
######################################################################################
PLATFORM=Linux
PFLAGS=-D$(PLATFORM) -DUNIX -DNCURSES 
CURSESLIBS=-lncurses

# To use XCURSES (i.e. PDCurses built on UNIX)...
#PFLAGS=-D$(PLATFORM) -DUNIX -DXCURSES -
#XDIR=/usr/X11R6/lib
#CURSESLIBS=-L$(XDIR) -lXt -lXaw -lXCurses

######################################################################################
# MinGW32 Compiler For Windows in MSYS environment
######################################################################################
#PLATFORM=MINGW32
#PFLAGS=-D$(PLATFORM) -DUNIX -DXCURSES
#CURSESLIBS=-L/mingw/lib -lpdcurses


######################################################################################
# End of platform specific settings.
######################################################################################

#CFLAGS=-O3 $(PFLAGS) -Wall -Wno-unused-variable -Wno-sign-compare

#Or for Debug...
CFLAGS=-g -DDEBUG $(PFLAGS) -Wno-unused-variable -Wno-sign-compare

#For redistributable statically linked...
#LDFLAGS=-static

all	: crimesquad dumpcaps

crimesquad	: game.o cursesmovie.o lcsio.o compat.o cursesgraphics.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ game.o cursesmovie.o lcsio.o compat.o cursesgraphics.o\
	$(CURSESLIBS)
	
game.o	: game.cpp lcsio.h compat.h cursesgraphics.h cursesmovie.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ game.cpp
	
cursesmovie.o	: cursesmovie.cpp compat.h lcsio.h cursesmovie.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ cursesmovie.cpp

lcsio.o	: lcsio.cpp lcsio.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ lcsio.cpp
	
compat.o	: compat.cpp compat.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ compat.cpp

cursesgraphics.o	: cursesgraphics.cpp  cursesgraphics.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ cursesgraphics.cpp

################################################
# Test Programs
################################################

dumpcaps	: dumpcaps.o lcsio.o cursesgraphics.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ dumpcaps.o lcsio.o cursesgraphics.o $(CURSESLIBS)

dumpcaps.o	: dumpcaps.cpp lcsio.h cursesgraphics.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ dumpcaps.cpp
	

