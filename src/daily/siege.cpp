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
#include <cstring>


/* TODO
make it less likely to be raided based on:
        - the number of sleepers.
        - how effective said sleepers are.
        - what action each sleeper is doing (promoting liberalism decreases chance while promoting conservatism increases chance)
        - what the sleeper does for a living (police officers are more influential than janitors, for instance)

make it more likely to be raided:
        - when a liberal hacks or steals credit cards.
        - dead bodies in the base. [DONE]
*/


/* siege - updates upcoming sieges */
void siegecheck(char canseethings)
{
   if(disbanding)return;



   //FIRST, THE COPS
   int numpres;
   for(int l=0;l<location.size();l++)
   {
      if(policestation_closed)continue;
      if(location[l]->siege.siege)continue;
      if(location[l]->renting==RENTING_NOCONTROL)continue;
      numpres=0;

      if(location[l]->siege.timeuntillocated==-2)
      {
         //IF JUST SIEGED, BUY SOME TIME
         location[l]->siege.timeuntillocated=-1;
      }
      else
      {
         //HUNTING
         if(location[l]->siege.timeuntillocated>0)
         {
            if(location[l]->front_business==-1||LCSrandom(2))
            {
               location[l]->siege.timeuntillocated--;
               // Hunt faster if location is extremely hot
               if(location[l]->heat > 300)
               {
                  int hunt_speed;
                  hunt_speed = location[l]->heat / 150;
                  while(hunt_speed&&location[l]->siege.timeuntillocated>1)
                  {
                     location[l]->siege.timeuntillocated--;
                     hunt_speed--;
                  }
               }
               if(offended_cops&&
                  location[l]->siege.timeuntillocated>1&&!LCSrandom(2))
               {
                  location[l]->siege.timeuntillocated--;
               }
            }
         }

         //CHECK FOR CRIMINALS AT THIS BASE
         int crimes=0;
         int kidnapped=0;
         int heatprotection=0;
         for(int p=0;p<pool.size();p++)
         {
            
            if(pool[p]->location!=l)continue; // People not at this base don't count
            if(!pool[p]->alive) // Corpses attract attention
            {
               crimes=10;
               continue;
            }
            if(pool[p]->flag & CREATUREFLAG_KIDNAPPED &&
               pool[p]->align!=1)kidnapped++; // Kidnapped persons increase heat
            if(pool[p]->align!=1)continue; // Non-liberals don't count other than that
            numpres++;
            
            // Cleanse record on things that aren't illegal right now
            if(law[LAW_FLAGBURNING]>0)pool[p]->crimes_suspected[LAWFLAG_BURNFLAG]=0;
            if(law[LAW_DRUGS]>0)pool[p]->crimes_suspected[LAWFLAG_BROWNIES]=0;
            if(law[LAW_IMMIGRATION]==2)pool[p]->flag &= ~CREATUREFLAG_ILLEGALALIEN;
            if(law[LAW_FREESPEECH]>-2)pool[p]->crimes_suspected[LAWFLAG_SPEECH]=0;

            //Heat doesn't matter for sieges until it gets high
            crimes=pool[p]->heat-5;
         }

         // Let the place slowly cool off if there are no criminals there
         if(!crimes&&location[l]->heat)
         {
            if(location[l]->heat > 500)
               location[l]->heat-=200-police_heat*40;
            else
               location[l]->heat-=5-police_heat;
            if(location[l]->heat<0)location[l]->heat=0;
         }
         else if(crimes*3>location[l]->heat+location[l]->heat_protection*100||kidnapped)
         {
            // Determine how effective your current safehouse
            // is at keeping the police confused
            location[l]->update_heat_protection();
            heatprotection=static_cast<int>(location[l]->heat_protection * 20);

            //Having hostages reduces protection
            if(kidnapped)
            {
               heatprotection-=kidnapped;
               crimes+=kidnapped*5;
            }

            if(heatprotection<0)heatprotection=0;

            crimes-=crimes*heatprotection/20;

            location[l]->heat+=LCSrandom(LCSrandom(crimes)+1);

            if(location[l]->siege.timeuntillocated==-1 &&
               location[l]->heat > 100 &&
               !LCSrandom(30))
            {
               if(LCSrandom(crimes) > 25)location[l]->siege.escalationstate++;
               if(LCSrandom(crimes) > 50)location[l]->siege.escalationstate++;
               if(LCSrandom(crimes) > 75)location[l]->siege.escalationstate++;
               if(location[l]->siege.escalationstate>3)
                  location[l]->siege.escalationstate=3;
               // Begin planning siege if high heat on location
               int siegetime = 5*(1 + 1 * heatprotection - kidnapped);
               if(siegetime<5)siegetime=5;
               location[l]->siege.timeuntillocated += siegetime + LCSrandom(siegetime);
            }
         }

         // *JDS* Sleepers at the police department may give a warning just before police raids
         if(location[l]->siege.timeuntillocated==1)
         {
            int policesleeperwarning=0;
            for(int pl=0;pl<pool.size();pl++)
            {
               if(pool[pl]->flag & CREATUREFLAG_SLEEPER&&
                  pool[pl]->location!=-1&& // <- this must be executed before the line below it
                  location[pool[pl]->location]->type==SITE_GOVERNMENT_POLICESTATION)
               {
                  if(pool[pl]->infiltration*100>LCSrandom(50)) { policesleeperwarning=1; break; }
               }
            }
            if(policesleeperwarning)
            {
               erase();
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr("You have received advance warning from your sleepers regarding");
               move(9,1);
               addstr("in imminent police raid on ");
               addlocationname(location[l]);
               addstr(".");
               
               if(location[l]->siege.escalationstate==1)
               {
                  move(11,1);
                  addstr("The police are requesting the aid of National Guard troops.");
               }
               if(location[l]->siege.escalationstate==2)
               {
                  move(11,1);
                  addstr("Plans are in motion for the National Guard to bring in armored tanks.");
               }
               if(location[l]->siege.escalationstate>=3)
               {
                  move(11,1);
                  addstr("The Air Force and National Guard are working closely on this.");
               }
               refresh();
               getch();
            }
         }

         //COPS RAID THIS LOCATION
         if(!location[l]->siege.timeuntillocated)
         {
            location[l]->siege.timeuntillocated=-2;
            /*location[l]->heat-=250;
            if(location[l]->heat<0)*/location[l]->heat=0;

            if(numpres>0)
            {
               erase();
               set_color(COLOR_WHITE,COLOR_BLACK,1);

               if(location[l]->siege.escalationstate>0)
               {
                  move(8,1);
                  addstr("The police have surrounded the ");
                  addlocationname(location[l]);
                  addstr(".");
                  location[l]->siege.underattack=0;
               }
               else
               {
                  move(8,1);
                  addstr("A police SWAT team is raiding the ");
                  addlocationname(location[l]);
                  addstr("!");
                  location[l]->siege.underattack=1;
               }

               //MENTION ESCALATION STATE
               if(location[l]->siege.escalationstate==1)
               {
                  move(9,1);
                  addstr("Some national guard troops have joined them.");
               }
               if(location[l]->siege.escalationstate==2)
               {
                  move(9,1);
                  addstr("You hear tanks rolling around outside.");
               }
               if(location[l]->siege.escalationstate>=3)
               {
                  move(9,1);
                  addstr("You hear jet bombers streak overhead.");
               }

               refresh();
               getch();

               statebrokenlaws(l);

               location[l]->siege.siege=1;
               location[l]->siege.siegetype=SIEGE_POLICE;
               location[l]->siege.lights_off=0;
               location[l]->siege.cameras_off=0;
            }
            else
            {
               erase();
               set_color(COLOR_WHITE,COLOR_BLACK,1);

               move(8,1);
               addstr("The cops have raided the ");
               addlocationname(location[l]);
               addstr(", an unoccupied safehouse.");

               refresh();
               getch();

               int y=9;

               for(int p=pool.size()-1;p>=0;p--)
               {
                  if(pool[p]->location!=l)continue;
                  if(!pool[p]->alive)
                  {
                     move(y,1);y++;
                     addstr(pool[p]->name);
                     addstr("'s corpse has been recovered.");
                     refresh();
                     getch();

                     delete pool[p];
                     if(pool[p]->align==1)
                     {
                        int boss=getpoolcreature(pool[p]->hireid);
                        if(boss!=-1&&pool[boss]->juice>50)
                        {
                           int juice=pool[boss]->juice-50;
                           if(juice>10)juice=10;
                           addjuice(*pool[boss],-juice);
                        }
                     }
                     pool.erase(pool.begin() + p);
                     continue;
                  }
                  if(pool[p]->align!=1)
                  {
                     move(y,1);y++;
                     addstr(pool[p]->name);
                     addstr(" has been rescued.");
                     refresh();
                     getch();

                     delete pool[p];
                     pool.erase(pool.begin() + p);
                     continue;
                  }
               }
                  
               for(int l2=0;l2<location[l]->loot.size();l2++)
               {
                  delete location[l]->loot[l2];
               }
               location[l]->loot.clear();

               for(int v=(int)vehicle.size()-1;v>=0;v--)
               {
                  if(vehicle[v]->location==l)
                  {
                     removecarprefs_pool(vehicle[v]->id);
                     delete vehicle[v];
                     vehicle.erase(vehicle.begin() + v);
                  }
               }
            }
         }

         //OTHER OFFENDABLE ENTITIES
            //CORPS
         if(location[l]->heat&&location[l]->siege.timeuntilcorps==-1&&!location[l]->siege.siege&&offended_corps&&!LCSrandom(600)&&numpres>0)
         {
            location[l]->siege.timeuntilcorps=LCSrandom(3)+1;
            // *JDS* CEO sleepers may give a warning before corp raids
            int ceosleepercount=0;
            for(int pl=0;pl<pool.size();pl++)
            {
               if(pool[pl]->flag & CREATUREFLAG_SLEEPER&&
                  pool[pl]->type==CREATURE_CORPORATE_CEO)
               {
                  if(pool[pl]->infiltration*100>LCSrandom(50)) { ceosleepercount=1; break; }
                  
               }
            }
            if(ceosleepercount||!LCSrandom(5))
            {
               erase();
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr("You have received ");
               if(ceosleepercount)addstr("a sleeper CEO's warning");
               else("an anonymous tip");
               addstr(" that the Corporations");
               move(9,1);
               addstr("are hiring mercenaries to attack ");
               if(ceosleepercount)addstr(location[l]->name);
               else addstr("the LCS");
               addstr(".");
               refresh();
               getch();
            }
         }
         else if(location[l]->siege.timeuntilcorps>0)location[l]->siege.timeuntilcorps--; // Corp raid countdown!
         else if(location[l]->siege.timeuntilcorps==0&&!location[l]->siege.siege&&offended_corps&&numpres>0)
         {
            location[l]->siege.timeuntilcorps=-1;
            // Corps raid!
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,1);

            move(8,1);
            addstr("Corporate mercenaries are raiding the ");
            addlocationname(location[l]);
            addstr("!");

            refresh();
            getch();

            location[l]->siege.siege=1;
            location[l]->siege.siegetype=SIEGE_CORPORATE;
            location[l]->siege.underattack=1;
            location[l]->siege.lights_off=0;
            location[l]->siege.cameras_off=0;
            offended_corps=0;
         }
         else if(location[l]->siege.timeuntilcorps==0)location[l]->siege.timeuntilcorps=-1; // Silently call off foiled corp raids

            //CONSERVATIVE CRIME SQUAD
         if(endgamestate>=ENDGAME_CCS_SIEGES&&endgamestate<ENDGAME_CCS_DEFEATED)
         {
            if(location[l]->heat&&location[l]->siege.timeuntilccs==-1&&!location[l]->siege.siege&&!LCSrandom(60)&&numpres>0)
            {
               location[l]->siege.timeuntilccs=LCSrandom(3)+1;
               // CCS sleepers may give a warning before raids
               int ccssleepercount=0;
               for(int pl=0;pl<pool.size();pl++)
               {
                  if(pool[pl]->flag & CREATUREFLAG_SLEEPER&&
                     (pool[pl]->type==CREATURE_CCS_VIGILANTE || pool[pl]->type==CREATURE_CCS_ARCHCONSERVATIVE ||
                      pool[pl]->type==CREATURE_CCS_MOLOTOV || pool[pl]->type==CREATURE_CCS_SNIPER))
                  {
                     if(pool[pl]->infiltration*100>LCSrandom(50)) { ccssleepercount=1; break; }
                  }
               }
               if(ccssleepercount>1)
               {
                  erase();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr("You have received a sleeper warning that the CCS");
                  move(9,1);
                  addstr(" is gearing up to attack ");
                  addstr(location[l]->name);
                  addstr(".");
                  refresh();
                  getch();
               }
            }
            else if(location[l]->siege.timeuntilccs>0)location[l]->siege.timeuntilccs--; // CCS raid countdown!
            else if(location[l]->siege.timeuntilccs==0&&!location[l]->siege.siege&&numpres>0)
            {
               location[l]->siege.timeuntilccs=-1;
               // CCS raid!
               erase();
               set_color(COLOR_WHITE,COLOR_BLACK,1);

               move(8,1);
               addstr("A screeching truck pulls up to ");
               addlocationname(location[l]);
               addstr("!");

               refresh();
               getch();

               if(!(location[l]->compound_walls & COMPOUND_TANKTRAPS) &&
                  !LCSrandom(5))
               {
                  // CCS Carbombs safehouse!!
                  erase();
                  set_color(COLOR_RED,COLOR_BLACK,1);

                  move(8,1);
                  addstr("The truck barrels into the building and explodes massively!");

                  refresh();
                  getch();

                  erase();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);

                  move(0,1);
                  addstr("CCS CAR BOMBING CASUALTY REPORT");

                  move(2,1);
                  addstr("KILLED: ");
                  int killed_y = 2;
                  int killed_x = 9;

                  move(12,1);
                  addstr("INJURED: ");
                  int injured_y = 12;
                  int injured_x = 10;

                  for(int i=0;i<pool.size();i++)
                  {
                     if(pool[i]->location==l)
                     {
                        if(LCSrandom(2))
                        {
                           int namelength=strlen(pool[i]->name);
                           pool[i]->blood-=LCSrandom(101-pool[i]->juice/10)+10;
                           if(pool[i]->blood<0)
                           {
                              if(killed_x+namelength>79)
                              {
                                 killed_y++;
                                 killed_x=1;
                              }
                              move(killed_y,killed_x);
                              pool[i]->alive=0;
                           }
                           else
                           {
                              if(injured_x+namelength>79)
                              {
                                 injured_y++;
                                 injured_x=1;
                              }
                              move(injured_y,injured_x);
                           }
                           set_alignment_color(pool[i]->align,false);
                           addstr(pool[i]->name);
                        }
                     }
                  }

                  refresh();
                  getch();
               }
               else
               {
                  // CCS Raids safehouse
                  erase();
                  set_color(COLOR_YELLOW,COLOR_BLACK,1);

                  move(8,1);
                  addstr("The CCS is attacking the safehouse!");

                  refresh();
                  getch();

                  location[l]->siege.siege=1;
                  location[l]->siege.siegetype=SIEGE_CCS;
                  location[l]->siege.underattack=1;
                  location[l]->siege.lights_off=0;
                  location[l]->siege.cameras_off=0;
               }
            }
            else if(location[l]->siege.timeuntilccs==0)location[l]->siege.timeuntilccs=-1; // Silently call off foiled ccs raids
         }


            //CIA
         if(location[l]->heat&&location[l]->siege.timeuntilcia==-1&&!location[l]->siege.siege&&offended_cia&&!LCSrandom(600)&&numpres>0)
         {
            location[l]->siege.timeuntilcia=LCSrandom(3)+1;
            // *JDS* agent sleepers may give a warning before cia raids
            int agentsleepercount=0;
            for(int pl=0;pl<pool.size();pl++)
            {
               if(pool[pl]->flag & CREATUREFLAG_SLEEPER&&
                  pool[pl]->type==CREATURE_AGENT)
               {
                  if(pool[pl]->infiltration*100>LCSrandom(100)) { agentsleepercount=1; break; }
               }
            }
            if(agentsleepercount)
            {
               erase();
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr("A sleeper agent has reported that the CIA is planning");
               move(9,1);
               addstr("to launch an attack on ");
               addstr(location[l]->name);
               addstr(".");
               refresh();
               getch();
            }
         }
         else if(location[l]->siege.timeuntilcia>0)location[l]->siege.timeuntilcia--; // CIA raid countdown!
         else if(location[l]->siege.timeuntilcia==0&&!location[l]->siege.siege&&offended_cia&&numpres>0)
         {
            location[l]->siege.timeuntilcia=-1;
            // CIA raids!
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,1);

            move(8,1);
            addstr("Unmarked black vans are surrounding the ");
            addlocationname(location[l]);
            addstr("!");

            if(location[l]->compound_walls & COMPOUND_CAMERAS)
            {
               move(9,1);
               addstr("Through some form of high technology, they've managed");
               move(10,1);
               addstr("to shut off the lights and the cameras!");
            }
            else if(location[l]->compound_walls & COMPOUND_GENERATOR)
            {
               move(9,1);
               addstr("Through some form of high technology, they've managed");
               move(10,1);
               addstr("to shut off the lights!");
            }
            else
            {
               move(9,1);
               addstr("They've shut off the lights!");
            }

            refresh();
            getch();

            location[l]->siege.siege=1;
            location[l]->siege.siegetype=SIEGE_CIA;
            location[l]->siege.underattack=1;
            location[l]->siege.lights_off=1;
            location[l]->siege.cameras_off=1;
         }
         else if(location[l]->siege.timeuntilcia==0)location[l]->siege.timeuntilcia=-1; // Silently call off foiled cia raids
                   
            //HICKS
         if(!location[l]->siege.siege&&offended_amradio&&attitude[VIEW_AMRADIO]<=35&&!LCSrandom(600)&&numpres>0)
         {
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,1);

            move(8,1);
            addstr("Masses dissatisfied with your lack of respect for AM Radio");
            move(9,1);
            addstr("are storming the ");
            addlocationname(location[l]);
            addstr("!");

            refresh();
            getch();

            location[l]->siege.siege=1;
            location[l]->siege.siegetype=SIEGE_HICKS;
            location[l]->siege.underattack=1;
            location[l]->siege.lights_off=0;
            location[l]->siege.cameras_off=0;
            offended_amradio=0;
         }
         if(!location[l]->siege.siege&&offended_cablenews&&attitude[VIEW_CABLENEWS]<=35&&!LCSrandom(600)&&numpres>0)
         {
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,1);

            move(8,1);
            addstr("Masses dissatisfied with your lack of respect for Cable News");
            move(9,1);
            addstr("are storming the ");
            addlocationname(location[l]);
            addstr("!");

            refresh();
            getch();

            location[l]->siege.siege=1;
            location[l]->siege.siegetype=SIEGE_HICKS;
            location[l]->siege.underattack=1;
            location[l]->siege.lights_off=0;
            location[l]->siege.cameras_off=0;
            offended_cablenews=0;
         }
         //Firemen
         if(law[LAW_FREESPEECH]==-2 && location[l]->siege.timeuntilfiremen==-1 && !location[l]->siege.siege &&
            offended_firemen && numpres>0 && location[l]->compound_walls & COMPOUND_PRINTINGPRESS && !LCSrandom(90))
         {
            location[l]->siege.timeuntilfiremen=LCSrandom(3)+1;

            // Sleeper Firemen can warn you of an impending raid
            int firemensleepercount=0;
            for(int pl=0;pl<pool.size();pl++)
            {
               if(pool[pl]->flag & CREATUREFLAG_SLEEPER&&
                  pool[pl]->type==CREATURE_FIREFIGHTER)
               {
                  firemensleepercount++;
               }
            }
            if(LCSrandom(firemensleepercount+1)>0||!LCSrandom(10))
            {
               erase();
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               if(firemensleepercount)
               {
                  addstr("A sleeper Fireman has informed you that");
               } else
               {
                  addstr("Word in the underground is that");
               }
               move(9,1);
               addstr("the Firemen are planning to burn ");
               addstr(location[l]->name);
               addstr(".");
               refresh();
               getch();
            }

         } else if(location[l]->siege.timeuntilfiremen>0)location[l]->siege.timeuntilfiremen--;
         else if(location[l]->siege.timeuntilfiremen==0 && !location[l]->siege.siege&&numpres>0)
         {
            location[l]->siege.timeuntilfiremen=-1;
            // Firemen raid!
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,1);

            move(8,1);
            addstr("Screaming fire engines pull up to the ");
            addlocationname(location[l]);
            addstr("!");

            move(9,1);
            addstr("Armored firemen swarm out, pilot lights burning.");
            
            refresh();
            getch();

            erase();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(1,1);
            addstr("You hear a screeching voice over the sound of fire engine sirens:");

            move(3,1);
            addstr("Surrender yourselves!");

            move(4,1);
            addstr("Unacceptable Speech has occurred at this location.");

            move(6,1);
            addstr("Come quietly and you will not be harmed.");

            refresh();
            getch();

            location[l]->siege.siege=1;
            location[l]->siege.siegetype=SIEGE_FIREMEN;
            location[l]->siege.underattack=1;
            location[l]->siege.lights_off=0;
            location[l]->siege.cameras_off=0;
            offended_firemen=0;
         } 
         else if(location[l]->siege.timeuntilfiremen==0)
         {
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,1);

            move(8,1);
            addstr("The Firemen have raided the ");
            addlocationname(location[l]);
            addstr(", an unoccupied safehouse.");

            refresh();
            getch();

            int y=9;

            for(int p=pool.size()-1;p>=0;p--)
            {
               if(pool[p]->location!=l)continue;
               if(!pool[p]->alive)
               {
                  move(y,1);y++;
                  addstr(pool[p]->name);
                  addstr("'s corpse has been recovered.");
                  refresh();
                  getch();

                  delete pool[p];
                  if(pool[p]->align==1)
                  {
                     int boss=getpoolcreature(pool[p]->hireid);
                     if(boss!=-1&&pool[boss]->juice>50)
                     {
                        int juice=pool[boss]->juice-50;
                        if(juice>10)juice=10;
                        addjuice(*pool[boss],-juice);
                     }
                  }
                  pool.erase(pool.begin() + p);
                  continue;
               }
               if(pool[p]->align!=1)
               {
                  move(y,1);y++;
                  addstr(pool[p]->name);
                  addstr(" has been rescued.");
                  refresh();
                  getch();

                  delete pool[p];
                  pool.erase(pool.begin() + p);
                  continue;
               }
            }
            for(int l2=0;l2<location[l]->loot.size();l2++)
            {
               delete location[l]->loot[l2];
            }
            location[l]->loot.clear();

            if(location[l]->compound_walls & COMPOUND_PRINTINGPRESS)
            {
               move(10,1);
               addstr("The printing press is dismantled and burned.");
               location[l]->compound_walls &= ~COMPOUND_PRINTINGPRESS;
               offended_firemen=0;
            }

            if(location[l]->front_business!=-1)
            {
               move(12,1);
               addstr("Materials relating to the business front have been destroyed.");
               location[l]->front_business=-1;
            }
         }
      }
   }
}



/* siege - updates sieges in progress */
void siegeturn(char clearformess)
{
   if(disbanding)return;

   // Count people at each location
   int l;
   int hs=-1;
   int* liberalcount = new int[location.size()];
   char* food_prep   = new char[location.size()];

   // Clear food_prep list
   std::memset(food_prep,0,location.size());

   for(int p=0;p<pool.size();p++)
   {
      if(!pool[p]->alive)continue; // Dead people don't count
      if(pool[p]->align!=1)continue; // Non-liberals don't count
      if(pool[p]->location==-1)continue; // Vacationers don't count
      liberalcount[pool[p]->location]++;
      //Get the best cooking skill for each location
      if(food_prep[pool[p]->location]<pool[p]->skillval(SKILL_COOKING))
         food_prep[pool[p]->location]=pool[p]->skillval(SKILL_COOKING);
   }
   for(l=0;l<location.size();l++)
   {
      if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
      {
         hs=l;
         break;
      }
   }

   for(l=0;l<location.size();l++)
   {
      if(!location[l]->siege.siege && location[l]->renting>=0 && location[l]->type!=SITE_RESIDENTIAL_SHELTER)
      {
         //locations not under siege eat
         int eat=numbereating(l);
         int price=(int)(3*eat*((10-food_prep[l])/10.0f)+0.5f);
         if(price<0)price=0;
         if(ledger.get_funds()>=price)
         {
            ledger.subtract_funds(price,EXPENSE_FOOD);
         }
         else
         {
            if(location[l]->compound_stores>=eat)location[l]->compound_stores-=eat;
            else if(liberalcount[l])
            {
               int moved=0;
               for(int p=0;p<pool.size();p++)
               {
                  if(!pool[p]->alive)continue; // Dead people don't move
                  //if(pool[p]->align!=1)continue; // Non-liberals DO move
                  if(pool[p]->location==l)
                  {
                     moved++;
                     pool[p]->base=hs;
                     pool[p]->location=hs;
                  }
               }
               
               erase();
               set_color(COLOR_WHITE,COLOR_BLACK,1);

               move(8,1);
               addstr("The LCS has no money for food.");
               move(10,1);
               addlocationname(location[l]);
               addstr(" has been abandoned.");
               move(12,1);
               addstr("The Liberals will return to the homeless shelter for handouts.");
               move(14,1);
               addstr("Neither Governments nor Revolutions are free. . .");

               refresh();
               getch();
            }
         }
      }
      else if(location[l]->siege.siege)
      {
         //resolve sieges with no people
         if(liberalcount[l]==0)
         {
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,1);

            move(8,1);
            addstr("Conservatives have raided the ");
            addlocationname(location[l]);
            addstr(", an unoccupied safehouse.");

            if(location[l]->siege.siegetype==SIEGE_CCS)
            {
               if(location[l]->type==SITE_INDUSTRY_WAREHOUSE)
                  location[l]->renting=RENTING_CCS; // CCS Captures warehouse
            }

            refresh();
            getch();

            int y=9;

            for(int p=pool.size()-1;p>=0;p--)
            {
               if(pool[p]->location!=l)continue;
               if(!pool[p]->alive)
               {
                  move(y,1);y++;
                  addstr(pool[p]->name);
                  addstr("'s corpse has been recovered.");
                  refresh();
                  getch();

                  delete pool[p];
                  if(pool[p]->align==1)
                  {
                     int boss=getpoolcreature(pool[p]->hireid);
                     if(boss!=-1&&pool[boss]->juice>50)
                     {
                        int juice=pool[boss]->juice-50;
                        if(juice>10)juice=10;
                        addjuice(*pool[boss],-juice);
                     }
                  }
                  pool.erase(pool.begin() + p);
                  continue;
               }
               if(pool[p]->align!=1)
               {
                  move(y,1);y++;
                  addstr(pool[p]->name);
                  addstr(" has been rescued.");
                  refresh();
                  getch();

                  delete pool[p];
                  pool.erase(pool.begin() + p);
                  continue;
               }
            }
            for(int l2=0;l2<location[l]->loot.size();l2++)
            {
               delete location[l]->loot[l2];
            }
            location[l]->loot.clear();

            for(int v=(int)vehicle.size()-1;v>=0;v--)
            {
               if(vehicle[v]->location==l)
               {
                  removecarprefs_pool(vehicle[v]->id);
                  delete vehicle[v];
                  vehicle.erase(vehicle.begin() + v);
               }
            }
         }

         if(!location[l]->siege.underattack)
         {
            //EAT
            bool starving=false;
            int eaters=numbereating(l);
            if(location[l]->compound_stores==0)
            {
               starving=true;
            }
            if(location[l]->compound_stores>=eaters)location[l]->compound_stores-=eaters;
            else location[l]->compound_stores=0;

            //ATTACK!
            char attack=0;
            int kidnapped=0;
            int criminalcount=0;

            for(int p=0;p<pool.size();p++)
            {
               if(!pool[p]->alive)continue;
               if(pool[p]->location!=l)continue;

               if(starving)pool[p]->blood-=LCSrandom(5)+1;

               if(pool[p]->flag & CREATUREFLAG_KIDNAPPED)
               {
                  kidnapped++;
               }

               if(iscriminal(*pool[p]))criminalcount++;
            }

            if(!LCSrandom(1+criminalcount+kidnapped*5))attack=1;

            if(attack)
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
               addstr("The cops are coming!");

               refresh();
               getch();

               location[l]->siege.underattack=1;
            }
            else
            {
               char no_bad=1;

               //CUT LIGHTS
               if(!location[l]->siege.lights_off &&
                  !(location[l]->compound_walls & COMPOUND_GENERATOR) && !LCSrandom(3))
               {
                  no_bad=0;

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
                  addstr("The police have cut the lights!");

                  refresh();
                  getch();

                  location[l]->siege.lights_off=1;
               }

               //SNIPER
               if(!(location[l]->compound_walls & COMPOUND_BASIC) && !LCSrandom(5))
               {
                  no_bad=0;

                  vector<int> pol;
                  for(int p=0;p<pool.size();p++)
                  {
                     if(pool[p]->alive&&pool[p]->location==l)
                     {
                        pol.push_back(p);
                     }
                  }

                  if(pol.size()>0)
                  {
                     if(clearformess)erase();
                     else makedelimiter(8,0);
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(8,1);
                     int targ=pol[LCSrandom(pol.size())];
                     if((int)LCSrandom(50)>pool[targ]->juice)
                     {
                        addstr("A sniper takes out ");
                        addstr(pool[targ]->name);
                        addstr("!");

                        if(pool[targ]->align==1)
                        {
                           stat_dead++;
                           liberalcount[l]--;
                        }

                        removesquadinfo(*pool[targ]);

                        pool[targ]->alive=0;
                        //delete pool[targ];
                        //pool.erase(pool.begin() + targ);
                     }
                     else
                     {
                        addstr("A sniper nearly hits ");
                        addstr(pool[targ]->name);
                        addstr("!");
                     }
                     refresh();
                     getch();
                  }
               }
            
               if(location[l]->siege.escalationstate>=3 && !LCSrandom(3))
               {
                  no_bad=0;

                  //AIR STRIKE!
                  char hit=!LCSrandom(3);
                  if(!(location[l]->compound_walls & COMPOUND_GENERATOR))hit=0;

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
                  addstr("You hear planes streak over head!");
                  refresh();
                  getch();
                  if(clearformess)
                  {
                     erase();
                  }
                  else
                  {
                     makedelimiter(8,0);
                  }
                  move(8,1);
                  addstr("Explosions rock the compound!");
                  refresh();
                  getch();

                  if(hit)
                  {
                     if(clearformess)erase();
                     else makedelimiter(8,0);
                     move(8,1);
                     addstr("The generator has been destroyed!");
                     refresh();
                     getch();
                     if(clearformess)erase();
                     else makedelimiter(8,0);
                     move(8,1);
                     addstr("The lights fade and all is dark. ");
                     refresh();
                     getch();
                  }

                  if(!LCSrandom(2))
                  {
                     vector<int> pol;
                     for(int p=0;p<pool.size();p++)
                     {
                        if(pool[p]->alive&&pool[p]->location==l)
                        {
                           pol.push_back(p);
                        }
                     }

                     if(pol.size()>0)
                     {
                        if(clearformess)erase();
                        else makedelimiter(8,0);
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(8,1);
                        int targ=pol[LCSrandom(pol.size())];
                        if((int)LCSrandom(100)>pool[targ]->juice)
                        {
                           addstr(pool[targ]->name);
                           addstr(" was killed in the bombing!");

                           if(pool[targ]->align==1)
                           {
                              stat_dead++;
                              liberalcount[l]--;
                           }

                           removesquadinfo(*pool[targ]);

                           pool[targ]->alive=0;
                           //delete pool[targ];
                           //pool.erase(pool.begin() + targ);
                        }
                        else
                        {
                           addstr(pool[targ]->name);
                           addstr(" narrowly avoided death!");
                        }
                        refresh();
                        getch();
                     }
                  }
                  else
                  {
                     if(clearformess)erase();
                     else makedelimiter(8,0);
                     move(8,1);
                     addstr("Fortunately, no one is hurt.");
                     refresh();
                     getch();
                  }

                  if(hit)
                  {
                     location[l]->compound_walls&=~COMPOUND_GENERATOR;
                     location[l]->siege.lights_off=1;
                  }
               }
               if((location[l]->compound_walls & COMPOUND_TANKTRAPS) &&
                  location[l]->siege.escalationstate>=3 && !LCSrandom(50))
               {
                  no_bad=0;

                  //ENGINEERS
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
                  addstr("Engineers have removed your tank traps.");
                  refresh();
                  getch();

                  location[l]->compound_walls&=~COMPOUND_TANKTRAPS;
               }

               //NEED GOOD THINGS TO BALANCE THE BAD

               // ELITE REPORTER SNEAKS IN
               if(!LCSrandom(50)&&no_bad&&liberalcount[l]>0)
               {
                  char repname[200];
                  generate_name(repname);

                  set_color(COLOR_WHITE,COLOR_BLACK,0);

                  erase();
                  move(1,1);
                  addstr("Elitist ");
                  addstr(repname);
                  addstr(" from the ");
                  switch(LCSrandom(5))
                  {
                     case 0:addstr("news program");break;
                     case 1:addstr("news magazine");break;
                     case 2:addstr("website");break;
                     case 3:addstr("scandal rag");break;
                     case 4:addstr("newspaper");break;
                  }
                  addstr(" ");
                  switch(LCSrandom(12))
                  {
                     case 0:addstr("Daily");break;
                     case 1:addstr("Nightly");break;
                     case 2:addstr("Current");break;
                     case 3:addstr("Pressing");break;
                     case 4:addstr("Socialist");break;
                     case 5:addstr("American");break;
                     case 6:addstr("National");break;
                     case 7:addstr("Union");break;
                     case 8:addstr("Foreign");break;
                     case 9:addstr("Associated");break;
                     case 10:addstr("International");break;
                     case 11:addstr("County");break;
                  }
                  addstr(" ");
                  switch(LCSrandom(11))
                  {
                     case 0:addstr("Reporter");break;
                     case 1:addstr("Issue");break;
                     case 2:addstr("Take");break;
                     case 3:addstr("Constitution");break;
                     case 4:addstr("Times");break;
                     case 5:addstr("Post");break;
                     case 6:addstr("News");break;
                     case 7:addstr("Affair");break;
                     case 8:addstr("Statesman");break;
                     case 9:addstr("Star");break;
                     case 10:addstr("Inquirer");break;
                  }
                  move(2,1);
                  addstr("got into the compound somehow!");
                  refresh();
                  getch();

                  int best=-1,bestvalue=0,sum;
                  for(int p=0;p<pool.size();p++)
                  {
                     if(!pool[p]->alive)continue;
                     if(pool[p]->align!=1)continue;
                     if(pool[p]->location!=l)continue;

                     sum=0;
                     sum+=pool[p]->attval(ATTRIBUTE_INTELLIGENCE);
                     sum+=pool[p]->attval(ATTRIBUTE_HEART);
                     sum+=pool[p]->attval(ATTRIBUTE_CHARISMA)*2;
                     sum+=pool[p]->skillval(SKILL_PERSUASION)*3;
                     sum+=pool[p]->skillval(SKILL_LEADERSHIP)*5;

                     if(sum>bestvalue||best==-1)
                     {
                        best=p;
                        bestvalue=sum;
                     }
                  }

                  move(4,1);
                  addstr(pool[best]->name);
                  addstr(" decides to give an interview.");
                  refresh();
                  getch();

                  move(6,1);
                  addstr("The interview is wide-ranging, covering a variety of topics.");
                  refresh();
                  getch();

                  int segmentpower=LCSrandom(bestvalue*2+1);

                  move(8,1);
                  if(segmentpower<10)
                  {
                     addstr(repname);
                     addstr(" cancelled the interview halfway through");
                     move(9,1);
                     addstr("and later used the material for a Broadway play called");
                     move(10,1);
                     switch(LCSrandom(11))
                     {
                        case 0:addstr("Flaming");break;
                        case 1:
                                if(law[LAW_FREESPEECH]==-2) {
                                        addstr("Dumb");
                                } else {
                                        addstr("Retarded");
                                }
                                break;
                        case 2:addstr("Insane");break;
                        case 3:addstr("Crazy");break;
                        case 4:addstr("Loopy");break;
                        case 5:addstr("Idiot");break;
                        case 6:addstr("Empty-Headed");break;
                        case 7:addstr("Nutty");break;
                        case 8:addstr("Half-Baked");break;
                        case 9:addstr("Pot-Smoking");break;
                        case 10:addstr("Stoner");break;
                     }
                     addstr(" ");
                     switch(LCSrandom(10))
                     {
                        case 0:addstr("Liberal");break;
                        case 1:addstr("Socialist");break;
                        case 2:addstr("Anarchist");break;
                        case 3:addstr("Communist");break;
                        case 4:addstr("Marxist");break;
                        case 5:addstr("Green");break;
                        case 6:addstr("Elite");break;
                        case 7:addstr("Guerrilla");break;
                        case 8:addstr("Commando");break;
                        case 9:addstr("Soldier");break;
                     }
                     addstr(".");
                  }
                  else if(segmentpower<15)
                  {
                     addstr("But the interview is so boring that ");
                     addstr(repname);
                     addstr(" falls asleep.");
                  }
                  else if(segmentpower<20)addstr("But the interview sucked.");
                  else if(segmentpower<25)addstr("It was nothing special, though.");
                  else if(segmentpower<32)addstr("It went pretty well.");
                  else if(segmentpower<40)addstr("The discussion was exciting and dynamic.");
                  else if(segmentpower<50)addstr("It was almost perfect.");
                  else
                  {
                     addstr(repname);
                     addstr(" later went on to win a Pulitzer for it.");
                     move(9,1);
                     addstr("Virtually everyone in America was moved by ");
                     addstr(pool[best]->name);
                     addstr("'s words.");
                  }
                  refresh();
                  getch();

                  //CHECK PUBLIC OPINION
                  change_public_opinion(VIEW_LIBERALCRIMESQUAD,20);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,(segmentpower-25)/2,segmentpower+50);
                  int viewhit;
                  for(int v=0;v<5;v++)
                  {
                     viewhit=LCSrandom(VIEWNUM-3);
                     change_public_opinion(viewhit,(segmentpower-25)/2);
                  }
               }
            }
         }
      }
   }
   delete[] liberalcount;
   delete[] food_prep;
}



/* siege - handles giving up */
void giveup(void)
{
   int loc=-1;
   if(selectedsiege!=-1)loc=selectedsiege;
   if(activesquad!=NULL)loc=activesquad->squad[0]->location;
   if(loc==-1)return;

   if(location[loc]->renting>1)location[loc]->renting=RENTING_NOCONTROL;

   //IF POLICE, END SIEGE
   if(location[loc]->siege.siegetype==SIEGE_POLICE ||
      location[loc]->siege.siegetype==SIEGE_FIREMEN)
   {
      int polsta=-1;
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_GOVERNMENT_POLICESTATION)
         {
            polsta=l;
            break;
         }
      }
      

      //END SIEGE
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(1,1);
      if(location[loc]->siege.siegetype==SIEGE_POLICE)
         addstr("The police");
      else
         addstr("The firemen");
      addstr(" confiscate everything, including Squad weapons.");

      int kcount=0;
      int pcount=0;
      char kname[100];
      char pname[100];
      char pcname[100];
      int icount=0;
      int p;
      for(p=pool.size()-1;p>=0;p--)
      {
         if(pool[p]->location!=loc)continue;
         if(!pool[p]->alive)continue;

         if(pool[p]->flag & CREATUREFLAG_ILLEGALALIEN)icount++;

         if(pool[p]->flag & CREATUREFLAG_MISSING && pool[p]->align==-1)
         {
            kcount++;
            strcpy(kname,pool[p]->propername);
            if(pool[p]->type==CREATURE_RADIOPERSONALITY)offended_amradio=1;
            if(pool[p]->type==CREATURE_NEWSANCHOR)offended_cablenews=1;
            //clear interrogation data if deleted
            delete reinterpret_cast<interrogation*>(pool[p]->activity.arg);
         }
      }

      //CRIMINALIZE POOL IF FOUND WITH KIDNAP VICTIM OR ALIEN
      if(kcount>0)criminalizepool(LAWFLAG_KIDNAPPING,-1,loc);
      if(icount>0)criminalizepool(LAWFLAG_HIREILLEGAL,-1,loc);

      if(location[loc]->siege.siegetype==SIEGE_FIREMEN)
      {
         if(location[loc]->compound_walls & COMPOUND_PRINTINGPRESS)
         {
            // Criminalize pool for unacceptable speech
            if(icount>0)criminalizepool(LAWFLAG_SPEECH,-1,loc);
         }
      }

      //LOOK FOR PRISONERS (MUST BE AFTER CRIMINALIZATION ABOVE)
      for(p=pool.size()-1;p>=0;p--)
      {
         if(pool[p]->location!=loc)continue;
         if(!pool[p]->alive)continue;

         if(iscriminal(*pool[p])&&!(pool[p]->flag & CREATUREFLAG_MISSING && pool[p]->align==-1))
         {
            pcount++;
            strcpy(pname,pool[p]->propername);
            strcpy(pcname,pool[p]->name);
         }
      }

      if(kcount==1)
      {
         move(3,1);
         addstr(kname);
         addstr(" is rehabilitated and freed.");
      }
      if(kcount>1)
      {
         move(3,1);
         addstr("The kidnap victims are rehabilitated and freed.");
      }
      if(pcount==1)
      {
         move(5,1);
         addstr(pname);
         if(stricmp(pname,pcname))
         {
            addstr(", aka ");
            addstr(pcname);
            addstr(",");
         }
         move(6,1);
         addstr("is taken to the police station.");
      }
      if(pcount>1)
      {
         char num[20];
         move(5,1);
         itoa(pcount,num,10);
         addstr(num);
         addstr(" Liberals are taken to the police station.");
      }
      if(ledger.get_funds()>0)
      {
         if(ledger.get_funds()<=10000 || location[loc]->siege.siegetype==SIEGE_FIREMEN)
         {
            move(8,1);
            addstr("Fortunately, your funds remain intact.");
         }
         else
         {
            move(8,1);
            int confiscated = LCSrandom(LCSrandom(ledger.get_funds()-10000)+1)+1000;
            char str[100];
            sprintf(str,"Law enforcement have confiscated $%d in LCS funds.",confiscated);
            addstr(str);
            ledger.subtract_funds(confiscated,EXPENSE_CONFISCATED);
         }
      }
      if(location[loc]->siege.siegetype==SIEGE_FIREMEN)
      {
         if(location[loc]->compound_walls & COMPOUND_PRINTINGPRESS)
         {
            // Criminalize pool for unacceptable speech
            if(icount>0)criminalizepool(LAWFLAG_SPEECH,-1,loc);

            move(10,1);
            addstr("The printing press is dismantled and burned.");
            location[loc]->compound_walls &= ~COMPOUND_PRINTINGPRESS;
         }
      }
      else
      {
         if(location[loc]->compound_walls)
         {
            move(10,1);
            addstr("The compound is dismantled.");
            location[loc]->compound_walls=0;
         }
      }
      if(location[loc]->front_business!=-1)
      {
         move(12,1);
         addstr("Materials relating to the business front have been taken.");
         location[loc]->front_business=-1;
      }

      refresh();
      getch();

      location[loc]->siege.siege=0;
      if(location[loc]->siege.siegetype==SIEGE_FIREMEN)
         offended_firemen=0; // Firemen do not hold grudges

      for(p=pool.size()-1;p>=0;p--)
      {
         if(pool[p]->location!=loc)continue;

         //ALL KIDNAP VICTIMS FREED REGARDLESS OF CRIMES
         if((pool[p]->flag & CREATUREFLAG_MISSING)||
            !pool[p]->alive)
         {
            // Clear actions for anybody who was tending to this person
            for(int i=0;i<pool.size();++i)
            {
               if(!pool[i]->alive)continue;
               if(pool[i]->activity.type==ACTIVITY_HOSTAGETENDING)
               {
                  if(pool[i]->activity.arg==pool[p]->id)
                  {
                     pool[i]->activity.type=ACTIVITY_NONE;
                  }
               }
            }

            removesquadinfo(*pool[p]);
            delete pool[p];
            if(pool[p]->align==1)
            {
               int boss=getpoolcreature(pool[p]->hireid);
               if(boss!=-1&&pool[boss]->juice>50)
               {
                  int juice=pool[boss]->juice-50;
                  if(juice>10)juice=10;
                  addjuice(*pool[boss],-juice);
               }
            }
            pool.erase(pool.begin() + p);
            continue;
         }

         //TAKE SQUAD EQUIPMENT
         if(pool[p]->squadid!=-1)
         {
            long sq=getsquad(pool[p]->squadid);
            if(sq!=-1)
            {
               for(int l=0;l<squad[sq]->loot.size();l++)delete squad[sq]->loot[l];
               squad[sq]->loot.clear();
            }
         }

         pool[p]->weapon.type=WEAPON_NONE;
         pool[p]->weapon.ammo=0;
         
         if(iscriminal(*pool[p]))
         {
            removesquadinfo(*pool[p]);
            pool[p]->location=polsta;
            pool[p]->activity.type=ACTIVITY_NONE;
         }
      }

      location[loc]->siege.siege=0;
   }
   else
   {
      //OTHERWISE IT IS SUICIDE
      int killnumber=0;
      for(int p=pool.size()-1;p>=0;p--)
      {
         if(pool[p]->location!=loc)continue;

         if(pool[p]->alive&&pool[p]->align==1)stat_dead++;

         killnumber++;
         removesquadinfo(*pool[p]);
         pool[p]->alive=0;
         pool[p]->location=-1;
         //delete pool[p];
         //pool.erase(pool.begin() + p);
      }

      if(location[loc]->siege.siegetype==SIEGE_CCS)
      {
         if(location[loc]->type==SITE_INDUSTRY_WAREHOUSE)
            location[loc]->renting=RENTING_CCS; // CCS Captures warehouse
      }

      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(1,1);
      addstr("Everyone in the ");
      addlocationname(location[loc]);
      addstr(" is slain.");
      refresh();
      getch();


      newsstoryst *ns=new newsstoryst;
         ns->type=NEWSSTORY_MASSACRE;
         ns->loc=loc;
         ns->crime.push_back(location[loc]->siege.siegetype);
         ns->crime.push_back(killnumber);
      newsstory.push_back(ns);

      //MUST SET cursite TO SATISFY endcheck() CODE
      cursite=loc;
      endcheck();
      cursite=-1;

      location[loc]->siege.siege=0;
   }

   //CONFISCATE MATERIAL
   for(int l=0;l<location[loc]->loot.size();l++)delete location[loc]->loot[l];
   location[loc]->loot.clear();
   for(int v=(int)vehicle.size()-1;v>=0;v--)
   {
      if(vehicle[v]->location==loc)
      {
         removecarprefs_pool(vehicle[v]->id);
         delete vehicle[v];
         vehicle.erase(vehicle.begin() + v);
      }
   }
}



/* siege - checks how many days of food left at the site */
int fooddaysleft(int loc)
{
   int eaters=numbereating(loc);

   if(eaters==0)return -1;

   int days=location[loc]->compound_stores/eaters;
   if((location[loc]->compound_stores%eaters)>eaters/2)days++;
   return days;
}



/* siege - checks how many people are eating at the site */
int numbereating(int loc)
{
   int eaters=0;

   for(int p=0;p<pool.size();p++)
   {
      //Not here? Not eating here!
      if(pool[p]->location!=loc)continue;
      //Not alive? Not eating!
      if(!pool[p]->alive)continue;
      //You're a prisoner getting starved? Not eating!
      if(pool[p]->align==-1 &&
         reinterpret_cast<interrogation*>(pool[p]->activity.arg)->nofood)continue;
      //You're a sleeper agent? Sleepers don't eat! Feh! (Rather, they eat on Conservatives' expense, not yours.)
      if(pool[p]->flag & CREATUREFLAG_SLEEPER)continue;
      //None of the above? You're eating!
      eaters++;
   }

   return eaters;
}



/* siege - prepares for entering site mode to fight the siege */
void escape_engage(void) // FIXME: Wait... LCC? //Liberal Command Center. Probably a pre-3.0 term. Fixed. -Fox
{
   //GIVE INFO SCREEN
   erase();
   set_color(COLOR_RED,COLOR_BLACK,1);
   move(1,26);
   addstr("UNDER SIEGE: ESCAPE OR ENGAGE");

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(3,16);
   addstr("You are about to engage Conservative forces in battle.");
   move(4,11);
   addstr("You will find yourself in the Liberal safehouse, and it will");
   move(5,11);
   addstr("be swarming with Conservative units.  The Liberal Crime");
   move(6,11);
   addstr("Squad will be located far from the entrance to the safehouse.");
   move(7,11);
   addstr("It is your task to bring your squad out to safety, or fight");
   move(8,11);
   addstr("off the Conservatives within the perimeter.  Either way you");
   move(9,11);
   addstr("choose, any equipment from the LCC which isn't held by a");
   move(10,11);
   addstr("Liberal will be scattered about the compound.  Save what");
   move(11,11);
   addstr("you can.  You might notice your Squad has filled out to");
   move(12,11);
   addstr("six members if any were available.  If you have a larger pool");
   move(13,11);
   addstr("of Liberals, they will be traveling behind the Squad.");
   move(14,11);
   addstr("There is a new button, (R)eorganize, which reflects this.");
   move(15,11);
   addstr("Squad members in the back with firearms can provide cover");
   move(16,11);
   addstr("fire.  If you have at least six people total, then six must");
   move(17,11);
   addstr("be in the Squad.  If less than six, then they all must.");
   
   int loc=-1;
   if(selectedsiege!=-1)loc=selectedsiege;
   if(activesquad!=NULL)loc=activesquad->squad[0]->location;
   if(loc==-1)return;

   if(location[loc]->compound_walls & COMPOUND_CAMERAS)
   {
      move(18,16);
      addstr("Your security cameras let you see units on the (M)ap.");
   }
   if(location[loc]->compound_walls & COMPOUND_TRAPS)
   {
      move(19,16);
      addstr("Your traps will harass the enemy, but not the Squad.");
   }

   set_color(COLOR_RED,COLOR_BLACK,1);
   move(23,11);
   addstr("Press any key to Confront the Conservative Aggressors");

   refresh();
   getch();

   if(location[loc]->siege.siegetype==SIEGE_CCS)
   {
      if(location[loc]->type==SITE_INDUSTRY_WAREHOUSE)
         location[loc]->renting=RENTING_CCS; // CCS Captures warehouse -- this will be reversed if you fight them off
   }

   //CRIMINALIZE
   if(location[loc]->siege.siegetype==SIEGE_POLICE)criminalizepool(LAWFLAG_RESIST,-1,loc);

   //DELETE ALL SQUADS IN THIS AREA UNLESS THEY ARE THE activesquad
   for(int sq=squad.size()-1;sq>=0;sq--)
   {
      if(squad[sq]==activesquad)continue;
      if(squad[sq]->squad[0]!=NULL)
      {
         if(squad[sq]->squad[0]->location==loc)
         {
            if(activesquad)
            {
               for(int p=0;p<6;p++)
               {
                  if(squad[sq]->squad[p]==NULL)continue;
                  squad[sq]->squad[p]->squadid=-1;
               }
               delete squad[sq];
               squad.erase(squad.begin() + sq);
            }
            else
            {
               activesquad = squad[sq];
            }
         }
      }
   }

   // No squads at the location? Form a new one.
   if(activesquad==NULL)
   {
      squad.push_back(new squadst);
      if (location[selectedsiege]->front_business==-1)
	 strcpy(squad.back()->name,location[selectedsiege]->shortname);
      else
	 strcpy(squad.back()->name,location[selectedsiege]->front_shortname);

      strcat(squad.back()->name," Defense");
      int i=0;
      for(int p=0;p<pool.size();p++)
      {
         if(pool[p]->location == selectedsiege)
         {
            squad.back()->squad[i]=pool[p];
            pool[p]->squadid=squad.back()->id;
            i++;
            if(i>=6)break;
         }
      }
      activesquad = squad.back();
   }

   //MAKE SURE PARTY IS ORGANIZED
   autopromote(loc);

   //START FIGHTING
   newsstoryst *ns=new newsstoryst;
      if(location[loc]->siege.underattack)ns->type=NEWSSTORY_SQUAD_FLEDATTACK;
      else ns->type=NEWSSTORY_SQUAD_ESCAPED;
      ns->positive=1;
      ns->loc=loc;
      ns->siegetype=location[loc]->siege.siegetype;
   newsstory.push_back(ns);
   mode_site(loc);
}



/* siege - what happens when you escaped the siege */
void escapesiege(char won)
{
   //TEXT IF DIDN'T WIN
   if(!won)
   {
      //GIVE INFO SCREEN
      erase();
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      move(1,32);
      addstr("You have escaped!");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(3,16);
      addstr("The Conservatives thought that the Liberal Crime Squad was");
      move(4,11);
      addstr("finished, but once again, Conservative Thinking has proven");
      move(5,11);
      addstr("itself to be based on Unsound Notions.");
      move(6,16);
      addstr("However, all is not well.  In your haste to escape you have");
      move(7,11);
      addstr("lost everything that you've left behind.  You'll have");
      move(8,11);
      addstr("to start from scratch in a new safe house.  Your");
      move(9,11);
      addstr("funds remain under your control, fortunately.  Your flight has");
      move(10,11);
      addstr("given you some time to regroup, but the Conservatives will");
      move(11,11);
      addstr("doubtless be preparing another assault.");

      int homes=-1;
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
         {
            homes=l;
         }
      }

      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      move(13,11);
      addstr("Press any key to split up and lay low for a few days");

      refresh();
      getch();

      //GET RID OF DEAD, etc.
      if(location[cursite]->renting>1)location[cursite]->renting=RENTING_NOCONTROL;

      for(int p=pool.size()-1;p>=0;p--)
      {
         if(pool[p]->location!=cursite)continue;
         if(!pool[p]->alive)
         {
            delete pool[p];
            if(pool[p]->align==1)
            {
               int boss=getpoolcreature(pool[p]->hireid);
               if(boss!=-1&&pool[boss]->juice>50)
               {
                  int juice=pool[boss]->juice-50;
                  if(juice>10)juice=10;
                  addjuice(*pool[boss],-juice);
               }
            }
            pool.erase(pool.begin() + p);
            continue;
         }

         //BASE EVERYONE LEFT AT HOMELESS SHELTER
         removesquadinfo(*pool[p]);
         pool[p]->hiding=LCSrandom(3)+2;
         pool[p]->location=-1;
         pool[p]->base=homes;
      }
      for(int l2=0;l2<location[cursite]->loot.size();l2++)
      {
         delete location[cursite]->loot[l2];
      }
      location[cursite]->loot.clear();

      for(int v=(int)vehicle.size()-1;v>=0;v--)
      {
         if(vehicle[v]->location==cursite)
         {
            removecarprefs_pool(vehicle[v]->id);
            delete vehicle[v];
            vehicle.erase(vehicle.begin() + v);
         }
      }

      location[cursite]->compound_walls=0;
      location[cursite]->compound_stores=0;
      location[cursite]->front_business=-1;
      initlocation(*location[cursite]);
   }

   //SET UP NEW SIEGE CHARACTERISTICS, INCLUDING TIMING
   location[cursite]->siege.siege=0;
   if(won)
   {
      if(location[cursite]->siege.siegetype==SIEGE_POLICE)
      {
         location[cursite]->siege.timeuntillocated=LCSrandom(4)+4;
         location[cursite]->siege.escalationstate++;
         if(police_heat<4)police_heat++;
      }
   }
}



/* siege - flavor text when you fought off the raid */
void conquertext(void)
{
   //GIVE INFO SCREEN
   erase();
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   move(1,26);
   addstr("* * * * *   VICTORY   * * * * *");

   if(location[cursite]->siege.siegetype==SIEGE_POLICE)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(3,16);
      addstr("The Conservative automatons have been driven back -- for");
      move(4,11);
      addstr("the time being.  While they are regrouping, you might consider");
      move(5,11);
      addstr("abandoning this safe house for a safer location.");
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(3,16);
      addstr("The Conservative automatons have been driven back.");
      move(4,11);
      addstr("Unfortunately, you will never truly be safe from");
      move(5,11);
      addstr("this filth until the Liberal Agenda is realized.");
   }

   move(7,19);
   addstr("Press C to Continue Liberally.");

   do
   {
      refresh();
      int c=getch();
      translategetch(c);

      if(c=='c')break;
   }while(1);
}



/* siege - flavor text when you crush a CCS safe house */
void conquertextccs(void)
{
   //GIVE INFO SCREEN
   erase();
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   move(1,26);
   addstr("* * * * *   VICTORY   * * * * *");

   if(ccs_kills<3)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(3,16);
      addstr("Gunfire still ringing in their ears, the squad revels in");
      move(4,11);
      addstr("their victory.  The CCS is bound to have another safe house, but");
      move(5,11);
      addstr("for now, their power has been severely weakened.  This will make");
      move(6,11);
      addstr("a fine base for future Liberal operations.");
   }
   else
   {
      move(3,16);
      addstr("Gunfire still ringing in their ears, the squad revels in");
      move(4,11);
      addstr("their final victory.");
      move(6,16);
      addstr("As your Liberals pick through the remains of the safehouse,");
      move(7,11);
      addstr("it is increasingly clear that this was their final safe house.");
      move(9,16);
      addstr("The CCS has been completely destroyed.  Now wasn't there a");
      move(10,16);
      addstr("revolution to attend to?");

      move(12,5);
      addstr("+100 JUICE TO EVERYONE FOR ERADICATING THE CONSERVATIVE CRIME SQUAD");

      for(int p=0;p<pool.size();p++)
      {
         addjuice(*pool[p],100);
      }
   }

   move(15,19);
   addstr("Press C to Continue Liberally.");

   do
   {
      refresh();
      int c=getch();
      translategetch(c);

      if(c=='c')break;
   }while(1);
}



/* siege - "you are wanted for _______ and other crimes..." */
void statebrokenlaws(int loc)
{
   short breakercount[LAWFLAGNUM] = {0};
   int typenum=0;
   int criminalcount=0;
   char kname[100];
   int kidnapped=0;
   int confessed=0;

   for(int p=0;p<pool.size();p++)
   {
      if(!pool[p]->alive)continue;
      if(pool[p]->location!=loc)continue;

      if(pool[p]->flag & CREATUREFLAG_KIDNAPPED)
      {
         strcpy(kname,pool[p]->propername);
         kidnapped++;
      }

      if(iscriminal(*pool[p]))criminalcount++;

      for(int i=0;i<LAWFLAGNUM;i++)
      {
         if(pool[p]->crimes_suspected[i])
            breakercount[i]++;
      }
      if(pool[p]->confessions)confessed=1;
   }
   for(int i=0;i<LAWFLAGNUM;i++)
   {
      if(breakercount[i])typenum++;
   }

   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(1,1);
   if(location[loc]->siege.underattack)addstr("You hear shouts:");
   else addstr("You hear a blaring voice on a loudspeaker:");

   move(3,1);
   if(location[loc]->siege.escalationstate>=2 && publicmood(-1) < 20)
      addstr("In the name of God, your campaign of terror ends here!");
   else
      addstr("Surrender yourselves!");

   //KIDNAP VICTIM
   if(kidnapped)
   {
      move(4,1);
      addstr("Release ");
      addstr(kname);
      if(kidnapped>1)addstr(" and the others");
      addstr(" unharmed!");
   }
   //TREASON
   else if(breakercount[LAWFLAG_TREASON])
   {
      move(4,1);
      addstr("You are wanted for treason");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //TERRORISM
   else if(breakercount[LAWFLAG_TERRORISM])
   {
      move(4,1);
      addstr("You are wanted for terrorism");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //MURDERER
   else if(breakercount[LAWFLAG_MURDER])
   {
      move(4,1);
      addstr("You are wanted for murder");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //KIDNAPPER
   else if(breakercount[LAWFLAG_KIDNAPPING])
   {
      move(4,1);
      addstr("You are wanted for kidnapping");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //BURN FLAG
   else if(breakercount[LAWFLAG_BURNFLAG])
   {
      move(4,1);
      if(law[LAW_FLAGBURNING]==-2)addstr("You are wanted for flag murder");
      else addstr("You are wanted for flag burning");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //SPEECH
   else if(breakercount[LAWFLAG_SPEECH])
   {
      move(4,1);
      addstr("You are wanted for harmful speech");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //BROWNIES
   else if(breakercount[LAWFLAG_BROWNIES])
   {
      move(4,1);
      addstr("You are wanted for drug dealing");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //ESCAPED
   else if(breakercount[LAWFLAG_ESCAPED])
   {
      move(4,1);
      addstr("You are wanted for escaping prison");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //HELP ESCAPED
   else if(breakercount[LAWFLAG_HELPESCAPE])
   {
      move(4,1);
      addstr("You are wanted for aiding a prison escape");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //JURY
   else if(breakercount[LAWFLAG_JURY])
   {
      move(4,1);
      addstr("You are wanted for jury tampering");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //RACKETEERING
   else if(breakercount[LAWFLAG_RACKETEERING])
   {
      move(4,1);
      addstr("You are wanted for racketeering");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //ASSAULT
   else if(breakercount[LAWFLAG_ARMEDASSAULT])
   {
      move(4,1);
      addstr("You are wanted for armed assault");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //ASSAULT
   else if(breakercount[LAWFLAG_ASSAULT])
   {
      move(4,1);
      addstr("You are wanted for assault");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //CAR THEFT
   else if(breakercount[LAWFLAG_CARTHEFT])
   {
      move(4,1);
      addstr("You are wanted for motor theft");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //CC FRAUD
   else if(breakercount[LAWFLAG_CCFRAUD])
   {
      move(4,1);
      addstr("You are wanted for credit card fraud");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //THIEF
   else if(breakercount[LAWFLAG_THEFT])
   {
      move(4,1);
      addstr("You are wanted for theft");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //PROSTITUTION
   else if(breakercount[LAWFLAG_PROSTITUTION])
   {
      move(4,1);
      addstr("You are wanted for prostitution");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //HIRE ILLEGAL
   else if(breakercount[LAWFLAG_HIREILLEGAL])
   {
      move(4,1);
      addstr("You are wanted for hiring an illegal alien");//XXX: Maybe `undocumented worker` if LAW_IMMIGRATION is 0, or maybe 1...
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //GUN CARRY
   else if(breakercount[LAWFLAG_GUNUSE])
   {
      move(4,1);
      addstr("You are wanted for use of an illegal weapon");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //GUN CARRY
   else if(breakercount[LAWFLAG_GUNCARRY])
   {
      move(4,1);
      addstr("You are wanted for carrying an illegal weapon");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //COMMERCE
   else if(breakercount[LAWFLAG_COMMERCE])
   {
      move(4,1);
      addstr("You are wanted for interfering with interstate commerce");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //INFORMATION
   else if(breakercount[LAWFLAG_INFORMATION])
   {
      move(4,1);
      addstr("You are wanted for unlawfully accessing an information system");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //UNLAWFUL BURIAL
   else if(breakercount[LAWFLAG_BURIAL])
   {
      move(4,1);
      addstr("You are wanted for unlawful burial");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //BREAKING
   else if(breakercount[LAWFLAG_BREAKING])
   {
      move(4,1);
      addstr("You are wanted for breaking and entering");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //VANDALISM
   else if(breakercount[LAWFLAG_VANDALISM])
   {
      move(4,1);
      addstr("You are wanted for vandalism");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //RESIST
   else if(breakercount[LAWFLAG_RESIST])
   {
      move(4,1);
      addstr("You are wanted for resisting arrest");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }
   //DISTURBANCE
   else if(breakercount[LAWFLAG_DISTURBANCE])
   {
      move(4,1);
      addstr("You are wanted for disturbing the peace");
      if(typenum>1)addstr(" and other crimes");
      addstr(".");
   }

   refresh();
   getch();
}


void statebrokenlaws(Creature & cr)
{
   int kidnapped=0;
   bool criminal=false;
   bool breakercount[LAWFLAGNUM];

   if(cr.flag & CREATUREFLAG_KIDNAPPED)
      kidnapped++;

   for(int i=0;i<LAWFLAGNUM;i++)
      if(cr.crimes_suspected[i])
      {
         breakercount[i]=true;
         criminal=true;
      }
      else
      {
         breakercount[i]=false;
      }

   if(!(criminal||kidnapped))
      return;

   set_color(COLOR_YELLOW,COLOR_BLACK,1);
   addstr("WANTED FOR ");

   //KIDNAP VICTIM
   if(kidnapped)
   {
      addstr("REHABILITATION");
   }
   //TREASON
   else if(breakercount[LAWFLAG_TREASON])
   {
      addstr("TREASON");
   }
   //TERRORISM
   else if(breakercount[LAWFLAG_TERRORISM])
   {
      addstr("TERRORISM");
   }
   //MURDERER
   else if(breakercount[LAWFLAG_MURDER])
   {
      addstr("MURDER");
   }
   //KIDNAPPER
   else if(breakercount[LAWFLAG_KIDNAPPING])
   {
      addstr("KIDNAPPING");
   }
   //BURN FLAG
   else if(breakercount[LAWFLAG_BURNFLAG])
   {
      if(law[LAW_FLAGBURNING]==-2)addstr("FLAG MURDER");
      else addstr("FLAG BURNING");
   }
   //SPEECH
   else if(breakercount[LAWFLAG_SPEECH])
   {
      addstr("HARMFUL SPEECH");
   }
   //BROWNIES
   else if(breakercount[LAWFLAG_BROWNIES])
   {
      addstr("DRUG DEALING");
   }
   //ESCAPED
   else if(breakercount[LAWFLAG_ESCAPED])
   {
      addstr("ESCAPING PRISON");
   }
   //HELP ESCAPED
   else if(breakercount[LAWFLAG_HELPESCAPE])
   {
      addstr("RELEASING PRISONERS");
   }
   //JURY
   else if(breakercount[LAWFLAG_JURY])
   {
      addstr("JURY TAMPERING");
   }
   //RACKETEERING
   else if(breakercount[LAWFLAG_RACKETEERING])
   {
      addstr("RACKETEERING");
   }
   //ASSAULT
   else if(breakercount[LAWFLAG_ARMEDASSAULT])
   {
      addstr("ARMED ASSAULT");
   }
   //ASSAULT
   else if(breakercount[LAWFLAG_ASSAULT])
   {
      addstr("ASSAULT");
   }
   //CAR THEFT
   else if(breakercount[LAWFLAG_CARTHEFT])
   {
      addstr("GRAND THEFT AUTO");
   }
   //CC FRAUD
   else if(breakercount[LAWFLAG_CCFRAUD])
   {
      addstr("CREDIT CARD FRAUD");
   }
   //THIEF
   else if(breakercount[LAWFLAG_THEFT])
   {
      addstr("THEFT");
   }
   //PROSTITUTION
   else if(breakercount[LAWFLAG_PROSTITUTION])
   {
      addstr("PROSTITUTION");
   }
   //HIRE ILLEGAL
   else if(breakercount[LAWFLAG_HIREILLEGAL])
   {
      addstr("HIRING ILLEGAL ALIENS");
   }
   //GUN CARRY
   else if(breakercount[LAWFLAG_GUNUSE])
   {
      addstr("FIRING ILLEGAL WEAPONS");
   }
   //GUN CARRY
   else if(breakercount[LAWFLAG_GUNCARRY])
   {
      addstr("USING ILLEGAL WEAPONS");
   }
   //COMMERCE
   else if(breakercount[LAWFLAG_COMMERCE])
   {
      addstr("ELECTRONIC SABOTAGE");
   }
   //INFORMATION
   else if(breakercount[LAWFLAG_INFORMATION])
   {
      addstr("HACKING");
   }
   //UNLAWFUL BURIAL
   else if(breakercount[LAWFLAG_BURIAL])
   {
      addstr("UNLAWFUL BURIAL");
   }
   //BREAKING
   else if(breakercount[LAWFLAG_BREAKING])
   {
      addstr("BREAKING AND ENTERING");
   }
   //VANDALISM
   else if(breakercount[LAWFLAG_VANDALISM])
   {
      addstr("VANDALISM");
   }
   //RESIST
   else if(breakercount[LAWFLAG_RESIST])
   {
      addstr("RESISTING ARREST");
   }
   //DISTURBANCE
   else if(breakercount[LAWFLAG_DISTURBANCE])
   {
      addstr("DISTURBING THE PEACE");
   }
}

