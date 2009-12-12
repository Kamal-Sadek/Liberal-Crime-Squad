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

//#include <includes.h>
#include <externs.h>


/* recruit struct constructor */
recruitst::recruitst() : task(0), timeleft(0), level(0), eagerness1(0)
{
   //Has heard of the LCS
   if((int)LCSrandom(100)<attitude[VIEW_LIBERALCRIMESQUAD])
   {
      //Likes the LCS
      if((int)LCSrandom(100)<attitude[VIEW_LIBERALCRIMESQUADPOS])
      {
         eagerness1=1;
      }
      //Doesn't like the LCS
      else eagerness1=-1;
   }
   
}

char recruitst::eagerness()
{
   char eagerness_temp = eagerness1 + pool[getpoolcreature(recruiter_id)]->skillval(SKILL_LEADERSHIP); 
   if(recruit->talkreceptive())
   {
      eagerness_temp+=2;
   }
   if(recruit->attval(ATTRIBUTE_HEART)>recruit->attval(ATTRIBUTE_WISDOM))
   {
      eagerness_temp++;
   }
   if(recruit->attval(ATTRIBUTE_HEART) > 9)
   {
      eagerness_temp++;
   }
   if(recruit->weapon.ranged())
   {
      eagerness_temp++;
   }
   //Moderates are decidedly less interested
   if(recruit->align==0)eagerness_temp-=2;
   //Conservatives are extremely uninterested
   if(recruit->align==-1)eagerness_temp-=4;
   return eagerness_temp;
}


/* daily - recruit - recruit completes task */
char completerecruittask(recruitst &r,int p,char &clearformess)
{
   if(clearformess)erase();
   else
   {
      makedelimiter(8,0);
   }

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(8,1);
   if(r.task==TASK_ARRESTED)
   {
      addstr(r.recruit->name);
      addstr(" was arrested for ");
      switch(LCSrandom(4))
      {
      case 0:addstr("vandalism.");break;
      case 1:addstr("assault.");break;
      case 2:addstr("burglary.");break;
      case 3:addstr("disturbing the peace.");break;
      }
      refresh();
      getch();
      delete r.recruit;
      return 1;
   }
   addstr(r.recruit->name);
   addstr(" has ");

   switch(r.task)
   {
   case TASK_BUYWEAPON:
      addstr("acquired a weapon.");

      //Select a weapon to arm the recruit with
      if(r.recruit->money>1500 &&
         r.recruit->skillval(SKILL_RIFLE) &&
         (law[LAW_GUNCONTROL]==-2 || r.recruit->skillval(SKILL_STREETSENSE)))
      {
         switch(LCSrandom(3))
         {
         case 0:r.recruit->weapon.type=WEAPON_CARBINE_M4;break;
         case 1:r.recruit->weapon.type=WEAPON_AUTORIFLE_M16;break;
         case 2:r.recruit->weapon.type=WEAPON_AUTORIFLE_AK47;break;
         }
         r.recruit->weapon.ammo=30;
         r.recruit->clip[CLIP_ASSAULT]=4;
      }
      else if(r.recruit->money>1200 &&
         r.recruit->skillval(SKILL_RIFLE) &&
         (law[LAW_GUNCONTROL]==-2 || r.recruit->skillval(SKILL_STREETSENSE)))
      {
         r.recruit->weapon.type=WEAPON_SMG_MP5;
         r.recruit->weapon.ammo=15;
         r.recruit->clip[CLIP_SMG]=4;
      }
      else if(r.recruit->money>400 &&
         r.recruit->skillval(SKILL_SHOTGUN) &&
         (law[LAW_GUNCONTROL]<2 || r.recruit->skillval(SKILL_STREETSENSE)))
      {
         r.recruit->weapon.type=WEAPON_SEMIRIFLE_AR15;
         r.recruit->weapon.ammo=30;
         r.recruit->clip[CLIP_ASSAULT]=4;
      }
      else if(r.recruit->money>350 &&
         r.recruit->skillval(SKILL_RIFLE) &&
         (law[LAW_GUNCONTROL]<=-1 || r.recruit->skillval(SKILL_STREETSENSE)))
      {
         r.recruit->weapon.type=WEAPON_SEMIRIFLE_AR15;
         r.recruit->weapon.ammo=30;
         r.recruit->clip[CLIP_ASSAULT]=4;
      }
      else if(r.recruit->money>300 &&
         r.recruit->skillval(SKILL_PISTOL) &&
         (law[LAW_GUNCONTROL]<1 || r.recruit->skillval(SKILL_STREETSENSE)))
      {
         switch(LCSrandom(4))
         {
         case 0:r.recruit->weapon.type=WEAPON_SEMIPISTOL_9MM;
                r.recruit->weapon.ammo=15;
                r.recruit->clip[CLIP_9]=4;
                break;
         case 1:r.recruit->weapon.type=WEAPON_SEMIPISTOL_45;
                r.recruit->weapon.ammo=15;
                r.recruit->clip[CLIP_45]=4;
                break;
         case 2:r.recruit->weapon.type=WEAPON_REVOLVER_44;
                r.recruit->weapon.ammo=6;
                r.recruit->clip[CLIP_44]=4;
                break;
         }
      }
      else if(r.recruit->skillval(SKILL_SWORD))
      {
         if(LCSrandom(5)) r.recruit->weapon.type=WEAPON_SWORD;
         else r.recruit->weapon.type=WEAPON_DAISHO;
      }
      else if(r.recruit->skillval(SKILL_AXE))
      {
         r.recruit->weapon.type=WEAPON_AXE;
      }
      else if(r.recruit->skillval(SKILL_PISTOL))
      {
         r.recruit->weapon.type=WEAPON_REVOLVER_38;
         r.recruit->weapon.ammo=6;
         r.recruit->clip[CLIP_38]=4;
      }
      else if(r.recruit->skillval(SKILL_CLUB) && !r.recruit->skillval(SKILL_KNIFE))
      {
         if(LCSrandom(2))r.recruit->weapon.type=WEAPON_BASEBALLBAT;
         else r.recruit->weapon.type=WEAPON_CROWBAR;
      }
      else if(LCSrandom(2))r.recruit->weapon.type=WEAPON_KNIFE;
      else r.recruit->weapon.type=WEAPON_SHANK;
      break;

   case TASK_COMMUNITYSERVICE:
      addstr("finished doing community service.");
      r.level++;
      break;

   case TASK_ACTIVISM:
      addstr("finished causing trouble.");
      r.level++;
      break;
   case TASK_CRIMES:
      addstr("finished committing crimes.");
      r.level++;
      break;
   default:
      addstr("finished REPORTING THIS BUG TO THE PROGRAMMERS.");
      break;
   }

   refresh();
   getch();
   return 0;
}


static void getissueeventstring(char* str)
{
   switch(LCSrandom(VIEWNUM-3))
   {
   case VIEW_DRUGS:strcat(str,"a collection of studies on the health effects of marijuana");break;
   case VIEW_MILITARY:strcat(str,"a book on the history of military atrocities");break;
   case VIEW_IMMIGRATION:strcat(str,"a reality TV episode on the lives of immigrants");break;
   case VIEW_CIVILRIGHTS:strcat(str,"a documentary on the civil rights struggle");break;
   case VIEW_WOMEN:strcat(str,"a documentary on the women's rights struggle");break;
   case VIEW_GAY:strcat(str,"a documentary on the gay rights struggle");break;
   case VIEW_DEATHPENALTY:strcat(str,"a research paper on abuses of the death penalty");break;
   case VIEW_TAXES:strcat(str,"an economic paper on the flaws of trickle-down");break;
   case VIEW_NUCLEARPOWER:strcat(str,"a video tour of the Chernobyl dead zone");break;
   case VIEW_ANIMALRESEARCH:strcat(str,"a documentary on animal research");break;
   case VIEW_POLICEBEHAVIOR:strcat(str,"a hand-recorded video of police brutality");break;
   case VIEW_TORTURE:strcat(str,"a government inquiry into military interrogations");break;
   case VIEW_INTELLIGENCE:strcat(str,"a documentary on privacy rights");break;
   case VIEW_FREESPEECH:strcat(str,"a collection of banned books");break;
   case VIEW_GENETICS:strcat(str,"a video about genetic engineering accidents");break;
   case VIEW_JUSTICES:strcat(str,"a Liberal policy paper inquiring into judicial decisions");break;
   case VIEW_GUNCONTROL:strcat(str,"a book profiling school shootings");break;
   case VIEW_SWEATSHOPS:strcat(str,"a hand-recorded video of unregulated sweatshops");break;
   case VIEW_POLLUTION:strcat(str,"a leaked government paper on environmental conditions");break;
   case VIEW_CORPORATECULTURE:strcat(str,"a documentary on life under corporate culture");break;
   case VIEW_CEOSALARY:strcat(str,"a Liberal think-tank survey of top CEO salaries");break;
   case VIEW_AMRADIO:strcat(str,"a collection of Conservative radio host rants");break;
   case VIEW_CABLENEWS:strcat(str,"a collection of leaked Conservative cable news memos");break;
   //case VIEW_POLITICALVIOLENCE:strcat(str,"a documentary about progress made by direct action");break;
   }
}


/* daily - recruit - recruit meeting */
char completerecruitmeeting(recruitst &r,int p,char &clearformess)
{
   clearformess=1;

   

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   if(pool[p]->meetings++>5 && LCSrandom(pool[p]->meetings-5))
   {
      addstr(pool[p]->name);
      addstr(" accidentally missed the meeting with ");
      addstr(r.recruit->name);
      move(1,0);
      addstr("due to multiple booking of recruitment sessions.");

      move(3,0);
      addstr("Get it together, ");
      addstr(pool[p]->name);
      addstr("!");
      getch();

      return 1;
   }
   addstr("Meeting with ");
   addstr(r.recruit->name);
   addstr(", ");
   char str[75];
   getrecruitcreature(str,r.recruit->type);
   addstr(str);

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   printfunds(0,1,"Money: ");

   printcreatureinfo(r.recruit,r.level);
   makedelimiter(8,0);

   move(10,0);
   addstr(r.recruit->name);
   switch(r.eagerness())
   {
   case 0:
   case 1:
   case 2:
   case 3:
      addstr(" has someplace better to be, really.");
      break;
   case 4:
   case 5:
      addstr(" is curious what this is all about.");
      break;
   case 6:
   case 7:
      addstr(" is determined to do something.");
      break;
   default:
      if(r.eagerness()>=8)
         addstr(" is ready to fight for the Liberal Cause.");
      else
      {
         addstr(" is obviously humoring ");
         addstr(pool[p]->name);
         addch('.');
      }
      break;
   }
   move(11,0);
   addstr("How should ");
   addstr(pool[p]->name);
   addstr(" approach the situation?");

   move(13,0);
   if(funds<50)set_color(COLOR_BLACK,COLOR_BLACK,1);
   addstr("A - Spend $50 on props and a book for them to keep afterward.");
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(14,0);
   addstr("B - Just casually chat with them and discuss politics.");
   
   move(15,0);
   if(subordinatesleft(*pool[p]))
   {
      addstr("C - Offer to let ");
      addstr(r.recruit->name);
      addstr(" to join the LCS as a full member.");
   }
   else
   {
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr("C - ");
      addstr(pool[p]->name);
      addstr(" needs more Juice or Leadership to recruit.");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
   }
   move(16,0);
   addstr("D - Break off the meetings.");

   int y=18;
   

   do
   {
      refresh();
      int c=getch();
      translategetch(c);

      if(c=='c' && subordinatesleft(*pool[p]))
      {
         move(y,0);
         addstr(pool[p]->name);
         addstr(" offers to let ");
         addstr(r.recruit->name);
         addstr(" join the Liberal Crime Squad.");

         refresh();
         getch();

         if(r.eagerness()>=8)
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            move(y+=2,0);
            
            addstr(r.recruit->name);
            addstr(" accepts, and is eager to get started.");

            liberalize(*r.recruit,false);

            refresh();
            getch();

            erase();
            sleeperize_prompt(*r.recruit,*pool[p],6);

            r.recruit->hireid=pool[p]->id;

            pool[p]->train(SKILL_LEADERSHIP,5);
            pool[p]->train(SKILL_PERSUASION,5);

            pool.push_back(r.recruit);
            stat_recruits++;

            return 1;
         }
         else
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,0);
            move(y+2,0);
            addstr(r.recruit->name);
            addstr(" isn't crazy! ");
            addstr("Thanks, but no thanks.");

            refresh();
            getch();

            delete r.recruit;
            return 1;
         }
      }
      if(c=='b' || (c=='a' && funds>=50))
      {
         if(c=='a')
         {
            funds -= 50;
            moneylost_dating += 50;
         }
         //SAV - You can get your skill up to a 3 by chatting. Past that,
         // you must successfully recruit people. Training is slower the
         // better you are.
         //JDS - Increased max skill to get to 5 under this system, gave
         // minimum of 1 exp for the action.
         pool[p]->train(SKILL_PERSUASION,
            max(5-pool[p]->skillval(SKILL_PERSUASION),1));
         pool[p]->train(SKILL_SCIENCE,
            max(r.recruit->skillval(SKILL_SCIENCE)-pool[p]->skillval(SKILL_SCIENCE),0));
         pool[p]->train(SKILL_RELIGION,
            max(r.recruit->skillval(SKILL_RELIGION)-pool[p]->skillval(SKILL_RELIGION),0));
         pool[p]->train(SKILL_LAW,
            max(r.recruit->skillval(SKILL_LAW)-pool[p]->skillval(SKILL_LAW),0));
         pool[p]->train(SKILL_BUSINESS,
            max(r.recruit->skillval(SKILL_BUSINESS)-pool[p]->skillval(SKILL_BUSINESS),0));
         
         int lib_persuasiveness = LCSrandom(pool[p]->skillval(SKILL_PERSUASION)*2+
                                  pool[p]->skillval(SKILL_BUSINESS)+
                                  pool[p]->skillval(SKILL_SCIENCE)+
                                  pool[p]->skillval(SKILL_RELIGION)+
                                  pool[p]->skillval(SKILL_LAW)+1)+
                                  LCSrandom(pool[p]->attval(ATTRIBUTE_HEART)+
                                  pool[p]->attval(ATTRIBUTE_CHARISMA)*2+
                                  pool[p]->attval(ATTRIBUTE_INTELLIGENCE));

         int recruit_reluctance = LCSrandom(r.recruit->skillval(SKILL_BUSINESS)+
                                  r.recruit->skillval(SKILL_SCIENCE)+
                                  r.recruit->skillval(SKILL_RELIGION)+
                                  r.recruit->skillval(SKILL_LAW))+
                                  LCSrandom(r.recruit->attval(ATTRIBUTE_WISDOM)*3+
                                  r.recruit->attval(ATTRIBUTE_INTELLIGENCE));

         int max_eagerness      = pool[p]->attval(ATTRIBUTE_HEART)-
                                  r.recruit->attval(ATTRIBUTE_WISDOM);

         if(c=='a')
         {
            lib_persuasiveness+=LCSrandom(20);
            max_eagerness     +=LCSrandom(20);

            move(y++,0);
            addstr(pool[p]->name);
            addstr(" shares ");
            strcpy(str,"");
            getissueeventstring(str);
            addstr(str);
            addstr(".");
            
            refresh();
            getch();
         }
         else
         {
            move(y++,0);
            addstr(pool[p]->name);
            addstr(" explains ");
            if(pool[p]->gender_liberal==GENDER_MALE||
               (pool[p]->gender_liberal==GENDER_NEUTRAL&&pool[p]->gender_conservative==GENDER_MALE))
               addstr("his ");
            else
               addstr("her ");
            addstr("views on ");
            getviewsmall(str,LCSrandom(VIEWNUM-3));
            addstr(str);
            addstr(".");
            refresh();
            getch();
         }


         if((lib_persuasiveness         > recruit_reluctance) &&
            (max_eagerness+(int)LCSrandom(5) > r.eagerness()     ))
         {
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            r.level++;
            r.eagerness1++;
            move(y++,0);
            addstr(r.recruit->name);
            addstr(" found ");
            addstr(pool[p]->name);
            addstr("'s views to be insightful.");
            move(y++,0);
            addstr("They'll definitely meet again tomorrow.");
         }
         else if((lib_persuasiveness > recruit_reluctance))
         {
            r.level++;
            move(y++,0);
            addstr(r.recruit->name);
            addstr(" enjoyed talking with ");
            addstr(pool[p]->name);
            addstr(".");
            move(y++,0);
            addstr("They'll meet again tomorrow.");
         }
         else
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            move(y++,0);
            addstr(pool[p]->name);
            addstr(" comes off as slightly insane.");
            move(y++,0);
            addstr("This whole thing was a mistake. There won't be another meeting.");
            refresh();
            getch();
            return 1;
         }
         refresh();
         getch();
         return 0;
      }
      if(c=='d')
      {
         return 1;
      }
      // Donations below -- removed because it's a grindfest
      // - Jonathan S. Fox
      /*if(c=='c')
      {
         if(r.eagerness()>5 && r.recruit->money)
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            int donationamount=0;
            move(y++,0);
            addstr(r.recruit->name);
            addstr(" would be happy to help.");
            
            donationamount=r.recruit->money;

            if(r.eagerness()>7)
               donationamount=static_cast<int>((1.0+LCSrandom(6)/10.0)*donationamount);
            if(r.recruit->attval(ATTRIBUTE_HEART)>8)
               donationamount=static_cast<int>((1.0+LCSrandom(6)/10.0)*donationamount);
            if(r.recruit->attval(ATTRIBUTE_WISDOM)>8)
               donationamount=static_cast<int>((1.0-LCSrandom(6)/10.0)*donationamount);
            if(r.recruit->align==-1)
               donationamount=static_cast<int>((0.5-LCSrandom(4)/10.0)*donationamount);
            refresh();
            getch();
            move(y++,0);
            addstr("Here's $");
            if(donationamount<=0)donationamount=1;
            itoa(donationamount,str,10);
            addstr(str);
            addstr(" for the cause.");
            funds+=donationamount;
            moneygained_donate+=donationamount;
         }
         else
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            move(y++,0);
            addstr("Sorry, ");
            addstr(r.recruit->name);
            addstr(" just doesn't have that kind of money lying around.");
         }
         refresh();
         getch();
         return 1;
      }*/
   }while(1);
}
