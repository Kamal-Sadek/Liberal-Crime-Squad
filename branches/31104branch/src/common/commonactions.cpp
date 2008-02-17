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


/* common - test for possible game over */
char endcheck(short cause)
{
   char dead=1;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->alive&&
         pool[p]->align==1&&
         !(pool[p]->flag & CREATUREFLAG_SLEEPER))
      {
         dead=0;
         break;
      }
   }

   if(dead)
   {
      if(cause==-1)
      {
         if(location[cursite]->siege.siege)
         {
            switch(location[cursite]->siege.siegetype)
            {
               case SIEGE_POLICE:savehighscore(END_POLICE);break;
               case SIEGE_CIA:savehighscore(END_CIA);break;
               case SIEGE_HICKS:savehighscore(END_HICKS);break;
               case SIEGE_CORPORATE:savehighscore(END_CORP);break;
            }
         } 
         else savehighscore(END_DEAD);
      }
      else savehighscore(cause);

      reset();
      viewhighscores();
      end_game();
   }

   return 0;
}


/* common - detatches all liberals from a specified car */
//GETS RID OF CAR PREFERENCES FROM pool LIBERALS, BY CAR ID NUMBER
void removecarprefs_pool(long carid)
{
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->pref_carid==carid)
      {
         pool[p]->pref_carid=-1;
      }
   }
}

/* common - tests if the person is a wanted criminal */
// *JDS* Checks if the character is a criminal
bool iscriminal(creaturest &cr)
{
   for(int i=0;i<LAWFLAGNUM;i++)
      if(cr.lawflag[i])
         return 1;
   return 0;
}



/* common - sends somebody to the hospital */
/***************************************************
*JDS* Hospitalize -- sends current person to the
specified clinic or hospital.
***************************************************/
void hospitalize(int loc, creaturest &patient)
{
   // He's dead, Jim
   if(!patient.alive)return;

   int time=clinictime(patient);

   if(time>0)
   {
      squadst* patientsquad = NULL;
      if(patient.squadid!=-1)
         patientsquad = squad[getsquad(patient.squadid)];
      patient.clinic=time;
      patient.squadid=-1;
      patient.location=loc;

      char num[20];

      // Inform about the hospitalization
      makedelimiter(8,0);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(patient.name);
      addstr(" will be at ");
      addstr(location[loc]->name);
      addstr(" for ");
      itoa(time,num,10);
      addstr(num);
      addstr(" ");
      if(time>1)addstr("weeks");
      else addstr("week");
      addstr(".");

      refresh();
      getch();

      if(patientsquad)
      {
         for(int p=0;p<6;p++)
         {
            if(patientsquad->squad[p]==&patient)
               patientsquad->squad[p]=NULL;
         }

         // Reorganize patient's squad
         for(int i=0;i<5;i++)
         {
            for(int p2=1;p2<6;p2++)
            {
               if(patientsquad->squad[p2-1]==NULL&&patientsquad->squad[p2]!=NULL)
               {
                  patientsquad->squad[p2-1]=patientsquad->squad[p2];
                  patientsquad->squad[p2]=NULL;
               }
            }
         }
         testsquadclear(*patientsquad, patient.base); 
      }  
   }
}



/* common - determines how long a creature's injuries will take to heal */
int clinictime(creaturest &g)
{
   int time=0;

   for(int w=0;w<BODYPARTNUM;w++)
   {
      if(((g.wound[w] & WOUND_NASTYOFF))&&
          (g.blood < 100))
      {
         time++;
      }
   }
   if(g.blood<=10)time++;
   if(g.blood<=50)time++;
   if(g.blood<100)time++;
   if(!g.special[SPECIALWOUND_RIGHTLUNG])time++;
   if(!g.special[SPECIALWOUND_LEFTLUNG])time++;
   if(!g.special[SPECIALWOUND_HEART])time+=2;
   if(!g.special[SPECIALWOUND_LIVER])time++;
   if(!g.special[SPECIALWOUND_STOMACH])time++;
   if(!g.special[SPECIALWOUND_RIGHTKIDNEY])time++;
   if(!g.special[SPECIALWOUND_LEFTKIDNEY])time++;
   if(!g.special[SPECIALWOUND_SPLEEN])time++;
   if(g.special[SPECIALWOUND_RIBS]<RIBNUM)time++;
   if(!g.special[SPECIALWOUND_NECK])time++;
   if(!g.special[SPECIALWOUND_UPPERSPINE])time++;
   if(!g.special[SPECIALWOUND_LOWERSPINE])time++;

   return time;
}



/* common - purges squad of loot and vehicles if it has no members */
/***************************************************
*JDS* testsquadclear - 
Clears the squad of loot and cars if it has no
members. Returns 0 if squad is okay, returns 1 if
squad is cleared.
***************************************************/
int testsquadclear(squadst &thissquad, int obase)
{
   //SPLIT IF SQUAD IS GONE
   char hasmembers=0;
   for(int p=0;p<6;p++)
   {
      if(thissquad.squad[p]!=NULL)hasmembers=1;
   }
   if(!hasmembers)
   {
      //LOSE ALL CARS
      for(int p=0;p<6;p++)
      {
         if(thissquad.squad[p]==NULL)continue;
         if(thissquad.squad[p]->carid!=-1)
         {
            long v=id_getcar(thissquad.squad[p]->carid);
            if(v!=-1)
            {
               removecarprefs_pool(vehicle[v]->id);
               delete vehicle[v];
               vehicle.erase(vehicle.begin() + v);
            }
         }
      }

      //RETURN ALL LOOT ITEMS TO BASE
      for(int l=0;l<thissquad.loot.size();l++)
      {
         location[obase]->loot.push_back(thissquad.loot[l]);
      }
      thissquad.loot.clear();
      return 1;
   }
   return 0;
}



/* common - returns the creature's maximum level in the given skill */
int maxskill(int skill,creaturest& cr)
{
   switch(skill)
   {
   case SKILL_HANDTOHAND:
   case SKILL_CLUB:
      return cr.attval(ATTRIBUTE_STRENGTH)*2;
   case SKILL_KNIFE:
   case SKILL_SWORD:
   case SKILL_PISTOL:
   case SKILL_RIFLE:
   case SKILL_SMG:
   case SKILL_SHOTGUN:
   case SKILL_DRIVING:
   case SKILL_IMPROVISED:
   case SKILL_SLEIGHTOFHAND:
   case SKILL_STEALTH:
      return cr.attval(ATTRIBUTE_AGILITY)*2;
   case SKILL_PERSUASION:
   case SKILL_DISGUISE:
   case SKILL_GANGSTERISM:
   case SKILL_TEACHING:
   case SKILL_SEDUCTION:
      return cr.attval(ATTRIBUTE_CHARISMA)*2;
   case SKILL_ART:
   case SKILL_MUSIC:
      return cr.attval(ATTRIBUTE_HEART)*2;
   case SKILL_RELIGION:
   case SKILL_BUSINESS:
      return cr.attval(ATTRIBUTE_WISDOM)*2;
   case SKILL_SCIENCE:
   case SKILL_LAW:
   case SKILL_SURVIVAL:
   case SKILL_MEDICAL:
   case SKILL_SECURITY:
   case SKILL_INTERROGATION:
   case SKILL_COOKING:
   case SKILL_COMPUTERS:
   case SKILL_GARMENTMAKING:
   case SKILL_WRITING:
   case SKILL_STREETSENSE:
      return cr.attval(ATTRIBUTE_INTELLIGENCE)*2;
   case SKILL_LEADERSHIP:
      if(cr.juice<10)return 0;
      if(cr.juice<50)return 1;
      if(cr.juice<100)return 2;
      if(cr.juice<200)return 3;
      if(cr.juice<500)return 4;
      return 5;
   default:
      return -1;
   }
}

/* common - returns the associated skill for the given weapon type */
int weaponskill(int weapon)
{
   int wsk=SKILL_HANDTOHAND;
   switch(weapon)
   {
      case WEAPON_KNIFE:
         wsk=SKILL_KNIFE;
         break;
      case WEAPON_SHANK:
      case WEAPON_SYRINGE:
      case WEAPON_CROWBAR:
      case WEAPON_HAMMER:
      case WEAPON_CHAIN:
      case WEAPON_GAVEL:
      case WEAPON_CROSS:
      case WEAPON_TORCH:
      case WEAPON_PITCHFORK:
         wsk=SKILL_IMPROVISED;
         break;
      case WEAPON_BASEBALLBAT:
      case WEAPON_NIGHTSTICK:
      case WEAPON_MAUL:
      case WEAPON_STAFF:
         wsk=SKILL_CLUB;
         break;
      case WEAPON_SWORD:
      case WEAPON_DAISHO:
         wsk=SKILL_SWORD;
         break;
      case WEAPON_REVOLVER_22:
      case WEAPON_REVOLVER_44:
      case WEAPON_SEMIPISTOL_9MM:
      case WEAPON_SEMIPISTOL_45:
         wsk=SKILL_PISTOL;
         break;
      case WEAPON_SHOTGUN_PUMP:
         wsk=SKILL_SHOTGUN;
         break;
      case WEAPON_SMG_MP5:
         wsk=SKILL_SMG;
         break;
      case WEAPON_AUTORIFLE_M16:
      case WEAPON_AUTORIFLE_AK47:
      case WEAPON_CARBINE_M4:
      case WEAPON_SEMIRIFLE_AR15:
         wsk=SKILL_RIFLE;
         break;
      case WEAPON_GUITAR:
         wsk=SKILL_MUSIC;
         break;
      case WEAPON_SPRAYCAN:
         wsk=SKILL_ART;
         break;
   }
   return wsk;
}



/* common - applies a crime to everyone in the active party */
void criminalizeparty(short crime)
{
   if(activesquad==NULL)return;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         if(!activesquad->squad[p]->alive)continue;
         activesquad->squad[p]->lawflag[crime]++;
         activesquad->squad[p]->heat+=lawflagheat(crime);
      }
   }
}



/* common - applies a crime to everyone in a location, or the entire LCS */
void criminalizepool(short crime,long exclude,short loc)
{
   for(int p=0;p<pool.size();p++)
   {
      if(p==exclude)continue;
      if(loc!=-1&&pool[p]->location!=loc)continue;

      pool[p]->lawflag[crime]++;
      pool[p]->heat+=lawflagheat(crime);
   }
}

/* common - applies a crime to a person */
void criminalize(creaturest &cr,short crime)
{
   cr.lawflag[crime]++;
   cr.heat+=lawflagheat(crime);
}



/* common - gives juice to everyone in the active party */
void juiceparty(long juice)
{
   if(activesquad!=NULL)
   {
      for(int p=0;p<6;p++)
      {
         if(activesquad->squad[p]!=NULL)
         {
            if(activesquad->squad[p]->alive)
            {
               addjuice(*activesquad->squad[p],juice);
            }
         }
      }
   }
}



/* common - gives juice to a given creature */
void addjuice(creaturest &cr,long juice,long cap)
{
   if(cr.juice>=cap)return;

   if(cr.juice+juice>cap)juice=cap-cr.juice;
   
   cr.juice+=juice;
   
   //For juice gains over 20, leader gets
   //half of that in leadership skill
   if(cr.juice>20 && juice>=2 && cr.hireid!=-1)
   {
      for(int i=0;i<pool.size();i++)
      {
         if(pool[i]->id==cr.hireid)
         {
            //If your maximum leadership skill is greater than
            //current skill. In other words, you need to be able
            //to gain a level before you'll be given the experience
            if(maxskill(SKILL_LEADERSHIP,*pool[i])>pool[i]->skill[SKILL_LEADERSHIP])
               pool[i]->skill_ip[SKILL_LEADERSHIP]+=juice>>1;
            break;
         }
      }
   }
   if(cr.juice>1000)cr.juice=1000;
   if(cr.juice<-50)cr.juice=-50;
}



/* common - removes the liberal from all squads */
void removesquadinfo(creaturest &cr)
{
   if(cr.squadid!=-1)
   {
      long sq=getsquad(cr.squadid);
      if(sq!=-1)
      {
         char flipstart=0;
         for(int pt=0;pt<6;pt++)
         {
            if(squad[sq]->squad[pt]==&cr)flipstart=1;
            if(flipstart&&pt<5)squad[sq]->squad[pt]=squad[sq]->squad[pt+1];
         }
         if(flipstart)squad[sq]->squad[5]=NULL;
      }
      cr.squadid=-1;
   }
}



/* common - purges empty squads from existance */
void cleangonesquads(void)
{
   char hasmembers;
   for(int sq=squad.size()-1;sq>=0;sq--)
   {
      //NUKE SQUAD IF IT IS GONE
      hasmembers=0;
      for(int p=0;p<6;p++)
      {
         if(squad[sq]->squad[p]!=NULL)hasmembers=1;
      }
      if(!hasmembers)
      {
         //SQUAD LOOT WILL BE DESTROYED
         if(activesquad==squad[sq])activesquad=NULL;
         delete squad[sq];
         squad.erase(squad.begin() + sq);
      }
      //OTHERWISE YOU CAN TAKE ITS MONEY
      else
      {
         for(int l=squad[sq]->loot.size()-1;l>=0;l--)
         {
            if(squad[sq]->loot[l]->type==ITEM_MONEY)
            {
               funds+=squad[sq]->loot[l]->money;
               stat_funds+=squad[sq]->loot[l]->money;
               moneygained_thievery+=squad[sq]->loot[l]->money;
               delete squad[sq]->loot[l];
               squad[sq]->loot.erase(squad[sq]->loot.begin() + l);
            }
         }
      }
   }
}



/* common - moves all squad members and their cars to a new location */
void locatesquad(squadst *st,long loc)
{
   for(int p=0;p<6;p++)
   {
      if(st->squad[p]!=NULL)
      {
         st->squad[p]->location=loc;

         if(st->squad[p]->carid!=-1)
         {
            long v=id_getcar(st->squad[p]->carid);
            if(v!=-1)
            {
               vehicle[v]->location=loc;
            }
         }
      }
   }
}



/* common - assigns a new base to all members of a squad */
void basesquad(squadst *st,long loc)
{
   for(int p=0;p<6;p++)
   {
      if(st->squad[p]!=NULL)
      {
         st->squad[p]->base=loc;
      }
   }
}



/* common - shifts public opinion on an issue */
void change_public_opinion(int v,int power,char affect,char cap)
{
   if(v==VIEW_LIBERALCRIMESQUAD)affect=0;
   if(v==VIEW_LIBERALCRIMESQUADPOS)affect=0;

   if(v==VIEW_LIBERALCRIMESQUADPOS)
   {
      int mood = publicmood(-1);
      if(cap>mood)cap=mood;
   }

   int effpower=power;

   // Affect is whether the LCS is publically known to be behind
   // the circumstances creating the public opinion change
   if(affect)
   {
      // Aff is the % of people who know about the LCS
      int aff=attitude[VIEW_LIBERALCRIMESQUAD];
      // Rawpower is the amount of the action proportional
      // to the people who, not having heard of the LCS,
      // do not allow the LCS' reputation to affect their
      // opinions
      int rawpower=(int)((float)power * (float)(100-aff)/100.0f);
      // Affected power is the remainder of the action besides
      // rawpower, the amount of the people who know of the LCS
      // and have it alter their opinion
      int affectedpower=power-rawpower;

      if(affectedpower>0)
      {
         // Dist is the difference between the LCS popularity and
         // the issue's popularity -- if the LCS is less popular
         // than the issue, the action will backfire for these
         // people, else it will be extra powerful
         int dist=attitude[VIEW_LIBERALCRIMESQUADPOS]-attitude[v];
         // If the issue is more popular than the LCS, dist will be
         // negative. Not good for you!

         // Then we increase dist by ten points, so you have a 10%
         // bonus to your effective popularity when doing this
         // calculation. This helps to make things a little easier.
         dist+=10;
         // Affected power is then scaled by dist -- if the LCS is
         // equally popular as the issue, it's equally powerful as
         // the rawpower. For every 10% up or down past there, it's
         // 1x the effect -- so if the LCS is 10% more popular than
         // the issue, you get 2x power, if the LCS is 20% less
         // popular than the issue, it'll not just nullify the effect,
         // but backfire with 1x power! So if the LCS is unpopular,
         // people will gravitate away from the causes the LCS fights
         // for.
         affectedpower=(int)(((float)affectedpower*(float)dist)/10.0f+0.5f);
      }

      // Effpower is then the sum of the rawpower (people who don't know
      // about the LCS) and the affectedpower (people who do know
      // about the LCS and had their judgment swayed by their opinion
      // of it).
      effpower=rawpower+affectedpower;
   }

   if(v==VIEW_LIBERALCRIMESQUAD)
   {
      //Only half the country will ever hear about the LCS at one time,
      //and nobody will ever forget about it
      if(effpower<0)effpower=0;
      if(effpower>50)effpower=50;
   }
   else if(v==VIEW_LIBERALCRIMESQUADPOS)
   {
      //Only 80% of the country can be swayed at once in their views
      //of the LCS.
      if(effpower<-80)effpower=-80;
      if(effpower>80)effpower=80;
   }
   
   //Scale the magnitude of the effect based on how much
   //people are paying attention to the issue
   effpower=(int)(effpower*(1+(float)public_interest[v]/50));

   //Then affect public interest
   if(public_interest[v]<cap || (v==VIEW_LIBERALCRIMESQUADPOS && public_interest[v]<100))
      public_interest[v]+=abs(effpower);

   if(effpower>0)
   {
      //Some things will never persuade the last x% of the population.
      //If there's a cap on how many people will be impressed, this
      //is where that's handled.
      if(attitude[v]+effpower>cap)
      {
         if(attitude[v]>cap)effpower=0;
         else effpower = cap - attitude[v];
      }
   }

   

   //Finally, apply the effect.
   attitude[v]+=effpower;
   
   if(attitude[v]<0)attitude[v]=0;
   if(attitude[v]>100)attitude[v]=100;
}

/* returns the amount of heat associated with a given crime */
int lawflagheat(int lawflag)
{
   switch(lawflag)
   {
   case LAWFLAG_KIDNAPPING:return 2;
   case LAWFLAG_MURDER:return 2;
   case LAWFLAG_THEFT:return 0;
   case LAWFLAG_BREAKING:return 0;
   case LAWFLAG_TERRORISM:return 10;
   case LAWFLAG_JURY:return 0;
   case LAWFLAG_TREASON:return 10;
   case LAWFLAG_ESCAPED:return 5;
   case LAWFLAG_HELPESCAPE:return 5;
   case LAWFLAG_RESIST:return 0;
   case LAWFLAG_BURNFLAG:return 0;
   case LAWFLAG_SPEECH:return 0;
   case LAWFLAG_VANDALISM:return 0;
   case LAWFLAG_ASSAULT:return 0;
   case LAWFLAG_CARTHEFT:return 0;
   case LAWFLAG_INFORMATION:return 2;
   case LAWFLAG_COMMERCE:return 2;
   case LAWFLAG_CCFRAUD:return 1;
   case LAWFLAG_BROWNIES:return 1;
   case LAWFLAG_BURIAL:return 0;
   case LAWFLAG_PROSTITUTION:return 0;
   case LAWFLAG_DISTURBANCE:return 0;
   case LAWFLAG_HIREILLEGAL:return 0;
   case LAWFLAG_RACKETEERING:return 5;
   case LAWFLAG_LOITERING:return 0;
   case LAWFLAG_GUNCARRY:return 0;
   default:return 0;
   }
}


