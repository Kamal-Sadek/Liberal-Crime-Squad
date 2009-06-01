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

//#include <includes.h>
#include <externs.h>


// Imperfect but quick and dirty line of sight check
// Only works if the target point is at most two spaces
// away in any direction
bool LineOfSight(int x, int y, int z)
{
   if(levelmap[x][y][z].flag & SITEBLOCK_KNOWN)
      return true;

   int x1,x2;
   int y1,y2;

   if(abs(x-locx)==1)
   {
      x1=locx;
      x2=x;
   }
   else x1=x2=(x+locx)/2;

   if(abs(y-locy)==1)
   {
      y1=locy;
      y2=y;
   }
   else y1=y2=(y+locy)/2;

   // Check for obstructions
   if(levelmap[x1][y2][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT))
   {
      if(levelmap[x2][y1][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT))
      {
         return 0; // Blocked on some axis
      }
   }
   return 1;
}

// Prints the map graphics in the bottom right of the screen
void printsitemap(int x,int y,int z)
{
   int xscreen, xsite;
   int yscreen, ysite;

   // Build the frame
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   for(xscreen=53;xscreen<80;xscreen++)
   {
      move(24,xscreen);
      addch('-');
   }
   for(yscreen=9;yscreen<24;yscreen++)
   {
      move(yscreen,79);
      addch('|');
      move(yscreen,53);
      addch('|');
   }

   // Display the map
   for(xsite=x-2,xscreen=79-5*5;xsite<x+3;xscreen+=5,xsite++)
   {
      for(ysite=y-2,yscreen=24-3*5;ysite<y+3;yscreen+=3,ysite++)
      {
         printblock(xsite,ysite,z,xscreen,yscreen);
      }
   }

   //PRINT SPECIAL
   char str[200];
   switch(levelmap[locx][locy][locz].special)
   {
      case SPECIAL_LAB_COSMETICS_CAGEDANIMALS:strcpy(str,"Caged Animals");break;
      case SPECIAL_NUCLEAR_ONOFF:strcpy(str,"The On/Off Switch");break;
      case SPECIAL_LAB_GENETIC_CAGEDANIMALS:strcpy(str,"Caged \"Animals\"");break;
      case SPECIAL_POLICESTATION_LOCKUP:strcpy(str,"Police Detention Room");break;
      case SPECIAL_COURTHOUSE_LOCKUP:strcpy(str,"Court House Jail");break;
      case SPECIAL_COURTHOUSE_JURYROOM:strcpy(str,"Jury Room");break;
      case SPECIAL_PRISON_CONTROL:strcpy(str,"Prison Control Room");break;
      case SPECIAL_INTEL_SUPERCOMPUTER:strcpy(str,"Super-computer");break;
      case SPECIAL_SWEATSHOP_EQUIPMENT:strcpy(str,"Textile Equipment");break;
      case SPECIAL_POLLUTER_EQUIPMENT:strcpy(str,"Factory Equipment");break;
      case SPECIAL_HOUSE_PHOTOS:strcpy(str,"Safe");break;
      case SPECIAL_CORPORATE_FILES:strcpy(str,"Safe");break;
      case SPECIAL_RADIO_BROADCASTSTUDIO:strcpy(str,"Radio Broadcast Room");break;
      case SPECIAL_NEWS_BROADCASTSTUDIO:strcpy(str,"News Broadcast Studio");break;
      case SPECIAL_APARTMENT_LANDLORD:strcpy(str,"Landlord's Office");break;
      case SPECIAL_APARTMENT_SIGN:strcpy(str,"Sign");break;
      case SPECIAL_STAIRS_UP:strcpy(str,"Stairs Up");break;
      case SPECIAL_STAIRS_DOWN:strcpy(str,"Stairs Down");break;
      case SPECIAL_RESTAURANT_TABLE:strcpy(str,"Table");break;
      case SPECIAL_CAFE_COMPUTER:strcpy(str,"Computer");break;
      case SPECIAL_PARK_BENCH:strcpy(str,"Bench");break;
      case SPECIAL_CLUB_BOUNCER:strcpy(str,"");break;
      case SPECIAL_CLUB_BOUNCER_SECONDVISIT:strcpy(str,"");break;
   }
   if(levelmap[locx][locy][locz].special!=-1)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(24,67-(strlen(str)>>1));
      addstr(str);
   }

   //PRINT PARTY
   int partyalive=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;
      if(activesquad->squad[p]->alive==1)partyalive++;
   }
   if(partyalive>0)set_color(COLOR_GREEN,COLOR_BLACK,1);
   else set_color(COLOR_BLACK,COLOR_BLACK,1);
   move(16,64);
   addstr("SQUAD");

   int encsize=0;
   for(int e=0;e<ENCMAX;e++)
   {
      if(encounter[e].exists)
      {
         encsize++;
      }
   }
   //PRINT ANY OPPOSING FORCE INFO
   if(encsize>0)
   {
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      move(17,64);
      if(levelmap[locx][locx][locz].siegeflag & SIEGEFLAG_HEAVYUNIT)
      {
         addstr("ARMOR");
      }
      else if(levelmap[locx][locx][locz].siegeflag & SIEGEFLAG_UNIT)
      {
         addstr("ENEMY");
      }
      else if(levelmap[locx][locx][locz].siegeflag & SIEGEFLAG_UNIT_DAMAGED)
      {
         addstr("enemy");
      }
      else addstr("ENCTR");

      printencounter();
   }

   if(groundloot.size()>0)
   {
      set_color(COLOR_MAGENTA,COLOR_BLACK,1);
      move(15,64);
      addstr("LOOT!");

      printencounter();
   }
}

#define WALL_UP    0
#define WALL_DOWN  1
#define WALL_LEFT  2
#define WALL_RIGHT 3

void printwall(int x, int y, int z, int px, int py)
{
   bool visible[4]     = {false,false,false,false};
   bool bloody[4]      = {false,false,false,false};
   char graffiti[4][4] = {"   ","   ","   ","   "};
   char graffiticolor[4] = {COLOR_BLACK,COLOR_BLACK,COLOR_BLACK,COLOR_BLACK};

   char type = 0; // Are we drawing a wall or a door?
   if(levelmap[x][y][z].flag & SITEBLOCK_BLOCK)     { type = SITEBLOCK_BLOCK; }
   else if(levelmap[x][y][z].flag & SITEBLOCK_DOOR) { type = levelmap[x][y][z].flag; } // Retain locked/jammed data

   // Now follows a series of checks to determine the faces of the wall that should be
   // displayed. Note the order of these checks is important:
   //
   // 1) You will see the wall if it's the upward face and you're above it (directional visibility)...
   // 2) ...unless your line of sight is blocked (LOS)...
   // 3) ...but line of sight and directional visibility is not important if you have already seen that
   //          tile (memory)...
   // 4) ...and regardless of any of the above, if there's a physical obstruction that would prevent you
   //          from seeing it even if you were near it, like a wall, it should not be shown (blockages).
   //
   // The order of the remainder of the checks is not crucial.

   // 1) Check for directional visibility
   if(y<locy && y>0)         { visible[WALL_DOWN] = true; }
   else if(y>locy && y<MAPY) { visible[WALL_UP] = true;   }

   if(x<locx && x>0)         { visible[WALL_RIGHT] = true; }
   else if(x>locx && x<MAPX) { visible[WALL_LEFT] = true;  }

   // 2) Check LOS
   if(!LineOfSight(x-1,y,z)) { visible[WALL_LEFT] = false; }
   if(!LineOfSight(x+1,y,z)) { visible[WALL_RIGHT] = false; }
   if(!LineOfSight(x,y-1,z)) { visible[WALL_UP] = false; }
   if(!LineOfSight(x,y+1,z)) { visible[WALL_DOWN] = false; }

   // 3) Check for memory
   if(levelmap[x-1][y][z].flag & (SITEBLOCK_KNOWN)) { visible[WALL_LEFT] = true; }
   if(levelmap[x+1][y][z].flag & (SITEBLOCK_KNOWN)) { visible[WALL_RIGHT] = true; }
   if(levelmap[x][y-1][z].flag & (SITEBLOCK_KNOWN)) { visible[WALL_UP] = true; }
   if(levelmap[x][y+1][z].flag & (SITEBLOCK_KNOWN)) { visible[WALL_DOWN] = true; }

   // 4) Check for blockages
   if(levelmap[x-1][y][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)) { visible[WALL_LEFT] = false; }
   if(levelmap[x+1][y][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)) { visible[WALL_RIGHT] = false; }
   if(levelmap[x][y-1][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)) { visible[WALL_UP] = false; }
   if(levelmap[x][y+1][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)) { visible[WALL_DOWN] = false; }

   // Below not used for doors
   if(levelmap[x][y][z].flag & SITEBLOCK_BLOCK)
   {
      // Check for bloody walls
      if(levelmap[x-1][y][z].flag & SITEBLOCK_BLOODY2) { bloody[WALL_LEFT] = true; }
      if(levelmap[x+1][y][z].flag & SITEBLOCK_BLOODY2) { bloody[WALL_RIGHT] = true; }
      if(levelmap[x][y-1][z].flag & SITEBLOCK_BLOODY2) { bloody[WALL_UP] = true; }
      if(levelmap[x][y+1][z].flag & SITEBLOCK_BLOODY2) { bloody[WALL_DOWN] = true; }

      // Check for other graffiti TODO: Stalinist graffiti
      if(levelmap[x-1][y][z].flag & SITEBLOCK_GRAFFITI_OTHER) { strcpy(graffiti[WALL_LEFT],"GNG"); graffiticolor[WALL_LEFT] = COLOR_BLACK; }
      if(levelmap[x+1][y][z].flag & SITEBLOCK_GRAFFITI_OTHER) { strcpy(graffiti[WALL_RIGHT],"GNG"); graffiticolor[WALL_RIGHT] = COLOR_BLACK; }
      if(levelmap[x][y-1][z].flag & SITEBLOCK_GRAFFITI_OTHER) { strcpy(graffiti[WALL_UP],"GNG"); graffiticolor[WALL_UP] = COLOR_BLACK; }
      if(levelmap[x][y+1][z].flag & SITEBLOCK_GRAFFITI_OTHER) { strcpy(graffiti[WALL_DOWN],"GNG"); graffiticolor[WALL_DOWN] = COLOR_BLACK; }

      // Check for CCS graffiti
      if(levelmap[x-1][y][z].flag & SITEBLOCK_GRAFFITI_CCS) { strcpy(graffiti[WALL_LEFT],"CCS"); graffiticolor[WALL_LEFT] = COLOR_RED; }
      if(levelmap[x+1][y][z].flag & SITEBLOCK_GRAFFITI_CCS) { strcpy(graffiti[WALL_RIGHT],"CCS"); graffiticolor[WALL_RIGHT] = COLOR_RED; }
      if(levelmap[x][y-1][z].flag & SITEBLOCK_GRAFFITI_CCS) { strcpy(graffiti[WALL_UP],"CCS"); graffiticolor[WALL_UP] = COLOR_RED; }
      if(levelmap[x][y+1][z].flag & SITEBLOCK_GRAFFITI_CCS) { strcpy(graffiti[WALL_DOWN],"CCS"); graffiticolor[WALL_DOWN] = COLOR_RED; }

      // Check for LCS graffiti
      if(levelmap[x-1][y][z].flag & SITEBLOCK_GRAFFITI) { strcpy(graffiti[WALL_LEFT],"LCS"); graffiticolor[WALL_LEFT] = COLOR_GREEN; }
      if(levelmap[x+1][y][z].flag & SITEBLOCK_GRAFFITI) { strcpy(graffiti[WALL_RIGHT],"LCS"); graffiticolor[WALL_RIGHT] = COLOR_GREEN; }
      if(levelmap[x][y-1][z].flag & SITEBLOCK_GRAFFITI) { strcpy(graffiti[WALL_UP],"LCS"); graffiticolor[WALL_UP] = COLOR_GREEN; }
      if(levelmap[x][y+1][z].flag & SITEBLOCK_GRAFFITI) { strcpy(graffiti[WALL_DOWN],"LCS"); graffiticolor[WALL_DOWN] = COLOR_GREEN; }
   }
   
   for(int dir=0;dir<4;dir++)
   {
      x=px;
      y=py;

      // Draw the wall/door
      if(visible[dir])
      {
         if(type == SITEBLOCK_BLOCK)
         {
            // Position cursor at the start of where the graffiti tag would go
            if(dir==WALL_UP) { x++; }
            if(dir==WALL_DOWN) { y+=2; x++; }
            if(dir==WALL_RIGHT) { x+=4; }
            if(dir==WALL_LEFT) { }

            // Blood overrides graffiti overrides gray base wall color
            if(bloody[dir])
               set_color(COLOR_RED,COLOR_RED,0);
            else if(graffiti[dir][0]!=' ')
               set_color(graffiticolor[dir],COLOR_WHITE,0);
            else
               set_color(COLOR_WHITE,COLOR_WHITE,0);
            
            // Draw the chunk of wall where the graffiti would/will go
            for(int gchar=0;gchar<3;gchar++)
            {
               move(y,x);
               addch(graffiti[dir][gchar]);

               if(dir==WALL_RIGHT||dir==WALL_LEFT)
                  y++;
               else
                  x++;
            }

            // For the long faces (top and bottom) of the wall, there is
            // additional space to either side of the 'tag' (or lack of tag)
            // that needs to be filled in with wall color

            // Note that I'm ignoring bloodiness for these points; this is
            // somewhat intentional
            if(dir == WALL_UP || dir == WALL_DOWN)
            {
               set_color(COLOR_WHITE,COLOR_WHITE,0);
               if(!visible[WALL_LEFT])
               {
                  move(y,px);
                  addch(' ');
               }
               if(!visible[WALL_RIGHT])
               {
                  move(y,px+4);
                  addch(' ');
               }
            }
         }
         else if(type & SITEBLOCK_DOOR)
         {
            // Doors are, thankfully, much simpler, as they do not
            // support blood or graffiti

            // Position cursor at the start of face
            if(dir==WALL_DOWN)  { y+=2; }
            if(dir==WALL_RIGHT) { x+=4; }

            // Pick color
            if(type & SITEBLOCK_CLOCK && type & SITEBLOCK_LOCKED)
               set_color(COLOR_RED,COLOR_BLACK,0);
            else if(type & SITEBLOCK_KLOCK && type & SITEBLOCK_LOCKED)
               set_color(COLOR_BLACK,COLOR_BLACK,1);
            else
               set_color(COLOR_YELLOW,COLOR_BLACK,0);
            
            // Draw face
            if(dir==WALL_RIGHT||dir==WALL_LEFT)
            {
               for(int i=0;i<3;i++)
               { move(y,x); addch('|'); y++; }
            }
            else
            {
               for(int i=0;i<5;i++)
               { move(y,x); addch('-'); x++; }
            }
         }
      }
   }
}

void printblock(int x,int y,int z,int px, int py)
{
   if(!LineOfSight(x,y,locz))
   {
      set_color(COLOR_BLACK,COLOR_BLACK,0);
      for(x=px;x<px+5;x++)
      {
         for(y=py;y<py+3;y++)
         {
            move(y,x);
            addch(' ');
         }
      }
      return;
   }
   levelmap[x][y][z].flag |= SITEBLOCK_KNOWN;
   if(levelmap[x][y][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR))
   {
      printwall(x,y,z,px,py);
      return;
   }
   int backcolor=COLOR_BLACK;
   char blink=0;
   char ch=' ';
   if(levelmap[x][y][z].flag & SITEBLOCK_RESTRICTED)
   {
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      ch='+';
   }
   else if(levelmap[x][y][z].flag & SITEBLOCK_GRASSY)
   {
      set_color(COLOR_GREEN,COLOR_BLACK,0);
      ch='.';
   }
   else if(levelmap[x][y][z].flag & SITEBLOCK_OUTDOOR)
   {
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      ch=' ';
   }
   else
   {
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      ch=' ';
   }

   for(int px2=px;px2<px+5;px2++)
   {
      for(int py2=py;py2<py+3;py2++)
      {
         move(py2,px2);
         addch(ch);
      }
   }

   if(levelmap[x][y][z].flag & SITEBLOCK_DEBRIS)
   {
      set_color(COLOR_WHITE,backcolor,1,blink);
      move(py+0,px+1);
      addch('.');
      move(py+0,px+4);
      addch('^');
      move(py+1,px+0);
      addch('=');
      move(py+1,px+3);
      addch('.');
      move(py+1,px+4);
      addch('|');
      move(py+2,px+1);
      addch('.');
      move(py+2,px+4);
      addch('\\');
   }

   if(levelmap[x][y][z].flag & SITEBLOCK_FIRE_START)
   {
      set_color(COLOR_RED,backcolor,1,blink);
      move(py+0,px+1);
      addch('.');
      set_color(COLOR_YELLOW,backcolor,1,blink);
      move(py+1,px+3);
      addch('.');
      move(py+2,px+1);
      addch('.');
   }

   if(levelmap[x][y][z].flag & SITEBLOCK_FIRE_PEAK)
   {
      set_color(COLOR_RED,backcolor,1,blink);
      move(py+0,px+1);
      addch(':');
      set_color(COLOR_YELLOW,backcolor,1,blink);
      move(py+0,px+4);
      addch('$');
      set_color(COLOR_RED,backcolor,1,blink);
      move(py+1,px+0);
      addch('*');
      set_color(COLOR_YELLOW,backcolor,1,blink);
      move(py+1,px+3);
      addch(':');
      move(py+1,px+4);
      addch('%');
      set_color(COLOR_YELLOW,backcolor,1,blink);
      move(py+2,px+1);
      addch(':');
      move(py+2,px+4);
      addch('*');
   }

   if(levelmap[x][y][z].flag & SITEBLOCK_FIRE_END)
   {
      set_color(COLOR_WHITE,backcolor,1,blink);
      move(py+0,px+1);
      addch('.');
      set_color(COLOR_YELLOW,backcolor,1,blink);
      move(py+0,px+4);
      addch('~');
      set_color(COLOR_RED,backcolor,1,blink);
      move(py+1,px+0);
      addch('*');
      set_color(COLOR_WHITE,backcolor,1,blink);
      move(py+1,px+3);
      addch('.');
      move(py+1,px+4);
      addch('|');
      set_color(COLOR_WHITE,backcolor,1,blink);
      move(py+2,px+1);
      addch('.');
      set_color(COLOR_YELLOW,backcolor,1,blink);
      move(py+2,px+4);
      addch('#');
   }

   if(levelmap[x][y][z].flag & SITEBLOCK_BLOODY2)
   {
      set_color(COLOR_RED,backcolor,0,blink);
      move(py+1,px+1);
      addch('%');
      move(py+2,px+1);
      addch('.');
      move(py+1,px+2);
      addch('.');
   }
   else if(levelmap[x][y][z].flag & SITEBLOCK_BLOODY)
   {
      set_color(COLOR_RED,backcolor,0,blink);
      move(py+2,px+1);
      addch('.');
      move(py+1,px+2);
      addch('.');
   }

   if(levelmap[x][y][z].flag & SITEBLOCK_EXIT)
   {
      set_color(COLOR_WHITE,backcolor,0,blink);
      move(py+1,px+1);
      addstr("EXT");
   }
   else if(levelmap[x][y][z].flag & SITEBLOCK_DOOR)
   {
      if((levelmap[x][y][z].flag & SITEBLOCK_CLOCK) &&
         (levelmap[x][y][z].flag & SITEBLOCK_LOCKED))
      {
         set_color(COLOR_RED,backcolor,0,blink);
         move(py,px);
         addstr("=====");
         move(py+1,px);
         addstr("=====");
         move(py+2,px);
         addstr("=====");
      }
      else if((levelmap[x][y][z].flag & SITEBLOCK_KLOCK) &&
         (levelmap[x][y][z].flag & SITEBLOCK_LOCKED))
      {
         set_color(COLOR_BLACK,backcolor,1,blink);
         move(py,px);
         addstr("=====");
         move(py+1,px);
         addstr("=====");
         move(py+2,px);
         addstr("=====");
      }
      else
      {
         set_color(COLOR_YELLOW,backcolor,0,blink);
         move(py,px);
         addstr("=====");
         move(py+1,px);
         addstr("=====");
         move(py+2,px);
         addstr("=====");
      }
   }
   else if(levelmap[x][y][z].flag & SITEBLOCK_LOOT)
   {
      set_color(COLOR_MAGENTA,backcolor,1,blink);
      move(py,px+1);
      addstr("~$~");
   }

   if(levelmap[x][y][z].siegeflag & SIEGEFLAG_TRAP)
   {
      set_color(COLOR_YELLOW,backcolor,1,blink);
      move(py+1,px);
      addstr("TRAP!");
   }
   else if(levelmap[x][y][z].siegeflag & SIEGEFLAG_UNIT_DAMAGED)
   {
      set_color(COLOR_RED,backcolor,0,blink);
      move(py+2,px+1);
      addstr("enemy");
   }
   else if(levelmap[x][y][z].special!=-1)
   {
      set_color(COLOR_YELLOW,backcolor,1,blink);
      move(py,px);

      switch(levelmap[x][y][z].special)
      {
         case SPECIAL_LAB_COSMETICS_CAGEDANIMALS:addstr("CAGES");break;
         case SPECIAL_NUCLEAR_ONOFF:addstr("POWER");break;
         case SPECIAL_LAB_GENETIC_CAGEDANIMALS:addstr("CAGES");break;
         case SPECIAL_POLICESTATION_LOCKUP:addstr("CELLS");break;
         case SPECIAL_COURTHOUSE_LOCKUP:addstr("CELLS");break;
         case SPECIAL_COURTHOUSE_JURYROOM:addstr("JURY!");break;
         case SPECIAL_PRISON_CONTROL:addstr("CTROL");break;
         case SPECIAL_INTEL_SUPERCOMPUTER:addstr("INTEL");break;
         case SPECIAL_SWEATSHOP_EQUIPMENT:addstr("EQUIP");break;
         case SPECIAL_POLLUTER_EQUIPMENT:addstr("EQUIP");break;
         case SPECIAL_HOUSE_PHOTOS:addstr("SAFE!");break;
         case SPECIAL_CORPORATE_FILES:addstr("SAFE!");break;
         case SPECIAL_RADIO_BROADCASTSTUDIO:move(py,px+1);addstr("MIC");break;
         case SPECIAL_NEWS_BROADCASTSTUDIO:addstr("STAGE");break;
         case SPECIAL_APARTMENT_LANDLORD:addstr("RENT?");break;
         case SPECIAL_APARTMENT_SIGN:addstr("SIGN!");break;
         case SPECIAL_STAIRS_UP:move(py,px+1);addstr("UP");
                                addch(CH_UPWARDS_ARROW);break;
         case SPECIAL_STAIRS_DOWN:move(py,px+1);addstr("DN");
                                  addch(CH_DOWNWARDS_ARROW);break;
         case SPECIAL_RESTAURANT_TABLE:addstr("TABLE");break;
         case SPECIAL_CAFE_COMPUTER:move(py,px+1);addstr("CPU");break;
         case SPECIAL_PARK_BENCH:addstr("BENCH");break;
      }
   }
   if(levelmap[x][y][z].siegeflag & SIEGEFLAG_HEAVYUNIT)
   {
      set_color(COLOR_RED,backcolor,1,blink);
      move(py+2,px);
      addstr("ARMOR");
   }
   else if(levelmap[x][y][z].siegeflag & SIEGEFLAG_UNIT)
   {
      set_color(COLOR_RED,backcolor,1,blink);
      move(py+2,px);
      addstr("ENEMY");
   }
}

/* prints the names of creatures you see */
void printencounter(void)
{
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   for(int i=19;i<=24;i++)
   {
      move(i,1);
      addstr("                                                    ");
   }

   int px=1,py=19;
   for(int e=0;e<ENCMAX;e++)
   {
      if(encounter[e].exists)
      {
         if(!encounter[e].alive)set_color(COLOR_BLACK,COLOR_BLACK,1);
         else if(encounter[e].align==0)set_color(COLOR_WHITE,COLOR_BLACK,1);
         else if(encounter[e].align==1)
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
         }
         else set_color(COLOR_RED,COLOR_BLACK,1);
         move(py,px);
         addstr(encounter[e].name);
      }

      px+=18;
      if(px>37)
      {
         px=1;
         py++;
      }
   }
}



/* prints the names of creatures you see in car chases */
void printchaseencounter(void)
{
   if(chaseseq.enemycar.size()>0)
   {
      for(int i=19;i<=24;i++)
      {
         move(i,1);
         addstr("                                                  ");
      }

      int carsy[4]={20,20,20,20};

      char str[80];
      for(int v=0;v<chaseseq.enemycar.size();v++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(19,v*20+1);
         getcarfull(str,*chaseseq.enemycar[v],1);
         addstr(str);
      }

      for(int e=0;e<ENCMAX;e++)
      {
         if(encounter[e].exists)
         {
            for(int v=0;v<chaseseq.enemycar.size();v++)
            {
               if(chaseseq.enemycar[v]->id==encounter[e].carid)
               {
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  move(carsy[v],v*20+1);
                  addstr(encounter[e].name);
                  if(encounter[e].is_driver)addstr("-D");
                  carsy[v]++;
               }
            }
         }
      }
   }
   else printencounter();
}



/* blanks a part of the screen */
void clearcommandarea(void)
{
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   for(int y=9;y<16;y++)
   {
      for(int x=0;x<53;x++)
      {
         move(y,x);
         addch(' ');
      }
   }

   if(mode!=GAMEMODE_SITE)
   {
      clearmaparea(false,true);
   }
}


void refreshmaparea(void)
{
   if(mode==GAMEMODE_SITE)
   {
      printsitemap(locx,locy,locz);
   }
   else
   {
      clearmaparea(true,false);
   }
}


void clearmessagearea(bool redrawmaparea)
{
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   addstr("                                                    ");
   move(17,1);
   addstr("                                                    ");

   if(redrawmaparea)refreshmaparea();
}



void clearmaparea(bool lower,bool upper)
{
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   for(int y=9;y<24;y++)
   {
      if(!upper&&y<15)continue;
      if(!lower&&y>=15)continue;
      move(y,53);
      addstr("                          ");
   }
}
