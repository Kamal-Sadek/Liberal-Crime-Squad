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



/* politics - causes the people to vote (presidential, congressional, propositions) */
void elections(char clearformess,char canseethings)
{
   char num[20];
   int l, p, c;

   if(canseethings)
   {
      if(clearformess)
      {
         erase();
      }
      else
      {
         makedelimiter(8,0);
      }
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr("The Elections are being held today!");

      refresh();
      getch();
   }

   int mood=publicmood(-1);

   //PRESIDENTIAL
   if(year%4==0)
   {
      if(canseethings)
      {
         erase();

         set_color(COLOR_WHITE,COLOR_BLACK,1);

         move(0,0);
         addstr("Presidential Election ");
         itoa(year,num,10);
         addstr(num);
      }

      char candidate[5][80];
      int votes[5]={0,0,0,0,0};
      int oldwinnerspot=-1;
      for(c=0;c<5;c++)
      {
         if(c==0)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(c==1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(c==2)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(c==3)set_color(COLOR_BLUE,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);

         if(execterm==1&&c==exec[EXEC_PRESIDENT]+2)
         {
            oldwinnerspot=c;
            strcpy(candidate[c],execname[EXEC_PRESIDENT]);
         }
         else name(candidate[c]);

         if(canseethings)
         {
            move(6-c,0);
            addstr(candidate[c]);
            if(c==oldwinnerspot)addstr(", Incumbent");
         }
      }

      if(canseethings)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(8,0);
         addstr("Press any key to watch the election unfold.");

         refresh();
         getch();

         nodelay(stdscr,TRUE);
      }

      int winner=-1;
      char recount=0;
      char oldwinner=0;
      int vote;
      for(int l=0;l<100;l++)
      {
         vote=0;

         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;

         votes[vote]++;

         if(l==99)
         {
            int maxvote=0;

            for(c=0;c<5;c++)
            {
               if(votes[c]>maxvote)maxvote=votes[c];
            }

            vector<int> eligible;

            for(c=0;c<5;c++)
            {
               if(votes[c]==maxvote)
               {
                  eligible.push_back(c);
               }
            }

            if(eligible.size()>1)
            {
               winner=eligible[LCSrandom(eligible.size())];
               recount=1;
            }
            else winner=eligible[0];
         }

         if(canseethings)
         {
            for(int c=0;c<5;c++)
            {
               if(winner>=0&&c!=winner)set_color(COLOR_BLACK,COLOR_BLACK,1);
               else if(c==winner)set_color(COLOR_WHITE,COLOR_BLACK,1);
               else set_color(COLOR_WHITE,COLOR_BLACK,0);
               move(6-c,40);
               itoa(votes[c],num,10);
               addstr(num);
               addch('%');
               if(c==winner&&recount)addstr(" (After Recount)");
            }

            refresh();

            pause_ms(50);
            getch();
         }
      }

      if(canseethings)nodelay(stdscr,FALSE);

      if(winner==oldwinnerspot)oldwinner=1;

      if(canseethings)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(8,0);
         addstr("Press any key to continue the elections.   ");

         refresh();
         getch();
      }

      //CONSTRUCT EXECUTIVE BRANCH
      if(oldwinner)
      {
         execterm=2;
      }
      else
      {
         execterm=1;
         exec[EXEC_PRESIDENT]=winner-2;
         strcpy(execname[EXEC_PRESIDENT],candidate[winner]);

         for(int e=0;e<EXECNUM;e++)
         {
            if(e==EXEC_PRESIDENT)continue;
            if(winner-2==-2)exec[e]=-2;
            else if(winner-2==2)exec[e]=2;
            else exec[e]=winner+LCSrandom(3)-3;
            name(execname[e]);
         }
      }
   }

   //SENATE
   if(year%2==0)
   {
      int senmod=-1;
      if(year%6==0)
      {
         senmod=0;
      }
      if(year%6==2)
      {
         senmod=1;
      }
      if(year%6==4)
      {
         senmod=2;
      }
         
      if(canseethings)
      {
         erase();

         set_color(COLOR_WHITE,COLOR_BLACK,1);

         move(0,0);
         addstr("Senate Elections ");
         itoa(year,num,10);
         addstr(num);
      }

      int x=0,y=2, s=0;

      for(s=0;s<100;s++)
      {
         if(s%3!=senmod)continue;

         if(canseethings)
         {
            move(y,x);

            if(senate[s]==-2)
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               addstr("Arch-Conservative");
            }
            else if(senate[s]==-1)
            {
               set_color(COLOR_MAGENTA,COLOR_BLACK,1);
               addstr("Conservative");
            }
            else if(senate[s]==0)
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               addstr("moderate");
            }
            else if(senate[s]==1)
            {
               set_color(COLOR_BLUE,COLOR_BLACK,1);
               addstr("Liberal");
            }
            else
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               addstr("Elite Liberal");
            }
         }

         x+=20;
         if(x>70)
         {
            x=0;
            y++;
         }
      }

      if(canseethings)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(20,0);
         addstr("Press any key to watch the elections unfold.");

         refresh();
         getch();

         nodelay(stdscr,TRUE);
      }

      int vote;

      x=0;
      y=2;

      for(s=0;s<100;s++)
      {
         if(s%3!=senmod)continue;

         vote=0;

         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;

         senate[s]=vote-2;

         if(canseethings)
         {
            move(y,x);

            if(senate[s]==-2)
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               addstr("Arch-Conservative");
            }
            else if(senate[s]==-1)
            {
               set_color(COLOR_MAGENTA,COLOR_BLACK,1);
               addstr("Conservative         ");
            }
            else if(senate[s]==0)
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               addstr("moderate               ");
            }
            else if(senate[s]==1)
            {
               set_color(COLOR_BLUE,COLOR_BLACK,1);
               addstr("Liberal                ");
            }
            else
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               addstr("Elite Liberal       ");
            }
         }

         x+=20;
         if(x>70)
         {
            x=0;
            y++;
         }

         if(canseethings)
         {
            refresh();
            pause_ms(50);

            getch();
         }
      }

      if(canseethings)
      {
         nodelay(stdscr,FALSE);

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(20,0);
         addstr("Press any key to continue the elections.   ");

         refresh();
         getch();
      }
   }

   //HOUSE
   if(year%2==0)
   {
      if(canseethings)
      {
         erase();

         set_color(COLOR_WHITE,COLOR_BLACK,1);

         move(0,0);
         addstr("House Elections ");
         itoa(year,num,10);
         addstr(num);
      }

      int x=0,y=2, h=0;

      for(h=0;h<435;h++)
      {
         if(canseethings)
         {
            move(y,x);

            if(house[h]==-2)
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               addstr("C+");
            }
            else if(house[h]==-1)
            {
               set_color(COLOR_MAGENTA,COLOR_BLACK,1);
               addstr("C ");
            }
            else if(house[h]==0)
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               addstr("m ");
            }
            else if(house[h]==1)
            {
               set_color(COLOR_BLUE,COLOR_BLACK,1);
               addstr("L ");
            }
            else
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               addstr("L+");
            }
         }

         x+=3;
         if(x>78)
         {
            x=0;
            y++;
         }
      }

      if(canseethings)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(20,0);
         addstr("Press any key to watch the elections unfold.");

         refresh();
         getch();

         nodelay(stdscr,TRUE);
      }

      int vote;

      x=0;
      y=2;

      for(h=0;h<435;h++)
      {
         vote=0;

         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;

         house[h]=vote-2;

         if(canseethings)
         {
            move(y,x);

            if(house[h]==-2)
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               addstr("C+");
            }
            else if(house[h]==-1)
            {
               set_color(COLOR_MAGENTA,COLOR_BLACK,1);
               addstr("C ");
            }
            else if(house[h]==0)
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               addstr("m ");
            }
            else if(house[h]==1)
            {
               set_color(COLOR_BLUE,COLOR_BLACK,1);
               addstr("L ");
            }
            else
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               addstr("L+");
            }
         }

         x+=3;
         if(x>78)
         {
            x=0;
            y++;
         }

         if(canseethings)
         {
            refresh();

            pause_ms(10);

            getch();
         }
      }

      if(canseethings)
      {
         nodelay(stdscr,FALSE);

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(20,0);
         addstr("Press any key to continue the elections.    ");

         refresh();
         getch();
      }
   }

   //PROPOSITIONS
   if(canseethings)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);

      move(0,0);
      addstr("Important Propositions ");
      itoa(year,num,10);
      addstr(num);
   }

   vector<int> prop;
   vector<int> propdir;
   int pnum=LCSrandom(5)+2;
   char lawtaken[LAWNUM];
   memset(lawtaken,0,LAWNUM*sizeof(char));

   char lawpriority[LAWNUM];
   memset(lawpriority,0,LAWNUM*sizeof(char));
   char lawdir[LAWNUM];
   memset(lawdir,0,LAWNUM*sizeof(char));
   //DETERMINE PROPS
   int pmood,pvote;
   for(l=0;l<LAWNUM;l++)
   {
      pmood=publicmood(l);
      pvote=0;
      if(LCSrandom(100)<pmood)pvote++;
      if(LCSrandom(100)<pmood)pvote++;
      if(LCSrandom(100)<pmood)pvote++;
      if(LCSrandom(100)<pmood)pvote++;
      pvote-=2;

      if(law[l]<pvote)lawdir[l]=1;
      if(law[l]>=pvote)lawdir[l]=-1;
      if(law[l]==-2)lawdir[l]=1;
      if(law[l]==2)lawdir[l]=-1;

      //CALC PRIORITY
      if(law[l]==-2)pvote=0;
      else if(law[l]==-1)pvote=25;
      else if(law[l]==0)pvote=50;
      else if(law[l]==1)pvote=75;
      else pvote=100;

      lawpriority[l]=abs(pvote-pmood)+LCSrandom(10);
   }

   vector<int> canlaw;

   prop.resize(pnum);
   propdir.resize(pnum);
   for(p=0;p<pnum;p++)
   {
      int maxprior=0;
      for(l=0;l<LAWNUM;l++)
      {
         if(lawpriority[l]>maxprior&&!lawtaken[l])maxprior=lawpriority[l];
      }
      canlaw.clear();
      for(l=0;l<LAWNUM;l++)
      {
         if(lawpriority[l]==maxprior&&!lawtaken[l])
         {
            canlaw.push_back(l);
         }
      }

      prop[p]=canlaw[LCSrandom(canlaw.size())];

      lawtaken[prop[p]]=1;

      propdir[p]=lawdir[prop[p]];

      if(canseethings)
      {
         move(p*3+2,0);
         int propnum=0;
         switch(p)
         {
            case 0:propnum=2*(17-LCSrandom(2)*6)*(19-LCSrandom(2)*6);break;
            case 1:propnum=7*(17-LCSrandom(2)*6)*(19-LCSrandom(2)*6);break;
            case 2:propnum=3*(17-LCSrandom(2)*6)*(19-LCSrandom(2)*6);break;
            case 3:propnum=5*(17-LCSrandom(2)*6)*(2-LCSrandom(2)*1);break;
            case 4:propnum=11*(17-LCSrandom(2)*6)*(2-LCSrandom(2)*1);break;
            case 5:propnum=13*(17-LCSrandom(2)*6)*(2-LCSrandom(2)*1);break;
         }
         itoa(propnum,num,10);
         addstr("Proposition ");addstr(num);addstr(":");
         move(p*3+2,18);
         addstr("To ");
         switch(prop[p])
         {
            case LAW_ABORTION:
               if(propdir[p]==1)addstr("Strengthen Abortion Rights");
               else addstr("Protect the Unborn Child");
               break;
            case LAW_ANIMALRESEARCH:
               if(propdir[p]==1)addstr("Limit Animal Cruelty");
               else addstr("Expand Animal Research");
               break;
            case LAW_POLICEBEHAVIOR:
               if(propdir[p]==1)addstr("Curtail Police Misconduct");
               else addstr("Stop Harassment of Police Officers");
               break;
            case LAW_PRIVACY:
               if(propdir[p]==1)addstr("Enhance Privacy Protection");
               else addstr("Allow Corporations Access to Information");
               break;
            case LAW_DEATHPENALTY:
               if(propdir[p]==1)addstr("Limit the Death Penalty");
               else addstr("Expand Capital Punishment");
               break;
            case LAW_NUCLEARPOWER:
               if(propdir[p]==1)addstr("Limit Nuclear Power");
               else addstr("Expand Nuclear Power");
               break;
            case LAW_POLLUTION:
               if(propdir[p]==1)addstr("Punish Polluters");
               else addstr("Reward Industry");
               break;
            case LAW_LABOR:
               if(propdir[p]==1)addstr("Enhance Labor Standards");
               else addstr("End Undue Union Influence");
               break;
            case LAW_GAY:
               if(propdir[p]==1)addstr("Expand Homosexual Rights");
               else addstr("Support the Sanctity of Marriage");
               break;
            case LAW_CORPORATE:
               if(propdir[p]==1)addstr("Stop Corporate Criminals");
               else addstr("Reward Small Businesses");
               break;
            case LAW_FREESPEECH:
               if(propdir[p]==1)addstr("Protect Free Speech");
               else addstr("Limit Hurtful Speech");
               break;
            case LAW_TAX:
					if(propdir[p]==1)addstr("Punish the Wealthy");
					else addstr("Stimulate Economic Growth");
					break;
            case LAW_FLAGBURNING:
               if(propdir[p]==1)addstr("Limit Prohibitions on Flag Burning");
               else addstr("Protect the Symbol of Our Nation");
               break;
            case LAW_GUNCONTROL:
               if(propdir[p]==1)addstr("Prevent Gun Violence");
               else addstr("Assert our Second Amendment Rights");
               break;
         }

         refresh();
      }
   }

   if(canseethings)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(20,0);
      addstr("Press any key to watch the elections unfold.");

      refresh();
      getch();

      nodelay(stdscr,TRUE);
   }

   for(p=0;p<pnum;p++)
   {
      char yeswin=0;
      int yesvotes=0;
      char recount=0;
      int vote;
      mood=publicmood(prop[p]);
      for(int l=0;l<100;l++)
      {
         vote=0;
         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;
         vote-=2;

         if(law[prop[p]]>vote && propdir[p]==-1)yesvotes++;
         if(law[prop[p]]<vote && propdir[p]==1)yesvotes++;

         if(l==99)
         {
            if(yesvotes>50)yeswin=1;
            else if(yesvotes==50)
            {
               if(!LCSrandom(2))yeswin=1;
               recount=1;
            }
         }

         if(canseethings)
         {
            if(l==99&&yeswin)set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(l==99)set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(p*3+2,72);
            itoa(yesvotes,num,10);
            addstr(num);
            addstr("% Yes");

            if(l==99&&!yeswin)set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(l==99)set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(p*3+3,72);
            itoa(l+1-yesvotes,num,10);
            addstr(num);
            addstr("% No");

            refresh();

            pause_ms(20);
            getch();
         }
      }

      if(canseethings)
      {
         if(recount)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(p*3+3,0);
            addstr("A Recount was Necessary");
         }
      }

      if(yeswin)law[prop[p]]+=propdir[p];
   }

   if(canseethings)
   {
      nodelay(stdscr,FALSE);

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(20,0);
      addstr("Press any key to reflect on what has happened.");

      refresh();
      getch();
   }
}



/* politics - causes the supreme court to hand down decisions */
void supremecourt(char clearformess,char canseethings)
{
   int c;
   if(canseethings)
   {
      if(clearformess)
      {
         erase();
      }
      else
      {
         makedelimiter(8,0);
      }
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr("The Supreme court is handing down decisions!");

      refresh();
      getch();
   }

   //CHANGE THINGS AND REPORT
   char num[20];
   if(canseethings)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);

      move(0,0);
      addstr("Supreme Court Watch ");
      itoa(year,num,10);
      addstr(num);
   }

   vector<int> scase;
   vector<int> scasedir;
   int cnum=LCSrandom(5)+2;
   char lawtaken[LAWNUM];
   memset(lawtaken,0,LAWNUM*sizeof(char));

   scase.resize(cnum);
   scasedir.resize(cnum);
   for(c=0;c<cnum;c++)
   {
      do
      {
         scase[c]=LCSrandom(LAWNUM);
      }while(lawtaken[scase[c]]);

      lawtaken[scase[c]]=1;

      if(law[scase[c]]==2)scasedir[c]=-1;
      else if(law[scase[c]]==-2)scasedir[c]=1;
      else
      {
         if(!LCSrandom(2))scasedir[c]=1;
         else scasedir[c]=-1;
      }

      if(canseethings)
      {
         move(c*3+2,0);
         char name1[80];
         char name2[80];
         if(!LCSrandom(3))strcpy(name1,"The People");
         else lastname(name1);
         addstr(name1);
         addstr(" vs. ");
         lastname(name2);
         addstr(name2);

         move(c*3+3,0);
         addstr("A Decision could ");
         switch(scase[c])
         {
            case LAW_ABORTION:
               if(scasedir[c]==1)addstr("Strengthen Abortion Rights");
               else addstr("Protect the Unborn Child");
               break;
            case LAW_ANIMALRESEARCH:
               if(scasedir[c]==1)addstr("Limit Animal Cruelty");
               else addstr("Expand Animal Research");
               break;
            case LAW_POLICEBEHAVIOR:
               if(scasedir[c]==1)addstr("Curtail Police Misconduct");
               else addstr("Stop Harassment of Police Officers");
               break;
            case LAW_PRIVACY:
               if(scasedir[c]==1)addstr("Enhance Privacy Protection");
               else addstr("Allow Corporations Access to Information");
               break;
            case LAW_DEATHPENALTY:
               if(scasedir[c]==1)addstr("Limit the Death Penalty");
               else addstr("Expand Capital Punishment");
               break;
            case LAW_NUCLEARPOWER:
               if(scasedir[c]==1)addstr("Limit Nuclear Power");
               else addstr("Expand Nuclear Power");
               break;
            case LAW_POLLUTION:
               if(scasedir[c]==1)addstr("Punish Polluters");
               else addstr("Reward Industry");
               break;
            case LAW_LABOR:
               if(scasedir[c]==1)addstr("Enhance Labor Standards");
               else addstr("End Undue Union Influence");
               break;
            case LAW_GAY:
               if(scasedir[c]==1)addstr("Expand Homosexual Rights");
               else addstr("Support the Sanctity of Marriage");
               break;
            case LAW_CORPORATE:
               if(scasedir[c]==1)addstr("Stop Corporate Criminals");
               else addstr("Reward Small Businesses");
               break;
            case LAW_FREESPEECH:
               if(scasedir[c]==1)addstr("Protect Free Speech");
               else addstr("Limit Hurtful Speech");
               break;
            case LAW_TAX:
					if(scasedir[c]==1)addstr("Punish the Wealthy");
					else addstr("Stimulate Economic Growth");
					break;
            case LAW_FLAGBURNING:
               if(scasedir[c]==1)addstr("Limit Prohibitions on Flag Burning");
               else addstr("Protect the Symbol of Our Nation");
               break;
            case LAW_GUNCONTROL:
               if(scasedir[c]==1)addstr("Prevent Gun Violence");
               else addstr("Protect our Second Amendment Rights");
               break;
         }

         refresh();
      }
   }

   if(canseethings)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(20,0);
      addstr("Press any key to watch the decisions unfold.");

      refresh();
      getch();

      nodelay(stdscr,TRUE);
   }

   for(c=0;c<cnum;c++)
   {
      char yeswin=0;
      int yesvotes=0;

      int vote;

      for(int l=0;l<9;l++)
      {
         vote=court[l];
         if(vote>=-1&&vote<=1)vote+=LCSrandom(3)-1;

         if(law[scase[c]]>vote && scasedir[c]==-1)yesvotes++;
         if(law[scase[c]]<vote && scasedir[c]==1)yesvotes++;

         if(l==8)
         {
            if(yesvotes>=5)yeswin=1;
         }

         if(canseethings)
         {
            if(l==8&&yeswin)set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(l==8)set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(c*3+2,63);
            itoa(yesvotes,num,10);
            addstr(num);
            addstr(" for Change");

            if(l==8&&!yeswin)set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(l==8)set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(c*3+3,63);
            itoa(l+1-yesvotes,num,10);
            addstr(num);
            addstr(" for Status Quo");

            refresh();

            pause_ms(120);

            getch();
         }
      }

      if(yeswin)law[scase[c]]+=scasedir[c];
   }

   if(canseethings)
   {
      nodelay(stdscr,FALSE);

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(20,0);
      addstr("Press any key to reflect on what has happened.");

      refresh();
      getch();
   }

   //CHANGE A JUSTICE 40% OF THE TIME
   if(LCSrandom(10)>=6)
   {
      if(canseethings)
      {
         erase();

         set_color(COLOR_WHITE,COLOR_BLACK,1);

         move(0,0);
         addstr("Changing the Guard!");
      }

      int j=LCSrandom(9);

      if(canseethings)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(2,0);
         addstr("Justice ");
         addstr(courtname[j]);
         addstr(", ");
         switch(court[j])
         {
            case -2:addstr("Arch-Conservative");break;
            case -1:addstr("Conservative");break;
            case 0:addstr("moderate");break;
            case 1:addstr("Liberal");break;
            case 2:addstr("Elite Liberal");break;
         }
         addstr(", is stepping down.");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(7,0);
         addstr("Press any key to see what happens.");

         refresh();
         getch();
      }

      name(courtname[j]);
      float president=exec[EXEC_PRESIDENT];
      float sen=0;
      for(int s=0;s<100;s++)sen+=senate[s];
      sen/=100.0f;

      float consensus=(president+sen)*.5f;

      if(consensus<-1.5f)court[j]=-2;
      else if(consensus<-.5f)court[j]=-1;
      else if(consensus<.5f)court[j]=0;
      else if(consensus<1.5f)court[j]=1;
      else court[j]=2;

      if(canseethings)
      {
         move(4,0);
         addstr("After much debate and televised testimony, a new justice,");
         move(5,0);
         addstr("the Honorable ");
         addstr(courtname[j]);
         addstr(", ");
         switch(court[j])
         {
            case -2:addstr("Arch-Conservative");break;
            case -1:addstr("Conservative");break;
            case 0:addstr("moderate");break;
            case 1:addstr("Liberal");break;
            case 2:addstr("Elite Liberal");break;
         }
         addstr(", is appointed to the bench.");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(7,0);
         addstr("Press any key to reflect on what has happened.");

         refresh();
         getch();
      }
   }
}



/* politics - causes congress to act on legislation */
void congress(char clearformess,char canseethings)
{
   int l, c;
   if(canseethings)
   {
      if(clearformess)
      {
         erase();
      }
      else
      {
         makedelimiter(8,0);
      }
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr("Congress is acting on legislation!");

      refresh();
      getch();
   }

   //CHANGE THINGS AND REPORT
   char num[20];
   if(canseethings)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);

      move(0,0);
      addstr("Legislative Agenda ");
      itoa(year,num,10);
      addstr(num);
   }

   vector<int> bill;
   vector<int> billdir;
   vector<int> killbill;
   int cnum=LCSrandom(5)+2;
   char lawtaken[LAWNUM];
   memset(lawtaken,0,LAWNUM*sizeof(char));

   int lawpriority[LAWNUM];
   memset(lawpriority,0,LAWNUM*sizeof(int));
   char lawdir[LAWNUM];
   memset(lawdir,0,LAWNUM*sizeof(char));

   //DETERMINE BILLS
   int pup,pdown,pprior;
   for(l=0;l<LAWNUM;l++)
   {
      pup=0;
      pdown=0;
      pprior=0;

      if(!LCSrandom(3))
      {
         for(int cl=0;cl<435;cl++)
         {
            if(law[l]<house[cl])pup=1;
            else if(law[l]>house[cl])pdown=1;
            pprior+=abs(house[cl]-law[l]);
         }
      }
      else if(LCSrandom(2))
      {
         for(int sl=0;sl<100;sl++)
         {
            if(law[l]<senate[sl])pup++;
            else if(law[l]>senate[sl])pdown++;
            pprior+=abs(senate[sl]-law[l]);
         }
      }
      else
      {
         for(int cl=0;cl<435;cl++)
         {
            if(law[l]<house[cl])pup++;
            else if(law[l]>house[cl])pdown++;
            pprior+=abs(house[cl]-law[l]);
         }
         for(int sl=0;sl<100;sl++)
         {
            if(law[l]<senate[sl])pup+=4;
            else if(law[l]>senate[sl])pdown+=4;
            pprior+=abs(senate[sl]-law[l])*4;
         }
      }

      if(pup>pdown)lawdir[l]=1;
      else if(pup==pdown)lawdir[l]=LCSrandom(2)*2-1;
      else lawdir[l]=-1;
      if(law[l]==-2)lawdir[l]=1;
      if(law[l]==2)lawdir[l]=-1;

      //CALC PRIORITY
      lawpriority[l]=pprior;
   }

   vector<int> canlaw;

   bill.resize(cnum);
   billdir.resize(cnum);
   killbill.resize(cnum);
   for(c=0;c<cnum;c++)
   {
      killbill[c]=0;

      int maxprior=0;
      for(l=0;l<LAWNUM;l++)
      {
         if(lawpriority[l]>maxprior&&!lawtaken[l])maxprior=lawpriority[l];
      }
      canlaw.clear();
      for(l=0;l<LAWNUM;l++)
      {
         if(lawpriority[l]==maxprior&&!lawtaken[l])
         {
            canlaw.push_back(l);
         }
      }

      bill[c]=canlaw[LCSrandom(canlaw.size())];

      lawtaken[bill[c]]=1;

      billdir[c]=lawdir[bill[c]];

      if(canseethings)
      {
         move(c*3+2,0);
         addstr("Joint Resolution ");
         itoa(year,num,10);
         addstr(num);
         addstr("-");
         itoa(c+1,num,10);
         addstr(num);

         move(c*3+3,0);
         addstr("To ");
         switch(bill[c])
         {
            case LAW_ABORTION:
               if(billdir[c]==1)addstr("Strengthen Abortion Rights");
               else addstr("Protect the Unborn Child");
               break;
            case LAW_ANIMALRESEARCH:
               if(billdir[c]==1)addstr("Limit Animal Cruelty");
               else addstr("Expand Animal Research");
               break;
            case LAW_POLICEBEHAVIOR:
               if(billdir[c]==1)addstr("Curtail Police Misconduct");
               else addstr("Stop Harassment of Police Officers");
               break;
            case LAW_PRIVACY:
               if(billdir[c]==1)addstr("Enhance Privacy Protection");
               else addstr("Allow Corporations Information Access");
               break;
            case LAW_DEATHPENALTY:
               if(billdir[c]==1)addstr("Limit the Death Penalty");
               else addstr("Expand Capital Punishment");
               break;
            case LAW_NUCLEARPOWER:
               if(billdir[c]==1)addstr("Limit Nuclear Power");
               else addstr("Expand Nuclear Power");
               break;
            case LAW_POLLUTION:
               if(billdir[c]==1)addstr("Punish Polluters");
               else addstr("Reward Industry");
               break;
            case LAW_LABOR:
               if(billdir[c]==1)addstr("Enhance Labor Standards");
               else addstr("End Undue Union Influence");
               break;
            case LAW_GAY:
               if(billdir[c]==1)addstr("Expand Homosexual Rights");
               else addstr("Support the Sanctity of Marriage");
               break;
            case LAW_CORPORATE:
               if(billdir[c]==1)addstr("Stop Corporate Criminals");
               else addstr("Reward Small Businesses");
               break;
            case LAW_FREESPEECH:
               if(billdir[c]==1)addstr("Protect Free Speech");
               else addstr("Limit Hurtful Speech");
               break;
            case LAW_TAX:
					if(billdir[c]==1)addstr("Punish the Wealthy");
					else addstr("Stimulate Economic Growth");
					break;
            case LAW_FLAGBURNING:
               if(billdir[c]==1)addstr("Limit Prohibitions on Flag Burning");
               else addstr("Protect the Symbol of Our Nation");
               break;
            case LAW_GUNCONTROL:
               if(billdir[c]==1)addstr("Prevent Gun Violence");
               else addstr("Protect our Second Amendment Rights");
               break;
         }

         refresh();
      }
   }

   if(canseethings)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(20,0);
      addstr("Press any key to watch the votes unfold.");

      refresh();
      getch();

      move(0,62);
      addstr("House");

      move(0,70);
      addstr("Senate");

      nodelay(stdscr,TRUE);
   }

   for(c=0;c<cnum;c++)
   {
      char yeswin_h=0;
      char yeswin_s=0;
      int yesvotes_h=0;
      int yesvotes_s=0;

      int vote;
      int s=-1;

      for(int l=0;l<435;l++)
      {
         vote=house[l];
         if(vote>=-1&&vote<=1)vote+=LCSrandom(3)-1;

         if(law[bill[c]]>vote && billdir[c]==-1)yesvotes_h++;
         if(law[bill[c]]<vote && billdir[c]==1)yesvotes_h++;

         if(l==434)
         {
            if(yesvotes_h>=218)yeswin_h=1;
         }

         if(canseethings)
         {
            if(l==434&&yeswin_h)set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(l==434)set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(c*3+2,62);
            itoa(yesvotes_h,num,10);
            addstr(num);
            addstr(" Yea");

            if(l==434&&!yeswin_h)set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(l==434)set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(c*3+3,62);
            itoa(l+1-yesvotes_h,num,10);
            addstr(num);
            addstr(" Nay");
         }

         if(l%4==0&&s<99)
         {
            s++;

            vote=senate[s];
            if(vote>=-1&&vote<=1)vote+=LCSrandom(3)-1;

            if(law[bill[c]]>vote && billdir[c]==-1)yesvotes_s++;
            if(law[bill[c]]<vote && billdir[c]==1)yesvotes_s++;
         }

         if(l==434)
         {
            if(yesvotes_s>=51)yeswin_s=1;
            if(yesvotes_s==50)
            {
               //TIE BREAKER
               int vote=(exec[EXEC_PRESIDENT]+
                  exec[EXEC_VP]+
                  exec[EXEC_STATE]+
                  exec[EXEC_ATTORNEY]+LCSrandom(9)-4)/4;

               if(law[bill[c]]>vote && billdir[c]==-1)yeswin_s=1;
               if(law[bill[c]]<vote && billdir[c]==1)yeswin_s=1;

               //ASSURED SIGNING BY PRESIDENT IF VP VOTED YES
               if(yeswin_s)killbill[c]=-1;
            }
         }

         if(canseethings)
         {
            if(l==434&&yeswin_s)set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(l==434)set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(c*3+2,70);
            itoa(yesvotes_s,num,10);
            addstr(num);
            addstr(" Yea");

            if(l==434&&yesvotes_s==50&&yeswin_s)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(c*3+2,78);
               addstr("VP");
            }

            if(l==434&&!yeswin_s)set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(l==434)set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(c*3+3,70);
            itoa(s+1-yesvotes_s,num,10);
            addstr(num);
            addstr(" Nay");

            if(l==434&&yesvotes_s==50&&!yeswin_s)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(c*3+3,78);
               addstr("VP");
            }

            if(l%5==0)
            {
               refresh();

               pause_ms(10);
            }

            getch();
         }
      }

      if(!yeswin_h)killbill[c]=1;
      if(!yeswin_s)killbill[c]=1;
   }

   if(canseethings)nodelay(stdscr,FALSE);

   int havebill=0;
   for(c=0;c<cnum;c++)
   {
      if(killbill[c]<=0)havebill++;
   }

   if(havebill>0)
   {
      if(canseethings)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(20,0);
         addstr("Press any key to watch the President.                   ");

         refresh();
         getch();

         move(0,35);
         addstr("President");
         refresh();

         nodelay(stdscr,TRUE);

         pause_ms(500);
      }

      for(int c=0;c<bill.size();c++)
      {
         char sign=0;
         if(killbill[c]==1)sign=-1;
         else
         {
            int vote=(exec[EXEC_PRESIDENT]+
               exec[EXEC_VP]+
               exec[EXEC_STATE]+
               exec[EXEC_ATTORNEY]+(short)LCSrandom(9)-4)/4;
            if(exec[EXEC_PRESIDENT]==2)vote=2;
            if(exec[EXEC_PRESIDENT]==-2)vote=-2;

            if(law[bill[c]]>vote && billdir[c]==-1)sign=1;
            if(law[bill[c]]<vote && billdir[c]==1)sign=1;
            if(killbill[c]==-1)sign=1;
         }

         if(canseethings)
         {
            move(c*3+2,35);
            if(sign==1)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               addstr(execname[EXEC_PRESIDENT]);
            }
            else if(sign==-1)
            {
               set_color(COLOR_BLACK,COLOR_BLACK,1);
               addstr("Dead in Congress");
            }
            else if(!sign)
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               addstr("*** VETO ***");
            }

            refresh();

            pause_ms(500);
         }

         if(sign==1)law[bill[c]]+=billdir[c];
      }

      if(canseethings)
      {
         nodelay(stdscr,FALSE);

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(20,0);
         addstr("Press any key to reflect on what has happened.    ");

         refresh();
         getch();
      }
   }
   else
   {
      if(canseethings)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(20,0);
         addstr("None of the items made it to the President's desk.");
         move(21,0);
         addstr("Press any key to reflect on what has happened.    ");

         refresh();
         getch();
      }
   }

   //CONGRESS CONSTITUTION CHANGES
   int housemake[5]={0,0,0,0,0};
   for(int h=0;h<435;h++)
   {
      housemake[house[h]+2]++;
   }

   int senatemake[5]={0,0,0,0,0};
   for(int s=0;s<100;s++)
   {
      senatemake[senate[s]+2]++;
   }

   //TOSS JUSTICES?
   char tossj=0;
   for(int j=0;j<9;j++)
   {
      if(court[j]<=1)
      {
         tossj=1;
         break;
      }
   }

   if(housemake[4]>=290&&senatemake[4]>=67&&tossj)
   {
      tossjustices(canseethings);
   }
}



/* politics - checks if the game is won */
char wincheck(void)
{
   for(int e=0;e<EXECNUM;e++)
   {
      if(exec[e]!=2)return 0;
   }

   for(int l=0;l<LAWNUM;l++)
   {
      if(law[l]!=2)return 0;
   }

   int housemake[5]={0,0,0,0,0};
   for(int h=0;h<435;h++)
   {
      housemake[house[h]+2]++;
   }
   if(housemake[4]<290)return 0;

   int senatemake[5]={0,0,0,0,0};
   for(int s=0;s<100;s++)
   {
      senatemake[senate[s]+2]++;
   }
   if(senatemake[4]<67)return 0;

   int elibjudge=0;
   for(int c=0;c<9;c++)
   {
      if(court[c]==2)elibjudge++;
   }

   if(elibjudge<5)return 0;

   return 1;
}



/* politics - checks the prevailing attitude on a specific law, or overall */
int publicmood(int l)
{
   if(l==-1)
   {
      int sum=0;

      for(int v=0;v<VIEWNUM;v++)
      {
         if(v==VIEW_LIBERALCRIMESQUAD)continue;
         if(v==VIEW_LIBERALCRIMESQUADPOS)continue;

         sum+=attitude[v];
      }

      sum/=(VIEWNUM-2);

      return sum;
   }
   else
   {
      switch(l)
      {
         case LAW_ABORTION:return attitude[VIEW_ABORTION];
         case LAW_ANIMALRESEARCH:return attitude[VIEW_ANIMALRESEARCH];
         case LAW_POLICEBEHAVIOR:return attitude[VIEW_POLICEBEHAVIOR];
         case LAW_PRIVACY:return attitude[VIEW_INTELLIGENCE];
         case LAW_DEATHPENALTY:return attitude[VIEW_DEATHPENALTY];
         case LAW_NUCLEARPOWER:return attitude[VIEW_NUCLEARPOWER];
         case LAW_POLLUTION:return attitude[VIEW_POLLUTION];
         case LAW_LABOR:return attitude[VIEW_SWEATSHOPS];
         case LAW_GAY:return attitude[VIEW_GAY];
         case LAW_CORPORATE:return (attitude[VIEW_CORPORATECULTURE]+
                            attitude[VIEW_CEOSALARY])/2;
         case LAW_FREESPEECH:return attitude[VIEW_FREESPEECH];
			case LAW_TAX:return attitude[VIEW_TAXES];
         case LAW_FLAGBURNING:return attitude[VIEW_FREESPEECH];
         case LAW_GUNCONTROL:
               if(!disbanding)
                  return (attitude[VIEW_POLICEBEHAVIOR]+
                          attitude[VIEW_PRISONS]+
                          attitude[VIEW_DEATHPENALTY]+
                          ((attitude[VIEW_LIBERALCRIMESQUADPOS]-50)*
                            attitude[VIEW_LIBERALCRIMESQUAD]         )/100)/3;
               else
                  return (attitude[VIEW_POLICEBEHAVIOR]+
                          attitude[VIEW_PRISONS]+
                          attitude[VIEW_DEATHPENALTY])/3;
      }
   }

   return 50;
}
