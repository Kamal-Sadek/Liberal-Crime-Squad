/**
 * Constitutional amendments.
 */
/*
 * Copyright (c) 2002,2003,2004 by Tarn Adams
 * Copyright 2017 Stephen M. Webb
 *
 * This file is part of Liberal Crime Squad.
 *
 * Liberal Crime Squad is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/*
 * This file was created by Chris Johnson (grundee@users.sourceforge.net)
 * by copying code from game.cpp into monthly/endgame.cpp.
 */
#include "politics/amendments.h"
#include <externs.h>

/*
    TODO: I'm not sure if anything in here should be logged. Perhaps only the notification
       that the country has become arch-conservative... --Addictgamer
*/

/**
 * Display the header for announcing constitutional amendments.
 */
static void
amendmentheading()
{
   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,1);

   mvaddstr(0,0,"Proposed Amendment ");
   romannumeral(amendnum);
   addstr(" to the United States Constitution:");
}


/**
 * Check if a constitutional amendment is ratified.
 */
static bool
ratify(int level, int lawview, int view, char congress, bool canseethings)
{
   if(canseethings)
   {
      music.play(MUSIC_ELECTIONS);
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);

      move(0,0);
      addstr("The Ratification Process:");
   }

   //THE STATE VOTE WILL BE BASED ON VIEW OF LAW
   int mood=publicmood(lawview);
   //OR OF A PARTICULAR ISSUE
   if(view>=0) mood=attitude[view];

   //CONGRESS
   bool ratified=false;

   int y=0;

   if(congress)
   {
      ratified=true;

      if(canseethings)
      {
         move(0,62);
         addstr("House");

         move(0,70);
         addstr("Senate");

         move(24,0);
         addstr("Press any key to watch the Congressional votes unfold.     ");

         getkey();
      }

      bool yeswin_h=false,yeswin_s=false;
      int yesvotes_h=0,yesvotes_s=0,vote,s=0;

      for(int l=0;l<HOUSENUM;l++)
      {
         vote=house[l];
         if(vote>=-1&&vote<=1) vote+=LCSrandom(3)-1;

         if(level==vote) yesvotes_h++;

         if(l==HOUSENUM-1) if(yesvotes_h>=HOUSESUPERMAJORITY) yeswin_h=true;

         if(canseethings)
         {
            if(l==HOUSENUM-1&&yeswin_h) set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(l==HOUSENUM-1) set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(2,62);
            addstr(yesvotes_h);
            addstr(" Yea");

            if(l==HOUSENUM-1&&!yeswin_h) set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(l==HOUSENUM-1) set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(3,62);
            addstr(l+1-yesvotes_h);
            addstr(" Nay");
         }

         if(l%4==0&&s<SENATENUM)
         {
            vote=senate[s++];
            if(vote>=-1&&vote<=1) vote+=LCSrandom(3)-1;

            if(level==vote) yesvotes_s++;
         }

         if(l==HOUSENUM-1&&yesvotes_s>=SENATESUPERMAJORITY) yeswin_s=true;

         if(canseethings)
         {
            if(l==HOUSENUM-1&&yeswin_s) set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(l==HOUSENUM-1) set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(2,70);
            addstr(yesvotes_s);
            addstr(" Yea");

            if(l==HOUSENUM-1&&!yeswin_s) set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(l==HOUSENUM-1) set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(3,70);
            addstr(s-yesvotes_s);
            addstr(" Nay");

            if(l%5==0) pause_ms(10);
         }
      }

      if(!yeswin_h||!yeswin_s) ratified=false;

      y+=4;
   }
   else ratified=true;

   if(level==3) level=-2; // special case for Stalinists: do this after Congress but before the states

   //STATES
   if(ratified)
   {
      ratified=false;

      int yesstate=0;

      if(canseethings)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);

         for(int s=0;s<50;s++)
         {
            if(s<17) move(5+s,0);
            else if(s<34) move(5+s-17,27);
            else move(5+s-34,54);
            addstr(statename(s));
         }

         move(24,0);
         addstr("Press any key to watch the State votes unfold.              ");

         getkey();
      }

      int vote,smood;
      for(int s=0;s<STATENUM;s++)
      {
         smood=mood;

         // State biases.
         int multiplier = 5+LCSrandom(3);
         switch(s)
         {
            case 0:smood-=3*multiplier;break;  // Alabama
            case 1:smood-=4*multiplier;break;  // Alaska
            case 2:smood-=1*multiplier;break;  // Arkansas
            case 3:smood-=2*multiplier;break;  // Arizona
            case 4:smood+=4*multiplier;break;  // California
            case 5:break;                      // Colorado
            case 6:smood+=3*multiplier;break;  // Connecticut
            case 7:smood+=3*multiplier;break;  // Delaware
            case 8:break;                      // Florida
            case 9:smood-=2*multiplier;break;  // Georgia
            case 10:smood+=4*multiplier;break; // Hawaii
            case 11:smood-=5*multiplier;break; // Idaho
            case 12:smood+=4*multiplier;break; // Illinois
            case 13:smood-=1*multiplier;break; // Indiana
            case 14:smood+=1*multiplier;break; // Iowa
            case 15:smood-=3*multiplier;break; // Kansas
            case 16:smood-=3*multiplier;break; // Kentucky
            case 17:smood-=1*multiplier;break; // Louisiana
            case 18:smood+=2*multiplier;break; // Maine
            case 19:smood+=3*multiplier;break; // Maryland
            case 20:smood+=6*multiplier;break; // Massachusetts
            case 21:smood+=2*multiplier;break; // Michigan
            case 22:smood+=2*multiplier;break; // Minnesota
            case 23:smood-=4*multiplier;break; // Mississippi
            case 24:smood-=1*multiplier;break; // Missouri
            case 25:smood-=2*multiplier;break; // Montana
            case 26:smood-=3*multiplier;break; // Nebraska
            case 27:break;                     // Nevada
            case 28:smood+=1*multiplier;break; // New Hampshire
            case 29:smood+=3*multiplier;break; // New Jersey
            case 30:smood+=1*multiplier;break; // New Mexico
            case 31:smood+=5*multiplier;break; // New York
            case 32:smood-=1*multiplier;break; // North Carolina
            case 33:smood-=3*multiplier;break; // North Dakota
            case 34:break;                     // Ohio
            case 35:smood-=4*multiplier;break; // Oklahoma
            case 36:smood+=3*multiplier;break; // Oregon
            case 37:smood+=2*multiplier;break; // Pennsylvania
            case 38:smood+=4*multiplier;break; // Rhode Island
            case 39:smood-=5*multiplier;break; // South Carolina
            case 40:smood-=3*multiplier;break; // South Dakota
            case 41:smood-=2*multiplier;break; // Tennessee
            case 42:smood-=4*multiplier;break; // Texas
            case 43:smood-=6*multiplier;break; // Utah
            case 44:smood+=5*multiplier;break; // Vermont
            case 45:break;                     // Virginia
            case 46:smood+=3*multiplier;break; // Washington
            case 47:smood-=2*multiplier;break; // West Virginia
            case 48:smood+=2*multiplier;break; // Wisconsin
            case 49:smood-=5*multiplier;break; // Wyoming
         }

         vote=-2;
         if(LCSrandom(100)<smood)vote++;
         if(LCSrandom(100)<smood)vote++;
         if(LCSrandom(100)<smood)vote++;
         if(LCSrandom(100)<smood)vote++;
         if(vote==1&&!LCSrandom(2)) vote=2;
         if(vote==-1&&!LCSrandom(2)) vote=-2;

         if(canseethings)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            if(s<17) move(5+s,22);
            else if(s<34) move(5+s-17,49);
            else move(5+s-34,76);
         }
         if(vote==level)
         {
            yesstate++;
            if(canseethings) addstr("Yea");
         }
         else if(canseethings) addstr("Nay");

         if(canseethings)
         {
            if(s==STATENUM-1&&yesstate>=STATESUPERMAJORITY) set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(s==STATENUM-1) set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(23,50);
            addstr(yesstate);
            addstr(" Yea");

            if(s==STATENUM-1&&yesstate<STATESUPERMAJORITY) set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(s==STATENUM-1) set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(23,60);
            addstr(s+1-yesstate);
            addstr(" Nay");

            pause_ms(50);
         }
      }

      if(yesstate>=STATESUPERMAJORITY) ratified=true;
   }

   if(canseethings)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(23,0);
      if(ratified) addstr("AMENDMENT ADOPTED.");
      else addstr("AMENDMENT REJECTED.");
   }

   return ratified;
}


/**
 * attempt to pass a constitutional amendment to help win the game.
 */
void
tossjustices(bool canseethings)
{
   int j;
   if(canseethings)
   {
      music.play(MUSIC_ELECTIONS);
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);

      move(12,6);
      addstr("The Elite Liberal Congress is proposing an ELITE LIBERAL AMENDMENT!");

      getkey();
   }

   //STATE THE AMENDMENT
   if(canseethings)
   {
      int tossnum=0;
      for(j=0;j<COURTNUM;j++) if(court[j]!=ALIGN_ELITELIBERAL) tossnum++;

      amendmentheading();

      move(2,5);
      addstr("The following former citizen");
      if(tossnum!=1)addstr("s are");
      else addstr(" is");
      addstr(" branded Arch-Conservative:");

      int y=4;

      for(j=0;j<COURTNUM;j++) if(court[j]!=ALIGN_ELITELIBERAL)
      {
         move(y++,0);
         addstr(courtname[j]);
      }

      move(y+1,5);
      addstr("In particular, the aforementioned former citizen");
      if(tossnum!=1)addstr("s");
      addstr(" may");
      move(y+2,0);
      addstr("not serve on the Supreme Court.  Said former citizen");
      if(tossnum!=1)addstr("s");
      addstr(" will");
      move(y+3,0);
      addstr("be deported to ");
      if(tossnum!=1)addstr("Conservative countries");
      else addstr("a Conservative country");
      addstr(" of the President's");
      move(y+4,0);
      addstr("choosing to be replaced by ");
      if(tossnum!=1)addstr("Proper Justices");
      else addstr("a Proper Justice");
      addstr(", also of");
      move(y+5,0);
      addstr("the President's choosing with the advice and consent of");
      move(y+6,0);
      addstr("the Senate.");


      move(24,0);
      addstr("Press 'C' to watch the ratification process unfold.");

      while(getkey()!='c');
   }

   if(ratify(2,-1,-1,1,canseethings))
   {
      //BLAST JUSTICES
      for(int j=0;j<COURTNUM;j++) if(court[j]!=ALIGN_ELITELIBERAL)
      {
         do generate_name(courtname[j]); while(len(courtname[j])>20);
         court[j]=ALIGN_ELITELIBERAL;
      }

      amendnum++;
   }

   if(canseethings)
   {
      move(24,0);
      addstr("Press any key to reflect on what has happened.");

      getkey();
   }
}

/**
 * Attempt to pass a constitutional amendment to help win the game.
 */
void
amendment_termlimits(bool canseethings)
{
   if(termlimits)return; // Durr~! Don't pass this amendment if it's already passed!
   if(canseethings)
   {
      music.play(MUSIC_ELECTIONS);
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);

      move(12,6);
      addstr("A National Convention has proposed an ELITE LIBERAL AMENDMENT!");

      getkey();
   }

   //STATE THE AMENDMENT
   if(canseethings)
   {
      amendmentheading();

      move(2,5);
      addstr("In light of the Conservative nature of entrenched politicians,");
      move(3,0);
      addstr("and the corrupting influence of incumbency on the democratic process,");
      move(4,0);
      addstr("all members of the House of Representatives and Senate shall henceforth");
      move(5,0);
      addstr("be limited to one term in office.  This shall be immediately enforced");
      move(6,0);
      addstr("by holding elections to replace all members of Congress upon the");
      move(7,0);
      addstr("ratification of this amendment.");

      move(24,0);
      addstr("Press 'C' to watch the ratification process unfold.");

      while(getkey()!='c');
   }

   if(ratify(2,-1,-1,0,canseethings))
   {
      termlimits = true;
      if(canseethings)
      {
         move(24,0);
         addstr("Press any key to hold new elections!                           ");

         getkey();
      }
      elections_senate(0,canseethings);
      elections_senate(1,canseethings);
      elections_senate(2,canseethings);
      elections_house(canseethings);

      amendnum++;
   }
   else if(canseethings)
   {
      move(24,0);
      addstr("Press any key to reflect on what has happened.");

      getkey();
   }
}

/**
 * Attempt to pass a constitutional amendment to lose the game.
 */
void
reaganify(bool canseethings)
{
   if(canseethings)
   {
      music.play(MUSIC_ELECTIONS);
      set_color(COLOR_WHITE,COLOR_BLACK,1);

      erase();
      move(12,3);
      addstr("The Arch-Conservative Congress is proposing an ARCH-CONSERVATIVE AMENDMENT!");

      getkey();

      //STATE THE AMENDMENT
      amendmentheading();

      move(2,5);
      addstr("In recognition of the fact that society is degenerating under");
      move(3,0);
      addstr("the pressure of the elite liberal threat, WE THE PEOPLE HEREBY");
      move(4,0);
      addstr("REPEAL THE CONSTITUTION.  The former United States are to be");
      move(5,0);
      addstr("reorganized into the CONFEDERATED STATES OF AMERICA, with new");
      move(6,0);
      addstr("boundaries to be determined by leading theologians.");
      move(8,5);
      addstr("Ronald Reagan is to be King, forever, even after death.");
      move(10,5);
      addstr("The following Executive Officers are also chosen in perpetuity:");
      move(11,0);
      addstr("Minister of Love Strom Thurmond, Minister of Peace Jesse Helms,");
      move(12,0);
      addstr("and Minister of Truth Jerry Falwell.");
      move(14,5);
      addstr("In the event of the deaths of any of the aforementioned");
      move(15,0);
      addstr("persons, though they shall still nominally hold these posts,");
      move(16,0);
      addstr("actual decisions shall be made by business representatives,");
      move(17,0);
      addstr("chosen by respected business leaders.");
      move(19,5);
      addstr("People may petition Jesus for a redress of grievances, as");
      move(20,0);
      addstr("He will be the only one listening.");
      move(22,5);
      addstr("Have a nice day.");

      move(24,0);
      addstr("Press 'C' to watch the ratification process unfold.");

      while(getkey()!='c');
   }

   if(ratify(-2,-1,-1,1,canseethings))
   {
      music.play(MUSIC_REAGANIFIED);
      if(canseethings)
      {
         move(24,0);
         addstr("Press any key to reflect on what has happened ONE LAST TIME.");

         getkey();
      }

      amendnum = 1; // Constitution repealed...

      //REAGANIFY
      if(canseethings)
      {
         strcpy(execname[EXEC_PRESIDENT],"Ronald Reagan");
         strcpy(execname[EXEC_VP],"Strom Thurmond");
         strcpy(execname[EXEC_STATE],"Jesse Helms");
         strcpy(execname[EXEC_ATTORNEY],"Jerry Falwell");
         for(int e=0;e<EXECNUM;e++) exec[e]=ALIGN_ARCHCONSERVATIVE;
         for(int l=0;l<LAWNUM;l++) law[l]=ALIGN_ARCHCONSERVATIVE;
         liberalagenda(-1);
         savehighscore(END_REAGAN);
      }
      else
      {
         switch(cantseereason)
         {
            case 1:
               //DATING AND REAGANIFIED
               set_color(COLOR_WHITE,COLOR_BLACK,1);

               erase();
               move(12,10);
               addstr("You went on vacation when the country was on the verge of collapse.");

               getkey();

               set_color(COLOR_WHITE,COLOR_BLACK,0);
               erase();
               move(12,12);
               addstr("The Conservatives have made the world in their image.");

               getkey();

               set_color(COLOR_BLACK,COLOR_BLACK,1);
               erase();
               move(12,14);
               addstr("They'll round up the last of you eventually.  All is lost.");

               getkey();

               savehighscore(END_DATING);
               break;
            case 2:
               //HIDING AND REAGANIFIED
               set_color(COLOR_WHITE,COLOR_BLACK,1);

               erase();
               move(12,10);
               addstr("You went into hiding when the country was on the verge of collapse.");

               getkey();

               set_color(COLOR_WHITE,COLOR_BLACK,0);
               erase();
               move(12,12);
               addstr("The Conservatives have made the world in their image.");

               getkey();

               set_color(COLOR_BLACK,COLOR_BLACK,1);
               erase();
               move(12,14);
               addstr("They'll round the last of you up eventually.  All is lost.");

               getkey();

               savehighscore(END_HIDING);
               break;
            case 3:
               //IF YOU ARE ALL IN PRISON, JUST PASS AWAY QUIETLY
               set_color(COLOR_WHITE,COLOR_BLACK,1);

               erase();
               move(12,10);
               addstr("While you were on the inside, the country degenerated...");

               getkey();

               set_color(COLOR_WHITE,COLOR_BLACK,0);
               erase();
               move(12,12);
               addstr("Your kind are never released these days.");

               getkey();

               set_color(COLOR_BLACK,COLOR_BLACK,1);
               erase();
               move(12,14);
               addstr("Ain't no sunshine...");

               getkey();

               savehighscore(END_PRISON);
               break;
            case 4:
               //DISBANDED AND REAGANIFIED
               set_color(COLOR_WHITE,COLOR_BLACK,1);

               erase();
               move(12,10);
               addstr("You disappeared safely, but you hadn't done enough.");

               getkey();

               set_color(COLOR_WHITE,COLOR_BLACK,0);
               erase();
               move(12,12);
               addstr("The Conservatives have made the world in their image.");

               getkey();

               set_color(COLOR_BLACK,COLOR_BLACK,1);
               erase();
               move(12,14);
               addstr("They'll round the last of you up eventually.  All is lost.");

               getkey();

               savehighscore(END_DISBANDLOSS);
               break;
         }
      }
      reset(savefile_name);
      viewhighscores(MUSIC_REAGANIFIED);
      end_game();
   }
   else
   {
      if(canseethings)
      {
         move(24,0);
         addstr("Press any key to breathe a sigh of relief.                   ");

         getkey();
      }
   }
}



/**
 * Attempt to pass a constitutional amendment to lose the game.
 */
void
stalinize(bool canseethings)
{
   if(canseethings)
   {
      music.play(MUSIC_ELECTIONS);
      set_color(COLOR_WHITE,COLOR_BLACK,1);

      erase();
      move(12,11);
      addstr("The Stalinist Congress is proposing a STALINIST AMENDMENT!");

      getkey();

      //STATE THE AMENDMENT
      amendmentheading();

      move(2,5);
      addstr("In recognition of the fact that the proletariat is being exploited");
      move(3,0);
      addstr("by bourgeoisie faux-leftist liberal elites, WE THE PEOPLE HEREBY");
      move(4,0);
      addstr("REPEAL THE CONSTITUTION.  The former United States are to be");
      move(5,0);
      addstr("reorganized into the PEOPLE'S REPUBLIC OF AMERICA, with new");
      move(6,0);
      addstr("boundaries to be determined by leading Stalinist philosophers.");
      move(8,5);
      addstr("Josef Stalin is General Secretary and Premier, forever, even after death.");
      move(10,5);
      addstr("The following Politburo Commissars are also chosen in perpetuity:");
      move(11,0);
      addstr("People's Commissar for Foreign Affairs Vyacheslav Molotov");
      move(12,0);
      addstr("and People's Commissar for Internal Affairs Lavrentiy Beria.");
      move(14,5);
      addstr("In the event of the deaths of any of the aforementioned");
      move(15,0);
      addstr("persons, though they shall still nominally hold these posts,");
      move(16,0);
      addstr("actual decisions shall be made by Stalinist Party leaders,");
      move(17,0);
      addstr("chosen by Stalinist Party loyalists.");
      move(19,5);
      addstr("Anyone attempting to petition for redress of grievances will be sent");
      move(20,0);
      addstr("to a gulag in newly annexed Canada or, if you're lucky, executed.");
      move(22,5);
      addstr("Have a nice day.");

      move(24,0);
      addstr("Press 'C' to watch the ratification process unfold.");

      while(getkey()!='c');
   }

   if(ratify(3,-2,-2,1,canseethings))
   {
      music.play(MUSIC_STALINIZED);
      if(canseethings)
      {
         move(24,0);
         addstr("Press any key to reflect on what has happened ONE LAST TIME.");

         getkey();
      }

      amendnum = 1; // Constitution repealed...

      //STALINIZE
      if(canseethings)
      {
         strcpy(execname[EXEC_PRESIDENT],"Josef Stalin");
         strcpy(execname[EXEC_VP],"Josef Stalin");
         strcpy(execname[EXEC_STATE],"Vyacheslav Molotov");
         strcpy(execname[EXEC_ATTORNEY],"Lavrentiy Beria");
         for(int e=0;e<EXECNUM;e++) exec[e]=ALIGN_STALINIST;
         for(int l=0;l<LAWNUM;l++) law[l]=stalinview(l,true)?ALIGN_ELITELIBERAL:ALIGN_ARCHCONSERVATIVE;
         liberalagenda(-2);
         savehighscore(END_STALIN);
      }
      else
      {
         switch(cantseereason)
         {
            case 1:
               //DATING AND STALINIZED
               set_color(COLOR_WHITE,COLOR_BLACK,1);

               erase();
               move(12,10);
               addstr("You went on vacation when the country was on the verge of collapse.");

               getkey();

               set_color(COLOR_WHITE,COLOR_BLACK,0);
               erase();
               move(12,12);
               addstr("The Stalinists have made the world in their image.");

               getkey();

               set_color(COLOR_BLACK,COLOR_BLACK,1);
               erase();
               move(12,14);
               addstr("They'll round up the last of you eventually.  All is lost.");

               getkey();

               savehighscore(END_DATING);
               break;
            case 2:
               //HIDING AND STALINIZED
               set_color(COLOR_WHITE,COLOR_BLACK,1);

               erase();
               move(12,10);
               addstr("You went into hiding when the country was on the verge of collapse.");

               getkey();

               set_color(COLOR_WHITE,COLOR_BLACK,0);
               erase();
               move(12,12);
               addstr("The Stalinists have made the world in their image.");

               getkey();

               set_color(COLOR_BLACK,COLOR_BLACK,1);
               erase();
               move(12,14);
               addstr("They'll round the last of you up eventually.  All is lost.");

               getkey();

               savehighscore(END_HIDING);
               break;
            case 3:
               //IF YOU ARE ALL IN PRISON, YOU END UP IN A GULAG, COMRADE
               set_color(COLOR_WHITE,COLOR_BLACK,1);

               erase();
               move(12,10);
               addstr("While you were on the inside, the country degenerated...");

               getkey();

               set_color(COLOR_WHITE,COLOR_BLACK,0);
               erase();
               move(12,12);
               addstr("You've been shipped off to a gulag in newly annexed Canada...");

               getkey();

               set_color(COLOR_BLACK,COLOR_BLACK,1);
               erase();
               move(12,14);
               addstr("Ain't no sunshine in this frozen tundra...");

               getkey();

               savehighscore(END_PRISON);
               break;
            case 4:
               //DISBANDED AND STALINIZED
               set_color(COLOR_WHITE,COLOR_BLACK,1);

               erase();
               move(12,10);
               addstr("You disappeared safely, but you hadn't done enough.");

               getkey();

               set_color(COLOR_WHITE,COLOR_BLACK,0);
               erase();
               move(12,12);
               addstr("The Stalinists have made the world in their image.");

               getkey();

               set_color(COLOR_BLACK,COLOR_BLACK,1);
               erase();
               move(12,14);
               addstr("They'll round the last of you up eventually.  All is lost.");

               getkey();

               savehighscore(END_DISBANDLOSS);
               break;
         }
      }
      reset(savefile_name);
      viewhighscores(MUSIC_STALINIZED);
      end_game();
   }
   else
   {
      if(canseethings)
      {
         move(24,0);
         addstr("Press any key to breathe a sigh of relief.                   ");

         getkey();
      }
   }
}

