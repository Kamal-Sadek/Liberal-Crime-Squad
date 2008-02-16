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
      move(16,1);
      addstr("You see the nuclear power plant control room.");
      move(17,1);
      addstr("Attempt to shut down the reactor? (Yes or No)");

      refresh();

      int c=getch();
      translategetch(c);

      if(c=='y')
      {
         clearmessagearea();
         levelmap[locx][locy][locz].special=-1;

         char max=30;
         creaturest* maxs=0;

         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL&&activesquad->squad[p]->alive)
            {
               if(activesquad->squad[p]->skill[SKILL_SCIENCE]*6+
                  activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE)>max)
               {
                  maxs=activesquad->squad[p];
                  max=activesquad->squad[p]->skill[SKILL_SCIENCE]*6+
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

            move(17,1);
            addstr("The lights dim...  power must be out state-wide.");

            change_public_opinion(VIEW_NUCLEARPOWER,15,0,95);

            refresh();
            getch();

            juiceparty(20); // Instant juice!
            
            sitestory->crime.push_back(CRIME_SHUTDOWNREACTOR);
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
      addstr("You see horrible misshapen creatures in a locked cage.");
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

         if(unlock(UNLOCK_DOOR,actual))
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
         }

         if(actual)
         {
            alienationcheck(1);
            noticecheck(-1);
            levelmap[locx][locy][locz].special=-1;
            sitecrime+=3;
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

         if(unlock(UNLOCK_DOOR,actual))
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
                     activesquad->squad[p]->skill[SKILL_PERSUASION]+
                     activesquad->squad[p]->skill[SKILL_LAW]*2)>maxattack)
                  {
                     maxattack=activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
                        activesquad->squad[p]->attval(ATTRIBUTE_HEART)+
                        activesquad->squad[p]->skill[SKILL_PERSUASION]+
                        activesquad->squad[p]->skill[SKILL_LAW]*2;
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
                     activesquad->squad[p]->skill[SKILL_PERSUASION]+
                     activesquad->squad[p]->skill[SKILL_LAW]*2)==maxattack)
                  {
                     goodp.push_back(p);
                  }
               }
            }
         }

         if(goodp.size()>0)
         {
            int p=goodp[LCSrandom(goodp.size())];

            short aroll=LCSrandom(21)+activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
               activesquad->squad[p]->attval(ATTRIBUTE_HEART)+LCSrandom(activesquad->squad[p]->skill[SKILL_PERSUASION]+1)+
               LCSrandom(activesquad->squad[p]->skill[SKILL_LAW]+1)*2;
            short troll=LCSrandom(21)+20;
            activesquad->squad[p]->skill_ip[SKILL_PERSUASION]+=troll;
            activesquad->squad[p]->skill_ip[SKILL_LAW]+=troll;

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
               switch(LCSrandom(10))
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
         sitecrime+=3;
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

   refresh();
   getch();

   int time=20+LCSrandom(10);
   if(time<1)time=1;
   if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;
   
   alienationcheck(0);
   noticecheck(-1);
   levelmap[locx][locy][locz].flag|=SITEBLOCK_GRAFFITI;
   struct sitechangest change(locx,locy,locz,SITEBLOCK_GRAFFITI);
   location[cursite]->changes.push_back(change);
   sitecrime++;

   criminalizeparty(LAWFLAG_VANDALISM);

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
         if(radio_broadcast())levelmap[locx][locy][locz].special=-1;

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
         if(news_broadcast())levelmap[locx][locy][locz].special=-1;

         return;
      }
      else if(c=='n')return;

   }while(1);
}
