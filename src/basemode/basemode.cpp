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



bool show_disbanding_screen(int& oldforcemonth)
{
   if(oldforcemonth == month) return true;

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

   set_alignment_color(exec[EXEC_PRESIDENT], true);
   mvaddstr(1,0,"President: ");
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
      housemake[house[h]+2]++;
   int lsum=housemake[3]+housemake[4]-housemake[0]-housemake[1];
   if(lsum<=-145)set_color(COLOR_RED,COLOR_BLACK,1);
   else if(lsum<0)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   else if(lsum<145)set_color(COLOR_YELLOW,COLOR_BLACK,1);
   else if(housemake[4]<290)set_color(COLOR_CYAN,COLOR_BLACK,1);
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
      senatemake[senate[s]+2]++;
   lsum=senatemake[3]+senatemake[4]-senatemake[0]-senatemake[1];
   if(lsum<=-33)set_color(COLOR_RED,COLOR_BLACK,1);
   else if(lsum<0)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   else if(lsum<33)set_color(COLOR_YELLOW,COLOR_BLACK,1);
   else if(senatemake[4]<67)set_color(COLOR_CYAN,COLOR_BLACK,1);
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
   if(courtmake[0]>=5)set_alignment_color(ALIGN_ARCHCONSERVATIVE, true);
   else if(courtmake[0]+courtmake[1]>=5)set_alignment_color(ALIGN_CONSERVATIVE, true);
   else if(courtmake[3]+courtmake[4]<5)set_alignment_color(ALIGN_MODERATE, true);
   else if(courtmake[4]<5)set_alignment_color(ALIGN_LIBERAL, true);
   else set_alignment_color(ALIGN_ELITELIBERAL, true);
   mvaddstr(4,0,"Supreme Court: ");
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
      set_alignment_color(law[l], true);
      move(6+l/3,l%3*30);
      char str[40];
      getlaw(str,l);
      addstr(str);
   }

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   mvaddstr(19,33,"Public Mood");
   mvaddstr(21,1,"Conservative");
   mvaddstr(21,66,"Liberal");
   mvaddstr(22,0,"<------------------------------------------------------------------------------>");
   move(22,77*publicmood(-1)/100+1);
   addstr("|");
   mvaddstr(23,0,"R - Recreate the Liberal Crime Squad                  Any Other Key - Next Month");
   refresh();
   char c=getch();

   if(c=='r') return false;
   else return true;
}

enum CantSeeReason
{
   CANTSEE_DATING = 1,
   CANTSEE_HIDING = 2,
   CANTSEE_OTHER = 3,
   CANTSEE_DISBANDING = 4
};

void mode_base(void)
{
   short buyer=0;

   char forcewait,canseethings;
   int nonsighttime=0;
   int oldforcemonth=month;

   int length=0;

   int l = 0;

   do
   {
      forcewait=1;
      canseethings=0;
      cantseereason=CANTSEE_OTHER;
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
               if(pool[p]->dating==1 && cantseereason>CANTSEE_DATING) cantseereason=CANTSEE_DATING;
               else if(pool[p]->hiding!=0 && cantseereason>CANTSEE_HIDING) cantseereason=CANTSEE_HIDING;
            }
         }
      }
      else
      {
         cantseereason=CANTSEE_DISBANDING;
      }

      if(disbanding)
      {
         disbanding = show_disbanding_screen(oldforcemonth);
      }

      if(!forcewait)
      {
         if(nonsighttime>=365*4)
         {
            erase();
            char str[100];
            if(nonsighttime>=365*16) {
               strcpy(str,"How long since you've heard these sounds...  times have changed.");
            } else if(nonsighttime>=365*8) {
               strcpy(str,"It has been a long time.  A lot must have changed...");
            } else {
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
         if(location[l]->is_lcs_safehouse())safenumber++;
      }

      Location *loc=NULL;
      if(selectedsiege!=-1)loc = location[selectedsiege];
      if(activesquad!=NULL && activesquad->squad[0]->location!=-1)
         loc = location[activesquad->squad[0]->location];

      siegest *siege=NULL;
      if(loc) siege= &loc->siege;

      char sieged=0;
      char underattack=0;
      if(siege!=NULL)
      {
         sieged=siege->siege;
         if(sieged)
            underattack=siege->underattack;
      }

      char haveflag=0;
      if(loc) haveflag=loc->haveflag;

      // Count people at each location
      int* num_present = new int[location.size()];
      for(int i=0;i<location.size();i++)
      {
         num_present[i]=0;
      }
      for(int p=0;p<pool.size();p++)
      {
         if(!pool[p]->alive)continue; // Dead people don't count
         if(pool[p]->align!=1)continue; // Non-liberals don't count
         if(pool[p]->location==-1)continue; // Vacationers don't count
         num_present[pool[p]->location]++;
      }

      char cannotwait=0;
      for(l=0;l<location.size();l++)
      {
         if(!location[l]->siege.siege)continue;

         if(location[l]->siege.underattack)
         {
            // Allow siege if no liberals present
            if(num_present[l])cannotwait=1;
            break;
         }
      }
      delete[] num_present;

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
               if(loc)stock=loc->compound_stores;
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
            if(location[l]->is_lcs_safehouse())
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
            addstr(getmonth(month,true),gamelog);
            addstr(" ", gamelog);
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
      else if(c=='p'&&ledger.get_funds()>=20&&!sieged&&loc)
      {
         ledger.subtract_funds(20,EXPENSE_COMPOUND);
         if(loc)loc->haveflag=1;
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
