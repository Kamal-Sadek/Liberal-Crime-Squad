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
* compat.cpp
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
* itoa() removed 2014/07/01 yetisyny
* - it's unnecessary, use tostring() or toCstring() instead
* - for exact same functionality as itoa(value,str,10) use strcpy(str,value)
* - another alternative is sprintf() variants or addstr_f() or mvaddstr_f() variants with "%d"
* - many functions like addstr(), mvaddstr(), strcpy(), strcat(), etc. have been overloaded to accept integers directly
*/

#include <externs.h>

#ifndef HAS_STRICMP
// Portable equivalent of Windows stricmp() function.
// This is strcmp() on lowercase versions of the string.
int stricmp(const char *str1,const char *str2)
{
   std::string lstr1=str1,lstr2=str2;
   std::transform(lstr1.begin(), lstr1.end(), lstr1.begin(), ::tolower);
   std::transform(lstr2.begin(), lstr2.end(), lstr2.begin(), ::tolower);
   return lstr1.compare(lstr2);
}
#endif

#ifdef Linux // BSD and SVr4 too

int init_alarm=0; // Flag to indicate if alarmHandler() has been registered.
struct itimerval timer_off,timer_on;

void alarmHandler(int signal)
{
   //WAKE UP and turn the timer off, this will un-pause().
   setitimer(ITIMER_REAL,&timer_off,NULL);
}

void setTimeval(struct timeval *value,long sec,long usec)
{
   value->tv_sec=sec,value->tv_usec=usec;
}

void msToItimerval(int ms,struct itimerval *value)
{
   long sec=0,usec=0;
   if(ms>999)sec=(long)(ms/1000),usec=(long)((ms%1000)*1000);
   else usec=(long)(ms*1000);
   setTimeval(&value->it_interval,sec,usec);
   setTimeval(&value->it_value,sec,usec);
}

void initalarm()
{
   signal(SIGALRM, &alarmHandler);
   init_alarm=1;
   /* Initialise a zero value itimerval that will turn the timer off.*/
   setTimeval(&timer_off.it_interval,0,0);
   setTimeval(&timer_off.it_value,0,0);
}
#endif

#ifdef WIN32
int ptime=GetTickCount();
#endif

void alarmset(int t)
{
   #ifdef WIN32
   ptime=GetTickCount()+t;
   #else
   /* If the signal handler is not set up set it up now */
   if(!init_alarm) initalarm();
   // setitimer() will start a timer, pause() will stop the process until a
   // SGIALRM from the timer is recieved. This will be caught be alarmHandler()
   // which will turn off the timer and the process will resume.
   msToItimerval(t,&timer_on);
   setitimer(ITIMER_REAL,&timer_on,NULL);
   #endif
}

void alarmwait()
{
   #ifdef WIN32
   while(ptime>(int)GetTickCount());
   #else
   struct itimerval timer_now;
   getitimer(ITIMER_REAL,&timer_now);
   //If the timer is on we will wait for it to complete...
   //Make sure the usecs are at least enough that execution is sure
   //to reach the pause before the timer expires or we will wait forever
   //VM on netbook needed this to prevent elections from occasionally locking up.
   if(timer_now.it_interval.tv_sec||timer_now.it_interval.tv_usec>100||timer_now.it_value.tv_sec||timer_now.it_value.tv_usec>100) pause();
   #endif
}

void pause_ms(int t)
{
   alarmset(t);
   refresh();
   alarmwait();
}
