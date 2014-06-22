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


/* active squad visits the hospital */
void hospital(int loc)
{
   locatesquad(activesquad,loc);

   int partysize=0;
   for(int p=0;p<6;p++) if(activesquad->squad[p]!=NULL) partysize++;

   while(true)
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

      if(partysize>0&&(party_status==-1||partysize>1)) set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(13,1);
      addstr("# - Check the status of a squad Liberal");
      if(party_status!=-1) set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(14,1);
      addstr("0 - Show the squad's Liberal status");

      int c=getkey();

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;

      if(c=='0') party_status=-1;

      if(c>='1'&&c<='6'&&activesquad!=NULL)
         if(activesquad->squad[c-'1']!=NULL)
         {
            if(party_status==c-'1')fullstatus(party_status);
            else party_status=c-'1';
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
   }
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

   locatesquad(activesquad,loc);

   int partysize=0;
   for(int p=0;p<6;p++)
      if(activesquad->squad[p]!=NULL)
         partysize++;

   while(true)
   {
      erase();

      locheader();
      printparty();

      Creature *sleepercarsalesman=NULL;
      for(int p=0;p<(int)pool.size();p++)
         if(pool[p]->alive&&(pool[p]->flag & CREATUREFLAG_SLEEPER)&&
            pool[p]->type==CREATURE_CARSALESMAN&&location[pool[p]->location]->city==location[loc]->city)
            sleepercarsalesman=pool[p];

      Vehicle* car_to_sell=0;
      int price=0;

      for(int v=(int)vehicle.size()-1;v>=0;v--)
         if(vehicle[v]->id()==activesquad->squad[buyer]->carid)
            car_to_sell = vehicle[v];

      if(!car_to_sell) set_color(COLOR_WHITE,COLOR_BLACK,0);
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
         addstr("S - Sell the "+car_to_sell->fullname()+" ($"+tostring(price)+")");
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

      int c=getkey();

      // Leave
      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR)break;

      //Sell the car
      if(c=='s' && car_to_sell)
      {
         ledger.add_funds(price,INCOME_CARS);
         for(int v=(int)vehicle.size()-1;v>=0;v--)
            if(vehicle[v]==car_to_sell)
            {
            	delete_and_remove(vehicle,v);
               break;
            }
      }

      // Get a car
      if(c=='g' && !car_to_sell)
      {
         int carchoice;

         vector<int> availablevehicle;
         vector<string> vehicleoption;
         for(int i=0;i<(int)vehicletype.size();i++)
            if (vehicletype[i]->availableatshop())
            {
               availablevehicle.push_back(i);
               vehicleoption.push_back(vehicletype[i]->longname()+" ($"+
                  tostring(sleepercarsalesman?vehicletype[i]->sleeperprice():vehicletype[i]->price())+")");
            }
         while(true)
         {
            carchoice = choiceprompt("Choose a vehicle","",vehicleoption,"Vehicle",
                                     true,"We don't need a Conservative car");
            if (carchoice!=-1 && (sleepercarsalesman?vehicletype[availablevehicle[carchoice]]->sleeperprice():
                                  vehicletype[availablevehicle[carchoice]]->price()) > ledger.get_funds())
            {
               set_color(COLOR_RED,COLOR_BLACK,0);
               move(1,1);
               addstr("You don't have enough money!");

               getkey();
            }
            else break;
         }

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

         ledger.subtract_funds((sleepercarsalesman?v->sleeperprice():v->price()),EXPENSE_CARS);
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
         if(activesquad->squad[c-'1']!=NULL)
         {
            if(party_status==c-'1')fullstatus(party_status);
            else party_status=c-'1';
         }

   }
}



/* active squad visits the department store */
void deptstore(int loc)
{
   locatesquad(activesquad,loc);
   CMarkup xml; // -XML
   xml.Load(string(artdir) + "deptstore.xml");
   Shop deptstore(xml.GetDoc());
   deptstore.enter(*activesquad);
}



/* active squad visits the oubliette */
void halloweenstore(int loc)
{
   locatesquad(activesquad,loc);
   CMarkup xml;
   xml.Load(string(artdir) + "oubliette.xml");
   Shop oubliette(xml.GetDoc());
   oubliette.enter(*activesquad);
}


/* choose buyer */
void choose_buyer(short &buyer)
{
   if(activesquad==NULL)return;

   party_status=-1;

   int partysize=0;
   for(int p=0;p<6;p++) if(activesquad->squad[p]!=NULL) partysize++;

   if(partysize<=1) return;

   while(true)
   {
      printparty();

      move(8,20);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("Choose a Liberal squad member to SPEND.");

      int c=getkey();

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) return;

      if(c>='1'&&c<=partysize+'1'-1)
      {
         buyer=c-'1';
         return;
      }
   }
}
