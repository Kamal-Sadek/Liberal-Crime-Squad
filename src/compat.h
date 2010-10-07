//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//Copyright (c) 2004 by Kevin Sadler                                                    //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////
//This file is part of Liberal Crime Squad.                                             //
//                                                                                      //
//    Liberal Crime Squad is free software; you can redistribute it and/or modify       //
//    it under the terms of the GNU General Public License as published by              //
//    the Free Software Foundation; either version 2 of the License, or                 //
//    (at your option) any later version.                                               //
//                                                                                      //
//    Liberal Crime Squad is distributed in the hope that it will be useful,            //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of                    //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                     //
//    GNU General Public License for more details.                                      //
//                                                                                      //
//    You should have received a copy of the GNU General Public License                 //
//    along with Liberal Crime Squad; if not, write to the Free Software                //
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA         //
//////////////////////////////////////////////////////////////////////////////////////////

/**
* compat.h
*
* Compatibility Functions to support ports to non-Windows Platforms
*
* Liberal Crime Squad
*
* Abstract
* 
* Portability Functions
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
* History
*
* LiteralKa's changes:
* 1) Added Window's mostly nonsensical custom type definitions.
*
* Changes for portability...
* 1) For Visual C++ 6.0 don't use namespace std.
* 2) MINGW32 had a name clash between the UNIX-like time() function and
* the time variable used for WIN32. Changed time to ptime.
*
* Revision 1.3  2004/06/30 22:46:33  sadler
* Moved itoa() from game into compat.cpp
*
*
*/

#ifdef Linux
 // <http://msdn.microsoft.com/en-us/library/aa383751(VS.85).aspx>
 // <http://msdn.microsoft.com/en-us/library/s3f49ktz.aspx>

#if defined(__x86_64__) && !defined(_WIN64)
    #define LCS_WIN64
#endif

typedef signed int INT32;
typedef long long __int64;
typedef signed long long INT64;
typedef unsigned long ULONG;

#ifdef LCS_WIN64
    typedef long long LONG_PTR; 
    typedef unsigned long long ULONG_PTR;
#else
    typedef long LONG_PTR; 
    typedef unsigned long ULONG_PTR;
#endif

#if defined(i386) || defined(i486) || \
        defined(intel) || defined(x86) || defined(i86pc)
    #define LCS_M_IX86
#endif

#ifndef LCS_M_IX86
    typedef long long LONGLONG;
    typedef unsigned long long ULONGLONG;
#else
    typedef double LONGLONG;
    typedef double ULONGLONG;
#endif


typedef unsigned long DWORD;
typedef ULONGLONG DWORDLONG; // ULONGLONG and ULONG_PTR vary depending on
typedef ULONG_PTR DWORD_PTR; //  architecture
typedef unsigned int DWORD32;
typedef unsigned long long DWORD64;

typedef void *PVOID;
typedef PVOID HANDLE;
#endif



#ifndef HAS_SRTICMP
// Portable equivalent of Windows stricmp() function.
// This is strcmp() on lowercase versions of the string.

// strToLower() allocates a string and converts it to lower Case using POSIX
// tolower() function.
// Free returned string after use.

char *strToLower (const char *str);

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
// The function is not fully ported and doesn't support other bases, it's just
//  enough for this program to be ported.
// Ensure buffer is of sufficient size.
char *itoa(int value, char *buffer, int radix);
#endif

#ifndef HAS_SNPRINTF
#if defined(__BORLANDC__) || defined(_MSC_VER)
#define snprintf _snprintf
#endif
#endif 
