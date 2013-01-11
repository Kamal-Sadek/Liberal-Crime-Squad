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
         eagerness1=3;
      }
      //Doesn't like the LCS
      else eagerness1=0;
   }
   else eagerness1=2;
   
}

recruitst::~recruitst()
{
   delete recruit;
}

char recruitst::eagerness()
{
   char eagerness_temp = eagerness1;
   //Moderates are decidedly less interested
   if(recruit->align==0)eagerness_temp-=2;
   //Conservatives are extremely uninterested
   if(recruit->align==-1)eagerness_temp-=4;
   return eagerness_temp;
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
   case 1:
      addstr(" will take a lot of persuading.");
      break;
   case 2:
      addstr(" is interested in learning more.");
      break;
   case 3:
      addstr(" feels something needs to be done.");
      break;
   default:
      if(r.eagerness()>=4)
         addstr(" is ready to fight for the Liberal Cause.");
      else
      {
         addstr(" kind of regrets agreeing to this.");
      }
      break;
   }
   move(11,0);
   addstr("How should ");
   addstr(pool[p]->name);
   addstr(" approach the situation?");

   move(13,0);
   if(ledger.get_funds()<50)set_color(COLOR_BLACK,COLOR_BLACK,1);
   addstr("A - Spend $50 on props and a book for them to keep afterward.");
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(14,0);
   addstr("B - Just casually chat with them and discuss politics.");
   
   move(15,0);
   if(subordinatesleft(*pool[p]) && r.eagerness()>=4)
   {
      addstr("C - Offer to let ");
      addstr(r.recruit->name);
      addstr(" to join the LCS as a full member.");
   }
   else if(!subordinatesleft(*pool[p]))
   {
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr("C - ");
      addstr(pool[p]->name);
      addstr(" needs more Juice to recruit.");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
   }
   else
   {
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr("C - ");
      addstr(r.recruit->name);
      addstr(" isn't ready to join the LCS.");
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

      if(c=='c' && subordinatesleft(*pool[p]) && r.eagerness()>=4)
      {
         move(y,0);
         addstr(pool[p]->name);
         addstr(" offers to let ");
         addstr(r.recruit->name);
         addstr(" join the Liberal Crime Squad.");

         refresh();
         getch();

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

         pool[p]->train(SKILL_PERSUASION,25);

         pool.push_back(r.recruit);
         r.recruit = NULL;
         stat_recruits++;

         return 1;
      }
      if(c=='b' || (c=='a' && ledger.get_funds()>=50))
      {
         if(c=='a')
         {
            ledger.subtract_funds(50,EXPENSE_RECRUITMENT);
         }
         //SAV - You can get your skill up to a 3 by chatting. Past that,
         // you must successfully recruit people. Training is slower the
         // better you are.
         //JDS - Increased max skill to get to 12 under this system, gave
         // minimum of 5 exp for the action.
         pool[p]->train(SKILL_PERSUASION,
            max(12-pool[p]->get_skill(SKILL_PERSUASION),5));
         pool[p]->train(SKILL_SCIENCE,
            max(r.recruit->get_skill(SKILL_SCIENCE)-pool[p]->get_skill(SKILL_SCIENCE),0));
         pool[p]->train(SKILL_RELIGION,
            max(r.recruit->get_skill(SKILL_RELIGION)-pool[p]->get_skill(SKILL_RELIGION),0));
         pool[p]->train(SKILL_LAW,
            max(r.recruit->get_skill(SKILL_LAW)-pool[p]->get_skill(SKILL_LAW),0));
         pool[p]->train(SKILL_BUSINESS,
            max(r.recruit->get_skill(SKILL_BUSINESS)-pool[p]->get_skill(SKILL_BUSINESS),0));
         
         int lib_persuasiveness = pool[p]->get_skill(SKILL_BUSINESS)+
                                  pool[p]->get_skill(SKILL_SCIENCE)+
                                  pool[p]->get_skill(SKILL_RELIGION)+
                                  pool[p]->get_skill(SKILL_LAW)+
                                  pool[p]->get_attribute(ATTRIBUTE_INTELLIGENCE,true);

         int recruit_reluctance = 5 +
                                  r.recruit->get_skill(SKILL_BUSINESS)+
                                  r.recruit->get_skill(SKILL_SCIENCE)+
                                  r.recruit->get_skill(SKILL_RELIGION)+
                                  r.recruit->get_skill(SKILL_LAW)+
                                  r.recruit->get_attribute(ATTRIBUTE_WISDOM,true)+
                                  r.recruit->get_attribute(ATTRIBUTE_INTELLIGENCE,true);

         if(lib_persuasiveness > recruit_reluctance)
            recruit_reluctance = 0;
         else
            recruit_reluctance -= lib_persuasiveness;

         int difficulty = recruit_reluctance;

         if(c=='a')
         {
            difficulty -= 5;

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
            if(pool[p]->gender_liberal==GENDER_MALE)
               addstr("his ");
            else if(pool[p]->gender_liberal==GENDER_FEMALE)
               addstr("her ");
            else
               addstr("their ");
            addstr("views on ");
            getviewsmall(str,LCSrandom(VIEWNUM-3));
            addstr(str);
            addstr(".");
            refresh();
            getch();
         }
         
         bool success=0;

         if(pool[p]->skill_check(SKILL_PERSUASION,difficulty))
         {
            success=1;
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            if(r.level<127) r.level++;
            if(r.eagerness1<127) r.eagerness1++;
            move(y++,0);
            addstr(r.recruit->name);
            addstr(" found ");
            addstr(pool[p]->name);
            addstr("'s views to be insightful.");
            move(y++,0);
            addstr("They'll definitely meet again tomorrow.");
         }
         else if(pool[p]->skill_check(SKILL_PERSUASION,difficulty)) // Second chance to not fail horribly
         {
            if(r.level<127) r.level++;
            if(r.eagerness1>-128) r.eagerness1--;
            move(y++,0);
            addstr(r.recruit->name);
            addstr(" is skeptical about some of ");
            addstr(pool[p]->name);
            addstr("'s arguments.");
            move(y++,0);
            addstr("They'll meet again tomorrow.");
         }
         else
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            move(y++,0);
            if(r.recruit->talkreceptive() && r.recruit->align==ALIGN_LIBERAL)
            {
               addstr(r.recruit->name);
               addstr(" isn't convinced ");
               addstr(pool[p]->name);
               addstr(" really understands the problem.");
               move(y++,0);
               addstr("Maybe ");
               addstr(pool[p]->name);
               addstr(" needs more experience.");
            }
            else
            {
               addstr(pool[p]->name);
               addstr(" comes off as slightly insane.");
               move(y++,0);
               addstr("This whole thing was a mistake. There won't be another meeting.");
            }
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
   }while(1);
}
