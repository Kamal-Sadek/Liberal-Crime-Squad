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

//#include <includes.h>
#include <externs.h>



bool show_disbanding_screen(int& oldforcemonth)
{
   if(oldforcemonth==month) return true;

   for(int p=pool.size()-1;p>=0;p--)
   {
      int targetjuice=LCSrandom(100*(year-disbandtime+1));
      if(targetjuice>1000||targetjuice<0) targetjuice=1000; // checking for targetjuice<0 checks for int overflow (meaning a REALLY big number)
      if(pool[p]->juice<targetjuice&&pool[p]->hireid!=-1&&!(pool[p]->flag&CREATUREFLAG_SLEEPER))
         pool[p]->alive=0; // Kill for the purposes of disbanding all contacts below
   }
   oldforcemonth=month;
   erase();
   move(0,0);
   char num[20];
   itoa(year,num,10);
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   addstr(getmonth(month));
   addstr(" ");
   addstr(num);

   signed char align=exec[EXEC_PRESIDENT];
   set_alignment_color(align,true);
   mvaddstr(1,0,"President: ");
   addstr(execname[EXEC_PRESIDENT]);addstr(", ");
   switch(align)
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
   for(int h=0;h<435;h++) housemake[house[h]+2]++;
   if(housemake[0]>=218) align=ALIGN_ARCHCONSERVATIVE;
   else if(housemake[0]+housemake[1]>=218) align=ALIGN_CONSERVATIVE;
   else if(housemake[3]+housemake[4]<218) align=ALIGN_MODERATE;
   else if(housemake[4]<218) align=ALIGN_LIBERAL;
   else align=ALIGN_ELITELIBERAL;
   set_alignment_color(align,true);
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
   for(int s=0;s<100;s++) senatemake[senate[s]+2]++;
   senatemake[exec[EXEC_VP]+2]++; // Vice President is tie-breaking vote in the Senate
   if(senatemake[0]>=51) align=ALIGN_ARCHCONSERVATIVE;
   else if(senatemake[0]+senatemake[1]>=51) align=ALIGN_CONSERVATIVE;
   else if(senatemake[3]+senatemake[4]<51) align=ALIGN_MODERATE;
   else if(senatemake[4]<51) align=ALIGN_LIBERAL;
   else align=ALIGN_ELITELIBERAL;
   set_alignment_color(align,true);
   senatemake[exec[EXEC_VP]+2]--; // Vice President isn't actually a Senator though
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
   for(int s=0;s<9;s++) courtmake[court[s]+2]++;
   if(courtmake[0]>=5) align=ALIGN_ARCHCONSERVATIVE;
   else if(courtmake[0]+courtmake[1]>=5) align=ALIGN_CONSERVATIVE;
   else if(courtmake[3]+courtmake[4]<5) align=ALIGN_MODERATE;
   else if(courtmake[4]<5) align=ALIGN_LIBERAL;
   else align=ALIGN_ELITELIBERAL;
   set_alignment_color(align,true);
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

   for(int l=0;l<LAWNUM;l++)
   {
      align=law[l];
      set_alignment_color(align,true);
      move(6+l/3,l%3*30);
      char str[40];
      getlaw(str,l);
      addstr(str);
   }

   int mood=0; // the mood position from 1 to 78 (left=left-wing, right=right=wing)
   for(int v=0;v<VIEWNUM-3;v++) mood+=attitude[v];
   mood=78-(mood*77)/((VIEWNUM-3)*100); // very accurate mood positioning!
   if(mood>=64) align=ALIGN_ARCHCONSERVATIVE;
   else if(mood>=48) align=ALIGN_CONSERVATIVE;
   else if(mood>=32) align=ALIGN_MODERATE;
   else if(mood>=16) align=ALIGN_LIBERAL;
   else align=ALIGN_ELITELIBERAL;
   set_alignment_color(align,true);
   mvaddstr(19,33,"Public Mood");
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   mvaddstr(21,1,"Liberal");
   set_color(COLOR_RED,COLOR_BLACK,1);
   mvaddstr(21,67,"Conservative");
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   mvaddstr(22,0,"\x11ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
   set_color(COLOR_CYAN,COLOR_BLACK,1);
   mvaddstr(22,16,"ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
   set_color(COLOR_YELLOW,COLOR_BLACK,1);
   mvaddstr(22,32,"ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
   set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   mvaddstr(22,48,"ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
   set_color(COLOR_RED,COLOR_BLACK,1);
   mvaddstr(22,64,"ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\x10");
   set_alignment_color(align,true);
   mvaddstr(22,mood,"O");
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   mvaddstr(23,0,"R - Recreate the Liberal Crime Squad                  Any Other Key - Next Month");

   if(getkey()=='r') return false;
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
         for(int p=0;p<(int)pool.size();p++)
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

            getkey();
         }

         nonsighttime=0;
      }

      int partysize=0;
      if(activesquad!=NULL)
      {
         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL)partysize++;
         }
         if(!partysize)
         {
            delete_and_remove(squad,getsquad(activesquad->id));
            activesquad=NULL;
         }
      }

      int safenumber=0;
      for(l=0;l<(int)location.size();l++)if(location[l]->is_lcs_safehouse())safenumber++;

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
      for(int i=0;i<(int)location.size();i++)num_present[i]=0;
      for(int p=0;p<(int)pool.size();p++)
      {
         if(!pool[p]->alive)continue; // Dead people don't count
         if(pool[p]->align!=1)continue; // Non-liberals don't count
         if(pool[p]->location==-1)continue; // Vacationers don't count
         num_present[pool[p]->location]++;
      }

      char cannotwait=0;
      for(l=0;l<(int)location.size();l++)
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
               move(p+10,31);
               if(p<4)
               {
                  set_color(COLOR_WHITE,COLOR_BLUE,1);
                  if(p==0) addstr(":.:.:.:.:");
                  else if(p<3) addstr(":::::::::");
                  else for(int i=0;i<9;i++)addch(CH_LOWER_HALF_BLOCK);
                  set_color(COLOR_WHITE,COLOR_RED,1);
                  for(int i=9;i<18;i++)addch(CH_LOWER_HALF_BLOCK);
               }
               else
               {
                  if(p<6)set_color(COLOR_WHITE,COLOR_RED,1);
                  else set_color(COLOR_RED,COLOR_BLACK,0);
                  for(int i=0;i<18;i++)
                  {
                     if(p==6)addch(CH_UPPER_HALF_BLOCK);
                     else addch(CH_LOWER_HALF_BLOCK);
                  }
               }
            }
         }

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(18,10);
         addstr("ÄÄÄ ACTIVISM ÄÄÄ");
         move(18,51);
         addstr("ÄÄÄ PLANNING ÄÄÄ");

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
            addstr("Ä"); //in case of overlap, at least make it clear where the name ends.
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
         for(int p=0;p < (int)pool.size();p++)
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
               for(int p=0;p<(int)pool.size();p++)
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
      }

      int c='w';
      if(!forcewait)
         c=getkey();

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
            for(int sq=0;sq<(int)squad.size();sq++)
            {
               if(squad[sq]==activesquad)
               {
                  if(sq==(int)squad.size()-1)activesquad=squad[0];
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

         if(sl >= (int)location.size()) sl = 0;

         for(int l=sl;l<(int)location.size();l++)
         {
            if(location[l]->is_lcs_safehouse())
            {
               selectedsiege=l;
               break;
            }
            else if(l==(int)location.size()-1)l=-1;
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
