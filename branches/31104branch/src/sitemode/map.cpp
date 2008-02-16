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



/* re-create site from seed before squad arrives */
void initsite(locationst &loc)
{
   int x = 0;
 
   //PREP
   if(activesquad==NULL)return;

   for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;

   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         activesquad->squad[p]->forceinc=0;
      }
   }

   for(int l=0;l<groundloot.size();l++)
   {
      delete groundloot[l];
   }
   groundloot.clear();


   //MAKE MAP
   unsigned long oldseed=seed;
   seed=loc.mapseed;
   
   for(x=0;x<MAPX;x++)
   {
      for(int y=0;y<MAPY;y++)
      {
         for(int z=0;z<MAPZ;z++)
         {
            levelmap[x][y][z].flag=SITEBLOCK_BLOCK;
            levelmap[x][y][z].special=-1;
            levelmap[x][y][z].siegeflag=0;
         }
      }
   }

   levelmap[MAPX>>1][0][0].flag=SITEBLOCK_EXIT|SITEBLOCK_OUTDOOR;
   levelmap[(MAPX>>1)+1][0][0].flag=SITEBLOCK_EXIT|SITEBLOCK_OUTDOOR;
   levelmap[(MAPX>>1)+1][1][0].flag=SITEBLOCK_EXIT|SITEBLOCK_OUTDOOR;
   levelmap[(MAPX>>1)-1][0][0].flag=SITEBLOCK_EXIT|SITEBLOCK_OUTDOOR;
   levelmap[(MAPX>>1)-1][1][0].flag=SITEBLOCK_EXIT|SITEBLOCK_OUTDOOR;

   levelmap[MAPX>>1][1][0].flag=SITEBLOCK_OUTDOOR;

   levelmap[MAPX>>1][2][0].flag=SITEBLOCK_DOOR;

   //ADD RESTRICTIONS
   char restricted=0;
   switch(loc.type)
   {
      case SITE_INDUSTRY_SWEATSHOP:
      case SITE_INDUSTRY_POLLUTER:
      case SITE_INDUSTRY_NUCLEAR:
      case SITE_GOVERNMENT_INTELLIGENCEHQ:
      case SITE_CORPORATE_HEADQUARTERS:
      case SITE_CORPORATE_HOUSE:
         restricted++;
      case SITE_LABORATORY_COSMETICS:
      case SITE_LABORATORY_GENETIC:
      case SITE_GOVERNMENT_POLICESTATION:
      case SITE_GOVERNMENT_COURTHOUSE:
      case SITE_GOVERNMENT_PRISON:
      case SITE_MEDIA_AMRADIO:
      case SITE_MEDIA_CABLENEWS:
      case SITE_RESIDENTIAL_APARTMENT:
      case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
      case SITE_RESIDENTIAL_TENEMENT:
         restricted++;
         for(x=2;x<MAPX-2;x++)
         {
            for(int y=2;y<MAPY-2;y++)
            {
               for(int z=0;z<MAPZ;z++)
               {
                  levelmap[x][y][z].flag|=SITEBLOCK_RESTRICTED;
               }
            }
         }
         break;
   }

   if(loc.type==SITE_RESIDENTIAL_APARTMENT_UPSCALE||
      loc.type==SITE_RESIDENTIAL_APARTMENT||
      loc.type==SITE_RESIDENTIAL_TENEMENT)
   {
      levelmap[MAPX>>1][1][0].special=SPECIAL_APARTMENT_SIGN;
      short height;
      int floors=LCSrandom(6)+1;
      int swap;
      for(int z=0;z<floors;z++)
      {
         for(int y=3;y<MAPY-3;y++)
         {
            levelmap[MAPX>>1][y][z].flag=0;
            if(y%4==0)
            {
               height=y+LCSrandom(3)-1;
               levelmap[(MAPX>>1)-1][height][z].flag&=~SITEBLOCK_BLOCK;
               levelmap[(MAPX>>1)-1][height][z].flag|=SITEBLOCK_DOOR;
               generateroom((MAPX>>1)-8,y-1,7,3,z);

               height=y+LCSrandom(3)-1;
               levelmap[(MAPX>>1)+1][height][z].flag&=~SITEBLOCK_BLOCK;
               levelmap[(MAPX>>1)+1][height][z].flag|=SITEBLOCK_DOOR;
               generateroom((MAPX>>1)+2,y-1,7,3,z);
               if(y==4&&z==0)
               {
                  levelmap[(MAPX>>1)+2][height][z].flag=0;
                  levelmap[(MAPX>>1)+2][height][z].special=SPECIAL_APARTMENT_LANDLORD;
               }
            }
         }
         swap=(z%2)*2-1;
         if(z>0)
         {
            levelmap[(MAPX>>1)+1*swap][MAPY-4][z].flag=0;
            levelmap[(MAPX>>1)+1*swap][MAPY-4][z].special=SPECIAL_STAIRS_DOWN;
         }
         if(z<floors-1)
         {
            levelmap[(MAPX>>1)-1*swap][MAPY-4][z].flag=0;
            levelmap[(MAPX>>1)-1*swap][MAPY-4][z].special=SPECIAL_STAIRS_UP;
         }
      }
   }
   else
   {
      switch(loc.type)
      {
         case SITE_OUTDOOR_PUBLICPARK:
         {
            for(int x=(MAPX>>1)-7;x<=(MAPX>>1)+7;x++)
            {
               for(int y=0;y<15;y++)
               {
                  if(x==(MAPX>>1)-7||x==(MAPX>>1)+7||
                     y==0||y==14)levelmap[x][y][0].flag=SITEBLOCK_EXIT|SITEBLOCK_OUTDOOR;
                  else if(x==(MAPX>>1)||y==7)levelmap[x][y][0].flag=SITEBLOCK_OUTDOOR;
                  else levelmap[x][y][0].flag=SITEBLOCK_GRASSY;
                  levelmap[x][y][0].special=-1;
                  levelmap[x][y][0].siegeflag=0;
               }
            }
            break;
         }
         case SITE_BUSINESS_LATTESTAND:
         {
            for(int x=(MAPX>>1)-4;x<=(MAPX>>1)+4;x++)
            {
               for(int y=0;y<7;y++)
               {
                  if(x==(MAPX>>1)-4||x==(MAPX>>1)+4||
                     y==0||y==6)levelmap[x][y][0].flag=SITEBLOCK_EXIT;
                  else levelmap[x][y][0].flag=0;
                  levelmap[x][y][0].special=-1;
                  levelmap[x][y][0].siegeflag=0;
               }
            }
            break;
         }
         case SITE_BUSINESS_JUICEBAR:
         case SITE_BUSINESS_CIGARBAR:
         case SITE_BUSINESS_VEGANCOOP:
         case SITE_BUSINESS_INTERNETCAFE:
         {
            for(int x=(MAPX>>1)-4;x<=(MAPX>>1)+4;x++)
            {
               for(int y=3;y<10;y++)
               {
                  levelmap[x][y][0].flag=0;
                  levelmap[x][y][0].special=-1;
                  levelmap[x][y][0].siegeflag=0;
               }
            }
            break;
         }
         case SITE_BUSINESS_CRACKHOUSE:
         {
            int dx=LCSrandom(5)*2+19;
            int dy=LCSrandom(3)*2+7;
            int rx=(MAPX>>1)-(dx>>1);
            int ry=3;
            generateroom(rx,ry,dx,dy,0);
            break;
         }
         default:
         {
            int dx=LCSrandom(5)*2+35;
            int dy=LCSrandom(3)*2+15;
            int rx=(MAPX>>1)-(dx>>1);
            int ry=3;
            generateroom(rx,ry,dx,dy,0);
            break;
         }
      }
   }

   //CLEAR AWAY BLOCKED DOORWAYS
   char block;
   char opennum;
   for(x=0;x<MAPX;x++)
   {
      for(int y=0;y<MAPY;y++)
      {
         for(int z=0;z<MAPZ;z++)
         {
            if(levelmap[x][y][z].flag & SITEBLOCK_DOOR)
            {
               block=BIT1 | BIT2 | BIT3 | BIT4;
               opennum=0;

               if(x>0)if(!(levelmap[x-1][y][z].flag&SITEBLOCK_BLOCK)){block&=~BIT2;opennum++;}
               if(x<MAPX-1)if(!(levelmap[x+1][y][z].flag&SITEBLOCK_BLOCK)){block&=~BIT3;opennum++;}
               if(y>0)if(!(levelmap[x][y-1][z].flag&SITEBLOCK_BLOCK)){block&=~BIT1;opennum++;}
               if(y<MAPY-1)if(!(levelmap[x][y+1][z].flag&SITEBLOCK_BLOCK)){block&=~BIT4;opennum++;}

               if(opennum>=2)continue;

               //BLAST EVERYTHING AROUND TOTALLY BLOCKED DOOR
               if(block==(BIT1 | BIT2 | BIT3 | BIT4))
               {
                  if(x>0)levelmap[x-1][y][z].flag&=~SITEBLOCK_BLOCK;
                  if(x<MAPX-1)levelmap[x+1][y][z].flag&=~SITEBLOCK_BLOCK;
                  if(y>0)levelmap[x][y-1][z].flag&=~SITEBLOCK_BLOCK;
                  if(y<MAPY-1)levelmap[x][y+1][z].flag&=~SITEBLOCK_BLOCK;
               }
               //DEAD-END OR OPEN A THREE-BLOCKED DOOR
               else if(!(block&BIT1))
               {
                  if(y<MAPY-1)levelmap[x][y+1][z].flag&=~SITEBLOCK_BLOCK;
                  else levelmap[x][y+1][z].flag|=SITEBLOCK_BLOCK;
               }
               else if(!(block&BIT4))
               {
                  if(y>0)levelmap[x][y-1][z].flag&=~SITEBLOCK_BLOCK;
                  else levelmap[x][y-1][z].flag|=SITEBLOCK_BLOCK;
               }
               else if(!(block&BIT2))
               {
                  if(x<MAPX-1)levelmap[x+1][y][z].flag&=~SITEBLOCK_BLOCK;
                  else levelmap[x+1][y][z].flag|=SITEBLOCK_BLOCK;
               }
               else if(!(block&BIT3))
               {
                  if(x>0)levelmap[x-1][y][z].flag&=~SITEBLOCK_BLOCK;
                  else levelmap[x-1][y][z].flag|=SITEBLOCK_BLOCK;
               }
            }
         }
      }
   }

   //DELETE NON-DOORS
   for(x=0;x<MAPX;x++)
   {
      for(int y=0;y<MAPY;y++)
      {
         for(int z=0;z<MAPZ;z++)
         {
            if(levelmap[x][y][z].flag & SITEBLOCK_DOOR)
            {
               block=BIT1 | BIT2 | BIT3 | BIT4;

               if(x>0)if(!(levelmap[x-1][y][z].flag&SITEBLOCK_BLOCK))block&=~BIT2;
               if(x<MAPX-1)if(!(levelmap[x+1][y][z].flag&SITEBLOCK_BLOCK))block&=~BIT3;
               if(y>0)if(!(levelmap[x][y-1][z].flag&SITEBLOCK_BLOCK))block&=~BIT1;
               if(y<MAPY-1)if(!(levelmap[x][y+1][z].flag&SITEBLOCK_BLOCK))block&=~BIT4;

               if((block & BIT1) && (block & BIT4))continue;
               if((block & BIT2) && (block & BIT3))continue;

               levelmap[x][y][z].flag&=~SITEBLOCK_DOOR;
               levelmap[x][y][z].flag&=~SITEBLOCK_LOCKED;
            }
         }
      }
   }

   //CLEAR FIRST FLOOR RESTRICTIONS NEAR TO DOOR
   char acted;
   if(restricted<2)levelmap[MAPX>>1][2][0].flag&=~SITEBLOCK_RESTRICTED;
   //Clear out restrictions
   do
   {
      acted=0;

      for(int x=2;x<MAPX-2;x++)
      {
         for(int y=2;y<MAPY-2;y++)
         {
            for(int z=0;z<MAPZ;z++)
            {
               //Un-restrict blocks if they have neighboring
               //restricted blocks
               if(!(levelmap[x][y][z].flag & SITEBLOCK_DOOR)&&
                  !(levelmap[x][y][z].flag & SITEBLOCK_BLOCK)&&
                  (levelmap[x][y][z].flag & SITEBLOCK_RESTRICTED))
               {
                  if(!(levelmap[x-1][y][z].flag & SITEBLOCK_RESTRICTED)||
                     !(levelmap[x+1][y][z].flag & SITEBLOCK_RESTRICTED)||
                     !(levelmap[x][y-1][z].flag & SITEBLOCK_RESTRICTED)||
                     !(levelmap[x][y+1][z].flag & SITEBLOCK_RESTRICTED))
                  {
                     levelmap[x][y][z].flag&=~SITEBLOCK_RESTRICTED;
                     acted=1;
                     continue;
                  }
               }
               //Un-restrict and unlock doors if they lead between two
               //unrestricted sections. If they lead between one
               //unrestricted section and a restricted section, lock
               //them instead.
               else if((levelmap[x][y][z].flag & SITEBLOCK_DOOR)&&
                  !(levelmap[x][y][z].flag & SITEBLOCK_BLOCK)&&
                  (levelmap[x][y][z].flag & SITEBLOCK_RESTRICTED))
               {
                  //Unrestricted on two opposite sides?
                  if((!(levelmap[x-1][y][z].flag & SITEBLOCK_RESTRICTED)&&
                     !(levelmap[x+1][y][z].flag & SITEBLOCK_RESTRICTED))||
                     (!(levelmap[x][y-1][z].flag & SITEBLOCK_RESTRICTED)&&
                     !(levelmap[x][y+1][z].flag & SITEBLOCK_RESTRICTED)))
                  {
                     levelmap[x][y][z].flag&=~SITEBLOCK_LOCKED;
                     levelmap[x][y][z].flag&=~SITEBLOCK_RESTRICTED;
                     acted=1;
                     continue;
                  }
                  //Unrestricted on at least one side and I'm not locked?
                  else if((!(levelmap[x-1][y][z].flag & SITEBLOCK_RESTRICTED)||
                     !(levelmap[x+1][y][z].flag & SITEBLOCK_RESTRICTED)||
                     !(levelmap[x][y-1][z].flag & SITEBLOCK_RESTRICTED)||
                     !(levelmap[x][y+1][z].flag & SITEBLOCK_RESTRICTED))&&
                     !(levelmap[x][y][z].flag & SITEBLOCK_LOCKED))
                  {
                     levelmap[x][y][z].flag|=SITEBLOCK_LOCKED;
                     acted=1;
                     continue;
                  }
               }
            }
         }
      }
   }while(acted);

   //ADD ACCESSORIES
   seed=oldseed;
   for(x=2;x<MAPX-2;x++)
   {
      for(int y=2;y<MAPY-2;y++)
      {
         for(int z=0;z<MAPZ;z++)
         {
            if(!(levelmap[x][y][0].flag & SITEBLOCK_DOOR)&&
               !(levelmap[x][y][0].flag & SITEBLOCK_BLOCK)&&
               (levelmap[x][y][0].flag & SITEBLOCK_RESTRICTED)&&
               !LCSrandom(10))
            {
               switch(loc.type)
               {
                  case SITE_RESIDENTIAL_SHELTER:
                  case SITE_BUSINESS_CRACKHOUSE:
                  case SITE_BUSINESS_JUICEBAR:
                  case SITE_BUSINESS_CIGARBAR:
                  case SITE_BUSINESS_LATTESTAND:
                  case SITE_BUSINESS_VEGANCOOP:
                  case SITE_BUSINESS_INTERNETCAFE:
                  case SITE_INDUSTRY_WAREHOUSE:
                     break;
                  default:
                     levelmap[x][y][z].flag|=SITEBLOCK_LOOT;
                     break;
               }
            }

            if(!(levelmap[x][y][0].flag & SITEBLOCK_DOOR)&&
               !(levelmap[x][y][0].flag & SITEBLOCK_BLOCK)&&
               !(levelmap[x][y][0].flag & SITEBLOCK_LOOT)&&
               (levelmap[x][y][0].flag & SITEBLOCK_RESTRICTED)&&
               loc.type==SITE_LABORATORY_COSMETICS&&!LCSrandom(10))
            {
               levelmap[x][y][z].special=SPECIAL_LAB_COSMETICS_CAGEDANIMALS;
            }
            if(!(levelmap[x][y][0].flag & SITEBLOCK_DOOR)&&
               !(levelmap[x][y][0].flag & SITEBLOCK_BLOCK)&&
               !(levelmap[x][y][0].flag & SITEBLOCK_LOOT)&&
               (levelmap[x][y][0].flag & SITEBLOCK_RESTRICTED)&&
               loc.type==SITE_LABORATORY_GENETIC&&!LCSrandom(10))
            {
               levelmap[x][y][z].special=SPECIAL_LAB_GENETIC_CAGEDANIMALS;
            }
            if(!(levelmap[x][y][0].flag & SITEBLOCK_DOOR)&&
               !(levelmap[x][y][0].flag & SITEBLOCK_BLOCK)&&
               !(levelmap[x][y][0].flag & SITEBLOCK_LOOT)&&
               (levelmap[x][y][0].flag & SITEBLOCK_RESTRICTED)&&
               loc.type==SITE_INDUSTRY_SWEATSHOP&&!LCSrandom(10))
            {
               levelmap[x][y][z].special=SPECIAL_SWEATSHOP_EQUIPMENT;
            }
            if(!(levelmap[x][y][0].flag & SITEBLOCK_DOOR)&&
               !(levelmap[x][y][0].flag & SITEBLOCK_BLOCK)&&
               !(levelmap[x][y][0].flag & SITEBLOCK_LOOT)&&
               (levelmap[x][y][0].flag & SITEBLOCK_RESTRICTED)&&
               loc.type==SITE_INDUSTRY_POLLUTER&&!LCSrandom(10))
            {
               levelmap[x][y][z].special=SPECIAL_POLLUTER_EQUIPMENT;
            }
            if(levelmap[x][y][0].flag==0&&
               (loc.type==SITE_BUSINESS_JUICEBAR||
               loc.type==SITE_BUSINESS_CIGARBAR||
               loc.type==SITE_BUSINESS_LATTESTAND||
               loc.type==SITE_BUSINESS_INTERNETCAFE)&&
               !LCSrandom(10))
            {
               levelmap[x][y][z].special=SPECIAL_RESTAURANT_TABLE;
            }
            if(levelmap[x][y][z].flag==0&&loc.type==SITE_BUSINESS_INTERNETCAFE&&
               !LCSrandom(10))
            {
               levelmap[x][y][z].special=SPECIAL_CAFE_COMPUTER;
            }
         }
      }
   }

   int freex,freey,freez=0;

   //ADD FIRST SPECIAL
   int count=100000;

   do
   {
      freex=LCSrandom(MAPX-4)+2;
      freey=LCSrandom(MAPY-4)+2;
      if(freex>=(MAPX>>1)-2&&
         freex<=(MAPX>>1)+2)freey=LCSrandom(MAPY-6)+4;
      count--;
   }while(( levelmap[freex][freey][freez].flag & SITEBLOCK_DOOR  ||
            levelmap[freex][freey][freez].flag & SITEBLOCK_BLOCK ||
            levelmap[freex][freey][freez].flag & SITEBLOCK_LOOT  ||
            levelmap[freex][freey][freez].special!=-1   )&&count>0);

   switch(loc.type)
   {
      case SITE_INDUSTRY_NUCLEAR:
         levelmap[freex][freey][freez].special=SPECIAL_NUCLEAR_ONOFF;
         break;
      case SITE_GOVERNMENT_POLICESTATION:
         levelmap[freex][freey][freez].special=SPECIAL_POLICESTATION_LOCKUP;
         break;
      case SITE_GOVERNMENT_COURTHOUSE:
         levelmap[freex][freey][freez].special=SPECIAL_COURTHOUSE_LOCKUP;
         break;
      case SITE_GOVERNMENT_PRISON:
         levelmap[freex][freey][freez].special=SPECIAL_PRISON_CONTROL;
         break;
      case SITE_GOVERNMENT_INTELLIGENCEHQ:
         levelmap[freex][freey][freez].special=SPECIAL_INTEL_SUPERCOMPUTER;
         break;
      case SITE_CORPORATE_HEADQUARTERS:
         levelmap[freex][freey][freez].special=SPECIAL_CORPORATE_FILES;
         break;
      case SITE_CORPORATE_HOUSE:
         levelmap[freex][freey][freez].special=SPECIAL_HOUSE_PHOTOS;
         break;
      case SITE_MEDIA_AMRADIO:
         levelmap[freex][freey][freez].special=SPECIAL_RADIO_BROADCASTSTUDIO;
         break;
      case SITE_MEDIA_CABLENEWS:
         levelmap[freex][freey][freez].special=SPECIAL_NEWS_BROADCASTSTUDIO;
         break;
   }

   count=100000;

   //ADD SECOND SPECIAL
   do
   {
      freex=LCSrandom(MAPX-4)+2;
      freey=LCSrandom(MAPY-4)+2;
      if(freex>=(MAPX>>1)-2&&
         freex<=(MAPX>>1)+2)freey=LCSrandom(MAPY-6)+4;
      count--;
   }while(( levelmap[freex][freey][freez].flag & SITEBLOCK_DOOR  ||
            levelmap[freex][freey][freez].flag & SITEBLOCK_BLOCK ||
            levelmap[freex][freey][freez].flag & SITEBLOCK_LOOT  ||
            levelmap[freex][freey][freez].special!=-1   )&&count>0);

   switch(loc.type)
   {
      case SITE_GOVERNMENT_COURTHOUSE:
         levelmap[freex][freey][freez].special=SPECIAL_COURTHOUSE_JURYROOM;
         break;
   }
}



/* recursive dungeon-generating algorithm */
void generateroom(int rx,int ry,int dx,int dy,int z)
{
   for(int x=rx;x<rx+dx;x++)
   {
      for(int y=ry;y<ry+dy;y++)
      {
         levelmap[x][y][z].flag&=~SITEBLOCK_BLOCK;
      }
   }

   if((dx<=3||dy<=3)&&!LCSrandom(2))return;

   if(dx<=2&&dy<=2)return;

   //LAY DOWN WALL AND ITERATE
   if((!LCSrandom(2)||dy<=2)&&dx>2)
   {
      int wx=rx+LCSrandom(dx-2)+1;

      for(int wy=0;wy<dy;wy++)levelmap[wx][ry+wy][z].flag|=SITEBLOCK_BLOCK;
      int rny=LCSrandom(dy);
      levelmap[wx][ry+rny][z].flag&=~SITEBLOCK_BLOCK;
      levelmap[wx][ry+rny][z].flag|=SITEBLOCK_DOOR;
      if(!LCSrandom(3))levelmap[wx][ry+rny][z].flag|=SITEBLOCK_LOCKED;

      generateroom(rx,ry,wx-rx,dy,z);

      generateroom(wx+1,ry,rx+dx-wx-1,dy,z);
   }
   else
   {
      int wy=ry+LCSrandom(dy-2)+1;

      for(int wx=0;wx<dx;wx++)levelmap[rx+wx][wy][z].flag|=SITEBLOCK_BLOCK;
      int rnx=LCSrandom(dx);
      levelmap[rx+rnx][wy][z].flag&=~SITEBLOCK_BLOCK;
      levelmap[rx+rnx][wy][z].flag|=SITEBLOCK_DOOR;
      if(!LCSrandom(3))levelmap[rx+rnx][wy][z].flag|=SITEBLOCK_LOCKED;

      generateroom(rx,ry,dx,wy-ry,z);

      generateroom(rx,wy+1,dx,ry+dy-wy-1,z);
   }
}



/* marks the area around the specified tile as explored */
void knowmap(int locx,int locy,int locz)
{
   levelmap[locx][locy][locz].flag|=SITEBLOCK_KNOWN;

   if(locx>0)levelmap[locx-1][locy][locz].flag|=SITEBLOCK_KNOWN;
   if(locx<MAPX-1)levelmap[locx+1][locy][locz].flag|=SITEBLOCK_KNOWN;
   if(locy>0)levelmap[locx][locy-1][locz].flag|=SITEBLOCK_KNOWN;
   if(locy<MAPY-1)levelmap[locx][locy+1][locz].flag|=SITEBLOCK_KNOWN;

   if(locx>0&&locy>0)
   {
      if(!(levelmap[locx-1][locy][locz].flag & SITEBLOCK_BLOCK)||
         !(levelmap[locx][locy-1][locz].flag & SITEBLOCK_BLOCK))
      {
         levelmap[locx-1][locy-1][locz].flag|=SITEBLOCK_KNOWN;
      }
   }
   if(locx<MAPX-1&&locy>0)
   {
      if(!(levelmap[locx+1][locy][locz].flag & SITEBLOCK_BLOCK)||
         !(levelmap[locx][locy-1][locz].flag & SITEBLOCK_BLOCK))
      {
         levelmap[locx+1][locy-1][locz].flag|=SITEBLOCK_KNOWN;
      }
   }
   if(locx>0&&locy<MAPY-1)
   {
      if(!(levelmap[locx-1][locy][locz].flag & SITEBLOCK_BLOCK)||
         !(levelmap[locx][locy+1][locz].flag & SITEBLOCK_BLOCK))
      {
         levelmap[locx-1][locy+1][locz].flag|=SITEBLOCK_KNOWN;
      }
   }
   if(locx<MAPX-1&&locy<MAPY-1)
   {
      if(!(levelmap[locx+1][locy][locz].flag & SITEBLOCK_BLOCK)||
         !(levelmap[locx][locy+1][locz].flag & SITEBLOCK_BLOCK))
      {
         levelmap[locx+1][locy+1][locz].flag|=SITEBLOCK_KNOWN;
      }
   }
}
