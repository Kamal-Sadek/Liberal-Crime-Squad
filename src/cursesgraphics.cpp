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
#ifdef WIN32
#define CH_USE_CP437
#else
#define CH_USE_ASCII_HACK
#endif

#define CURSES_GRAPHICS
#include "cursesgraphics.h"

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

