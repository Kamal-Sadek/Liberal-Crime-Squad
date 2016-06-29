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

#if defined __linux__ || defined __APPLE__ // BSD and SVr4 too

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

// FNV-1a 32-bit hash function (fast and effective) -- helper function for getSeed()
void fnvHash(unsigned long &fnv_hash,unsigned long num)
{
   fnv_hash*=16777619UL; // multiply by the FNV-1a hash's 32-bit prime
   fnv_hash&=0xffffffffUL; // keep the number 32-bit (we could be on a 64-bit system)
   fnv_hash^=(num&0xffUL); // xor together with number's 1st byte
   num>>=8; // right shift number to 2nd byte
   fnv_hash*=16777619UL; // multiply by the FNV-1a hash's 32-bit prime
   fnv_hash&=0xffffffffUL; // keep the number 32-bit (we could be on a 64-bit system)
   fnv_hash^=(num&0xffUL); // xor together with number's 2nd byte
   num>>=8; // right shift number to 3rd byte
   fnv_hash*=16777619UL; // multiply by the FNV-1a hash's 32-bit prime
   fnv_hash&=0xffffffffUL; // keep the number 32-bit (we could be on a 64-bit system)
   fnv_hash^=(num&0xffUL); // xor together with number's 3rd byte
   num>>=8; // right shift number to 4th byte
   fnv_hash*=16777619UL; // multiply by the FNV-1a hash's 32-bit prime
   fnv_hash&=0xffffffffUL; // keep the number 32-bit (we could be on a 64-bit system)
   fnv_hash^=(num&0xffUL); // xor together with number's 4th byte
}

// A high-quality RNG seed with multiple sources of entropy hashed together with FNV-1a hash
unsigned long getSeed()
{
   unsigned long _seed=2166136261UL; // start out at the FNV-1a hash's 32-bit offset basis
#ifdef GO_PORTABLE // we have access to time.h's functions as entropy sources
   fnvHash(_seed,time(NULL)); /* Seconds since 1970-01-01 00:00:00 */
   fnvHash(_seed,clock()); /* clock ticks since LCS was launched (clock ticks are some fraction of a second that varies on different implementations) */
#endif // GO_PORTABLE
#ifdef WIN32 // We're on Windows and can use the Win32 API as entropy sources
   fnvHash(_seed,GetTickCount()); /* ms since system boot */
   fnvHash(_seed,GetCurrentProcessId()); /* process ID for current process */
   SYSTEM_INFO info; /* a whole bunch of system info */
   GetSystemInfo(&info); /* get the system info */
   fnvHash(_seed,(unsigned long)info.lpMinimumApplicationAddress); /* pointer to minimum accessible memory location */
   fnvHash(_seed,(unsigned long)info.lpMaximumApplicationAddress); /* pointer to maximum accessible memory location */
#else // we're on a POSIX system and can use POSIX API entropy sources
#if defined(_SC_AVPHYS_PAGES) && defined(_SC_PAGESIZE) // might or might not be defined... optional in POSIX
   fnvHash(_seed,sysconf(_SC_AVPHYS_PAGES)*sysconf(_SC_PAGESIZE)); /* current available memory */
#endif // defined(_SC_AVPHYS_PAGES) && defined(_SC_PAGESIZE)
#ifdef _SC_NPROCESSORS_ONLN // might or might not be defined... optional in POSIX
   fnvHash(_seed,sysconf(_SC_NPROCESSORS_ONLN));
#endif // _SC_NPROCESSORS_ONLN
#endif // WIN32
   return _seed;
}

// Return a random number from 1 to 0xffffffff (any 32-bit integer except 0), using all of seed[].
// This is a 32-bit version of George Marsaglia's xorshift pseudorandom number generator with 128-bit state space.
// 32-bit xorshift RNGs produce values in the range from 1 to 0xffffffff (any 32-bit integer except 0),
// and don't allow the values to be zero... if they do return zero they've been improperly initialized,
// and would continue to output nothing but zeros forever unless the problem is rectified.
// So there's a recovery mechanism in place in case it's about to return a zero value, to fix that.
// It's a nice ultrafast 32-bit pseudorandom number generator with good randomness properties, small state space, and very short code.
unsigned long r_num()
{
   while(true) // this loop is in case of error, so the recovery mechanism can work... it'll exit with a return value upon success
   {
      unsigned long t=seed[0]^((seed[0]<<11)&0xffffffff); // set temp variable and keep it within 32 bits
#ifdef MORERANDOM
      t^=getSeed(); // add entropy
#endif // MORERANDOM
      seed[0]=seed[1]; seed[1]=seed[2]; seed[2]=seed[3]; // shift variables
      seed[3]=seed[3]^(seed[3]>>19)^t^(t>>8); // calculate random number
      if(seed[3]) return seed[3]; // return a number unless it's zero, in which case it was initialized wrong
      initMainRNG(); // recovery mechanism in case things were badly initialized
   }
}

// Picks a random number from 0 to max-1
long LCSrandom(long max)
{  // This expects r_num() to return a random number between 1 and 0xffffffff (any 32-bit integer except 0)... which it does
   return (long)(max*(((long double)(r_num()-1))/((long double)0xffffffffUL)));
}

// Returns a random number from 0 to 0xffffffff (any 32-bit integer), only using seed[0].
// This is a linear congruential generator using parameters suggested in a paper by Pierre L'Ecuyer
// that has better randomness characteristics than most linear congruential generators (the simplest type of RNG).
// No linear congruential generators is very random though, so this is just used to help initialize the xorshift RNG.
unsigned long r_num2()
{
   return seed[0]=(seed[0]*32310901UL+433494437UL)&0xffffffff; // return a random number, kept within 32 bits
}

// Initializes the xorshift Random Number Generator with help getSeed() and r_num2()
void initMainRNG()
{  // we got 4 integers to initialize, which we'll get from a well-seeded linear congruential generator
   seed[0]=getSeed(); // seed the linear congruential generator
   for(int i=RNG_SIZE-1;i>=0;i--) seed[i]=r_num2(); // initialize all the integers
}

// Copies a xorshift Random Number Generator from src to dest
void copyRNG(unsigned long(&dest)[RNG_SIZE],unsigned long(&src)[RNG_SIZE])
{
   for(int i=0;i<RNG_SIZE;i++) dest[i]=src[i]; // copy all the integers
}

// Sets up another xorshift Random Number Generator whose state space is passed as an argument
void initOtherRNG(unsigned long(&rng)[RNG_SIZE])
{
   r_num(); // randomize main xorshift RNG beforehand
   copyRNG(rng,seed); // copy main xorshift RNG to the one we're setting up
   r_num(); // randomize main xorshift RNG afterwards
}
