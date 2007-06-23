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

/* base - activate the uninvolved */
void activate(void)
{
   vector<creaturest *> temppool;
   int sq;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->alive&&
         pool[p]->align==1&&
         pool[p]->clinic==0&&
         pool[p]->dating==0&&
         pool[p]->hiding==0&&
         !(pool[p]->flag & CREATUREFLAG_SLEEPER))
      {
         if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
            location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
            location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON)
         {
            if(pool[p]->squadid!=-1)
            {
               sq=getsquad(pool[p]->squadid);
               if(sq!=-1)
               {
                  if(squad[sq]->activity.type!=ACTIVITY_NONE)continue;
               }
            }
            temppool.push_back(pool[p]);
         }
      }
   }

   if(temppool.size()==0)return;

   short page=0;

   char str[80];
   char num[20];

   do
   {
      erase();

      set_color(COLOR_GREEN,COLOR_BLACK,1);
      itoa(funds,num,10);
      move(0,78-strlen(num));
      addstr("$");
      addstr(num);

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Activate Uninvolved Liberals");
      move(1,0);
      addstr("----CODE NAME------------SKILL---HEALTH---LOCATION------------------------------");
      move(1,57);
      addstr("ACTIVITY");

      int y=2;
      for(int p=page*19;p<temppool.size()&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         char bright=0;
         unsigned long skill=0;
         for(int sk=0;sk<SKILLNUM;sk++)
         {
            skill+=(unsigned long)temppool[p]->skill[sk];
            if(temppool[p]->skill_ip[sk]>=100*((10+temppool[p]->skill[sk])/10)&&
               temppool[p]->skill[sk]<temppool[p]->attval(skillatt(sk))*2)bright=1;
         }

         set_color(COLOR_WHITE,COLOR_BLACK,bright);

         move(y,25);
         itoa(skill,num,10);
         addstr(num);

         printhealthstat(*temppool[p],y,33,TRUE);

         if(mode==REVIEWMODE_JUSTICE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,42);
         addstr(location[temppool[p]->location]->shortname);

         move(y,57);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         getactivity(str,temppool[p]->activity);
         addstr(str);

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to Assign an Activity.");
      move(23,0);
      if(interface_pgup=='[')
      {
         addstr("[] to view other Liberal pages.");
      }
      else if(interface_pgup=='.')
      {
         addstr("; and : to view other Liberal pages.");
      }
      else
      {
         addstr("PGUP/PGDN to view other Liberal pages.");
      }
      move(24,0);
      addstr("Press Z to assign simple tasks in bulk.");

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if(c==interface_pgup&&page>0)page--;
      //PAGE DOWN
      if(c==interface_pgdn&&(page+1)*19<temppool.size())page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<temppool.size())
         {
            activate(temppool[p]);
         }
      }

      if(c=='z')
      {
         activatebulk();
      }

      if(c==10)break;
   }while(1);
}


void activate(creaturest *cr)
{
   int hostagecount=0;
   char havedead=0;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->alive&&pool[p]->align!=1)hostagecount++;
      if(!pool[p]->alive)havedead=1;
   }

   char num[20];

   do
   {
      erase();

      set_color(COLOR_GREEN,COLOR_BLACK,1);
      itoa(funds,num,10);
      move(0,78-strlen(num));
      addstr("$");
      addstr(num);

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Taking Action:   What will ");
      addstr(cr->name);
      addstr(" be doing today?");

      printcreatureinfo(cr);

      makedelimiter(8,0);

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(10,1);
      addstr("A - Perpetrating random acts of Liberal Disobedience.");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(11,1);
      addstr("D - Soliciting Donations.");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(11,40);
      addstr("B - Selling Brownies.");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(12,1);
      addstr("C - Making Clothing.");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(12,40);
      addstr("R - Repairing Clothing.");

      if(hostagecount>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(13,1);
      addstr("H - Tending to a Conservative hostage.");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(13,40);
      addstr("P - Surfing the Net for opinion polls.");

      move(14,1);
      if(cr->canwalk())
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr("S - Stealing a Car.");
      }
      else
      {
         if(!(cr->flag & CREATUREFLAG_WHEELCHAIR))set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         addstr("S - Procuring a Wheelchair.");
      }

      if(location[cr->location]->compound_walls==COMPOUND_PRINTINGPRESS)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(14,40);
      addstr("W - Writing for the Liberal Guardian.");      

      if(clinictime(*cr))set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,1);
      addstr("M - Move to the Free CLINIC.");

      if(havedead)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,40);
      addstr("Z - Dispose of bodies.");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(17,1);
      addstr("Enter - Nothing for now.");

      refresh();
      int c=getch();
      translategetch(c);

      if(c=='a')
      {
         long flevel=select_troublefundinglevel(cr);

         if(flevel>=0)
         {
            cr->activity.type=ACTIVITY_TROUBLE;
            cr->activity.arg=flevel;
            return;
         }
      }
      if(c=='d')
      {
         cr->activity.type=ACTIVITY_FUNDS_LEGAL;
         break;
      }
      if(c=='b')
      {
         cr->activity.type=ACTIVITY_FUNDS_ILLEGAL;
         break;
      }
      if(c=='h'&&hostagecount>0)
      {
         activityst oact=cr->activity;
         cr->activity.type=ACTIVITY_NONE;
         select_tendhostage(cr);
         if(cr->activity.type==ACTIVITY_HOSTAGETENDING)break;
         else cr->activity=oact;
      }
      if(c=='c')
      {
         activityst oact=cr->activity;
         cr->activity.type=ACTIVITY_NONE;
         select_makeclothing(cr);
         if(cr->activity.type==ACTIVITY_MAKE_ARMOR)break;
         else cr->activity=oact;
      }
      if(c=='r')
      {
         cr->activity.type=ACTIVITY_REPAIR_ARMOR;
         break;
      }
      if(c=='p')
      {
         cr->activity.type=ACTIVITY_POLLS;
         break;
      }
      if(c=='s')
      {
         if(cr->canwalk())
         {
            cr->activity.type=ACTIVITY_STEALCARS;
            break;
         }
         else if(!(cr->flag & CREATUREFLAG_WHEELCHAIR))
         {
            cr->activity.type=ACTIVITY_WHEELCHAIR;
            break;
         }
      }
      if(c=='w'&&location[cr->location]->compound_walls==COMPOUND_PRINTINGPRESS)
      {
         activityst oact=cr->activity;
         cr->activity.type=ACTIVITY_NONE;
         if(select_view(cr,cr->activity.arg))
            cr->activity.type=ACTIVITY_WRITE;
         else cr->activity=oact;
         break;
      }
      if(c=='m'&&clinictime(*cr))
      {
         cr->activity.type=ACTIVITY_CLINIC;
         break;
      }
      if(c=='z'&&havedead)
      {
         cr->activity.type=ACTIVITY_BURY;
         break;
      }

      if(c==10)
      {
         cr->activity.type=ACTIVITY_NONE;
         break;
      }
   }while(1);
}


void activatebulk(void)
{
   vector<creaturest *> temppool;
   int sq;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->alive&&
         pool[p]->align==1&&
         pool[p]->clinic==0&&
         pool[p]->dating==0&&
         pool[p]->hiding==0&&
         !(pool[p]->flag & CREATUREFLAG_SLEEPER))
      {
         if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
            location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
            location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON)
         {
            if(pool[p]->squadid!=-1)
            {
               sq=getsquad(pool[p]->squadid);
               if(sq!=-1)
               {
                  if(squad[sq]->activity.type!=ACTIVITY_NONE)continue;
               }
            }
            temppool.push_back(pool[p]);
         }
      }
   }

   if(temppool.size()==0)return;

   short page=0;

   char str[80];
   char num[20];

   int selectedactivity=0;

   do
   {
      erase();

      set_color(COLOR_GREEN,COLOR_BLACK,1);
      itoa(funds,num,10);
      move(0,78-strlen(num));
      addstr("$");
      addstr(num);

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Activate Uninvolved Liberals");
      move(1,0);
      addstr("----CODE NAME------------CURRENT ACTIVITY---------------------------------------");
      move(1,51);
      addstr("BULK ACTIVITY");

      if(selectedactivity==0)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(2,51);
      addstr("1 - Causing Trouble ($0).");
      if(selectedactivity==1)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(3,51);
      addstr("2 - Causing Trouble ($20).");
      if(selectedactivity==2)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(4,51);
      addstr("3 - Causing Trouble ($50).");
      if(selectedactivity==3)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(5,51);
      addstr("4 - Causing Trouble ($100).");
      if(selectedactivity==4)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(6,51);
      addstr("5 - Causing Trouble ($500).");
      if(selectedactivity==5)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(7,51);
      addstr("6 - Soliciting Donations.");
      if(selectedactivity==6)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(8,51);
      addstr("7 - Selling Brownies.");
      if(selectedactivity==7)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(9,51);
      addstr("8 - Stealing Cars.");

      int y=2;
      for(int p=page*19;p<temppool.size()&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         move(y,25);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         getactivity(str,temppool[p]->activity);
         addstr(str);
         if(temppool[p]->activity.type==ACTIVITY_TROUBLE)
         {
            addstr(" ($");
            itoa(temppool[p]->activity.arg,num,10);
            addstr(num);
            addstr(")");
         }

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to Assign an Activity.  Press a Number to select an Activity.");
      move(23,0);
      if(interface_pgup=='[')
      {
         addstr("[] to view other Liberal pages.");
      }
      
         else if(interface_pgup=='.')
         {
            addstr("; and : to view other Liberal pages");
         }
      else
      {
         addstr("PGUP/PGDN to view other Liberal pages.");
      }

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if(c==interface_pgup&&page>0)page--;
      //PAGE DOWN
      if(c==interface_pgdn&&(page+1)*19<temppool.size())page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<temppool.size())
         {
            switch(selectedactivity)
            {
               case 0:
                  temppool[p]->activity.type=ACTIVITY_TROUBLE;
                  temppool[p]->activity.arg=0;
                  break;
               case 1:
                  temppool[p]->activity.type=ACTIVITY_TROUBLE;
                  temppool[p]->activity.arg=20;
                  break;
               case 2:
                  temppool[p]->activity.type=ACTIVITY_TROUBLE;
                  temppool[p]->activity.arg=50;
                  break;
               case 3:
                  temppool[p]->activity.type=ACTIVITY_TROUBLE;
                  temppool[p]->activity.arg=100;
                  break;
               case 4:
                  temppool[p]->activity.type=ACTIVITY_TROUBLE;
                  temppool[p]->activity.arg=500;
                  break;
               case 5:
                  temppool[p]->activity.type=ACTIVITY_FUNDS_LEGAL;
                  break;
               case 6:
                  temppool[p]->activity.type=ACTIVITY_FUNDS_ILLEGAL;
                  break;
               case 7:
                  temppool[p]->activity.type=ACTIVITY_STEALCARS;
                  break;
            }
         }
      }
      if(c>='1'&&c<='8')
      {
         selectedactivity=c-'1';
      }

      if(c==10)break;
   }while(1);
}


/* base - activate - hostages */
void select_tendhostage(creaturest *cr)
{
   vector<creaturest *> temppool;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->align!=1&&
         pool[p]->alive)
      {
         temppool.push_back(pool[p]);
      }
   }

   if(temppool.size()==0)return;

   short page=0;

   char num[20];

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Which hostage will ");
      addstr(cr->name);
      addstr(" be watching over?");
      move(1,0);
      addstr("----CODE NAME------------SKILL---HEALTH---LOCATION------------------------------");
      move(1,57);
      addstr("DAYS IN CAPTIVITY");

      int y=2;
      for(int p=page*19;p<temppool.size()&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         char bright=0;
         unsigned long skill=0;
         for(int sk=0;sk<SKILLNUM;sk++)
         {
            skill+=(unsigned long)temppool[p]->skill[sk];
            if(temppool[p]->skill_ip[sk]>=100*((10+temppool[p]->skill[sk])/10)&&
               temppool[p]->skill[sk]<temppool[p]->attval(skillatt(sk))*2)bright=1;
         }

         set_color(COLOR_WHITE,COLOR_BLACK,bright);

         move(y,25);
         itoa(skill,num,10);
         addstr(num);

         printhealthstat(*temppool[p],y,33,TRUE);

         if(mode==REVIEWMODE_JUSTICE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,42);
         addstr(location[temppool[p]->location]->shortname);

         move(y,57);
         set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         char num[20];
         itoa(temppool[p]->joindays,num,10);
         addstr(num);
         addstr(" ");
         if(temppool[p]->joindays>1)addstr("Days");
         else addstr("Day");

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Conservative");
      move(23,0);
      if(interface_pgup=='[')
      {
         addstr("[] to view other Liberal pages.");
      }
         else if(interface_pgup=='.')
         {
            addstr("; and : to view other Liberal pages.");
         }
      else
      {
         addstr("PGUP/PGDN to view other Liberal pages.");
      }

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if(c==interface_pgup&&page>0)page--;
      //PAGE DOWN
      if(c==interface_pgdn&&(page+1)*19<temppool.size())page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<temppool.size())
         {
            long flevel=select_hostagefundinglevel(cr,temppool[p]);

            if(flevel>=0)
            {
               cr->activity.type=ACTIVITY_HOSTAGETENDING;
               cr->activity.arg=temppool[p]->id;
               cr->activity.arg2=flevel;
               return;
            }
         }
      }

      if(c==10)break;
   }while(1);
}


long select_hostagefundinglevel(creaturest *cr,creaturest *hs)
{
   long flevel=-1;

   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(0,0);
   addstr("Select a Funding Level for this Operation:");

   move(2,0);
   addstr("A - Don't spend anything.  ");
   addstr(cr->name);
   addstr(" is just on watch duty.");

   move(3,0);
   addstr("B - Don't spend anything.  ");
   addstr(cr->name);
   addstr(" will turn the prisoner over time.");

   move(4,0);
   addstr("C - $20 per day.  Enough for some props.");

   move(5,0);
   addstr("D - $50 per day.  ");
   addstr(cr->name);
   addstr(" will go for a thrifty indoctrination.");

   move(6,0);
   addstr("E - $100 per day.  ");
   addstr(cr->name);
   addstr(" needs enough freedom to be creative.");

   move(7,0);
   addstr("F - $500 per day.  It is imperative that the Conservative be turned quickly.");

   move(8,0);
   addstr("K - This Conservative has become a liability and needs to be terminated.");

   move(10,0);
   addstr("X - On second thought, this isn't a job for ");
   addstr(cr->name);
   addstr(".");

   refresh();
   int c=getch();
   translategetch(c);

   if(c=='a')flevel=0;
   if(c=='b')flevel=1;
   if(c=='c')flevel=20;
   if(c=='d')flevel=50;
   if(c=='e')flevel=100;
   if(c=='f')flevel=500;
   if(c=='k')flevel=666;

   return flevel;
}



/* base - activate - make clothing */
void select_makeclothing(creaturest *cr)
{
   vector<int> armortype;
   for(int a=0;a<ARMORNUM;a++)
   {
      switch(a)
      {
         case ARMOR_NONE:
         case ARMOR_MITHRIL:
         case ARMOR_MASK:
         case ARMOR_BALLISTICVEST:
         case ARMOR_HEAVYBALLISTICVEST:
            break;
         default:
            armortype.push_back(a);
            break;
      }
   }

   short page=0;

   char str[200];
   char num[20];

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Which will ");
      addstr(cr->name);
      addstr(" try to make?   (Note: Half Cost if you have cloth)");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(1,0);
      addstr("----NAME-----------------------------DIFFICULTY-------------COST----------------");

      int y=2,difficulty;
      for(int p=page*19;p<armortype.size()&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         getarmorfull(str,armortype[p]);
         addstr(str);

         move(y,37);
         difficulty=armor_makedifficulty(armortype[p],cr);
         switch(difficulty)
         {
            case 0:
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               addstr("Simple");
               break;
            case 1:
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               addstr("Very Easy");
               break;
            case 2:
               set_color(COLOR_CYAN,COLOR_BLACK,0);
               addstr("Easy");
               break;
            case 3:
               set_color(COLOR_BLUE,COLOR_BLACK,1);
               addstr("Below Average");
               break;
            case 4:
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               addstr("Average");
               break;
            case 5:
               set_color(COLOR_WHITE,COLOR_BLACK,0);
               addstr("Above Average");
               break;
            case 6:
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               addstr("Hard");
               break;
            case 7:
               set_color(COLOR_MAGENTA,COLOR_BLACK,0);
               addstr("Very Hard");
               break;
            case 8:
               set_color(COLOR_MAGENTA,COLOR_BLACK,1);
               addstr("Extremely Difficult");
               break;
            case 9:
               set_color(COLOR_RED,COLOR_BLACK,0);
               addstr("Nearly Impossible");
               break;
            default:
               set_color(COLOR_RED,COLOR_BLACK,1);
               addstr("Impossible");
               break;
         }

         set_color(COLOR_GREEN,COLOR_BLACK,1);
         itoa(armor_makeprice(armortype[p]),num,10);
         move(y,63-strlen(num));
         addch('$');addstr(num);

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Type of Clothing");
      move(23,0);
      if(interface_pgup=='[')
      {
         addstr("[] to view other Liberal pages.");
      }
      
         else if(interface_pgup=='.')
         {
            addstr("; and : to view other Liberal pages.");
         }
      else
      {
         addstr("PGUP/PGDN to view other Liberal pages.");
      }

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if(c==interface_pgup&&page>0)page--;
      //PAGE DOWN
      if(c==interface_pgdn&&(page+1)*19<armortype.size())page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<armortype.size())
         {
            cr->activity.type=ACTIVITY_MAKE_ARMOR;
            cr->activity.arg=armortype[p];
            return;
         }
      }

      if(c==10)break;
   }while(1);
}



long armor_makedifficulty(int type,creaturest *cr)
{
   long basedif;

   switch(type)
   {
      case ARMOR_TOGA:
      case ARMOR_WIFEBEATER:
         basedif=2;
         break;
      case ARMOR_CLOTHES:
      case ARMOR_OVERALLS:
      case ARMOR_WORKCLOTHES:
         basedif=3;
         break;
      case ARMOR_CLOWNSUIT:
      case ARMOR_PRISONER:
      case ARMOR_CHEAPDRESS:
      case ARMOR_TRENCHCOAT:
      case ARMOR_LABCOAT:
      case ARMOR_BLACKROBE:
      case ARMOR_BONDAGEGEAR:
         basedif=4;
         break;
      case ARMOR_SECURITYUNIFORM:
      case ARMOR_PRISONGUARD:
      case ARMOR_MILITARY:
      case ARMOR_POLICEUNIFORM:
         basedif=5;
         break;
      case ARMOR_CHEAPSUIT:
         basedif=6;
         break;
      case ARMOR_BLACKSUIT:
      case ARMOR_BLACKDRESS:
         basedif=7;
         break;
      case ARMOR_EXPENSIVESUIT:
      case ARMOR_EXPENSIVEDRESS:
         basedif=9;
         break;
      default:
         basedif=10;
         break;
   }

   basedif-=cr->skill[SKILL_GARMENTMAKING]-3;
   if(basedif<0)basedif=0;

   return basedif;
}



long armor_makeprice(int type)
{
   long price=0;

   switch(type)
   {
      case ARMOR_TOGA:
      case ARMOR_WIFEBEATER:
         price=5;
         break;
      case ARMOR_CLOTHES:
      case ARMOR_OVERALLS:
      case ARMOR_WORKCLOTHES:
         price=10;
         break;
      case ARMOR_CLOWNSUIT:
      case ARMOR_PRISONER:
      case ARMOR_CHEAPDRESS:
      case ARMOR_TRENCHCOAT:
      case ARMOR_LABCOAT:
      case ARMOR_BLACKROBE:
         price=20;
         break;
      case ARMOR_BONDAGEGEAR:
         price=30;
         break;
      case ARMOR_SECURITYUNIFORM:
      case ARMOR_PRISONGUARD:
      case ARMOR_MILITARY:
      case ARMOR_POLICEUNIFORM:
         price=40;
         break;
      case ARMOR_CHEAPSUIT:
         price=50;
         break;
      case ARMOR_BLACKSUIT:
      case ARMOR_BLACKDRESS:
         price=60;
         break;
      case ARMOR_EXPENSIVESUIT:
      case ARMOR_EXPENSIVEDRESS:
         price=300;
         break;
   }

   return price;
}



/* base - activate - trouble */
long select_troublefundinglevel(creaturest *cr)
{
   long flevel=-1;

   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(0,0);
   addstr("Select a Funding Level for this Operation:");

   move(2,0);
   addstr("A - Don't spend anything.  ");
   addstr(cr->name);
   addstr(" just needs something constructive to do.");

   move(3,0);
   addstr("B - $20 per day.  Some minor purchases are needed.");

   move(4,0);
   addstr("C - $50 per day.  Disobedience costs money.");

   move(5,0);
   addstr("D - $100 per day.  Enough to be really disobedient.");

   move(6,0);
   addstr("E - $500 per day.  The Machine will buckle under the weight of");
   move(7,0);
   addstr("    ");
   addstr(cr->name);
   addstr("'s Numerous and Varied Liberal Acts.");

   move(9,0);
   addstr("X - On second thought, this isn't a job for ");
   addstr(cr->name);
   addstr(".");

   refresh();
   int c=getch();
   translategetch(c);

   if(c=='a')flevel=0;
   if(c=='b')flevel=20;
   if(c=='c')flevel=50;
   if(c=='d')flevel=100;
   if(c=='e')flevel=500;

   return flevel;
}



/* base - activate - select a topic to write about */
char select_view(creaturest *cr,long &v)
{
   int page=0;
   char str[80];

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Write a news story if the LCS makes the news on the selected topic today, or");
      move(1,0);
      addstr("write editorials if there is no current news but there is public interest.");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(2,0);
      addstr("----TOPIC-----------------------------------INTEREST---------------------------");

      int y=3,x=0;
      for(int p=page*18;p<VIEWNUM-2&&p<page*18+18;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,x);
         addch((p-page*18)+'A');addstr(" - ");
         getview(str,p);
         addstr(str);

         move(y,44);
         if(newspaper_topicwork1[p]>40)
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr("Major Controversy");
         }
         else if(newspaper_topicwork1[p]>10)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr("Significant");
         }
         else if(newspaper_topicwork1[p]>0)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr("Minor");
         }
         else
         {
            set_color(COLOR_BLACK,COLOR_BLACK,1);
            addstr("None");
         }

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Topic");
      move(23,0);
      if(interface_pgup=='[')
      {
         addstr("[] to view other Liberal pages.");
      }
      else if(interface_pgup=='.')
      {
         addstr("; and : to view other Liberal pages.");
      }
      else
      {
         addstr("PGUP/PGDN to view other Liberal pages.");
      }

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if(c==interface_pgup&&page>0)page--;
      //PAGE DOWN
      if(c==interface_pgdn&&(page+1)*16<VIEWNUM-2)page++;

      if(c>='a'&&c<='a'+18)
      {
         int p=page*18+(int)(c-'a');
         if(p<VIEWNUM-2)
         {
            v=p;
            return 1;
         }
      }

      if(c==10)break;
   }while(1);

   return 0;
}

