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


/* base - burn the flag */
void burnflag(void)
{
   int flagparts=112;
   long flag[16][7][4];
   int x;
   int y;

   for(int p=0;p<7;p++)
   {
      if(p<3)
      {
         for(x=0;x<6;x++)
         {
            flag[x][p][0]=':';
            flag[x][p][1]=COLOR_WHITE;
            flag[x][p][2]=COLOR_BLUE;
            flag[x][p][3]=1;
         }

         for(x=6;x<16;x++)
         {
            flag[x][p][0]=CH_LOWER_HALF_BLOCK;
            flag[x][p][3]=1;
            flag[x][p][1]=COLOR_WHITE;
            flag[x][p][2]=COLOR_RED;
         }
      }
      else
      {
         for(x=0;x<16;x++)
         {
            if(p<6)
            {
               flag[x][p][0]=CH_LOWER_HALF_BLOCK;
               flag[x][p][1]=COLOR_WHITE;
               flag[x][p][2]=COLOR_RED;
               flag[x][p][3]=1;
            }
            else
            {
               flag[x][p][0]=CH_UPPER_HALF_BLOCK;
               flag[x][p][1]=COLOR_RED;
               flag[x][p][2]=COLOR_BLACK;
               flag[x][p][3]=0;
            }
         }
      }
   }

   x=LCSrandom(16);
   y=LCSrandom(7);
   flag[x][y][0]=CH_DARK_SHADE;
   flag[x][y][1]=COLOR_YELLOW;
   flag[x][y][2]=COLOR_BLACK;
   flag[x][y][3]=1;

   char first=1;

   while(flagparts>0)
   {
      if(!first)
      {
         for(x=0;x<16;x++)
         {
            for(y=0;y<7;y++)
            {
               if(flag[x][y][0]==CH_BOX_DRAWINGS_LIGHT_VERTICAL)flag[x][y][0]=CH_DARK_SHADE;
               else if(flag[x][y][0]==CH_DARK_SHADE)
               {
                  flag[x][y][0]=CH_MEDIUM_SHADE;
                  flag[x][y][1]=COLOR_RED;
                  flag[x][y][2]=COLOR_BLACK;
                  flag[x][y][3]=0;
               }
               else if(flag[x][y][0]==CH_MEDIUM_SHADE)
               {
                  flag[x][y][0]=CH_LIGHT_SHADE;
                  flag[x][y][1]=COLOR_BLACK;
                  flag[x][y][2]=COLOR_BLACK;
                  flag[x][y][3]=1;
               }
               else if(flag[x][y][0]==CH_LIGHT_SHADE)
               {
                  flagparts--;
                  flag[x][y][0]=' ';
                  flag[x][y][1]=COLOR_BLACK;
                  flag[x][y][2]=COLOR_BLACK;
                  flag[x][y][3]=0;
               }
            }
         }
      }
      else first=0;

      for(x=0;x<16;x++)
      {
         for(y=0;y<7;y++)
         {
            move(y+10,x+32);
            set_color(short(flag[x][y][1]),short(flag[x][y][2]),char(flag[x][y][3]));
            addch(flag[x][y][0]);
         }
      }
      refresh();

      pause_ms(10);

      char gotnew=0;
      while(!gotnew&&flagparts>3)
      {
         x=LCSrandom(16);
         y=LCSrandom(7);
         char conf=0;
         if(flag[x][y][0]==':'||flag[x][y][0]==CH_UPPER_HALF_BLOCK||flag[x][y][0]==CH_LOWER_HALF_BLOCK)
         {
            if(x>0)
            {
               if(flag[x-1][y][0]!=':'&&
                  flag[x-1][y][0]!=CH_UPPER_HALF_BLOCK&&
                  flag[x-1][y][0]!=CH_LOWER_HALF_BLOCK)conf=1;
            }
            if(x<15)
            {
               if(flag[x+1][y][0]!=':'&&
                  flag[x+1][y][0]!=CH_UPPER_HALF_BLOCK&&
                  flag[x+1][y][0]!=CH_LOWER_HALF_BLOCK)conf=1;
            }
            if(y>0)
            {
               if(flag[x][y-1][0]!=':'&&
                  flag[x][y-1][0]!=CH_UPPER_HALF_BLOCK&&
                  flag[x][y-1][0]!=CH_LOWER_HALF_BLOCK)conf=1;
            }
            if(y<6)
            {
               if(flag[x][y+1][0]!=':'&&
                  flag[x][y+1][0]!=CH_UPPER_HALF_BLOCK&&
                  flag[x][y+1][0]!=CH_LOWER_HALF_BLOCK)conf=1;
            }
            if(conf)
            {
               flag[x][y][0]=CH_BOX_DRAWINGS_LIGHT_VERTICAL;
               flag[x][y][1]=COLOR_YELLOW;
               flag[x][y][2]=COLOR_BLACK;
               flag[x][y][3]=1;
               gotnew=1;
            }
         }
      }
   }
}

/* base - new slogan */
void getslogan(void)
{
   set_color(COLOR_WHITE,COLOR_BLACK,0);

   move(15,0);
   addstr("What is your new slogan?");
   move(16,0);
   addstr("                                                                                          ");

   keypad(stdscr,FALSE);
   raw_output(FALSE);
   echo();
   curs_set(1);
   move(16,0);
   enter_name(slogan,SLOGAN_LEN);

   curs_set(0);
   noecho();
   raw_output(TRUE);
   keypad(stdscr,TRUE);
}



/* base - reorder party */
void orderparty(void)
{
   party_status=-1;
   if(activesquad==NULL)return;

   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)partysize++;
   }

   if(partysize<=1)return;

   int spot=0;

   do
   {
      printparty();

      move(8,20);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("Choose a Liberal squad member for Place ");
      char num[20];
      itoa(spot+1,num,10);
      addstr(num);
      addstr(".");

      refresh();

      int c=getch();
      translategetch(c);

      if(c==10||c==ESC)return;

      if(c>=spot+'1'&&c<=partysize+'1'-1)
      {
         Creature *swap=activesquad->squad[spot];
         activesquad->squad[spot]=activesquad->squad[c-'1'];
         activesquad->squad[c-'1']=swap;
         spot++;
      }
   }while(spot<partysize-1);
}

/* base - reorder party */
void orderpartyV2(void)
{
   party_status=-1;
   if(activesquad==NULL)return;

   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)partysize++;
   }

   if(partysize<=1)return;

   int spot=0;

   do
   {
      printparty();
      move(8,20);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("Choose squad member to replace ");
      refresh();

      int c=getch();
      translategetch(c);

      if(c==10||c==ESC)return;

      int oldPos = c;
      Creature *swap = NULL;
      if(c>=spot+'1'&&c<=partysize+'1'-1)
      {
         swap=activesquad->squad[oldPos-'1'];
      }
      if(swap == NULL)
      {
         // Haven't found a member
         return;
      }
      char num[20];
      itoa(oldPos,num,10);
      addstr(swap->name);
      addstr(" with");

      c=getch();
      translategetch(c);

      if(c==10||c==ESC)return;

      if(c>=spot+'1'&&c<=partysize+'1'-1)
      {
         activesquad->squad[oldPos-'1']=activesquad->squad[c-'1'];
         activesquad->squad[c-'1']=swap;
      }
   }while(true);
}

/* base - go forth to stop evil */
void stopevil(void)
{
   int l = 0;
   int p = 0;

   if(activesquad==NULL)return;

   bool havecar = false;
   for(p=0; p<6; p++)
   {
      if(activesquad->squad[p] != NULL)
      {
         if(activesquad->squad[p]->pref_carid != -1)
         {
            havecar = true;
            break;
         }
      }
   }

   Location* squad_location = location[activesquad->squad[0]->location];

   int page = 0;
   int loc = -1;

   // Start at the city level, rather than the absolute top
   if(multipleCityMode) {
      for(l=0;l<location.size();l++) {
         if(location[l]->type == squad_location->city) {
            loc = l;
            break;
         }
      }
   }

   vector<long> temploc;
   // 1. LCS safe houses
   for(l=0;l<location.size();l++)
      if(location[l]->parent==loc && location[l]->renting>=0 && !location[l]->hidden)
         temploc.push_back(l);
   // 2. CCS safe houses
   for(l=0;l<location.size();l++)
      if(location[l]->parent==loc && location[l]->renting==RENTING_CCS && !location[l]->hidden)
         temploc.push_back(l);
   // 3. Other sites
   for(l=0;l<location.size();l++)
      if(location[l]->parent==loc && location[l]->renting==RENTING_NOCONTROL && !location[l]->hidden)
         temploc.push_back(l);

   // Determine cost of tickets for travel
   int squadsize = 0;
   for(int s=0; s<6; s++)
   {
      if(activesquad->squad[s] != NULL)
         squadsize++;
   }
   int ticketprice = 100 * squadsize;
   char ticketpricestr[10];
   itoa(100 * squadsize, ticketpricestr, 10);

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Where will the Squad go?");

      printparty();

      if(loc!=-1)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddstr(8,0,location[loc]->getname(-1, true));
      }

      /*mvaddstr(12,50,"Z - Toggle Squad Stance");
      switch(activesquad->stance)
      {
      case SQUADSTANCE_ANONYMOUS:
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         mvaddstr(13,50,"ANONYMOUS ACTION");
         set_color(COLOR_BLACK,COLOR_BLACK,1);
         mvaddstr(14,50,"+ Less Likely to be identified");
         mvaddstr(15,50,"- Can't gain Juice");
         mvaddstr(16,50,"- Unlikely to affect issues");
         break;
      case SQUADSTANCE_STANDARD:
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         mvaddstr(13,50,"CLAIMED ACTION");
         set_color(COLOR_BLACK,COLOR_BLACK,1);
         mvaddstr(14,50,"+ Can sneak and bluff");
         mvaddstr(15,50,"+ Can gain juice");
         mvaddstr(16,50,"+ Can affect issues");
         break;
      case SQUADSTANCE_BATTLECOLORS:
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         mvaddstr(13,50,"GREEN ARMBANDS");
         set_color(COLOR_BLACK,COLOR_BLACK,1);
         mvaddstr(14,50,"- Alarm conservatives on sight");
         mvaddstr(15,50,"+ Gain extra juice");
         mvaddstr(16,50,"+ Bonus issue effects");
         break;
      }*/

      temploc.clear();
      for(l=0;l<location.size();l++)
      {
         if(location[l]->parent==loc&&location[l]->renting>=0&&!location[l]->hidden)temploc.push_back(l);
      }
      for(l=0;l<location.size();l++)
      {
         if(location[l]->parent==loc&&location[l]->renting==RENTING_CCS&&!location[l]->hidden)temploc.push_back(l);
      }
      for(l=0;l<location.size();l++)
      {
         if(location[l]->parent==loc&&location[l]->renting==RENTING_NOCONTROL&&!location[l]->hidden)temploc.push_back(l);
      }


      int y=10;
      for(p=page*11;p<temploc.size()&&p<page*11+11;p++)
      {
         if(p == -1) break;
         Location* this_location = location[temploc[p]];

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddch(y,0,y-10+(int)'A');
         addstr(" - ");
         addstr(location[temploc[p]]->getname());

         bool show_safehouse_info = false;
         if(this_location == squad_location) {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" (Current Location)");
            show_safehouse_info = true;
         } else if(this_location->renting >= 0) {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            addstr(" (Safe House)");
            show_safehouse_info = true;
         } else if(this_location->renting == RENTING_CCS) {
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr(" (Enemy Safe House)");
         } else if(this_location->closed) {
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr(" (Closed Down)");
         } else if(this_location->highsecurity) {
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            addstr(" (High Security)");
         } else if(multipleCityMode && this_location->type == squad_location->city) {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(" (Current Location)");
         } else if(this_location->area != squad_location->area && !havecar) {
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            addstr(" (Need Car)");
         } else if(this_location->type == SITE_TRAVEL) {
            if(ledger.get_funds() < ticketprice)
               set_color(COLOR_RED,COLOR_BLACK,1);
            else
               set_color(COLOR_GREEN,COLOR_BLACK,1);
            addstr_f(" ($%s)", ticketpricestr);
         }
         if(this_location->siege.siege > 0) {
            set_color(COLOR_RED,COLOR_BLACK,0);
            addstr(" (Under Siege)");
         }

         if(show_safehouse_info)
         {
            char num[10];
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(y,50);
            addstr("Heat: ");
            if(this_location->heat > 100)
               set_color(COLOR_RED,COLOR_BLACK,1);
            else if(this_location->heat > 0)
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
            else
               set_color(COLOR_GREEN,COLOR_BLACK,1);
            itoa(location[temploc[p]]->heat,num,10);
            addstr(num);
            addstr("%");
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(y,61);
            addstr("Secrecy: ");
            set_color(COLOR_BLACK,COLOR_BLACK,1);
            location[temploc[p]]->update_heat_protection();
            itoa(location[temploc[p]]->heat_protection,num,10);
            addstr(num);
            addstr("%");
         }

         if(multipleCityMode && this_location->city == this_location->type)
         {
            set_color(COLOR_BLACK,COLOR_BLACK,1);
            mvaddstr(y,50,this_location->city_description());
         }

         y++;
      }
      if(multipleCityMode && loc != -1 && location[loc]->type == location[loc]->city)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddch(y+1,0,y-10+(int)'A');
         addstr(" - Travel to a Different City");
         if(!havecar) {
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            addstr(" (Need Car)");
         } else {
            if(ledger.get_funds() < ticketprice)
               set_color(COLOR_RED,COLOR_BLACK,1);
            else
               set_color(COLOR_GREEN,COLOR_BLACK,1);
            addstr_f(" ($%s)", ticketpricestr);
         }
         temploc.push_back(-1);
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      //PAGE UP
      if(page>0)
      {
         move(10,60);
         addprevpagestr();
      }
      //PAGE DOWN
      if((page+1)*11<temploc.size())
      {
         move(20,60);
         addnextpagestr();
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(24,1);
      if((loc == -1) || (multipleCityMode && location[loc]->type == squad_location->city)) addstr("Enter - The squad is not yet Liberal enough.");
      else addstr("Enter - Back one step.");

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*11<temploc.size())page++;

      if(c>='a'&&c<='k')
      {
         int sq=page*11+(int)(c-'a');
         if(sq<temploc.size()&&sq>=0)
         {
            int oldloc=loc;
            loc=temploc[sq];
            if((loc==-1 || (multipleCityMode && location[loc]->city != squad_location->city)) && !havecar)
            {
               loc = oldloc;
            }
            int subcount = 0;

            for(l=0;l<location.size();l++)
               if(location[l]->parent==loc)
                  subcount++;

            if(subcount==0 || (multipleCityMode && loc >= 0 && location[loc]->city != squad_location->city))
            {
               if(!location[loc]->closed&&
                  ((location[loc]->area==squad_location->area&&location[loc]->city==squad_location->city)||havecar))
               {
                  activesquad->activity.type=ACTIVITY_VISIT;
                  activesquad->activity.arg=loc;
                  return;
               }
               else
               {
                  loc=oldloc;
               }
            }
         }
      }

      /*if(c=='z')
      {
      activesquad->stance++;
      if(activesquad->stance>SQUADSTANCE_STANDARD)
      activesquad->stance=0;
      }*/

      if(c==10||c==ESC)
      {
         if(loc!=-1 && (location[loc]->city != location[loc]->type || location[loc]->city != squad_location->city))
         {
            loc=location[loc]->parent;
         }
         else
         {
            activesquad->activity.type=ACTIVITY_NONE; // Clear squad activity
            break;
         }
      }
   }while(1);
}



/* base - invest in this location */
void investlocation(void)
{
   int loc=selectedsiege;

   do
   {
      erase();

      locheader();
      printlocation(loc);

      if(location[loc]->can_be_fortified())
      {
         if(ledger.get_funds()>=2000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(8,1);
         if(location[loc]->type == SITE_OUTDOOR_BUNKER)
            addstr("W - Repair the Bunker Fortifications ($2000)");
         else if(location[loc]->type == SITE_RESIDENTIAL_BOMBSHELTER)
            addstr("W - Fortify the Bomb Shelter Entrances ($2000)");
         else
            addstr("W - Fortify the Compound for a Siege ($2000)");
      }

      if(!(location[loc]->compound_walls & COMPOUND_CAMERAS))
      {
         if(ledger.get_funds()>=2000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(9,1);
         addstr("C - Place Security Cameras around the Compound ($2000)");
      }

      if(location[loc]->can_be_trapped())
      {
         if(ledger.get_funds()>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,1);
         addstr("B - Place Booby Traps throughout the Compound ($3000)");
      }

      if(location[loc]->can_install_tanktraps())
      {
         if(ledger.get_funds()>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,1);
         addstr("T - Ring the Compound with Tank Traps ($3000)");
      }

      if(!(location[loc]->compound_walls & COMPOUND_GENERATOR))
      {
         if(ledger.get_funds()>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,1);
         addstr("G - Buy a Generator for emergency electricity ($3000)");
      }

      if(!(location[loc]->compound_walls & COMPOUND_AAGUN))
      {
         if(law[LAW_GUNCONTROL]==ALIGN_ARCHCONSERVATIVE)
         {
            if(ledger.get_funds()>=35000)set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(13,1);
            addstr("A - Install a perfectly legal Anti-Aircraft gun on the roof ($35,000)");
         }
         else
         {
            if(ledger.get_funds()>=200000)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(13,1);
            addstr("A - Install and conceal an illegal Anti-Aircraft gun on the roof ($200,000)");
         }
      }

      if(!(location[loc]->compound_walls & COMPOUND_PRINTINGPRESS))
      {
         if(ledger.get_funds()>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(14,1);
         addstr("P - Buy a Printing Press to start your own newspaper ($3000)");
      }

      if(location[loc]->can_have_businessfront())
      {
         if(ledger.get_funds()>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(15,1);
         addstr("F - Setup a Business Front to ward off suspicion ($3000)");
      }

      if(ledger.get_funds()>=150)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(16,1);
      addstr("R - Stockpile 20 daily rations of food ($150)");

      move(17,1);
      addstr("Enter - Done");

      int c=getch();
      translategetch(c);

      if(c==10||c==ESC)break;

      if(c=='w')
      {
         if(location[loc]->can_be_fortified()&&ledger.get_funds()>=2000)
         {
            ledger.subtract_funds(2000,EXPENSE_COMPOUND);
            location[loc]->compound_walls|=COMPOUND_BASIC;
         }
      }

      if(c=='c')
      {
         if(!(location[loc]->compound_walls & COMPOUND_CAMERAS)&&ledger.get_funds()>=2000)
         {
            ledger.subtract_funds(2000,EXPENSE_COMPOUND);
            location[loc]->compound_walls|=COMPOUND_CAMERAS;
         }
      }

      if(c=='b')
      {
         if(location[loc]->can_be_trapped()&&ledger.get_funds()>=3000)
         {
            ledger.subtract_funds(3000,EXPENSE_COMPOUND);
            location[loc]->compound_walls|=COMPOUND_TRAPS;
         }
      }

      if(c=='t')
      {
         if(location[loc]->can_install_tanktraps()&&ledger.get_funds()>=3000)
         {
            ledger.subtract_funds(3000,EXPENSE_COMPOUND);
            location[loc]->compound_walls|=COMPOUND_TANKTRAPS;
         }
      }

      if(c=='g')
      {
         if(!(location[loc]->compound_walls & COMPOUND_GENERATOR)&&ledger.get_funds()>=3000)
         {
            ledger.subtract_funds(3000,EXPENSE_COMPOUND);
            location[loc]->compound_walls|=COMPOUND_GENERATOR;
         }
      }

      if(c=='a')
      {
         int aagunPrice = 200000;
         if(law[LAW_GUNCONTROL] == ALIGN_ARCHCONSERVATIVE)
            aagunPrice = 35000;

         if(!(location[loc]->compound_walls & COMPOUND_AAGUN)&&ledger.get_funds()>=aagunPrice)
         {
            ledger.subtract_funds(aagunPrice,EXPENSE_COMPOUND);
            location[loc]->compound_walls|=COMPOUND_AAGUN;
         }
      }

      if(c=='p')
      {
         if(!(location[loc]->compound_walls & COMPOUND_PRINTINGPRESS)&&ledger.get_funds()>=3000)
         {
            ledger.subtract_funds(3000,EXPENSE_COMPOUND);
            location[loc]->compound_walls|=COMPOUND_PRINTINGPRESS;
         }
      }

      if(c=='r')
      {
         if(ledger.get_funds()>=150)
         {
            ledger.subtract_funds(150,EXPENSE_COMPOUND);
            location[loc]->compound_stores+=20;
         }
      }

      if(c=='f')
      {
         if(location[loc]->can_have_businessfront()&&ledger.get_funds()>=3000)
         {
            ledger.subtract_funds(3000,EXPENSE_COMPOUND);
            do
            {
               location[loc]->front_business=LCSrandom(BUSINESSFRONTNUM);
               lastname(location[loc]->front_name);
               strcat(location[loc]->front_name," ");
               switch(location[loc]->front_business)
               {
               case BUSINESSFRONT_INSURANCE:
                  switch(LCSrandom(7))
                  {
                  case 0:
                     strcat(location[loc]->front_name,"Auto");
                     strcpy(location[loc]->front_shortname,"Auto");
                     break;
                  case 1:
                     strcat(location[loc]->front_name,"Life");
                     strcpy(location[loc]->front_shortname,"Life");
                     break;
                  case 2:
                     strcat(location[loc]->front_name,"Health");
                     strcpy(location[loc]->front_shortname,"Health");
                     break;
                  case 3:
                     strcat(location[loc]->front_name,"Home");
                     strcpy(location[loc]->front_shortname,"Home");
                     break;
                  case 4:
                     strcat(location[loc]->front_name,"Boat");
                     strcpy(location[loc]->front_shortname,"Boat");
                     break;
                  case 5:
                     strcat(location[loc]->front_name,"Fire");
                     strcpy(location[loc]->front_shortname,"Fire");
                     break;
                  case 6:
                     strcat(location[loc]->front_name,"Flood");
                     strcpy(location[loc]->front_shortname,"Flood");
                     break;
                  }
                  strcat(location[loc]->front_name," Insurance");
                  strcat(location[loc]->front_shortname," Ins.");
                  break;
               case BUSINESSFRONT_TEMPAGENCY:
                  switch(LCSrandom(7))
                  {
                  case 0:
                     strcat(location[loc]->front_name,"Temp Agency");
                     strcpy(location[loc]->front_shortname,"Agency");
                     break;
                  case 1:
                     strcat(location[loc]->front_name,"Manpower, LLC");
                     strcpy(location[loc]->front_shortname,"Manpower");
                     break;
                  case 2:
                     strcat(location[loc]->front_name,"Staffing, Inc");
                     strcpy(location[loc]->front_shortname,"Staff");
                     break;
                  case 3:
                     strcat(location[loc]->front_name,"Labor Ready");
                     strcpy(location[loc]->front_shortname,"Labor");
                     break;
                  case 4:
                     strcat(location[loc]->front_name,"Employment");
                     strcpy(location[loc]->front_shortname,"Employ");
                     break;
                  case 5:
                     strcat(location[loc]->front_name,"Services");
                     strcpy(location[loc]->front_shortname,"Services");
                     break;
                  case 6:
                     strcat(location[loc]->front_name,"Solutions");
                     strcpy(location[loc]->front_shortname,"Solutns");
                     break;
                  }
                  break;
               case BUSINESSFRONT_RESTAURANT:
                  switch(LCSrandom(7))
                  {
                  case 0:
                     strcat(location[loc]->front_name,"Fried Chicken");
                     strcpy(location[loc]->front_shortname,"Chicken");
                     break;
                  case 1:
                     strcat(location[loc]->front_name,"Hamburgers");
                     strcpy(location[loc]->front_shortname,"Burgers");
                     break;
                  case 2:
                     strcat(location[loc]->front_name,"Steakhouse");
                     strcpy(location[loc]->front_shortname,"Steak");
                     break;
                  case 3:
                     strcat(location[loc]->front_name,"Wok Buffet");
                     strcpy(location[loc]->front_shortname,"Wok");
                     break;
                  case 4:
                     strcat(location[loc]->front_name,"Thai Cuisine");
                     strcpy(location[loc]->front_shortname,"Thai");
                     break;
                  case 5:
                     strcat(location[loc]->front_name,"Pizzeria");
                     strcpy(location[loc]->front_shortname,"Pizza");
                     break;
                  case 6:
                     strcat(location[loc]->front_name,"Fine Dining");
                     strcpy(location[loc]->front_shortname,"Diner");
                     break;
                  }
                  break;
               case BUSINESSFRONT_MISCELLANEOUS:
                  switch(LCSrandom(7))
                  {
                  case 0:
                     strcat(location[loc]->front_name,"Real Estate");
                     strcpy(location[loc]->front_shortname,"Realty");
                     break;
                  case 1:
                     strcat(location[loc]->front_name,"Imported Goods");
                     strcpy(location[loc]->front_shortname,"Import");
                     break;
                  case 2:
                     strcat(location[loc]->front_name,"Waste Disposal");
                     strcpy(location[loc]->front_shortname,"Disposal");
                     break;
                  case 3:
                     strcat(location[loc]->front_name,"Liquor Shop");
                     strcpy(location[loc]->front_shortname,"Liquor");
                     break;
                  case 4:
                     strcat(location[loc]->front_name,"Antiques");
                     strcpy(location[loc]->front_shortname,"Antique");
                     break;
                  case 5:
                     strcat(location[loc]->front_name,"Repair, Inc");
                     strcpy(location[loc]->front_shortname,"Repair");
                     break;
                  case 6:
                     strcat(location[loc]->front_name,"Pet Store");
                     strcpy(location[loc]->front_shortname,"Pets");
                     break;
                  }
                  break;
               }
            } while (location[loc]->duplicatelocation());
         }
      }
   }while(1);
}



/* base - assign a vehicle to this squad */
void setvehicles(void)
{
   int p, l;
   if(activesquad==NULL)return;

   int page=0;

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Choosing the Right Liberal Vehicle");

      printparty();

      int x=1,y=10;
      char str[200];

      for(l=page*18;l<vehicle.size()&&l<page*18+18;l++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         for(p=0;p<6;p++)
         {
            if(activesquad->squad[p]==NULL)continue;
            if(activesquad->squad[p]->alive&&
               activesquad->squad[p]->pref_carid==vehicle[l]->id())
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               break;
            }
         }
         for(p=0;p<pool.size();p++)
         {
            if(pool[p]->squadid!=-1&&pool[p]->alive&&
               pool[p]->pref_carid==vehicle[l]->id()&&pool[p]->squadid!=activesquad->id)
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               break;
            }
         }

         str[0]=l-page*18+'A';
         str[1]='\x0';
         strcat(str," - ");
         strcat(str,vehicle[l]->fullname(true).c_str());

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
      if(page>0)
      {
         move(17,1);
         addprevpagestr();
      }
      //PAGE DOWN
      if((page+1)*18<vehicle.size())
      {
         move(17,53);
         addnextpagestr();
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(18,1);
      addstr("Press a letter to specify passengers for that Liberal vehicle");
      move(19,1);
      addstr("Capitalize the letter to designate a driver.");
      move(20,1);
      addstr("Press a number to remove that squad member from a vehicle.");
      move(21,1);
      addstr("Note:  Vehicles in yellow have already been selected by another squad");
      move(22,1);
      addstr("       These cars may be used by both squads but not on the same day.");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(24,1);
      addstr("Enter - Done");

      refresh();

      int c=getch();
      translategetch_cap(c);

      if(c>='A'&&c<='R')
      {
         int slot=c-'A'+page*18;

         if(slot>=0&&slot<vehicle.size())
         {
            bool choice = true;
            if (activesquad->squad[0])
            {
               choice = false;
               for (int c=1; c<6; c++)
               {
                  if (activesquad->squad[c]) //are these slots always filled in order?
                  {
                     choice=true;
                     break;
                  }
               }
            }
            int c;
            if (choice)
            {
               move(8,20);
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               addstr("Choose a Liberal squad member to drive it.");

               refresh();

               c=getch();
               translategetch(c);
            }
            else
            {
               c='1';
            }

            if(c>='1'&&c<='6')
            {
               if(activesquad->squad[c-'1']!=NULL)
               {
                  activesquad->squad[c-'1']->pref_carid=vehicle[slot]->id();
                  if(activesquad->squad[c-'1']->canwalk())
                  {
                     activesquad->squad[c-'1']->pref_is_driver=1;
                  }
                  else activesquad->squad[c-'1']->pref_is_driver=0;
               }
            }
         }
      }

      if(c>='a'&&c<='r')
      {
         int slot=c-'a'+page*18;

         if(slot>=0&&slot<vehicle.size())
         {
            bool choice = true;
            if (activesquad->squad[0])
            {
               choice = false;
               for (int c=1; c<6; c++)
               {
                  if (activesquad->squad[c]) //are these slots always filled in order?
                  {
                     choice=true;
                     break;
                  }
               }
            }
            int c;
            if (choice)
            {
               move(8,20);
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               addstr("Choose a Liberal squad member to be a passenger.");

               refresh();

               c=getch();
               translategetch(c);
            }
            else
            {
               c='1';
            }

            if(c>='1'&&c<='6')
            {
               if(activesquad->squad[c-'1']!=NULL)
               {
                  activesquad->squad[c-'1']->pref_carid=vehicle[slot]->id();
                  activesquad->squad[c-'1']->pref_is_driver=0;
               }
            }
         }
      }

      //SAV - adding way to remove people from vehicles.

      if(c>='1'&&c<='6')
      {
         // 1. Is there someone there?
         if(activesquad->squad[c-'1']!=NULL)
         {
            // 2. Are they in a vehicle? Someday we'll want to enforce car capacity
            int vin=activesquad->squad[c-'1']->pref_carid;
            if ( vin > -1)
            {
               activesquad->squad[c-'1']->pref_carid=-1;
               activesquad->squad[c-'1']->pref_is_driver=0;
            }
         }
      }

      //SAV - End add

      if(c=='x'||c=='X'||c==ESC||c==10)return;

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*18<vehicle.size())page++;

   }while(1);
}
