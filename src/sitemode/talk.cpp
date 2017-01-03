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
#include <includeDefault.h>
#include "configfile.h"
#include "tinydir.h"
#include <includeEnum.h>
#include <includeCommon.h>

/*
consolesupport.cpp
*/
#include "common\\consolesupport.h"

#include <includeNews.h>
#include <includeFunctions.h>

//includeTalk.h includes all the externs talk.cpp requires, and no more
#include <includeTalk.h>
extern char newscherrybusted;
extern vector<Location *> location;
extern Alignment exec[EXECNUM];

extern vector<vector<string>> no_free_speech_flirt;
extern vector<vector<string>> pickupLines;
extern vector<string> dog_rejection;
extern vector<string> mutant_rejection;
extern vector<string> that_is_disturbing;
extern vector<string> that_is_not_disturbing;
extern vector<vector<string>> lovingly_talk_to_dog;
extern vector<vector<string>> normal_talk_to_dog;
extern vector<vector<string>> lovingly_talk_to_mutant;
extern vector<vector<string>> normal_talk_to_mutant;

extern vector<string> robbing_bank;
extern vector<string> teller_gestures;
extern vector<string> teller_complies;

extern vector<string> come_at_me_bro;
extern vector<string> backs_off;
extern vector<string> threaten_hostage;
extern vector<string> please_spare_hostage;
extern vector<string> who_cares_about_hostage;
extern vector<string> hostage_negotiation;
extern vector<string> please_no_more;
extern vector<string> let_hostages_go;
extern vector<string> go_ahead_and_die;
extern vector<string> agree_to_release_hostages;

string while_naked = " while naked";

char heyMisterDog(Creature &a, Creature &tk);
char heyMisterMonster(Creature &a, Creature &tk);

char talkInCombat(Creature &a, Creature &tk);
char talkToBankTeller(Creature &a, Creature &tk);
char talkToGeneric(Creature &a, Creature &tk);

char wannaHearSomethingDisturbing(Creature &a, Creature &tk);
char talkAboutIssues(Creature &a, Creature &tk);

char doYouComeHereOften(Creature &a, Creature &tk);

char heyINeedAGun(Creature &a, Creature &tk);
char heyIWantToRentARoom(Creature &a, Creature &tk);
char heyIWantToCancelMyRoom(Creature &a, Creature &tk);

/* bluff, date, issues */
char talk(Creature &a,int t)
{
   Creature &tk = encounter[t];

   // TALKING TO DOGS
   if(tk.type == CREATURE_GUARDDOG && tk.align != ALIGN_LIBERAL)
   {
      return heyMisterDog(a, tk);
   }

   // TALKING TO MONSTERS
   if(tk.type == CREATURE_GENETIC && tk.align != ALIGN_LIBERAL)
   {
      return heyMisterMonster(a, tk);
   }

   // BLUFFING
   if((sitealarm||location[cursite]->siege.siege)&&tk.enemy())
   {
      return talkInCombat(a, tk);
   }

   return talkToGeneric(a, tk);
}

char talkToBankTeller(Creature &a, Creature &tk)
{

   clearcommandarea();clearmessagearea();clearmaparea();
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   /*move(9,1);
   addstr(a.name);
   addstr(" prepares to rob the bank:");*/
   bool is_naked = a.is_naked() && a.animalgloss != ANIMALGLOSS_ANIMAL;
   move(11,1);
   addstr("A - Quietly pass the teller a robbery note");
   if(is_naked)addstr(while_naked);
   addstr(".");
   move(12,1);
   addstr("B - Threaten bystanders and demand access to the vault");
   if(is_naked)addstr(while_naked);
   addstr(".");
   move(13,1);
   addstr("C - On second thought, don't rob the bank");
   if(is_naked)addstr(while_naked);
   addstr(".");

   int c;
   do c=getkey(); while(c<'a'&&c>'c');

   switch(c)
   {
   case 'a':
      clearcommandarea();clearmessagearea();clearmaparea();
      set_color(COLOR_WHITE, COLOR_BLACK, 1);
      move(9, 1);
      addstr(a.name, gamelog);
      addstr(" slips the teller a note: ", gamelog);
      set_color(COLOR_GREEN, COLOR_BLACK, 1);
      move(10, 1);
	  addstr(pickrandom(robbing_bank), gamelog);
      gamelog.newline();

      getkey();

      if(location[cursite]->highsecurity)
      {
         set_color(COLOR_WHITE, COLOR_BLACK, 1);
         move(11, 1);
         addstr("The bank teller reads the note, ", gamelog);
         addstr(pickrandom(teller_gestures), gamelog);
         move(12, 1);
         addstr("and dives for cover as the guards move in on the squad!", gamelog);
         gamelog.newline();

         getkey();

         sitealarm=1;
         criminalize(a, LAWFLAG_BANKROBBERY);
         sitestory->crime.push_back(CRIME_BANKTELLERROBBERY);
         sitecrime+=30;
         makecreature(encounter[0],CREATURE_MERC);
         makecreature(encounter[1],CREATURE_MERC);
         makecreature(encounter[2],CREATURE_MERC);
         makecreature(encounter[3],CREATURE_MERC);
      }
      else
      {
         set_color(COLOR_WHITE, COLOR_BLACK, 1);
         move(11, 1);
         addstr("The bank teller reads the note, ", gamelog);
         addstr(pickrandom(teller_complies), gamelog);
         move(12, 1);
         addstr("and slips several bricks of cash into the squad's bag.", gamelog);
         gamelog.newline();

         getkey();

         criminalize(a, LAWFLAG_BANKROBBERY);
         sitestory->crime.push_back(CRIME_BANKTELLERROBBERY);
         sitecrime+=30;
         sitealarmtimer=0;
         activesquad->loot.push_back(new Money(5000));
      }
      tk.cantbluff=1;
      return 1;
   case 'b':
   {
      clearcommandarea();clearmessagearea();clearmaparea();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      Creature *armed_liberal=NULL;
      for(int i=0;i<6;i++)
      {
         if(activesquad->squad[i] &&
            activesquad->squad[i]->get_weapon().is_threatening())
         {
            armed_liberal=activesquad->squad[i];
            break;
         }
      }
      if(armed_liberal)
      {
         move(9,1);
         addstr(armed_liberal->name, gamelog);
         addstr(" brandishes the ", gamelog);
         addstr(armed_liberal->get_weapon().get_shortname(0), gamelog);
         addstr(".", gamelog);
         gamelog.newline();

         getkey();

         clearmessagearea();
      }
      move(10,1);
      addstr(a.name, gamelog);
      addstr(" says, ", gamelog);
      set_color(COLOR_GREEN,COLOR_BLACK,1);
      move(11,1);
      addstr("\"", gamelog);
      addstr(slogan, gamelog);
      gamelog.record(" ");
      move(12,1);
      addstr("OPEN THE VAULT, NOW!\"", gamelog);
      gamelog.newline();

      getkey();

      int roll       = a.skill_roll(SKILL_PERSUASION);
      int difficulty = DIFFICULTY_VERYEASY;

      if(armed_liberal == NULL)
         difficulty += 12;
      if(location[cursite]->highsecurity>0)
         difficulty += 12;

      clearcommandarea();clearmessagearea();clearmaparea();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      if(roll<difficulty)
      {
         set_color(COLOR_WHITE, COLOR_BLACK, 1);
         move(9, 1);
         addstr("The bank teller and dives for cover as ", gamelog);
         move(10, 1);
         addstr("guards move in on the squad!", gamelog);
         gamelog.newline();

         getkey();

         sitealarm=1;
         sitealienate=2;
         criminalizeparty(LAWFLAG_BANKROBBERY);
         sitestory->crime.push_back(CRIME_BANKSTICKUP);
         sitecrime+=50;
         CreatureTypes guard = CREATURE_SECURITYGUARD;
         if(location[cursite]->highsecurity>0) guard = CREATURE_MERC;
         makecreature(encounter[0],guard);
         makecreature(encounter[1],guard);
         makecreature(encounter[2],guard);
         makecreature(encounter[3],guard);
         makecreature(encounter[4],guard);
         makecreature(encounter[5],guard);
      }
      else
      {
         set_color(COLOR_WHITE, COLOR_BLACK, 1);
         move(9, 1);
         addstr("The bank employees hesitantly cooperate!", gamelog);
         gamelog.newline();

         getkey();

         move(10, 1);
         addstr("The vault is open!", gamelog);
         gamelog.newline();

         getkey();

         criminalizeparty(LAWFLAG_BANKROBBERY);
         sitestory->crime.push_back(CRIME_BANKSTICKUP);
         sitecrime+=50;
         sitealarm=1;
         sitealienate=2;

         for(int x=0;x<MAPX;x++)
         for(int y=0;y<MAPY;y++)
         for(int z=0;z<MAPZ;z++)
         {
            levelmap[x][y][z].flag &= ~SITEBLOCK_LOCKED;
            if(levelmap[x][y][z].flag & SITEBLOCK_METAL)
               levelmap[x][y][z].flag &= ~SITEBLOCK_DOOR;
            if(levelmap[x][y][z].special == SPECIAL_BANK_VAULT)
               levelmap[x][y][z].special = SPECIAL_NONE;
         }

         encounter[0].exists = false;
      }
   }
   default:
   case 'c':
      return 0;
   }
}

char talkToGeneric(Creature &a, Creature &tk)
{
   clearcommandarea();clearmessagearea();clearmaparea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(9,1);
   addstr(a.name);
   addstr(" talks to ");
   switch(tk.align)
   {
   case ALIGN_CONSERVATIVE:
      set_color(COLOR_RED,COLOR_BLACK,1);
      break;
   case ALIGN_LIBERAL:
      set_color(COLOR_GREEN,COLOR_BLACK,1);
      break;
   case ALIGN_MODERATE:
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      break;
   }
   addstr(tk.name);
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   add_age(tk);
   addstr(":");

   bool is_naked = a.is_naked() && a.animalgloss != ANIMALGLOSS_ANIMAL;

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(11,1);
   addstr("A - Strike up a conversation about politics");
   if(is_naked)addstr(while_naked);
   addstr(".");
   move(12,1);
   if(tk.can_date(a))set_color(COLOR_WHITE,COLOR_BLACK,0);
   else set_color(COLOR_BLACK,COLOR_BLACK,1);
   addstr("B - Drop a pickup line");
   if(is_naked)addstr(while_naked);
   addstr(".");
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(13,1);
   addstr("C - On second thought, don't say anything");
   if(is_naked)addstr(while_naked);
   addstr(".");
   if(tk.type==CREATURE_LANDLORD&&location[cursite]->renting==-1)
   {
      move(14,1);
      addstr("D - Rent a room");
      if(is_naked)addstr(while_naked);
      addstr(".");
   }
   else if(tk.type==CREATURE_LANDLORD&&location[cursite]->renting>0)
   {
      move(14,1);
      addstr("D - Stop renting a room");
      if(is_naked)addstr(while_naked);
      addstr(".");
   }
   else if(tk.type==CREATURE_GANGMEMBER||tk.type==CREATURE_MERC)
   {
      move(14,1);
      addstr("D - Buy weapons");
      if(is_naked)addstr(while_naked);
      addstr(".");
   }
   else if(tk.type==CREATURE_BANK_TELLER)
   {
      move(14,1);
      addstr("D - Rob the bank");
      if(is_naked)addstr(while_naked);
      addstr(".");
   }

   while(true)
   {
      int c=getkey();

      switch(c)
      {
      case 'a':
         return wannaHearSomethingDisturbing(a, tk);
      case 'b':
         if(!tk.can_date(a)) break;
         return doYouComeHereOften(a, tk);
      case 'c':
         return 0;
      case 'd':
         if(tk.type==CREATURE_LANDLORD&&location[cursite]->renting==-1)
            return heyIWantToRentARoom(a, tk);
         else if (tk.type == CREATURE_LANDLORD && location[cursite]->renting > 0)
            return heyIWantToCancelMyRoom(a, tk);
         else if(tk.type==CREATURE_GANGMEMBER||tk.type==CREATURE_MERC)
            return heyINeedAGun(a, tk);
         else if(tk.type==CREATURE_BANK_TELLER)
            return talkToBankTeller(a, tk);
         break;
      }
   }
}

char heyIWantToCancelMyRoom(Creature &a, Creature &tk)
{
   clearcommandarea();
   clearmessagearea();
   clearmaparea();
   set_color(COLOR_WHITE, COLOR_BLACK, 1);
   move(9, 1);
   addstr(a.name, gamelog);
   addstr(" says, ", gamelog);
   set_color(COLOR_GREEN, COLOR_BLACK, 1);
   move(10, 1);
   addstr("\"I'd like cancel my room.\"", gamelog);
   gamelog.newline();

   getkey();
   bool is_naked = a.is_naked() && a.animalgloss != ANIMALGLOSS_ANIMAL;
   if (is_naked)
   {
      set_color(COLOR_WHITE, COLOR_BLACK, 1);
      move(12, 1);
      addstr(tk.name, gamelog);
      addstr(" responds, ", gamelog);
      set_color(COLOR_CYAN, COLOR_BLACK, 1);
      move(13, 1);
      addstr("\"Put some clothes on before I call the cops.\"", gamelog);
      gamelog.newline();

      getkey();

      return 1;
   }

   set_color(COLOR_WHITE, COLOR_BLACK, 1);
   move(12, 1);
   addstr(tk.name, gamelog);
   addstr(" responds, ", gamelog);
   set_color(COLOR_CYAN, COLOR_BLACK, 1);
   move(13, 1);
   addstr("\"Alright. Please clear out your room.\"", gamelog);
   gamelog.newline();

   getkey();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(15,1);
   addstr("<Your possessions at this location have been moved to the shelter.>", gamelog);
   gamelog.newline();

   getkey();

   location[cursite]->renting=RENTING_NOCONTROL;

   //MOVE ALL ITEMS AND SQUAD MEMBERS
   int hs=find_homeless_shelter(cursite);
   for(int p=0;p<len(pool);p++)
   {
      if(pool[p]->location==cursite)pool[p]->location=hs;
      if(pool[p]->base==cursite)pool[p]->base=hs;
   }
   location[hs]->getloot(location[cursite]->loot);

   location[cursite]->compound_walls=0;
   location[cursite]->compound_stores=0;
   location[cursite]->front_business=-1;

   return 1;
}

char heyIWantToRentARoom(Creature &a, Creature &tk)
{
   clearcommandarea();clearmessagearea();clearmaparea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(9,1);addstr(a.name, gamelog);addstr(" says, ", gamelog);
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   move(10,1);
   addstr("\"I'd like to rent a room.\"", gamelog);
   gamelog.newline();

   getkey();
   bool is_naked = a.is_naked() && a.animalgloss != ANIMALGLOSS_ANIMAL;
   if(is_naked)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      move(13,1);
      addstr("\"Put some clothes on before I call the cops.\"", gamelog);
      gamelog.newline();

      getkey();

      return 1;
   }

   int rent;
   switch(location[cursite]->type)
   {
   default:rent=200;break;
   case SITE_RESIDENTIAL_APARTMENT:rent=650;break;
   case SITE_RESIDENTIAL_APARTMENT_UPSCALE:rent=1500;break;
   }

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
   set_color(COLOR_CYAN,COLOR_BLACK,1);
   move(13,1);
   addstr("\"It'll be $", gamelog);
   addstr(rent, gamelog);
   addstr(" a month.", gamelog);
   gamelog.newline();
   move(14,1);
   addstr("I'll need $", gamelog);
   addstr(rent, gamelog);
   addstr(" now as a security deposit.\"", gamelog);
   gamelog.newline();

   getkey();

   clearcommandarea();clearmessagearea();clearmaparea();

   while(true)
   {
      int c='a';

      if(ledger.get_funds()<rent)set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(11,1);
      addstr("A - Accept.");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(12,1);
      addstr("B - Decline.");
      move(13,1);
      addstr("C - Threaten the landlord.");

      c=getkey();

      switch(c)
      {
      case 'a': // Accept rent deal
         if(ledger.get_funds()<rent) break;

         clearcommandarea();clearmessagearea();clearmaparea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(9,1);addstr(a.name, gamelog);addstr(" says, ", gamelog);
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(10,1);
         addstr("\"I'll take it.\"", gamelog);
         gamelog.newline();

         getkey();

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         move(13,1);
         addstr("\"Rent is due by the third of every month.", gamelog);
         gamelog.newline();
         move(14,1);
         addstr("We'll start next month.\"", gamelog);
         gamelog.newline();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         addstr(" <turns away>");

         getkey();

         ledger.subtract_funds(rent,EXPENSE_RENT);
         location[cursite]->renting=rent;
         location[cursite]->newrental=1;

         basesquad(activesquad,cursite);
         return 1;

      case 'b': // Refuse rent deal
         clearcommandarea();clearmessagearea();clearmaparea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(9,1);addstr(a.name, gamelog);addstr(" says, ", gamelog);
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(10,1);
         addstr("\"Whoa, I was looking for something cheaper.\"", gamelog);
         gamelog.newline();

         getkey();

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         move(13,1);
         addstr("\"Not my problem...\"", gamelog);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         addstr(" <turns away>", gamelog);
         gamelog.newline();

         getkey();

         return 1;

      case 'c': // Threaten landlord
         clearcommandarea();clearmessagearea();clearmaparea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         Creature *armed_liberal=NULL;
         for(int i=0;i<6;i++)
         {
            if(activesquad->squad[i] &&
               activesquad->squad[i]->get_weapon().is_threatening())
            {
               armed_liberal=activesquad->squad[i];
               break;
            }
         }
         if(armed_liberal)
         {
            move(9,1);
            addstr(armed_liberal->name, gamelog);
            addstr(" brandishes the ", gamelog);
            addstr(armed_liberal->get_weapon().get_shortname(0), gamelog);
            addstr(".", gamelog);
            gamelog.newline();

            getkey();
            clearmessagearea();
         }
         move(9,1);
         addstr(a.name, gamelog);
         addstr(" says, ", gamelog);
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(10,1);
         addstr("\"What's the price for the Liberal Crime Squad?\"", gamelog);
         gamelog.newline();

         getkey();

         int roll       = a.skill_roll(SKILL_PERSUASION);
         int difficulty = DIFFICULTY_FORMIDABLE;

         if(newscherrybusted == false)
            difficulty += 6;
         if(armed_liberal == NULL)
            difficulty += 6;

         if(roll < difficulty - 1)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            move(13,1);
            addstr("\"I think you'd better leave.\"", gamelog);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" <crosses arms>", gamelog);
            gamelog.newline();

            getkey();

            tk.cantbluff = 1;
            return 1;
         }
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            move(13,1);
            addstr("\"Jesus... it's yours...\"", gamelog);
            gamelog.newline();

            getkey();

            int rent;

            // Either he calls the cops...
            if(roll<difficulty)
            {
               for(int i=0;i<6;i++)
                  if(activesquad->squad[i])
                     criminalize(*(activesquad->squad[i]),LAWFLAG_EXTORTION);
               location[cursite]->siege.timeuntillocated = 2;
               rent=10000000; // Yeah he's kicking you out next month
            }
            // ...or it's yours for free
            else rent=0;

            location[cursite]->renting=rent;
            location[cursite]->newrental=true;

            basesquad(activesquad,cursite);
            return 1;
         }
      }
   }
}

char heyINeedAGun(Creature &a, Creature &tk)
{
   clearcommandarea();clearmessagearea();clearmaparea();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(9,1);addstr(a.name, gamelog);addstr(" says, ", gamelog);
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   move(10,1);
   addstr("\"Hey, I need a gun.\"", gamelog);
   gamelog.newline();

   getkey();
   bool is_naked = a.is_naked() && a.animalgloss != ANIMALGLOSS_ANIMAL;
   if(is_naked)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      move(13,1);
      addstr("\"Jesus...\"", gamelog);
      gamelog.newline();

      getkey();

      return 1;
   }
   if(a.get_armor().get_itemtypename() == "ARMOR_POLICEUNIFORM" ||
      a.get_armor().get_itemtypename() == "ARMOR_POLICEARMOR" ||
      a.get_armor().get_itemtypename() == "ARMOR_SWATARMOR" ||
      (law[LAW_POLICEBEHAVIOR]==-2 && law[LAW_DEATHPENALTY]==-2 &&
      a.get_armor().get_itemtypename() == "ARMOR_DEATHSQUADUNIFORM"))
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      move(13,1);
      addstr("\"I don't sell guns, officer.\"", gamelog);
      gamelog.newline();

      getkey();

      return 1;
   }
   if(sitealarm!=0)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      move(13,1);
      addstr("\"We can talk when things are calm.\"", gamelog);
      gamelog.newline();

      getkey();

      return 1;
   }
   switch(location[cursite]->type)
   {
   case SITE_OUTDOOR_BUNKER:
   case SITE_BUSINESS_CRACKHOUSE:
   case SITE_BUSINESS_BARANDGRILL:
   case SITE_BUSINESS_ARMSDEALER:
   case SITE_RESIDENTIAL_TENEMENT:
   case SITE_RESIDENTIAL_BOMBSHELTER:
   case SITE_RESIDENTIAL_SHELTER:
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      move(13,1);
      addstr("\"What exactly do you need?\"", gamelog);
      gamelog.newline();

      getkey();

      armsdealer(cursite);
      return 1;
   default:
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      move(13,1);
      addstr("\"Uhhh... not a good place for this.\"", gamelog);
      gamelog.newline();

      getkey();

      return 1;
   }
}

char wannaHearSomethingDisturbing(Creature &a, Creature &tk)
{
   clearcommandarea();clearmessagearea();clearmaparea();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(9,1);addstr(a.name, gamelog);addstr(" says, ", gamelog);

   set_color(COLOR_GREEN,COLOR_BLACK,1);
   move(10,1);
   addstr("\"Do you want to hear something disturbing?\"", gamelog);
   gamelog.newline();

   getkey();

   bool interested = tk.talkreceptive();

   if(!interested && a.skill_check(SKILL_PERSUASION,DIFFICULTY_AVERAGE))
      interested = true;

   if((tk.animalgloss==ANIMALGLOSS_ANIMAL&&tk.align!=ALIGN_LIBERAL)||
      tk.animalgloss==ANIMALGLOSS_TANK)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);

      switch(tk.type)
      {
      case CREATURE_TANK: addstr(" rumbles disinterestedly.", gamelog); break;
      case CREATURE_GUARDDOG: addstr(" barks.", gamelog); break;
      default: addstr(" doesn't understand.", gamelog); break;
      }
      gamelog.newline();

      getkey();

      return 1;
   }
   else if(strcmp(tk.name,"Prisoner")!=0 && interested)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      move(13,1);
      addstr("\"What?\"", gamelog);
      gamelog.newline();

      getkey();

      return talkAboutIssues(a, tk);
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      move(13,1);
      if(strcmp(tk.name,"Prisoner")==0)
      {
         if(tk.align==ALIGN_LIBERAL)
            addstr("\"Now's not the time!\"", gamelog);
         else addstr("\"Leave me alone.\"", gamelog);
      }
      else addstr("\"No.\"", gamelog);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr(" <turns away>", gamelog);
      gamelog.newline();

      getkey();

      return 1;
   }
}

char doYouComeHereOften(Creature &a, Creature &tk)
{
   int y=12;
   clearcommandarea();clearmessagearea();clearmaparea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(9,1);addstr(a.name, gamelog);addstr(" says, ", gamelog);
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   move(10,1);
   vector<string> selected_flirt;
   int line;
   if(law[LAW_FREESPEECH]==-2)
   {
	   selected_flirt = pickrandom(no_free_speech_flirt);

   }
   else
   {
	   selected_flirt = pickrandom(pickupLines);
	   /*int current_flirt = LCSrandom(pickupLines.size);
	   for(int i = 0; i < 5; i++){
		   selected_flirt.push_back(pickupLines[current_flirt][i]);
	   }*/
   }
   addstr(selected_flirt[0], gamelog);
   if(selected_flirt[1] != ""){
			move(11,1);y++;
			addstr(selected_flirt[1],gamelog);
   }
   gamelog.newline();

   getkey();

   bool succeeded = false;

   int difficulty = DIFFICULTY_HARD;

   if(tk.type == CREATURE_CORPORATE_CEO)
      difficulty = DIFFICULTY_HEROIC;

   bool is_naked = a.is_naked() && a.animalgloss != ANIMALGLOSS_ANIMAL;
   if(is_naked) difficulty-=4;

   if(a.skill_check(SKILL_SEDUCTION,difficulty))
      succeeded = true;

   if((tk.animalgloss==ANIMALGLOSS_ANIMAL&&law[LAW_ANIMALRESEARCH]!=2&&a.animalgloss!=ANIMALGLOSS_ANIMAL)||
      tk.animalgloss==ANIMALGLOSS_TANK)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(y++,1);addstr(tk.name, gamelog);

      switch(tk.type)
      {
      case CREATURE_TANK:
         addstr(" shakes its turret a firm 'no'.", gamelog);
         break;
      case CREATURE_GUARDDOG:
         addstr(" says, ", gamelog);
         move(y,1);
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr(pickrandom(dog_rejection), gamelog);
         tk.align=ALIGN_CONSERVATIVE;
         tk.cantbluff=1;
         break;
      case CREATURE_GENETIC:
         addstr(" says, ", gamelog);
         move(y,1);
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr(pickrandom(mutant_rejection), gamelog);
         tk.align=ALIGN_CONSERVATIVE;
         tk.cantbluff=1;
         break;
      default:
         addstr(" doesn't quite pick up on the subtext.", gamelog);
      }
      gamelog.newline();

      getkey();

      return 1;
   }

   a.train(SKILL_SEDUCTION,LCSrandom(5)+2);

   if((a.get_armor().get_itemtypename() == "ARMOR_POLICEUNIFORM" // Police property on armor? -XML
      || a.get_armor().get_itemtypename() == "ARMOR_POLICEARMOR"
      || a.get_armor().get_itemtypename() == "ARMOR_SWATARMOR"
      || (law[LAW_POLICEBEHAVIOR]== ALIGN_ARCHCONSERVATIVE && law[LAW_DEATHPENALTY]== ALIGN_ARCHCONSERVATIVE
      && a.get_armor().get_itemtypename() == "ARMOR_DEATHSQUADUNIFORM"))
      && tk.type==CREATURE_PROSTITUTE)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(y++,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_RED,COLOR_BLACK,1);
      move(y++,1);

      addstr("\"Dirty. You know that's illegal, officer.\"", gamelog);
      gamelog.newline();

      getkey();

      tk.cantbluff=1;
   }
   else if(succeeded)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(y++,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      move(y++,1);

	  addstr(selected_flirt[2], gamelog);

      gamelog.newline();

      getkey();

      move(++y,1);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(y++,1);
      addstr(a.name, gamelog);
      addstr(" and ", gamelog);
      addstr(tk.name, gamelog);
      addstr(" make plans for tonight", gamelog);
      if(strcmp(tk.name,"Prisoner")==0)
      {
         addstr(", and ", gamelog);
         move(y++,1);
         addstr(tk.name, gamelog);
         addstr(" breaks for the exit", gamelog);
         criminalize(tk,LAWFLAG_ESCAPED);
      }
      addstr(".  ", gamelog);
      gamelog.newline();

      getkey();

      datest *newd=NULL;
      for(int d=0;d<len(date);d++)
      {
         if(date[d]->mac_id==a.id)
         {
            newd=date[d];
            break;
         }
      }

      if(newd==NULL)
      {
         newd=new datest;
            newd->mac_id=a.id;
            newd->city=location[a.location]->city;

         date.push_back(newd);
      }

      Creature *newcr=new Creature;
      *newcr=tk;
      newcr->namecreature();

      newcr->location=a.location;
      newcr->base=a.base;

      newd->date.push_back(newcr);

      delenc(&tk-encounter,0);
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(y++,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_RED,COLOR_BLACK,1);
      move(y++,1);
      if(tk.type == CREATURE_CORPORATE_CEO)
      {
         if(a.gender_liberal != GENDER_MALE)
            addstr("\"I'm a happily married man, sweetie.\"", gamelog);
         else addstr("\"This ain't Brokeback Mountain, son.\"", gamelog);
      }
      else 
      {
		 addstr(selected_flirt[3], gamelog);
		 if(selected_flirt[4] != ""){
			 set_color(COLOR_WHITE,COLOR_BLACK,1);
			addstr(selected_flirt[4], gamelog);
		 }
      }

      gamelog.newline();

      getkey();

      tk.cantbluff=1;
   }

   return 1;
}
char talkAboutIssues(Creature &a, Creature &tk)
{
   int lw = LCSrandom(LAWNUM); // pick a random law to talk about

   bool succeeded         = false;
   bool you_are_stupid    = false;
   bool issue_too_liberal = false;

   if(!(a.attribute_check(ATTRIBUTE_INTELLIGENCE,DIFFICULTY_EASY)))
      you_are_stupid = true;
   else if(law[lw]==ALIGN_ELITELIBERAL && newscherrybusted)
      issue_too_liberal = true;

   clearcommandarea();clearmessagearea();clearmaparea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(9,1);addstr(a.name, gamelog);addstr(" says, ", gamelog);
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   int y=10;
   move(y++,1);
   if(you_are_stupid)
   {
      switch(lw)
      {
      case LAW_ABORTION:      addstr("\"Conservatives make women turn to coat hangers.\"", gamelog);break;
      case LAW_ANIMALRESEARCH:addstr("\"Richard Gere put a gerbil in his butt!\"", gamelog);break;
      case LAW_POLICEBEHAVIOR:
                              if(law[LAW_FREESPEECH]==ALIGN_ARCHCONSERVATIVE)
                                  addstr("\"[The police are not doing their job very well!]\"", gamelog);
                              else
                                  addstr("\"The cops suck!\"", gamelog);
                              break;
      case LAW_PRIVACY:       addstr("\"The government, like, knows things about you.\"", gamelog);break;
      case LAW_DEATHPENALTY:  addstr("\"They executed this one dude, and like, his head caught on fire.\"", gamelog);break;
      case LAW_NUCLEARPOWER:  addstr("\"Have you seen Godzilla?  Nuclear power is bad, yo.\"", gamelog);break;
      case LAW_POLLUTION:     addstr("\"You wanna look like the Toxic Avenger?  Oppose pollution!\"", gamelog);break;
      case LAW_LABOR:         addstr("\"Bad people wanna make babies work and stuff.\"", gamelog);break;
      case LAW_GAY:           addstr("\"Lots of people don't like homosexuals.\"", gamelog);break;
      case LAW_CORPORATE:     addstr("\"The corporations are putting you down, dude.\"", gamelog);break;
      case LAW_FREESPEECH:    addstr("\"Better watch what you say.  They've got ears everywhere.\"", gamelog);break;
      case LAW_FLAGBURNING:   addstr("\"The flag is stupid.\"", gamelog);break;
      case LAW_TAX:           addstr("\"Rich people, like, have money, man.\"", gamelog);break;
      case LAW_GUNCONTROL:    addstr("\"People like, think they need lots of guns.\"", gamelog);break;
      case LAW_WOMEN:         addstr("\"We need more women!\"", gamelog);break;
      case LAW_CIVILRIGHTS:   addstr("\"Conservatives are all racists!\"", gamelog);break;
      case LAW_DRUGS:         addstr("\"Dude, the government won't let you do drugs.\"", gamelog);break;
      case LAW_IMMIGRATION:   addstr("\"They're all trying to keep people out of the country.\"", gamelog);break;
      case LAW_ELECTIONS:     addstr("\"The politicians are just tools of the corporations!\"", gamelog);break;
      case LAW_MILITARY:      addstr("\"Patriots are idiots! Give peace a chance!\"", gamelog);break;
      case LAW_TORTURE:       addstr("\"Torture is bad!\"", gamelog);break;
      case LAW_PRISONS:       addstr("\"Prisoners don't have freedom!\"", gamelog);break;
      }
   }
   else if(issue_too_liberal)
   {
      switch(lw)
      {
      case LAW_ABORTION:      addstr("\"Conservatives don't like abortion.\"", gamelog);break;
      case LAW_ANIMALRESEARCH:addstr("\"Animals are denied the right to vote.\"", gamelog);break;
      case LAW_POLICEBEHAVIOR:addstr("\"The police are still out there.\"", gamelog);break;
      case LAW_PRIVACY:       addstr("\"The government runs intelligence agencies.\"", gamelog);break;
      case LAW_DEATHPENALTY:  addstr("\"You can go to prison for life for serious crimes.\"", gamelog);break;
      case LAW_NUCLEARPOWER:  addstr("\"Some people support legalizing nuclear power.\"", gamelog);break;
      case LAW_POLLUTION:     addstr("\"We're still polluting a little bit.\"", gamelog);break;
      case LAW_LABOR:         addstr("\"Corporate bosses don't always give in to unions.\"", gamelog);break;
      case LAW_GAY:           addstr("\"Not everybody likes gay people.\"", gamelog);break;
      case LAW_CORPORATE:     addstr("\"There are corporations.\"", gamelog);break;
      case LAW_FREESPEECH:    addstr("\"People get mad if you swear a lot in public.\"", gamelog);break;
      case LAW_FLAGBURNING:   addstr("\"The flag code says you shouldn't make it into clothing.\"", gamelog);break;
      case LAW_TAX:           addstr("\"There's still inequality in this country.\"", gamelog);break;
      case LAW_GUNCONTROL:    addstr("\"We need to repeal the second amendment.\"", gamelog);break;
      case LAW_WOMEN:         addstr("\"Some people are sexist.\"", gamelog);break;
      case LAW_CIVILRIGHTS:   addstr("\"I knew some people that were pretty racist.\"", gamelog);break;
      case LAW_DRUGS:         addstr("\"Drugs are expensive.\"", gamelog);break;
      case LAW_IMMIGRATION:   addstr("\"All the immigrants, not everybody likes them.\"", gamelog);break;
      case LAW_ELECTIONS:     addstr("\"Some of these politicians rub me the wrong way.\"", gamelog);break;
      case LAW_MILITARY:      addstr("\"We still have a military.\"", gamelog);break;
      case LAW_TORTURE:       addstr("\"Some conservatives support torture.\"", gamelog);break;
      case LAW_PRISONS:       addstr("\"Prisons still exist.\"", gamelog);break;
      }
   }
   else
   {
      switch(lw)
      {
      case LAW_ABORTION:
         addstr("\"The government is systematically trying to rob women of the right ", gamelog);move(y++,1);
         addstr("to control their own destinies.\"", gamelog);
         break;
      case LAW_ANIMALRESEARCH:
         addstr("\"Animals are routinely subjected to inhumane treatment in labs in this", gamelog);move(y++,1);
         addstr("country.\"", gamelog);
         break;
      case LAW_POLICEBEHAVIOR:
         addstr("\"The police regularly torture minority suspects during interrogations.\"", gamelog);
         break;
      case LAW_PRIVACY:
         addstr("\"Files are being kept on innocent citizens whose only crime is to ", gamelog);move(y++,1);
         addstr("speak out against a system that is trying to farm them like beasts.\"", gamelog);
         break;
      case LAW_DEATHPENALTY:
         addstr("\"Over thirty innocent people have been executed over the past decade.\"", gamelog);
         break;
      case LAW_NUCLEARPOWER:
         addstr("\"Radioactive waste is being stored all over the country, and it poses ", gamelog);move(y++,1);
         addstr("a serious threat to many families, even in this neighborhood.\"", gamelog);
         break;
      case LAW_POLLUTION:
         addstr("\"Industries that stop at nothing to become more profitable are polluting ", gamelog);move(y++,1);
         if (tk.animalgloss==ANIMALGLOSS_ANIMAL) addstr("the environment in ways that hurt not only humans, but animals too.", gamelog);
         else addstr("the environment in ways that hurt not only animals, but people too.", gamelog);
         break;
      case LAW_LABOR:
         addstr("\"Have you noticed how people are working more and more hours for less and ", gamelog);move(y++,1);
         addstr("less money?  It's all part of a plan to keep you enslaved, man.\"", gamelog);
         break;
      case LAW_GAY:
         addstr("\"Homosexuals are people like anyone else, and yet they are treated in this ", gamelog);move(y++,1);
         addstr("country as if they are deviants fit only for cheap entertainment.\"", gamelog);
         break;
      case LAW_CORPORATE:
         addstr("\"Corporate executives use giant corporations as a means to become parasites ", gamelog);move(y++,1);
         addstr("that suck wealth out of this country and put it into their pockets.\"", gamelog);
         break;
      case LAW_FREESPEECH:
         addstr("\"Protests and demonstrations are regularly and often brutally suppressed in ", gamelog);move(y++,1);
         addstr("this country.  People have to watch what they write -- even what they read.\"", gamelog);
         break;
      case LAW_FLAGBURNING:
         addstr("\"Burning a piece of cloth is actually stigmatized in this country. ", gamelog);move(y++,1);
         addstr("You can love freedom and still hate what our government stands for.\"", gamelog);
         break;
      case LAW_TAX:
         addstr("\"The tax code has been designed to perpetuate an unjust class ", gamelog);move(y++,1);
         addstr("structure that is keeping you oppressed.\"", gamelog);
         break;
      case LAW_GUNCONTROL:
         addstr("\"We live in such a backwards country right now that people think it's ", gamelog);move(y++,1);
         addstr("a right to walk around with the power to murder at any moment.\"", gamelog);
         break;
      case LAW_WOMEN:
         addstr("\"Sexism is still pervasive, in subtle ways, and women make much less ", gamelog);move(y++,1);
         addstr("than they deserve for their labor.\"", gamelog);
         break;
      case LAW_CIVILRIGHTS:
         addstr("\"Despite our progress, this society is still strangled by its continuing ", gamelog);move(y++,1);
         addstr("legacy of racial discrimination and inequality.\"", gamelog);
         break;
      case LAW_DRUGS:
         addstr("\"The government's drug policy is a mess.  We need to stop filling ", gamelog);move(y++,1);
         addstr("prisons with drug users, and only intervene when people really need help.\"", gamelog);
         break;
      case LAW_IMMIGRATION:
         addstr("\"Millions of people are doing jobs most folks don't even want, and ", gamelog);move(y++,1);
         addstr("saving their families from poverty, but we just try to kick them out.\"", gamelog);
         break;
      case LAW_ELECTIONS:
         addstr("\"Political favors are bought and sold for campaign contributions, ", gamelog);move(y++,1);
         addstr("and the voting system enforces two party dominance.\"", gamelog);
         break;
      case LAW_MILITARY:
         addstr("\"Take a breath and think about the world we live in, that we're spending ", gamelog);move(y++,1);
         addstr("hundreds of billions on new ways to kill people.  This has to stop!\"", gamelog);
         break;
      case LAW_TORTURE:
         addstr("\"In the name of the war on terror, we've sacrificed our soul by letting ", gamelog);move(y++,1);
         addstr("the government torture and abuse human beings on our behalf.\"", gamelog);
         break;
      case LAW_PRISONS:
         addstr("\"The prison system doesn't help criminals by providing rehabilitation, so ", gamelog);move(y++,1);
         addstr("when they get released, they mostly become criminals again.\"", gamelog);
         break;
      }
   }
   gamelog.newline();

   getkey();

   int difficulty = DIFFICULTY_VERYEASY;

   if(tk.align == ALIGN_CONSERVATIVE)
      difficulty += 7;
   if(!(tk.talkreceptive()))
      difficulty += 7;
   if(you_are_stupid)
      difficulty += 5;
   if(issue_too_liberal)
      difficulty += 5;
   bool is_naked = a.is_naked() && a.animalgloss != ANIMALGLOSS_ANIMAL;
   if(is_naked)
      difficulty += 5;

   succeeded = a.skill_check(SKILL_PERSUASION,difficulty);

   // Prisoners never accept to join you, you must liberate them instead
   if(succeeded && strcmp(tk.name,"Prisoner")!=0)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move((++y)++,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      move(y++,1);
      if(tk.type==CREATURE_MUTANT&&tk.get_attribute(ATTRIBUTE_INTELLIGENCE,true)<3)
         addstr("\"Aaaahhh...\"", gamelog);
      else
      {
         switch(LCSrandom(10))
         {         
		 case 0: addstr("\"Oh, really?\"", gamelog);

                 getkey();

                 move(y++,1);
                 set_color(COLOR_GREEN,COLOR_BLACK,1);
                 addstr("\"Yeah, really!\"", gamelog);
                 break;
		 case 1: addstr("\"You got anything to smoke on you?\"", gamelog);
                 set_color(COLOR_WHITE,COLOR_BLACK,1);
                 addstr("*cough*", gamelog);
                 break;
		 default:
			 addstr(pickrandom(that_is_disturbing), gamelog);
			 break;
         }
      }
      gamelog.newline();

      getkey();

      move(++y,1);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("After more discussion, ", gamelog);
      addstr(tk.name, gamelog);
      addstr(" agrees to come by later tonight.", gamelog);
      gamelog.newline();

      getkey();

      Creature *newcr=new Creature;
      *newcr=tk;
      newcr->namecreature();

      recruitst *newrst=new recruitst;
      newrst->recruit=newcr;
      newrst->recruiter_id = a.id;

      recruit.push_back(newrst);

      delenc(&tk-encounter,0);
      return 1;
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move((++y)++,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      if(tk.type==CREATURE_MUTANT&&
         tk.get_attribute(ATTRIBUTE_INTELLIGENCE,true)<3)
      {
         move(y++,1);
         addstr("\"Ugh.  Pfft.\"", gamelog);
      }
      else
      {
         if(tk.align==ALIGN_CONSERVATIVE && you_are_stupid)
         {
            move(y++,1);
            if(tk.type==CREATURE_GANGUNIT)
               addstr("\"Do you want me to arrest you?\"", gamelog);
            else if(tk.type==CREATURE_DEATHSQUAD)
               addstr("\"If you don't shut up, I'm going to shoot you.\"", gamelog);
            else
            {
               addstr(pickrandom(that_is_not_disturbing), gamelog);
            }
         }
         else if(tk.align != ALIGN_LIBERAL && tk.attribute_check(ATTRIBUTE_WISDOM,DIFFICULTY_AVERAGE))
         {
            move(y++,1);
            switch(lw)
            {
            case LAW_ABORTION:      addstr("\"Abortion is murder.\"", gamelog);break;
            case LAW_ANIMALRESEARCH:addstr("\"Don't pretend animals are human.\"", gamelog);break;
            case LAW_POLICEBEHAVIOR:addstr("\"Only criminals have reason to fear police.\"", gamelog);break;
            case LAW_PRIVACY:       addstr("\"National security is important.\"", gamelog);break;
            case LAW_DEATHPENALTY:  addstr("\"Some people deserve to die.\"", gamelog);break;
            case LAW_NUCLEARPOWER:  addstr("\"Nuclear power is clean.\"", gamelog);break;
            case LAW_POLLUTION:     addstr("\"It's not that bad.\"", gamelog);break;
            case LAW_LABOR:         addstr("\"Trust the free market, it hasn't failed us yet.\"", gamelog);break;
            case LAW_GAY:           addstr("\"Homosexuality is a sin.\"", gamelog);break;
            case LAW_CORPORATE:     addstr("\"Corporations are part of capitalism.\"", gamelog);break;
            case LAW_FREESPEECH:    addstr("\"Don't be offensive and you'll be fine.\"", gamelog);break;
            case LAW_FLAGBURNING:   addstr("\"That flag is the sacred symbol of our country.\"", gamelog);break;
            case LAW_TAX:           addstr("\"I want to pay lower taxes.\"", gamelog);break;
            case LAW_GUNCONTROL:    addstr("\"Without guns, we're slaves to the Government.\"", gamelog);break;
            case LAW_WOMEN:         addstr("\"Why don't you go burn a bra or something?\"", gamelog);break;
            case LAW_CIVILRIGHTS:   addstr("\"Reverse discrimination is still discrimination.\"", gamelog);break;
            case LAW_DRUGS:         addstr("\"Drugs are a terrible influence on society.\"", gamelog);break;
            case LAW_IMMIGRATION:   addstr("\"Immigration undermines our economy and culture.\"", gamelog);break;
            case LAW_ELECTIONS:     addstr("\"Unregulated campaigning is a matter of free speech.\"", gamelog);break;
            case LAW_MILITARY:      addstr("\"The military protects us and enables our way of life.\"", gamelog);break;
            case LAW_TORTURE:       addstr("\"The terrorists would do worse to us.\"", gamelog);break;
            case LAW_PRISONS:       addstr("\"Criminals deserve what they get in prison.\"", gamelog);break;
            }
         }
         else
         {
            move(y++,1);
            addstr("\"Whatever.\"", gamelog);
         }
      }
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr(" <turns away>", gamelog);
      gamelog.newline();

      getkey();

      tk.cantbluff=1;
      return 1;
   }
   return 0;
}

char talkInCombat(Creature &a, Creature &tk)
{
   clearcommandarea();
   clearmessagearea();
   clearmaparea();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(9,1);
   addstr(a.name, gamelog);
   addstr(" talks to ", gamelog);
   switch(tk.align)
   {
   case ALIGN_CONSERVATIVE: set_color(COLOR_RED,COLOR_BLACK,1); break;
   case ALIGN_LIBERAL: set_color(COLOR_GREEN,COLOR_BLACK,1); break;
   case ALIGN_MODERATE: set_color(COLOR_WHITE,COLOR_BLACK,1); break;
   }
   addstr(tk.name, gamelog);
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   addstr(":", gamelog);
   gamelog.newline();

   int c=0,hostages=0,weaponhostage=0;
   bool cop=0;

   for(int i=0;i<6;i++)
   {
      if(activesquad->squad[i]&&
         activesquad->squad[i]->prisoner&&
         activesquad->squad[i]->prisoner->alive&&
         activesquad->squad[i]->prisoner->enemy())
      {
         hostages++;
         if (activesquad->squad[i]->get_weapon().can_threaten_hostages())
           weaponhostage++;
      }
   }
   if(tk.type==CREATURE_COP ||
      tk.type==CREATURE_GANGUNIT ||
      tk.type==CREATURE_DEATHSQUAD ||
      tk.type==CREATURE_SWAT ||
      tk.type==CREATURE_SECURITYGUARD ||
      tk.type==CREATURE_MERC ||
      tk.type==CREATURE_SOLDIER ||
      tk.type==CREATURE_MILITARYPOLICE ||
      tk.type==CREATURE_MILITARYOFFICER ||
      tk.type==CREATURE_SEAL)
   {
      cop=1;
   }

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(11,1);
   addstr("A - Intimidate");
   if(!hostages)set_color(COLOR_BLACK,COLOR_BLACK,1);
   move(12,1);
   addstr("B - Threaten hostages");
   if(tk.cantbluff!=2)set_color(COLOR_WHITE,COLOR_BLACK,0);
   else set_color(COLOR_BLACK,COLOR_BLACK,1);
   move(13,1);
   addstr("C - Bluff");
   if(cop)set_color(COLOR_WHITE,COLOR_BLACK,0);
   else set_color(COLOR_BLACK,COLOR_BLACK,1);
   move(14,1);
   addstr("D - Surrender to authorities");
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   while(true)
   {
      c=getkey();

      if(c=='a')break;
      if(c=='b' && hostages)break;
      if(c=='c' && tk.cantbluff!=2)break;
      if(c=='d' && cop)break;
   }

   if(c=='a')
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr(a.name, gamelog);
      addstr(": ", gamelog);
      move(17,1);
      set_color(COLOR_GREEN,COLOR_BLACK,1);

      switch(LCSrandom(4))
      {
      case 0:
         {   // Formatting the slogan so that it always has quotes around it and punctuation
            if(slogan[0]!='"') addchar('"',gamelog);
            addstr(slogan,gamelog);
            int last=len(slogan);
            if(last && slogan[last-1]!='"' && slogan[last-1]!='!' && slogan[last-1]!='.' && slogan[last-1]!='?')
               addchar('!',gamelog);
            if(last && slogan[last-1]!='"') addchar('"',gamelog);

            if(!sitestory->claimed)
               sitestory->claimed=1;
            break;
         }
	  default:
		  addstr(pickrandom(come_at_me_bro), gamelog);
		  break;
      }

      getkey();

      set_color(COLOR_WHITE,COLOR_BLACK,1);

      for(int e=0;e<ENCMAX;e++)
      {
         if(encounter[e].exists&&encounter[e].alive&&encounter[e].enemy())
         {
            int attack = a.juice / 50 + attitude[VIEW_LIBERALCRIMESQUAD] / 10;
            int defense = encounter[e].attribute_roll(ATTRIBUTE_WISDOM);

            if(attack > defense)
            {
               if(encounter[e].type==CREATURE_COP||
                  encounter[e].type==CREATURE_GANGUNIT||
                  encounter[e].type==CREATURE_SWAT||
                  encounter[e].type==CREATURE_DEATHSQUAD||
                  encounter[e].type==CREATURE_SOLDIER||
                  encounter[e].type==CREATURE_HARDENED_VETERAN||
                  encounter[e].type==CREATURE_CCS_ARCHCONSERVATIVE||
                  encounter[e].type==CREATURE_AGENT||
                  encounter[e].type==CREATURE_SECRET_SERVICE)
               {
                  if(LCSrandom(3)) continue;
               }
               clearmessagearea();
               move(16,1);
               addstr(encounter[e].name, gamelog);
               addstr(" " + pickrandom(backs_off), gamelog);
               delenc(e,0);
               addjuice(a,2,200); // Instant juice!

               getkey();
            }
         }
      }
   }
   else if(c=='b')
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      addstr(a.name, gamelog);
      addstr(": ", gamelog);
      set_color(COLOR_GREEN,COLOR_BLACK,1);
      move(17,1);
      switch(LCSrandom(6))
      {
      case 0:addstr("\"Don't push the LCS!\"", gamelog);
         if(!sitestory->claimed)sitestory->claimed=1;break;
      case 1:
             if(law[LAW_FREESPEECH]==-2)addstr("\"Don't [play] with me!\"", gamelog);
             else addstr("\"Don't fuck with me!\"", gamelog);
             break;
	  default:
		  addstr(pickrandom(threaten_hostage), gamelog);
		  break;
      }
      gamelog.newline();

      sitecrime+=5;
      criminalizeparty(LAWFLAG_KIDNAPPING);

      addjuice(a,-2,-10); // DE-juice for this shit

      getkey();

      bool noretreat=false;

      if(weaponhostage)
      {
         int e;
         for(e=0;e<ENCMAX;e++)
         {
            if(encounter[e].exists&&encounter[e].alive&&
               encounter[e].enemy()&&encounter[e].blood>70)
            {
               if((encounter[e].type==CREATURE_DEATHSQUAD||
                  encounter[e].type==CREATURE_SOLDIER||
                  encounter[e].type==CREATURE_HARDENED_VETERAN||
                  encounter[e].type==CREATURE_CCS_ARCHCONSERVATIVE||
                  encounter[e].type==CREATURE_AGENT||
                  encounter[e].type==CREATURE_MERC||
                  encounter[e].type==CREATURE_COP||
                  encounter[e].type==CREATURE_GANGUNIT||
                  encounter[e].type==CREATURE_SWAT||
                  encounter[e].type==CREATURE_SECRET_SERVICE)&&
                  LCSrandom(5))
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  clearmessagearea();
                  move(16,1);
                  addstr(encounter[e].name, gamelog);
                  addstr(": ", gamelog);
                  move(17,1);

                  if(encounter[e].align!=ALIGN_CONSERVATIVE||
                     (encounter[e].type==CREATURE_SECRET_SERVICE&&exec[EXEC_PRESIDENT]>ALIGN_CONSERVATIVE))
                  {
                     set_color(COLOR_GREEN,COLOR_BLACK,1);
                     addstr(pickrandom(please_spare_hostage), gamelog);
                  }
                  else
                  {
                     set_color(COLOR_RED,COLOR_BLACK,1);
                     if(((encounter[e].type==CREATURE_DEATHSQUAD||
                        encounter[e].type==CREATURE_AGENT||
                        encounter[e].type==CREATURE_MERC||
                        encounter[e].type==CREATURE_CCS_ARCHCONSERVATIVE||
                        encounter[e].type==CREATURE_GANGUNIT))
                        &&encounter[e].align==ALIGN_CONSERVATIVE)
                     {
                        addstr(pickrandom(who_cares_about_hostage), gamelog);
                     }
                     else
                     {
                        switch(LCSrandom(5))
                        {
                        case 0:
                           if(hostages>1)
                              addstr("\"Release your hostages, and nobody gets hurt.\"", gamelog);
                           else addstr("\"Let the hostage go, and nobody gets hurt.\"", gamelog);
                           break;
						default:
							addstr(pickrandom(hostage_negotiation), gamelog);
							break;
                        }
                     }

                  }
                  gamelog.newline();

                  getkey();

                  noretreat=true;
                  break;
               }
            }
         }
         if(noretreat==false)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            clearmessagearea();
            move(16,1);
            addstr("The ploy works! The Conservatives back off.", gamelog);
            gamelog.newline();
            for(int i=ENCMAX;i>=0;i--)
            {
               if(encounter[i].exists&&
                  encounter[i].alive&&
                  encounter[i].align<=-1)
               {
                  delenc(i,0);
               }
            }

            getkey();
         }
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            clearcommandarea();
            clearmessagearea();
            clearmaparea();
            move(9,1);
            addstr("How should ");
            addstr(a.name);
            addstr(" respond?");
            move(11,1);
            if(hostages>1)
               addstr("A - Execute a hostage");
            else
               addstr("A - Execute the hostage");
            move(12,1);
            if(hostages>1)
               addstr("B - Offer to trade the hostages for freedom");
            else
               addstr("B - Offer to trade the hostage for freedom");
            move(13,1);
            addstr("C - No reply");

            while(true)
            {
               c=getkey();

               if(c=='a'||c=='b')break; // TODO: something to happen if you press 'c'
            }
            if(c=='a')
            {
               Creature* executer=0;
               if(a.prisoner)
                  executer=&a;
               else for(int i=0;i<6;i++)
               {
                  if(activesquad->squad[i] &&
                     activesquad->squad[i]->prisoner &&
                     activesquad->squad[i]->prisoner->alive &&
                     activesquad->squad[i]->prisoner->enemy())
                  {
                     executer=activesquad->squad[i];
                     break;
                  }
               }

               move(16,1);
               set_color(COLOR_RED,COLOR_BLACK,1);
               if(executer->get_weapon().is_ranged()
                  && executer->get_weapon().get_ammoamount()>0)
               {
                  addstr("BLAM!", gamelog);
                  gamelog.newline();
                  executer->get_weapon().decrease_ammo(1); //What if it doesn't use ammo? -XML
               }
               else
               {
                  addstr("CRUNCH!", gamelog);
                  gamelog.newline();
               }

               getkey();

               move(17,1);
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               addstr(executer->name, gamelog);
               addstr(" drops ", gamelog);
               addstr(executer->prisoner->name, gamelog);
               addstr("'s body.", gamelog);
               gamelog.newline();

               addjuice(*executer,-5,-50); // DE-juice for this shit
               sitecrime+=10;
               sitestory->crime.push_back(CRIME_KILLEDSOMEBODY);
               criminalize(*executer,LAWFLAG_MURDER);

               if(executer->prisoner->type==CREATURE_CORPORATE_CEO||
                  executer->prisoner->type==CREATURE_RADIOPERSONALITY||
                  executer->prisoner->type==CREATURE_NEWSANCHOR||
                  executer->prisoner->type==CREATURE_SCIENTIST_EMINENT||
                  executer->prisoner->type==CREATURE_JUDGE_CONSERVATIVE)sitecrime+=30;

               makeloot(*executer->prisoner,groundloot);

               getkey();

               delete_and_nullify(executer->prisoner);

               if(hostages>1&&LCSrandom(2))
               {
                  clearmessagearea();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(16,1);
                  addstr(encounter[e].name, gamelog);
                  addstr(": ", gamelog);
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  move(17,1);
                  if(law[LAW_FREESPEECH]>ALIGN_ARCHCONSERVATIVE)
                     addstr("\"Fuck! ", gamelog);
                  else addstr("\"[No!] ", gamelog);
				  addstr(pickrandom(please_no_more), gamelog);
                  gamelog.newline();

                  for(int i=ENCMAX;i>=0;i--)
                     if(encounter[i].exists && encounter[i].enemy() && encounter[i].alive)
                        delenc(i,0);

                  getkey();
               }
            }
            else if(c=='b')
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr(a.name, gamelog);
               addstr(": ", gamelog);
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(17,1);
               switch(LCSrandom(5))
               {
               case 0:
                  if(hostages>1) addstr("\"Back off and we'll let the hostages go.\"", gamelog);
                  else addstr("\"Back off and the hostage goes free.\"", gamelog);
                  break;
			   default:
				   addstr(pickrandom(let_hostages_go), gamelog);
				   break;
               }
               gamelog.newline();

               getkey();

               if(((encounter[e].type==CREATURE_DEATHSQUAD||
                  encounter[e].type==CREATURE_AGENT||
                  encounter[e].type==CREATURE_MERC||
                  encounter[e].type==CREATURE_CCS_ARCHCONSERVATIVE||
                  encounter[e].type==CREATURE_GANGUNIT)&&
                  LCSrandom(2))&&encounter[e].align==ALIGN_CONSERVATIVE)
               {
                  clearmessagearea();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(16,1);
                  addstr(encounter[e].name, gamelog);
                  addstr(": ", gamelog);
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  move(17,1);
                  addstr(pickrandom(go_ahead_and_die), gamelog);
                  gamelog.newline();

                  getkey();
               }
               else
               {
                  clearmessagearea();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(16,1);
                  addstr(encounter[e].name, gamelog);
                  addstr(": ", gamelog);
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  move(17,1);
                  addstr(pickrandom(agree_to_release_hostages), gamelog);
                  gamelog.newline();

                  getkey();

                  for(int i=ENCMAX;i>=0;i--)
                     if(encounter[i].exists&&encounter[i].enemy()&&encounter[i].alive)
                        delenc(i,0);
                  clearmessagearea();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(16,1);
                  juiceparty(15,200); // Instant juice for successful hostage negotiation
                  if(hostages>1)addstr("The squad releases all hostages in the trade.", gamelog);
                  else addstr("The squad releases the hostage in the trade.", gamelog);
                  gamelog.newline();
                  for(int i=0;i<6;i++)
                  {
                     if(activesquad->squad[i] &&
                        activesquad->squad[i]->prisoner &&
                        activesquad->squad[i]->prisoner->enemy())
                     {
                        delete_and_nullify(activesquad->squad[i]->prisoner);
                     }
                  }

                  getkey();
               }
            }
         }
      }
      else
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         clearmessagearea();
         move(16,1);
         addstr(tk.name, gamelog);
         addstr(" isn't interested in your pathetic threats.", gamelog);
         gamelog.newline();

         getkey();
      }
   }
   else if(c=='c')
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);
      if(location[cursite]->siege.siege)
      {
         addstr(a.name, gamelog);
         addstr(" ", gamelog);
         switch(location[cursite]->siege.siegetype)
         {
            case SIEGE_POLICE:
               addstr("pretends to be part of a police raid.", gamelog);
               break;
            case SIEGE_CIA:
               addstr("pretends to be a Secret Agent.", gamelog);
               break;
            case SIEGE_CCS:
            case SIEGE_HICKS:
               switch(LCSrandom(2))
               {
                  case 0:
                     addstr("pretends to be Mountain ", gamelog);
                     move(17,1);
                     addstr("like Patrick Swayze in Next of Kin.", gamelog);
                     break;
                  case 1:
                     addstr("squeals like Ned Beatty ", gamelog);
                     move(17,1);
                     addstr("in Deliverance.", gamelog);
                     break;
               }
               break;
            case SIEGE_CORPORATE:
               addstr("pretends to be a mercenary.", gamelog);
               break;
            case SIEGE_FIREMEN:
               addstr("lights a match and throws it on the ground. ", gamelog);
               if((!(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_END) ||
                  !(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_PEAK) ||
                  !(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_START) ||
                  !(levelmap[locx][locy][locz].flag & SITEBLOCK_DEBRIS)) && !LCSrandom(10))
               {
                  levelmap[locx][locy][locz].flag |= SITEBLOCK_FIRE_START;
                  move(17,1);
                  addstr("The carpet smolders, then bursts into flame.", gamelog);
                  gamelog.newline();
                  move(18,1);
                  addstr("Perhaps that was a bad idea...", gamelog);
               }
               break;
         }
      }
      else        //Special bluff messages for various uniforms
      {
         set_color(COLOR_GREEN, COLOR_BLACK, 1);
         if(a.get_armor().get_itemtypename() == "ARMOR_POLICEUNIFORM" ||
            a.get_armor().get_itemtypename() == "ARMOR_POLICEARMOR" ||
            a.get_armor().get_itemtypename() == "ARMOR_SWATARMOR")
         {
            addstr("\"The situation is under control.\"", gamelog);
         }
         else if (a.get_armor().get_itemtypename() == "ARMOR_BUNKERGEAR")
         {
            if(siteonfire) addstr("\"Fire! Evacuate immediately!\"", gamelog);
            else addstr("\"Everything's in check.\"", gamelog);
         }

         else if(a.get_armor().get_itemtypename() == "ARMOR_LABCOAT")
            addstr("\"Make way, I'm a doctor!\"", gamelog);

         else if(a.get_armor().get_itemtypename() == "ARMOR_DEATHSQUADUNIFORM")
            addstr("\"Non-targets please leave the site.\"", gamelog);

         else if(a.get_armor().get_itemtypename() == "ARMOR_MITHRIL")
         {
            addstr(a.name, gamelog);
            addstr(" engraves ", gamelog);
            set_color(COLOR_CYAN, COLOR_BLACK, 1);
            addstr("Elbereth", gamelog);     //Fanciful multicolor message
            set_color(COLOR_GREEN, COLOR_BLACK, 1);
            addstr(" on the floor.", gamelog);
         }
         else
         {
            addstr(a.name, gamelog);
            addstr(" talks like a Conservative ", gamelog);
            move(17,1);
            addstr("and pretends to belong here.", gamelog);
         }
      }
      gamelog.newline();

      getkey();

      bool fooled=true;
      int e;

      for(e=0;e<ENCMAX;e++)
      {
         if(encounter[e].exists&&encounter[e].alive&&
            encounter[e].enemy())
         {
            int roll = a.skill_roll(SKILL_DISGUISE);
            int diff = encounter[e].get_attribute(ATTRIBUTE_WISDOM,true)>10 ? DIFFICULTY_CHALLENGING : DIFFICULTY_AVERAGE;
            fooled = roll >= diff;
            
            if (roll+1 == diff && fieldskillrate == FIELDSKILLRATE_HARD)
               a.train(SKILL_DISGUISE, 20);
            

            if(!fooled) break;
         }
      }

      switch (fieldskillrate)
      {
         case FIELDSKILLRATE_FAST:
            a.train(SKILL_DISGUISE, 50);break;
         case FIELDSKILLRATE_CLASSIC:
            a.train(SKILL_DISGUISE, 20);break;
         case FIELDSKILLRATE_HARD:
            a.train(SKILL_DISGUISE, 0);break;
      }

      if(!fooled)
      {
         clearmessagearea();

         set_color(COLOR_RED,COLOR_BLACK,1);
         move(16,1);
         if(encounter[e].type==CREATURE_HICK)
         {
            addstr("But ", gamelog);
            addstr(encounter[e].name, gamelog);
            addstr(" weren't born yesterday.", gamelog);
         }
         else
         {
            addstr(encounter[e].name, gamelog);
            if(law[LAW_FREESPEECH]==ALIGN_ARCHCONSERVATIVE)
               addstr(" is not fooled by that [act].", gamelog);
            else addstr(" is not fooled by that crap.", gamelog);
         }

         getkey();
      }
      else
      {
         clearmessagearea();

         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(16,1);
         addstr("The Enemy is fooled and departs.", gamelog);

         getkey();

         for(int e=ENCMAX-1;e>=0;e--)
            if(encounter[e].exists&&encounter[e].alive&&encounter[e].enemy())
               delenc(e,0);
      }
      gamelog.newline();
   }
   else
   {
      move(14,1);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("The Squad is arrested.", gamelog);
      gamelog.newline();

      getkey();

      int stolen=0;
      // Police assess stolen goods in inventory
      for(int l=0;l<len(activesquad->loot);l++)
         if(activesquad->loot[l]->is_loot())
            stolen++;

      for(int i=0;i<6;i++)
      {
         if(activesquad->squad[i])
         {
            activesquad->squad[i]->crimes_suspected[LAWFLAG_THEFT]+=stolen;
            capturecreature(*activesquad->squad[i]);
         }
         activesquad->squad[i]=NULL;
      }
      location[cursite]->siege.siege=0;
   }
   return 1;
}

char heyMisterDog(Creature &a, Creature &tk)
{
	
   bool success = false;
   string pitch;
   string response;

   // Find most Heartful Liberal
   int bestp=0;
   for(int p=0; p<6; p++)
   {
      if(activesquad->squad[p] &&
         activesquad->squad[p]->get_attribute(ATTRIBUTE_HEART, true) >
         activesquad->squad[bestp]->get_attribute(ATTRIBUTE_HEART, true))
      {
         bestp = p;
      }
   }

   // Say something unbelievably hippie
   if(activesquad->squad[bestp]->get_attribute(ATTRIBUTE_HEART, true) >= 15)
   {
      success = true;
	  vector<string> which_choice = pickrandom(lovingly_talk_to_dog);
	  pitch = which_choice[0];
	  response = which_choice[1];
   }
   else // or not
   {
      tk.cantbluff=1;
	  vector<string> which_choice = pickrandom(normal_talk_to_dog);
	  pitch = which_choice[0];
	  response = which_choice[1];
      
   }

   clearcommandarea();
   clearmessagearea();
   clearmaparea();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(10,1);
   addstr(activesquad->squad[bestp]->name, gamelog);
   addstr(" says, ", gamelog);
   move(11,1);
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   addstr(pitch, gamelog);
   gamelog.newline();

   getkey();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(13,1);
   addstr(tk.name, gamelog);
   addstr(" says, ", gamelog);
   move(14,1);
   set_color(COLOR_YELLOW,COLOR_BLACK,1);
   addstr(response, gamelog);
   gamelog.newline();

   getkey();

   if(success)
      for(int i=0;i<ENCMAX;i++)
         if(encounter[i].type == CREATURE_GUARDDOG)
            encounter[i].align = ALIGN_LIBERAL;

   return 1;
}

char heyMisterMonster(Creature &a, Creature &tk)
{
	
   bool success = false;
   string pitch;
   string response;

   // Find most Heartful Liberal
   int bestp=0;
   for(int p=0; p<6; p++)
   {
      if(activesquad->squad[p] &&
         activesquad->squad[p]->get_attribute(ATTRIBUTE_HEART, true) >
         activesquad->squad[bestp]->get_attribute(ATTRIBUTE_HEART, true))
      {
         bestp = p;
      }
   }

   // Say something diplomatic
   if(activesquad->squad[bestp]->get_attribute(ATTRIBUTE_HEART, true) >= 15)
   {
      success = true;
	  vector<string> which_choice = pickrandom(lovingly_talk_to_mutant);
      pitch = which_choice[0];
	  response = which_choice[1];

   }
   else // or not
   {
      tk.cantbluff=1;

	  vector<string> which_choice = pickrandom(normal_talk_to_mutant);
	  pitch = which_choice[0];
	  response = which_choice[1];


   }

   clearcommandarea();
   clearmessagearea();
   clearmaparea();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(10,1);
   addstr(activesquad->squad[bestp]->name, gamelog);
   addstr(" says, ", gamelog);
   move(11,1);
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   addstr(pitch, gamelog);
   gamelog.newline();

   getkey();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(13,1);
   addstr(tk.name, gamelog);
   addstr(" says, ", gamelog);
   move(14,1);
   set_color(COLOR_YELLOW,COLOR_BLACK,1);
   addstr(response, gamelog);
   gamelog.newline();

   getkey();

   if(success)
      for(int i=0;i<ENCMAX;i++)
         if(encounter[i].type == CREATURE_GENETIC)
            encounter[i].align = ALIGN_LIBERAL;

   return 1;
}
