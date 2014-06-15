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

enum bouncer_reject_reason
{
   REJECTED_CCS,
   REJECTED_NUDE,
   REJECTED_WEAPONS,
   REJECTED_UNDERAGE,
   REJECTED_FEMALEISH,
   REJECTED_FEMALE,
   REJECTED_BLOODYCLOTHES,
   REJECTED_DAMAGEDCLOTHES,
   REJECTED_CROSSDRESSING,
   REJECTED_GUESTLIST,
   REJECTED_DRESSCODE,
   REJECTED_SECONDRATECLOTHES,
   REJECTED_SMELLFUNNY,
   NOT_REJECTED
};

void special_bouncer_greet_squad()
{
   // add a bouncer if there isn't one in the first slot
   if(!sitealarm && location[cursite]->renting!=RENTING_PERMANENT)
   {
      if(location[cursite]->renting==RENTING_CCS)
      {
         makecreature(encounter[0],CREATURE_CCS_VIGILANTE);
         makecreature(encounter[1],CREATURE_CCS_VIGILANTE);
      }
      else if(!encounter[0].exists || encounter[0].type!=CREATURE_BOUNCER)
      {
         makecreature(encounter[0],CREATURE_BOUNCER);
         makecreature(encounter[1],CREATURE_BOUNCER);
      }
   }
}

void special_bouncer_assess_squad()
{
   if(location[cursite]->renting==RENTING_PERMANENT) return;

   bool autoadmit=0;
   char sleepername[80];
   for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;

   special_bouncer_greet_squad();

   for(int p=0;p<(int)pool.size();p++)
   {
      if(pool[p]->base==cursite&&pool[p]->type==CREATURE_BOUNCER)
      {
         autoadmit=1;
         strcpy(sleepername,pool[p]->name);
         strcpy(encounter[0].name,sleepername);
         encounter[0].align=1;
         break;
      }
   }
   //clearmessagearea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   if(autoadmit)
   {
      addstr("Sleeper ", gamelog);
      addstr(sleepername, gamelog);
      addstr(" smirks and lets the squad in.", gamelog);
      gamelog.newline();

      levelmap[locx][locy][locz].special=-1;
   }
   else
   {
      if(location[cursite]->renting==RENTING_CCS)
         addstr("The Conservative scum block the door.", gamelog);
      else
         addstr("The bouncer assesses your squad.", gamelog);
      gamelog.newline();
      levelmap[locx][locy][locz].special=SPECIAL_CLUB_BOUNCER_SECONDVISIT;
   }
   printencounter();

   getkey();

   char rejected=NOT_REJECTED;

   // Size up the squad for entry
   if(!autoadmit)
   {
      for(int s=0;s<6;s++)
      {
         if(activesquad->squad[s])
         {
            // Wrong clothes? Gone
            if(activesquad->squad[s]->is_naked() && activesquad->squad[s]->animalgloss!=ANIMALGLOSS_ANIMAL)
               if(rejected>REJECTED_NUDE)rejected=REJECTED_NUDE;
            if(!hasdisguise(*activesquad->squad[s]))
               if(rejected>REJECTED_DRESSCODE)rejected=REJECTED_DRESSCODE;
            // Busted, cheap, bloody clothes? Gone
            if(activesquad->squad[s]->get_armor().is_bloody())
               if(rejected>REJECTED_BLOODYCLOTHES)rejected=REJECTED_BLOODYCLOTHES;
            if(activesquad->squad[s]->get_armor().is_damaged())
               if(rejected>REJECTED_DAMAGEDCLOTHES)rejected=REJECTED_DAMAGEDCLOTHES;
            if(activesquad->squad[s]->get_armor().get_quality()!=1)
               if(rejected>REJECTED_SECONDRATECLOTHES)rejected=REJECTED_SECONDRATECLOTHES;
            // Suspicious weapons? Gone
            if(weaponcheck(*activesquad->squad[s])>0)
               if(rejected>REJECTED_WEAPONS)rejected=REJECTED_WEAPONS;
            // Fail a tough disguise check? Gone
            if(disguisesite(sitetype) && !(activesquad->squad[s]->skill_check(SKILL_DISGUISE,DIFFICULTY_CHALLENGING)))
               if(rejected>REJECTED_SMELLFUNNY)rejected=REJECTED_SMELLFUNNY;
            // Underage? Gone
            if(activesquad->squad[s]->age<18)
               if(rejected>REJECTED_UNDERAGE)rejected=REJECTED_UNDERAGE;
            // Not a gentleman by their definition?
            if(sitetype==SITE_BUSINESS_CIGARBAR &&
               (activesquad->squad[s]->gender_conservative!=GENDER_MALE ||
                activesquad->squad[s]->gender_liberal == GENDER_FEMALE) &&
                law[LAW_WOMEN]<1)
            {
               // Are you passing as a man? Are you skilled enough to pull it off?
               if(activesquad->squad[s]->gender_liberal == GENDER_FEMALE)
               {
                  // Not a man by your own definition either
                  if(rejected>REJECTED_FEMALE)rejected=REJECTED_FEMALE;
               }
               else if(disguisesite(sitetype) && !(activesquad->squad[s]->skill_check(SKILL_DISGUISE,DIFFICULTY_HARD)) && law[LAW_GAY]!=2)
               {
                  // Not skilled enough to pull it off
                  if(rejected>REJECTED_FEMALEISH)rejected=REJECTED_FEMALEISH;
               }
            }
            // High security in gentleman's club? Gone
            if(sitetype==SITE_BUSINESS_CIGARBAR && location[cursite]->highsecurity)
               if(rejected>REJECTED_GUESTLIST)rejected=REJECTED_GUESTLIST;
            if(location[cursite]->renting==RENTING_CCS && location[cursite]->type!=SITE_BUSINESS_BARANDGRILL)
               rejected=REJECTED_CCS;
         }
      }
      move(17,1);
      switch(rejected)
      {
      case REJECTED_CCS:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(11))
         {
         case 0:addstr("\"Can I see... heh heh... some ID?\"", gamelog);break;
         case 1:addstr("\"Woah... you think you're coming in here?\"", gamelog);break;
         case 2:addstr("\"Check out this fool. Heh.\"", gamelog);break;
         case 3:addstr("\"Want some trouble, dumpster breath?\"", gamelog);break;
         case 4:addstr("\"You're gonna stir up the hornet's nest, fool.\"", gamelog);break;
         case 5:addstr("\"Come on, take a swing at me. Just try it.\"", gamelog);break;
         case 6:addstr("\"You really don't want to fuck with me.\"", gamelog);break;
         case 7:addstr("\"Hey girly, have you written your will?\"", gamelog);break;
         case 8:addstr("\"Oh, you're trouble. I *like* trouble.\"", gamelog);break;
         case 9:addstr("\"I'll bury you in those planters over there.\"", gamelog);break;
         case 10:addstr("\"Looking to check on the color of your blood?\"", gamelog);break;
         }
         break;
      case REJECTED_NUDE:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(4))
         {
         case 0:addstr("\"No shirt, no underpants, no service.\"", gamelog);break;
         case 1:addstr("\"Put some clothes on! That's disgusting.\"", gamelog);break;
         case 2:addstr("\"No! No, you can't come in naked! God!!\"", gamelog);break;
		 case 3:addstr("\"No shoes, no shirt and you don't get service\"", gamelog);break;
         }
         break;
      case REJECTED_UNDERAGE:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(5))
         {
         case 0:addstr("\"Hahaha, come back in a few years.\"", gamelog);break;
         case 1:addstr("\"Find some kiddy club.\"", gamelog);break;
         case 2:addstr("\"You don't look 18 to me.\"", gamelog);break;
         case 3:addstr("\"Go back to your treehouse.\"", gamelog);break;
         case 4:addstr("\"Where's your mother?\"", gamelog);break;
         }
         break;
      case REJECTED_FEMALE:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(4))
         {
         case 0:addstr("\"Move along ma'am, this club's for men.\"", gamelog);break;
         case 1:addstr("\"This 'ain't no sewing circle, ma'am.\"", gamelog);break;
         case 2:addstr("\"Sorry ma'am, this place is only for the men.\"", gamelog);break;
         case 3:addstr("\"Where's your husband?\"", gamelog);break;
         }
         break;
      case REJECTED_FEMALEISH:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(3))
         {
         case 0:addstr("\"You /really/ don't look like a man to me...\"", gamelog);break;
         case 1:addstr("\"Y'know... the \'other\' guys won't like you much.\"", gamelog);break;
         case 2:addstr("\"Uhh... can't let you in, ma'am. Sir. Whatever.\"", gamelog);break;
         }
         break;
      case REJECTED_DRESSCODE:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(3))
         {
         case 0:addstr("\"Check the dress code.\"", gamelog);break;
         case 1:addstr("\"We have a dress code here.\"", gamelog);break;
         case 2:addstr("\"I can't let you in looking like that.\"", gamelog);break;
         }
         break;
      case REJECTED_SMELLFUNNY:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(6))
         {
         case 0:addstr("\"God, you smell.\"", gamelog);break;
         case 1:addstr("\"Not letting you in. Because I said so.\"", gamelog);break;
         case 2:addstr("\"There's just something off about you.\"", gamelog);break;
         case 3:addstr("\"Take a shower.\"", gamelog);break;
         case 4:addstr("\"You'd just harass the others, wouldn't you?\"", gamelog);break;
         case 5:
                if(law[LAW_FREESPEECH]==-2)addstr("\"Get the [heck] out of here.\"", gamelog);
                else if(law[LAW_FREESPEECH]==2)addstr("\"Get the fuck out of here.\"", gamelog);
                else addstr("\"Get the hell out of here.\"", gamelog);break;
         }
         break;
      case REJECTED_BLOODYCLOTHES:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(5))
         {
         case 0:addstr("\"Good God! What is wrong with your clothes?\"", gamelog);break;
         case 1:addstr("\"Absolutely not. Clean up a bit.\"", gamelog);break;
         case 2:addstr("\"This isn't a goth club, bloody clothes don't cut it here.\"", gamelog);break;
         case 3:addstr("\"Uh, maybe you should wash... replace... those clothes.\"", gamelog);break;
         case 4:addstr("\"Did you spill something on your clothes?\"", gamelog);break;
         case 5:addstr("\"Come back when you get the red wine out of your clothes.\"", gamelog);break;
         }
         break;
      case REJECTED_DAMAGEDCLOTHES:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(2))
         {
         case 0:addstr("\"Good God! What is wrong with your clothes?\"", gamelog);break;
         case 1:addstr("\"This isn't a goth club, ripped clothes don't cut it here.\"", gamelog);break;
         }
         break;
      case REJECTED_SECONDRATECLOTHES:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(2))
         {
         case 0:addstr("\"That looks like you sewed it yourself.\"", gamelog);break;
         case 1:addstr("\"If badly cut clothing is a hot new trend, I missed it.\"", gamelog);break;
         }
         break;
      case REJECTED_WEAPONS:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(5))
         {
         case 0:addstr("\"No weapons allowed.\"", gamelog);break;
         case 1:addstr("\"I can't let you in carrying that.\"", gamelog);break;
         case 2:addstr("\"I can't let you take that in.\"", gamelog);break;
         case 3:addstr("\"Come to me armed, and I'll tell you to take a hike.\"", gamelog);break;
         case 4:addstr("\"Real men fight with fists. And no, you can't come in.\"", gamelog);break;
         }
         break;
      case REJECTED_GUESTLIST:
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr("\"This club is by invitation only.\"", gamelog);
         break;
      case NOT_REJECTED:
         set_color(COLOR_GREEN,COLOR_BLACK,1);

         switch(LCSrandom(4))
         {
         case 0:addstr("\"Keep it civil and don't drink too much.\"", gamelog);break;
         case 1:addstr("\"Let me get the door for you.\"", gamelog);break;
         case 2:addstr("\"Ehh, alright, go on in.\"", gamelog);break;
         case 3:addstr("\"Come on in.\"", gamelog);break;
         }
         break;
      }
      gamelog.newline();

      getkey();
   }
   else encounter[0].exists=0;
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   for(int dx=-1; dx<=1; dx++)
   for(int dy=-1; dy<=1; dy++)
   {
      if(levelmap[locx+dx][locy+dy][locz].flag & SITEBLOCK_DOOR)
      {
         if(rejected<NOT_REJECTED)
         {
            levelmap[locx+dx][locy+dy][locz].flag |= SITEBLOCK_LOCKED;
            levelmap[locx+dx][locy+dy][locz].flag |= SITEBLOCK_CLOCK;
         }
         else levelmap[locx+dx][locy+dy][locz].flag &= ~SITEBLOCK_DOOR;
      }
   }
   encounter[0].cantbluff=1;


}

void special_lab_cosmetics_cagedanimals(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You see fluffy white rabbits in a locked cage.", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("Free them? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         char actual;

         if(unlock(UNLOCK_CAGE,actual))
         {
            int time=20+LCSrandom(10);
            if(time<1)time=1;
            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

            sitecrime++;
            juiceparty(3,100);
            sitestory->crime.push_back(CRIME_FREE_RABBITS);
            criminalizeparty(LAWFLAG_VANDALISM);
         }

         if(actual)
         {
            alienationcheck(0);
            noticecheck(-1);
            levelmap[locx][locy][locz].special=-1;
         }

         return;
      }
      else if(c=='n')return;

   }while(true);
}



void special_readsign(int sign)
{
   clearmessagearea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);

   switch(sign) //TODO: Log these?
   {
   case SPECIAL_SIGN_ONE:
      switch(location[cursite]->type)
      {
      default:
         move(16,1);
         addstr("\"The best way not to fail is to succeed.\"");
         break;
      case SITE_INDUSTRY_NUCLEAR:
         move(16,1);
         addstr("Welcome to the NPP Nuclear Plant. Please enjoy");
         move(17,1);
         addstr("the museum displays in the gift shop.");
         break;
      case SITE_RESIDENTIAL_TENEMENT:
      case SITE_RESIDENTIAL_APARTMENT:
      case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
         move(16,1);
         addstr("The landlord's office is the first door");
         move(17,1);
         addstr("on the left.");
         break;
      }
      break;
   case SPECIAL_SIGN_TWO:
      switch(location[cursite]->type)
      {
      default:
         move(16,1);
         addstr("\"Great work is done by people who do great work.\"");
         break;
      }
      break;
   case SPECIAL_SIGN_THREE:
      switch(location[cursite]->type)
      {
      default:
         move(16,1);
         addstr("Employees Only");
         break;
      }
      break;
   }

   getkey();
}



void special_nuclear_onoff(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      if(law[LAW_NUCLEARPOWER]==2)
      {
         move(16,1);
         addstr("You see the nuclear waste center control room.", gamelog);
         gamelog.newline();
         move(17,1);
         addstr("Attempt to release nuclear waste? (Yes or No)");
      }
      else
      {
         move(16,1);
         addstr("You see the nuclear power plant control room.", gamelog);
         gamelog.newline();
         move(17,1);
         addstr("Mess with the reactor settings? (Yes or No)");
      }

      int c=getkey();

      if(c=='y')
      {
         clearmessagearea();
         levelmap[locx][locy][locz].special=-1;

         char max=DIFFICULTY_HARD;
         Creature* maxs=0;

         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL&&activesquad->squad[p]->alive)
            {
               if(activesquad->squad[p]->skill_check(SKILL_SCIENCE,max))
               {
                  maxs=activesquad->squad[p];
                  break;
               }
            }
         }

         if(maxs)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr(maxs->name, gamelog);
            addstr(" presses the big red button!", gamelog);
            gamelog.newline();

            getkey();

            move(17,1);
            addstr(".", gamelog);

            getkey();

            addstr(".", gamelog);

            getkey();

            addstr(".", gamelog);

            getkey();

            if(law[LAW_NUCLEARPOWER]==2)
            {
               move(17,1);
               addstr("The nuclear waste gets released into the state's water supply!", gamelog);
               gamelog.newline();
               change_public_opinion(VIEW_NUCLEARPOWER,15,0,95);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,-50,0,0);

               getkey();

               juiceparty(40,1000); // Instant juice!
			      sitecrime+=25; //Shutdown Site

               sitestory->crime.push_back(CRIME_SHUTDOWNREACTOR);

            }
            else
            {
               move(16,1);
               addstr("A deafening alarm sounds!", gamelog);
               gamelog.newline();
               addstr("                "); // Remove remaining part of previous text.
               move(17,1);
               addstr("The reactor is overheating!", gamelog);
               gamelog.newline();
               change_public_opinion(VIEW_NUCLEARPOWER,15,0,95);

               getkey();

               juiceparty(100,1000); // Instant juice!
			      sitecrime+=50; //Shutdown Site

               sitestory->crime.push_back(CRIME_SHUTDOWNREACTOR);
            }
         }
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("After some failed attempts, and a very loud alarm, ", gamelog);
            move(17,1);
            addstr("the Squad resigns to just leaving a threatening note.", gamelog);
            gamelog.newline();

            getkey();

            juiceparty(15,500);
         }
         sitealarm=1;
         alienationcheck(1);
         levelmap[locx][locy][locz].special=-1;
         sitecrime+=5;
         criminalizeparty(LAWFLAG_TERRORISM);

         return;
      }
      else if(c=='n')return;

   }while(true);
}



void special_lab_genetic_cagedanimals(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You see horrible misshapen creatures in a sealed cage.", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("Free them? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         char actual;

         if(unlock(UNLOCK_CAGE_HARD,actual))
         {
            int time=20+LCSrandom(10);
            if(time<1)time=1;
            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

            sitecrime++;
            juiceparty(5,200);
            sitestory->crime.push_back(CRIME_FREE_BEASTS);
            criminalizeparty(LAWFLAG_VANDALISM);

            if(!LCSrandom(2))
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("Uh, maybe that idea was Conservative in retrospect...", gamelog);
               gamelog.newline();

               int numleft=LCSrandom(6)+1;
               for(int e=0;e<ENCMAX;e++)
               {
                  if(!encounter[e].exists)
                  {
                     makecreature(encounter[e],CREATURE_GENETIC);
                     numleft--;
                  }
                  if(numleft==0)break;
               }

               if(mode==GAMEMODE_CHASECAR||
                  mode==GAMEMODE_CHASEFOOT)printchaseencounter();
               else printencounter();

               getkey();

               sitealarm=1;
               alienationcheck(1);
            }
            else
            {
               alienationcheck(0);
            }
         }
         else if(actual)
         {
            noticecheck(-1);
         }

         if(actual)
         {
            levelmap[locx][locy][locz].special=-1;
         }

         return;
      }
      else if(c=='n')return;

   }while(true);
}



void special_policestation_lockup(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You see prisoners in the detention room.", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("Free them? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         char actual;

         if(unlock(UNLOCK_CELL,actual))
         {
            int numleft=LCSrandom(8)+2;
            for(int e=0;e<ENCMAX;e++)
            {
               if(!encounter[e].exists)
               {
                  makecreature(encounter[e],CREATURE_PRISONER);
                  numleft--;
               }
               if(numleft==0)break;
            }

            juiceparty(50,1000);
            sitecrime+=20;

            int time=20+LCSrandom(10);
            if(time<1)time=1;
            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

            if(mode==GAMEMODE_CHASECAR||
                  mode==GAMEMODE_CHASEFOOT)printchaseencounter();
            else printencounter();
            refresh();

            partyrescue(SPECIAL_POLICESTATION_LOCKUP);
         }

         if(actual)
         {
            alienationcheck(1);
            noticecheck(-1,DIFFICULTY_HARD);
            levelmap[locx][locy][locz].special=-1;
            sitecrime+=2;
            sitestory->crime.push_back(CRIME_POLICE_LOCKUP);
            criminalizeparty(LAWFLAG_HELPESCAPE);
         }

         return;
      }
      else if(c=='n')return;

   }while(true);
}



void special_courthouse_lockup(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You see prisoners in the Courthouse jail.", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("Free them? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         char actual;

         if(unlock(UNLOCK_CELL,actual))
         {
            int numleft=LCSrandom(8)+2;
            for(int e=0;e<ENCMAX;e++)
            {
               if(!encounter[e].exists)
               {
                  makecreature(encounter[e],CREATURE_PRISONER);
                  numleft--;
               }
               if(numleft==0)break;
            }

            juiceparty(50,1000);
            sitecrime+=20;

            int time=20+LCSrandom(10);
            if(time<1)time=1;
            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

            if(mode==GAMEMODE_CHASECAR||
                  mode==GAMEMODE_CHASEFOOT)printchaseencounter();
            else printencounter();
            refresh();

            partyrescue(SPECIAL_COURTHOUSE_LOCKUP);
         }

         if(actual)
         {
            alienationcheck(1);
            noticecheck(-1,DIFFICULTY_HARD);
            levelmap[locx][locy][locz].special=-1;
            sitecrime+=3;
            sitestory->crime.push_back(CRIME_COURTHOUSE_LOCKUP);
            criminalizeparty(LAWFLAG_HELPESCAPE);
         }

         return;
      }
      else if(c=='n')return;

   }while(true);
}



void special_courthouse_jury(void)
{
   int p;
   if(sitealarm||sitealienate)
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("It appears as if this room has been ", gamelog);
      move(17,1);
      addstr("vacated in a hurry.", gamelog);
      gamelog.newline();

      getkey();

      return;
   }

   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You've found a Jury in deliberations!", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("Attempt to influence them? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         levelmap[locx][locy][locz].special=-1;

         char succeed=0;

         int maxattack=0;
         int maxp = -1;

         for(p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL)
            {
               if(activesquad->squad[p]->alive)
               {
                  if(activesquad->squad[p]->get_attribute(ATTRIBUTE_CHARISMA,true)+
                     activesquad->squad[p]->get_attribute(ATTRIBUTE_INTELLIGENCE,true)+
                     activesquad->squad[p]->get_skill(SKILL_PERSUASION)+
                     activesquad->squad[p]->get_skill(SKILL_LAW)>maxattack)
                  {
                     maxattack = activesquad->squad[p]->get_attribute(ATTRIBUTE_CHARISMA,true)+
                                 activesquad->squad[p]->get_attribute(ATTRIBUTE_INTELLIGENCE,true)+
                                 activesquad->squad[p]->get_skill(SKILL_PERSUASION)+
                                 activesquad->squad[p]->get_skill(SKILL_LAW);
                     maxp = p;
                  }
               }
            }
         }

         if(maxp > -1)
         {
            int p=maxp;

            activesquad->squad[p]->train(SKILL_PERSUASION,20);
            activesquad->squad[p]->train(SKILL_LAW,20);

            if(activesquad->squad[p]->skill_check(SKILL_PERSUASION,DIFFICULTY_HARD) &&
               activesquad->squad[p]->skill_check(SKILL_LAW,DIFFICULTY_CHALLENGING))succeed=1;

            if(succeed)
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr(activesquad->squad[p]->name, gamelog);
               addstr(" works the room like in Twelve Angry Men, and the jury ", gamelog);
               move(17,1);
               addstr("concludes that ", gamelog);//XXX: This is very awkward grammar.
               switch(LCSrandom(16))     // Fixed. -Fox
               {
                  case 0:addstr("murder", gamelog);break;
                  case 1:addstr("assault", gamelog);break;
                  case 2:addstr("theft", gamelog);break;
                  case 3:addstr("mugging", gamelog);break;
                  case 4:addstr("burglary", gamelog);break;
                  case 5:addstr("property destruction", gamelog);break;
                  case 6:addstr("vandalism", gamelog);break;
                  case 7:addstr("libel", gamelog);break;
                  case 8:addstr("slander", gamelog);break;
                  case 9:addstr("sodomy", gamelog);break;
                  case 10:addstr("obstruction of justice", gamelog);break;
                  case 11:addstr("breaking and entering", gamelog);break;
                  case 12:addstr("public indecency", gamelog);break;
                  case 13:addstr("arson", gamelog);break;
                  case 14:addstr("resisting arrest", gamelog);break;
                  case 15:addstr("tax evasion", gamelog);break;
               }
               addstr(" wasn't really wrong here.", gamelog);
               gamelog.newline();

               getkey();

               alienationcheck(0);
               noticecheck(-1);

               //INSTANT JUICE BONUS
               addjuice(*(activesquad->squad[p]),25,200);
            }
            else
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr(activesquad->squad[p]->name, gamelog);

               addstr(" wasn't quite convincing...", gamelog);
               gamelog.newline();

               getkey();

               int numleft=12;
               for(int e=0;e<ENCMAX;e++)
               {
                  if(!encounter[e].exists)
                  {
                     makecreature(encounter[e],CREATURE_JUROR);
                     numleft--;
                  }
                  if(numleft==0)break;
               }

               if(mode==GAMEMODE_CHASECAR||
                  mode==GAMEMODE_CHASEFOOT)printchaseencounter();
               else printencounter();
               refresh();

               sitealarm=1;
               sitealienate=2;
               sitecrime+=10;
               sitestory->crime.push_back(CRIME_JURYTAMPERING);

               criminalizeparty(LAWFLAG_JURY);
            }
         }

         return;
      }
      else if(c=='n')return;

   }while(true);
}



void special_prison_control(short prison_control_type)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You've found the ", gamelog);
      if(prison_control_type==SPECIAL_PRISON_CONTROL_LOW)
         addstr("low security ", gamelog);
      else if(prison_control_type==SPECIAL_PRISON_CONTROL_MEDIUM)
         addstr("medium security ", gamelog);
      else if(prison_control_type==SPECIAL_PRISON_CONTROL_HIGH)
         addstr("high security ", gamelog);
      addstr("prison control room.", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("Free the prisoners? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         int numleft=LCSrandom(8)+2;
         if(prison_control_type==SPECIAL_PRISON_CONTROL_LOW)
         {
            switch(law[LAW_DEATHPENALTY])
            {
               case -1: numleft=LCSrandom(6)+2;break;
               case -2: numleft=LCSrandom(3)+1;break;
            }
         }
         else if(prison_control_type==SPECIAL_PRISON_CONTROL_MEDIUM)
         {
            switch(law[LAW_DEATHPENALTY])
            {
               case 2: numleft=LCSrandom(4)+1;
               case 1: numleft=LCSrandom(6)+1;
            }
         }
         else if(prison_control_type==SPECIAL_PRISON_CONTROL_HIGH)
         {
            switch(law[LAW_DEATHPENALTY])
            {
               case  2: numleft=0;break;
               case  1: numleft=LCSrandom(4);break;
               case -1: numleft+=LCSrandom(4);break;
               case -2: numleft+=LCSrandom(4)+2;break;
            }
         }

         for(int e=0;e<ENCMAX;e++)
         {
            if(!encounter[e].exists)
            {
               makecreature(encounter[e],CREATURE_PRISONER);
               numleft--;
            }
            if(numleft==0)break;
         }

         int time=20+LCSrandom(10);
         if(time<1)time=1;
         if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

         if(mode==GAMEMODE_CHASECAR||
                  mode==GAMEMODE_CHASEFOOT)printchaseencounter();
         else printencounter();
         refresh();

         partyrescue(prison_control_type);

         alienationcheck(1);
         noticecheck(-1);
         levelmap[locx][locy][locz].special=-1;
         sitecrime+=30;
         juiceparty(50,1000);
         sitestory->crime.push_back(CRIME_PRISON_RELEASE);
         criminalizeparty(LAWFLAG_HELPESCAPE);

         return;
      }
      else if(c=='n')return;

   }while(true);
}



void special_intel_supercomputer(void)
{
   if(sitealarm||sitealienate)
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("The security alert has caused the ", gamelog);
      move(17,1);
      addstr("computer to shut down.", gamelog);
      gamelog.newline();

      getkey();

      return;
   }

   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You've found the Intelligence Supercomputer.", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("Hack it? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         char actual;

         if(hack(HACK_SUPERCOMPUTER,actual))
         {
            clearmessagearea();

            //char *loot;
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("The Squad obtains sensitive information", gamelog);
            if(endgamestate>=ENDGAME_CCS_APPEARANCE && endgamestate < ENDGAME_CCS_DEFEATED && ccsexposure<CCSEXPOSURE_LCSGOTDATA)
            {
               addstr(",", gamelog);
               move(17,1);
               addstr("including a list of government backers of the CCS.", gamelog);

               Item *it=new Loot(*loottype[getloottype("LOOT_CCS_BACKERLIST")]);
               activesquad->loot.push_back(it);

               ccsexposure=CCSEXPOSURE_LCSGOTDATA;
            }
            else
            {
               addstr(".", gamelog);
            }
            gamelog.newline();

            juiceparty(50,1000);

            Item *it=new Loot(*loottype[getloottype("LOOT_INTHQDISK")]);
            activesquad->loot.push_back(it);

            getkey();
         }

         if(actual)
         {
            int time=20+LCSrandom(10);
            if(time<1)time=1;
            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

            alienationcheck(1);
            noticecheck(-1,DIFFICULTY_HARD);
            levelmap[locx][locy][locz].special=-1;
            sitecrime+=3;
            sitestory->crime.push_back(CRIME_HACK_INTEL);

            criminalizeparty(LAWFLAG_TREASON);
         }

         return;
      }
      else if(c=='n')return;

   }while(true);
}


void special_graffiti(void)
{
   clearmessagearea();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   addstr("The squad sprays Liberal Graffiti!", gamelog);
   gamelog.newline();

   if(!sitestory->claimed)
      sitestory->claimed=1;

   getkey();

   int time=20+LCSrandom(10);
   if(time<1)time=1;
   if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

   alienationcheck(0);
   noticecheck(-1,DIFFICULTY_HARD);
   levelmap[locx][locy][locz].flag|=SITEBLOCK_GRAFFITI;
   levelmap[locx][locy][locz].flag&=~(SITEBLOCK_GRAFFITI_CCS|SITEBLOCK_GRAFFITI_OTHER);
   if(!location[cursite]->highsecurity)
   {
      // Erase any previous semi-permanent graffiti here
      for(int i=0;i<(int)location[cursite]->changes.size();i++)
      {
         if((location[cursite]->changes[i].x == locx) &&
            (location[cursite]->changes[i].y == locy) &&
            (location[cursite]->changes[i].z == locz) &&
            ((location[cursite]->changes[i].flag == SITEBLOCK_GRAFFITI) ||
             (location[cursite]->changes[i].flag == SITEBLOCK_GRAFFITI_CCS) ||
             (location[cursite]->changes[i].flag == SITEBLOCK_GRAFFITI_OTHER)))
         {
            location[cursite]->changes.erase(location[cursite]->changes.begin()+i);
            break;
         }
      }

      // Add new semi-permanent graffiti
      struct sitechangest change(locx,locy,locz,SITEBLOCK_GRAFFITI);
      location[cursite]->changes.push_back(change);
   }
   sitecrime++;
   for(int i=0;i<6;i++)
   {
      if(activesquad->squad[i])
         addjuice(*(activesquad->squad[i]),1,50);
   }

   criminalizeparty(LAWFLAG_VANDALISM);
   sitestory->crime.push_back(CRIME_TAGGING);

   return;
}


void special_sweatshop_equipment(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You see some textile equipment.", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("Destroy it? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         int time=20+LCSrandom(10);
         if(time<1)time=1;
         if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

         alienationcheck(0);
         noticecheck(-1,DIFFICULTY_HEROIC);
         levelmap[locx][locy][locz].special=-1;
         levelmap[locx][locy][locz].flag|=SITEBLOCK_DEBRIS;
         sitecrime++;
         juiceparty(5,100);
         sitestory->crime.push_back(CRIME_BREAK_SWEATSHOP);

         criminalizeparty(LAWFLAG_VANDALISM);

         return;
      }
      else if(c=='n')return;

   }while(true);
}



void special_polluter_equipment(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You see some industrial equipment.", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("Destroy it? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         int time=20+LCSrandom(10);
         if(time<1)time=1;
         if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

         change_public_opinion(VIEW_POLLUTION,2,1,70);

         alienationcheck(0);
         noticecheck(-1,DIFFICULTY_HEROIC);
         levelmap[locx][locy][locz].special=-1;
         levelmap[locx][locy][locz].flag|=SITEBLOCK_DEBRIS;
         sitecrime+=2;
         juiceparty(5,100);
         sitestory->crime.push_back(CRIME_BREAK_FACTORY);

         criminalizeparty(LAWFLAG_VANDALISM);

         return;
      }
      else if(c=='n')return;

   }while(true);
}



void special_house_photos(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You've found a safe.", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("Open it? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         char actual;

         if(unlock(UNLOCK_SAFE,actual))
         {
            bool empty=true;
            Item *it;

            if(deagle==false)
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("The squad has found a Desert Eagle.", gamelog);
               gamelog.newline();

               getkey();

               Weapon* de=new Weapon(*weapontype[getweapontype("WEAPON_DESERT_EAGLE")]);
               Clip r(*cliptype[getcliptype("CLIP_50AE")]);
               de->reload(r);
               activesquad->loot.push_back(de);

               it=new Clip(*cliptype[getcliptype("CLIP_50AE")],9);
               activesquad->loot.push_back(it);

               deagle=true;
               empty=false;
            }

            if(LCSrandom(2))
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("This guy sure had a lot of $100 bills.", gamelog);
               gamelog.newline();

               getkey();

               it=new Money(1000*(1+LCSrandom(10)));
               activesquad->loot.push_back(it);

               empty=false;
            }

            if(LCSrandom(2))
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("The squad Liberates some expensive jewelery.", gamelog);
               gamelog.newline();

               getkey();

               it=new Loot(*loottype[getloottype("LOOT_EXPENSIVEJEWELERY")],3);
               activesquad->loot.push_back(it);

               empty=false;
            }

            if(!LCSrandom(3))
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("There are some... very compromising photos here.", gamelog);
               gamelog.newline();

               getkey();

               it=new Loot(*loottype[getloottype("LOOT_CEOPHOTOS")]);
               activesquad->loot.push_back(it);

               empty=false;
            }

            if(!LCSrandom(3))
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("There are some drugs here.", gamelog);
               gamelog.newline();

               getkey();

               empty=false;
            }

            if(!LCSrandom(3))
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("Wow, get a load of these love letters. ", gamelog);
               move(17,1);
               addstr("The squad will take those.");
               gamelog.newline();

               getkey();

               it=new Loot(*loottype[getloottype("LOOT_CEOLOVELETTERS")]);
               activesquad->loot.push_back(it);

               empty=false;
            }

            if(!LCSrandom(3))
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("These documents show serious tax evasion.", gamelog);
               gamelog.newline();

               getkey();

               it=new Loot(*loottype[getloottype("LOOT_CEOTAXPAPERS")]);
               activesquad->loot.push_back(it);

               empty=false;
            }

            if(empty)
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("Wow, it's empty.  That sucks.", gamelog);
               gamelog.newline();

               getkey();
            }
            else
            {
               juiceparty(50,1000);
               sitecrime+=40;
               sitestory->crime.push_back(CRIME_HOUSE_PHOTOS);
               criminalizeparty(LAWFLAG_THEFT);

               int time=20+LCSrandom(10);
               if(time<1)time=1;
               if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;
            }
         }

         if(actual)
         {
            alienationcheck(0);
            noticecheck(-1);
            levelmap[locx][locy][locz].special=-1;
         }

         return;
      }
      else if(c=='n')return;

   }while(true);
}


void special_armory(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You've found the armory.", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("Break in? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         clearmessagearea();

         sitealarm=1;
         move(16,1);
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr("Alarms go off!", gamelog);
         gamelog.newline();

         getkey();

         bool empty=true;
         Item *it;

         if(m249==false && location[cursite]->type == SITE_GOVERNMENT_ARMYBASE)
         {
            clearmessagearea();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("Jackpot! The squad found a M249 Machine Gun!", gamelog);
            gamelog.newline();

            getkey();

            Weapon* de=new Weapon(*weapontype[getweapontype("WEAPON_M249_MACHINEGUN")]);
            Clip r(*cliptype[getcliptype("CLIP_DRUM")]);
            de->reload(r);
            activesquad->loot.push_back(de);

            it=new Clip(*cliptype[getcliptype("CLIP_DRUM")],9);
            activesquad->loot.push_back(it);

            m249=true;
            empty=false;
         }

         if(LCSrandom(2))
         {
            clearmessagearea();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("The squad finds some M16 Assault Rifles.", gamelog);
            gamelog.newline();

            getkey();

            int num = 0;

            do
            {
               Weapon* de=new Weapon(*weapontype[getweapontype("WEAPON_AUTORIFLE_M16")]);
               Clip r(*cliptype[getcliptype("CLIP_ASSAULT")]);
               de->reload(r);
               activesquad->loot.push_back(de);

               it=new Clip(*cliptype[getcliptype("CLIP_ASSAULT")],5);
               activesquad->loot.push_back(it);
               num++;
            }
            while(num<2 || (LCSrandom(2) && num<5));

            empty=false;
         }

         if(LCSrandom(2))
         {
            clearmessagearea();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("The squad finds some M4 Carbines.", gamelog);
            gamelog.newline();

            getkey();

            int num = 0;

            do
            {
               Weapon* de=new Weapon(*weapontype[getweapontype("WEAPON_CARBINE_M4")]);
               Clip r(*cliptype[getcliptype("CLIP_ASSAULT")]);
               de->reload(r);
               activesquad->loot.push_back(de);

               it=new Clip(*cliptype[getcliptype("CLIP_ASSAULT")],5);
               activesquad->loot.push_back(it);
               num++;
            }
            while(num<2 || (LCSrandom(2) && num<5));

            empty=false;
         }

         if(LCSrandom(2))
         {
            clearmessagearea();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("The squad finds some body armor.", gamelog);
            gamelog.newline();

            getkey();

            int num = 0;

            do
            {
               Armor* de;
               if(location[cursite]->type == SITE_GOVERNMENT_ARMYBASE)
                  de=new Armor(*armortype[getarmortype("ARMOR_ARMYARMOR")]);
               else
                  de=new Armor(*armortype[getarmortype("ARMOR_CIVILLIANARMOR")]);
               activesquad->loot.push_back(de);
               num++;
            }
            while(num<2 || (LCSrandom(2) && num<5));

            empty=false;
         }

         if(empty)
         {
            criminalizeparty(LAWFLAG_TREASON);

            clearmessagearea();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("It's a trap!  The armory is empty.", gamelog);
            gamelog.newline();

            getkey();

            int numleft=LCSrandom(8)+2;
            for(int e=0;e<ENCMAX;e++)
            {
               if(!encounter[e].exists)
               {
                  if(location[cursite]->type == SITE_GOVERNMENT_ARMYBASE)
                     makecreature(encounter[e],CREATURE_SOLDIER);
                  else
                     makecreature(encounter[e],CREATURE_MERC);
                  numleft--;
               }
               if(numleft==0)break;
            }
         }
         else
         {
            juiceparty(50,1000);
            sitecrime+=40;
            sitestory->crime.push_back(CRIME_ARMORY);
            criminalizeparty(LAWFLAG_THEFT);
            criminalizeparty(LAWFLAG_TREASON);

            int time=20+LCSrandom(10);
            if(time<1)time=1;
            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

            clearmessagearea();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("Guards are everywhere!", gamelog);
            gamelog.newline();

            getkey();

            int numleft=LCSrandom(4)+2;
            for(int e=0;e<ENCMAX;e++)
            {
               if(!encounter[e].exists)
               {
                  if(location[cursite]->type == SITE_GOVERNMENT_ARMYBASE)
                     makecreature(encounter[e],CREATURE_SOLDIER);
                  else
                     makecreature(encounter[e],CREATURE_MERC);
                  numleft--;
               }
               if(numleft==0)break;
            }
         }

         alienationcheck(0);
         noticecheck(-1);
         levelmap[locx][locy][locz].special=-1;

         return;
      }
      else if(c=='n')return;

   }while(true);
}



void special_corporate_files(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You've found a safe.", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("Open it? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         char actual;

         if(unlock(UNLOCK_SAFE,actual))
         {
            clearmessagearea();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("The Squad has found some very interesting files.", gamelog);
            gamelog.newline();

            Item *it=new Loot(*loottype[getloottype("LOOT_CORPFILES")]);
            activesquad->loot.push_back(it);
            it=new Loot(*loottype[getloottype("LOOT_CORPFILES")]);
            activesquad->loot.push_back(it);

            juiceparty(50,1000);
            sitecrime+=40;

            int time=20+LCSrandom(10);
            if(time<1)time=1;
            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

            getkey();
         }

         if(actual)
         {
            alienationcheck(0);
            noticecheck(-1);
            levelmap[locx][locy][locz].special=-1;
            sitecrime+=3;
            sitestory->crime.push_back(CRIME_CORP_FILES);

            criminalizeparty(LAWFLAG_THEFT);
         }

         return;
      }
      else if(c=='n')return;

   }while(true);
}



void special_radio_broadcaststudio(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      if(sitealarm||sitealienate)
      {
         move(16,1);
         addstr("The radio broadcasters left the equipment on in ", gamelog);
         move(17,1);
         addstr("their rush to get out.", gamelog);
         gamelog.newline();
         addstr(" Take over the studio? (Yes or No)");
      }
      else
      {
         move(16,1);
         addstr("You've found a radio broadcasting room.", gamelog);
         gamelog.newline();
         move(17,1);
         addstr("Interrupt this evening's programming? (Yes or No)");
      }

      int c=getkey();

      if(c=='y')
      {
         if(radio_broadcast())
         {
            sitestory->claimed=2;
            levelmap[locx][locy][locz].special=-1;
         }

         return;
      }
      else if(c=='n')return;

   }while(true);
}



void special_news_broadcaststudio(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      if(sitealarm||sitealienate)
      {
         move(16,1);
         addstr("The Cable News broadcasters left the equipment on in", gamelog);
         move(17,1);
         addstr("their rush to get out.");
         gamelog.newline();
         addstr(" Take over the studio? (Yes or No)");
      }
      else
      {
         move(16,1);
         addstr("You've found a Cable News broadcasting studio.", gamelog);
         move(17,1);
         addstr("Start an impromptu news program? (Yes or No)");
      }

      int c=getkey();

      if(c=='y')
      {
         if(news_broadcast())
         {
            sitestory->claimed=2;
            levelmap[locx][locy][locz].special=-1;
         }

         return;
      }
      else if(c=='n')return;

   }while(true);
}


void special_display_case(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You see a display case.", gamelog);
      move(17,1);
      addstr("Smash it? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         int time=20+LCSrandom(10);
         if(time<1)time=1;
         if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

         alienationcheck(0);
         noticecheck(-1,DIFFICULTY_HEROIC);
         levelmap[locx][locy][locz].special=-1;
         levelmap[locx][locy][locz].flag|=SITEBLOCK_DEBRIS;
         sitecrime++;
         juiceparty(5,100);
         sitestory->crime.push_back(CRIME_VANDALISM);

         criminalizeparty(LAWFLAG_VANDALISM);

         return;
      }
      else if(c=='n')return;

   }while(true);
}

void spawn_security(void)
{
   // add a bouncer if there isn't one in the first slot
   if(!sitealarm && !encounter[0].exists)
   {
      switch(location[cursite]->type)
      {
      default:
      case SITE_CORPORATE_HEADQUARTERS:
      case SITE_CORPORATE_HOUSE:
      case SITE_BUSINESS_ARMSDEALER:
      case SITE_BUSINESS_BANK:
      case SITE_INDUSTRY_NUCLEAR:
         makecreature(encounter[0], CREATURE_MERC);
         makecreature(encounter[1], CREATURE_MERC);
         break;
      case SITE_GOVERNMENT_POLICESTATION:
      case SITE_GOVERNMENT_COURTHOUSE:
      case SITE_OUTDOOR_PUBLICPARK:
         makecreature(encounter[0], CREATURE_COP);
         makecreature(encounter[1], CREATURE_COP);
         break;
      case SITE_GOVERNMENT_PRISON:
         makecreature(encounter[0], CREATURE_PRISONGUARD);
         makecreature(encounter[1], CREATURE_PRISONGUARD);
         makecreature(encounter[2], CREATURE_GUARDDOG);
         break;
      case SITE_GOVERNMENT_WHITE_HOUSE:
         makecreature(encounter[0], CREATURE_SECRET_SERVICE);
         makecreature(encounter[1], CREATURE_SECRET_SERVICE);
         makecreature(encounter[2], CREATURE_SECRET_SERVICE);
         makecreature(encounter[3], CREATURE_SECRET_SERVICE);
         break;
      case SITE_GOVERNMENT_INTELLIGENCEHQ:
         makecreature(encounter[0], CREATURE_AGENT);
         makecreature(encounter[1], CREATURE_AGENT);
         makecreature(encounter[2], CREATURE_GUARDDOG);
         break;
      case SITE_GOVERNMENT_ARMYBASE:
         makecreature(encounter[0], CREATURE_MILITARYPOLICE);
         makecreature(encounter[1], CREATURE_MILITARYPOLICE);
         break;
      case SITE_BUSINESS_BARANDGRILL:
      case SITE_RESIDENTIAL_BOMBSHELTER:
      case SITE_OUTDOOR_BUNKER:
         if(location[cursite]->renting==RENTING_CCS)
         {
            makecreature(encounter[0],CREATURE_CCS_VIGILANTE);
            makecreature(encounter[1],CREATURE_CCS_VIGILANTE);
         }
      }
   }
}

void special_security(bool metaldetect)
{
   char autoadmit=0;
   char sleepername[80];
   for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;

   spawn_security();

   for(int p=0;p<(int)pool.size();p++)
   {
      if(pool[p]->base==cursite)
      {
         autoadmit=1;
         if(pool[p]->type == encounter[0].type)
         {
            autoadmit=2;
            strcpy(sleepername,pool[p]->name);
            strcpy(encounter[0].name,sleepername);
            encounter[0].align=1;
            encounter[0].cantbluff=1;
            break;
         }
      }
   }
   //clearmessagearea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   if(sitealarm)
   {
      addstr("The security checkpoint is abandoned.", gamelog);
      gamelog.newline();
      levelmap[locx][locy][locz].special=SPECIAL_NONE;
      return;
   }
   else if(autoadmit)
   {
      addstr("The squad flashes ID badges.", gamelog);
      metaldetect=false;
      gamelog.newline();
      levelmap[locx][locy][locz].special=SPECIAL_SECURITY_SECONDVISIT;
   }
   else
   {
      if(metaldetect) addstr("The squad steps into a metal detector.", gamelog);
      else addstr("This door is guarded.", gamelog);
      gamelog.newline();
      levelmap[locx][locy][locz].special=SPECIAL_SECURITY_SECONDVISIT;
   }
   printencounter();

   getkey();

   char rejected=NOT_REJECTED;

   // Size up the squad for entry
   for(int s=0;s<6;s++)
   {
      if(activesquad->squad[s])
      {
         // Wrong clothes? Gone
         if(activesquad->squad[s]->is_naked() && activesquad->squad[s]->animalgloss!=ANIMALGLOSS_ANIMAL)
            if(rejected>REJECTED_NUDE)rejected=REJECTED_NUDE;

         if(autoadmit<1 && !hasdisguise(*activesquad->squad[s]))
            if(rejected>REJECTED_DRESSCODE)rejected=REJECTED_DRESSCODE;
         // Busted, cheap, bloody clothes? Gone
         if(autoadmit<2 && activesquad->squad[s]->get_armor().is_bloody())
            if(rejected>REJECTED_BLOODYCLOTHES)rejected=REJECTED_BLOODYCLOTHES;
         if(autoadmit<2 && activesquad->squad[s]->get_armor().is_damaged())
            if(rejected>REJECTED_DAMAGEDCLOTHES)rejected=REJECTED_DAMAGEDCLOTHES;
         if(autoadmit<2 && activesquad->squad[s]->get_armor().get_quality()!=1)
            if(rejected>REJECTED_SECONDRATECLOTHES)rejected=REJECTED_SECONDRATECLOTHES;
         // Suspicious weapons? Gone
         if(autoadmit<2 && weaponcheck(*activesquad->squad[s], metaldetect)>0)
            if(rejected>REJECTED_WEAPONS)rejected=REJECTED_WEAPONS;
         // Fail a tough disguise check? Gone
         if(autoadmit<1 && disguisesite(sitetype) && !(activesquad->squad[s]->skill_check(SKILL_DISGUISE,DIFFICULTY_CHALLENGING)))
            if(rejected>REJECTED_SMELLFUNNY)rejected=REJECTED_SMELLFUNNY;
         // Underage? Gone
         if(autoadmit<1 && activesquad->squad[s]->age<18)
            if(rejected>REJECTED_UNDERAGE)rejected=REJECTED_UNDERAGE;
      }
   }
   move(17,1);
   switch(rejected)
   {
   case REJECTED_NUDE:
      set_color(COLOR_RED,COLOR_BLACK,1);
      if(autoadmit) addstr("\"Jesus! Put some clothes on!\"", gamelog);
      else switch(LCSrandom(4))
      {
      case 0:addstr("\"Get out of here you nudist!!\"", gamelog);break;
      case 1:addstr("\"Back off, creep!\"", gamelog);break;
      case 2:addstr("\"Jesus!! Somebody call the cops!\"", gamelog);break;
	   case 3:addstr("\"Are you sleepwalking?!\"", gamelog);break;
      }
      gamelog.newline();
      break;
   case REJECTED_UNDERAGE:
      set_color(COLOR_RED,COLOR_BLACK,1);
      switch(LCSrandom(4))
      {
      case 0:addstr("\"No admittance, youngster.\"", gamelog);break;
      case 1:addstr("\"You're too young to work here.\"", gamelog);break;
      case 2:addstr("\"Go play someplace else.\"", gamelog);break;
      case 3:addstr("\"Where's your mother?\"", gamelog);break;
      }
      gamelog.newline();
      break;
   case REJECTED_DRESSCODE:
      set_color(COLOR_RED,COLOR_BLACK,1);
      switch(LCSrandom(1))
      {
      case 0:addstr("\"Employees only.\"", gamelog);break;
      }
      gamelog.newline();
      break;
   case REJECTED_SMELLFUNNY:
      set_color(COLOR_RED,COLOR_BLACK,1);
      switch(LCSrandom(4))
      {
      case 0:addstr("\"You don't work here, do you?\"", gamelog);break;
      case 1:addstr("\"Hmm... can I see your badge?\"", gamelog);break;
      case 2:addstr("\"There's just something off about you.\"", gamelog);break;
      case 3:addstr("\"You must be new. You'll need your badge.\"", gamelog);break;
      }
      gamelog.newline();
      break;
   case REJECTED_BLOODYCLOTHES:
      set_color(COLOR_RED,COLOR_BLACK,1);
      switch(LCSrandom(5))
      {
      case 0:addstr("\"Good God! What is wrong with your clothes?\"", gamelog);break;
      case 1:addstr("\"Are you hurt?! The aid station is the other way!\"", gamelog);break;
      case 2:addstr("\"Your clothes, that's blood!\"", gamelog);break;
      case 3:addstr("\"Blood?! That's more than a little suspicious...\"", gamelog);break;
      case 4:addstr("\"Did you just butcher a cat?!\"", gamelog);break;
      case 5:addstr("\"Blood everywhere...?\"", gamelog);break;
      }
      gamelog.newline();
      break;
   case REJECTED_DAMAGEDCLOTHES:
      set_color(COLOR_RED,COLOR_BLACK,1);
      switch(LCSrandom(2))
      {
      case 0:addstr("\"Good God! What is wrong with your clothes?\"", gamelog);break;
      case 1:addstr("\"Is that a damaged halloween costume?\"", gamelog);break;
      }
      gamelog.newline();
      break;
   case REJECTED_SECONDRATECLOTHES:
      set_color(COLOR_RED,COLOR_BLACK,1);
      switch(LCSrandom(2))
      {
      case 0:addstr("\"That looks like you sewed it yourself.\"", gamelog);break;
      case 1:addstr("\"That's a poor excuse for a uniform. Who are you?\"", gamelog);break;
      }
      gamelog.newline();
      break;
   case REJECTED_WEAPONS:
      set_color(COLOR_RED,COLOR_BLACK,1);
      if(metaldetect)
      {
         addstr("-BEEEP- -BEEEP- -BEEEP-", gamelog);
         sitealarm=1;
      }
      else switch(LCSrandom(5))
      {
      case 0:addstr("\"Put that away!\"", gamelog);break;
      case 1:addstr("\"Hey, back off!\"", gamelog);break;
      case 2:addstr("\"Don't try anything!\"", gamelog);break;
      case 3:addstr("\"Are you here to make trouble?\"", gamelog);break;
      case 4:addstr("\"Stay back!\"", gamelog);break;
      }
      gamelog.newline();
      break;
   case NOT_REJECTED:
      set_color(COLOR_GREEN,COLOR_BLACK,1);

      switch(LCSrandom(4))
      {
      case 0:addstr("\"Move along.\"", gamelog);break;
      case 1:addstr("\"Have a nice day.\"", gamelog);break;
      case 2:addstr("\"Quiet day, today.\"", gamelog);break;
      case 3:addstr("\"Go on in.\"", gamelog);break;
      }
      gamelog.newline();
      break;
   }

   getkey();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   for(int dx=-1; dx<=1; dx++)
   for(int dy=-1; dy<=1; dy++)
   {
      if(levelmap[locx+dx][locy+dy][locz].flag & SITEBLOCK_DOOR)
      {
         if(rejected<NOT_REJECTED)
         {
            levelmap[locx+dx][locy+dy][locz].flag |= SITEBLOCK_LOCKED;
            levelmap[locx+dx][locy+dy][locz].flag |= SITEBLOCK_CLOCK;
         }
         else levelmap[locx+dx][locy+dy][locz].flag &= ~SITEBLOCK_DOOR;
      }
   }
   encounter[0].cantbluff=1;
}

void special_security_checkpoint(void)
{
   special_security(false);
}

void special_security_metaldetectors(void)
{
   special_security(true);
}

void special_security_secondvisit(void)
{
   spawn_security();
}

void special_bank_vault(void)
{
   clearmessagearea();
   move(16,1);
   addstr("The vault door has three layers: A combo lock, ", gamelog);
   move(17,1);
   addstr("an electronic lock, and a biometric lock.", gamelog);
   gamelog.newline();

   getkey();

   clearmessagearea();
   move(16,1);
   addstr("You will need a security expert, a computer ", gamelog);
   move(17,1);
   addstr("expert, and one of the bank managers.", gamelog);
   gamelog.newline();

   getkey();

   for(int p=0;p<(int)pool.size();p++)
   {
      if(pool[p]->type==CREATURE_BANK_MANAGER &&
         pool[p]->flag & CREATUREFLAG_SLEEPER &&
         pool[p]->base == cursite)
      {
         clearmessagearea();
         move(16,1);
         addstr("Sleeper ", gamelog);
         addstr(pool[p]->name, gamelog);
         addstr(" can handle the biometrics, ", gamelog);
         move(17,1);
         addstr("but you'll still have to crack the other locks.", gamelog);
         gamelog.newline();

         getkey();

         break;
      }
   }

   do
   {
      clearmessagearea();
      move(16,1);
      addstr("Open the bank vault? (Yes or No)");

      int c=getkey();

      if(c=='y')
      {
         char actual;

         clearmessagearea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         addstr("First is the combo lock that will have ", gamelog);
         move(17,1);
         addstr("be cracked by a security expert.", gamelog);
         gamelog.newline();

         getkey();

         if(!unlock(UNLOCK_VAULT,actual))
         {
            clearmessagearea();
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("The squad can only dream of the money ", gamelog);
            move(17,1);
            addstr("on the other side of this door...", gamelog);
            gamelog.newline();

            getkey();

            levelmap[locx][locy][locz].special=-1;
         }
         else
         {
            clearmessagearea();
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("Next is the electronic lock that will have ", gamelog);
            move(17,1);
            addstr("be bypassed by a computer expert.", gamelog);
            gamelog.newline();

            getkey();

            if(!hack(HACK_VAULT,actual))
            {
               clearmessagearea();
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("The money was so close the squad could taste it!", gamelog);
               gamelog.newline();

               getkey();

               levelmap[locx][locy][locz].special=-1;
            }
            else
            {
               clearmessagearea();
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("Last is the biometric lock that keyed only ", gamelog);
               move(17,1);
               addstr("to the bank's managers.", gamelog);
               gamelog.newline();

               getkey();

               Creature *manager = 0;
               bool canbreakin = false;

               for(int s=0;s<6;s++)
               {
                  Creature *c = activesquad->squad[s];

                  if(c)
                  {
                     if(c->type == CREATURE_BANK_MANAGER)
                     {
                        manager = c;
                        if(c->joindays < 30 && !(c->flag & CREATUREFLAG_KIDNAPPED))
                        {
                           clearmessagearea();
                           set_color(COLOR_WHITE,COLOR_BLACK,1);
                           move(16,1);
                           addstr(c->name, gamelog);
                           addstr(" opens the vault.", gamelog);
                           gamelog.newline();

                           getkey();

                           canbreakin = true;
                           break;
                        }
                     }

                     if(c->prisoner && c->prisoner->type == CREATURE_BANK_MANAGER)
                     {
                        clearmessagearea();
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(16,1);
                        addstr("The hostage is forced to open the vault.", gamelog);
                        gamelog.newline();

                        getkey();

                        canbreakin = true;
                        break;
                     }
                  }
               }

               if(!canbreakin)
               {
                  for(int p=0; p<(int)pool.size(); p++)
                  {
                     if(pool[p]->base == cursite && pool[p]->type == CREATURE_BANK_MANAGER)
                     {
                        clearmessagearea();
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(16,1);
                        addstr("Sleeper ", gamelog);
                        addstr(pool[p]->name, gamelog);
                        addstr(" opens the vault, ", gamelog);
                        move(17,1);
                        addstr("and will join the active LCS to avoid arrest.", gamelog);
                        gamelog.newline();

                        getkey();

                        canbreakin = true;

                        pool[p]->location = pool[p]->base = activesquad->squad[0]->base;
                        pool[p]->flag &= ~CREATUREFLAG_SLEEPER;
                        pool[p]->activity.type = ACTIVITY_NONE;
                        pool[p]->crimes_suspected[LAWFLAG_BANKROBBERY]++;

                        break;
                     }
                  }
               }

               if(canbreakin)
               {
                  criminalizeparty(LAWFLAG_BANKROBBERY);
                  sitecrime+=20;
                  sitestory->crime.push_back(CRIME_BANKVAULTROBBERY);
                  levelmap[locx+1][locy][locz].flag &= ~SITEBLOCK_DOOR;
                  levelmap[locx-1][locy][locz].flag &= ~SITEBLOCK_DOOR;
                  levelmap[locx][locy+1][locz].flag &= ~SITEBLOCK_DOOR;
                  levelmap[locx][locy-1][locz].flag &= ~SITEBLOCK_DOOR;
                  levelmap[locx][locy][locz].special=-1;
               }
               else
               {
                  if(manager)
                  {
                     clearmessagearea();
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(16,1);
                     addstr(manager->name, gamelog);
                     addstr(" is no longer recognized.", gamelog);
                     gamelog.newline();

                     getkey();
                  }
                  else
                  {
                     clearmessagearea();
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(16,1);
                     addstr("The squad has nobody that can do the job.", gamelog);
                     gamelog.newline();

                     getkey();
                  }
               }
            }
         }

         if(actual)
         {
            alienationcheck(0);
            noticecheck(-1);
         }

         return;
      }
      else if(c=='n')return;

   }while(true);
}

void special_bank_teller(void)
{
   if(sitealarm||sitealienate||
      location[cursite]->siege.siege)
   {
      clearmessagearea(false);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("The teller window is empty.", gamelog);
      gamelog.newline();
      levelmap[locx][locy][locz].special=-1;

      getkey();
   }
   else
   {
      clearmessagearea(false);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("A bank teller is available.", gamelog);
      gamelog.newline();
      levelmap[locx][locy][locz].special=-1;

      getkey();

      for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;
      makecreature(encounter[0],CREATURE_BANK_TELLER);
   }
}

void special_bank_money(void)
{
   static int swat_counter = 0;

   clearmessagearea(false);
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   move(16,1);
   addstr("The squad loads bricks of cash into a duffel bag.", gamelog);
   gamelog.newline();

   levelmap[locx][locy][locz].special=-1;

   activesquad->loot.push_back(new Money(20000));
   sitecrime+=20;

   if(postalarmtimer <= 80) swat_counter = 0;

   if(!sitealarm && sitealarmtimer!=0) sitealarmtimer=0;
   else if(!sitealarm && !LCSrandom(2)) sitealarm=1;
   else if(sitealarm && postalarmtimer <= 60) postalarmtimer += 20;
   else if(sitealarm && postalarmtimer <= 80 && LCSrandom(2)) postalarmtimer = 81;
   else if(sitealarm && postalarmtimer > 80 && LCSrandom(2) && swat_counter < 2)
   {
      getkey();

      move(17,1);
      if(swat_counter > 0)
         addstr("Another SWAT team moves in!!", gamelog);
      else
         addstr("A SWAT team storms the vault!!", gamelog);
      gamelog.newline();
      swat_counter += 1;

      int swatnum = 9;
      for(int e=0;e<ENCMAX;e++)
      {
         if(!encounter[e].exists)
         {
            makecreature(encounter[e],CREATURE_SWAT);
            swatnum--;
            if(swatnum<=0) break;
         }
      }
   }

   getkey();
}

void special_oval_office(void)
{
   // Clear entire Oval Office area
   for(int dx=-1; dx<=1; dx++)
   for(int dy=-1; dy<=1; dy++)
   {
      if(levelmap[locx+dx][locy+dy][locz].special == SPECIAL_OVAL_OFFICE_NW ||
         levelmap[locx+dx][locy+dy][locz].special == SPECIAL_OVAL_OFFICE_NE ||
         levelmap[locx+dx][locy+dy][locz].special == SPECIAL_OVAL_OFFICE_SW ||
         levelmap[locx+dx][locy+dy][locz].special == SPECIAL_OVAL_OFFICE_SE)
      {
         levelmap[locx+dx][locy+dy][locz].special = -1;
      }
   }

   if(sitealarm)
   {
      for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;

      clearmessagearea(false);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      mvaddstr(16,1,"The President isn't here... ",gamelog);
      printsitemap(locx,locy,locz);

      getkey();

      mvaddstr(17,1,"Secret Service agents ambush the squad!", gamelog);
      gamelog.newline();
      for(int e=0;e<6;e++)makecreature(encounter[e],CREATURE_SECRET_SERVICE);
      printencounter();

      getkey();

      enemyattack();
      creatureadvance();
   }
   else
   {
      for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;

      clearmessagearea(false);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      mvaddstr(16,1,"The President is in the Oval Office.",gamelog);
      gamelog.newline();
      printsitemap(locx,locy,locz);
      for(int e=0;e<2;e++)makecreature(encounter[e],CREATURE_SECRET_SERVICE);
      encounter[2] = uniqueCreatures.President();
      printencounter();

      getkey();
   }
}

void special_ccs_boss(void)
{
   if(sitealarm||sitealienate||
      location[cursite]->siege.siege)
   {
      clearmessagearea(false);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("The CCS leader is ready for you!", gamelog);
      gamelog.newline();
      levelmap[locx][locy][locz].special=-1;

      getkey();

      for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;
      makecreature(encounter[0],CREATURE_CCS_ARCHCONSERVATIVE);
      makecreature(encounter[1],CREATURE_CCS_VIGILANTE);
      makecreature(encounter[2],CREATURE_CCS_VIGILANTE);
      makecreature(encounter[3],CREATURE_CCS_VIGILANTE);
      makecreature(encounter[4],CREATURE_CCS_VIGILANTE);
      makecreature(encounter[5],CREATURE_CCS_VIGILANTE);
   }
   else
   {
      clearmessagearea(false);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("The CCS leader is here.", gamelog);
      gamelog.newline();
      levelmap[locx][locy][locz].special=-1;

      getkey();

      for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;
      makecreature(encounter[0],CREATURE_CCS_ARCHCONSERVATIVE);
   }
}
