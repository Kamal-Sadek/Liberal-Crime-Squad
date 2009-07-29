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
   if(!sitealarm)
   {
      if(!encounter[0].exists || encounter[0].type!=CREATURE_BOUNCER)
      {
         makecreature(encounter[0],CREATURE_BOUNCER);
      }
   }
}

void special_bouncer_assess_squad()
{
   bool autoadmit=0;
   char sleepername[80];
   for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->base==cursite&&pool[p]->type==CREATURE_BOUNCER && !LCSrandom(3))
      {
         autoadmit=1;
         strcpy(sleepername,pool[p]->name);
         strcpy(encounter[0].name,sleepername);
         break;
      }
   }
   makecreature(encounter[0],CREATURE_BOUNCER);
   if(autoadmit==1)encounter[0].align=1;
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
            if(activesquad->squad[s]->armor.type==ARMOR_NONE)
               if(rejected>REJECTED_NUDE)rejected=REJECTED_NUDE;
            if(!hasdisguise(*activesquad->squad[s],sitetype))
               if(rejected>REJECTED_DRESSCODE)rejected=REJECTED_DRESSCODE;
            // Busted, cheap, bloody clothes? Gone
            if(activesquad->squad[s]->armor.flag & ARMORFLAG_BLOODY)
               if(rejected>REJECTED_BLOODYCLOTHES)rejected=REJECTED_BLOODYCLOTHES;
            if(activesquad->squad[s]->armor.flag & ARMORFLAG_DAMAGED)
               if(rejected>REJECTED_DAMAGEDCLOTHES)rejected=REJECTED_DAMAGEDCLOTHES;
            if(activesquad->squad[s]->armor.quality!='1')
               if(rejected>REJECTED_SECONDRATECLOTHES)rejected=REJECTED_SECONDRATECLOTHES;
            // Suspicious weapons? Gone
            if(weaponcheck(*activesquad->squad[s],sitetype)>0)
               if(rejected>REJECTED_WEAPONS)rejected=REJECTED_WEAPONS;
            // Fail a tough disguise check? Gone
            if(disguisesite(sitetype) && disguiseskill()+LCSrandom(20)<40)
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
               if(activesquad->squad[s]->gender_liberal != GENDER_NEUTRAL ||
                  activesquad->squad[s]->gender_liberal != GENDER_MALE)
               {
                  // Not a man by your own definition either
                  if(rejected>REJECTED_FEMALE)rejected=REJECTED_FEMALE;
               }
               else if(disguisesite(sitetype) && disguiseskill()+LCSrandom(20)<40 && law[LAW_GAY]!=2)
               {
                  // Not skilled enough to pull it off
                  if(rejected>REJECTED_FEMALEISH)rejected=REJECTED_FEMALEISH;
               }
            }
            // High security in gentleman's club? Gone
            if(sitetype==SITE_BUSINESS_CIGARBAR && location[cursite]->highsecurity)
               if(rejected>REJECTED_GUESTLIST)rejected=REJECTED_GUESTLIST;
         }
      }
      move(17,1);
      switch(rejected)
      {
      case REJECTED_NUDE:
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(3))
         {
         case 0:addstr("\"No shirt, no underpants, no service.\"");break;
         case 1:addstr("\"Put some clothes on! That's disgusting.\"");break;
         case 2:addstr("\"No! No, you can't come in naked! God!!\"");break;
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
         switch(LCSrandom(3))
         {
         case 0:addstr("\"Move along ma'am, this club's for men.\"");break;
         case 1:addstr("\"This 'ain't no sewing circle, ma'am.\"");break;
         case 2:addstr("\"Sorry ma'am, this place is only for the men.\"");break;
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
         
         switch(LCSrandom(3))
         {
         case 0:addstr("\"Keep it civil and don't drink too much.\"");break;
         case 1:addstr("\"Let me get the door for you.\"");break;
         case 2:addstr("\"Ehh, alright, go on in.\"");break;
         }
         break;
      }
      refresh();
      getch();
   }
   else encounter[0].exists=0;
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   if(rejected<NOT_REJECTED)
   {
      levelmap[locx][locy+1][locz].flag |= SITEBLOCK_LOCKED;
      levelmap[locx][locy+1][locz].flag |= SITEBLOCK_CLOCK;
   }
   else levelmap[locx][locy+1][locz].flag &= ~SITEBLOCK_DOOR;
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
            juiceparty(3);
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
   move(16,1);
   addstr("The landlord's office is the first door");
   move(17,1);
   addstr("on the left.");
   getch();
}



void special_nuclear_onoff(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
	  if(law[LAW_NUCLEARPOWER]=2)
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
      addstr("Attempt to shut down the reactor? (Yes or No)");
	  }
      refresh();

      int c=getch();
      translategetch(c);

      if(c=='y')
      {
         clearmessagearea();
         levelmap[locx][locy][locz].special=-1;

         char max=15;
         Creature* maxs=0;

         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL&&activesquad->squad[p]->alive)
            {
               if(activesquad->squad[p]->skillval(SKILL_SCIENCE)*4+
                  activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE)>max)
               {
                  maxs=activesquad->squad[p];
                  max=activesquad->squad[p]->skillval(SKILL_SCIENCE)*4+
                      activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE);
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
	  if(law[LAW_NUCLEARPOWER]=2)
	  {
	  move(17,1);
      addstr("The nuclear waste gets released into the state's water supply!");
            change_public_opinion(VIEW_NUCLEARPOWER,15,0,95);
            refresh();
            getch();

            juiceparty(20); // Instant juice!
            
            sitestory->crime.push_back(CRIME_SHUTDOWNREACTOR);

	  }
	  else
	  {
            move(17,1);
            addstr("The lights dim...  power must be out state-wide.");
            change_public_opinion(VIEW_NUCLEARPOWER,15,0,95);
            refresh();
            getch();

            juiceparty(20); // Instant juice!
            
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

            juiceparty(5);
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
            juiceparty(4);
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
               alienationcheck(0);
            }
            else
            {
               alienationcheck(0);
               noticecheck(-1);
            }
         }
         else if(actual)
         {
            alienationcheck(0);
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

            juiceparty(10);
            sitecrime+=20;

            int time=20+LCSrandom(10);
            if(time<1)time=1;
            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

            if(mode==GAMEMODE_CHASECAR||
                  mode==GAMEMODE_CHASEFOOT)printchaseencounter();
            else printencounter();
            refresh();

            partyrescue();
         }

         if(actual)
         {
            alienationcheck(1);
            noticecheck(-1);
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

            juiceparty(10);
            sitecrime+=20;

            int time=20+LCSrandom(10);
            if(time<1)time=1;
            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

            if(mode==GAMEMODE_CHASECAR||
                  mode==GAMEMODE_CHASEFOOT)printchaseencounter();
            else printencounter();
            refresh();

            partyrescue();
         }

         if(actual)
         {
            alienationcheck(1);
            noticecheck(-1);
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

         for(p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL)
            {
               if(activesquad->squad[p]->alive)
               {
                  if((activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
                     activesquad->squad[p]->attval(ATTRIBUTE_HEART)+
                     activesquad->squad[p]->skillval(SKILL_PERSUASION)+
                     activesquad->squad[p]->skillval(SKILL_LAW)*2)>maxattack)
                  {
                     maxattack=activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
                        activesquad->squad[p]->attval(ATTRIBUTE_HEART)+
                        activesquad->squad[p]->skillval(SKILL_PERSUASION)+
                        activesquad->squad[p]->skillval(SKILL_LAW)*2;
                  }
               }
            }
         }

         vector<int> goodp;

         for(p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL)
            {
               if(activesquad->squad[p]->alive)
               {
                  if((activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
                     activesquad->squad[p]->attval(ATTRIBUTE_HEART)+
                     activesquad->squad[p]->skillval(SKILL_PERSUASION)+
                     activesquad->squad[p]->skillval(SKILL_LAW)*2)==maxattack)
                  {
                     goodp.push_back(p);
                  }
               }
            }
         }

         if(goodp.size()>0)
         {
            int p=goodp[LCSrandom(goodp.size())];

            short aroll=LCSrandom(21)+
               activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
               activesquad->squad[p]->attval(ATTRIBUTE_HEART)+
               activesquad->squad[p]->skillval(SKILL_PERSUASION)+
               activesquad->squad[p]->skillval(SKILL_LAW)*2;
            short troll=LCSrandom(21)+20;
            activesquad->squad[p]->train(SKILL_PERSUASION,troll);
            activesquad->squad[p]->train(SKILL_LAW,troll);

            if(aroll>troll)succeed=1;

            if(succeed)
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr(activesquad->squad[p]->name);
               addstr(" works the room like Twelve Angry Men, and the jury");
               move(17,1);
               addstr("concludes that the ");
               switch(LCSrandom(16))
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
               juiceparty(10);
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



void special_prison_control(void)
{
   do
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You've found the prison control room.");
      move(17,1);
      addstr("Free the prisoners? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

      if(c=='y')
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

         int time=20+LCSrandom(10);
         if(time<1)time=1;
         if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

         if(mode==GAMEMODE_CHASECAR||
                  mode==GAMEMODE_CHASEFOOT)printchaseencounter();
         else printencounter();
         refresh();

         partyrescue();

         alienationcheck(1);
         noticecheck(-1);
         levelmap[locx][locy][locz].special=-1;
         sitecrime+=30;
         juiceparty(10);
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
      addstr("You've found the Intelligence Super Computer.");
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
            
            juiceparty(10);

            itemst *it=new itemst;
               it->type=ITEM_LOOT;
               it->loottype=LOOT_INTHQDISK;
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
            noticecheck(-1);
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
   noticecheck(-1);
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
         noticecheck(-1);
         levelmap[locx][locy][locz].special=-1;
         sitecrime++;
         juiceparty(2);
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
         
         alienationcheck(1);
         noticecheck(-1);
         levelmap[locx][locy][locz].special=-1;
         sitecrime+=2;
         juiceparty(2);
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
            clearmessagearea();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("The Squad has found some... very comprising photos.");

            itemst *it=new itemst;
               it->type=ITEM_LOOT;
               it->loottype=LOOT_CEOPHOTOS;
            activesquad->loot.push_back(it);

            juiceparty(10);
            sitecrime+=20;

            int time=20+LCSrandom(10);
            if(time<1)time=1;
            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

            refresh();
            getch();
         }

         if(actual)
         {
            alienationcheck(1);
            noticecheck(-1);
            levelmap[locx][locy][locz].special=-1;
            sitecrime+=3;
            sitestory->crime.push_back(CRIME_HOUSE_PHOTOS);

            criminalizeparty(LAWFLAG_THEFT);
         }

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

            itemst *it=new itemst;
               it->type=ITEM_LOOT;
               it->loottype=LOOT_CORPFILES;
            activesquad->loot.push_back(it);
            
            juiceparty(10);
            sitecrime+=20;

            int time=20+LCSrandom(10);
            if(time<1)time=1;
            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

            refresh();
            getch();
         }

         if(actual)
         {
            alienationcheck(1);
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
      move(16,1);
      addstr("You've found a Cable News broadcasting studio.");
      move(17,1);
      addstr("Start an impromptu news program? (Yes or No)");

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
