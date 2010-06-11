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
#include <math.h>
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
bool iscriminal(Creature &cr)
{
   for(int i=0;i<LAWFLAGNUM;i++)
      if(cr.crimes_suspected[i])
         return 1;
   return 0;
}



/* common - sends somebody to the hospital */
/***************************************************
*JDS* Hospitalize -- sends current person to the
specified clinic or hospital.
***************************************************/
void hospitalize(int loc, Creature &patient)
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
      if(time>1)addstr("months");
      else addstr("month");
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
int clinictime(Creature &g)
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

/* common - returns the associated skill for the given weapon type */
int weaponskill(int weapon)
{
   int wsk=SKILL_HANDTOHAND;
   switch(weapon)
   {
      case WEAPON_KNIFE:
      case WEAPON_SHANK:
      case WEAPON_SYRINGE:
         wsk=SKILL_KNIFE;
         break;
      case WEAPON_MOLOTOV:
         wsk=SKILL_THROWING;
         break;
      case WEAPON_SPRAYCAN:
      case WEAPON_CROWBAR:
      case WEAPON_CHAIN:
      case WEAPON_GAVEL:
      case WEAPON_CROSS:
      case WEAPON_TORCH:
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
      case WEAPON_PITCHFORK:
      case WEAPON_SWORD:
      case WEAPON_DAISHO:
         wsk=SKILL_SWORD;
         break;
      case WEAPON_REVOLVER_38:
      case WEAPON_REVOLVER_44:
      case WEAPON_DESERT_EAGLE:
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

         criminalize(*(activesquad->squad[p]),crime);
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

      criminalize(*pool[p], crime);
   }
}

/* common - applies a crime to a person */
void criminalize(Creature &cr,short crime)
{
   if(mode==GAMEMODE_SITE)
   {
      if(location[cursite]->siege.siege)
      {
         // Do not criminalize the LCS for self-defense against
         // extrajudicial raids
         if(location[cursite]->siege.siegetype!=SIEGE_POLICE)
            return;
      }
      else if(location[cursite]->renting==RENTING_CCS)
      {
         // Do not criminalize the LCS for crimes against the CCS
         return;
      }
      else if(location[cursite]->type==SITE_BUSINESS_CRACKHOUSE)
      {
         // Do not criminalize the LCS for crimes against the gangs
         return;
      }
   }
   cr.crimes_suspected[crime]++;
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
void addjuice(Creature &cr,long juice,long cap)
{
   juice *= 2; // Less grinding mode, engaged!

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
void removesquadinfo(Creature &cr)
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
      //OTHERWISE YOU CAN TAKE ITS MONEY (and other gear)
      else
      {
         for(int l=squad[sq]->loot.size()-1;l>=0;l--)
         {
            if(squad[sq]->loot[l]->type==ITEM_MONEY)
            {
               ledger.add_funds(squad[sq]->loot[l]->money,INCOME_THIEVERY);
               delete squad[sq]->loot[l];
               squad[sq]->loot.erase(squad[sq]->loot.begin() + l);
            }
            else
            {
               // Empty squad inventory into base inventory
               location[squad[sq]->squad[0]->base]->loot.push_back(squad[sq]->loot[l]);
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


// Picks a random option, based on the weights provided
int choose_one(const int * weight_list, int number_of_options, int default_value)
{
   int weight_total=0;
   int option;
   for (option=0;option<number_of_options;option++)
   {
      weight_total+=weight_list[option];
   }
   if(weight_total<1) return default_value; // No acceptable results; use default
   
   int choose=LCSrandom(weight_total);
   for (option=0;option<number_of_options;option++)
   {
      choose -= weight_list[option];
      if(choose<0)break;
   }
   return option;
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
      if(cap>mood+40)cap=mood+40;
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
   // Note that for the purposes of this function, we're not looking at how severe the crime is,
   // but how vigorously it is pursued by law enforcement. This determines how quickly they raid
   // you for it, and how much of a penalty you get in court for it. Some crimes are inflated
   // heat, others are deflated (such as the violent crimes).
   //
   // - Jonathan S. Fox
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
   case LAWFLAG_RESIST:return 1;
   case LAWFLAG_EXTORTION:return 1;
   case LAWFLAG_BURNFLAG:return 0;
   case LAWFLAG_SPEECH:return 0;
   case LAWFLAG_VANDALISM:return 0;
   case LAWFLAG_ASSAULT:return 0;      // XXX: This is on the same level as "harmful speech"?
   case LAWFLAG_ARMEDASSAULT:return 0;    // Fox: Yes. You get too many assault charges to put heat on it.
   case LAWFLAG_CARTHEFT:return 0;
   case LAWFLAG_INFORMATION:return 5;
   case LAWFLAG_COMMERCE:return 2;
   case LAWFLAG_CCFRAUD:return 2;
   case LAWFLAG_BROWNIES:return 5;
   case LAWFLAG_BURIAL:return 0;
   case LAWFLAG_PROSTITUTION:return 0;
   case LAWFLAG_DISTURBANCE:return 0;
   case LAWFLAG_HIREILLEGAL:return 1;
   case LAWFLAG_RACKETEERING:return 5;
   case LAWFLAG_LOITERING:return 0;
   //case LAWFLAG_GUNCARRY:return 0;
   //case LAWFLAG_GUNUSE:return 1;
   case LAWFLAG_ARSON:return 5;
   case LAWFLAG_PUBLICNUDITY:return 0;
   default:return 0;
   }
}

// Determines the number of subordinates a creature may command
int maxsubordinates(const Creature& cr)
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
   recruitcap += cr.get_skill(SKILL_LEADERSHIP); 
   //Cap for founder
   if(cr.hireid == -1 && cr.align == 1) recruitcap += 6;
   return recruitcap;
}

// Determines the number of subordinates a creature may recruit,
// based on their max and the number they already command
int subordinatesleft(const Creature& cr)
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
int loveslaves(const Creature& cr)
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
int loveslavesleft(const Creature& cr)
{
   // Get maximum lovers
   int loveslavecap = cr.get_skill(SKILL_SEDUCTION)/2+1;

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

/* common - Checks if a site (typically safehouse) has a unique short name, and for business fronts, if the front has a unique shortname. */
char duplicatelocation(locationst &loc) {
   for(int l = 0; l < location.size(); l++)
   {
      if(location[l] == &loc)
         continue;

      if(!strcmp(location[l]->shortname, loc.shortname))
         return 1;

      if (location[l]->front_business != -1 &&
     loc.front_business != -1 &&
     !strcmp (location[l]->front_shortname, loc.front_shortname))
    return 1;
   }
   return 0;
}

// Prompt to turn new recruit into a sleeper
void sleeperize_prompt(Creature &converted, Creature &recruiter, int y)
{
   char selection=0;
       
   while(1)
   {
      move(y,0);
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("In what capacity will ");
      addstr(converted.name);
      addstr(" best serve the Liberal cause?");
      move(y+2,0);
      set_color(COLOR_WHITE,COLOR_BLACK,selection==0);
      if(selection==0)
      {
         addstr("-> ");
      }
      else
      {
         addstr("   ");
      }
      addstr("Come to ");
      addstr(location[recruiter.location]->name);
      addstr(" as a ");
      set_color(COLOR_GREEN,COLOR_BLACK,selection==0);
      addstr("regular member");
      set_color(COLOR_WHITE,COLOR_BLACK,selection==0);
      addstr(".");
      move(y+3,0);
      set_color(COLOR_WHITE,COLOR_BLACK,selection==1);
      if(selection==1)
         addstr("-> ");
      else
         addstr("   ");
      addstr("Stay at ");
      addstr(location[converted.worklocation]->name);
      addstr(" as a ");
      set_color(COLOR_CYAN,COLOR_BLACK,selection==1);
      addstr("sleeper agent");
      set_color(COLOR_WHITE,COLOR_BLACK,selection==1);
      addstr(".");
      
   
      int keystroke = getch();
      translategetch(keystroke);
      if(keystroke == 10 && selection==1)
      {
         converted.flag |= CREATUREFLAG_SLEEPER;
         converted.location = converted.worklocation;
         location[converted.worklocation]->interrogated=1;
         location[converted.worklocation]->hidden=0;
         converted.base = converted.worklocation;
         liberalize(converted,false);
         break;
      }
      else if(keystroke == 10 && selection==0)
      {
         converted.location=recruiter.location;
         converted.base=recruiter.base;
         liberalize(converted,false);
         break;
      }
      else if(keystroke == KEY_DOWN || keystroke == KEY_UP)
      {
         selection=!selection;
      }
   }
}

/* common - Sort a list of creatures.*/
void sortliberals(std::vector<Creature *>& liberals, short sortingchoice, bool sortdefault)
{
   if(!sortdefault && sortingchoice==SORTING_DEFAULT)return;
   bool nochange = false;
   bool swap = false;
   unsigned loopnr = 0;
   while (!nochange)
   {
      ++loopnr;
      nochange = true;
      for (unsigned i = 0; i < liberals.size()-loopnr; ++i)
      {
         swap = false;
         switch (sortingchoice)
         {
            case SORTING_DEFAULT:
               swap = liberals[i]->id > liberals[i+1]->id; //I guess this is equivalent to the default. -blomkvist
               break;
            case SORTING_NAME:
               swap = strcmp(liberals[i]->name,liberals[i+1]->name)>0;
               break;
            case SORTING_LOCATION_AND_NAME:
               swap = (liberals[i]->location > liberals[i+1]->location
                       || (liberals[i]->location == liberals[i+1]->location
                           && strcmp(liberals[i]->name,liberals[i+1]->name)>0));
               break;
            case SORTING_SQUAD_OR_NAME:
               swap = ((liberals[i]->squadid == -1 && liberals[i+1]->squadid != -1)
                        || (liberals[i+1]->squadid != -1
                            && liberals[i]->squadid > liberals[i+1]->squadid)
                        || (liberals[i]->squadid == -1
                            && liberals[i+1]->squadid == -1
                            && strcmp(liberals[i]->name,liberals[i+1]->name)>0)
                        || (liberals[i]->squadid == liberals[i+1]->squadid 
                            && strcmp(liberals[i]->name,liberals[i+1]->name)>0)); //This last one should rather compare their order in the squad. -blomkvist
               break;
            default: break;
         }
         
         if(swap)
         {
            Creature* tmp = liberals[i];
            liberals[i] = liberals[i+1];
            liberals[i+1] = tmp;
            nochange = false;
         }
      }
   }
}

/* common - Prompt to decide how to sort liberals.*/
void sorting_prompt(short listforsorting)
{
   erase();
   move(1,1);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   addstr("Choose how to sort list of ");
   switch(listforsorting)
   {
      case SORTINGCHOICE_LIBERALS: addstr("active Liberals.");break;
      case SORTINGCHOICE_HOSTAGES: addstr("hostages.");break;
      case SORTINGCHOICE_CLINIC: addstr("Liberals in hospital.");break;
      case SORTINGCHOICE_JUSTICE: addstr("oppressed Liberals.");break;
      case SORTINGCHOICE_SLEEPERS: addstr("sleepers.");break;
      case SORTINGCHOICE_DEAD: addstr("dead people.");break;
      case SORTINGCHOICE_AWAY: addstr("people away.");break;
      case SORTINGCHOICE_ACTIVATE: addstr("Liberal activity.");break;
      case SORTINGCHOICE_ACTIVATESLEEPERS: addstr("sleeper activity.");break;
      case SORTINGCHOICE_ASSEMBLESQUAD: addstr("available Liberals.");break;
      case SORTINGCHOICE_BASEASSIGN: addstr("squadless members.");break;
      default: addstr("ERROR: INVALID VALUE FOR SORTINGCHOICE!");break;
   }   
   move(3,2);
   addstr("A - No sorting.");
   move(4,2);
   addstr("B - Sort by name.");
   move(5,2);
   addstr("C - Sort by location and name.");
   move(6,2);
   addstr("D - Sort by squad or name.");

   while(1)
   {
      int c = getch();
      translategetch(c);
      
      if(c=='a')
      {
         activesortingchoice[listforsorting]=SORTING_DEFAULT;
         break;
      }
      else if(c=='b')
      {
         activesortingchoice[listforsorting]=SORTING_NAME;
         break;
      }
      else if(c=='c')
      {
         activesortingchoice[listforsorting]=SORTING_LOCATION_AND_NAME;
         break;
      }
      else if(c=='d')
      {
         activesortingchoice[listforsorting]=SORTING_SQUAD_OR_NAME;
         break;
      }
   }
}

/* common - Returns appropriate sortingchoice enum value for a reviewmode enum value.
            Is currently unnecessary unless the enums are changed.*/
short reviewmodeenum_to_sortingchoiceenum(short reviewmode)
{
   switch (reviewmode)
   {
      case REVIEWMODE_LIBERALS: return SORTINGCHOICE_LIBERALS;
      case REVIEWMODE_HOSTAGES: return SORTINGCHOICE_HOSTAGES;
      case REVIEWMODE_CLINIC: return SORTINGCHOICE_CLINIC;
      case REVIEWMODE_JUSTICE: return SORTINGCHOICE_JUSTICE;
      case REVIEWMODE_SLEEPERS: return SORTINGCHOICE_SLEEPERS;
      case REVIEWMODE_DEAD: return SORTINGCHOICE_DEAD;
      case REVIEWMODE_AWAY: return SORTINGCHOICE_AWAY;
   }
   return 0;//-1; 
}

