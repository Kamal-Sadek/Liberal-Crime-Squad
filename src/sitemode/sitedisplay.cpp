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

// Note: this file is encoded in the PC-8 / Code Page 437 / OEM-US character set
// (The same character set used by Liberal Crime Squad when it is running)
// Certain special characters won't display correctly unless your text editor is
// set to use that character set, such as this e with an accent: ‚

// In Windows Notepad with the Terminal font, OEM/DOS encoding it should work fine.
// You can set this in Notepad by going to Format->Font and choosing the Terminal font,
// then choosing OEM/DOS in the Script dropdown box.

// In Notepad++ go to the Encoding menu, Character sets, Western European, OEM-US... easy!

// In Code::Blocks's editor go to Settings->Editor->the Other Settings tab and
// then pick WINDOWS-437 from the dropdown box and then choose the radio button
// to make this the default encoding and disable auto-detection of the encoding.
// Then close the file and reopen it (since Code::Blocks detects the encoding
// when it opens the file and it can't be changed after that; what we changed was
// how it detects encoding for files it opens in the future, not files already open).

// In Microsoft Visual C++, right-click the file in the Solution Explorer,
// select "Open With...", choose "C++ Source Code Editor (with encoding)",
// then choose "OEM United States - Codepage 437".

// In MS-DOS Editor (included with Windows as EDIT.COM in your system32 directory),
// the codepage will be correct already since it's running in a console window just
// like Liberal Crime Squad. Well OK, the encoding might be wrong, but then it's wrong
// in Liberal Crime Squad TOO, and to fix it, go to Control Panel, Regional and Language Settings,
// Advanced tab, and choose English (United States) from the dropdown box as the encoding
// for non-Unicode applications, then press OK.

// If you have a Linux or other UNIX-based system you are obviously smart enough
// to figure out for yourself how to open a file in OEM-US PC-8 codepage 437 in
// your favorite text editor. If you're on Mac OS X, well that's UNIX-based, figure
// it out for yourself.

#include <externs.h>

// Imperfect but quick and dirty line of sight check
// Only works if the target point is at most two spaces
// away in any direction
bool LineOfSight(int x, int y, int z)
{
   if(levelmap[x][y][z].flag&SITEBLOCK_KNOWN) return true; // already explored

   if(DIFF(x,locx)>2||DIFF(y,locy)>2) return false; // too far away

   if(DIFF(x,locx)<=1&&DIFF(y,locy)<=1) return true; // right next to us or right where we're standing

   int x1,x2,y1,y2;

   if(DIFF(x,locx)==1) x1=locx,x2=x;
   else x1=x2=(x+locx)/2; // difference is either 0 or 2

   if(DIFF(y,locy)==1) y1=locy,y2=y;
   else y1=y2=(y+locy)/2; // difference is either 0 or 2

   // Check for obstructions
   if((levelmap[x1][y2][z].flag&(SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT))&&
      (levelmap[x2][y1][z].flag&(SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT)))
      return false; // Blocked on some axis

   return true;
}

// Prints the map graphics in the bottom right of the screen
void printsitemap(int x,int y,int z)
{
   mapshowing=true;
   int xscreen,xsite,yscreen,ysite;

   // Build the frame
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   mvaddstr(8,53,"ÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂ"); // 27 characters - top of map
   mvaddstr(24,53,"ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ"); // 27 characters - bottom of map
   for(yscreen=9;yscreen<24;yscreen++)
      mvaddstr(yscreen,53,"³                         ³"); // 27 characters - the map itself

   // Do a preliminary Line of Sight iteration for better Line of Sight detection
   for(xsite=x-2;xsite<x+3;xsite++)
      for(ysite=y-2;ysite<y+3;ysite++)
         if(LineOfSight(xsite,ysite,z))
            levelmap[xsite][ysite][z].flag |= SITEBLOCK_KNOWN;

   // Display the map
   for(xsite=x-2,xscreen=79-5*5;xsite<x+3;xscreen+=5,xsite++)
      for(ysite=y-2,yscreen=24-3*5;ysite<y+3;yscreen+=3,ysite++)
         printblock(xsite,ysite,z,xscreen,yscreen);


   //PRINT SPECIAL
   char str[200];
   switch(levelmap[locx][locy][locz].special)
   {
      case SPECIAL_LAB_COSMETICS_CAGEDANIMALS:strcpy(str,"Caged Animals");break;
      case SPECIAL_NUCLEAR_ONOFF:strcpy(str,"Reactor Control Room");break;
      case SPECIAL_LAB_GENETIC_CAGEDANIMALS:strcpy(str,"Caged \"Animals\"");break;
      case SPECIAL_POLICESTATION_LOCKUP:strcpy(str,"Police Detention Room");break;
      case SPECIAL_COURTHOUSE_LOCKUP:strcpy(str,"Courthouse Jail");break;
      case SPECIAL_COURTHOUSE_JURYROOM:strcpy(str,"Jury Room");break;
      case SPECIAL_PRISON_CONTROL:
      case SPECIAL_PRISON_CONTROL_LOW:
      case SPECIAL_PRISON_CONTROL_MEDIUM:
      case SPECIAL_PRISON_CONTROL_HIGH:strcpy(str,"Prison Control Room");break;
      case SPECIAL_INTEL_SUPERCOMPUTER:strcpy(str,"Supercomputer");break;
      case SPECIAL_SWEATSHOP_EQUIPMENT:strcpy(str,"Textile Equipment");break;
      case SPECIAL_POLLUTER_EQUIPMENT:strcpy(str,"Factory Equipment");break;
      case SPECIAL_ARMORY:strcpy(str,"Armory");break;
      case SPECIAL_HOUSE_CEO:strcpy(str,"CEO's Study");break;
      case SPECIAL_HOUSE_PHOTOS:
      case SPECIAL_CORPORATE_FILES:strcpy(str,"Safe");break;
      case SPECIAL_RADIO_BROADCASTSTUDIO:strcpy(str,"Radio Broadcast Room");break;
      case SPECIAL_NEWS_BROADCASTSTUDIO:strcpy(str,"News Broadcast Studio");break;
      case SPECIAL_APARTMENT_LANDLORD:strcpy(str,"Landlord's Office");break;
      case SPECIAL_SIGN_ONE:
      case SPECIAL_SIGN_TWO:
      case SPECIAL_SIGN_THREE:strcpy(str,"Sign");break;
      case SPECIAL_DISPLAY_CASE:strcpy(str,"Display Case");break;
      case SPECIAL_STAIRS_UP:strcpy(str,"Stairs Up");break;
      case SPECIAL_STAIRS_DOWN:strcpy(str,"Stairs Down");break;
      case SPECIAL_RESTAURANT_TABLE:strcpy(str,"Table");break;
      case SPECIAL_CAFE_COMPUTER:strcpy(str,"Computer");break;
      case SPECIAL_PARK_BENCH:strcpy(str,"Bench");break;
      case SPECIAL_BANK_VAULT:strcpy(str,"Bank Vault");break;
      case SPECIAL_BANK_TELLER:strcpy(str,"Bank Teller");break;
      case SPECIAL_BANK_MONEY:strcpy(str,"Oh Wow So Much Money");break;
      case SPECIAL_CCS_BOSS:strcpy(str,"CCS Boss");break;
      case SPECIAL_OVAL_OFFICE_NW:
      case SPECIAL_OVAL_OFFICE_NE:
      case SPECIAL_OVAL_OFFICE_SW:
      case SPECIAL_OVAL_OFFICE_SE:strcpy(str,"The Office of the President");break;
      default:strcpy(str,"");break;
   }
   if(levelmap[locx][locy][locz].special!=-1)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      mvaddstr(24,67-(len(str)>>1),str);
   }

   //PRINT PARTY
   int partyalive=0;
   for(int p=0;p<6;p++)
      if(activesquad->squad[p]) if(activesquad->squad[p]->alive==1) partyalive++;
   if(partyalive>0) set_color(COLOR_GREEN,COLOR_BLACK,1);
   else set_color(COLOR_BLACK,COLOR_BLACK,1);
   mvaddstr(16,64,"SQUAD");

   int encsize=0;
   for(int e=0;e<ENCMAX;e++) if(encounter[e].exists) encsize++;
   //PRINT ANY OPPOSING FORCE INFO
   if(encsize)
   {
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      if(levelmap[locx][locx][locz].siegeflag & SIEGEFLAG_HEAVYUNIT)
         mvaddstr(17,64,"ARMOR");
      else if(levelmap[locx][locx][locz].siegeflag & SIEGEFLAG_UNIT)
         mvaddstr(17,64,"ENEMY");
      else if(levelmap[locx][locx][locz].siegeflag & SIEGEFLAG_UNIT_DAMAGED)
         mvaddstr(17,64,"enemy");
      else mvaddstr(17,64,"ENCTR");

      printencounter();
   }

   if(len(groundloot))
   {
      set_color(COLOR_MAGENTA,COLOR_BLACK,1);
      mvaddstr(15,64,"LOOT!");

      printencounter();
   }
}

#define WALL_UP    0
#define WALL_DOWN  1
#define WALL_LEFT  2
#define WALL_RIGHT 3
#define CORNER_UL  4
#define CORNER_UR  5
#define CORNER_DL  6
#define CORNER_DR  7

void printwall(int x, int y, int z, int px, int py)
{
   bool visible[8]     = {false,false,false,false,false,false,false,false};
   bool bloody[8]      = {false,false,false,false,false,false,false,false};
   char graffiti[4][4] = {"   ","   ","   ","   "};
   char graffiticolor[4] = {COLOR_BLACK,COLOR_BLACK,COLOR_BLACK,COLOR_BLACK};

   int type = levelmap[x][y][z].flag; // What are we drawing here? Wall/door? Locked/jammed? Metal/normal?

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
   if(x>locx && x<MAPX) visible[WALL_LEFT] = true;
   if(x>0 && x<locx) visible[WALL_RIGHT] = true;
   if(y>locy && y<MAPY) visible[WALL_UP] = true;
   if(y>0 && y<locy) visible[WALL_DOWN] = true;

   if(y>locy && y<MAPY && x>locx && x<MAPX) visible[CORNER_UL] = true;
   if(y>locy && y<MAPY && x>0 && x<locx) visible[CORNER_UR] = true;
   if(y>0 && y<locy && x>locx && x<MAPX) visible[CORNER_DL] = true;
   if(y>0 && y<locy && x>0 && x<locx) visible[CORNER_DR] = true;

   // 2) Check LOS
   if(!LineOfSight(x-1,y,z)) visible[WALL_LEFT] = false;
   if(!LineOfSight(x+1,y,z)) visible[WALL_RIGHT] = false;
   if(!LineOfSight(x,y-1,z)) visible[WALL_UP] = false;
   if(!LineOfSight(x,y+1,z)) visible[WALL_DOWN] = false;

   if(!LineOfSight(x-1,y-1,z)) visible[CORNER_UL] = false;
   if(!LineOfSight(x+1,y-1,z)) visible[CORNER_UR] = false;
   if(!LineOfSight(x-1,y+1,z)) visible[CORNER_DL] = false;
   if(!LineOfSight(x+1,y+1,z)) visible[CORNER_DR] = false;

   // 3) Check for memory
   if(levelmap[x-1][y][z].flag & SITEBLOCK_KNOWN) visible[WALL_LEFT] = true;
   if(levelmap[x+1][y][z].flag & SITEBLOCK_KNOWN) visible[WALL_RIGHT] = true;
   if(levelmap[x][y-1][z].flag & SITEBLOCK_KNOWN) visible[WALL_UP] = true;
   if(levelmap[x][y+1][z].flag & SITEBLOCK_KNOWN) visible[WALL_DOWN] = true;

   if(levelmap[x-1][y-1][z].flag & SITEBLOCK_KNOWN) visible[CORNER_UL] = true;
   if(levelmap[x+1][y-1][z].flag & SITEBLOCK_KNOWN) visible[CORNER_UR] = true;
   if(levelmap[x-1][y+1][z].flag & SITEBLOCK_KNOWN) visible[CORNER_DL] = true;
   if(levelmap[x+1][y+1][z].flag & SITEBLOCK_KNOWN) visible[CORNER_DR] = true;

   // 4) Check for blockages
   if(levelmap[x-1][y][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)) visible[WALL_LEFT] = false;
   if(levelmap[x+1][y][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)) visible[WALL_RIGHT] = false;
   if(levelmap[x][y-1][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)) visible[WALL_UP] = false;
   if(levelmap[x][y+1][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)) visible[WALL_DOWN] = false;

   if(levelmap[x-1][y-1][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)) visible[CORNER_UL] = false;
   if(levelmap[x+1][y-1][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)) visible[CORNER_UR] = false;
   if(levelmap[x-1][y+1][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)) visible[CORNER_DL] = false;
   if(levelmap[x+1][y+1][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)) visible[CORNER_DR] = false;

   // Below not used for doors
   if(levelmap[x][y][z].flag & SITEBLOCK_BLOCK)
   {
      // Check for bloody walls
      if(levelmap[x-1][y][z].flag & SITEBLOCK_BLOODY2) bloody[WALL_LEFT] = true;
      if(levelmap[x+1][y][z].flag & SITEBLOCK_BLOODY2) bloody[WALL_RIGHT] = true;
      if(levelmap[x][y-1][z].flag & SITEBLOCK_BLOODY2) bloody[WALL_UP] = true;
      if(levelmap[x][y+1][z].flag & SITEBLOCK_BLOODY2) bloody[WALL_DOWN] = true;

      if(levelmap[x-1][y-1][z].flag & SITEBLOCK_BLOODY2) bloody[CORNER_UL] = true;
      if(levelmap[x+1][y-1][z].flag & SITEBLOCK_BLOODY2) bloody[CORNER_UR] = true;
      if(levelmap[x-1][y+1][z].flag & SITEBLOCK_BLOODY2) bloody[CORNER_DL] = true;
      if(levelmap[x+1][y+1][z].flag & SITEBLOCK_BLOODY2) bloody[CORNER_DR] = true;

      // Check for other graffiti
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

   for(int dir=4;dir<8;dir++)
   {
      x=px,y=py;

      // Draw the corner
      if(visible[dir] && (type & SITEBLOCK_BLOCK))
      {
         bool blink = type & SITEBLOCK_METAL;

         // Position cursor in the correct corner
         //if(dir==CORNER_UL) // Nothing to do in this case
         if(dir==CORNER_UR) x+=4;
         if(dir==CORNER_DL) y+=2;
         if(dir==CORNER_DR) y+=2,x+=4;

         // Blood overrides gray base wall color
         if(bloody[dir]) set_color(COLOR_RED,COLOR_RED,0);
         else set_color(COLOR_WHITE,COLOR_WHITE,0,blink);

         // The corner's ready to draw now
         mvaddchar(y,x,' ');
      }
   }

   for(int dir=0;dir<4;dir++)
   {
      x=px,y=py;

      // Draw the wall/door
      if(visible[dir])
      {
         if(type & SITEBLOCK_BLOCK)
         {
            bool blink = type & SITEBLOCK_METAL;

            // Position cursor at the start of where the graffiti tag would go
            //if(dir==WALL_LEFT) // Nothing to do in this case
            if(dir==WALL_RIGHT) x+=4;
            if(dir==WALL_UP) x++;
            if(dir==WALL_DOWN) y+=2,x++;

            // Blood overrides graffiti overrides gray base wall color
            if(bloody[dir])
               set_color(COLOR_RED,COLOR_RED,0);
            else if(graffiti[dir][0]!=' ')
               set_color(graffiticolor[dir],COLOR_WHITE,0,blink);
            else set_color(COLOR_WHITE,COLOR_WHITE,0,blink);

            // Draw the chunk of wall where the graffiti would/will go
            for(int gchar=0;gchar<3;gchar++)
            {
               mvaddchar(y,x,graffiti[dir][gchar]);

               if(dir==WALL_RIGHT||dir==WALL_LEFT) y++;
               else x++;
            }

            // For the long faces (top and bottom) of the wall, there is
            // additional space to either side of the 'tag' (or lack of tag)
            // that needs to be filled in with wall or blood color

            if(dir==WALL_UP||dir==WALL_DOWN)
            {
               if(bloody[dir]) set_color(COLOR_RED,COLOR_RED,0);
               else set_color(COLOR_WHITE,COLOR_WHITE,0,blink);
               if(!visible[WALL_LEFT]) mvaddchar(y,px,' ');
               if(!visible[WALL_RIGHT]) mvaddchar(y,px+4,' ');
            }
         }
         else if(type & SITEBLOCK_DOOR)
         {
            // Doors are, thankfully, much simpler, as they do not
            // support blood or graffiti

            // Position cursor at the start of face
            if(dir==WALL_DOWN) y+=2;
            if(dir==WALL_RIGHT) x+=4;

            // Pick color
            if(type & SITEBLOCK_METAL)
               set_color(COLOR_WHITE,COLOR_WHITE,1);
            else if(type & SITEBLOCK_CLOCK && type & SITEBLOCK_LOCKED)
               set_color(COLOR_RED,COLOR_BLACK,0);
            else if(type & SITEBLOCK_KLOCK && type & SITEBLOCK_LOCKED)
               set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_YELLOW,COLOR_BLACK,0);

            // Draw face
            if(dir==WALL_RIGHT||dir==WALL_LEFT)
               for(int i=0;i<3;i++) mvaddstr(y++,x,"º");
            else for(int i=0;i<5;i++) mvaddstr(y,x++,"Í");

            // Corners are possible if walls nearby are blown away, although this is rare
            if((dir==WALL_LEFT&&visible[WALL_UP])||(dir==WALL_UP&&visible[WALL_LEFT])) mvaddstr(py,px,"É");
            if((dir==WALL_RIGHT&&visible[WALL_UP])||(dir==WALL_UP&&visible[WALL_RIGHT])) mvaddstr(py,px+4,"»");
            if((dir==WALL_LEFT&&visible[WALL_DOWN])||(dir==WALL_DOWN&&visible[WALL_LEFT])) mvaddstr(py+2,px,"È");
            if((dir==WALL_RIGHT&&visible[WALL_DOWN])||(dir==WALL_DOWN&&visible[WALL_RIGHT])) mvaddstr(py+2,px+4,"¼");
         }
      }
   }
}

void printblock(int x,int y,int z,int px, int py)
{
   if(!LineOfSight(x,y,z))
   {
      set_color(COLOR_BLACK,COLOR_BLACK,0);
      for(x=px;x<px+5;x++)
         for(y=py;y<py+3;y++)
            mvaddchar(y,x,' ');
      return;
   }
   levelmap[x][y][z].flag |= SITEBLOCK_KNOWN;
   if(levelmap[x][y][z].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR))
   {
      printwall(x,y,z,px,py);
      return;
   }
   int backcolor=COLOR_BLACK;
   char blink=0,ch=' ';
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
      for(int py2=py;py2<py+3;py2++)
         mvaddchar(py2,px2,ch);

   if(levelmap[x][y][z].flag & SITEBLOCK_DEBRIS)
   {
      set_color(COLOR_WHITE,backcolor,1,blink);
      mvaddchar(py+0,px+1,'.');
      mvaddchar(py+0,px+4,'^');
      mvaddchar(py+1,px+0,'=');
      mvaddchar(py+1,px+3,'.');
      mvaddchar(py+1,px+4,'|');
      mvaddchar(py+2,px+1,'.');
      mvaddchar(py+2,px+4,'\\');
   }

   if(levelmap[x][y][z].flag & SITEBLOCK_FIRE_START)
   {
      set_color(COLOR_RED,backcolor,1,blink);
      mvaddchar(py+0,px+1,'.');
      set_color(COLOR_YELLOW,backcolor,1,blink);
      mvaddchar(py+1,px+3,'.');
      mvaddchar(py+2,px+1,'.');
   }

   if(levelmap[x][y][z].flag & SITEBLOCK_FIRE_PEAK)
   {
      set_color(COLOR_RED,backcolor,1,blink);
      mvaddchar(py+0,px+1,':');
      mvaddchar(py+1,px+0,'*');
      set_color(COLOR_YELLOW,backcolor,1,blink);
      mvaddchar(py+0,px+4,'$');
      mvaddchar(py+1,px+3,':');
      mvaddchar(py+1,px+4,'%');
      mvaddchar(py+2,px+1,':');
      mvaddchar(py+2,px+4,'*');
   }

   if(levelmap[x][y][z].flag & SITEBLOCK_FIRE_END)
   {
      set_color(COLOR_RED,backcolor,1,blink);
      mvaddchar(py+1,px+0,'*');
      set_color(COLOR_YELLOW,backcolor,1,blink);
      mvaddchar(py+0,px+4,'~');
      mvaddchar(py+2,px+4,'#');
      set_color(COLOR_WHITE,backcolor,1,blink);
      mvaddchar(py+0,px+1,'.');
      mvaddchar(py+1,px+3,'.');
      mvaddchar(py+1,px+4,'|');
      mvaddchar(py+2,px+1,'.');
   }

   if(levelmap[x][y][z].flag & SITEBLOCK_BLOODY2)
   {
      set_color(COLOR_RED,backcolor,0,blink);
      mvaddchar(py+1,px+1,'%');
      mvaddchar(py+2,px+1,'.');
      mvaddchar(py+1,px+2,'.');
   }
   else if(levelmap[x][y][z].flag & SITEBLOCK_BLOODY)
   {
      set_color(COLOR_RED,backcolor,0,blink);
      mvaddchar(py+2,px+1,'.');
      mvaddchar(py+1,px+2,'.');
   }

   if(levelmap[x][y][z].flag & SITEBLOCK_EXIT)
   {
      set_color(COLOR_WHITE,backcolor,0,blink);
      mvaddstr(py+1,px+1,"EXT");
   }
   else if(levelmap[x][y][z].flag & SITEBLOCK_LOOT)
   {
      set_color(COLOR_MAGENTA,backcolor,1,blink);
      mvaddstr(py,px+1,"~$~");
   }

   if(levelmap[x][y][z].siegeflag & SIEGEFLAG_TRAP)
   {
      set_color(COLOR_YELLOW,backcolor,1,blink);
      mvaddstr(py+1,px,"TRAP!");
   }
   else if(levelmap[x][y][z].siegeflag & SIEGEFLAG_UNIT_DAMAGED)
   {
      set_color(COLOR_RED,backcolor,0,blink);
      mvaddstr(py+2,px,"enemy");
   }
   else if(levelmap[x][y][z].special!=-1)
   {
      set_color(COLOR_YELLOW,backcolor,1,blink);

      switch(levelmap[x][y][z].special)
      {
         case SPECIAL_NUCLEAR_ONOFF:mvaddstr(py,px,"POWER");break;
         case SPECIAL_LAB_COSMETICS_CAGEDANIMALS:
         case SPECIAL_LAB_GENETIC_CAGEDANIMALS:mvaddstr(py,px,"CAGES");break;
         case SPECIAL_POLICESTATION_LOCKUP:
         case SPECIAL_COURTHOUSE_LOCKUP:mvaddstr(py,px,"CELLS");break;
         case SPECIAL_COURTHOUSE_JURYROOM:mvaddstr(py,px,"JURY!");break;
         case SPECIAL_PRISON_CONTROL:
         case SPECIAL_PRISON_CONTROL_LOW:
         case SPECIAL_PRISON_CONTROL_MEDIUM:
         case SPECIAL_PRISON_CONTROL_HIGH:mvaddstr(py,px,"CTROL");break;
         case SPECIAL_INTEL_SUPERCOMPUTER:mvaddstr(py,px,"INTEL");break;
         case SPECIAL_SWEATSHOP_EQUIPMENT:
         case SPECIAL_POLLUTER_EQUIPMENT:mvaddstr(py,px,"EQUIP");break;
         case SPECIAL_ARMORY:mvaddstr(py,px,"ARMRY");break;
         case SPECIAL_HOUSE_CEO:mvaddstr(py,px+1,"CEO");break;
         case SPECIAL_HOUSE_PHOTOS:
         case SPECIAL_CORPORATE_FILES:mvaddstr(py,px,"SAFE!");break;
         case SPECIAL_RADIO_BROADCASTSTUDIO:mvaddstr(py,px+1,"MIC");break;
         case SPECIAL_NEWS_BROADCASTSTUDIO:mvaddstr(py,px,"STAGE");break;
         case SPECIAL_APARTMENT_LANDLORD:mvaddstr(py,px,"RENT?");break;
         case SPECIAL_SIGN_ONE:
         case SPECIAL_SIGN_TWO:
         case SPECIAL_SIGN_THREE:mvaddstr(py,px,"SIGN!");break;
         case SPECIAL_STAIRS_UP:mvaddstr(py,px+1,"UP\x18");break;
         case SPECIAL_STAIRS_DOWN:mvaddstr(py,px+1,"DN\x19");break;
         case SPECIAL_RESTAURANT_TABLE:mvaddstr(py,px,"TABLE");break;
         case SPECIAL_CAFE_COMPUTER:mvaddstr(py,px+1,"CPU");break;
         case SPECIAL_PARK_BENCH:mvaddstr(py,px,"BENCH");break;
         case SPECIAL_SECURITY_METALDETECTORS:mvaddstr(py,px,"METAL");break;
         case SPECIAL_SECURITY_CHECKPOINT:mvaddstr(py,px,"GUARD");break;
         case SPECIAL_DISPLAY_CASE:mvaddstr(py,px,"CASE");break;
         case SPECIAL_BANK_VAULT:mvaddstr(py,px,"VAULT");break;
         case SPECIAL_BANK_TELLER:mvaddstr(py,px,"TELER");break;
         case SPECIAL_BANK_MONEY:mvaddstr(py,px,"MONEY");break;
         case SPECIAL_CCS_BOSS:mvaddstr(py,px,"BOSS!");break;
         case SPECIAL_OVAL_OFFICE_NW:mvaddstr(py,px+3,"OV");break;
         case SPECIAL_OVAL_OFFICE_NE:mvaddstr(py,px,"AL");break;
         case SPECIAL_OVAL_OFFICE_SW:mvaddstr(py,px+2,"OFF");break;
         case SPECIAL_OVAL_OFFICE_SE:mvaddstr(py,px,"ICE");break;
      }
   }
   if(levelmap[x][y][z].siegeflag & SIEGEFLAG_HEAVYUNIT)
   {
      set_color(COLOR_RED,backcolor,1,blink);
      mvaddstr(py+2,px,"ARMOR");
   }
   else if(levelmap[x][y][z].siegeflag & SIEGEFLAG_UNIT)
   {
      set_color(COLOR_RED,backcolor,1,blink);
      mvaddstr(py+2,px,"ENEMY");
   }
}

/* prints the names of creatures you see */
void printencounter()
{
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   for(int i=19;i<=24;i++)
      mvaddstr(i,0,"                                                     "); // 53 spaces

   int px=1,py=19;
   for(int e=0;e<ENCMAX;e++)
   {
      if(encounter[e].exists)
      {
         if(!encounter[e].alive) set_color(COLOR_BLACK,COLOR_BLACK,1);
         else if(encounter[e].align==0) set_color(COLOR_WHITE,COLOR_BLACK,1);
         else if(encounter[e].align==1) set_color(COLOR_GREEN,COLOR_BLACK,1);
         else set_color(COLOR_RED,COLOR_BLACK,1);
         mvaddstr(py,px,encounter[e].name);
      }

      px+=18;
      if(px>37) px=1,py++;
   }
}



/* prints the names of creatures you see in car chases */
void printchaseencounter()
{
   for(int i=19;i<=24;i++)
      mvaddstr(i,0,"                                                                                "); // 80 spaces

   if(len(chaseseq.enemycar))
   {
      int carsy[4]={20,20,20,20};

      for(int v=0;v<len(chaseseq.enemycar);v++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         mvaddstr(19,v*20+1,chaseseq.enemycar[v]->fullname(true));
      }

      for(int e=0;e<ENCMAX;e++) if(encounter[e].exists)
         for(int v=0;v<len(chaseseq.enemycar);v++)
            if(chaseseq.enemycar[v]->id()==encounter[e].carid)
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               mvaddstr(carsy[v],v*20+1,encounter[e].name);
               if(encounter[e].is_driver) addstr("-D");
               carsy[v]++;
            }
   }
   else printencounter();
}



/* blanks a part of the screen */
void clearcommandarea()
{
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   for(int y=9;y<16;y++)
      mvaddstr(y,0,"                                                     "); // 53 spaces
   if(mode!=GAMEMODE_SITE) clearmaparea(false,true);
}


void refreshmaparea()
{
   if(mode==GAMEMODE_SITE) printsitemap(locx,locy,locz);
   else clearmaparea(true,false);
}


void clearmessagearea(bool redrawmaparea)
{
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   if(redrawmaparea)
   {
      mvaddstr(16,0,"                                                                                "); // 80 spaces
      mvaddstr(17,0,"                                                                                "); // 80 spaces
      refreshmaparea();
      // Must reprint chasers when no map to the right
      if(mode==GAMEMODE_CHASECAR||mode==GAMEMODE_CHASEFOOT) printchaseencounter();
   }
   else
   {
      mvaddstr(16,0,"                                                     "); // 53 spaces
      mvaddstr(17,0,"                                                     "); // 53 spaces
   }
}



void clearmaparea(bool lower,bool upper)
{
   if(upper) mapshowing=false;
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   for(int y=8;y<25;y++)
   {
      if(!upper&&y<15)continue;
      if(!lower&&y>=15)continue;
      if(y==8) mvaddstr(y,53,"ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");  // 27 characters
      else mvaddstr(y,53,"                           ");  // 27 spaces
   }
}
