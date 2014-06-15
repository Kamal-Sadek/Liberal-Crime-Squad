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
// set to use that character set, such as this e with an accent: �

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
      if(targetjuice>1000) targetjuice=1000;
      if(pool[p]->juice<targetjuice&&pool[p]->hireid!=-1&&!(pool[p]->flag&CREATUREFLAG_SLEEPER))
         pool[p]->alive=0; // Kill for the purposes of disbanding all contacts below
   }
   oldforcemonth=month;
   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   mvaddstr(0,0,getmonth(month)+" ");
   addstr(year);

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
   mvaddstr(2,0,"House: ");
   addstr(tostring(housemake[4])+"Lib+, ");
   addstr(tostring(housemake[3])+"Lib, ");
   addstr(tostring(housemake[2])+"Mod, ");
   addstr(tostring(housemake[1])+"Cons, ");
   addstr(tostring(housemake[0])+"Cons+");

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
   mvaddstr(3,0,"Senate: ");
   addstr(tostring(senatemake[4])+"Lib+, ");
   addstr(tostring(senatemake[3])+"Lib, ");
   addstr(tostring(senatemake[2])+"Mod, ");
   addstr(tostring(senatemake[1])+"Cons, ");
   addstr(tostring(senatemake[0])+"Cons+");

   int courtmake[5]={0,0,0,0,0};
   for(int s=0;s<9;s++) courtmake[court[s]+2]++;
   if(courtmake[0]>=5) align=ALIGN_ARCHCONSERVATIVE;
   else if(courtmake[0]+courtmake[1]>=5) align=ALIGN_CONSERVATIVE;
   else if(courtmake[3]+courtmake[4]<5) align=ALIGN_MODERATE;
   else if(courtmake[4]<5) align=ALIGN_LIBERAL;
   else align=ALIGN_ELITELIBERAL;
   set_alignment_color(align,true);
   mvaddstr(4,0,"Supreme Court: ");
   addstr(tostring(courtmake[4])+"Lib+, ");
   addstr(tostring(courtmake[3])+"Lib, ");
   addstr(tostring(courtmake[2])+"Mod, ");
   addstr(tostring(courtmake[1])+"Cons, ");
   addstr(tostring(courtmake[0])+"Cons+");

   for(int l=0;l<LAWNUM;l++)
   {
      align=law[l];
      set_alignment_color(align,true);
      char str[40];
      getlaw(str,l);
      mvaddstr(6+l/3,l%3*30,str);
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
   mvaddstr(22,0,"\x11���������������");
   set_color(COLOR_CYAN,COLOR_BLACK,1);
   mvaddstr(22,16,"����������������");
   set_color(COLOR_YELLOW,COLOR_BLACK,1);
   mvaddstr(22,32,"����������������");
   set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   mvaddstr(22,48,"����������������");
   set_color(COLOR_RED,COLOR_BLACK,1);
   mvaddstr(22,64,"���������������\x10");
   set_alignment_color(align,true);
   mvaddchar(22,mood,'O');
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   mvaddstr(23,0,"R - Recreate the Liberal Crime Squad                  Any Other Key - Next Month");

   return(getkey()!='r');
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
   int nonsighttime=0,oldforcemonth=month,length=0,l=0;

   while(true)
   {
      forcewait=1,canseethings=0,cantseereason=CANTSEE_OTHER;
      if(disbanding)
      {
         cantseereason=CANTSEE_DISBANDING;
         disbanding=show_disbanding_screen(oldforcemonth);
      }
      else
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
                  if(pool[p]->clinic==0) { forcewait=0; break; }
               }
            }
            else
            {
               if(pool[p]->dating==1 && cantseereason>CANTSEE_DATING) cantseereason=CANTSEE_DATING;
               else if(pool[p]->hiding!=0 && cantseereason>CANTSEE_HIDING) cantseereason=CANTSEE_HIDING;
            }
         }
      }

      if(!forcewait)
      {
         if(nonsighttime>=365*4)
         {
            erase();
            char str[100];
            if(nonsighttime>=365*16)
               strcpy(str,"How long since you've heard these sounds...  times have changed.");
            else if(nonsighttime>=365*8)
               strcpy(str,"It has been a long time.  A lot must have changed...");
            else strcpy(str,"It sure has been a while.  Things might have changed a bit.");
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            mvaddstr(12,39-((strlen(str)-1)>>1),str,gamelog);
            gamelog.nextMessage(); //Write out buffer to prepare for the next message.

            getkey();
         }
         nonsighttime=0;
      }

      int partysize=0;
      if(activesquad!=NULL)
      {
         for(int p=0;p<6;p++) if(activesquad->squad[p]!=NULL) partysize++;
         if(!partysize)
         {
            delete_and_remove(squad,getsquad(activesquad->id));
            activesquad=NULL;
         }
      }

      int safenumber=0;
      for(l=0;l<(int)location.size();l++) if(location[l]->is_lcs_safehouse()) safenumber++;

      Location *loc=NULL;
      if(selectedsiege!=-1) loc=location[selectedsiege];
      if(activesquad!=NULL && activesquad->squad[0]->location!=-1)
         loc=location[activesquad->squad[0]->location];

      siegest *siege=NULL;
      if(loc) siege=&loc->siege;

      char sieged=0,underattack=0;
      if(siege!=NULL)
      {
         sieged=siege->siege;
         if(sieged) underattack=siege->underattack;
      }

      char haveflag=0;
      if(loc) haveflag=loc->haveflag;

      // Count people at each location
      int* num_present = new int[location.size()];
      for(int i=0;i<(int)location.size();i++) num_present[i]=0;
      for(int p=0;p<(int)pool.size();p++)
      {  // Dead people, non-liberals, and vacationers don't count
         if(!pool[p]->alive||pool[p]->align!=1||pool[p]->location==-1) continue;
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

         if(activesquad!=NULL) selectedsiege=-1;

         locheader();
         if(selectedsiege!=-1)
         {
            printlocation(selectedsiege);

            if(location[selectedsiege]->can_be_upgraded()&&
               !location[selectedsiege]->siege.siege)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,0);
               mvaddstr(8,1,"I - Invest in this location");
            }
         }
         else if(activesquad!=NULL) printparty();
         else makedelimiter(8,0);

         if(sieged)
         {
            if(underattack)
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               mvaddstr(8,1,"Under Attack");
            }
            else
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               mvaddstr(8,1,"Under Siege");
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
                  else for(int i=0;i<9;i++) addchar((char)CH_LOWER_HALF_BLOCK);
                  set_color(COLOR_WHITE,COLOR_RED,1);
                  for(int i=9;i<18;i++) addchar((char)CH_LOWER_HALF_BLOCK);
               }
               else
               {
                  if(p<6) set_color(COLOR_WHITE,COLOR_RED,1);
                  else set_color(COLOR_RED,COLOR_BLACK,0);
                  for(int i=0;i<18;i++)
                  {
                     if(p==6)addchar((char)CH_UPPER_HALF_BLOCK);
                     else addchar((char)CH_LOWER_HALF_BLOCK);
                  }
               }
            }
         }

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddstr(18,10,"��� ACTIVISM ���");
         mvaddstr(18,51,"��� PLANNING ���");

         if(partysize>0&&!underattack) set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         mvaddstr(19,40,"E - Equip Squad");
         if(vehicle.size()>0&&partysize>0) set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         mvaddstr(19,60,"V - Vehicles");
         if(pool.size()>0) set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         mvaddstr(20,40,"R - Review Assets and Form Squads");

         if(partysize>1) set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         if(partysize>0&&!sieged) mvaddstr(8,30,"O - Reorder");

         if(activesquad)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            mvaddstr(8,1,activesquad->name);
            addchar('�'); //in case of overlap, at least make it clear where the name ends.
         }
         if(squad.size()>1||(activesquad==NULL&&squad.size()>0)) set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         mvaddstr(8,43,"TAB - Next Squad");

         if(safenumber>0) set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         mvaddstr(8,62,"Z - Next Location");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddstr(21,40,"L - The Status of the Liberal Agenda");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddstr(21,1,"A - Activate Liberals");


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
         mvaddstr(21,25,"B - Sleepers");

         if(partysize>0)
         {
            if(activesquad->activity.type!=ACTIVITY_NONE) set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
         }
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         mvaddstr(20,1,"C - Cancel this Squad's Departure");

         if(sieged)
         {
            if(partysize>0) set_color(COLOR_WHITE,COLOR_BLACK,0);
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
            mvaddstr(19,1,"F - Escape/Engage");

            set_color(COLOR_WHITE,COLOR_BLACK,0);
            mvaddstr(19,23,"G - Give Up");
         }
         else
         {
            if(partysize>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            mvaddstr(19,1,"F - Go forth to stop EVIL");
         }

         //if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
         //else set_color(COLOR_BLACK,COLOR_BLACK,1);
         //mvaddstr(19,40,"# - Check the status of a squad Liberal");
         //if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
         //else set_color(COLOR_BLACK,COLOR_BLACK,1);
         //mvaddstr(18,40,"0 - Show the squad's Liberal status");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddstr(23,40,"X - Live to fight EVIL another day");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         if(cannotwait) mvaddstr(23,1,"Cannot Wait until Siege Resolved");
         else
         {
            if(sieged) mvaddstr(23,1,"W - Wait out the siege");
            else mvaddstr(23,1,"W - Wait a day");
            if(day==monthday()) addstr(" (next month)");
         }

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddstr(22,40,"S - FREE SPEECH: the Liberal Slogan");
         if(haveflag)
         {
            if(sieged) set_color(COLOR_GREEN,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            mvaddstr(22,1,"P - PROTEST: burn the flag");
         }
         else
         {
            if(ledger.get_funds()>=20&&!sieged&&(selectedsiege!=-1||activesquad!=NULL))
               set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            mvaddstr(22,1,"P - PATRIOTISM: fly a flag here ($20)");
         }

         length=strlen(slogan);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         if(haveflag) mvaddstr(17,40-(length>>1),slogan);
         else mvaddstr(13,40-(length>>1),slogan);
      }

      switch(int c=forcewait?'w':getkey())
      {
      case 'x': return;
      case 'i': if(selectedsiege!=-1)
         if(location[selectedsiege]->can_be_upgraded()&&!location[selectedsiege]->siege.siege)
            investlocation(); break;
      case 'l': disbanding=liberalagenda(0); break;
      case 'g': if(sieged) { giveup(); cleangonesquads(); } break;
      case 'f': if(!sieged&&partysize>0) stopevil();
         else if(underattack) { escape_engage(); cleangonesquads(); }
         else if(sieged) { sally_forth(); cleangonesquads(); } break;
      case 'o': if(partysize>1) orderparty(); break;
      case 'c': if(partysize>0) activesquad->activity.type=ACTIVITY_NONE; break;
      case 'a': activate(); break;
      case 'b': activate_sleepers(); break;
      case TAB: if(squad.size()>0) {
         if(!activesquad) activesquad=squad[0];
         else for(int sq=0;sq<(int)squad.size();sq++)
            if(squad[sq]==activesquad)
            {
               if(sq==(int)squad.size()-1) activesquad=squad[0];
               else activesquad=squad[sq+1];
               break;
            } } break;
      case 'z': if(safenumber>0) {
         activesquad=NULL;
         for(int l=(selectedsiege==-1||selectedsiege+1>=(int)location.size())?0:selectedsiege+1;
             l<(int)location.size();l++)
            if(location[l]->is_lcs_safehouse()) { selectedsiege=l; break; }
            else if(l==(int)location.size()-1) l=-1; } break;
      case 'e': if(partysize>0&&!underattack&&activesquad->squad[0]->location!=-1) {
		   party_status=-1;
         equip(location[activesquad->squad[0]->location]->loot,-1); } break;
      case 'r': if(pool.size()>0) review(); break;
      case 'w': if(forcewait||!cannotwait) {
         char clearformess=forcewait;
         if(!canseethings) nonsighttime++;
         advanceday(clearformess,canseethings);
         if(day>monthday()) passmonth(clearformess,canseethings);
         advancelocations();
         if(forcewait)
         {
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            mvaddstr(7,5,"Time passes...",gamelog);
            mvaddstr(9,12,getmonth(month,true)+" ",gamelog);
            mvaddstr(9,17,tostring(day)+", ",gamelog);
            mvaddstr(9,21,year,gamelog);
            gamelog.nextMessage(); //Write out buffer to prepare for the next message.
            refresh();
         } } break;
      case 'v': if(vehicle.size()>0&&partysize>0) setvehicles(); break;
      case 'p': if(haveflag) {
         burnflag();
         stat_burns++;
         if(selectedsiege!=-1)
         {
            location[selectedsiege]->haveflag=0;
            if(law[LAW_FLAGBURNING]<1)
               criminalizepool(LAWFLAG_BURNFLAG,-1,selectedsiege);
         }
         if(activesquad)
         {
            location[activesquad->squad[0]->base]->haveflag=0;
            if(law[LAW_FLAGBURNING]<1)
               criminalizepool(LAWFLAG_BURNFLAG,-1,activesquad->squad[0]->base);
         }
         if(sieged)
         {  //PUBLICITY IF BURN FLAG DURING SIEGE ESPECIALLY IF IT IS REALLY ILLEGAL
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
      } else if(ledger.get_funds()>=20&&!sieged&&loc) {
         ledger.subtract_funds(20,EXPENSE_COMPOUND);
         if(loc) loc->haveflag=1;
         stat_buys++; } break;
      case 's': getslogan(); break;
      case '0': party_status=-1; break;
      case '1': case '2': case '3': case '4': case '5': case '6':
         if(activesquad) if(activesquad->squad[c-'1']) {
         if(party_status==c-'1') fullstatus(party_status);
         else party_status=c-'1'; } break;
      }
   }
}
