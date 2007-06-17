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


/* base - burn the flag */
void burnflag(void)
{
   int flagparts=112;
   short flag[16][7][4];
   int x;
   int y;

   for(int p=0;p<7;p++)
   {
      if(p<3)
      {
         for(x=0;x<6;x++)
         {
            flag[x][p][0]=':';
            flag[x][p][1]=COLOR_WHITE;
            flag[x][p][2]=COLOR_BLUE;
            flag[x][p][3]=1;
         }

         for(x=6;x<16;x++)
         {
            flag[x][p][0]=CH_LOWER_HALF_BLOCK;
            flag[x][p][3]=1;
            flag[x][p][1]=COLOR_WHITE;
            flag[x][p][2]=COLOR_RED;
         }
      }
      else
      {
         for(x=0;x<16;x++)
         {
            if(p<6)
            {
               flag[x][p][0]=CH_LOWER_HALF_BLOCK;
               flag[x][p][1]=COLOR_WHITE;
               flag[x][p][2]=COLOR_RED;
               flag[x][p][3]=1;
            }
            else
            {
               flag[x][p][0]=CH_UPPER_HALF_BLOCK;
               flag[x][p][1]=COLOR_RED;
               flag[x][p][2]=COLOR_BLACK;
               flag[x][p][3]=0;
            }
         }
      }
   }

   x=LCSrandom(16);
   y=LCSrandom(7);
   flag[x][y][0]=CH_DARK_SHADE;
   flag[x][y][1]=COLOR_YELLOW;
   flag[x][y][2]=COLOR_BLACK;
   flag[x][y][3]=1;

   char first=1;

   while(flagparts>0)
   {
      if(!first)
      {
         for(x=0;x<16;x++)
         {
            for(y=0;y<7;y++)
            {
               if(flag[x][y][0]==CH_BOX_DRAWINGS_LIGHT_VERTICAL)flag[x][y][0]--;
               else if(flag[x][y][0]==CH_DARK_SHADE)
               {
                  flag[x][y][0]=CH_MEDIUM_SHADE;
                  flag[x][y][1]=COLOR_RED;
                  flag[x][y][2]=COLOR_BLACK;
                  flag[x][y][3]=0;
               }
               else if(flag[x][y][0]==CH_MEDIUM_SHADE)
               {
                  flag[x][y][0]=CH_LIGHT_SHADE;
                  flag[x][y][1]=COLOR_BLACK;
                  flag[x][y][2]=COLOR_BLACK;
                  flag[x][y][3]=1;
               }
               else if(flag[x][y][0]==CH_LIGHT_SHADE)
               {
                  flagparts--;
                  flag[x][y][0]=CH_FULL_BLOCK;
                  flag[x][y][1]=COLOR_BLACK;
                  flag[x][y][2]=COLOR_BLACK;
                  flag[x][y][3]=0;
               }
            }
         }
      }
      else first=0;

      for(x=0;x<16;x++)
      {
         for(y=0;y<7;y++)
         {
            move(y+17,x+32);
            set_color(flag[x][y][1],flag[x][y][2],flag[x][y][3]);
            addch(flag[x][y][0]);
         }
      }
      refresh();

      pause_ms(10);

      char gotnew=0;
      while(!gotnew&&flagparts>3)
      {
         x=LCSrandom(16);
         y=LCSrandom(7);
         char conf=0;
         if(flag[x][y][0]==':'||flag[x][y][0]==CH_UPPER_HALF_BLOCK||flag[x][y][0]==220)
         {
            if(x>0)
            {
               if(flag[x-1][y][0]!=':'&&
                  flag[x-1][y][0]!=CH_UPPER_HALF_BLOCK&&
                  flag[x-1][y][0]!=CH_LOWER_HALF_BLOCK)conf=1;
            }
            if(x<15)
            {
               if(flag[x+1][y][0]!=':'&&
                  flag[x+1][y][0]!=CH_UPPER_HALF_BLOCK&&
                  flag[x+1][y][0]!=CH_LOWER_HALF_BLOCK)conf=1;
            }
            if(y>0)
            {
               if(flag[x][y-1][0]!=':'&&
                  flag[x][y-1][0]!=CH_UPPER_HALF_BLOCK&&
                  flag[x][y-1][0]!=CH_LOWER_HALF_BLOCK)conf=1;
            }
            if(y<6)
            {
               if(flag[x][y+1][0]!=':'&&
                  flag[x][y+1][0]!=CH_UPPER_HALF_BLOCK&&
                  flag[x][y+1][0]!=CH_LOWER_HALF_BLOCK)conf=1;
            }
            if(conf)
            {
               flag[x][y][0]=CH_BOX_DRAWINGS_LIGHT_VERTICAL;
               flag[x][y][1]=COLOR_YELLOW;
               flag[x][y][2]=COLOR_BLACK;
               flag[x][y][3]=1;
               gotnew=1;
            }
         }
      }
   }
}

/* base - new slogan */
void getslogan(void)
{
   set_color(COLOR_WHITE,COLOR_BLACK,0);

   move(23,0);
   addstr("What is your new slogan?");
   move(24,0);
   addstr("                                                                                          ");

   keypad(stdscr,FALSE);
   raw_output(FALSE);
   echo();
   curs_set(1);
   mvgetstr(24,0,slogan);
   curs_set(0);
   noecho();
   raw_output(TRUE);
   keypad(stdscr,TRUE);
}



/* base - reorder party */
void orderparty(void)
{
   if(activesquad==NULL)return;

   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)partysize++;
   }

   if(partysize<=1)return;

   int spot=0;

   do
   {
      printparty();

      move(8,20);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("Choose a Liberal squad member for Place ");
      char num[20];
      itoa(spot+1,num,10);
      addstr(num);
      addstr(".");

      refresh();

      int c=getch();
      translategetch(c);

      if(c==10)return;

      if(c>=spot+'1'&&c<=partysize+'1'-1)
      {
         creaturest *swap=activesquad->squad[spot];
         activesquad->squad[spot]=activesquad->squad[c-'1'];
         activesquad->squad[c-'1']=swap;
         spot++;
      }
   }while(spot<partysize-1);
}



/* base - go forth to stop evil */
void stopevil(void)
{
 int l = 0;
 int p = 0;
 
   if(activesquad==NULL)return;

   char havecar=0;
   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(activesquad->squad[p]->pref_carid!=-1)
         {
            havecar=1;
            break;
         }
      }
   }

   int page=0;
   long loc=-1;

   vector<long> temploc;
   for(l=0;l<location.size();l++)
   {
      if(location[l]->parent==loc&&location[l]->renting>=0)temploc.push_back(l);
   }
   for(l=0;l<location.size();l++)
   {
      if(location[l]->parent==loc&&location[l]->renting==-1)temploc.push_back(l);
   }

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Where will the Squad go?");

      printparty();

      if(loc!=-1)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(8,0);
         addlocationname(location[loc]);
      }

      int y=10;
      for(p=page*11;p<temploc.size()&&p<page*11+11;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y-10+(int)'A');
         addstr(" - ");

         addlocationname(location[temploc[p]]);

         if(temploc[p]==activesquad->squad[0]->location)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" (Current Location)");
         }
         else if(location[temploc[p]]->renting>=0)
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            addstr(" (Safe House)");
         }
         if(location[temploc[p]]->closed>0)
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr(" (Too Hot)");
         }
         if(location[temploc[p]]->highsecurity)
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            addstr(" (High Security)");
         }
         if(location[temploc[p]]->needcar>0&&!havecar)
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            addstr(" (Need Car)");
         }
         if(location[temploc[p]]->siege.siege>0)
         {
            set_color(COLOR_RED,COLOR_BLACK,0);
            addstr(" (Under Siege)");
         }
         y++;
      }

      //PAGE UP
      if(page>0)
      {
         move(10,60);
         if(interface_pgup=='[')
         {
            addstr("[ - Previous");
         }
         else if(interface_pgup=='.')
         {
            addstr("; - Previous");
         }
         else
         {
            addstr("PGUP - Previous");
         }
      }
      //PAGE DOWN
      if((page+1)*11<temploc.size())
      {
         move(20,60);
         if(interface_pgup=='[')
         {
            addstr("] - Next");
         }
         else if(interface_pgup=='.')
         {
            addstr(": - Next");
         }
         else
         {
            addstr("PGDN - Next");
         }
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(24,1);
      if(loc!=-1)addstr("Enter - Back one step.");
      else addstr("Enter - The squad is not yet Liberal enough.");

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if(c==interface_pgup&&page>0)page--;
      //PAGE DOWN
      if(c==interface_pgdn&&(page+1)*11<temploc.size())page++;

      if(c>='a'&&c<='k')
      {
         int sq=page*11+(int)(c-'a');
         if(sq<temploc.size()&&sq>=0)
         {
            int oldloc=loc;
            loc=temploc[sq];
            temploc.clear();

            for(l=0;l<location.size();l++)
            {
               if(location[l]->parent==loc&&location[l]->renting>=0)temploc.push_back(l);
            }
            for(l=0;l<location.size();l++)
            {
               if(location[l]->parent==loc&&location[l]->renting==-1)temploc.push_back(l);
            }

            if(temploc.size()==0)
            {
               if(!location[loc]->closed&&
                  (location[loc]->needcar==0||havecar))
               {
                  activesquad->activity.type=ACTIVITY_VISIT;
                  activesquad->activity.arg=loc;
                  return;
               }
               else
               {
                  loc=oldloc;
                  for(l=0;l<location.size();l++)
                  {
                     if(location[l]->parent==loc&&location[l]->renting>=0)temploc.push_back(l);
                  }
                  for(l=0;l<location.size();l++)
                  {
                     if(location[l]->parent==loc&&location[l]->renting==-1)temploc.push_back(l);
                  }
               }
            }
         }
      }

      if(c==10&&loc!=-1)
      {
         loc=location[loc]->parent;
         temploc.clear();
         for(l=0;l<location.size();l++)
         {
            if(location[l]->parent==loc&&location[l]->renting>=0)temploc.push_back(l);
         }
         for(l=0;l<location.size();l++)
         {
            if(location[l]->parent==loc&&location[l]->renting==-1)temploc.push_back(l);
         }
      }
      else if(c==10)break;

   }while(1);
}



/* base - liberal agenda */
char liberalagenda(char won)
{
   erase();

   if(won==1)
   {
      set_color(COLOR_GREEN,COLOR_BLACK,1);
      move(0,0);
      addstr("The Triumph of the Liberal Agenda");
   }
   else if(won==-1)
   {
      set_color(COLOR_RED,COLOR_BLACK,1);
      move(0,0);
      addstr("The Abject Failure of the Liberal Agenda");
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("The Status of the Liberal Agenda");
   }

   if(exec[EXEC_PRESIDENT]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
   else if(exec[EXEC_PRESIDENT]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   else if(exec[EXEC_PRESIDENT]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
   else if(exec[EXEC_PRESIDENT]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
   else set_color(COLOR_GREEN,COLOR_BLACK,1);
   move(1,0);
   if(won!=-1)addstr("President: ");
   else addstr("King ");
   addstr(execname[EXEC_PRESIDENT]);addstr(", ");
   switch(exec[EXEC_PRESIDENT])
   {
      case -2:addstr("Arch-Conservative");break;
      case -1:addstr("Conservative");break;
      case 0:addstr("moderate");break;
      case 1:addstr("Liberal");break;
      case 2:addstr("Elite Liberal");break;
   }
   if(won!=-1)
   {
      if(execterm==1)addstr(", 1st Term");
      else addstr(", 2nd Term");
   }
   if(exec[EXEC_VP]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
   else if(exec[EXEC_VP]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   else if(exec[EXEC_VP]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
   else if(exec[EXEC_VP]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
   else set_color(COLOR_GREEN,COLOR_BLACK,1);
   move(2,0);
   addstr("Vice President: ");addstr(execname[EXEC_VP]);addstr(", ");
   switch(exec[EXEC_VP])
   {
      case -2:addstr("Arch-Conservative");break;
      case -1:addstr("Conservative");break;
      case 0:addstr("moderate");break;
      case 1:addstr("Liberal");break;
      case 2:addstr("Elite Liberal");break;
   }
   if(exec[EXEC_STATE]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
   else if(exec[EXEC_STATE]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   else if(exec[EXEC_STATE]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
   else if(exec[EXEC_STATE]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
   else set_color(COLOR_GREEN,COLOR_BLACK,1);
   move(3,0);
   addstr("Secretary of State: ");addstr(execname[EXEC_STATE]);addstr(", ");
   switch(exec[EXEC_STATE])
   {
      case -2:addstr("Arch-Conservative");break;
      case -1:addstr("Conservative");break;
      case 0:addstr("moderate");break;
      case 1:addstr("Liberal");break;
      case 2:addstr("Elite Liberal");break;
   }
   if(exec[EXEC_ATTORNEY]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
   else if(exec[EXEC_ATTORNEY]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   else if(exec[EXEC_ATTORNEY]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
   else if(exec[EXEC_ATTORNEY]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
   else set_color(COLOR_GREEN,COLOR_BLACK,1);
   move(4,0);
   addstr("Attorney General: ");addstr(execname[EXEC_ATTORNEY]);addstr(", ");
   switch(exec[EXEC_ATTORNEY])
   {
      case -2:addstr("Arch-Conservative");break;
      case -1:addstr("Conservative");break;
      case 0:addstr("moderate");break;
      case 1:addstr("Liberal");break;
      case 2:addstr("Elite Liberal");break;
   }

   int y=22-LAWNUM;
   for(int l=0;l<LAWNUM;l++)
   {
      if(won==-1)set_color(COLOR_RED,COLOR_BLACK,1);
      else
      {
         if(law[l]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(law[l]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(law[l]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(law[l]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);
      }

      move(y,0);

      switch(l)
      {
         case LAW_TAX:
				if(won==-1)addstr("There are no taxes, yet most people have no money.");
				else if(law[l]==-2)addstr("The tax code is a nightmare designed to maintain class structure.");
				else if(law[l]==-1)addstr("A flat tax is in effect.");
				else if(law[l]==0)addstr("Taxes are moderate, and the code has loop-holes.");
				else if(law[l]==1)addstr("Taxes are very high and steeply graded.");
				else addstr("Rich people are virtually unheard of, due to taxation.");
				break;
         case LAW_ABORTION:
            if(won==-1)addstr("Use of contraception and abortion are capital offenses.");
            else if(law[l]==-2)addstr("Abortion is illegal in all cases.");
            else if(law[l]==-1)addstr("Abortion is illegal except in extreme circumstances.");
            else if(law[l]==0)addstr("Abortion is illegal in the second and third trimesters.");
            else if(law[l]==1)addstr("Abortion is illegal in the third trimester.");
            else addstr("Abortion is legal.");
            break;
         case LAW_ANIMALRESEARCH:
            if(won==-1)addstr("All forms of human experimentation on the poor are encouraged.");
            else if(law[l]==-2)addstr("Animal research is completely unregulated.");
            else if(law[l]==-1)addstr("Animal research is lightly regulated.");
            else if(law[l]==0)addstr("Animal research is moderately regulated.");
            else if(law[l]==1)addstr("Animal research is stiffly regulated.");
            else addstr("Animal research is illegal in all cases.");
            break;
         case LAW_POLICEBEHAVIOR:
            if(won==-1)addstr("Policing is administered by corporations and has a draft.");
            else if(law[l]==-2)addstr("Law enforcement is given free reign.");
            else if(law[l]==-1)addstr("Law enforcement is lightly regulated.");
            else if(law[l]==0)addstr("Law enforcement is moderately regulated.");
            else if(law[l]==1)addstr("Law enforcement is strictly controlled.");
            else addstr("All law enforcement positions are subject to election and recall.");
            break;
         case LAW_PRIVACY:
            if(won==-1)addstr("Files on each citizen are easily accessible to corporations.");
            else if(law[l]==-2)addstr("Any corporation requesting private information is granted access.");
            else if(law[l]==-1)addstr("Privacy laws are weak.");
            else if(law[l]==0)addstr("Privacy laws are moderate.");
            else if(law[l]==1)addstr("Privacy laws are strong.");
            else addstr("Individual privacy is sacred.");
            break;
         case LAW_DEATHPENALTY:
            if(won==-1)addstr("Poor criminals receive mandatory death sentences.");
            else if(law[l]==-2)addstr("People can be put to death for minor offenses.");
            else if(law[l]==-1)addstr("The death penalty is actively enforced in many states.");
            else if(law[l]==0)addstr("The death penalty is in effect but under scrutiny.");
            else if(law[l]==1)addstr("The death penalty is not permitted in many circumstances.");
            else addstr("The death penalty is considered barbaric and never practiced.");
            break;
         case LAW_NUCLEARPOWER:
            if(won==-1)addstr("Nuclear power plants are ubiquitous.");
            else if(law[l]==-2)addstr("Nuclear power is proliferating with no controls.");
            else if(law[l]==-1)addstr("Nuclear power is a preferred energy source.");
            else if(law[l]==0)addstr("Nuclear power is often an energy source.");
            else if(law[l]==1)addstr("Nuclear power is intensely regulated and seldom used.");
            else addstr("Nuclear power is illegal.");
            break;
         case LAW_POLLUTION:
            if(won==-1)addstr("Deformed children are the norm in industrial zones.");
            else if(law[l]==-2)addstr("Industry may pollute as much as they like.");
            else if(law[l]==-1)addstr("Industry voluntarily regulates pollution.");
            else if(law[l]==0)addstr("Industry is subject to moderate pollution regulations.");
            else if(law[l]==1)addstr("Industry is subject to strict pollution regulations.");
            else addstr("Industry is subject to zero-tolerance pollution regulations.");
            break;
         case LAW_LABOR:
            if(won==-1)addstr("People are bred in pens to be farmed out to corporations like beasts.");
            else if(law[l]==-2)addstr("There is no weekend and children are forced to work.");
            else if(law[l]==-1)addstr("Working conditions are miserable and the minimum wage is low.");
            else if(law[l]==0)addstr("Workers still require some benefits.");
            else if(law[l]==1)addstr("Workers are fairly compensated and have benefits.");
            else addstr("There are universal workers' rights and high wages.");
            break;
         case LAW_GAY:
            if(won==-1)addstr("Homosexuals are executed regularly.");
            else if(law[l]==-2)addstr("Homosexuals are routinely persecuted with no recourse.");
            else if(law[l]==-1)addstr("Homosexuals are not tolerated.");
            else if(law[l]==0)addstr("Homosexuals are grudgingly tolerated but have few equal rights.");
            else if(law[l]==1)addstr("Homosexuals have many rights shared by heterosexuals.");
            else addstr("Homosexuals have equal rights.");
            break;
         case LAW_CORPORATE:
            if(won==-1)addstr("Corporations under the King run the country in a feudal system.");
            else if(law[l]==-2)addstr("Corporations essentially run the country in a feudal system.");
            else if(law[l]==-1)addstr("Corporate culture is corrupt and there is a great disparity in wages.");
            else if(law[l]==0)addstr("Corporations are moderately regulated, although wages are still unfair.");
            else if(law[l]==1)addstr("Corporations are stiffly regulated, and executives are fairly compensated.");
            else addstr("Corporations are subject to intense regulation, and there is a maximum wage law.");
            break;
         case LAW_FREESPEECH:
            if(won==-1)addstr("Unacceptable speech is a capital crime.");
            else if(law[l]==-2)addstr("Speech is routinely suppressed.");
            else if(law[l]==-1)addstr("Some individuals are harassed because of their speech.");
            else if(law[l]==0)addstr("Free speech is tolerated.");
            else if(law[l]==1)addstr("Free speech is encouraged.");
            else addstr("Free speech is universally supported.");
            break;
         case LAW_FLAGBURNING:
            if(won==-1)addstr("Images or words describing flag burning are punished by death.");
            else if(law[l]==-2)addstr("Burning the flag is a crime on par with murder.");
            else if(law[l]==-1)addstr("Burning the flag is a felony.");
            else if(law[l]==0)addstr("Flag-burning is a misdemeanor.");
            else if(law[l]==1)addstr("Flag-burning is legal but stigmatized.");
            else addstr("Flag-burning is legal.");
            break;
         case LAW_GUNCONTROL:
            if(won==-1)addstr("Gangs of young children carrying AK-47s roam the streets.");
            else if(law[l]==-2)addstr("Machine guns can be bought and sold freely.");
            else if(law[l]==-1)addstr("Military weapons are banned, but similar-looking guns are available.");
            else if(law[l]==0)addstr("A comprehensive ban on military-style weapons is in effect.");
            else if(law[l]==1)addstr("Most guns cannot be sold to anyone outside of law enforcement.");
            else addstr("It is illegal to buy, sell, or carry a gun in public.");
            break;
      }

      y++;
   }

   int housemake[5]={0,0,0,0,0};
   for(int h=0;h<435;h++)
   {
      housemake[house[h]+2]++;
   }
   int lsum=housemake[3]+housemake[4]
      -housemake[0]-housemake[1];
   if(lsum<=-145)set_color(COLOR_RED,COLOR_BLACK,1);
   else if(lsum<0)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   else if(lsum<145)set_color(COLOR_YELLOW,COLOR_BLACK,1);
   else if(housemake[4]<290)set_color(COLOR_BLUE,COLOR_BLACK,1);
   else set_color(COLOR_GREEN,COLOR_BLACK,1);
   char num[20];
   if(won!=-1)
   {
      move(5,0);
      addstr("House: ");
      itoa(housemake[4],num,10);
      addstr(num);addstr("Lib+, ");
      itoa(housemake[3],num,10);
      addstr(num);addstr("Lib, ");
      itoa(housemake[2],num,10);
      addstr(num);addstr("Mod, ");
      itoa(housemake[1],num,10);
      addstr(num);addstr("Cons, ");
      itoa(housemake[0],num,10);
      addstr(num);addstr("Cons+");
   }

   int senatemake[5]={0,0,0,0,0};
   for(int s=0;s<100;s++)
   {
      senatemake[senate[s]+2]++;
   }
   lsum=senatemake[3]+senatemake[4]
      -senatemake[0]-senatemake[1];
   if(lsum<=-33)set_color(COLOR_RED,COLOR_BLACK,1);
   else if(lsum<0)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   else if(lsum<33)set_color(COLOR_YELLOW,COLOR_BLACK,1);
   else if(senatemake[4]<67)set_color(COLOR_BLUE,COLOR_BLACK,1);
   else set_color(COLOR_GREEN,COLOR_BLACK,1);
   if(won!=-1)
   {
      move(6,0);
      addstr("Senate: ");
      itoa(senatemake[4],num,10);
      addstr(num);addstr("Lib+, ");
      itoa(senatemake[3],num,10);
      addstr(num);addstr("Lib, ");
      itoa(senatemake[2],num,10);
      addstr(num);addstr("Mod, ");
      itoa(senatemake[1],num,10);
      addstr(num);addstr("Cons, ");
      itoa(senatemake[0],num,10);
      addstr(num);addstr("Cons+");
   }
   else
   {
      set_color(COLOR_RED,COLOR_BLACK,1);
      move(7,0);
      addstr("The Congress consists of CEOs and televangelists.");
   }

   int elibjudge=0;
   for(int c=0;c<9;c++)
   {
      if(court[c]==2)elibjudge++;
   }

   if(won==-1)set_color(COLOR_RED,COLOR_BLACK,1);
   else if(won==1||elibjudge>=5)set_color(COLOR_GREEN,COLOR_BLACK,1);
   else set_color(COLOR_WHITE,COLOR_BLACK,1);

   move(0,56);addch('S');
   move(1,56);addch('U');
   move(2,56);addch('P');
   move(3,56);addch('R');
   move(4,56);addch('E');
   move(5,56);addch('M');
   move(6,56);addch('E');

   move(0,58);addch('C');
   move(1,58);addch('O');
   move(2,58);addch('U');
   move(3,58);addch('R');
   move(4,58);addch('T');

   if(won!=-1)
   {
      y=0;

      for(int c=0;c<9;c++)
      {
         if(court[c]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(court[c]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(court[c]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(court[c]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);

         move(y,60);
         addstr(courtname[c]);

         y++;
      }
   }
   else
   {
      set_color(COLOR_RED,COLOR_BLACK,1);
      move(0,60);
      addstr("   Replaced");
      move(1,60);
      addstr(" By Corporate");
      move(2,60);
      addstr("Ethics Officers");
   }

   if(won==1)
   {
      set_color(COLOR_GREEN,COLOR_BLACK,1);
      move(23,0);
      addstr("The country has achieved Elite Liberal status.");
      move(24,0);
      addstr("Press 'L' to view the high score list.");

      do
      {
         refresh();
         int c=getch();
         translategetch(c);

         if(c=='l')break;
      }while(1);
   }
   else if(won==-1)
   {
      set_color(COLOR_RED,COLOR_BLACK,1);
      move(23,0);
      addstr("The country has been Reaganified.");
      move(24,0);
      addstr("Press 'L' to view the high score list.");

      do
      {
         refresh();
         int c=getch();
         translategetch(c);

         if(c=='l')break;
      }while(1);
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(23,0);
      addstr("Once these are Green, the country will have achieved Elite Liberal status.");
      move(24,0);
      addstr("Press D to disband and wait.  Press any other key to consider the situation.");

      do
      {
         refresh();
         int c=getch();
         translategetch(c);

         if(c=='d')
         {
            return confirmdisband();
         }

         break;
      }while(1);
   }

   return 0;
}



/* base - liberal agenda - disband */
char confirmdisband(void)
{
   char word[80];
   int pos=0;

   switch(LCSrandom(5))
   {
      case 0:strcpy(word,"Corporate Accountability");break;
      case 1:strcpy(word,"Free Speech");break;
      case 2:strcpy(word,"Gay Marriage");break;
      case 3:strcpy(word,"Abortion Rights");break;
      case 4:strcpy(word,"Separation Clause");break;
   }

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Are you sure you want to disband?");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(2,0);
      addstr("Disbanding ends the game.  The country will be simulated until a resolution");
      move(3,0);
      addstr("is reached.  Any sleepers in place will still be effective, but the rest of");
      move(4,0);
      addstr("your infrastructure will be dismantled.");

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(6,0);
      addstr("Type this Liberal phrase to confirm (press a wrong letter to rethink it):");

      for(int x=0;x<strlen(word);x++)
      {
         move(8,x);
         if(x==pos)set_color(COLOR_GREEN,COLOR_BLACK,0);
         else if(x<pos)set_color(COLOR_GREEN,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         addch(word[x]);
      }

      int c=getch();
      translategetch(c);

      if((c==word[pos])||((c+'A'-'a')==word[pos]))
      {
         pos++;
         if(word[pos]==' ')pos++;
         if(pos>=strlen(word))
         {
            //SET UP THE DISBAND
            for(int p=0;p<pool.size();p++)
            {
               if(pool[p]->alive&&
                  !(pool[p]->flag & CREATUREFLAG_SLEEPER))
               {
                  pool[p]->activity.type=ACTIVITY_NONE;
                  pool[p]->clinic=0;
                  for(int i=0;i<LAWFLAGNUM;i++)
                  {
                     pool[p]->lawflag[i]=0;
                  }
                  pool[p]->dating=0;
                  pool[p]->hiding=0;
               }
            }

            disbandtime=year;

            return 1;
         }
      }
      else break;

   }while(1);

   return 0;
}



/* base - invest in this location */
void investlocation(void)
{
   int loc=selectedsiege;

   do
   {
      erase();

      locheader();
      printlocation(loc);

      if(!(location[loc]->compound_walls & COMPOUND_BASIC))
      {
         if(funds>=2000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(8,1);
         addstr("W - Fortify the Compound for a Siege ($2000)");
      }

      if(!(location[loc]->compound_walls & COMPOUND_CAMERAS))
      {
         if(funds>=2000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(9,1);
         addstr("C - Place Security Cameras around the Compound ($2000)");
      }

      if(!(location[loc]->compound_walls & COMPOUND_TRAPS))
      {
         if(funds>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,1);
         addstr("B - Place Booby Traps throughout the Compound ($3000)");
      }

      if(!(location[loc]->compound_walls & COMPOUND_TANKTRAPS))
      {
         if(funds>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,1);
         addstr("T - Ring the Compound with Tank Traps ($3000)");
      }

      if(!(location[loc]->compound_walls & COMPOUND_GENERATOR))
      {
         if(funds>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,1);
         addstr("G - Buy a Generator for Electricity ($3000)");
      }

      if(!(location[loc]->compound_walls & COMPOUND_PRINTINGPRESS))
      {
         if(funds>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,1);
         addstr("P - Setup a Printing Press ($3000)");
      }

      if(location[loc]->front_business==-1)
      {
         if(funds>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(14,1);
         addstr("F - Setup a Business Front ($3000)");
      }

      if(funds>=150)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,1);
      addstr("R - Buy 20 more daily rations ($150)");

      move(16,1);
      addstr("Enter - Done");

      int c=getch();
      translategetch(c);

      if(c==10)break;

      if(c=='w')
      {
         if(!(location[loc]->compound_walls & COMPOUND_BASIC)&&funds>=2000)
         {
            funds-=2000;
            stat_spent+=2000;
            moneylost_compound+=2000;
            location[loc]->compound_walls|=COMPOUND_BASIC;
         }
      }

      if(c=='c')
      {
         if(!(location[loc]->compound_walls & COMPOUND_CAMERAS)&&funds>=2000)
         {
            funds-=2000;
            stat_spent+=2000;
            moneylost_compound+=2000;
            location[loc]->compound_walls|=COMPOUND_CAMERAS;
         }
      }

      if(c=='b')
      {
         if(!(location[loc]->compound_walls & COMPOUND_TRAPS)&&funds>=3000)
         {
            funds-=3000;
            stat_spent+=3000;
            moneylost_compound+=3000;
            location[loc]->compound_walls|=COMPOUND_TRAPS;
         }
      }

      if(c=='t')
      {
         if(!(location[loc]->compound_walls & COMPOUND_TANKTRAPS)&&funds>=3000)
         {
            funds-=3000;
            stat_spent+=3000;
            moneylost_compound+=3000;
            location[loc]->compound_walls|=COMPOUND_TANKTRAPS;
         }
      }

      if(c=='g')
      {
         if(!(location[loc]->compound_walls & COMPOUND_GENERATOR)&&funds>=3000)
         {
            funds-=3000;
            stat_spent+=3000;
            moneylost_compound+=3000;
            location[loc]->compound_walls|=COMPOUND_GENERATOR;
         }
      }

      if(c=='p')
      {
         if(!(location[loc]->compound_walls & COMPOUND_PRINTINGPRESS)&&funds>=3000)
         {
            funds-=3000;
            stat_spent+=3000;
            moneylost_compound+=3000;
            location[loc]->compound_walls|=COMPOUND_PRINTINGPRESS;
         }
      }

      if(c=='r')
      {
         if(funds>=150)
         {
            funds-=150;
            stat_spent+=150;
            moneylost_compound+=150;
            location[loc]->compound_stores+=20;
         }
      }

      if(c=='f')
      {
         if(location[loc]->front_business==-1&&funds>=3000)
         {
            funds-=3000;
            stat_spent+=3000;
            moneylost_compound+=3000;
            location[loc]->front_business=LCSrandom(BUSINESSFRONTNUM);
            lastname(location[loc]->front_name);
            strcat(location[loc]->front_name," ");
            switch(location[loc]->front_business)
            {
               case BUSINESSFRONT_INSURANCE:
                  switch(LCSrandom(3))
                  {
                     case 0:strcat(location[loc]->front_name,"Auto");break;
                     case 1:strcat(location[loc]->front_name,"Life");break;
                     case 2:strcat(location[loc]->front_name,"Health");break;
                  }
                  strcat(location[loc]->front_name," Insurance");
                  break;
               case BUSINESSFRONT_TEMPAGENCY:
                  switch(LCSrandom(2))
                  {
                     case 0:strcat(location[loc]->front_name,"Temp Agency");break;
                     case 1:strcat(location[loc]->front_name,"Manpower, LLC");break;
                  }
                  break;
            }
         }
      }
   }while(1);
}



/* base - assign a vehicle to this squad */
void setvehicles(void)
{
   int p, l;
   if(activesquad==NULL)return;

   int page=0;

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Choosing the Right Liberal Vehicle");

      printparty();

      int x=1,y=10;
      char str[200],str2[200];

      for(l=page*18;l<vehicle.size()&&l<page*18+18;l++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         for(p=0;p<6;p++)
         {
            if(activesquad->squad[p]==NULL)continue;
            if(activesquad->squad[p]->alive&&
               activesquad->squad[p]->pref_carid==vehicle[l]->id)
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               break;
            }
         }
         for(p=0;p<pool.size();p++)
         {
            if(pool[p]->squadid!=-1&&pool[p]->alive&&
               pool[p]->pref_carid==vehicle[l]->id&&pool[p]->squadid!=activesquad->id)
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               break;
            }
         }

         getcarfull(str2,*vehicle[l],1);
         str[0]=l-page*18+'A';
         str[1]='\x0';
         strcat(str," - ");
         strcat(str,str2);

         move(y,x);
         addstr(str);

         x+=26;
         if(x>53)
         {
            x=1;
            y++;
         }
      }

      //PAGE UP
      if(page>0)
      {
         move(17,1);
         if(interface_pgup=='[')
         {
            addstr("[ - Previous");
         }
         
         else if(interface_pgup=='.')
         {
            addstr("; - Previous");
         }
         else
         {
            addstr("PGUP - Previous");
         }
      }
      //PAGE DOWN
      if((page+1)*18<vehicle.size())
      {
         move(17,53);
         if(interface_pgup=='[')
         {
            addstr("] - Next");
         }
         
         else if(interface_pgup=='.')
         {
            addstr(": - Next");
         }
         else
         {
            addstr("PGDN - Next");
         }
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(19,1);
      addstr("Press a letter to specify passengers for that Liberal vehicle");
      move(20,1);
      addstr("Capitalize the letter to designate a driver.");
      move(21,1);
      addstr("Note:  Vehicles in yellow have already been selected by another squad");
      move(22,1);
      addstr("       These cars may be used by both squads but not on the same day.");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(24,1);
      addstr("X - Done");

      refresh();

      int c=getch();
      translategetch_cap(c);

      if(c>='A'&&c<='R')
      {
         int slot=c-'A'+page*18;

         if(slot>=0&&slot<vehicle.size())
         {
            move(8,20);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr("Choose a Liberal squad member drive it.");

            refresh();

            int c=getch();
            translategetch(c);

            if(c>='1'&&c<='6')
            {
               if(activesquad->squad[c-'1']!=NULL)
               {
                  activesquad->squad[c-'1']->pref_carid=vehicle[slot]->id;
                  if(activesquad->squad[c-'1']->canwalk())
                  {
                     activesquad->squad[c-'1']->pref_is_driver=1;
                  }
                  else activesquad->squad[c-'1']->pref_is_driver=0;
               }
            }
         }
      }

      if(c>='a'&&c<='r')
      {
         int slot=c-'a'+page*18;

         if(slot>=0&&slot<vehicle.size())
         {
            move(8,20);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr("Choose a Liberal squad member be a passenger.");

            refresh();

            int c=getch();
            translategetch(c);

            if(c>='1'&&c<='6')
            {
               if(activesquad->squad[c-'1']!=NULL)
               {
                  activesquad->squad[c-'1']->pref_carid=vehicle[slot]->id;
                  activesquad->squad[c-'1']->pref_is_driver=0;
               }
            }
         }
      }

      if(c=='x')return;

      //PAGE UP
      if(c==interface_pgup&&page>0)page--;
      //PAGE DOWN
      if(c==interface_pgdn&&(page+1)*18<vehicle.size())page++;

   }while(1);
}
