/*

Copyright (c) 2002,2003,2004 by Tarn Adams                                            //
                                                                                      //
This file is part of Liberal Crime Squad.                                             //
                                                                                    //
    Liberal Crime Squad is free software; you can redistribute it and/or modify     //
    it under the terms of the GNU General Public License as published by            //
    the Free Software Foundation; either version 2 of the License, or               //
    (at your option) any later version.                                             //
                                                                                    //
    Liberal Crime Squad is distributed in the hope that it will be useful,          //
    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the                  //
    GNU General Public License for more details.                                    //
                                                                                    //
    You should have received a copy of the GNU General Public License               //
    along with Liberal Crime Squad; if not, write to the Free Software              //
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   02111-1307   USA     //
*/

/*
        This file was created by Chris Johnson (grundee@users.sourceforge.net)
        by copying code from game.cpp.
        To see descriptions of files and functions, see the list at
        the bottom of includes.h in the top src folder.
*/

#define CONSOLE_SUPPORT // define this BEFORE including anything

#include <includeDefault.h>
//#include "configfile.h"
//#include "tinydir.h"
#include <includeEnum.h>
//#include <includeCommon.h>
//#include <includeNews.h>
//#include <includeFunctions.h>
//#include <includeTitle.h>

//#include <includeTalk.h>
#include <includeExternDefault.h>
//#include <includeExternPolitics.h>
//#include <includeExternStat.h>

#if defined(USE_NCURSES)
#include <term.h>
#elif defined(USE_NCURSES_W)
#include <ncursesw/term.h>
#endif

// These 4 variables to keep track of the current color are for
// this file only
short curForeground=COLOR_WHITE,curBackground=COLOR_BLACK;
bool isBright=false,isBlinking=false;

//sets current color to desired setting
void set_color(short f,short b,bool bright,bool blink)
{
   // keep track of current color
   curForeground=f,curBackground=b,isBright=bright,isBlinking=blink;

   //color swap required for PDcurses
   if(f==7&&b==0) f=0,b=0;
   else if(f==0&&b==0) f=7,b=0;

   chtype blinky=(blink?A_BLINK:0), brighty=(bright?A_BOLD:0);

   //pick color pair based on foreground and background
   attrset(brighty | blinky | COLOR_PAIR(f*8+b));
}



//IN CASE FUNKY ARROW KEYS ARE SENT IN, TRANSLATE THEM BACK
void translategetch(int &c)
{
   //if(c==-63)c='7';
   //if(c==-62)c='8';
   //if(c==-61)c='9';
   //if(c==-60)c='4';
   //if(c==-59)c='5';
   //if(c==-58)c='6';
   //if(c==-57)c='1';
   //if(c==-56)c='2';
   //if(c==-55)c='3';

   if(c== -6||c==0x1FA) c='0';
   if(c==-50||c==0x1CE) c='.';
   if(c==-53||c==0x1CB) c=ENTER;
   if(c==-47||c==0x1D1) c='+';
   if(c==-48||c==0x1D0) c='-';
   if(c==-49||c==0x1CF) c='*';
   if(c==-54||c==0x1CA) c='/';

   /*
   if(c==2)c='2';
   if(c==3)c='8';
   if(c==4)c='4';
   if(c==5)c='6';
   */

   // SPECIAL TRANSLATION for AZERTY keyboard
   if(interface_pgup=='.')
   {
      if(c==';')c='.';
      if(c==':')c='/';
      if(c=='&')c='1';
      if(c==0x82)c='2';
      if(c=='\"')c='3';
      if(c=='\'')c='4';
      if(c=='(')c='5';
      if(c=='-')c='6';
      if(c==0x8A)c='7';
      if(c=='_')c='8';
      if(c==0x87)c='9';
      if(c==0x85)c='0';
   }

   if(c>='A'&&c<='Z'){c+='a'-'A';}

   /* Support Cursor Keys...*/
   //if(c==KEY_LEFT)c='a';
   //if(c==KEY_RIGHT)c='d';
   //if(c==KEY_UP)c='w';
   //if(c==KEY_DOWN)c='x';

}



void translategetch_cap(int &c)
{
   //if(c==-63)c='7';
   //if(c==-62)c='8';
   //if(c==-61)c='9';
   //if(c==-60)c='4';
   //if(c==-59)c='5';
   //if(c==-58)c='6';
   //if(c==-57)c='1';
   //if(c==-56)c='2';
   //if(c==-55)c='3';

   if(c== -6||c==0x1FA) c='0';
   if(c==-50||c==0x1CE) c='.';
   if(c==-53||c==0x1CB) c=ENTER;
   if(c==-47||c==0x1D1) c='+';
   if(c==-48||c==0x1D0) c='-';
   if(c==-49||c==0x1CF) c='*';
   if(c==-54||c==0x1CA) c='/';

   /*
   if(c==2)c='2';
   if(c==3)c='8';
   if(c==4)c='4';
   if(c==5)c='6';
   */
}



/* Refreshes the screen, empties the keyboard buffer, waits for a new key to be pressed, and returns the key pressed */
int getkey()
{
   refresh();
   nodelay(stdscr,TRUE);
   while(getch()!=ERR);
   nodelay(stdscr,FALSE);
   int c=getch();
   translategetch(c);
   return c;
}



/* Variant of getkey() that doesn't make all letters lowercase */
int getkey_cap()
{
   refresh();
   nodelay(stdscr,TRUE);
   while(getch()!=ERR);
   nodelay(stdscr,FALSE);
   int c=getch();
   translategetch_cap(c);
   return c;
}



/* Empties the keyboard buffer, and returns most recent key pressed, if any */
int checkkey()
{
   int c=ERR,ret=ERR;
   nodelay(stdscr,TRUE);
   do
   {
      ret=c;
      c=getch();
      translategetch(c);
   } while(c!=ERR);
   nodelay(stdscr,FALSE);
   return ret;
}



/* Variant of checkkey() that doesn't make all letters lowercase */
int checkkey_cap()
{
   int c=ERR,ret=ERR;
   nodelay(stdscr,TRUE);
   do
   {
      ret=c;
      c=getch();
      translategetch_cap(c);
   } while(c!=ERR);
   nodelay(stdscr,FALSE);
   return ret;
}



#ifdef CH_USE_UNICODE
bool unicode_enabled = false;

bool setup_unicode() {
   #ifdef WIN32
   #ifdef PDC_WIDE
   unicode_enabled = true; // We're using a version of PDCurses with UTF-8 support (e.g. from pdc34dllu.zip)
   #else
   unicode_enabled = false; // We're using a version of PDCurses without UTF-8 support (e.g. from pdc34dllw.zip)
   #endif
   #else
   // Is it a UTF-8 locale?
   unicode_enabled = !strcmp(nl_langinfo(CODESET), "UTF-8");
   #endif
   return unicode_enabled;
}

int lookup_unicode_hack(int c) {
   for(int i=0;i<len(unicode_hacks);i++)
      if(unicode_hacks[i].unicode_char==c)
         return unicode_hacks[i].hack_char;
   return '?';
}
#endif

#ifndef CH_USE_CP437
// This function's for both UTF-8 and the ASCII hack (only disabled in pure CP437 mode)
int addch_unicode(int c) {
#ifdef CH_USE_UNICODE
// This part here is for Unicode only, not the ASCII hack
   wchar_t wch;
   cchar_t cch;

   if (unicode_enabled) {
      // We can do this because we've already verified
      // that __STDC_ISO_10646__ is set.
      wch = c;

      setcchar(&cch, &wch, 0, 0, NULL);
      return add_wch(&cch);
   } else {
      c=lookup_unicode_hack(c);
#endif
// Now this code will run on both Unicode AND the ASCII hack
      if(c&A_REVERSE)
      {  // we need to reverse the colors
         c&=~A_REVERSE; // unset A_REVERSE for the character, curses does it wrong
         set_color(curBackground,curForeground,isBlinking,isBright); // reverse colors
         int ret=addch(c); // add the character
         set_color(curBackground,curForeground,isBlinking,isBright); // reverse them back again
         return ret; // done
      } // don't need to reverse colors, just add the character
      else return addch(c);
#ifdef CH_USE_UNICODE
   }
#endif
}
#endif

void set_title(char *s)
{
#ifdef NCURSES
   if(tgetflag("hs"))
   { // terminal has status line support
      char buf[255]={0};
      char *p=buf; // tgetstr modifies its second argument, let buf keep pointing to the beginning
      char *ok; // tgetstr's return value is apparently undocumented, except that it's NULL on errors

      ok=tgetstr("tsl",&p); // "to status line"
      if(!ok) return;
      strcpy(p-1,s); // tgetstr leaves us *after* the null, so skip back a bit
      p+=len(s)-1; // same here

      ok=tgetstr("fsl",&p); // "from status line"
      if(!ok) return;

      putp(buf);
   }
#else // assume pdcurses
   PDC_set_title(s);
#endif
}

// Initialize the console, depending on the OS and language/code page settings
void init_console()
{
   #ifdef WIN32
   // This has to be set to Code Page 437 in Windows regardless of Unicode, that's just how PDCurses works on Windows, even the UTF-8 version of PDCurses
   SetConsoleOutputCP(437); // use Code Page 437 (US English code page for DOS) for output, regardless of anything else
   SetConsoleCP(437); // use Code Page 437 (US English code page for DOS) for input, regardless of anything else
   setlocale(LC_ALL,"English_United States.437");
   _setmbcp(_MB_CP_LOCALE); // use same code page as multibyte code page
   #else // WIN32
   #ifdef CH_USE_UNICODE
   setlocale(LC_ALL,"en_US.UTF-8"); // POSIX-compliant OSes DO support UTF-8/Unicode for setlocale, unlike Windows
   #endif
   #ifdef CH_USE_CP437
   setlocale(LC_ALL,"en_US.CP437");
   #endif
   #ifdef CH_USE_ASCII_HACK
   setlocale(LC_ALL,"en_US.CP437");
   #endif
   #endif // WIN32
   #ifdef CH_USE_UNICODE
   setup_unicode();
   #endif
}

#ifdef WIN32
#define  FE_FONTSMOOTHINGSTANDARD           1
#define  FE_FONTSMOOTHINGCLEARTYPE          2
#define  SPI_GETFONTSMOOTHINGTYPE      0x200A
#define  SPI_SETFONTSMOOTHINGTYPE      0X200B
#define  SPI_GETFONTSMOOTHINGCONTRAST  0X200C
#define  SPI_SETFONTSMOOTHINGCONTRAST  0X200D
BOOL FontSmoothingEnabled;
UINT TypeOfFontSmoothing;

void begin_cleartype_fix() // execute this function after loading settings from init.txt, but before the user is actively playing the game
{
   if(fixcleartype) // only do anything if fixcleartype was set in init.txt and we're running Windows XP or later
	{
      // first we get the font smoothing parameters from Windows so that the old settings are backed up
      SystemParametersInfo(SPI_GETFONTSMOOTHING, 0, &FontSmoothingEnabled, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
      SystemParametersInfo(SPI_GETFONTSMOOTHINGTYPE, 0, &TypeOfFontSmoothing, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
      // now we see if the game crashed or exited prematurely the last time, and load the font smoothing parameters from the file if
      //     that is the case, to ensure that the user's original font smoothing settings will be restored when they exit the game
      FILE *h;
      h=LCSOpenFile("cleartype.dat", "rb", LCSIO_PRE_HOME);
      if(h!=NULL)
      {
         fread(&FontSmoothingEnabled,sizeof(BOOL),1,h);
         fread(&TypeOfFontSmoothing,sizeof(UINT),1,h);
         LCSCloseFile(h);
      }
      // now that we know for sure what the original settings were, and both the variables FontSmoothingEnabled and
      // TypeOfFontSmoothing are guaranteed to be the original settings prior to any modifications by this game, we can
      // back the original settings up to disk, in case the game crashes or is exited prematurely
      h=LCSOpenFile("cleartype.dat", "wb", LCSIO_PRE_HOME);
      if(h!=NULL)
      {
         fwrite(&FontSmoothingEnabled,sizeof(BOOL),1,h);
         fwrite(&TypeOfFontSmoothing,sizeof(UINT),1,h);
         LCSCloseFile(h);
         // now that everything is safely backed up, we set the font smoothing parameters to be optimal for Liberal Crime Squad
         // ClearType messes things up and causes visual artifacts in Win32 Console Apps like this game so it's going to be off
         // but we'll have standard font smoothing on because it looks really good with Lucida Console, and doesn't affect raster fonts at all
         SystemParametersInfo(SPI_SETFONTSMOOTHING, TRUE, 0, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
         SystemParametersInfo(SPI_SETFONTSMOOTHINGTYPE, 0, (void*)FE_FONTSMOOTHINGSTANDARD, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
      }
	}
}

void end_cleartype_fix() // execute this function after the user is done playing the game, but before the program closes
{
   if(fixcleartype) // only do anything if fixcleartype was set in init.txt and we're running Windows XP or later
   {
      // restore the original settings the user had for font smoothing
      SystemParametersInfo(SPI_SETFONTSMOOTHINGTYPE, 0, (void*)TypeOfFontSmoothing, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
      SystemParametersInfo(SPI_SETFONTSMOOTHING, FontSmoothingEnabled, 0, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
      // now that the settings are safely restored, it's time to delete the temporary backup of the font smoothing settings
      LCSDeleteFile("cleartype.dat",LCSIO_PRE_HOME);
   }
}
#endif
