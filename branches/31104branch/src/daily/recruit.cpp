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

#include <includes.h>
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
   char eagerness_temp = eagerness1 + level + pool[getpoolcreature(recruiter_id)]->skill[SKILL_LEADERSHIP]; 
   if(talkreceptive(*recruit))
   {
      eagerness1+=3;
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
         (law[LAW_GUNCONTROL]==-2 || r.recruit->skill[SKILL_GANGSTERISM]))
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
         (law[LAW_GUNCONTROL]==-2 || r.recruit->skill[SKILL_GANGSTERISM]))
      {
         r.recruit->weapon.type=WEAPON_SMG_MP5;
         r.recruit->weapon.ammo=15;
         r.recruit->clip[CLIP_SMG]=4;
      }
      else if(r.recruit->money>400 &&
         r.recruit->skill[SKILL_SHOTGUN] &&
         (law[LAW_GUNCONTROL]<2 || r.recruit->skill[SKILL_GANGSTERISM]))
      {
         r.recruit->weapon.type=WEAPON_SEMIRIFLE_AR15;
         r.recruit->weapon.ammo=30;
         r.recruit->clip[CLIP_ASSAULT]=4;
      }
      else if(r.recruit->money>350 &&
         r.recruit->skill[SKILL_RIFLE] &&
         (law[LAW_GUNCONTROL]<=-1 || r.recruit->skill[SKILL_GANGSTERISM]))
      {
         r.recruit->weapon.type=WEAPON_SEMIRIFLE_AR15;
         r.recruit->weapon.ammo=30;
         r.recruit->clip[CLIP_ASSAULT]=4;
      }
      else if(r.recruit->money>300 &&
         r.recruit->skill[SKILL_PISTOL] &&
         (law[LAW_GUNCONTROL]<1 || r.recruit->skill[SKILL_GANGSTERISM]))
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
   char str[25];
   getrecruitcreature(str,r.recruit->type);
   addstr(str);

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   printfunds(0,1,"Money: ");

   printcreatureinfo(r.recruit);
   makedelimiter(8,0);

   move(10,0);
   addstr(r.recruit->name);
   switch(r.level)
   {
   case 0:
      if(r.eagerness()>6 && subordinatesleft(*pool[p]))addstr(" hopes this is about joining the LCS.");
      else addstr(" is curious what this is all about.");
      break;
   case 1:
      if(r.eagerness()>7 && subordinatesleft(*pool[p]))addstr(" is taking this pretty seriously.");
      else addstr(" has completed the community service.");
      break;
   case 2:
      if(r.eagerness()>8 && subordinatesleft(*pool[p]))addstr(" is determined and eager to join.");
      else addstr(" did well with the assigned tasks.");
      break;
   case 3:
      if(r.eagerness()>9 && subordinatesleft(*pool[p]))addstr(" is ready to fight for the Liberal Cause.");
      else
      {
         addstr(" has done everything ");
         addstr(pool[p]->name);
         addstr(" has asked.");
      }
      break;
   }
   move(11,0);
   addstr("How should ");
   addstr(pool[p]->name);
   addstr(" approach the situation?");

   switch(r.level)
   {
   case 0:
      move(13,0);
      addstr("A - Invite ");
      addstr(r.recruit->name);
      addstr(" to join the LCS immediately.");
      move(14,0);
      addstr("B - Ease them in: organize a week of community service tasks.");
      break;
   case 1:
      move(13,0);
      addstr("A - Invite ");
      addstr(r.recruit->name);
      addstr(" to join the LCS.");
      move(14,0);
      addstr("B - Encourage them to engage in more aggressive political activism.");
      break;
   case 2:
      move(13,0);
      addstr("A - Invite ");
      addstr(r.recruit->name);
      addstr(" to join the LCS.");
      move(14,0);
      addstr("B - Ask ");
      addstr(r.recruit->name);
      addstr(" to commit some minor crimes.");
      break;
   case 3:
      move(13,0);
      addstr("A - Welcome ");
      addstr(r.recruit->name);
      addstr(" to the LCS.");
      move(14,0);
      if(funds>=1000)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr("B - Offer ");
      addstr(r.recruit->name);
      addstr(" $1000 to join the LCS.");
      break;
   }
   move(15,0);
   int y;
   if(r.recruit->weapon.type==WEAPON_NONE && r.level>=2)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);      
      addstr("C - Tell ");
      addstr(r.recruit->name);
      addstr("to get a weapon.");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(16,0);
      if(funds>=500)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr("D - Tell ");
      addstr(r.recruit->name);
      addstr("to get a weapon. Give them $500.");
      move(17,0);
      if(funds>=1500)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr("E - Tell ");
      addstr(r.recruit->name);
      addstr("to get a weapon. Give them $1500.");
      move(18,0);
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("F - ");
      addstr(r.recruit->name);
      addstr(" is not LCS material. Just go home.");
      y=20;
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("C - ");
      addstr(r.recruit->name);
      addstr(" is not LCS material. Just go home.");
      y=17;
   }
   

   do
   {
      refresh();
      int c=getch();
      translategetch(c);

      char test=0;
      if(c=='a')
      {
         move(y,0);
         addstr(pool[p]->name);
         addstr(" invites ");
         addstr(r.recruit->name);
         addstr(" to join the Liberal Crime Squad.");

         refresh();
         getch();

         if(r.eagerness()>6 && subordinatesleft(*pool[p]))
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            move(y+2,0);
            
            addstr(r.recruit->name);
            addstr(" accepts, and is eager to get started.");

            r.recruit->align=1;

            r.recruit->location=pool[p]->location;
            r.recruit->base=pool[p]->base;
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
            addstr(" is not that insane. ");
            addstr("Thanks, but no thanks.");

            refresh();
            getch();

            delete r.recruit;
            return 1;
         }
      }
      if(c=='b' || (c>='c' && c<='e' && r.level>1 && r.recruit->weapon.type==WEAPON_NONE
                    && r.recruit->align<=0 && r.recruit->skill[SKILL_GANGSTERISM]==0))
      {
         if(r.level<3 && c=='b' && r.eagerness()>=r.level*2)
         {
            move(y,0);
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            r.task = r.level+1;
            r.timeleft = 7;
            addstr(r.recruit->name);
            addstr(" will come back in a week, when the job is done.");
            refresh();
            getch();

            return 0;
         }
         else if(r.level==3 && c=='b' && funds>=1000)
         {
            move(y,0);
            addstr(pool[p]->name);
            addstr(" shows ");
            addstr(r.recruit->name);
            addstr(" $1000 and invites them to join.");
            refresh();
            getch();

            if(r.eagerness()>5&&subordinatesleft(*pool[p]))
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(y+2,0);
               
               addstr(r.recruit->name);
               addstr(" accepts, and is eager to get started.");

               r.recruit->align=1;

               r.recruit->location=pool[p]->location;
               r.recruit->base=pool[p]->base;
               r.recruit->hireid=pool[p]->id;

               pool[p]->skill_ip[SKILL_LEADERSHIP]+=5;

               pool.push_back(r.recruit);
               stat_recruits++;

               funds-=1000;

               refresh();
               getch();

               return 1;
            }
            else
            {
               set_color(COLOR_MAGENTA,COLOR_BLACK,0);
               move(y+2,0);
               addstr(r.recruit->name);
               addstr(" isn't swayed by the money.");
               move(y+3,0);
               addstr("Thanks, but no thanks."); 
               refresh();
               getch();

               delete r.recruit;
               return 1;
            }
         }
         else if(r.level<3)
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,0);
            move(y+2,0);
            addstr(r.recruit->name);
            if(r.level!=0)addstr(" is just not hardcore enough for that. Thanks, but no thanks.");
            else addstr(" thought this would take less committment. Good luck though.");
            refresh();
            getch();

            return 1;
         }
      }
      if(c=='e' && r.level>=2 && funds>=1500 && r.recruit->weapon.type==WEAPON_NONE)
      {
         move(y,0);
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         if((r.recruit->skill[SKILL_RIFLE] || r.recruit->skill[SKILL_SMG]) &&
            (r.recruit->skill[SKILL_GANGSTERISM] || law[LAW_GUNCONTROL]==-2))
         {
            addstr(r.recruit->name);
            addstr(" takes the $1500 and sets off to acquire a wicked gun.");

            r.task = TASK_BUYWEAPON;
            r.timeleft = 1;

            r.recruit->money+=1500;
            funds-=1500;

            refresh();
            getch();

            return 0;
         }
         if((r.recruit->skill[SKILL_RIFLE] && law[LAW_GUNCONTROL]<=-1) ||
            ((r.recruit->skill[SKILL_SHOTGUN] || r.recruit->skill[SKILL_PISTOL]) &&
               (law[LAW_GUNCONTROL]<=1 || r.recruit->skill[SKILL_GANGSTERISM])))
         {
            addstr(r.recruit->name);
            addstr(" only accepts $500, and promises to return soon.");

            r.task = TASK_BUYWEAPON;
            r.timeleft = 1;

            r.recruit->money+=500;
            funds-=500;

            refresh();
            getch();

            return 0;
         }
         else
         {
            addstr(r.recruit->name);
            addstr(" refuses the money, but agrees to get a weapon.");

            r.task = TASK_BUYWEAPON;
            r.timeleft = 1;

            refresh();
            getch();

            return 0;
         }
      }
      if(c=='d' && r.level>=2 && funds>=500 && r.recruit->weapon.type==WEAPON_NONE)
      {
         move(y,0);
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         if((r.recruit->skill[SKILL_RIFLE] && law[LAW_GUNCONTROL]<=-1) ||
            ((r.recruit->skill[SKILL_SHOTGUN] || r.recruit->skill[SKILL_PISTOL]) &&
               (law[LAW_GUNCONTROL]<=1 || r.recruit->skill[SKILL_GANGSTERISM])))
         {
            addstr(r.recruit->name);
            addstr(" takes the $500, and promises to return soon.");

            r.task = TASK_BUYWEAPON;
            r.timeleft = 1;

            r.recruit->money+=500;
            funds-=500;

            refresh();
            getch();

            return 0;
         }
         else
         {
            addstr(r.recruit->name);
            addstr(" refuses the money, but agrees to get a weapon.");

            r.task = TASK_BUYWEAPON;
            r.timeleft = 1;

            refresh();
            getch();

            return 0;
         }
      }
      if(c=='c' && r.level>=2 && r.recruit->weapon.type==WEAPON_NONE)
      {
         move(y,0);
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         addstr(r.recruit->name);
         addstr(" agrees to get a weapon.");

         r.task = TASK_BUYWEAPON;
         r.timeleft = 1;

         refresh();
         getch();

         return 0;
      }
      if(c=='f' || c=='c')
      {
         move(y,0);
         addstr(pool[p]->name);
         addstr(" sends ");
         addstr(r.recruit->name);
         addstr(" home.");

         refresh();
         getch();

         delete r.recruit;
         return 1;
      }
   }while(1);
}
