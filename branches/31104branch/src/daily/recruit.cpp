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
   if(LCSrandom(100)<attitude[VIEW_LIBERALCRIMESQUAD])
   {
      //Likes the LCS
      if(LCSrandom(100)<attitude[VIEW_LIBERALCRIMESQUADPOS])
      {
         eagerness1=1;
      }
      //Doesn't like the LCS
      else eagerness1=-1;
   }
   
}

char recruitst::eagerness()
{
   char eagerness_temp = eagerness1 + pool[getpoolcreature(recruiter_id)]->skill[SKILL_LEADERSHIP]; 
   if(talkreceptive(*recruit))
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
   int e=0;

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
         r.recruit->skill[SKILL_RIFLE] &&
         (law[LAW_GUNCONTROL]==-2 || r.recruit->skill[SKILL_STREETSENSE]))
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
         r.recruit->skill[SKILL_SMG] &&
         (law[LAW_GUNCONTROL]==-2 || r.recruit->skill[SKILL_STREETSENSE]))
      {
         r.recruit->weapon.type=WEAPON_SMG_MP5;
         r.recruit->weapon.ammo=15;
         r.recruit->clip[CLIP_SMG]=4;
      }
      else if(r.recruit->money>400 &&
         r.recruit->skill[SKILL_SHOTGUN] &&
         (law[LAW_GUNCONTROL]<2 || r.recruit->skill[SKILL_STREETSENSE]))
      {
         r.recruit->weapon.type=WEAPON_SEMIRIFLE_AR15;
         r.recruit->weapon.ammo=30;
         r.recruit->clip[CLIP_ASSAULT]=4;
      }
      else if(r.recruit->money>350 &&
         r.recruit->skill[SKILL_RIFLE] &&
         (law[LAW_GUNCONTROL]<=-1 || r.recruit->skill[SKILL_STREETSENSE]))
      {
         r.recruit->weapon.type=WEAPON_SEMIRIFLE_AR15;
         r.recruit->weapon.ammo=30;
         r.recruit->clip[CLIP_ASSAULT]=4;
      }
      else if(r.recruit->money>300 &&
         r.recruit->skill[SKILL_PISTOL] &&
         (law[LAW_GUNCONTROL]<1 || r.recruit->skill[SKILL_STREETSENSE]))
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
      else if(r.recruit->skill[SKILL_SWORD])
      {
         if(LCSrandom(5)) r.recruit->weapon.type=WEAPON_SWORD;
         else r.recruit->weapon.type=WEAPON_DAISHO;
      }
      else if(r.recruit->skill[SKILL_AXE])
      {
         r.recruit->weapon.type=WEAPON_AXE;
      }
      else if(r.recruit->skill[SKILL_PISTOL])
      {
         r.recruit->weapon.type=WEAPON_REVOLVER_22;
         r.recruit->weapon.ammo=6;
         r.recruit->clip[CLIP_22]=4;
      }
      else if(r.recruit->skill[SKILL_CLUB] && !r.recruit->skill[SKILL_KNIFE])
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
      addstr("finished chilling out. THIS IS A BUG, btw.");
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
   case VIEW_ABORTION:strcat(str,"a documentary on the women's rights struggle");break;
   case VIEW_GAY:strcat(str,"a documentary on the gay rights struggle");break;
   case VIEW_DEATHPENALTY:strcat(str,"a research paper on abuses of the death penalty");break;
	case VIEW_TAXES:strcat(str,"an economic paper on the flaws of trickle-down");break;
   case VIEW_NUCLEARPOWER:strcat(str,"a video tour of the chernobyl dead zone");break;
   case VIEW_ANIMALRESEARCH:strcat(str,"a documentary on animal research");break;
   case VIEW_POLICEBEHAVIOR:strcat(str,"a hand-recorded video of police brutality");break;
   case VIEW_PRISONS:strcat(str,"a government inquiry into prison conditions");break;
   case VIEW_INTELLIGENCE:strcat(str,"a documentary on privacy rights");break;
   case VIEW_FREESPEECH:strcat(str,"a collection of banned books");break;
   case VIEW_GENETICS:strcat(str,"a video about genetic engineering accidents");break;
   case VIEW_JUSTICES:strcat(str,"a Liberal policy paper inquiring into judicial decisions");break;
   case VIEW_SWEATSHOPS:strcat(str,"a hand-recorded video of unregulated sweatshops");break;
   case VIEW_POLLUTION:strcat(str,"a leaked government paper on environmental conditions");break;
   case VIEW_CORPORATECULTURE:strcat(str,"a documentary on life under corporate culture");break;
   case VIEW_CEOSALARY:strcat(str,"a Liberal think-tank survey of top CEO salaries");break;
   case VIEW_AMRADIO:strcat(str,"a collection of Conservative radio host rants");break;
   case VIEW_CABLENEWS:strcat(str,"a collection of leaked Conservative cable news memos");break;
   }
}


/* daily - recruit - recruit meeting */
char completerecruitmeeting(recruitst &r,int p,char &clearformess)
{
   int e;
   clearformess=1;

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
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
   addstr("C - Ask them to help the Liberal cause with a cash donation.");
   move(16,0);
   if(subordinatesleft(*pool[p]))
   {
      addstr("D - Offer to let ");
      addstr(r.recruit->name);
      addstr(" to join the LCS as a full member.");
   }
   else
   {
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr("D - ");
      addstr(pool[p]->name);
      addstr(" needs more Juice or Leadership to recurit.");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
   }
   

   int y=18;
   

   do
   {
      refresh();
      int c=getch();
      translategetch(c);

      char test=0;
      if(c=='d' && subordinatesleft(*pool[p]))
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
            move(y+2,0);
            
            addstr(r.recruit->name);
            addstr(" accepts, and is eager to get started.");

            liberalize(*r.recruit,false);

            erase();

            move(6,0);
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr("In what capacity will ");
            addstr(r.recruit->name);
            addstr(" best serve the Liberal cause?");
            move(7,0);
            addstr("1) Stay at ");
            addstr(location[r.recruit->worklocation]->name);
            addstr(" as a ");
            set_color(COLOR_CYAN,COLOR_BLACK,0);
            addstr("sleeper agent");
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr(".");
            move(8,0);
            addstr("2) Come to ");
            addstr(location[pool[p]->location]->name);
            addstr(" as a ");
            set_color(COLOR_GREEN,COLOR_BLACK,0);
            addstr("regular member");
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr(".");

            
            while(1)
            {
               char keystroke = getch();
               if(keystroke == '1')
               {
                  r.recruit->flag |= CREATUREFLAG_SLEEPER;
                  r.recruit->location = r.recruit->worklocation;
                  r.recruit->base = r.recruit->worklocation;
                  r.recruit->infiltration/=2;
                  break;
               }
               else if(keystroke == '2')
               {
                  r.recruit->location=pool[p]->location;
                  r.recruit->base=pool[p]->base;
                  liberalize(*r.recruit,false);
                  r.recruit->infiltration/=2;
                  break;
               }
            }

            r.recruit->hireid=pool[p]->id;

            pool[p]->skill_ip[SKILL_LEADERSHIP]+=5;

            pool.push_back(r.recruit);
            stat_recruits++;

            refresh();
            getch();

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
         pool[p]->skill_ip[SKILL_PERSUASION]+=5;
         
         int lib_persuasiveness = pool[p]->skill[SKILL_PERSUASION]*2+
                                  pool[p]->skill[SKILL_BUSINESS]+
                                  pool[p]->skill[SKILL_SCIENCE]+
                                  pool[p]->skill[SKILL_RELIGION]+
                                  pool[p]->skill[SKILL_LAW]+
                                  pool[p]->attval(ATTRIBUTE_HEART)+
                                  pool[p]->attval(ATTRIBUTE_CHARISMA)+
                                  pool[p]->attval(ATTRIBUTE_INTELLIGENCE);

         int recruit_reluctance = r.recruit->skill[SKILL_BUSINESS]+
                                  r.recruit->skill[SKILL_SCIENCE]+
                                  r.recruit->skill[SKILL_RELIGION]+
                                  r.recruit->skill[SKILL_LAW]+
                                  r.recruit->attval(ATTRIBUTE_WISDOM)*3+
                                  r.recruit->attval(ATTRIBUTE_INTELLIGENCE)+
                                  LCSrandom(10);

         int max_eagerness      = pool[p]->attval(ATTRIBUTE_HEART)-
                                  r.recruit->attval(ATTRIBUTE_WISDOM);

         if(c=='a')
         {
            lib_persuasiveness+=LCSrandom(20);
            max_eagerness     +=LCSrandom(20);
         }


         if((lib_persuasiveness > recruit_reluctance) &&
            (max_eagerness      > r.eagerness()     ))
         {
            set_color(COLOR_BLUE,COLOR_BLACK,1);
            r.level++;
            r.eagerness1++;
            move(y++,0);
            addstr(r.recruit->name);
            addstr(" found ");
            addstr(pool[p]->name);
            addstr("'s views to be insightful.");
            move(y++,0);
            addstr("They'll definately meet again tomorrow.");
         }
         else if((lib_persuasiveness+LCSrandom(5) > recruit_reluctance))
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
      if(c=='c')
      {
         if(r.eagerness()>5)
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            int donationamount=0;
            move(y++,0);
            addstr(r.recruit->name);
            addstr(" would be happy to help.");
            
            if(r.recruit->money>150)
            {
               donationamount=500;
            }
            else if(r.recruit->money>50)
            {
               donationamount=100;
            }
            else if(r.recruit->money)
            {
               donationamount=50;
            }
            else
            {
               donationamount=10;
            }

            if(r.eagerness()>7)
               donationamount=static_cast<int>((1.5+LCSrandom(6)/10.0)*donationamount);
            if(r.recruit->attval(ATTRIBUTE_HEART)>8)
               donationamount=static_cast<int>((1.5+LCSrandom(6)/10.0)*donationamount);
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
      }
   }while(1);
}
