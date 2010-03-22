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

/* base - review and reorganize liberals */
void review(void)
{
   int page=0;

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Review your Liberals and Assemble Squads");

      move(1,0);
      addstr("----SQUAD NAME-----------------LOCATION------------ACTIVITY----------------------");

      int y=2;
      for(int p=page*19;p<squad.size()+REVIEWMODENUM+1&&p<page*19+19;p++)
      {
         if(p<squad.size())
         {
            if(activesquad==squad[p])set_color(COLOR_WHITE,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(y,0);
            addch(y+'A'-2);addstr(" - ");
            addstr(squad[p]->name);

            move(y,31);
            if(squad[p]->squad[0]!=NULL)
            {
               addshortname(location[squad[p]->squad[0]->location]);
            }

            move(y,51);
            if(squad[p]->squad[0]!=NULL)
            {
               char str[80];
               getactivity(str,squad[p]->activity);

               if(squad[p]->activity.type==ACTIVITY_NONE)
               {
                  int count=0;char haveact=0;
                  for(int p2=0;p2<6;p2++)
                  {
                     if(squad[p]->squad[p2]==NULL)continue;
                     count++;
                     if(squad[p]->squad[p2]->activity.type!=ACTIVITY_NONE)
                     {
                        getactivity(str,squad[p]->squad[p2]->activity);
                        haveact=1;
                     }
                  }
                  if(haveact&&count>1)strcpy(str,"Acting Individually");
               }

               addstr(str);
            }
         }
         else if(p==squad.size())
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            move(y,0);
            addch('1');addstr(" - ");
            addstr("Active Liberals");
         }
         else if(p==squad.size()+1)
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            move(y,0);
            addch('2');addstr(" - ");
            addstr("Hostages");
         }
         else if(p==squad.size()+2)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(y,0);
            addch('3');addstr(" - ");
            addstr("Hospital");
         }
         else if(p==squad.size()+3)
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(y,0);
            addch('4');addstr(" - ");
            addstr("Justice System");
         }
         else if(p==squad.size()+4)
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            move(y,0);
            addch('5');addstr(" - ");
            addstr("Sleepers");
         }
         else if(p==squad.size()+5)
         {
            set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(y,0);
            addch('6');addstr(" - ");
            addstr("The Dead");
         }
         else if(p==squad.size()+6)
         {
            set_color(COLOR_BLUE,COLOR_BLACK,1);
            move(y,0);
            addch('7');addstr(" - ");
            addstr("Away");
         }
         else if(p==squad.size()+7)
         {
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            move(y,0);
            addch('8');addstr(" - ");
            addstr("Review and Move Equipment");
         }

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a squad.  1-7 to view Liberal groups.");
      move(23,0);
      addpagestr();
      addstr("  Press U to Promote Liberals.");
      move(24,0);
      addstr("Press Z to Assemble a New Squad.  Press T to Assign New Bases to the Squadless.");

      refresh();

      int c=getch();
      translategetch(c);

      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<squad.size()+REVIEWMODENUM)page++;

      if(c==10)return;

      if(c>='a'&&c<='s')
      {
         int sq=page*19+(int)(c-'a');
         if(sq<squad.size()&&sq>=0)
         {
            if(squad[sq]==activesquad)assemblesquad(squad[sq]);
            else activesquad=squad[sq];
         }
      }
      if(c>='1'&&c<='7')review_mode((int)(c-'1'));
      if(c=='8')equipmentbaseassign();
      if(c=='z')
      {
         assemblesquad(NULL);
         if(activesquad==NULL&&squad.size()>0)
         {
            activesquad=squad[squad.size()-1];
         }
      }
      if(c=='t')squadlessbaseassign();
      if(c=='u')promoteliberals();
   }while(1);
}



void review_mode(short mode)
{
   vector<Creature *> temppool;
   Creature *swap = NULL;
   int swapPos = 0;

   for(int p=0;p<pool.size();p++)
   {
      switch(mode)
      {
      case REVIEWMODE_LIBERALS:
         if(pool[p]->alive==1&&
            pool[p]->align==1&&
            pool[p]->clinic==0&&
            pool[p]->dating==0&&
            pool[p]->hiding==0&&
            !(pool[p]->flag & CREATUREFLAG_SLEEPER))
         {
            if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
               location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
               location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON)temppool.push_back(pool[p]);
         }
         break;
      case REVIEWMODE_HOSTAGES:
         if(pool[p]->align!=1&&
            pool[p]->alive==1)
            temppool.push_back(pool[p]);
         break;
      case REVIEWMODE_CLINIC:
         if(pool[p]->clinic>0&&
            pool[p]->alive==1)temppool.push_back(pool[p]);
         break;
      case REVIEWMODE_JUSTICE:
         if(!(pool[p]->flag & CREATUREFLAG_SLEEPER)&&
            pool[p]->clinic==0&&
            pool[p]->dating==0&&
            pool[p]->hiding==0&&
            pool[p]->alive==1)
         {
            if(location[pool[p]->location]->type==SITE_GOVERNMENT_POLICESTATION||
               location[pool[p]->location]->type==SITE_GOVERNMENT_COURTHOUSE||
               location[pool[p]->location]->type==SITE_GOVERNMENT_PRISON)temppool.push_back(pool[p]);
         }
         break;
      case REVIEWMODE_SLEEPERS:
         if(pool[p]->alive==1&&
            pool[p]->align==1&&
            pool[p]->clinic==0&&
            pool[p]->dating==0&&
            pool[p]->hiding==0&&
            (pool[p]->flag & CREATUREFLAG_SLEEPER))temppool.push_back(pool[p]);
         break;
      case REVIEWMODE_DEAD:
         if(!pool[p]->alive)temppool.push_back(pool[p]);
         break;
      case REVIEWMODE_AWAY:
         if((pool[p]->dating>0||
            pool[p]->hiding!=0)&&
            pool[p]->alive==1)temppool.push_back(pool[p]);
         break;
      }
   }

   if(temppool.size()==0)return;

   int page=0;

   char num[20];

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      switch(mode)
      {
      case REVIEWMODE_LIBERALS:
         addstr("Active Liberals");
         break;
      case REVIEWMODE_HOSTAGES:
         addstr("Conservative Automatons in Captivity");
         break;
      case REVIEWMODE_CLINIC:
         addstr("Liberals in CLINICS");
         break;
      case REVIEWMODE_JUSTICE:
         addstr("Liberals and the Justice System");
         break;
      case REVIEWMODE_SLEEPERS:
         addstr("Sleepers");
         break;
      case REVIEWMODE_DEAD:
         addstr("Liberal Martyrs and Dead Bodies");
         break;
      case REVIEWMODE_AWAY:
         addstr("Liberals that are Away");
         break;
      }
      move(1,0);
      addstr("----CODE NAME------------SKILL---HEALTH---LOCATION------------------------------");

      move(1,57);
      switch(mode)
      {
      case REVIEWMODE_LIBERALS:
         addstr("SQUAD / ACTIVITY");
         break;
      case REVIEWMODE_HOSTAGES:
         addstr("DAYS IN CAPTIVITY");
         break;
      case REVIEWMODE_JUSTICE:
         addstr("MONTHS LEFT");
         break;
      case REVIEWMODE_CLINIC:
         addstr("PROGNOSIS");
         break;
      case REVIEWMODE_SLEEPERS:
         addstr("PROFESSION");
         break;
      case REVIEWMODE_DEAD:
         addstr("DAYS SINCE PASSING");
         break;
      case REVIEWMODE_AWAY:
         addstr("DAYS UNTIL RETURN");
         break;
      }

      int y=2;
      for(int p=page*19;p<temppool.size()&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         char bright=0;
         int skill=0;
         for(int sk=0;sk<SKILLNUM;sk++)
         {
            skill+=(int)temppool[p]->get_skill(sk);
            if(temppool[p]->get_skill_ip(sk)>=100+(10*temppool[p]->get_skill(sk))&&
               temppool[p]->get_skill(sk)<temppool[p]->skill_cap(sk,true))bright=1;
         }

         set_color(COLOR_WHITE,COLOR_BLACK,bright);

         move(y,25);
         itoa(skill,num,10);
         addstr(num);

         printhealthstat(*temppool[p],y,33,TRUE);

         if(mode==REVIEWMODE_JUSTICE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,42);
         if(temppool[p]->location==-1)
         {
            addstr("Away");
         }
         else
         {
            addshortname(location[temppool[p]->location]);
         }

         move(y,57);
         switch(mode)
         {
         case REVIEWMODE_LIBERALS:
            {
               char usepers=1;
               if(temppool[p]->squadid!=-1)
               {
                  int sq=getsquad(temppool[p]->squadid);
                  if(sq!=-1)
                  {
                     if(squad[sq]->activity.type!=ACTIVITY_NONE)
                     {
                        set_color(COLOR_GREEN,COLOR_BLACK,1);
                        addstr("SQUAD");
                        usepers=0;
                     }
                  }
               }
               if(usepers)
               {
                  // Let's add some color here...
                  set_activity_color(temppool[p]->activity.type);
                  char str[80];
                  getactivity(str,temppool[p]->activity);
                  addstr(str);
               }
               break;
            }
         case REVIEWMODE_HOSTAGES:
            {
               set_color(COLOR_MAGENTA,COLOR_BLACK,1);
               char num[20];
               itoa(temppool[p]->joindays,num,10);
               addstr(num);
               addstr(" ");
               if(temppool[p]->joindays>1)addstr("Days");
               else addstr("Day");
               break;
            }
         case REVIEWMODE_JUSTICE:
            {
               if(temppool[p]->deathpenalty&&temppool[p]->sentence!=0)
               {
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  addstr("DEATH ROW: ");
                  itoa(temppool[p]->sentence,num,10);
                  addstr(num);
                  addstr(" ");
                  if(temppool[p]->sentence>1)addstr("Months");
                  else addstr("Month");
               }
               else if(temppool[p]->sentence==-1)
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,0);
                  addstr("Life Sentence");
               }
               else if(temppool[p]->sentence!=0)
               {
                  set_color(COLOR_YELLOW,COLOR_BLACK,1);
                  char num[20];
                  itoa(temppool[p]->sentence,num,10);
                  addstr(num);
                  addstr(" ");
                  if(temppool[p]->sentence>1)addstr("Months");
                  else addstr("Month");
               }
               else
               {
                  set_color(COLOR_BLACK,COLOR_BLACK,1);
                  addstr("-------");
               }
               break;
            }
         case REVIEWMODE_CLINIC:
            {
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               char num[20];
               itoa(temppool[p]->clinic,num,10);
               addstr("Out in ");
               addstr(num);
               addstr(" ");
               if(temppool[p]->clinic>1)addstr("Weeks");
               else addstr("Week");
               break;
            }
         case REVIEWMODE_SLEEPERS:
            {
               char str[200];
               if(temppool[p]->align==-1)set_color(COLOR_RED,COLOR_BLACK,1);
               else if(temppool[p]->align==0)set_color(COLOR_WHITE,COLOR_BLACK,1);
               else set_color(COLOR_GREEN,COLOR_BLACK,1);
               getrecruitcreature(str,temppool[p]->type);
               addstr(str);
               break;
            }
         case REVIEWMODE_DEAD:
            {
               set_color(COLOR_MAGENTA,COLOR_BLACK,1);
               char num[20];
               itoa(temppool[p]->deathdays,num,10);
               addstr(num);
               addstr(" ");
               if(temppool[p]->deathdays>1)addstr("Days");
               else addstr("Day");
               break;
            }
         case REVIEWMODE_AWAY:
            {
               set_color(COLOR_BLUE,COLOR_BLACK,1);
               if(temppool[p]->hiding!=-1)
               {
                  char num[20];
                  itoa(temppool[p]->dating+
                     temppool[p]->hiding,num,10);
                  addstr(num);
                  addstr(" ");
                  if(temppool[p]->dating+
                     temppool[p]->hiding>1)addstr("Days");
                  else addstr("Day");
               }
               else addstr("<No Contact>");
               break;
            }
         }

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to View Status.        Z - ");
      if (swap) {
	      addstr ("Place ");
	      addstr (swap->name);
      }
      else {
	      addstr ("Reorder Liberals");
      }
      move(23,0);
      addpagestr();

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<temppool.size())page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<temppool.size())
         {
            int page=0;
            const int pagenum=2;
            do
            {
               erase();

               move(0,0);
               if(temppool[p]->align!=1)
               {
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  addstr("Profile of an Automaton");
               }
               else
               {
                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  addstr("Profile of a Liberal");
               }

               if(page==0)
                  printliberalstats(*temppool[p]);
               else if(page==1)
                  printliberalskills(*temppool[p]);
               else if(page==2)               
                  printliberalcrimes(*temppool[p]);
               
               // Add removal of squad members member
               move(22,0);

               if((temppool[p]->flag != CREATUREFLAG_SLEEPER)&&
                  temppool[p]->hireid !=-1 &&
                  temppool[p]->clinic==0&&
                  temppool[p]->dating==0&&
                  temppool[p]->hiding==0&&
                  temppool[p]->alive==1&&
                  location[temppool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION &&
                  location[temppool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE &&
                  location[temppool[p]->location]->type!=SITE_GOVERNMENT_PRISON)  // If alive and not own boss? (suicide?)
               {
                  addstr("R - Remove member         K - Kill member");
               }

               move(23,0);      

               if(temppool[p]->align!=1)addstr("Press N to change this Automaton's Code Name");
               else
               {
                  addstr("N - Change Code Name      G - Fix Gender Label");
               }
               if(temppool.size()>1)
               {
                  addstr("    LEFT/RIGHT - View Others");
               }
               move(24,0);
               addstr("Press any other key to continue the Struggle");
               addstr("    UP/DOWN  - More Info");

               refresh();
               int c=getch();
               translategetch(c);

               if(temppool.size()>0&&((c==KEY_LEFT)||(c==KEY_RIGHT)))
               {
                  int sx=1;
                  if((c==KEY_LEFT))sx=-1;
                  p=(p+(int)temppool.size()+sx)%((int)temppool.size());
                  continue;
               }

               if(c==KEY_DOWN)
               {
                  page++;
                  if(page>2)page=0;
                  continue;
               }

               if(c==KEY_UP)
               {
                  page--;
                  if(page<0)page=2;
                  continue;
               }

               if(c=='n')
               {
                  move(23,0);
                  set_color(COLOR_WHITE,COLOR_BLACK,0);
                  addstr("What is the new code name?                                       ");
                  move(24,0);
                  addstr("                                                                 ");

                  keypad(stdscr,FALSE);
                  raw_output(FALSE);
                  echo();
                  curs_set(1);
                  move(24,0);
                  enter_name(temppool[p]->name,CREATURE_NAMELEN,
                     temppool[p]->propername);

                  curs_set(0);
                  noecho();
                  raw_output(TRUE);
                  keypad(stdscr,TRUE);
               }
               else if(c=='g' && temppool[p]->align==1)
               {
                  temppool[p]->gender_liberal++;
                  if(temppool[p]->gender_liberal > 2)
                     temppool[p]->gender_liberal = 0;
               }      
               else if(c=='r' && (temppool[p]->flag != CREATUREFLAG_SLEEPER)&&
                  temppool[p]->hireid !=-1 &&
                  temppool[p]->clinic==0&&
                  temppool[p]->dating==0&&
                  temppool[p]->hiding==0&&
                  temppool[p]->alive==1&&
                  location[temppool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION &&
                  location[temppool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE &&
                  location[temppool[p]->location]->type!=SITE_GOVERNMENT_PRISON)  // If alive and not own boss? (suicide?)
               {
                  int boss = getpoolcreature(temppool[p]->hireid);

                  move(22,0);
                  set_color(COLOR_WHITE,COLOR_BLACK,0);
                  addstr("Do you want to permanently release this squad member from the LCS?          ");

                  move(23,0);
                  addstr("If the member has low heart they may go to the police.                         ");

                  move(24,0);
                  addstr("  C - Confirm       Any other key to continue                                                ");

                  c=getch();
                  translategetch(c);

                  if(c=='c')
                  {
                     // Release squad member
                     move(22,0);
                     addstr(temppool[p]->name);
                     addstr(" has been released.                                                                      ");
                     move(23,0);
                     addstr("                                                                                         ");
                     move(24,0);
                     addstr("                                                                                         ");
                     getch();
                     // Chance of member going to police if boss has criminal record and
                     // if they have low heart
                     // TODO: Do law check against other members?
                     if(temppool[p]->get_attribute(ATTRIBUTE_HEART,true) < temppool[p]->get_attribute(ATTRIBUTE_WISDOM,true)+ LCSrandom(5)
                        && iscriminal(*pool[boss]))
                     {
                        if(!LCSrandom(5)) // Chance of tip off
                        {
                           set_color(COLOR_CYAN,COLOR_BLACK,1);
                           move(22,0);
                           addstr("A Liberal friend tips you off on ");
                           addstr(temppool[p]->name);
                           addstr("'s whereabouts.");
                           move(23,0);
                           addstr("The Conservative traitor has ratted you out to the police.");
                           getch();
                        }

                        criminalize(*pool[boss],LAWFLAG_RACKETEERING);
                        pool[boss]->confessions++;

                        // TODO: Depending on the crime increase heat or make seige

                        if(location[pool[boss]->location]->heat > 20)
                           location[pool[boss]->location]->siege.timeuntillocated=3;
                        else
                           location[pool[boss]->location]->heat += 20;
                     }

                     // Remove squad member
                     removesquadinfo(*temppool[p]);
                     if(temppool[p]->align==1)
                     {
                        if(boss!=-1&&pool[boss]->juice>50)
                        {
                           int juice=pool[boss]->juice-50;
                           if(juice>5)juice=5;
                           addjuice(*pool[boss],-juice);
                        }
                     }

                     int thisPersonIndex = getpoolcreature(temppool[p]->id);
                     delete temppool[p];
                     temppool.erase(temppool.begin() + p);
                     pool.erase(pool.begin() + thisPersonIndex);
                     break;
                  }
               }               
               else if(c=='k' && (temppool[p]->flag != CREATUREFLAG_SLEEPER)&&
                  temppool[p]->hireid !=-1 &&
                  temppool[p]->clinic==0&&
                  temppool[p]->dating==0&&
                  temppool[p]->hiding==0&&
                  temppool[p]->alive==1&&
                  location[temppool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION &&
                  location[temppool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE &&
                  location[temppool[p]->location]->type!=SITE_GOVERNMENT_PRISON)  // If alive and not own boss? (suicide?)
               {
                  // Kill squad member
                  int boss = getpoolcreature(temppool[p]->hireid);

                  move(22,0);
                  set_color(COLOR_WHITE,COLOR_BLACK,0);
                  addstr("Confirm you want to have ");
                  addstr(pool[boss]->name);
                  addstr(" kill this squad member?");
                  move(23,0);
                  addstr("Killing your squad members is Not a Liberal Act.                              ");
                  move(24,0);
                  addstr("  C - Confirm       Any other key to continue                                                ");

                  c=getch();
                  translategetch(c);

                  if(c=='c')
                  {
                     temppool[p]->die();
                     stat_kills++;

                     move(22,0);
                     addstr(pool[boss]->name);
                     addstr(" executes ");
                     addstr(temppool[p]->name);
                     addstr(" by ");
                     switch(LCSrandom(3))
                     {
                     case 0:addstr("strangling to death.                             ");break;
                     case 1:addstr("beating to death.                                ");break;
                     case 2:addstr("cold execution.                                  ");break;
                     }
                     move(23,0);
                     addstr("                                                                            ");
                     move(24,0);
                     addstr("                                                                            ");

                     getch();
                     move(22,0);

                     if(boss!=-1)
                     {
                        // Dejuice boss
                        int juice=temppool[p]->juice/10;
                        if(juice<5)
                        {
                           juice=5;
                        }
                        addjuice(*pool[boss],-juice);

                        if(LCSrandom(pool[boss]->get_attribute(ATTRIBUTE_HEART,false))>LCSrandom(3))
                        {
                           set_color(COLOR_GREEN,COLOR_BLACK,1);
                           addstr(pool[boss]->name);
                           addstr(" feels sick to the stomach afterward and ");
                           pool[boss]->adjust_attribute(ATTRIBUTE_HEART,-1);
                           switch(LCSrandom(4))
                           {
                           case 0:addstr("throws up in a trash can.                                          ");break;
                           case 1:addstr("gets drunk, eventually falling asleep.                             ");break;
                           case 2:addstr("curls up in a ball, crying softly.                                 ");break;
                           case 3:addstr("shoots up and collapses in a heap on the floor.                    ");break;
                           }
                           move(23,0);
                           addstr(pool[boss]->name);
                           addstr(" has lost heart.");
                           getch();
                        }
                        else if(!LCSrandom(3))
                        {
                           set_color(COLOR_CYAN,COLOR_BLACK,1);
                           addstr(pool[boss]->name);
                           addstr(" grows colder.                                                            ");
                           pool[boss]->adjust_attribute(ATTRIBUTE_WISDOM,+1);
                           move(23,0);
                           addstr(pool[boss]->name);
                           addstr(" has gained wisdom.                                                           ");
                           getch();
                        }
                     }

                     break;
                  }      
               }
               else break;
            }while(1);
         }
      }

      // Reorder squad
      if(c=='z')
      {
	 if(temppool.size()<=1)break;

	 move(22,0);
	 addstr("                                                                               ");
	 move(23,0);
	 addstr("                                                                               ");
	 
	 move(22,8);
	 set_color(COLOR_WHITE,COLOR_BLACK,1);
	 addstr("Choose squad member to replace ");

	 if (swap == NULL) {
            refresh();

            int c=getch();
            translategetch(c);

            if(c==10)break;

            if(c<'a'||c>'s')
            {
               // Not within correct range
               break;
            }

            // Get first member to swap
	    int p=page*19+(int)(c-'a');

            if(p<temppool.size())
            {
               swap=temppool[p];
	       swapPos = p;
            }
	 }
	 else { // non-null swap
            addstr(swap->name);     
            addstr(" with");

            c=getch();
            translategetch(c);

            if(c==10)break;

            if(c<'a'||c>'s')
            {
               // Not within correct range
               break;
            }

	    Creature *swap2 = NULL;

	    int p=page*19+(int)(c-'a');

            if(p<temppool.size() && temppool[p] != swap)
            {
               swap2=temppool[p];

	       for (int i = 0; i < pool.size(); i++) {
		  if (pool[i]->id == swap->id) {
		     pool.erase(pool.begin() + i);
		     break;
		  }
	       }

	       for (int i = 0; i < pool.size(); i++) {
		  if (pool[i]->id == swap2->id) {
		     pool.insert (pool.begin() + i + (swapPos < p ? 1 : 0), swap);
		     break;
		  }
	       }

	       temppool.erase (temppool.begin() + swapPos);
	       temppool.insert (temppool.begin() + p, swap);

	       swap = NULL;
	    }
	 }
      }

      if(c==10)break;
   }while(1);
}



/* base - review - assemble a squad */
void assemblesquad(squadst *cursquad)
{
   int culloc=-1;
   int p;
   if(cursquad!=NULL)
   {
      culloc=cursquad->squad[0]->location;
   }

   char newsquad=0;
   if(cursquad==NULL)
   {
      cursquad=new squadst;
      cursquad->id=cursquadid;
      cursquadid++;
      newsquad=1;
   }

   vector<Creature *> temppool;
   for(p=0;p<pool.size();p++)
   {
      if(pool[p]->alive==1&&
         pool[p]->align==1&&
         pool[p]->clinic==0&&
         pool[p]->dating==0&&
         pool[p]->hiding==0&&
         !(pool[p]->flag & CREATUREFLAG_SLEEPER))
      {
         if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
            location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
            location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON&&
            (pool[p]->location==culloc||culloc==-1))temppool.push_back(pool[p]);
      }
   }

   //BUILD LIST OF BASES FOR EACH SQUAD IN CASE IT ENDS UP EMPTY
   //THEN WILL DROP ITS LOOT THERE
   vector<int> squadloc;
   squadloc.resize(squad.size());
   for(int sl=0;sl<squad.size();sl++)
   {
      squadloc[sl]=squad[sl]->squad[0]->location;
      if(squadloc[sl]!=-1)
      {
         if(location[squadloc[sl]]->renting==RENTING_NOCONTROL)
         {
            squadloc[sl]=-1;
         }
      }
   }

   int page=0;

   int squadsize;

   char num[20];

   do
   {
      squadsize=0;
      for(p=0;p<6;p++)if(cursquad->squad[p]!=NULL)squadsize++;

      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      if(squadsize<6)addstr("Assemble the squad!");
      else addstr("The squad is full.");

      if(newsquad)
      {
         move(0,71);
         addstr("New Squad");
      }
      else
      {
         move(0,73-strlen(cursquad->name));
         addstr("Squad: ");
         addstr(cursquad->name);
      }

      move(1,0);
      addstr("----CODE NAME------------SKILL---HEALTH-----------PROFESSION--------------------");

      int y=2;
      for(p=page*19;p<temppool.size()&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         char bright=0;
         int skill=0;
         for(int sk=0;sk<SKILLNUM;sk++)
         {
            skill+=(int)temppool[p]->get_skill(sk);
            if(temppool[p]->get_skill_ip(sk)>=100+(10*temppool[p]->get_skill(sk))&&
               temppool[p]->get_skill(sk)<temppool[p]->skill_cap(sk,true))bright=1;
         }

         set_color(COLOR_WHITE,COLOR_BLACK,bright);

         move(y,25);
         itoa(skill,num,10);
         addstr(num);

         printhealthstat(*temppool[p],y,33,FALSE);

         if(temppool[p]->squadid==cursquad->id)
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            move(y,75);
            addstr("SQUAD");
         }
         else if(temppool[p]->squadid!=-1)
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,0);
            move(y,75);
            addstr("SQUAD");
         }
         else if(cursquad->squad[0]!=NULL)
         {
            if(cursquad->squad[0]->location!=temppool[p]->location)
            {
               set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(y,75);
               addstr("AWAY");
            }
         }

         char str[200];
         if(temppool[p]->align==-1)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(temppool[p]->align==0)set_color(COLOR_WHITE,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(y,50);
         getrecruitcreature(str,temppool[p]->type);
         addstr(str);
         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to add or remove a Liberal from the squad.");
      move(23,0);
      addpagestr();
      move(24,0);
      if(squadsize>0)addstr("Enter - The squad is ready.");
      else addstr("Enter - I need no squad!");
      if(squadsize>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(24,40);
      addstr("9 - Dissolve the squad.");

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<temppool.size())page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<temppool.size())
         {
            char conf=1;
            if(cursquad->squad[0]!=NULL)
            {
               if(cursquad->squad[0]->location!=temppool[p]->location)
               {
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  move(22,0);
                  addstr("                                                                               ");
                  move(23,0);
                  addstr("           Liberals must be in the same location to form a Squad.              ");
                  move(24,0);
                  addstr("                                                                               ");
                  refresh();
                  getch();

                  conf=0;
               }
            }
            if(!temppool[p]->canwalk()&&
               !(temppool[p]->flag & CREATUREFLAG_WHEELCHAIR))
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               move(22,0);
               addstr("                                                                              ");
               move(23,0);
               addstr("                Squad Liberals must be able to move around.                   ");
               move(24,0);
               addstr("                  Have this Liberal procure a wheelchair.                     ");
               refresh();
               getch();

               conf=0;
            }
            if(conf)
            {
               if(temppool[p]->squadid==cursquad->id)
               {
                  char flipstart=0;
                  for(int pt=0;pt<6;pt++)
                  {
                     if(cursquad->squad[pt]==temppool[p])
                     {
                        flipstart=1;
                        cursquad->squad[pt]->squadid=-1;
                     }
                     if(flipstart&&pt<5)cursquad->squad[pt]=cursquad->squad[pt+1];
                  }
                  if(flipstart)cursquad->squad[5]=NULL;
               }
               else if(squadsize<6)
               {
                  for(int pt=0;pt<6;pt++)
                  {
                     if(cursquad->squad[pt]==NULL)
                     {
                        removesquadinfo(*temppool[p]);
                        cursquad->squad[pt]=temppool[p];
                        temppool[p]->squadid=cursquad->id;
                        break;
                     }
                  }
               }
            }
         }
      }
      if(c==10)
      {
         //CHECK IF GOOD
         char good=1;
         char care=0;

         for(int p=0;p<6;p++)
         {
            if(cursquad->squad[p]!=NULL)
            {
               if(cursquad->squad[p]->align==1)
               {
                  care=1;
                  break;
               }
               else good=0;
            }
         }

         if(good||care)break;
         else
         {
            set_color(COLOR_RED,COLOR_BLACK,1);

            move(22,0);
            addstr("                                                                               ");
            move(23,0);
            addstr("You cannot form a Squad with only Conservatives!                               ");
            move(24,0);
            addstr("                                                                               ");

            refresh();
            getch();
         }
      }
      if(c=='9')
      {
         for(int p=0;p<6;p++)
         {
            if(cursquad->squad[p]!=NULL)
            {
               cursquad->squad[p]->squadid=-1;
               cursquad->squad[p]=NULL;
            }
         }
      }
   }while(1);

   //FINALIZE NEW SQUADS
   char hasmembers=0;

   for(p=0;p<6;p++)
   {
      if(cursquad->squad[p]!=NULL)
      {
         hasmembers=1;
         break;
      }
   }

   if(newsquad)
   {
      if(hasmembers)
      {
         move(22,0);
         addstr("                                                                               ");
         move(23,0);
         addstr("What shall we designate this Liberal squad?                                    ");
         move(24,0);
         addstr("                                                                               ");

         keypad(stdscr,FALSE);
         raw_output(FALSE);
         echo();
         curs_set(1);
         mvgetstr(24,0,cursquad->name);
         curs_set(0);
         noecho();
         raw_output(TRUE);
         keypad(stdscr,TRUE);

         squad.push_back(cursquad);
      }
      else delete cursquad;
   }

   //NUKE ALL EMPTY SQUADS
   for(int sq=squad.size()-1;sq>=0;sq--)
   {
      hasmembers=0;

      for(int p=0;p<6;p++)
      {
         if(squad[sq]->squad[p]!=NULL)
         {
            hasmembers=1;
            break;
         }
      }

      if(!hasmembers && mode==GAMEMODE_BASE)
      {
         if(squadloc[sq]!=-1)
         {
            for(int l=0;l<squad[sq]->loot.size();l++)
            {
               location[squadloc[sq]]->loot.push_back(squad[sq]->loot[l]);
            }
            squad[sq]->loot.clear();
         }

         if(activesquad==squad[sq])activesquad=NULL;
         delete squad[sq];
         squad.erase(squad.begin() + sq);
      }
   }
}



/* base - review - assign new bases to the squadless */
void squadlessbaseassign(void)
{
   int p = 0;
   int l = 0;
   vector<Creature *> temppool;
   for(p=0;p<pool.size();p++)
   {
      if(pool[p]->alive&&
         pool[p]->align==1&&
         pool[p]->clinic==0&&
         pool[p]->dating==0&&
         pool[p]->hiding==0&&
         pool[p]->squadid==-1&&
         !(pool[p]->flag & CREATUREFLAG_SLEEPER))
      {
         if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
            location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
            location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON)
         {
            temppool.push_back(pool[p]);
         }
      }
   }

   if(temppool.size()==0)return;

   vector<int> temploc;
   for(l=0;l<location.size();l++)
   {
      if(location[l]->renting>=0&&!location[l]->siege.siege)temploc.push_back(l);
   }
   if(temploc.size()==0)return;

   int page_lib=0;
   int page_loc=0;

   int selectedbase=0;

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      printfunds(0,1,"Money: ");

      move(0,0);
      addstr("New Bases for Squadless Liberals");
      move(1,0);
      addstr("----CODE NAME------------CURRENT BASE-------------------------------------------");
      move(1,51);
      addstr("NEW BASE");

      int y=2;
      for(p=page_lib*19;p<temppool.size()&&p<page_lib*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         move(y,25);
         addshortname(location[temppool[p]->base]);

         y++;
      }

      y=2;
      for(p=page_loc*9;p<temploc.size()&&p<page_loc*9+9;p++)
      {
         if(p==selectedbase)set_color(COLOR_WHITE,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,51);
         addch(y+'1'-2);addstr(" - ");
         addshortname(location[temploc[p]]);

         y++;
      }


      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to assign a Base.  Press a Number to select a Base.");
      if(temppool.size()>19)
      {
         move(23,0);
         addpagestr();
      }
      if(temploc.size()>9)
      {
         move(24,0);
         addstr(",. to view other Base pages.");
      }

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page_lib>0)page_lib--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page_lib+1)*19<temppool.size())page_lib++;

      //PAGE UP
      if(c==','&&page_loc>0)page_loc--;
      //PAGE DOWN
      if(c=='.'&&(page_loc+1)*9<temploc.size())page_loc++;

      if(c>='a'&&c<='s')
      {
         int p=page_lib*19+(int)(c-'a');
         if(p<temppool.size())
         {
            temppool[p]->base=temploc[selectedbase];
         }
      }
      if(c>='1'&&c<='9')
      {
         int p=page_loc*9+(int)(c-'1');
         if(p<temploc.size())
         {
            selectedbase=p;
         }
      }

      if(c==10)break;
   }while(1);
}

// prints a formatted name, used by promoteliberals
static void printname(Creature &cr)
{
   int bracketcolor=-1;
   int namecolor;
   int brightness;

   if(cr.hiding)
      bracketcolor=COLOR_BLACK;

   // Determine bracket color, if any, based on location
   if(cr.location!=-1)
   {
      switch(location[cr.location]->type)
      {
      case SITE_GOVERNMENT_POLICESTATION:
      case SITE_GOVERNMENT_COURTHOUSE:
         if(!(cr.flag & CREATUREFLAG_SLEEPER))
            bracketcolor=COLOR_YELLOW;
         break;
      case SITE_GOVERNMENT_PRISON:
         if(!(cr.flag & CREATUREFLAG_SLEEPER))
            bracketcolor=COLOR_RED;
         break;
      default:
         break;
      }
   }

   // Determine name color, based on recruitment style
   if(cr.flag & CREATUREFLAG_LOVESLAVE)
      namecolor=COLOR_MAGENTA;
   else if(cr.flag & CREATUREFLAG_BRAINWASHED)
      namecolor=COLOR_YELLOW;
   else
      namecolor=COLOR_WHITE;

   // Determine name brightness, based on subordinates left
   /*if(subordinatesleft(cr))
   brightness=1;
   else*/
   brightness=0;

   // add bracket (if used)
   if(bracketcolor!=-1)
   {
      set_color(bracketcolor,COLOR_BLACK,1);
      addstr("[");
   }

   if(cr.flag & CREATUREFLAG_SLEEPER)
   {
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      addstr("[");
   }

   // add name
   set_color(namecolor,COLOR_BLACK,brightness);
   addstr(cr.name);

   // add close bracket (if used)
   if(cr.flag & CREATUREFLAG_SLEEPER)
   {
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      addstr("]");
   }
   if(bracketcolor!=-1)
   {
      set_color(bracketcolor,COLOR_BLACK,1);
      addstr("]");
   }

   set_color(COLOR_WHITE,COLOR_BLACK,0);
}

/* base - review - promote liberals */
void promoteliberals(void)
{
   const static int PAGELENGTH=19;
   vector<Creature *> temppool;
   vector<int> level;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->alive&&
         pool[p]->align==1)
      {
         temppool.push_back(pool[p]);
      }
   }

   if(temppool.size()==0)return;

   //SORT
   sortbyhire(temppool,level);

   //PROMOTE
   int page=0;

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      printfunds(0,1,"Money: ");

      move(0,0);
      addstr("Promote the Elite Liberals");
      move(1,0);
      addstr("----CODE NAME--------------CURRENT CONTACT--------------------------------------");
      move(1,54);
      addstr("CONTACT AFTER PROMOTION");

      int y=2;

      for(int p=page*PAGELENGTH;p<temppool.size()&&p<page*PAGELENGTH+PAGELENGTH;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");

         move(y,27);
         int p2 = 0;


         for(p2=0;p2<pool.size();p2++)
         {
            int p3 = 0;
            if(pool[p2]->alive==1&&pool[p2]->id==temppool[p]->hireid)
            {
               printname(*pool[p2]);

               move(y,54);
               for(p3=0;p3<pool.size();p3++)
               {
                  if(pool[p3]->alive==1&&pool[p3]->id==pool[p2]->hireid)
                  {
                     if(temppool[p]->flag&CREATUREFLAG_LOVESLAVE)
                        addstr("<Refuses Promotion>");
                     else if(!subordinatesleft(*pool[p3])&&!(temppool[p]->flag&CREATUREFLAG_BRAINWASHED))
                        addstr("<Can't Lead More>");
                     else
                        printname(*pool[p3]);
                     break;
                  }
               }

               break;
            }
         }
         if(p2==pool.size())addstr("<LCS Leader>");

         move(y,4+level[p]);
         printname(*temppool[p]);

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(21,0);
      addstr("Recruited/");
      set_color(COLOR_MAGENTA,COLOR_BLACK,0);
      addstr("Seduced");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("/");
      set_color(COLOR_YELLOW,COLOR_BLACK,0);
      addstr("Enlightened");
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      addstr("   [");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("Arrested");
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      addstr("]");
      set_color(COLOR_RED,COLOR_BLACK,1);
      addstr(" [");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("In Jail");
      set_color(COLOR_RED,COLOR_BLACK,1);
      addstr("]");
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr(" [");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("In Hiding");
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr("]");
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      addstr(" [");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("Sleeper");
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      addstr("]");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to Promote a Liberal. You can not Promote Liberals in Hiding.");
      move(23,0);
      addstr("Enlightened liberals follow anyone. Seduced liberals follow only their lover.");
      if(temppool.size()>PAGELENGTH)
      {
         move(24,0);
         addpagestr();
      }

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*PAGELENGTH<temppool.size())page++;

      if(c>='a'&&c<='a'+PAGELENGTH)
      {
         int p=page*PAGELENGTH+(int)(c-'a');
         // *JDS* can't promote liberals in hiding OR loveslaves
         if(p<temppool.size()&&!temppool[p]->hiding&&!(temppool[p]->flag&CREATUREFLAG_LOVESLAVE))
         {
            for(int p2=0;p2<pool.size();p2++)
            {
               if(pool[p2]->alive==1&&pool[p2]->id==temppool[p]->hireid)
               {
                  addstr(pool[p2]->name);

                  for(int p3=0;p3<pool.size();p3++)
                  {
                     // Can't promote if new boss can't accept more subordinates
                     if(pool[p3]->alive==1&&pool[p3]->id==pool[p2]->hireid&&
                        (temppool[p]->flag&CREATUREFLAG_BRAINWASHED||subordinatesleft(*pool[p3])))
                     {
                        temppool[p]->hireid=pool[p2]->hireid;
                        sortbyhire(temppool,level);
                        break;
                     }
                  }
                  break;
               }
            }
         }
      }

      if(c==10)break;
   }while(1);
}



void sortbyhire(vector<Creature *> &temppool,vector<int> &level)
{
   vector<Creature *> newpool;
   level.clear();

   for(int i=temppool.size()-1;i>=0;i--)
   {
      if(temppool[i]->hireid==-1)
      {
         newpool.insert(newpool.begin(),temppool[i]);
         level.insert(level.begin(),0);
         temppool.erase(temppool.begin() + i);
      }
   }

   char changed;

   do
   {
      changed=0;

      for(int i=0;i<newpool.size();i++)
      {
         for(int j=temppool.size()-1;j>=0;j--)
         {
            if(temppool[j]->hireid==newpool[i]->id)
            {
               newpool.insert(newpool.begin()+i+1,temppool[j]);
               level.insert(level.begin()+i+1,level[i]+1);
               temppool.erase(temppool.begin() + j);
               changed=1;
            }
         }
      }
   }while(changed);

   temppool.clear();
   for(int p=0;p<newpool.size();p++)
   {
      temppool.push_back(newpool[p]);
   }
}
