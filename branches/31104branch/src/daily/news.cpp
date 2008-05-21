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

   string story;
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

      string ad;
      if(!liberalguardian)
      {
         switch(LCSrandom(5))
         {
            case 0:
               ad="&cNo Fee&r";
               ad+="&cConsignment Program&r&r";
               ad+="&cCall for Details&r";
               break;
            case 1:
               ad="&cFine Leather Chairs&r&r";
               ad+="&cSpecial Purchase&r";
               ad+="&cNow $599&r";
               break;
            case 2:
               ad="&cParis Flea Market&r&r";
               ad+="&cSale&r";
               ad+="&c50% Off&r";
               break;
            case 3:
               ad="&cQuality Pre-Owned&r";
               ad+="&cVehicles&r";
               ad+="&c1999 Lexus GS 300&r";
               ad+="&cSedan 4D&r";
               ad+="&cOnly $21,988&r";
               break;
            case 4:
               ad="&cSpa&r";
               ad+="&cHealth, Beauty&r";
               ad+="&cand Fitness&r&r";
               ad+="&c7 Days a Week&r";
               break;
         }
      }
      else
      {
         switch(2) // Liberal Guardian Ads
         {
         case 0:
            ad="&cWant Organic?&r&r";
            ad+="&cVisit The Vegan&r";
            ad+="&cCo-Op&r";
            break;
         case 1:
            ad="&cLiberal Defense Lawyer&r";
            ad+="&c26 Years Experience&r&r";
            ad+="&cCall Today&r";
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
                  string str;
                  str="";
                  switch(LCSrandom(5))
                  {
                     case 0:str+="Shadow";break;
                     case 1:str+="Dark";break;
                     case 2:str+="Abyssal";break;
                     case 3:str+="Orwellian";break;
                     case 4:str+="Craggy";break;
                  }
                  str+=" ";
                  switch(LCSrandom(5))
                  {
                     case 0:str+="Actor";break;
                     case 1:str+="Lord";break;
                     case 2:str+="Emperor";break;
                     case 3:str+="Puppet";break;
                     case 4:str+="Dementia";break;
                  }
                  str+=": A new book further documenting the other side of Reagan.";
                  displaycenteredsmallnews(str.c_str(),12);
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
                  string str;
         str="This major CEO ";
         switch(LCSrandom(10))
         {
            case 0:
               str+="regularly visits prostitutes.";
               break;
            case 1:
               str+="seeks the aid of psychics.";
               break;
            case 2:
               str+="donated millions to the KKK.";
               break;
            case 3:
               str+="evaded paying taxes.";
               break;
            case 4:
               str+="took out a contract on his wife.";
               break;
            case 5:
               str+="doesn't know what his company does.";
               break;
            case 6:
               str+="has a zoo of imported exotic worms.";
               break;
            case 7:
               str+="paid millions for high-tech bondage gear.";
               break;
            case 8:
               str+="installed a camera in an office bathroom.";
               break;
            case 9:
               str+="owns slaves in another country.";
               break;
         }
                  displaycenteredsmallnews(str.c_str(),12);
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
                  string str;
                  str="";
                  switch(LCSrandom(5))
                  {
                     case 0:str+="Great";break;
                     case 1:str+="Noble";break;
                     case 2:str+="True";break;
                     case 3:str+="Pure";break;
                     case 4:str+="Golden";break;
                  }
                  str+=" ";
                  switch(LCSrandom(5))
                  {
                     case 0:str+="Leadership";break;
                     case 1:str+="Courage";break;
                     case 2:str+="Pioneer";break;
                     case 3:str+="Communicator";break;
                     case 4:str+="Faith";break;
                  }
                  str+=": A new book lauding Reagan and the greatest generation.";
                  displaycenteredsmallnews(str.c_str(),12);
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

         story=lcityname;
         story+=" - ";

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

               story+="A routine arrest went horribly wrong yesterday, ";
               story+="according to a spokesperson from the police department.";
               story+="&r";
               story+="  A suspect, whose identify is unclear, ";
               story+="killed ";
               if(crime[CRIME_KILLEDSOMEBODY]>1)
               {
                  char num[3];
                  itoa(crime[CRIME_KILLEDSOMEBODY],num,10);
                  story+=num;
                  story+=" police officers";
               }
               else story+="a police officer";
               story+=" that were attempting to perform an arrest.  A passerby had allegedly spotted the suspect committing a car theft.  ";
               story+="The names of the officers have not been released pending notification of their families.";
               story+="&r";
               break;
            }
            case NEWSSTORY_SQUAD_ESCAPED:
               story+="Members of the Liberal Crime Squad ";
               story+="escaped from a police siege yesterday, according ";
               if(!liberalguardian)story+="to a spokesperson from the police department.";
               else story+="to a Liberal Crime Squad spokesperson.";
               story+="&r";
               break;
            case NEWSSTORY_SQUAD_FLEDATTACK:
               story+="Members of the Liberal Crime Squad ";
               story+="escaped from police officers during a raid yesterday, according ";
               if(!liberalguardian)story+="to a spokesperson from the police department.";
               else story+="to a Liberal Crime Squad spokesperson.";
               story+="&r";
               break;
            case NEWSSTORY_SQUAD_DEFENDED:
               story+="Members of the Liberal Crime Squad ";
               story+="fought off a police raid yesterday, according ";
               if(!liberalguardian)story+="to a spokesperson from the police department.";
               else story+="to a Liberal Crime Squad spokesperson.";
               story+="&r";
               break;
            case NEWSSTORY_SQUAD_BROKESIEGE:
               story+="Members of the Liberal Crime Squad ";
               story+="violently broke a police siege yesterday, according ";
               if(!liberalguardian)story+="to a spokesperson from the police department.";
               else story+="to a Liberal Crime Squad spokesperson.";
               story+="&r";
               break;
            case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
               story+="Members of the Liberal Crime Squad were ";
               if(!liberalguardian)
               {
                  story+="slain during a police raid yesterday, according ";
                  story+="to a spokesperson from the police department.";
               }
               else
               {
                  story+="murdered during a police raid yesterday, according ";
                  story+="to a Liberal Crime Squad spokesperson.";
               }
               story+="&r";
               break;
            case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
               story+="Members of the Liberal Crime Squad were ";
               if(!liberalguardian)
               {
                  story+="slain trying to escape from a police siege yesterday, according ";
                  story+="to a spokesperson from the police department.";
               }
               else
               {
                  story+="murdered trying to escape from a police siege yesterday, according ";
                  story+="to a Liberal Crime Squad spokesperson.";
               }
               story+="&r";
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
                        story+="A group calling itself the Liberal Crime Squad ";
                        story+="burst onto the scene of political activism yesterday, according ";
                        story+="to a spokesperson from the police department.";
                        story+="&r";
                     }
                     else
                     {
                        story+="A group of thugs calling itself the Liberal Crime Squad ";
                        story+="went on a rampage yesterday, according ";
                        story+="to a spokesperson from the police department.";
                     }
                  }
                  else
                  {
                     if(ns.positive)
                     {
                        story+="The Liberal Crime Squad has struck again.  ";
                        story+="&r";
                     }
                     else
                     {
                        if(!liberalguardian)
                           story+="The Liberal Crime Squad has gone on a rampage.  ";
                        else
                           story+="A Liberal Crime Squad operation went horribly wrong.  ";
                        story+="&r";
                     }
                  }
               }
               else if(ns.type==NEWSSTORY_CCS_SITE)
               {
                  if(newscherrybusted<2)
                  {
                     if(ns.positive&&!liberalguardian)
                     {
                        story+="A group of M16-wielding vigilantes calling itself the Conservative Crime Squad ";
                        story+="burst onto the scene of political activism yesterday, according ";
                        story+="to a spokesperson from the police department.";
                        story+="&r";
                     }
                     else
                     {
                        story+="A group of worthless M16-toting hicks calling itself the Conservative Crime Squad ";
                        story+="went on a rampage yesterday, according ";
                        story+="to a spokesperson from the police department.";
                     }
                  }
                  else
                  {
                     if(ns.positive&&!liberalguardian)
                     {
                        story+="The Conservative Crime Squad has struck again.  ";
                        story+="&r";
                     }
                     else
                     {
                        story+="The Conservative Crime Squad has gone on a rampage.  ";
                        story+="&r";
                     }
                  }
               }
               else if(ns.type==NEWSSTORY_CCS_KILLED_SITE)
               {
                  if(newscherrybusted<2)
                  {
                     if(ns.positive&&!liberalguardian)
                     {
                        story+="A group of M16-wielding vigilantes calling itself the Conservative Crime Squad ";
                        story+="burst briefly onto the scene of political activism yesterday, according ";
                        story+="to a spokesperson from the police department.  ";
                        story+="&r";
                     }
                     else
                     {
                        story+="A group of worthless M16-toting hicks calling itself the Conservative Crime Squad ";
                        story+="went on a suicidal rampage yesterday, according ";
                        story+="to a spokesperson from the police department.  ";
                        story+="&r";
                     }
                  }
                  else
                  {
                     if(ns.positive&&!liberalguardian)
                     {
                        story+="The Conservative Crime Squad has struck again, albeit with a tragic end.  ";
                        story+="&r";
                     }
                     else
                     {
                        story+="The Conservative Crime Squad has gone on a rampage, and they got what they deserved.  ";
                        story+="&r";
                     }
                  }
               }
               else
               {
                  if(!newscherrybusted&&!liberalguardian)
                  {
                     if(ns.positive)
                     {
                        story+="A group calling itself the Liberal Crime Squad ";
                        story+="burst briefly onto the scene of political activism yesterday, according ";
                        story+="to a spokesperson from the police department.  ";
                        story+="&r";
                     }
                     else
                     {
                        story+="A group of thugs calling itself the Liberal Crime Squad ";
                        story+="went on a suicidal rampage yesterday, according ";
                        story+="to a spokesperson from the police department.  ";
                        story+="&r";
                     }
                  }
                  else
                  {
                     if(ns.positive)
                     {
                        story+="The Liberal Crime Squad has struck again, albeit with a tragic end.  ";
                        story+="&r";
                     }
                     else
                     {
                        if(!liberalguardian)
                           story+="The Liberal Crime Squad has gone on a rampage, and they got what they deserved.  ";
                        else
                           story+="A Liberal Crime Squad operation went horribly wrong, and came to a tragic end.  ";
                        story+="&r";
                     }
                  }
               }
               if(ns.type==NEWSSTORY_CCS_KILLED_SITE||
                  ns.type==NEWSSTORY_CCS_SITE)
                  ccs=1;

               story+="  The events took place at the ";
               if(liberalguardian&&!ccs)story+="notorious ";
               if(ccs)
               {
                  switch(location[ns.loc]->type)
                  {
                  case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
                     story+="University Dormitory.  ";break;
                  case SITE_BUSINESS_CIGARBAR:
                     story+="Lady Luck Strip Club.  ";break;
                  case SITE_LABORATORY_COSMETICS:
                     story+="Animal Shelter.  ";break;
                  case SITE_LABORATORY_GENETIC:
                     story+="Research Ethics Commission HQ.  ";break;
                  case SITE_GOVERNMENT_POLICESTATION:
                     story+="Seedy Back Alley(tm).  ";break;
                  case SITE_GOVERNMENT_COURTHOUSE:
                     story+="Abortion Clinic.  ";break;
                  case SITE_GOVERNMENT_PRISON:
                     story+="Ace Ghetto Pool Hall.  ";break;
                  case SITE_GOVERNMENT_INTELLIGENCEHQ:
                     story+="ACLU Branch Office.  ";break;
                  case SITE_INDUSTRY_SWEATSHOP:
                     story+="Labor Union HQ.  ";break;
                  case SITE_INDUSTRY_POLLUTER:
                     story+="Greenpeace Offices.  ";break;
                  case SITE_INDUSTRY_NUCLEAR:
                     story+="Whirled Peas Museum.  ";break;
                  case SITE_CORPORATE_HEADQUARTERS:
                     story+="Welfare Assistance Agency.  ";break;
                  case SITE_CORPORATE_HOUSE:
                     story+="Food Bank.  ";break;
                  case SITE_MEDIA_AMRADIO:
                     story+="Public Radio Station.  ";break;
                  case SITE_MEDIA_CABLENEWS:
                     story+="Network News Station.  ";break;
                  default:
                     story+=location[ns.loc]->name;
                     story+=".  ";break;
                  }
               }
               else story+=location[ns.loc]->name;
               if(liberalguardian&&!ccs)
               {
                  switch(location[ns.loc]->type)
                  {
                  case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
                     story+=", known for its rich and snooty residents.  ";break;
                  case SITE_BUSINESS_CIGARBAR:
                     story+=", a spawning ground of Wrong Conservative Ideas.  ";break;
                  case SITE_LABORATORY_COSMETICS:
                     story+=", a Conservative animal rights abuser.  ";break;
                  case SITE_LABORATORY_GENETIC:
                     story+=", a dangerous Conservative genetic research lab.  ";break;
                  case SITE_GOVERNMENT_POLICESTATION:
                     story+=", headquarters of one of the most oppressive and Conservative police forces in the country.  ";break;
                  case SITE_GOVERNMENT_COURTHOUSE:
                     story+=", site of numerous Conservative Injustices.  ";break;
                  case SITE_GOVERNMENT_PRISON:
                     story+=", where innocent people are regularly beaten by Conservative guards.  ";break;
                  case SITE_GOVERNMENT_INTELLIGENCEHQ:
                     story+=", the Conservative headquarters of one of the biggest privacy violators in the world.  ";break;
                  case SITE_INDUSTRY_SWEATSHOP:
                     story+=", a Conservative sweatshop and human rights abuser.  ";break;
                  case SITE_INDUSTRY_POLLUTER:
                     story+=", a factory whose Conservative smokestacks choke the city with deadly pollutants.  ";break;
                  case SITE_INDUSTRY_NUCLEAR:
                     story+=", also known to be a Conservative storage facility for radioactive waste.  ";break;
                  case SITE_CORPORATE_HEADQUARTERS:
                     story+=", where evil and Conservatism coagulate in the hallways.  ";break;
                  case SITE_CORPORATE_HOUSE:
                     story+=", a building with enough square footage enough to house a hundred people if it weren't in Conservative Hands.  ";break;
                  case SITE_MEDIA_AMRADIO:
                  case SITE_MEDIA_CABLENEWS:
                     story+=", known for its Extreme Conservative Bias.  ";break;
                  default:
                     story+=".  ";break;
                  }
               }
               else if(!ccs)
                  story+=".  ";

               if(ns.type==NEWSSTORY_SQUAD_KILLED_SITE)
               {
                  if(liberalguardian)story+="Unfortunately, the LCS group was defeated by the forces of evil.  ";
                  else if(ns.positive)story+="Everyone in the LCS group was arrested or killed.  ";
                  else story+="Fortunately, the LCS thugs were stopped by brave citizens.  ";
               }
               if(ns.type==NEWSSTORY_CCS_KILLED_SITE)
               {
                  if(ns.positive&&!liberalguardian)story+="Everyone in the CCS group was arrested or killed.  ";
                  else story+="Fortunately, the CCS thugs were stopped by brave citizens.  ";
               }
               story+="&r";

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
                     story+="  Accorded to sources that were at the scene, ";
                     story+="the Liberal Crime Squad caused the power out that struck the state ";
                     story+="yesterday by tampering with equipment on the site.";
                     story+="&r";
                  }
                  else
                  {
                     story+="  The Liberal Crime Squad caused the power outage that struck the state yesterday, ";
                     story+="demonstrating the extreme vulnerability and danger of Nuclear Power Plants. ";
                     story+="&r";
                  }
               }
               if(crime[CRIME_POLICE_LOCKUP])
               {
                  if(!liberalguardian)
                  {
                     story+="  Accorded to sources that were at the scene, ";
                     story+="the Liberal Crime Squad allegedly freed or attempted to free prisoners from the police lockup.";
                     story+="&r";
                  }
                  else
                  {
                     story+="  The Liberal Crime Squad attempted to rescue innocent people from the police lockup, ";
                     story+="saving them from torture and brutality at the hands of Conservative police interrogators.";
                     story+="&r";
                  }
               }
               if(crime[CRIME_COURTHOUSE_LOCKUP])
               {
                  if(!liberalguardian)
                  {
                     story+="  Accorded to sources that were at the scene, ";
                     story+="the Liberal Crime Squad allegedly freed or attempted to free prisoners from the court house lockup.";
                     story+="&r";
                  }
                  else
                  {
                     story+="  The Liberal Crime Squad attempted to rescue innocent people from the court house lockup, ";
                     story+="saving them from the highly corrupt Conservative justice system.";
                     story+="&r";
                  }
               }
               if(crime[CRIME_PRISON_RELEASE])
               {
                  if(!liberalguardian)
                  {
                     story+="  Accorded to sources that were at the scene, ";
                     story+="the Liberal Crime Squad allegedly freed prisoners while in the facility.";
                     story+="&r";
                  }
                  else
                  {
                     story+="  The Liberal Crime Squad attempted to rescue innocent people from the Conservative conditions ";
                     story+="at the prison.";
                     story+="&r";
                  }
               }
               if(crime[CRIME_JURYTAMPERING])
               {
                  if(!liberalguardian)
                  {
                     story+="  Accorded to police sources that were at the scene, ";
                     story+="the Liberal Crime Squad allegedly violated the sacred ";
                     story+="trust and attempted to influence a jury.";
                     story+="&r";
                  }
                  else
                  {
                     story+="  The Liberal Crime Squad has apologized over reports that the operation ";
                     story+="interfered with jury deliberations.";
                     story+="&r";
                  }
               }
               if(crime[CRIME_HACK_INTEL])
               {
                  if(!liberalguardian)
                  {
                     story+="  Accorded to police sources that were at the scene, ";
                     story+="intelligence officials seemed very nervous about something.";
                     story+="&r";
                  }
                  else
                  {
                     story+="  Liberal Crime Squad hackers worked to penetrate CIA computers.";
                     story+="&r";
                  }
               }
               if(crime[CRIME_HOUSE_PHOTOS])
               {
                  if(!liberalguardian)
                  {
                     story+="  Accorded to police sources that were at the scene, ";
                     story+="the owner of the house seemed very frantic about some missing property.";
                     story+="&r";
                  }
                  else
                  {
                     story+="  The Liberal Crime Squad was attempting to uncover the CEO's Conservative corruption.";
                     story+="&r";
                  }
               }
               if(crime[CRIME_CORP_FILES])
               {
                  if(!liberalguardian)
                  {
                     story+="  Accorded to police sources that were at the scene, ";
                     story+="executives on the scene seemed very nervous about something.";
                     story+="&r";
                  }
                  else
                  {
                     story+="  The Liberal Crime Squad was attempting to uncover the company's Conservative corruption.";
                     story+="&r";
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
                        story+="  Further details are sketchy, but police sources suggest that the LCS ";
                        story+="engaged in ";
                     }
                     else
                     {
                        story+="  The Liberal Crime Squad ";
                     }
                  }
                  else
                  {
                     story+="  Further details are sketchy, but police sources suggest that the CCS ";
                        story+="engaged in ";
                  }
                  if(!liberalguardian||ccs)
                  {
                     if(crime[CRIME_KILLEDSOMEBODY])
                     {
                        story+="murder";
                        if(typesum>=3)story+=", ";
                        else if(typesum==2)story+=" and ";
                        typesum--;
                     }
                     if(crime[CRIME_ATTACKED_MISTAKE])
                     {
                        story+="violence";
                        if(typesum>=3)story+=", ";
                        else if(typesum==2)story+=" and ";
                        typesum--;
                     }
                     if(crime[CRIME_ATTACKED])
                     {
                        if(crime[CRIME_ATTACKED_MISTAKE])story+="more violence";
                        else story+="violence";
                        if(typesum>=3)story+=", ";
                        else if(typesum==2)story+=" and ";
                        typesum--;
                     }
                  }
                  else
                  {
                     if(crime[CRIME_ATTACKED])
                     {
                        story+="engaged in combat with Conservative forces";
                        if(typesum>=3)story+=", ";
                        else if(typesum==2)story+=" and ";
                        typesum--;
                     }
                  }
                  if(crime[CRIME_STOLEGROUND])
                  {
                     if(!liberalguardian||ccs)
                     {
                        story+="theft";
                     }
                     else
                     {
                        story+="gained control of enemy equipment";
                     }
                     if(typesum>=3)story+=", ";
                     else if(typesum==2)story+=" and ";
                     typesum--;
                  }
                  if(crime[CRIME_FREE_RABBITS]||crime[CRIME_FREE_BEASTS])
                  {
                     if(!liberalguardian)
                     {
                        story+="tampering with lab animals";
                     }
                     else
                     {
                        story+="liberated abused lab animals";
                     }

                     if(typesum>=3)story+=", ";
                     else if(typesum==2)story+=" and ";
                     typesum--;
                  }
                  if(crime[CRIME_BREAK_SWEATSHOP]||crime[CRIME_BREAK_FACTORY])
                  {
                     if(!liberalguardian||ccs)
                     {
                        story+="destruction of private property";
                     }
                     else
                     {
                        story+="damaged enemy infrastructure";
                     }

                     if(typesum>=3)story+=", ";
                     else if(typesum==2)story+=" and ";
                     typesum--;
                  }
                  if(crime[CRIME_BROKEDOWNDOOR])
                  {
                     if(!liberalguardian||ccs)
                     {
                        story+="breaking and entering";
                     }
                     else
                     {
                        story+="forced entry into a secure area";
                     }

                     if(typesum>=3)story+=", ";
                     else if(typesum==2)story+=" and ";
                     typesum--;
                  }
                  if(crime[CRIME_UNLOCKEDDOOR])
                  {
                     if(!liberalguardian||ccs)
                     {
                        story+="unlawful entry";
                     }
                     else
                     {
                        story+="evaded Conservative security measures";
                     }

                     if(typesum>=3)story+=", ";
                     else if(typesum==2)story+=" and ";
                     typesum--;
                  }
                  story+=".&r";
               }

               if(crime[CRIME_CARCHASE])
               {
                  if(!liberalguardian||ccs)
                  {
                     story+="  It is known that there was a high-speed chase ";
                     story+="following the incident.  ";
                  }
                  else
                  {
                     story+="  Conservative operatives engaged in a dangerous ";
                     story+="pursuit of the LCS.  ";
                  }

                  if(crime[CRIME_CARCRASH])
                  {
                     if(crime[CRIME_CARCRASH]>1)
                     {
                        char num[3];
                        itoa(crime[CRIME_CARCRASH],num,10);
                        story+=num;
                        story+=" vehicles crashed.  ";
                     }
                     else story+="One vehicle crashed.  ";
                     if(!liberalguardian||ccs)
                        story+="Details about injuries were not released.  ";
                  }

                  if(crime[CRIME_FOOTCHASE])
                  {
                     if(!liberalguardian||ccs)
                        story+="There was also a foot chase when the suspect or suspects bailed out after the high-speed pursuit.  ";
                     else
                        story+="The Liberal Crime Squad ended the dangerous high-speed chase to protect the public, and attempted to escape on foot.  ";
                  }
                  story+="&r";
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

         story=lcityname;
         story+=" - ";
         if(ns.crime[1]>2)
         {
            char num[20];
            itoa(ns.crime[1],num,10);
            story+=num;
            story+=" bodies were ";
         }
         else if(ns.crime[1]>1)story+=" Two bodies were ";
         else story+=" A body was ";
         story+=" found in the ";
         story+=location[ns.loc]->name;
         story+=" yesterday.";
         if(!liberalguardian)
         {
            story+="  According to a spokesperson for ";
            story+="the police department, the matter is under investigation as a homicide.";
            story+="&r";
            story+="  Privately, sources in the department confide that there aren't any leads.  ";
            story+="According to one person familiar with the case, \"";
         }
         else
         {
            story+="  The police have opened an investigation into the massacre, but seem ";
            story+="unwilling to pursue the case with all of their effort.";
            story+="&r";
            story+="  The Liberal Crime Squad has claimed that the ";
            if(ns.crime[1]>1)story+="victims were ";
            else story+="victim was ";
            story+="a member of the LCS targeted simply due to their affiliation.  ";
            story+="According to an LCS spokesperson, \"";
         }
         switch(ns.crime[0])
         {
            case SIEGE_CIA:
               if(!liberalguardian)
               {
                  if(ns.crime[1]>1)story+="The bodies had no faces or ";
                  else story+="The body had no face or ";
                  story+="fingerprints.  Like, it was all smooth.  ";
                  story+="D*mnd*st thing I've ever seen";
               }
               else
               {
                  story+="We have strong evidence that this was an extra-judicial slaughter ";
                  story+="carried out by the Central Intelligence Agency in retaliation for our ";
                  story+="previous actions to uncover human rights abuses and corruption in the ";
                  story+="intelligene community";
               }
               break;
            case SIEGE_POLICE:
            case SIEGE_HICKS:
               if(!liberalguardian)
               {
                  story+="Burned...  stabbed with, maybe, pitchforks.  There may have been bite marks.  Nothing recognizable left";
               }
               else
               {
                  story+="We have reason to believe that this brutal massacre was ";
                  story+="inspired by the Conservative media's brainwashing propaganda";
               }
               break;
            case SIEGE_CORPORATE:
               if(!liberalguardian)
               {
                  story+="It was execution style.  Professional.  We've got nothing";
               }
               else
               {
                  story+="This massacre has the signature mark of a group of mercenaries ";
                  story+="known to work with several corporations we've had confrotations ";
                  story+="with in the past.  If the police can't figure this one out, they're ";
                  story+="just covering it up";
               }
               break;
         }
         story+=".\"  ";
         story+="&r";

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

         story=lcityname;
         story+=" - The disappearance of ";
         story+=ns.cr->propername;
         story+=" is now considered a kidnapping, ";
         story+="accorded to a police spokesperson.";
         story+="&r";

         string dstr,dstr2;
         story+="  ";
         firstname(dstr);lastname(dstr2);
         story+=dstr;
         story+=" ";
         story+=dstr2;
         story+=", speaking on behalf of the police department, stated ";
         story+="\"We now believe that ";
         story+=ns.cr->propername;
         story+=" was taken ";
         char num[20];
         itoa(ns.cr->joindays-1,num,10);
         story+=num;
         story+=" days ago, by a person or persons as yet undetermined.  ";
         story+="We have several leads and are confident that we will ";
         story+="bring ";
         story+=ns.cr->propername;
         story+=" back home and bring the kidnappers to justice.  ";
         story+="As the investigation is ongoing, I cannot be more specific at this time.  ";
         story+="To the citizens, please contact the department if you have any additional information.";
         story+="\"";
         story+="&r";
         story+="  According to sources, ";
         story+=ns.cr->propername;
         story+="'s last known location was the ";
         story+=location[ns.cr->worklocation]->name;
         story+=".  Police were seen searching the surrounding area yesterday.";
         story+="&r";

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



void displaycenterednewsfont(const char *str,int y)
{
   int width=-1;
   int s;
   int length=strlen(str);
   for(s=0;s<length;s++)
   {
      if(str[s]>='A'&&str[s]<='Z')width+=6;
      else if(str[s]=='\'')width+=4;
      else width+=3;
   }

   int x=39-width/2;

   for(s=0;s<length;s++)
   {
      if((str[s]>='A'&&str[s]<='Z')||str[s]=='\'')
      {
         int p;
         if(str[s]>='A'&&str[s]<='Z')p=str[s]-'A';
         else p=26;
         int lim=6;
         if(str[s]=='\'')lim=4;
         if(s==length-1)lim--;
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



void displaycenteredsmallnews(const char *str,int y)
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
void constructeventstory(string& story,short view,char positive)
{
   story="";

   if(positive)
   {
      switch(view)
      {
         case VIEW_ABORTION:
         {
            string str,str2;
            string gen;
            cityname(str);
            story+=str;
            story+=" - ";
            story+="A doctor that routinely performed abortions was ruthlessly ";
            story+="gunned down outside of the ";
            lastname(str);story+=str;
            story+=" CLINIC yesterday.  ";
            story+="Dr. ";
            string dstr,dstr2;
            firstname(dstr);lastname(dstr2);
            story+=dstr;
            story+=" ";
            story+=dstr2;
            char gn=LCSrandom(2);
            switch(gn)
            {
               case 0:gen="his";break;
               case 1:gen="her";break;
            }
            story+=" was walking to ";story+=gen;
            story+=" car when, according to police reports, ";
            story+="shots were fired from a nearby vehicle.  ";
            story+=dstr2;
            story+=" was hit ";
            char num[20];
            itoa(LCSrandom(15)+3,num,10);
            story+=num;
            story+=" times and died immediately in the parking lot.  ";
            story+="The suspected shooter, ";
            firstname(str);story+=str;
            story+=" ";
            lastname(str2);story+=str2;
            story+=", is in custody.&r";
            story+="  Witnesses report that ";
            story+=str2;
            story+=" remained at the scene after the shooting, screaming ";
            story+="verses of the Bible at the stunned onlookers.  Someone ";
            story+="called the police on a cellphone and they arrived shortly thereafter.  ";
            story+=str2;
            story+=" surrendered without a struggle, reportedly saying that God's work ";
            story+="had been completed.&r";
            story+="  ";
            story+=dstr2;
            story+=" is survived by ";
            story+=gen;
            story+=" ";
            char spouse=LCSrandom(2);
            if(law[LAW_GAY]<=1)
            {
               spouse=1-gn;
            }
            switch(spouse)
            {
               case 0:story+="husband";break;
               case 1:story+="wife";break;
            }
            story+=" and ";
            switch(LCSrandom(3))
            {
               case 0:story+="two";break;
               case 1:story+="three";break;
               case 2:story+="five";break;
            }
            story+=" children.&r";
            break;
         }
         case VIEW_GAY:
         {
            cityname(story);
            story+=" - ";
            string dstr,dstr2;
            firstname(dstr);lastname(dstr2);
            story+=dstr;
            story+=" ";
            story+=dstr2;
            if(law[LAW_GAY]<=-1)story+=", a known homosexual, was ";
            else story+=", a homosexual, was ";
            switch(LCSrandom(3))
            {
               case 0:story+="dragged to death behind a pickup truck";break;
               case 1:story+="burned alive";break;
               case 2:story+="beaten to death";break;
            }
            story+=" here yesterday.  ";
            story+="A police spokesperson reported that four suspects ";
            story+="were apprehended after a high speed chase.  Their names ";
            story+="have not yet been released.";
            story+="&r";
            story+="  Witnesses of the freeway chase described the pickup of the alleged ";
            story+="murderers swerving wildly, ";
            switch(LCSrandom(3))
            {
               case 0:story+="throwing beer bottles";break;
               case 1:story+="urinating out the window";break;
               case 2:story+="taking swipes";break;
            }
            story+=" at the pursuing police cruisers.  ";
            story+="The chase ended when ";
            switch(LCSrandom(3))
            {
               case 0:story+="the suspects ran out of gas, ";break;
               case 1:story+="the suspects collided with a manure truck, ";break;
               case 2:story+="the suspects veered into a ditch, ";break;
            }
            story+="at which point they were taken into custody.  Nobody was seriously injured during the incident.";
            story+="&r";
            story+="  Authorities have stated that they will vigorously ";
            story+="prosecute this case as a hate crime, due to the ";
            story+="aggravated nature of the offense";
            if(law[LAW_GAY]==-2)story+=", even though being gay is deviant, as we all know.";
            else story+=".";
            story+="&r";
            break;
         }
         case VIEW_DEATHPENALTY:
         {
            story+="Texas - ";
            story+="An innocent citizen has been put to death in the electric chair.  ";
            string dstr,dstr2,dstr3;
            firstname(dstr);firstname(dstr2);lastname(dstr3);
            story+=dstr;
            story+=" ";
            story+=dstr2;
            story+=" ";
            story+=dstr3;
            story+=" was pronounced dead at 7:06PM yesterday at the ";
            string jstr;
            lastname(jstr);
            story+=jstr;
            story+=" Correctional Facility.&r";

            story+="  ";
            story+=dstr3;
            story+=" was convicted in ";
            char num[20];
            itoa(year-LCSrandom(11)-10,num,10);
            story+=num;
            story+=" of thirteen serial murders.  ";
            story+="Since then, numerous pieces of exculpatory evidence ";
            story+="have been produced, including ";
            switch(LCSrandom(3))
            {
               case 0:story+="a confession from another convict.  ";break;
               case 1:story+="a battery of negative DNA tests.  ";break;
               case 2:
                  story+="an admission from a former prosecutor that ";
                  story+=dstr3;
                  story+=" was framed.  ";
                  break;
            }
            story+="The state still went through with the execution, with a ";
            story+="spokesperson for the governor saying, ";
            story+="\"";
            switch(LCSrandom(3))
            {
               case 0:story+="Let's not forget the convict is colored.  You know how their kind are";break;
               case 1:
                  story+="The convict is always referred to by three names.  ";
                  story+="Assassin, serial killer, either way -- guilty.  ";
                  story+="End of story";
                  break;
               case 2:story+="The family wants closure.  We don't have time for another trial";break;
            }
            story+=".\"";
            story+="&r";
            story+="  Candlelight vigils were held throughout the country last night during the execution, ";
            story+="and more events are expected this evening.  If there is a bright side to be found from this ";
            story+="tragedy, it will be that our nation is now evaluating the ease with which people ";
            story+="can be put to death in this country.";
            story+="&r";
            break;
         }
        /* case VIEW_GUNS:
         {
            cityname(story);
            strcat(" - ");
            story+="A student has gone on a shooting rampage at a local high school.  ";
            string dstr,dstr2;
            firstname(dstr);lastname(dstr2);
            story+=dstr;
            story+=" ";
            story+=dstr2;
            story+=", 15, used a variety of weapons to shoot more than a dozen classmates and two teachers at ";
            string jstr;
            lastname(jstr);
            story+=jstr;
            story+=" High School, before committing suicide.&r";

            story+="  ";
            story+=dstr;
            story+=" was considered an outcast ";
            char num[20];
            itoa(year-LCSrandom(11)-10,num,10);
            story+=num;
            story+=" of thirteen serial murders.  ";
            story+="Since then, numerous pieces of exculpatory evidence ";
            story+="have been produced, including ";
            switch(LCSrandom(3))
            {
               case 0:story+="a confession from another convict.  ";break;
               case 1:story+="a battery of negative DNA tests.  ";break;
               case 2:
                  story+="an admission from a former prosecutor that ";
                  story+=dstr3;
                  story+=" was framed.  ";
                  break;
            }
            story+="The state still went through with the execution, with a ";
            story+="spokesperson for the governor saying, ";
            story+="\"";
            switch(LCSrandom(3))
            {
               case 0:story+="Let's not forget the convict is colored.  You know how their kind are";break;
               case 1:
                  story+="The convict is always referred to by three names.  ";
                  story+="Assassin, serial killer, either way -- guilty.  ";
                  story+="End of story";
                  break;
               case 2:story+="The family wants closure.  We don't have time for another trial";break;
            }
            story+=".\"";
            story+="&r";
            story+="  Candlelight vigils were held throughout the country last night during the execution, ";
            story+="and more events are expected this evening.  If there is a bright side to be found from this ";
            story+="tragedy, it will be that our nation is now evaluating the ease with which people ";
            story+="can be put to death in this country.";
            story+="&r";
            break;
         } */
         case VIEW_PRISONS:
         {
            cityname(story);
            story+=" - A former prisoner has written a book describing in horrifying ";
            story+="detail what goes on behind bars.  ";
            story+="Although popular culture has used, or perhaps overused, the ";
            story+="prison theme lately in its offerings for mass consumption, rarely ";
            story+="have these works been as poignant as ";
            string dstr,dstr2;
            firstname(dstr);lastname(dstr2);
            story+=dstr;
            story+=" ";
            story+=dstr2;
            story+="'s new tour-de-force, _";
            switch(LCSrandom(5))
            {
               case 0:story+="Nightmare";break;
               case 1:story+="Primal";break;
               case 2:story+="Animal";break;
               case 3:story+="American";break;
               case 4:story+="Solitary";break;
            }
            story+="_";
            switch(LCSrandom(7))
            {
               case 0:story+="Packer";break;
               case 1:story+="Soap";break;
               case 2:story+="Punk";break;
               case 3:story+="Kid";break;
               case 4:story+="Cell";break;
               case 5:story+="Shank";break;
               case 6:story+="Lockdown";break;
               case 7:story+="Buttlord";break;
            }
            story+="_.&r";
            story+="   Take this excerpt, \"";

            story+="The steel bars grated forward in their rails, ";
            story+="coming to a halt with a deafening clang that said it all -- ";
            story+="I was trapped with them now.  There were three, looking me over ";
            story+="with dark glares of bare lust, as football players might stare at a stupified, drunken, helpless teenager.  ";
            story+="My shank's under the mattress.  Better to be a man and fight or a punk and let them take it?  ";
            story+="Maybe lose an eye the one way, maybe catch AIDS the other.  A helluva choice, ";
            story+="and I would only have a few seconds before they made it for me";
            story+=".\"";
            story+="&r";
            break;
         }
         case VIEW_INTELLIGENCE:
         {
            story+="Washington D.C. - The FBI might be keeping tabs on you.  ";
            story+="This newspaper yesterday received a collection of files from a source in the Federal Bureau of Investigations.  ";
            story+="The files contain information on which people have been attending demonstrations, organizing ";
            story+="unions, working for liberal organizations -- even ";
            switch(LCSrandom(2))
            {
               case 0:story+="buying music with 'Explicit Lyrics' labels.";break;
               case 1:story+="helping homeless people";break;
            }
            story+=".";
            story+="&r";
            story+="  More disturbingly, the files make reference to a plan to ";
            story+="\"deal with the undesirables\", although this phrase is not clarified.  ";
            story+="&r";
            story+="  The FBI refused to comment initially, but when confronted with the information, ";
            story+="a spokesperson stated, \"";
            story+="Well, you know, there's privacy, and there's privacy.  ";
            story+="It might be a bit presumptive to assume that ";
            story+="these files deal with the one and not the other.  ";
            story+="You think about that before you continue slanging accusations";
            story+=".\"";
            story+="&r";
            break;
         }
         case VIEW_FREESPEECH:
         {
            cityname(story);
            story+=" - A children's story has been removed from libraries here after ";
            story+="the city bowed to pressure from religious groups.";
            story+="&r";
            story+="   The book, ";
            story+="_";
            string dstr,nstr;
            firstname(nstr);
            story+=nstr;
            story+="_";
            lastname(dstr);
            story+=dstr;
            story+="_and_the_";
            switch(LCSrandom(5))
            {
               case 0:story+="Mysterious";break;
               case 1:story+="Magical";break;
               case 2:story+="Golden";break;
               case 3:story+="Invisible";break;
               case 4:story+="Wondrous";break;
            }
            story+="_";
            switch(LCSrandom(5))
            {
               case 0:story+="Thing";break;
               case 1:story+="Stuff";break;
               case 2:story+="Object";break;
               case 3:story+="Whatever";break;
               case 4:story+="Something";break;
            }
            story+="_, is the third in an immensely popular series by ";
            switch(LCSrandom(5))
            {
               case 0:story+="British";break;
               case 1:story+="Indian";break;
               case 2:story+="Chinese";break;
               case 3:story+="Rwandan";break;
               case 4:story+="Palestinian";break;
            }
            story+=" author ";
            char c[2];
            c[0]='A'+LCSrandom(26);
            c[1]='\x0';
            story+=c;
            story+=".";
            c[0]='A'+LCSrandom(26);
            c[1]='\x0';
            story+=c;
            story+=". ";
            lastname(dstr);
            story+=dstr;
            story+=".  ";
            story+="Although the series is adored by children worldwide, ";
            story+="some conservatives feel that the books ";
            switch(LCSrandom(5))
            {
               case 0:story+="glorify Satan worship and are spawned by demons from the pit.  ";break;
               case 1:story+="teach children to kill their parents and hate life.  ";break;
               case 2:story+="cause violence in schools and are a gateway to cocaine use.  ";break;
               case 3:story+="breed demonic thoughts that manifest themselves as dreams of murder.  ";break;
               case 4:story+="contain step-by-step instructions to summon the Prince of Darkness.  ";break;
            }
            story+="In their complaint, the groups cited an incident involving ";
            switch(LCSrandom(3))
            {
               case 0:story+="a child that swore in class";break;
               case 1:story+="a child that said a magic spell at her parents";break;
               case 2:story+="a child that pushed her older sister";break;
            }
            story+=" as key evidence of the dark nature of the book.";
            story+="&r";
            story+="   When the decision to ban the book was announced yesterday, ";
            story+="many area children spontoneously broke into tears.  One child was ";
            story+="heard saying, \"";
            switch(LCSrandom(2))
            {
               case 0:
                  story+="Mamma, is ";
                  story+=nstr;
                  story+=" dead?";
                  break;
               case 1:
                  story+="Mamma, why did they kill ";
                  story+=nstr;
                  story+="?";
                  break;
            }
            story+="\"";
            story+="&r";
            break;
         }
         case VIEW_JUSTICES:
         {
            cityname(story);
            story+=" - Conservative federal judge ";
            string dstr,dstr2;
            firstname(dstr);lastname(dstr2);
            story+=dstr;
            story+=" ";
            story+=dstr2;
            story+=" has resigned in disgrace after being caught with a prostitute.";
            story+="&r";
            story+="  ";
            story+=dstr2;
            story+=", who once ";
            switch(LCSrandom(2))
            {
               case 0:story+="defied the federal government by putting a Ten Commandments monument in the local federal building";break;
               case 1:story+="stated that, \"Segregation wasn't the bad idea everybody makes it out to be these days\"";break;
            }
            story+=", was found with ";
            string pstr,pstr2;
            firstname(pstr);lastname(pstr2);
            story+=pstr;
            story+=" ";
            story+=pstr2;
            story+=" last week in a hotel during a police sting operation.  ";
            story+="According to sources familiar with the particulars, ";
            story+="when police broke into the hotel room they saw ";
            switch(LCSrandom(3))
            {
               case 0:story+="\"the most perverse and spine-tingling debauchery imaginable, at least with only two people.\"";break;
               case 1:story+="the judge relieving himself on the prostitute.";break;
               case 2:story+="the prostitute hollering like a cowboy astride the judge.";break;
            }
            story+="  ";
            story+=pstr2;
            story+=" reportedly offered ";
            switch(LCSrandom(3))
            {
               case 0:story+="the arresting officers money";break;
               case 1:story+="to let the officers join in";break;
               case 2:story+="the arresting officers \"favors\"";break;
            }
            story+=" in exchange for their silence.";
            story+="&r";
            story+="  ";
            story+=dstr2;
            story+=" could not be reached for comment, although an aid stated that ";
            story+="the judge would be going on a Bible retreat for a few weeks to ";
            story+="\"Make things right with the Almighty Father.\"  ";
            story+="&r";
            break;
         }
         case VIEW_AMRADIO:
         {
            cityname(story);
            story+=" - Well-known AM radio personality ";
            string dstr,dstr2;
            firstname(dstr);lastname(dstr2);
            story+=dstr;
            story+=" ";
            story+=dstr2;
            story+=" went off for fifteen minutes in an inexplicable rant ";
            story+="two nights ago during the syndicated radio program \"";
            switch(LCSrandom(3))
            {
               case 0:story+="Straight";break;
               case 1:story+="Real";break;
               case 2:story+="True";break;
            }
            story+=" ";
            switch(LCSrandom(3))
            {
               case 0:story+="Talk";break;
               case 1:story+="Chat";break;
               case 2:story+="Discussion";break;
            }
            story+="\".";
            story+="&r";
            story+="  ";
            story+=dstr2;
            story+="'s monologue for the evening began the way that fans ";
            story+="had come to expect, with attacks on the \"liberal media establishment\" and ";
            story+="the \"elite liberal agenda\".  But when the radio icon said, \"";
            switch(LCSrandom(3))
            {
               case 0:story+="and the Grays are going to take over the planet in the End Times";break;
               case 1:story+="a liberal chupacabra will suck the blood from us like a goat, a goat!, a goat!";break;
               case 2:story+="I feel translucent rods passing through my body...  it's like making love to the future";break;
            }
            story+="\", a former fan of the show, ";
            string nstr,nstr2;
            firstname(nstr);lastname(nstr2);
            story+=nstr;
            story+=" ";
            story+=nstr2;
            story+=", knew that \"";
            switch(LCSrandom(3))
            {
               case 0:story+="my old hero";break;
               case 1:story+="my old idol";break;
               case 2:story+="the legend";break;
            }
            story+=" had ";
            switch(LCSrandom(3))
            {
               case 0:
                  story+="lost ";
                  switch(LCSrandom(2))
                  {
                     case 0:story+="his";break;
                     case 1:story+="her";break;
                  }
                  story+=" g*dd*mn mind";
                  break;
               case 1:story+="maybe gone a little off the deep end";break;
               case 2:story+="probably been listening to Art Bell in the next studio a little too long";break;
            }
            story+=".  After that, it just got worse and worse.\"";
            story+="&r";
            story+="  ";
            story+=dstr2;
            story+=" issued an apology later in the program, but ";
            story+="the damage might already be done.  ";
            story+="According to a poll completed yesterday, ";
            story+="fully half of the host's most loyal supporters ";
            story+="have decided to leave the program for saner ";
            story+="pastures.  Of these, many said that they would be switching over ";
            story+="to the FM band.";
            story+="&r";
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
            story+=" - Perhaps parents can rest easier tonight.  ";
            story+="The authorities have apprehended their primary suspect in the ";
            story+="string of brutal child killings that has kept everyone in the area on edge, ";
            story+="according to a spokesperson for the police department here.  ";
            string dstr,dstr2,dstr3;
            firstname(dstr);firstname(dstr2);lastname(dstr3);
            story+=dstr;
            story+=" ";
            story+=dstr2;
            story+=" ";
            story+=dstr3;
            story+=" was detained yesterday afternoon, reportedly in possession of ";
            switch(LCSrandom(5))
            {
               case 0:story+="pieces of another victim";break;
               case 1:story+="bloody toys";break;
               case 2:story+="a child's clothing stained with DNA evidence";break;
               case 3:story+="seven junior high school yearbooks";break;
               case 4:story+="two small backpacks";break;
            }
            story+=".  Over twenty children in the past two years have gone missing, ";
            story+="only to turn up later dead and ";
            switch(LCSrandom(5))
            {
               case 0:story+="carved with satanic symbols";break;
               case 1:story+="sexually mutilated";break;
               case 2:story+="missing all of their teeth";break;
               case 3:story+="missing all of their fingers";break;
               case 4:story+="without eyes";break;
            }
            story+=".  Sources say that the police got a break in the case when ";
            switch(LCSrandom(5))
            {
               case 0:story+="a victim called 911 just prior to being slain while still on the phone";break;
               case 1:story+="the suspect allegedly carved an address into one of the bodies";break;
               case 2:story+="an eye witness allegedly spotted the suspect luring a victim into a car";break;
               case 3:story+="a blood trail was found on a road that led them to the suspect's car trunk";break;
               case 4:story+="they found a victim in a ditch, still clinging to life";break;
            }
            story+=".";
            story+="&r";
            story+="   The district attorney's office has already repeatedly said it will be ";
            story+="seeking the death penalty in this case.";
            story+="&r";
            break;
         }
         case VIEW_ANIMALRESEARCH:
         {
            cityname(story);
            story+=" - Researchers here report that they have discovered an amazing new wonder drug.  ";
            story+="Called ";
            switch(LCSrandom(5))
            {
               case 0:story+="Anal";break;
               case 1:story+="Colo";break;
               case 2:story+="Lacta";break;
               case 3:story+="Pur";break;
               case 4:story+="Loba";break;
            }
            switch(LCSrandom(5))
            {
               case 0:story+="nephrin";break;
               case 1:story+="tax";break;
               case 2:story+="zac";break;
               case 3:story+="thium";break;
               case 4:story+="drene";break;
            }
            story+=", the drug apparently ";
            switch(LCSrandom(5))
            {
               case 0:story+="boosts intelligence in chimpanzees";break;
               case 1:story+="corrects erectile dysfunction in chimpanzees";break;
               case 2:story+="allows chimpanzees to move blocks with their minds";break;
               case 3:story+="allows chimpanzees to fly short distances";break;
               case 4:story+="increases the attention span of young chimpanzees";break;
            }
            story+=".  ";
            story+="&r";
            story+="   Along with bonobos, chimpanzees are our closest cousins";
            if(law[LAW_FREESPEECH]==-2)story+=", at least according to the now-discredited theory of evolution";
            story+=".  ";
            story+="Fielding questions about the ethics of their experiments from reporters during a press conference yesterday, ";
            story+="a spokesperson for the research team stated that, \"It really isn't so bad as all that.  Chimpanzees are very resilient creatures.  ";
            switch(LCSrandom(3))
            {
               case 0:story+="The ones that survived are all doing very well";break;
               case 1:story+="They hardly notice when you drill their brains out, if you're fast";break;
               case 2:story+="When we started muffling the screams of our subjects, the other chimps all calmed down quite a bit";break;
            }
            story+=".  We have a very experienced research team.  ";
            story+="While we understand your concerns, any worries are entirely unfounded.  ";
            story+="I think the media should be focusing on the enormous benefits of this drug.";
            story+="\"";
            story+="&r";
            story+="   The first phase of human trials is slated to begin in a few months.";
            story+="&r";
            break;
         }
         case VIEW_PRISONS:
         {
            cityname(story);
            story+=" - The hostage crisis at the ";
            string jstr;
            lastname(jstr);
            story+=jstr;
            story+=" Correctional Facility ended tragically yesterday with the ";
            story+="death of both the prison guard being held hostage and ";
            switch(LCSrandom(2))
            {
               case 0:story+="his";break;
               case 1:story+="her";break;
            }
            story+=" captor.";
            story+="&r";
            story+="   Two weeks ago, convicted rapist ";
            string dstr,dstr2;
            firstname(dstr);lastname(dstr2);
            story+=dstr;
            story+=" ";
            story+=dstr2;
            story+=", an inmate at ";
            story+=jstr;
            story+=", overpowered ";
            string gstr,gstr2;
            firstname(gstr);lastname(gstr2);
            story+=gstr;
            story+=" ";
            story+=gstr2;
            story+=" and barricaded ";
            switch(LCSrandom(2))
            {
               case 0:story+="himself";break;
               case 1:story+="herself";break;
            }
            story+=" with the guard in a prison tower.  ";
            story+="Authorities locked down the prison and ";
            story+="attempted to negotiate by phone for 13 days, ";
            story+="but talks were cut short when ";
            story+=dstr2;
            story+=" reportedly screamed into the receiver \"";
            switch(LCSrandom(3))
            {
               case 0:story+="Ah, f*ck this sh*t.  This punk b*tch is f*ckin' dead!";break;
               case 1:story+="F*ck a m*th*f*ck*n' bull.  I'm killin' this pig sh*t.";break;
               case 2:story+="Why the f*ck am I talkin' to you?  I'd rather kill this pig.";break;
            }
            story+="\"";
            story+="  The tower was breached in an attempt to reach ";
            story+="the hostage, but ";
            story+=dstr2;
            story+=" had already ";
            switch(LCSrandom(3))
            {
               case 0:story+="slit the guard's throat with a shank";break;
               case 1:story+="strangled the guard to death with a knotted bed sheet";break;
               case 2:story+="chewed out the guard's throat";break;
            }
            story+=".  The prisoner was beaten to death while ";
            story+="\"resisting capture\", according to a prison spokesperson.";
            story+="&r";
            break;
         }
         case VIEW_INTELLIGENCE:
         {
            story+="Washington D.C. - The CIA announced yesterday that it has averted a terror attack that ";
            story+="would have occurred on American soil.";
            story+="&r";
            story+="   According to a spokesperson for the agency, ";
            switch(LCSrandom(3))
            {
               case 0:story+="white supremacists";break;
               case 1:story+="Islamic fundamentalists";break;
               case 2:story+="outcast goths from a suburban highschool";break;
            }
            story+=" planned to ";
            switch(LCSrandom(9))
            {
               case 0:story+="fly planes into skyscrapers";break;
               case 1:story+="detonate a fertilizer bomb at a federal building";break;
               case 2:story+="ram a motorboat loaded with explosives into a warship";break;
               case 3:story+="detonate explosives on a school bus";break;
               case 4:story+="blow out a section of a major bridge";break;
               case 5:story+="kidnap the president";break;
               case 6:story+="assassinate the president";break;
               case 7:story+="destroy the Capitol Building";break;
               case 8:story+="detonate a nuclear bomb in New York";break;
            }
            story+=".  However, intelligence garnered from deep within the mysterious ";
            story+="terrorist organization allowed the plot to be foiled just days before it ";
            story+="was to occur.";
            story+="&r";
            story+="   The spokesperson further stated, \"";
            story+="I won't compromise our sources and methods, but let me just say ";
            story+="that we are grateful to the Congress and this Administration for ";
            story+="providing us with the tools we need to neutralize these enemies of ";
            story+="civilization before they can destroy American families.  ";
            story+="However, let me also say that there's more that needs to be done.  ";
            story+="The Head of the Agency will be sending a request to Congress ";
            story+="for what we feel are the essential tools for combating terrorism in ";
            story+="this new age.";
            story+="\"";
            story+="&r";
            break;
         }
         case VIEW_GENETICS:
         {
            cityname(story);
            story+=" - The genetic foods industry staged a major event here yesterday ";
            story+="to showcase its upcoming products.  Over thirty companies set up ";
            story+="booths and gave talks to wide-eyed onlookers.";
            story+="&r";
            story+="   One such corporation, ";
            switch(LCSrandom(5))
            {
               case 0:story+="Altered";break;
               case 1:story+="Gene-tech";break;
               case 2:story+="DNA";break;
               case 3:story+="Proteomic";break;
               case 4:story+="Genomic";break;
            }
            story+=" ";
            switch(LCSrandom(5))
            {
               case 0:story+="Foods";break;
               case 1:story+="Agriculture";break;
               case 2:story+="Meals";break;
               case 3:story+="Farming";break;
               case 4:story+="Living";break;
            }
            story+=", presented their product, \"";
            switch(LCSrandom(5))
            {
               case 0:story+="Mega";break;
               case 1:story+="Epic";break;
               case 2:story+="Overlord";break;
               case 3:story+="Franken";break;
               case 4:story+="Transcendent";break;
            }
            story+=" ";
            switch(LCSrandom(5))
            {
               case 0:story+="Rice";break;
               case 1:story+="Beans";break;
               case 2:story+="Corn";break;
               case 3:story+="Wheat";break;
               case 4:story+="Potatoes";break;
            }
            story+="\", during an afternoon Power Point presentation.  ";
            story+="According to the public relations representative speaking, ";
            story+="this amazing new product actually ";
            switch(LCSrandom(5))
            {
               case 0:story+="extends human life by a few minutes every bite";break;
               case 1:story+="mends split-ends upon digestion.  Hair is also made glossier and thicker";break;
               case 2:story+="allows people to see in complete darkness";break;
               case 3:story+="causes a person to slowly attain their optimum weight with repeated use";break;
               case 4:story+="cures the common cold";break;
            }
            story+=".";
            story+="&r";
            story+="   Spokespeople for the GM corporations were universal ";
            story+="in their dismissal of the criticism which often follows the industry.  ";
            story+="One in particular said, \"";
            story+="Look, these products are safe.  That thing about the ";
            switch(LCSrandom(4))
            {
               case 0:story+="guy going on a killing spree";break;
               case 1:story+="gal turning blue and exploding";break;
               case 2:story+="guy speaking in tongues and worshipping Satan";break;
               case 3:story+="gal having a ruptured intestine";break;
            }
            story+=" is just a load of ";
            switch(LCSrandom(4))
            {
               case 0:story+="hooey";break;
               case 1:story+="poppycock";break;
               case 2:story+="horse radish";break;
               case 3:story+="skunk weed";break;
            }
            story+=".  Would we stake the reputation of our company on unsafe products?  ";
            story+="No.  That's just ridiculous.  I mean, sure companies have put unsafe products out, ";
            story+="but the GM industry operates at a higher ethical standard.  That goes without saying.";
            story+="\"";
            story+="&r";
            break;
         }
         case VIEW_JUSTICES:
         {
            cityname(story);
            story+=" - The conviction of confessed serial killer ";
            string dstr,dstr2,dstr3;
            firstname(dstr);firstname(dstr2);lastname(dstr3);
            story+=dstr;
            story+=" ";
            story+=dstr2;
            story+=" ";
            story+=dstr3;
            story+=" was overturned by a federal judge yesterday.  ";
            story+="Justice ";
            string jstr,jstr2;
            firstname(jstr);lastname(jstr2);
            story+=jstr;
            story+=" ";
            story+=jstr2;
            story+=" of the notoriously liberal circuit of appeals here ";
            story+="made the decision based on ";
            string gen;
            switch(LCSrandom(2))
            {
               case 0:gen="his";break;
               case 1:gen="her";break;
            }
            switch(LCSrandom(5))
            {
               case 0:story+="ten-year-old eye witness testimony";break;
               case 1:story+=gen;story+=" general feeling about police corruption";break;
               case 2:story+=gen;story+=" belief that the crimes were a right-wing conspiracy";break;
               case 3:
                  story+=gen;
                  story+=" belief that ";
                  story+=dstr3;
                  story+=" deserved another chance";
                  break;
               case 4:
                  story+=gen;
                  story+=" personal philosophy of liberty";break;
            }
            story+=", despite the confession of ";
            story+=dstr3;
            story+=", which even Justice ";
            story+=jstr2;
            story+=" grants was not coerced in any way.&r";

            story+="   Ten years ago, ";
            story+=dstr3;
            story+=" was convicted of the now-infamous ";
            string sstr;
            lastname(sstr);
            story+=sstr;
            story+=" slayings.  ";
            story+="After an intensive manhunt, ";
            story+=dstr3;
            story+=" was found with the murder weapon, ";
            story+="covered in the victims' blood.  ";
            story+=dstr3;
            story+=" confessed and was sentenced to life, saying \"";
            story+="Thank you for saving me from myself.  ";
            story+="If I were to be released, I would surely kill again.\"&r";
            story+="   A spokesperson for the district attorney ";
            story+="has stated that the case will not be retried, due ";
            story+="to the current economic doldrums that have left the state ";
            story+="completely strapped for cash.&r";
            break;
         }
         case VIEW_POLLUTION:
            cityname(story);
            story+=" - Pollution might not be so bad after all.  The ";
            switch(LCSrandom(5))
            {
               case 0:story+="American";break;
               case 1:story+="United";break;
               case 2:story+="Patriot";break;
               case 3:story+="Family";break;
               case 4:story+="Children's";break;
            }
            story+=" ";
            switch(LCSrandom(5))
            {
               case 0:story+="Heritage";break;
               case 1:story+="Enterprise";break;
               case 2:story+="Freedom";break;
               case 3:story+="Liberty";break;
               case 4:story+="Charity";break;
            }
            story+=" ";
            switch(LCSrandom(5))
            {
               case 0:story+="Partnership";break;
               case 1:story+="Institute";break;
               case 2:story+="Consortium";break;
               case 3:story+="Forum";break;
               case 4:story+="Center";break;
            }
            story+=" recently released a wide-ranging report detailing recent trends ";
            story+="and the latest science on the issue.  ";
            story+="Among the most startling of the think tank's findings is that ";
            switch(LCSrandom(5))
            {
               case 0:story+="a modest intake of radiactive waste";break;
               case 1:story+="a healthy dose of radiation";break;
               case 2:story+="a bath in raw sewage";break;
               case 3:story+="watching animals die in oil slicks";break;
               case 4:story+="inhaling carbon monoxide";break;
            }
            story+=" might actually ";
            switch(LCSrandom(5))
            {
               case 0:story+="purify the soul";break;
               case 1:story+="increase test scores";break;
               case 2:story+="increase a child's attention span";break;
               case 3:story+="make children behave better";break;
               case 4:story+="make shy children fit in";break;
            }
            story+=".";
            story+="&r";
            story+="   When questioned about the science behind these results, ";
            story+="a spokesperson stated that, \"";
            switch(LCSrandom(3))
            {
               case 0:story+="Research is complicated, and there are always two ways to think about things";break;
               case 1:story+="The jury is still out on pollution.  You really have to keep an open mind";break;
               case 2:story+="They've got their scientists, and we have ours.  The issue of pollution is wide open as it stands today";break;
            }
            story+=".  You have to realize that ";
            switch(LCSrandom(3))
            {
               case 0:story+="the elitist liberal media often distorts";break;
               case 1:story+="the vast left-wing education machine often distorts";break;
               case 2:story+="the fruits, nuts, and flakes of the environmentalist left often distort";break;
            }
            story+=" these issues to their own advantage.  ";
            story+="All we've done is introduced a little clarity into the ongoing debate.  ";
            story+="Why is there contention on the pollution question?  It's because ";
            story+="there's work left to be done.  We should study much more ";
            story+="before we urge any action.  Society really just ";
            story+="needs to take a breather on this one.  We don't see why there's such a rush to judgement here.  ";
            story+="&r";
            break;
         case VIEW_CORPORATECULTURE:
            cityname(story);
            story+=" - Several major companies have announced ";
            story+="at a joint news conference here that they ";
            story+="will be expanding their work forces considerably ";
            story+="during the next quarter.  Over thirty thousand jobs ";
            story+="are expected in the first month, with ";
            story+="tech giant ";
            switch(LCSrandom(10))
            {
               case 0:story+="Ameri";break;
               case 1:story+="Gen";break;
               case 2:story+="Oro";break;
               case 3:story+="Amelia";break;
               case 4:story+="Vivo";break;
               case 5:story+="Benji";break;
               case 6:story+="Amal";break;
               case 7:story+="Ply";break;
               case 8:story+="Seli";break;
               case 9:story+="Rio";break;
            }
            switch(LCSrandom(10))
            {
               case 0:story+="tech";break;
               case 1:story+="com";break;
               case 2:story+="zap";break;
               case 3:story+="cor";break;
               case 4:story+="dyne";break;
               case 5:story+="bless";break;
               case 6:story+="chip";break;
               case 7:story+="co";break;
               case 8:story+="wire";break;
               case 9:story+="rex";break;
            }
            story+=" increasing its payrolls by over ten thousand workers alone.  ";
            story+="Given the state of the economy recently and in ";
            story+="light of the tendency ";
            story+="of large corporations to export jobs overseas these days, ";
            story+="this welcome news is bound to be a pleasant surprise to those in the unemployment lines.  ";
            story+="The markets reportedly responded to the announcement with mild interest, ";
            story+="although the dampened movement might be expected due to the uncertain ";
            story+="futures of some of the companies in the tech sector.  On the whole, however, ";
            story+="analysts suggest that not only does the expansion speak to the health ";
            story+="of the tech industry but is also indicative of a full economic recover.&r";
            break;
         case VIEW_AMRADIO:
         {
            //THIS ONE IS SHORTER BECAUSE OF DOUBLE HEADLINE
            cityname(story);
            story+=" - Infamous FM radio shock jock ";
            string dstr,dstr2;
            firstname(dstr);lastname(dstr2);
            story+=dstr;
            story+=" ";
            story+=dstr2;
            story+=" has brought radio entertainment to a new low.  During yesterday's ";
            story+="broadcast of the program \"";
            story+=dstr;
            story+="'s ";
            switch(LCSrandom(5))
            {
               case 0:story+="Morning";break;
               case 1:story+="Commuter";break;
               case 2:story+="Jam";break;
               case 3:story+="Talk";break;
               case 4:story+="Radio";break;
            }
            story+=" ";
            switch(LCSrandom(5))
            {
               case 0:story+="Swamp";break;
               case 1:story+="Jolt";break;
               case 2:story+="Club";break;
               case 3:story+="Show";break;
               case 4:story+="Fandango";break;
            }
            story+="\", ";
            story+=dstr2;
            story+=" reportedly ";
            switch(LCSrandom(5))
            {
               case 0:story+="had intercourse";break;
               case 1:story+="encouraged listeners to call in and relieve themselves";break;
               case 2:story+="screamed \"f*ck the police those g*odd*mn m*th*f*ck*rs.  I got a f*cking ticket this morning and I'm f*cking p*ss*d as sh*t.\"";break;
               case 3:story+="breastfed from a lactating woman";break;
               case 4:story+="masturbated";break;
            }
            story+=" on the air.  Although ";
            story+=dstr2;
            story+=" later apologized, ";
            story+="the FCC received thousands of complaints ";
            story+="from irate listeners.  A spokesperson for the FCC ";
            story+="stated that the incident is under investigation.";
            story+="&r";
         }
            break;
      }
   }
}



/* news - draws the specified block of text to the screen */
void displaynewsstory(string& story,short *storyx_s,short *storyx_e,int y)
{
   vector<string> text;
   vector<char> centered;

   int totalwidth;
   int curpos=0;

   int addstrcur;
   string addstring;
   addstring.resize(1000);

   char content;
   int cury=y;
   int length;
   char endparagraph=0;
   char iscentered=0;
   unsigned int i=0;

   while(curpos<story.length()&&cury<25)
   {
      content=0;
      totalwidth=0;
      addstrcur=0;
      length=storyx_e[cury]-storyx_s[cury]+1;
      if(length==0){cury++;if(endparagraph>0)endparagraph--;continue;}

      for(i=curpos;i<story.length();i++)
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
               //addstrcur+=1;
               //addstring[addstrcur-1]=' ';
               //addstring[addstrcur]='\x0';
               addstring+=' ';
               addstring+='\x0';
               endparagraph=1;
               break;
            }
         }
         else
         {
            content=1;

            if(story[i]=='&')i++;
            //addstring[addstrcur]=story[i];
            //addstring[addstrcur+1]='\x0';
            addstring+=story[i];
            addstring+='\x0';
            totalwidth++;
            if(totalwidth>length)
            {
               while(story[i]!=' '){i--;addstrcur--;}
               while(story[i]==' ')i++;
               //addstring[addstrcur]='\x0';
               addstring+='\x0';
               break;
            }
            addstrcur++;
         }
      }

      if(i==story.size())//addstring[addstrcur]='\x0';
         addstring+='\x0';

      if(addstring.length()>0&&content)
      {
         int words=0;
         char silent=1;
         vector<int> spacex;
         for(int s2=0;s2<addstring.length();s2++)
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

         while(!endparagraph&&words>1&&addstring.length()<length&&!iscentered)
         {
            int csp=spacex[LCSrandom(spacex.size())];

            for(int x=0;x<spacex.size();x++)
            {
               if(spacex[x]>csp)spacex[x]++;
            }

            for(int s=addstring.length()+1;s>csp;s--)
            {
               addstring[s]=addstring[s-1];
            }
         }

         text.push_back(addstring);
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
         move(y+t,((storyx_s[y+t]+storyx_e[y+t])>>1)-((text[t].length()-1)>>1));
      }
      else move(y+t,storyx_s[y+t]);
      addstr(text[t]);
   }
   text.clear();
}



/* news - make some filler junk */
void generatefiller(string& story,int amount)
{
   string str;
   story+="&r";
   cityname(str);
   story+=str;
   story+=" - ";
   int par=0;
   while(amount>0)
   {
      par++;
      for(int i=0;i<LCSrandom(10)+3;i++)story+="~";
      if(amount>1)story+=" ";
      if(par>=50&&!LCSrandom(5)&&amount>20)
      {
         par=0;
         story+="&r";
         story+="  ";
      }
      amount--;
   }
   story+="&r";
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
                     string str;
                     str="Tonight on a Cable News channel: ";
                     switch(LCSrandom(5))
                     {
                        case 0:str+="Cross";break;
                        case 1:str+="Hard";break;
                        case 2:str+="Lightning";break;
                        case 3:str+="Washington";break;
                        case 4:str+="Capital";break;
                     }
                     switch(LCSrandom(5))
                     {
                        case 0:str+=" Fire";break;
                        case 1:str+=" Ball";break;
                        case 2:str+=" Talk";break;
                        case 3:str+=" Insider";break;
                        case 4:str+=" Gang";break;
                     }
                     str+=" with ";
                     string bname;
                     name(bname);
                     str+=bname;

                     erase();
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(0,39-((str.size()-1)>>1));
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
