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

/* unlock attempt */
char unlock(short type,char &actual)
{
   int p;
   int difficulty=0;

   switch(type)
   {
      case UNLOCK_DOOR:difficulty=10;break;
      case UNLOCK_CAGE:difficulty=10;break;
      case UNLOCK_SAFE:difficulty=20;break;
   }

   int maxattack=0;

   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(activesquad->squad[p]->alive)
         {
            if(activesquad->squad[p]->skill[SKILL_SECURITY]>maxattack)
            {
               maxattack=activesquad->squad[p]->skill[SKILL_SECURITY];
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
            if(activesquad->squad[p]->skill[SKILL_SECURITY]==maxattack)
            {
               goodp.push_back(p);
            }
         }
      }
   }

   if(goodp.size()>0)
   {
      int p=goodp[LCSrandom(goodp.size())];

      int aroll=LCSrandom(11)+maxattack;
      activesquad->squad[p]->skill_ip[SKILL_SECURITY]+=difficulty;

      if(aroll>difficulty)
      {
         clearmessagearea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         addstr(activesquad->squad[p]->name);
         addstr(" ");
         switch(type)
         {
            case UNLOCK_DOOR:addstr("unlocks the door");break;
            case UNLOCK_CAGE:addstr("unlocks the cage");break;
            case UNLOCK_SAFE:addstr("cracks the safe");break;
         }
         addstr("!");
         refresh();
         getch();

         actual=1;
         return 1;
      }
      else
      {
         clearmessagearea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         addstr(activesquad->squad[p]->name);
         addstr(" doesn't succeed.");
         refresh();
         getch();

         actual=1;
         return 0;
      }
   }
   else
   {
      clearmessagearea();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You can't find anyone to do the job.");
      refresh();
      getch();
   }

   actual=0;
   return 0;
}



/* bash attempt */
char bash(short type,char &actual)
{
   int difficulty=0;
   char crowable=0;
   int p = 0;

   switch(type)
   {
      case BASH_DOOR:difficulty=15;crowable=1;break;
   }

   if(crowable)
   {
      if(!squadhasitem(*activesquad,ITEM_WEAPON,WEAPON_CROWBAR))crowable=0;
   }

   int maxattack=0;

   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(activesquad->squad[p]->alive)
         {
            if(activesquad->squad[p]->attval(ATTRIBUTE_STRENGTH)*
               (bashstrengthmod(activesquad->squad[p]->weapon.type)+1)>maxattack)
            {
               maxattack=activesquad->squad[p]->attval(ATTRIBUTE_STRENGTH)*
                  (bashstrengthmod(activesquad->squad[p]->weapon.type)+1);
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
            if(activesquad->squad[p]->attval(ATTRIBUTE_STRENGTH)*
               (bashstrengthmod(activesquad->squad[p]->weapon.type)+1)==maxattack)
            {
               goodp.push_back(p);
            }
         }
      }
   }

   if(goodp.size()>0)
   {
      int p=goodp[LCSrandom(goodp.size())];

      int aroll=LCSrandom(11)+maxattack;

      if(aroll>difficulty||crowable)
      {
         clearmessagearea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         addstr(activesquad->squad[p]->name);
         addstr(" ");
         switch(type)
         {
            case BASH_DOOR:
            {
               if(crowable)addstr("uses a crowbar on the door");
               else addstr("bashes in the door");
               break;
            }
         }
         addstr("!");
         refresh();
         getch();

         actual=1;
         return 1;
      }
      else
      {
         clearmessagearea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         addstr(activesquad->squad[p]->name);
         switch(type)
         {
            case BASH_DOOR:addstr(" slams the door");break;
         }
         addstr("!");
         refresh();
         getch();

         actual=1;
         return 0;
      }
   }
   else
   {
      clearmessagearea();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You can't find anyone to do the job.");
      refresh();
      getch();
   }

   actual=0;
   return 0;
}



/* returns the bash bonus provided by the specified weapon */
long bashstrengthmod(int t)
{
   switch(t)
   {
      case WEAPON_BASEBALLBAT:
      case WEAPON_SWORD:
      case WEAPON_DAISHO:
      case WEAPON_MAUL:
         return 5;
      case WEAPON_CROWBAR:
      case WEAPON_SHOTGUN_PUMP:
      case WEAPON_STAFF:
      case WEAPON_NIGHTSTICK:
      case WEAPON_SMG_MP5:
      case WEAPON_SEMIRIFLE_AR15:
      case WEAPON_CARBINE_M4:
      case WEAPON_AUTORIFLE_M16:
      case WEAPON_AUTORIFLE_AK47:
      case WEAPON_PITCHFORK:
         return 3;
      case WEAPON_REVOLVER_22:
      case WEAPON_REVOLVER_44:
      case WEAPON_SEMIPISTOL_9MM:
      case WEAPON_SEMIPISTOL_45:
      case WEAPON_HAMMER:
      case WEAPON_CROSS:
         return 1;
      default:
         return 0;
   }
}



/* computer hack attempt */
char hack(short type,char &actual)
{
   int difficulty=0;
   int p;

   switch(type)
   {
      case HACK_SUPERCOMPUTER:difficulty=20;break;
   }

   int maxattack=0;
   char blind=0;

   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(activesquad->squad[p]->alive)
         {
            if((activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE)+
               activesquad->squad[p]->skill[SKILL_COMPUTERS])>maxattack)
            {
               if(activesquad->squad[p]->skill[SKILL_COMPUTERS]>0)
               {
                  if(!activesquad->squad[p]->special[SPECIALWOUND_RIGHTEYE]&&
                     !activesquad->squad[p]->special[SPECIALWOUND_LEFTEYE])
                  {
                     blind=1;
                  }
                  else
                  {
                     maxattack=activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE)+
                        activesquad->squad[p]->skill[SKILL_COMPUTERS];
                  }
               }
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
            if((activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE)+
               activesquad->squad[p]->skill[SKILL_COMPUTERS])==maxattack)
            {
               if(activesquad->squad[p]->skill[SKILL_COMPUTERS]>0)
               {
                  if(activesquad->squad[p]->special[SPECIALWOUND_RIGHTEYE]||
                     activesquad->squad[p]->special[SPECIALWOUND_LEFTEYE])
                  {
                     goodp.push_back(p);
                  }
               }
            }
         }
      }
   }

   if(goodp.size()>0)
   {
      int p=goodp[LCSrandom(goodp.size())];

      int aroll=LCSrandom(11)+maxattack;
      activesquad->squad[p]->skill_ip[SKILL_COMPUTERS]+=difficulty;

      if(aroll>difficulty)
      {
         clearmessagearea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         addstr(activesquad->squad[p]->name);
         addstr(" ");
         switch(type)
         {
            case HACK_SUPERCOMPUTER:addstr("has hacked into the Super Computer!");break;
         }
         addstr("!");
         refresh();
         getch();

         actual=1;
         return 1;
      }
      else
      {
         clearmessagearea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         addstr(activesquad->squad[p]->name);
         switch(type)
         {
            case HACK_SUPERCOMPUTER:addstr(" fails to hack the computer.");break;
         }
         addstr(".");
         refresh();
         getch();

         actual=1;
         return 0;
      }
   }
   else
   {
      clearmessagearea();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("You can't find anyone to do the job.");
      if(blind)
      {
         refresh();
         getch();
         move(17,1);
         addstr("Including the UNSIGHTED HACKER you brought.");
      }
      refresh();
      getch();
   }

   actual=0;
   return 0;
}



/* run a radio broadcast */
char radio_broadcast(void)
{
   sitealarm=1;

   int enemy=0;
   for(int e=0;e<ENCMAX;e++)
   {
      if(encounter[e].exists&&encounter[e].alive)
      {
         if(encounter[e].align==-1)enemy++;
      }
   }

   if(enemy>0)
   {
      clearmessagearea();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("The Conservatives in the room hurry the Squad, so");
      move(17,1);
      addstr("the broadcast never happens.");

      refresh();
      getch();
      return 0;
   }

   criminalizeparty(LAWFLAG_DISTURBANCE);

   clearmessagearea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   addstr("The Squad takes control of the microphone and");
   move(17,1);
   int viewhit=LCSrandom(VIEWNUM);
   switch(viewhit)
   {
      case VIEW_ABORTION:addstr("discusses abortion.");break;
      case VIEW_GAY:addstr("discusses homosexual rights.");break;
      case VIEW_DEATHPENALTY:addstr("examines the death penalty.");break;
      case VIEW_NUCLEARPOWER:addstr("runs down nuclear power.");break;
      case VIEW_ANIMALRESEARCH:addstr("discusses the horrors of animal research.");break;
      case VIEW_POLICEBEHAVIOR:addstr("goes over cases of police brutality.");break;
      case VIEW_PRISONS:addstr("discusses the prison system.");break;
      case VIEW_INTELLIGENCE:addstr("debates privacy law.");break;
      case VIEW_FREESPEECH:addstr("talks about free speech.");break;
      case VIEW_GENETICS:addstr("discusses the implications of genetic research.");break;
      case VIEW_JUSTICES:addstr("talks about the record of a Conservative judge.");break;
      case VIEW_SWEATSHOPS:addstr("brings details about sweatshops to light.");break;
      case VIEW_POLLUTION:addstr("does a show on industrial pollution.");break;
      case VIEW_CORPORATECULTURE:addstr("jokes about corporate culture.");break;
      case VIEW_CEOSALARY:addstr("gives examples of CEO excesses.");break;
      case VIEW_AMRADIO:addstr("discusses other AM radio shows.");break;
      case VIEW_CABLENEWS:addstr("talks about Cable News.");break;
      case VIEW_LIBERALCRIMESQUAD:addstr("lets people know about the Liberal Crime Squad.");break;
      case VIEW_LIBERALCRIMESQUADPOS:addstr("extols the virtues of the Liberal Crime Squad.");break;
   }

   refresh();
   getch();

   int segmentpower=0;
   int usegmentpower;
   int partysize=0;
   int p = 0;

   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(!activesquad->squad[p]->alive)continue;

         segmentpower+=LCSrandom(activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE));
         segmentpower+=activesquad->squad[p]->attval(ATTRIBUTE_HEART);
         segmentpower+=LCSrandom(activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA));
         segmentpower+=activesquad->squad[p]->skill[SKILL_MUSIC];
         segmentpower+=activesquad->squad[p]->skill[SKILL_RELIGION];
         segmentpower+=activesquad->squad[p]->skill[SKILL_SCIENCE];
         segmentpower+=activesquad->squad[p]->skill[SKILL_BUSINESS];
         segmentpower+=activesquad->squad[p]->skill[SKILL_PERSUASION];
         activesquad->squad[p]->skill_ip[SKILL_PERSUASION]+=50;
         partysize++;
      }
   }

   int segmentbonus=segmentpower/4;
   if(partysize>1)segmentpower/=partysize;
   segmentpower+=segmentbonus;

   clearmessagearea();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   if(segmentpower<5)addstr("The Squad sounds wholly insane.");
   else if(segmentpower<10)addstr("The show really sucks.");
   else if(segmentpower<15)addstr("It is a very boring hour.");
   else if(segmentpower<20)addstr("It is mediocre radio.");
   else if(segmentpower<30)addstr("The show was all right.");
   else if(segmentpower<40)addstr("The Squad put on a good show.");
   else if(segmentpower<50)addstr("It was thought-provoking, even humorous.");
   else addstr("It was the best hour of AM radio EVER.");

   refresh();
   getch();

   //CHECK PUBLIC OPINION
   change_public_opinion(VIEW_LIBERALCRIMESQUAD,10,0);
   change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,(segmentpower-10)/2,0);
   if(viewhit!=VIEW_LIBERALCRIMESQUAD)change_public_opinion(viewhit,(segmentpower-10)/2,1,80);
   else change_public_opinion(viewhit,segmentpower/2,1);

   //PRISONER PARTS
   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(activesquad->squad[p]->prisoner!=NULL)
         {
            if(activesquad->squad[p]->prisoner->type==CREATURE_RADIOPERSONALITY)
            {
               viewhit=LCSrandom(VIEWNUM);
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("The hostage ");
               addstr(activesquad->squad[p]->prisoner->name);
               addstr(" is forced on to");
               move(17,1);
               switch(viewhit)
               {
                  case VIEW_ABORTION:addstr("discuss abortion.");break;
                  case VIEW_GAY:addstr("discuss homosexual rights.");break;
                  case VIEW_DEATHPENALTY:addstr("examine the death penalty.");break;
                  case VIEW_NUCLEARPOWER:addstr("run down nuclear power.");break;
                  case VIEW_ANIMALRESEARCH:addstr("discuss the horrors of animal research.");break;
                  case VIEW_POLICEBEHAVIOR:addstr("go over cases of police brutality.");break;
                  case VIEW_PRISONS:addstr("discuss the prison system.");break;
                  case VIEW_INTELLIGENCE:addstr("advocate privacy laws.");break;
                  case VIEW_FREESPEECH:addstr("talk about free speech.");break;
                  case VIEW_GENETICS:addstr("discuss the implications of genetic research.");break;
                  case VIEW_JUSTICES:addstr("talk about the record of a Conservative judge.");break;
                  case VIEW_SWEATSHOPS:addstr("bring details about sweatshops to light.");break;
                  case VIEW_POLLUTION:addstr("do a segment on industrial pollution.");break;
                  case VIEW_CORPORATECULTURE:addstr("joke about corporate culture.");break;
                  case VIEW_CEOSALARY:addstr("give examples of CEO excesses.");break;
                  case VIEW_AMRADIO:addstr("make fun of its own AM radio show.");break;
                  case VIEW_CABLENEWS:addstr("talk about Cable News.");break;
                  case VIEW_LIBERALCRIMESQUAD:addstr("let people know about the Liberal Crime Squad.");break;
                  case VIEW_LIBERALCRIMESQUADPOS:addstr("extol the virtues of the Liberal Crime Squad.");break;
               }

               usegmentpower=10; //FAME BONUS
               usegmentpower+=LCSrandom(activesquad->squad[p]->prisoner->attval(ATTRIBUTE_INTELLIGENCE));
               usegmentpower+=activesquad->squad[p]->prisoner->attval(ATTRIBUTE_HEART);
               usegmentpower+=LCSrandom(activesquad->squad[p]->prisoner->attval(ATTRIBUTE_CHARISMA));
               usegmentpower+=activesquad->squad[p]->prisoner->skill[SKILL_PERSUASION];

               if(viewhit!=VIEW_LIBERALCRIMESQUAD)change_public_opinion(viewhit,(usegmentpower-10)/2,1,80);
               else change_public_opinion(viewhit,usegmentpower/2,1);

               segmentpower+=usegmentpower;

               refresh();
               getch();
            }
            else
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr(activesquad->squad[p]->prisoner->name);
               addstr(", the hostage, is kept off-air.");

               refresh();
               getch();
            }
         }
      }
   }

   if(sitealienate>=1&&segmentpower>=40)
   {
      sitealienate=0;

      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("Moderates at the station appreciated the show.");
      move(17,1);
      addstr("They no longer feel alienated.");

      refresh();
      getch();
   }

   //POST-SECURITY BLITZ IF IT SUCKED
   if(segmentpower<50)
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("Security is waiting for the Squad");
      move(17,1);
      addstr("after the show!");

      refresh();
      getch();

      int numleft=LCSrandom(8)+2;
      for(int e=0;e<ENCMAX;e++)
      {
         if(!encounter[e].exists)
         {
            makecreature(encounter[e],CREATURE_SECURITYGUARD);
            numleft--;
         }
         if(numleft==0)break;
      }
   }
   else
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("The show was so good that security listened to it");
      move(17,1);
      addstr("at their desks.  The Squad might yet escape.");

      refresh();
      getch();
   }

   return 1;
}



/* run a tv broadcast */
char news_broadcast(void)
{
   sitealarm=1;
   int p;

   int enemy=0;
   for(int e=0;e<ENCMAX;e++)
   {
      if(encounter[e].exists&&encounter[e].alive)
      {
         if(encounter[e].align==-1)enemy++;
      }
   }

   if(enemy>0)
   {
      clearmessagearea();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("The Conservatives in the room hurry the Squad, so");
      move(17,1);
      addstr("the broadcast never happens.");

      refresh();
      getch();
      return 0;
   }

   criminalizeparty(LAWFLAG_DISTURBANCE);

   clearmessagearea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   addstr("The Squad steps in front of the cameras and");
   move(17,1);
   int viewhit=LCSrandom(VIEWNUM);
   switch(viewhit)
   {
      case VIEW_ABORTION:addstr("discusses abortion.");break;
      case VIEW_GAY:addstr("discusses homosexual rights.");break;
      case VIEW_DEATHPENALTY:addstr("examines the death penalty.");break;
      case VIEW_NUCLEARPOWER:addstr("runs down nuclear power.");break;
      case VIEW_ANIMALRESEARCH:addstr("discusses the horrors of animal research.");break;
      case VIEW_POLICEBEHAVIOR:addstr("goes over cases of police brutality.");break;
      case VIEW_PRISONS:addstr("discusses the prison system.");break;
      case VIEW_INTELLIGENCE:addstr("debates privacy law.");break;
      case VIEW_FREESPEECH:addstr("talks about free speech.");break;
      case VIEW_GENETICS:addstr("discusses the implications of genetic research.");break;
      case VIEW_JUSTICES:addstr("talks about the record of a Conservative judge.");break;
      case VIEW_SWEATSHOPS:addstr("brings details about sweatshops to light.");break;
      case VIEW_POLLUTION:addstr("does a show on industrial pollution.");break;
      case VIEW_CORPORATECULTURE:addstr("jokes about corporate culture.");break;
      case VIEW_CEOSALARY:addstr("gives examples of CEO excesses.");break;
      case VIEW_AMRADIO:addstr("discusses AM radio shows.");break;
      case VIEW_CABLENEWS:addstr("talks about Cable News.");break;
      case VIEW_LIBERALCRIMESQUAD:addstr("lets people know about the Liberal Crime Squad.");break;
      case VIEW_LIBERALCRIMESQUADPOS:addstr("extols the virtues of the Liberal Crime Squad.");break;
   }

   refresh();
   getch();

   int segmentpower=0;
   int usegmentpower;
   int partysize=0;
   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(!activesquad->squad[p]->alive)continue;

         segmentpower+=LCSrandom(activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE));
         segmentpower+=activesquad->squad[p]->attval(ATTRIBUTE_HEART);
         segmentpower+=LCSrandom(activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA));
         segmentpower+=activesquad->squad[p]->skill[SKILL_MUSIC];
         segmentpower+=activesquad->squad[p]->skill[SKILL_RELIGION];
         segmentpower+=activesquad->squad[p]->skill[SKILL_SCIENCE];
         segmentpower+=activesquad->squad[p]->skill[SKILL_BUSINESS];
         segmentpower+=activesquad->squad[p]->skill[SKILL_PERSUASION];
         activesquad->squad[p]->skill_ip[SKILL_PERSUASION]+=50;
         partysize++;
      }
   }

   int segmentbonus=segmentpower/4;
   if(partysize>1)segmentpower/=partysize;
   segmentpower+=segmentbonus;

   clearmessagearea();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   if(segmentpower<5)addstr("The Squad looks completely insane.");
   else if(segmentpower<10)addstr("The show really sucks.");
   else if(segmentpower<15)addstr("It is a very boring hour.");
   else if(segmentpower<20)addstr("It is mediocre TV.");
   else if(segmentpower<30)addstr("The show was all right.");
   else if(segmentpower<40)addstr("The Squad put on a good show.");
   else if(segmentpower<50)addstr("It was thought-provoking, even humorous.");
   else addstr("It was the best hour of Cable TV EVER.");

   refresh();
   getch();

   //CHECK PUBLIC OPINION
   change_public_opinion(VIEW_LIBERALCRIMESQUAD,10,0);
   change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,(segmentpower-50)/10,0);
   if(viewhit!=VIEW_LIBERALCRIMESQUAD)change_public_opinion(viewhit,(segmentpower-50)/10,1,80);
   else change_public_opinion(viewhit,segmentpower/10,1);

   //PRISONER PARTS
   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(activesquad->squad[p]->prisoner!=NULL)
         {
            if(activesquad->squad[p]->prisoner->type==CREATURE_NEWSANCHOR)
            {
               viewhit=LCSrandom(VIEWNUM);
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("The hostage ");
               addstr(activesquad->squad[p]->prisoner->name);
               addstr(" is forced on to");
               move(17,1);
               switch(viewhit)
               {
                  case VIEW_ABORTION:addstr("discuss abortion.");break;
                  case VIEW_GAY:addstr("discuss homosexual rights.");break;
                  case VIEW_DEATHPENALTY:addstr("examine the death penalty.");break;
                  case VIEW_NUCLEARPOWER:addstr("run down nuclear power.");break;
                  case VIEW_ANIMALRESEARCH:addstr("discuss the horrors of animal research.");break;
                  case VIEW_POLICEBEHAVIOR:addstr("go over cases of police brutality.");break;
                  case VIEW_PRISONS:addstr("discuss the prison system.");break;
                  case VIEW_INTELLIGENCE:addstr("advocate privacy laws.");break;
                  case VIEW_FREESPEECH:addstr("talk about free speech.");break;
                  case VIEW_GENETICS:addstr("discuss the implications of genetic research.");break;
                  case VIEW_JUSTICES:addstr("talk about the record of a Conservative judge.");break;
                  case VIEW_SWEATSHOPS:addstr("bring details about sweatshops to light.");break;
                  case VIEW_POLLUTION:addstr("do a segment on industrial pollution.");break;
                  case VIEW_CORPORATECULTURE:addstr("joke about corporate culture.");break;
                  case VIEW_CEOSALARY:addstr("give examples of CEO excesses.");break;
                  case VIEW_AMRADIO:addstr("talk about AM radio.");break;
                  case VIEW_CABLENEWS:addstr("make fun of its own Cable News show.");break;
                  case VIEW_LIBERALCRIMESQUAD:addstr("let people know about the Liberal Crime Squad.");break;
                  case VIEW_LIBERALCRIMESQUADPOS:addstr("extol the virtues of the Liberal Crime Squad.");break;
               }

               usegmentpower=10; //FAME BONUS
               usegmentpower+=LCSrandom(activesquad->squad[p]->prisoner->attval(ATTRIBUTE_INTELLIGENCE));
               usegmentpower+=activesquad->squad[p]->prisoner->attval(ATTRIBUTE_HEART);
               usegmentpower+=LCSrandom(activesquad->squad[p]->prisoner->attval(ATTRIBUTE_CHARISMA));
               usegmentpower+=activesquad->squad[p]->prisoner->skill[SKILL_PERSUASION];

               if(viewhit!=VIEW_LIBERALCRIMESQUAD)change_public_opinion(viewhit,(usegmentpower-10)/2,1);
               else change_public_opinion(viewhit,usegmentpower/2,1);

               segmentpower+=usegmentpower;

               refresh();
               getch();
            }
            else
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr(activesquad->squad[p]->prisoner->name);
               addstr(", the hostage, is kept off-air.");

               refresh();
               getch();
            }
         }
      }
   }

   if(sitealienate>=1&&segmentpower>=40)
   {
      sitealienate=0;

      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("Moderates at the station appreciated the show.");
      move(17,1);
      addstr("They no longer feel alienated.");

      refresh();
      getch();
   }

   //POST-SECURITY BLITZ IF IT SUCKED
   if(segmentpower<50)
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("Security is waiting for the Squad");
      move(17,1);
      addstr("after the show!");

      refresh();
      getch();

      int numleft=LCSrandom(8)+2;
      for(int e=0;e<ENCMAX;e++)
      {
         if(!encounter[e].exists)
         {
            makecreature(encounter[e],CREATURE_SECURITYGUARD);
            numleft--;
         }
         if(numleft==0)break;
      }
   }
   else
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("The show was so entertaining that security watched it");
      move(17,1);
      addstr("at their desks.  The Squad might yet escape.");

      refresh();
      getch();
   }

   return 1;
}



/* rescues people held at the activeparty's current location */
void partyrescue(void)
{
   int freeslots=0;
   int p, pl;
   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)freeslots++;
   }
   int hostslots=0;
   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(activesquad->squad[p]->alive&&activesquad->squad[p]->prisoner==NULL)
         {
            hostslots++;
         }
      }
   }

   for(pl=0;pl<pool.size();pl++)
   {
      if(pool[pl]->location==cursite&&
         !(pool[pl]->flag & CREATUREFLAG_SLEEPER))
      {
         if(LCSrandom(2)&&freeslots)
         {
            for(int p=0;p<6;p++)
            {
               if(activesquad->squad[p]==NULL)
               {
                  activesquad->squad[p]=pool[pl];
                  activesquad->squad[p]->squadid=activesquad->id;
                  activesquad->squad[p]->lawflag[LAWFLAG_ESCAPED]++;
                  activesquad->squad[p]->flag|=CREATUREFLAG_JUSTESCAPED;
                  break;
               }
            }
            hostslots++;
            freeslots--;

            clearmessagearea();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("You've rescued ");
            addstr(pool[pl]->name);
            addstr(" from the Conservatives.");

            printparty();
            refresh();
            getch();

            pool[pl]->location=-1;
            pool[pl]->base=activesquad->squad[0]->base;
         }
      }
   }
   for(pl=0;pl<pool.size();pl++)
   {
      if(pool[pl]->location==cursite&&
         !(pool[pl]->flag & CREATUREFLAG_SLEEPER))
      {
         if(hostslots)
         {
            for(int p=0;p<6;p++)
            {
               if(activesquad->squad[p]!=NULL)
               {
                  if(activesquad->squad[p]->alive&&activesquad->squad[p]->prisoner==NULL)
                  {
                     activesquad->squad[p]->prisoner=pool[pl];
                     pool[pl]->squadid=activesquad->id;
                     pool[pl]->lawflag[LAWFLAG_ESCAPED]++;
                     pool[pl]->flag|=CREATUREFLAG_JUSTESCAPED;

                     clearmessagearea();

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(16,1);
                     addstr("You've rescued ");
                     addstr(pool[pl]->name);
                     addstr(" from the Conservatives.");

                     refresh();
                     getch();

                     clearmessagearea();
                     move(16,1);
                     addstr(pool[pl]->name);
                     addstr(" ");
                     switch(LCSrandom(3))
                     {
                        case 0:addstr("was tortured recently");break;
                        case 1:addstr("was beaten severely yesterday");break;
                        case 2:addstr("was on a hunger strike");break;
                     }
                     move(17,1);
                     addstr("so ");
                     addstr(activesquad->squad[p]->name);
                     addstr(" will have to haul a Liberal.");

                     pool[pl]->location=-1;
                     pool[pl]->base=activesquad->squad[p]->base;

                     printparty();
                     refresh();
                     getch();
                     break;
                  }
               }
            }

            hostslots--;
         }
         if(!hostslots)break;
      }
   }

   int stillpcount=0;
   char stillpname[200];
   for(pl=0;pl<pool.size();pl++)
   {
      if(pool[pl]->location==cursite&&
         !(pool[pl]->flag & CREATUREFLAG_SLEEPER))
      {
         stillpcount++;
         if(stillpcount==1)strcpy(stillpname,pool[pl]->name);
      }
   }

   if(stillpcount==1)
   {
      clearmessagearea();
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      move(16,1);
      addstr("There's nobody left to carry ");
      addstr(stillpname);
      addstr(".");
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(17,1);
      addstr("You'll have to come back later.");
      refresh();
      getch();
   }
   else if(stillpcount>1)
   {
      clearmessagearea();
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      move(16,1);
      addstr("There's nobody left to carry the others.");
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(17,1);
      addstr("You'll have to come back later.");
      refresh();
      getch();
   }
}



/* everybody reload! */
void reloadparty(void)
{
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;
      if(!activesquad->squad[p]->alive)continue;

      if(ammotype(activesquad->squad[p]->weapon.type)!=-1)
      {
         int ammomax=2;
         switch(ammotype(activesquad->squad[p]->weapon.type))
         {
            case CLIP_9:ammomax=15;break;
            case CLIP_45:ammomax=15;break;
            case CLIP_ASSAULT:ammomax=30;break;
            case CLIP_SMG:ammomax=15;break;
            case CLIP_22:ammomax=6;break;
            case CLIP_44:ammomax=6;break;
            case CLIP_BUCKSHOT:ammomax=6;break;
         }

         if(activesquad->squad[p]->weapon.ammo<ammomax&&activesquad->squad[p]->clip[ammotype(activesquad->squad[p]->weapon.type)]>0)
         {
            activesquad->squad[p]->weapon.ammo=ammomax;

            activesquad->squad[p]->clip[ammotype(activesquad->squad[p]->weapon.type)]--;

         }
      }
   }
}
