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


 #ifndef HAS_SRTICMP
 // Portable equivalent of Windows stricmp() function.
 // This is strcmp() on lowercase versions of the
 //string.
 
 //strToLower() allocates a string and converts it to
 //Lower Case using POSIX tolower() function.
 //Free returned string after use.
 
 char *strToLower(const char *str);
 
 int stricmp(const char *str1, const char *str2);
 #endif

  #ifdef Linux // BSD and SVr4 too

  extern int init_alarm;
  extern struct itimerval timer_off;

void alarmHandler(int signal);

void setTimeval(struct  timeval *value, long sec, long usec);
void msToItimerval(int ms, struct  itimerval *value);
#endif 
  
void pause_ms(int t);
void alarmset(int t);
void alarmwait();

 #ifndef HAS_ITOA
 // Portable equivalent of Windows itoa() function.
 // Note the radix parameter is expected to be 10.
 // The function is not fully ported and doesn't support
 //other bases, it's just enough for this program to be
 //ported.
 // Ensure buffer is of sufficient size.
 char *itoa(int value, char *buffer, int radix);
 #endif

