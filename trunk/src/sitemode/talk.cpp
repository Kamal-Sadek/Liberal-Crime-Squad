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

   move(11,1);
   addstr("A - Quietly pass the teller a robbery note");
   if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
   addstr(".");
   move(12,1);
   addstr("B - Threaten bystanders and demand access to the vault");
   if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
   addstr(".");
   move(13,1);
   addstr("C - On second thought, don't rob the bank");
   if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
   addstr(".");

   int c;
   do {
      c=getch();
      translategetch(c);
   } while(c<'a' && c>'c');

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
      switch(LCSrandom(10))
      {
      case 0:addstr("KINDLY PUT MONEY IN BAG. OR ELSE.", gamelog);break;
      case 1:addstr("I AM LIBERATING YOUR MONEY SUPPLY.", gamelog);break;
      case 2:addstr("THIS IS A ROBBERY. GIVE ME THE MONEY.", gamelog);break;
      case 3:addstr("I HAVE A GUN. CASH PLEASE.", gamelog);break;
      case 4:addstr("THE LIBERAL CRIME SQUAD REQUESTS CASH.", gamelog);break;
      case 5:addstr("I AM MAKING A WITHDRAWAL. ALL YOUR MONEY.", gamelog);break;
      case 6:addstr("YOU ARE BEING ROBBED. GIVE ME YOUR MONEY.", gamelog);break;
      case 7:addstr("PLEASE PLACE LOTS OF DOLLARS IN THIS BAG.", gamelog);break;
      case 8:addstr("SAY NOTHING. YOU ARE BEING ROBBED.", gamelog);break;
      case 9:addstr("ROBBERY. GIVE ME CASH. NO FUNNY MONEY.", gamelog);break;
      }
      gamelog.newline();
      refresh();
      getch();

      if(location[cursite]->highsecurity)
      {
         set_color(COLOR_WHITE, COLOR_BLACK, 1);
         move(11, 1);
         addstr("The bank teller reads the note, ", gamelog);
         switch(LCSrandom(5))
         {
         case 0:addstr("gestures, ", gamelog);break;
         case 1:addstr("signals, ", gamelog);break;
         case 2:addstr("shouts, ", gamelog);break;
         case 3:addstr("screams, ", gamelog);break;
         case 4:addstr("gives a warning, ", gamelog);break;
         }
         move(12, 1);
         addstr("and dives for cover as the guards move in on the squad!", gamelog);
         gamelog.newline();
         refresh();
         getch();
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
         switch(LCSrandom(5))
         {
         case 0:addstr("nods calmly, ", gamelog);break;
         case 1:addstr("looks startled, ", gamelog);break;
         case 2:addstr("bites her lip, ", gamelog);break;
         case 3:addstr("grimaces, ", gamelog);break;
         case 4:addstr("frowns, ", gamelog);break;
         }
         move(12, 1);
         addstr("and slips several bricks of cash into the squad's bag.", gamelog);
         gamelog.newline();
         refresh();
         getch();
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
         addstr(armed_liberal->get_weapon().get_shortname(0).c_str(), gamelog);
         addstr(".", gamelog);
         gamelog.newline();
         refresh();
         getch();
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
      refresh();
      getch();

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
         refresh();
         getch();
         sitealarm=1;
         sitealienate=2;
         criminalizeparty(LAWFLAG_BANKROBBERY);
         sitestory->crime.push_back(CRIME_BANKSTICKUP);
         sitecrime+=50;
         CreatureType guard = CREATURE_SECURITYGUARD;
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
         refresh();
         getch();
         move(10, 1);
         addstr("The vault is open!", gamelog);
         gamelog.newline();
         refresh();
         getch();
         
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
   addstr(tk.name);
   add_age(tk);
   addstr(":");

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(11,1);
   addstr("A - Strike up a conversation about politics");
   if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
   addstr(".");
   move(12,1);
   if(tk.can_date(a))set_color(COLOR_WHITE,COLOR_BLACK,0);
   else set_color(COLOR_BLACK,COLOR_BLACK,1);
   addstr("B - Drop a pickup line");
   if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
   addstr(".");
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(13,1);
   addstr("C - On second thought, don't say anything");
   if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
   addstr(".");
   if(tk.type==CREATURE_LANDLORD&&location[cursite]->renting==-1)
   {
      move(14,1);
      addstr("D - Rent a room");
      if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
      addstr(".");
   }
   else if(tk.type==CREATURE_LANDLORD&&location[cursite]->renting>0)
   {
      move(14,1);
      addstr("D - Stop renting a room");
      if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
      addstr(".");
   }
   else if(tk.type==CREATURE_GANGMEMBER||tk.type==CREATURE_MERC)
   {
      move(14,1);
      addstr("D - Buy weapons");
      if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
      addstr(".");
   }
   else if(tk.type==CREATURE_BANK_TELLER)
   {
      move(14,1);
      addstr("D - Rob the bank");
      if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
      addstr(".");
   }

   do
   {
      int c=getch();
      translategetch(c);

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
   } while(1);
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
   refresh();
   getch();

   if (a.is_naked() && a.animalgloss != ANIMALGLOSS_ANIMAL)
   {
      set_color(COLOR_WHITE, COLOR_BLACK, 1);
      move(12, 1);
      addstr(tk.name, gamelog);
      addstr(" responds, ", gamelog);
      set_color(COLOR_BLUE, COLOR_BLACK, 1);
      move(13, 1);
      addstr("\"Put some clothes on before I call the cops.\"", gamelog);
      gamelog.newline();
      refresh();
      getch();
      return 1;
   }

   set_color(COLOR_WHITE, COLOR_BLACK, 1);
   move(12, 1);
   addstr(tk.name, gamelog);
   addstr(" responds, ", gamelog);
   set_color(COLOR_BLUE, COLOR_BLACK, 1);
   move(13, 1);
   addstr("\"Alright. Please clear out your room.\"", gamelog);
   gamelog.newline();
   refresh();
   getch();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(15,1);
   addstr("<Your possessions at this location have been moved to the shelter.>", gamelog);
   gamelog.newline();
   refresh();
   getch();

   location[cursite]->renting=RENTING_NOCONTROL;

   //MOVE ALL ITEMS AND SQUAD MEMBERS
   int hs=0;
   for(int l2=0;l2<location.size();l2++)
   {
      if(location[l2]->type==SITE_RESIDENTIAL_SHELTER)
      {
         hs=l2;
         break;
      }
   }
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->location==cursite)pool[p]->location=hs;
      if(pool[p]->base==cursite)pool[p]->base=hs;
   }
   for(int l2=0;l2<location[cursite]->loot.size();l2++)
   {
      location[hs]->loot.push_back(location[cursite]->loot[l2]);
   }
   location[cursite]->loot.clear();

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
   refresh();
   getch();

   if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      move(13,1);
      addstr("\"Put some clothes on before I call the cops.\"", gamelog);
      gamelog.newline();
      refresh();
      getch();
      return 1;
   }

   int rent;
   switch(location[cursite]->type)
   {
   default:rent=200;break;
   case SITE_RESIDENTIAL_APARTMENT:rent=650;break;
   case SITE_RESIDENTIAL_APARTMENT_UPSCALE:rent=1500;break;
   }

   char num[20];
   itoa(rent,num,10);

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
   set_color(COLOR_BLUE,COLOR_BLACK,1);
   move(13,1);
   addstr("\"It'll be $", gamelog);
   addstr(num, gamelog);
   addstr(" a month.", gamelog);
   gamelog.newline();
   move(14,1);
   addstr("I'll need $", gamelog);
   addstr(num, gamelog);
   addstr(" now as a security deposit.\"", gamelog);
   gamelog.newline();
   refresh();
   getch();
   
   clearcommandarea();clearmessagearea();clearmaparea();

   do
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

      c=getch();
      translategetch(c);

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
         refresh();
         getch();

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
         set_color(COLOR_BLUE,COLOR_BLACK,1);
         move(13,1);
         addstr("\"Rent is due by the third of every month.", gamelog);
         gamelog.newline();
         move(14,1);
         addstr("We'll start next month.\"", gamelog);
         gamelog.newline();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         addstr(" <turns away>");
         refresh();
         getch();

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
         refresh();
         getch();

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
         set_color(COLOR_BLUE,COLOR_BLACK,1);
         move(13,1);
         addstr("\"Not my problem...\"", gamelog);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         addstr(" <turns away>", gamelog);
         gamelog.newline();
         refresh();
         getch();

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
            addstr(armed_liberal->get_weapon().get_shortname(0).c_str(), gamelog);
            addstr(".", gamelog);
            gamelog.newline();
            refresh();
            getch();
            clearmessagearea();
         }
         move(9,1);
         addstr(a.name, gamelog);
         addstr(" says, ", gamelog);
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(10,1);
         addstr("\"What's the price for the Liberal Crime Squad?\"", gamelog);
         gamelog.newline();
         refresh();
         getch();

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
            set_color(COLOR_BLUE,COLOR_BLACK,1);
            move(13,1);
            addstr("\"I think you'd better leave.\"", gamelog);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" <crosses arms>", gamelog);
            gamelog.newline();
            refresh();
            getch();
            tk.cantbluff = 1;
            return 1;
         }
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
            set_color(COLOR_BLUE,COLOR_BLACK,1);
            move(13,1);
            addstr("\"Jesus... it's yours...\"", gamelog);
            gamelog.newline();
            refresh();
            getch();

            int rent;

            // Either he calls the cops...
            if(roll < difficulty) 
            {
               for(int i=0;i<6;i++)
               {
                  if(activesquad->squad[i])
                     criminalize(*(activesquad->squad[i]),LAWFLAG_EXTORTION);
               }
               location[cursite]->siege.timeuntillocated = 2;
               rent = 10000000; // Yeah he's kicking you out next month
            }
            // ...or it's yours for free
            else
            {
               rent = 0;
            }

            location[cursite]->renting=rent;
            location[cursite]->newrental=true;

            basesquad(activesquad,cursite);
            return 1;
         }
      }
   } while(1);
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
   refresh();
   getch();

   if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      move(13,1);
      addstr("\"Jesus...\"", gamelog);
      gamelog.newline();
      refresh();
      getch();
      return 1;
   }
   if(a.get_armor().get_itemtypename()=="ARMOR_POLICEUNIFORM"
      ||a.get_armor().get_itemtypename()=="ARMOR_POLICEARMOR")
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      move(13,1);
      addstr("\"I don't sell guns, officer.\"", gamelog);
      gamelog.newline();
      refresh();
      getch();
      return 1;
   }
   if(sitealarm!=0)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      move(13,1);
      addstr("\"We can talk when things are calm.\"", gamelog);
      gamelog.newline();
      refresh();
      getch();
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
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      move(13,1);
      addstr("\"What exactly do you need?\"", gamelog);
      gamelog.newline();
      refresh();
      getch();

      armsdealer(cursite);
      return 1;
   default:
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      move(13,1);
      addstr("\"Uhhh... not a good place for this.\"", gamelog);
      gamelog.newline();
      refresh();
      getch();
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
   refresh();
   getch();
   
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
      case CREATURE_TANK:
         addstr(" rumbles disinterestedly.", gamelog);
         break;
      case CREATURE_GUARDDOG:
         addstr(" barks.", gamelog);
         break;
      default:
         addstr(" doesn't understand.", gamelog);
      }
      gamelog.newline();
      
      refresh();
      getch();

      return 1;
   }
   else if(tk.type!=CREATURE_PRISONER && interested)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      move(13,1);
      addstr("\"What?\"", gamelog);
      gamelog.newline();
      refresh();
      getch();

      return talkAboutIssues(a, tk);
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      move(13,1);
      if(tk.type==CREATURE_PRISONER)
      {
         if(tk.align==ALIGN_LIBERAL)
            addstr("\"Now's not the time!\"", gamelog);
         else
            addstr("\"Leave me alone.\"", gamelog);
      }
      else
      {
         addstr("\"No.\"", gamelog);
      }
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr(" <turns away>", gamelog);
      gamelog.newline();
      refresh();
      getch();

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
   int line;
   if(law[LAW_FREESPEECH]==-2)
   {
      line=LCSrandom(3);
      switch(line)
      {
         case 0:addstr("\"[What church do you go to?]\"", gamelog);break;
         case 1:addstr("\"[Will you marry me?]\"", gamelog);break;
         case 2:addstr("\"[Do you believe in abstinence education?]\"", gamelog);break;
      }
   }
   else
   {
      line=LCSrandom(44);
      switch(line)
      {
      case 0:addstr("\"Hey baby, you're kinda ugly.  I like that.\"", gamelog);break;
      case 1:addstr("\"I lost my phone number.  Could I have yours?\"", gamelog);break;
      case 2:addstr("\"Hey, you wanna go rub one off?\"", gamelog);break;
      case 3:addstr("\"Hot damn.  You're built like a brick shithouse, honey.\"", gamelog);break;
      case 4:addstr("\"I know I've seen you on the back of a milk carton, ", gamelog);
             move(11,1);y++;
             addstr("cuz you've been missing from my life.\"", gamelog);
             break;
      case 5:addstr("\"I'm big where it counts.\"", gamelog);break;
      case 6:addstr("\"Daaaaaamn girl, I want to wrap your legs around my face and ", gamelog);
             move(11,1);y++;
             addstr("wear you like a feed bag!\"", gamelog); // Bill Hicks
             break;
      case 7:addstr("\"Let's play squirrel.  I'll bust a nut in your hole.\"", gamelog);break;
      case 8:addstr("\"You know, if I were you, I'd have sex with me.\"", gamelog);break;
      case 9:addstr("\"You don't sweat much for a fat chick.\"", gamelog);break;
      case 10:addstr("\"Fuck me if I'm wrong but you want to kiss me, right?\"", gamelog);break;
      case 11:addstr("\"Your parents must be retarded, because you are special.\"", gamelog);break;
      case 12:addstr("\"Let's play trains...  you can sit on my face and I will chew chew chew.\"", gamelog);break;
      case 13:addstr("\"Is it hot in here or is it just you?\"", gamelog);break;
      case 14:addstr("\"I may not be Fred Flintstone, but I can make your bed rock!\"", gamelog);break;
      case 15:addstr("\"What do you say we go behind a rock and get a little boulder?\"", gamelog);break;
      case 16:addstr("\"Do you have stars on your panties?  Your ass is outta this world!\"", gamelog);break;
      case 17:addstr("\"Those pants would look great on the floor of my bedroom.\"", gamelog);break;
      case 18:addstr("\"If I said you had a nice body, would you hold it against me?\"", gamelog);break;
      case 19:addstr("\"Are you tired?  You've been running around in my thoughts all day.\"", gamelog);break;
      case 20:addstr("\"If I could change the alphabet baby, I would put the U and I together!\"", gamelog);break;
      case 21:addstr("\"Your lips look sweet.  Can I taste them?\"", gamelog);break;
      case 22:addstr("\"Nice shoes.  Wanna fuck?\"", gamelog);break;
      case 23:addstr("\"Your sexuality makes me nervous and this frustrates me.\"", gamelog);break;
      case 24:addstr("\"Are you Jamaican?  Cuz Jamaican me horny.\"", gamelog);break;
      case 25:addstr("\"Hey pop tart, fancy coming in my toaster of love?\"", gamelog);break;
      case 26:addstr("\"Wanna play army?  You lie down and I'll blow you away.\"", gamelog);break;
      case 27:addstr("\"Can I lick your forehead?\"", gamelog);break;
      case 28:addstr("\"I have a genital rash.  Will you rub this ointment on me?\"", gamelog);break;
      case 29:addstr("\"What's your sign?\"", gamelog);break;
      case 30:addstr("\"Do you work for the post office? ", gamelog);
            move(11,1);y++;
            addstr("Because I could have sworn you were checking out my package.\"", gamelog);
            break;
      case 31:addstr("\"I'm not the most attractive person in here, ", gamelog);
            move(11,1);y++;
            addstr("but I'm the only one talking to you.\"", gamelog);
            break;
      case 32:addstr("\"Hi.  I suffer from amnesia.  Do I come here often?\"", gamelog);break;
      case 33:addstr("\"I'm new in town.  Could you give me directions to your apartment?\"", gamelog);break;
      case 34:addstr("\"Stand still so I can pick you up!\"", gamelog);break;
      case 35:addstr("\"Your daddy must have been a baker, cuz you've got a nice set of buns.\"", gamelog);break;
      case 36:addstr("\"If you were a laser, you'd be set on 'stunning'.\"", gamelog);break;
      case 37:addstr("\"Is that a keg in your pants?  Cuz I'd love to tap that ass.\"", gamelog);break;
      case 38:addstr("\"If I could be anything, I'd love to be your bathwater.\"", gamelog);break;
      case 39:addstr("\"Stop, drop and roll, baby.  You are on fire.\"", gamelog);break;
      case 40:addstr("\"Do you want to see something swell?\"", gamelog);break;
      case 41:addstr("\"Excuse me.  Do you want to fuck or should I apologize?\"", gamelog);break;
      case 42:addstr("\"Say, did we go to different schools together?\"", gamelog);break;
      case 43:addstr("\"You smell...  Let's go take a shower.\"", gamelog);break;
      }
   }
   gamelog.newline();
   refresh();
   getch();

   bool succeeded = false;

   int difficulty = DIFFICULTY_HARD;

   if(tk.type == CREATURE_CORPORATE_CEO)
      difficulty = DIFFICULTY_HEROIC;

   if(a.skill_check(SKILL_SEDUCTION,difficulty))
      succeeded = true;

   if((tk.animalgloss==ANIMALGLOSS_ANIMAL&&law[LAW_ANIMALRESEARCH]!=2&&a.animalgloss!=ANIMALGLOSS_ANIMAL)||
      tk.animalgloss==ANIMALGLOSS_TANK)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(12,1);addstr(tk.name, gamelog);

      switch(tk.type)
      {
      case CREATURE_TANK:
         addstr(" shakes its turret a firm 'no'.", gamelog);
         break;
      case CREATURE_GUARDDOG:
         addstr(" says, ", gamelog);
         move(13,1);
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(3))
         {
         case 0:addstr("\"No! Wrong! I'm a dog!! Jesus.\"", gamelog);break;
         case 1:addstr("\"What?! Ugh, I'm going to toss my kibble.\"", gamelog);break;
         case 2:addstr("\"Okay, you need to stop petting me now.\"", gamelog);break;
         }
         tk.align=ALIGN_CONSERVATIVE;
         tk.cantbluff=1;
         break;
      case CREATURE_GENETIC:
         addstr(" says, ", gamelog);
         move(13,1);
         set_color(COLOR_RED,COLOR_BLACK,1);
         switch(LCSrandom(8))
         {
         case 0:addstr("\"Foolish human!\"", gamelog);break;
         case 1:addstr("\"Never thought I'd hear that...\"", gamelog);break;
         case 2:addstr("\"STRANGER DANGER.\"", gamelog);break;
         case 3:addstr("\"I am not laughing, mortal!\"", gamelog);break;
         case 4:addstr("\"Gag!\"", gamelog);break;
         case 5:addstr("\"You would make jokes with the likes of me?!\"", gamelog);break;
         case 6:addstr("\"I am above such mortal sins!\"", gamelog);break;
         case 7:addstr("\"You foul, disgusting human...!\"", gamelog);break;
         }
         tk.align=ALIGN_CONSERVATIVE;
         tk.cantbluff=1;
         break;
      default:
         addstr(" doesn't quite pick up on the subtext.", gamelog);
      }
      gamelog.newline();
      
      refresh();
      getch();

      return 1;
   }

   a.train(SKILL_SEDUCTION,LCSrandom(5)+2);

   if((a.get_armor().get_itemtypename()=="ARMOR_POLICEUNIFORM" // Police property on armor? -XML
            || a.get_armor().get_itemtypename()=="ARMOR_POLICEARMOR")
      && tk.type==CREATURE_PROSTITUTE)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(y,1);y++;addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_RED,COLOR_BLACK,1);
      move(y,1);y++;

      addstr("Dirty. You know that's illegal, officer.", gamelog);
      gamelog.newline();

      refresh();
      getch();

      tk.cantbluff=1;
   }
   else if(succeeded)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(y,1);y++;addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      move(y,1);y++;

      if(law[LAW_FREESPEECH]==-2)
      {
         switch(line)
         {
            case 0:addstr("\"[I go to your church.]\"", gamelog);break;
            case 1:addstr("\"[Yes.]\"", gamelog);break;
            case 2:addstr("\"[Yes.  Yes, I do.]\"", gamelog);break;
         }
      }
      else
      {
         switch(line)
         {
         //LIMIT             :-----------------------------------------------------------------------------:
         case 0:addstr("\"You're not so cute yourself.  Wanna get a room?\"", gamelog);break;
         case 1:addstr("\"How sweet!  You can call me tonight...\"", gamelog);break;
         case 2:addstr("\"You bet, baby.\"", gamelog);break;
         case 3:addstr("\"He he, I'll let that one slide.  Besides, I like country folk...\"", gamelog);break;
         case 4:addstr("\"That's sick.  I can do sick tonight.\"", gamelog);break;
         case 5:addstr("\"Oooo, let me see!\"", gamelog);break;
         case 6:addstr("\"Wow, looks like I'm going to have to reward creativity tonight!\"", gamelog);break;
         case 7:addstr("\"Winter's coming.  You'd better bust more than one.\"", gamelog);break;
         case 8:addstr("\"But you're not, so the pleasure's all mine.\"", gamelog);break;
         case 9:addstr("\"Just wait until tonight, baby.\"", gamelog);break;
         case 10:addstr("\"You're wrong.\"", gamelog);break;
         case 11:addstr("\"I can drool on you if you like it that way.\"", gamelog);break;
         case 12:addstr("\"Oooo, all aboard baby!\"", gamelog);break;
         case 13:addstr("\"Not as hot as we'll be tonight you slut.\"", gamelog);break;
         case 14:addstr("\"Goober.  You wanna hook up tonight?\"", gamelog);break;
         case 15:addstr("\"Oooo, we should get stoned too!  He he.\"", gamelog);break;
         case 16:addstr("\"You'll have to whip out your rocket to get some.  Let's do it.\"", gamelog);break;
         case 17:addstr("\"So would my underwear.\"", gamelog);break;
         case 18:addstr("\"Yeah, and you're going to repay me tonight.\"", gamelog);break;
         case 19:addstr("\"Then stop *thinking* about it and come over tonight.\"", gamelog);break;
         case 20:addstr("\"As long as you put a condom between them, I'm all for it.\"", gamelog);break;
         case 21:addstr("\"Sure, but you can't use your mouth.\"", gamelog);break;
         case 22:addstr("\"I hope you don't have a foot fetish, but I'm game.\"", gamelog);break;
         case 23:addstr("\"My sex could do even more.\"", gamelog);break;
         case 24:addstr("\"Let me invite you to visit my island paradise.  Tonight.\"", gamelog);break;
         case 25:addstr("\"Oh, man...  just don't tell anybody I'm seeing you.\"", gamelog);break;
         case 26:addstr("\"I hope we're shooting blanks, soldier.  I'm out of condoms.\"", gamelog);break;
         case 27:addstr("\"You can lick all my decals off, baby.\"", gamelog);break;
         case 28:addstr("\"Only if I'm not allowed to use my hands.\"", gamelog);break;
         case 29:addstr("\"The one that says 'Open All Night'.\"", gamelog);break;
         case 30:addstr("\"It looks like a letter bomb to me.  Let me blow it up.\"", gamelog);break;
         case 31:addstr("\"Hey, I could do better.  But I'm feeling cheap tonight.\"", gamelog);break;
         case 32:addstr("\"Yeah.  I hope you remember the lube this time.\"", gamelog);break;
         case 33:addstr("\"But if we use a hotel, you won't get shot by an angry spouse tonight.\"", gamelog);break;
         case 34:addstr("\"I think you'll appreciate the way I move after tonight.\"", gamelog);break;
         case 35:addstr("\"They make a yummy bedtime snack.\"", gamelog);break;
         case 36:addstr("\"Oh..  oh, God.  I can't believe I'm going to date a Trekkie.\"", gamelog);break;
         case 37:addstr("\"Oh, it isn't safe for you to drive like that.  You'd better stay the night.\"", gamelog);break;
         case 38:addstr("\"Come over tonight and I can show you what it's like.\"", gamelog);break;
         case 39:addstr("\"I'll stop, drop and roll if you do it with me.\"", gamelog);break;
         case 40:addstr("\"I'd rather feel something swell.\"", gamelog);break;
         case 41:addstr("\"You can apologize later if it isn't any good.\"", gamelog);break;
         case 42:addstr("\"Yeah, and we tonight can try different positions together.\"", gamelog);break;
         case 43:addstr("\"Don't you like it dirty?\"", gamelog);break;
         }
      }
      gamelog.newline();
      refresh();
      getch();

      y++;
      move(y,1);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(y,1);y++;
      addstr(a.name, gamelog);
      addstr(" and ", gamelog);
      addstr(tk.name, gamelog);
      addstr(" make plans for tonight", gamelog);
      if(tk.type==CREATURE_PRISONER)
      {
         addstr(", and ", gamelog);
         move(y,1);y++;
         addstr(tk.name, gamelog);
         addstr(" breaks for the exit", gamelog);
         criminalize(tk,LAWFLAG_ESCAPED);
      }
      addstr(".  ", gamelog);
      gamelog.newline();
      refresh();
      getch();

      int olddate=0;
      datest *newd=NULL;
      for(int d=0;d<date.size();d++)
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
      move(y,1);y++;addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_RED,COLOR_BLACK,1);
      move(y,1);y++;
      if(tk.type == CREATURE_CORPORATE_CEO)
      {
         if(a.gender_liberal != GENDER_MALE)
         {
            addstr("\"I'm a happily married man, sweetie.\"", gamelog);
         }
         else
         {
            addstr("\"This ain't Brokeback Mountain, son.\"", gamelog);
         }
      }
      else
      {
         switch(LCSrandom(9))
         {
         case 0: addstr("\"Jesus...\"", gamelog);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" <turns away>", gamelog);break;
         case 1: addstr("\"Touch me and you'll regret it.\"", gamelog);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" <crosses arms>", gamelog);break;
         case 2: addstr("\"I'm.. uh.. waiting for someone.\"", gamelog);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" <turns away>", gamelog);break;
         case 3: addstr("\"Hey, look, a UFO!\"", gamelog);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" <ducks away>", gamelog);break;
         case 4: addstr("\"You're not my type. I like sane people.\"", gamelog);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" <turns away>", gamelog);break;
         case 5: addstr("\"Hahahahaha!\"", gamelog);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" <shakes head>", gamelog);break;
         case 6: addstr("\"You're disgusting.\"", gamelog);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" <turns away>", gamelog);break;
         case 7: addstr("\"Are you serious?\"", gamelog);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" <turns away>", gamelog);break;
         case 8: addstr("\"You're a pig.\"", gamelog);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" <turns away>", gamelog);break;
         }
      }
      gamelog.newline();
      
      refresh();
      getch();

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
   move(y,1);y++;
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
         addstr("\"The government is systematically trying to rob women of the right ", gamelog);move(y,1);y++;
         addstr("to control their own destinies.\"", gamelog);
         break;
      case LAW_ANIMALRESEARCH:
         addstr("\"Animals are routinely subjected to inhumane treatment in labs in this", gamelog);move(y,1);y++;
         addstr("country.\"", gamelog);
         break;
      case LAW_POLICEBEHAVIOR:
         addstr("\"The police regularly torture minority suspects during interrogations.\"", gamelog);
         break;
      case LAW_PRIVACY:
         addstr("\"Files are being kept on innocent citizens whose only crime is to ", gamelog);move(y,1);y++;
         addstr("speak out against a system that is trying to farm them like beasts.\"", gamelog);
         break;
      case LAW_DEATHPENALTY:
         addstr("\"Over thirty innocent people have been executed over the past decade.\"", gamelog);
         break;
      case LAW_NUCLEARPOWER:
         addstr("\"Radioactive waste is being stored all over the country, and it poses ", gamelog);move(y,1);y++;
         addstr("a serious threat to many families, even in this neighborhood.\"", gamelog);
         break;
      case LAW_POLLUTION:
         addstr("\"Industries that stop at nothing to become more profitable are polluting ", gamelog);move(y,1);y++;
         addstr("the environment in ways that hurt not only animals, but people too.", gamelog);
         break;
      case LAW_LABOR:
         addstr("\"Have you noticed how people are working more and more hours for less and ", gamelog);move(y,1);y++;
         addstr("less money?  It's all part of a plan to keep you enslaved, man.\"", gamelog);
         break;
      case LAW_GAY:
         addstr("\"Homosexuals are people like anyone else, and yet they are treated in this ", gamelog);move(y,1);y++;
         addstr("country as if they are deviants fit only for cheap entertainment.\"", gamelog);
         break;
      case LAW_CORPORATE:
         addstr("\"Corporate executives use giant corporations as a means to become parasites ", gamelog);move(y,1);y++;
         addstr("that suck wealth out of this country and put it into their pockets.\"", gamelog);
         break;
      case LAW_FREESPEECH:
         addstr("\"Protests and demonstrations are regularly and often brutally suppressed in ", gamelog);move(y,1);y++;
         addstr("this country.  People have to watch what they write -- even what they read.\"", gamelog);
         break;
      case LAW_FLAGBURNING:
         addstr("\"Burning a piece of cloth is actually stigmatized in this country. ", gamelog);move(y,1);y++;
         addstr("You can love freedom and still hate what our government stands for.\"", gamelog);
         break;
      case LAW_TAX:
         addstr("\"The tax code has been designed to perpetuate an unjust class ", gamelog);move(y,1);y++;
         addstr("structure that is keeping you oppressed.\"", gamelog);
         break;
      case LAW_GUNCONTROL:
         addstr("\"We live in such a backwards country right now that people think it's ", gamelog);move(y,1);y++;
         addstr("a right to walk around with the power to murder at any moment.\"", gamelog);
         break;
      case LAW_WOMEN:
         addstr("\"Sexism is still pervasive, in subtle ways, and women make much less ", gamelog);move(y,1);y++;
         addstr("than they deserve for their labor.\"", gamelog);
         break;
      case LAW_CIVILRIGHTS:
         addstr("\"Despite our progress, this society is still strangled by its continuing ", gamelog);move(y,1);y++;
         addstr("legacy of racial discrimination and inequality.\"", gamelog);
         break;
      case LAW_DRUGS:
         addstr("\"The government's drug policy is a mess.  We need to stop filling ", gamelog);move(y,1);y++;
         addstr("prisons with drug users, and only intervene when people really need help.\"", gamelog);
         break;
      case LAW_IMMIGRATION:
         addstr("\"Millions of people are doing jobs most folks don't even want, and ", gamelog);move(y,1);y++;
         addstr("saving their families from poverty, but we just try to kick them out.\"", gamelog);
         break;
      case LAW_ELECTIONS:
         addstr("\"Political favors are bought and sold for campaign contributions, ", gamelog);move(y,1);y++;
         addstr("and the voting system enforces two party dominance.\"", gamelog);
         break;
      case LAW_MILITARY:
         addstr("\"Take a breath and think about the world we live in, that we're spending ", gamelog);move(y,1);y++;
         addstr("hundreds of billions on new ways to kill people.  This has to stop!\"", gamelog);
         break;
      case LAW_TORTURE:
         addstr("\"In the name of the war on terror, we've sacrificed our soul by letting ", gamelog);move(y,1);y++;
         addstr("the government torture and abuse human beings on our behalf.\"", gamelog);
         break;
      case LAW_PRISONS:
         addstr("\"The prison system doesn't help criminals by providing rehabilitation, so ", gamelog);move(y,1);y++;
         addstr("when they get released, they mostly become criminals again.\"", gamelog);
         break;
      }
   }
   gamelog.newline();
   refresh();
   getch();

   int difficulty = DIFFICULTY_VERYEASY;

   if(tk.align == ALIGN_CONSERVATIVE)
      difficulty += 7;
   if(!(tk.talkreceptive()))
      difficulty += 7;
   if(you_are_stupid)
      difficulty += 5;
   if(issue_too_liberal)
      difficulty += 5;
   if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)
      difficulty += 5;
   
   succeeded = a.skill_check(SKILL_PERSUASION,difficulty);

   // Prisoners never accept to join you, you must liberate them instead
   if(succeeded && tk.type!=CREATURE_PRISONER)
   {
      y++;
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(y,1);y++;addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      move(y,1);y++;
      if(tk.type==CREATURE_MUTANT&&
         tk.get_attribute(ATTRIBUTE_INTELLIGENCE,true)<3)
      {
         addstr("\"Aaaahhh...\"", gamelog);
      }
      else
      {
         switch(LCSrandom(10))
         {
         case 0: addstr("\"Dear me! Is there anything we can do?\"", gamelog); break;
         case 1: addstr("\"That *is* disturbing!   What can I do?\"", gamelog); break;
         case 2: addstr("\"Gosh!   Is there anything I can do?\"", gamelog); break;
         case 3: addstr("\"That's frightening!   What can we do?\"", gamelog); break;
         case 4: addstr("\"Oh, really?\" ", gamelog);
                 refresh();
                 getch();
                 move(y,1);y++;
                 set_color(COLOR_GREEN,COLOR_BLACK,1);
                 addstr("\"Yeah, really!\"", gamelog);
                 break;
         case 5: addstr("\"Oh my Science!   We've got to do something!\"", gamelog); break;
         case 6: addstr("\"Dude... that's like... totally bumming me.\"", gamelog); break;
         case 7: addstr("\"Gadzooks! Something must be done!\"", gamelog);break;
         case 8: addstr("\"You got anything to smoke on you?\" ", gamelog);
                 set_color(COLOR_WHITE,COLOR_BLACK,1);
                 addstr("*cough*", gamelog);
                 break;
         case 9: addstr("\"Lawks, I don't think we can allow that.\"", gamelog);break;
         }
      }
      gamelog.newline();
      
      refresh();
      getch();

      y++;move(y,1);y++;
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("After more discussion, ", gamelog);
      addstr(tk.name, gamelog);
      addstr(" agrees to come by later tonight.", gamelog);
      gamelog.newline();
      refresh();
      getch();

      Creature *newcr=new Creature;
         *newcr=tk;
      newcr->namecreature();

      recruitst *newrst=new recruitst;
      newrst->recruit=newcr;
      newrst->recruiter_id = a.id;

      //newcr->align=1;

      recruit.push_back(newrst);

      delenc(&tk-encounter,0);
      return 1;
   }
   else
   {
      y++;
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(y,1);y++;addstr(tk.name, gamelog);addstr(" responds, ", gamelog);
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      if(tk.type==CREATURE_MUTANT&&
         tk.get_attribute(ATTRIBUTE_INTELLIGENCE,true)<3)
      {
         move(y,1);y++;
         addstr("\"Ugh.  Pfft.\"", gamelog);
      }
      else
      {
         if(tk.align==ALIGN_CONSERVATIVE && you_are_stupid)
         {
            move(y,1);y++;
            if(tk.type==CREATURE_GANGUNIT)
               addstr("\"Do you want me to arrest you?\"", gamelog);
            else if(tk.type==CREATURE_DEATHSQUAD)
               addstr("\"If you don't shut up, I'm going to shoot you.\"", gamelog);
            else
            {
               switch(LCSrandom(10))
               {
               case 0:addstr("\"Get away from me, you hippie.\"", gamelog);break;
               case 1:addstr("\"My heart aches for humanity.\"", gamelog);break;
               case 2:addstr("\"I'm sorry, but I think I'm done talking to you.\"", gamelog);break;
               case 3:addstr("\"Do you need some help finding the exit?\"", gamelog);break;
               case 4:addstr("\"People like you are the reason I'm on medication.\"", gamelog);break;
               case 5:addstr("\"Everyone is entitled to be stupid, but you abuse the privilege.\"", gamelog);break;
               case 6:addstr("\"I don't know what you're on, but I hope it's illegal.\"", gamelog);break;
               case 7:addstr("\"Don't you have a parole meeting to get to?\"", gamelog);break;
               case 8:addstr("\"Wow. Why am I talking to you again?\"", gamelog);break;
               case 9:addstr("\"Were you dropped as a child?\"", gamelog);break;
               }
            }
         }
         else if(tk.align != ALIGN_LIBERAL && tk.attribute_check(ATTRIBUTE_WISDOM,DIFFICULTY_AVERAGE))
         {
            move(y,1);y++;
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
            move(y,1);y++;
            addstr("\"Whatever.\"", gamelog);
         }
      }
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr(" <turns away>", gamelog);
      gamelog.newline();
      refresh();
      getch();

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
   addstr(tk.name, gamelog);
   addstr(":", gamelog);
   gamelog.newline();

   int c=0;
   int hostages=0;
   int weaponhostage=0;
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
   refresh();
   while(1)
   {
      c=getch();
      translategetch(c);
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

      switch(LCSrandom(3))
      {
      case 0:
         {   // Formatting the slogan so that it always has quotes around it and punctuation
            if(slogan[0]!='"')addch('"');
            addstr(slogan, gamelog);
            int last=strlen(slogan);
            if(last && slogan[last-1]!='"' && slogan[last-1]!='!' && slogan[last-1]!='.' && slogan[last-1]!='?')
            {
               addch('!');
            }
            if(last && slogan[last-1]!='"')addstr("\"", gamelog);

            if(!sitestory->claimed)
               sitestory->claimed=1;
            break;
         }
      case 1:
         addstr("Die you Conservative dogs!", gamelog);
         break;
      case 2:
         addstr("We're the Liberal Crime Squad!", gamelog);
         break;
      case 3:
         addstr("Praying won't help you now!", gamelog);
         break;
      }
      
      refresh();
      getch();
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
                  encounter[e].type==CREATURE_AGENT)
               {
                  if(LCSrandom(3))
                  {
                     continue;
                  }
               }
               clearmessagearea();
               move(16,1);
               addstr(encounter[e].name, gamelog);
               switch(LCSrandom(6))
               {
               case 0:addstr(" chickens out!", gamelog);break;
               case 1:addstr(" backs off!", gamelog);break;
               case 2:addstr(" doesn't want to die!", gamelog);break;
               case 3:addstr(" is out of there!", gamelog);break;
               case 4:addstr(" has a family!", gamelog);break;
               case 5:addstr(" is too young to die!", gamelog);break;
               }
               delenc(e,0);
               addjuice(a,2,200); // Instant juice!
               refresh();
               getch();
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
      case 0:addstr("\"Back off or the hostage dies!\"", gamelog);break;
      case 1:addstr("\"Don't push the LCS!\"", gamelog);
         if(!sitestory->claimed)sitestory->claimed=1;break;
      case 2:addstr("\"Hostage says you better leave!\"", gamelog);break;
      case 3:addstr("\"I'll do it! I'll kill this one!\"", gamelog);break;
      case 4:addstr("\"You gonna tell the family you pushed me?!\"", gamelog);break;
      case 5:
             if(law[LAW_FREESPEECH]==-2)addstr("\"Don't [play] with me!\"", gamelog);
             else addstr("\"Don't fuck with me!\"", gamelog);
             break;
      }
      gamelog.newline();

      sitecrime+=5;
      criminalizeparty(LAWFLAG_KIDNAPPING);

      addjuice(a,-2,-10); // DE-juice for this shit

      
      refresh();
      getch();

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
                  encounter[e].type==CREATURE_SWAT)&&
                  LCSrandom(5))
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  clearmessagearea();
                  move(16,1);
                  addstr(encounter[e].name, gamelog);
                  addstr(": ", gamelog);
                  move(17,1);

                  if(encounter[e].align!=ALIGN_CONSERVATIVE)
                  {
                     set_color(COLOR_GREEN,COLOR_BLACK,1);
                     switch(LCSrandom(5))
                     {
                     case 0:addstr("\"Let them go. Think about what you're doing.\"", gamelog);break;
                     case 1:addstr("\"Calm down, and let's talk about this.\"", gamelog);break;
                     case 2:addstr("\"Wait! We can work this out.\"", gamelog);break;
                     case 3:addstr("\"This isn't right, think about it.\"", gamelog);break;
                     case 4:addstr("\"Slow down. We can work this out.\"", gamelog);break;
                     }
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
                        switch(LCSrandom(5))
                        {
                        case 0:addstr("\"Hahahaha...\"", gamelog);break;
                        case 1:addstr("\"You think you can scare me?\"", gamelog);break;
                        case 2:addstr("\"You're not getting out of here alive.\"", gamelog);break;
                        case 3:addstr("\"What's wrong?  Need your diaper changed?\"", gamelog);break;
                        case 4:addstr("\"Three... two...\"", gamelog);break;
                        }
                     }
                     else
                     {
                        switch(LCSrandom(5))
                        {
                        case 0:
                           if(hostages>1)
                              addstr("\"Release your hostages, and nobody gets hurt.\"", gamelog);
                           else
                              addstr("\"Let the hostage go, and nobody gets hurt.\"", gamelog);
                           break;
                        case 1:addstr("\"You got about five seconds to back down.\"", gamelog);break;
                        case 2:addstr("\"You want to do this the hard way?\"", gamelog);break;
                        case 3:addstr("\"Big mistake.\"", gamelog);break;
                        case 4:addstr("\"Release them, and I'll let you go.\"", gamelog);break;
                        }
                     }

                  }
                  gamelog.newline();

                  refresh();
                  getch();

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
            refresh();
            getch();
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

            while(1)
            {
               c=getch();
               translategetch(c);
               if(c=='a'||c=='b'||c=='b')break;
            }
            if(c=='a')
            {
               Creature* executer=0;
               if(a.prisoner)
               {
                  executer=&a;
               }
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
               refresh();
               getch();
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

               refresh();
               getch();

               delete executer->prisoner;
               executer->prisoner=NULL;

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
                  else
                     addstr("\"[No!] ", gamelog);
                  switch(LCSrandom(5))
                  {
                  case 0:addstr("Okay, okay, you win!\"", gamelog);break;
                  case 1:addstr("Don't shoot!\"", gamelog);break;
                  case 2:addstr("Do you even care?!\"", gamelog);break;
                  case 3:addstr("Heartless!\"", gamelog);break;
                  case 4:addstr("It's not worth it!\"", gamelog);break;
                  }
                  gamelog.newline();

                  for(int i=ENCMAX;i>=0;i--)
                  {
                     if(encounter[i].exists && encounter[i].enemy() && encounter[i].alive)
                     {
                        delenc(i,0);
                     }
                  }

                  refresh();
                  getch();
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
                  if(hostages>1)
                     addstr("\"Back off and we'll let the hostages go.\"", gamelog);
                  else
                     addstr("\"Back off and the hostage goes free.\"", gamelog);
                  break;
               case 1:addstr("\"Freedom for freedom, understand?\"", gamelog);break;
               case 2:addstr("\"Let me go in peace, okay?\"", gamelog);break;
               case 3:addstr("\"Let's make a trade, then.\"", gamelog);break;
               case 4:addstr("\"I just want out of here, yeah?\"", gamelog);break;
               }
               gamelog.newline();

               refresh();
               getch();

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
                  switch(LCSrandom(5))
                  {
                  case 0:addstr("\"Do I look like a loving person?\"", gamelog);break;
                  case 1:addstr("\"You don't take a hint, do you?\"", gamelog);break;
                  case 2:addstr("\"I'm doing the world a favor.\"", gamelog);break;
                  case 3:addstr("\"That's so pathetic...\"", gamelog);break;
                  case 4:addstr("\"It's a deal.\"", gamelog);break;
                  }
                  gamelog.newline();

                  refresh();
                  getch();
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
                  switch(LCSrandom(4))
                  {
                  case 0:addstr("\"Right. Let's do it.\"", gamelog);break;
                  case 1:addstr("\"No further conditions.\"", gamelog);break;
                  case 2:addstr("\"Let them go, and we're done.\"", gamelog);break;
                  case 3:addstr("\"No tricks, okay?\"", gamelog);break;
                  }
                  gamelog.newline();
                  refresh();
                  getch();
                  for(int i=ENCMAX;i>=0;i--)
                  {
                     if(encounter[i].exists &&
                        encounter[i].enemy() &&
                        encounter[i].alive)
                     {
                        delenc(i,0);
                     }
                  }
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
                        delete activesquad->squad[i]->prisoner;
                        activesquad->squad[i]->prisoner=NULL;
                     }
                  }
                  refresh();
                  getch();
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
         refresh();
         getch();
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
            a.get_armor().get_itemtypename() == "SWATARMOR")
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

      refresh();
      getch();

      bool fooled=true;
      int e;
      
      for(e=0;e<ENCMAX;e++)
      {
         if(encounter[e].exists&&encounter[e].alive&&
            encounter[e].enemy())
         {
            if(encounter[e].get_attribute(ATTRIBUTE_WISDOM,true)>10)
               fooled = a.skill_check(SKILL_DISGUISE,DIFFICULTY_CHALLENGING);
            else
               fooled = a.skill_check(SKILL_DISGUISE,DIFFICULTY_AVERAGE);

            if(fooled == false)break;
         }
      }

      a.train(SKILL_DISGUISE,20);

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

         refresh();
         getch();
      }
      else
      {
         clearmessagearea();

         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(16,1);
         addstr("The Enemy is fooled and departs.", gamelog);

         refresh();
         getch();
         
         for(int e=ENCMAX-1;e>=0;e--)
         {
            if(encounter[e].exists&&encounter[e].alive&&
               encounter[e].enemy())
            {
               delenc(e,0);
            }
         }
      }
      gamelog.newline();
   }
   else
   {
      move(14,1);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("The Squad is arrested.", gamelog);
      gamelog.newline();
      getch();

      int stolen=0;
      // Police assess stolen goods in inventory
      for(int l=0;l<activesquad->loot.size();l++)
      {
         if(activesquad->loot[l]->is_loot())
            stolen++;
      }
         
      for(int i=0;i<6;++i)
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
   const char *pitch;
   const char *response;

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
      switch(LCSrandom(11))
      {
      case 0:
         pitch = "\"I love dogs more than people.\"";
         response = "\"A human after my own heart, in more ways than one.\"";
         break;
      case 1:
         pitch = "\"Dogs are the future of humanity.\"";
         response = "\"I don't see it, but I'll hear you out.\"";
         break;
      case 2:
         pitch = "\"Power to the canines!\"";
         response = "\"Down with the feline establishment!\"";
         break;
      case 3:
         pitch = "\"We need to recruit more dogs.\"";
         response = "\"Oh yeah? I'm a dog. What do you represent?\"";
         break;
      case 4:
         pitch = "\"Wanna join the LCS?\"";
         response = "\"Do you have a good veteranary plan?\"";
         break;
      case 5:
         pitch = "\"Want me to untie you?\"";
         response = "\"Yes, please! This collar is painful!\"";
         break;
      case 6:
         pitch = "\"You deserve better than this.\"";
         response = "\"Finally, a human that understands.\"";
         break;
      case 7:
         pitch = "\"Dogs are the best anything ever.\"";
         response = "\"Heheheh, you're funny. Okay, I won't rat you out.\"";
         break;
      case 8:
         pitch = "\"Conservatives kick dogs!\"";
         response = "\"That IS disturbing. What can I do?\"";
         break;
      case 9:
         pitch = "\"All we are saying is give fleas a chance.\"";
         response = "\"We'll fight the fleas until our dying itch.\"";
         break;
      case 10:
         pitch = "\"Dogs are better than humans.\"";
         response = "\"You're pandering, but I love it.\"";
         break;
      }
   }
   else // or not
   {
      tk.cantbluff=1;
      switch(LCSrandom(11))
      {
      case 0:
         pitch = "\"Hi Mister Dog!\"";
         response = "\"Woof?\"";
         break;
      case 1:
         pitch = "\"Good dog!\"";
         response = "\"Bark!\"";
         break;
      case 2:
         pitch = "\"Hey there, boy.\"";
         response = "\"Woof!\"";
         break;
      case 3:
         pitch = "\"Woof...?\"";
         response = "\"Woof!\"";
         break;
      case 4:
         pitch = "\"Bark at the man for me!\"";
         response = "\"Bark! Grr...\"";
         break;
      case 5:
         pitch = "\"Down, boy!\"";
         response = "\"Rr...?\"";
         break;
      case 6:
         pitch = "\"Don't bite me!\"";
         response = "\"Grrr...!\"";
         break;
      case 7:
         pitch = "\"Hi doggy!\"";
         response = "\"Bark!\"";
         break;
      case 8:
         pitch = "\"Hi, puppy.\"";
         response = "\"Bark!\"";
         break;
      case 9:
         pitch = "\"OH MAN I LOVE DOGS!\"";
         response = "\"Bark!\"";
         break;
      case 10:
         pitch = "\"Bark! Bark!\"";
         response = "\"Your accent is atrocious.\"";
         break;
      }
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
   refresh();
   getch();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(13,1);
   addstr(tk.name, gamelog);
   addstr(" says, ", gamelog);
   move(14,1);
   set_color(COLOR_YELLOW,COLOR_BLACK,1);
   addstr(response, gamelog);
   gamelog.newline();
   refresh();
   getch();

   if(success)
   {
      for(int i=0;i<ENCMAX;i++)
      {
         if(encounter[i].type == CREATURE_GUARDDOG)
            encounter[i].align = ALIGN_LIBERAL;
      }
   }

   return 1;
}

char heyMisterMonster(Creature &a, Creature &tk)
{
   bool success = false;
   const char *pitch;
   const char *response;

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
      switch(LCSrandom(11))
      {
      case 0:
         pitch = "\"I love diversity in all its forms.\"";
         response = "\"Your tolerance is impressive, human!\"";
         break;
      case 1:
         pitch = "\"Your kind are the future of humanity.\"";
         response = "\"Your recognition of our superiority is wise.\"";
         break;
      case 2:
         pitch = "\"Power to the genetic monsters!\"";
         response = "\"Down with the human establishment!\"";
         break;
      case 3:
         pitch = "\"We need to recruit more genetic monsters.\"";
         response = "\"For what purpose do you seek our aid?\"";
         break;
      case 4:
         pitch = "\"Wanna join the LCS?\"";
         response = "\"Maybe. Can we scare small children?\"";
         break;
      case 5:
         pitch = "\"You're free! Join us to liberate more!\"";
         response = "\"Is this what compassion is?\"";
         break;
      case 6:
         pitch = "\"You deserve better than this.\"";
         response = "\"Nobeast deserves to be an experiment!\"";
         break;
      case 7:
         pitch = "\"You are the best anything ever.\"";
         response = "\"It's okay blokes, this one is friendly.\"";
         break;
      case 8:
         pitch = "\"We should flay geneticists together!\"";
         response = "\"My favorite future hobby!\"";
         break;
      case 9:
         pitch = "\"All we are saying is give peace a chance.\"";
         response = "\"Will humans ever let us have peace?\"";
         break;
      case 10:
         pitch = "\"Monsters are better than humans.\"";
         response = "\"You're a clever one.\"";
         break;
      }
   }
   else // or not
   {
      tk.cantbluff=1;
      switch(LCSrandom(11))
      {
      case 0:
         pitch = "\"Hi Mister Monster!\"";
         response = "\"Die in a fire!\"";
         break;
      case 1:
         pitch = "\"Good monster!\"";
         response = "\"Die in a fire!\"";
         break;
      case 2:
         pitch = "\"Woah, uh... shit!\"";
         response = "\"Foolish mortal!\"";
         break;
      case 3:
         pitch = "\"Don't kill us!\"";
         response = "\"You're already dead!\"";
         break;
      case 4:
         pitch = "\"Oh crap!\"";
         response = "\"Where is your god now, mortal?!\"";
         break;
      case 5:
         pitch = "\"Uhhh... down, boy!\"";
         response = "\"I'm a girl, fool!\"";
         break;
      case 6:
         pitch = "\"Don't eat me!\"";
         response = "\"I will feast on your flesh!\"";
         break;
      case 7:
         pitch = "\"Excuse me, I am, uh...\"";
         response = "\"About to die?!\"";
         break;
      case 8:
         pitch = "\"Shh... it's okay... I'm a friend!\"";
         response = "\"We will kill you AND your friends!\"";
         break;
      case 9:
         pitch = "\"OH MAN I LOVE MONSTERS!\"";
         response = "\"WHAT A COINCIDENCE, I'M HUNGRY!\"";
         break;
      case 10:
         pitch = "\"Slurp! Boom! Raaahgh!\"";
         response = "\"Your mockery will be met with death!\"";
         break;
      }
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
   refresh();
   getch();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(13,1);
   addstr(tk.name, gamelog);
   addstr(" says, ", gamelog);
   move(14,1);
   set_color(COLOR_YELLOW,COLOR_BLACK,1);
   addstr(response, gamelog);
   gamelog.newline();
   refresh();
   getch();

   if(success)
   {
      for(int i=0;i<ENCMAX;i++)
      {
         if(encounter[i].type == CREATURE_GENETIC)
            encounter[i].align = ALIGN_LIBERAL;
      }
   }

   return 1;
}
