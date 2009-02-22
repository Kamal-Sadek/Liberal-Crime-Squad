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


/* common - test for possible game over */
char endcheck(char cause)
{
   char dead=1;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->alive&&
         pool[p]->align==1&&
         !(pool[p]->flag & CREATUREFLAG_SLEEPER && pool[p]->hireid!=-1)) // Allow sleepers to lead LCS without losing
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
               case SIEGE_CCS:savehighscore(END_CCS);break;
               case SIEGE_FIREMEN:savehighscore(END_FIREMEN);break;
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
int maxskill(int skill,creaturest& cr,bool use_juice)
{
   switch(skill)
   {
   case SKILL_HANDTOHAND:
   case SKILL_IMPROVISED:
      return (cr.attval(ATTRIBUTE_STRENGTH,use_juice)+cr.attval(ATTRIBUTE_AGILITY,use_juice))/2;
   case SKILL_FLAMETHROWER:
      // Flamethrower fully limited by agility, but also so heavy that it
      // requires strength too
      if(cr.attval(ATTRIBUTE_STRENGTH,use_juice) < cr.attval(ATTRIBUTE_AGILITY,use_juice))
         return cr.attval(ATTRIBUTE_STRENGTH,use_juice);
      else
         return cr.attval(ATTRIBUTE_AGILITY,use_juice);
   case SKILL_CLUB:
   case SKILL_AXE:
      return cr.attval(ATTRIBUTE_STRENGTH,use_juice);
   case SKILL_KNIFE:
   case SKILL_SWORD:
   case SKILL_PISTOL:
   case SKILL_RIFLE:
   case SKILL_SMG:
   case SKILL_SHOTGUN:
   case SKILL_DRIVING:
   case SKILL_THEFT:
   case SKILL_STEALTH:
   case SKILL_THROWING:
      return cr.attval(ATTRIBUTE_AGILITY,use_juice);
   case SKILL_PERSUASION:
   case SKILL_DISGUISE:
   case SKILL_SEDUCTION:
      return cr.attval(ATTRIBUTE_CHARISMA,use_juice);
   case SKILL_ART:
   case SKILL_MUSIC:
   case SKILL_COOKING:
      return cr.attval(ATTRIBUTE_HEART,use_juice);
   case SKILL_WRITING:
   case SKILL_PSYCHOLOGY:
      return (cr.attval(ATTRIBUTE_INTELLIGENCE,use_juice)+cr.attval(ATTRIBUTE_HEART,use_juice))/2;
   case SKILL_RELIGION:
      return (cr.attval(ATTRIBUTE_WISDOM,use_juice)+cr.attval(ATTRIBUTE_HEART,use_juice))/2;
   case SKILL_BUSINESS:
      return (cr.attval(ATTRIBUTE_WISDOM,use_juice)+cr.attval(ATTRIBUTE_INTELLIGENCE,use_juice))/2;
   case SKILL_SECURITY:
   case SKILL_TAILORING:
      return (cr.attval(ATTRIBUTE_INTELLIGENCE,use_juice)+cr.attval(ATTRIBUTE_AGILITY,use_juice))/2;
   case SKILL_TEACHING:
      return (cr.attval(ATTRIBUTE_INTELLIGENCE,use_juice)+cr.attval(ATTRIBUTE_CHARISMA,use_juice))/2;
   //case SKILL_SURVIVAL:
   //   return cr.attval(ATTRIBUTE_HEALTH,use_juice);
   case SKILL_FIRSTAID:
   case SKILL_SCIENCE:
   case SKILL_LAW:
   case SKILL_COMPUTERS:
   case SKILL_STREETSENSE:
   case SKILL_TACTICS:
      return cr.attval(ATTRIBUTE_INTELLIGENCE,use_juice);
   case SKILL_LEADERSHIP:
      if(cr.juice<10)return 0;
      if(cr.juice<50)return 1;
      if(cr.juice<100)return 2;
      if(cr.juice<200)return 4;
      if(cr.juice<500)return 7;
      if(cr.juice<1000)return 10;
      return 14;
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
      case WEAPON_SHANK:
         wsk=SKILL_KNIFE;
         break;
      case WEAPON_SYRINGE:
      case WEAPON_CROWBAR:
      case WEAPON_CHAIN:
      case WEAPON_GAVEL:
      case WEAPON_CROSS:
      case WEAPON_TORCH:
      case WEAPON_PITCHFORK:
      case WEAPON_SPRAYCAN:
         wsk=SKILL_IMPROVISED;
         break;
      case WEAPON_MOLOTOV:
         wsk=SKILL_THROWING;
         break;
      case WEAPON_BASEBALLBAT:
      case WEAPON_NIGHTSTICK:
      case WEAPON_MAUL:
      case WEAPON_STAFF:
      case WEAPON_HAMMER:
         wsk=SKILL_CLUB;
         break;
      case WEAPON_AXE:
         wsk=SKILL_AXE;
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
      case WEAPON_FLAMETHROWER:
         wsk=SKILL_FLAMETHROWER;
         break;
      case WEAPON_GUITAR:
         wsk=SKILL_MUSIC;
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
   if(juice>0 && cr.juice>=cap)
   {
      juice/=10;
      if(juice==0)return;
   }
   
   cr.juice+=juice;
   
   //For juice gains over 20, leader gets
   //half of that in leadership skill
   if(cr.juice>20 && juice>=2 && cr.hireid!=-1)
   {
      for(int i=0;i<pool.size();i++)
      {
         if(pool[i]->id==cr.hireid)
         {
            pool[i]->train(SKILL_LEADERSHIP,juice>>1);
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
         if(squad[sq]->squad[p]!=NULL)
         {
            // Let's do a bit of housekeeping here
            // And see if we can't gracefully eliminate that
            // pesky dead liberal in my squad bug
            if(squad[sq]->squad[p]->alive==false)
            {
               removesquadinfo(*squad[sq]->squad[p]);
               p=-1; // restart this for loop
            }
            else hasmembers=1;
         }
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
   // First note this in the liberal influence -- mostly for the
   // sake of the nice visual intelligence report entry
   if(v<VIEWNUM-5)
   {
      background_liberal_influence[v]+=power*10;
   }

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
   if(affect==1)
   {
      // Aff is the % of people who know/care about the LCS
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
         // Dist is a combination of the relative popularity of the LCS
         // to the issue and the absolute popularity of the LCS. Very
         // popular LCS on a very unpopular issue is very influential.
         // Very unpopular LCS on a very popular issue has the ability
         // to actually have a reverse effect.
         int dist=attitude[VIEW_LIBERALCRIMESQUADPOS]-attitude[v] +
                  attitude[VIEW_LIBERALCRIMESQUADPOS]-50;

         // Affected power is then scaled by dist -- if the LCS is
         // equally popular as the issue, it's equally powerful as
         // the rawpower. For every 10% up or down past there, it's
         // 10% more or less powerful.
         affectedpower=(int)(((float)affectedpower*(100.0+(float)dist))/100.0f);
      }

      // Effpower is then the sum of the rawpower (people who don't know
      // about the LCS) and the affectedpower (people who do know
      // about the LCS and had their judgment swayed by their opinion
      // of it).
      effpower=rawpower+affectedpower;
   }
   else if(affect==-1)
   {
      // Simplifed algorithm for affect by CCS respect
      effpower=power*(100-attitude[VIEW_CONSERVATIVECRIMESQUAD])/100;
   }

   if(v==VIEW_LIBERALCRIMESQUAD)
   {
      //Only half the country will ever hear about the LCS at one time,
      //and people will only grudgingly lose fear of it
      if(effpower<-5)effpower=5;
      if(effpower>50)effpower=50;
   }
   else if(v==VIEW_LIBERALCRIMESQUADPOS)
   {
      //Only 50% of the country can be swayed at once in their views
      //of the LCS negatively, 5% positively
      if(effpower<-50)effpower=-50;
      if(effpower>5)effpower=5;
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
   case LAWFLAG_ARMEDASSAULT:return 1;
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
   case LAWFLAG_GUNUSE:return 1;
   case LAWFLAG_ARSON:return 3;
   case LAWFLAG_PUBLICNUDITY:return 0;
   default:return 0;
   }
}

// Determines the number of subordinates a creature may command
int maxsubordinates(const creaturest& cr)
{
   //brainwashed recruits can't recruit normally
   if(cr.flag & CREATUREFLAG_BRAINWASHED)return 0;

   int recruitcap = 0;
   //Cap based on juice
   if(cr.juice >= 500)      recruitcap += 6;
   else if(cr.juice >= 200) recruitcap += 5;
   else if(cr.juice >= 100) recruitcap += 3;
   else if(cr.juice >= 50)  recruitcap += 1;
   //Cap based on leadership
   recruitcap += cr.skill[SKILL_LEADERSHIP]; 
   //Cap for founder
   if(cr.hireid == -1 && cr.align == 1) recruitcap += 6;
   return recruitcap;
}

// Determines the number of subordinates a creature may recruit,
// based on their max and the number they already command
int subordinatesleft(const creaturest& cr)
{
   int recruitcap = maxsubordinates(cr);
   for(int p=0; p<pool.size(); p++)
   {
      // ignore seduced and brainwashed characters
      if(pool[p]->hireid == cr.id && pool[p]->alive && !(pool[p]->flag&(CREATUREFLAG_LOVESLAVE|CREATUREFLAG_BRAINWASHED)))
         recruitcap--;
   }
   if(recruitcap > 0) return recruitcap;
   else return 0;
}

// Determines the number of love slaves a creature has
int loveslaves(const creaturest& cr)
{
   int loveslaves=0;
   for(int p=0; p<pool.size(); p++)
   {
      // If subordinate and a love slave
      if(pool[p]->hireid == cr.id && pool[p]->alive && pool[p]->flag & CREATUREFLAG_LOVESLAVE)
         loveslaves++;
   }
   return loveslaves;
}

// Determines the number of love slaves a creature may recruit,
// based on max minus number they already command
int loveslavesleft(const creaturest& cr)
{
   // Get maximum lovers
   int loveslavecap = cr.skill[SKILL_SEDUCTION]/2+1;

   // -1 if they're a love slave (their boss is a lover)
   if(cr.flag & CREATUREFLAG_LOVESLAVE)loveslavecap--;

   // Subtract number of love slaves they have
   loveslavecap -= loveslaves(cr);

   // If they can have more, return that number
   if(loveslavecap > 0) return loveslavecap;
   // If they're at 0 or less, return 0
   else return 0;
}

/* common - random issue by public interest */
int randomissue(bool core_only)
{
   int interest_array[VIEWNUM];
   int i;
   int total_interest=0;
   for(i=0;i<VIEWNUM-(core_only*5);i++)
   {
      interest_array[i]=public_interest[i]+total_interest+25;
      total_interest+=public_interest[i]+25;
   }
   int roll = LCSrandom(total_interest);
   for(i=0;i<VIEWNUM-(core_only*5);i++)
   {
      if(roll<interest_array[i])
         return i;
   }
   return -1;
}
