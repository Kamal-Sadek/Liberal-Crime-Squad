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

#include <includes.h>
#include <externs.h>



//sets current color to desired setting
void set_color(short f,short b,char bright)
{
   //color swap required for PDcurses
   if(f==7&&b==0)
   {
      f=0;b=0;
   }
   else if(f==0&&b==0)
   {
      f=7;b=0;
   }

   //pick color pair based on foreground and background
   if(bright)attrset(A_BOLD | COLOR_PAIR(f*8+b));
   else
   {
      attrset(COLOR_PAIR(f*8+b));
   }
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

   if(c==-6)c='0';
   if(c==-50)c='.';
   if(c==-53)c=10;
   if(c==-47)c='+';
   if(c==-48)c='-';
   if(c==-49)c='*';
   if(c==-54)c='/';

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

   if(c>='A'&&c<='Z'){c-='A';c+='a';}

   /* Support Cursor Keys...*/
   if(c==KEY_LEFT)c='a';
   if(c==KEY_RIGHT)c='d';
   if(c==KEY_UP)c='w';
   if(c==KEY_DOWN)c='x';

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

   if(c==-6)c='0';
   if(c==-50)c='.';
   if(c==-53)c=10;
   if(c==-47)c='+';
   if(c==-48)c='-';
   if(c==-49)c='*';
   if(c==-54)c='/';

   /*
   if(c==2)c='2';
   if(c==3)c='8';
   if(c==4)c='4';
   if(c==5)c='6';
   */
}

