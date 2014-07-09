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


void advanceday(char &clearformess,char canseethings)
{
   int p;
   showcarprefs=0;
   int w=0;
   //int l2;

   ledger.resetDailyAmounts();
   //*JDS* Save the game to save.dat each day. :)
   if((!disbanding) and autosave)savegame("save.dat");

   //CLEAR CAR STATES
   vector<long> caridused;
   for(p=0;p<(int)pool.size();p++)pool[p]->carid=-1;

   // Aging
   for(p=0;p<(int)pool.size();p++)
   {
      pool[p]->stunned = 0; // For lack of a better place, make stunning expire here

      if(!pool[p]->alive)continue;
      // animals, tanks don't have age effects at the moment
//TODO: Start aging effects for animals at age 12, take into account if they are genetic monsters or not.
      if(!pool[p]->animalgloss)
      {
         if(pool[p]->age>60)
         {
            int decrement=0;
            while(pool[p]->age - decrement>60)
            {
               if(LCSrandom(365*10)==0)
               {
                  pool[p]->adjust_attribute(ATTRIBUTE_HEALTH,-1);
                  if(pool[p]->get_attribute(ATTRIBUTE_HEALTH,false)<=0 &&
                     pool[p]->get_attribute(ATTRIBUTE_HEALTH,true)<=1)
                  {
                     pool[p]->die();
                     if(clearformess) erase();
                     else makedelimiter();
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(8,1);
                     addstr(pool[p]->name, gamelog);
                     addstr(" has passed away at the age of ", gamelog);
                     addstr(pool[p]->age, gamelog);
                     addstr(". The Liberal will be missed.", gamelog);
                     gamelog.nextMessage();

                     getkey();

                     break;
                  }
               }
               decrement+=10;
            }
            if(!pool[p]->alive)continue;
         }
         if(month==pool[p]->birthday_month&&
            day==pool[p]->birthday_day)
         {
            pool[p]->age++;
            switch(pool[p]->age)
            {
            case 13:
               pool[p]->type=CREATURE_TEENAGER; // aww, all grown up
               pool[p]->type_idname="CREATURE_TEENAGER";
               break;
            case 18:
               pool[p]->type=CREATURE_POLITICALACTIVIST; // ok seriously this time
               pool[p]->type_idname="CREATURE_POLITICALACTIVIST";
               break;
            }
         }
      }

      if(disbanding) continue;

      if(!pool[p]->is_active_liberal() || pool[p]->squadid != -1)
         continue;

      // Prevent moving people to a sieged location,
      // but don't evacuate people already under siege. - wisq
      if(pool[p]->location != pool[p]->base &&
         location[pool[p]->base]->siege.siege)
      {
         pool[p]->base=find_homeless_shelter(*pool[p]);
      }

      pool[p]->location=pool[p]->base;
   }

   //ADVANCE SQUADS
   squadst *oactivesquad=activesquad;
   for(int sq=0;sq<(int)squad.size();sq++)
   {
      if(disbanding)break;

      //MAKE SURE MEMBERS DON'T ACT IF SQUAD DOES

      if(squad[sq]->activity.type!=ACTIVITY_NONE)
      {
         for(int p=0;p<6;p++)
         {
            if(squad[sq]->squad[p]!=NULL)
            {
               if(squad[sq]->squad[p]->activity.type!=ACTIVITY_NONE&&
                  squad[sq]->squad[p]->activity.type!=ACTIVITY_VISIT)
               {
                  if(clearformess) erase();
                  else makedelimiter();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(squad[sq]->squad[p]->name, gamelog);
                  addstr(" acted with ", gamelog);
                  addstr(squad[sq]->name, gamelog);
                  addstr(" instead of ", gamelog);
                  addstr(getactivity(squad[sq]->squad[p]->activity), gamelog);
                  addstr(".", gamelog);
                  gamelog.newline();

                  getkey();
               }
               squad[sq]->squad[p]->activity.type=ACTIVITY_VISIT;
               squad[sq]->squad[p]->activity.arg=squad[sq]->activity.arg;
            }
         }
      }

      if(squad[sq]->activity.type==ACTIVITY_VISIT)
      {
         //TURN AWAY SQUADS FROM RECENTLY CLOSED OR SIEGED SITES
         if(location[squad[sq]->activity.arg]->closed||
            location[squad[sq]->activity.arg]->siege.siege)
         {
            if(clearformess) erase();
            else makedelimiter();
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(squad[sq]->name, gamelog);
            addstr(" decided ", gamelog);
            addstr(location[squad[sq]->activity.arg]->getname(), gamelog);
            addstr(" was too hot to risk.", gamelog);
            gamelog.nextMessage();

            getkey();

            //ON TO THE NEXT SQUAD
            squad[sq]->activity.type=ACTIVITY_NONE;
            continue;
         }

         //CAR UP AS NECESSARY
         vector<long> wantcar;

         for(p=0;p<6;p++)
         {
            if(squad[sq]->squad[p]!=NULL)
            {
               long wid=squad[sq]->squad[p]->pref_carid;
               if(wid==-1)continue;
               for(w=0;w<(int)wantcar.size();w++)
               {
                  if(wantcar[w]==wid)break;
               }
               if(w==(int)wantcar.size())wantcar.push_back(wid);
            }
         }

         if(wantcar.size()>0)
         {
            //CULL UNAVAILABLE CARS
            for(int c=wantcar.size()-1;c>=0;c--)
            {
               for(int c2=0;c2<(int)caridused.size();c2++)
               {
                  if(wantcar[c]==caridused[c2])
                  {
                     long v=id_getcar(caridused[c2]);
                     if(v!=-1)
                     {
                        if(clearformess) erase();
                        else makedelimiter();
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(8,1);
                        addstr(squad[sq]->name, gamelog);
                        addstr(" couldn't use the ", gamelog);
                        addstr(vehicle[v]->fullname(), gamelog);
                        addstr(".", gamelog);
                        gamelog.nextMessage();

                        getkey();
                     }
                     wantcar.erase(wantcar.begin() + c);
                     break;
                  }
               }
            }
         }

         //ASSIGN AVAILABLE CARS
         if(wantcar.size()>0)
         {
            vector<int> driver,passenger;
            for(w=0;w<(int)wantcar.size();w++)
            {
               driver.clear();
               passenger.clear();
               caridused.push_back(wantcar[w]);

               //FILL CAR WITH DESIGNATED DRIVERS AND PASSENGERS
               for(p=0;p<6;p++)
               {
                  if(squad[sq]->squad[p]!=NULL)
                  {
                     if(squad[sq]->squad[p]->pref_carid==wantcar[w])
                     {
                        squad[sq]->squad[p]->carid=wantcar[w];
                        squad[sq]->squad[p]->is_driver=squad[sq]->squad[p]->pref_is_driver;
                        if(squad[sq]->squad[p]->is_driver)driver.push_back(p);
                        else passenger.push_back(p);
                     }
                  }
               }

               //NO DRIVER?
               if(driver.size()==0)
               {
                  //MAKE BEST DRIVING PASSENGER INTO A DRIVER
                  if(passenger.size()>0)
                  {
                     int max=0;
                     for(p=0;p<(int)passenger.size();p++)
                     {
                        long v=id_getcar(squad[sq]->squad[passenger[p]]->carid);
                        if(v >= 0)
                        {
                           if(driveskill(*squad[sq]->squad[passenger[p]],*vehicle[v])>max&&
                              squad[sq]->squad[passenger[p]]->canwalk())
                           {
                                 max=driveskill(*squad[sq]->squad[passenger[p]],*vehicle[v]);
                           }
                        }
                     }
                     vector<int> goodp;
                     for(p=0;p<(int)passenger.size();p++)
                     {
                        long v=id_getcar(squad[sq]->squad[passenger[p]]->carid);
                        if(v >= 0)
                        {
                              if(driveskill(*squad[sq]->squad[passenger[p]],*vehicle[v])==max&&
                              squad[sq]->squad[passenger[p]]->canwalk())
                           {
                              goodp.push_back(passenger[p]);
                           }
                        }
                     }

                     if(goodp.size()>0)
                     {
                        int p=goodp[LCSrandom(goodp.size())];
                        squad[sq]->squad[p]->is_driver=1;
                     }
                  }
               }
               //TOO MANY DRIVERS?
               else if(driver.size()>1)
               {
                  //TOSS ALL BUT THE BEST
                  int max=0;
                  for(p=0;p<(int)driver.size();p++)
                  {
                     long v=id_getcar(squad[sq]->squad[driver[p]]->carid);
                     if (v >= 0)
                     {
                        if(driveskill(*squad[sq]->squad[driver[p]],*vehicle[v])>max)
                        {
                              max=driveskill(*squad[sq]->squad[driver[p]],*vehicle[v]);
                        }
                     }
                  }
                  vector<int> goodp;
                  for(p=0;p<(int)driver.size();p++)
                  {
                     long v=id_getcar(squad[sq]->squad[driver[p]]->carid);
                     if (v >= 0)
                     {
                        if(driveskill(*squad[sq]->squad[driver[p]],*vehicle[v])==max)
                        {
                           goodp.push_back(p);
                        }
                     }
                  }

                  if(goodp.size()>0)
                  {
                     int p=goodp[LCSrandom(goodp.size())];
                     for(int p2=0;p2<(int)driver.size();p2++)
                     {
                        if(p2==p)continue;
                        squad[sq]->squad[driver[p2]]->is_driver=0;
                     }
                  }
               }
            }

            //PUT PEOPLE WITHOUT CARS INTO RANDOM CARS
               //THESE PEOPLE WILL NOT DRIVE
            for(int p=0;p<6;p++)
            {
               if(squad[sq]->squad[p]!=NULL)
               {
                  if(squad[sq]->squad[p]->carid==-1)
                  {
                     squad[sq]->squad[p]->carid=wantcar[LCSrandom(wantcar.size())];
                     squad[sq]->squad[p]->is_driver=0;
                  }
               }
            }
         }

         //IF NEED CAR AND DON'T HAVE ONE...
            //NOTE: SQUADS DON'T TAKE FREE CARS
         if(location[squad[sq]->activity.arg]->needcar&&
            squad[sq]->squad[0]!=NULL)
         {
            if(squad[sq]->squad[0]->carid==-1)
            {
               if(clearformess) erase();
               else makedelimiter();
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(squad[sq]->name, gamelog);
               addstr(" didn't have a car to get to ", gamelog);
               addstr(location[squad[sq]->activity.arg]->getname(), gamelog);
               addstr(".", gamelog);
               gamelog.nextMessage();

               getkey();

               //ON TO THE NEXT SQUAD
               squad[sq]->activity.type=ACTIVITY_NONE;
               continue;
            }
         }

         // Give drivers experience if they actually travel
         if(squad[sq]->activity.arg != squad[sq]->squad[0]->base)
         {
            for(int i=0;i<6;i++)
            {
               if(squad[sq]->squad[i] && squad[sq]->squad[i]->carid != -1 && squad[sq]->squad[i]->is_driver)
                  squad[sq]->squad[i]->train(SKILL_DRIVING,5);
            }
         }

         //GO PLACES
         // Identify the "travel location" -- top level in multi-city play,
         // a particular district in one-city play
         int travelLocation = -1;
         for(int i=0; i < (int)location.size(); i++)
         {
            if(location[i]->type == SITE_TRAVEL)
            {
               travelLocation = i;
               break;
            }
         }

         // Verify travellers can afford the cost, and charge them
         bool canDepart = true;
         if(location[squad[sq]->activity.arg]->parent == travelLocation)
         {
            if(clearformess) erase();
            else makedelimiter();
            move(8,1);
            int squadNum;
            for(squadNum = 0; squadNum < 6; squadNum++)
               if(squad[sq]->squad[squadNum] == NULL) break;
            if(ledger.get_funds() < 100 * squadNum)
            {
               addstr_fl(gamelog, "%s couldn't afford tickets to go to %s.", squad[sq]->name, location[squad[sq]->activity.arg]->getname().c_str());
               canDepart = false;
            }
            else
            {
               ledger.subtract_funds(100 * squadNum, EXPENSE_TRAVEL);
               addstr_fl(gamelog, "%s spent $%d on tickets to go to %s.", squad[sq]->name, 100*squadNum, location[squad[sq]->activity.arg]->getname().c_str());
            }

            getkey();
         }

         if(canDepart) switch(location[squad[sq]->activity.arg]->type)
         {
         case SITE_CITY_NEW_YORK:
         case SITE_CITY_SEATTLE:
         case SITE_CITY_LOS_ANGELES:
         case SITE_CITY_CHICAGO:
         case SITE_CITY_DETROIT:
         case SITE_CITY_ATLANTA:
         case SITE_CITY_MIAMI:
         case SITE_CITY_WASHINGTON_DC:
            if(clearformess) erase();
            else makedelimiter();
            move(8,1);
            addstr(squad[sq]->name, gamelog);
            addstr(" arrives in ", gamelog);
            addstr(location[squad[sq]->activity.arg]->getname(), gamelog);
            addstr(".", gamelog);
            gamelog.nextMessage();

            getkey();

            {
               int l = find_homeless_shelter(squad[sq]->activity.arg);
               // Base at new city's homeless shelter
               basesquad(squad[sq],l);
               locatesquad(squad[sq],l);
            }

            clearformess=1;
            break;
         case SITE_BUSINESS_DEPTSTORE:
         case SITE_BUSINESS_HALLOWEEN:
         case SITE_BUSINESS_PAWNSHOP:
         case SITE_BUSINESS_CARDEALERSHIP:
         case SITE_BUSINESS_ARMSDEALER:
            if(clearformess) erase();
            else makedelimiter();
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(squad[sq]->name, gamelog);
            addstr(" has arrived at ", gamelog);
            addstr(location[squad[sq]->activity.arg]->getname(), gamelog);
            addstr(".", gamelog);
            gamelog.nextMessage();

            getkey();

            activesquad=squad[sq];
            showcarprefs=-1;
            switch(location[squad[sq]->activity.arg]->type)
            {
               case SITE_BUSINESS_DEPTSTORE:
                  deptstore(squad[sq]->activity.arg);
                  break;
               case SITE_BUSINESS_HALLOWEEN:
                  halloweenstore(squad[sq]->activity.arg);
                  break;
               case SITE_BUSINESS_PAWNSHOP:
                  pawnshop(squad[sq]->activity.arg);
                  break;
               case SITE_BUSINESS_CARDEALERSHIP:
                  dealership(squad[sq]->activity.arg);
                  break;
               case SITE_BUSINESS_ARMSDEALER:
                  armsdealer(squad[sq]->activity.arg);
                  break;
            }
            showcarprefs=0;
            if(activesquad->squad[0]!=NULL)
               locatesquad(activesquad,activesquad->squad[0]->base);
            clearformess=1;
            break;
         case SITE_HOSPITAL_UNIVERSITY:
         case SITE_HOSPITAL_CLINIC:
            if(clearformess) erase();
            else makedelimiter();
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(squad[sq]->name, gamelog);
            addstr(" has arrived at ", gamelog);
            addstr(location[squad[sq]->activity.arg]->getname(), gamelog);
            addstr(".", gamelog);
            gamelog.nextMessage();

            getkey();

            activesquad=squad[sq];
            hospital(squad[sq]->activity.arg);
            if(activesquad->squad[0]!=NULL)
               locatesquad(activesquad,activesquad->squad[0]->base);
            clearformess=1;
            break;
         default:
            if(clearformess) erase();
            else makedelimiter();
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            if(squad[sq]->squad[0]->base==squad[sq]->activity.arg)
            {
               addstr(squad[sq]->name, gamelog);
               addstr(" looks around ", gamelog);
               addstr(location[squad[sq]->activity.arg]->getname(), gamelog);
               addstr(".", gamelog);
               gamelog.nextMessage();
            }
            else
            {
               addstr(squad[sq]->name, gamelog);
               addstr(" has arrived at ", gamelog);
               addstr(location[squad[sq]->activity.arg]->getname(), gamelog);
               addstr(".", gamelog);
               gamelog.nextMessage();
            }

            getkey();

            if(clearformess) erase();
            else makedelimiter();

            int c='t';

            if(location[squad[sq]->activity.arg]->renting>=0&&
               location[squad[sq]->activity.arg]->type==SITE_INDUSTRY_WAREHOUSE)
               c='s';
            else if(location[squad[sq]->activity.arg]->renting>=0&&
               squad[sq]->squad[0]->base!=squad[sq]->activity.arg)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr("Why is the squad here?   (S)afe House, to cause (T)rouble, or (B)oth?");

               do c=getkey(); while(c!='s'&&c!='b'&&c!='t');
            }

            if(c=='s'||c=='b')basesquad(squad[sq],squad[sq]->activity.arg);
            if(c=='t'||c=='b')
            {
               activesquad=squad[sq];
               newsstoryst *ns=new newsstoryst;
               ns->type=NEWSSTORY_SQUAD_SITE;
               ns->positive=1;
               ns->loc=squad[sq]->activity.arg;
               newsstory.push_back(ns);
               mode_site(squad[sq]->activity.arg);
            }
            if(squad[sq]->squad[0]!=NULL)
               locatesquad(squad[sq],squad[sq]->squad[0]->base);

            clearformess=1;
            break;
         }
         squad[sq]->activity.type=ACTIVITY_NONE;
      }
   }
   activesquad=oactivesquad;

   //HOSTAGES
   for(p=pool.size()-1;p>=0;p--)
   {
      if(disbanding)break;

      if(!pool[p]->alive)continue;
      if(pool[p]->align!=1)
         tendhostage(pool[p],clearformess);
   }

   //ACTIVITIES FOR INDIVIDUALS
   for(p=0;p<(int)pool.size();p++)
   {
      pool[p]->income=0;
      if(disbanding)break;

      if(!pool[p]->alive)continue;
      if(pool[p]->clinic)continue;
      if(pool[p]->dating)continue;
      if(pool[p]->hiding)continue;

      if(pool[p]->location==-1)
      {
         //Had a crash bug on the siege check. Location was -1, probably set by 'partyrescue' and not reset during to messy violent chase.
         //This may prevent crashes.
         pool[p]->location = pool[p]->base;
      }
      //CLEAR ACTIONS FOR PEOPLE UNDER SIEGE
      if(location[pool[p]->location]->siege.siege)
      {
         switch(pool[p]->activity.type)
         {
         case ACTIVITY_HOSTAGETENDING:
         case ACTIVITY_TEACH_POLITICS:
         case ACTIVITY_TEACH_FIGHTING:
         case ACTIVITY_TEACH_COVERT:
         case ACTIVITY_HEAL:
         case ACTIVITY_REPAIR_ARMOR:
            break;
         default:
            pool[p]->activity.type=ACTIVITY_NONE;
            break;
         }
      }
      switch(pool[p]->activity.type)
      {
      case ACTIVITY_REPAIR_ARMOR:
         repairarmor(*pool[p],clearformess);
         break;
      case ACTIVITY_MAKE_ARMOR:
         makearmor(*pool[p],clearformess);
         // Uncomment this to have people stop making armor after the first day
         //pool[p]->activity.type=ACTIVITY_NONE;
         break;
      case ACTIVITY_WHEELCHAIR:
         getwheelchair(*pool[p],clearformess);
         if(pool[p]->flag & CREATUREFLAG_WHEELCHAIR)pool[p]->activity.type=ACTIVITY_NONE;
         break;
      case ACTIVITY_RECRUITING:
         recruitment_activity(*pool[p],clearformess);
         break;
      case ACTIVITY_STEALCARS:
         if(stealcar(*pool[p],clearformess))
            pool[p]->activity.type=ACTIVITY_NONE;
         else if(pool[p]->location!=-1&&location[pool[p]->location]->type==SITE_GOVERNMENT_POLICESTATION)
            criminalize(*pool[p],LAWFLAG_CARTHEFT);
         break;
      case ACTIVITY_POLLS:
         if(clearformess) erase();
         else makedelimiter();

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);
         addstr(pool[p]->name, gamelog);
         addstr(" surfs the Net for recent opinion polls.", gamelog);
         gamelog.nextMessage();

         getkey();

         pool[p]->train(SKILL_COMPUTERS,MAX(3-pool[p]->get_skill(SKILL_COMPUTERS),1));

         survey(pool[p]);
         //pool[p]->activity.type=ACTIVITY_NONE;  No reason for this not to repeat.  -AM-

         clearformess=1;
         break;
      case ACTIVITY_VISIT:
         pool[p]->activity.type=ACTIVITY_NONE;
         break;
      case ACTIVITY_NONE:
         if(pool[p]->align==1&&!pool[p]->is_imprisoned()&&(pool[p]->get_armor().is_bloody()||pool[p]->get_armor().is_damaged()))
            repairarmor(*pool[p],clearformess);
         break;
      }
   }

   funds_and_trouble(clearformess);

   // Healing - determine medical support at each location
   int *healing=new int[location.size()];
   int *healing2=new int[location.size()];
   for(p=0;p<(int)location.size();p++)
   {
      // Clinic is equal to a skill 6 liberal
      if(location[p]->type==SITE_HOSPITAL_CLINIC)healing[p]=6;
      // Hospital is equal to a skill 12 liberal
      else if(location[p]->type==SITE_HOSPITAL_UNIVERSITY)healing[p]=12;
      else healing[p]=0;
      healing2[p]=0;
   }
   for(p=0;p<(int)pool.size();p++)
   {

      if(disbanding)break;

      if(!pool[p]->alive)continue;
      if(pool[p]->hiding)continue;
      if(pool[p]->flag & CREATUREFLAG_SLEEPER)continue;
      // People will help heal even if they aren't specifically assigned to do so
      // Having a specific healing activity helps bookkeeping for the player, though
      // Only the highest medical skill is considered
      if(pool[p]->activity.type==ACTIVITY_HEAL||pool[p]->activity.type==ACTIVITY_NONE)
         if(pool[p]->location>-1&&
            healing[pool[p]->location]<pool[p]->get_skill(SKILL_FIRSTAID))
         {
            healing[pool[p]->location]=pool[p]->get_skill(SKILL_FIRSTAID);
            pool[p]->activity.type=ACTIVITY_HEAL;
         }
   }

   // Don't let starving locations heal
   for(p=0;p<(int)location.size();++p)
      if(location[p]->type!=SITE_HOSPITAL_CLINIC && location[p]->type!=SITE_HOSPITAL_UNIVERSITY)
         if(!fooddaysleft(p))
            if(location[p]->siege.siege)
               healing[p]=0;

   //HEAL NON-CLINIC PEOPLE AND TRAIN
   for(p=0;p<(int)pool.size();p++)
   {
      if(disbanding)break;
      if(!(pool[p]->alive))continue;

      if(clinictime(*pool[p]))
      {
         // For people in LCS home treatment
         if(pool[p]->clinic==false)
         {
            int damage=0; // Amount health degrades
            //int release=1;
            int transfer=0;

            // Give experience to caretakers
            if(pool[p]->location>-1)healing2[pool[p]->location]+=100-pool[p]->blood;

            // Cap blood at 100-injurylevel*20
            if(pool[p]->blood<100-(clinictime(*pool[p])-1)*20)
            {
               // Add health
               if(pool[p]->location>-1)pool[p]->blood+=1+healing[pool[p]->location]/3;
               if(pool[p]->blood>100-(clinictime(*pool[p])-1)*20)
                  pool[p]->blood=100-(clinictime(*pool[p])-1)*20;
               if(pool[p]->blood>100)
                  pool[p]->blood=100;
            }

            if(pool[p]->alive&&pool[p]->blood<0)
            {
               if(clearformess) erase();
               else makedelimiter();
               move(8,1);
               pool[p]->die();
               addstr(pool[p]->name, gamelog);
               addstr(" has died of injuries.", gamelog);
               gamelog.nextMessage();
            }

            for(int w=0;w<BODYPARTNUM;w++)
            {
               // Limbs blown off
               if(pool[p]->wound[w] & WOUND_NASTYOFF)
               {
                  // Chance to stabilize/amputate wound
                  // Difficulty 12 (Will die if not treated)
                  if(pool[p]->location>-1&&healing[pool[p]->location]+LCSrandom(10)>12)
                  {
                     pool[p]->wound[w]=WOUND_CLEANOFF;
                  }
                  // Else take bleed damage (4)
                  else
                  {
                     damage+=4;
                     //release=0;
                     if(pool[p]->location>-1&&healing[pool[p]->location]+9<=12)
                        transfer=1;
                  }
               }
               // Bleeding wounds
               else if(pool[p]->wound[w] & WOUND_BLEEDING)
               {
                  // Chance to stabilize wound
                  // Difficulty 8 (1 in 10 of happening naturally)
                  if(pool[p]->location>-1&&healing[pool[p]->location]+LCSrandom(10)>8)
                  {
                     // Toggle bleeding off
                     pool[p]->wound[w] &= ~WOUND_BLEEDING;
                  }
                  // Else take bleed damage (1)
                  else
                  {
                     damage+=1;
                     //release=0;
                  }
               }
               // Non-bleeding wounds
               else
               {  // Erase wound if almost fully healed, but preserve loss of limbs.
                  if(pool[p]->blood>=95)
                     pool[p]->wound[w]&=WOUND_CLEANOFF;
               }
            }
            // Critical hit wounds
            for(int i=SPECIALWOUND_RIGHTLUNG;i<SPECIALWOUNDNUM;++i)
            {
               int healdiff=14,permdamage=0,bleed=0,healed=0;
               // Specific treatment information on wounds
               switch(i)
               {
               case SPECIALWOUND_HEART:
                  healdiff=16;
                  bleed=8;
               case SPECIALWOUND_RIGHTLUNG:
               case SPECIALWOUND_LEFTLUNG:
                  permdamage=1;
               case SPECIALWOUND_LIVER:
               case SPECIALWOUND_STOMACH:
               case SPECIALWOUND_RIGHTKIDNEY:
               case SPECIALWOUND_LEFTKIDNEY:
               case SPECIALWOUND_SPLEEN:
                  healed=1;
                  bleed++;
                  break;
               case SPECIALWOUND_RIBS:
                  healed=RIBNUM;
                  break;
               case SPECIALWOUND_NECK:
               case SPECIALWOUND_UPPERSPINE:
               case SPECIALWOUND_LOWERSPINE:
                  healed=2;
                  break;
               }
               // If wounded
               if(pool[p]->special[i]!=healed&&(i==SPECIALWOUND_RIBS || pool[p]->special[i]!=1))
               {
                  // Chance to stabilize wound
                  if(pool[p]->location>-1&&healing[pool[p]->location]+LCSrandom(10)>healdiff)
                  {
                     // Remove wound
                     pool[p]->special[i]=healed;

                     if(permdamage)
                     {
                        // May take permanent health damage depending on
                        // quality of care
                        if(LCSrandom(20)>healing[pool[p]->location])
                        {
                           pool[p]->adjust_attribute(ATTRIBUTE_HEALTH,-1);
                           if(pool[p]->get_attribute(ATTRIBUTE_HEALTH,false)<=0)
                           {
                              pool[p]->set_attribute(ATTRIBUTE_HEALTH,1);
                           }
                        }
                     }
                  }
                  // Else take bleed damage
                  else
                  {
                     damage+=bleed;
                     //release=0;
                     if(healing[pool[p]->location]+9<=healdiff)
                        transfer=1;
                  }
               }
            }

            // Apply damage
            pool[p]->blood-=damage;

            if(transfer&&pool[p]->location>-1&&
               pool[p]->alive==1&&
               pool[p]->align==1&&
               location[pool[p]->location]->renting!=RENTING_NOCONTROL&&
               location[pool[p]->location]->type!=SITE_HOSPITAL_UNIVERSITY)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(pool[p]->name, gamelog);
               addstr("'s injuries require professional treatment.", gamelog);
               gamelog.nextMessage();
               pool[p]->activity.type=ACTIVITY_CLINIC;

               getkey();
            }
         }
      }
   }

   //Give experience to medics
   for(int p=0;p<(int)pool.size();p++)
   {
      //If present, qualified to heal, and doing so
      if(pool[p]->location>=0 && pool[p]->activity.type == ACTIVITY_HEAL)
      {
         //Clear activity if their location doesn't have healing work to do
         if(healing2[pool[p]->location]==0)
            pool[p]->activity.type=ACTIVITY_NONE;
         //Give experience based on work done and current skill
         else
            pool[p]->train(SKILL_FIRSTAID,MAX(0,healing2[pool[p]->location]/5-pool[p]->get_skill(SKILL_FIRSTAID)*2));
      }
   }
   delete[] healing;
   delete[] healing2;

   //DISPERSAL CHECK
   dispersalcheck(clearformess);

   //DO RENT
   if(day==3&&!disbanding)
      for(int l=0;l<(int)location.size();l++)
         if(location[l]->renting>0&&
            !location[l]->newrental)
         {  // if rent >= 1000000 this means you get should kicked out automatically
            if(ledger.get_funds()>=location[l]->renting&&location[l]->renting<1000000)
               ledger.subtract_funds(location[l]->renting,EXPENSE_RENT);
            //EVICTED!!!!!!!!!
            else
            {
               if(clearformess) erase();
               else makedelimiter();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr("EVICTION NOTICE: ", gamelog);
               addstr(location[l]->name, gamelog);
               addstr(".  Possessions go to the shelter.", gamelog);
               gamelog.nextMessage();

               getkey();

               location[l]->renting=RENTING_NOCONTROL;

               int hs=find_homeless_shelter(l);
               //MOVE ALL ITEMS AND SQUAD MEMBERS
               for(int p=0;p<(int)pool.size();p++)
               {
                  if(pool[p]->location==l)pool[p]->location=hs;
                  if(pool[p]->base==l)pool[p]->base=hs;
               }
               location[hs]->getloot(location[l]->loot);

               location[l]->compound_walls=0;
               location[l]->compound_stores=0;
               location[l]->front_business=-1;
            }
         }

   //MEET WITH POTENTIAL RECRUITS
   for(int i=pool.size()-1;i>=0;i--)
      pool[i]->meetings=0;
   for(int r=recruit.size()-1;r>=0;r--)
   {
      if(disbanding)break;

      int p=getpoolcreature(recruit[r]->recruiter_id);
      // Stand up recruits if 1) recruiter does not exist, 2) recruiter was not able to return to a safehouse today
      // or 3) recruiter is dead.
      if(p!=-1&&((pool[p]->location!=-1&&location[pool[p]->location]->renting!=RENTING_NOCONTROL&&pool[p]->alive&&
         location[pool[p]->location]->city==location[recruit[r]->recruit->location]->city)||recruit[r]->timeleft>0))
      {
         //MEET WITH RECRUIT

         //TERMINATE NULL RECRUIT MEETINGS
         if(location[pool[p]->location]->siege.siege)
         {
            delete_and_remove(recruit,r);
            continue;
         }
         //DO MEETING
         else if(completerecruitmeeting(*recruit[r],p,clearformess))
         {
            delete_and_remove(recruit,r);
            continue;
         }
      }
      else
      {
         delete_and_remove(recruit,r);
         continue;
      }
   }

   //DO DATES
   for(int d=date.size()-1;d>=0;d--)
   {
      if(disbanding)break;

      int p=getpoolcreature(date[d]->mac_id);
      // Stand up dates if 1) dater does not exist, or 2) dater was not able to return to a safehouse today (and is not in the hospital)
      if(p!=-1&&((pool[p]->location!=-1&&
	     (location[pool[p]->location]->renting!=RENTING_NOCONTROL||
         location[pool[p]->location]->type==SITE_HOSPITAL_CLINIC||
         location[pool[p]->location]->type==SITE_HOSPITAL_UNIVERSITY)&&
         location[pool[p]->location]->city==date[d]->city)||
	     date[d]->timeleft))
      {
         //VACATION
         if(date[d]->timeleft>0)
         {
            pool[p]->dating=--date[d]->timeleft;

            if(date[d]->timeleft==0)
            {
               int hs=find_homeless_shelter(date[d]->city);

               if(location[pool[p]->base]->siege.siege)
                  pool[p]->base=hs;

               pool[p]->location=pool[p]->base;

               if(completevacation(*date[d],p,clearformess))
               {
                  delete_and_remove(date,d);
                  continue;
               }
            }
         }
         //DO A DATE
         else
         {
            //TERMINATE NULL DATES
            if(location[pool[p]->location]->siege.siege)
            {
               delete_and_remove(date,d);
               continue;
            }
            //DO DATE
            else if(completedate(*date[d],p,clearformess))
            {
               delete_and_remove(date,d);
               continue;
            }
            else
            {
               pool[p]->dating=date[d]->timeleft;
               if(pool[p]->dating>0)
               {
                  //NOW KICK THE DATER OUT OF THE SQUAD AND LOCATION
                  removesquadinfo(*pool[p]);
                  pool[p]->location=-1;
               }
            }
         }
      }
      else
      {
         delete_and_remove(date,d);
         continue;
      }
   }

   //AGE THINGS
   day++;
   for(p=0;p<(int)pool.size();p++)
   {
      // Heal over time
      if(pool[p]->blood<100)pool[p]->blood+=1;

      // Updating for in hiding
      if(pool[p]->hiding>0)
      {
         if((--pool[p]->hiding)==0)
         {
            if(location[pool[p]->base]->siege.siege)
               pool[p]->hiding=1;
            else
            {
               pool[p]->location=pool[p]->base;

               if(clearformess) erase();
               else makedelimiter();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(pool[p]->name, gamelog);
               addstr(" regains contact with the LCS.", gamelog);
               gamelog.nextMessage();

               getkey();
            }
         }
      }

      // Check if news reports kidnapping
      if((pool[p]->flag & CREATUREFLAG_MISSING)&&
         !(pool[p]->flag & CREATUREFLAG_KIDNAPPED))
      {
         if(LCSrandom(14)+4<pool[p]->joindays)
         {
            pool[p]->flag|=CREATUREFLAG_KIDNAPPED;

            newsstoryst *ns=new newsstoryst;
            ns->type=NEWSSTORY_KIDNAPREPORT;
            ns->loc=pool[p]->location;
            ns->cr=pool[p];
            newsstory.push_back(ns);
         }
      }

      // Increment number of days since joined/kidnapped
      pool[p]->joindays++;

      // Increment number of days been dead if dead
      if(!pool[p]->alive)
      {
         pool[p]->deathdays++;
         continue;
      }

      // Gain skill levels for anything where you have enough experience
      pool[p]->skill_up();
   }

   //DO REPORTING BY MAJOR NEWSPAPERS
   majornewspaper(clearformess,canseethings);

   //CLEAN UP GONE SQUADS
   cleangonesquads();

   //SIEGE?
   siegeturn(clearformess);
   siegecheck(canseethings);

   //CLEAN GONE SQUADS AGAIN
   cleangonesquads();

   showcarprefs=1;
}

#define DISPERSAL_ABANDONLCS   5
#define DISPERSAL_BOSSINHIDING 4
#define DISPERSAL_HIDING       3
#define DISPERSAL_BOSSINPRISON 2
#define DISPERSAL_NOCONTACT    1
#define DISPERSAL_BOSSSAFE     0
#define DISPERSAL_SAFE        -1

/* squad members with no chain of command lose contact */
void dispersalcheck(char &clearformess)
{
   int p = 0;
   //NUKE DISPERSED SQUAD MEMBERS WHOSE MASTERS ARE NOT AVAILABLE
   if(pool.size()>0)
   {
      // *JDS* I'm documenting this algorithm carefully because it
      // took me awhile to figure out what exactly was going on here.
      //
      // dispersal_status tracks whether each person has a secure chain of command.
      //
      // if dispersal_status == NOCONTACT, no confirmation of contact has been made
      // if dispersal_status == BOSSSAFE, confirmation that THEY are safe is given,
      //    but it is still needed to check whether their subordinates
      //    can reach them.
      // if dispersal_status == SAFE, confirmation has been made that this squad
      //    member is safe, and their immediate subordinates have also
      //    checked.
      //
      // The way the algorithm works, everyone starts at dispersal_status = NOCONTACT.
      // Then we start at the top of the chain of command and walk
      // down it slowly, marking people BOSSSAFE and then SAFE as we sweep
      // down the chain. If someone is dead or in an unreachable state,
      // they block progression down the chain to their subordinates,
      // preventing everyone who requires contact with that person
      // from being marked safe. After everyone reachable has been
      // reached and marked safe, all remaining squad members are nuked.
      vector<int> dispersal_status;
      dispersal_status.resize(pool.size());

      bool promotion;
      do
      {
         promotion=0;
         for(p=0;p<(int)pool.size();p++)
         {
            // Default: members are marked dispersal_status = NOCONTACT
            //(no contact verified)
            dispersal_status[p]=DISPERSAL_NOCONTACT;
            // If member has no boss (founder level), mark
            // them dispersal_status = BOSSSAFE, using them as a starting point
            // at the top of the chain.
            if(pool[p]->hireid==-1)
            {
               if(!disbanding)
               {
                  dispersal_status[p]=DISPERSAL_BOSSSAFE;
                  if(pool[p]->hiding==-1)
                     pool[p]->hiding=LCSrandom(10)+5;
               }
               else dispersal_status[p]=DISPERSAL_BOSSINHIDING;
            }
            // If they're dead, mark them dispersal_status = SAFE, so they
            // don't ever have their subordinates checked
            // and aren't lost themselves (they're a corpse,
            // corpses don't lose contact)
            if(!pool[p]->alive && !disbanding)
            {
               dispersal_status[p]=DISPERSAL_SAFE;
               //Attempt to promote their subordinates
               if(promotesubordinates(*pool[p], clearformess))promotion=1;

               if(pool[p]->location==-1 || location[pool[p]->location]->renting == RENTING_NOCONTROL)
                  delete_and_remove(pool,p--);
            }
         }
      }while(promotion);

      char changed;

      do // while(changed!=0)
      {
         changed=0;

         char inprison;

         // Go through the entire pool to locate people at dispersal_status = BOSSSAFE,
         // so we can verify that their subordinates can reach them.
         for(p=pool.size()-1;p>=0;p--)
         {
            if(!pool[p]->alive)continue;
            if(pool[p]->location!=-1&&
               location[pool[p]->location]->type==SITE_GOVERNMENT_PRISON &&
               !(pool[p]->flag & CREATUREFLAG_SLEEPER))
            {
               inprison=1;
            }
            else inprison=0;

            // If your boss is in hiding
            if(dispersal_status[p]==DISPERSAL_BOSSINHIDING)
            {
               dispersal_status[p]=DISPERSAL_HIDING;
               for(int p2=pool.size()-1;p2>=0;p2--)
               {
                  if(pool[p2]->hireid==pool[p]->id && pool[p2]->alive)
                  {
                     dispersal_status[p2]=DISPERSAL_BOSSINHIDING; // Mark them as unreachable
                     changed=1; // Need another iteration
                  }
               }
            }

            // If in prison or unreachable due to a member of the command structure
            // above being in prison
            else if((dispersal_status[p]==DISPERSAL_BOSSSAFE&&inprison)||dispersal_status[p]==DISPERSAL_BOSSINPRISON)
            {
               int dispersalval=DISPERSAL_SAFE;
               if(pool[p]->flag & CREATUREFLAG_LOVESLAVE)
               {
                  if((dispersal_status[p]==DISPERSAL_BOSSINPRISON && !inprison) ||
                     (dispersal_status[p]==DISPERSAL_BOSSSAFE     &&  inprison))
                  {
                     pool[p]->juice--; // Love slaves bleed juice when not in prison with their lover
                     if(pool[p]->juice < -50)
                     {
                        dispersalval=DISPERSAL_ABANDONLCS;
                     }
                  }
               }
               dispersal_status[p]=dispersalval; // Guaranteed contactable in prison

               // Find all subordinates
               for(int p2=pool.size()-1;p2>=0;p2--)
               {
                  if(pool[p2]->hireid==pool[p]->id && pool[p2]->alive)
                  {
                     if(inprison)
                        dispersal_status[p2]=DISPERSAL_BOSSINPRISON;
                     else
                        dispersal_status[p2]=DISPERSAL_BOSSSAFE;
                     changed=1; // Need another iteration
                  }
               }
            }
            // Otherwise, if they're reachable
            else if(dispersal_status[p]==DISPERSAL_BOSSSAFE&&!inprison)
            {
               // Start looking through the pool again.
               for(int p2=pool.size()-1;p2>=0;p2--)
               {
                  // Locate each of this person's subordinates.
                  if(pool[p2]->hireid==pool[p]->id)
                  {
                     // Protect them from being dispersed -- their boss is
                     // safe. Their own subordinates will then be considered
                     // in the next loop iteration.
                     dispersal_status[p2]=DISPERSAL_BOSSSAFE;
                     // If they're hiding indefinitely and their boss isn't
                     // hiding at all, then have them discreetly return in a
                     // couple of weeks
                     if(pool[p2]->hiding==-1&&!pool[p]->hiding)
                     {
                        pool[p2]->hiding=LCSrandom(10)+3;
                     }
                     changed=1; // Take note that another iteration is needed.
                  }
               }
               // Now that we've dealt with this person's subordinates, mark
               // them so that we don't look at them again in this loop.
               dispersal_status[p]=DISPERSAL_SAFE;
            }
         }
      }while(changed); // If another iteration is needed, continue the loop.

      // After checking through the entire command structure, proceed
      // to nuke all squad members who are unable to make contact with
      // the LCS.
      for(p=pool.size()-1;p>=0;p--)
      {
         if(dispersal_status[p]==DISPERSAL_NOCONTACT||dispersal_status[p]==DISPERSAL_HIDING||dispersal_status[p]==DISPERSAL_ABANDONLCS)
         {
            if(clearformess) erase();
            else makedelimiter();

            if(!disbanding)
            {
               if(!pool[p]->hiding&&dispersal_status[p]==DISPERSAL_HIDING)
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(pool[p]->name, gamelog);
                  addstr(" has lost touch with the Liberal Crime Squad.", gamelog);
                  gamelog.nextMessage();

                  getkey();

                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  move(9,1);
                  addstr("The Liberal has gone into hiding...", gamelog);
                  gamelog.nextMessage();

                  getkey();
               }
               else if(dispersal_status[p]==DISPERSAL_ABANDONLCS)
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(pool[p]->name, gamelog);
                  addstr(" has abandoned the LCS.", gamelog);
                  gamelog.nextMessage();

                  getkey();
               }
               else if(dispersal_status[p]==DISPERSAL_NOCONTACT)
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(pool[p]->name, gamelog);
                  addstr(" has lost touch with the Liberal Crime Squad.", gamelog);
                  gamelog.nextMessage();

                  getkey();
               }
            }

            removesquadinfo(*pool[p]);
            if(dispersal_status[p]==DISPERSAL_NOCONTACT||dispersal_status[p]==DISPERSAL_ABANDONLCS)
               delete_and_remove(pool,p);
            else
            {
               pool[p]->location=-1;
               if(!(pool[p]->flag & CREATUREFLAG_SLEEPER)) //Sleepers end up in shelter otherwise.
                  pool[p]->base=find_homeless_shelter(*pool[p]);
               pool[p]->activity.type=ACTIVITY_NONE;
               pool[p]->hiding=-1; // Hide indefinitely
            }
         }
      }
   }

   //MUST DO AN END OF GAME CHECK HERE BECAUSE OF DISPERSAL
   endcheck(END_DISPERSED);

   cleangonesquads();
}



/* promote a subordinate to maintain chain of command when boss is lost */
bool promotesubordinates(Creature &cr, char &clearformess)
{
   int p;

   int newboss=-1;
   int bigboss=-2;
   if(cr.hireid==-1)bigboss=-1;//Special: Founder
   int maxjuice=0; //Need more than 0 juice to get promoted
   int subordinates=0;

   //Need REVOLUTIONARY (100+) juice to take over founder role
   if(cr.hireid==-1)maxjuice=99;

   //Identify big boss and top subordinate
   for(p=0;p<(int)pool.size();p++)
   {
      if(pool[p]->id==cr.id)continue;
      if(pool[p]->id==cr.hireid)bigboss=p;
      if(pool[p]->hireid==cr.id && pool[p]->alive && pool[p]->align==1)
      {
         subordinates++;
         //Brainwashed people inelligible for promotion to founder
         if(bigboss==-1 && pool[p]->flag & CREATUREFLAG_BRAINWASHED)continue;
         //Loveslaves inelligible for promotion to anything unless juice is high
         //in which case they get over it and continue to serve as a normal member
         if(pool[p]->flag & CREATUREFLAG_LOVESLAVE)
         {
            if(pool[p]->juice<100)continue;
            else pool[p]->flag &= ~CREATUREFLAG_LOVESLAVE;
         }

         // Highest juice liberal not a sleeper or subject to a life sentence gets promoted
         if(pool[p]->juice>maxjuice&&
            !(pool[p]->flag & CREATUREFLAG_SLEEPER)&&
            (pool[p]->location==-1||
            (location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON||pool[p]->sentence>=0)))
         {
            maxjuice=pool[p]->juice;
            newboss=p;
         }
      }
   }

   //No subordinates or none with sufficient juice to carry on
   if(subordinates==0 || newboss==-1)
   {
      if(cr.hireid!=-1)return 0;

      if(subordinates>0) // Disintegration of the LCS
      {
         if(clearformess) erase();
         else makedelimiter();

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);
         addstr(cr.name, gamelog);
         addstr(" has died.", gamelog);
         gamelog.newline();

         getkey();

         move(10,1);
         addstr("There are none left with the courage and conviction to lead....", gamelog);
         gamelog.nextMessage();

         getkey();
      }
      return 0;
   }

   //Chain of command totally destroyed if dead person's boss also dead
   if(bigboss==-2 || (cr.hireid!=-1 && bigboss!=-1 && !pool[bigboss]->alive))return 0;

   //Promote the new boss
   pool[newboss]->hireid=cr.hireid;

   //Order secondary subordinates to follow the new boss
   if(subordinates>1)
   {
      for(p=0;p<(int)pool.size();p++)
      {
         if(pool[p]->hireid==cr.id && // recruited by old boss that died
            p!=newboss &&             // not the new boss
            !(pool[p]->flag & CREATUREFLAG_LOVESLAVE)) // is not a love slave
         {
            pool[p]->hireid=pool[newboss]->id; // promote
         }
      }
   }

   if(clearformess) erase();
   else makedelimiter();

   if(bigboss!=-1) // Normal promotion
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(pool[bigboss]->name, gamelog);
      addstr(" has promoted ", gamelog);
      addstr(pool[newboss]->name, gamelog);
      move(9,1);
      addstr("due to the death of ", gamelog);
      addstr(cr.name, gamelog);
      addstr(".", gamelog);
      if(subordinates>1)
      {
         gamelog.newline();
         move(11,1);
         addstr(pool[newboss]->name, gamelog);
         addstr(" will take over for ", gamelog);
         addstr(cr.name, gamelog);
         addstr(" in the command chain.", gamelog);
      }
      gamelog.nextMessage();

      getkey();
   }
   else // Founder level promotion
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(cr.name, gamelog);
      addstr(" has died.", gamelog);
      gamelog.newline();

      getkey();

      move(10,1);
      addstr(pool[newboss]->name, gamelog);
      addstr(" is the new leader of the Liberal Crime Squad!", gamelog);
      gamelog.nextMessage();

      getkey();

      cr.hireid=-2; // Make dead founder not founder.
   }
   return 1;
}




/* daily - manages too hot timer and when a site map should be re-seeded and renamed */
void advancelocations()
{
   //ADVANCE LOCATIONS
   for(int l=0;l<(int)location.size();l++)
   {
      if(location[l]->closed>0)
      {
         location[l]->closed--;
         if(location[l]->closed==0)
         {  //Clean up graffiti, patch up walls, restore fire damage
            location[l]->changes.clear();

            //If high security is supported, chance to throw guards everywhere
            if(securityable(location[l]->type)&&LCSrandom(2))
               location[l]->highsecurity=60;
            //Else remodel the location, invalidate maps
            else initlocation(*location[l]);
         }
      }
      else if(location[l]->highsecurity>0)
      {  // Bank will remain on high security much longer
         if(location[l]->type!=SITE_BUSINESS_BANK)
            location[l]->highsecurity--;
         else if(!LCSrandom(5))
            location[l]->highsecurity--;
      }
   }
}



/* daily - returns true if the site type supports high security */
char securityable(int type)
{
   switch(type)
   {
      case SITE_BUSINESS_CIGARBAR:
      case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
      case SITE_LABORATORY_COSMETICS:
      case SITE_LABORATORY_GENETIC:
      case SITE_GOVERNMENT_FIRESTATION:
      case SITE_INDUSTRY_SWEATSHOP:
      case SITE_INDUSTRY_POLLUTER:
      case SITE_CORPORATE_HEADQUARTERS:
      case SITE_MEDIA_AMRADIO:
      case SITE_MEDIA_CABLENEWS:
         return 1;
//These places have better quality locks.
      case SITE_BUSINESS_BANK:
      case SITE_INDUSTRY_NUCLEAR:
      case SITE_GOVERNMENT_POLICESTATION:
      case SITE_GOVERNMENT_COURTHOUSE:
      case SITE_GOVERNMENT_PRISON:
      case SITE_GOVERNMENT_INTELLIGENCEHQ:
      case SITE_GOVERNMENT_ARMYBASE:
      case SITE_CORPORATE_HOUSE:
      case SITE_GOVERNMENT_WHITE_HOUSE:
		  return 2;
   }

   return 0;
}


/* daily - returns the number of days in the current month */
int monthday()
{
   switch(month)
   {
      case 2: return 28+(year%4==0&&(year%100!=0||year%400==0)); // February
      case 4: // April
      case 6: // June
      case 9: // September
      case 11: return 30; // November
      default: return 31; // January, March, May, July, August, October, & December
   }
}
