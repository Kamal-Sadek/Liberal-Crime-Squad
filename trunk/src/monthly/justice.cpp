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

/* monthly - hold trial on a liberal */
void trial(Creature &g)
{
   // If their old base is no longer under LCS control, wander back to the
   // homeless shelter instead.
   if(location[g.base]->renting<0)
   {
      for(int i=0;i<location.size();++i)
      {
         if(location[i]->type==SITE_RESIDENTIAL_SHELTER)
         {
            g.base=i;
            break;
         }
      }
   }
   g.location=g.base;
   bool breaker[LAWFLAGNUM]={0};

   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(1,1);
   addstr(g.name, gamelog);
   addstr(" is standing trial.", gamelog);
   gamelog.newline();
   refresh();
   getch();

   set_color(COLOR_WHITE,COLOR_BLACK,0);

   if(!iscriminal(g))criminalize(g,LAWFLAG_LOITERING);

   int typenum=0;
   int scarefactor=0;
      // *JDS* Scarefactor is the severity of the case against you; if you're a really
      // nasty person with a wide variety of major charges against you, then scarefactor
      // can get up there

   for(int i=0;i<LAWFLAGNUM;i++)
   {
      if(g.crimes_suspected[i])
      {
         typenum++;
         scarefactor+=lawflagheat(i)*g.crimes_suspected[i];
         breaker[i]=1;
      }
   }

   //CHECK FOR SLEEPERS
   bool autoconvict=0;
   Creature *sleeperjudge=NULL;
   Creature *sleeperlawyer=NULL;
   int maxsleeperskill=0;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->alive&&(pool[p]->flag & CREATUREFLAG_SLEEPER))
      {
         if(pool[p]->type==CREATURE_JUDGE_CONSERVATIVE||
            pool[p]->type==CREATURE_JUDGE_LIBERAL)
         {
            if(pool[p]->infiltration*100>=LCSrandom(100))
               sleeperjudge=pool[p];
         }
         if(pool[p]->type==CREATURE_LAWYER)/*&&
            (pool[p]->infiltration*100>=LCSrandom(100)||
            (pool[p]->flag & CREATUREFLAG_LOVESLAVE&&pool[p]->hireid==g.id)))*/
         {
            if(pool[p]->get_skill(SKILL_LAW)+pool[p]->get_skill(SKILL_PERSUASION)>=maxsleeperskill)
            {
               sleeperlawyer=pool[p];
               maxsleeperskill=pool[p]->get_skill(SKILL_LAW)+sleeperlawyer->get_skill(SKILL_PERSUASION);
            }
         }
      }
   }

   //STATE CHARGES
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(3,1);
   if(sleeperjudge)
   {
      addstr("Sleeper ", gamelog);
      addstr(sleeperjudge->name, gamelog);
      addstr(" reads the charges, trying to hide a smile:", gamelog);
   }
   else addstr("The judge reads the charges:", gamelog);
   gamelog.newline();
   set_color(COLOR_RED,COLOR_BLACK,1);
   move(5,1);
   addstr("The defendant, ", gamelog);
   addstr(g.propername, gamelog);
   addstr(", is charged with ", gamelog);
   int x=2,y=5;
   while(typenum>0)
   {
      typenum--;

      x++;
      if(x>=2){x=0;y++;move(y,1);}
      //////////////////////////////////////////////////////////////////////////
      //                                Treason                               //
      //////////////////////////////////////////////////////////////////////////
      // Technically, treason is exposing state secrets, or somesuch.         //
      // Illegal Immigrants cannot commit treason, because treason can only   //
      // be committed by `those owing allegiance to the United States`.       //
      //////////////////////////////////////////////////////////////////////////

      // The above is already respected by LCS; treason occurs from exposing
      // intelligence secrets, and illegal immigrants are not taken to trial.
      //    - Jonathan S. Fox

      // Oh, right. Forgot about that. Even if the law is L+?
      if(breaker[LAWFLAG_TREASON])
      {
         if(g.crimes_suspected[LAWFLAG_TREASON]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_TREASON],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("treason", gamelog);
         breaker[LAWFLAG_TREASON]=0;
      }
      else if(breaker[LAWFLAG_TERRORISM])
      {
         if(g.crimes_suspected[LAWFLAG_TERRORISM]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_TERRORISM],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("terrorism", gamelog);
         breaker[LAWFLAG_TERRORISM]=0;
      }
      else if(breaker[LAWFLAG_KIDNAPPING])
      {
         if(g.crimes_suspected[LAWFLAG_KIDNAPPING]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_KIDNAPPING],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("kidnapping", gamelog);
         breaker[LAWFLAG_KIDNAPPING]=0;
      }
      else if(breaker[LAWFLAG_BURNFLAG] && law[LAW_FLAGBURNING] <= 0)
      {
         if(g.crimes_suspected[LAWFLAG_BURNFLAG]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_BURNFLAG],str,10);
            addstr(str);
            addstr(" counts of ", gamelog);
         }
         if(law[LAW_FLAGBURNING]==-2)
            addstr("Flag Murder", gamelog);
         else if(law[LAW_FLAGBURNING]==-1)
            addstr("felony flag burning", gamelog);
         else if(law[LAW_FLAGBURNING]==0)
            addstr("flag burning", gamelog);
         breaker[LAWFLAG_BURNFLAG]=0;
      }
      else if(breaker[LAWFLAG_SPEECH])
      {
         if(g.crimes_suspected[LAWFLAG_SPEECH]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_SPEECH],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("harmful speech", gamelog);
         breaker[LAWFLAG_SPEECH]=0;
      }
      else if(breaker[LAWFLAG_BROWNIES])
      {
         if(g.crimes_suspected[LAWFLAG_BROWNIES]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_BROWNIES],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("distribution of a controlled substance", gamelog);
         breaker[LAWFLAG_BROWNIES]=0;
         x=2;
      }
      else if(breaker[LAWFLAG_ESCAPED])
      {
         if(g.crimes_suspected[LAWFLAG_ESCAPED]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_ESCAPED],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("escaping prison", gamelog);
         breaker[LAWFLAG_ESCAPED]=0;
         //autoconvict=1; // *Impossible* to beat this charge if this line isn't commented out
      }
      else if(breaker[LAWFLAG_HELPESCAPE])
      {
         if(g.crimes_suspected[LAWFLAG_HELPESCAPE]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_HELPESCAPE],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("aiding a prison escape", gamelog);
         breaker[LAWFLAG_HELPESCAPE]=0;
         x=2;
      }
      else if(breaker[LAWFLAG_JURY])
      {
         if(g.crimes_suspected[LAWFLAG_JURY]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_JURY],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("jury tampering", gamelog);
         breaker[LAWFLAG_JURY]=0;
      }
      else if(breaker[LAWFLAG_RACKETEERING])
      {
         addstr("racketeering", gamelog);
         breaker[LAWFLAG_RACKETEERING]=0;
      }
      else if(breaker[LAWFLAG_ARMEDASSAULT])
      {
         if(g.crimes_suspected[LAWFLAG_ARMEDASSAULT]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_ARMEDASSAULT],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("felony assault", gamelog);
         breaker[LAWFLAG_ARMEDASSAULT]=0;
      }
      else if(breaker[LAWFLAG_MURDER])//XXX: How about the addition of a `manslaughter` charge? -- LK
      {                               //     Manslaughter is murder without forethought or malice, IIRC. -- LK
                                      // Well, I can't think of an instance of this in LCS besides fruit stand employees. -Fox
                                      // Possibly people you accidentally kill when shooting at conservatives. -- LK
         if(g.crimes_suspected[LAWFLAG_MURDER]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_MURDER],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("murder", gamelog);
         breaker[LAWFLAG_MURDER]=0;
      }
      else if(breaker[LAWFLAG_EXTORTION])
      {
         if(g.crimes_suspected[LAWFLAG_EXTORTION]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_EXTORTION],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("extortion", gamelog);
         breaker[LAWFLAG_EXTORTION]=0;
      }
      else if(breaker[LAWFLAG_ARSON])
      {
         if(g.crimes_suspected[LAWFLAG_ARSON]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_ARSON],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("arson", gamelog);
         breaker[LAWFLAG_ARSON]=0;
      }
      else if(breaker[LAWFLAG_ASSAULT])
      {
         if(g.crimes_suspected[LAWFLAG_ASSAULT]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_ASSAULT],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("misdemeanor assault", gamelog);
         breaker[LAWFLAG_ASSAULT]=0;
      }
      /*else if(breaker[LAWFLAG_GUNCARRY])
      {
         if(g.crimes_suspected[LAWFLAG_GUNCARRY]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_GUNCARRY],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("possession of an illegal weapon");
         breaker[LAWFLAG_GUNCARRY]=0;
         x=2;
      }
      else if(breaker[LAWFLAG_GUNUSE])
      {
         if(g.crimes_suspected[LAWFLAG_GUNUSE]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_GUNUSE],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("firing an illegal weapon");
         breaker[LAWFLAG_GUNUSE]=0;
         x=2;
      }*/
      else if(breaker[LAWFLAG_CARTHEFT])
      {
         if(g.crimes_suspected[LAWFLAG_CARTHEFT]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_CARTHEFT],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("grand theft auto", gamelog);// If chase lasts more than 20 `turns` then
         breaker[LAWFLAG_CARTHEFT]=0;// this should be `Grand Theft Auto`
      }                              //                 -- LK
                                          // We'll just make it grand theft auto anyway :) -Fox
      else if(breaker[LAWFLAG_BANKROBBERY])
      {
         if(g.crimes_suspected[LAWFLAG_BANKROBBERY]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_BANKROBBERY],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("bank robbery", gamelog);
         breaker[LAWFLAG_BANKROBBERY]=0;
      }
      else if(breaker[LAWFLAG_CCFRAUD])
      {
         if(g.crimes_suspected[LAWFLAG_CCFRAUD]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_CCFRAUD],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("credit card fraud", gamelog);
         breaker[LAWFLAG_CCFRAUD]=0;
      }
      else if(breaker[LAWFLAG_THEFT])
      {
         if(g.crimes_suspected[LAWFLAG_THEFT]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_THEFT],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("theft", gamelog);
         breaker[LAWFLAG_THEFT]=0;
      }
      else if(breaker[LAWFLAG_PROSTITUTION])
      {
         if(g.crimes_suspected[LAWFLAG_PROSTITUTION]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_PROSTITUTION],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("prostitution", gamelog);
         breaker[LAWFLAG_PROSTITUTION]=0;
      }
      else if(breaker[LAWFLAG_HIREILLEGAL])
      {
         if(g.crimes_suspected[LAWFLAG_HIREILLEGAL]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_HIREILLEGAL],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("hiring an illegal alien", gamelog);
         breaker[LAWFLAG_HIREILLEGAL]=0;
         x=2;
      }
      else if(breaker[LAWFLAG_COMMERCE])
      {
         if(g.crimes_suspected[LAWFLAG_COMMERCE]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_COMMERCE],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("interference with interstate commerce", gamelog);
         breaker[LAWFLAG_COMMERCE]=0;
         x=2;
      }
      else if(breaker[LAWFLAG_INFORMATION])
      {
         if(g.crimes_suspected[LAWFLAG_INFORMATION]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_INFORMATION],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("unlawful access of an information system", gamelog);
         breaker[LAWFLAG_INFORMATION]=0;
         x=2;
      }
      else if(breaker[LAWFLAG_BURIAL])
      {
         if(g.crimes_suspected[LAWFLAG_BURIAL]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_BURIAL],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("unlawful burial", gamelog);
         breaker[LAWFLAG_BURIAL]=0;
      }
      else if(breaker[LAWFLAG_BREAKING])
      {
         if(g.crimes_suspected[LAWFLAG_BREAKING]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_BREAKING],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("breaking and entering", gamelog);
         breaker[LAWFLAG_BREAKING]=0;
      }
      else if(breaker[LAWFLAG_VANDALISM])
      {
         if(g.crimes_suspected[LAWFLAG_VANDALISM]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_VANDALISM],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("vandalism", gamelog);
         breaker[LAWFLAG_VANDALISM]=0;
      }
      else if(breaker[LAWFLAG_RESIST])
      {
         addstr("resisting arrest", gamelog);
         breaker[LAWFLAG_RESIST]=0;
      }
      else if(breaker[LAWFLAG_DISTURBANCE])
      {
         if(g.crimes_suspected[LAWFLAG_DISTURBANCE]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_DISTURBANCE],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("disturbing the peace", gamelog);
         breaker[LAWFLAG_DISTURBANCE]=0;
      }
      else if(breaker[LAWFLAG_PUBLICNUDITY])
      {
         if(g.crimes_suspected[LAWFLAG_PUBLICNUDITY]>1)
         {
            char str[10];
            itoa(g.crimes_suspected[LAWFLAG_PUBLICNUDITY],str,10);
            addstr(str, gamelog);
            addstr(" counts of ", gamelog);
         }
         addstr("indecent exposure", gamelog);
         breaker[LAWFLAG_PUBLICNUDITY]=0;
      }
      else if(breaker[LAWFLAG_LOITERING])
      {
         addstr("loitering", gamelog);
         breaker[LAWFLAG_LOITERING]=0;
      }

      if(typenum>1)addstr(", ", gamelog);
      if(typenum==1)addstr(" and ", gamelog);
      if(typenum==0)addstr(".", gamelog);

      refresh();
      getch();
   }
   gamelog.newline();

   if(g.confessions && !sleeperjudge)       //Made sleeper judge prevent these lunatics from testifying
   {
      move(y+=2,1);

      if(g.confessions>1)
      {
         char str[10];
         itoa(g.confessions,str,10);
         addstr(str, gamelog);
         addstr(" former LCS members will testify against ", gamelog);
      }
      else addstr("A former LCS member will testify against ", gamelog);
      addstr(g.name, gamelog);
      addstr(".", gamelog);
      gamelog.newline();
      refresh();
      getch();
   }

   //CHOOSE DEFENSE
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(y+2,1);
   addstr("How will you conduct the defense?");

   char attorneyname[200];
   int oldseed=seed;
   seed=attorneyseed;
   generate_name(attorneyname);
   seed=oldseed;

   y+=4;
   move(y,1);y++;
   addstr("A - Use a court-appointed attorney.");
   move(y,1);y++;
   addstr("B - Defend self!");
   move(y,1);y++;
   addstr("C - Plead guilty.");
   if(ledger.get_funds()<5000)set_color(COLOR_BLACK,COLOR_BLACK,1);
   move(y,1);y++;
   addstr("D - Pay $5000 to hire ace Liberal attorney ");
   addstr(attorneyname);
   addstr(".");
   if(sleeperlawyer)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(y,1);y++;
      addstr("E - Accept sleeper ");
      addstr(sleeperlawyer->name);
      addstr("'s offer to assist pro bono.");
   }
   if(ledger.get_funds()<5000)set_color(COLOR_WHITE,COLOR_BLACK,0);

   //SAV - added in display of skills and relevant attributes to help
   // decide when to defend self.
   char temp[20];
   y++;
   move(y,5);
   addstr("Heart: ");
   addstr(itoa(g.get_attribute(ATTRIBUTE_HEART,true),temp,10));
   move(y,25);
   addstr("Persuasion: ");
   addstr(itoa(g.get_skill(SKILL_PERSUASION),temp,10));
   y++;
   move(y,5);
   addstr("Charisma: ");
   addstr(itoa(g.get_attribute(ATTRIBUTE_CHARISMA,true),temp,10));
   move(y,25);
   addstr("Law: ");
   addstr(itoa(g.get_skill(SKILL_LAW),temp,10));
   y++;
   move(y,5);
   addstr("Intelligence: ");
   addstr(itoa(g.get_attribute(ATTRIBUTE_INTELLIGENCE,true),temp,10));
   y++;
   // End SAV's adds

   short defense;
   int c;
   do
   {
      refresh();
      c=getch();
      translategetch(c);
      if(c=='a'){defense=0;break;}
      if(c=='b'){defense=1;break;}
      if(c=='c'){defense=2;break;}
      if(c=='d'&&ledger.get_funds()>=5000)
      {
         ledger.subtract_funds(5000,EXPENSE_LEGAL);
         defense=3;
         break;
      }
      if(c=='e'&&sleeperlawyer)
      {
         defense=4;
         strcpy(attorneyname,sleeperlawyer->name);
         break;
      }
   }while(1);

   //TRIAL
   if(defense!=2)
   {
      int prosecution=0;
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(1,1);
      addstr(g.name, gamelog);
      addstr(" is standing trial.");

      //TRIAL MESSAGE
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(3,1);
      addstr("The trial proceeds.  Jury selection is first.", gamelog);
      gamelog.newline();
      refresh();
      getch();

      //JURY MAKEUP MESSAGE
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(5,1);
      int jury=LCSrandom(61)-(60*publicmood(-1))/100; // Political leanings of the population determine your jury
      if(sleeperjudge)jury-=20;
      if(defense==3) // Hired $5000 ace attorney
      {
         if(LCSrandom(10))
         {
            addstr(attorneyname, gamelog);
            addstr(" ensures the jury is stacked in ", gamelog);
            addstr(g.name, gamelog);
            addstr("'s favor!", gamelog);
            gamelog.newline();
            if(jury>0)jury=0;
            jury-=30;
         }
         else
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr(attorneyname, gamelog);
            addstr("'s CONSERVATIVE ARCH-NEMESIS will represent the prosecution!!!", gamelog);
            gamelog.newline();
            jury=0;
            prosecution+=100; // DUN DUN DUN!!
         }
      }
      else if(jury<=-29)
      {
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         switch(LCSrandom(4))
         {
            case 0:addstr(g.name);addstr("'s best friend from childhood is a juror.", gamelog);break;
            case 1:addstr("The jury is Flaming Liberal.", gamelog);break;
            case 2:addstr("A few of the jurors are closet Socialists.", gamelog);break;
            case 3:addstr("One of the jurors flashes a SECRET LIBERAL HAND SIGNAL when no one is looking.", gamelog);break;
         }
         gamelog.newline();
      }
      else if(jury<=-15)addstr("The jury is fairly Liberal.", gamelog);
      else if(jury<15)addstr("The jury is quite moderate.", gamelog);
      else if(jury<29)addstr("The jury is a bit Conservative.", gamelog);
      else
      {
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         switch(LCSrandom(4))
         {
            case 0:addstr("Such a collection of Conservative jurors has never before been assembled.", gamelog);break;
            case 1:addstr("One of the accepted jurors is a Conservative activist.", gamelog);break;
            case 2:addstr("A few of the jurors are members of the KKK.", gamelog);break;
            case 3:addstr("The jury is frighteningly Conservative.", gamelog);break;
         }
      }
      gamelog.newline();

      // Debug jury bias
      #ifdef SHOWMECHANICS
      {
         char str[20];
         itoa(jury,str,10);
         addstr(" (");
         if(str[0]!='-')addch('+');

         addstr(str);
         addstr(" to convict)");
      }
      #endif

      refresh();
      getch();

      //PROSECUTION MESSAGE
      // *JDS* The bigger your record, the stronger the evidence
      prosecution+=40+LCSrandom(101);
      prosecution+=scarefactor;
      prosecution+=20*g.confessions;
      if(sleeperjudge)prosecution>>=1;
      if(defense==3)prosecution-=60;

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(7,1);

      if(autoconvict)
      {
         addstr("There is no question of ", gamelog);
         addstr(g.name, gamelog);
         addstr("'s guilt.", gamelog);
         gamelog.newline();
      }
      else
      {
         if(prosecution<=50)addstr("The prosecution's presentation is terrible.", gamelog);
         else if(prosecution<=75)addstr("The prosecution gives a standard presentation.", gamelog);
         else if(prosecution<=125)addstr("The prosecution's case is solid.", gamelog);
         else if(prosecution<=175)addstr("The prosecution makes an airtight case.", gamelog);
         else
         {
            addstr("The prosecution is incredibly strong.", gamelog);
         }
         gamelog.newline();
      }

      // Debug prosecution power
      #ifdef SHOWMECHANICS
      {
         char str[20];
         itoa(prosecution/2,str,10);
         addstr(" (+");
         addstr(str);
         addstr(" to +");
         itoa(prosecution,str,10);
         addstr(str);
         addstr(" to convict)");
      }
      #endif

      refresh();
      getch();

      jury+=LCSrandom(prosecution/2+1)+prosecution/2;

      //DEFENSE MESSAGE
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(9,1);

      int defensepower=0;
      if(defense==0||defense==3||defense==4)
      {
         if(autoconvict)
         {
            addstr("The defense makes a noble attempt, but the outcome is inevitable.", gamelog);
            gamelog.newline();
         }
         else
         {
            if(defense==0)
               defensepower=LCSrandom(71);    // Court-appointed attorney
            else if(defense==3)
               defensepower=LCSrandom(71)+80; // Ace Liberal attorney
            else if(defense==4)
            {
               // Sleeper attorney
               defensepower=LCSrandom(71)+sleeperlawyer->get_skill(SKILL_LAW)*2
                                         +sleeperlawyer->get_skill(SKILL_PERSUASION)*2;
               sleeperlawyer->train(SKILL_LAW,prosecution/4);
               sleeperlawyer->train(SKILL_PERSUASION,prosecution/4);
            }

            if(defensepower<=5)addstr("The defense attorney rarely showed up.", gamelog);
            else if(defensepower<=15)addstr("The defense attorney accidentally said \"My client is GUILTY!\" during closing.", gamelog);
            else if(defensepower<=25)addstr("The defense is totally lame.", gamelog);
            else if(defensepower<=50)addstr("The defense was lackluster.", gamelog);
            else if(defensepower<=75)addstr("Defense arguments were pretty good.", gamelog);
            else if(defensepower<=100)addstr("The defense was really slick.", gamelog);
            else if(defensepower<=145)
            {
               if(prosecution<100)addstr("The defense makes the prosecution look like amateurs.", gamelog);
               else
               {
                  addstr("The defense is extremely compelling.", gamelog);
               }
            }
            else
            {
               if(prosecution<100)
               {
                  addstr(attorneyname, gamelog);
                  addstr("'s arguments made several of the jurors stand up ", gamelog);
                  move(10,1);
                  addstr("and shout \"NOT GUILTY!\" before deliberations even began.", gamelog);
                  if(defense==4)addjuice(*sleeperlawyer,10,500); // Bow please
               }
               else
               {
                  addstr(attorneyname, gamelog);
                  addstr(" conducts an incredible defense.", gamelog);
               }
            }
            gamelog.newline();
         }
      }
      if(defense==1)
      {
         // *JDS* LEGAL SELF-REPRESENTATION: To succeed here, you really need to have two skills be
         // high: persuasion and law, with law being 1.5 times as influential. You can't have
         // just one or just the other. Even if you're a very persuasive person, the court will eat
         // you alive  if you can't sound intelligent when talking about the relevant charges, and you
         // won't be able to fool the jury into letting you go if you aren't persuasive, as no
         // matter how encyclopedic your legal knowledge is, it's all in the pitch.
         //
         // If either your persuasion or your law roll is too low, you'll end up getting a negative
         // result that will drag down your defense. So try not to suck in either area.
         defensepower=5*(g.skill_roll(SKILL_PERSUASION)-3)+
                      10*(g.skill_roll(SKILL_LAW)-3);
         g.train(SKILL_PERSUASION,50);
         g.train(SKILL_LAW,50);

         addstr(g.name, gamelog);
         if(defensepower<=0)
         {
            addstr(" makes one horrible mistake after another.", gamelog);
            gamelog.newline();
            addjuice(g,-10,-50); // You should be ashamed
         }
         else if(defensepower<=25)addstr("'s case really sucked.", gamelog);
         else if(defensepower<=50)addstr(" did all right, but made some mistakes.", gamelog);
         else if(defensepower<=75)addstr("'s arguments were pretty good.", gamelog);
         else if(defensepower<=100)addstr(" worked the jury very well.", gamelog);
         else if(defensepower<=150)addstr(" made a very powerful case.", gamelog);
         else
         {
            addstr(" had the jury, judge, and prosecution crying for freedom.", gamelog);
            addjuice(g,50,1000); // That shit is legend
         }
         gamelog.newline();
      }

      // Debug defense power
      #ifdef SHOWMECHANICS
      {
         char str[20];
         itoa(defensepower,str,10);
         addstr(" (");
         addstr(str);
         addstr(", need ");
         itoa(jury+1,str,10);
         addstr(str);
         addstr(" to acquit)");
      }
      #endif

      refresh();
      getch();

      //DELIBERATION MESSAGE
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(12,1);
      addstr("The jury leaves to consider the case.", gamelog);
      gamelog.newline();
      refresh();
      getch();

      //JURY RETURN MESSAGE
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(1,1);
      addstr("The jury has returned from deliberations.", gamelog);
      gamelog.newline();
      refresh();
      getch();

      bool keeplawflags=0;

      //HUNG JURY
      if(defensepower==jury)
      {
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         move(3,1);
         addstr("But they can't reach a verdict!", gamelog);
         gamelog.newline();
         refresh();
         getch();

         //RE-TRY
         if(LCSrandom(2)||scarefactor>=10||g.confessions)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(5,1);
            addstr("The case will be re-tried next month.", gamelog);
            gamelog.newline();
            refresh();
            getch();

            int ps=-1;
            for(int l=0;l<location.size();l++)
            {
               if(location[l]->type==SITE_GOVERNMENT_COURTHOUSE)
               {
                  ps=l;
               }
            }
            g.location=ps;
            keeplawflags=true;
         }
         //NO RE-TRY
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(5,1);
            addstr("The prosecution declines to re-try the case.", gamelog);
            gamelog.newline();
            if(g.sentence==0)
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(7,1);
               addstr(g.name, gamelog);
               addstr(" is free!", gamelog);
            }
            else
            {
               set_color(COLOR_WHITE,COLOR_BLACK,0);
               move(7,1);
               addstr(g.name, gamelog);
               addstr(" will be returned to prison to resume an earlier sentence.", gamelog);
               if(g.deathpenalty)
               {
                  g.sentence=3;
                  move(9,1);
                  addstr("The execution is scheduled to occur three months from now.", gamelog);
               }
            }
            gamelog.nextMessage();
            refresh();
            getch();
         }
      }
      //ACQUITTAL!
      else if(!autoconvict && defensepower>jury)
      {
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(3,1);
         addstr("NOT GUILTY!", gamelog);
         gamelog.newline();
         refresh();
         getch();

         if(g.sentence==0)
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            move(5,1);
            addstr(g.name, gamelog);
            addstr(" is free!", gamelog);
         }
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(5,1);
            addstr(g.name, gamelog);
            addstr(" will be returned to prison to resume an earlier sentence.", gamelog);
            if(g.deathpenalty)
            {
               g.sentence=3;
               move(7,1);
               addstr("The execution is scheduled to occur three months from now.", gamelog);
            }
         }
         gamelog.nextMessage();
         
         if(defense==4)
         {
            // Juice sleeper
            addjuice(*sleeperlawyer,10,100);
         }
         if(defense==1)
         {
            // Juice for self-defense
            addjuice(g,10,100);
         }
         refresh();
         getch();
      }
      //LENIENCE
      else
      {
         if(defense==4)
         {
            // De-Juice sleeper
            addjuice(*sleeperlawyer,-5,0);
         }

         if(defense!=2)
         {
            // Juice for getting convicted of something :)
            addjuice(g,25,200);
         }

         // Check for lenience; sleeper judge will always be merciful
         if(defensepower/3>=jury/4 || sleeperjudge || defense==2)
         {
            penalize(g,1);
         }
         else
         {
            penalize(g,0);
         }
      }
      //CLEAN UP LAW FLAGS
      if(!keeplawflags)
      {
         for(int i=0;i<LAWFLAGNUM;i++)
         {
            g.crimes_suspected[i]=0;
         }
      }
      g.heat=0;
      //Clean up confessions
      g.confessions=0;
      //PLACE PRISONER
      if(g.sentence!=0)
      {
         imprison(g);
      }
      else
      {
         Armor clothes(*armortype[getarmortype("ARMOR_CLOTHES")]);
         g.give_armor(clothes,NULL);
      }
   }
   //GUILTY PLEA
   // How about "nolo" (Nolo contendere) -- LK
   //  I would imagine this would disregard the strength of the defense. -- LK
   else
   {
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(1,1);
      addstr("The court accepts the plea.", gamelog);
      gamelog.nextMessage();
      refresh();
      getch();

      penalize(g,LCSrandom(2));
      //CLEAN UP LAW FLAGS
      for(int i=0;i<LAWFLAGNUM;i++)
      {
         g.crimes_suspected[i]=0;
      }
      g.heat=0;
      //Clean up confessions
      g.confessions=0;
      //PLACE PRISONER
      if(g.sentence!=0)
      {
         imprison(g);
      }
      else
      {
         Armor clothes(*armortype[getarmortype("ARMOR_CLOTHES")]);
         g.give_armor(clothes,NULL);
      }
   }
}



/* monthly - sentence a liberal */
void penalize(Creature &g,char lenient)
{
   set_color(COLOR_RED,COLOR_BLACK,1);
   move(3,1);
   addstr("GUILTY!", gamelog);
   gamelog.newline();
   refresh();
   getch();

   short oldsentence=g.sentence;
   char olddeathpenalty=g.deathpenalty;
   g.sentence=0;
   g.deathpenalty=0;

   if(!lenient&&((g.crimes_suspected[LAWFLAG_MURDER])||(g.crimes_suspected[LAWFLAG_TREASON])||
      ((g.crimes_suspected[LAWFLAG_BURNFLAG])&&law[LAW_FLAGBURNING]==-2)||
      law[LAW_DEATHPENALTY]==-2))
   {
      if(law[LAW_DEATHPENALTY]==-2)g.deathpenalty=1;
      if(law[LAW_DEATHPENALTY]==-1)g.deathpenalty=LCSrandom(3);
      if(law[LAW_DEATHPENALTY]==0)g.deathpenalty=LCSrandom(2);
      if(law[LAW_DEATHPENALTY]==1)g.deathpenalty=!LCSrandom(5);
      if(law[LAW_DEATHPENALTY]==2)g.deathpenalty=0;
   }

   for(int l=0;l<LAWFLAGNUM;l++)
   {
      if(g.crimes_suspected[l]>10)
         g.crimes_suspected[l]=10;
   }

   //CALC TIME
   if(!g.deathpenalty)
   {
      if(!(g.sentence<0))
      {
         g.sentence+=(36+LCSrandom(18))*g.crimes_suspected[LAWFLAG_KIDNAPPING];
         g.sentence+=(1+LCSrandom(4))*g.crimes_suspected[LAWFLAG_THEFT];
         //g.sentence+=(4+LCSrandom(12))*(!!g.crimes_suspected[LAWFLAG_GUNUSE])+ // Extra for first incident only
         //            (2+LCSrandom(4)*g.crimes_suspected[LAWFLAG_GUNUSE]);      // Generally
         //g.sentence+=(1+LCSrandom(4))*(!!g.crimes_suspected[LAWFLAG_GUNCARRY]);
         g.sentence+=(6+LCSrandom(7))*g.crimes_suspected[LAWFLAG_CARTHEFT];
         g.sentence+=(1+LCSrandom(13))*g.crimes_suspected[LAWFLAG_INFORMATION];
         g.sentence+=(1+LCSrandom(13))*g.crimes_suspected[LAWFLAG_COMMERCE];
         g.sentence+=(6+LCSrandom(25))*g.crimes_suspected[LAWFLAG_CCFRAUD];
         g.sentence+=(3+LCSrandom(12))*g.crimes_suspected[LAWFLAG_BURIAL];
         g.sentence+=(1+LCSrandom(6))*g.crimes_suspected[LAWFLAG_PROSTITUTION];
         g.sentence+=1*g.crimes_suspected[LAWFLAG_DISTURBANCE];
         g.sentence+=1*g.crimes_suspected[LAWFLAG_PUBLICNUDITY];
         //g.sentence+=1*g.crimes_suspected[LAWFLAG_LOITERING];
         g.sentence+=1*g.crimes_suspected[LAWFLAG_HIREILLEGAL];
         g.sentence+=(12+LCSrandom(100))*g.crimes_suspected[LAWFLAG_RACKETEERING];

         // How illegal is marijuana?
         if(law[LAW_DRUGS]==-2)g.sentence+=(3+LCSrandom(360))*g.crimes_suspected[LAWFLAG_BROWNIES]; //insanely illegal
         else if(law[LAW_DRUGS]==-1)g.sentence+=(3+LCSrandom(120))*g.crimes_suspected[LAWFLAG_BROWNIES]; //very illegal
         else if(law[LAW_DRUGS]==0)g.sentence+=(3+LCSrandom(12))*g.crimes_suspected[LAWFLAG_BROWNIES]; //moderately illegal
         // else not illegal

         g.sentence+=1*g.crimes_suspected[LAWFLAG_BREAKING];
         g.sentence+=(60+LCSrandom(181))*g.crimes_suspected[LAWFLAG_TERRORISM];
         g.sentence+=(30+LCSrandom(61))*g.crimes_suspected[LAWFLAG_BANKROBBERY];
         g.sentence+=(30+LCSrandom(61))*g.crimes_suspected[LAWFLAG_JURY];
         g.sentence+=(30+LCSrandom(61))*g.crimes_suspected[LAWFLAG_HELPESCAPE];
         g.sentence+=(3+LCSrandom(16))*g.crimes_suspected[LAWFLAG_ESCAPED];
         g.sentence+=(1+LCSrandom(1))*g.crimes_suspected[LAWFLAG_RESIST];
         g.sentence+=(6+LCSrandom(1))*g.crimes_suspected[LAWFLAG_EXTORTION];

         g.sentence+=(4+LCSrandom(3))*g.crimes_suspected[LAWFLAG_SPEECH];
         g.sentence+=1*g.crimes_suspected[LAWFLAG_VANDALISM];
         g.sentence+=(12+LCSrandom(12))*g.crimes_suspected[LAWFLAG_ARSON];
         g.sentence+=(12+LCSrandom(1))*g.crimes_suspected[LAWFLAG_ARMEDASSAULT];
         g.sentence+=(3+LCSrandom(1))*g.crimes_suspected[LAWFLAG_ASSAULT];
      }
      if(law[LAW_FLAGBURNING]==-2)
      {
         if(!LCSrandom(2))g.sentence+=(120+LCSrandom(241))*g.crimes_suspected[LAWFLAG_BURNFLAG];
         else if(g.crimes_suspected[LAWFLAG_BURNFLAG])g.sentence=-1*g.crimes_suspected[LAWFLAG_BURNFLAG];
      }
      else if(law[LAW_FLAGBURNING]==-1)g.sentence+=36*g.crimes_suspected[LAWFLAG_BURNFLAG];
      else if(law[LAW_FLAGBURNING]==0)g.sentence+=1*g.crimes_suspected[LAWFLAG_BURNFLAG];

      if((LCSrandom(4)-g.crimes_suspected[LAWFLAG_MURDER])>0)
      {
         if(!(g.sentence<0))g.sentence+=(120+LCSrandom(241))*g.crimes_suspected[LAWFLAG_MURDER];
      }
      else
      {
         if(g.sentence<0)g.sentence-=-1*g.crimes_suspected[LAWFLAG_MURDER];
         else if(g.crimes_suspected[LAWFLAG_MURDER])
            g.sentence=-1*g.crimes_suspected[LAWFLAG_MURDER];
      }
      if(g.sentence<0)g.sentence-=1*g.crimes_suspected[LAWFLAG_TREASON];
      else if(g.crimes_suspected[LAWFLAG_TREASON]) g.sentence=-1*g.crimes_suspected[LAWFLAG_TREASON];
      if(lenient&&g.sentence!=-1)g.sentence/=2;
      if(lenient&&g.sentence==-1)g.sentence=240+LCSrandom(120);
   }
   //LENIENCY AND CAPITAL PUNISHMENT DON'T MIX
   else if(g.deathpenalty&&lenient)
   {
      g.deathpenalty=0;
      g.sentence=-1;
   }

   //MENTION LENIENCY
   if(lenient)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(5,1);
      addstr("During sentencing, the judge grants some leniency.", gamelog);
      gamelog.newline();
      refresh();
      getch();
   }

   //MENTION SENTENCE
   if(olddeathpenalty)
   {
      g.deathpenalty=1;
      g.sentence=3;
      set_color(COLOR_RED,COLOR_BLACK,1);
      move(7,1);
      addstr(g.propername, gamelog);
      addstr(", you will be returned to prison to carry out your death sentence.", gamelog);
      gamelog.newline();
      refresh();
      getch();
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(9,1);
      addstr("The execution is scheduled to occur three months from now.", gamelog);
      refresh();
      getch();
   }
   else if(g.deathpenalty)
   {
      g.sentence=3;
      set_color(COLOR_RED,COLOR_RED,1);
      move(7,1);
      addstr(g.propername, gamelog);
      addstr(", you are sentenced to DEATH!", gamelog);
      gamelog.newline();
      refresh();
      getch();
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(9,1);
      addstr("The execution is scheduled to occur three months from now.", gamelog);
      refresh();
      getch();
   }
   // Don't give a time-limited sentence if they already have a life sentence.
   else if ((g.sentence>=0 && oldsentence<0) ||
            (g.sentence==0 && oldsentence>0))
   {
      g.sentence=oldsentence;
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(7,1);
      addstr(g.propername, gamelog);
      addstr(", the court sees no need to add to your existing sentence.", gamelog);
      refresh();
      getch();
   }
   else if(g.sentence==0)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(7,1);
      addstr(g.propername, gamelog);
      addstr(", consider this a warning.  You are free to go.", gamelog);
      refresh();
      getch();
   }
   else
   {
      if(g.sentence>=36)g.sentence-=g.sentence%12;

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(7,1);
      addstr(g.propername, gamelog);
      addstr(", you are sentenced to ", gamelog);
      if(g.sentence>1200) g.sentence/=-1200;

      if(g.sentence<=-1)
      {
         if(g.sentence<-1)
         {
            char num[20];
            itoa(-(g.sentence),num,10);
            addstr(num, gamelog);
            addstr(" consecutive life terms in prison", gamelog);
            gamelog.newline();

            // Don't bother saying this if the convicted already has one or
            // more life sentences. Makes the 'consecutively' and 'concurrently'
            // statements later easier to tack on.
            if(oldsentence>=0) 
            {
               addstr(".", gamelog);
               refresh();
               getch();
               move(9,1);
               addstr("Have a nice day, ", gamelog);
               addstr(g.propername, gamelog);
            }
         }
         else addstr("life in prison", gamelog);
      }
      else if(g.sentence>=36)
      {
         char num[20];
         itoa(g.sentence/12,num,10);
         addstr(num, gamelog);
         addstr(" years in prison", gamelog);
      }
      else
      {
         char num[20];
         itoa(g.sentence,num,10);
         addstr(num, gamelog);
         addstr(" month", gamelog);
         if(g.sentence>1)addstr("s", gamelog);
         addstr(" in prison", gamelog);
      }

      if((g.sentence>0 && oldsentence>0) ||
         (g.sentence<0 && oldsentence<0))
      {
         addstr(",", gamelog);
         move(8,1);
         if(lenient)
         {
            if(abs(oldsentence)>abs(g.sentence))
               g.sentence=oldsentence;
            addstr("to be served concurrently", gamelog);
         }
         else
         {
            g.sentence+=oldsentence;
            addstr("to be served consecutively", gamelog);
         }
      }

      addstr(".", gamelog);

      //dejuice boss
      int boss=getpoolcreature(g.hireid);
      if(boss!=-1&&pool[boss]->juice>50)
      {
         int juice=g.juice/10;
         if(juice<5)juice=5;
         addjuice(*pool[boss],-juice,0);
      }

      refresh();
      getch();
   }
   gamelog.nextMessage();
}



/* monthly - move a liberal to jail */
void imprison(Creature &g)
{
   g.location=find_site_index_in_city(SITE_GOVERNMENT_PRISON,location[g.location]->city);
}



/* monthly - advances a liberal's prison time or executes them */
//RETURNS IF SCREEN WAS ERASED
char prison(Creature &g)
{
	static const char *cruel_and_unusual_execution_methods[] =
	{
		"beheading",
		"drawing and quartering",
		"disemboweling",
		"one thousand cuts",
		"feeding the lions",
		"repeated gladiatorial death matches",
		"burning",
		"crucifixion",
		"head-squishing",
		"piranha tank swimming exhibition",
		"forced sucking of Ronald Reagan's ass",
		"covering with peanut butter and letting rats eat",
		"burying up to the neck in a fire ant nest",
		"running truck over the head",
		"drowning in a sewage digester vat",
		"chipper-shredder",
		"use in lab research",
		"blood draining",
		"chemical weapons test",
		"sale to a furniture maker",
		"sale to a CEO as a personal pleasure toy",
		"sale to foreign slave traders",
		"exposure to degenerate Bay 12 Curses games"
	};

	static const char *standard_execution_methods[] =
	{
		"lethal injection",
		"hanging",
		"firing squad",
		"electrocution"
	};

	static const char *supposedly_painless_execution_method =
		"lethal injection";

   char showed=0;

   // People not on death row or about to be released can have a scene in prison
   if(!g.deathpenalty && g.sentence!=1)
   {
      if(law[LAW_PRISONS]==2)
      {
         //Liberal therapy.
         reeducation(g);
      }
      else if(law[LAW_PRISONS]==-2)
      {
         //Labor camp.
         laborcamp(g);
      }
      else
      {
         //Normal prison.
         prisonscene(g);
      }
   }

   if(g.sentence>0)
   {
      //COMMUTE DEATH IN RIGHT CLIMATE
      if(g.deathpenalty&&law[LAW_DEATHPENALTY]==2)
      {
         erase();
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(8,1);
         addstr(g.name, gamelog);
         addstr("'s death sentence has been commuted to life, ", gamelog);
         move(9,1);
         addstr("due to the abolition of the death penalty.", gamelog);
         gamelog.nextMessage();
         refresh();
         getch();

         g.sentence=-1;
         g.deathpenalty=0;
         return 1;
      }

      //ADVANCE SENTENCE
      g.sentence--;
      if(g.sentence==0)
      {
         //EXECUTE
         if(g.deathpenalty)
         {
            erase();
            set_color(COLOR_RED,COLOR_BLACK,1);
            move(8,1);
            addstr("FOR SHAME:", gamelog);
            gamelog.newline();
            move(9,1);
            addstr("Today, the Conservative Machine executed ", gamelog);
            addstr(g.name, gamelog);
            gamelog.record(" "); //Log this for formatting purposes.
            move(10,1);
            addstr("by ", gamelog);
            if(law[LAW_DEATHPENALTY]==-2)
            {
            	addstr(selectRandomString(	cruel_and_unusual_execution_methods,
            										ARRAY_ELEMENTS(cruel_and_unusual_execution_methods)), gamelog);
            }
            else if(law[LAW_DEATHPENALTY]==-1||law[LAW_DEATHPENALTY]==0)
            {
            	addstr(selectRandomString(	standard_execution_methods,
            										ARRAY_ELEMENTS(standard_execution_methods)), gamelog);
            }
            else
            {
               addstr(supposedly_painless_execution_method, gamelog);
            }
            addstr(".", gamelog);
            refresh();
            getch();

            //dejuice boss
            int boss=getpoolcreature(g.hireid);
            if(boss!=-1)
            {
               gamelog.newline();
               set_color(COLOR_WHITE,COLOR_BLACK,0);
               move(12,1);
               addstr(pool[boss]->name, gamelog);
               addstr(" has failed the Liberal Crime Squad.", gamelog);
               gamelog.newline();
               move(14,1);
               addstr("If you can't protect your own people, who can you protect?", gamelog);
               refresh();
               getch();

               addjuice(*pool[boss],-50,-50);
            }
            gamelog.nextMessage();

            g.die();
            stat_dead++;
            showed=1;
         }
         //SET FREE
         else
         {
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(8,1);
            addstr(g.name, gamelog);
            addstr(" has been released from prison.", gamelog);
            gamelog.newline();
            move(9,1);
            addstr("No doubt there are some mental scars, but the Liberal is back.", gamelog);
            gamelog.nextMessage();
            refresh();
            getch();

            Armor clothes(*armortype[getarmortype("ARMOR_CLOTHES")]);
            g.give_armor(clothes,NULL);
            // If their old base is no longer under LCS control, wander back to the
            // homeless shelter instead.
            if(location[g.base]->renting<0)
            {
               for(int i=0;i<location.size();++i)
               {
                  if(location[i]->type==SITE_RESIDENTIAL_SHELTER)
                  {
                     g.base=i;
                     break;
                  }
               }
            }
            g.location=g.base;
            showed=1;
         }
      }
      //NOTIFY OF IMPENDING THINGS
      else if(g.sentence==1)
      {
         if(g.deathpenalty)
         {
            erase();
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(8,1);
            addstr(g.name, gamelog);
            addstr(" is due to be executed next month.", gamelog);
            gamelog.nextMessage();
            refresh();
            getch();

            showed=1;
         }
         else
         {
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(g.name, gamelog);
            addstr(" is due to be released next month.", gamelog);
            gamelog.nextMessage();
            refresh();
            getch();

            showed=1;
         }
      }
      else
      {
         if(g.deathpenalty)
         {
            char str[5];
            erase();
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(8,1);
            addstr(g.name, gamelog);
            addstr(" is due to be executed in ", gamelog);
            itoa(g.sentence, str, 10);
            addstr(str, gamelog);
            addstr(" months.", gamelog);
            gamelog.nextMessage();
            refresh();
            getch();

            showed=1;
         }
      }
   }

   return showed;
}

void reeducation(Creature &g)
{
	static const char *reeducation_experiences[] =
	{
	   " is subjected to rehabilitative therapy in prison.",
	   " works on a prison mural about political diversity.",
	   " routinely sees a Liberal therapist in prison.",
	   " participates in a group therapy session in prison.",
	   " sings songs with prisoners of all political persuasions.",
	   " is encouraged to befriend Conservatives in prison.",
	   " puts on an anti-crime performance in prison.",
	   " sees an video in prison by victims of political crime."
	};

   //int resist=0;

   //clearmessagearea();
   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(8,1);
   addstr(g.name, gamelog);
   addstr(selectRandomString(reeducation_experiences, ARRAY_ELEMENTS(reeducation_experiences)), gamelog);
   gamelog.newline();
   getch();

   move(10,1);
   if(!g.attribute_check(ATTRIBUTE_HEART,DIFFICULTY_FORMIDABLE))
   {
      if(g.juice>0 && LCSrandom(2))
      {
         addstr(g.name, gamelog);
         addstr(" feels bad about LCS actions, and loses juice!", gamelog);
         addjuice(g,-50,0);
      }
      else if(LCSrandom(15)>g.get_attribute(ATTRIBUTE_WISDOM,true)
           || g.get_attribute(ATTRIBUTE_WISDOM,true) < g.get_attribute(ATTRIBUTE_HEART,true))
      {
         addstr(g.name, gamelog);
         addstr(" silently grows Wiser...", gamelog);
         g.adjust_attribute(ATTRIBUTE_WISDOM,+1);
      }
      else if(g.align==ALIGN_LIBERAL && g.flag & CREATUREFLAG_LOVESLAVE && LCSrandom(4))
      {
         addstr(g.name, gamelog);
         addstr(" stays loyal to the LCS for ", gamelog);
         addstr(pool[g.hireid]->name, gamelog);
         addstr(".", gamelog);
      }
      else
      {
         addstr(g.name, gamelog);
         addstr(" abandons the Liberal Crime Squad!", gamelog);
         //conservatise(g);

         //Rat out contact
         int contact = getpoolcreature(g.hireid);
         if(contact >= 0)
         {
            criminalize(*pool[contact],LAWFLAG_RACKETEERING);
            pool[contact]->confessions++;
         }

         g.die();
         g.location=-1;
      }
   }
   else
   {
      addstr(g.name, gamelog);
      addstr(" remains strong.", gamelog);
   }
   gamelog.nextMessage();

   refresh();
   getch();
   erase();

   return;
}

void laborcamp(Creature &g)
{
	static const char *labor_camp_experiences[] =
	{
		" is forced to work hard labor in prison.",
		" operates dangerous machinery day after day in prison.",
		" is beaten by sadistic prison guards.",
		" carries heavy burdens back and forth in prison labor camp.",
		" does back-breaking work all month in prison.",
		" gets in a brutal fight with other distraught prisoners.",
		" participates in a quickly-suppressed prison riot.",
		" participates in a quickly-suppressed prison riot."
	};

   //int resist=0;

   //clearmessagearea();
   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(8,1);
   addstr(g.name, gamelog);
   addstr(selectRandomString(labor_camp_experiences, ARRAY_ELEMENTS(labor_camp_experiences)), gamelog);
   gamelog.newline();
   getch();

   move(10,1);
   if(!LCSrandom(10))
   {
      if(g.get_attribute(ATTRIBUTE_HEALTH, true) > 1)
      {
         addstr(g.name, gamelog);
         addstr(" is badly hurt in the process.", gamelog);
         addjuice(g,-40,0);
         addjuice(g,-10,-50);
      }
      else
      {
         addstr(g.name, gamelog);
         addstr(" is found dead.", gamelog);
         //conservatise(g);

         g.die();
         g.location=-1;
      }
   }
   else
   {
      addstr(g.name, gamelog);
      addstr(" carries on, regardless.", gamelog);
   }
   gamelog.nextMessage();

   refresh();
   getch();
   erase();

   return;
}

void prisonscene(Creature &g)
{
   // Populate this with scenes for normal prison
   return;
}
