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

  #define HAS_ITOA
  #define HAS_STRICMP
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
#include "compat.h"

#define BIT1 1
#define BIT2 2
#define BIT3 4
#define BIT4 8
#define BIT5 16
#define BIT6 32
#define BIT7 64
#define BIT8 128
#define BIT9 256
#define BIT10 512
#define BIT11 1024
#define BIT12 2048
#define BIT13 4096
#define BIT14 8192
#define BIT15 16384
#define BIT16 32768

using namespace std;

#include "cursesmovie.h"
extern CursesMoviest movie;
void set_color(int16 f,int16 b,char bright);
void translategetch(int32 &c);

void filelistst::open_diskload(FILE* h)
{
	int32 dummy;
	int16 dummy2;
	uint32 numbytes;

	clean();

    numbytes=fread(&dummy,sizeof(int32),1,h);
    dummy=swap_endian_32(dummy);
	list.resize(dummy);

	for(int32 l=0;l<list.size();l++)
	{
	    numbytes=fread(&dummy2,sizeof(int16),1,h);
	    dummy2=swap_endian_32(dummy2);
        if(dummy2>0)
		{
		list[l]=new char[dummy2+1];
		list[l][dummy2]='\x0';
		}
        else list[l]=NULL;
	}
}

void filelistst::open_disksave(FILE *h)
{
	int32 dummy;
	int16 dummy2;
	uint32 numbytes;

	dummy=swap_endian_32(list.size());

	numbytes=fwrite(&dummy,sizeof(int32),1,h);

	for(int32 l=0;l<list.size();l++)
	{
	if(list[l]!=NULL)
    {
		dummy2=swap_endian_16(strlen(list[l]));
		numbytes=fwrite(&dummy2,sizeof(int16),1,h);
		if(dummy2>0)
		{
		    numbytes=fwrite(list[l],dummy2,1,h);
		}
        else
		{
            dummy2=swap_endian_16(0);

            numbytes=fwrite(&dummy2,sizeof(int16),1,h);
		}
	}
    }
}

void filelistst::smartappend(filelistst &list2)
{
	char conf;

	for(int32 l2=0;l2<list2.list.size();l2++)
	{
	   if(list2.list[l2]==NULL)continue;

	   conf=1;

	   for(int32 l=0;l<list.size();l++)
	   {
	      if(!stricmp(list2.list[l2],list[l]))
		   {
		      conf=0;
		      break;
		   }
	   }

	   if(conf&&strlen(list2.list[l2])>0)
	   {
	      char *news=new char[strlen(list2.list[l2])+1];
	      strcpy(news,list2.list[l2]);
	      list.push_back(news);
	   }
	}
}

void CursesMoviest::savemovie(char *filename,int32 flags=0)
{
	uint32 numbytes;
	FILE *h;
	h=LCSOpenFile(filename, "wb", flags);

	int32 dummy;

	if(h!=NULL)
	{
        uint32 temp;
        uint16 temp2;

        temp=swap_endian_32(picnum);
        numbytes=fwrite(&temp,sizeof(uint32),1,h);
        temp=swap_endian_32(dimx);
		numbytes=fwrite(&temp,sizeof(uint32),1,h);
		temp=swap_endian_32(dimy);
		numbytes=fwrite(&temp,sizeof(uint32),1,h);
		numbytes=fwrite(picture,sizeof(unsigned char),80*25*4*picnum,h);
		dummy=frame.size();
		temp=swap_endian_32(dummy);
		numbytes=fwrite(&temp,sizeof(int32),1,h);
		for(int32 f=0;f<dummy;f++)
		{
		    temp2=swap_endian_16(frame[f]->frame);
		    numbytes=fwrite(&temp2,sizeof(int16),1,h);
		    temp=swap_endian_32(frame[f]->start);
			numbytes=fwrite(&temp,sizeof(int32),1,h);
            temp=swap_endian_32(frame[f]->stop);
			numbytes=fwrite(&temp,sizeof(int32),1,h);
			temp2=swap_endian_16(frame[f]->sound);
			numbytes=fwrite(&temp2,sizeof(int16),1,h);
			temp2=swap_endian_16(frame[f]->song);
			numbytes=fwrite(&temp2,sizeof(int16),1,h);
			temp2=swap_endian_16(frame[f]->effect);
			numbytes=fwrite(&temp2,sizeof(int16),1,h);
			temp2=swap_endian_16(frame[f]->flag);
			numbytes=fwrite(&temp2,sizeof(int16),1,h);

		}

		songlist.open_disksave(h);
		soundlist.open_disksave(h);

		LCSCloseFile(h);
	}
}

void CursesMoviest::loadmovie(char *filename)
{
	clean();

	uint32 numbytes;
	FILE* h;
	h=LCSOpenFile(filename,"rb", LCSIO_PRE_ART);

	int32 dummy;



	if(h!=NULL)
	{
		fread(&picnum,sizeof(uint32),1,h);
		picnum=swap_endian_32(picnum);
		fread(&dimx,sizeof(uint32),1,h);
		dimx=swap_endian_32(dimx);
		fread(&dimy,sizeof(uint32),1,h);
		dimy=swap_endian_32(dimy);
		fread(picture,sizeof(unsigned char)*80*25*4*picnum,1,h);

		fread(&dummy,sizeof(int32),1,h);
		dummy=swap_endian_32(dummy);
		frame.resize(dummy);
		for(int32 f=0;f<dummy;f++)
		{
			frame[f]=new CursesMovie_framest;
			fread(&frame[f]->frame,sizeof(int16),1,h);
			frame[f]->frame=swap_endian_16(frame[f]->frame);
			fread(&frame[f]->start,sizeof(int32),1,h);
			frame[f]->start=swap_endian_32(frame[f]->start);
			fread(&frame[f]->stop,sizeof(int32),1,h);
			frame[f]->stop=swap_endian_32(frame[f]->stop);
			fread(&frame[f]->sound,sizeof(int16),1,h);
			frame[f]->sound=swap_endian_16(frame[f]->sound);
			fread(&frame[f]->song,sizeof(int16),1,h);
			frame[f]->song=swap_endian_16(frame[f]->song);
			fread(&frame[f]->effect,sizeof(int16),1,h);
			frame[f]->effect=swap_endian_16(frame[f]->effect);
			fread(&frame[f]->flag,sizeof(uint16),1,h);
			frame[f]->flag=swap_endian_16(frame[f]->flag);
		}

		songlist.open_diskload(h);
		soundlist.open_diskload(h);

		LCSCloseFile(h);
	}
}

void CursesMoviest::clean(void)
{
	for(int32 f=0;f<frame.size();f++)
	{
		delete frame[f];
	}
	frame.clear();
}

void CursesMoviest::convertindices_song(filelistst &master)
{
   int32 s2;
	if(songlist.list.size()==0)return;

	vector<int32> convert;
	convert.resize(songlist.list.size());

	for(int32 s=0;s<songlist.list.size();s++)
	{
		for(s2=0;s2<master.list.size();s2++)
		{
			if(!stricmp(master.list[s2],songlist.list[s]))
			{
				convert[s]=s2;
				break;
			}
		}
		if(s2==master.list.size())convert[s]=-1;
	}

	for(int32 f=0;f<frame.size();f++)
	{
		if(frame[f]->song!=-1)frame[f]->song=convert[frame[f]->song];
	}
}

void CursesMoviest::convertindices_sound(filelistst &master)
{
   int32 s2;
	if(soundlist.list.size()==0)return;

	vector<int32> convert;
	convert.resize(soundlist.list.size());

	for(int32 s=0;s<soundlist.list.size();s++)
	{
		for(s2=0;s2<master.list.size();s2++)
		{
			if(!stricmp(master.list[s2],soundlist.list[s]))
			{
				convert[s]=s2;
				break;
			}
		}
		if(s2==master.list.size())convert[s]=-1;
	}

	for(int32 f=0;f<frame.size();f++)
	{
		if(frame[f]->sound!=-1)frame[f]->sound=convert[frame[f]->sound];
	}
}

void CursesMoviest::playmovie(int32 x,int32 y)
{
	nodelay(stdscr,TRUE);

	int32 timer=0;

	char cont,pted;

	int32 finalframe=0;

	do
		{
		//uint32 time=GetTickCount();
		alarmset(10);

		cont=0;
		pted=0;

		//ASSUMES FRAME ORDERED BY STOP TIMER
		for(int32 f=0;f<frame.size();f++)
			{
			if(frame[f]->stop>=finalframe)finalframe=frame[f]->stop;
			if(frame[f]->start<=timer&&frame[f]->stop>=timer)
				{
				//PLAY SOUND
				if(frame[f]->start==timer&&frame[f]->sound!=-1)
					{
					//playsound(frame[f]->sound);
					}
				//PLAY SONG
				if(frame[f]->start==timer&&frame[f]->song!=-1)
					{
					//startbackgroundmusic(frame[f]->song);
					}
				//DRAW FRAME
				if(frame[f]->frame!=-1)
					{
					for(int32 fx=0;fx<movie.dimx&&fx+x<80;fx++)
						{
						for(int32 fy=0;fy<movie.dimy&&fy+y<25;fy++)
							{
							if((movie.picture[frame[f]->frame][fx][fy][0]==' '||
								movie.picture[frame[f]->frame][fx][fy][0]==0)&&
								frame[f]->flag & CM_FRAMEFLAG_OVERLAY)continue;

							move(fy+y,fx+x);
							set_color(movie.picture[frame[f]->frame][fx][fy][1],
								movie.picture[frame[f]->frame][fx][fy][2],
								movie.picture[frame[f]->frame][fx][fy][3]);
							addch(movie.picture[frame[f]->frame][fx][fy][0]);
							}
						}
					pted=1;
					}
				}
			if(frame[f]->stop>=timer)cont=1;
			}

		if(pted)refresh();

		timer++;
		//while(time+10>GetTickCount);
		alarmwait();

		int32 c=getch();
		translategetch(c);

		if(c==10||c==32||c==27)timer=finalframe;

		}while(cont);
}
