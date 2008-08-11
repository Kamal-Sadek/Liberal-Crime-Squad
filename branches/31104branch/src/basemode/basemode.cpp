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





void mode_base(void)
{
   short advanced=0;
   short directing=0;
   short in_newspaper=0;
   short in_halloween=0;
   short in_halloween2=0;
   short in_training=0;
   short in_compound=0;

   short investing=0;
   short investing_newspaper=0;
   short investing_halloween=0;
   short investing_training=0;
   short investing_compound=0;
   short investing_stores=0;

   short buyer=0;

   char forcewait,canseethings;
   long nonsighttime=0;
   int oldforcemonth=month;
   
   int length=0;

   long l = 0;

   do
   {
      forcewait=1;
      canseethings=0;
      cantseereason=3;
      if(!disbanding)
      {
         for(int p=0;p<pool.size();p++)
         {
            if(pool[p]->alive&&
               pool[p]->align==1&&
               pool[p]->dating==0&&
               pool[p]->hiding==0&&
               !(pool[p]->flag & CREATUREFLAG_SLEEPER))
            {
               if(location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON&&
                  location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
                  location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION)
               {
                  canseethings=1;
                  if(pool[p]->clinic==0){forcewait=0;break;}
               }
            }
            else
            {
               if(pool[p]->dating==1&&cantseereason>1)cantseereason=1;
               else if(pool[p]->hiding==1&&cantseereason>2)cantseereason=2;
            }
         }
      }
      else
      {
         cantseereason=4;
      }

      if(disbanding&&oldforcemonth!=month)
      {
         oldforcemonth=month;
         erase();
         move(0,0);
         char num[20];
         itoa(year,num,10);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         switch(month)
         {
            case 1:addstr("January ");break;
            case 2:addstr("February ");break;
            case 3:addstr("March ");break;
            case 4:addstr("April ");break;
            case 5:addstr("May ");break;
            case 6:addstr("June ");break;
            case 7:addstr("July ");break;
            case 8:addstr("August ");break;
            case 9:addstr("September ");break;
            case 10:addstr("October ");break;
            case 11:addstr("November ");break;
            case 12:addstr("December ");break;
         }
         addstr(num);


         int y=2;

         if(exec[EXEC_PRESIDENT]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(exec[EXEC_PRESIDENT]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(exec[EXEC_PRESIDENT]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(exec[EXEC_PRESIDENT]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(1,0);
         addstr("President: ");
         addstr(execname[EXEC_PRESIDENT]);addstr(", ");
         switch(exec[EXEC_PRESIDENT])
         {
            case -2:addstr("Arch-Conservative");break;
            case -1:addstr("Conservative");break;
            case 0:addstr("moderate");break;
            case 1:addstr("Liberal");break;
            case 2:addstr("Elite Liberal");break;
         }
         if(execterm==1)addstr(", 1st Term");
         else addstr(", 2nd Term");
         if(exec[EXEC_VP]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(exec[EXEC_VP]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(exec[EXEC_VP]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(exec[EXEC_VP]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(2,0);
         addstr("Vice President: ");addstr(execname[EXEC_VP]);addstr(", ");
         switch(exec[EXEC_VP])
         {
            case ALIGN_ARCHCONSERVATIVE:addstr("Arch-Conservative");break;
            case ALIGN_CONSERVATIVE:addstr("Conservative");break;
            case ALIGN_MODERATE:addstr("moderate");break;
            case ALIGN_LIBERAL:addstr("Liberal");break;
            case ALIGN_ELITELIBERAL:addstr("Elite Liberal");break;
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
            case ALIGN_ARCHCONSERVATIVE:addstr("Arch-Conservative");break;
            case ALIGN_CONSERVATIVE:addstr("Conservative");break;
            case ALIGN_MODERATE:addstr("moderate");break;
            case ALIGN_LIBERAL:addstr("Liberal");break;
            case ALIGN_ELITELIBERAL:addstr("Elite Liberal");break;
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
            case ALIGN_ARCHCONSERVATIVE:addstr("Arch-Conservative");break;
            case ALIGN_CONSERVATIVE:addstr("Conservative");break;
            case ALIGN_MODERATE:addstr("moderate");break;
            case ALIGN_LIBERAL:addstr("Liberal");break;
            case ALIGN_ELITELIBERAL:addstr("Elite Liberal");break;
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

         int elibjudge=0;
         int econjudge=0;
         for(int c=0;c<9;c++)
         {
            if(court[c]==2)elibjudge++;
            if(court[c]==-2)econjudge++;
         }

         if(econjudge>=5)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(elibjudge>=5)set_color(COLOR_GREEN,COLOR_BLACK,1);
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

         for(int l=0;l<LAWNUM;l++)
         {
            if(law[l]==ALIGN_ARCHCONSERVATIVE)set_color(COLOR_RED,COLOR_BLACK,1);
            else if(law[l]==ALIGN_CONSERVATIVE)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            else if(law[l]==ALIGN_MODERATE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
            else if(law[l]==ALIGN_LIBERAL)set_color(COLOR_BLUE,COLOR_BLACK,1);
            else set_color(COLOR_GREEN,COLOR_BLACK,1);

            move(10+l/2,l%2*40);

            switch(l)
            {
               case LAW_TAX:
                  addstr("Tax Structure");
			         break;
               case LAW_ABORTION:
                  addstr("Abortion Rights");
                  break;
               case LAW_ANIMALRESEARCH:
                  addstr("Animal Rights");
                  break;
               case LAW_POLICEBEHAVIOR:
                  addstr("Police Regulation");
                  break;
               case LAW_PRIVACY:
                  addstr("Privacy Rights");
                  break;
               case LAW_DEATHPENALTY:
                  addstr("Death Penalty");
                  break;
               case LAW_NUCLEARPOWER:
                  addstr("Nuclear Power");
                  break;
               case LAW_POLLUTION:
                  addstr("Pollution");
                  break;
               case LAW_LABOR:
                  addstr("Labor Laws");
                  break;
               case LAW_GAY:
                  addstr("Gay Rights");
                  break;
               case LAW_CORPORATE:
                  addstr("Corporate Regulation");
                  break;
               case LAW_FREESPEECH:
                  addstr("Free Speech");
                  break;
               case LAW_FLAGBURNING:
                  addstr("Flag Burning");
                  break;
               case LAW_GUNCONTROL:
                  addstr("Gun Control");
                  break;
            }
         }

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(21,33);
         addstr("Public Mood");
         move(22,1);
         addstr("Conservative");
         move(22,66);
         addstr("Liberal");
         move(23,0);
         addstr("<------------------------------------------------------------------------------>");
         move(23,77*publicmood(-1)/100+1);
         addstr("|");

         refresh();
      }

      if(!forcewait)
      {
         if(nonsighttime>=365*4)
         {
            erase();
            char str[100];
            if(nonsighttime>=365*16)
            {
               strcpy(str,"How long since you've heard these sounds...  times have changed.");
            }
            else if(nonsighttime>=365*8)
            {
               strcpy(str,"It has been a long time.  A lot must have changed...");
            }
            else
            {
               strcpy(str,"It sure has been a while.  Things might have changed a bit.");
            }
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(12,39-((strlen(str)-1)>>1));
            addstr(str);

            refresh();
            getch();
         }

         nonsighttime=0;
      }

      int partysize=0;
      int partydead=0;
      if(activesquad!=NULL)
      {
         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL)partysize++;
            else
            {
               if(p==buyer)buyer=0;
               continue;
            }
            // *JDS* This bay be a hack vvv
            if(!partysize)
            {
               delete activesquad;
               activesquad=NULL;
            }

            if(!activesquad->squad[p]->alive)partydead++;
         }
      }

      long safenumber=0;
      for(l=0;l<location.size();l++)
      {
         if(location[l]->renting>=0)safenumber++;
      }

      siegest *siege=NULL;
      if(selectedsiege!=-1)siege=&location[selectedsiege]->siege;
      if(activesquad!=NULL)siege=&location[activesquad->squad[0]->location]->siege;
      char sieged=0;
      if(siege!=NULL)sieged=siege->siege;
      char underattack=0;
      if(siege!=NULL)
      {
         if(sieged)underattack=siege->underattack;
      }
      
      char haveflag=0;
      if(selectedsiege!=-1)haveflag=location[selectedsiege]->haveflag;
      if(activesquad!=NULL)haveflag=location[activesquad->squad[0]->location]->haveflag;
      
      // Count people at each location
      int* location2 = new int[location.size()];
      for(int i=0;i<location.size();i++)
      {
         location2[i]=0;
      }
      for(int p=0;p<pool.size();p++)
      {
         if(!pool[p]->alive)continue; // Dead people don't count
         if(pool[p]->align!=1)continue; // Non-liberals don't count
         if(pool[p]->location==-1)continue; // Vacationers don't count
         location2[pool[p]->location]++;
      }

      char cannotwait=0;
      for(l=0;l<location.size();l++)
      {
         if(!location[l]->siege.siege)continue;

         

         if(location[l]->siege.underattack)
         {
            // Allow siege if no liberals present
            if(location2[l])cannotwait=1;
            break;
         }
         //NOTE: returns -1 if no eaters, so is okay
         if(fooddaysleft(l)==0)
         {
            // Allow siege if no liberals present
            if(location2[l])cannotwait=1;
            break;
         }
      }
      delete[] location2;

      if(!forcewait)
      {
         erase();

         if(activesquad!=NULL)selectedsiege=-1;

         locheader();
         if(selectedsiege!=-1)
         {
            printlocation(selectedsiege);

            if(location[selectedsiege]->type==SITE_INDUSTRY_WAREHOUSE&&
               !location[selectedsiege]->siege.siege)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,0);
               move(8,1);
               addstr("I - Invest in this location");
            }
         }
         else if(activesquad!=NULL)printparty();
         else makedelimiter(8,0);

         if(sieged)
         {
            move(8,1);
            if(underattack)
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               addstr("Under Attack");
            }
            else
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               addstr("Under Siege");
            }
         }

         if(haveflag)
         {
            for(int p=0;p<7;p++)
            {
               move(p+17,32);
               if(p<3)
               {
                  set_color(COLOR_WHITE,COLOR_BLUE,1);
                  move(p+17,32);
                  addstr("::::::");
                  set_color(COLOR_WHITE,COLOR_RED,1);
                  move(p+17,38);
                  for(int i=0;i<10;i++)addch(CH_LOWER_HALF_BLOCK);
               }
               else
               {
                  if(p<6)set_color(COLOR_WHITE,COLOR_RED,1);
                  else set_color(COLOR_RED,COLOR_BLACK,0);
                  for(int i=0;i<16;i++)
                  {
                     if(p==6)addch(CH_UPPER_HALF_BLOCK);
                     else addch(CH_LOWER_HALF_BLOCK);
                  }
               }
            }
         }

         if(partysize>0&&!underattack)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,1);
         addstr("E - Equipment");
         if(pool.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,1);
         addstr("R - Review and reorganize Liberals");
         if(partysize>1)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,40);
         addstr("O - Change the squad's Liberal order");

         if(squad.size()>1||(activesquad==NULL&&squad.size()>0))set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,40);
         addstr("TAB - Next Squad");

         if(safenumber>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,62);
         addstr("Z - Next Location");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(14,40);
         addstr("L - The Status of the Liberal Agenda");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(13,1);
         addstr("A - Activate the Uninvolved");

         if(partysize>0)
         {
            if(activesquad->activity.type!=ACTIVITY_NONE)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
         }
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(14,1);
         addstr("C - Cancel this Squad's Departure");

         if(sieged)
         {
            if(partysize>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else
            {
               set_color(COLOR_BLACK,COLOR_BLACK,1);
               for(int p=0;p<pool.size();p++)
               {
                  if(pool[p]->location==selectedsiege)
                  {
                     set_color(COLOR_WHITE,COLOR_BLACK,0);
                     break;
                  }
               }
            }
            move(10,1);
            addstr("F - Escape/Engage");

            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(10,23);
            addstr("G - Give Up");
         }
         else
         {
            if(partysize>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,1);
            addstr("F - Go forth to stop EVIL");
         }
         if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,40);
         addstr("# - Check the status of a squad Liberal");
         if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,40);
         addstr("0 - Show the squad's Liberal status");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(15,1);
         addstr("X - Live to fight EVIL another day");
         move(16,1);
         if(cannotwait)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr("Cannot Wait until Siege Resolved");
         }
         else
         {
            move(16,1);
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr("W - Wait a day");
            if(day==monthday())addstr(" (next month)");
         }

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(16,40);
         addstr("S - FREE SPEECH: the Liberal Slogan");
         if(vehicle.size()>0&&partysize>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,23);
         addstr("V - Vehicles");
         move(15,40);
         if(haveflag)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr("P - PROTEST: burn the flag");
         }
         else
         {
            if(funds>=20&&!sieged&&
               (selectedsiege!=-1||activesquad!=NULL))set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            addstr("P - PATRIOTISM: fly a flag here ($20)");
         }

         length=strlen(slogan);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         if(haveflag)move(24,40-(length>>1));
         else move(20,40-(length>>1));
         addstr(slogan);

         refresh();
      }

      int c='w';
      if(!forcewait)
      {
         c=getch();
         translategetch(c);
      }

      if(c=='x')break;

      if(c=='i'&&selectedsiege!=-1)
      {
         if(location[selectedsiege]->type==SITE_INDUSTRY_WAREHOUSE&&
            !location[selectedsiege]->siege.siege)
         {
            investlocation();
         }
      }

      if(c=='l')
      {
         disbanding=liberalagenda(0);
      }

      if(c=='g'&&sieged)
      {
         giveup();
         cleangonesquads();
      }

      if(c=='f')
      {
         //NOTE THAT THERE ARE TWO MORE OF THESE
            //IDENTICAL LINES BELOW
         if(!sieged&&partysize>0)
         {
            stopevil();
         }
         else
         {
            escape_engage();
            cleangonesquads();
         }
      }

      if(c=='o'&&partysize>1)orderparty();

      if(c=='c'&&partysize>0)
      {
         activesquad->activity.type=ACTIVITY_NONE;
      }

      if(c=='a')
      {
         activate();
      }

      if(c==9&&squad.size()>0)
      {
         if(activesquad==NULL)activesquad=squad[0];
         else
         {
            for(int sq=0;sq<squad.size();sq++)
            {
               if(squad[sq]==activesquad)
               {
                  if(sq==squad.size()-1)activesquad=squad[0];
                  else activesquad=squad[sq+1];
                  break;
               }
            }
         }
      }

      if(c=='z'&&safenumber>0)
      {
         activesquad=NULL;
         long sl;
         if(selectedsiege==-1)sl=0;
         else sl=selectedsiege+1;

         for(long l=sl;l<location.size();l++)
         {
            if(location[l]->renting>=0)
            {
               selectedsiege=l;
               break;
            }
            else if(l==location.size()-1)l=-1;
         }
      }

      if(c=='e'&&partysize>0&&!underattack)
      {
         if(location[activesquad->squad[0]->location]->renting>=0)
         {
            equip(activesquad->loot,activesquad->squad[0]->location);
         }
         else equip(activesquad->loot,-1);
      }

      if(c=='r'&&pool.size()>0)review();

      if(c=='w'&&(forcewait||!cannotwait))
      {
         char clearformess=forcewait;
         if(!canseethings)nonsighttime++;
         advanceday(clearformess,canseethings);
         if(day>monthday())passmonth(clearformess,canseethings);
         advancelocations();
      }

      if(c=='v'&&vehicle.size()>0&&partysize>0)setvehicles();

      if(c=='p'&&haveflag)
      {
         burnflag();
         stat_burns++;
         if(selectedsiege!=-1)
         {
            location[selectedsiege]->haveflag=0;
            criminalizepool(LAWFLAG_BURNFLAG,-1,selectedsiege);
         }
         if(activesquad!=NULL)
         {
            location[activesquad->squad[0]->base]->haveflag=0;
            criminalizepool(LAWFLAG_BURNFLAG,-1,activesquad->squad[0]->base);
         }

         //PUBLICITY IF BURN FLAG DURING SIEGE
            //ESPECIALLY IF IT IS REALLY ILLEGAL
         if(sieged)
         {
            change_public_opinion(VIEW_LIBERALCRIMESQUAD,1);
            change_public_opinion(VIEW_FREESPEECH,1,1,30);
            if(law[LAW_FLAGBURNING]<=0)
            {
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,1);
               change_public_opinion(VIEW_FREESPEECH,1,1,50);
            }
            if(law[LAW_FLAGBURNING]<=-1)
            {
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,5);
               change_public_opinion(VIEW_FREESPEECH,2,1,70);
            }
            if(law[LAW_FLAGBURNING]==-2)
            {
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,15);
               change_public_opinion(VIEW_FREESPEECH,5,1,90);
            }
         }
      }
      else if(c=='p'&&funds>=20&&!sieged&&
               (selectedsiege!=-1||activesquad!=NULL))
      {
         funds-=20;
         stat_spent+=20;
         if(selectedsiege!=-1)location[selectedsiege]->haveflag=1;
         if(activesquad!=NULL)location[activesquad->squad[0]->base]->haveflag=1;
         stat_buys++;
         moneylost_compound+=20;
      }

      if(c=='s')getslogan();

      if(c=='0')party_status=-1;

      if(c>='1'&&c<='6'&&activesquad!=NULL)
      {
         if(activesquad->squad[c-'1']!=NULL)
         {
            if(party_status==c-'1')fullstatus(party_status);
            else party_status=c-'1';
         }
      }

   }while(1);
}
