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
   short buyer=0;

   char forcewait,canseethings;
   long nonsighttime=0;
   int oldforcemonth=month;
   
   int length=0;

   int l = 0;

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
               if(!location[pool[p]->location]->part_of_justice_system())
               {
                  canseethings=1;
                  if(pool[p]->clinic==0){forcewait=0;break;}
               }
            }
            else
            {
               if(pool[p]->dating==1&&cantseereason>1)cantseereason=1;
               else if(pool[p]->hiding!=0&&cantseereason>2)cantseereason=2;
            }
         }
      }
      else
      {
         cantseereason=4;
      }

      if(disbanding&&oldforcemonth!=month)
      {
         for(int p=pool.size()-1;p>=0;p--)
         {
            int targetjuice=0;
            for(int i=0;i<(year-disbandtime)+1;i++)
            {
               targetjuice+=LCSrandom(100);
            }
            if(targetjuice>1000)
            {
               targetjuice=1000;
            }
            if(pool[p]->juice<targetjuice)
            {
               if(pool[p]->hireid!=-1 && !(pool[p]->flag & CREATUREFLAG_SLEEPER))
                  pool[p]->alive=0; // Kill for the purposes of disbanding all contacts below
            }
         }
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
         move(2,0);
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
         move(3,0);
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

         int courtmake[5]={0,0,0,0,0};
         for(int s=0;s<9;s++)
         {
            courtmake[court[s]+2]++;
         }
         lsum=courtmake[3]+courtmake[4]
             -courtmake[0]-courtmake[1];
         if(courtmake[0]>=5)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(courtmake[0]+courtmake[1]>=5)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(courtmake[3]+courtmake[4]<5)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(courtmake[4]<5)set_color(COLOR_BLUE,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(4,0);
         addstr("Supreme Court: ");
         itoa(courtmake[4],num,10);
         addstr(num);addstr("Lib+, ");
         itoa(courtmake[3],num,10);
         addstr(num);addstr("Lib, ");
         itoa(courtmake[2],num,10);
         addstr(num);addstr("Mod, ");
         itoa(courtmake[1],num,10);
         addstr(num);addstr("Cons, ");
         itoa(courtmake[0],num,10);
         addstr(num);addstr("Cons+");

         y=0;
         for(int l=0;l<LAWNUM;l++)
         {
            if(law[l]==ALIGN_ARCHCONSERVATIVE)set_color(COLOR_RED,COLOR_BLACK,1);
            else if(law[l]==ALIGN_CONSERVATIVE)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            else if(law[l]==ALIGN_MODERATE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
            else if(law[l]==ALIGN_LIBERAL)set_color(COLOR_BLUE,COLOR_BLACK,1);
            else set_color(COLOR_GREEN,COLOR_BLACK,1);

            move(6+l/3,l%3*30);

            switch(l)
            {
               case LAW_WOMEN:
                  addstr("Women's Rights");
                  break;
               case LAW_CIVILRIGHTS:
                  addstr("Civil Rights");
                  break;
               case LAW_DRUGS:
                  addstr("Drug Law");
                  break;
               case LAW_IMMIGRATION:
                  addstr("Immigration");
                  break;
               case LAW_ELECTIONS:
                  addstr("Election Reform");
                  break;
               case LAW_MILITARY:
                  addstr("Military Spending");
                  break;
               case LAW_TORTURE:
                  addstr("Human Rights");
                  break;
               case LAW_PRISONS:
                  addstr("Prison Regulation");
                  break;
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
         move(19,33);
         addstr("Public Mood");
         move(21,1);
         addstr("Conservative");
         move(21,66);
         addstr("Liberal");
         move(22,0);
         addstr("<------------------------------------------------------------------------------>");
         move(22,77*publicmood(-1)/100+1);
         addstr("|");

         move(23,0);
         addstr("R - Recreate the Liberal Crime Squad                  Any Other Key - Next Month");

         refresh();
         char c=getch();
         if(c=='r')
         {
            disbanding=0;
         }
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
            addstr(str, gamelog);
            gamelog.nextMessage(); //Write out buffer to prepare for the next message.

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

      int safenumber=0;
      for(l=0;l<location.size();l++)
      {
         if(location[l]->renting>=0)safenumber++;
      }

      siegest *siege=NULL;
      if(selectedsiege!=-1)siege=&location[selectedsiege]->siege;
      if (activesquad!=NULL && activesquad->squad[0]->location!=-1)
      {
          siege=&location[activesquad->squad[0]->location]->siege;
      }
      char sieged=0;
      if(siege!=NULL)sieged=siege->siege;
      char underattack=0;
      if(siege!=NULL)
      {
         if(sieged)underattack=siege->underattack;
      }
      
      char haveflag=0;
      if(selectedsiege!=-1)haveflag=location[selectedsiege]->haveflag;
      if(activesquad!=NULL && activesquad->squad[0]->location!=-1)
         haveflag=location[activesquad->squad[0]->location]->haveflag;
      
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

            // Allow waiting if there's no food...
            //   we'll handle this by decrementing starving Liberals' health
            //if(location2[l])cannotwait=1;
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

            if(location[selectedsiege]->can_be_upgraded()&&
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
               int stock=1;
               if(selectedsiege!=-1)stock=location[selectedsiege]->compound_stores;
               else if(activesquad!=NULL && activesquad->squad[0]->location!=-1)stock=location[activesquad->squad[0]->location]->compound_stores;
               if(!stock)addstr(" (No Food)");
            }
         }

         if(haveflag)
         {
            for(int p=0;p<7;p++)
            {
               move(p+10,32);
               if(p<3)
               {
                  set_color(COLOR_WHITE,COLOR_BLUE,1);
                  move(p+10,32);
                  addstr("::::::");
                  set_color(COLOR_WHITE,COLOR_RED,1);
                  move(p+10,38);
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

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(18,10);
         addstr("--- ACTIVISM ---");
         move(18,51);
         addstr("--- PLANNING ---");

         if(partysize>0&&!underattack)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(19,40);
         addstr("E - Equip Squad");
         if(vehicle.size()>0&&partysize>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(19,60);
         addstr("V - Vehicles");
         if(pool.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(20,40);
         addstr("R - Review Assets and Form Squads");


         if(partysize>1)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(8,30);
         if(partysize>0 && !sieged)
            addstr("O - Reorder");

         if (activesquad)
         {
            move(8,1);
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr(activesquad->name);
            addstr("-"); //in case of overlap, at least make it clear where the name ends.
         }
         if(squad.size()>1||(activesquad==NULL&&squad.size()>0))set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(8,43);
         addstr("TAB - Next Squad");

         if(safenumber>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(8,62);
         addstr("Z - Next Location");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(21,40);
         addstr("L - The Status of the Liberal Agenda");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(21,1);
         addstr("A - Activate Liberals");

         
         set_color(COLOR_BLACK,COLOR_BLACK,1);
         for(int p=0;p < pool.size();p++)
         {
            if(pool[p]->alive==true&&
               pool[p]->flag & CREATUREFLAG_SLEEPER&&
               pool[p]->align==ALIGN_LIBERAL&&
               pool[p]->hiding==false&&
               pool[p]->clinic==false&&
               pool[p]->dating==false)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,0);
               break;
            }
         }
         move(21,25);
         addstr("B - Sleepers");

         if(partysize>0)
         {
            if(activesquad->activity.type!=ACTIVITY_NONE)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
         }
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(20,1);
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
            move(19,1);
            addstr("F - Escape/Engage");

            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(19,23);
            addstr("G - Give Up");
         }
         else
         {
            if(partysize>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(19,1);
            addstr("F - Go forth to stop EVIL");
         }
         //if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
         //else set_color(COLOR_BLACK,COLOR_BLACK,1);
         //move(19,40);
         //addstr("# - Check the status of a squad Liberal");
         //if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
         //else set_color(COLOR_BLACK,COLOR_BLACK,1);
         //move(18,40);
         //addstr("0 - Show the squad's Liberal status");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(23,40);
         addstr("X - Live to fight EVIL another day");
         move(23,1);
         if(cannotwait)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr("Cannot Wait until Siege Resolved");
         }
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            if(sieged) addstr("W - Wait out the siege");
            else addstr("W - Wait a day");
            if(day==monthday())addstr(" (next month)");
         }

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(22,40);
         addstr("S - FREE SPEECH: the Liberal Slogan");
         move(22,1);
         if(haveflag)
         {
            
            if(sieged)
               set_color(COLOR_GREEN,COLOR_BLACK,1);
            else
               set_color(COLOR_WHITE,COLOR_BLACK,0);

            addstr("P - PROTEST: burn the flag");
         }
         else
         {
            if(ledger.get_funds()>=20&&!sieged&&
               (selectedsiege!=-1||activesquad!=NULL))set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            addstr("P - PATRIOTISM: fly a flag here ($20)");
         }

         length=strlen(slogan);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         if(haveflag)move(17,40-(length>>1));
         else move(13,40-(length>>1));
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
         if(location[selectedsiege]->can_be_upgraded()&&
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
         else if(underattack)
         {
            escape_engage();
            cleangonesquads();
         }
         else if(sieged)
         {
            sally_forth();
            cleangonesquads();
         }
      }

      if(c=='o'&&partysize>1)
     {
        orderparty();
     }

      if(c=='c'&&partysize>0)
      {
         activesquad->activity.type=ACTIVITY_NONE;
      }

      if(c=='a')
      {
         activate();
      }

      if(c=='b')
      {
         activate_sleepers();
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
         int sl;
         if(selectedsiege==-1)sl=0;
         else sl=selectedsiege+1;

         if(sl >= location.size()) sl = 0;

         for(int l=sl;l<location.size();l++)
         {
            if(location[l]->renting>=0)
            {
               selectedsiege=l;
               break;
            }
            else if(l==location.size()-1)l=-1;
         }
      }

      if(c=='e'&&partysize>0&&!underattack&&activesquad->squad[0]->location!=-1)
      {
		   party_status=-1;
         equip(location[activesquad->squad[0]->location]->loot,-1);
         /*if(location[activesquad->squad[0]->location]->renting>=0)
         {
            equip(activesquad->loot,activesquad->squad[0]->location);
         }
         else equip(activesquad->loot,-1);*/
      }

      if(c=='r'&&pool.size()>0)review();

      if(c=='w'&&(forcewait||!cannotwait))
      {
         char clearformess=forcewait;
         if(!canseethings)nonsighttime++;
         advanceday(clearformess,canseethings);
         if(day>monthday())passmonth(clearformess,canseethings);
         advancelocations();
         if(forcewait)
         {
            erase();
            char num[10];
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(7,5);
            addstr("Time passes...", gamelog);
            move(9,12);
            switch(month)
            {
            case 1:addstr("Jan", gamelog);break;
            case 2:addstr("Feb", gamelog);break;
            case 3:addstr("Mar", gamelog);break;
            case 4:addstr("Apr", gamelog);break;
            case 5:addstr("May", gamelog);break;
            case 6:addstr("Jun", gamelog);break;
            case 7:addstr("Jul", gamelog);break;
            case 8:addstr("Aug", gamelog);break;
            case 9:addstr("Sep", gamelog);break;
            case 10:addstr("Oct", gamelog);break;
            case 11:addstr("Nov", gamelog);break;
            case 12:addstr("Dec", gamelog);break;
            }
            addstr(". ", gamelog);
            move(9, 17);
            itoa(day,num,10);
            addstr(num, gamelog);
            addstr(", ", gamelog);
            move(9, 21);
            itoa(year,num,10);
            addstr(num, gamelog);
            gamelog.nextMessage(); //Write out buffer to prepare for the next message.
            refresh();
         }
      }

      if(c=='v'&&vehicle.size()>0&&partysize>0)setvehicles();

      if(c=='p'&&haveflag)
      {
         burnflag();
         stat_burns++;
         if(selectedsiege!=-1)
         {
            location[selectedsiege]->haveflag=0;
            if (law[LAW_FLAGBURNING] < 1) {
              criminalizepool(LAWFLAG_BURNFLAG,-1,selectedsiege);
            }
         }
         if(activesquad!=NULL)
         {
            location[activesquad->squad[0]->base]->haveflag=0;
            if (law[LAW_FLAGBURNING] < 1) {
              criminalizepool(LAWFLAG_BURNFLAG,-1,activesquad->squad[0]->base);
            }
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
      else if(c=='p'&&ledger.get_funds()>=20&&!sieged&&
               (selectedsiege!=-1||activesquad!=NULL))
      {
         ledger.subtract_funds(20,EXPENSE_COMPOUND);
         if(selectedsiege!=-1)location[selectedsiege]->haveflag=1;
         if(activesquad!=NULL)location[activesquad->squad[0]->base]->haveflag=1;
         stat_buys++;
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
