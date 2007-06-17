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

#define PICMAX 1000

struct filelistst
	{
	vector<char *> list;

	~filelistst()
		{
		for(int l=0;l<list.size();l++)
			{
			delete list[l];
			}
		list.clear();
		}

	void clean(void)
		{
		for(int l=0;l<list.size();l++)
			{
			delete list[l];
			}
		list.clear();
		}

	void open_diskload(HANDLE &h);
	void open_disksave(HANDLE &h);
	void smartappend(filelistst &list2);
	};

#define CM_FRAMEFLAG_OVERLAY BIT1

struct CursesMovie_framest
{
	short frame;
	long start,stop;
	short sound;
	short song;
	short effect;
	unsigned short flag;

	CursesMovie_framest()
		{
		frame=0;
		start=0;
		stop=0;
		sound=-1;
		song=-1;
		effect=-1;
		flag=0;
		}
};

struct CursesMoviest
{
	unsigned char picture[PICMAX][80][25][4];
	unsigned long picnum;
	unsigned long dimx,dimy;
	filelistst songlist;
	filelistst soundlist;
	vector<CursesMovie_framest *> frame;

	CursesMoviest()
		{
		picnum=1;dimx=80;dimy=25;
		}
	~CursesMoviest()
		{
		clean();
		}

	void savemovie(char *filename);
	void loadmovie(char *filename);
	void clean(void);
	void convertindices_song(filelistst &master);
	void convertindices_sound(filelistst &master);
	void playmovie(int x,int y);
};
