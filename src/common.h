/*
    File created by Rich McGrew.

    This file includes useful macros and functions for universal use, not just
    in .cpp files, but in .h files too. Having them in includes.h isn't good
    enough because then they are not accessible to header files that are
    included in includes.h. You can't have 2 header files both include each
    other. Thus this file was designed to be something any file could include
    and make full use of, with general-purpose functions and macros that help
    simplify common coding tasks.

    All code released under GNU GPL.
*/

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

// uncomment this next line if you want to disable SDL (which is for music/sound)
//#define DONT_INCLUDE_SDL

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

#ifdef _WIN32
   #include <windows.h>

#  ifdef KEY_EVENT
#   undef KEY_EVENT
#  endif
#  ifdef MOUSE_MOVED
#   undef MOUSE_MOVED
#  endif
   #define GO_PORTABLE
   #include <io.h> //needed for unlink()
   #include <direct.h>
   #ifndef __STRICT_ANSI__
      #define HAS_STRICMP
   #endif
   //Visual C++ .NET (7) includes the STL with vector, so we
   //will use that, otherwise the HP STL Vector.h will be used.
   #ifdef __MINGW32__
      #include <vector>
      #include <map>
   #else
      #if _MSC_VER > 1200
         #define WIN32_DOTNET
         #include <ciso646> // alternate keywords included in the ISO C++ standard
                            // but not directly supported by Microsoft Visual Studio C++
         #include <vector>
         #include <map>
      #else
         #define WIN32_PRE_DOTNET
         #include "vector.h"
         #include "map.h"
      #endif
   #endif

   //#define PDC_WIDE // uncomment this if and only if you want to use UTF-8 version of PDCurses (from pdc34dllu.zip instead of pdc34dllw.zip)... the "U" version
   #include <curses.h>
   //undo PDCurses macros that break vector class
   #undef erase
   #undef clear

   #define CH_USE_CP437 // this works best with the "W" version of PDCurses on Windows (from pdc34dllw.zip instead of pdc34dllu.zip)... it's the standard way we do it
   //#define CH_USE_ASCII_HACK // this works with either the "W" or "U" version of PDCurses just fine either way, as it only uses standard ASCII characters (ugly though)
   //#define CH_USE_UNICODE // this only works on Windows with the UTF-8 version of PDCurses (the "U" version), and if you have the PDC_WIDE define above uncommented
#else
   #include <vector>
   #include <map>
   #include <ctype.h>
   #include <unistd.h>
   #define GO_PORTABLE

   #if defined(HAVE_WIDE_NCURSES) && defined(__STDC_ISO_10646__)
     #define CH_USE_UNICODE
   #else
     #define CH_USE_ASCII_HACK
   #endif

   #ifdef HAVE_LIBXCURSES
      #define XCURSES
   #endif
   #ifdef HAVE_LIBNCURSES
      #define NCURSES
   #endif
   #ifdef XCURSES
      #define HAVE_PROTO 1
      #define CPLUSPLUS  1
      /* Try these PDCurses/Xcurses options later...
      #define FAST_VIDEO
      #define REGISTERWINDOWS
      */
      #include <xcurses.h> //This is the X11 Port of PDCurses
   //undo PDCurses macros that break vector class
      #undef erase // FIXME: Umm... Now erase() and clear() don't work in
      #undef clear //       dumpcaps.cpp
   #else
      #if defined(USE_NCURSES)
         #include <ncurses.h>
         #define NCURSES
      #elif defined(USE_NCURSES_W)
         #include <ncursesw/ncurses.h>
         #define NCURSES
      #elif defined(NCURSES)
         #define USE_NCURSES
         #include <ncurses.h>
      #else
         #include <curses.h>
      #endif
      // Undo mvaddstr macro and re-implement as function to support overloading
      //#ifdef mvaddstr
      //   #undef mvaddstr
      //   inline int mvaddstr(int y,int x,const char* text) { int ret=move(y,x); if(ret!=ERR) ret=addstr(text); return ret; }
      //#endif
   #endif
#endif

#ifndef CH_USE_CP437
   // addch_unicode() is implemented in common\consolesupport.cpp
   // we need it for ALL non-CP437 code pages though, including UTF-8 and the ASCII hack
   int addch_unicode(int c);

   // Make sure we don't override addch for consolesupport.cpp,
   // because addch_unicode may use addch internally.
   #ifndef CONSOLE_SUPPORT
      #undef addch
      inline int addch(int c) { return addch_unicode(c); }
      #undef mvaddch
      inline int mvaddch(int y,int x,int c) { int ret=move(y,x); if(ret!=ERR) ret=addch_unicode(c); return ret; }
   #endif
#endif

/* Headers for Portability */
#ifdef GO_PORTABLE
   #include <time.h>

   #if defined __linux__ || defined __APPLE__ // BSD and SVr4 too
      /*
      This #undef addstr...It exists only to make the overloaded addstr
         work in linux (because otherwise it clashes with ncurses).
         The normal addstr works fine for me, so I have no idea what's going on.
         I'll just leave this warning here...If addstr breaks or something, it
         might be related to this undefine. At this point in time though, at least
         on my machine, everything's working just fine.

      This still strikes me as being odd, though. :/
      Oh well, I just hope it'll work for everybody.

      Ciprian Ilies, September 26, 2012
      */
      #undef addstr
      #include <sys/time.h>
      #include <signal.h>
   #endif
#endif

#include "cmarkup/Markup.h" //For XML.

#ifdef _WIN32
#ifdef __STRICT_ANSI__ /* mbctype.h doesn't work in strict ansi mode so this hack makes it work */
#define STRICT_ANSI_TEMP_OFF
#undef __STRICT_ANSI__
#endif
#include <mbctype.h>
#ifdef STRICT_ANSI_TEMP_OFF
#define __STRICT_ANSI__
#undef STRICT_ANSI_TEMP_OFF
#endif /* this is also the end of the hack, now the compiler is back to the mode it was in before */
#endif

#ifndef DONT_INCLUDE_SDL
#include "sdl/SDL.h"
#include "sdl/SDL_mixer.h"
#endif // DONT_INCLUDE_SDL

#ifndef WIN32_PRE_DOTNET
using namespace std;
#endif

/*--------------------------------------------------------------------------
 * Portability Functions
 *
 * These functions are intended to replace explicit calls to Windows API.
 *
 * We can do the following:
 *
 * (a) Write alternative calls for the ports, keep Windows calls.
 * (b) Write portable alternatives for use by Windows and ports.
 * (c) Do (a) and (b) and decide what Windows does (API or portable)
 *       based on the value of a MACRO GO_PORTABLE.
 *
 * compat.cpp is the place for non-trivial or more global functions.
 *--------------------------------------------------------------------------*/

/* raw_output() is provided in PDcurses/Xcurses but is not in ncurses.
  * This function is for compatibility and is currently a do nothing function.
  */
#ifdef NCURSES
inline int raw_output(bool bf)
{
   raw();
   return OK;
}
#endif

/*--------------------------------------------------------------------------
 * End of Portability Functions
 *--------------------------------------------------------------------------*/

/* Macro definition */
#ifndef MAX
   // maximum of 2 numbers
   #define MAX(a,b) (((a)<(b))?(b):(a))
#endif

#ifndef MIN
   // minimum of 2 numbers
   #define MIN(a,b) (((a)>(b))?(b):(a))
#endif

#ifndef ABS
   // absolute value of a number
   #define ABS(x) ((x)<0)?(-x):(x)
#endif

#ifndef DIFF
   // difference between 2 numbers (absolute value of subtracting one from the other)
   #define DIFF(x,y) ((x)<(y)?((y)-(x)):((x)-(y)))
#endif

#define BIT1  (1<<0 )
#define BIT2  (1<<1 )
#define BIT3  (1<<2 )
#define BIT4  (1<<3 )
#define BIT5  (1<<4 )
#define BIT6  (1<<5 )
#define BIT7  (1<<6 )
#define BIT8  (1<<7 )
#define BIT9  (1<<8 )
#define BIT10 (1<<9 )
#define BIT11 (1<<10)
#define BIT12 (1<<11)
#define BIT13 (1<<12)
#define BIT14 (1<<13)
#define BIT15 (1<<14)
#define BIT16 (1<<15)
#define BIT17 (1<<16)
#define BIT18 (1<<17)
#define BIT19 (1<<18)
#define BIT20 (1<<19)
#define BIT21 (1<<20)
#define BIT22 (1<<21)
#define BIT23 (1<<22)
#define BIT24 (1<<23)
#define BIT25 (1<<24)
#define BIT26 (1<<25)
#define BIT27 (1<<26)
#define BIT28 (1<<27)
#define BIT29 (1<<28)
#define BIT30 (1<<29)
#define BIT31 (1<<30)
#define BIT32 (1<<31)

#define MAX_PATH_SIZE 2048

#define RNG_SIZE 4

/* These 6 random number generator functions are implemented in compat.cpp */
unsigned long getSeed();
unsigned long r_num();
long LCSrandom(long max);
void initMainRNG();
void copyRNG(unsigned long(&dest)[RNG_SIZE],unsigned long(&src)[RNG_SIZE]);
void initOtherRNG(unsigned long(&rng)[RNG_SIZE]);

/* Determine size of vectors and any other container that implements the size() function.
   This basically includes all types of containers except for the C++11 std::forward_list. */
template <class Container> inline long len(const Container& x)
{
   return x.size();
}

/* Determine array size in pickrandom() and various functions throughout the code.
   Only works on actual arrays, not on vectors or other containers. */
template <typename T,size_t N> inline long len(const T (&x)[N])
{
   return N;
}

/* Override of the prior function for null-terminated C-strings as char arrays.
   This override allows pickrandom() to pick a random character from a C-string we have as a char array. */
template <size_t N> inline long len(const char (&x)[N])
{
   return strlen(x);
}

/* Override of the prior function for null-terminated C-strings as char pointers.
   This override allows pickrandom() to pick a random character from a C-string we have as a char pointer. */
inline long len(const char* x)
{
   return strlen(x);
}

/* Override for when it doesn't work with const in front of the char*
   (compilers are weird about template function overrides) */
inline long len(char* x)
{
   return strlen(x);
}

/* Pick a random element from a vector/deque/map/std::string/C++11 std::array/etc. (e.g. a random string from a vector of strings).
   It works on any container class that implements the [] operator, size() function, and value_type typename.
   This doesn't work if the vector/deque/map/std::string/C++11 std::array/etc. has zero elements. */
template <class Container> inline typename Container::value_type& pickrandom(const Container& x)
{
   return const_cast<typename Container::value_type&>(x[LCSrandom(len(x))]);
}

/* Pick a random element from an array (e.g. a random string from an array of strings).
   This doesn't work if the array has zero elements. */
template <typename T,size_t N> inline T& pickrandom(const T (&x)[N])
{
   return const_cast<T&>(x[LCSrandom(len(x))]);
}

/* Pick a random element from a C-string.
   Returns '\x0' (null character) for empty string. */
inline char& pickrandom(const char* x)
{
   return const_cast<char&>(x[LCSrandom(len(x))]);
}

/* Override for when it doesn't work with const in front of the char*
   (compilers are weird about template function overrides) */
inline char& pickrandom(char* x)
{
   return x[LCSrandom(len(x))];
}

/* Deletes a specified pointer and sets it to NULL. */
template <typename T> inline void delete_and_nullify(T* &o)
{
   delete o;
   o=NULL;
}

/* Deletes and removes a specified pointer from a container. */
template <class Container> inline void delete_and_remove(Container& c,int i)
{
   if(i<=-1) return; // check for invalid index, and don't delete anything in this case
	delete c[i];
   c.erase(c.begin()+i);
}

/* Deletes and removes a specified pointer from 2 containers it's in.
   c1 and c2 must not be the same container! */
template <class Container> inline void delete_and_remove(Container& c1,int i1,Container& c2,int i2)
{
   if(i1<=-1||i2<=-1) return; // check for invalid index, and don't delete anything in this case
   if(c1[i1]==c2[i2])
   { // it's the same pointer, so just delete it once, then remove it from both containers
      delete_and_remove(c1,i1);
      if(c1!=c2||i1!=i2) c2.erase(c2.begin()+i2); // remove it from 2nd location, unless 2nd location is identical to 1st location
   }
   else
   { // it's 2 different pointers, so delete them both and remove them from both containers
      delete_and_remove(c1,i1);
      delete_and_remove(c2,i2);
   }
}

/* Deletes and removes all pointers in a container. */
template <class Container> inline void delete_and_clear(Container& c)
{
   while(len(c))
   {
      delete c.back();
      c.pop_back();
   }
}

/* Deletes and removes all pointers that 2 containers have in common. */
template <class Container> inline void delete_and_clear(Container& c1,Container& c2)
{
   for(int i1=len(c1)-1;i1>=0;i1--)
      for(int i2=len(c2)-1;i2>=0;i2--)
         if(c1[i1]==c2[i2]) delete_and_remove(c1,i1,c2,i2);
}

#endif
