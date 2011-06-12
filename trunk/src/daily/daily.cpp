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
   int l2;

   //Save the game to autosave.dat each day.
   //autosave();
   //*JDS* Save the game to save.dat each day. :)
   if(!disbanding)save();

   //CLEAR CAR STATES
   vector<long> caridused;
   for(p=0;p<pool.size();p++)pool[p]->carid=-1;

   //SHUFFLE AROUND THE SQUADLESS
   int homes=-1;
   for(int l=0;l<location.size();l++)
   {
      if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
      {
         homes=l;
         break;
      }
   }
   if (homes==-1)
   {
      //TODO: Error unable to find location
      homes=0;
   }
      
   // Aging
   for(p=0;p<pool.size();p++)
   {
      pool[p]->stunned = 0; // For lack of a better place, make stunning expire here

      if(!pool[p]->alive||pool[p]->animalgloss)continue;
      // animals, tanks don't have age effects at the moment
//TODO: Start aging effects for animals at age 12, take into account if they are genetic monsters or not.
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
                  if(clearformess)erase();
                  else
                  {
                     makedelimiter(8,0);
                  }
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(pool[p]->name);
                  addstr(" has passed away at the age of ");
                  char str[5];
                  itoa(pool[p]->age,str,10);
                  addstr(str);
                  addstr(". The Liberal will be missed.");
                  refresh();
                  getch();
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
            break;
         case 18:
            pool[p]->type=CREATURE_POLITICALACTIVIST; // ok seriously this time
            break;
         }
      }

      if(disbanding)continue;

      if(pool[p]->clinic)continue;
      if(pool[p]->dating)continue;
      if(pool[p]->hiding)continue;
      if(pool[p]->squadid!=-1)continue;
      if(pool[p]->location==-1)continue;

      if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
         location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
         location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON)
      {
         // Prevent moving people to a sieged location,
         // but don't evacuate people already under siege. - wisq
         if(pool[p]->location != pool[p]->base &&
            location[pool[p]->base]->siege.siege)
         {
            pool[p]->base=homes;
         }

         pool[p]->location=pool[p]->base;
      }
   }

   //ADVANCE SQUADS
   squadst *oactivesquad=activesquad;
   for(int sq=0;sq<squad.size();sq++)
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
                  if(clearformess)erase();
                  else
                  {
                     makedelimiter(8,0);
                  }
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(squad[sq]->squad[p]->name);
                  addstr(" acted with ");
                  addstr(squad[sq]->name);
                  addstr(" instead of ");
                  char str[200];
                  getactivity(str,squad[sq]->squad[p]->activity);
                  addstr(str);
                  addstr(".");

                  refresh();
                  getch();
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
            if(clearformess)erase();
            else
            {
               makedelimiter(8,0);
            }
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(squad[sq]->name);
            addstr(" decided ");
            addlocationname(location[squad[sq]->activity.arg]);
            addstr(" was too hot to risk.");

            refresh();
            getch();

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
               for(w=0;w<wantcar.size();w++)
               {
                  if(wantcar[w]==wid)break;
               }
               if(w==wantcar.size())wantcar.push_back(wid);
            }
         }

         if(wantcar.size()>0)
         {
            //CULL UNAVAILABLE CARS
            for(int c=wantcar.size()-1;c>=0;c--)
            {
               for(int c2=0;c2<caridused.size();c2++)
               {
                  if(wantcar[c]==caridused[c2])
                  {
                     long v=id_getcar(caridused[c2]);
                     if(v!=-1)
                     {
                        if(clearformess)erase();
                        else
                        {
                           makedelimiter(8,0);
                        }
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(8,1);
                        addstr(squad[sq]->name);
                        addstr(" couldn't use the ");
                        addstr(vehicle[v]->fullname().c_str());
                        addstr(".");

                        refresh();
                        getch();
                     }
                     wantcar.erase(wantcar.begin() + c);
                  }
               }
            }
         }

         //ASSIGN AVAILABLE CARS
         if(wantcar.size()>0)
         {
            vector<int> driver;
            vector<int> passenger;
            for(w=0;w<wantcar.size();w++)
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
                     for(p=0;p<passenger.size();p++)
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
                     for(p=0;p<passenger.size();p++)
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
                  for(p=0;p<driver.size();p++)
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
                  for(p=0;p<driver.size();p++)
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
                     for(int p2=0;p2<driver.size();p2++)
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
               if(clearformess)erase();
               else
               {
                  makedelimiter(8,0);
               }
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(squad[sq]->name);
               addstr(" didn't have a car to get to ");
               addlocationname(location[squad[sq]->activity.arg]);
               addstr(".");

               refresh();
               getch();

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
         switch(location[squad[sq]->activity.arg]->type)
         {
            case SITE_BUSINESS_DEPTSTORE:
            case SITE_BUSINESS_HALLOWEEN:
            case SITE_BUSINESS_PAWNSHOP:
            case SITE_BUSINESS_CARDEALERSHIP:
            case SITE_BUSINESS_ARMSDEALER:
               if(clearformess)erase();
               else
               {
                  makedelimiter(8,0);
               }
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(squad[sq]->name);
               addstr(" has arrived at ");
               addlocationname(location[squad[sq]->activity.arg]);
               addstr(".");

               refresh();
               getch();

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
               {
                  locatesquad(activesquad,activesquad->squad[0]->base);
               }
               clearformess=1;
               break;
            case SITE_HOSPITAL_UNIVERSITY:
            case SITE_HOSPITAL_CLINIC:
               if(clearformess)erase();
               else
               {
                  makedelimiter(8,0);
               }
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(squad[sq]->name);
               addstr(" has arrived at ");
               addlocationname(location[squad[sq]->activity.arg]);
               addstr(".");

               refresh();
               getch();

               activesquad=squad[sq];
               hospital(squad[sq]->activity.arg);
               if(activesquad->squad[0]!=NULL)
               {
                  locatesquad(activesquad,activesquad->squad[0]->base);
               }
               clearformess=1;
               break;
            default:
            {
               if(clearformess)erase();
               else
               {
                  makedelimiter(8,0);
               }

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               if(squad[sq]->squad[0]->base==squad[sq]->activity.arg)
               {
                  addstr(squad[sq]->name);
                  addstr(" looks around ");
                  addlocationname(location[squad[sq]->activity.arg]);
                  addstr(".");
               }
               else
               {
                  addstr(squad[sq]->name);
                  addstr(" has arrived at ");
                  addlocationname(location[squad[sq]->activity.arg]);
                  addstr(".");
               }

               refresh();
               getch();

               if(clearformess)erase();
               else
               {
                  makedelimiter(8,0);
               }

               int c='t';

               if(location[squad[sq]->activity.arg]->renting>=0&&
                  location[squad[sq]->activity.arg]->type==SITE_INDUSTRY_WAREHOUSE)
               {
                  c='s';
               }
               else if(location[squad[sq]->activity.arg]->renting>=0&&
                  squad[sq]->squad[0]->base!=squad[sq]->activity.arg)
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr("Why is the squad here?   (S)afe House, to cause (T)rouble, or (B)oth?");

                  do
                  {
                     c=getch();                     translategetch(c);                  }while(c!='s'&&c!='b'&&c!='t');               }

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
               {
                  locatesquad(squad[sq],squad[sq]->squad[0]->base);
               }

               clearformess=1;
               break;
            }
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
      {
         tendhostage(pool[p],clearformess);
      }
   }

   //ACTIVITIES FOR INDIVIDUALS
   for(p=0;p<pool.size();p++)
   {
      pool[p]->income=0;
      if(disbanding)break;

      if(!pool[p]->alive)continue;
      if(pool[p]->clinic)continue;
      if(pool[p]->dating)continue;
      if(pool[p]->hiding)continue;

	  if (pool[p]->location == -1)
	  {
//Had a crash bug on the siege check. Location was -1, probably set by 'partyrescue' and not reset during to messy violent chase.
//This may prevent crashes.
		  pool[p]->location = pool[p]->base;
	  }
	  //CLEAR ACTIONS FOR PEOPLE UNDER SIEGE
	  if(location[pool[p]->location]->siege.siege)
	  {
		 pool[p]->activity.type=ACTIVITY_NONE;
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
         case ACTIVITY_STEALCARS:
            if(stealcar(*pool[p],clearformess))
            {
               pool[p]->activity.type=ACTIVITY_NONE;
            }
            else if(pool[p]->location!=-1&&location[pool[p]->location]->type==SITE_GOVERNMENT_POLICESTATION)
            {
               criminalize(*pool[p],LAWFLAG_CARTHEFT);
            }
            break;
         case ACTIVITY_POLLS:
            if(clearformess)erase();
            else
            {
               makedelimiter(8,0);
            }

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(pool[p]->name);
            addstr(" surfs the Net for recent opinion polls.");

            refresh();
            getch();

            pool[p]->train(SKILL_COMPUTERS,MAX(3-pool[p]->get_skill(SKILL_COMPUTERS),1));

            survey(pool[p]);
            //pool[p]->activity.type=ACTIVITY_NONE;  No reason for this not to repeat.  -AM-

            clearformess=1;
            break;
         case ACTIVITY_VISIT:
            pool[p]->activity.type=ACTIVITY_NONE;
            break;
         case ACTIVITY_NONE:
            if(pool[p]->align == 1 && location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION
                                   && location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE)
            {
               if(pool[p]->get_armor().is_bloody() || pool[p]->get_armor().is_damaged())
               {
                  repairarmor(*pool[p],clearformess);
               }
            }
                 break;
      }
   }

   funds_and_trouble(clearformess);

   // Healing - determine medical support at each location
   int *healing=new int[location.size()];
   int *healing2=new int[location.size()];
   for(p=0;p<location.size();++p)
   {
      // Clinic is equal to a skill 6 liberal
      if(location[p]->type==SITE_HOSPITAL_CLINIC)healing[p]=6;
      // Hospital is equal to a skill 12 liberal
      else if(location[p]->type==SITE_HOSPITAL_UNIVERSITY)healing[p]=12;
      else healing[p]=0;
      healing2[p]=0;
   }
   for(p=0;p<pool.size();p++)
   {
      
      if(disbanding)break;

      if(!pool[p]->alive)continue;
      if(pool[p]->hiding)continue;
      if(pool[p]->flag & CREATUREFLAG_SLEEPER)continue;
      // People will help heal even if they aren't specifically assigned to do so
      // Having a specific healing activity helps bookkeeping for the player, though
      // Only the highest medical skill is considered
      if(pool[p]->activity.type==ACTIVITY_HEAL||pool[p]->activity.type==ACTIVITY_NONE)
      {
         if(pool[p]->location>-1&&
            healing[pool[p]->location]<pool[p]->get_skill(SKILL_FIRSTAID))
         {
            healing[pool[p]->location]=pool[p]->get_skill(SKILL_FIRSTAID);
            pool[p]->activity.type=ACTIVITY_HEAL;
         }
      }
   }

   // Don't let starving locations heal
   for(p=0;p<location.size();++p)
   {
      if(location[p]->type!=SITE_HOSPITAL_CLINIC && location[p]->type!=SITE_HOSPITAL_UNIVERSITY)
      {
         if(!fooddaysleft(p))
         {
            if(location[p]->siege.siege)
            {
               healing[p]=0;
            }
         }
      }
   }

   //HEAL NON-CLINIC PEOPLE AND TRAIN
   for(p=0;p<pool.size();p++)
   {
      if(disbanding)break;
      if(!(pool[p]->alive))continue;

      if(clinictime(*pool[p]))
      {
         // For people in LCS home treatment
         if(pool[p]->clinic==false)
         {
            int damage=0; // Amount health degrades
            int release=1;
            int transfer=0;

            // Give experience to caretakers
            if(pool[p]->location>-1)healing2[pool[p]->location]+=100-pool[p]->blood;
            
            // Cap blood at 100-injurylevel*20
            if(pool[p]->blood<100-(clinictime(*pool[p])-1)*20)
            {
               // Add health
               if(pool[p]->location>-1)pool[p]->blood+=1+healing[pool[p]->location]/10;
               if(pool[p]->blood>100-(clinictime(*pool[p])-1)*20)
                  pool[p]->blood=100-(clinictime(*pool[p])-1)*20;
            }

            if(pool[p]->alive&&pool[p]->blood<0)
            {
               if(clearformess)
               {
                  erase();
               }
               else
               {
                  makedelimiter(8,0);
               }
               move(8,1);
               pool[p]->die();
               addstr(pool[p]->name);
               addstr(" has died of injuries.");
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
                     release=0;
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
                     pool[p]->wound[w] ^= WOUND_BLEEDING;
                  }
                  // Else take bleed damage (1)
                  else
                  {
                     damage+=1;
                     release=0;
                  }
               }
               // Non-bleeding wounds
               else
               {
                  // Erase wound if almost fully healed, but preserve loss of limbs.
                  if(pool[p]->blood>=95)
                  {
                     pool[p]->wound[w]&=WOUND_CLEANOFF;
                  }
               }
            }
            // Critical hit wounds
            for(int i=SPECIALWOUND_RIGHTLUNG;i<SPECIALWOUNDNUM;++i)
            {
               int healdiff=14;
               int permdamage=0;
               int bleed=0;
               int healed;
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
                     release=0;
                     if(healing[pool[p]->location]+9<=healdiff)
                     {
                        transfer=1;
                     }
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
               addstr(pool[p]->name);
               addstr("'s injuries require professional treatment.");
               pool[p]->activity.type=ACTIVITY_CLINIC;
               refresh();
               getch();
            }
         }
      }
   }

   //Give experience to medics
   for(int p=0;p<pool.size();p++)
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
   {
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->renting>0&&
            !location[l]->newrental)
         {
            if(ledger.get_funds()>=location[l]->renting)
            {
               ledger.subtract_funds(location[l]->renting,EXPENSE_RENT);
            }
            //EVICTED!!!!!!!!!
            else
            {
               if(clearformess)erase();
               else
               {
                  makedelimiter(8,0);
               }

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr("EVICTION NOTICE: ");
               addstr(location[l]->name);
               addstr(".  Possessions go to the shelter.");

               refresh();
               getch();

               location[l]->renting=RENTING_NOCONTROL;

               //MOVE ALL ITEMS AND SQUAD MEMBERS
               int hs=0;
               for(l2=0;l2<location.size();l2++)
               {
                  if(location[l2]->type==SITE_RESIDENTIAL_SHELTER)
                  {
                     hs=l2;
                     break;
                  }
               }
               for(int p=0;p<pool.size();p++)
               {
                  if(pool[p]->location==l)pool[p]->location=hs;
                  if(pool[p]->base==l)pool[p]->base=hs;
               }
               for(l2=0;l2<location[l]->loot.size();l2++)
               {
                  location[hs]->loot.push_back(location[l]->loot[l2]);
               }
               location[l]->loot.clear();

               location[l]->compound_walls=0;
               location[l]->compound_stores=0;
               location[l]->front_business=-1;
            }
         }
      }
   }

   //MEET WITH POTENTIAL RECRUITS
   for(int i=pool.size()-1;i>=0;i--)
   {
      pool[i]->meetings=0;
   }
   for(int r=recruit.size()-1;r>=0;r--)
   {
      if(disbanding)break;

      int p=getpoolcreature(recruit[r]->recruiter_id);
      // Stand up recruits if 1) recruiter does not exist, 2) recruiter was not able to return to a safehouse today
      // or 3) recruiter is dead.
      if(p!=-1&&((pool[p]->location!=-1&&location[pool[p]->location]->renting!=RENTING_NOCONTROL&&pool[p]->alive)||
         recruit[r]->timeleft>0))
      {
         //MEET WITH RECRUIT
         
         //TERMINATE NULL RECRUIT MEETINGS
         if(location[pool[p]->location]->siege.siege)
         {
            delete recruit[r];
            recruit.erase(recruit.begin() + r);
            continue;
         }
         //DO MEETING
         else if(completerecruitmeeting(*recruit[r],p,clearformess))
         {
            delete recruit[r];
            recruit.erase(recruit.begin() + r);
            continue;
         }
      }
      else
      {
         delete recruit[r];
         recruit.erase(recruit.begin() + r);
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
         location[pool[p]->location]->type==SITE_HOSPITAL_UNIVERSITY))||
	     date[d]->timeleft))
      {
         //VACATION
         if(date[d]->timeleft>0)
         {
            date[d]->timeleft--;

            pool[p]->dating=date[d]->timeleft;

            if(date[d]->timeleft==0)
            {
               int hs=-1;
               for(int l=0;l<location.size();l++)
               {
                  if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
                  {
                     hs=l;
                     break;
                  }
               }
               if (hs==-1)
               {
                  //TODO: Error unable to find location
                  hs=0;
               }

               if(location[pool[p]->base]->siege.siege)
               {
                  pool[p]->base=hs;
               }

               pool[p]->location=pool[p]->base;

               if(completevacation(*date[d],p,clearformess))
               {
                  delete date[d];
                  date.erase(date.begin() + d);
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
               delete date[d];
               date.erase(date.begin() + d);
               continue;
            }
            //DO DATE
            else if(completedate(*date[d],p,clearformess))
            {
               delete date[d];
               date.erase(date.begin() + d);
               continue;
            }
            else
            {
               pool[p]->dating=date[d]->timeleft;
               if(pool[p]->dating>0)
               {
                  long sq=-1;
                  
                  //IF YOU ARE THE LAST PERSON IN YOUR SQUAD
                  //YOU HAVE TO DROP OFF THE EQUIPMENT WHEREVER YOUR BASE IS
                  //BECAUSE YOUR SQUAD IS ABOUT TO BE DESTROYED
                  if(pool[p]->squadid!=-1)
                     sq=getsquad(pool[p]->squadid);

                  //NOW KICK THE DATER OUT OF THE SQUAD AND LOCATION
                  removesquadinfo(*pool[p]);
                  pool[p]->location=-1;

                  if(sq!=-1)
                  {
                     testsquadclear(*squad[sq], pool[p]->base);
                  }
               }
            }
         }
      }
      else
      {
         delete date[d];
         date.erase(date.begin() + d);
         continue;
      }
   }

   //AGE THINGS
   day++;
   for(p=0;p<pool.size();p++)
   {
      // Heal over time
      if(pool[p]->blood<100)pool[p]->blood+=1;

      // Updating for in hiding
      if(pool[p]->hiding>0)
      {
         pool[p]->hiding--;
         if(pool[p]->hiding==0)
         {
            if(location[pool[p]->base]->siege.siege)
            {
               pool[p]->hiding=1;
            }
            else
            {
               pool[p]->location=pool[p]->base;

               if(clearformess)erase();
               else
               {
                  makedelimiter(8,0);
               }

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(pool[p]->name);
               addstr(" regains contact with the LCS.");

               refresh();
               getch();
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
         for(p=0;p<pool.size();p++)
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
                  {
                     pool[p]->hiding=LCSrandom(10)+5;
                  }
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
               {
                  delete pool[p];
                  pool.erase(pool.begin() + p);
               }
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
               location[pool[p]->location]->type==SITE_GOVERNMENT_PRISON&&               !(pool[p]->flag & CREATUREFLAG_SLEEPER))            {               inprison=1;
            }            else inprison=0;

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
            if(clearformess)
            {
               erase();
            }
            else
            {
               makedelimiter(8,0);
            }

            if(!disbanding)
            {
               if(!pool[p]->hiding&&dispersal_status[p]==DISPERSAL_HIDING)
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(pool[p]->name);
                  addstr(" has lost touch with the Liberal Crime Squad.");
                  refresh();
                  getch();
                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  move(9,1);
                  addstr("The Liberal has gone into hiding...");
                  refresh();
                  getch();
               }
               else if(dispersal_status[p]==DISPERSAL_ABANDONLCS)
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(pool[p]->name);
                  addstr(" has abandoned the LCS.");
                  refresh();
                  getch();
               }
               else if(dispersal_status[p]==DISPERSAL_NOCONTACT)
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(pool[p]->name);
                  addstr(" has lost touch with the Liberal Crime Squad.");
                  refresh();
                  getch();
               }
            }


            removesquadinfo(*pool[p]);
            if(dispersal_status[p]==DISPERSAL_NOCONTACT||dispersal_status[p]==DISPERSAL_ABANDONLCS)
            {
               delete pool[p];
               pool.erase(pool.begin() + p);
            }
            else
            {
               int hs=0;
               for(int l=0;l<location.size();l++)
               {
                  if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
                  {
                     hs=l;
                     break;
                  }
               }
               pool[p]->location=-1;
               if(!(pool[p]->flag & CREATUREFLAG_SLEEPER)) //Sleepers end up in shelter otherwise.
                  pool[p]->base=hs;
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
   for(p=0;p<pool.size();p++)
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

         // Highest juice liberal not subject to a life sentence gets promoted
         if(pool[p]->juice>maxjuice&&
            (location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON||pool[p]->sentence>=0))
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
         addstr(cr.name);
         addstr(" has died.");
         refresh();
         getch();

         move(10,1);
         addstr("There are none left with the courage and conviction to lead....");
         refresh();
         getch();
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
      for(p=0;p<pool.size();p++)
      {
         if(pool[p]->hireid==cr.id && // recruited by old boss that died
            p!=newboss &&             // not the new boss
            !(pool[p]->flag & CREATUREFLAG_LOVESLAVE)) // is not a love slave
         {
            pool[p]->hireid=pool[newboss]->id; // promote
         }
      }
   }

   if(clearformess)
   {
      erase();
   }
   else
   {
      makedelimiter(8,0);
   }

   if(bigboss!=-1) // Normal promotion
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(pool[bigboss]->name);
      addstr(" has promoted ");
      addstr(pool[newboss]->name);
      move(9,1);
      addstr("due to the death of ");
      addstr(cr.name);
      addstr(".");
      if(subordinates>1)
      {
         move(11,1);
         addstr(pool[newboss]->name);
         addstr(" will take over for ");
         addstr(cr.name);
         addstr(" in the command chain.");
      }
      refresh();
      getch();
   }
   else // Founder level promotion
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(cr.name);
      addstr(" has died.");
      refresh();
      getch();

      move(10,1);
      addstr(pool[newboss]->name);
      addstr(" is the new leader of the Liberal Crime Squad!");
      refresh();
      getch();
      
      cr.hireid=-2; // Make dead founder not founder.
   }
   return 1;
}




/* daily - manages too hot timer and when a site map should be re-seeded and renamed */
void advancelocations(void)
{
   //ADVANCE LOCATIONS
   for(int l=0;l<location.size();l++)
   {
      if(location[l]->closed>0)
      {
         location[l]->closed--;
         if(location[l]->closed==0)
         {
            if(location[l]->type==SITE_GOVERNMENT_POLICESTATION)
               policestation_closed=0;
            if(location[l]->type==SITE_MEDIA_AMRADIO)
               amradio_closed=0;
            if(location[l]->type==SITE_MEDIA_CABLENEWS)
               cablenews_closed=0;

            //Clean up graffiti, patch up walls, restore fire damage
            location[l]->changes.clear();

            switch(LCSrandom(2))
            {
               case 0:
                  //If high security is supported
                  if(securityable(location[l]->type))
                  {
                     //Throw guards everywhere
                     location[l]->highsecurity=60;
                  }
                  //Else remodel the location, invalidate maps
                  else initlocation(*location[l]);
                  break;
               case 1:
                  //Remodel, invalidate maps
                  initlocation(*location[l]);
                  break;
            }
         }
      }
      else if(location[l]->highsecurity>0)
      {
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
	  case SITE_INDUSTRY_NUCLEAR:
      case SITE_GOVERNMENT_POLICESTATION:
      case SITE_GOVERNMENT_COURTHOUSE:
      case SITE_GOVERNMENT_PRISON:
      case SITE_GOVERNMENT_INTELLIGENCEHQ:
      case SITE_GOVERNMENT_ARMYBASE:
      case SITE_CORPORATE_HOUSE:
		  return 2;
   }

   return 0;
}



/* daily - seeds and names a site (will re-seed and rename if used after start) */
void initlocation(locationst &loc)
{
   loc.init();

   char str[80];

   switch(loc.type)
   {
      case SITE_GOVERNMENT_POLICESTATION:
         strcpy(loc.name,"Police Station");
         strcpy(loc.shortname,"Police Station");
         break;
      case SITE_GOVERNMENT_COURTHOUSE:
         if(law[LAW_DEATHPENALTY]==-2)
         {
            strcpy(loc.name,"Halls of Ultimate Judgment");
            strcpy(loc.shortname,"Judge Hall");
         }
         else
         {
            strcpy(loc.name,"Court House");
            strcpy(loc.shortname,"Court House");
         }
         break;
      case SITE_GOVERNMENT_FIRESTATION:
         if(law[LAW_FREESPEECH]==-2)
         {
            strcpy(loc.name,"Fireman HQ");
            strcpy(loc.shortname,"Fireman HQ");
         }
         else
         {
            strcpy(loc.name,"Fire Station");
            strcpy(loc.shortname,"Fire Station");
         }
         break;
      case SITE_GOVERNMENT_PRISON:
         if(law[LAW_DEATHPENALTY]==-2&&
            law[LAW_POLICEBEHAVIOR]==-2)
         {
            switch(LCSrandom(5))
            {
               case 0:strcpy(loc.name,"Happy");break;
               case 1:strcpy(loc.name,"Cheery");break;
               case 2:strcpy(loc.name,"Quiet");break;
               case 3:strcpy(loc.name,"Green");break;
               case 4:strcpy(loc.name,"Nectar");break;
            }
            strcat(loc.name," ");
            switch(LCSrandom(5))
            {
               case 0:strcat(loc.name,"Valley");break;
               case 1:strcat(loc.name,"Meadow");break;
               case 2:strcat(loc.name,"Hills");break;
               case 3:strcat(loc.name,"Glade");break;
               case 4:strcat(loc.name,"Forest");break;
            }
            strcat(loc.name," Re-education Camp");
            strcpy(loc.shortname,"Re-ed Camp");
         }
         else
         {
            lastname(str);
            strcpy(loc.name,str);
            strcat(loc.name," Prison");
            strcpy(loc.shortname,"Prison");
         }
         break;
      case SITE_INDUSTRY_NUCLEAR:
                  if(law[LAW_NUCLEARPOWER]==2)
                  {
                        strcpy(loc.name,"Nuclear Waste Center");
            strcpy(loc.shortname,"NWaste Center");
                  }
                  else
                  {
         strcpy(loc.name,"Nuclear Power Plant");
         strcpy(loc.shortname,"NPower Plant");
                  }
         break;
      case SITE_GOVERNMENT_INTELLIGENCEHQ:
         if(law[LAW_PRIVACY]==-2&&
            law[LAW_POLICEBEHAVIOR]==-2)
         {
            strcpy(loc.name,"Ministry of Love");
            strcpy(loc.shortname,"Min. Love");
         }
         else
         {
            strcpy(loc.name,"Intelligence HQ");
            strcpy(loc.shortname,"Int. HQ");
         }
         break;
      
      case SITE_GOVERNMENT_ARMYBASE:
         lastname(str);
         strcpy(loc.name,str);
         strcat(loc.name," Army Base");
         strcpy(loc.shortname,"Army Base");
         break;
      case SITE_CORPORATE_HEADQUARTERS:
         strcpy(loc.name,"Corporate HQ");
         strcpy(loc.shortname,"Corp. HQ");
         break;
      case SITE_BUSINESS_PAWNSHOP:
         if(law[LAW_GUNCONTROL]==ALIGN_ELITELIBERAL)
         {
            char str[80];
            lastname(str);
            strcpy(loc.name,str);
            strcat(loc.name,"'s Pawnshop");
            strcpy(loc.shortname,"Pawnshop");
         }
         else
         {
            char str[80];
            lastname(str);
            strcpy(loc.name,str);
            strcat(loc.name," Pawn & Gun");
            strcpy(loc.shortname,"Pawnshop");
         }
         break;
      case SITE_CORPORATE_HOUSE:
         if(law[LAW_CORPORATE]==-2&&
                                law[LAW_TAX]==-2)
         {
            strcpy(loc.name,"CEO Castle");
            strcpy(loc.shortname,"CEO Castle");
         }
         else
         {
            strcpy(loc.name,"CEO Residence");
            strcpy(loc.shortname,"CEO House");
         }
         break;
      case SITE_RESIDENTIAL_SHELTER:
         strcpy(loc.name,"Homeless Shelter");
         strcpy(loc.shortname,"Shelter");
         break;
      case SITE_INDUSTRY_WAREHOUSE:
      {
         do {
            strcpy(loc.name,"Old ");
            char str[50];
            lastname(str);
            strcat(loc.name,str);
            strcat(loc.name," ");
              
                 switch(LCSrandom(10))
                 {
                    case 0:
                      strcat(loc.name,"Meat Plant");
                      strcpy(loc.shortname,"Meat Plant");
                      break;
                    case 1:
                      strcat(loc.name,"Warehouse");
                      strcpy(loc.shortname,"Warehouse");
                      break;
                    case 2:
                      strcat(loc.name,"Paper Mill");
                      strcpy(loc.shortname,"Paper Mill");
                      break;
                    case 3:
                      strcat(loc.name,"Cement Factory");
                      strcpy(loc.shortname,"Cement");
                      break;
                    case 4:
                      strcat(loc.name,"Fertilizer Plant");
                      strcpy(loc.shortname,"Fertilizer");
                      break;
                    case 5:
                      strcat(loc.name,"Drill Factory");
                      strcpy(loc.shortname,"Drill");
                      break;
                    case 6:
                      strcat(loc.name,"Steel Plant");
                      strcpy(loc.shortname,"Steel");
                      break;
                    case 7:
                      strcat(loc.name,"Packing Plant");
                      strcpy(loc.shortname,"Packing");
                      break;
                    case 8:
                      strcat(loc.name,"Toy Factory");
                      strcpy(loc.shortname,"Toy");
                      break;
                    case 9:
                      strcat(loc.name,"Building Site");
                      strcpy(loc.shortname,"Building");
                      break;
                 }
              } while (duplicatelocation(loc));
         break;
      }
      case SITE_INDUSTRY_POLLUTER:
         switch(LCSrandom(5))
         {
            case 0:
               strcpy(loc.name,"Aluminum Factory");
               strcpy(loc.shortname,"Alum Fact");
               break;
            case 1:
               strcpy(loc.name,"Plastic Factory");
               strcpy(loc.shortname,"Plast Fact");
               break;
            case 2:
               strcpy(loc.name,"Oil Refinery");
               strcpy(loc.shortname,"Refinery");
               break;
            case 3:
               strcpy(loc.name,"Auto Plant");
               strcpy(loc.shortname,"Auto Plant");
               break;
            case 4:
               strcpy(loc.name,"Chemical Factory");
               strcpy(loc.shortname,"Chem Fact");
               break;
         }
         break;
      case SITE_MEDIA_CABLENEWS:
         strcpy(loc.name,"Cable News Station");
         strcpy(loc.shortname,"News Station");
         break;
      case SITE_MEDIA_AMRADIO:
         strcpy(loc.name,"AM Radio Station");
         strcpy(loc.shortname,"Radio Station");
         break;
      case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
      case SITE_RESIDENTIAL_APARTMENT:
         do {
            lastname(str);
            strcpy(loc.name,str);
            strcat(loc.name," Apartments");
            strcpy(loc.shortname,str);
            strcat(loc.shortname," Apts");
         } while (duplicatelocation(loc));
         break;
      case SITE_RESIDENTIAL_TENEMENT:
         do {
            char str[50];
            lastname(str);
            strcpy(loc.name,str);
            strcat(loc.name," St. ");
            strcat (loc.name, "Housing Projects");
            strcpy (loc.shortname, "Projects");
         } while (duplicatelocation (loc));
         break;
      case SITE_HOSPITAL_UNIVERSITY:
         strcpy(loc.name,"The University Hospital");
         strcpy(loc.shortname,"U Hospital");
         break;
      case SITE_HOSPITAL_CLINIC:
         strcpy(loc.name,"The Free CLINIC");
         strcpy(loc.shortname,"CLINIC");
         break;
      case SITE_LABORATORY_GENETIC:
         lastname(str);
         strcpy(loc.name,str);
         strcat(loc.name," Genetics");
         strcpy(loc.shortname,"Genetics Lab");
         break;
      case SITE_LABORATORY_COSMETICS:
         lastname(str);
         strcpy(loc.name,str);
         strcat(loc.name," Cosmetics");
         strcpy(loc.shortname,"Cosmetics Lab");
         break;
      case SITE_BUSINESS_CARDEALERSHIP:
         firstname(str,GENDER_WHITEMALEPATRIARCH);
         strcpy(loc.name,str);
         strcat(loc.name," ");
         lastname(str);
         strcat(loc.name,str);
         strcat(loc.name,"'s Used Cars");
         strcpy(loc.shortname,"Dealership");
         break;
      case SITE_BUSINESS_DEPTSTORE:
         lastname(str);
         strcpy(loc.name,str);
         strcat(loc.name,"'s Department Store");
         strcpy(loc.shortname,"Dept. Store");
         break;
      case SITE_BUSINESS_HALLOWEEN:
         lastname(str);
         strcpy(loc.name,"The Oubliette");
         strcpy(loc.shortname,"Oubliette");
         break;
      case SITE_INDUSTRY_SWEATSHOP:
         lastname(str);
         strcpy(loc.name,str);
         strcat(loc.name," Garment Makers");
         strcpy(loc.shortname,"Sweatshop");
         break;
      case SITE_BUSINESS_CRACKHOUSE:
      do {
         char str[50];
         lastname(str);
         strcpy(loc.name,str);
         strcat(loc.name," St. ");

         if (law[LAW_DRUGS]==2)
         {
            switch (LCSrandom (4))
            {
            case 0:
               strcat (loc.name, "Recreational Drugs Center");
               strcpy (loc.shortname, "Drugs Center");
               break;
            case 1:
               strcat (loc.name, "Coffee House");
               strcpy (loc.shortname, "Coffee House");
               break;
            case 2:
               strcat (loc.name, "Cannabis Lounge");
               strcpy (loc.shortname, "Cannabis Lounge");
               break;
            case 3:
               strcat (loc.name, "Marijuana Dispensary");
               strcpy (loc.shortname, "Dispensary");
               break;
            }
         }
         else
         {
            strcat (loc.name, "Crack House");
            strcpy (loc.shortname, "Crack House");
         }
      } while (duplicatelocation (loc));
      break;

      case SITE_BUSINESS_JUICEBAR:
         strcpy(loc.name,"");
         switch(LCSrandom(5))
         {
            case 0:strcat(loc.name,"Natural");break;
            case 1:strcat(loc.name,"Harmonious");break;
            case 2:strcat(loc.name,"Restful");break;
            case 3:strcat(loc.name,"Healthy");break;
            case 4:strcat(loc.name,"New You");break;
         }
         strcat(loc.name," ");
         switch(LCSrandom(5))
         {
            case 0:strcat(loc.name,"Diet");break;
            case 1:strcat(loc.name,"Methods");break;
            case 2:strcat(loc.name,"Plan");break;
            case 3:strcat(loc.name,"Orange");break;
            case 4:strcat(loc.name,"Carrot");break;
         }
         strcat(loc.name," Juice Bar");
         strcpy(loc.shortname,"Juice Bar");
         break;
      case SITE_BUSINESS_VEGANCOOP:
         strcpy(loc.name,"");
         switch(LCSrandom(5))
         {
            case 0:strcat(loc.name,"Asparagus");break;
            case 1:strcat(loc.name,"Tofu");break;
            case 2:strcat(loc.name,"Broccoli");break;
            case 3:strcat(loc.name,"Radish");break;
            case 4:strcat(loc.name,"Eggplant");break;
         }
         strcat(loc.name," ");
         switch(LCSrandom(5))
         {
            case 0:strcat(loc.name,"Forest");break;
            case 1:strcat(loc.name,"Rainbow");break;
            case 2:strcat(loc.name,"Garden");break;
            case 3:strcat(loc.name,"Farm");break;
            case 4:strcat(loc.name,"Meadow");break;
         }
         strcat(loc.name," Vegan Co-op");
         strcpy(loc.shortname,"Vegan Co-op");
         break;
      case SITE_BUSINESS_INTERNETCAFE:
         strcpy(loc.name,"");
         switch(LCSrandom(5))
         {
            case 0:strcat(loc.name,"Electric");break;
            case 1:strcat(loc.name,"Wired");break;
            case 2:strcat(loc.name,"Nano");break;
            case 3:strcat(loc.name,"Micro");break;
            case 4:strcat(loc.name,"Techno");break;
         }
         strcat(loc.name," ");
         switch(LCSrandom(5))
         {
            case 0:strcat(loc.name,"Panda");break;
            case 1:strcat(loc.name,"Troll");break;
            case 2:strcat(loc.name,"Latte");break;
            case 3:strcat(loc.name,"Unicorn");break;
            case 4:strcat(loc.name,"Pixie");break;
         }
         strcat(loc.name," Internet Cafe");
         strcpy(loc.shortname,"Net Cafe");
         break;
      case SITE_BUSINESS_CIGARBAR:
         lastname(str);
         strcpy(loc.name,"The ");
         strcat(loc.name,str);
         strcat(loc.name," Gentlemen's Club");
         strcpy(loc.shortname,"Cigar Bar");
         break;
      case SITE_BUSINESS_LATTESTAND:
         strcpy(loc.name,"");
         switch(LCSrandom(5))
         {
            case 0:strcat(loc.name,"Frothy");break;
            case 1:strcat(loc.name,"Milky");break;
            case 2:strcat(loc.name,"Caffeine");break;
            case 3:strcat(loc.name,"Morning");break;
            case 4:strcat(loc.name,"Evening");break;
         }
         strcat(loc.name," ");
         switch(LCSrandom(5))
         {
            case 0:strcat(loc.name,"Mug");break;
            case 1:strcat(loc.name,"Cup");break;
            case 2:strcat(loc.name,"Jolt");break;
            case 3:strcat(loc.name,"Wonder");break;
            case 4:strcat(loc.name,"Express");break;
         }
         strcat(loc.name," Latte Stand");
         strcpy(loc.shortname,"Latte Stand");
         break;
      case SITE_OUTDOOR_PUBLICPARK:
         lastname(loc.name);
         strcat(loc.name," Park");
         strcpy(loc.shortname,"Park");
         break;
      case SITE_RESIDENTIAL_BOMBSHELTER:
         strcpy(loc.name,"Fallout Shelter");
         strcpy(loc.shortname,"Bomb Shelter");
         break;
      case SITE_BUSINESS_BARANDGRILL:
         strcpy(loc.name,"Desert Eagle Bar & Grill");
         strcpy(loc.shortname,"Bar & Grill");
         break;
      case SITE_OUTDOOR_BUNKER:
         strcpy(loc.name,"Robert E. Lee Bunker");
         strcpy(loc.shortname,"Bunker");
         break;
      case SITE_BUSINESS_ARMSDEALER:
         strcpy(loc.name,"Black Market");
         strcpy(loc.shortname,"Black Market");
   }
}



/* daily - returns the number of days in the current month */
int monthday(void)
{
   switch(month)
   {
      case 4:return 30;
      case 6:return 30;
      case 9:return 30;
      case 11:return 30;
      case 2:
         if(year%4==0&&(year%100!=0||year%400==0))return 29;
         else return 28;
      default:
         return 31;
   }
}
