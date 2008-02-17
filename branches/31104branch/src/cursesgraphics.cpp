////////////////////////////////////////////////////////////////////////////////
//																				
//Copyright (c) 2004 by Kevin Sadler											
//																				
////////////////////////////////////////////////////////////////////////////////
//This file is part of Liberal Crime Squad.										
//																				
//    Liberal Crime Squad is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by		
//    the Free Software Foundation; either version 2 of the License, or			
//    (at your option) any later version.										
//																				
//    Liberal Crime Squad is distributed in the hope that it will be useful,	
//    but WITHOUT ANY WARRANTY; without even the implied warranty of			
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				
//    GNU General Public License for more details.								
//																				
//    You should have received a copy of the GNU General Public License			
//    along with Liberal Crime Squad; if not, write to the Free Software		
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA	
////////////////////////////////////////////////////////////////////////////////

/**
* \file		cursesgraphics.cpp
*
* \brief	Portability Functions for CP437 Code Page Graphics Characters
*
* $Author: sadler $
* $Date: 2004/08/11 18:28:38 $
* \version	$Name: HEAD $ $Revision: 1.4 $
*
* $Id: cursesgraphics.cpp,v 1.4 2004/08/11 18:28:38 sadler Exp $
*
* <HR>
* \b Liberal Crime Squad
*
* 
* <HR>
*
* \par Abstract
*
* Translate character codes, stored in cp437 within files in the arts directory
* into native alternative characters. 
*
* <HR>
*
* \par History
*
* \par
* $Log: cursesgraphics.cpp,v $
* Revision 1.4  2004/08/11 18:28:38  sadler
* New translateGraphicsChar() routine.
* Replace NULL with an inverse video space.
*
* Revision 1.3  2004/08/08 07:49:55  sadler
* *** empty log message ***
*
* Revision 1.2  2004/07/04 11:03:03  sadler
* no message
*
* Revision 1.1  2004/07/03 07:57:49  sadler
* Curses Graphics Tests
*
*
*
*/

#define CURSES_GRAPHICS
#include <includes.h>

#ifdef CH_USE_CP437
int translateGraphicsChar(int c)
{
	return c;
}
#else
int translateGraphicsChar(int c)
{
	// This will be no good for Unicode...
	
	int cin = c;
	int cout = cin;
	
//	if ((cin >= 0) && (cin <32))
//	{
//		cout = (cin | A_ALTCHARSET);
//	} 
	if ((cin >= 0) && (cin < 256))
	{
		cout = gchar[cin].native_code;
		if (cout == 0)
		{
			cout = cin;
		}
	}
	return cout;
}
#endif

#ifdef CH_USE_UNICODE
char unicode_mode = 0;

char setup_unicode() {
   char *codeset;

   // Get LC_CTYPE from environment.
   setlocale(LC_CTYPE, "");

   // Is it a UTF-8 locale?
   codeset = nl_langinfo(CODESET);
   unicode_mode = !strcmp(codeset, "UTF-8");

   return unicode_mode;
}

int lookup_unicode_hack(int c) {
   if (c < 128)
      return c;

   for (int i = 0; ; i++) {
      int unicode = unicode_hacks[i].unicode_char;

      if (unicode == c || unicode == UNICODE_HACKS_END)
         return unicode_hacks[i].hack_char;
   }
}

int addch_unicode(int c) {
   wchar_t wch;
   cchar_t cch;

   if (unicode_mode) {
      // We can do this because we've already verified
      // that __STDC_ISO_10646__ is set.
      wch = c;
   } else {
      wch = lookup_unicode_hack(c);
   }

   setcchar(&cch, &wch, 0, 0, NULL);
   return add_wch(&cch);
}
#endif
