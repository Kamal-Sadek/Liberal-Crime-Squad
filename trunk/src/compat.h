//////////////////////////////////////////////////////////////////////////////////////////
//																						//
//Copyright (c) 2004 by Kevin Sadler								                    //
//																						//
//////////////////////////////////////////////////////////////////////////////////////////
//This file is part of Liberal Crime Squad.												//
//																						//
//    Liberal Crime Squad is free software; you can redistribute it and/or modify		//
//    it under the terms of the GNU General Public License as published by				//
//    the Free Software Foundation; either version 2 of the License, or					//
//    (at your option) any later version.												//
//																						//
//    Liberal Crime Squad is distributed in the hope that it will be useful,			//
//    but WITHOUT ANY WARRANTY; without even the implied warranty of					//
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						//
//    GNU General Public License for more details.										//
//																						//
//    You should have received a copy of the GNU General Public License					//
//    along with Liberal Crime Squad; if not, write to the Free Software				//
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA			//
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \file		compat.h
*
* \brief	Compatibility Functions to support ports to non-Windows Platforms
*
* $Author: sadler $
* $Date: 2004/06/30 22:46:33 $
* \version	$Name: HEAD $ $Revision: 1.3 $
*
* $Id: compat.h,v 1.3 2004/06/30 22:46:33 sadler Exp $
*
* <HR>
* \b Liberal Crime Squad
*
*
* <HR>
*
* \par Abstract
*
* \par Portability Functions
*
* These functions are intended to replace explicit calls to Windows API.
*
* We can do the following:
*
* (a) Write alternative calls for the ports, keep Windows calls.
* (b) Write portable alternatives for use by Windows and ports.
* (c) Do (a) and (b) and decide what Windows does (API or portable)
*     based on the value of a MACRO GO_PORTABLE.
*
* compat.cpp is the place for non-trivial or more global functions,
*
*
* <HR>
*
* \par History
*
* \par
* $Log: compat.h,v $
* Revision 1.3  2004/06/30 22:46:33  sadler
* Moved itoa() from game into compat.cpp
*
*
*/

#ifndef COMPAT_H
#define COMPAT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef PACKAGE_NAME

#if SIZEOF_LONG==4
    typedef unsigned long uint32;
    typedef long int32;
#endif

#if SIZEOF_INT<SIZEOF_LONG

    #if SIZEOF_INT==4
        typedef unsigned int uint32;
        typedef int int32;
    #endif

    #if SIZEOF_INT==2
        typedef unsigned int uint16;
        typedef int int16;
    #endif

#endif

#if SIZEOF_SHORT<SIZEOF_INT
    #if SIZEOF_SHORT==2
        typedef unsigned short uint16;
        typedef short int16;
    #endif

    #if SIZEOF_SHORT==1
        typedef unsigned short uint8;
        typedef short int8;
    #endif
#endif

#if SIZEOF_CHAR<SIZEOF_SHORT
    #if SIZEOF_CHAR==1
    typedef unsigned char uint8;
    typedef char int8;
    #endif
#endif

#else

//Assume we are on Windows
typedef uint32 uint32;
typedef int32 int32;
typedef uint16 uint16;
typedef int16 int16;
typedef unsigned char uint8;
typedef char int8;

#endif



 #ifndef HAS_SRTICMP
 // Portable equivalent of Windows stricmp() function.
 // This is strcmp() on lowercase versions of the
 //string.

 //strToLower() allocates a string and converts it to
 //Lower Case using POSIX tolower() function.
 //Free returned string after use.

 char *strToLower (const char *str);

 int32 stricmp(const char *str1, const char *str2);
 #endif

  #ifdef Linux // BSD and SVr4 too

  extern int32 init_alarm;
  extern struct itimerval timer_off;

void alarmHandler(int32 signal);

void setTimeval(struct  timeval *value, int32 sec, int32 usec);
void msToItimerval(int32 ms, struct  itimerval *value);
#endif

void pause_ms(int32 t);
void alarmset(int32 t);
void alarmwait();

 #ifndef HAS_ITOA
 // Portable equivalent of Windows itoa() function.
 // Note the radix parameter is expected to be 10.
 // The function is not fully ported and doesn't support
 //other bases, it's just enough for this program to be
 //ported.
 // Ensure buffer is of sufficient size.
 char *itoa(int32 value, char *buffer, int32 radix);
 #endif





#endif //guard
