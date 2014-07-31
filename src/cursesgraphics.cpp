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
* cursesgraphics.cpp
*
* Portability Functions for CP437 Code Page Graphics Characters
*
* $Author: sadler $
* $Date: 2004/08/11 18:28:38 $
* $Name: HEAD $ $Revision: 1.4 $
*
* $Id: cursesgraphics.cpp,v 1.4 2004/08/11 18:28:38 sadler Exp $
*
* Liberal Crime Squad
*
* Abstract
*
* Translate character codes, stored in cp437 within files in the arts directory
* into native alternative characters.
*
* History
*
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

#define CURSES_GRAPHICS // define this BEFORE including anything
#include <externs.h>

#ifndef CH_USE_CP437
/* Translate CP437 extended characters into the code page used by the console */
int translateGraphicsChar(int c)
{
    // This will be no good for Unicode...
    int cin = c;
    int cout = cin;

//    if ((cin >= 0) && (cin <32))
//    {
//        cout = (cin | A_ALTCHARSET);
//    }
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
#endif // CH_USE_CP437

#ifdef NCURSES
/* Translates PDCurses' numerical color values to NCurses' corresponding
   numerical color values.
   PDCurses can be made to use the same values as NCurses by defining PDC_RGB,
   but the color values in the graphics and movie files would also have to be
   changed to make this function obsolete. */
short translateGraphicsColor(short c)
{
    switch (c)
    {
        case 0: /*c = 0;*/ break; // Black
        case 1:   c = 4;   break; // Blue
        case 2: /*c = 2;*/ break; // Green
        case 3:   c = 6;   break; // Cyan
        case 4:   c = 1;   break; // Red
        case 5: /*c = 5;*/ break; // Magenta
        case 6:   c = 3;   break; // Yellow
        case 7: /*c = 7;*/ break; // White
    }
    return c;
}
#endif // NCURSES
