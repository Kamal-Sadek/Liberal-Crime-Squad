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

/* monthly - hold trial on a liberal */
void trial(creaturest &g)
{
   g.sentence=0;
   g.deathpenalty=0;
   // If their old base is no longer under LCS control, wander back to the
   // homeless shelter instead.
   if(location[g.base]->renting==-1)
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
   addstr(g.name);
   addstr(" is standing trial.");
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
      if(g.lawflag[i])
      {
         typenum++;
         scarefactor+=lawflagheat(i)*g.lawflag[i];
         breaker[i]=1;
      }
   }

   //CHECK FOR SLEEPERS
   vector<int> sjudge;
   char sleeperlawyer=0;
   char sleeperjudge=0;
   bool autoconvict=0;
   char *sleeperjname=NULL;
   char *sleepername=NULL;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->alive&&(pool[p]->flag & CREATUREFLAG_SLEEPER))
      {
         if(pool[p]->type==CREATURE_JUDGE_CONSERVATIVE||
            pool[p]->type==CREATURE_JUDGE_LIBERAL)
         {
            sjudge.push_back(p);
         }
         if(pool[p]->type==CREATURE_LAWYER&&!sleeperlawyer)
         {
            sleeperlawyer=1;
            sleepername=pool[p]->name;
         }
      }
   }

   if(LCSrandom(10)<sjudge.size())
   {
      sleeperjudge=1;
      sleeperjname=pool[sjudge[LCSrandom(sjudge.size())]]->name;
   }

   //STATE CHARGES
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(3,1);
   if(sleeperjudge)
   {
      addstr("Sleeper ");
      addstr(sleeperjname);
      addstr(" reads the charges, trying to hide a smile:");
   }
   else addstr("The judge reads the charges:");
   set_color(COLOR_RED,COLOR_BLACK,1);
   move(5,1);
   addstr("The defendant, ");
   addstr(g.propername);
   addstr(", is charged with ");
   int x=2,y=5;
   while(typenum>0)
   {
      typenum--;

      x++;
      if(x>=2){x=0;y++;move(y,1);}

      if(breaker[LAWFLAG_TREASON])
      {
         if(g.lawflag[LAWFLAG_TREASON]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_TREASON],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("treason");
         breaker[LAWFLAG_TREASON]=0;
      }
      else if(breaker[LAWFLAG_MURDER])
      {
         if(g.lawflag[LAWFLAG_MURDER]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_MURDER],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("murder");
         breaker[LAWFLAG_MURDER]=0;
      }
      else if(breaker[LAWFLAG_TERRORISM])
      {
         if(g.lawflag[LAWFLAG_TERRORISM]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_TERRORISM],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("terrorism");
         breaker[LAWFLAG_TERRORISM]=0;
      }
      else if(breaker[LAWFLAG_KIDNAPPING])
      {
         if(g.lawflag[LAWFLAG_KIDNAPPING]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_KIDNAPPING],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("kidnapping");
         breaker[LAWFLAG_KIDNAPPING]=0;
      }
      else if(breaker[LAWFLAG_BURNFLAG])
      {
         if(g.lawflag[LAWFLAG_BURNFLAG]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_BURNFLAG],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         if(law[LAW_FLAGBURNING]==-2)
            addstr("Flag Murder");
         else if(law[LAW_FLAGBURNING]==-1)
            addstr("felony flag burning");
         else if(law[LAW_FLAGBURNING]==0)
            addstr("flag burning");
         breaker[LAWFLAG_BURNFLAG]=0;
      }
      else if(breaker[LAWFLAG_SPEECH])
      {
         if(g.lawflag[LAWFLAG_SPEECH]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_SPEECH],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("harmful speech");
         breaker[LAWFLAG_SPEECH]=0;
      }
      else if(breaker[LAWFLAG_BROWNIES])
      {
         if(g.lawflag[LAWFLAG_BROWNIES]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_BROWNIES],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("selling drugs");
         breaker[LAWFLAG_BROWNIES]=0;
      }
      else if(breaker[LAWFLAG_ESCAPED])
      {
         if(g.lawflag[LAWFLAG_ESCAPED]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_ESCAPED],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("escaping prison");
         breaker[LAWFLAG_ESCAPED]=0;
         autoconvict=1; // *Impossible* to beat this charge
      }
      else if(breaker[LAWFLAG_HELPESCAPE])
      {
         if(g.lawflag[LAWFLAG_HELPESCAPE]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_HELPESCAPE],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("aiding a prison escape");
         breaker[LAWFLAG_HELPESCAPE]=0;
         x=2;
      }
      else if(breaker[LAWFLAG_JURY])
      {
         if(g.lawflag[LAWFLAG_JURY]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_JURY],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("jury tampering");
         breaker[LAWFLAG_JURY]=0;
      }
      else if(breaker[LAWFLAG_RACKETEERING])
      {
         addstr("racketeering");
         breaker[LAWFLAG_RACKETEERING]=0;
      }
      else if(breaker[LAWFLAG_ASSAULT])
      {
         if(g.lawflag[LAWFLAG_ASSAULT]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_ASSAULT],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("assault");
         breaker[LAWFLAG_ASSAULT]=0;
      }
      else if(breaker[LAWFLAG_GUNCARRY])
      {
         if(g.lawflag[LAWFLAG_GUNCARRY]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_GUNCARRY],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("carrying an illegal weapon");
         breaker[LAWFLAG_GUNCARRY]=0;
         x=2;
      }
      else if(breaker[LAWFLAG_CARTHEFT])
      {
         if(g.lawflag[LAWFLAG_CARTHEFT]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_CARTHEFT],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("car theft");
         breaker[LAWFLAG_CARTHEFT]=0;
      }
      else if(breaker[LAWFLAG_CCFRAUD])
      {
         if(g.lawflag[LAWFLAG_CCFRAUD]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_CCFRAUD],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("credit card fraud");
         breaker[LAWFLAG_CCFRAUD]=0;
      }
      else if(breaker[LAWFLAG_THEFT])
      {
         if(g.lawflag[LAWFLAG_THEFT]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_THEFT],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("theft");
         breaker[LAWFLAG_THEFT]=0;
      }
      else if(breaker[LAWFLAG_PROSTITUTION])
      {
         if(g.lawflag[LAWFLAG_PROSTITUTION]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_PROSTITUTION],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("prostitution");
         breaker[LAWFLAG_PROSTITUTION]=0;
      }
      else if(breaker[LAWFLAG_HIREILLEGAL])
      {
         if(g.lawflag[LAWFLAG_HIREILLEGAL]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_HIREILLEGAL],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("hiring an illegal alien");
         breaker[LAWFLAG_HIREILLEGAL]=0;
         x=2;
      }
      else if(breaker[LAWFLAG_COMMERCE])
      {
         if(g.lawflag[LAWFLAG_COMMERCE]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_COMMERCE],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("interference with interstate commerce");
         breaker[LAWFLAG_COMMERCE]=0;
         x=2;
      }
      else if(breaker[LAWFLAG_INFORMATION])
      {
         if(g.lawflag[LAWFLAG_INFORMATION]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_INFORMATION],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("unlawful access of an information system");
         breaker[LAWFLAG_INFORMATION]=0;
         x=2;
      }
      else if(breaker[LAWFLAG_BURIAL])
      {
         if(g.lawflag[LAWFLAG_BURIAL]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_BURIAL],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("unlawful burial");
         breaker[LAWFLAG_BURIAL]=0;
      }
      else if(breaker[LAWFLAG_BREAKING])
      {
         if(g.lawflag[LAWFLAG_BREAKING]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_BREAKING],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("breaking and entering");
         breaker[LAWFLAG_BREAKING]=0;
      }
      else if(breaker[LAWFLAG_VANDALISM])
      {
         if(g.lawflag[LAWFLAG_VANDALISM]>1)
         {
            char str[10];
            itoa(g.lawflag[LAWFLAG_VANDALISM],str,10);
            addstr(str);
            addstr(" counts of ");
         }
         addstr("vandalism");
         breaker[LAWFLAG_VANDALISM]=0;
      }
      else if(breaker[LAWFLAG_RESIST])
      {
         addstr("resisting arrest");
         breaker[LAWFLAG_RESIST]=0;
      }
      else if(breaker[LAWFLAG_DISTURBANCE])
      {
         addstr("disturbing the peace");
         breaker[LAWFLAG_DISTURBANCE]=0;
      }
      else if(breaker[LAWFLAG_LOITERING])
      {
         addstr("loitering");
         breaker[LAWFLAG_LOITERING]=0;
      }

      if(typenum>1)addstr(", ");
      if(typenum==1)addstr(" and ");
      if(typenum==0)addstr(".");

      refresh();
      getch();
   }

   if(g.confessions)
   {
      move(y+=2,1);

      if(g.confessions>1)
      {
         char str[10];
         itoa(g.confessions,str,10);
         addstr(str);
         addstr(" former LCS members will testify against ");
      }
      else addstr("A former LCS member will testify against ");
      addstr(g.name);
      addstr(".");
      refresh();
      getch();
   }

   //CHOOSE DEFENSE
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(y+2,1);
   addstr("How will you conduct the defense?");

   char attorneyname[200];
   unsigned long oldseed=seed;
   seed=attorneyseed;
   name(attorneyname);
   seed=oldseed;

   y+=4;
   move(y,1);y++;
   addstr("A - Use a court-appointed attorney.");
   move(y,1);y++;
   addstr("B - Defend self!");
   move(y,1);y++;
   addstr("C - Plead guilty.");
   if(sleeperlawyer)
   {
      move(y,1);y++;
      addstr("D - Use Sleeper ");
      addstr(sleepername);
      addstr(" as your attorney.");
   }
   else if(funds>=5000) // *JDS* hiring a good attorney is now more expensive
   {
      move(y,1);y++;
      addstr("D - Use $5000 to hire ace Liberal attorney ");
      addstr(attorneyname);
      addstr(".");
   }

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
      if(c=='d'&&sleeperlawyer)
      {
         defense=3;
         break;
      }
      else if(c=='d'&&funds>=5000)
      {
         funds-=5000;
         stat_spent+=5000;
         defense=3;
         moneylost_legal+=5000;
         break;
      }
   }while(1);

   //TRIAL
   if(defense!=2)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(1,1);
      addstr(g.name);
      addstr(" is standing trial.");

      //TRIAL MESSAGE
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(3,1);
      addstr("The trial proceeds.  Jury selection is first.");
      refresh();
      getch();

      //JURY MAKEUP MESSAGE
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(5,1);
      int jury=LCSrandom(61)-(60*publicmood(-1))/100; // Political leanings of the population determine your jury
      if(sleeperjudge)jury-=20;
      if(jury<=-29)
      {
         switch(LCSrandom(4))
         {
            case 0:addstr(g.name);addstr("'s best friend from childhood is a juror.");break;
            case 1:addstr("The jury is Flaming Liberal.");break;
            case 2:addstr("Four of the jurors are closet Socialists.");break;
            case 3:addstr("One of the jurors flashes a SECRET LIBERAL HAND SIGNAL when no one is looking.");break;
         }
      }
      else if(jury<=-15)addstr("The jury is fairly Liberal.");
      else if(jury<15)addstr("The jury is quite moderate.");
      else if(jury<29)addstr("The jury is a bit Conservative.");
      else
      {
         switch(LCSrandom(4))
         {
            case 0:addstr("Such a collection of Conservative jurors has never before been assembled.");break;
            case 1:addstr("One of the accepted jurors is a Conservative activist.");break;
            case 2:addstr("Three of the jurors are members of the KKK.");break;
            case 3:addstr("The jury is frighteningly Conservative.");break;
         }
      }
      refresh();
      getch();

      //PROSECUTION MESSAGE
      int prosecution;
      // *JDS* The bigger your record, the stronger the evidence
      prosecution=40+LCSrandom(61);
      prosecution+=scarefactor;
      prosecution+=20*g.confessions;
      if(sleeperjudge)prosecution>>=1;

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(7,1);

      if(autoconvict)
      {
         addstr("There is no question of ");
         addstr(g.name);
         addstr("'s guilt.");
      }
      else
      {
         if(prosecution<=50)addstr("The prosecution's presentation is terrible.");
         else if(prosecution<=75)addstr("The prosecution gives a standard presentation.");
         else if(prosecution<=100)addstr("The prosecution's case is solid.");
         else if(prosecution<=150)addstr("The prosecution makes an airtight case.");
         else
         {
            addstr(g.name);
            addstr("'s chances are beyond bleak.");
         }
      }

      refresh();
      getch();

      jury+=LCSrandom(prosecution/2+1)+prosecution/2;

      //DEFENSE MESSAGE
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(9,1);

      int defensepower=0;
      if(defense==0||defense==3)
      {
         if(autoconvict)
         {
            addstr("The defense makes a noble attempt, but the outcome is inevitable.");
         }
         else
         {
            if(defense==0)defensepower=LCSrandom(71); // *JDS* gave you a better shake with the court-appointed attorney
            else
            {
               defensepower=LCSrandom(101)+50;
            }

            if(defensepower<=15)addstr("The defense attorney accidentally said \"My client is GUILTY!\" during closing.");
            else if(defensepower<=25)addstr("The defense is totally lame.");
            else if(defensepower<=50)addstr("The defense was lackluster.");
            else if(defensepower<=75)addstr("Defense arguments were pretty good.");
            else if(defensepower<=100)addstr("The defense was really slick.");
            else if(defensepower<=145)
            {
               if(prosecution<100)addstr("The defense makes the prosecution look like amateurs.");
               else
               {
                  addstr("The defense is extremely compelling.");
               }
            }
            else
            {
               if(prosecution<100)
               {
                  addstr(attorneyname);
                  addstr("'s arguments made several of the jurors stand up");
                  move(10,1);
                  addstr("and shout \"NOT GUILTY!\" before deliberations even began.");
               }
               else
               {
                  addstr(attorneyname);
                  addstr(" conducts an incredible defense.");
               }
            }
         }
      }
      if(defense==1)
      {
         // *JDS* LEGAL SELF-REPRESENTATION: Your intelligence, heart, and twice your charisma
         // form the baseline the jury will evaluate your arguments from, and after that it's
         // up to your roll. To succeed here, you really need to have two skills be
         // high: persuasion and law, with law being 1.5 times as influential. You can't have
         // just one or just the other, and that's a change I've made which will make this
         // much harder. Even if you're a very persuasive person, the court will eat you alive
         // if you can't sound intelligent when talking about the relevant charges, and you
         // won't be able to fool the jury into letting you go if you aren't persuasive, as no
         // matter how encyclopedic your legal knowledge is, it's all in the pitch. After these
         // skills are multiplied together, you make a roll on this "defense skill" value; this curves your
         // results toward a central value, and away from extreme good and bad performances.
         // All in all, it is significantly *harder* to defend yourself after these changes,
         // and a very powerful case will require someone have lawyerly skills to begin with.
         // Major note: Defense skill is capped at 100. Once you're rolling d200, adding more
         // would be really unrealistic. You can only be so persuasive.
         //
         // EXAMPLE: Suppose your Liberal has 4 persuasion and 6 law -- the average starting
         // skills for a seduced lawyer. This results in a defenseskill of 51. If their intelligence,
         // heart, and charisma are each four, that makes their roll 16+LCSrandom(102).
         // This is significantly weaker than a paid or Sleeper attorney, but much stronger
         // than the public defender, and about on par with the prosecutor so long as it isn't
         // a high-profile case.

         // A character build spefically to be strong in this area *will* still start out
         // slightly stronger than the public defender (and will be notably better once they
         // hit activist level).
         int defenseskill=3*g.skill[SKILL_PERSUASION]+6*g.skill[SKILL_LAW];
         defensepower+=g.attval(ATTRIBUTE_INTELLIGENCE);
         defensepower+=g.attval(ATTRIBUTE_HEART);
         defensepower+=g.attval(ATTRIBUTE_CHARISMA)*2;
         defensepower+=LCSrandom(min(defenseskill*2,max(200,prosecution+100)));
         g.skill_ip[SKILL_PERSUASION]+=max(50-g.skill[SKILL_PERSUASION]*2,0);
         g.skill_ip[SKILL_LAW]+=max(50-g.skill[SKILL_LAW]*2,0);

         if(autoconvict)
         {
            if(defensepower<=50)
            {
               addstr(g.name);
               addstr("'s defense is humiliating.");
            }
            else
            {
               addstr("The jury is completely impassive as ");
               addstr(g.name);
               addstr(" argues the defense.");
            }
         }
         else
         {
            addstr(g.name);
            if(defensepower<=15)addstr("'s defense looks like Colin Ferguson's.");
            else if(defensepower<=25)addstr("'s case really sucked.");
            else if(defensepower<=50)addstr(" did all right, but made some mistakes.");
            else if(defensepower<=75)addstr("'s arguments were pretty good.");
            else if(defensepower<=100)addstr(" worked the jury very well.");
            else if(defensepower<=150)addstr(" made a very powerful case.");
            else
            {
               addstr(" had the jurors crying for freedom.");
            }
         }
      }

      refresh();
      getch();

      //DELIBERATION MESSAGE
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(12,1);
      addstr("The jury leaves to consider the case.");
      refresh();
      getch();

      //JURY RETURN MESSAGE
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(1,1);
      addstr("The jury has returned from deliberations.");
      refresh();
      getch();

      //HUNG JURY
      if(defensepower==jury)
      {
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         move(3,1);
         addstr("But they can't reach a verdict!");
         refresh();
         getch();

         //RE-TRY
         if(LCSrandom(2)||scarefactor>=10||g.confessions)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(5,1);
            addstr("The case will be re-tried next month.");
            refresh();
            getch();

            long ps=-1;
            for(long l=0;l<location.size();l++)
            {
               if(location[l]->type==SITE_GOVERNMENT_COURTHOUSE)
               {
                  ps=l;
               }
            }
            g.location=ps;
         }
         //NO RE-TRY
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(5,1);
            addstr("The prosecution declines to re-try the case.");
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            move(7,1);
            addstr(g.name);
            addstr(" is free!");
            refresh();
            getch();
         }
      }
      //ACQUITTAL!
      else if(!autoconvict && defensepower>jury)
      {
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(3,1);
         addstr("NOT GUILTY!");
         refresh();
         getch();
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(5,1);
         addstr(g.name);
         addstr(" is free!");
         refresh();
         getch();
      }
      //LENIENCE
      else if(!autoconvict && (defensepower/3>=jury/4 || sleeperjudge)) // *JDS* sleeper judge will always reduce the sentence
      {
         penalize(g,1);
      }
      else
      {
         penalize(g,0);
      }
      //CLEAN UP LAW FLAGS
      for(int i=0;i<LAWFLAGNUM;i++)
      {
         g.lawflag[i]=0;
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
         g.armor.type=ARMOR_CLOTHES;
         g.armor.quality='1';
         g.armor.flag=0;
      }
   }
   //GUILTY PLEA
   else
   {
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(1,1);
      addstr("The court accepts the plea.");
      refresh();
      getch();

      penalize(g,LCSrandom(2));
      //CLEAN UP LAW FLAGS
      for(int i=0;i<LAWFLAGNUM;i++)
      {
         g.lawflag[i]=0;
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
         g.armor.type=ARMOR_CLOTHES;
         g.armor.quality='1';
         g.armor.flag=0;
      }
   }
}



/* monthly - sentence a liberal */
void penalize(creaturest &g,char lenient)
{
   set_color(COLOR_RED,COLOR_BLACK,1);
   move(3,1);
   addstr("GUILTY!");
   refresh();
   getch();

   if(g.lawflag[LAWFLAG_ESCAPED])
   {
      lenient=0;
      if(law[LAW_DEATHPENALTY]==2)g.deathpenalty=0;
   }
   else g.deathpenalty=0;

   if(!lenient&&((g.lawflag[LAWFLAG_MURDER])||(g.lawflag[LAWFLAG_TREASON])||
      ((g.lawflag[LAWFLAG_BURNFLAG])&&law[LAW_FLAGBURNING]==-2)||
      law[LAW_DEATHPENALTY]==-2))
   {
      if(law[LAW_DEATHPENALTY]==-2)g.deathpenalty=1;
      if(law[LAW_DEATHPENALTY]==-1)g.deathpenalty=LCSrandom(3);
      if(law[LAW_DEATHPENALTY]==0)g.deathpenalty=LCSrandom(2);
      if(law[LAW_DEATHPENALTY]==1)g.deathpenalty=!LCSrandom(5);
      if(law[LAW_DEATHPENALTY]==2)g.deathpenalty=0;
   }

   //CALC TIME
   if(!g.deathpenalty)
   {
      if(!(g.sentence<0))
      {
         g.sentence+=(36+LCSrandom(18))*g.lawflag[LAWFLAG_KIDNAPPING];
         g.sentence+=(1+LCSrandom(4))*g.lawflag[LAWFLAG_THEFT];
         g.sentence+=(4+LCSrandom(12))*(!!g.lawflag[LAWFLAG_GUNCARRY]);
         g.sentence+=(6+LCSrandom(7))*g.lawflag[LAWFLAG_CARTHEFT];
         g.sentence+=(1+LCSrandom(13))*g.lawflag[LAWFLAG_INFORMATION];
         g.sentence+=(1+LCSrandom(13))*g.lawflag[LAWFLAG_COMMERCE];
         g.sentence+=(6+LCSrandom(25))*g.lawflag[LAWFLAG_CCFRAUD];
         g.sentence+=(3+LCSrandom(12))*g.lawflag[LAWFLAG_BURIAL];
         g.sentence+=(1+LCSrandom(6))*g.lawflag[LAWFLAG_PROSTITUTION];
         g.sentence+=1*g.lawflag[LAWFLAG_DISTURBANCE];
         //g.sentence+=1*g.lawflag[LAWFLAG_LOITERING];
         g.sentence+=1*g.lawflag[LAWFLAG_HIREILLEGAL];
         g.sentence+=(12+LCSrandom(100))*g.lawflag[LAWFLAG_RACKETEERING];
         if(LCSrandom(3))g.sentence+=(3+LCSrandom(12))*g.lawflag[LAWFLAG_BROWNIES];
         else
         {
            if(LCSrandom(3))g.sentence+=(3+LCSrandom(120))*g.lawflag[LAWFLAG_BROWNIES];
            else g.sentence+=(3+LCSrandom(360))*g.lawflag[LAWFLAG_BROWNIES];
         }
         g.sentence+=1*g.lawflag[LAWFLAG_BREAKING];
         g.sentence+=(60+LCSrandom(181))*g.lawflag[LAWFLAG_TERRORISM];
         g.sentence+=(30+LCSrandom(61))*g.lawflag[LAWFLAG_JURY];
         g.sentence+=(30+LCSrandom(61))*g.lawflag[LAWFLAG_HELPESCAPE];
         g.sentence+=(1+LCSrandom(1))*g.lawflag[LAWFLAG_RESIST];
         
         g.sentence+=(4+LCSrandom(3))*g.lawflag[LAWFLAG_SPEECH];
         g.sentence+=1*g.lawflag[LAWFLAG_VANDALISM];
         g.sentence+=(3+LCSrandom(1))*g.lawflag[LAWFLAG_ASSAULT];
      }
      if(law[LAW_FLAGBURNING]==-2)
      {
         if(!LCSrandom(2))g.sentence+=(120+LCSrandom(241))*g.lawflag[LAWFLAG_BURNFLAG];
         else if(g.lawflag[LAWFLAG_BURNFLAG])g.sentence=-1*g.lawflag[LAWFLAG_BURNFLAG];
      }
      else if(law[LAW_FLAGBURNING]==-1)g.sentence+=36*g.lawflag[LAWFLAG_BURNFLAG];
      else if(law[LAW_FLAGBURNING]==0)g.sentence+=1*g.lawflag[LAWFLAG_BURNFLAG];
      
      if((LCSrandom(4)-g.lawflag[LAWFLAG_MURDER])>0)
      {
         if(!(g.sentence<0))g.sentence+=(120+LCSrandom(241))*g.lawflag[LAWFLAG_MURDER];
      }
      else
      {
         if(!(g.sentence<0))g.sentence-=-1*g.lawflag[LAWFLAG_MURDER];
         else if(g.lawflag[LAWFLAG_BURNFLAG])g.sentence=-1*g.lawflag[LAWFLAG_MURDER];
      }
      if(g.sentence<0)g.sentence-=1*g.lawflag[LAWFLAG_ESCAPED];
      else if(g.lawflag[LAWFLAG_ESCAPED]) g.sentence=-1*g.lawflag[LAWFLAG_ESCAPED];
      if(g.sentence<0)g.sentence-=1*g.lawflag[LAWFLAG_TREASON];
      else if(g.lawflag[LAWFLAG_TREASON]) g.sentence=-1*g.lawflag[LAWFLAG_TREASON];
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
      addstr("During sentencing, the judge grants some leniency.");
      refresh();
      getch();
   }

   //MENTION SENTENCE
   if(g.deathpenalty)
   {
      g.sentence=3;
      set_color(COLOR_RED,COLOR_RED,1);
      move(7,1);
      addstr(g.propername);
      addstr(", you are sentenced to DEATH!");
      refresh();
      getch();
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(9,1);
      addstr("The execution is scheduled to occur three months from now.");
      refresh();
      getch();
   }
   else if(g.sentence==0)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(7,1);
      addstr(g.propername);
      addstr(", consider this a warning.  You are free to go.");
      refresh();
      getch();
   }
   else
   {
      if(g.sentence>=36)g.sentence-=g.sentence%12;

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(7,1);
      addstr(g.propername);
      addstr(", you are sentenced to ");
      if(g.sentence>1200) g.sentence/=-1200;
      
      if(g.sentence<=-1)
      {
         if(g.sentence<-1)
         {
            char num[20];
            itoa(-(g.sentence),num,10);
            addstr(num);
            addstr(" consecutive life terms in prison.");
            
            refresh();
            getch();

            move(9,1);
            addstr("Have a nice day, ");
            addstr(g.propername);
            addstr(".");
         }
         else addstr("life in prison.");
      }
      else if(g.sentence>=36)
      {
         char num[20];
         itoa(g.sentence/12,num,10);
         addstr(num);
         addstr(" year");
         if(g.sentence/12>1)addstr("s");
         addstr(" in prison.");
      }
      else
      {
         char num[20];
         itoa(g.sentence,num,10);
         addstr(num);
         addstr(" month");
         if(g.sentence>1)addstr("s");
         addstr(" in prison.");
      }

      //dejuice boss
      int boss=getpoolcreature(g.hireid);
      if(boss!=-1&&pool[boss]->juice>50)
      {
         int juice=pool[boss]->juice-50;
         if(juice>5)juice=5;
         addjuice(*pool[boss],-juice);
      }

      refresh();
      getch();
   }
}



/* monthly - move a liberal to jail */
void imprison(creaturest &g)
{
   long ps=-1;
   for(long l=0;l<location.size();l++)
   {
      if(location[l]->type==SITE_GOVERNMENT_PRISON)
      {
         ps=l;
      }
   }
   g.location=ps;
}



/* monthly - advances a liberal's prison time or executes them */
//RETURNS IF SCREEN WAS ERASED
char prison(creaturest &g)
{
   char showed=0;

   if(g.sentence>0)
   {
      //COMMUTE DEATH IN RIGHT CLIMATE
      if(g.deathpenalty&&law[LAW_DEATHPENALTY]==2)
      {
         erase();
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(8,1);
         addstr(g.name);
         addstr("'s death sentence has been commuted to life,");
         move(9,1);
         addstr("due to the abolition of the death penalty.");
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
            addstr("FOR SHAME:");
            move(9,1);
            addstr("Today, the Conservative Machine executed ");
            addstr(g.name);
            move(10,1);
            addstr("by ");
            if(law[LAW_DEATHPENALTY]==-2)
            {
               switch(LCSrandom(23))
               {
                  case 0:addstr("beheading");break;
                  case 1:addstr("drawing and quartering");break;
                  case 2:addstr("disemboweling");break;
                  case 3:addstr("one thousand cuts");break;
                  case 4:addstr("feeding the lions");break;
                  case 5:addstr("repeated gladiatory death matches");break;
                  case 6:addstr("burning");break;
                  case 7:addstr("crucifixion");break;
                  case 8:addstr("head-squishing");break;
                  case 9:addstr("piranha tank swimming exhibition");break;
                  case 10:addstr("forced sucking of Ronald Reagan's ass");break;
                  case 11:addstr("covering with peanut butter and letting rats eat");break;
                  case 12:addstr("burying up to the neck in a fire ant nest");break;
                  case 13:addstr("running truck over the head");break;
                  case 14:addstr("drowning in a sewage digestor vat");break;
                  case 15:addstr("chipper-shredder");break;
                  case 16:addstr("use in lab research");break;
                  case 17:addstr("blood draining");break;
                  case 18:addstr("chemical weapons test");break;
                  case 19:addstr("sale to a furniture maker");break;
                  case 20:addstr("sale to a CEO as a personal pleasure toy");break;
                  case 21:addstr("sale to foreign slave traders");break;
                  case 22:addstr("exposure to degenerate Bay 12 Curses games");break;
               }
            }
            else if(law[LAW_DEATHPENALTY]==-1||law[LAW_DEATHPENALTY]==0)
            {
               switch(LCSrandom(4))
               {
                  case 0:addstr("lethal injection");break;
                  case 1:addstr("hanging");break;
                  case 2:addstr("firing squad");break;
                  case 3:addstr("electrocution");break;
               }
            }
            else
            {
               addstr("lethal injection");
            }
            addstr(".");
            refresh();
            getch();

            //dejuice boss
            int boss=getpoolcreature(g.hireid);
            if(boss!=-1)
            {
               addjuice(*pool[boss],-15);
            }

            g.alive=0;
            stat_dead++;
            showed=1;
         }
         //SET FREE
         else
         {
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(8,1);
            addstr(g.name);
            addstr(" has been released from prison.");
            move(9,1);
            addstr("No doubt there are some mental scars, but the Liberal is back.");
            refresh();
            getch();

            g.armor.type=ARMOR_CLOTHES;
            g.armor.quality='1';
            g.armor.flag=0;
            // If their old base is no longer under LCS control, wander back to the
            // homeless shelter instead.
            if(location[g.base]->renting==-1)
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
      if(g.sentence==1)
      {
         if(g.deathpenalty)
         {
            erase();
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(8,1);
            addstr(g.name);
            addstr(" is due to be executed next month.");
            refresh();
            getch();

            showed=1;
         }
         else
         {
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(g.name);
            addstr(" is due to be released next month.");
            refresh();
            getch();

            showed=1;
         }
      }
   }

   return showed;
}
