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
   bool autoadmit=0;
   char sleepername[80];
   for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;
   
   special_bouncer_greet_squad();

   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->base==cursite&&pool[p]->type==CREATURE_BOUNCER && !LCSrandom(3))
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
      addstr("Sleeper ");
      addstr(sleepername);
      addstr(" smirks and lets the squad in.");
      
      levelmap[locx][locy][locz].special=-1;
   }
   else
   {
      if(location[cursite]->renting==RENTING_CCS)
         addstr("The Conservative scum block the door.");
      else
         addstr("The bouncer assesses your squad.");
      levelmap[locx][locy][locz].special=SPECIAL_CLUB_BOUNCER_SECONDVISIT;
   }
   printencounter();
   refresh();
   getch();
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
            if(location[cursite]->renting==RENTING_CCS)
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
         case 0:addstr("\"Can I see... heh heh... some ID?\"");break;
         case 1:addstr("\"Woah... you think you're coming in here?\"");break;
         case 2:addstr("\"Check out this fool. Heh.\"");break;
         case 3:addstr("\"Want some trouble, dumpster breath?\"");break;
         case 4:addstr("\"You're gonna stir up the hornet's nest, fool.\"");break;
         case 5:addstr("\"Come on, take a swing at me. Just try it.\"");break;
         case 6:addstr("\"You really don't want to fuck with me.\"");break;
         case 7:addstr("\"Hey girly, have you written your will?\"");break;
         case 8:addstr("\"Oh, you're trouble. I *like* trouble.\"");break;
         case 9:addstr("\"I'll bury you in those planters over there.\"");break;
         case 10:addstr("\"Looking to check on the color of your blood?\"");break;
         }
         break;
      case REJECTED_NUDE:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(4))
         {
         case 0:addstr("\"No shirt, no underpants, no service.\"");break;
         case 1:addstr("\"Put some clothes on! That's disgusting.\"");break;
         case 2:addstr("\"No! No, you can't come in naked! God!!\"");break;
		 case 3:addstr("\"No shoes, no shirt and you don't get service\"");break;
         }
         break;
      case REJECTED_UNDERAGE:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(5))
         {
         case 0:addstr("\"Hahaha, come back in a few years.\"");break;
         case 1:addstr("\"Find some kiddy club.\"");break;
         case 2:addstr("\"You don't look 18 to me.\"");break;
         case 3:addstr("\"Go back to your treehouse.\"");break;
         case 4:addstr("\"Where's your mother?\"");break;
         }
         break;
      case REJECTED_FEMALE:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(4))
         {
         case 0:addstr("\"Move along ma'am, this club's for men.\"");break;
         case 1:addstr("\"This 'ain't no sewing circle, ma'am.\"");break;
         case 2:addstr("\"Sorry ma'am, this place is only for the men.\"");break;
         case 3:addstr("\"Where's your husband?\"");break;
         }
         break;
      case REJECTED_FEMALEISH:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(3))
         {
         case 0:addstr("\"You /really/ don't look like a man to me...\"");break;
         case 1:addstr("\"Y'know... the \'other\' guys won't like you much.\"");break;
         case 2:addstr("\"Uhh... can't let you in, ma'am. Sir. Whatever.\"");break;
         }
         break;
      case REJECTED_DRESSCODE:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(3))
         {
         case 0:addstr("\"Check the dress code.\"");break;
         case 1:addstr("\"We have a dress code here.\"");break;
         case 2:addstr("\"I can't let you in looking like that.\"");break;
         }
         break;
      case REJECTED_SMELLFUNNY:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(6))
         {
         case 0:addstr("\"God, you smell.\"");break;
         case 1:addstr("\"Not letting you in. Because I said so.\"");break;
         case 2:addstr("\"There's just something off about you.\"");break;
         case 3:addstr("\"Take a shower.\"");break;
         case 4:addstr("\"You'd just harass the others, wouldn't you?\"");break;
         case 5:
                if(law[LAW_FREESPEECH]==-2)addstr("\"Get the [heck] out of here.\"");
                else if(law[LAW_FREESPEECH]==2)addstr("\"Get the fuck out of here.\"");
                else addstr("\"Get the hell out of here.\"");break;
         }
         break;
      case REJECTED_BLOODYCLOTHES:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(5))
         {
         case 0:addstr("\"Good God! What is wrong with your clothes?\"");break;
         case 1:addstr("\"Absolutely not. Clean up a bit.\"");break;
         case 2:addstr("\"This isn't a goth club, bloody clothes don't cut it here.\"");break;
         case 3:addstr("\"Uh, maybe you should wash... replace... those clothes.\"");break;
         case 4:addstr("\"Did you spill something on your clothes?\"");break;
         case 5:addstr("\"Come back when you get the red wine out of your clothes.\"");break;
         }
         break;
      case REJECTED_DAMAGEDCLOTHES:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(2))
         {
         case 0:addstr("\"Good God! What is wrong with your clothes?\"");break;
         case 1:addstr("\"This isn't a goth club, ripped clothes don't cut it here.\"");break;
         }
         break;
      case REJECTED_SECONDRATECLOTHES:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(2))
         {
         case 0:addstr("\"That looks like you sewed it yourself.\"");break;
         case 1:addstr("\"If badly cut clothing is a hot new trend, I missed it.\"");break;
         }
         break;
      case REJECTED_WEAPONS:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(5))
         {
         case 0:addstr("\"No weapons allowed.\"");break;
         case 1:addstr("\"I can't let you in carrying that.\"");break;
         case 2:addstr("\"I can't let you take that in.\"");break;
         case 3:addstr("\"Come to me armed, and I'll tell you to take a hike.\"");break;
         case 4:addstr("\"Real men fight with fists. And no, you can't come in.\"");break;
         }
         break;
      case REJECTED_GUESTLIST:
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr("\"This club is by invitation only.\"");
         break;
      case NOT_REJECTED:
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         
         switch(LCSrandom(4))
         {
         case 0:addstr("\"Keep it civil and don't drink too much.\"");break;
         case 1:addstr("\"Let me get the door for you.\"");break;
         case 2:addstr("\"Ehh, alright, go on in.\"");break;
         case 3:addstr("\"Come on in.\"");break;
         }
         break;
      }
      refresh();
      getch();
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
      addstr("You see fluffy white rabbits in a locked cage.");
      move(17,1);
      addstr("Free them? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

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

   }while(1);
}



void special_readsign(int sign)
{
   clearmessagearea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);

   switch(sign)
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
         addstr("\"Avoid problems by not having them.\"");
         break;
      }
      break;
   }
   getch();
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
         addstr("You see the nuclear waste center control room.");
         move(17,1);
         addstr("Attempt to release nuclear waste? (Yes or No)");
      }
      else
      {
         move(16,1);
         addstr("You see the nuclear power plant control room.");
         move(17,1);
         addstr("Mess with the reactor settings? (Yes or No)");
      }
      refresh();

      int c=getch();
      translategetch(c);

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
            addstr(maxs->name);
            addstr(" presses the big red button!");
            refresh();
            getch();
            move(17,1);
            addstr(".");
            refresh();
            getch();
            addstr(".");
            refresh();
            getch();
            addstr(".");
            refresh();
            getch();
            if(law[LAW_NUCLEARPOWER]==2)
            {
               move(17,1);
               addstr("The nuclear waste gets released into the state's water supply!");
               change_public_opinion(VIEW_NUCLEARPOWER,15,0,95);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,-50,0,0);
               refresh();
               getch();

               juiceparty(40,1000); // Instant juice!
			      sitecrime+=25; //Shutdown Site
               
               sitestory->crime.push_back(CRIME_SHUTDOWNREACTOR);

            }
            else
            {
               move(16,1);
               addstr("A deafening alarm sounds!");
               move(17,1);
               addstr("The reactor is overheating!");
               change_public_opinion(VIEW_NUCLEARPOWER,15,0,95);
               refresh();
               getch();

               juiceparty(100,1000); // Instant juice!
			      sitecrime+=50; //Shutdown Site
               
               sitestory->crime.push_back(CRIME_SHUTDOWNREACTOR);
            }
         }
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("After some failed attempts, and a very loud alarm,");
            move(17,1);
            addstr("the Squad resigns to just leaving a threatening note.");

            refresh();
            getch();

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

   }while(1);
}



void special_lab_genetic_cagedanimals(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You see horrible misshapen creatures in a sealed cage.");
      move(17,1);
      addstr("Free them? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

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
               addstr("Uh, maybe that idea was Conservative in retrospect...");

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

               refresh();
               getch();

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

   }while(1);
}



void special_policestation_lockup(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You see prisoners in the detention room.");
      move(17,1);
      addstr("Free them? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

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

   }while(1);
}



void special_courthouse_lockup(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You see prisoners in the Court House jail.");
      move(17,1);
      addstr("Free them? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

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

   }while(1);
}



void special_courthouse_jury(void)
{
   int p;
   if(sitealarm||sitealienate)
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("It appears as if this room has been");
      move(17,1);
      addstr("vacated in a hurry.");

      refresh();
      getch();
      return;
   }

   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You've found a Jury in deliberations!");
      move(17,1);
      addstr("Attempt to influence them? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

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
               addstr(activesquad->squad[p]->name);
               addstr(" works the room like in Twelve Angry Men, and the jury");
               move(17,1);
               addstr("concludes that ");//XXX: This is very awkward grammar.
               switch(LCSrandom(16))     // Fixed. -Fox
               {
                  case 0:addstr("murder");break;
                  case 1:addstr("assault");break;
                  case 2:addstr("theft");break;
                  case 3:addstr("mugging");break;
                  case 4:addstr("burglary");break;
                  case 5:addstr("property destruction");break;
                  case 6:addstr("vandalism");break;
                  case 7:addstr("libel");break;
                  case 8:addstr("slander");break;
                  case 9:addstr("sodomy");break;
                  case 10:addstr("obstruction of justice");break;
                  case 11:addstr("breaking and entering");break;
                  case 12:addstr("public indecency");break;
                  case 13:addstr("arson");break;
                  case 14:addstr("resisting arrest");break;
                  case 15:addstr("tax evasion");break;
               }
               addstr(" wasn't really wrong here.");

               refresh();
               getch();

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
               addstr(activesquad->squad[p]->name);

               addstr(" wasn't quite convincing...");

               refresh();
               getch();

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

   }while(1);
}



void special_prison_control(short prison_control_type)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You've found the ");
      if(prison_control_type==SPECIAL_PRISON_CONTROL_LOW)
         addstr("low security ");
      else if(prison_control_type==SPECIAL_PRISON_CONTROL_MEDIUM)
         addstr("medium security ");
      else if(prison_control_type==SPECIAL_PRISON_CONTROL_HIGH)
         addstr("high security ");
      addstr("prison control room.");
      move(17,1);
      addstr("Free the prisoners? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

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

   }while(1);
}



void special_intel_supercomputer(void)
{
   if(sitealarm||sitealienate)
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("The security alert has caused the");
      move(17,1);
      addstr("computer to shut down.");

      refresh();
      getch();
      return;
   }

   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You've found the Intelligence Supercomputer.");
      move(17,1);
      addstr("Hack it? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

      if(c=='y')
      {
         char actual;

         if(hack(HACK_SUPERCOMPUTER,actual))
         {
            clearmessagearea();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("The Squad obtains sensitive information.");
            
            juiceparty(50,1000);

            Item *it=new Loot(*loottype[getloottype("LOOT_INTHQDISK")]);
            activesquad->loot.push_back(it);

            refresh();
            getch();
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

   }while(1);
}


void special_graffiti(void)
{
   clearmessagearea();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   addstr("The squad sprays Liberal Graffiti!");

   if(!sitestory->claimed)
      sitestory->claimed=1;

   refresh();
   getch();

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
      for(int i=0;i<location[cursite]->changes.size();i++)
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
      addstr("You see some textile equipment.");
      move(17,1);
      addstr("Destroy it? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

      if(c=='y')
      {
         int time=20+LCSrandom(10);
         if(time<1)time=1;
         if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;
         
         alienationcheck(0);
         noticecheck(-1,DIFFICULTY_HEROIC);
         levelmap[locx][locy][locz].special=-1;
         sitecrime++;
         juiceparty(5,100);
         sitestory->crime.push_back(CRIME_BREAK_SWEATSHOP);

         criminalizeparty(LAWFLAG_VANDALISM);

         return;
      }
      else if(c=='n')return;

   }while(1);
}



void special_polluter_equipment(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You see some industrial equipment.");
      move(17,1);
      addstr("Destroy it? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

      if(c=='y')
      {
         int time=20+LCSrandom(10);
         if(time<1)time=1;
         if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

         change_public_opinion(VIEW_POLLUTION,2,1,70);
         
         alienationcheck(0);
         noticecheck(-1,DIFFICULTY_HEROIC);
         levelmap[locx][locy][locz].special=-1;
         sitecrime+=2;
         juiceparty(5,100);
         sitestory->crime.push_back(CRIME_BREAK_FACTORY);

         criminalizeparty(LAWFLAG_VANDALISM);

         return;
      }
      else if(c=='n')return;

   }while(1);
}



void special_house_photos(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You've found a safe.");
      move(17,1);
      addstr("Open it? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

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
               addstr("The squad has found a Desert Eagle.");

               refresh();
               getch();

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
               addstr("This guy sure had a lot of $100 bills.");

               refresh();
               getch();

               it=new Money(1000*(1+LCSrandom(10)));
               activesquad->loot.push_back(it);
               
               empty=false;
            }

            if(LCSrandom(2))
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("The squad Liberates some expensive jewelery.");

               refresh();
               getch();

               it=new Loot(*loottype[getloottype("LOOT_EXPENSIVEJEWELERY")],3);
               activesquad->loot.push_back(it);
               
               empty=false;
            }

            if(!LCSrandom(3))
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("There are some... very compromising photos here.");

               refresh();
               getch();

               it=new Loot(*loottype[getloottype("LOOT_CEOPHOTOS")]);
               activesquad->loot.push_back(it);
               
               empty=false;
            }

            if(!LCSrandom(3))
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("There are some drugs here.");

               refresh();
               getch();
               
               empty=false;
            }

            if(!LCSrandom(3))
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("Wow, get a load of these love letters.");
               move(17,1);
               addstr("The squad will take those.");

               refresh();
               getch();

               it=new Loot(*loottype[getloottype("LOOT_CEOLOVELETTERS")]);
               activesquad->loot.push_back(it);
               
               empty=false;
            }

            if(!LCSrandom(3))
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("These documents show serious tax evasion.");

               refresh();
               getch();

               it=new Loot(*loottype[getloottype("LOOT_CEOTAXPAPERS")]);
               activesquad->loot.push_back(it);
               
               empty=false;
            }

            if(empty)
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("Wow, it's empty.  That sucks.");

               refresh();
               getch();
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

   }while(1);
}


void special_armory(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You've found the armory.");
      move(17,1);
      addstr("Break in? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

      if(c=='y')
      {
         clearmessagearea();

         sitealarm=1;
         move(16,1);
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr("Alarms go off!");
         refresh();
         getch();

         bool empty=true;
         Item *it;
         
         if(m249==false && location[cursite]->type == SITE_GOVERNMENT_ARMYBASE)
         {
            clearmessagearea();
            
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("Jackpot! The squad found a M249 Machine Gun!");

            refresh();
            getch();

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
            addstr("The squad finds some M16 Assault Rifles.");

            refresh();
            getch();

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
            addstr("The squad finds some M4 Carbines.");

            refresh();
            getch();

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
            addstr("The squad finds some body armor.");

            refresh();
            getch();

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
            addstr("It's a trap!  The armory is empty.");

            refresh();
            getch();

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
            addstr("Guards are everywhere!");

            refresh();
            getch();

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

   }while(1);
}



void special_corporate_files(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You've found a safe.");
      move(17,1);
      addstr("Open it? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

      if(c=='y')
      {
         char actual;

         if(unlock(UNLOCK_SAFE,actual))
         {
            clearmessagearea();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("The Squad has found some very interesting files.");

            Item *it=new Loot(*loottype[getloottype("LOOT_CORPFILES")]);
            activesquad->loot.push_back(it);
            it=new Loot(*loottype[getloottype("LOOT_CORPFILES")]);
            activesquad->loot.push_back(it);
            
            juiceparty(50,1000);
            sitecrime+=40;

            int time=20+LCSrandom(10);
            if(time<1)time=1;
            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

            refresh();
            getch();
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

   }while(1);
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
         addstr("The radio broadcasters left the equipment on in");
         move(17,1);
         addstr("their rush to get out. Take over the studio? (Yes or No)");
      }
      else
      {
         move(16,1);
         addstr("You've found a radio broadcasting room.");
         move(17,1);
         addstr("Interrupt this evening's programming? (Yes or No)");
      }

      refresh();

      int c=getch();
      translategetch(c);

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

   }while(1);
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
         addstr("The Cable News broadcasters left the equipment on in");
         move(17,1);
         addstr("their rush to get out. Take over the studio? (Yes or No)");
      }
      else
      {
         move(16,1);
         addstr("You've found a Cable News broadcasting studio.");
         move(17,1);
         addstr("Start an impromptu news program? (Yes or No)");
      }

      refresh();

      int c=getch();
      translategetch(c);

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

   }while(1);
}


void special_display_case(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You see a display case.");
      move(17,1);
      addstr("Smash it? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

      if(c=='y')
      {
         int time=20+LCSrandom(10);
         if(time<1)time=1;
         if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;
         
         alienationcheck(0);
         noticecheck(-1,DIFFICULTY_HEROIC);
         levelmap[locx][locy][locz].special=-1;
         sitecrime++;
         juiceparty(5,100);
         sitestory->crime.push_back(CRIME_VANDALISM);

         criminalizeparty(LAWFLAG_VANDALISM);

         return;
      }
      else if(c=='n')return;

   }while(1);
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
   bool autoadmit=0;
   char sleepername[80];
   for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;

   spawn_security();

   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->base==cursite)
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
   if(sitealarm)
   {
      addstr("Looks like security is in disarray.");
      levelmap[locx][locy][locz].special=SPECIAL_NONE;
   }
   else if(autoadmit)
   {
      addstr("The squad flashes ID badges.");
      levelmap[locx][locy][locz].special=SPECIAL_SECURITY_SECONDVISIT;
   }
   else
   {
      if(metaldetect) addstr("The squad steps into a metal detector.");
      else addstr("This door is guarded.");
      levelmap[locx][locy][locz].special=SPECIAL_SECURITY_SECONDVISIT;
   }
   printencounter();
   refresh();
   getch();

   if(sitealarm) return;

   char rejected=NOT_REJECTED;
   
   // Size up the squad for entry
   for(int s=0;s<6;s++)
   {
      if(activesquad->squad[s])
      {
         // Wrong clothes? Gone
         if(activesquad->squad[s]->is_naked() && activesquad->squad[s]->animalgloss!=ANIMALGLOSS_ANIMAL)
            if(rejected>REJECTED_NUDE)rejected=REJECTED_NUDE;
         if(!autoadmit && !hasdisguise(*activesquad->squad[s]))
            if(rejected>REJECTED_DRESSCODE)rejected=REJECTED_DRESSCODE;
         // Busted, cheap, bloody clothes? Gone
         if(!autoadmit && activesquad->squad[s]->get_armor().is_bloody())
            if(rejected>REJECTED_BLOODYCLOTHES)rejected=REJECTED_BLOODYCLOTHES;
         if(!autoadmit && activesquad->squad[s]->get_armor().is_damaged())
            if(rejected>REJECTED_DAMAGEDCLOTHES)rejected=REJECTED_DAMAGEDCLOTHES;
         if(!autoadmit && activesquad->squad[s]->get_armor().get_quality()!=1)
            if(rejected>REJECTED_SECONDRATECLOTHES)rejected=REJECTED_SECONDRATECLOTHES;
         // Suspicious weapons? Gone
         if(!autoadmit && weaponcheck(*activesquad->squad[s])>0, metaldetect)
            if(rejected>REJECTED_WEAPONS)rejected=REJECTED_WEAPONS;
         // Fail a tough disguise check? Gone
         if(!autoadmit && disguisesite(sitetype) && !(activesquad->squad[s]->skill_check(SKILL_DISGUISE,DIFFICULTY_CHALLENGING)))
            if(rejected>REJECTED_SMELLFUNNY)rejected=REJECTED_SMELLFUNNY;
         // Underage? Gone
         if(!autoadmit && activesquad->squad[s]->age<18)
            if(rejected>REJECTED_UNDERAGE)rejected=REJECTED_UNDERAGE;
      }
   }
   move(17,1);
   switch(rejected)
   {
   case REJECTED_NUDE:
      set_color(COLOR_RED,COLOR_BLACK,1);
      if(autoadmit) addstr("\"Jesus! Put some clothes on!\"");
      else switch(LCSrandom(4))
      {
      case 0:addstr("\"Get out of here you nudist!!\"");break;
      case 1:addstr("\"Back off, creep!\"");break;
      case 2:addstr("\"Jesus!! Somebody call the cops!\"");break;
	   case 3:addstr("\"Are you sleepwalking?!\"");break;
      }
      break;
   case REJECTED_UNDERAGE:
      set_color(COLOR_RED,COLOR_BLACK,1);
      switch(LCSrandom(4))
      {
      case 0:addstr("\"No admittance, youngster.\"");break;
      case 1:addstr("\"You're too young to work here.\"");break;
      case 2:addstr("\"Go play someplace else.\"");break;
      case 3:addstr("\"Where's your mother?\"");break;
      }
      break;
   case REJECTED_DRESSCODE:
      set_color(COLOR_RED,COLOR_BLACK,1);
      switch(LCSrandom(1))
      {
      case 0:addstr("\"Employees only.\"");break;
      }
      break;
   case REJECTED_SMELLFUNNY:
      set_color(COLOR_RED,COLOR_BLACK,1);
      switch(LCSrandom(4))
      {
      case 0:addstr("\"You don't work here, do you?\"");break;
      case 1:addstr("\"Hmm... can I see your badge?\"");break;
      case 2:addstr("\"There's just something off about you.\"");break;
      case 3:addstr("\"You must be new. You'll need your badge.\"");break;
      }
      break;
   case REJECTED_BLOODYCLOTHES:
      set_color(COLOR_RED,COLOR_BLACK,1);
      switch(LCSrandom(5))
      {
      case 0:addstr("\"Good God! What is wrong with your clothes?\"");break;
      case 1:addstr("\"Are you hurt?! The aid station is the other way!\"");break;
      case 2:addstr("\"Your clothes, that's blood!\"");break;
      case 3:addstr("\"Blood?! That's more than a little suspicious...\"");break;
      case 4:addstr("\"Did you just butcher a cat?!\"");break;
      case 5:addstr("\"Blood everywhere...?\"");break;
      }
      break;
   case REJECTED_DAMAGEDCLOTHES:
      set_color(COLOR_RED,COLOR_BLACK,1);
      switch(LCSrandom(2))
      {
      case 0:addstr("\"Good God! What is wrong with your clothes?\"");break;
      case 1:addstr("\"Is that a damaged halloween costume?\"");break;
      }
      break;
   case REJECTED_SECONDRATECLOTHES:
      set_color(COLOR_RED,COLOR_BLACK,1);
      switch(LCSrandom(2))
      {
      case 0:addstr("\"That looks like you sewed it yourself.\"");break;
      case 1:addstr("\"That's a poor excuse for a uniform. Who are you?\"");break;
      }
      break;
   case REJECTED_WEAPONS:
      set_color(COLOR_RED,COLOR_BLACK,1);
      if(metaldetect)
      {
         addstr("-BEEEP- -BEEEP- -BEEEP-");
         sitealarm=1;
      }
      else switch(LCSrandom(5))
      {
      case 0:addstr("\"Put that away!\"");break;
      case 1:addstr("\"Hey, back off!\"");break;
      case 2:addstr("\"Don't try anything!\"");break;
      case 3:addstr("\"Are you here to make trouble?\"");break;
      case 4:addstr("\"Stay back!\"");break;
      }
      break;
   case NOT_REJECTED:
      set_color(COLOR_GREEN,COLOR_BLACK,1);
      
      switch(LCSrandom(4))
      {
      case 0:addstr("\"Move along.\"");break;
      case 1:addstr("\"Have a nice day.\"");break;
      case 2:addstr("\"Quiet day, today.\"");break;
      case 3:addstr("\"Go on in.\"");break;
      }
      break;
   }
   refresh();
   getch();

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
