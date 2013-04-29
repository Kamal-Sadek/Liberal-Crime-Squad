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

There are a few bugs regarding sieges, including:
        - After coming back after disbanding, ALL sleepers end up in the homeless shelter, and will be killed/arrested during raids, however, they still
           can only be controlled as sleepers, meaning, no moving them out of the homeless shelter.

make it more likely to be raided:
        - when a liberal hacks or steals credit cards.
        - dead bodies in the base. [DONE]
*/


/* siege - gives up on sieges with empty locations */
/* Work in progress. It works, but needs to be called in more places. */
/* Currently, it only works when you confront a siege and then fail. */
void resolvesafehouses(void)
{
   for(int l=0;l<location.size();l++)
   {
      //Location* d_loc = location[l];
      //int d_renting = d_loc->renting;
      if(location[l]->renting>=0&&location[l]->siege.siege)
      {
         cleangonesquads();
         selectedsiege = l; // hack for calling giveup()
         giveup();

         int bp=0;
      }
   }
}


/* siege - updates upcoming sieges */
void siegecheck(char canseethings)
{
   if(disbanding)return;

// Upkeep - even base-less should be considered.
// XXX - candidate to create nice function?
  // Cleanse record on things that aren't illegal right now
  for(int p=0;p<pool.size();p++)
  {
    if(law[LAW_FLAGBURNING]>0)pool[p]->crimes_suspected[LAWFLAG_BURNFLAG]=0;
    if(law[LAW_DRUGS]>0)pool[p]->crimes_suspected[LAWFLAG_BROWNIES]=0;
    if(law[LAW_IMMIGRATION]==2)pool[p]->flag &= ~CREATUREFLAG_ILLEGALALIEN;
    if(law[LAW_FREESPEECH]>-2)pool[p]->crimes_suspected[LAWFLAG_SPEECH]=0;
  }
  if(law[LAW_FREESPEECH]>-2)offended_firemen=0;

   //FIRST, THE COPS
   int numpres;
   for(int l=0;l<location.size();l++)
   {
      if(policestation_closed)
      {
         location[l]->heat = static_cast<int>(location[l]->heat * 0.95);
         continue;
      }
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
            }
         }

         //CHECK FOR CRIMINALS AT THIS BASE
         int crimes=0;
         int kidnapped=0;
         int heatprotection=0;
         for(int p=0;p<pool.size();p++)
         {
            // Sleepers and people not at this base don't count
            if(pool[p]->location!=l || pool[p]->flag & CREATUREFLAG_SLEEPER)continue;
            
            if(!pool[p]->alive) // Corpses attract attention
            {
               crimes += 10;
               continue;
            }
            if(pool[p]->flag & CREATUREFLAG_KIDNAPPED &&
               pool[p]->align!=1)
            {
               crimes += 5*pool[p]->joindays; // Kidnapped persons increase heat
               continue;
            }
            if(pool[p]->align!=1)continue; // Non-liberals don't count other than that
            numpres++;
            
            //Heat doesn't matter for sieges until it gets high
            crimes += max(0,(pool[p]->heat)/10-10);
            //Heat decay
            if(pool[p]->heat > 100) pool[p]->heat-=pool[p]->heat/100;
         }

         // Let the place cool off if nobody is present
         if(!crimes)
         {
            location[l]->heat-=5;

            if(location[l]->heat<0)
               location[l]->heat = 0;
         }
         else
         {
            // Determine how effective your current safehouse
            // is at keeping the police confused
            location[l]->update_heat_protection();

            // Having hostages increases heat
            if(kidnapped)
            {
               crimes+=kidnapped*20;
            }

            // Update location heat
            int max_heat = static_cast<int>(crimes * (100 - location[l]->heat_protection) / 100.0);
            location[l]->heat += (max_heat - location[l]->heat)/10;

            // Begin planning siege if high heat on location
            if(LCSrandom(3000) < location[l]->heat
               && !(location[l]->siege.timeuntillocated>=0)) //Do not re-plan siege.
            {
               // Set force deployment (military, bombers, etc.)
               if(LCSrandom(location[l]->heat) > 50)location[l]->siege.escalationstate++;
               if(LCSrandom(location[l]->heat) > 150)location[l]->siege.escalationstate++;
               if(LCSrandom(location[l]->heat) > 300)location[l]->siege.escalationstate++;

               if(location[l]->siege.escalationstate>3)
                  location[l]->siege.escalationstate=3;

               // Set time until siege is carried out
               location[l]->siege.timeuntillocated += 14 + LCSrandom(15);
            }
         }

         // *JDS* Sleepers at the police department may give a warning just before police raids
         if(location[l]->siege.timeuntillocated==1)
         {
            int policesleeperwarning=0;
            for(int pl=0;pl<pool.size();pl++)
            {
               if(pool[pl]->flag & CREATUREFLAG_SLEEPER &&
                  pool[pl]->location!=-1 &&
                  location[pool[pl]->location]->type==SITE_GOVERNMENT_POLICESTATION)
               {
                  //if(pool[pl]->infiltration*100>LCSrandom(50))
                  {
                     policesleeperwarning=1;
                     break;
                  }
               }
            }
            if(policesleeperwarning)
            {
               erase();
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr("You have received advance warning from your sleepers regarding ", gamelog);
               move(9,1);
               addstr("an imminent police raid on ", gamelog);
               addstr(location[l]->getname());
               addstr(".", gamelog);
               gamelog.newline();
               
               if(location[l]->siege.escalationstate>=2)
               {
                  move(11,1);
                  addstr("The fighting force will be composed of national guard troops.", gamelog);
               }
               if(location[l]->siege.escalationstate>=2)
               {
                  move(12,1);
                  addstr("A tank will cover the entrance to the compound.", gamelog);
               }
               if(location[l]->siege.escalationstate>=3)
               {
                  move(13,1);
                  addstr("Planes will bomb the compound during the siege, and the final ", gamelog);
                  move(14,1);
                  addstr("attack will be carried out by SEAL Team 6.", gamelog);
               }
               gamelog.nextMessage(); //Write out buffer to prepare for next message.

               move (15,1);
               addstr("Press Esc to ponder the situation...");
               refresh();
               int c;
               do {
                  c = getch();
                  translategetch(c);
               } while(c!='x'&&c!=ESC);
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

               move(8,1);
               addstr("The police have surrounded the ", gamelog);
               addstr(location[l]->getname(), gamelog);
               addstr("!", gamelog);
               gamelog.newline();
               location[l]->siege.underattack=0;
               refresh();
               getch();

               //MENTION ESCALATION STATE
               if(location[l]->siege.escalationstate>=1)
               {
                  move(9,1);
                  addstr("National Guard troops are replacing normal SWAT units.", gamelog);
                  gamelog.nextMessage();
                  refresh();
                  getch();
               }
               if(location[l]->siege.escalationstate>=2)
               {
                  move(10,1);
                  if(location[l]->compound_walls & COMPOUND_TANKTRAPS)
                     addstr("An M1 Abrams Tank is stopped by the tank traps.", gamelog);
                  else
                     addstr("An M1 Abrams Tank takes up position outside the compound.", gamelog);
                  gamelog.nextMessage();
                  refresh();
                  getch();
               }
               if(location[l]->siege.escalationstate>=3)
               {
                  move(11,1);
                  addstr("You hear jet bombers streak overhead.", gamelog);
                  gamelog.nextMessage();
                  refresh();
                  getch();
               }

               // "You are wanted for blahblah and other crimes."
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
               addstr("The cops have raided the ", gamelog);
               addstr(location[l]->getname(), gamelog);
               addstr(", an unoccupied safehouse.", gamelog);
               gamelog.newline();

               refresh();
               getch();

               int y=9;

               for(int p=pool.size()-1;p>=0;p--)
               {
                  if(pool[p]->location!=l)continue;
                  if(!pool[p]->alive)
                  {
                     move(y,1);y++;
                     addstr(pool[p]->name, gamelog);
                     addstr("'s corpse has been recovered.", gamelog);
                     gamelog.newline();
                     refresh();
                     getch();

                     delete pool[p];
                     pool.erase(pool.begin() + p);
                     continue;
                  }
                  if(pool[p]->align!=1)
                  {
                     move(y,1);y++;
                     addstr(pool[p]->name, gamelog);
                     addstr(" has been rescued.", gamelog);
                     gamelog.newline();
                     refresh();
                     getch();

                     delete pool[p];
                     pool.erase(pool.begin() + p);
                     continue;
                  }
               }

               gamelog.newline();
                  
               for(int l2=0;l2<location[l]->loot.size();l2++)
               {
                  delete location[l]->loot[l2];
               }
               location[l]->loot.clear();

               for(int v=(int)vehicle.size()-1;v>=0;v--)
               {
                  if(vehicle[v]->get_location()==l)
                  {
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
                  ceosleepercount=1;
               }
            }
            if(ceosleepercount||!LCSrandom(5))
            {
               erase();
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr("You have received ", gamelog);
               if(ceosleepercount)addstr("your sleeper CEO's warning", gamelog);
               else addstr("an anonymous tip", gamelog);
               addstr(" that the Corporations ", gamelog);
               move(9,1);
               addstr("are hiring mercenaries to attack ", gamelog);
               if(ceosleepercount)addstr(location[l]->getname(), gamelog);
               else addstr("the LCS", gamelog);
               addstr(".", gamelog);
               gamelog.nextMessage();

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
            addstr("Corporate mercenaries are raiding the ", gamelog);
            addstr(location[l]->getname(), gamelog);
            addstr("!", gamelog);
            gamelog.nextMessage();

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
         bool ccs_active = endgamestate>=ENDGAME_CCS_APPEARANCE && endgamestate<ENDGAME_CCS_DEFEATED;
         bool target_interesting = endgamestate>=ENDGAME_CCS_SIEGES || location[l]->compound_walls & COMPOUND_PRINTINGPRESS;
         if(ccs_active && target_interesting)
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
                     ccssleepercount=1;
                     break;
                  }
               }
               if(ccssleepercount>0)
               {
                  erase();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr("You have received a sleeper warning that the CCS ", gamelog);
                  move(9,1);
                  addstr(" is gearing up to attack ", gamelog);
                  addstr(location[l]->name, gamelog);
                  addstr(".", gamelog);
                  gamelog.nextMessage();

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
               addstr("A screeching truck pulls up to ", gamelog);
               addstr(location[l]->getname(), gamelog);
               addstr("!", gamelog);
               gamelog.newline();

               refresh();
               getch();

               if(!(location[l]->compound_walls & COMPOUND_TANKTRAPS) &&
                  !LCSrandom(5))
               {
                  // CCS Carbombs safehouse!!
                  erase();
                  set_color(COLOR_RED,COLOR_BLACK,1);

                  move(8,1);
                  addstr("The truck plows into the building and explodes!", gamelog);
                  gamelog.nextMessage();

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

                  move(6,1);
                  addstr("INJURED: ");
                  int injured_y = 6;
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
                              if(killed_x+namelength>78)
                              {
                                 killed_y++;
                                 killed_x=1;
                                 //Add limit for killed_y.
                              }
                              move(killed_y,killed_x);
                              pool[i]->die();
                              set_alignment_color(pool[i]->align,false);
                              addstr(pool[i]->name);
                              addstr(", ");
                              killed_x+=namelength+2;
                           }
                           else
                           {
                              if(injured_x+namelength>78)
                              {
                                 injured_y++;
                                 injured_x=1;
                                 //Add limit for injured_y.
                              }
                              move(injured_y,injured_x);
                              set_alignment_color(pool[i]->align,false);
                              addstr(pool[i]->name);
                              addstr(", ");
                              injured_x+=namelength+2;
                           }
                           //set_alignment_color(pool[i]->align,false);
                           //addstr(pool[i]->name);
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
                  set_color(COLOR_RED,COLOR_BLACK,1);

                  move(8,1);
                  addstr("CCS members pour out of the truck and shoot in the front doors!", gamelog);
                  gamelog.nextMessage();

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
         if(location[l]->heat&&location[l]->siege.timeuntilcia==-1&&!location[l]->siege.siege&&offended_cia&&!LCSrandom(300)&&numpres>0)
         {
            location[l]->siege.timeuntilcia=LCSrandom(3)+1;
            // *JDS* agent sleepers may give a warning before cia raids
            int agentsleepercount=0;
            for(int pl=0;pl<pool.size();pl++)
            {
               if(pool[pl]->flag & CREATUREFLAG_SLEEPER&&
                  pool[pl]->type==CREATURE_AGENT)
               {
                  //if(pool[pl]->infiltration*100>LCSrandom(100))
                  {
                     agentsleepercount=1;
                     break;
                  }
               }
            }
            if(agentsleepercount)
            {
               erase();
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr("A sleeper agent has reported that the CIA is planning ", gamelog);
               move(9,1);
               addstr("to launch an attack on ", gamelog);
               addstr(location[l]->getname(), gamelog);
               addstr(".", gamelog);
               gamelog.nextMessage();

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
            addstr("Unmarked black vans are surrounding the ", gamelog);
            addstr(location[l]->getname(), gamelog);
            addstr("!", gamelog);
            gamelog.newline();

            if(location[l]->compound_walls & COMPOUND_CAMERAS)
            {
               move(9,1);
               addstr("Through some form of high technology, they've managed ", gamelog);
               move(10,1);
               addstr("to shut off the lights and the cameras!", gamelog);
               gamelog.nextMessage();
            }
            else if(location[l]->compound_walls & COMPOUND_GENERATOR)
            {
               move(9,1);
               addstr("Through some form of high technology, they've managed ", gamelog);
               move(10,1);
               addstr("to shut off the lights!", gamelog);
               gamelog.nextMessage();
            }
            else
            {
               move(9,1);
               addstr("They've shut off the lights!", gamelog);
               gamelog.nextMessage();
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
            addstr("Masses dissatisfied with your lack of respect for AM Radio ", gamelog);
            move(9,1);
            addstr("are storming the ", gamelog);
            addstr(location[l]->getname(), gamelog);
            addstr("!", gamelog);
            gamelog.nextMessage();

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
            addstr("Masses dissatisfied with your lack of respect for Cable News ", gamelog);
            move(9,1);
            addstr("are storming the ", gamelog);
            addstr(location[l]->getname(), gamelog);
            addstr("!", gamelog);
            gamelog.nextMessage();

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
                  addstr("A sleeper Fireman has informed you that ", gamelog);
               } else
               {
                  addstr("Word in the underground is that ", gamelog);
               }
               move(9,1);
               addstr("the Firemen are planning to burn ", gamelog);
               addstr(location[l]->name, gamelog);
               addstr(".", gamelog);
               gamelog.nextMessage();

               refresh();
               getch();
            }

         } else if(location[l]->siege.timeuntilfiremen>0)location[l]->siege.timeuntilfiremen--;
         else if(law[LAW_FREESPEECH]==-2 && location[l]->siege.timeuntilfiremen==0 && !location[l]->siege.siege&&numpres>0)
         {
            location[l]->siege.timeuntilfiremen=-1;
            // Firemen raid!
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,1);

            move(8,1);
            addstr("Screaming fire engines pull up to the ", gamelog);
            addstr(location[l]->getname(), gamelog);
            addstr("!", gamelog);
            gamelog.newline();

            move(9,1);
            addstr("Armored firemen swarm out, pilot lights burning.", gamelog);
            gamelog.newline();
            
            refresh();
            getch();

            erase();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(1,1);
            addstr("You hear a screeching voice over the sound of fire engine sirens:", gamelog);
            gamelog.newline();

            move(3,1);
            addstr("Surrender yourselves!", gamelog);
            gamelog.newline();

            move(4,1);
            addstr("Unacceptable Speech has occurred at this location.", gamelog);
            gamelog.newline();

            move(6,1);
            addstr("Come quietly and you will not be harmed.", gamelog);
            gamelog.nextMessage();

            refresh();
            getch();

            location[l]->siege.siege=1;
            location[l]->siege.siegetype=SIEGE_FIREMEN;
            location[l]->siege.underattack=1;
            location[l]->siege.lights_off=0;
            location[l]->siege.cameras_off=0;
            offended_firemen=0;
         } 
         else if(law[LAW_FREESPEECH]==-2 && location[l]->siege.timeuntilfiremen==0)
         {
            location[l]->siege.timeuntilfiremen=-1;
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,1);

            move(8,1);
            addstr("The Firemen have raided the ", gamelog);
            addstr(location[l]->getname(), gamelog);
            addstr(", an unoccupied safehouse.", gamelog);
            gamelog.newline();

            refresh();
            getch();

            int y=9;

            for(int p=pool.size()-1;p>=0;p--)
            {
               if(pool[p]->location!=l)continue;
               if(!pool[p]->alive)
               {
                  move(y,1);y++;
                  addstr(pool[p]->name, gamelog);
                  addstr("'s corpse has been recovered.", gamelog);
                  gamelog.newline();
                  refresh();
                  getch();

                  delete pool[p];
                  pool.erase(pool.begin() + p);
                  continue;
               }
               if(pool[p]->align!=1)
               {
                  move(y,1);y++;
                  addstr(pool[p]->name, gamelog);
                  addstr(" has been rescued.", gamelog);
                  gamelog.newline();
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
               addstr("The printing press is dismantled and burned.", gamelog);
               gamelog.newline();
               location[l]->compound_walls &= ~COMPOUND_PRINTINGPRESS;
               offended_firemen=0;
            }

            if(location[l]->front_business!=-1)
            {
               move(12,1);
               addstr("Materials relating to the business front have been destroyed.", gamelog);
               gamelog.newline();
               location[l]->front_business=-1;
            }
            gamelog.newline();
         }
         else if (law[LAW_FREESPEECH]<=-1 && location[l]->siege.timeuntilfiremen==0)
         {
            location[l]->siege.timeuntilfiremen=-1;
            offended_firemen=0;
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

   // Clear food_prep and liberalcount lists
   std::memset(food_prep,0,location.size());
   std::memset(liberalcount,0,sizeof(int)*location.size());

   for(int p=0;p<pool.size();p++)
   {
      if(!pool[p]->alive)continue; // Dead people don't count
      if(pool[p]->align!=1)continue; // Non-liberals don't count
      if(pool[p]->location==-1)continue; // Vacationers don't count
      liberalcount[pool[p]->location]++;
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
      // FOOD -- currently free
      /*if(!location[l]->siege.siege && location[l]->renting>=0 && location[l]->type!=SITE_RESIDENTIAL_SHELTER)
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
               addstr(location[l]->getname());
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
      else*/
      if(location[l]->siege.siege)
      {
         //resolve sieges with no people
         if(liberalcount[l]==0)
         {
            erase();
            set_color(COLOR_WHITE,COLOR_BLACK,1);

            move(8,1);
            addstr("Conservatives have raided the ", gamelog);
            addstr(location[l]->getname(), gamelog);
            addstr(", an unoccupied safehouse.", gamelog);
            gamelog.newline();

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
                  addstr("'s corpse has been recovered.", gamelog);
                  gamelog.newline();
                  refresh();
                  getch();

                  delete pool[p];
                  pool.erase(pool.begin() + p);
                  continue;
               }
               if(pool[p]->align!=1)
               {
                  move(y,1);y++;
                  addstr(pool[p]->name);
                  addstr(" has been rescued.", gamelog);
                  gamelog.newline();
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
               if(vehicle[v]->get_location()==l)
               {
                  delete vehicle[v];
                  vehicle.erase(vehicle.begin() + v);
               }
            }

            gamelog.newline();
            
            location[l]->siege.siege=0;
         }

         if(!location[l]->siege.underattack)
         {
            // Seperate logging message.
            gamelog.record("A day passes while under siege...");
            gamelog.newline();

            //EAT
            bool starving=false;
            int eaters=numbereating(l);
            if(location[l]->compound_stores==0&&eaters>0)
            {
               starving=true;
               if(clearformess)erase();
               else makedelimiter(8,0);
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr("Your Liberals are starving!", gamelog);
               gamelog.newline();

               refresh();
               getch();
            }
            if(location[l]->compound_stores>=eaters)location[l]->compound_stores-=eaters;
            else location[l]->compound_stores=0;

            //ATTACK!
            char attack=0;
            //int kidnapped=0; // unused
            //int criminalcount=0; // unused
            //int fortified=0; // unused

            for(int p=0;p<pool.size();p++)
            {
               if(!pool[p]->alive)continue;
               if(pool[p]->location!=l)continue;

               if(starving)pool[p]->blood-=LCSrandom(8)+4;

               // Check if liberal starved to death.
               if(pool[p]->blood<=0)
               {
                  pool[p]->die();

                  if(clearformess)erase();
                  else makedelimiter(8,0);
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(pool[p]->name , gamelog);
                  addstr(" has starved to death.", gamelog);
                  gamelog.newline();

                  refresh();
                  getch();
               }

               /*if(pool[p]->flag & CREATUREFLAG_KIDNAPPED)
               {
                  kidnapped++;
               }*/

               /*if(iscriminal(*pool[p]))criminalcount++;*/
            }

            /*if(location[l]->compound_walls & COMPOUND_BASIC)fortified=5;*/

            if(!LCSrandom(12))attack=1;

            if(attack)
            {
               if(clearformess)erase();
               else makedelimiter(8,0);
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr("The cops are coming!", gamelog);
               gamelog.newline();

               refresh();
               getch();

               location[l]->siege.underattack=1;
            }
            else
            {
               char no_bad=1;

               //CUT LIGHTS
               if(!location[l]->siege.lights_off &&
                  !(location[l]->compound_walls & COMPOUND_GENERATOR) && !LCSrandom(10))
               {
                  no_bad=0;

                  if(clearformess)erase();
                     else makedelimiter(8,0);
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr("The police have cut the lights!", gamelog);
                  gamelog.newline();

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
                        addstr("A sniper takes out ", gamelog);
                        addstr(pool[targ]->name, gamelog);
                        addstr("!", gamelog);
                        gamelog.newline();

                        if(pool[targ]->align==1)
                        {
                           stat_dead++;
                           liberalcount[l]--;
                        }

                        removesquadinfo(*pool[targ]);

                        pool[targ]->die();
                        //delete pool[targ];
                        //pool.erase(pool.begin() + targ);
                     }
                     else
                     {
                        addstr("A sniper nearly hits ", gamelog);
                        addstr(pool[targ]->name, gamelog);
                        addstr("!", gamelog);
                        gamelog.newline();
                     }
                     refresh();
                     getch();
                  }
               }
            
               if(location[l]->siege.escalationstate>=3 && !LCSrandom(3))
               {
                  no_bad=0;

                  //AIR STRIKE!
                  char hit=true;

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
                  addstr("You hear planes streak overhead!", gamelog);
                  gamelog.newline();
                  refresh();
                  getch();

                  bool hasAAGun = location[l]->compound_walls & COMPOUND_AAGUN;
                  bool hasGenerator = location[l]->compound_walls & COMPOUND_GENERATOR;

                  if(hasAAGun)
                  {
                     if(clearformess) erase();
                     else makedelimiter(8,0);
                     move(8,1);
                     addstr("The thunder of the anti-aircraft gun shakes the compound!", gamelog);
                     gamelog.newline();
                     refresh();
                     getch();
                     if(clearformess)erase();
                     else makedelimiter(8,0);
                     move(8,1);
                     if(LCSrandom(5))
                     {
                        hit=false;
                        if(LCSrandom(2))
                        {
                           addstr("You didn't shoot any down, but you've made them think twice!", gamelog);
                        }
                        else
                        {
                           addstr("Hit! One of the bombers slams into to the ground.", gamelog);
                           gamelog.newline();
                           refresh();
                           getch();
                           if(clearformess)erase();
                           else makedelimiter(8,0);
                           move(8,1);
                           addstr("It's all over the TV. Everyone in the Liberal Crime Squad gains 20 juice!", gamelog);
                           for(int p=0;p<pool.size();p++)
                           {
                              addjuice(*pool[p],20,1000);
                           }
                        }
                        gamelog.newline();
                        refresh();
                        getch();
                     }
                     else
                     {
                        addstr("A skilled pilot gets through!", gamelog);
                        gamelog.newline();
                        refresh();
                        getch();
                     }
                  }

                  if(hit)
                  {
                     if(clearformess) erase();
                     else makedelimiter(8,0);
                     move(8,1);
                     addstr("Explosions rock the compound!", gamelog);
                     gamelog.newline();
                     refresh();
                     getch();
                     if(hasAAGun && !LCSrandom(3))
                     {
                        if(clearformess)erase();
                        else makedelimiter(8,0);
                        move(8,1);
                        addstr("The anti-aircraft gun takes a direct hit!", gamelog);
                        gamelog.newline();
                        refresh();
                        getch();
                        if(clearformess)erase();
                        else makedelimiter(8,0);
                        move(8,1);
                        addstr("There's nothing left but smoking wreckage...", gamelog);
                        gamelog.newline();
                        refresh();
                        getch();
                        
                        location[l]->compound_walls&=~COMPOUND_AAGUN;
                     }
                     else if(hasGenerator && !LCSrandom(3))
                     {
                        if(clearformess)erase();
                        else makedelimiter(8,0);
                        move(8,1);
                        addstr("The generator has been destroyed!", gamelog);
                        gamelog.newline();
                        refresh();
                        getch();
                        if(clearformess)erase();
                        else makedelimiter(8,0);
                        move(8,1);
                        addstr("The lights fade and all is dark.", gamelog);
                        gamelog.newline();
                        refresh();
                        getch();
                        
                        location[l]->compound_walls&=~COMPOUND_GENERATOR;
                        location[l]->siege.lights_off=1;
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
                              addstr(pool[targ]->name, gamelog);
                              addstr(" was killed in the bombing!", gamelog);
                              gamelog.newline();

                              if(pool[targ]->align==1)
                              {
                                 stat_dead++;
                                 liberalcount[l]--;
                              }

                              removesquadinfo(*pool[targ]);

                              pool[targ]->die();
                              //delete pool[targ];
                              //pool.erase(pool.begin() + targ);
                           }
                           else
                           {
                              addstr(pool[targ]->name, gamelog);
                              addstr(" narrowly avoided death!", gamelog);
                              gamelog.newline();
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
                        addstr("Fortunately, no one is hurt.", gamelog);
                        gamelog.newline();
                        refresh();
                        getch();
                     }
                  }
               }

               if((location[l]->compound_walls & COMPOUND_TANKTRAPS) &&
                  location[l]->siege.escalationstate>=3 && !LCSrandom(15))
               {
                  no_bad=0;

                  //ENGINEERS
                  if(clearformess)erase();
                  else makedelimiter(8,0);
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr("Army engineers have removed your tank traps.", gamelog);
                  gamelog.newline();

                  refresh();
                  getch();
                  
                  if(clearformess)
                  {
                     move(9,1);
                  }
                  else
                  {
                     makedelimiter(8,0);
                     move(8,1);
                  }
                  addstr("The tank moves forward to your compound entrance.", gamelog);
                  gamelog.newline();

                  refresh();
                  getch();

                  location[l]->compound_walls&=~COMPOUND_TANKTRAPS;
               }

               //NEED GOOD THINGS TO BALANCE THE BAD

               // ELITE REPORTER SNEAKS IN
               if(!LCSrandom(20)&&no_bad&&liberalcount[l]>0)
               {
                  char repname[200];
                  generate_name(repname);

                  set_color(COLOR_WHITE,COLOR_BLACK,0);

                  erase();
                  move(1,1);
                  addstr("Elitist ", gamelog);
                  addstr(repname, gamelog);
                  addstr(" from the ", gamelog);
                  switch(LCSrandom(5))
                  {
                     case 0:addstr("news program", gamelog);break;
                     case 1:addstr("news magazine", gamelog);break;
                     case 2:addstr("website", gamelog);break;
                     case 3:addstr("scandal rag", gamelog);break;
                     case 4:addstr("newspaper", gamelog);break;
                  }
                  addstr(" ");
                  switch(LCSrandom(12))
                  {
                     case 0:addstr("Daily", gamelog);break;
                     case 1:addstr("Nightly", gamelog);break;
                     case 2:addstr("Current", gamelog);break;
                     case 3:addstr("Pressing", gamelog);break;
                     case 4:addstr("Socialist", gamelog);break;
                     case 5:addstr("American", gamelog);break;
                     case 6:addstr("National", gamelog);break;
                     case 7:addstr("Union", gamelog);break;
                     case 8:addstr("Foreign", gamelog);break;
                     case 9:addstr("Associated", gamelog);break;
                     case 10:addstr("International", gamelog);break;
                     case 11:addstr("County", gamelog);break;
                  }
                  addstr(" ", gamelog);
                  switch(LCSrandom(11))
                  {
                     case 0:addstr("Reporter", gamelog);break;
                     case 1:addstr("Issue", gamelog);break;
                     case 2:addstr("Take", gamelog);break;
                     case 3:addstr("Constitution", gamelog);break;
                     case 4:addstr("Times", gamelog);break;
                     case 5:addstr("Post", gamelog);break;
                     case 6:addstr("News", gamelog);break;
                     case 7:addstr("Affair", gamelog);break;
                     case 8:addstr("Statesman", gamelog);break;
                     case 9:addstr("Star", gamelog);break;
                     case 10:addstr("Inquirer", gamelog);break;
                  }
                  move(2,1);
                  addstr("got into the compound somehow!", gamelog);
                  gamelog.newline();
                  refresh();
                  getch();

                  int best=-1,bestvalue=0,sum;
                  for(int p=0;p<pool.size();p++)
                  {
                     if(!pool[p]->alive)continue;
                     if(pool[p]->align!=1)continue;
                     if(pool[p]->location!=l)continue;

                     sum=0;
                     sum+=pool[p]->get_attribute(ATTRIBUTE_INTELLIGENCE, true);
                     sum+=pool[p]->get_attribute(ATTRIBUTE_HEART, true);
                     sum+=pool[p]->get_skill(SKILL_PERSUASION);
                     sum+=pool[p]->juice;

                     if(sum>bestvalue||best==-1)
                     {
                        best=p;
                        bestvalue=sum;
                     }
                  }

                  move(4,1);
                  addstr(pool[best]->name, gamelog);
                  addstr(" decides to give an interview.", gamelog);
                  gamelog.newline();
                  refresh();
                  getch();

                  move(6,1);
                  addstr("The interview is wide-ranging, covering a variety of topics.", gamelog);
                  gamelog.newline();
                  refresh();
                  getch();

                  sum=0;
                  sum+=pool[best]->attribute_roll(ATTRIBUTE_INTELLIGENCE);
                  sum+=pool[best]->attribute_roll(ATTRIBUTE_HEART);
                  sum+=pool[best]->skill_roll(SKILL_PERSUASION);
                  sum+=pool[best]->skill_roll(SKILL_PERSUASION);
                  sum+=pool[best]->skill_roll(SKILL_PERSUASION);

                  int segmentpower=sum;

                  move(8,1);
                  if(segmentpower<15)
                  {
                     addstr(repname, gamelog);
                     addstr(" cancelled the interview halfway through", gamelog);
                     move(9,1);
                     addstr("and later used the material for a Broadway play called", gamelog);
                     move(10,1);
                     switch(LCSrandom(11))
                     {
                        case 0:addstr("Flaming", gamelog);break;
                        case 1:
                                if(law[LAW_FREESPEECH]==-2) {
                                        addstr("Dumb", gamelog);
                                } else {
                                        addstr("Retarded", gamelog);
                                }
                                break;
                        case 2:addstr("Insane", gamelog);break;
                        case 3:addstr("Crazy", gamelog);break;
                        case 4:addstr("Loopy", gamelog);break;
                        case 5:addstr("Idiot", gamelog);break;
                        case 6:addstr("Empty-Headed", gamelog);break;
                        case 7:addstr("Nutty", gamelog);break;
                        case 8:addstr("Half-Baked", gamelog);break;
                        case 9:addstr("Pot-Smoking", gamelog);break;
                        case 10:addstr("Stoner", gamelog);break;
                     }
                     addstr(" ", gamelog);
                     switch(LCSrandom(10))
                     {
                        case 0:addstr("Liberal", gamelog);break;
                        case 1:addstr("Socialist", gamelog);break;
                        case 2:addstr("Anarchist", gamelog);break;
                        case 3:addstr("Communist", gamelog);break;
                        case 4:addstr("Marxist", gamelog);break;
                        case 5:addstr("Green", gamelog);break;
                        case 6:addstr("Elite", gamelog);break;
                        case 7:addstr("Guerrilla", gamelog);break;
                        case 8:addstr("Commando", gamelog);break;
                        case 9:addstr("Soldier", gamelog);break;
                     }
                     addstr(".", gamelog);
                     gamelog.newline();
                  }
                  else if(segmentpower<20)
                  {
                     addstr("But the interview is so boring that ", gamelog);
                     addstr(repname, gamelog);
                     addstr(" falls asleep.", gamelog);
                     gamelog.newline();
                  }
                  else if(segmentpower<25)
                  {
                     addstr("But ", gamelog);
                     addstr(pool[best]->name, gamelog);
                     addstr(" stutters nervously the whole time.", gamelog);
                     gamelog.newline();
                  }
                  else if(segmentpower<30)
                  {
                     addstr(pool[best]->name, gamelog);
                     addstr("'s verbal finesse leaves something to be desired.", gamelog);
                     gamelog.newline();
                  }
                  else if(segmentpower<45)
                  {
                     addstr(pool[best]->name, gamelog);
                     addstr(" represents the LCS well.", gamelog);
                     gamelog.newline();
                  }
                  else if(segmentpower<60)
                  {
                     addstr("The discussion was exciting and dynamic.", gamelog);
                     move(9,1);
                     addstr("Even the Cable News and AM Radio spend days talking about it.", gamelog);
                     gamelog.newline();
                  }
                  else
                  {
                     addstr(repname);
                     addstr(" later went on to win a Pulitzer for it.", gamelog);
                     move(9,1);
                     addstr("Virtually everyone in America was moved by ", gamelog);
                     addstr(pool[best]->name, gamelog);
                     addstr("'s words.", gamelog);
                     gamelog.newline();
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
            gamelog.newline(); // single blank line after every siege day
         } // end if(!location[l]->siege.underattack)
      } // end if(location[l]->siege.siege)
   } // end for(l=0;l<location.size();l++)
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
      if(location[loc]->siege.siegetype==SIEGE_POLICE && location[loc]->siege.escalationstate == 0)
         addstr("The police", gamelog);
      else if(location[loc]->siege.siegetype==SIEGE_POLICE && location[loc]->siege.escalationstate >= 1)
         addstr("The soldiers", gamelog);
      else
         addstr("The firemen", gamelog);
      addstr(" confiscate everything, including Squad weapons.", gamelog);
      gamelog.newline();

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
         addstr(" is rehabilitated and freed.", gamelog);
         gamelog.newline();
      }
      if(kcount>1)
      {
         move(3,1);
         addstr("The kidnap victims are rehabilitated and freed.", gamelog);
         gamelog.newline();
      }
      if(pcount==1)
      {
         move(5,1);
         addstr(pname, gamelog);
         if(stricmp(pname,pcname))
         {
            addstr(", aka ", gamelog);
            addstr(pcname, gamelog);
            addstr(",", gamelog);
         }
         move(6,1);
         addstr("is taken to the police station.", gamelog);
         gamelog.newline();
      }
      if(pcount>1)
      {
         char num[20];
         move(5,1);
         itoa(pcount,num,10);
         addstr(num, gamelog);
         addstr(" Liberals are taken to the police station.", gamelog);
         gamelog.newline();
      }
      if(ledger.get_funds()>0)
      {
         if(ledger.get_funds()<=2000 || location[loc]->siege.siegetype==SIEGE_FIREMEN)
         {
            move(8,1);
            addstr("Fortunately, your funds remain intact.", gamelog);
            gamelog.newline();
         }
         else
         {
            move(8,1);
            int confiscated = LCSrandom(LCSrandom(ledger.get_funds()-2000)+1)+1000;
            if(ledger.get_funds()-confiscated > 50000)
               confiscated += ledger.get_funds() - 30000 - LCSrandom(20000) - confiscated;
            char str[100];
            sprintf(str,"Law enforcement has confiscated $%d in LCS funds.",confiscated);
            addstr(str, gamelog);
            gamelog.newline();
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
            addstr("The printing press is dismantled and burned.", gamelog);
            gamelog.newline();
            location[loc]->compound_walls &= ~COMPOUND_PRINTINGPRESS;
         }
      }
      else
      {
         if(location[loc]->compound_walls)
         {
            move(10,1);
            addstr("The compound is dismantled.", gamelog);
            gamelog.newline();
            location[loc]->compound_walls=0;
         }
      }
      if(location[loc]->front_business!=-1)
      {
         move(12,1);
         addstr("Materials relating to the business front have been taken.", gamelog);
         gamelog.newline();
         location[loc]->front_business=-1;
      }

      refresh();
      getch();

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

         pool[p]->drop_weapons_and_clips(NULL);

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
         pool[p]->die();
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
      addstr("Everyone in the ", gamelog);
      addstr(location[loc]->getname(), gamelog);
      addstr(" is slain.", gamelog);
      gamelog.newline();
      refresh();
      getch();


      newsstoryst *ns=new newsstoryst;
         ns->type=NEWSSTORY_MASSACRE;
         ns->loc=loc;
         ns->crime.push_back(location[loc]->siege.siegetype);
         ns->crime.push_back(killnumber);
      newsstory.push_back(ns);

      //MUST SET cursite TO SATISFY endcheck() CODE
      int tmp=cursite;
      cursite=loc;
      endcheck();
      cursite=tmp;

      location[loc]->siege.siege=0;
   }

   //CONFISCATE MATERIAL
   for(int l=0;l<location[loc]->loot.size();l++)delete location[loc]->loot[l];
   location[loc]->loot.clear();
   for(int v=(int)vehicle.size()-1;v>=0;v--)
   {
      if(vehicle[v]->get_location()==loc)
      {
         delete vehicle[v];
         vehicle.erase(vehicle.begin() + v);
      }
   }

   gamelog.newline();

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
      //Don't count Conservatives as eaters. Just assume they get fed scraps or something.
      if(pool[p]->align==-1)continue;
      //You're a sleeper agent? Sleepers don't eat! Feh! (Rather, they eat on Conservatives' expense, not yours.)
      if(pool[p]->flag & CREATUREFLAG_SLEEPER)continue;
      //None of the above? You're eating!
      eaters++;
   }

   return eaters;
}

// Siege -- Mass combat outside safehouse
char sally_forth_aux(int loc)
{
   int p;
   reloadparty();
   siegest siege = location[loc]->siege;
   cursite = loc;

   switch(siege.siegetype)
   {
   case SIEGE_CIA:
   case SIEGE_HICKS:
   case SIEGE_CORPORATE:
   case SIEGE_CCS:
   case SIEGE_FIREMEN:
   default:

   case SIEGE_POLICE: // Currently only police sieges should allow this

      // SWAT teams
      if(siege.escalationstate==0)
      {
         for(int e=0;e<ENCMAX-9;e++)
         {
            makecreature(encounter[e],CREATURE_SWAT);
         }
      }
      // Military
      else if(siege.escalationstate>=1)
      {
         for(int e=0;e<ENCMAX-9;e++)
         {
            makecreature(encounter[e],CREATURE_SOLDIER);
         }
      }

      // M1 Tank
      if(siege.escalationstate>=2 && !(location[loc]->compound_walls & COMPOUND_TANKTRAPS))
      {
         makecreature(encounter[ENCMAX-9],CREATURE_TANK);
      }
      break;
   }

   mode=GAMEMODE_CHASEFOOT;
   bool ranaway = false;

   do
   {
      // Count heroes
      int partysize=0;
      int partyalive=0;
      for(p=0;p<pool.size();p++)
      {
         if(pool[p]->align==1&&
            pool[p]->location==cursite&&
            !(pool[p]->flag & CREATUREFLAG_SLEEPER))
         {
            partysize++;
            if(pool[p]->alive) partyalive++;
         }
      }

      // Count bad guys
      int encsize=0;
      for(int e=0;e<ENCMAX;e++)
      {
         if(encounter[e].exists)
         {
            encsize++;
         }
      }

      // Let's roll
      autopromote(loc);
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr(location[loc]->getname());

      // Player's party
      if(partyalive==0)party_status=-1;
      printparty();

      if(partyalive>0)
      {
         // Options
         if(partysize>1)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(9,40);
         addstr("O - Change the squad's Liberal order");
         if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,40);
         addstr("# - Check the status of a squad Liberal");
         if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,40);
         addstr("0 - Show the squad's Liberal status");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(9,1);
         addstr("D - Escape");
         move(10,1);
         addstr("E - Equip");
         move(11,1);
         addstr("F - Fight!");
         move(12,1);
         addstr("G - Surrender");
      }
      else
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(9,1);
         addstr("C - Reflect on your Conservative judgment.");
      }

      // Enemies
      printencounter();

      refresh();

      // check if we fought the previous loop; if so, add a blank gamelog line
      if(foughtthisround)gamelog.newline();
      foughtthisround=0;

      int c=getch();
      translategetch(c);

      // Reflecting on your poor judgment
      if(partyalive==0&&c=='c')
      {
         /*for(p=0;p<6;p++)
         {
            if(activesquad->squad[p]==NULL)continue;

            for(int pl=pool.size()-1;pl>=0;pl--)
            {
               if(pool[pl]==activesquad->squad[p])
               {
                  pool[pl]->die();
                  pool[pl]->location=-1;
                  break;
               }
            }

            activesquad->squad[p]=NULL;
         }*/

         if(!endcheck())
         {
            mode=GAMEMODE_BASE;
            return 0;
         }
      }

      // Providing orders
      if(partyalive>0)
      {
         // Reorder
         if(c=='o'&&partysize>1)orderparty();

         // View status
         if(c=='0')party_status=-1;

         // Character info
         if(c>='1'&&c<='6')
         {
            if(activesquad->squad[c-'1']!=NULL)
            {
               if(party_status==c-'1')fullstatus(party_status);
               else party_status=c-'1';
            }
         }

         // Surrender
         if(c=='g')
         {
            giveup();
         }

         // Run away
         if(c=='d')
         {
            if(encounter[0].exists&&
               encounter[0].type==CREATURE_COP)
            {
               sitestory->crime.push_back(CRIME_FOOTCHASE);
               criminalizeparty(LAWFLAG_RESIST);
            }

            evasiverun();
            enemyattack();
            creatureadvance();

            ranaway = true;
         }


         if(c=='f')
         {
            youattack();
            enemyattack();
            creatureadvance();
         }

         if(c=='e')
         {
            equip(location[loc]->loot,-1);
         }

         // Check for victory
         partysize=0;
         partyalive=0;
         for(p=0;p<pool.size();p++)
         {
            if(pool[p]->align==1&&
               pool[p]->location==cursite&&
               !(pool[p]->flag & CREATUREFLAG_SLEEPER))
            {
               partysize++;
               if(pool[p]->alive) partyalive++;
            }
         }

         int baddiecount=0;
         for(int e=0;e<ENCMAX;e++)
         {
            if(encounter[e].enemy()&&
               encounter[e].alive&&
               encounter[e].exists)baddiecount++;
         }

         if(partyalive>0&&baddiecount==0)
         {
            for(int p=0;p<pool.size();p++)
            {
               for(int w=0;w<BODYPARTNUM;w++)
               {
                  pool[p]->wound[w]&=~WOUND_BLEEDING;
               }
            }
            mode=GAMEMODE_BASE;

            if(ranaway == true)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               clearmessagearea();
               move(16,1);
               addstr("You're free!", gamelog);
               gamelog.nextMessage();
               refresh();
               getch();

               escapesiege(false);
               return 1;
            }
            else
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               clearmessagearea();
               move(16,1);
               addstr("The siege is broken!", gamelog);
               gamelog.nextMessage();
               refresh();
               getch();

               conquertext();
               escapesiege(true);
               return 2;
            }
         }
      }
      
   }while(1);

   mode=GAMEMODE_BASE;
   return 1;
}

/* siege - prepares for exiting the siege to fight the attackers head on */
void sally_forth(void)
{
   //GIVE INFO SCREEN
   erase();
   set_color(COLOR_RED,COLOR_BLACK,1);
   move(1,26);
   addstr("UNDER SIEGE: ESCAPE OR ENGAGE");

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(3,16);
   addstr("You are about to exit the compound to lift the Conservative");
   move(4,11);
   addstr("siege on your safehouse.  The enemy is ready for you, and");
   move(5,11);
   addstr("you will have to defeat them all or run away to survive this");
   move(6,11);
   addstr("encounter.");
   move(8,11);
   addstr("Your Squad has filled out to six members if any were ");
   move(9,11);
   addstr("available.  If you have a larger pool of Liberals, they");
   move(10,11);
   addstr("will provide cover fire from the compound until needed.");
   
   int loc=-1;
   if(selectedsiege!=-1)loc=selectedsiege;
   if(activesquad!=NULL)loc=activesquad->squad[0]->location;
   if(loc==-1)return;

   set_color(COLOR_RED,COLOR_BLACK,1);
   move(23,11);
   addstr("Press any key to Confront the Conservative Aggressors");

   // Seperate logging text
   gamelog.log("Your Liberals sally forth to confront the siege.");

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
      squad.back()->id=cursquadid++;
      strcpy(squad.back()->name,location[selectedsiege]->getname(true).c_str());
      strcat(squad.back()->name," Defense");
      int i=0;
      for(int p=0;p<pool.size();p++)
      {
         if(pool[p]->location == selectedsiege &&
            pool[p]->alive && pool[p]->align == 1)
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
      ns->type=NEWSSTORY_SQUAD_ESCAPED;
      ns->positive=1;
      ns->loc=loc;
      ns->siegetype=location[loc]->siege.siegetype;
   newsstory.push_back(ns);
   sitestory = ns;
   char result = sally_forth_aux(loc);
   if(result==2)
   {
      ns->type=NEWSSTORY_SQUAD_BROKESIEGE;
   }
   // If you fail, make sure the safehouse isn't under siege anymore by
   // forcing you to "give up".
   if(result==0)
   {
      gamelog.log("You have been defeated.");
      resolvesafehouses();
   }
}


/* siege - prepares for entering site mode to fight the siege */
void escape_engage(void)
{
   //GIVE INFO SCREEN
   erase();
   set_color(COLOR_RED,COLOR_BLACK,1);
   move(1,26);
   addstr("UNDER ATTACK: ESCAPE OR ENGAGE");

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
   addstr("choose, any equipment from the safehouse which isn't held by a");
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

   // Seperate logging text
   gamelog.log("Your Liberals confront the Conservatives within the safehouse.");

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
      squad.back()->id=cursquadid++;
      strcpy(squad.back()->name,location[selectedsiege]->getname(true).c_str());
      strcat(squad.back()->name," Defense");
      int i=0;
      for(int p=0;p<pool.size();p++)
      {
         if(pool[p]->location == selectedsiege &&
            pool[p]->alive && pool[p]->align == 1)
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
      addstr("You have escaped!", gamelog);
      gamelog.nextMessage();

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

      // Seperate logging text
      gamelog.log("Your Liberals split up and lay low for a few days.");

      refresh();
      getch();

//dump retrieved loot in homeless shelter - is there anywhere better to put it?
      if (activesquad && homes != -1)
      {
         for(int l=0;l<activesquad->loot.size();l++)
         {
            location[homes]->loot.push_back(activesquad->loot[l]);
         }
         activesquad->loot.clear();
      }

      activesquad=NULL; //active squad cannot be disbanded in removesquadinfo, 
                        //but we need to disband current squad as the people are going to be 'away'.

      //GET RID OF DEAD, etc.
      if(location[cursite]->renting>1)location[cursite]->renting=RENTING_NOCONTROL;

      for(int p=pool.size()-1;p>=0;p--)
      {
         if(pool[p]->location!=cursite)continue;
         if(!pool[p]->alive)
         {
            delete pool[p];
            pool.erase(pool.begin() + p);
            continue;
         }

         //BASE EVERYONE LEFT AT HOMELESS SHELTER
         removesquadinfo(*pool[p]);
         pool[p]->hiding=LCSrandom(3)+2;
         if(pool[p]->align==1) // not a hostage
         {
            pool[p]->location=-1;
         }
         else // hostages don't go into hiding, just shove em into the homeless shelter
         {
            pool[p]->location=homes;
         }
         pool[p]->base=homes;
      }
      for(int l2=0;l2<location[cursite]->loot.size();l2++)
      {
         delete location[cursite]->loot[l2];
      }
      location[cursite]->loot.clear();

      for(int v=(int)vehicle.size()-1;v>=0;v--)
      {
         if(vehicle[v]->get_location()==cursite)
         {
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
   addstr("* * * * *   VICTORY   * * * * *", gamelog);
   gamelog.newline();

   if(location[cursite]->siege.siegetype==SIEGE_POLICE)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(3,16);
      addstr("The Conservative automatons have been driven back -- for ", gamelog);
      move(4,11);
      addstr("the time being.  While they are regrouping, you might consider ", gamelog);
      move(5,11);
      addstr("abandoning this safe house for a safer location.", gamelog);
      gamelog.nextMessage();
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(3,16);
      addstr("The Conservative automatons have been driven back. ", gamelog);
      move(4,11);
      addstr("Unfortunately, you will never truly be safe from ", gamelog);
      move(5,11);
      addstr("this filth until the Liberal Agenda is realized.", gamelog);
      gamelog.nextMessage();
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
      if(ccs_siege_kills>10)
      {
         addstr("Gunfire still ringing in their ears, the squad revels in");
         move(4,11);
         addstr("their victory.  ");
      }
      else
      {
         addstr("The CCS Lieutenant lying dead at their feet, the squad");
         move(4,11);
         addstr("slips away.  ");
      }
      addstr("The CCS Founder wasn't here, but for now, their");
      move(5,11);
      addstr("power has been severely weakened.  Once the safehouse cools off,");
      move(6,11);
      addstr("this will make a fine base for future Liberal operations.");
   }
   else
   {
      move(3,16);
      if(ccs_siege_kills>10)
      {
         addstr("Gunfire still ringing in their ears, the squad revels in");
         move(4,11);
         addstr("their final victory.  ");
         
         move(6,16);
         addstr("As your Liberals pick through the remains of the safehouse,");
         move(7,11);
         addstr("it is increasingly clear that this was the CCS's last safe house.");
      }
      else
      {
         addstr("The CCS Founder lying dead at their feet, the squad");
         move(4,11);
         addstr("slips away.  ");
            
         move(6,16);
         addstr("With its Founder killed in the heart of their own base,");
         move(7,11);
         addstr("the last of the enemy's morale and confidence is shattered.");
      }

      move(9,16);
      addstr("The CCS has been completely destroyed.  Now wasn't there a");
      move(10,16);
      addstr("revolution to attend to?");

      move(12,5);
      addstr("+200 JUICE TO EVERYONE FOR ERADICATING THE CONSERVATIVE CRIME SQUAD");

      for(int p=0;p<pool.size();p++)
      {
         addjuice(*pool[p],200,1000);
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
   if(location[loc]->siege.underattack)addstr("You hear shouts:", gamelog);
   else addstr("You hear a blaring voice on a loudspeaker:", gamelog);
   gamelog.newline();

   move(3,1);
   if(location[loc]->siege.escalationstate>=2 && publicmood(-1) < 20)
      addstr("In the name of God, your campaign of terror ends here!", gamelog);
   else
      addstr("Surrender yourselves!", gamelog);
   gamelog.newline();

   move(4,1);

   //KIDNAP VICTIM
   if(kidnapped)
   {
      addstr("Release ", gamelog);
      addstr(kname, gamelog);
      if(kidnapped>1)addstr(" and the others", gamelog);
      addstr(" unharmed!", gamelog);
   }
   //TREASON
   else if(breakercount[LAWFLAG_TREASON])
   {
      addstr("You are wanted for treason", gamelog);
   }
   //TERRORISM
   else if(breakercount[LAWFLAG_TERRORISM])
   {
      addstr("You are wanted for terrorism", gamelog);
   }
   //MURDERER
   else if(breakercount[LAWFLAG_MURDER])
   {
      addstr("You are wanted for murder", gamelog);
   }
   //KIDNAPPER
   else if(breakercount[LAWFLAG_KIDNAPPING])
   {
      addstr("You are wanted for kidnapping", gamelog);
   }
   //BANK ROBBER
   else if(breakercount[LAWFLAG_BANKROBBERY])
   {
      addstr("You are wanted for bank robbery", gamelog);
   }
   //BURN FLAG
   else if(breakercount[LAWFLAG_BURNFLAG])
   {
      if(law[LAW_FLAGBURNING]==-2)addstr("You are wanted for flag murder", gamelog);
      else addstr("You are wanted for flag burning", gamelog);
   }
   //SPEECH
   else if(breakercount[LAWFLAG_SPEECH])
   {
      addstr("You are wanted for harmful speech", gamelog);
   }
   //BROWNIES
   else if(breakercount[LAWFLAG_BROWNIES])
   {
      addstr("You are wanted for drug dealing", gamelog);
   }
   //ESCAPED
   else if(breakercount[LAWFLAG_ESCAPED])
   {
      addstr("You are wanted for escaping prison", gamelog);
   }
   //HELP ESCAPED
   else if(breakercount[LAWFLAG_HELPESCAPE])
   {
      addstr("You are wanted for aiding a prison escape", gamelog);
   }
   //JURY
   else if(breakercount[LAWFLAG_JURY])
   {
      addstr("You are wanted for jury tampering", gamelog);
   }
   //RACKETEERING
   else if(breakercount[LAWFLAG_RACKETEERING])
   {
      addstr("You are wanted for racketeering", gamelog);
   }
   //EXTORTION
   else if(breakercount[LAWFLAG_EXTORTION])
   {
      addstr("You are wanted for extortion", gamelog);
   }
   //ASSAULT
   else if(breakercount[LAWFLAG_ARMEDASSAULT])
   {
      addstr("You are wanted for armed assault", gamelog);
   }
   //ASSAULT
   else if(breakercount[LAWFLAG_ASSAULT])
   {
      addstr("You are wanted for assault", gamelog);
   }
   //CAR THEFT
   else if(breakercount[LAWFLAG_CARTHEFT])
   {
      addstr("You are wanted for motor theft", gamelog);
   }
   //CC FRAUD
   else if(breakercount[LAWFLAG_CCFRAUD])
   {
      addstr("You are wanted for credit card fraud", gamelog);
   }
   //THIEF
   else if(breakercount[LAWFLAG_THEFT])
   {
      addstr("You are wanted for theft", gamelog);
   }
   //PROSTITUTION
   else if(breakercount[LAWFLAG_PROSTITUTION])
   {
      addstr("You are wanted for prostitution", gamelog);
   }
   //HIRE ILLEGAL
   else if(breakercount[LAWFLAG_HIREILLEGAL])
   {
      addstr("You are wanted for hiring an illegal alien", gamelog);//XXX: Maybe `undocumented worker` if LAW_IMMIGRATION is 0, or maybe 1...
   }
   //GUN CARRY
   /*else if(breakercount[LAWFLAG_GUNUSE])
   {
      addstr("You are wanted for use of an illegal weapon", gamelog);
   }
   //GUN CARRY
   else if(breakercount[LAWFLAG_GUNCARRY])
   {
      addstr("You are wanted for carrying an illegal weapon", gamelog);
   }*/
   //COMMERCE
   else if(breakercount[LAWFLAG_COMMERCE])
   {
      addstr("You are wanted for interfering with interstate commerce", gamelog);
   }
   //INFORMATION
   else if(breakercount[LAWFLAG_INFORMATION])
   {
      addstr("You are wanted for unlawfully accessing an information system", gamelog);
   }
   //UNLAWFUL BURIAL
   else if(breakercount[LAWFLAG_BURIAL])
   {
      addstr("You are wanted for unlawful burial", gamelog);
   }
   //BREAKING
   else if(breakercount[LAWFLAG_BREAKING])
   {
      addstr("You are wanted for breaking and entering", gamelog);
   }
   //VANDALISM
   else if(breakercount[LAWFLAG_VANDALISM])
   {
      addstr("You are wanted for vandalism", gamelog);
   }
   //RESIST
   else if(breakercount[LAWFLAG_RESIST])
   {
      addstr("You are wanted for resisting arrest", gamelog);
   }
   //DISTURBANCE
   else if(breakercount[LAWFLAG_DISTURBANCE])
   {
      addstr("You are wanted for disturbing the peace", gamelog);
   //UNDEFINED
   }else
   {
      addstr("You are wanted for finding a bug", gamelog);
   }

   if(!kidnapped)
   {
      if(typenum>1)addstr(" and other crimes", gamelog);
      addstr(".", gamelog);
   }
   gamelog.nextMessage();

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
   //BANK ROBBER
   else if(breakercount[LAWFLAG_BANKROBBERY])
   {
      addstr("BANK ROBBERY");
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
   /*else if(breakercount[LAWFLAG_GUNUSE])
   {
      addstr("FIRING ILLEGAL WEAPONS");
   }
   //GUN CARRY
   else if(breakercount[LAWFLAG_GUNCARRY])
   {
      addstr("USING ILLEGAL WEAPONS");
   }*/
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
   //RESIST
   else if(breakercount[LAWFLAG_EXTORTION])
   {
      addstr("EXTORTION");
   }
}

