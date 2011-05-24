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

#define MAX_PATH_SIZE 2048
extern char artdir[MAX_PATH_SIZE];


/* supporting function -- gives "standard" prices on guns */
long gun_price(const string& type) //Temporary solution until shops are fixed. -XML
{
   double price=0;

   if (type == "WEAPON_SHANK")
      price = 1;
   else if (type == "WEAPON_KNIFE"
            || type == "WEAPON_SYRINGE"
            || type == "WEAPON_MOLOTOV")
      price=10;
   else if (type == "WEAPON_SWORD"
            || type == "WEAPON_DAISHO"
            || type == "WEAPON_HAMMER"
            || type == "WEAPON_MAUL"
            || type == "WEAPON_AXE")
      price=100;
   else if (type == "WEAPON_REVOLVER_38")
      price=200;
   else if (type == "WEAPON_SEMIPISTOL_9MM"
            || type == "WEAPON_SEMIPISTOL_45")
      price=300;
   else if (type == "WEAPON_SHOTGUN_PUMP")
      price=400;
   else if (type == "WEAPON_REVOLVER_44")
      price=500;
   else if (type == "WEAPON_DESERT_EAGLE"
            || type == "WEAPON_SEMIRIFLE_AR15")
      price=800;
   else if (type == "WEAPON_SMG_MP5"
            || type == "WEAPON_FLAMETHROWER")
         price=1100;
   else if (type == "WEAPON_AUTORIFLE_AK47")
         price=1400;
   else if (type == "WEAPON_AUTORIFLE_M16")
         price=1500;
   else if (type == "WEAPON_CARBINE_M4")
         price=1500;

   // Handle illegal weapons by making them more expensive

   // Note no else: the more illegal guns are,
   // the more the price of the most dangerous ones spirals
   // upward   
   if (weapontype[getweapontype(type)]->get_legality() < law[LAW_GUNCONTROL])
      price*=2.0;
   if (weapontype[getweapontype(type)]->get_legality()+1 < law[LAW_GUNCONTROL])
      price*=2.0;
   if (weapontype[getweapontype(type)]->get_legality()+2 < law[LAW_GUNCONTROL])
      price*=2.0;
   if (weapontype[getweapontype(type)]->get_legality()+3 < law[LAW_GUNCONTROL])
      price*=2.0;

   return static_cast<int>(price);
}


/* active squad visits the hospital */
void hospital(int loc)
{
   locatesquad(activesquad,loc);

   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         partysize++;
      }
   }

   do
   {
      erase();

      locheader();
      printparty();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(10,1);
      addstr("F - Go in and fix up Conservative wounds");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(12,1);
      addstr("Enter - Leave");

      if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(13,1);
      addstr("# - Check the status of a squad Liberal");
      if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(14,1);
      addstr("0 - Show the squad's Liberal status");

      int c=getch();
      translategetch(c);

      if(c==10)break;

      if(c=='0')party_status=-1;

      if(c>='1'&&c<='6'&&activesquad!=NULL)
      {
         if(activesquad->squad[c-'1']!=NULL)
         {
            if(party_status==c-'1')fullstatus(party_status);
            else party_status=c-'1';
         }
      }

      if(c=='f')
      {
         for(int p=5;p>=0;p--)
         {
            if(activesquad->squad[p]==NULL)continue;
            hospitalize(loc,*activesquad->squad[p]);
         }
         break;
      }
   }while(1);
}

/* select a gun for arms dealership */
char gunselect(Creature &cr,int &gunindex,bool legal=1)
{
   gunindex=-1;

   vector<int> guntypei;
   guntypei.push_back(getweapontype("WEAPON_SMG_MP5"));
   if(legal)
   {
      guntypei.push_back(getweapontype("WEAPON_CARBINE_M4"));
      guntypei.push_back(getweapontype("WEAPON_AUTORIFLE_M16"));
   }
   if(!(legal && law[LAW_GUNCONTROL]!=-2))
   {
      guntypei.push_back(getweapontype("WEAPON_AUTORIFLE_AK47"));
   }
   if(!((legal && law[LAW_GUNCONTROL]>-1) || !legal))   
      guntypei.push_back(getweapontype("WEAPON_SEMIRIFLE_AR15"));
   if(!(legal && law[LAW_GUNCONTROL]>-1))
      guntypei.push_back(getweapontype("WEAPON_REVOLVER_44"));
   if(!(legal && law[LAW_GUNCONTROL]>0))
   {
      guntypei.push_back(getweapontype("WEAPON_SEMIPISTOL_9MM"));
      guntypei.push_back(getweapontype("WEAPON_SEMIPISTOL_45"));
   }
   if(!((legal && law[LAW_GUNCONTROL]>1) || !legal))
      guntypei.push_back(getweapontype("WEAPON_REVOLVER_38"));
   guntypei.push_back(getweapontype("WEAPON_SHOTGUN_PUMP"));

   erase();
      
   string firstline = "Which weapon will " + string(cr.name) + " buy?";
   const string secondline = "----PRODUCT NAME-----------------------PRICE------------------------------------";
   vector< pair<string,int> > option;
   for (int i = 0; i < guntypei.size(); ++i)
   {
      int price = gun_price(weapontype[guntypei[i]]->get_idname());
      const string& name = weapontype[guntypei[i]]->get_name();
      option.push_back(make_pair(name,price));
   }
   
   int bought = buyprompt(firstline, secondline, option, 35,
                          "Weapon", (string(cr.name) + " needs no weapon"));

   if (bought != -1)
   {
      gunindex=guntypei[bought];
      ledger.subtract_funds(option[bought].second,EXPENSE_SHOPPING);
      return 1;
   }
   return 0;
}

/* active squad visits the arms dealer */
void armsdealer(int loc)
{
   locatesquad(activesquad,loc);
   CMarkup xml; // -XML
   xml.Load(string(artdir) + "armsdealer.xml");
   Shop armsdealer(xml.GetDoc());
   armsdealer.enter(*activesquad);
}


/* active squad visits the pawn shop */
void pawnshop(int loc)
{
   locatesquad(activesquad,loc);
   CMarkup xml; // -XML
   xml.Load(string(artdir) + "pawnshop.xml");
   Shop pawnshop(xml.GetDoc());
   pawnshop.enter(*activesquad);
}



/* active squad visits the car dealership */
void dealership(int loc)
{
   short buyer=0;
   int l;
   char str[80];

   locatesquad(activesquad,loc);

   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         partysize++;
      }
   }

   do
   {
      erase();

      locheader();
      printparty();

      Vehicle* car_to_sell=0;
      int price=0;

      for(int v=(int)vehicle.size()-1;v>=0;v--)
      {
         if(vehicle[v]->id()==activesquad->squad[buyer]->carid)
         {
            car_to_sell = vehicle[v];
         }
      }

      if(!car_to_sell)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(10,1);
      addstr("G - Get a Liberal car");

      move(11,1);
      if(car_to_sell)
      {
         price = static_cast<int>(0.8*car_to_sell->price());

         if(car_to_sell->get_heat())
            price/=10;
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr(("S - Sell the "+car_to_sell->fullname()+" ($"+tostring(price)+")").c_str());
      }
      else
      {
         set_color(COLOR_BLACK,COLOR_BLACK,1);
         addstr("S - Sell a car");
      }

      /*if(car_to_sell && car_to_sell->heat>1 && ledger.get_funds()>=500)
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(12,1);
      addstr("P - Repaint car, replace plates and tags ($500)");*/

      if(partysize>=2)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(16,1);
      addstr("B - Choose a buyer");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(16,40);
      addstr("Enter - Leave");

      if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,1);
      addstr("0 - Show the squad's Liberal status");
      if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,40);
      addstr("# - Check the status of a squad Liberal");

      int c=getch();
      translategetch(c);

      
      // Leave
      if(c==10)break;

      //Sell the car
      if(c=='s' && car_to_sell)
      {
         ledger.add_funds(price,INCOME_CARS);
         delete car_to_sell;
         for(int v=(int)vehicle.size()-1;v>=0;v--)
         {
            if(vehicle[v]==car_to_sell)
            {
               vehicle.erase(vehicle.begin() + v);
               break;
            }
         }
      }

      // Get a car
      if(c=='g' && !car_to_sell)
      {
         int carchoice;

         vector<int> availablevehicle;
         vector<string> vehicleoption;
         for (int i=0; i<vehicletype.size(); ++i)
         {
            if (vehicletype[i]->availableatshop())
            {
               availablevehicle.push_back(i);
               vehicleoption.push_back(vehicletype[i]->longname()+" ($"+tostring(vehicletype[i]->price())+")");
            }
         }

         do
         {
            carchoice = choiceprompt("Choose a vehicle","",vehicleoption,"Vehicle",
                                  true,"We don't need a Conservative car");
            if (carchoice!=-1
                && vehicletype[availablevehicle[carchoice]]->price() > ledger.get_funds())
            {
               set_color(COLOR_RED,COLOR_BLACK,0);
               move(1,1);
               addstr("You don't have enough money!");
               getch();
            }
            else
               break;
         }
         while (1);
         
         if(carchoice==-1) continue;

         //Picked a car, pick color
         int colorchoice;
         //if (vehicletype[availablevehicle[choice]]->color().size()>1) //Allow to back out if you don't like single colour? -XML
         //{
            colorchoice = choiceprompt("Choose a color","",vehicletype[availablevehicle[carchoice]]->color(),
                                       "Color",true,"These colors are Conservative");
         //}
         //else
         //   colorchoice = 0;
         if(colorchoice==-1) continue;

         Vehicle *v=new Vehicle(*vehicletype[availablevehicle[carchoice]],
                     vehicletype[availablevehicle[carchoice]]->color()[colorchoice],year);
         activesquad->squad[buyer]->pref_carid = v->id();
         vehicle.push_back(v);

         ledger.subtract_funds(v->price(),EXPENSE_CARS);
      }

      // Reduce heat
      /*if(c=='p' && car_to_sell && car_to_sell->heat>1 && ledger.get_funds()>=500)
      {
         funds-=500;
         moneylost_goods+=500;
         car_to_sell->heat=1;
      }*/

      if(c=='b')choose_buyer(buyer);

      if(c=='0')party_status=-1;

      if(c>='1'&&c<='6'&&activesquad!=NULL)
      {
         if(activesquad->squad[c-'1']!=NULL)
         {
            if(party_status==c-'1')fullstatus(party_status);
            else party_status=c-'1';
         }
      }

   }while(1);
}



/* active squad visits the department store */
void deptstore(int loc)
{
   locatesquad(activesquad,loc);
   CMarkup xml; // -XML
   xml.Load(string(artdir) + "deptstore.xml");
   Shop deptstore(xml.GetDoc());
   deptstore.enter(*activesquad);
   /*short buyer=0;

   locatesquad(activesquad,loc);

   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         partysize++;
      }
   }

   do
   {
      int armorbought=-1;

      erase();

      locheader();
      printparty();

      move(8,45);
      addstr("Buyer: ");
      addstr(activesquad->squad[buyer]->name);

      if(ledger.get_funds()>=400)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(10,1);
      addstr("C - Buy a Cheap Suit         ($400)");

      if(ledger.get_funds()>=5000)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(11,1);
      addstr("V - Buy an Expensive Suit   ($5000)");

      if(ledger.get_funds()>=500)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(12,1);
      addstr("S - Buy a Black Suit         ($500)");

      if(ledger.get_funds()>=400)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(10,40);
      addstr("H - Buy a Cheap Dress        ($400)");

      if(ledger.get_funds()>=5000)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(11,40);
      addstr("D - Buy an Expensive Dress  ($5000)");

      if(ledger.get_funds()>=500)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(12,40);
      addstr("R - Buy a Black Dress        ($500)");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(15,1);
      addstr("E - Look over Equipment");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(15,30);
      addstr("Enter - Leave");
      if(partysize>=2)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,50);
      addstr("B - Choose a buyer");
      if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(14,1);
      addstr("0 - Show the squad's Liberal status");
      if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(14,40);
      addstr("# - Check the status of a squad Liberal");

      int c=getch();
      translategetch(c);

      if(c==10)break;

      if(ledger.get_funds()>=400&&c=='c')
      {
         armorbought=getarmortype("ARMOR_CHEAPSUIT");
         ledger.subtract_funds(400,EXPENSE_SHOPPING);
      }
      if(ledger.get_funds()>=5000&&c=='v')
      {
         armorbought=getarmortype("ARMOR_EXPENSIVESUIT");
         ledger.subtract_funds(5000,EXPENSE_SHOPPING);
      }
      if(ledger.get_funds()>=500&&c=='s')
      {
         armorbought=getarmortype("ARMOR_BLACKSUIT");
         ledger.subtract_funds(500,EXPENSE_SHOPPING);
      }
      if(ledger.get_funds()>=400&&c=='h')
      {
         armorbought=getarmortype("ARMOR_CHEAPDRESS");
         ledger.subtract_funds(400,EXPENSE_SHOPPING);
      }
      if(ledger.get_funds()>=5000&&c=='d')
      {
         armorbought=getarmortype("ARMOR_EXPENSIVEDRESS");
         ledger.subtract_funds(5000,EXPENSE_SHOPPING);
      }
      if(ledger.get_funds()>=500&&c=='r')
      {
         armorbought=getarmortype("ARMOR_BLACKDRESS");
         ledger.subtract_funds(500,EXPENSE_SHOPPING);
      }

      if(armorbought!=-1)
      {
         Armor a=Armor(*armortype[armorbought]);
         activesquad->squad[buyer]->give_armor(a,&location[activesquad->squad[0]->base]->loot);
      }

      if(c=='e' && activesquad->squad[0]->location!=-1)equip(location[activesquad->squad[0]->location]->loot,-1);
      if(c=='b')choose_buyer(buyer);
      if(c=='0')party_status=-1;
      if(c>='1'&&c<='6'&&activesquad!=NULL)
      {
         if(activesquad->squad[c-'1']!=NULL)
         {
            if(party_status==c-'1')fullstatus(party_status);
            else party_status=c-'1';
         }
      }

   }while(1);*/
}



/* active squad visits the oubliette */
void halloweenstore(int loc)
{
   short buyer=0;
   short in_halloween=0;

   locatesquad(activesquad,loc);

   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         partysize++;
      }
   }

   do
   {
      int weaponbought=-1;
      int armorbought=-1,armorbought2=-1;

      erase();

      locheader();
      printparty();

      move(8,45);
      addstr("Buyer: ");
      addstr(activesquad->squad[buyer]->name);

      if(in_halloween==1)
      {
         if(ledger.get_funds()>=70)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,1);
         addstr("T - Buy a Trench Coat         ($70)");

         if(ledger.get_funds()>=50)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,1);
         addstr("W - Buy Work Clothes          ($50)");

         if(ledger.get_funds()>=200)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,1);
         addstr("L - Buy a Lab Coat           ($200)");

         if(ledger.get_funds()>=200)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,1);
         addstr("R - Buy a Black Judge's Robe ($200)");

         if(ledger.get_funds()>=200)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,40);
         addstr("C - Buy a Clown Suit         ($200)");

         if(ledger.get_funds()>=350)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,40);
         addstr("G - Buy Bondage Gear         ($350)");

         if(ledger.get_funds()>=15)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,40);
         addstr("M - Buy a Mask                ($15)");

         if(ledger.get_funds()>=90)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,40);
         addstr("O - Buy a Toga                ($90)");

         if(ledger.get_funds()>=1000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(14,40);
         addstr("E - Buy an Elephant Suit    ($1000)");

         if(ledger.get_funds()>=1000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(14,40);
         addstr("D - Buy a Donkey Suit       ($1000)");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(15,30);
         addstr("Enter - Done");
      }
      else if(in_halloween==2)
      {
         if(year<2100)
         {
            if(ledger.get_funds()>=20)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,1);
            addstr("K - Buy a Knife                 ($20)");

            if(ledger.get_funds()>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,1);
            addstr("S - Buy the Sword of Morfiegor ($250)");

            if(ledger.get_funds()>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,1);
            addstr("A - Buy a Katana and Wakizashi ($250)");
         }
         else
         {
            if(ledger.get_funds()>=20)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,1);
            addstr("K - Buy a Vibro-Knife          ($20)");

            if(ledger.get_funds()>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,1);
            addstr("S - Buy a Light Sword          ($250)");

            if(ledger.get_funds()>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,1);
            addstr("A - Buy the Liberal Twin Swords($250)");
         }

         if(ledger.get_funds()>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,1);
         addstr("H - Buy a Dwarven Hammer       ($250)");

         if(ledger.get_funds()>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,40);
         addstr("M - Buy the Maul of Anrin      ($250)");

         if(ledger.get_funds()>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,40);
         addstr("C - Buy a Silver Cross         ($250)");

         if(ledger.get_funds()>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,40);
         addstr("W - Buy a Wizard's Staff       ($250)");

         if(ledger.get_funds()>=1000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,40);
         addstr("! - Buy Mithril Mail          ($1000)");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(15,30);
         addstr("Enter - Done");
      }
      else
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(10,1);
         addstr("C - Purchase Halloween Costumes");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(11,1);
         addstr("M - Purchase Medieval Gear");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(15,30);
         addstr("Enter - Leave");
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(15,1);
      addstr("E - Look over Equipment");
      if(partysize>=2)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,50);
      addstr("B - Choose a buyer");
      if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(14,1);
      addstr("0 - Show the squad's Liberal status");
      if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(14,40);
      addstr("# - Check the status of a squad Liberal");

      int c=getch();
      translategetch(c);

      if(in_halloween==1)
      {
         if(c==10)in_halloween=0;

         if(c=='t'&&ledger.get_funds()>=70)
         {
            armorbought=getarmortype("ARMOR_TRENCHCOAT");
            ledger.subtract_funds(70,EXPENSE_SHOPPING);
         }

         if(c=='w'&&ledger.get_funds()>=50)
         {
            armorbought=getarmortype("ARMOR_WORKCLOTHES");
            ledger.subtract_funds(50,EXPENSE_SHOPPING);
         }

         if(c=='l'&&ledger.get_funds()>=200)
         {
            armorbought=getarmortype("ARMOR_LABCOAT");
            ledger.subtract_funds(200,EXPENSE_SHOPPING);
         }

         if(c=='r'&&ledger.get_funds()>=200)
         {
            armorbought=getarmortype("ARMOR_BLACKROBE");
            ledger.subtract_funds(200,EXPENSE_SHOPPING);
         }

         if(c=='c'&&ledger.get_funds()>=200)
         {
            armorbought=getarmortype("ARMOR_CLOWNSUIT");
            ledger.subtract_funds(200,EXPENSE_SHOPPING);
         }

         if(c=='g'&&ledger.get_funds()>=350)
         {
            armorbought=getarmortype("ARMOR_BONDAGEGEAR");
            ledger.subtract_funds(350,EXPENSE_SHOPPING);
         }

         if(c=='e'&&ledger.get_funds()>=1000)
         {
            armorbought=getarmortype("ARMOR_ELEPHANTSUIT");
            ledger.subtract_funds(1000,EXPENSE_SHOPPING);
         }

         if(c=='d'&&ledger.get_funds()>=1000)
         {
            armorbought=getarmortype("ARMOR_DONKEYSUIT");
            ledger.subtract_funds(1000,EXPENSE_SHOPPING);
         }

         if(c=='m'&&ledger.get_funds()>=15)
         {
            short mask;
            if(maskselect(activesquad->squad[buyer],mask))
            {
               armorbought=mask;
               ledger.subtract_funds(15,EXPENSE_SHOPPING);
            }
         }

         if(c=='o'&&ledger.get_funds()>=90)
         {
            armorbought=getarmortype("ARMOR_TOGA");
            ledger.subtract_funds(90,EXPENSE_SHOPPING);
         }
      }
      else if(in_halloween==2)
      {
         if(c==10)in_halloween=0;

         if(c=='k'&&ledger.get_funds()>=20)
         {
            weaponbought=getweapontype("WEAPON_KNIFE");
            ledger.subtract_funds(20,EXPENSE_SHOPPING);
         }

         if(c=='s'&&ledger.get_funds()>=250)
         {
            weaponbought=getweapontype("WEAPON_SWORD");
            ledger.subtract_funds(250,EXPENSE_SHOPPING);
         }

         if(c=='a'&&ledger.get_funds()>=250)
         {
            weaponbought=getweapontype("WEAPON_DAISHO");
            ledger.subtract_funds(250,EXPENSE_SHOPPING);
         }

         if(c=='h'&&ledger.get_funds()>=250)
         {
            weaponbought=getweapontype("WEAPON_HAMMER");
            ledger.subtract_funds(250,EXPENSE_SHOPPING);
         }

         if(c=='m'&&ledger.get_funds()>=250)
         {
            weaponbought=getweapontype("WEAPON_MAUL");
            ledger.subtract_funds(250,EXPENSE_SHOPPING);
         }

         if(c=='c'&&ledger.get_funds()>=250)
         {
            weaponbought=getweapontype("WEAPON_CROSS");
            ledger.subtract_funds(250,EXPENSE_SHOPPING);
         }

         if(c=='w'&&ledger.get_funds()>=250)
         {
            weaponbought=getweapontype("WEAPON_STAFF");
            ledger.subtract_funds(250,EXPENSE_SHOPPING);
         }

         if(c=='!'&&ledger.get_funds()>=1000)
         {
            armorbought=getarmortype("ARMOR_MITHRIL");
            ledger.subtract_funds(1000,EXPENSE_SHOPPING);
         }
      }
      else
      {
         if(c==10)break;
         if(c=='c')in_halloween=1;
         if(c=='m')in_halloween=2;
      }

      if(armorbought!=-1)
      {
         Armor a=Armor(*armortype[armorbought]);
         activesquad->squad[buyer]->give_armor(a,&location[activesquad->squad[0]->base]->loot);
      }
      if(weaponbought!=-1)
      {
         Weapon* w=new Weapon(*weapontype[weaponbought]);
         activesquad->squad[buyer]->give_weapon(*w,&location[activesquad->squad[0]->base]->loot);
         if (w->empty())
            delete w;
         else
            location[activesquad->squad[0]->base]->loot.push_back(w);
      }

      if(c=='e' && activesquad->squad[0]->location!=-1)equip(location[activesquad->squad[0]->location]->loot,-1);
      if(c=='b')choose_buyer(buyer);
      if(c=='0')party_status=-1;
      if(c>='1'&&c<='6'&&activesquad!=NULL)
      {
         if(activesquad->squad[c-'1']!=NULL)
         {
            if(party_status==c-'1')fullstatus(party_status);
            else party_status=c-'1';
         }
      }

   }while(1);
}



/* oubliette - buy a mask */
char maskselect(Creature *cr,short &maskindex)
{
   maskindex=-1;

   vector<unsigned> masktype;
   for(unsigned a=0;a<armortype.size();a++)
   {
      if (armortype[a]->is_mask() && !armortype[a]->is_surprise_mask())
         masktype.push_back(a);
   }

   unsigned page=0;

   char str[200];

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Which mask will ");
      addstr(cr->name);
      addstr(" buy?");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(1,0);
      addstr("----PRODUCT NAME-----------------------DESCRIPTION------------------------------");

      int y=2;
      for(unsigned p=page*19;p<masktype.size()&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         addstr(armortype[masktype[p]]->get_name().c_str());

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,39);
         addstr(armortype[masktype[p]]->get_description().c_str());

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Mask");
      move(23,0);
      addpagestr();
      move(24,0);
      addstr("Z - Surprise ");
      addstr(cr->name);
      addstr(" With a Random Mask");

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<masktype.size())page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<masktype.size())
         {
            maskindex=masktype[p];
            return 1;
         }
      }
      if(c=='z')
      {
         for (int i = 0; i < armortype.size(); ++i)
         {
            if (armortype[i]->is_mask() && armortype[i]->is_surprise_mask())
               masktype.push_back(i);
         }
         maskindex=masktype[LCSrandom(masktype.size())];
         
         return 1;
      }

      if(c==10)break;
   }while(1);

   return 0;
}



/* pick stuff to fence */
int fenceselect(void)
{
   int ret=0;

   consolidateloot(location[activesquad->squad[0]->base]->loot);

   int page=0;

   vector<int> selected(location[activesquad->squad[0]->base]->loot.size(),0);

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("What will you sell?");

      if(ret>0)
      {
         char num[20];
         move(0,30);
         addstr("Estimated Liberal Amount: $");
         itoa(ret,num,10);
         addstr(num);
      }

      printparty();

      int x=1,y=10;
      char str[200];
      string itemstr;

      for(int l=page*18;l<location[activesquad->squad[0]->base]->loot.size()&&l<page*18+18;l++)
      {
         if(selected[l])set_color(COLOR_GREEN,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         itemstr = location[activesquad->squad[0]->base]->loot[l]->equip_title();
         if(location[activesquad->squad[0]->base]->loot[l]->get_number()>1)
         {
            if(selected[l]>0)
            {
               itemstr += tostring(selected[l]) + "/";
            }
            else
               itemstr += "x";
            itemstr += tostring(location[activesquad->squad[0]->base]->loot[l]->get_number());
         }

         str[0]=l-page*18+'A';
         str[1]='\x0';
         strcat(str," - ");
         strcat(str,itemstr.c_str());

         move(y,x);
         addstr(str);

         x+=26;
         if(x>53)
         {
            x=1;
            y++;
         }
      }

      //PAGE UP
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      if(page>0)
      {
         move(17,1);
         addprevpagestr();
      }
      //PAGE DOWN
      if((page+1)*18<location[activesquad->squad[0]->base]->loot.size())
      {
         move(17,53);
         addnextpagestr();
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(23,1);
      addstr("Press a letter to select an item to sell.");
      move(24,1);
      addstr("X - Done");

      refresh();

      int c=getch();
      translategetch(c);

      if(c>='a'&&c<='r')
      {
         int slot=c-'a'+page*18;

         if(slot>=0&&slot<location[activesquad->squad[0]->base]->loot.size())
         {
            if(selected[slot])
            {
               ret-=location[activesquad->squad[0]->base]->loot[slot]->get_fencevalue()*selected[slot];
               selected[slot]=0;
            }
            else
            {
               if(!location[activesquad->squad[0]->base]->loot[slot]->is_good_for_sale())
               {
                  printparty();

                  move(8,15);
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  addstr(" You can't sell damaged goods.");

                  refresh();
                  getch();
               }
               else
               {
                  if(location[activesquad->squad[0]->base]->loot[slot]->get_number()>1)
                  {
                     selected[slot]=1;

                     printparty();

                     move(8,15);
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     addstr("       How many?          ");

                     refresh();

                     char str[100];

                     keypad(stdscr,FALSE);
                     raw_output(FALSE);
                     echo();
                     curs_set(1);
                     mvgetstr(8,30,str);
                     curs_set(0);
                     noecho();
                     raw_output(TRUE);
                     keypad(stdscr,TRUE);

                     selected[slot]=atoi(str);
                     if(selected[slot]<0)
                        selected[slot]=0;
                     else if(selected[slot]>location[activesquad->squad[0]->base]->loot[slot]->get_number())
                        selected[slot]=location[activesquad->squad[0]->base]->loot[slot]->get_number();
                  }
                  else
                     selected[slot]=1;
                  ret+=location[activesquad->squad[0]->base]->loot[slot]->get_fencevalue()*selected[slot];
               }
            }
         }
      }

      if(c=='x')break;

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*18<location[activesquad->squad[0]->base]->loot.size())page++;

   }while(1);

   for(int l=location[activesquad->squad[0]->base]->loot.size()-1;l>=0;l--)
   {
      if(selected[l]>0)
      {
         location[activesquad->squad[0]->base]->loot[l]->decrease_number(selected[l]);
         if(location[activesquad->squad[0]->base]->loot[l]->get_number()<=0)
         {
            delete location[activesquad->squad[0]->base]->loot[l];
            location[activesquad->squad[0]->base]->loot.erase(location[activesquad->squad[0]->base]->loot.begin() + l);
         }
      }
   }

   return ret;
}



/* choose buyer */
void choose_buyer(short &buyer)
{
   if(activesquad==NULL)return;

   party_status=-1;

   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)partysize++;
   }

   if(partysize<=1)return;

   do
   {
      printparty();

      move(8,20);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("Choose a Liberal squad member to SPEND.");

      refresh();

      int c=getch();
      translategetch(c);

      if(c==10)return;

      if(c>='1'&&c<=partysize+'1'-1)
      {
         buyer=c-'1';
         return;
      }
   }while(1);
}
