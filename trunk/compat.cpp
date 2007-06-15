//////////////////////////////////////////////////////////////////////////////////////////
//																						//
//Copyright (c) 2004 by Kevin Sadler											//
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
* \file		compat.cpp
*
* \brief	Compatibility Functions to support ports to non-Windows Platforms
*
* $Author: sadler $
* $Date: 2004/07/01 20:09:11 $
* \version	$Name: HEAD $ $Revision: 1.4 $
*
* $Id: compat.cpp,v 1.4 2004/07/01 20:09:11 sadler Exp $
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
* <HR>
*
* \par History
*
* \par
* $Log: compat.cpp,v $
* Revision 1.4  2004/07/01 20:09:11  sadler
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

/* Headers for Portability */
  #include <string.h>
  #include <stdlib.h>

#ifdef WIN32
  #include <windows.h>
  #define HAS_STRICMP
  #define HAS_ITOA
 #ifdef __MINGW32__
    #include <iostream>
 #else
    #if _MSC_VER > 1200
      #define WIN32_DOTNET
      #include <iostream>
    #else
      #define WIN32_PRE_DOTNET
      #include <iostream.h>
    #endif
 #endif
#else
 #include <iostream>
 #ifdef Linux // And BSD and SVr4
    #include <unistd.h>
    #include <sys/time.h>  
    #include <signal.h>
    #include <ctype.h>
  #endif 
#endif

#ifndef WIN32_PRE_DOTNET
using namespace std;
#endif

 #ifndef HAS_STRICMP
 // Portable equivalent of Windows stricmp() function.
 // This is strcmp() on lowercase versions of the
 //string.
 
 //strToLower() allocates a string and converts it to
 //Lower Case using POSIX tolower() function.
 //Free returned string after use.
 
 char *strToLower(const char *str)
 {
 int len = strlen(str);
 char *lstr = NULL;
 int i = 0;
 
 lstr = (char *)malloc((len+1)*sizeof(char));
 
 for (i=0; i< len; i++)
 {
 lstr[i] = tolower(str[i]);
 }
 return(lstr);
 }
 
 int stricmp(const char *str1, const char *str2)
 {
 char *lstr1 = NULL;
 char *lstr2 = NULL;
 int result = 0;
  
 lstr1=strToLower(str1);
 lstr2=strToLower(str2);
   
 result = strcmp(lstr1, lstr2);
 
 free(lstr1);
 free(lstr2);
 
 return(result);
 }
 #endif
 
 
 #ifdef Linux // BSD and SVr4 too
 
  int init_alarm = 0; // Flag to indicate if alarmHandler() has been registered.
  struct itimerval timer_off;
  struct itimerval timer_on;


void alarmHandler(int signal)
{
 //WAKE UP and turn the timer off, this will un-pause().
  setitimer(ITIMER_REAL, &timer_off, NULL); 
}  

void setTimeval(struct  timeval *value, long sec, long usec)
{
  value->tv_sec = sec;
  value->tv_usec = usec;
}

void msToItimerval(int ms, struct  itimerval *value)
{
long sec=0;
long usec=0;

  if (ms > 999)
    {
     sec = (long)(ms/1000);
     usec = (long)((ms%1000)*1000);
    }
    else
    {
      usec = (long)(ms*1000);    
    }
    
 setTimeval(&value->it_interval, sec, usec);
 setTimeval(&value->it_value, sec, usec);    
}

void initalarm()
    {
     signal(SIGALRM, &alarmHandler);
     init_alarm=1;
     /* Initialise a zero value itimerval that will turn the timer off.*/
     setTimeval(&timer_off.it_interval, 0, 0);
     setTimeval(&timer_off.it_value, 0, 0);
    }
#endif 

#ifdef WIN32
  unsigned long ptime=GetTickCount();
#endif

void alarmset(int t)
{
#ifdef WIN32
  ptime=GetTickCount() + t;
#else
  /* If the signal handler is not set up set it up now */
  if (init_alarm==0)
    {
     initalarm();
    }
  // setitimer() will start a timer, pause() will stop the process until a
  // SGIALRM from the timer is recieved. This will be caught be alarmHandler()
  // which will turn off the timer and the process will resume.  
  msToItimerval(t, &timer_on);
  setitimer(ITIMER_REAL, &timer_on, NULL); 
#endif
}

void alarmwait()
{
#ifdef WIN32
 while(ptime > GetTickCount());
#else
  struct itimerval timer_now;
  getitimer(ITIMER_REAL, &timer_now);
  //If the timer is on we will wait for it to complete...
  if (
  ((timer_now.it_interval.tv_sec!=0)&&
   (timer_now.it_interval.tv_usec!=0))&&
  ((timer_now.it_value.tv_sec!=0)&&
   (timer_now.it_value.tv_usec!=0)))
  {
   pause();
  }
#endif
}
  
void pause_ms(int t)
{
  #ifdef Linux // BSD and SVr4 too

  alarmset(t);
  
  pause();
  
 #else
   #ifdef WIN32
  ptime=GetTickCount() + t;
 
 // Sadler - In 3.05 this while() was also checking that time <= GetTickCount()
 //          but as that should always be true it is removed.
 while(ptime > GetTickCount());
   #endif
 
 #endif
  
}

#ifndef HAS_ITOA
 // Portable equivalent of Windows itoa() function.
 // Note the radix parameter is expected to be 10.
 // The function is not fully ported and doesn't support
 //other bases, it's just enough for this program to be
 //ported.
 // Ensure buffer is of sufficient size.
 char *itoa(int value, char *buffer, int radix)
 {
 if (radix != 10)
   {
    // Error - base other than 10 not supported.
    cerr << "Error: itoa() - Ported function does not support bases other than 10." << endl;
    exit(1); 
   }
   else if (buffer != NULL)
   {
   sprintf(buffer, "%d", value);
   }
   return buffer;
 }
 #endif
 
