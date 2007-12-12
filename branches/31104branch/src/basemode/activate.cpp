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
#include <externs.h>

/* base - activate the uninvolved */
void activate(void)
{
   vector<creaturest *> temppool;
   int sq;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->alive&&
         pool[p]->align==1&&
         pool[p]->clinic==0&&
         pool[p]->dating==0&&
         pool[p]->hiding==0&&
         !(pool[p]->flag & CREATUREFLAG_SLEEPER))
      {
         if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
            location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
            location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON)
         {
            if(pool[p]->squadid!=-1)
            {
               sq=getsquad(pool[p]->squadid);
               if(sq!=-1)
               {
                  if(squad[sq]->activity.type!=ACTIVITY_NONE)continue;
               }
            }
            temppool.push_back(pool[p]);
         }
      }
   }

   if(temppool.size()==0)return;

   short page=0;

   char str[80];
   char num[20];

   do
   {
      erase();
      
      set_color(COLOR_WHITE,COLOR_BLACK,0);     
      printfunds(0,1,"Money: ");

      
      move(0,0);
      addstr("Activate Uninvolved Liberals");
      move(1,0);
      addstr("----CODE NAME------------SKILL---HEALTH---LOCATION------------------------------");
      move(1,57);
      addstr("ACTIVITY");

      int y=2;
      for(int p=page*19;p<temppool.size()&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         char bright=0;
         unsigned long skill=0;
         for(int sk=0;sk<SKILLNUM;sk++)
         {
            skill+=(unsigned long)temppool[p]->skill[sk];
            if(temppool[p]->skill_ip[sk]>=100+(10*temppool[p]->skill[sk])&&
               temppool[p]->skill[sk]<maxskill(sk,*temppool[p]))bright=1;
         }

         set_color(COLOR_WHITE,COLOR_BLACK,bright);

         move(y,25);
         itoa(skill,num,10);
         addstr(num);

         printhealthstat(*temppool[p],y,33,TRUE);

         if(mode==REVIEWMODE_JUSTICE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,42);
         addstr(location[temppool[p]->location]->shortname);

         move(y,57);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         getactivity(str,temppool[p]->activity);
         addstr(str);

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to Assign an Activity.");
      move(23,0);
      if(interface_pgup=='[')
      {
         addstr("[] to view other Liberal pages.");
      }
      else if(interface_pgup=='.')
      {
         addstr("; and : to view other Liberal pages.");
      }
      else
      {
         addstr("PGUP/PGDN to view other Liberal pages.");
      }
      move(24,0);
      addstr("Press Z to assign simple tasks in bulk.");

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if(c==interface_pgup&&page>0)page--;
      //PAGE DOWN
      if(c==interface_pgdn&&(page+1)*19<temppool.size())page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<temppool.size())
         {
            activate(temppool[p]);
         }
      }

      if(c=='z')
      {
         activatebulk();
      }

      if(c==10)break;
   }while(1);
}


void activate(creaturest *cr)
{
   int hostagecount=0;
   int state=0;
   int choice=0;
   char havedead=0;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->alive&&pool[p]->align!=1)hostagecount++;
      if(!pool[p]->alive)havedead=1;
   }

   char num[20];

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      printfunds(0,1,"Money: ");
      
      move(0,0);
      addstr("Taking Action:   What will ");
      addstr(cr->name);
      addstr(" be doing today?");

      printcreatureinfo(cr);

      makedelimiter(8,0);

      set_color(COLOR_WHITE,COLOR_BLACK,state=='a');
      move(10,1);
      addstr("A - Engaging in Liberal Activism");

      set_color(COLOR_WHITE,COLOR_BLACK,state=='b');
      move(11,1);
      addstr("B - Legal Fundraising");
      
      set_color(COLOR_WHITE,COLOR_BLACK,state=='c');
      move(12,1);
      addstr("C - Illegal Fundraising");

      set_color(COLOR_WHITE,COLOR_BLACK,state=='d');
      move(13,1);
      addstr("D - Make/Repair Clothing");
      
      set_color(COLOR_WHITE,COLOR_BLACK,state=='e');
      move(14,1);
      addstr("E - Teaching Other Liberals");
      
      if(cr->skill[SKILL_MEDICAL]!=0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,(cr->activity.type==ACTIVITY_HEAL||cr->activity.type==ACTIVITY_NONE)&&state==0);
      }
      else
      {
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      }
      move(15,1);
      addstr("H - Heal Liberals");

      move(16,1);
      if(cr->canwalk())
      {
         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STEALCARS&&state==0);
         addstr("S - Stealing a Car");
      }
      else
      {
         if(!(cr->flag & CREATUREFLAG_WHEELCHAIR))set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_WHEELCHAIR&&state==0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         addstr("S - Procuring a Wheelchair");
      }

      if(clinictime(*cr))set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_CLINIC&&state==0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(18,1);
      addstr("M - Move to the Free CLINIC");

      if(hostagecount>0)set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_HOSTAGETENDING&&state==0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(17,1);
      addstr("I - Interrogate a Conservative hostage");
      
      if(havedead)set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_BURY&&state==0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(19,1);
      addstr("Z - Dispose of bodies");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(20,40);
      addstr("Enter - Confirm Selection");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(20,1);
      addstr("X - Nothing for Now");

      switch(state)
      {
      case 'a':
         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_COMMUNITYSERVICE);
         move(10,40);
         addstr("1 - Peaceful Community Service");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_TROUBLE);
         move(11,40);
         addstr("2 - Liberal Disobedience");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_GRAFFITI);
         move(12,40);
         addstr("3 - Pro-LCS Graffiti");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_POLLS);
         move(13,40);
         addstr("4 - Check Opinion Polls");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_DOS_ATTACKS);
         move(14,40);
         addstr("5 - DoS Cyber Activism");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_HACKING);
         move(15,40);
         addstr("6 - Hack Secure Networks");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         switch(choice)
         {
         case '1':
            move(22,1);
            addstr("  Spend time working on a non-political cause and ask for nothing in return.");
            break;
         case '2':
            move(22,1);
            addstr("  Cause trouble and disrupt society with aggressive activism to try to get");
            move(23,1);
            addstr("people to question their Conservative assumptions.");
            break;
         case '3':
            move(22,1);
            addstr("  Create name recognition and put artistic skills to work creating street art");
            move(23,1);
            addstr("with a deep political message.");
            break;
         case '4':
            move(22,1);
            addstr("  Spend the day searching for the latest public opinion polls online.");
            break;
         case '5':
            move(22,1);
            addstr("  Attack Conservative web sites by launching massive denial of service attacks");
            move(23,1);
            addstr("to effectively knock them offline. No warning, no demands, no mercy.");
            break;
         case '6':
            move(22,1);
            addstr("  Break into private networks to steal and publish their Conservative secrets.");
            break;
         }
         break;
      case 'b':
         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_DONATIONS);
         move(10,40);
         addstr("1 - Solicit Donations");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_SELL_TSHIRTS);
         move(11,40);
         addstr("2 - Sell T-Shirts");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_SELL_ART);
         move(12,40);
         addstr("3 - Sell Liberal Art");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_SELL_MUSIC);
         move(13,40);
         addstr("4 - Sell Liberal Music");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         switch(choice)
         {
         case '1':
            move(22,1);
            addstr("  Ask people to give money to directly support the Liberal Crime Squad.");
            break;
         case '2':
            move(22,1);
            addstr("  Make and sell Liberal T-Shirts with slogans and tie-dye patterns.");
            break;
         case '3':
            move(22,1);
            addstr("  Create and sell avant-garde Liberal art to raise money for a good cause.");
            break;
         case '4':
            move(22,1);
            addstr("  Record and sell political protest music written by LCS members.");
            break;
         }
         break;
      case 'c':
         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_SELL_DRUGS);
         move(10,40);
         addstr("1 - Sell Brownies");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_PROSTITUTION);
         move(11,40);
         addstr("2 - Prostitution");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_CCFRAUD);
         move(12,40);
         addstr("3 - Credit Card Fraud");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_DOS_RACKET);
         move(13,40);
         addstr("4 - DoS Protection Racket");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         switch(choice)
         {
         case '1':
            move(22,1);
            addstr("  Sell drugs at high prices to users and addicts. High demand, but");
            move(23,1);
            addstr("equally illegal, and there is risk of conflict with local gangs.");
            break;
         case '2':
            move(22,1);
            addstr("  Trade sex for money. A misdemeanor. The police have a special vice team");
            move(23,1);
            addstr("dedicated to tracking down and gathering evidence against street workers.");
            break;
         case '3':
            move(22,1);
            addstr("  Systematic electronic fraud, in small amounts and across legal boundries that");
            move(23,1);
            addstr("limit the risk of law enforcement. The authorities usually don't bother to");
            move(24,1);
            addstr("investigate cases involving less than $200, at least in theory...");
            break;
         case '4':
            move(22,1);
            addstr("  Extortion of web sites with the threat of launching massive denial of service");
            move(23,1);
            addstr("attacks to effectively knock them offline. Most pay. A few fight.");
            break;
         }
         break;
      case 'd':
         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_MAKE_ARMOR);
         move(10,40);
         addstr("1 - Make Clothing");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_REPAIR_ARMOR);
         move(11,40);
         addstr("2 - Repair Clothing");
         break;
      case 'e':
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(10,40);
         addstr("Teach Liberals About What?");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_TEACH_GENERALED);
         move(12,40);
         addstr("1 - General Education");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_TEACH_POLITICS);
         move(13,40);
         addstr("2 - Politics and Law");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_TEACH_SURVIVAL);
         move(14,40);
         addstr("3 - Street Survival");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_TEACH_FIGHTING);
         move(15,40);
         addstr("4 - Fighting and Killing");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_TEACH_COVERT);
         move(16,40);
         addstr("5 - Covert Actions");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         switch(cr->activity.type)
         {
         case ACTIVITY_TEACH_GENERALED:
            move(22,1);
            addstr("  Attributes Trained: Intelligence");
            move(23,1);
            addstr("  Skills Trained: Computers, Writing, Music, Art, Science, Religion, Business");
            break;
         case ACTIVITY_TEACH_POLITICS:
            move(22,1);
            addstr("  Attributes Trained: Intelligence, Charisma, Heart, Wisdom");
            move(23,1);
            addstr("  Skills Trained: Law, Persuasion, Training, Leadership");
            break;
         case ACTIVITY_TEACH_SURVIVAL:
            move(22,1);
            addstr("  Attributes Trained: Intelligence, Health, Agility");
            move(23,1);
            addstr("  Skills Trained: Driving, First Aid, Cooking, Survival, Street Sense,");
            move(24,1);
            addstr("Garment Making, Hand to Hand, Improvised Weapons");
            break;
         case ACTIVITY_TEACH_FIGHTING:
            move(22,1);
            addstr("  Attributes Trained: Health, Agility, Strength");
            move(23,1);
            addstr("  Skills Trained: Knife, Sword, Club, Pistol, Rifle, SMG, Shotgun, Gangsterism");
            break;
         case ACTIVITY_TEACH_COVERT:
            move(22,1);
            addstr("  Attributes Trained: Intelligence, Agility, Charisma");
            move(23,1);
            addstr("  Skills Trained: Persuasion, Security, Disguise, Stealth, Seduction,");
            move(24,1);
            addstr("Interrogation");
            break;
         }
         break;
      }

      refresh();
      int c=getch();
      translategetch(c);
      if(c>='a'&&c<='z'){state=c;}
      if(c>='1'&&c<='9')
      {
         choice=c;
         switch(state)
         {
         case 'a':
            switch(choice)
            {
            case '1':cr->activity.type=ACTIVITY_COMMUNITYSERVICE;break;
            case '2':cr->activity.type=ACTIVITY_TROUBLE;break;
            case '3':cr->activity.type=ACTIVITY_GRAFFITI;
                     cr->activity.arg=0;break;
            case '4':cr->activity.type=ACTIVITY_POLLS;break;
            case '5':cr->activity.type=ACTIVITY_DOS_ATTACKS;break;
            case '6':cr->activity.type=ACTIVITY_HACKING;break;
            }
            break;
         case 'b':
            switch(choice)
            {
            case '1':cr->activity.type=ACTIVITY_DONATIONS;break;
            case '2':cr->activity.type=ACTIVITY_SELL_TSHIRTS;break;
            case '3':cr->activity.type=ACTIVITY_SELL_ART;break;
            case '4':cr->activity.type=ACTIVITY_SELL_MUSIC;break;
            }
            break;
         case 'c':
            switch(choice)
            {
            case '1':cr->activity.type=ACTIVITY_SELL_DRUGS;break;
            case '2':cr->activity.type=ACTIVITY_PROSTITUTION;break;
            case '3':cr->activity.type=ACTIVITY_CCFRAUD;break;
            case '4':cr->activity.type=ACTIVITY_DOS_RACKET;break;
            }
            break;
         case 'd':
            if(choice=='2')cr->activity.type=ACTIVITY_REPAIR_ARMOR;
            break;
         case 'e':
            switch(choice)
            {
            case '1':cr->activity.type=ACTIVITY_TEACH_GENERALED;break;
            case '2':cr->activity.type=ACTIVITY_TEACH_POLITICS;break;
            case '3':cr->activity.type=ACTIVITY_TEACH_SURVIVAL;break;
            case '4':cr->activity.type=ACTIVITY_TEACH_FIGHTING;break;
            case '5':cr->activity.type=ACTIVITY_TEACH_COVERT;break;
            }
            break;
         }
      }

      if(c=='h'&&cr->skill[SKILL_MEDICAL]!=0)
      {
         cr->activity.type=ACTIVITY_HEAL;
         break;
      }
      if(state=='d'&&choice=='1')
      {
         activityst oact=cr->activity;
         cr->activity.type=ACTIVITY_NONE;
         select_makeclothing(cr);
         if(cr->activity.type==ACTIVITY_MAKE_ARMOR)break;
         else cr->activity=oact;
      }
      if(c=='i'&&hostagecount>0)
      {
         activityst oact=cr->activity;
         cr->activity.type=ACTIVITY_NONE;
         select_tendhostage(cr);
         if(cr->activity.type==ACTIVITY_HOSTAGETENDING)break;
         else cr->activity=oact;
      }
      if(c=='s')
      {
         if(cr->canwalk())
         {
            cr->activity.type=ACTIVITY_STEALCARS;
            break;
         }
         else if(!(cr->flag & CREATUREFLAG_WHEELCHAIR))
         {
            cr->activity.type=ACTIVITY_WHEELCHAIR;
            break;
         }
      }
      if(c=='w'&&location[cr->location]->compound_walls==COMPOUND_PRINTINGPRESS)
      {
         activityst oact=cr->activity;
         cr->activity.type=ACTIVITY_NONE;
         if(select_view(cr,cr->activity.arg))
            cr->activity.type=ACTIVITY_WRITE_GUARDIAN;
         else cr->activity=oact;
         break;
      }
      if(c=='m'&&clinictime(*cr))
      {
         cr->activity.type=ACTIVITY_CLINIC;
         break;
      }
      if(c=='z'&&havedead)
      {
         cr->activity.type=ACTIVITY_BURY;
         break;
      }
      if(c=='x')
      {
         cr->activity.type=ACTIVITY_NONE;
         break;
      }
      if(c==10)
      {
         break;
      }
   }while(1);
}


void activatebulk(void)
{
   vector<creaturest *> temppool;
   int sq;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->alive&&
         pool[p]->align==1&&
         pool[p]->clinic==0&&
         pool[p]->dating==0&&
         pool[p]->hiding==0&&
         !(pool[p]->flag & CREATUREFLAG_SLEEPER))
      {
         if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
            location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
            location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON)
         {
            if(pool[p]->squadid!=-1)
            {
               sq=getsquad(pool[p]->squadid);
               if(sq!=-1)
               {
                  if(squad[sq]->activity.type!=ACTIVITY_NONE)continue;
               }
            }
            temppool.push_back(pool[p]);
         }
      }
   }

   if(temppool.size()==0)return;

   short page=0;

   char str[80];
   char num[20];

   int selectedactivity=0;

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      printfunds(0,1,"Money: ");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Activate Uninvolved Liberals");
      move(1,0);
      addstr("----CODE NAME------------CURRENT ACTIVITY---------------------------------------");
      move(1,51);
      addstr("BULK ACTIVITY");

      if(selectedactivity==0)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(2,51);
      addstr("1 - Causing Trouble ($0).");
      if(selectedactivity==1)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(3,51);
      addstr("2 - Causing Trouble ($20).");
      if(selectedactivity==2)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(4,51);
      addstr("3 - Causing Trouble ($50).");
      if(selectedactivity==3)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(5,51);
      addstr("4 - Causing Trouble ($100).");
      if(selectedactivity==4)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(6,51);
      addstr("5 - Causing Trouble ($500).");
      if(selectedactivity==5)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(7,51);
      addstr("6 - Soliciting Donations.");
      if(selectedactivity==6)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(8,51);
      addstr("7 - Selling Brownies.");
      if(selectedactivity==7)set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(9,51);
      addstr("8 - Stealing Cars.");

      int y=2;
      for(int p=page*19;p<temppool.size()&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         move(y,25);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         getactivity(str,temppool[p]->activity);
         addstr(str);
         if(temppool[p]->activity.type==ACTIVITY_TROUBLE)
         {
            addstr(" ($");
            itoa(temppool[p]->activity.arg,num,10);
            addstr(num);
            addstr(")");
         }

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to Assign an Activity.  Press a Number to select an Activity.");
      move(23,0);
      if(interface_pgup=='[')
      {
         addstr("[] to view other Liberal pages.");
      }
      
         else if(interface_pgup=='.')
         {
            addstr("; and : to view other Liberal pages");
         }
      else
      {
         addstr("PGUP/PGDN to view other Liberal pages.");
      }

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if(c==interface_pgup&&page>0)page--;
      //PAGE DOWN
      if(c==interface_pgdn&&(page+1)*19<temppool.size())page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<temppool.size())
         {
            switch(selectedactivity)
            {
               case 0:
                  temppool[p]->activity.type=ACTIVITY_TROUBLE;
                  temppool[p]->activity.arg=0;
                  break;
               case 1:
                  temppool[p]->activity.type=ACTIVITY_TROUBLE;
                  temppool[p]->activity.arg=20;
                  break;
               case 2:
                  temppool[p]->activity.type=ACTIVITY_TROUBLE;
                  temppool[p]->activity.arg=50;
                  break;
               case 3:
                  temppool[p]->activity.type=ACTIVITY_TROUBLE;
                  temppool[p]->activity.arg=100;
                  break;
               case 4:
                  temppool[p]->activity.type=ACTIVITY_TROUBLE;
                  temppool[p]->activity.arg=500;
                  break;
               case 5:
                  temppool[p]->activity.type=ACTIVITY_DONATIONS;
                  break;
               case 6:
                  temppool[p]->activity.type=ACTIVITY_SELL_DRUGS;
                  break;
               case 7:
                  temppool[p]->activity.type=ACTIVITY_STEALCARS;
                  break;
            }
         }
      }
      if(c>='1'&&c<='8')
      {
         selectedactivity=c-'1';
      }

      if(c==10)break;
   }while(1);
}


/* base - activate - hostages */
void select_tendhostage(creaturest *cr)
{
   vector<creaturest *> temppool;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->align!=1&&
         pool[p]->alive)
      {
         temppool.push_back(pool[p]);
      }
   }

   if(temppool.size()==0)return;

   short page=0;

   char num[20];

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Which hostage will ");
      addstr(cr->name);
      addstr(" be watching over?");
      move(1,0);
      addstr("----CODE NAME------------SKILL---HEALTH---LOCATION------------------------------");
      move(1,57);
      addstr("DAYS IN CAPTIVITY");

      int y=2;
      for(int p=page*19;p<temppool.size()&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         char bright=0;
         unsigned long skill=0;
         for(int sk=0;sk<SKILLNUM;sk++)
         {
            skill+=(unsigned long)temppool[p]->skill[sk];
            if(temppool[p]->skill_ip[sk]>=100+(10*temppool[p]->skill[sk])&&
               temppool[p]->skill[sk]<maxskill(sk,*temppool[p]))bright=1;
         }

         set_color(COLOR_WHITE,COLOR_BLACK,bright);

         move(y,25);
         itoa(skill,num,10);
         addstr(num);

         printhealthstat(*temppool[p],y,33,TRUE);

         if(mode==REVIEWMODE_JUSTICE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,42);
         addstr(location[temppool[p]->location]->shortname);

         move(y,57);
         set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         char num[20];
         itoa(temppool[p]->joindays,num,10);
         addstr(num);
         addstr(" ");
         if(temppool[p]->joindays>1)addstr("Days");
         else addstr("Day");

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Conservative");
      move(23,0);
      if(interface_pgup=='[')
      {
         addstr("[] to view other Liberal pages.");
      }
         else if(interface_pgup=='.')
         {
            addstr("; and : to view other Liberal pages.");
         }
      else
      {
         addstr("PGUP/PGDN to view other Liberal pages.");
      }

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if(c==interface_pgup&&page>0)page--;
      //PAGE DOWN
      if(c==interface_pgdn&&(page+1)*19<temppool.size())page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<temppool.size())
         {
            cr->activity.type=ACTIVITY_HOSTAGETENDING;
            cr->activity.arg=temppool[p]->id;
            return;
         }
      }

      if(c==10)break;
   }while(1);
}


long select_hostagefundinglevel(creaturest *cr,creaturest *hs)
{
   long flevel=-1;

   erase();
   
   

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   printfunds(0,1,"Money: ");
   move(0,0);
   addstr("Select a Funding Level for this Operation:");

   move(2,0);
   addstr("A - Don't spend anything.  ");
   addstr(cr->name);
   addstr(" is just on watch duty.");

   move(3,0);
   addstr("B - Don't spend anything.  ");
   addstr(cr->name);
   addstr(" will turn the prisoner over time.");

   move(4,0);
   addstr("C - $20 per day.  Enough for some props.");

   move(5,0);
   addstr("D - $50 per day.  ");
   addstr(cr->name);
   addstr(" will go for a thrifty indoctrination.");

   move(6,0);
   addstr("E - $100 per day.  ");
   addstr(cr->name);
   addstr(" needs enough freedom to be creative.");

   move(7,0);
   addstr("F - $500 per day.  It is imperative that the Conservative be turned quickly.");

   move(8,0);
   addstr("K - This Conservative has become a liability and needs to be terminated.");

   move(10,0);
   addstr("Enter - On second thought, this isn't a job for ");
   addstr(cr->name);
   addstr(".");

   refresh();
   int c=getch();
   translategetch(c);

   if(c=='a')flevel=0;
   if(c=='b')flevel=1;
   if(c=='c')flevel=20;
   if(c=='d')flevel=50;
   if(c=='e')flevel=100;
   if(c=='f')flevel=500;
   if(c=='k')flevel=666;

   return flevel;
}



/* base - activate - make clothing */
void select_makeclothing(creaturest *cr)
{
   vector<int> armortype;
   for(int a=0;a<ARMORNUM;a++)
   {
      switch(a)
      {
         case ARMOR_NONE:
         case ARMOR_MITHRIL:
         case ARMOR_MASK:
         case ARMOR_BALLISTICVEST:
         case ARMOR_HEAVYBALLISTICVEST:
            break;
         default:
            armortype.push_back(a);
            break;
      }
   }

   short page=0;

   char str[200];
   char num[20];

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Which will ");
      addstr(cr->name);
      addstr(" try to make?   (Note: Half Cost if you have cloth)");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(1,0);
      addstr("----NAME-----------------------------DIFFICULTY-------------COST----------------");

      int y=2,difficulty;
      for(int p=page*19;p<armortype.size()&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         getarmorfull(str,armortype[p]);
         addstr(str);

         move(y,37);
         difficulty=armor_makedifficulty(armortype[p],cr);
         switch(difficulty)
         {
            case 0:
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               addstr("Simple");
               break;
            case 1:
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               addstr("Very Easy");
               break;
            case 2:
               set_color(COLOR_CYAN,COLOR_BLACK,0);
               addstr("Easy");
               break;
            case 3:
               set_color(COLOR_BLUE,COLOR_BLACK,1);
               addstr("Below Average");
               break;
            case 4:
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               addstr("Average");
               break;
            case 5:
               set_color(COLOR_WHITE,COLOR_BLACK,0);
               addstr("Above Average");
               break;
            case 6:
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               addstr("Hard");
               break;
            case 7:
               set_color(COLOR_MAGENTA,COLOR_BLACK,0);
               addstr("Very Hard");
               break;
            case 8:
               set_color(COLOR_MAGENTA,COLOR_BLACK,1);
               addstr("Extremely Difficult");
               break;
            case 9:
               set_color(COLOR_RED,COLOR_BLACK,0);
               addstr("Nearly Impossible");
               break;
            default:
               set_color(COLOR_RED,COLOR_BLACK,1);
               addstr("Impossible");
               break;
         }

         set_color(COLOR_GREEN,COLOR_BLACK,1);
         itoa(armor_makeprice(armortype[p]),num,10);
         move(y,63-strlen(num));
         addch('$');addstr(num);

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Type of Clothing");
      move(23,0);
      if(interface_pgup=='[')
      {
         addstr("[] to view other Liberal pages.");
      }
      
         else if(interface_pgup=='.')
         {
            addstr("; and : to view other Liberal pages.");
         }
      else
      {
         addstr("PGUP/PGDN to view other Liberal pages.");
      }

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if(c==interface_pgup&&page>0)page--;
      //PAGE DOWN
      if(c==interface_pgdn&&(page+1)*19<armortype.size())page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<armortype.size())
         {
            cr->activity.type=ACTIVITY_MAKE_ARMOR;
            cr->activity.arg=armortype[p];
            return;
         }
      }

      if(c==10)break;
   }while(1);
}



long armor_makedifficulty(int type,creaturest *cr)
{
   long basedif;

   switch(type)
   {
      case ARMOR_TOGA:
      case ARMOR_WIFEBEATER:
         basedif=2;
         break;
      case ARMOR_CLOTHES:
      case ARMOR_OVERALLS:
      case ARMOR_WORKCLOTHES:
         basedif=3;
         break;
      case ARMOR_CLOWNSUIT:
      case ARMOR_PRISONER:
      case ARMOR_CHEAPDRESS:
      case ARMOR_TRENCHCOAT:
      case ARMOR_LABCOAT:
      case ARMOR_BLACKROBE:
      case ARMOR_BONDAGEGEAR:
         basedif=4;
         break;
      case ARMOR_SECURITYUNIFORM:
      case ARMOR_PRISONGUARD:
      case ARMOR_MILITARY:
      case ARMOR_POLICEUNIFORM:
         basedif=5;
         break;
      case ARMOR_CHEAPSUIT:
         basedif=6;
         break;
      case ARMOR_BLACKSUIT:
      case ARMOR_BLACKDRESS:
         basedif=7;
         break;
      case ARMOR_EXPENSIVESUIT:
      case ARMOR_EXPENSIVEDRESS:
         basedif=9;
         break;
      default:
         basedif=10;
         break;
   }

   basedif-=cr->skill[SKILL_GARMENTMAKING]-3;
   if(basedif<0)basedif=0;

   return basedif;
}



long armor_makeprice(int type)
{
   long price=0;

   switch(type)
   {
      case ARMOR_TOGA:
      case ARMOR_WIFEBEATER:
         price=5;
         break;
      case ARMOR_CLOTHES:
      case ARMOR_OVERALLS:
      case ARMOR_WORKCLOTHES:
         price=10;
         break;
      case ARMOR_CLOWNSUIT:
      case ARMOR_PRISONER:
      case ARMOR_CHEAPDRESS:
      case ARMOR_TRENCHCOAT:
      case ARMOR_LABCOAT:
      case ARMOR_BLACKROBE:
         price=20;
         break;
      case ARMOR_BONDAGEGEAR:
         price=30;
         break;
      case ARMOR_SECURITYUNIFORM:
      case ARMOR_PRISONGUARD:
      case ARMOR_MILITARY:
      case ARMOR_POLICEUNIFORM:
         price=40;
         break;
      case ARMOR_CHEAPSUIT:
         price=50;
         break;
      case ARMOR_BLACKSUIT:
      case ARMOR_BLACKDRESS:
         price=60;
         break;
      case ARMOR_EXPENSIVESUIT:
      case ARMOR_EXPENSIVEDRESS:
         price=300;
         break;
   }

   return price;
}



/* base - activate - trouble */
long select_troublefundinglevel(creaturest *cr)
{
   long flevel=-1;

   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   printfunds(0,1,"Money: ");
   move(0,0);
   addstr("Select a Funding Level for this Operation:");

   move(2,0);
   addstr("A - Don't spend anything.  ");
   addstr(cr->name);
   addstr(" just needs something constructive to do.");

   move(3,0);
   addstr("B - $20 per day.  Some minor purchases are needed.");

   move(4,0);
   addstr("C - $50 per day.  Disobedience costs money.");

   move(5,0);
   addstr("D - $100 per day.  Enough to be really disobedient.");

   move(6,0);
   addstr("E - $500 per day.  The Machine will buckle under the weight of");
   move(7,0);
   addstr("    ");
   addstr(cr->name);
   addstr("'s Numerous and Varied Liberal Acts.");

   move(9,0);
   addstr("Enter - On second thought, this isn't a job for ");
   addstr(cr->name);
   addstr(".");

   refresh();
   int c=getch();
   translategetch(c);

   if(c=='a')flevel=0;
   if(c=='b')flevel=20;
   if(c=='c')flevel=50;
   if(c=='d')flevel=100;
   if(c=='e')flevel=500;

   return flevel;
}



/* base - activate - select a topic to write about */
char select_view(creaturest *cr,long &v)
{
   int page=0;
   char str[80];

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Write a news story if the LCS makes the news on the selected topic today, or");
      move(1,0);
      addstr("write editorials if there is no current news but there is public interest.");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(2,0);
      addstr("----TOPIC-----------------------------------INTEREST---------------------------");

      int y=3,x=0;
      for(int p=page*18;p<VIEWNUM-2&&p<page*18+18;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,x);
         addch((p-page*18)+'A');addstr(" - ");
         getview(str,p);
         addstr(str);

         move(y,44);
         if(public_interest[p]>100)
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr("Extremely Controvertial");
         }
         else if(public_interest[p]>50)
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            addstr("Dinner Table Topic");
         }
         else if(public_interest[p]>10)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr("Significant Interest");
         }
         else if(public_interest[p]>0)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr("Minor Discussion");
         }
         else
         {
            set_color(COLOR_BLACK,COLOR_BLACK,1);
            addstr("Exhausted");
         }

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Topic");
      move(23,0);
      if(interface_pgup=='[')
      {
         addstr("[] to view other Liberal pages.");
      }
      else if(interface_pgup=='.')
      {
         addstr("; and : to view other Liberal pages.");
      }
      else
      {
         addstr("PGUP/PGDN to view other Liberal pages.");
      }

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if(c==interface_pgup&&page>0)page--;
      //PAGE DOWN
      if(c==interface_pgdn&&(page+1)*16<VIEWNUM-2)page++;

      if(c>='a'&&c<='a'+18)
      {
         int p=page*18+(int)(c-'a');
         if(p<VIEWNUM-2)
         {
            v=p;
            return 1;
         }
      }

      if(c==10)break;
   }while(1);

   return 0;
}

