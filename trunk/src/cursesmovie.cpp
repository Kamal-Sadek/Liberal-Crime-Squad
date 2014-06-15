//////////////////////////////////////////////////////////////////////////////////////////
//																						//
//Copyright (c) 2002,2003,2004 by Tarn Adams											//
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

#include "common.h" /* include this prior to checking if WIN32 is defined */

#ifdef WIN32
  #include <windows.h>
  #include <string.h>
  #include <iostream>
  #include <fstream>
  #ifdef __MINGW32__
    #include <vector>
    #include <io.h> //needed for unlink()
  #else
  //Visual C++ .NET (7) includes the STL with vector, so we
  //will use that, otherwise the HP STL Vector.h will be used.
    #if _MSC_VER > 1200
      #define WIN32_DOTNET
      #include <vector>
    #else
      #define WIN32_PRE_DOTNET
      #include "vector.h"
    #endif
  #endif
  #include "curses.h"
  //undo PDCurses macros that break vector class
  #undef erase
  #undef clear
  #ifndef __STRICT_ANSI__
    #define HAS_STRICMP
    #define HAS_ITOA
  #endif
#else
  #include <vector>
  #include <string.h>
  #include <iostream>
  #include <fstream>
  #include <ctype.h>
  #define GO_PORTABLE
  #ifdef XCURSES
    #define HAVE_PROTO 1
    #define CPLUSPLUS  1
    /* Try these PDCurses/Xcurses options later...
    #define FAST_VIDEO
    #define REGISTERWINDOWS
    */
    #include <xcurses.h> //This is the X11 Port of PDCurses
    //undo PDCurses macros that break vector class
    #undef erase
    #undef clear
  #else
    #ifdef NCURSES
      #include <ncurses.h>
    #else
      #include <curses.h>
    #endif
  #endif
#endif
#include <string.h>
#include "lcsio.h"
#include "externs.h"

using namespace std;

extern CursesMoviest movie;

void filelistst::open_diskload(FILE* h)
{
   int dummy;
   short dummy2;

   clean();

   fread(&dummy,sizeof(int),1,h);
   list.resize(dummy);

   for(int l=0;l<(int)list.size();l++)
   {
      fread(&dummy2,sizeof(short),1,h);

      if(dummy2>0)
      {
         list[l]=new char[dummy2+1];
         fread(list[l],1,dummy2,h);
         list[l][dummy2]='\x0';
      }
      else list[l]=NULL;
   }
}

void filelistst::open_disksave(FILE *h)
{
   int dummy=list.size();
   short dummy2;

   fwrite(&dummy,sizeof(int),1,h);

   for(int l=0;l<(int)list.size();l++) if(list[l]!=NULL)
   {
      dummy2=strlen(list[l]);
      fwrite(&dummy2,sizeof(short),1,h);
      if(dummy2>0) fwrite(list[l],dummy2,1,h);
      else fwrite(&(dummy2=0),sizeof(short),1,h);
   }
}

void filelistst::smartappend(filelistst &list2)
{
   char conf;

   for(int l2=0;l2<(int)list2.list.size();l2++)
   {
      if(list2.list[l2]==NULL) continue;

      conf=1;

      for(int l=0;l<(int)list.size();l++)
         if(!strcmp(list2.list[l2],list[l]))
         {  conf=0; break; }

      if(conf&&strlen(list2.list[l2])>0)
      {
         char *news=new char[strlen(list2.list[l2])+1];
         strcpy(news,list2.list[l2]);
         list.push_back(news);
      }
   }
}

void CursesMoviest::savemovie(const char *filename,int flags=0)
{
   FILE *h;
   h=LCSOpenFile(filename, "wb", flags);

   long dummy;

   if(h!=NULL)
   {
      fwrite(&picnum,sizeof(int),1,h);
      fwrite(&dimx,sizeof(int),1,h);
      fwrite(&dimy,sizeof(int),1,h);
      fwrite(picture,sizeof(char),80*25*4*picnum,h);
      dummy=frame.size();
      fwrite(&dummy,sizeof(long),1,h);
      for(int f=0;f<dummy;f++)
      {
         fwrite(&frame[f]->frame,sizeof(short),1,h);
         fwrite(&frame[f]->start,sizeof(long),1,h);
         fwrite(&frame[f]->stop,sizeof(long),1,h);
         fwrite(&frame[f]->sound,sizeof(short),1,h);
         fwrite(&frame[f]->song,sizeof(short),1,h);
         fwrite(&frame[f]->effect,sizeof(short),1,h);
         fwrite(&frame[f]->flag,sizeof(short),1,h);
      }

      songlist.open_disksave(h);
      soundlist.open_disksave(h);

      LCSCloseFile(h);
   }
}

void CursesMoviest::loadmovie(const char *filename)
{
   clean();

   FILE* h;
   h=LCSOpenFile(filename,"rb", LCSIO_PRE_ART);

   long dummy;

   if(h!=NULL)
   {
      fread(&picnum,sizeof(int),1,h);
      fread(&dimx,sizeof(int),1,h);
      fread(&dimy,sizeof(int),1,h);
      fread(picture,sizeof(char)*80*25*4*picnum,1,h);
      fread(&dummy,sizeof(long),1,h);
      frame.resize(dummy);
      for(int f=0;f<dummy;f++)
      {
         frame[f]=new CursesMovie_framest;
         fread(&frame[f]->frame,sizeof(short),1,h);
         fread(&frame[f]->start,sizeof(long),1,h);
         fread(&frame[f]->stop,sizeof(long),1,h);
         fread(&frame[f]->sound,sizeof(short),1,h);
         fread(&frame[f]->song,sizeof(short),1,h);
         fread(&frame[f]->effect,sizeof(short),1,h);
         fread(&frame[f]->flag,sizeof(int short),1,h);
      }

      songlist.open_diskload(h);
      soundlist.open_diskload(h);

      LCSCloseFile(h);
   }
}

void CursesMoviest::clean(void)
{
   delete_and_clear(frame);
}

void CursesMoviest::convertindices_song(filelistst &master)
{
   if(songlist.list.size()==0) return;

   vector<int> convert;
   convert.resize(songlist.list.size());

   int s2;
   for(int s=0;s<(int)songlist.list.size();s++)
   {
      for(s2=0;s2<(int)master.list.size();s2++)
         if(!stricmp(master.list[s2],songlist.list[s]))
         {  convert[s]=s2; break; }
      if(s2==(int)master.list.size())convert[s]=-1;
   }

   for(int f=0;f<(int)frame.size();f++)
      if(frame[f]->song!=-1) frame[f]->song=convert[frame[f]->song];
}

void CursesMoviest::convertindices_sound(filelistst &master)
{
   if(soundlist.list.size()==0) return;

   vector<int> convert;
   convert.resize(soundlist.list.size());

   int s2;
   for(int s=0;s<(int)soundlist.list.size();s++)
   {
      for(s2=0;s2<(int)master.list.size();s2++)
         if(!stricmp(master.list[s2],soundlist.list[s]))
         {  convert[s]=s2; break; }
      if(s2==(int)master.list.size()) convert[s]=-1;
   }

   for(int f=0;f<(int)frame.size();f++)
      if(frame[f]->sound!=-1) frame[f]->sound=convert[frame[f]->sound];
}

void CursesMoviest::playmovie(int x,int y)
{
   long timer=0,finalframe=0;
   bool cont,pted;

   do
   {
      //int time=GetTickCount();
      alarmset(10);

      cont=false,pted=false;

      //ASSUMES FRAME ORDERED BY STOP TIMER
      for(int f=0;f<(int)frame.size();f++)
      {
         if(frame[f]->stop>=finalframe)finalframe=frame[f]->stop;
         if(frame[f]->start<=timer&&frame[f]->stop>=timer)
         {
            /* Sound and songs never got implemented!
            //PLAY SOUND
            if(frame[f]->start==timer&&frame[f]->sound!=-1)
               playsound(frame[f]->sound);
            //PLAY SONG
            if(frame[f]->start==timer&&frame[f]->song!=-1)
               startbackgroundmusic(frame[f]->song);
            */
            //DRAW FRAME
            if(frame[f]->frame!=-1)
            {
               for(int fx=0;fx<movie.dimx&&fx+x<80;fx++)
                  for(int fy=0;fy<movie.dimy&&fy+y<25;fy++)
                  {
                     if((movie.picture[frame[f]->frame][fx][fy][0]==' '||
                         movie.picture[frame[f]->frame][fx][fy][0]==0)&&
                         frame[f]->flag & CM_FRAMEFLAG_OVERLAY) continue;

                     move(fy+y,fx+x);
                     set_color(movie.picture[frame[f]->frame][fx][fy][1],
                               movie.picture[frame[f]->frame][fx][fy][2],
                               movie.picture[frame[f]->frame][fx][fy][3]);
                     addch(movie.picture[frame[f]->frame][fx][fy][0]);
                  }
               pted=true;
            }
         }
         if(frame[f]->stop>=timer) cont=true;
      }

      if(pted) refresh();

      timer++;
      //while(time+10>GetTickCount);
      alarmwait();

      int c=checkkey();

      if(c==ENTER||c==ESC||c==SPACEBAR) timer=finalframe;

   } while(cont);
}
