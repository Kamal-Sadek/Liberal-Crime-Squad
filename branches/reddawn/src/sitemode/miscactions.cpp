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

/* unlock attempt */
char unlock(short type,char &actual)
{
   int p;
   int difficulty=0;

   switch(type)
   {
      case UNLOCK_DOOR:
         if(securityable(location[cursite]->type) == 1)
            difficulty=DIFFICULTY_CHALLENGING;
         else if(securityable(location[cursite]->type) > 1)
            difficulty=DIFFICULTY_HARD;
         else
            difficulty=DIFFICULTY_EASY;
         break;
      case UNLOCK_CAGE:       difficulty=DIFFICULTY_VERYEASY;break;
      case UNLOCK_CAGE_HARD:  difficulty=DIFFICULTY_AVERAGE;break;
      case UNLOCK_CELL:       difficulty=DIFFICULTY_FORMIDABLE;break;
      case UNLOCK_ARMORY:     difficulty=DIFFICULTY_HEROIC;break;
      case UNLOCK_SAFE:       difficulty=DIFFICULTY_HEROIC;break;
      case UNLOCK_VAULT:      difficulty=DIFFICULTY_HEROIC;break;
   }

   int maxattack=-1;

   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(activesquad->squad[p]->alive)
         {
            if(activesquad->squad[p]->get_skill(SKILL_SECURITY)>maxattack)
            {
               maxattack=activesquad->squad[p]->get_skill(SKILL_SECURITY);
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
            if(activesquad->squad[p]->get_skill(SKILL_SECURITY)==maxattack)
            {
               goodp.push_back(p);
            }
         }
      }
   }

   if(goodp.size()>0)
   {
      int p=goodp[LCSrandom(goodp.size())];

	//lock pick succeeded.
      if(activesquad->squad[p]->skill_check(SKILL_SECURITY,difficulty))
      {
         //skill goes up in proportion to the chance of you failing.
         if(maxattack<=difficulty)
         {
            activesquad->squad[p]->train(SKILL_SECURITY,1+(difficulty-maxattack));
         }
         clearmessagearea(false);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         addstr(activesquad->squad[p]->name, gamelog);
         addstr(" ", gamelog);
         switch(type)
         {
            case UNLOCK_DOOR:addstr("unlocks the door", gamelog);break;
            case UNLOCK_CAGE_HARD:
            case UNLOCK_CAGE:addstr("unlocks the cage", gamelog);break;
            case UNLOCK_SAFE:addstr("cracks the safe", gamelog);break;
            case UNLOCK_ARMORY:addstr("opens the armory", gamelog);break;
            case UNLOCK_CELL:addstr("unlocks the cell", gamelog);break;
            case UNLOCK_VAULT:addstr("cracks the combo locks", gamelog);break;
         }
         addstr("!", gamelog);
         gamelog.newline();
         for(int j=0;j<6;j++) //If people witness a successful unlock, they learn a little bit.
         {
            if(j==p) continue;
            if(activesquad->squad[j]!=NULL&&
               activesquad->squad[j]->alive&&
               activesquad->squad[j]->get_skill(SKILL_SECURITY)<difficulty)
            {
               if(activesquad->squad[j]->alive)
               {
                  activesquad->squad[j]->train(SKILL_SECURITY,difficulty-activesquad->squad[j]->get_skill(SKILL_SECURITY));
               }
            }
         }

         refresh();
         getch();

         actual=1;
         return 1;
      }
      else
      {
         clearmessagearea(false);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         
         int i;
         //gain some experience for failing only if you could have succeeded.
         for (i = 0; i < 3; i++)
         {
            if(activesquad->squad[p]->skill_check(SKILL_SECURITY,difficulty))
            {
               activesquad->squad[p]->train(SKILL_SECURITY,10);
               
               addstr(activesquad->squad[p]->name, gamelog);
               addstr(" is close, but can't quite get the lock open.", gamelog);
               gamelog.newline();
               break;
            }
         }
         
         if (i == 3)
         {
            addstr(activesquad->squad[p]->name, gamelog);
            addstr(" can't figure the lock out.", gamelog);
            gamelog.newline();
         }
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
      addstr("You can't find anyone to do the job.", gamelog);
      gamelog.newline();
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
   case BASH_DOOR:
      if(!securityable(location[cursite]->type))
      {
         difficulty=DIFFICULTY_EASY; // Run down dump
         crowable=1;
      }
      else if(location[cursite]->type!=SITE_GOVERNMENT_PRISON&&
              location[cursite]->type!=SITE_GOVERNMENT_INTELLIGENCEHQ)
      {
         difficulty=DIFFICULTY_CHALLENGING; // Respectable place
         crowable=1;
      }
      else
      {
         difficulty=DIFFICULTY_FORMIDABLE; // Very high security
         crowable=0;
      }
      break;
   }

   if(crowable)
   {
      //if(!squadhasitem(*activesquad,ITEM_WEAPON,WEAPON_CROWBAR))
      //{
         crowable=0;
         
         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL)
            {
               if (activesquad->squad[p]->get_weapon().auto_breaks_locks())
                  crowable = 1;
            }
         }
      
         if(!crowable) //didn't find in hands of any squad member
         {
            for(int l=0;l<activesquad->loot.size();l++)
            {
               if(activesquad->loot[l]->is_weapon())
               {
                  Weapon *w = static_cast<Weapon*>(activesquad->loot[l]); //cast -XML
                   if (w->auto_breaks_locks())
                     crowable = 1;
               }
            }
         }
      //}
   }

   int maxattack=0, maxp=0;

   if(!crowable)
   {
      for(p=0;p<6;p++)
      {
         if(activesquad->squad[p]!=NULL)
         {
            if(activesquad->squad[p]->alive)
            {
               if(activesquad->squad[p]->get_attribute(ATTRIBUTE_STRENGTH,true)*
                  activesquad->squad[p]->get_weapon().get_bashstrengthmod()>maxattack)
               {
                  maxattack=static_cast<int>(activesquad->squad[p]->get_attribute(ATTRIBUTE_STRENGTH,true)*
                                             activesquad->squad[p]->get_weapon().get_bashstrengthmod());
                  maxp = p;
               }
            }
         }
      }
   }

   difficulty = static_cast<int>(difficulty/activesquad->squad[maxp]->get_weapon().get_bashstrengthmod());

   if(crowable||activesquad->squad[maxp]->attribute_check(ATTRIBUTE_STRENGTH,difficulty))
   {
      clearmessagearea(false);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr(activesquad->squad[maxp]->name, gamelog);
      addstr(" ", gamelog);
      switch(type)
      {
      case BASH_DOOR:
         if(crowable)
            addstr("uses a crowbar on the door", gamelog);
         else if(activesquad->squad[maxp]->get_weapon().get_bashstrengthmod()>1)
            addstr("smashes in the door", gamelog);
         else if(activesquad->squad[maxp]->flag&CREATUREFLAG_WHEELCHAIR)
            addstr("rams open the door", gamelog);
         else
            addstr("kicks in the door", gamelog);
         break;
      }
      addstr("!", gamelog);
      gamelog.newline();
      refresh();
      getch();

      int timer = 5;
      if(crowable)
         timer = 20;
      
      if(sitealarmtimer<0 || sitealarmtimer>timer)
         sitealarmtimer=timer;
      else
         sitealarmtimer=0;

      //Bashing doors in secure areas sets off alarms
      if((location[cursite]->type==SITE_GOVERNMENT_PRISON ||
          location[cursite]->type==SITE_GOVERNMENT_INTELLIGENCEHQ) &&
          sitealarm==0)
      {
         sitealarm=1;
         move(17,1);
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr("Alarms go off!", gamelog);
         gamelog.newline();
         refresh();
         getch();
      }

      actual=1;
      return 1;
   }
   else
   {
      clearmessagearea(false);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr(activesquad->squad[maxp]->name, gamelog);
      switch(type)
      {
         case BASH_DOOR:
            if(activesquad->squad[maxp]->flag&CREATUREFLAG_WHEELCHAIR)
               addstr(" rams into the door", gamelog);
            else
               addstr(" kicks the door", gamelog);
            break;
      }
      addstr("!", gamelog);
      gamelog.newline();
      refresh();
      getch();

      if(sitealarmtimer<0)
         sitealarmtimer=25;
      else if(sitealarmtimer>10)
         sitealarmtimer-=10;
      else
         sitealarmtimer=0;

      actual=1;
      return 0;
   }

   actual=0;
   return 0;
}


/* computer hack attempt */
char hack(short type,char &actual)
{
   int difficulty=0;
   int p;

   switch(type)
   {
   case HACK_SUPERCOMPUTER:difficulty=DIFFICULTY_HEROIC;break;
   case HACK_VAULT:difficulty=DIFFICULTY_CHALLENGING;break;
   }

   int maxattack=0;
   char blind=0;
   int hacker = -1;

   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(activesquad->squad[p]->alive && activesquad->squad[p]->get_skill(SKILL_COMPUTERS))
         {
            int roll = activesquad->squad[p]->skill_roll(SKILL_COMPUTERS);
            if(roll>maxattack)
            {
               if(!activesquad->squad[p]->special[SPECIALWOUND_RIGHTEYE]&&
                  !activesquad->squad[p]->special[SPECIALWOUND_LEFTEYE])
               {
                  blind=1;
               }
               else
               {
                  maxattack = roll;
                  hacker = p;
               }
            }
         }
      }
   }

   if(hacker > -1)
   {
      activesquad->squad[hacker]->train(SKILL_COMPUTERS,difficulty);

      if(maxattack>difficulty)
      {
         clearmessagearea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         addstr(activesquad->squad[hacker]->name, gamelog);
         switch(type)
         {
         case HACK_SUPERCOMPUTER:addstr(" has burned a disk of top secret files!", gamelog);break;
         case HACK_VAULT:addstr(" has disabled the second layer of security!", gamelog);break;
         }
         gamelog.newline();
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
         addstr(activesquad->squad[hacker]->name, gamelog);
         switch(type)
         {
         case HACK_SUPERCOMPUTER:addstr(" couldn't bypass the supercomputer security.", gamelog);break;
         case HACK_VAULT:addstr(" couldn't bypass the vault's electronic lock.", gamelog);break;
         }
         gamelog.newline();
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
      addstr("You can't find anyone to do the job.", gamelog);
      gamelog.newline();
      if(blind)
      {
         // Screen readers FTW. Honestly, it should just be a handicap
         //        instead of an impossibility, just make the chances
         //        much, much less.
         //                        -- LK
         refresh();
         getch();
         move(17,1);
         addstr("Including the BLIND HACKER you brought.", gamelog);
         gamelog.newline();
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
      addstr("The Conservatives in the room hurry the Squad, so ", gamelog);
      move(17,1);
      addstr("the broadcast never happens.", gamelog);
      gamelog.newline();

      refresh();
      getch();
      return 0;
   }

   criminalizeparty(LAWFLAG_DISTURBANCE);

   clearmessagearea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   addstr("The Squad takes control of the microphone and ", gamelog);
   move(17,1);
   int viewhit=LCSrandom(VIEWNUM);
   switch(viewhit)
   {
      case VIEW_GAY:                    addstr("discusses homosexual rights.", gamelog);break;
      case VIEW_DEATHPENALTY:           addstr("examines the death penalty.", gamelog);break;
      case VIEW_TAXES:                  addstr("discusses the tax code.", gamelog);break;
      case VIEW_NUCLEARPOWER:           addstr("runs down nuclear power.", gamelog);break;
      case VIEW_ANIMALRESEARCH:         addstr("discusses the horrors of animal research.", gamelog);break;
      case VIEW_POLICEBEHAVIOR:         addstr("goes over cases of police brutality.", gamelog);break;
      case VIEW_TORTURE:                addstr("discusses prisoner abuse and torture.", gamelog);break;
      case VIEW_PRISONS:                addstr("discusses the prison system's flaws.", gamelog);break;
      case VIEW_INTELLIGENCE:           addstr("debates privacy law.", gamelog);break;
      case VIEW_FREESPEECH:             addstr("talks about free speech.", gamelog);break;
      case VIEW_GENETICS:               addstr("discusses the implications of genetic research.", gamelog);break;
      case VIEW_JUSTICES:               addstr("talks about the record of a Conservative judge.", gamelog);break;
      case VIEW_GUNCONTROL:             addstr("talks about gun control.", gamelog);break;
      case VIEW_SWEATSHOPS:             addstr("brings details about sweatshops to light.", gamelog);break;
      case VIEW_POLLUTION:              addstr("does a show on industrial pollution.", gamelog);break;
      case VIEW_CORPORATECULTURE:       addstr("jokes about corporate culture.", gamelog);break;
      case VIEW_CEOSALARY:              addstr("gives examples of CEO excesses.", gamelog);break;
      case VIEW_WOMEN:                  addstr("discusses abortion.", gamelog);break;// "Performs an abortion live on the air."
                                                                            // Get [LAWFLAG_SPEECH], [LAWFLAG_MURDER], [LAWFLAG_DISTURBANCE] -- LK
      case VIEW_CIVILRIGHTS:            addstr("debates affirmative action.", gamelog);break;
      case VIEW_DRUGS:                  addstr("has a frank talk about drugs.", gamelog);break;
      case VIEW_IMMIGRATION:            addstr("examines the issue of immigration.", gamelog);break;
      case VIEW_MILITARY:               addstr("talks about militarism in modern culture.", gamelog);break;
      case VIEW_AMRADIO:                addstr("discusses other AM radio shows.", gamelog);break;
      case VIEW_CABLENEWS:              addstr("talks about Cable News.", gamelog);break;
      case VIEW_LIBERALCRIMESQUAD:      addstr("lets people know about the Liberal Crime Squad.", gamelog);break;
      default:
      case VIEW_LIBERALCRIMESQUADPOS:   addstr("extols the virtues of the Liberal Crime Squad.", gamelog);break;
      case VIEW_CONSERVATIVECRIMESQUAD: addstr("demonizes the Conservative Crime Squad.", gamelog);break;
   }
   gamelog.newline();

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

         segmentpower+=activesquad->squad[p]->get_attribute(ATTRIBUTE_INTELLIGENCE,true);
         segmentpower+=activesquad->squad[p]->get_attribute(ATTRIBUTE_HEART,true);
         segmentpower+=activesquad->squad[p]->get_attribute(ATTRIBUTE_CHARISMA,true);
         segmentpower+=activesquad->squad[p]->get_skill(SKILL_MUSIC);
         segmentpower+=activesquad->squad[p]->get_skill(SKILL_RELIGION);
         segmentpower+=activesquad->squad[p]->get_skill(SKILL_SCIENCE);
         segmentpower+=activesquad->squad[p]->get_skill(SKILL_BUSINESS);
         segmentpower+=activesquad->squad[p]->get_skill(SKILL_PERSUASION);
         activesquad->squad[p]->train(SKILL_PERSUASION,50);
         partysize++;
      }
   }

   // LCS colors enhance the broadcast significantly
   //if(activesquad->stance==SQUADSTANCE_BATTLECOLORS)
   //   segmentpower = (segmentpower * 3) / 2;

   int segmentbonus=segmentpower/4;
   if(partysize>1)segmentpower/=partysize;
   segmentpower+=segmentbonus;

   clearmessagearea();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   if(segmentpower<25)addstr("The Squad sounds wholly insane.", gamelog);
   else if(segmentpower<35)addstr("The show really sucks.", gamelog);
   else if(segmentpower<45)addstr("It is a very boring hour.", gamelog);
   else if(segmentpower<55)addstr("It is mediocre radio.", gamelog);
   else if(segmentpower<70)addstr("The show was all right.", gamelog);
   else if(segmentpower<85)addstr("The Squad put on a good show.", gamelog);
   else if(segmentpower<100)addstr("It was thought-provoking, even humorous.", gamelog);
   else addstr("It was the best hour of AM radio EVER.", gamelog);
   gamelog.newline();

   refresh();
   getch();

   //CHECK PUBLIC OPINION
   change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
   change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,(segmentpower-50)/2);
   if(viewhit!=VIEW_LIBERALCRIMESQUAD)change_public_opinion(viewhit,(segmentpower-50)/2,1);
   else change_public_opinion(viewhit,segmentpower/2);

   //PRISONER PARTS
   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(activesquad->squad[p]->prisoner!=NULL && activesquad->squad[p]->prisoner->alive)
         {
            if(activesquad->squad[p]->prisoner->type==CREATURE_RADIOPERSONALITY)
            {
               viewhit=LCSrandom(VIEWNUM);
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("The hostage ", gamelog);
               addstr(activesquad->squad[p]->prisoner->name, gamelog);
               addstr(" is forced on to ", gamelog);
               move(17,1);
               switch(viewhit)
               {
                  case VIEW_GAY:                    addstr("discuss homosexual rights.", gamelog);break;
                  case VIEW_DEATHPENALTY:           addstr("examine the death penalty.", gamelog);break;
                  case VIEW_TAXES:                  addstr("discuss the tax code.", gamelog);break;
                  case VIEW_NUCLEARPOWER:           addstr("run down nuclear power.", gamelog);break;
                  case VIEW_ANIMALRESEARCH:         addstr("discuss the horrors of animal research.", gamelog);break;
                  case VIEW_POLICEBEHAVIOR:         addstr("go over cases of police brutality.", gamelog);break;
                  case VIEW_TORTURE:                addstr("discuss prisoner abuse and torture.", gamelog);break;
                  case VIEW_PRISONS:                addstr("discuss the prison system's flaws.", gamelog);break;
                  case VIEW_INTELLIGENCE:           addstr("debate privacy law.", gamelog);break;
                  case VIEW_FREESPEECH:             addstr("talk about free speech.", gamelog);break;
                  case VIEW_GENETICS:               addstr("discuss the implications of genetic research.", gamelog);break;
                  case VIEW_JUSTICES:               addstr("talk about the record of a Conservative judge.", gamelog);break;
                  case VIEW_GUNCONTROL:             addstr("talk about gun control.", gamelog);break;
                  case VIEW_SWEATSHOPS:             addstr("bring details about sweatshops to light.", gamelog);break;
                  case VIEW_POLLUTION:              addstr("do a show on industrial pollution.", gamelog);break;
                  case VIEW_CORPORATECULTURE:       addstr("joke about corporate culture.", gamelog);break;
                  case VIEW_CEOSALARY:              addstr("give examples of CEO excesses.", gamelog);break;
                  case VIEW_WOMEN:                  addstr("discuss abortion.", gamelog);break;// "Performs an abortion live on the air."
                                                                                      // Get [LAWFLAG_SPEECH], [LAWFLAG_MURDER], [LAWFLAG_DISTURBANCE] -- LK
                  case VIEW_CIVILRIGHTS:            addstr("debate affirmative action.", gamelog);break;
                  case VIEW_DRUGS:                  addstr("have a frank talk about drugs.", gamelog);break;
                  case VIEW_IMMIGRATION:            addstr("examine the issue of immigration.", gamelog);break;
                  case VIEW_MILITARY:               addstr("talk about militarism in modern culture.", gamelog);break;
                  case VIEW_AMRADIO:                addstr("discuss other AM radio shows.", gamelog);break;
                  case VIEW_CABLENEWS:              addstr("talk about Cable News.", gamelog);break;
                  case VIEW_LIBERALCRIMESQUAD:      addstr("let people know about the Liberal Crime Squad.", gamelog);break;
                  default:
                  case VIEW_LIBERALCRIMESQUADPOS:   addstr("extol the virtues of the Liberal Crime Squad.", gamelog);break;
                  case VIEW_CONSERVATIVECRIMESQUAD: addstr("demonize the Conservative Crime Squad.", gamelog);break;
               }
               gamelog.newline();

               usegmentpower=10; //FAME BONUS
               usegmentpower+=activesquad->squad[p]->prisoner->get_attribute(ATTRIBUTE_INTELLIGENCE,true);
               usegmentpower+=activesquad->squad[p]->prisoner->get_attribute(ATTRIBUTE_HEART,true);
               usegmentpower+=activesquad->squad[p]->prisoner->get_attribute(ATTRIBUTE_CHARISMA,true);
               usegmentpower+=activesquad->squad[p]->prisoner->get_skill(SKILL_PERSUASION);

               if(viewhit!=VIEW_LIBERALCRIMESQUAD)change_public_opinion(viewhit,(usegmentpower-10)/2,1,80);
               else change_public_opinion(viewhit,usegmentpower/2);

               segmentpower+=usegmentpower;

               refresh();
               getch();
            }
            else
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr(activesquad->squad[p]->prisoner->name, gamelog);
               addstr(", the hostage, is kept off-air.", gamelog);
               gamelog.newline();

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
      addstr("Moderates at the station appreciated the show.", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("They no longer feel alienated.", gamelog);
      gamelog.newline();

      refresh();
      getch();
   }

   //POST-SECURITY BLITZ IF IT SUCKED
   if(segmentpower<90)
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("Security is waiting for the Squad ", gamelog);
      move(17,1);
      addstr("after the show!", gamelog);
      gamelog.newline();

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
      addstr("The show was so good that security listened to it ", gamelog);
      move(17,1);
      addstr("at their desks.  The Squad might yet escape.", gamelog);
      gamelog.newline();

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
      addstr("The Conservatives in the room hurry the Squad, so ", gamelog);
      move(17,1);
      addstr("the broadcast never happens.", gamelog);
      gamelog.newline();

      refresh();
      getch();
      return 0;
   }

   criminalizeparty(LAWFLAG_DISTURBANCE);

   clearmessagearea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   addstr("The Squad steps in front of the cameras and ", gamelog);
   move(17,1);
   int viewhit=LCSrandom(VIEWNUM);
   switch(viewhit)
   {
      case VIEW_GAY:                    addstr("discusses homosexual rights.", gamelog);break;
      case VIEW_DEATHPENALTY:           addstr("examines the death penalty.", gamelog);break;
      case VIEW_TAXES:                  addstr("discusses the tax code.", gamelog);break;
      case VIEW_NUCLEARPOWER:           addstr("runs down nuclear power.", gamelog);break;
      case VIEW_ANIMALRESEARCH:         addstr("discusses the horrors of animal research.", gamelog);break;
      case VIEW_POLICEBEHAVIOR:         addstr("goes over cases of police brutality.", gamelog);break;
      case VIEW_TORTURE:                addstr("discusses prisoner abuse and torture.", gamelog);break;
      case VIEW_PRISONS:                addstr("discusses the prison system's flaws.", gamelog);break;
      case VIEW_INTELLIGENCE:           addstr("debates privacy law.", gamelog);break;
      case VIEW_FREESPEECH:             addstr("talks about free speech.", gamelog);break;
      case VIEW_GENETICS:               addstr("discusses the implications of genetic research.", gamelog);break;
      case VIEW_JUSTICES:               addstr("talks about the record of a Conservative judge.", gamelog);break;
      case VIEW_GUNCONTROL:             addstr("talks about gun control.", gamelog);break;
      case VIEW_SWEATSHOPS:             addstr("brings details about sweatshops to light.", gamelog);break;
      case VIEW_POLLUTION:              addstr("does a show on industrial pollution.", gamelog);break;
      case VIEW_CORPORATECULTURE:       addstr("jokes about corporate culture.", gamelog);break;
      case VIEW_CEOSALARY:              addstr("gives examples of CEO excesses.", gamelog);break;
      case VIEW_WOMEN:                  addstr("discusses abortion.", gamelog);break;// "Performs an abortion live on the air."
                                                                            // Get [LAWFLAG_SPEECH], [LAWFLAG_MURDER], [LAWFLAG_DISTURBANCE] -- LK
      case VIEW_CIVILRIGHTS:            addstr("debates affirmative action.", gamelog);break;
      case VIEW_DRUGS:                  addstr("has a frank talk about drugs.", gamelog);break;
      case VIEW_IMMIGRATION:            addstr("examines the issue of immigration.", gamelog);break;
      case VIEW_MILITARY:               addstr("talks about militarism in modern culture.", gamelog);break;
      case VIEW_AMRADIO:                addstr("discusses other AM radio shows.", gamelog);break;
      case VIEW_CABLENEWS:              addstr("talks about Cable News.", gamelog);break;
      case VIEW_LIBERALCRIMESQUAD:      addstr("lets people know about the Liberal Crime Squad.", gamelog);break;
      default:
      case VIEW_LIBERALCRIMESQUADPOS:   addstr("extols the virtues of the Liberal Crime Squad.", gamelog);break;
      case VIEW_CONSERVATIVECRIMESQUAD: addstr("demonizes the Conservative Crime Squad.", gamelog);break;
   }
   gamelog.newline();

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

         segmentpower+=activesquad->squad[p]->get_attribute(ATTRIBUTE_INTELLIGENCE,true);
         segmentpower+=activesquad->squad[p]->get_attribute(ATTRIBUTE_HEART,true);
         segmentpower+=activesquad->squad[p]->get_attribute(ATTRIBUTE_CHARISMA,true);
         segmentpower+=activesquad->squad[p]->get_skill(SKILL_MUSIC);
         segmentpower+=activesquad->squad[p]->get_skill(SKILL_RELIGION);
         segmentpower+=activesquad->squad[p]->get_skill(SKILL_SCIENCE);
         segmentpower+=activesquad->squad[p]->get_skill(SKILL_BUSINESS);
         segmentpower+=activesquad->squad[p]->get_skill(SKILL_PERSUASION);
         activesquad->squad[p]->train(SKILL_PERSUASION,50);
         partysize++;
      }
   }

   // LCS colors enhance the broadcast significantly
   if(activesquad->stance==SQUADSTANCE_BATTLECOLORS)
      segmentpower = (segmentpower * 3) / 2;

   int segmentbonus=segmentpower/4;
   if(partysize>1)segmentpower/=partysize;
   segmentpower+=segmentbonus;

   clearmessagearea();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   if(segmentpower<25)addstr("The Squad looks completely insane.", gamelog);
   else if(segmentpower<35)addstr("The show really sucks.", gamelog);
   else if(segmentpower<45)addstr("It is a very boring hour.", gamelog);
   else if(segmentpower<55)addstr("It is mediocre TV.", gamelog);
   else if(segmentpower<70)addstr("The show was all right.", gamelog);
   else if(segmentpower<85)addstr("The Squad put on a good show.", gamelog);
   else if(segmentpower<100)addstr("It was thought-provoking, even humorous.", gamelog);
   else addstr("It was the best hour of Cable TV EVER.", gamelog);
   gamelog.newline();

   refresh();
   getch();

   //CHECK PUBLIC OPINION
   change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
   change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,(segmentpower-50)/10);
   if(viewhit!=VIEW_LIBERALCRIMESQUAD)change_public_opinion(viewhit,(segmentpower-50)/5,1);
   else change_public_opinion(viewhit,segmentpower/10);

   //PRISONER PARTS
   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(activesquad->squad[p]->prisoner!=NULL && activesquad->squad[p]->prisoner->alive)
         {
            if(activesquad->squad[p]->prisoner->type==CREATURE_NEWSANCHOR)
            {
               viewhit=LCSrandom(VIEWNUM);
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("The hostage ", gamelog);
               addstr(activesquad->squad[p]->prisoner->name, gamelog);
               addstr(" is forced on to ", gamelog);
               move(17,1);
               switch(viewhit)
               {
                  case VIEW_GAY:                    addstr("discuss homosexual rights.", gamelog);break;
                  case VIEW_DEATHPENALTY:           addstr("examine the death penalty.", gamelog);break;
                  case VIEW_TAXES:                  addstr("discuss the tax code.", gamelog);break;
                  case VIEW_NUCLEARPOWER:           addstr("run down nuclear power.", gamelog);break;
                  case VIEW_ANIMALRESEARCH:         addstr("discuss the horrors of animal research.", gamelog);break;
                  case VIEW_POLICEBEHAVIOR:         addstr("go over cases of police brutality.", gamelog);break;
                  case VIEW_TORTURE:                addstr("discuss prisoner abuse and torture.", gamelog);break;
		  case VIEW_PRISONS:                addstr("discusses the prison system's flaws.", gamelog);break;
                  case VIEW_INTELLIGENCE:           addstr("debate privacy law.", gamelog);break;
                  case VIEW_FREESPEECH:             addstr("talk about free speech.", gamelog);break;
                  case VIEW_GENETICS:               addstr("discuss the implications of genetic research.", gamelog);break;
                  case VIEW_JUSTICES:               addstr("talk about the record of a Conservative judge.", gamelog);break;
                  case VIEW_GUNCONTROL:             addstr("talk about gun control.", gamelog);break;
                  case VIEW_SWEATSHOPS:             addstr("bring details about sweatshops to light.", gamelog);break;
                  case VIEW_POLLUTION:              addstr("do a show on industrial pollution.", gamelog);break;
                  case VIEW_CORPORATECULTURE:       addstr("joke about corporate culture.", gamelog);break;
                  case VIEW_CEOSALARY:              addstr("give examples of CEO excesses.", gamelog);break;
                  case VIEW_WOMEN:                  addstr("discuss abortion.", gamelog);break;// "Performs an abortion live on the air."
                                                                                      // Get [LAWFLAG_SPEECH], [LAWFLAG_MURDER], [LAWFLAG_DISTURBANCE] -- LK
                  case VIEW_CIVILRIGHTS:            addstr("debate affirmative action.", gamelog);break;
                  case VIEW_DRUGS:                  addstr("have a frank talk about drugs.", gamelog);break;
                  case VIEW_IMMIGRATION:            addstr("examine the issue of immigration.", gamelog);break;
                  case VIEW_MILITARY:               addstr("talk about militarism in modern culture.", gamelog);break;
                  case VIEW_AMRADIO:                addstr("discuss other AM radio shows.", gamelog);break;
                  case VIEW_CABLENEWS:              addstr("talk about Cable News.", gamelog);break;
                  case VIEW_LIBERALCRIMESQUAD:      addstr("let people know about the Liberal Crime Squad.", gamelog);break;
                  default:
                  case VIEW_LIBERALCRIMESQUADPOS:   addstr("extol the virtues of the Liberal Crime Squad.", gamelog);break;
                  case VIEW_CONSERVATIVECRIMESQUAD: addstr("demonize the Conservative Crime Squad.", gamelog);break;
               }
               gamelog.newline();

               usegmentpower=10; //FAME BONUS
               usegmentpower+=activesquad->squad[p]->prisoner->get_attribute(ATTRIBUTE_INTELLIGENCE,true);
               usegmentpower+=activesquad->squad[p]->prisoner->get_attribute(ATTRIBUTE_HEART,true);
               usegmentpower+=activesquad->squad[p]->prisoner->get_attribute(ATTRIBUTE_CHARISMA,true);
               usegmentpower+=activesquad->squad[p]->prisoner->get_skill(SKILL_PERSUASION);

               if(viewhit!=VIEW_LIBERALCRIMESQUAD)change_public_opinion(viewhit,(usegmentpower-10)/2);
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
               addstr(activesquad->squad[p]->prisoner->name, gamelog);
               addstr(", the hostage, is kept off-air.", gamelog);
               gamelog.newline();

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
      addstr("Moderates at the station appreciated the show.", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("They no longer feel alienated.", gamelog);
      gamelog.newline();

      refresh();
      getch();
   }

   //POST - SECURITY BLITZ IF IT SUCKED
   if(segmentpower<85 && segmentpower>=25)
   {
      clearmessagearea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr("Security is waiting for the Squad ", gamelog);
      move(17,1);
      addstr("after the show!", gamelog);
      gamelog.newline();

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
      addstr("The show was so ", gamelog);
      if(segmentpower<50)
         addstr("hilarious", gamelog);
      else
         addstr("entertaining", gamelog);
      addstr(" that security watched it ", gamelog);
      move(17,1);
      addstr("at their desks.  The Squad might yet escape.", gamelog);
      gamelog.newline();

      refresh();
      getch();
   }

   return 1;
}



/* rescues people held at the activeparty's current location */
void partyrescue(short special)
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
   
   vector<Creature*> waiting_for_rescue;
   for(pl=0;pl<pool.size();pl++)
   {
      if(pool[pl]->location==cursite&&
         !(pool[pl]->flag & CREATUREFLAG_SLEEPER)&&
         !(special==SPECIAL_PRISON_CONTROL_LOW&&!(pool[pl]->sentence>0&&!pool[pl]->deathpenalty))&& //Low is for normal time-limited sentences.
         !(special==SPECIAL_PRISON_CONTROL_MEDIUM&&!(pool[pl]->sentence<0&&!pool[pl]->deathpenalty))&& //Medium is for life sentences.
         !(special==SPECIAL_PRISON_CONTROL_HIGH&&!pool[pl]->deathpenalty)) //High is for death sentences.
            waiting_for_rescue.push_back(pool[pl]);
   }

   for(pl=0;pl<waiting_for_rescue.size();pl++)
   {
      if(LCSrandom(2)&&freeslots)
      {
         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]==NULL)
            {
               activesquad->squad[p]=waiting_for_rescue[pl];
               activesquad->squad[p]->squadid=activesquad->id;
               criminalize(*activesquad->squad[p],LAWFLAG_ESCAPED);
               activesquad->squad[p]->flag|=CREATUREFLAG_JUSTESCAPED;
               break;
            }
         }
         hostslots++;
         freeslots--;

         clearmessagearea();

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         addstr("You've rescued ", gamelog);
         addstr(waiting_for_rescue[pl]->name, gamelog);
         addstr(" from the Conservatives.", gamelog);
         gamelog.newline();

         printparty();
         refresh();
         getch();

         waiting_for_rescue[pl]->location=-1;
         waiting_for_rescue[pl]->base=activesquad->squad[0]->base;
         
         waiting_for_rescue.erase(waiting_for_rescue.begin()+pl);
         --pl;
      }
   }
   for(pl=0;pl<waiting_for_rescue.size();pl++)
   {
      if(hostslots)
      {
         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL)
            {
               if(activesquad->squad[p]->alive&&activesquad->squad[p]->prisoner==NULL)
               {
                  activesquad->squad[p]->prisoner=waiting_for_rescue[pl];
                  waiting_for_rescue[pl]->squadid=activesquad->id;
                  criminalize(*waiting_for_rescue[pl],LAWFLAG_ESCAPED);
                  waiting_for_rescue[pl]->flag|=CREATUREFLAG_JUSTESCAPED;

                  clearmessagearea();

                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(16,1);
                  addstr("You've rescued ", gamelog);
                  addstr(waiting_for_rescue[pl]->name, gamelog);
                  addstr(" from the Conservatives.", gamelog);
                  gamelog.newline();

                  refresh();
                  getch();

                  clearmessagearea();
                  move(16,1);
                  addstr(waiting_for_rescue[pl]->name, gamelog);
                  addstr(" ", gamelog);
                  switch(LCSrandom(3))
                  {
                     case 0:addstr("was tortured recently ", gamelog);break;
                     case 1:addstr("was beaten severely yesterday ", gamelog);break;
                     case 2:addstr("was on a hunger strike ", gamelog);break;
                  }
                  move(17,1);
                  addstr("so ", gamelog);
                  addstr(activesquad->squad[p]->name, gamelog);
                  addstr(" will have to haul a Liberal.", gamelog);
                  gamelog.newline();

                  waiting_for_rescue[pl]->location=-1;
                  waiting_for_rescue[pl]->base=activesquad->squad[p]->base;

                  printparty();
                  refresh();
                  getch();
                  
                  waiting_for_rescue.erase(waiting_for_rescue.begin()+pl);
                  --pl;
                  break;
               }
            }
         }

         hostslots--;
      }
      if(!hostslots)break;
   }

   if(waiting_for_rescue.size()==1)
   {
      clearmessagearea();
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      move(16,1);
      addstr("There's nobody left to carry ", gamelog);
      addstr(waiting_for_rescue[0]->name, gamelog);
      addstr(".", gamelog);
      gamelog.newline();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(17,1);
      addstr("You'll have to come back later.", gamelog);
      gamelog.newline();
      refresh();
      getch();
   }
   else if(waiting_for_rescue.size()>1)
   {
      clearmessagearea();
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      move(16,1);
      addstr("There's nobody left to carry the others.", gamelog);
      gamelog.newline();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(17,1);
      addstr("You'll have to come back later.", gamelog);
      gamelog.newline();
      refresh();
      getch();
   }
}



/* everybody reload! */
void reloadparty(bool wasteful)
{
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;
      if(!activesquad->squad[p]->alive)continue;

      if(activesquad->squad[p]->has_thrown_weapon)
      {
         activesquad->squad[p]->ready_another_throwing_weapon();
      }
      else if(activesquad->squad[p]->can_reload())
      {
         activesquad->squad[p]->reload(wasteful);
      }
   }
}
