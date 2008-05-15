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



/* news - determines the priority of a news story */
void setpriority(newsstoryst &ns)
{
   switch(ns.type)
   {
      case NEWSSTORY_MAJOREVENT:
         ns.priority=30000;
         break;
      case NEWSSTORY_SQUAD_SITE:
      case NEWSSTORY_SQUAD_ESCAPED:
      case NEWSSTORY_SQUAD_FLEDATTACK:
      case NEWSSTORY_SQUAD_DEFENDED:
      case NEWSSTORY_SQUAD_BROKESIEGE:
      case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
      case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
      case NEWSSTORY_SQUAD_KILLED_SITE:
      case NEWSSTORY_CARTHEFT:
      {
         ns.priority=0;

         long crime[CRIMENUM];
         memset(crime,0,CRIMENUM*sizeof(long));
         for(int c=0;c<ns.crime.size();c++)
         {
            crime[ns.crime[c]]++;
         }
         if(crime[CRIME_STOLEGROUND]>10)crime[CRIME_STOLEGROUND]=10;
         if(crime[CRIME_BROKEDOWNDOOR]>10)crime[CRIME_BROKEDOWNDOOR]=10;
         if(crime[CRIME_ATTACKED_MISTAKE]>10)crime[CRIME_ATTACKED_MISTAKE]=10;
         if(crime[CRIME_ATTACKED]>10)crime[CRIME_ATTACKED]=10;
         if(crime[CRIME_BREAK_SWEATSHOP]>10)crime[CRIME_BREAK_SWEATSHOP]=10;
         if(crime[CRIME_BREAK_FACTORY]>10)crime[CRIME_BREAK_FACTORY]=10;
         if(crime[CRIME_FREE_RABBITS]>10)crime[CRIME_FREE_RABBITS]=10;
         if(crime[CRIME_FREE_BEASTS]>10)crime[CRIME_FREE_BEASTS]=10;

         ns.priority+=crime[CRIME_STOLEGROUND];
         ns.priority+=crime[CRIME_BROKEDOWNDOOR];
         ns.priority+=crime[CRIME_ATTACKED_MISTAKE]*7;
         ns.priority+=crime[CRIME_ATTACKED]*4;
         ns.priority+=crime[CRIME_KILLEDSOMEBODY]*30;
         ns.priority+=crime[CRIME_SHUTDOWNREACTOR]*100;
         ns.priority+=crime[CRIME_POLICE_LOCKUP]*30;
         ns.priority+=crime[CRIME_COURTHOUSE_LOCKUP]*30;
         ns.priority+=crime[CRIME_PRISON_RELEASE]*50;
         ns.priority+=crime[CRIME_JURYTAMPERING]*30;
         ns.priority+=crime[CRIME_HACK_INTEL]*100;
         ns.priority+=crime[CRIME_BREAK_SWEATSHOP]*2;
         ns.priority+=crime[CRIME_BREAK_FACTORY]*2;
         ns.priority+=crime[CRIME_HOUSE_PHOTOS]*100;
         ns.priority+=crime[CRIME_CORP_FILES]*100;
         ns.priority+=crime[CRIME_FREE_RABBITS]*2;
         ns.priority+=crime[CRIME_FREE_BEASTS]*3;

         switch(ns.type)
         {
            case NEWSSTORY_SQUAD_ESCAPED:
               ns.priority+=10+attitude[VIEW_LIBERALCRIMESQUAD]/3;
               break;
            case NEWSSTORY_SQUAD_FLEDATTACK:
               ns.priority+=15+attitude[VIEW_LIBERALCRIMESQUAD]/3;
               break;
            case NEWSSTORY_SQUAD_DEFENDED:
               ns.priority+=30+attitude[VIEW_LIBERALCRIMESQUAD]/3;
               break;
            case NEWSSTORY_SQUAD_BROKESIEGE:
               ns.priority+=45+attitude[VIEW_LIBERALCRIMESQUAD]/3;
               break;
            case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
               ns.priority+=10+attitude[VIEW_LIBERALCRIMESQUAD]/3;
               break;
            case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
               ns.priority+=15+attitude[VIEW_LIBERALCRIMESQUAD]/3;
               break;
            case NEWSSTORY_SQUAD_KILLED_SITE:
               ns.priority+=10+attitude[VIEW_LIBERALCRIMESQUAD]/3;
               break;
         }

         if(ns.priority>20000)ns.priority=20000;
         break;
      }
      case NEWSSTORY_KIDNAPREPORT:
         ns.priority=20;
         if(ns.cr->type==CREATURE_CORPORATE_CEO||
            ns.cr->type==CREATURE_RADIOPERSONALITY||
            ns.cr->type==CREATURE_NEWSANCHOR||
            ns.cr->type==CREATURE_SCIENTIST_EMINENT||
            ns.cr->type==CREATURE_JUDGE_CONSERVATIVE)ns.priority=40;
         break;
      case NEWSSTORY_MASSACRE:
         ns.priority=10 + ns.crime[1]*5;
         break;
      case NEWSSTORY_CCS_SITE:
      case NEWSSTORY_CCS_KILLED_SITE:
      
         ns.crime.push_back(CRIME_BROKEDOWNDOOR);
         ns.priority=1;
         if(ns.positive==0)
         {
            ns.crime.push_back(CRIME_ATTACKED_MISTAKE);
            ns.priority+=7;
         }
         ns.crime.push_back(CRIME_ATTACKED);
         ns.priority+=4*(LCSrandom(10)+1);
         if(LCSrandom(endgamestate+1))
         {
            ns.crime.push_back(CRIME_KILLEDSOMEBODY);
            ns.priority+=LCSrandom(10)*30;
         }
         if(LCSrandom(endgamestate+1))
         {
            ns.crime.push_back(CRIME_STOLEGROUND);
            ns.priority+=LCSrandom(10);
         }
         if(!LCSrandom(endgamestate+4))
         {
            ns.crime.push_back(CRIME_BREAK_FACTORY);
            ns.priority+=LCSrandom(10)*2;
         }
         if(LCSrandom(2))
         {
            ns.crime.push_back(CRIME_CARCHASE);
         }
         break;
      case NEWSSTORY_CCS_DEFENDED:
      case NEWSSTORY_CCS_KILLED_SIEGEATTACK:
         ns.priority=40+attitude[VIEW_LIBERALCRIMESQUAD]/3;
         break;
   }
}



/* news - show major news story */
void displaystory(newsstoryst &ns,bool liberalguardian,int header)
{
   int it2;
   for(int x=0;x<80;x++)
   {
      for(int y=0;y<25;y++)
      {
         move(y,x);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addch(CH_FULL_BLOCK);
      }
   }

   if(ns.page==1||(liberalguardian&&ns.guardianpage==1))
   {
      //TOP
      int pap=LCSrandom(5);
      for(int x=0;x<80;x++)
      {
         for(int y=0;y<5;y++)
         {
            move(y,x);
            if(liberalguardian)
            {
               set_color(newstops[5][x][y][1],
                         newstops[5][x][y][2],
                         newstops[5][x][y][3]);
               addch(translateGraphicsChar(newstops[5][x][y][0]));
            }
            else
            {
               set_color(newstops[pap][x][y][1],
                         newstops[pap][x][y][2],
                         newstops[pap][x][y][3]);

               addch(translateGraphicsChar(newstops[pap][x][y][0]));
            }
         }
      }

      if(!liberalguardian) // Liberal Guardian graphics don't support adding a date
      {
         //DATE
         set_color(COLOR_BLACK,COLOR_WHITE,0);
         char num[20];
         if(day>=10)move(3,66);
         else move(3,67);
         switch(month)
         {
            case 1:addstr("Jan");break;
            case 2:addstr("Feb");break;
            case 3:addstr("Mar");break;
            case 4:addstr("Apr");break;
            case 5:addstr("May");break;
            case 6:addstr("Jun");break;
            case 7:addstr("Jul");break;
            case 8:addstr("Aug");break;
            case 9:addstr("Sep");break;
            case 10:addstr("Oct");break;
            case 11:addstr("Nov");break;
            case 12:addstr("Dec");break;
         }
         addstr(". ");
         itoa(day,num,10);
         addstr(num);
         addstr(", ");
         itoa(year,num,10);
         addstr(num);
      }
   }
   else
   {
      //PAGE
      set_color(COLOR_BLACK,COLOR_WHITE,0);
      char num[20];
      if(!liberalguardian)itoa(ns.page,num,10);
      else itoa(ns.guardianpage,num,10);
      move(0,76);
      addstr(num);
   }

   char story[5000];
   short storyx_s[25];
   short storyx_e[25];
   for(it2=0;it2<25;it2++)storyx_s[it2]=1;
   for(it2=0;it2<25;it2++)storyx_e[it2]=78;
   int adnumber=0;
   if(!liberalguardian)
   {
      if(ns.page>=10)adnumber++;
      if(ns.page>=20)adnumber+=LCSrandom(2)+1;
      if(ns.page>=30)adnumber+=LCSrandom(2)+1;
      if(ns.page>=40)adnumber+=LCSrandom(2)+1;
      if(ns.page>=50)adnumber+=LCSrandom(2)+1;
   }
   else
   {
      if(ns.guardianpage>=3)adnumber++;
      if(ns.guardianpage>=6)adnumber+=LCSrandom(2)+1;
      if(ns.guardianpage>=9)adnumber+=LCSrandom(2)+1;
      if(ns.guardianpage>=12)adnumber+=LCSrandom(2)+1;
      if(ns.guardianpage>=15)adnumber+=LCSrandom(2)+1;
   }
   char addplace[2][3]={0,0,0,0,0,0};
   if(adnumber>6)adnumber=6;
   while(adnumber>0)
   {
      int x,y;
      do
      {
         x=LCSrandom(2);y=LCSrandom(3);
      }while(addplace[x][y]);
      addplace[x][y]=1;
      adnumber--;

      int sx=0,ex=0,sy=0,ey=0;
      if(x==0)
      {
         sx=0;
         ex=23+LCSrandom(4);
      }
      if(x==1)
      {
         sx=57-LCSrandom(4);
         ex=79;
      }
      if(y==0)
      {
         sy=2;
         ey=9;
      }
      if(y==1)
      {
         sy=10;
         ey=17;
      }
      if(y==2)
      {
         sy=18;
         ey=24;
      }

      unsigned int ch='?';
      switch(LCSrandom(6))
      {
         case 0:ch=CH_LIGHT_SHADE;break;
         case 1:ch=CH_MEDIUM_SHADE;break;
         case 2:ch=CH_DARK_SHADE;break;
         case 3:ch=CH_FULL_BLOCK;break;
         case 4:ch=CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_HORIZONTAL;break;
         case 5:ch='*';break;
      }

      for(y=sy;y<=ey;y++)
      {
         if(storyx_s[y]<ex&&ex<43)storyx_s[y]=ex+2;
         if(storyx_e[y]>sx&&sx>43)storyx_e[y]=sx-2;

         if(ey==9&&y==9)continue;
         if(ey==17&&y==17)continue;
         for(x=sx;x<=ex;x++)
         {
            if(y==sy||y==8||y==16||y==24||x==sx||x==ex)
            {
               move(y,x);
               addch(ch);
            }
         }
      }

      //AD CONTENT
      short storyx_s[25];
      short storyx_e[25];
      for(it2=0;it2<25;it2++)storyx_s[it2]=40;
      for(it2=0;it2<25;it2++)storyx_e[it2]=40;
      for(it2=sy+1;it2<=ey-1;it2++)storyx_s[it2]=sx+1;
      for(it2=sy+1;it2<=ey-1;it2++)storyx_e[it2]=ex-1;

      char ad[500];
      if(!liberalguardian)
      {
         switch(LCSrandom(5))
         {
            case 0:
               strcpy(ad,"&cNo Fee&r");
               strcat(ad,"&cConsignment Program&r&r");
               strcat(ad,"&cCall for Details&r");
               break;
            case 1:
               strcpy(ad,"&cFine Leather Chairs&r&r");
               strcat(ad,"&cSpecial Purchase&r");
               strcat(ad,"&cNow $599&r");
               break;
            case 2:
               strcpy(ad,"&cParis Flea Market&r&r");
               strcat(ad,"&cSale&r");
               strcat(ad,"&c50% Off&r");
               break;
            case 3:
               strcpy(ad,"&cQuality Pre-Owned&r");
               strcat(ad,"&cVehicles&r");
               strcat(ad,"&c1999 Lexus GS 300&r");
               strcat(ad,"&cSedan 4D&r");
               strcat(ad,"&cOnly $21,988&r");
               break;
            case 4:
               strcpy(ad,"&cSpa&r");
               strcat(ad,"&cHealth, Beauty&r");
               strcat(ad,"&cand Fitness&r&r");
               strcat(ad,"&c7 Days a Week&r");
               break;
         }
      }
      else
      {
         switch(2) // Liberal Guardian Ads
         {
         case 0:
            strcpy(ad,"&cWant Organic?&r&r");
            strcat(ad,"&cVisit The Vegan&r");
            strcat(ad,"&cCo-Op&r");
            break;
         case 1:
            strcpy(ad,"&cLiberal Defense Lawyer&r");
            strcat(ad,"&c26 Years Experience&r&r");
            strcat(ad,"&cCall Today&r");
            break;
         }
      }

      displaynewsstory(ad,storyx_s,storyx_e,sy+1);
   }

   switch(ns.type)
   {
      case NEWSSTORY_MAJOREVENT:
         if(ns.positive)
         {
            switch(ns.view)
            {
               case VIEW_ABORTION:
                  displaycenterednewsfont("CLINIC MURDER",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
               case VIEW_GAY:
                  displaycenterednewsfont("CRIME OF HATE",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
               case VIEW_DEATHPENALTY:
                  displaycenterednewsfont("JUSTICE DEAD",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
                  /*
               case VIEW_GUNS:
                  displaycenterednewsfont("SCHOOL SHOOTING",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
                  */
               case VIEW_TAXES:
               {
                  displaycenterednewsfont("REAGAN FLAWED",5);
                  char str[80];
                  strcpy(str,"");
                  switch(LCSrandom(5))
                  {
                     case 0:strcat(str,"Shadow");break;
                     case 1:strcat(str,"Dark");break;
                     case 2:strcat(str,"Abyssal");break;
                     case 3:strcat(str,"Orwellian");break;
                     case 4:strcat(str,"Craggy");break;
                  }
                  strcat(str," ");
                  switch(LCSrandom(5))
                  {
                     case 0:strcat(str,"Actor");break;
                     case 1:strcat(str,"Lord");break;
                     case 2:strcat(str,"Emperor");break;
                     case 3:strcat(str,"Puppet");break;
                     case 4:strcat(str,"Dementia");break;
                  }
                  strcat(str,": A new book further documenting the other side of Reagan.");
                  displaycenteredsmallnews(str,12);
                  displaynewspicture(2,13);
                  break;
               }
               case VIEW_NUCLEARPOWER:
                  displaycenterednewsfont("MELTDOWN",5);
                  displaycenteredsmallnews("A nuclear power plant suffers a catastrophic meltdown.",12);
                  displaynewspicture(3,13);
                  break;
               case VIEW_ANIMALRESEARCH:
                  displaycenterednewsfont("HELL ON EARTH",5);
                  displaycenteredsmallnews("A mutant animal has escaped from a lab and killed thirty people.",12);
                  displaynewspicture(0,13);
                  break;
               case VIEW_PRISONS:
                  displaycenterednewsfont("ON THE INSIDE",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
               case VIEW_INTELLIGENCE:
                  displaycenterednewsfont("THE FBI FILES",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
               case VIEW_FREESPEECH:
                  displaycenterednewsfont("BOOK BANNED",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
               case VIEW_GENETICS:
                  displaycenterednewsfont("KILLER FOOD",5);
                  displaycenteredsmallnews("Over a hundred people become sick from genetically modified food.",12);
                  displaynewspicture(4,13);
                  break;
               case VIEW_JUSTICES:
                  displaycenterednewsfont("IN CONTEMPT",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
               case VIEW_SWEATSHOPS:
                  displaycenterednewsfont("CHILD'S PLEA",5);
                  displaycenteredsmallnews("A T-shirt in a store is found scrawled with a message from a sweatshop worker.",12);
                  displaynewspicture(12,13);
                  break;
               case VIEW_POLLUTION:
                  displaycenterednewsfont("RING OF FIRE",5);
                  displaycenteredsmallnews("The Ohio River caught on fire again.",12);
                  displaynewspicture(5,13);
                  break;
               case VIEW_CORPORATECULTURE:
                  displaycenterednewsfont("BELLY UP",5);
                  displaycenteredsmallnews("An enormous company files for bankruptcy, shattering the previous record.",12);
                  displaynewspicture(6,13);
                  break;
               case VIEW_CEOSALARY:
               {
                  displaycenterednewsfont("AMERICAN CEO",5);
                  char str[80];
         strcpy(str,"This major CEO ");
         switch(LCSrandom(10))
         {
            case 0:
               strcat(str,"regularly visits prostitutes.");
               break;
            case 1:
               strcat(str,"seeks the aid of psychics.");
               break;
            case 2:
               strcat(str,"donated millions to the KKK.");
               break;
            case 3:
               strcat(str,"evaded paying taxes.");
               break;
            case 4:
               strcat(str,"took out a contract on his wife.");
               break;
            case 5:
               strcat(str,"doesn't know what his company does.");
               break;
            case 6:
               strcat(str,"has a zoo of imported exotic worms.");
               break;
            case 7:
               strcat(str,"paid millions for high-tech bondage gear.");
               break;
            case 8:
               strcat(str,"installed a camera in an office bathroom.");
               break;
            case 9:
               strcat(str,"owns slaves in another country.");
               break;
         }
                  displaycenteredsmallnews(str,12);
                  displaynewspicture(1,13);
                  break;
               }
               case VIEW_AMRADIO:
                  displaycenterednewsfont("AM IMPLOSION",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
            }
         }
         else
         {
            switch(ns.view)
            {
               case VIEW_GAY:
                  displaycenterednewsfont("KINKY WINKY",5);
                  displaycenteredsmallnews("Jerry Falwell explains the truth about Tinky Winky.  Again.",12);
                  displaynewspicture(7,13);
                  break;
               case VIEW_DEATHPENALTY:
                  displaycenterednewsfont("LET'S FRY 'EM",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
                  /*
               case VIEW_GUNS:
                  displaycenterednewsfont("KILLER STOPPED",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
                  */
               case VIEW_TAXES:
               {
                  displaycenterednewsfont("REAGAN THE MAN",5);
                  char str[80];
                  strcpy(str,"");
                  switch(LCSrandom(5))
                  {
                     case 0:strcat(str,"Great");break;
                     case 1:strcat(str,"Noble");break;
                     case 2:strcat(str,"True");break;
                     case 3:strcat(str,"Pure");break;
                     case 4:strcat(str,"Golden");break;
                  }
                  strcat(str," ");
                  switch(LCSrandom(5))
                  {
                     case 0:strcat(str,"Leadership");break;
                     case 1:strcat(str,"Courage");break;
                     case 2:strcat(str,"Pioneer");break;
                     case 3:strcat(str,"Communicator");break;
                     case 4:strcat(str,"Faith");break;
                  }
                  strcat(str,": A new book lauding Reagan and the greatest generation.");
                  displaycenteredsmallnews(str,12);
                  displaynewspicture(2,13);
                  break;
               }
               case VIEW_NUCLEARPOWER:
                  displaycenterednewsfont("OIL CRUNCH",5);
                  displaycenteredsmallnews("OPEC cuts oil production sharply in response to a US foreign policy decision.",12);
                  displaynewspicture(8,13);
                  break;
               case VIEW_ANIMALRESEARCH:
                  displaycenterednewsfont("APE EXPLORERS",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
               case VIEW_POLICEBEHAVIOR:
                  displaycenterednewsfont("BASTARDS",5);
                  displaynewspicture(9,13);
                  break;
               case VIEW_PRISONS:
                  displaycenterednewsfont("HOSTAGE SLAIN",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
               case VIEW_INTELLIGENCE:
                  displaycenterednewsfont("DODGED BULLET",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
               case VIEW_FREESPEECH:
                  displaycenterednewsfont("HATE RALLY",5);
                  displaycenteredsmallnews("Free speech advocates fight hard to let a white supremacist rally take place.",12);
                  displaynewspicture(10,13);
                  break;
               case VIEW_GENETICS:
                  displaycenterednewsfont("GM FOOD FAIRE",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
               case VIEW_JUSTICES:
                  displaycenterednewsfont("JUSTICE AMOK",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
               case VIEW_SWEATSHOPS:
                  displaycenterednewsfont("THEY ARE HERE",5);
                  if(month>=8&&month<=11)displaycenteredsmallnews("Fall fashions hit the stores across the country.",12);
                  else displaycenteredsmallnews("Fall fashions are previewed in stores across the country.",12);
                  displaynewspicture(11,13);
                  break;
               case VIEW_POLLUTION:
                  displaycenterednewsfont("LOOKING UP",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
               case VIEW_CORPORATECULTURE:
                  displaycenterednewsfont("NEW JOBS",5);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,13);
                  break;
               case VIEW_AMRADIO:
                  displaycenterednewsfont("THE DEATH",5);
                  displaycenterednewsfont("OF CULTURE",13);
                  constructeventstory(story,ns.view,ns.positive);
                  displaynewsstory(story,storyx_s,storyx_e,21);
                  break;
            }
         }
         break;
      case NEWSSTORY_SQUAD_SITE:
      case NEWSSTORY_SQUAD_ESCAPED:
      case NEWSSTORY_SQUAD_FLEDATTACK:
      case NEWSSTORY_SQUAD_DEFENDED:
      case NEWSSTORY_SQUAD_BROKESIEGE:
      case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
      case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
      case NEWSSTORY_SQUAD_KILLED_SITE:
      case NEWSSTORY_CCS_SITE:
      case NEWSSTORY_CCS_KILLED_SITE:
      case NEWSSTORY_CARTHEFT:
      {
         int y=2;
         if(ns.page==1||(liberalguardian&&ns.guardianpage==1))
         {
            y=21;

            switch(ns.type)
            {
               case NEWSSTORY_CARTHEFT:
                  displaycenterednewsfont("POLICE KILLED",5);
                  y=13;
                  break;
               case NEWSSTORY_SQUAD_ESCAPED:
               case NEWSSTORY_SQUAD_FLEDATTACK:
                  displaycenterednewsfont("LCS ESCAPES",5);
                  displaycenterednewsfont("POLICE SIEGE",13);
                  break;
               case NEWSSTORY_SQUAD_DEFENDED:
               case NEWSSTORY_SQUAD_BROKESIEGE:
                  displaycenterednewsfont("LCS FIGHTS",5);
                  displaycenterednewsfont("OFF COPS",13);
                  break;
               case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
               case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
                  if(!liberalguardian)
                  {
                     displaycenterednewsfont("LCS SIEGE",5);
                     displaycenterednewsfont("TRAGIC END",13);
                  }
                  else
                  {
                     displaycenterednewsfont("POLICE KILL",5);
                     displaycenterednewsfont("LCS MARTYRS",13);
                  }
                  break;
               case NEWSSTORY_CCS_SITE:
               case NEWSSTORY_CCS_KILLED_SITE:
                  if(ns.positive)
                     displaycenterednewsfont("CCS STRIKES",5);
                  else
                     displaycenterednewsfont("CCS RAMPAGE",5);
                  y=13;
                  break;
               default:
                  if(ns.positive)
                  {
                     if(newscherrybusted||liberalguardian)
                     {
                        
                        if(!liberalguardian)
                        {
                           if(ns.priority>150)
                           {
                              displaycenterednewsfont("LIBERAL CRIME",5);
                              displaycenterednewsfont("SQUAD STRIKES",13);
                           }
                           else
                           {
                              y=13;
                              displaycenterednewsfont("LCS STRIKES",5);
                           }
                        }
                        else
                        {
                           y=13;
                           if(ns.priority>150)
                           {
                              change_public_opinion(header,5,1); // Bonus for big story
                              switch(header)
                              {
                              case VIEW_TAXES:
                              case VIEW_SWEATSHOPS:
                              case VIEW_CEOSALARY:
                                 displaycenterednewsfont("CLASS WAR",5);
                                 break;
                              case VIEW_NUCLEARPOWER:
                                 displaycenterednewsfont("NO NUKE POWER",5);
                                 break;
                              case VIEW_POLICEBEHAVIOR:
                                 displaycenterednewsfont("LCS COPS WAR",5);
                                 break;
                              case VIEW_PRISONS:
                                 displaycenterednewsfont("PRISON WAR",5);
                                 break;
                              case VIEW_INTELLIGENCE:
                                 displaycenterednewsfont("LCS CIA WAR",5);
                                 break;
                              case VIEW_ANIMALRESEARCH:
                              case VIEW_GENETICS:
                                 displaycenterednewsfont("EVIL RESEARCH",5);
                                 break;
                              case VIEW_FREESPEECH:
                              case VIEW_ABORTION:
                              case VIEW_GAY:
                              case VIEW_JUSTICES:
                                 displaycenterednewsfont("NO JUSTICE",5);
                                 break;
                              case VIEW_POLLUTION:
                                 displaycenterednewsfont("POLLUTER HIT",5);
                                 break;
                              case VIEW_CORPORATECULTURE:
                                 displaycenterednewsfont("LCS HITS CORP",5);
                                 break;
                              case VIEW_AMRADIO:
                                 displaycenterednewsfont("LCS HITS AM",5);
                                 break;
                              case VIEW_CABLENEWS:
                                 displaycenterednewsfont("LCS HITS TV",5);
                                 break;
                              default:
                                 displaycenterednewsfont("HEROIC STRIKE",5);
                              }
                           }
                           else
                           {
                              displaycenterednewsfont("LCS STRIKES",5);
                           }
                        }
                     }
                     else
                     {
                        displaycenterednewsfont("LIBERAL CRIME",5);
                        displaycenterednewsfont("SQUAD STRIKES",13);
                     }
                  }
                  else
                  {
                     if(newscherrybusted||liberalguardian)
                     {
                        if(!liberalguardian)
                           displaycenterednewsfont("LCS RAMPAGE",5);
                        else
                           displaycenterednewsfont("LCS SORRY",5);
                        y=13;
                     }
                     else
                     {
                        displaycenterednewsfont("LIBERAL CRIME",5);
                        displaycenterednewsfont("SQUAD RAMPAGE",13);
                     }
                  }
                  break;
            }
         }

         strcpy(story,lcityname);
         strcat(story," - ");

         switch(ns.type)
         {
            case NEWSSTORY_CARTHEFT:
            {
               int crime[CRIMENUM];
               std::memset(crime,0,sizeof(int)*CRIMENUM);
               for(int c=0;c<ns.crime.size();c++)
               {
                  crime[ns.crime[c]]++;
               }

               strcat(story,"A routine arrest went horribly wrong yesterday, ");
               strcat(story,"according to a spokesperson from the police department.");
               strcat(story,"&r");
               strcat(story,"  A suspect, whose identify is unclear, ");
               strcat(story,"killed ");
               if(crime[CRIME_KILLEDSOMEBODY]>1)
               {
                  char num[20];
                  itoa(crime[CRIME_KILLEDSOMEBODY],num,10);
                  strcat(story,num);
                  strcat(story," police officers");
               }
               else strcat(story,"a police officer");
               strcat(story," that were attempting to perform an arrest.  A passerby had allegedly spotted the suspect committing a car theft.  ");
               strcat(story,"The names of the officers have not been released pending notification of their families.");
               strcat(story,"&r");
               break;
            }
            case NEWSSTORY_SQUAD_ESCAPED:
               strcat(story,"Members of the Liberal Crime Squad ");
               strcat(story,"escaped from a police siege yesterday, according ");
               if(!liberalguardian)strcat(story,"to a spokesperson from the police department.");
               else strcat(story,"to a Liberal Crime Squad spokesperson.");
               strcat(story,"&r");
               break;
            case NEWSSTORY_SQUAD_FLEDATTACK:
               strcat(story,"Members of the Liberal Crime Squad ");
               strcat(story,"escaped from police officers during a raid yesterday, according ");
               if(!liberalguardian)strcat(story,"to a spokesperson from the police department.");
               else strcat(story,"to a Liberal Crime Squad spokesperson.");
               strcat(story,"&r");
               break;
            case NEWSSTORY_SQUAD_DEFENDED:
               strcat(story,"Members of the Liberal Crime Squad ");
               strcat(story,"fought off a police raid yesterday, according ");
               if(!liberalguardian)strcat(story,"to a spokesperson from the police department.");
               else strcat(story,"to a Liberal Crime Squad spokesperson.");
               strcat(story,"&r");
               break;
            case NEWSSTORY_SQUAD_BROKESIEGE:
               strcat(story,"Members of the Liberal Crime Squad ");
               strcat(story,"violently broke a police siege yesterday, according ");
               if(!liberalguardian)strcat(story,"to a spokesperson from the police department.");
               else strcat(story,"to a Liberal Crime Squad spokesperson.");
               strcat(story,"&r");
               break;
            case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
               strcat(story,"Members of the Liberal Crime Squad were ");
               if(!liberalguardian)
               {
                  strcat(story,"slain during a police raid yesterday, according ");
                  strcat(story,"to a spokesperson from the police department.");
               }
               else
               {
                  strcat(story,"murdered during a police raid yesterday, according ");
                  strcat(story,"to a Liberal Crime Squad spokesperson.");
               }
               strcat(story,"&r");
               break;
            case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
               strcat(story,"Members of the Liberal Crime Squad were ");
               if(!liberalguardian)
               {
                  strcat(story,"slain trying to escape from a police siege yesterday, according ");
                  strcat(story,"to a spokesperson from the police department.");
               }
               else
               {
                  strcat(story,"murdered trying to escape from a police siege yesterday, according ");
                  strcat(story,"to a Liberal Crime Squad spokesperson.");
               }
               strcat(story,"&r");
               break;
            default:
            {
               int ccs=0;
               if(ns.type==NEWSSTORY_SQUAD_SITE)
               {
                  if(!newscherrybusted&&!liberalguardian)
                  {
                     if(ns.positive)
                     {
                        strcat(story,"A group calling itself the Liberal Crime Squad ");
                        strcat(story,"burst onto the scene of political activism yesterday, according ");
                        strcat(story,"to a spokesperson from the police department.");
                        strcat(story,"&r");
                     }
                     else
                     {
                        strcat(story,"A group of thugs calling itself the Liberal Crime Squad ");
                        strcat(story,"went on a rampage yesterday, according ");
                        strcat(story,"to a spokesperson from the police department.");
                     }
                  }
                  else
                  {
                     if(ns.positive)
                     {
                        strcat(story,"The Liberal Crime Squad has struck again.  ");
                        strcat(story,"&r");
                     }
                     else
                     {
                        if(!liberalguardian)
                           strcat(story,"The Liberal Crime Squad has gone on a rampage.  ");
                        else
                           strcat(story,"A Liberal Crime Squad operation went horribly wrong.  ");
                        strcat(story,"&r");
                     }
                  }
               }
               else if(ns.type==NEWSSTORY_CCS_SITE)
               {
                  if(newscherrybusted<2)
                  {
                     if(ns.positive&&!liberalguardian)
                     {
                        strcat(story,"A group of M16-wielding vigilantes calling itself the Conservative Crime Squad ");
                        strcat(story,"burst onto the scene of political activism yesterday, according ");
                        strcat(story,"to a spokesperson from the police department.");
                        strcat(story,"&r");
                     }
                     else
                     {
                        strcat(story,"A group of worthless M16-toting hicks calling itself the Conservative Crime Squad ");
                        strcat(story,"went on a rampage yesterday, according ");
                        strcat(story,"to a spokesperson from the police department.");
                     }
                  }
                  else
                  {
                     if(ns.positive&&!liberalguardian)
                     {
                        strcat(story,"The Conservative Crime Squad has struck again.  ");
                        strcat(story,"&r");
                     }
                     else
                     {
                        strcat(story,"The Conservative Crime Squad has gone on a rampage.  ");
                        strcat(story,"&r");
                     }
                  }
               }
               else if(ns.type==NEWSSTORY_CCS_KILLED_SITE)
               {
                  if(newscherrybusted<2)
                  {
                     if(ns.positive&&!liberalguardian)
                     {
                        strcat(story,"A group of M16-wielding vigilantes calling itself the Conservative Crime Squad ");
                        strcat(story,"burst briefly onto the scene of political activism yesterday, according ");
                        strcat(story,"to a spokesperson from the police department.  ");
                        strcat(story,"&r");
                     }
                     else
                     {
                        strcat(story,"A group of worthless M16-toting hicks calling itself the Conservative Crime Squad ");
                        strcat(story,"went on a suicidal rampage yesterday, according ");
                        strcat(story,"to a spokesperson from the police department.  ");
                        strcat(story,"&r");
                     }
                  }
                  else
                  {
                     if(ns.positive&&!liberalguardian)
                     {
                        strcat(story,"The Conservative Crime Squad has struck again, albeit with a tragic end.  ");
                        strcat(story,"&r");
                     }
                     else
                     {
                        strcat(story,"The Conservative Crime Squad has gone on a rampage, and they got what they deserved.  ");
                        strcat(story,"&r");
                     }
                  }
               }
               else
               {
                  if(!newscherrybusted&&!liberalguardian)
                  {
                     if(ns.positive)
                     {
                        strcat(story,"A group calling itself the Liberal Crime Squad ");
                        strcat(story,"burst briefly onto the scene of political activism yesterday, according ");
                        strcat(story,"to a spokesperson from the police department.  ");
                        strcat(story,"&r");
                     }
                     else
                     {
                        strcat(story,"A group of thugs calling itself the Liberal Crime Squad ");
                        strcat(story,"went on a suicidal rampage yesterday, according ");
                        strcat(story,"to a spokesperson from the police department.  ");
                        strcat(story,"&r");
                     }
                  }
                  else
                  {
                     if(ns.positive)
                     {
                        strcat(story,"The Liberal Crime Squad has struck again, albeit with a tragic end.  ");
                        strcat(story,"&r");
                     }
                     else
                     {
                        if(!liberalguardian)
                           strcat(story,"The Liberal Crime Squad has gone on a rampage, and they got what they deserved.  ");
                        else
                           strcat(story,"A Liberal Crime Squad operation went horribly wrong, and came to a tragic end.  ");
                        strcat(story,"&r");
                     }
                  }
               }
               if(ns.type==NEWSSTORY_CCS_KILLED_SITE||
                  ns.type==NEWSSTORY_CCS_SITE)
                  ccs=1;

               strcat(story,"  The events took place at the ");
               if(liberalguardian&&!ccs)strcat(story,"notorious ");
               if(ccs)
               {
                  switch(location[ns.loc]->type)
                  {
                  case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
                     strcat(story,"University Dormitory.  ");break;
                  case SITE_BUSINESS_CIGARBAR:
                     strcat(story,"Lady Luck Strip Club.  ");break;
                  case SITE_LABORATORY_COSMETICS:
                     strcat(story,"Animal Shelter.  ");break;
                  case SITE_LABORATORY_GENETIC:
                     strcat(story,"Research Ethics Commission HQ.  ");break;
                  case SITE_GOVERNMENT_POLICESTATION:
                     strcat(story,"Seedy Back Alley(tm).  ");break;
                  case SITE_GOVERNMENT_COURTHOUSE:
                     strcat(story,"Abortion Clinic.  ");break;
                  case SITE_GOVERNMENT_PRISON:
                     strcat(story,"Ace Ghetto Pool Hall.  ");break;
                  case SITE_GOVERNMENT_INTELLIGENCEHQ:
                     strcat(story,"ACLU Branch Office.  ");break;
                  case SITE_INDUSTRY_SWEATSHOP:
                     strcat(story,"Labor Union HQ.  ");break;
                  case SITE_INDUSTRY_POLLUTER:
                     strcat(story,"Greenpeace Offices.  ");break;
                  case SITE_INDUSTRY_NUCLEAR:
                     strcat(story,"Whirled Peas Museum.  ");break;
                  case SITE_CORPORATE_HEADQUARTERS:
                     strcat(story,"Welfare Assistance Agency.  ");break;
                  case SITE_CORPORATE_HOUSE:
                     strcat(story,"Food Bank.  ");break;
                  case SITE_MEDIA_AMRADIO:
                     strcat(story,"Public Radio Station.  ");break;
                  case SITE_MEDIA_CABLENEWS:
                     strcat(story,"Network News Station.  ");break;
                  default:
                     strcat(story,location[ns.loc]->name);
                     strcat(story,".  ");break;
                  }
               }
               else strcat(story,location[ns.loc]->name);
               if(liberalguardian&&!ccs)
               {
                  switch(location[ns.loc]->type)
                  {
                  case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
                     strcat(story,", known for its rich and snooty residents.  ");break;
                  case SITE_BUSINESS_CIGARBAR:
                     strcat(story,", a spawning ground of Wrong Conservative Ideas.  ");break;
                  case SITE_LABORATORY_COSMETICS:
                     strcat(story,", a Conservative animal rights abuser.  ");break;
                  case SITE_LABORATORY_GENETIC:
                     strcat(story,", a dangerous Conservative genetic research lab.  ");break;
                  case SITE_GOVERNMENT_POLICESTATION:
                     strcat(story,", headquarters of one of the most oppressive and Conservative police forces in the country.  ");break;
                  case SITE_GOVERNMENT_COURTHOUSE:
                     strcat(story,", site of numerous Conservative Injustices.  ");break;
                  case SITE_GOVERNMENT_PRISON:
                     strcat(story,", where innocent people are regularly beaten by Conservative guards.  ");break;
                  case SITE_GOVERNMENT_INTELLIGENCEHQ:
                     strcat(story,", the Conservative headquarters of one of the biggest privacy violators in the world.  ");break;
                  case SITE_INDUSTRY_SWEATSHOP:
                     strcat(story,", a Conservative sweatshop and human rights abuser.  ");break;
                  case SITE_INDUSTRY_POLLUTER:
                     strcat(story,", a factory whose Conservative smokestacks choke the city with deadly pollutants.  ");break;
                  case SITE_INDUSTRY_NUCLEAR:
                     strcat(story,", also known to be a Conservative storage facility for radioactive waste.  ");break;
                  case SITE_CORPORATE_HEADQUARTERS:
                     strcat(story,", where evil and Conservatism coagulate in the hallways.  ");break;
                  case SITE_CORPORATE_HOUSE:
                     strcat(story,", a building with enough square footage enough to house a hundred people if it weren't in Conservative Hands.  ");break;
                  case SITE_MEDIA_AMRADIO:
                  case SITE_MEDIA_CABLENEWS:
                     strcat(story,", known for its Extreme Conservative Bias.  ");break;
                  default:
                     strcat(story,".  ");break;
                  }
               }
               else if(!ccs)
                  strcat(story,".  ");

               if(ns.type==NEWSSTORY_SQUAD_KILLED_SITE)
               {
                  if(liberalguardian)strcat(story,"Unfortunately, the LCS group was defeated by the forces of evil.  ");
                  else if(ns.positive)strcat(story,"Everyone in the LCS group was arrested or killed.  ");
                  else strcat(story,"Fortunately, the LCS thugs were stopped by brave citizens.  ");
               }
               if(ns.type==NEWSSTORY_CCS_KILLED_SITE)
               {
                  if(ns.positive&&!liberalguardian)strcat(story,"Everyone in the CCS group was arrested or killed.  ");
                  else strcat(story,"Fortunately, the CCS thugs were stopped by brave citizens.  ");
               }
               strcat(story,"&r");

               int crime[CRIMENUM];
               memset(crime,0,sizeof(int)*CRIMENUM);
               int typesum=0;
               for(int c=0;c<ns.crime.size();c++)
               {
                  crime[ns.crime[c]]++;

                  if(ns.crime[c]==CRIME_HOUSE_PHOTOS)continue;
                  if(ns.crime[c]==CRIME_CORP_FILES)continue;
                  if(ns.crime[c]==CRIME_SHUTDOWNREACTOR)continue;
                  if(ns.crime[c]==CRIME_POLICE_LOCKUP)continue;
                  if(ns.crime[c]==CRIME_COURTHOUSE_LOCKUP)continue;
                  if(ns.crime[c]==CRIME_PRISON_RELEASE)continue;
                  if(ns.crime[c]==CRIME_JURYTAMPERING)continue;
                  if(ns.crime[c]==CRIME_HACK_INTEL)continue;
                  if(ns.crime[c]==CRIME_HOUSE_PHOTOS)continue;
                  if(ns.crime[c]==CRIME_CORP_FILES)continue;
                  if(ns.crime[c]==CRIME_CARCHASE)continue;
                  if(ns.crime[c]==CRIME_CARCRASH)continue;
                  if(ns.crime[c]==CRIME_FOOTCHASE)continue;
                  if(crime[ns.crime[c]]==1)typesum++;
               }

               if(crime[CRIME_SHUTDOWNREACTOR])
               {
                  if(!liberalguardian)
                  {
                     strcat(story,"  Accorded to sources that were at the scene, ");
                     strcat(story,"the Liberal Crime Squad caused the power out that struck the state ");
                     strcat(story,"yesterday by tampering with equipment on the site.");
                     strcat(story,"&r");
                  }
                  else
                  {
                     strcat(story,"  The Liberal Crime Squad caused the power outage that struck the state yesterday, ");
                     strcat(story,"demonstrating the extreme vulnerability and danger of Nuclear Power Plants. ");
                     strcat(story,"&r");
                  }
               }
               if(crime[CRIME_POLICE_LOCKUP])
               {
                  if(!liberalguardian)
                  {
                     strcat(story,"  Accorded to sources that were at the scene, ");
                     strcat(story,"the Liberal Crime Squad allegedly freed or attempted to free prisoners from the police lockup.");
                     strcat(story,"&r");
                  }
                  else
                  {
                     strcat(story,"  The Liberal Crime Squad attempted to rescue innocent people from the police lockup, ");
                     strcat(story,"saving them from torture and brutality at the hands of Conservative police interrogators.");
                     strcat(story,"&r");
                  }
               }
               if(crime[CRIME_COURTHOUSE_LOCKUP])
               {
                  if(!liberalguardian)
                  {
                     strcat(story,"  Accorded to sources that were at the scene, ");
                     strcat(story,"the Liberal Crime Squad allegedly freed or attempted to free prisoners from the court house lockup.");
                     strcat(story,"&r");
                  }
                  else
                  {
                     strcat(story,"  The Liberal Crime Squad attempted to rescue innocent people from the court house lockup, ");
                     strcat(story,"saving them from the highly corrupt Conservative justice system.");
                     strcat(story,"&r");
                  }
               }
               if(crime[CRIME_PRISON_RELEASE])
               {
                  if(!liberalguardian)
                  {
                     strcat(story,"  Accorded to sources that were at the scene, ");
                     strcat(story,"the Liberal Crime Squad allegedly freed prisoners while in the facility.");
                     strcat(story,"&r");
                  }
                  else
                  {
                     strcat(story,"  The Liberal Crime Squad attempted to rescue innocent people from the Conservative conditions ");
                     strcat(story,"at the prison.");
                     strcat(story,"&r");
                  }
               }
               if(crime[CRIME_JURYTAMPERING])
               {
                  if(!liberalguardian)
                  {
                     strcat(story,"  Accorded to police sources that were at the scene, ");
                     strcat(story,"the Liberal Crime Squad allegedly violated the sacred ");
                     strcat(story,"trust and attempted to influence a jury.");
                     strcat(story,"&r");
                  }
                  else
                  {
                     strcat(story,"  The Liberal Crime Squad has apologized over reports that the operation ");
                     strcat(story,"interfered with jury deliberations.");
                     strcat(story,"&r");
                  }
               }
               if(crime[CRIME_HACK_INTEL])
               {
                  if(!liberalguardian)
                  {
                     strcat(story,"  Accorded to police sources that were at the scene, ");
                     strcat(story,"intelligence officials seemed very nervous about something.");
                     strcat(story,"&r");
                  }
                  else
                  {
                     strcat(story,"  Liberal Crime Squad hackers worked to penetrate CIA computers.");
                     strcat(story,"&r");
                  }
               }
               if(crime[CRIME_HOUSE_PHOTOS])
               {
                  if(!liberalguardian)
                  {
                     strcat(story,"  Accorded to police sources that were at the scene, ");
                     strcat(story,"the owner of the house seemed very frantic about some missing property.");
                     strcat(story,"&r");
                  }
                  else
                  {
                     strcat(story,"  The Liberal Crime Squad was attempting to uncover the CEO's Conservative corruption.");
                     strcat(story,"&r");
                  }
               }
               if(crime[CRIME_CORP_FILES])
               {
                  if(!liberalguardian)
                  {
                     strcat(story,"  Accorded to police sources that were at the scene, ");
                     strcat(story,"executives on the scene seemed very nervous about something.");
                     strcat(story,"&r");
                  }
                  else
                  {
                     strcat(story,"  The Liberal Crime Squad was attempting to uncover the company's Conservative corruption.");
                     strcat(story,"&r");
                  }
               }

               if(liberalguardian)
               {
                  if(crime[CRIME_ATTACKED_MISTAKE])typesum--;
                  if(crime[CRIME_KILLEDSOMEBODY])typesum--;
               }

               if(typesum>0)
               {
                  if(!ccs)
                  {
                     if(!liberalguardian)
                     {
                        strcat(story,"  Further details are sketchy, but police sources suggest that the LCS ");
                        strcat(story,"engaged in ");
                     }
                     else
                     {
                        strcat(story,"  The Liberal Crime Squad ");
                     }
                  }
                  else
                  {
                     strcat(story,"  Further details are sketchy, but police sources suggest that the CCS ");
                        strcat(story,"engaged in ");
                  }
                  if(!liberalguardian||ccs)
                  {
                     if(crime[CRIME_KILLEDSOMEBODY])
                     {
                        strcat(story,"murder");
                        if(typesum>=3)strcat(story,", ");
                        else if(typesum==2)strcat(story," and ");
                        typesum--;
                     }
                     if(crime[CRIME_ATTACKED_MISTAKE])
                     {
                        strcat(story,"violence");
                        if(typesum>=3)strcat(story,", ");
                        else if(typesum==2)strcat(story," and ");
                        typesum--;
                     }
                     if(crime[CRIME_ATTACKED])
                     {
                        if(crime[CRIME_ATTACKED_MISTAKE])strcat(story,"more violence");
                        else strcat(story,"violence");
                        if(typesum>=3)strcat(story,", ");
                        else if(typesum==2)strcat(story," and ");
                        typesum--;
                     }
                  }
                  else
                  {
                     if(crime[CRIME_ATTACKED])
                     {
                        strcat(story,"engaged in combat with Conservative forces");
                        if(typesum>=3)strcat(story,", ");
                        else if(typesum==2)strcat(story," and ");
                        typesum--;
                     }
                  }
                  if(crime[CRIME_STOLEGROUND])
                  {
                     if(!liberalguardian||ccs)
                     {
                        strcat(story,"theft");
                     }
                     else
                     {
                        strcat(story,"gained control of enemy equipment");
                     }
                     if(typesum>=3)strcat(story,", ");
                     else if(typesum==2)strcat(story," and ");
                     typesum--;
                  }
                  if(crime[CRIME_FREE_RABBITS]||crime[CRIME_FREE_BEASTS])
                  {
                     if(!liberalguardian)
                     {
                        strcat(story,"tampering with lab animals");
                     }
                     else
                     {
                        strcat(story,"liberated abused lab animals");
                     }

                     if(typesum>=3)strcat(story,", ");
                     else if(typesum==2)strcat(story," and ");
                     typesum--;
                  }
                  if(crime[CRIME_BREAK_SWEATSHOP]||crime[CRIME_BREAK_FACTORY])
                  {
                     if(!liberalguardian||ccs)
                     {
                        strcat(story,"destruction of private property");
                     }
                     else
                     {
                        strcat(story,"damaged enemy infrastructure");
                     }

                     if(typesum>=3)strcat(story,", ");
                     else if(typesum==2)strcat(story," and ");
                     typesum--;
                  }
                  if(crime[CRIME_BROKEDOWNDOOR])
                  {
                     if(!liberalguardian||ccs)
                     {
                        strcat(story,"breaking and entering");
                     }
                     else
                     {
                        strcat(story,"forced entry into a secure area");
                     }

                     if(typesum>=3)strcat(story,", ");
                     else if(typesum==2)strcat(story," and ");
                     typesum--;
                  }
                  if(crime[CRIME_UNLOCKEDDOOR])
                  {
                     if(!liberalguardian||ccs)
                     {
                        strcat(story,"unlawful entry");
                     }
                     else
                     {
                        strcat(story,"evaded Conservative security measures");
                     }

                     if(typesum>=3)strcat(story,", ");
                     else if(typesum==2)strcat(story," and ");
                     typesum--;
                  }
                  strcat(story,".&r");
               }

               if(crime[CRIME_CARCHASE])
               {
                  if(!liberalguardian||ccs)
                  {
                     strcat(story,"  It is known that there was a high-speed chase ");
                     strcat(story,"following the incident.  ");
                  }
                  else
                  {
                     strcat(story,"  Conservative operatives engaged in a dangerous ");
                     strcat(story,"pursuit of the LCS.  ");
                  }

                  if(crime[CRIME_CARCRASH])
                  {
                     if(crime[CRIME_CARCRASH]>1)
                     {
                        char num[20];
                        itoa(crime[CRIME_CARCRASH],num,10);
                        strcat(story," vehicles crashed.  ");
                     }
                     else strcat(story,"One vehicle crashed.  ");
                     if(!liberalguardian||ccs)
                        strcat(story,"Details about injuries were not released.  ");
                  }

                  if(crime[CRIME_FOOTCHASE])
                  {
                     if(!liberalguardian||ccs)
                        strcat(story,"There was also a foot chase when the suspect or suspects bailed out after the high-speed pursuit.  ");
                     else
                        strcat(story,"The Liberal Crime Squad ended the dangerous high-speed chase to protect the public, and attempted to escape on foot.  ");
                  }
                  strcat(story,"&r");
               }

               break;
            }
         }

         generatefiller(story,200);
         displaynewsstory(story,storyx_s,storyx_e,y);

         if(!newscherrybusted)newscherrybusted=1;
         if(ns.type==NEWSSTORY_CCS_SITE||
            ns.type==NEWSSTORY_CCS_KILLED_SITE)newscherrybusted=2;
         break;
      }
      case NEWSSTORY_MASSACRE:
      {
         int y=3;
         if(ns.page==1)
         {
            y=21;
            if(!liberalguardian)
            {
               displaycenterednewsfont("MYSTERIOUS",5);
               displaycenterednewsfont("MASSACRE",13);
            }
            else
            {
               displaycenterednewsfont("CONSERVATIVE",5);
               displaycenterednewsfont("MASSACRE",13);
            }
         }

         strcpy(story,lcityname);
         strcat(story," - ");
         if(ns.crime[1]>2)
         {
            char num[20];
            itoa(ns.crime[1],num,10);
            strcat(story,num);
            strcat(story," bodies were ");
         }
         else if(ns.crime[1]>1)strcat(story," Two bodies were ");
         else strcat(story," A body was ");
         strcat(story," found in the ");
         strcat(story,location[ns.loc]->name);
         strcat(story," yesterday.");
         if(!liberalguardian)
         {
            strcat(story,"  According to a spokesperson for ");
            strcat(story,"the police department, the matter is under investigation as a homicide.");
            strcat(story,"&r");
            strcat(story,"  Privately, sources in the department confide that there aren't any leads.  ");
            strcat(story,"According to one person familiar with the case, \"");
         }
         else
         {
            strcat(story,"  The police have opened an investigation into the massacre, but seem ");
            strcat(story,"unwilling to pursue the case with all of their effort.");
            strcat(story,"&r");
            strcat(story,"  The Liberal Crime Squad has claimed that the ");
            if(ns.crime[1]>1)strcat(story,"victims were ");
            else strcat(story,"victim was ");
            strcat(story,"a member of the LCS targeted simply due to their affiliation.  ");
            strcat(story,"According to an LCS spokesperson, \"");
         }
         switch(ns.crime[0])
         {
            case SIEGE_CIA:
               if(!liberalguardian)
               {
                  if(ns.crime[1]>1)strcat(story,"The bodies had no faces or ");
                  else strcat(story,"The body had no face or ");
                  strcat(story,"fingerprints.  Like, it was all smooth.  ");
                  strcat(story,"D*mnd*st thing I've ever seen");
               }
               else
               {
                  strcat(story,"We have strong evidence that this was an extra-judicial slaughter ");
                  strcat(story,"carried out by the Central Intelligence Agency in retaliation for our ");
                  strcat(story,"previous actions to uncover human rights abuses and corruption in the ");
                  strcat(story,"intelligene community");
               }
               break;
            case SIEGE_POLICE:
            case SIEGE_HICKS:
               if(!liberalguardian)
               {
                  strcat(story,"Burned...  stabbed with, maybe, pitchforks.  There may have been bite marks.  Nothing recognizable left");
               }
               else
               {
                  strcat(story,"We have reason to believe that this brutal massacre was ");
                  strcat(story,"inspired by the Conservative media's brainwashing propaganda");
               }
               break;
            case SIEGE_CORPORATE:
               if(!liberalguardian)
               {
                  strcat(story,"It was execution style.  Professional.  We've got nothing");
               }
               else
               {
                  strcat(story,"This massacre has the signature mark of a group of mercenaries ");
                  strcat(story,"known to work with several corporations we've had confrotations ");
                  strcat(story,"with in the past.  If the police can't figure this one out, they're ");
                  strcat(story,"just covering it up");
               }
               break;
         }
         strcat(story,".\"  ");
         strcat(story,"&r");

         generatefiller(story,200);
         displaynewsstory(story,storyx_s,storyx_e,y);
         break;
      }
      case NEWSSTORY_KIDNAPREPORT:
      {
         int y=2;
         if(ns.page==1)
         {
            y=21;
            if(liberalguardian)
            {
               displaycenterednewsfont("LCS DENIES",5);
               displaycenterednewsfont("KIDNAPPING",13);break;
            }
            else
            {
               switch(ns.cr->type)
               {
                  case CREATURE_CORPORATE_CEO:
                     displaycenterednewsfont("CEO",5);
                     displaycenterednewsfont("KIDNAPPED",13);break;
                  case CREATURE_RADIOPERSONALITY:
                     displaycenterednewsfont("RADIO HOST",5);
                     displaycenterednewsfont("KIDNAPPED",13);break;
                  case CREATURE_NEWSANCHOR:
                     displaycenterednewsfont("NEWS ANCHOR",5);
                     displaycenterednewsfont("KIDNAPPED",13);break;
                  case CREATURE_SCIENTIST_EMINENT:
                     displaycenterednewsfont("SCIENTIST",5);
                     displaycenterednewsfont("KIDNAPPED",13);break;
                  case CREATURE_JUDGE_CONSERVATIVE:
                     displaycenterednewsfont("JUDGE",5);
                     displaycenterednewsfont("KIDNAPPED",13);break;
                  case CREATURE_COP:
                  case CREATURE_GANGUNIT:
                  case CREATURE_DEATHSQUAD:
                     displaycenterednewsfont("COP",5);
                     displaycenterednewsfont("KIDNAPPED",13);break;
                  default:
                     displaycenterednewsfont("PERSON",5);
                     displaycenterednewsfont("KIDNAPPED",13);break;
               }
            }
         }

         strcpy(story,lcityname);
         strcat(story," - The disappearance of ");
         strcat(story,ns.cr->propername);
         strcat(story," is now considered a kidnapping, ");
         strcat(story,"accorded to a police spokesperson.");
         strcat(story,"&r");

         char dstr[200],dstr2[200];
         strcat(story,"  ");
         firstname(dstr);lastname(dstr2);
         strcat(story,dstr);
         strcat(story," ");
         strcat(story,dstr2);
         strcat(story,", speaking on behalf of the police department, stated ");
         strcat(story,"\"We now believe that ");
         strcat(story,ns.cr->propername);
         strcat(story," was taken ");
         char num[20];
         itoa(ns.cr->joindays-1,num,10);
         strcat(story,num);
         strcat(story," days ago, by a person or persons as yet undetermined.  ");
         strcat(story,"We have several leads and are confident that we will ");
         strcat(story,"bring ");
         strcat(story,ns.cr->propername);
         strcat(story," back home and bring the kidnappers to justice.  ");
         strcat(story,"As the investigation is ongoing, I cannot be more specific at this time.  ");
         strcat(story,"To the citizens, please contact the department if you have any additional information.");
         strcat(story,"\"");
         strcat(story,"&r");
         strcat(story,"  According to sources, ");
         strcat(story,ns.cr->propername);
         strcat(story,"'s last known location was the ");
         strcat(story,location[ns.cr->worklocation]->name);
         strcat(story,".  Police were seen searching the surrounding area yesterday.");
         strcat(story,"&r");

         generatefiller(story,200);
         displaynewsstory(story,storyx_s,storyx_e,y);
         break;
      }
   }

   int c;
   do
   {
      refresh();
      c=getch();
      translategetch(c);
   }while(c!=27&&c!=10&&c!=32);
}



/* news - graphics */
void loadgraphics(void)
{
   unsigned long picnum,dimx,dimy;

   unsigned int numbytes;
   FILE *h;

   h=LCSOpenFile("largecap.cpc", "rb", LCSIO_PRE_ART);
   if(h!=NULL)
   {

      numbytes=fread(&picnum,sizeof(unsigned long),1,h);
      numbytes=fread(&dimx,sizeof(unsigned long),1,h);
      numbytes=fread(&dimy,sizeof(unsigned long),1,h);
      for(int p=0;p<picnum;p++)
      {
         for(int x=0;x<dimx;x++)
         {
            for(int y=0;y<dimy;y++)
            {
               numbytes=fread(&bigletters[p][x][y][0],sizeof(unsigned char),4,h);
            }
         }
      }
      LCSCloseFile(h);
   }

   h=LCSOpenFile("newstops.cpc", "rb", LCSIO_PRE_ART);
   if(h!=NULL)
   {

      numbytes=fread(&picnum,sizeof(unsigned long),1,h);
      numbytes=fread(&dimx,sizeof(unsigned long),1,h);
      numbytes=fread(&dimy,sizeof(unsigned long),1,h);
      for(int p=0;p<picnum;p++)
      {
         for(int x=0;x<dimx;x++)
         {
            for(int y=0;y<dimy;y++)
            {
               numbytes=fread(&newstops[p][x][y][0],sizeof(unsigned char),4,h);
            }
         }
      }
      LCSCloseFile(h);
   }


  h=LCSOpenFile("newspic.cpc", "rb", LCSIO_PRE_ART);
   if(h!=NULL)
   {

      numbytes=fread(&picnum,sizeof(unsigned long),1,h);
      numbytes=fread(&dimx,sizeof(unsigned long),1,h);
      numbytes=fread(&dimy,sizeof(unsigned long),1,h);
      for(int p=0;p<picnum;p++)
      {
         for(int x=0;x<dimx;x++)
         {
            for(int y=0;y<dimy;y++)
            {
               numbytes=fread(&newspic[p][x][y][0],sizeof(unsigned char),4,h);
            }
         }
      }
      LCSCloseFile(h);
   }
}



void displaycenterednewsfont(char *str,int y)
{
   int width=-1;
   int s;
   for(s=0;s<strlen(str);s++)
   {
      if(str[s]>='A'&&str[s]<='Z')width+=6;
      else if(str[s]=='\'')width+=4;
      else width+=3;
   }

   int x=39-width/2;

   for(s=0;s<strlen(str);s++)
   {
      if((str[s]>='A'&&str[s]<='Z')||str[s]=='\'')
      {
         int p;
         if(str[s]>='A'&&str[s]<='Z')p=str[s]-'A';
         else p=26;
         int lim=6;
         if(str[s]=='\'')lim=4;
         if(s==strlen(str)-1)lim--;
         for(int x2=0;x2<lim;x2++)
         {
            for(int y2=0;y2<7;y2++)
            {
               move(y+y2,x+x2);

#ifdef NCURSES
               // Clean the square first.
               set_color(COLOR_BLACK, COLOR_BLACK, 0);
               addch(CH_FULL_BLOCK);
               move(y+y2,x+x2);
#endif

               if(x2==5)
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,0);
                  addch(CH_FULL_BLOCK);
               }
               else
               {
                  set_color(bigletters[p][x2][y2][1],
                     bigletters[p][x2][y2][2],
                     bigletters[p][x2][y2][3]);
                  addch(translateGraphicsChar(bigletters[p][x2][y2][0]));
               }
            }
         }
         x+=lim;
      }
      else
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         for(int x2=0;x2<3;x2++)
         {
            for(int y2=0;y2<7;y2++)
            {
               move(y+y2,x+x2);
               addch(CH_FULL_BLOCK);
            }
         }
         x+=3;
      }
   }
}



void displaycenteredsmallnews(char *str,int y)
{
   int x=39-((strlen(str)-1)>>1);
   move(y,x);
   set_color(COLOR_BLACK,COLOR_WHITE,0);
   addstr(str);
}



void displaynewspicture(int p,int y)
{
   for(int x2=0;x2<78;x2++)
   {
      for(int y2=0;y2<15;y2++)
      {
         if(y+y2>24)break;
         move(y+y2,1+x2);
         set_color(newspic[p][x2][y2][1],
            newspic[p][x2][y2][2],
            newspic[p][x2][y2][3]);
         addch(translateGraphicsChar(newspic[p][x2][y2][0]));
      }
   }
}



/* news - constructs non-LCS related event stories */
void constructeventstory(char *story,short view,char positive)
{
   strcpy(story,"");

   if(positive)
   {
      switch(view)
      {
         case VIEW_ABORTION:
         {
            char str[200],str2[200];
            char gen[50];
            cityname(story);
            strcat(story," - ");
            strcat(story,"A doctor that routinely performed abortions was ruthlessly ");
            strcat(story,"gunned down outside of the ");
            lastname(str);strcat(story,str);
            strcat(story," CLINIC yesterday.  ");
            strcat(story,"Dr. ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            char gn=LCSrandom(2);
            switch(gn)
            {
               case 0:strcpy(gen,"his");break;
               case 1:strcpy(gen,"her");break;
            }
            strcat(story," was walking to ");strcat(story,gen);
            strcat(story," car when, according to police reports, ");
            strcat(story,"shots were fired from a nearby vehicle.  ");
            strcat(story,dstr2);
            strcat(story," was hit ");
            char num[20];
            itoa(LCSrandom(15)+3,num,10);
            strcat(story,num);
            strcat(story," times and died immediately in the parking lot.  ");
            strcat(story,"The suspected shooter, ");
            firstname(str);strcat(story,str);
            strcat(story," ");
            lastname(str2);strcat(story,str2);
            strcat(story,", is in custody.&r");
            strcat(story,"  Witnesses report that ");
            strcat(story,str2);
            strcat(story," remained at the scene after the shooting, screaming ");
            strcat(story,"verses of the Bible at the stunned onlookers.  Someone ");
            strcat(story,"called the police on a cellphone and they arrived shortly thereafter.  ");
            strcat(story,str2);
            strcat(story," surrendered without a struggle, reportedly saying that God's work ");
            strcat(story,"had been completed.&r");
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story," is survived by ");
            strcat(story,gen);
            strcat(story," ");
            char spouse=LCSrandom(2);
            if(law[LAW_GAY]<=1)
            {
               spouse=1-gn;
            }
            switch(spouse)
            {
               case 0:strcat(story,"husband");break;
               case 1:strcat(story,"wife");break;
            }
            strcat(story," and ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"two");break;
               case 1:strcat(story,"three");break;
               case 2:strcat(story,"five");break;
            }
            strcat(story," children.&r");
            break;
         }
         case VIEW_GAY:
         {
            cityname(story);
            strcat(story," - ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            if(law[LAW_GAY]<=-1)strcat(story,", a known homosexual, was ");
            else strcat(story,", a homosexual, was ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"dragged to death behind a pickup truck");break;
               case 1:strcat(story,"burned alive");break;
               case 2:strcat(story,"beaten to death");break;
            }
            strcat(story," here yesterday.  ");
            strcat(story,"A police spokesperson reported that four suspects ");
            strcat(story,"were apprehended after a high speed chase.  Their names ");
            strcat(story,"have not yet been released.");
            strcat(story,"&r");
            strcat(story,"  Witnesses of the freeway chase described the pickup of the alleged ");
            strcat(story,"murderers swerving wildly, ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"throwing beer bottles");break;
               case 1:strcat(story,"urinating out the window");break;
               case 2:strcat(story,"taking swipes");break;
            }
            strcat(story," at the pursuing police cruisers.  ");
            strcat(story,"The chase ended when ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"the suspects ran out of gas, ");break;
               case 1:strcat(story,"the suspects collided with a manure truck, ");break;
               case 2:strcat(story,"the suspects veered into a ditch, ");break;
            }
            strcat(story,"at which point they were taken into custody.  Nobody was seriously injured during the incident.");
            strcat(story,"&r");
            strcat(story,"  Authorities have stated that they will vigorously ");
            strcat(story,"prosecute this case as a hate crime, due to the ");
            strcat(story,"aggravated nature of the offense");
            if(law[LAW_GAY]==-2)strcat(story,", even though being gay is deviant, as we all know.");
            else strcat(story,".");
            strcat(story,"&r");
            break;
         }
         case VIEW_DEATHPENALTY:
         {
            strcat(story,"Texas - ");
            strcat(story,"An innocent citizen has been put to death in the electric chair.  ");
            char dstr[200],dstr2[200],dstr3[200];
            firstname(dstr);firstname(dstr2);lastname(dstr3);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," ");
            strcat(story,dstr3);
            strcat(story," was pronounced dead at 7:06PM yesterday at the ");
            char jstr[200];
            lastname(jstr);
            strcat(story,jstr);
            strcat(story," Correctional Facility.&r");

            strcat(story,"  ");
            strcat(story,dstr3);
            strcat(story," was convicted in ");
            char num[20];
            itoa(year-LCSrandom(11)-10,num,10);
            strcat(story,num);
            strcat(story," of thirteen serial murders.  ");
            strcat(story,"Since then, numerous pieces of exculpatory evidence ");
            strcat(story,"have been produced, including ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"a confession from another convict.  ");break;
               case 1:strcat(story,"a battery of negative DNA tests.  ");break;
               case 2:
                  strcat(story,"an admission from a former prosecutor that ");
                  strcat(story,dstr3);
                  strcat(story," was framed.  ");
                  break;
            }
            strcat(story,"The state still went through with the execution, with a ");
            strcat(story,"spokesperson for the governor saying, ");
            strcat(story,"\"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"Let's not forget the convict is colored.  You know how their kind are");break;
               case 1:
                  strcat(story,"The convict is always referred to by three names.  ");
                  strcat(story,"Assassin, serial killer, either way -- guilty.  ");
                  strcat(story,"End of story");
                  break;
               case 2:strcat(story,"The family wants closure.  We don't have time for another trial");break;
            }
            strcat(story,".\"");
            strcat(story,"&r");
            strcat(story,"  Candlelight vigils were held throughout the country last night during the execution, ");
            strcat(story,"and more events are expected this evening.  If there is a bright side to be found from this ");
            strcat(story,"tragedy, it will be that our nation is now evaluating the ease with which people ");
            strcat(story,"can be put to death in this country.");
            strcat(story,"&r");
            break;
         }
        /* case VIEW_GUNS:
         {
            cityname(story);
            strcat(" - ");
            strcat(story,"A student has gone on a shooting rampage at a local high school.  ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story,", 15, used a variety of weapons to shoot more than a dozen classmates and two teachers at ");
            char jstr[200];
            lastname(jstr);
            strcat(story,jstr);
            strcat(story," High School, before committing suicide.&r");

            strcat(story,"  ");
            strcat(story,dstr);
            strcat(story," was considered an outcast ");
            char num[20];
            itoa(year-LCSrandom(11)-10,num,10);
            strcat(story,num);
            strcat(story," of thirteen serial murders.  ");
            strcat(story,"Since then, numerous pieces of exculpatory evidence ");
            strcat(story,"have been produced, including ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"a confession from another convict.  ");break;
               case 1:strcat(story,"a battery of negative DNA tests.  ");break;
               case 2:
                  strcat(story,"an admission from a former prosecutor that ");
                  strcat(story,dstr3);
                  strcat(story," was framed.  ");
                  break;
            }
            strcat(story,"The state still went through with the execution, with a ");
            strcat(story,"spokesperson for the governor saying, ");
            strcat(story,"\"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"Let's not forget the convict is colored.  You know how their kind are");break;
               case 1:
                  strcat(story,"The convict is always referred to by three names.  ");
                  strcat(story,"Assassin, serial killer, either way -- guilty.  ");
                  strcat(story,"End of story");
                  break;
               case 2:strcat(story,"The family wants closure.  We don't have time for another trial");break;
            }
            strcat(story,".\"");
            strcat(story,"&r");
            strcat(story,"  Candlelight vigils were held throughout the country last night during the execution, ");
            strcat(story,"and more events are expected this evening.  If there is a bright side to be found from this ");
            strcat(story,"tragedy, it will be that our nation is now evaluating the ease with which people ");
            strcat(story,"can be put to death in this country.");
            strcat(story,"&r");
            break;
         } */
         case VIEW_PRISONS:
         {
            cityname(story);
            strcat(story," - A former prisoner has written a book describing in horrifying ");
            strcat(story,"detail what goes on behind bars.  ");
            strcat(story,"Although popular culture has used, or perhaps overused, the ");
            strcat(story,"prison theme lately in its offerings for mass consumption, rarely ");
            strcat(story,"have these works been as poignant as ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story,"'s new tour-de-force, _");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Nightmare");break;
               case 1:strcat(story,"Primal");break;
               case 2:strcat(story,"Animal");break;
               case 3:strcat(story,"American");break;
               case 4:strcat(story,"Solitary");break;
            }
            strcat(story,"_");
            switch(LCSrandom(7))
            {
               case 0:strcat(story,"Packer");break;
               case 1:strcat(story,"Soap");break;
               case 2:strcat(story,"Punk");break;
               case 3:strcat(story,"Kid");break;
               case 4:strcat(story,"Cell");break;
               case 5:strcat(story,"Shank");break;
               case 6:strcat(story,"Lockdown");break;
               case 7:strcat(story,"Buttlord");break;
            }
            strcat(story,"_.&r");
            strcat(story,"   Take this excerpt, \"");

            strcat(story,"The steel bars grated forward in their rails, ");
            strcat(story,"coming to a halt with a deafening clang that said it all -- ");
            strcat(story,"I was trapped with them now.  There were three, looking me over ");
            strcat(story,"with dark glares of bare lust, as football players might stare at a stupified, drunken, helpless teenager.  ");
            strcat(story,"My shank's under the mattress.  Better to be a man and fight or a punk and let them take it?  ");
            strcat(story,"Maybe lose an eye the one way, maybe catch AIDS the other.  A helluva choice, ");
            strcat(story,"and I would only have a few seconds before they made it for me");
            strcat(story,".\"");
            strcat(story,"&r");
            break;
         }
         case VIEW_INTELLIGENCE:
         {
            strcat(story,"Washington D.C. - The FBI might be keeping tabs on you.  ");
            strcat(story,"This newspaper yesterday received a collection of files from a source in the Federal Bureau of Investigations.  ");
            strcat(story,"The files contain information on which people have been attending demonstrations, organizing ");
            strcat(story,"unions, working for liberal organizations -- even ");
            switch(LCSrandom(2))
            {
               case 0:strcat(story,"buying music with 'Explicit Lyrics' labels.");break;
               case 1:strcat(story,"helping homeless people");break;
            }
            strcat(story,".");
            strcat(story,"&r");
            strcat(story,"  More disturbingly, the files make reference to a plan to ");
            strcat(story,"\"deal with the undesirables\", although this phrase is not clarified.  ");
            strcat(story,"&r");
            strcat(story,"  The FBI refused to comment initially, but when confronted with the information, ");
            strcat(story,"a spokesperson stated, \"");
            strcat(story,"Well, you know, there's privacy, and there's privacy.  ");
            strcat(story,"It might be a bit presumptive to assume that ");
            strcat(story,"these files deal with the one and not the other.  ");
            strcat(story,"You think about that before you continue slanging accusations");
            strcat(story,".\"");
            strcat(story,"&r");
            break;
         }
         case VIEW_FREESPEECH:
         {
            cityname(story);
            strcat(story," - A children's story has been removed from libraries here after ");
            strcat(story,"the city bowed to pressure from religious groups.");
            strcat(story,"&r");
            strcat(story,"   The book, ");
            strcat(story,"_");
            char dstr[200],nstr[200];
            firstname(nstr);
            strcat(story,nstr);
            strcat(story,"_");
            lastname(dstr);
            strcat(story,dstr);
            strcat(story,"_and_the_");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Mysterious");break;
               case 1:strcat(story,"Magical");break;
               case 2:strcat(story,"Golden");break;
               case 3:strcat(story,"Invisible");break;
               case 4:strcat(story,"Wondrous");break;
            }
            strcat(story,"_");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Thing");break;
               case 1:strcat(story,"Stuff");break;
               case 2:strcat(story,"Object");break;
               case 3:strcat(story,"Whatever");break;
               case 4:strcat(story,"Something");break;
            }
            strcat(story,"_, is the third in an immensely popular series by ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"British");break;
               case 1:strcat(story,"Indian");break;
               case 2:strcat(story,"Chinese");break;
               case 3:strcat(story,"Rwandan");break;
               case 4:strcat(story,"Palestinian");break;
            }
            strcat(story," author ");
            char c[2];
            c[0]='A'+LCSrandom(26);
            c[1]='\x0';
            strcat(story,c);
            strcat(story,".");
            c[0]='A'+LCSrandom(26);
            c[1]='\x0';
            strcat(story,c);
            strcat(story,". ");
            lastname(dstr);
            strcat(story,dstr);
            strcat(story,".  ");
            strcat(story,"Although the series is adored by children worldwide, ");
            strcat(story,"some conservatives feel that the books ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"glorify Satan worship and are spawned by demons from the pit.  ");break;
               case 1:strcat(story,"teach children to kill their parents and hate life.  ");break;
               case 2:strcat(story,"cause violence in schools and are a gateway to cocaine use.  ");break;
               case 3:strcat(story,"breed demonic thoughts that manifest themselves as dreams of murder.  ");break;
               case 4:strcat(story,"contain step-by-step instructions to summon the Prince of Darkness.  ");break;
            }
            strcat(story,"In their complaint, the groups cited an incident involving ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"a child that swore in class");break;
               case 1:strcat(story,"a child that said a magic spell at her parents");break;
               case 2:strcat(story,"a child that pushed her older sister");break;
            }
            strcat(story," as key evidence of the dark nature of the book.");
            strcat(story,"&r");
            strcat(story,"   When the decision to ban the book was announced yesterday, ");
            strcat(story,"many area children spontoneously broke into tears.  One child was ");
            strcat(story,"heard saying, \"");
            switch(LCSrandom(2))
            {
               case 0:
                  strcat(story,"Mamma, is ");
                  strcat(story,nstr);
                  strcat(story," dead?");
                  break;
               case 1:
                  strcat(story,"Mamma, why did they kill ");
                  strcat(story,nstr);
                  strcat(story,"?");
                  break;
            }
            strcat(story,"\"");
            strcat(story,"&r");
            break;
         }
         case VIEW_JUSTICES:
         {
            cityname(story);
            strcat(story," - Conservative federal judge ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," has resigned in disgrace after being caught with a prostitute.");
            strcat(story,"&r");
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story,", who once ");
            switch(LCSrandom(2))
            {
               case 0:strcat(story,"defied the federal government by putting a Ten Commandments monument in the local federal building");break;
               case 1:strcat(story,"stated that, \"Segregation wasn't the bad idea everybody makes it out to be these days\"");break;
            }
            strcat(story,", was found with ");
            char pstr[200],pstr2[200];
            firstname(pstr);lastname(pstr2);
            strcat(story,pstr);
            strcat(story," ");
            strcat(story,pstr2);
            strcat(story," last week in a hotel during a police sting operation.  ");
            strcat(story,"According to sources familiar with the particulars, ");
            strcat(story,"when police broke into the hotel room they saw ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"\"the most perverse and spine-tingling debauchery imaginable, at least with only two people.\"");break;
               case 1:strcat(story,"the judge relieving himself on the prostitute.");break;
               case 2:strcat(story,"the prostitute hollering like a cowboy astride the judge.");break;
            }
            strcat(story,"  ");
            strcat(story,pstr2);
            strcat(story," reportedly offered ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"the arresting officers money");break;
               case 1:strcat(story,"to let the officers join in");break;
               case 2:strcat(story,"the arresting officers \"favors\"");break;
            }
            strcat(story," in exchange for their silence.");
            strcat(story,"&r");
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story," could not be reached for comment, although an aid stated that ");
            strcat(story,"the judge would be going on a Bible retreat for a few weeks to ");
            strcat(story,"\"Make things right with the Almighty Father.\"  ");
            strcat(story,"&r");
            break;
         }
         case VIEW_AMRADIO:
         {
            cityname(story);
            strcat(story," - Well-known AM radio personality ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," went off for fifteen minutes in an inexplicable rant ");
            strcat(story,"two nights ago during the syndicated radio program \"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"Straight");break;
               case 1:strcat(story,"Real");break;
               case 2:strcat(story,"True");break;
            }
            strcat(story," ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"Talk");break;
               case 1:strcat(story,"Chat");break;
               case 2:strcat(story,"Discussion");break;
            }
            strcat(story,"\".");
            strcat(story,"&r");
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story,"'s monologue for the evening began the way that fans ");
            strcat(story,"had come to expect, with attacks on the \"liberal media establishment\" and ");
            strcat(story,"the \"elite liberal agenda\".  But when the radio icon said, \"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"and the Grays are going to take over the planet in the End Times");break;
               case 1:strcat(story,"a liberal chupacabra will suck the blood from us like a goat, a goat!, a goat!");break;
               case 2:strcat(story,"I feel translucent rods passing through my body...  it's like making love to the future");break;
            }
            strcat(story,"\", a former fan of the show, ");
            char nstr[200],nstr2[200];
            firstname(nstr);lastname(nstr2);
            strcat(story,nstr);
            strcat(story," ");
            strcat(story,nstr2);
            strcat(story,", knew that \"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"my old hero");break;
               case 1:strcat(story,"my old idol");break;
               case 2:strcat(story,"the legend");break;
            }
            strcat(story," had ");
            switch(LCSrandom(3))
            {
               case 0:
                  strcat(story,"lost ");
                  switch(LCSrandom(2))
                  {
                     case 0:strcat(story,"his");break;
                     case 1:strcat(story,"her");break;
                  }
                  strcat(story," g*dd*mn mind");
                  break;
               case 1:strcat(story,"maybe gone a little off the deep end");break;
               case 2:strcat(story,"probably been listening to Art Bell in the next studio a little too long");break;
            }
            strcat(story,".  After that, it just got worse and worse.\"");
            strcat(story,"&r");
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story," issued an apology later in the program, but ");
            strcat(story,"the damage might already be done.  ");
            strcat(story,"According to a poll completed yesterday, ");
            strcat(story,"fully half of the host's most loyal supporters ");
            strcat(story,"have decided to leave the program for saner ");
            strcat(story,"pastures.  Of these, many said that they would be switching over ");
            strcat(story,"to the FM band.");
            strcat(story,"&r");
            break;
         }
      }
   }
   else
   {
      switch(view)
      {
         case VIEW_DEATHPENALTY:
         {
            cityname(story);
            strcat(story," - Perhaps parents can rest easier tonight.  ");
            strcat(story,"The authorities have apprehended their primary suspect in the ");
            strcat(story,"string of brutal child killings that has kept everyone in the area on edge, ");
            strcat(story,"according to a spokesperson for the police department here.  ");
            char dstr[200],dstr2[200],dstr3[200];
            firstname(dstr);firstname(dstr2);lastname(dstr3);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," ");
            strcat(story,dstr3);
            strcat(story," was detained yesterday afternoon, reportedly in possession of ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"pieces of another victim");break;
               case 1:strcat(story,"bloody toys");break;
               case 2:strcat(story,"a child's clothing stained with DNA evidence");break;
               case 3:strcat(story,"seven junior high school yearbooks");break;
               case 4:strcat(story,"two small backpacks");break;
            }
            strcat(story,".  Over twenty children in the past two years have gone missing, ");
            strcat(story,"only to turn up later dead and ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"carved with satanic symbols");break;
               case 1:strcat(story,"sexually mutilated");break;
               case 2:strcat(story,"missing all of their teeth");break;
               case 3:strcat(story,"missing all of their fingers");break;
               case 4:strcat(story,"without eyes");break;
            }
            strcat(story,".  Sources say that the police got a break in the case when ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"a victim called 911 just prior to being slain while still on the phone");break;
               case 1:strcat(story,"the suspect allegedly carved an address into one of the bodies");break;
               case 2:strcat(story,"an eye witness allegedly spotted the suspect luring a victim into a car");break;
               case 3:strcat(story,"a blood trail was found on a road that led them to the suspect's car trunk");break;
               case 4:strcat(story,"they found a victim in a ditch, still clinging to life");break;
            }
            strcat(story,".");
            strcat(story,"&r");
            strcat(story,"   The district attorney's office has already repeatedly said it will be ");
            strcat(story,"seeking the death penalty in this case.");
            strcat(story,"&r");
            break;
         }
         case VIEW_ANIMALRESEARCH:
         {
            cityname(story);
            strcat(story," - Researchers here report that they have discovered an amazing new wonder drug.  ");
            strcat(story,"Called ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Anal");break;
               case 1:strcat(story,"Colo");break;
               case 2:strcat(story,"Lacta");break;
               case 3:strcat(story,"Pur");break;
               case 4:strcat(story,"Loba");break;
            }
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"nephrin");break;
               case 1:strcat(story,"tax");break;
               case 2:strcat(story,"zac");break;
               case 3:strcat(story,"thium");break;
               case 4:strcat(story,"drene");break;
            }
            strcat(story,", the drug apparently ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"boosts intelligence in chimpanzees");break;
               case 1:strcat(story,"corrects erectile dysfunction in chimpanzees");break;
               case 2:strcat(story,"allows chimpanzees to move blocks with their minds");break;
               case 3:strcat(story,"allows chimpanzees to fly short distances");break;
               case 4:strcat(story,"increases the attention span of young chimpanzees");break;
            }
            strcat(story,".  ");
            strcat(story,"&r");
            strcat(story,"   Along with bonobos, chimpanzees are our closest cousins");
            if(law[LAW_FREESPEECH]==-2)strcat(story,", at least according to the now-discredited theory of evolution");
            strcat(story,".  ");
            strcat(story,"Fielding questions about the ethics of their experiments from reporters during a press conference yesterday, ");
            strcat(story,"a spokesperson for the research team stated that, \"It really isn't so bad as all that.  Chimpanzees are very resilient creatures.  ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"The ones that survived are all doing very well");break;
               case 1:strcat(story,"They hardly notice when you drill their brains out, if you're fast");break;
               case 2:strcat(story,"When we started muffling the screams of our subjects, the other chimps all calmed down quite a bit");break;
            }
            strcat(story,".  We have a very experienced research team.  ");
            strcat(story,"While we understand your concerns, any worries are entirely unfounded.  ");
            strcat(story,"I think the media should be focusing on the enormous benefits of this drug.");
            strcat(story,"\"");
            strcat(story,"&r");
            strcat(story,"   The first phase of human trials is slated to begin in a few months.");
            strcat(story,"&r");
            break;
         }
         case VIEW_PRISONS:
         {
            cityname(story);
            strcat(story," - The hostage crisis at the ");
            char jstr[200];
            lastname(jstr);
            strcat(story,jstr);
            strcat(story," Correctional Facility ended tragically yesterday with the ");
            strcat(story,"death of both the prison guard being held hostage and ");
            switch(LCSrandom(2))
            {
               case 0:strcat(story,"his");break;
               case 1:strcat(story,"her");break;
            }
            strcat(story," captor.");
            strcat(story,"&r");
            strcat(story,"   Two weeks ago, convicted rapist ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story,", an inmate at ");
            strcat(story,jstr);
            strcat(story,", overpowered ");
            char gstr[200],gstr2[200];
            firstname(gstr);lastname(gstr2);
            strcat(story,gstr);
            strcat(story," ");
            strcat(story,gstr2);
            strcat(story," and barricaded ");
            switch(LCSrandom(2))
            {
               case 0:strcat(story,"himself");break;
               case 1:strcat(story,"herself");break;
            }
            strcat(story," with the guard in a prison tower.  ");
            strcat(story,"Authorities locked down the prison and ");
            strcat(story,"attempted to negotiate by phone for 13 days, ");
            strcat(story,"but talks were cut short when ");
            strcat(story,dstr2);
            strcat(story," reportedly screamed into the receiver \"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"Ah, f*ck this sh*t.  This punk b*tch is f*ckin' dead!");break;
               case 1:strcat(story,"F*ck a m*th*f*ck*n' bull.  I'm killin' this pig sh*t.");break;
               case 2:strcat(story,"Why the f*ck am I talkin' to you?  I'd rather kill this pig.");break;
            }
            strcat(story,"\"");
            strcat(story,"  The tower was breached in an attempt to reach ");
            strcat(story,"the hostage, but ");
            strcat(story,dstr2);
            strcat(story," had already ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"slit the guard's throat with a shank");break;
               case 1:strcat(story,"strangled the guard to death with a knotted bed sheet");break;
               case 2:strcat(story,"chewed out the guard's throat");break;
            }
            strcat(story,".  The prisoner was beaten to death while ");
            strcat(story,"\"resisting capture\", according to a prison spokesperson.");
            strcat(story,"&r");
            break;
         }
         case VIEW_INTELLIGENCE:
         {
            strcat(story,"Washington D.C. - The CIA announced yesterday that it has averted a terror attack that ");
            strcat(story,"would have occurred on American soil.");
            strcat(story,"&r");
            strcat(story,"   According to a spokesperson for the agency, ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"white supremacists");break;
               case 1:strcat(story,"Islamic fundamentalists");break;
               case 2:strcat(story,"outcast goths from a suburban highschool");break;
            }
            strcat(story," planned to ");
            switch(LCSrandom(9))
            {
               case 0:strcat(story,"fly planes into skyscrapers");break;
               case 1:strcat(story,"detonate a fertilizer bomb at a federal building");break;
               case 2:strcat(story,"ram a motorboat loaded with explosives into a warship");break;
               case 3:strcat(story,"detonate explosives on a school bus");break;
               case 4:strcat(story,"blow out a section of a major bridge");break;
               case 5:strcat(story,"kidnap the president");break;
               case 6:strcat(story,"assassinate the president");break;
               case 7:strcat(story,"destroy the Capitol Building");break;
               case 8:strcat(story,"detonate a nuclear bomb in New York");break;
            }
            strcat(story,".  However, intelligence garnered from deep within the mysterious ");
            strcat(story,"terrorist organization allowed the plot to be foiled just days before it ");
            strcat(story,"was to occur.");
            strcat(story,"&r");
            strcat(story,"   The spokesperson further stated, \"");
            strcat(story,"I won't compromise our sources and methods, but let me just say ");
            strcat(story,"that we are grateful to the Congress and this Administration for ");
            strcat(story,"providing us with the tools we need to neutralize these enemies of ");
            strcat(story,"civilization before they can destroy American families.  ");
            strcat(story,"However, let me also say that there's more that needs to be done.  ");
            strcat(story,"The Head of the Agency will be sending a request to Congress ");
            strcat(story,"for what we feel are the essential tools for combating terrorism in ");
            strcat(story,"this new age.");
            strcat(story,"\"");
            strcat(story,"&r");
            break;
         }
         case VIEW_GENETICS:
         {
            cityname(story);
            strcat(story," - The genetic foods industry staged a major event here yesterday ");
            strcat(story,"to showcase its upcoming products.  Over thirty companies set up ");
            strcat(story,"booths and gave talks to wide-eyed onlookers.");
            strcat(story,"&r");
            strcat(story,"   One such corporation, ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Altered");break;
               case 1:strcat(story,"Gene-tech");break;
               case 2:strcat(story,"DNA");break;
               case 3:strcat(story,"Proteomic");break;
               case 4:strcat(story,"Genomic");break;
            }
            strcat(story," ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Foods");break;
               case 1:strcat(story,"Agriculture");break;
               case 2:strcat(story,"Meals");break;
               case 3:strcat(story,"Farming");break;
               case 4:strcat(story,"Living");break;
            }
            strcat(story,", presented their product, \"");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Mega");break;
               case 1:strcat(story,"Epic");break;
               case 2:strcat(story,"Overlord");break;
               case 3:strcat(story,"Franken");break;
               case 4:strcat(story,"Transcendent");break;
            }
            strcat(story," ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Rice");break;
               case 1:strcat(story,"Beans");break;
               case 2:strcat(story,"Corn");break;
               case 3:strcat(story,"Wheat");break;
               case 4:strcat(story,"Potatoes");break;
            }
            strcat(story,"\", during an afternoon Power Point presentation.  ");
            strcat(story,"According to the public relations representative speaking, ");
            strcat(story,"this amazing new product actually ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"extends human life by a few minutes every bite");break;
               case 1:strcat(story,"mends split-ends upon digestion.  Hair is also made glossier and thicker");break;
               case 2:strcat(story,"allows people to see in complete darkness");break;
               case 3:strcat(story,"causes a person to slowly attain their optimum weight with repeated use");break;
               case 4:strcat(story,"cures the common cold");break;
            }
            strcat(story,".");
            strcat(story,"&r");
            strcat(story,"   Spokespeople for the GM corporations were universal ");
            strcat(story,"in their dismissal of the criticism which often follows the industry.  ");
            strcat(story,"One in particular said, \"");
            strcat(story,"Look, these products are safe.  That thing about the ");
            switch(LCSrandom(4))
            {
               case 0:strcat(story,"guy going on a killing spree");break;
               case 1:strcat(story,"gal turning blue and exploding");break;
               case 2:strcat(story,"guy speaking in tongues and worshipping Satan");break;
               case 3:strcat(story,"gal having a ruptured intestine");break;
            }
            strcat(story," is just a load of ");
            switch(LCSrandom(4))
            {
               case 0:strcat(story,"hooey");break;
               case 1:strcat(story,"poppycock");break;
               case 2:strcat(story,"horse radish");break;
               case 3:strcat(story,"skunk weed");break;
            }
            strcat(story,".  Would we stake the reputation of our company on unsafe products?  ");
            strcat(story,"No.  That's just ridiculous.  I mean, sure companies have put unsafe products out, ");
            strcat(story,"but the GM industry operates at a higher ethical standard.  That goes without saying.");
            strcat(story,"\"");
            strcat(story,"&r");
            break;
         }
         case VIEW_JUSTICES:
         {
            cityname(story);
            strcat(story," - The conviction of confessed serial killer ");
            char dstr[200],dstr2[200],dstr3[200];
            firstname(dstr);firstname(dstr2);lastname(dstr3);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," ");
            strcat(story,dstr3);
            strcat(story," was overturned by a federal judge yesterday.  ");
            strcat(story,"Justice ");
            char jstr[200],jstr2[200];
            firstname(jstr);lastname(jstr2);
            strcat(story,jstr);
            strcat(story," ");
            strcat(story,jstr2);
            strcat(story," of the notoriously liberal circuit of appeals here ");
            strcat(story,"made the decision based on ");
            char gen[20];
            switch(LCSrandom(2))
            {
               case 0:strcpy(gen,"his");break;
               case 1:strcpy(gen,"her");break;
            }
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"ten-year-old eye witness testimony");break;
               case 1:strcat(story,gen);strcat(story," general feeling about police corruption");break;
               case 2:strcat(story,gen);strcat(story," belief that the crimes were a right-wing conspiracy");break;
               case 3:
                  strcat(story,gen);
                  strcat(story," belief that ");
                  strcat(story,dstr3);
                  strcat(story," deserved another chance");
                  break;
               case 4:
                  strcat(story,gen);
                  strcat(story," personal philosophy of liberty");break;
            }
            strcat(story,", despite the confession of ");
            strcat(story,dstr3);
            strcat(story,", which even Justice ");
            strcat(story,jstr2);
            strcat(story," grants was not coerced in any way.&r");

            strcat(story,"   Ten years ago, ");
            strcat(story,dstr3);
            strcat(story," was convicted of the now-infamous ");
            char sstr[200];
            lastname(sstr);
            strcat(story,sstr);
            strcat(story," slayings.  ");
            strcat(story,"After an intensive manhunt, ");
            strcat(story,dstr3);
            strcat(story," was found with the murder weapon, ");
            strcat(story,"covered in the victims' blood.  ");
            strcat(story,dstr3);
            strcat(story," confessed and was sentenced to life, saying \"");
            strcat(story,"Thank you for saving me from myself.  ");
            strcat(story,"If I were to be released, I would surely kill again.\"&r");
            strcat(story,"   A spokesperson for the district attorney ");
            strcat(story,"has stated that the case will not be retried, due ");
            strcat(story,"to the current economic doldrums that have left the state ");
            strcat(story,"completely strapped for cash.&r");
            break;
         }
         case VIEW_POLLUTION:
            cityname(story);
            strcat(story," - Pollution might not be so bad after all.  The ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"American");break;
               case 1:strcat(story,"United");break;
               case 2:strcat(story,"Patriot");break;
               case 3:strcat(story,"Family");break;
               case 4:strcat(story,"Children's");break;
            }
            strcat(story," ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Heritage");break;
               case 1:strcat(story,"Enterprise");break;
               case 2:strcat(story,"Freedom");break;
               case 3:strcat(story,"Liberty");break;
               case 4:strcat(story,"Charity");break;
            }
            strcat(story," ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Partnership");break;
               case 1:strcat(story,"Institute");break;
               case 2:strcat(story,"Consortium");break;
               case 3:strcat(story,"Forum");break;
               case 4:strcat(story,"Center");break;
            }
            strcat(story," recently released a wide-ranging report detailing recent trends ");
            strcat(story,"and the latest science on the issue.  ");
            strcat(story,"Among the most startling of the think tank's findings is that ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"a modest intake of radiactive waste");break;
               case 1:strcat(story,"a healthy dose of radiation");break;
               case 2:strcat(story,"a bath in raw sewage");break;
               case 3:strcat(story,"watching animals die in oil slicks");break;
               case 4:strcat(story,"inhaling carbon monoxide");break;
            }
            strcat(story," might actually ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"purify the soul");break;
               case 1:strcat(story,"increase test scores");break;
               case 2:strcat(story,"increase a child's attention span");break;
               case 3:strcat(story,"make children behave better");break;
               case 4:strcat(story,"make shy children fit in");break;
            }
            strcat(story,".");
            strcat(story,"&r");
            strcat(story,"   When questioned about the science behind these results, ");
            strcat(story,"a spokesperson stated that, \"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"Research is complicated, and there are always two ways to think about things");break;
               case 1:strcat(story,"The jury is still out on pollution.  You really have to keep an open mind");break;
               case 2:strcat(story,"They've got their scientists, and we have ours.  The issue of pollution is wide open as it stands today");break;
            }
            strcat(story,".  You have to realize that ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"the elitist liberal media often distorts");break;
               case 1:strcat(story,"the vast left-wing education machine often distorts");break;
               case 2:strcat(story,"the fruits, nuts, and flakes of the environmentalist left often distort");break;
            }
            strcat(story," these issues to their own advantage.  ");
            strcat(story,"All we've done is introduced a little clarity into the ongoing debate.  ");
            strcat(story,"Why is there contention on the pollution question?  It's because ");
            strcat(story,"there's work left to be done.  We should study much more ");
            strcat(story,"before we urge any action.  Society really just ");
            strcat(story,"needs to take a breather on this one.  We don't see why there's such a rush to judgement here.  ");
            strcat(story,"&r");
            break;
         case VIEW_CORPORATECULTURE:
            cityname(story);
            strcat(story," - Several major companies have announced ");
            strcat(story,"at a joint news conference here that they ");
            strcat(story,"will be expanding their work forces considerably ");
            strcat(story,"during the next quarter.  Over thirty thousand jobs ");
            strcat(story,"are expected in the first month, with ");
            strcat(story,"tech giant ");
            switch(LCSrandom(10))
            {
               case 0:strcat(story,"Ameri");break;
               case 1:strcat(story,"Gen");break;
               case 2:strcat(story,"Oro");break;
               case 3:strcat(story,"Amelia");break;
               case 4:strcat(story,"Vivo");break;
               case 5:strcat(story,"Benji");break;
               case 6:strcat(story,"Amal");break;
               case 7:strcat(story,"Ply");break;
               case 8:strcat(story,"Seli");break;
               case 9:strcat(story,"Rio");break;
            }
            switch(LCSrandom(10))
            {
               case 0:strcat(story,"tech");break;
               case 1:strcat(story,"com");break;
               case 2:strcat(story,"zap");break;
               case 3:strcat(story,"cor");break;
               case 4:strcat(story,"dyne");break;
               case 5:strcat(story,"bless");break;
               case 6:strcat(story,"chip");break;
               case 7:strcat(story,"co");break;
               case 8:strcat(story,"wire");break;
               case 9:strcat(story,"rex");break;
            }
            strcat(story," increasing its payrolls by over ten thousand workers alone.  ");
            strcat(story,"Given the state of the economy recently and in ");
            strcat(story,"light of the tendency ");
            strcat(story,"of large corporations to export jobs overseas these days, ");
            strcat(story,"this welcome news is bound to be a pleasant surprise to those in the unemployment lines.  ");
            strcat(story,"The markets reportedly responded to the announcement with mild interest, ");
            strcat(story,"although the dampened movement might be expected due to the uncertain ");
            strcat(story,"futures of some of the companies in the tech sector.  On the whole, however, ");
            strcat(story,"analysts suggest that not only does the expansion speak to the health ");
            strcat(story,"of the tech industry but is also indicative of a full economic recover.&r");
            break;
         case VIEW_AMRADIO:
         {
            //THIS ONE IS SHORTER BECAUSE OF DOUBLE HEADLINE
            cityname(story);
            strcat(story," - Infamous FM radio shock jock ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," has brought radio entertainment to a new low.  During yesterday's ");
            strcat(story,"broadcast of the program \"");
            strcat(story,dstr);
            strcat(story,"'s ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Morning");break;
               case 1:strcat(story,"Commuter");break;
               case 2:strcat(story,"Jam");break;
               case 3:strcat(story,"Talk");break;
               case 4:strcat(story,"Radio");break;
            }
            strcat(story," ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Swamp");break;
               case 1:strcat(story,"Jolt");break;
               case 2:strcat(story,"Club");break;
               case 3:strcat(story,"Show");break;
               case 4:strcat(story,"Fandango");break;
            }
            strcat(story,"\", ");
            strcat(story,dstr2);
            strcat(story," reportedly ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"had intercourse");break;
               case 1:strcat(story,"encouraged listeners to call in and relieve themselves");break;
               case 2:strcat(story,"screamed \"f*ck the police those g*odd*mn m*th*f*ck*rs.  I got a f*cking ticket this morning and I'm f*cking p*ss*d as sh*t.\"");break;
               case 3:strcat(story,"breastfed from a lactating woman");break;
               case 4:strcat(story,"masturbated");break;
            }
            strcat(story," on the air.  Although ");
            strcat(story,dstr2);
            strcat(story," later apologized, ");
            strcat(story,"the FCC received thousands of complaints ");
            strcat(story,"from irate listeners.  A spokesperson for the FCC ");
            strcat(story,"stated that the incident is under investigation.");
            strcat(story,"&r");
         }
            break;
      }
   }
}



/* news - draws the specified block of text to the screen */
void displaynewsstory(char *story,short *storyx_s,short *storyx_e,int y)
{
   vector<char *> text;
   vector<char> centered;

   int totalwidth;
   int curpos=0;

   int addstrcur;
   char addstring[500];

   char content;
   int cury=y;
   int length;
   char endparagraph=0;
   char iscentered=0;
   unsigned int i=0;

   while(curpos<strlen(story)&&cury<25)
   {
      content=0;
      totalwidth=0;
      addstrcur=0;
      length=storyx_e[cury]-storyx_s[cury]+1;
      if(length==0){cury++;if(endparagraph>0)endparagraph--;continue;}

      for(i=curpos;i<strlen(story);i++)
      {
         if(story[i]=='&'&&story[i+1]!='&')
         {
            i++;
            if(story[i]=='c')
            {
               iscentered=1;
            }
            if(story[i]=='r')
            {
               content=1;
               i++;
               addstrcur+=1;
               addstring[addstrcur-1]=' ';
               addstring[addstrcur]='\x0';
               endparagraph=1;
               break;
            }
         }
         else
         {
            content=1;

            if(story[i]=='&')i++;
            addstring[addstrcur]=story[i];
            addstring[addstrcur+1]='\x0';
            totalwidth++;
            if(totalwidth>length)
            {
               while(story[i]!=' '){i--;addstrcur--;}
               while(story[i]==' ')i++;
               addstring[addstrcur]='\x0';
               break;
            }
            addstrcur++;
         }
      }

      if(i==strlen(story))addstring[addstrcur]='\x0';

      if(strlen(addstring)>0&&content)
      {
         int words=0;
         char silent=1;
         vector<int> spacex;
         for(int s2=0;s2<strlen(addstring);s2++)
         {
            if(addstring[s2]==' ')
            {
               if(!silent)
               {
                  silent=1;
                  words++;
                  spacex.push_back(s2);
               }
            }
            else
            {
               if(silent)
               {
                  words++;
                  silent=0;
               }
            }
         }

         while(!endparagraph&&words>1&&strlen(addstring)<length&&!iscentered)
         {
            int csp=spacex[LCSrandom(spacex.size())];

            for(int x=0;x<spacex.size();x++)
            {
               if(spacex[x]>csp)spacex[x]++;
            }

            int l=strlen(addstring);
            for(int s=l+1;s>csp;s--)
            {
               addstring[s]=addstring[s-1];
            }
         }

         char *news=new char[strlen(addstring)+1];
         strcpy(news,addstring);
         text.push_back(news);
         centered.push_back(iscentered);
         cury++;
         if(endparagraph>0)
         {
            endparagraph--;
            iscentered=0;
         }
      }

      curpos=i;
   }

   set_color(COLOR_BLACK,COLOR_WHITE,0);
   for(int t=0;t<text.size();t++)
   {
      if(y+t>=25)break;
      if(centered[t])
      {
         move(y+t,((storyx_s[y+t]+storyx_e[y+t])>>1)-((strlen(text[t])-1)>>1));
      }
      else move(y+t,storyx_s[y+t]);
      addstr(text[t]);

      delete[] text[t];
   }
   text.clear();
}



/* news - make some filler junk */
void generatefiller(char *story,int amount)
{
   char str[80];
   strcat(story,"&r");
   cityname(str);
   strcat(story,str);
   strcat(story," - ");
   int par=0;
   while(amount>0)
   {
      par++;
      for(int i=0;i<LCSrandom(10)+3;i++)strcat(story,"~");
      if(amount>1)strcat(story," ");
      if(par>=50&&!LCSrandom(5)&&amount>20)
      {
         par=0;
         strcat(story,"&r");
         strcat(story,"  ");
      }
      amount--;
   }
   strcat(story,"&r");
}



/* news - major newspaper reporting on lcs and other topics */
void majornewspaper(char &clearformess,char canseethings)
{
   int n = 0;
   int i;

   int writers=0;

   for(i=0;i<pool.size();i++)
   {
      if(pool[i]->alive&&pool[i]->activity.type==ACTIVITY_WRITE_GUARDIAN)
      {
         if(location[pool[i]->location]->compound_walls==COMPOUND_PRINTINGPRESS)
         {
            pool[i]->skill_ip[SKILL_WRITING]+=LCSrandom(3); // Experience gain
            writers+=pool[i]->skill[SKILL_WRITING]; // Record the writer on this topic
            criminalize(*pool[i],LAWFLAG_SPEECH); // Record possibly illegal speech activity
         }
         else pool[i]->activity.type=ACTIVITY_NONE;
      }
   }

   //Conservative Crime Squad Strikes!
   if(endgamestate<ENDGAME_CCS_DEFEATED && LCSrandom(30)<endgamestate)
   {
      newsstoryst *ns=new newsstoryst;
      if(LCSrandom(10))ns->type=NEWSSTORY_CCS_SITE;
      else ns->type=NEWSSTORY_CCS_KILLED_SITE;

      ns->positive=LCSrandom(5);
      if(ns->positive)ns->positive=1;

      do
      {
         ns->loc=LCSrandom(location.size());
      } while(location[ns->loc]->renting!=-1);

      newsstory.push_back(ns);
   }

   //SET UP MAJOR EVENTS
   if(!LCSrandom(60))
   {
      newsstoryst *ns=new newsstoryst;
         ns->type=NEWSSTORY_MAJOREVENT;
         do
         {
            ns->view=LCSrandom(VIEWNUM-2);
            ns->positive=LCSrandom(2);

            //NO ABORTION
            if(ns->view==VIEW_ABORTION&&ns->positive&&law[LAW_ABORTION]==-2)continue;
            //NO PARTIAL BIRTH ABORTION
            if(ns->view==VIEW_ABORTION&&!ns->positive&&law[LAW_ABORTION]<2)continue;
            //NO DEATH PENALTY
            if(ns->view==VIEW_DEATHPENALTY&&law[LAW_ABORTION]==2)continue;
            //NO NUCLEAR POWER
            if(ns->view==VIEW_NUCLEARPOWER&&ns->positive&&law[LAW_NUCLEARPOWER]==2)continue;
            //NO ANIMAL RESEARCH
            if(ns->view==VIEW_ANIMALRESEARCH&&law[LAW_ANIMALRESEARCH]==2)continue;
            //NO BAD COPS
            if(ns->view==VIEW_POLICEBEHAVIOR&&ns->positive&&law[LAW_POLICEBEHAVIOR]==2)continue;
            //NO PRIVACY VIOLATIONS
            if(ns->view==VIEW_INTELLIGENCE&&ns->positive&&law[LAW_PRIVACY]==2)continue;
            //NO SWEATSHOPS
            if(ns->view==VIEW_SWEATSHOPS&&ns->positive&&law[LAW_LABOR]==2)continue;
            //NO POLLUTION
            if(ns->view==VIEW_POLLUTION&&ns->positive&&law[LAW_POLLUTION]>=1)continue;
            //NO ENRONS
            if(ns->view==VIEW_CORPORATECULTURE&&ns->positive&&law[LAW_CORPORATE]==2)continue;
            //NO CEOS
            if(ns->view==VIEW_CEOSALARY&&ns->positive&&law[LAW_CORPORATE]==2)continue;
            //NO FREEDOM OF SPEECH
            if(ns->view==VIEW_AMRADIO&&!ns->positive&&law[LAW_FREESPEECH]==-2)continue;

            break;
         }while(1);
      newsstory.push_back(ns);

      if(ns->positive)change_public_opinion(ns->view,20,0);
      else change_public_opinion(ns->view,-20,0);
      public_interest[ns->view]+=50;
   }

   //DELETE STORIES THAT HAVE NO CONTENT
   for(n=newsstory.size()-1;n>=0;n--)
   {
      if(newsstory[n]->type==NEWSSTORY_SQUAD_SITE&&
         newsstory[n]->crime.size()==0)
      {
         delete newsstory[n];
         newsstory.erase(newsstory.begin() + n);
         continue;
      }

      if(newsstory[n]->type==NEWSSTORY_CARTHEFT)
      {
         char conf=0;
         for(int c=0;c<newsstory[n]->crime.size();c++)
         {
            if(newsstory[n]->crime[c]==CRIME_KILLEDSOMEBODY)
            {
               conf=1;
               break;
            }
         }
         if(!conf)
         {
            delete newsstory[n];
            newsstory.erase(newsstory.begin() + n);
            continue;
         }
      }

      if((newsstory[n]->type==NEWSSTORY_SQUAD_ESCAPED||
         newsstory[n]->type==NEWSSTORY_SQUAD_FLEDATTACK||
         newsstory[n]->type==NEWSSTORY_SQUAD_DEFENDED||
         newsstory[n]->type==NEWSSTORY_SQUAD_BROKESIEGE||
         newsstory[n]->type==NEWSSTORY_SQUAD_KILLED_SIEGEATTACK||
         newsstory[n]->type==NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE)&&
         newsstory[n]->siegetype!=SIEGE_POLICE)
      {
         delete newsstory[n];
         newsstory.erase(newsstory.begin() + n);
         continue;
      }
   }

   clearformess=1;

   //DO TELEVISION AND OTHER NON-NEWS STORIES, THEN DELETE THEM
   if(canseethings&&newsstory.size())
   {
      char del;
      for(int n=newsstory.size()-1;n>=0;n--)
      {
         del=0;
         if(newsstory[n]->type==NEWSSTORY_MAJOREVENT)
         {
            if(newsstory[n]->positive)
            {
               switch(newsstory[n]->view)
               {
                  case VIEW_POLICEBEHAVIOR:
                     movie.loadmovie("art\\lacops.cmv");
                     movie.playmovie(0,0);
                     nodelay(stdscr,FALSE);

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(19,13);
                     addstr("/----------------------------------------------------\\");
                     move(20,13);
                     addstr("|     The  police  have  beaten  a  black  man  in    |");
                     move(21,13);
                     addstr("|   Los Angeles again.  This time, the incident is    |");
                     move(22,13);
                     addstr("|   taped by  a passerby  and saturates  the news.    |");
                     move(23,13);
                     addstr("\\----------------------------------------------------/");

                     refresh();
                     getch();

                     del=1;
                     break;
                  case VIEW_CABLENEWS:
                  {
                     char str[80];
                     strcpy(str,"Tonight on a Cable News channel: ");
                     switch(LCSrandom(5))
                     {
                        case 0:strcat(str,"Cross");break;
                        case 1:strcat(str,"Hard");break;
                        case 2:strcat(str,"Lightning");break;
                        case 3:strcat(str,"Washington");break;
                        case 4:strcat(str,"Capital");break;
                     }
                     switch(LCSrandom(5))
                     {
                        case 0:strcat(str," Fire");break;
                        case 1:strcat(str," Ball");break;
                        case 2:strcat(str," Talk");break;
                        case 3:strcat(str," Insider");break;
                        case 4:strcat(str," Gang");break;
                     }
                     strcat(str," with ");
                     char bname[80];
                     name(bname);
                     strcat(str,bname);

                     erase();
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(0,39-((strlen(str)-1)>>1));
                     addstr(str);

                     move(16,20);
                     addstr(bname);
                     move(17,20);
                     addstr("Washington D.C.");

                     move(16,41);
                     name(bname);
                     addstr(bname);
                     move(17,41);
                     switch(LCSrandom(3))
                     {
                        case 0:addstr("Eugene, OR");break;
                        case 1:addstr("San Francisco, CA");break;
                        case 2:addstr("Cambridge, MA");break;
                     }

                     movie.loadmovie("art\\newscast.cmv");

                     movie.playmovie(1,1);
                     nodelay(stdscr,FALSE);

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(19,13);
                     addstr("/----------------------------------------------------\\");
                     move(20,13);
                     addstr("|     A  Cable  News  anchor  accidentally  let  a   |");
                     move(21,13);
                     addstr("|   bright Liberal guest  finish a sentence.  Many   |");
                     move(22,13);
                     addstr("|   viewers  across  the  nation  were  listening.   |");
                     move(23,13);
                     addstr("\\----------------------------------------------------/");

                     refresh();
                     getch();

                     del=1;
                     break;
                  }
               }
            }
            else
            {
               switch(newsstory[n]->view)
               {
                  case VIEW_CEOSALARY:
                     movie.loadmovie("art\\glamshow.cmv");
                     movie.playmovie(0,0);
                     nodelay(stdscr,FALSE);

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(19,13);
                     addstr("/----------------------------------------------------\\");
                     move(20,13);
                     addstr("|     A new show glamorizing the lives of the rich   |");
                     move(21,13);
                     addstr("|   begins airing  this week.  With the nationwide   |");
                     move(22,13);
                     addstr("|   advertising  blitz, it's bound  to be popular.   |");
                     move(23,13);
                     addstr("\\----------------------------------------------------/");

                     refresh();
                     getch();

                     del=1;
                     break;
                  case VIEW_CABLENEWS:
                     movie.loadmovie("art\\anchor.cmv");
                     movie.playmovie(0,0);
                     nodelay(stdscr,FALSE);

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(19,13);
                     addstr("/----------------------------------------------------\\");
                     move(20,13);
                     addstr("|     A major Cable News channel has hired a slick   |");
                     move(21,13);
                     addstr("|   new anchor for one of  its news shows.  Guided   |");
                     move(22,13);
                     addstr("|   by impressive  advertising, America  tunes in.   |");
                     move(23,13);
                     addstr("\\----------------------------------------------------/");

                     refresh();
                     getch();

                     del=1;
                     break;
                  case VIEW_ABORTION:
                     erase();

                     movie.loadmovie("art\\abort.cmv");
                     movie.playmovie(0,0);
                     nodelay(stdscr,FALSE);

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(19,13);
                     addstr("/----------------------------------------------------\\");
                     move(20,13);
                     addstr("|     A  failed partial  birth abortion  goes on a   |");
                     move(21,13);
                     addstr("|   popular  afternoon  talk  show.    The  studio   |");
                     move(22,13);
                     addstr("|   audience and viewers nationwide feel its pain.   |");
                     move(23,13);
                     addstr("\\----------------------------------------------------/");

                     refresh();
                     getch();

                     del=1;
                     break;
               }
            }
         }
         if(del)
         {
            delete newsstory[n];
            newsstory.erase(newsstory.begin() + n);
         }
      }
   }

   //ASSIGN PAGE NUMBERS TO STORIES BASED ON THEIR PRIORITY
   if(newsstory.size())
   {
      for(n=0;n<newsstory.size();n++)
      {
         setpriority(*newsstory[n]);
         newsstory[n]->page=-1;
      }
      char acted;
      int curpage=1;
      int curguardianpage=1;
      do
      {
         acted=0;
         // Sort the major newspapers
         int maxn=-1;
         int maxp=-1;
         for(n=0;n<newsstory.size();n++)
         {
            if(newsstory[n]->priority>maxp&&
               newsstory[n]->page==-1)
            {
               maxn=n;
               maxp=newsstory[n]->priority;
            }
         }
         if(maxn!=-1)
         {
            if(newsstory[maxn]->priority<30&&curpage==1)curpage=2;
            if(newsstory[maxn]->priority<25&&curpage<3)curpage=3+LCSrandom(2);
            if(newsstory[maxn]->priority<20&&curpage<5)curpage=5+LCSrandom(5);
            if(newsstory[maxn]->priority<15&&curpage<10)curpage=10+LCSrandom(10);
            if(newsstory[maxn]->priority<10&&curpage<20)curpage=20+LCSrandom(10);
            if(newsstory[maxn]->priority<5&&curpage<30)curpage=30+LCSrandom(20);

            newsstory[maxn]->page=curpage;
            newsstory[maxn]->guardianpage=curguardianpage;
            curpage++;
            curguardianpage++;
            acted=1;
         }
      }while(acted);

      //DISPLAY PAPER
      if(canseethings)
      {
         for(n=0;n<newsstory.size();n++)
         {
            bool liberalguardian=0;
            int header = -1;
            if(writers&&newsstory[n]->type!=NEWSSTORY_MAJOREVENT)
            {
               --writers;
               liberalguardian=1;
            }
            switch(newsstory[n]->type)
            {
               case NEWSSTORY_SQUAD_SITE:
               case NEWSSTORY_SQUAD_KILLED_SITE:
                  switch(location[newsstory[n]->loc]->type)
                  {
                     case SITE_LABORATORY_COSMETICS:
                        header=VIEW_ANIMALRESEARCH;
                        break;
                     case SITE_LABORATORY_GENETIC:
                        header=VIEW_GENETICS;
                        break;
                     case SITE_GOVERNMENT_POLICESTATION:
                        header=VIEW_POLICEBEHAVIOR;
                        break;
                     case SITE_GOVERNMENT_COURTHOUSE:
                        header=VIEW_JUSTICES;
                        break;
                     case SITE_GOVERNMENT_PRISON:
                        header=VIEW_PRISONS;
                        break;
                     case SITE_GOVERNMENT_INTELLIGENCEHQ:
                        header=VIEW_INTELLIGENCE;
                        break;
                     case SITE_INDUSTRY_SWEATSHOP:
                        header=VIEW_SWEATSHOPS;
                        break;
                     case SITE_INDUSTRY_POLLUTER:
                        header=VIEW_POLLUTION;
                        break;
                     case SITE_INDUSTRY_NUCLEAR:
                        header=VIEW_NUCLEARPOWER;
                        break;
                     case SITE_CORPORATE_HEADQUARTERS:
					         header=VIEW_CORPORATECULTURE;
                        break;
                     case SITE_CORPORATE_HOUSE:
                        header=VIEW_CEOSALARY;
                        break;
                     case SITE_MEDIA_AMRADIO:
                        header=VIEW_AMRADIO;
                        break;
                     case SITE_MEDIA_CABLENEWS:
                        header=VIEW_CABLENEWS;
                        break;
                     case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
                     case SITE_BUSINESS_CIGARBAR:
                        header=VIEW_TAXES;
                        break;
                  }
                  break;
               case NEWSSTORY_SQUAD_ESCAPED:
               case NEWSSTORY_SQUAD_FLEDATTACK:
               case NEWSSTORY_SQUAD_DEFENDED:
               case NEWSSTORY_SQUAD_BROKESIEGE:
               case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
               case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
                  break;
            }
            displaystory(*newsstory[n],0,-1);
            if(liberalguardian)
            {
               if(newsstory[n]->type==NEWSSTORY_CCS_SITE||
                  newsstory[n]->type==NEWSSTORY_CCS_KILLED_SITE)
                  newsstory[n]->positive=0;
               displaystory(*newsstory[n],liberalguardian,header);
               if(newsstory[n]->positive)newsstory[n]->positive+=1;
            }
         }
      }
   }

   //Letters to the editor
   for(int p=0;p<pool.size();p++)
   {
      //Yes, crappy letters to the editor may backfire
      if(pool[p]->activity.type==ACTIVITY_WRITE_LETTERS)
      {
         background_liberal_influence[randomissue()]+=pool[p]->skill[SKILL_WRITING]-LCSrandom(3);
         pool[p]->skill_ip[SKILL_WRITING]+=LCSrandom(5)+1;
      }
   }

   //Essay writing
   for(int w=0;w<VIEWNUM;w++)
   {
      if(!public_interest[w])
      {
         continue;
      }
      else
      {
         if(writers)
         {
            public_interest[w]--;
            writers--;
            background_liberal_influence[w]++;
         }
      }
   }

   //CHANGE FOR SQUAD ACTS PUBLIC OPINION BASED ON PAGE NUMBERS
      //AND OVERALL POWER OF THE STORY
   int power;
   for(n=0;n<newsstory.size();n++)
   {
      if(newsstory[n]->type==NEWSSTORY_SQUAD_SITE||
         newsstory[n]->type==NEWSSTORY_SQUAD_ESCAPED||
         newsstory[n]->type==NEWSSTORY_SQUAD_FLEDATTACK||
         newsstory[n]->type==NEWSSTORY_SQUAD_DEFENDED||
         newsstory[n]->type==NEWSSTORY_SQUAD_BROKESIEGE||
         newsstory[n]->type==NEWSSTORY_SQUAD_KILLED_SIEGEATTACK||
         newsstory[n]->type==NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE||
         newsstory[n]->type==NEWSSTORY_SQUAD_KILLED_SITE||
         newsstory[n]->type==NEWSSTORY_CCS_SITE||
         newsstory[n]->type==NEWSSTORY_CCS_KILLED_SITE)
      {
         power=newsstory[n]->priority;

         if(newsstory[n]->type==NEWSSTORY_CCS_SITE||
            newsstory[n]->type==NEWSSTORY_CCS_KILLED_SITE)
         {
            newsstory[n]->positive=!newsstory[n]->positive;
         }

         //PAGE BONUS
         if(newsstory[n]->page==1)power*=5;
         else if(newsstory[n]->page==2)power*=3;
         else if(newsstory[n]->page==3)power*=2;

         int maxpower;
         if(newsstory[n]->page==1)maxpower=100;
         else if(newsstory[n]->page<5)maxpower=100-10*newsstory[n]->page;
         else if(newsstory[n]->page<10)maxpower=40;
         else if(newsstory[n]->page<20)maxpower=20;
         else if(newsstory[n]->page<30)maxpower=10;
         else if(newsstory[n]->page<40)maxpower=5;
         else maxpower=1;

         if(newsstory[n]->positive==2)power*=3;
         if(power>maxpower)power=maxpower;

         power/=10;
         power++;
         if(!(newsstory[n]->type==NEWSSTORY_CCS_SITE)&&
            !(newsstory[n]->type==NEWSSTORY_CCS_KILLED_SITE))
         {
            change_public_opinion(VIEW_LIBERALCRIMESQUAD,2+power);
            if(newsstory[n]->positive)change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,power);
            else change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,-power);
         }

         char colored=0;
         if(newsstory[n]->positive&&
            !(newsstory[n]->type==NEWSSTORY_CCS_SITE)&&
            !(newsstory[n]->type==NEWSSTORY_CCS_KILLED_SITE))colored=1;
         else power=-power;

         switch(location[newsstory[n]->loc]->type)
         {
            case SITE_LABORATORY_COSMETICS:
               change_public_opinion(VIEW_ANIMALRESEARCH,power,colored,power*10);
               break;
            case SITE_LABORATORY_GENETIC:
               change_public_opinion(VIEW_GENETICS,power,colored,power*10);
               break;
            case SITE_GOVERNMENT_POLICESTATION:
               change_public_opinion(VIEW_POLICEBEHAVIOR,power,colored,power*10);
               break;
            case SITE_GOVERNMENT_COURTHOUSE:
               change_public_opinion(VIEW_DEATHPENALTY,power,colored,power*10);
               change_public_opinion(VIEW_JUSTICES,power,colored,power*10);
               change_public_opinion(VIEW_FREESPEECH,power,colored,power*10);
               change_public_opinion(VIEW_ABORTION,power,colored,power*10);
               change_public_opinion(VIEW_GAY,power,colored,power*10);
               break;
            case SITE_GOVERNMENT_PRISON:
               change_public_opinion(VIEW_DEATHPENALTY,power,colored,power*10);
               change_public_opinion(VIEW_PRISONS,power,colored,power*10);
               break;
            case SITE_GOVERNMENT_INTELLIGENCEHQ:
               change_public_opinion(VIEW_INTELLIGENCE,power,colored,power*10);
               break;
            case SITE_INDUSTRY_SWEATSHOP:
               change_public_opinion(VIEW_SWEATSHOPS,power,colored,power*10);
               break;
            case SITE_INDUSTRY_POLLUTER:
               change_public_opinion(VIEW_POLLUTION,power,colored,power*10);
               break;
            case SITE_INDUSTRY_NUCLEAR:
               change_public_opinion(VIEW_NUCLEARPOWER,power,colored,power*10);
               break;
            case SITE_CORPORATE_HEADQUARTERS:
					change_public_opinion(VIEW_TAXES,power,colored,power);
               change_public_opinion(VIEW_CORPORATECULTURE,power,colored,power*10);
               break;
            case SITE_CORPORATE_HOUSE:
					change_public_opinion(VIEW_TAXES,power,colored,power*10);
					change_public_opinion(VIEW_CEOSALARY,power,colored,power*10);
               break;
            case SITE_MEDIA_AMRADIO:
               change_public_opinion(VIEW_AMRADIO,power,colored,power*10);
               break;
            case SITE_MEDIA_CABLENEWS:
               change_public_opinion(VIEW_CABLENEWS,power,colored,power*10);
               break;
            case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
					change_public_opinion(VIEW_TAXES,power,colored,power*10);
					change_public_opinion(VIEW_CEOSALARY,power,colored,power*10);
               break;
            case SITE_BUSINESS_CIGARBAR:
					change_public_opinion(VIEW_TAXES,power,colored,power*10);
					change_public_opinion(VIEW_CEOSALARY,power,colored,power*10);
               break;
         }
      }
   }

   //DELETE STORIES
   for(n=0;n<newsstory.size();n++)delete newsstory[n];
   newsstory.clear();
}
