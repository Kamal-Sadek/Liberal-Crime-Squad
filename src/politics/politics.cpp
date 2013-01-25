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
#include "compat.h"

//TODO: Not sure if anything in here should be logged...Perhaps only a summary of the results? --Addictgamer

/* politics - calculate presidential approval */
int presidentapproval()
{
   //Calculate Presidental approval rating
   char mood=publicmood(-1);
   int approval=0;
   int i;
   for(i=0;i<1000;i++)
   {
      if(i%2==0&&LCSrandom(2))      // Partyline supporter (~25%)
      {
         approval++;
      }
      else if(i%2==1&&LCSrandom(2)) // Partyline opponent  (~25%)
      {
         continue;
      }
      else                          // Swing issue voter   (~50%) (should be more than in the real election)
      {
         // Get their leanings as an issue voter
         int vote = getswingvoter();

         // If their views are close to the President's views, they should
         // approve, but might not if their party leaning conflicts with
         // the president's
         if(abs(exec[EXEC_PRESIDENT]-vote)<=1)
         {
            // Moderate president from the Conservative party is only supported
            // by moderates and Conservatives
            if(presparty==1)
            {
               if(vote<=0)
               {
                  approval++;
               }
            }
            // Moderate president from the Liberal party is only supported
            // by moderates and Liberals
            else
            {
               if(vote>=0)
               {
                  approval++;
               }
            }
         }
      }
   }
   return approval;
}

/* politics -- gets the leaning of an issue voter for an election */
int getswingvoter()
{
   // Take a random voter, calculate how liberal or conservative they are
   int bias = publicmood(-1)-LCSrandom(100);
   if(bias >  25) bias =  25;
   if(bias < -25) bias = -25;

   // Each issue they roll for their opinion on a 50-point subset of the
   // spectrum, determined by bias -- high liberal bias only rolls on the
   // liberal end of the spectrum, high conservative bias only rolls on
   // the conservative end of the spectrum
   int vote=-2;
   for(int i=0;i<4;i++)
   {
      if(25+(int)LCSrandom(50)-bias<attitude[randomissue(true)])vote++;
   }

   return vote;
}

/* politics -- gets the leaning of a partyline voter for an election */
int getsimplevoter(int leaning)
{
   int vote=leaning-1;
   for(int i=0;i<2;i++)
   {
      if((int)LCSrandom(100)<attitude[randomissue(true)])vote++;
   }

   return vote;
}


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
         addstr("Presidential General Election ");
         itoa(year,num,10);
         addstr(num);

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(2,0);
         addstr("After a long primary campaign, the people have rallied around two leaders...");
      }

      char candidate[2][80];
      int votes[2]={0,0};

      //Primaries
      int approvepres=0; // presidential approval within own party
      int approveveep=0; // vice-presidential approval within own party
      int libvotes[3]={0,0,0};   // liberal party's candidates votes recieved
      int consvotes[3]={0,0,0};  // conservative party's candidates votes recieved

      // run primaries for 100 voters
      for(int i=0;i<100;i++)
      {
         int voters[2]={0,0};
         // liberal party voter decides
         voters[0]+=getsimplevoter(1);
         // conservative party voter decides
         voters[1]+=getsimplevoter(1);

         // Incumbent can win primary automatically if their approval in their party is over 50%,
         // so we need to know what their inter-party approval rating is.

         // check if this voter supports the president (1/2 chance if closely aligned)
         if(voters[presparty]==exec[EXEC_PRESIDENT]+presparty*2||
            (abs((exec[EXEC_PRESIDENT]+presparty*2)-voters[presparty])==1 && !LCSrandom(2)))approvepres++;
         // check if this voter supports the vice-president (1/3 chance if closely aligned)
         if(voters[presparty]==exec[EXEC_VP]+presparty*2||
            (abs((exec[EXEC_VP]+presparty*2)-voters[presparty])==1 && !LCSrandom(3)))approveveep++;

         // count ballots
         consvotes[voters[1]]++;
         libvotes[voters[0]]++;
      }

      // determine conservative winner
      if(consvotes[0] > consvotes[1])
         if(consvotes[0] > consvotes[2])
            candidate[1][0]=-2;
         else candidate[1][0]=0;
      else if(consvotes[1] > consvotes[2])
         candidate[1][0]=-1;
      else candidate[1][0]=0;

      // determine liberal winner
      if(libvotes[0] > libvotes[1])
         if(libvotes[0] > libvotes[2])
            candidate[0][0]=0;
         else candidate[0][0]=2;
      else if(libvotes[1] > libvotes[2])
         candidate[0][0]=1;
      else candidate[0][0]=2;

      // name the candidates
      if(candidate[1][0]==-2)
         generate_name(candidate[1]+1,GENDER_WHITEMALEPATRIARCH);
      else if(candidate[1][0]==-1)
         generate_name(candidate[1]+1,GENDER_MALE);
      else
         generate_name(candidate[1]+1);
      generate_name(candidate[0]+1);

      // Special Incumbency Rules: If the incumbent president or vice president
      // has approval of over 50% in their party, they win their primary
      // automatically. Even if they don't have over 50%, if their alignment wins
      // using the normal primary process, they are the chosen candidate for
      // that alignment (this last bit only applies to President; unpopular VP
      // candidates just don't run, and if their alignment wins it will be
      // someone else).
      if(execterm==1) // President running for re-election
      {
         if(approvepres>=50)
         {
            candidate[presparty][0]=exec[EXEC_PRESIDENT];
         }
         
         if(candidate[presparty][0]==exec[EXEC_PRESIDENT])
         {
            strcpy(candidate[presparty]+1,execname[EXEC_PRESIDENT]);
         }
         else
         {
            execterm=2; // Boom! Incumbent president was defeated in their
                        // own party. New candidate works with a clean slate.
         }
      }
      else if(approveveep>=50 && // Vice-President running for President
              ((presparty==0 && exec[EXEC_VP]!=-1) || // We don't want conservative liberals
               (presparty==1 && exec[EXEC_VP]!=1)))   // or liberal conservatives.
      {
         if(approvepres>=50)
         {
            candidate[presparty][0]=exec[EXEC_VP];
            strcpy(candidate[presparty]+1,execname[EXEC_VP]);
         }
      }

      //Print candidates
      if(canseethings)
      {
         for(c=0;c<2;c++)
         {
            // Pick color by political orientation
            if(candidate[c][0]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
            else if(candidate[c][0]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            else if(candidate[c][0]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
            else if(candidate[c][0]==1)set_color(COLOR_CYAN,COLOR_BLACK,1);
            else set_color(COLOR_GREEN,COLOR_BLACK,1);

            move(6-c*2,0);
            // Choose title -- president or vice president special titles, otherwise
            // pick based on historically likely titles (eg, governor most likely...)
            if(c==presparty&&execterm==1)addstr("President ");
            else if(c==presparty&&!strcmp(candidate[c]+1,execname[EXEC_VP]))addstr("Vice President ");
            else
            {
               if(LCSrandom(2))
               {
                  addstr("Governor ");
               }
               else if(LCSrandom(2))
               {
                  addstr("Senator ");
               }
               else if(LCSrandom(2))
               {
                  addstr("Ret. General ");
               }
               else if(LCSrandom(2))
               {
                  addstr("Representative ");
               }
               else if(LCSrandom(2))
               {
                  addstr("Mr. ");
               }
               else
               {
                  addstr("Mrs. ");
               }
            }
            
            addstr(candidate[c]+1);
            switch(candidate[c][0])
            {
            case -2:addstr(", Arch Conservative");break;
            case -1:addstr(", Conservative");break;
            case 0:addstr(", moderate");break;
            case 1:addstr(", Liberal");break;
            case 2:addstr(", Elite Liberal");break;
            }
         }

         if(!disbanding)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(8,0);
            addstr("Press any key to watch the election unfold.");

            refresh();
            getch();

            nodelay(stdscr,TRUE);
         }
         else
         {
            refresh();
            pause_ms(200);
         }
      }

      int winner=-1;
      char recount=0;
      char oldwinner=0;
      int vote;

      for(int l=0;l<1000;l++) // 1000 Voters!
      {
         vote=-2;
         if(l%2==0&&LCSrandom(5))      // Partyline Liberals (~40%)
         {
            votes[0]++;
         }
         else if(l%2==1&&LCSrandom(5)) // Partyline Conservatives (~40%)
         {
            votes[1]++;
         }
         else                          // Swing Issue Voters (~20%)
         {
            // Get the leanings of an issue voter
            vote = getswingvoter();

            // If they are to the left or equal to the liberal candidate,
            // and they disagree with the conservative candidate, cast a
            // vote for the liberal candidate.
            if(vote>=candidate[0][0] && vote!=candidate[1][0])
            {
               votes[0]++;
            }
            // If they are to the right or equal to the conservative candidate,
            // or they disagree with the liberal candidate, cast a vote
            // for the conservative candidate.
            else if(vote<=candidate[1][0] && vote!=candidate[0][0])
            {
               votes[1]++;
            }
            // If both candidates agree with them, vote randomly.
            else
            {
               if(LCSrandom(2))votes[0]++;
               else votes[1]++;
            }
         }

         if(l==999)
         {
            int maxvote=0;

            for(c=0;c<2;c++)
            {
               if(votes[c]>maxvote)maxvote=votes[c];
            }

            vector<int> eligible;

            for(c=0;c<2;c++)
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
            else
            {
               winner=eligible[0];
            }
         }

         if((canseethings) && l%5==4)
         {
            for(int c=0;c<2;c++)
            {
               if(votes[c]<votes[!c] || (winner>=0&&c!=winner))set_color(COLOR_BLACK,COLOR_BLACK,1);
               else if(votes[c]>votes[!c] || c==winner)set_color(COLOR_WHITE,COLOR_BLACK,1);
               else set_color(COLOR_WHITE,COLOR_BLACK,0);
               move(6-c*2,45);
               itoa(votes[c]/10,num,10);
               addstr(num);
               itoa(votes[c]%10,num,10);
               addch('.');
               addstr(num);
               addch('%');
               if(c==winner&&recount)addstr(" (After Recount)");
            }
            
            refresh();
            if(!disbanding)pause_ms(40);
            else pause_ms(20);
         }
      }

      if(canseethings)nodelay(stdscr,FALSE);

      if(winner==presparty && execterm==1)oldwinner=1;

      if(canseethings)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(8,0);
         addstr("Press any key to continue the elections.   ");

         refresh();
         getch();
      }
      /*else if(disbanding)
      {
         refresh();
         pause_ms(800);
      }*/

      //CONSTRUCT EXECUTIVE BRANCH
      if(oldwinner)
      {
         execterm=2;
      }
      else
      {
         presparty=winner;
         execterm=1;
         exec[EXEC_PRESIDENT]=candidate[winner][0];
         strcpy(execname[EXEC_PRESIDENT],candidate[winner]+1);

         for(int e=0;e<EXECNUM;e++)
         {
            if(e==EXEC_PRESIDENT)continue;
            if(candidate[winner][0]==-2)exec[e]=-2;
            else if(candidate[winner][0]==2)exec[e]=2;
            else exec[e]=candidate[winner][0]+LCSrandom(3)-1;

            if(exec[e]==-2)
               generate_name(execname[e],GENDER_WHITEMALEPATRIARCH);
            else if(exec[e]==-1)
               generate_name(execname[e],GENDER_MALE);
            else
               generate_name(execname[e]);
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
         
      elections_senate(senmod, canseethings);
   }

   //HOUSE
   if(year%2==0)
   {
      elections_house(canseethings);
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
   int pnum=LCSrandom(4)+4;
   char lawtaken[LAWNUM];
   memset(lawtaken,0,LAWNUM*sizeof(char));

   int lawpriority[LAWNUM];
   memset(lawpriority,0,LAWNUM*sizeof(int));
   char lawdir[LAWNUM];
   memset(lawdir,0,LAWNUM*sizeof(char));
   //DETERMINE PROPS
   int pmood;
   int pvote;
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

      lawpriority[l]=abs((int)pvote-(int)pmood)+LCSrandom(10)+public_interest[l];
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
         
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         itoa(propnum,num,10);
         addstr("Proposition ");addstr(num);addstr(":");
         move(p*3+2,18);
         addstr("To ");
         if(propdir[p]==1)set_color(COLOR_GREEN,COLOR_BLACK,1);
         else set_color(COLOR_RED,COLOR_BLACK,1);
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
            case LAW_WOMEN:
               if(propdir[p]==1)addstr("Expand Women's Rights");
               else addstr("Preserve Traditional Gender Roles");
               break;
            case LAW_CIVILRIGHTS:
               if(propdir[p]==1)addstr("Expand Civil Rights");
               else addstr("Fight Reverse Discrimination");
               break;
            case LAW_DRUGS:
               if(propdir[p]==1)addstr("Limit Oppressive Drug Laws");
               else addstr("Strengthen the War On Drugs");
               break;
            case LAW_IMMIGRATION:
               if(propdir[p]==1)addstr("Protect Immigrant Rights");
               else addstr("Protect our Borders");
               break;
            case LAW_ELECTIONS:
               if(propdir[p]==1)addstr("Fight Political Corruption");
               else addstr("Limit Campaign Finance Reforms");
               break;
            case LAW_MILITARY:
               if(propdir[p]==1)addstr("Limit Military Spending");
               else addstr("Strengthen our National Defense");
               break;
            case LAW_TORTURE:
               if(propdir[p]==1)addstr("Protect Human Rights");
               else addstr("Permit Strong Tactics in Interrogations");
               break;
         }
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      }
   }

   if(canseethings)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(23,0);
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
      mood=publicmood(prop[p]);
      for(int l=0;l<1000;l++)
      {
         if(LCSrandom(100)<mood)
         {
            if(propdir[p]==1)
               yesvotes++;
         }
         else
         {
            if(propdir[p]==-1)
               yesvotes++;
         }
         /*vote=0;
         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;
         if(LCSrandom(100)<mood)vote++;
         vote-=2;

         if(law[prop[p]]>vote && propdir[p]==-1)yesvotes++;
         if(law[prop[p]]<vote && propdir[p]==1)yesvotes++;*/

         if(l==999)
         {
            if(yesvotes>500)yeswin=1;
            else if(yesvotes==500)
            {
               if(!LCSrandom(2))yeswin=1;
               recount=1;
            }
         }

         if(canseethings && (l%10 == 0 || l==999))
         {
            if(yesvotes>l/2 || yeswin)set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(yesvotes<l/2 || l==999)set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(p*3+2,70);
            itoa(yesvotes/10,num,10);
            addstr(num);
            addch('.');
            itoa(yesvotes%10,num,10);
            addstr(num);
            addstr("% Yes");

            if(yesvotes<l/2 || (l==999 && !yeswin))set_color(COLOR_WHITE,COLOR_BLACK,1);
            else if(yesvotes>l/2 || l==999)set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(p*3+3,70);
            itoa((l+1-yesvotes)/10,num,10);
            addstr(num);
            addch('.');
            itoa((l+1-yesvotes)%10,num,10);
            addstr(num);
            addstr("% No");

            refresh();
            pause_ms(10);
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
      move(23,0);
      addstr("Press any key to reflect on what has happened.");

      refresh();
      getch();
   }
}

void elections_senate(int senmod,char canseethings)
{
   char num[10];
   int mood = publicmood(-1);
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
      if(senmod!=-1 && s%3!=senmod)continue;

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
            set_color(COLOR_CYAN,COLOR_BLACK,1);
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
      move(23,0);
      addstr("Press any key to watch the elections unfold.");

      refresh();
      getch();

      nodelay(stdscr,TRUE);
   }

   int vote;
   int change[5] = {0,0,0,0,0};

   x=0;
   y=2;

   for(s=0;s<100;s++)
   {
      if(senmod!=-1 && s%3!=senmod)continue;

      vote=0;
      if(mood>LCSrandom(100))vote++;
      if(mood>LCSrandom(100))vote++;
      if(mood>LCSrandom(100))vote++;
      if(mood>LCSrandom(100))vote++;

      if(termlimits)
      {
         change[senate[s]+2]--;
         change[vote]++;
         senate[s]=vote-2;
      }
      else
      {
         change[senate[s]+2]--;

         if(senate[s]>0 && vote<3 && LCSrandom(mood+11)>10)vote++;
         if(senate[s]>1 && vote<4)vote++;

         if(senate[s]<0 && vote>1 && LCSrandom(100-mood+11)>10)vote--;
         if(senate[s]<-1 && vote>0)vote--;

         switch(senate[s])
         {
         case -2:
            if(mood<60)break;
            if(vote>=3)senate[s]=vote-1;
            break;
         case -1:
            if(vote==0 && LCSrandom(100-mood+1)>60)senate[s]=-2;
            if(mood<50 && LCSrandom(7))break;
            if(vote>=3 && LCSrandom(3))senate[s]=vote-1;
            break;
         case 0:
            if(!LCSrandom(3))senate[s]=vote-2;
            break;
         case 1:
            if(vote==4 && LCSrandom(mood+1)>60)senate[s]=2;
            if(mood>50 && LCSrandom(7))break;
            if(vote<=1 && LCSrandom(3))senate[s]=vote-1;
            break;
         case 2:
            if(mood>40)break;
            if(vote<=1)senate[s]=vote-1;
            break;
         }

         if(senate[s]>2)senate[s]=2;
         if(senate[s]<-2)senate[s]=-2;

         change[senate[s]+2]++;
      }


      if(canseethings)
      {
         move(y,x);

         if(senate[s]==-2)
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr("Arch-Conservative    ");
         }
         else if(senate[s]==-1)
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            addstr("Conservative         ");
         }
         else if(senate[s]==0)
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            addstr("moderate             ");
         }
         else if(senate[s]==1)
         {
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            addstr("Liberal              ");
         }
         else
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            addstr("Elite Liberal        ");
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
         set_color(COLOR_WHITE,COLOR_BLACK,0);

         char buffer[10];
         move(20,0);
         addstr("Net change: ");
         addstr("C+: ");
         if(change[0]>0)addstr("+");
         itoa(change[0],buffer,10);
         addstr(buffer);
         addstr("   C: ");
         if(change[1]>0)addstr("+");
         itoa(change[1],buffer,10);
         addstr(buffer);
         addstr("   m: ");
         if(change[2]>0)addstr("+");
         itoa(change[2],buffer,10);
         addstr(buffer);
         addstr("   L: ");
         if(change[3]>0)addstr("+");
         itoa(change[3],buffer,10);
         addstr(buffer);
         addstr("   L+: ");
         if(change[4]>0)addstr("+");
         itoa(change[4],buffer,10);
         addstr(buffer);
         addstr("        ");

         if(!disbanding)
         {
            refresh();
            pause_ms(30);
         }
      }
   }

   if(canseethings)
   {
      nodelay(stdscr,FALSE);

      move(21,0);
      if(change[0]+change[1]>change[3]+change[4])
      {
         if(change[1]<0 && mood<25)
         {
            addstr("The $$ U.S.A. Flag Eagle $$ Conservative Party claims victory!");
         }
         else
         {
            addstr("The Conservative Party claims victory!");
         }
      }
      else if(change[0]+change[1]<change[3]+change[4])
      {
         if(change[3]<0 && mood>75)
         {
            addstr("The Progressive Elite Social Liberal Party claims victory!");
         }
         else
         {
            addstr("The Liberal Party claims victory!");
         }
      }
      else
      {
         addstr("The next two years promise to be more of the same.");
      }

      move(22,0);
      addstr("Press any key to continue the elections.    ");

      refresh();
      getch();
   }
}

void elections_house(char canseethings)
{
   char num[10];
   int mood = publicmood(-1);
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
            set_color(COLOR_CYAN,COLOR_BLACK,1);
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
      move(23,0);
      addstr("Press any key to watch the elections unfold.");

      refresh();
      getch();

      nodelay(stdscr,TRUE);
   }

   int vote;
   int change[5] = {0,0,0,0,0};

   x=0;
   y=2;

   for(h=0;h<435;h++)
   {
      vote=0;
      if(mood>LCSrandom(100))vote++;
      if(mood>LCSrandom(100))vote++;
      if(mood>LCSrandom(100))vote++;
      if(mood>LCSrandom(100))vote++;

      if(termlimits)
      {
         change[house[h]+2]--;
         change[vote]++;
         house[h]=vote-2;
      }
      else
      {
         change[house[h]+2]--;

         if(house[h]>0 && vote<3 && LCSrandom(mood+11)>10)vote++;
         if(house[h]>1 && vote<4)vote++;

         if(house[h]<0 && vote>1 && LCSrandom(100-mood+11)>10)vote--;
         if(house[h]<-1 && vote>0)vote--;

         switch(house[h])
         {
         case -2:
            if(mood<60)break;
            if(vote>=3)house[h]=vote-1;
            break;
         case -1:
            if(vote==0 && LCSrandom(100-mood+1)>60)house[h]=-2;
            if(mood<50 && LCSrandom(7))break;
            if(vote>=3 && LCSrandom(3))house[h]=vote-1;
            break;
         case 0:
            if(!LCSrandom(3))house[h]=vote-2;
            break;
         case 1:
            if(vote==4 && LCSrandom(mood+1)>60)house[h]=2;
            if(mood>50 && LCSrandom(7))break;
            if(vote<=1 && LCSrandom(3))house[h]=vote-1;
            break;
         case 2:
            if(mood>40)break;
            if(vote<=1)house[h]=vote-1;
            break;
         }

         if(house[h]>2)house[h]=2;
         if(house[h]<-2)house[h]=-2;

         change[house[h]+2]++;
      }
      
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
            set_color(COLOR_CYAN,COLOR_BLACK,1);
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
         set_color(COLOR_WHITE,COLOR_BLACK,0);

         char buffer[10];
         move(20,0);
         addstr("Net change: ");
         addstr("C+: ");
         if(change[0]>0)addstr("+");
         itoa(change[0],buffer,10);
         addstr(buffer);
         addstr("   C: ");
         if(change[1]>0)addstr("+");
         itoa(change[1],buffer,10);
         addstr(buffer);
         addstr("   m: ");
         if(change[2]>0)addstr("+");
         itoa(change[2],buffer,10);
         addstr(buffer);
         addstr("   L: ");
         if(change[3]>0)addstr("+");
         itoa(change[3],buffer,10);
         addstr(buffer);
         addstr("   L+: ");
         if(change[4]>0)addstr("+");
         itoa(change[4],buffer,10);
         addstr(buffer);
         addstr("        ");

         if(!disbanding)
         {
            refresh();

            pause_ms(5);

            getch();
         }
      }
   }

   if(canseethings)
   {
      nodelay(stdscr,FALSE);

      move(21,0);
      if(change[0]+change[1]>change[3]+change[4])
      {
         if(change[1]<0 && mood<25)
         {
            addstr("The $$ U.S.A. Flag Eagle $$ Conservative Party claims victory!");
         }
         else
         {
            addstr("The Conservative Party claims victory!");
         }
      }
      else if(change[0]+change[1]<change[3]+change[4])
      {
         if(change[3]<0 && mood>75)
         {
            addstr("The Progressive Elite Social Liberal Party claims victory!");
         }
         else
         {
            addstr("The Liberal Party claims victory!");
         }
      }
      else
      {
         addstr("The next two years promise to be more of the same.");
      }
      if(!disbanding)
      {
         move(22,0);
         addstr("Press any key to continue the elections.    ");

         refresh();
         getch();
      }
      else
      {
         refresh();
         pause_ms(800);
      }
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

      set_color(COLOR_WHITE,COLOR_BLACK,0);
   }

   vector<int> scase;
   vector<int> scasedir;
   int cnum=LCSrandom(5)+2;
   int bias=0;
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

      //Constitutional bias -- free speech, flag burning issues, supreme court
      //is extra liberal, gun control, supreme court is extra conservative
      if(scase[c]==LAW_FREESPEECH || scase[c]==LAW_FLAGBURNING)bias=1;
      else if(scase[c]==LAW_GUNCONTROL)bias=-1;
      else bias=0;

      if(law[scase[c]]==2)scasedir[c]=-1;
      else if(law[scase[c]]==-2)scasedir[c]=1;
      else
      {
         if(bias)scasedir[c]=bias;
         else if(!LCSrandom(2))scasedir[c]=1;
         else scasedir[c]=-1;
      }

      if(canseethings)
      {
         move(c*3+2,0);
         char name1[80];
         char name2[80];
         if(!LCSrandom(5)) strcpy(name1,"United States");
         else lastname(name1);

         lastname(name2);
         if((scase[c]==LAW_LABOR||scase[c]==LAW_CORPORATE||
            scase[c]==LAW_ANIMALRESEARCH||scase[c]==LAW_POLLUTION)&&
            !LCSrandom(5))
         {
            switch(LCSrandom(5))
            {
            case 0:strcat(name2,", Inc.");break;
            case 1:strcat(name2,", L.L.C.");break;
            case 2:strcat(name2," Corp.");break;
            case 3:strcat(name2," Co.");break;
            case 4:strcat(name2,", Ltd.");break;
            }
         }

         addstr(name1);
         addstr(" vs. ");
         addstr(name2);

         move(c*3+3,0);
         addstr("A Decision could ");
         if(scasedir[c]==1)set_color(COLOR_GREEN,COLOR_BLACK,1);
         else set_color(COLOR_RED,COLOR_BLACK,1);
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
            case LAW_WOMEN:
               if(scasedir[c]==1)addstr("Expand Women's Rights");
               else addstr("Preserve Traditional Gender Roles");
               break;
            case LAW_CIVILRIGHTS:
               if(scasedir[c]==1)addstr("Expand Civil Rights");
               else addstr("Fight Reverse Discrimination");
               break;
            case LAW_DRUGS:
               if(scasedir[c]==1)addstr("Limit Oppressive Drug Laws");
               else addstr("Strengthen the War On Drugs");
               break;
            case LAW_IMMIGRATION:
               if(scasedir[c]==1)addstr("Protect Immigrant Rights");
               else addstr("Protect our Borders");
               break;
            case LAW_ELECTIONS:
               if(scasedir[c]==1)addstr("Fight Political Corruption");
               else addstr("Limit Regulation of Political Speech");
               break;
            case LAW_MILITARY:
               if(scasedir[c]==1)addstr("Limit Military Spending");
               else addstr("Strengthen our National Defense");
               break;
            case LAW_TORTURE:
               if(scasedir[c]==1)addstr("Protect Human Rights");
               else addstr("Permit Strong Tactics in Interrogations");
               break;
         }
         set_color(COLOR_WHITE,COLOR_BLACK,0);

         refresh();
      }
   }

   if(canseethings)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(23,0);
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
      //Constitutional bias -- free speech, flag burning issues, supreme court
      //is extra liberal, gun control, supreme court is extra conservative
	  //"All court justices will vote according to alignment and biasand do not consult
	  //popular opinion...---Servant Corps"
      if(scase[c]==LAW_FREESPEECH || scase[c]==LAW_FLAGBURNING)bias=1;
      else if(scase[c]==LAW_GUNCONTROL)bias=-1;
      else bias=0;

      for(int l=0;l<9;l++)
      {
         vote=court[l];
         if(vote>=-1&&vote<=1)vote+=bias;

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

            pause_ms(60);

            getch();
         }
      }

      if(yeswin)law[scase[c]]+=scasedir[c];
   }

   if(canseethings)
   {
      nodelay(stdscr,FALSE);

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(23,0);
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

      generate_name(courtname[j]);
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
   int cnum=LCSrandom(3)+4;
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
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(c*3+2,0);
         addstr("Joint Resolution ");
         itoa(year,num,10);
         addstr(num);
         addstr("-");
         itoa(c+1,num,10);
         addstr(num);

         move(c*3+3,0);
         addstr("To ");
         if(billdir[c]==1)set_color(COLOR_GREEN,COLOR_BLACK,1);
         else set_color(COLOR_RED,COLOR_BLACK,1);
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
            case LAW_WOMEN:
               if(billdir[c]==1)addstr("Expand Women's Rights");
               else addstr("Preserve Traditional Gender Roles");
               break;
            case LAW_CIVILRIGHTS:
               if(billdir[c]==1)addstr("Expand Civil Rights");
               else addstr("Fight Reverse Discrimination");
               break;
            case LAW_DRUGS:
               if(billdir[c]==1)addstr("Limit Oppressive Drug Laws");
               else addstr("Strengthen the War On Drugs");
               break;
            case LAW_IMMIGRATION:
               if(billdir[c]==1)addstr("Protect Immigrant Rights");
               else addstr("Protect our Borders");
               break;
            case LAW_ELECTIONS:
               if(billdir[c]==1)addstr("Fight Political Corruption");
               else addstr("Limit Regulation of Political Speech");
               break;
            case LAW_MILITARY:
               if(billdir[c]==1)addstr("Limit Military Spending");
               else addstr("Strengthen our National Defense");
               break;
            case LAW_TORTURE:
               if(billdir[c]==1)addstr("Ban Torture Techniques");
               else addstr("Permit Strong Tactics in Interrogations");
               break;
         }
         set_color(COLOR_WHITE,COLOR_BLACK,0);

         refresh();
      }
   }

   if(canseethings)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(23,0);
      addstr("Press any key to watch the votes unfold.");

      refresh();
      getch();

      move(0,62);
      addstr("House");

      move(0,70);
      addstr("Senate");

      nodelay(stdscr,TRUE);
   }

   //Liberals and Conservatives are 50% likely to consult popular opinion, otherwise they vote
   //according to alignment. Moderates are 80% likely to consult popular opinion. I am using
   //the old prop. voting code at present because I do not know how best to use the new prop. voting code.
   //Feel free to replace the code I'm using with the new prop. voting code when you get the chance. ---Servant Corps
   for(c=0;c<cnum;c++)
   {
      char yeswin_h=0;
      char yeswin_s=0;
      int yesvotes_h=0;
      int yesvotes_s=0;

      int mood = publicmood(bill[c]);    //A bubblygummy fix that may just work, at least compiles now --KViiri
      int vote;
      int s=-1;

      for(int l=0;l<435;l++)
      {
         vote=house[l];
         if(vote==-1)
         {
            if (LCSrandom(2))
            {
               vote=0;
               if (LCSrandom(100) < mood)vote++;
               if (LCSrandom(100) < mood)vote++;
               if (LCSrandom(100) < mood)vote++;
               if (LCSrandom(100) < mood)vote++;
               vote -= 2;
            }
         }
         else if(vote==1)
         {
            if (LCSrandom(2))
            {
               vote=0;
               if(LCSrandom(100)<mood)vote++;
               if(LCSrandom(100)<mood)vote++;
               if(LCSrandom(100)<mood)vote++;
               if(LCSrandom(100)<mood)vote++;
               vote-=2;
            }
         }
         else if(vote==0)
         {
            if (LCSrandom(10) <= 4)
            {
               vote = 0;
               if (LCSrandom(100) < mood)vote++;
               if (LCSrandom(100) < mood)vote++;
               if (LCSrandom(100) < mood)vote++;
               if (LCSrandom(100) < mood)vote++;
               vote -= 2;
            }
         }

         if (law[bill[c]] > vote && billdir[c] == -1)yesvotes_h++;
         if (law[bill[c]] < vote && billdir[c] == 1)yesvotes_h++;

         if(l==434)
         {
            if(yesvotes_h>=218)yeswin_h=1;
            if(yesvotes_h>=290)killbill[c]=-2;
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
            if(yesvotes_s<67 && killbill[c]==-2)killbill[c]=0;
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

               pause_ms(5);
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
         move(23,0);
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
               if(killbill[c]==-2)
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  addstr("FORCED BY CONGRESS");
                  sign=1;
               }
               else
               {
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  addstr("*** VETO ***");
               }
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
         move(23,0);
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
         move(23,0);
         addstr("None of the items made it to the President's desk.");
         move(24,0);
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

   // Throw out non-L+ Justices?
   char tossj=0;
   for(int j=0;j<9;j++)
   {
      if(court[j]<=1)
      {
         tossj=1;
         break;
      }
   }
   if(housemake[4]+housemake[3]/2>=290&&senatemake[4]+senatemake[3]/2>=67&&tossj&&!nocourtpurge)
   {
      tossjustices(canseethings);
   }
   // Purge Congress, implement term limits, and hold new elections?
   if((housemake[4]+housemake[3]/2<290||senatemake[4]+senatemake[3]/2<67)&&publicmood(-1)>80 && !notermlimit)
   {
      amendment_termlimits(canseethings);
   }
   // REPEAL THE CONSTITUTION AND LOSE THE GAME?
   if(housemake[0]>=290&&senatemake[0]>=67)
   {
      reaganify(canseethings);
   }
}



/* politics - checks if the game is won */
char wincheck(void)
{
   for(int e=0;e<EXECNUM;e++)
   {
      if(exec[e]<2)return 0;
   }

   if (wincondition==WINCONDITION_ELITE)
   {
      for(int l=0;l<LAWNUM;l++)
      {
         if(law[l]<2)return 0;
	  }
   }
   else
   {
	  int liberalLaws = 0;
	  int eliteLaws = 0;
      for(int l=0;l<LAWNUM;l++)
      {
         if(law[l]<1)return 0;
		 if(law[l]==1)
			 liberalLaws++;
		 else
			 eliteLaws++;
	  }
	  if (eliteLaws < liberalLaws) return 0;
   }

   int housemake[5]={0,0,0,0,0};
   for(int h=0;h<435;h++)
   {
      housemake[house[h]+2]++;
   }
   if(housemake[4]+housemake[3]/2<((wincondition==WINCONDITION_ELITE) ? 290 : 270))return 0;

   int senatemake[5]={0,0,0,0,0};
   for(int s=0;s<100;s++)
   {
      senatemake[senate[s]+2]++;
   }
   if(senatemake[4]+senatemake[3]/2<((wincondition==WINCONDITION_ELITE) ? 67 : 60))return 0;

   int elibjudge=0;
   int libjudge=0;
   for(int c=0;c<9;c++)
   {
      if(court[c]>=2)elibjudge++;
	  if(court[c]==1)libjudge++;
   }

   if (wincondition==WINCONDITION_ELITE)
		{if(elibjudge<5)return 0;}
	else
		{if (elibjudge<5 && elibjudge+libjudge/2<6)return 0;}

   return 1;
}


/*
        FIXED:
                At the present time, VIEW_CIVILRIGHTS has far too much sway.
                However, before this was the case, as an example, LAW_ABORTION
                and LAW_WOMEN, had the same "return attitude[]" attribute, and
                both returned VIEW_WOMEN. What I think is needed, is some sort
                of indicators for things such as:
                        1) Desire for change, which could be based on the
                                approval rate of the president.
                        2) Liberal, Conservative, and Stalinist percentage of
                                the country, i.e. how much support each party
                                has.
                        3) VIEW_HUMANRIGHTS, which should be based off of the
                                two previous things, (i.e. A higher liberal
                                percentage, and a higher desire for change would
                                make VIEW_HUMANRIGHTS get closer to 100.)

        -- LiteralKa

        ADDENDUM (20090812): Keeping this for historical purposes, and to
                possibly improve future changes to this issue.
*/
/*
FIXME, PART1:
        LAW_HUMANRIGHTS is added as a sort of an indictator, but it relies on
                all the other Human Rights issue, rather than affecting the
                issues to be more "pro-Human Rights". Essentially, if you
                support Gay Rights but not Abortion Rights, you will not be
                considered as someone who supports 'human rights'.

        ---Servant Corps

/////                                                                      /////

@Servant:
        As it stands, (revision 316) the only alarming thing is that this may
                influence LAW_*s that are affected by LAW_HUMANRIGHTS. This is
                only midly alarming because the LAW_* itself is effected, and
                not the attitude[VIEW_*].
        -- LiteralKa
*/

/* politics - checks the prevailing attitude on a specific law, or overall */
int publicmood(int l)
{
   switch(l)
   {
      // All laws should be affected by exactly one issue if there is a direct
      // correlation between that law and an issue. For example, police behavior
      // as a law should depend only upon police behavior as an issue. This keeps
      // the game logical to the player and ensures that the public opinion polls
      // displayed in-game accurately predict how people will vote in specific
      // issues. For a handful of laws, we might not have a directly correllating
      // issue; for example, as of this writing, there is no issue asking people's
      // opinions on torture. In this case, we can use the nearest issue, or we
      // can mix two closely related ones. As a general principle, try to avoid
      // getting too complicated here; this is under-the-hood stuff the player
      // will never appreciate, so it should be kept as simple and transparent as
      // possible so as to avoid creating unexpected results that will only confuse
      // players, like people refusing to further regulate nuclear power because
      // one of the other issues besides nuclear power is conservative, even when
      // the nuclear power issue is 100% Liberal. - Jonathan S. Fox
      case LAW_ABORTION:
         return attitude[VIEW_WOMEN];//XXX: There is no ``VIEW_ABORTION''!
      case LAW_ANIMALRESEARCH:
         return attitude[VIEW_ANIMALRESEARCH];
      case LAW_POLICEBEHAVIOR:
         return attitude[VIEW_POLICEBEHAVIOR];
      case LAW_PRIVACY:
         return attitude[VIEW_INTELLIGENCE];
      case LAW_DEATHPENALTY:
         return attitude[VIEW_DEATHPENALTY];
      case LAW_NUCLEARPOWER:
         return attitude[VIEW_NUCLEARPOWER];
      case LAW_POLLUTION:
         return attitude[VIEW_POLLUTION];
      case LAW_LABOR:
         return attitude[VIEW_SWEATSHOPS];
      case LAW_GAY:
         return attitude[VIEW_GAY];
      case LAW_CORPORATE:
         // We'll be merging these two views here because there is no CEO salary law.
         // The issue is there for flavor, and falls under the same umbrella of
         // corporate regulation. - Jonathan S. Fox
         return (attitude[VIEW_CORPORATECULTURE] + attitude[VIEW_CEOSALARY])/2;
      case LAW_FREESPEECH:
         return attitude[VIEW_FREESPEECH];
      case LAW_TAX:
         return attitude[VIEW_TAXES];
      case LAW_FLAGBURNING:
         return attitude[VIEW_FREESPEECH];   // <-- I'm keeping this pure free speech instead of free speech
      case LAW_WOMEN:                        // plus political violence. Ideologically, there's no association
         return attitude[VIEW_WOMEN];        // between flag burning and violence. - Jonathan S. Fox
      case LAW_CIVILRIGHTS:
         return attitude[VIEW_CIVILRIGHTS];
      case LAW_DRUGS:
         return attitude[VIEW_DRUGS];
      case LAW_IMMIGRATION:
         return attitude[VIEW_IMMIGRATION];//XXX: VIEW_DRUGS?
      case LAW_MILITARY:
         return attitude[VIEW_MILITARY];
      case LAW_TORTURE:
         return attitude[VIEW_TORTURE];
      case LAW_GUNCONTROL:
         return attitude[VIEW_GUNCONTROL];
      case LAW_ELECTIONS:
      default: //eg. -1
      {
         int sum=0;

         for(int v=0;v<VIEWNUM;v++)
         {
            if(v==VIEW_LIBERALCRIMESQUAD)continue;
            if(v==VIEW_LIBERALCRIMESQUADPOS)continue;
            if(v==VIEW_CONSERVATIVECRIMESQUAD)continue;
            sum+=attitude[v];
         }

         sum/=(VIEWNUM-3);

         return sum;
      }
   }
}
