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
#include <cstring>
#include "news/news.h"



/* news - determines the priority of a news story */
void setpriority(newsstoryst &ns)
{
   // Priority is set differently based on the type of the news story
   switch(ns.type)
   {
      // Major events always muscle to the front page by having a very high priority
      case NEWSSTORY_MAJOREVENT:
         ns.priority=30000;
         break;
      // LCS-related news stories are more important if they involve lots of headline-grabbing
      // crimes
      case NEWSSTORY_SQUAD_SITE:
      case NEWSSTORY_SQUAD_ESCAPED:
      case NEWSSTORY_SQUAD_FLEDATTACK:
      case NEWSSTORY_SQUAD_DEFENDED:
      case NEWSSTORY_SQUAD_BROKESIEGE:
      case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
      case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
      case NEWSSTORY_SQUAD_KILLED_SITE:
      case NEWSSTORY_CARTHEFT:
      case NEWSSTORY_NUDITYARREST:
      case NEWSSTORY_WANTEDARREST:
      case NEWSSTORY_DRUGARREST:
      case NEWSSTORY_GRAFFITIARREST:
      case NEWSSTORY_BURIALARREST:
      {
         ns.priority=0;

         int crime[CRIMENUM];
         memset(crime,0,CRIMENUM*sizeof(int));
         // Record all the crimes in this story
         for(int c=0;c<ns.crime.size();c++)
         {
            crime[ns.crime[c]]++;
         }
         // Cap publicity for more than ten repeats of an action of some type
         if(crime[CRIME_STOLEGROUND]>10)crime[CRIME_STOLEGROUND]=10;
         if(crime[CRIME_BROKEDOWNDOOR]>10)crime[CRIME_BROKEDOWNDOOR]=10;
         if(crime[CRIME_ATTACKED_MISTAKE]>10)crime[CRIME_ATTACKED_MISTAKE]=10;
         if(crime[CRIME_ATTACKED]>10)crime[CRIME_ATTACKED]=10;
         if(crime[CRIME_BREAK_SWEATSHOP]>10)crime[CRIME_BREAK_SWEATSHOP]=10;
         if(crime[CRIME_BREAK_FACTORY]>10)crime[CRIME_BREAK_FACTORY]=10;
         if(crime[CRIME_FREE_RABBITS]>10)crime[CRIME_FREE_RABBITS]=10;
         if(crime[CRIME_FREE_BEASTS]>10)crime[CRIME_FREE_BEASTS]=10;
         if(crime[CRIME_TAGGING]>10)crime[CRIME_TAGGING]=10;

         // Increase news story priority based on the number of instances of
         // various crimes, scaled by a factor dependant on the crime

         // Unique site crimes
         ns.priority+=crime[CRIME_SHUTDOWNREACTOR  ] * 100;
         ns.priority+=crime[CRIME_HACK_INTEL       ] * 100;
         ns.priority+=crime[CRIME_HOUSE_PHOTOS     ] * 100;
         ns.priority+=crime[CRIME_CORP_FILES       ] * 100;
         ns.priority+=crime[CRIME_PRISON_RELEASE   ] *  50;
         ns.priority+=crime[CRIME_JURYTAMPERING    ] *  30;
         ns.priority+=crime[CRIME_POLICE_LOCKUP    ] *  30;
         ns.priority+=crime[CRIME_COURTHOUSE_LOCKUP] *  30;

         // Common site crimes
         ns.priority+=crime[CRIME_KILLEDSOMEBODY   ] *  30;
         ns.priority+=crime[CRIME_FREE_BEASTS      ] *  12;
         ns.priority+=crime[CRIME_BREAK_SWEATSHOP  ] *   8;
         ns.priority+=crime[CRIME_BREAK_FACTORY    ] *   8;
         ns.priority+=crime[CRIME_FREE_RABBITS     ] *   8;
         ns.priority+=crime[CRIME_ATTACKED_MISTAKE ] *   7;
         ns.priority+=crime[CRIME_ATTACKED         ] *   4;
         ns.priority+=crime[CRIME_TAGGING          ] *   2;
         //ns.priority+=crime[CRIME_STOLEGROUND      ];
         //ns.priority+=crime[CRIME_BROKEDOWNDOOR    ];

         // Set story's political and violence levels for determining whether
         // a story becomes positive or negative
         if(ns.claimed)ns.politics_level=5;
         else ns.politics_level=0;
         
         ns.politics_level+=crime[CRIME_SHUTDOWNREACTOR  ] * 100;
         ns.politics_level+=crime[CRIME_HACK_INTEL       ] * 100;
         ns.politics_level+=crime[CRIME_HOUSE_PHOTOS     ] * 100;
         ns.politics_level+=crime[CRIME_CORP_FILES       ] * 100;
         ns.politics_level+=crime[CRIME_PRISON_RELEASE   ] *  50;
         ns.politics_level+=crime[CRIME_POLICE_LOCKUP    ] *  30;
         ns.politics_level+=crime[CRIME_COURTHOUSE_LOCKUP] *  30;
         ns.politics_level+=crime[CRIME_FREE_BEASTS      ] *  10;
         ns.politics_level+=crime[CRIME_BREAK_SWEATSHOP  ] *  10;
         ns.politics_level+=crime[CRIME_BREAK_FACTORY    ] *  10;
         ns.politics_level+=crime[CRIME_FREE_RABBITS     ] *  10;
         ns.politics_level+=crime[CRIME_TAGGING          ] *   3;

         ns.violence_level=0;

         ns.violence_level+=crime[CRIME_KILLEDSOMEBODY   ] *  20;
         ns.violence_level+=crime[CRIME_ATTACKED_MISTAKE ] *  12;
         ns.violence_level+=crime[CRIME_ATTACKED         ] *   4;

         short violence_threshhold;
         if(attitude[VIEW_POLITICALVIOLENCE]+attitude[VIEW_LIBERALCRIMESQUADPOS]<5)violence_threshhold=1;
         else if(attitude[VIEW_POLITICALVIOLENCE]+attitude[VIEW_LIBERALCRIMESQUADPOS]<25)violence_threshhold=2;
         else if(attitude[VIEW_POLITICALVIOLENCE]+attitude[VIEW_LIBERALCRIMESQUADPOS]<45)violence_threshhold=3;
         else if(attitude[VIEW_POLITICALVIOLENCE]+attitude[VIEW_LIBERALCRIMESQUADPOS]<65)violence_threshhold=4;
         else if(attitude[VIEW_POLITICALVIOLENCE]+attitude[VIEW_LIBERALCRIMESQUADPOS]<85)violence_threshhold=5;
         else if(attitude[VIEW_POLITICALVIOLENCE]+attitude[VIEW_LIBERALCRIMESQUADPOS]<105)violence_threshhold=6;
         else if(attitude[VIEW_POLITICALVIOLENCE]+attitude[VIEW_LIBERALCRIMESQUADPOS]<125)violence_threshhold=7;
         else if(attitude[VIEW_POLITICALVIOLENCE]+attitude[VIEW_LIBERALCRIMESQUADPOS]<145)violence_threshhold=8;
         else if(attitude[VIEW_POLITICALVIOLENCE]+attitude[VIEW_LIBERALCRIMESQUADPOS]<165)violence_threshhold=9;
         else if(attitude[VIEW_POLITICALVIOLENCE]+attitude[VIEW_LIBERALCRIMESQUADPOS]<185)violence_threshhold=10;
         else violence_threshhold=50;

         if(ns.violence_level / (ns.politics_level+1) > violence_threshhold)
            ns.positive = 0;
         else ns.positive = 1;

         // Add additional priority based on the type of news story
         // and how high profile the LCS is
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

         // Double profile if the squad moved out in full battle colors
         if(ns.claimed==2)ns.priority*=2;

         // Modify notability by location
         switch(location[ns.loc]->type)
         {
            // Nobody cares
         case SITE_RESIDENTIAL_TENEMENT:
         case SITE_BUSINESS_CRACKHOUSE:
            ns.priority/=8;
            break;

            // Normal priority
         case SITE_RESIDENTIAL_SHELTER:
         case SITE_INDUSTRY_WAREHOUSE:
         case SITE_RESIDENTIAL_BOMBSHELTER:
         case SITE_DOWNTOWN:
         case SITE_COMMERCIAL:
         case SITE_UDISTRICT:
         case SITE_OUTOFTOWN:
         case SITE_INDUSTRIAL:
         case SITE_RESIDENTIAL_APARTMENT:
         case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
         case SITE_LABORATORY_COSMETICS:
         case SITE_LABORATORY_GENETIC:
         case SITE_HOSPITAL_CLINIC:
         case SITE_HOSPITAL_UNIVERSITY:
         case SITE_INDUSTRY_SWEATSHOP:
         case SITE_INDUSTRY_POLLUTER:
         case SITE_INDUSTRY_NUCLEAR:
         case SITE_BUSINESS_PAWNSHOP:
         case SITE_BUSINESS_JUICEBAR:
         case SITE_BUSINESS_CIGARBAR:
         case SITE_BUSINESS_LATTESTAND:
         case SITE_BUSINESS_VEGANCOOP:
         case SITE_BUSINESS_INTERNETCAFE:
         case SITE_BUSINESS_DEPTSTORE:
         case SITE_BUSINESS_HALLOWEEN:
         case SITE_BUSINESS_BARANDGRILL:
         case SITE_BUSINESS_ARMSDEALER:
         case SITE_BUSINESS_CARDEALERSHIP:
         case SITE_OUTDOOR_PUBLICPARK:
         case SITE_OUTDOOR_BUNKER:
         default:
            break;

            // WOAH OMG
         case SITE_GOVERNMENT_POLICESTATION:
         case SITE_GOVERNMENT_COURTHOUSE:
         case SITE_GOVERNMENT_PRISON:
         case SITE_GOVERNMENT_INTELLIGENCEHQ:
         case SITE_GOVERNMENT_FIRESTATION:
         case SITE_CORPORATE_HEADQUARTERS:
         case SITE_CORPORATE_HOUSE:
         case SITE_MEDIA_AMRADIO:
         case SITE_MEDIA_CABLENEWS:
            ns.priority*=2;
            break;
         }

         // Cap news priority, in part so it can't displace major news stories
         if(ns.priority>20000)ns.priority=20000;
         break;
      }
      case NEWSSTORY_KIDNAPREPORT:
         // Kidnappings are higher priority if they're an archconservative
         ns.priority=20;
         if(ns.cr->type==CREATURE_CORPORATE_CEO||
            ns.cr->type==CREATURE_RADIOPERSONALITY||
            ns.cr->type==CREATURE_NEWSANCHOR||
            ns.cr->type==CREATURE_SCIENTIST_EMINENT||
            ns.cr->type==CREATURE_JUDGE_CONSERVATIVE)ns.priority=40;
         break;
      case NEWSSTORY_MASSACRE:
         // More people massacred, higher priority (I think; not verified ns.crime[1] is people present)
         ns.priority=10 + ns.crime[1]*5;
         break;
      case NEWSSTORY_CCS_SITE:
      case NEWSSTORY_CCS_KILLED_SITE:
         // CCS actions loosely simulate LCS actions; here it adds some
         // random site crimes to the story and increases the
         // priority accordingly
         ns.crime.push_back(CRIME_BROKEDOWNDOOR);
         ns.priority=1;
         ns.politics_level+=20;
         if(ns.positive==0)
         {
            ns.crime.push_back(CRIME_ATTACKED_MISTAKE);
            ns.priority+=7;
            ns.violence_level+=12;
         }
         ns.crime.push_back(CRIME_ATTACKED);
         ns.priority+=4*(LCSrandom(10)+1);
         ns.violence_level+=LCSrandom(10) *  4;
         if(LCSrandom(endgamestate+1))
         {
            ns.crime.push_back(CRIME_KILLEDSOMEBODY);
            ns.priority+=LCSrandom(10)*30;
            ns.violence_level+=LCSrandom(10) * 20;
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
            ns.politics_level+=LCSrandom(10) *  10;
         }
         if(LCSrandom(2))
         {
            ns.crime.push_back(CRIME_CARCHASE);
         }

         // Set story's political and violence levels for determining whether
         // a story becomes positive or negative

         short violence_threshhold;
         if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_CONSERVATIVECRIMESQUAD]<5)violence_threshhold=1;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_CONSERVATIVECRIMESQUAD]<25)violence_threshhold=2;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_CONSERVATIVECRIMESQUAD]<45)violence_threshhold=4;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_CONSERVATIVECRIMESQUAD]<65)violence_threshhold=6;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_CONSERVATIVECRIMESQUAD]<85)violence_threshhold=8;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_CONSERVATIVECRIMESQUAD]<105)violence_threshhold=10;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_CONSERVATIVECRIMESQUAD]<125)violence_threshhold=13;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_CONSERVATIVECRIMESQUAD]<145)violence_threshhold=17;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_CONSERVATIVECRIMESQUAD]<165)violence_threshhold=20;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_CONSERVATIVECRIMESQUAD]<185)violence_threshhold=30;
         else violence_threshhold=50;

         if(ns.violence_level / (ns.politics_level+1) > violence_threshhold)
            ns.positive = 1;
         else ns.positive = 0;

         break;
      case NEWSSTORY_CCS_DEFENDED:
      case NEWSSTORY_CCS_KILLED_SIEGEATTACK:
         ns.priority=40+attitude[VIEW_LIBERALCRIMESQUAD]/3;
         break;
		 
      case NEWSSTORY_STALIN_SITE:
      case NEWSSTORY_STALIN_KILLED_SITE:
         // Stalinists' actions loosely simulate LCS actions; here it adds some
         // random site crimes to the story and increases the
         // priority accordingly
         ns.crime.push_back(CRIME_BROKEDOWNDOOR);
         ns.priority=1;
         if(ns.positive==0)
         {
            ns.crime.push_back(CRIME_ATTACKED_MISTAKE);
            ns.priority+=7;
         }
         ns.crime.push_back(CRIME_ATTACKED);
         ns.priority+=4*(LCSrandom(10)+1);
         if(LCSrandom(5))
         {
            ns.crime.push_back(CRIME_KILLEDSOMEBODY);
            ns.priority+=LCSrandom(10)*30;
         }
         if(LCSrandom(6))
         {
            ns.crime.push_back(CRIME_STOLEGROUND);
            ns.priority+=LCSrandom(10);
         }
         if(!LCSrandom(7))
         {
            ns.crime.push_back(CRIME_BREAK_FACTORY);
            ns.priority+=LCSrandom(10)*2;
         }
         if(LCSrandom(2))
         {
            ns.crime.push_back(CRIME_CARCHASE);
         }
		          // Set story's political and violence levels for determining whether
         // a story becomes positive or negative
      case NEWSSTORY_STALIN_DEFENDED:
      case NEWSSTORY_STALIN_KILLED_SIEGEATTACK:
         ns.priority=40+attitude[VIEW_LIBERALCRIMESQUAD]/3;
         break;
      case NEWSSTORY_STALIN_CON_SITE:
      case NEWSSTORY_STALIN_CON_KILLED_SITE:
         // Stalinists' actions loosely simulate LCS actions; here it adds some
         // random site crimes to the story and increases the
         // priority accordingly
         ns.crime.push_back(CRIME_BROKEDOWNDOOR);
         ns.priority=1;
         if(ns.positive==0)
         {
            ns.crime.push_back(CRIME_ATTACKED_MISTAKE);
            ns.priority+=7;
         }
         ns.crime.push_back(CRIME_ATTACKED);
         ns.priority+=4*(LCSrandom(10)+1);
         if(LCSrandom(5))
         {
            ns.crime.push_back(CRIME_KILLEDSOMEBODY);
            ns.priority+=LCSrandom(10)*30;
         }
         if(LCSrandom(6))
         {
            ns.crime.push_back(CRIME_STOLEGROUND);
            ns.priority+=LCSrandom(10);
         }
         if(!LCSrandom(7))
         {
            ns.crime.push_back(CRIME_BREAK_FACTORY);
            ns.priority+=LCSrandom(10)*2;
         }
         if(LCSrandom(2))
         {
            ns.crime.push_back(CRIME_CARCHASE);
         }
         break;

         short stalinviolence_threshhold;
         if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_STALIN]<5)stalinviolence_threshhold=1;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_STALIN]<25)stalinviolence_threshhold=2;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_STALIN]<45)stalinviolence_threshhold=4;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_STALIN]<65)stalinviolence_threshhold=6;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_STALIN]<85)stalinviolence_threshhold=8;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_STALIN]<105)stalinviolence_threshhold=10;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_STALIN]<125)stalinviolence_threshhold=13;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_STALIN]<145)stalinviolence_threshhold=17;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_STALIN]<165)stalinviolence_threshhold=20;
         else if(attitude[VIEW_POLITICALVIOLENCE]+100-attitude[VIEW_STALIN]<185)stalinviolence_threshhold=30;
         else stalinviolence_threshhold=50;

         if(ns.violence_level / (ns.politics_level+1) > stalinviolence_threshhold)
            ns.positive = 1;
         else ns.positive = 0;

         break;
   }
}



/* news - show major news story */
void displaystory(newsstoryst &ns,bool liberalguardian,int header)
{
   int it2;
   preparepage(ns,liberalguardian);

   char story[5000];
   short storyx_s[25];
   short storyx_e[25];
   for(it2=0;it2<25;it2++)storyx_s[it2]=1;
   for(it2=0;it2<25;it2++)storyx_e[it2]=78;
   displayads(ns,liberalguardian,storyx_s,storyx_e,it2);

   switch(ns.type)
   {
      case NEWSSTORY_MAJOREVENT:
         displaymajoreventstory(ns,story,storyx_s,storyx_e);
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
      case NEWSSTORY_NUDITYARREST:
      case NEWSSTORY_WANTEDARREST:
      case NEWSSTORY_DRUGARREST:
      case NEWSSTORY_GRAFFITIARREST:
      case NEWSSTORY_BURIALARREST:
      {
         int y=2;
         if((!liberalguardian&&ns.page==1)||(liberalguardian&&ns.guardianpage==1))
         {
            y=21;

            displaystoryheader(ns,liberalguardian,y,header);
         }

         strcpy(story,lcityname);
         strcat(story," - ");

         switch(ns.type)
         {
            case NEWSSTORY_WANTEDARREST:
            case NEWSSTORY_GRAFFITIARREST:
            {
               int crime[CRIMENUM];
               std::memset(crime,0,sizeof(int)*CRIMENUM);
               for(int c=0;c<ns.crime.size();c++)
               {
                  crime[ns.crime[c]]++;
               }

               if(crime[CRIME_KILLEDSOMEBODY]>1)
               {
                  if(crime[CRIME_KILLEDSOMEBODY]==2)
                     strcat(story,"Two");
                  else
                     strcat(story,"Several");
                  strcat(story," police officers were");
               }
               else strcat(story,"A police officer was");
               strcat(story," killed in the line of duty yesterday, ");
               strcat(story,"according to a spokesperson from the police department.");
               strcat(story,"&r");
               strcat(story,"  A suspect, identified only as a member of the ");
               strcat(story,"radical political group Liberal Crime Squad, is believed to have killed ");
               if(crime[CRIME_KILLEDSOMEBODY]>1)
               {
                  char num[20];
                  itoa(crime[CRIME_KILLEDSOMEBODY],num,10);
                  strcat(story,num);
                  strcat(story," officers ");
               }
               else strcat(story,"the police officer ");
               strcat(story," while they were attempting to perform an arrest.  ");
               strcat(story,"The names of the officers have not been released pending notification of their families.");
               strcat(story,"&r");
               break;
            }
            case NEWSSTORY_NUDITYARREST:
            case NEWSSTORY_CARTHEFT:
            case NEWSSTORY_DRUGARREST:
            case NEWSSTORY_BURIALARREST:
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
                  strcat(story," police officers that were");
               }
               else strcat(story,"a police officer that was");
               strcat(story," attempting to perform an arrest.  ");
               if(ns.type==NEWSSTORY_NUDITYARREST)
                  strcat(story,"The incident apparently occurred as a response to a public nudity complaint.  ");
               else if(ns.type==NEWSSTORY_DRUGARREST)
                  strcat(story,"The suspect was alledgedly selling brownies.  ");
               else if(ns.type==NEWSSTORY_BURIALARREST)
               {
                  strcat(story,"A passerby alledgedly called the police after seeing the suspect dragging what appeared ");
                  strcat(story,"to be a corpse through an empty lot.  ");
               }
               else
                  strcat(story,"A passerby had allegedly spotted the suspect committing a car theft.  ");

               if(crime[CRIME_KILLEDSOMEBODY]>1)
               {
                  strcat(story,"The names of the officers have not been released pending notification of their families.");
               }
               else strcat(story,"The name of the officer has not been released pending notification of the officer's family."); 
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
               bool ccs=0;
               if(ns.type==NEWSSTORY_CCS_KILLED_SITE||ns.type==NEWSSTORY_CCS_SITE)ccs=1;

               squadstory_text_opening(ns,liberalguardian,ccs,story);

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
                  //if(ns.crime[c]==CRIME_TAGGING)continue;
                  if(crime[ns.crime[c]]==1)typesum++;
               }

               if(crime[CRIME_SHUTDOWNREACTOR])
               {
                  if(!liberalguardian)
                  {
                     strcat(story,"  According to sources that were at the scene, ");
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
                     strcat(story,"  According to sources that were at the scene, ");
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
                     strcat(story,"  According to sources that were at the scene, ");
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
                     strcat(story,"  According to sources that were at the scene, ");
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
                     strcat(story,"  According to police sources that were at the scene, ");
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
                     strcat(story,"  According to police sources that were at the scene, ");
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
                     strcat(story,"  According to police sources that were at the scene, ");
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
                     strcat(story,"  According to police sources that were at the scene, ");
                     strcat(story,"executives on the scene seemed very nervous about something.");
                     strcat(story,"&r");
                  }
                  else
                  {
                     strcat(story,"  The Liberal Crime Squad was attempting to uncover the company's Conservative corruption.");
                     strcat(story,"&r");
                  }
               }

               if(liberalguardian&&!ccs)
               {
                  if(crime[CRIME_ATTACKED_MISTAKE])typesum--;
                  if(crime[CRIME_KILLEDSOMEBODY])typesum--;
               }

               if(typesum>0)
               {
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
                     if(crime[CRIME_ARSON])
                     {
                        if(!liberalguardian||ccs)
                        {
                           strcat(story,"arson");
                        }
                        else
                        {
                           strcat(story,"set fire to Conservative property");
                        }

                        if(typesum>=3)strcat(story,", ");
                        else if(typesum==2)strcat(story," and ");
                        typesum--;
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
                     if(crime[CRIME_TAGGING])
                     {
                        if(!liberalguardian||ccs)
                        {
                           strcat(story,"vandalism");
                        }
                        else
                        {
                           strcat(story,"marked the site for Liberation");
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
                     strcat(story,".");
                  }

                  strcat(story,"&r");
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
            if(ns.crime[0]==SIEGE_CCS)
            {
               displaycenterednewsfont("CCS MASSACRE",5);
            }
            else if(!liberalguardian)
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
            case SIEGE_CCS:
               if(!liberalguardian)
               {
                  strcat(story,"Look, it was a Conservative Crime Squad hit, that's all we know, ");
                  strcat(story,"no names, no faces, not even where it happened really");
               }
               else
               {
                  strcat(story,"This is the doing of the Conservative Crime Squad butchers.  ");
                  strcat(story,"They have to be stopped before they kill again");
               }
               break;
			   case SIEGE_STALIN:
               if(!liberalguardian)
               {
                  strcat(story,"Listen, all we know is that they are working for the ");
                  strcat(story,"Stalinist Comrade Squad. No names, no faces, ");
                  strcat(story,"not even when it happened really.");
               }
               else
               {
                  strcat(story,"This was a crime committed by the Stalinist Comrade Squad ");
                  strcat(story,"who desire to establish a Conservative Police State. ");
                  strcat(story,"They must be stopped.");
               }
			   break;
            case SIEGE_FIREMEN:
               if(!liberalguardian)
               {
                  if(ns.crime[1]>1)strcat(story,"The recovered bodies were ");
                  else strcat(story,"The recovered body was ");
                  strcat(story,"burned unrecognizable.  ");
                  strcat(story,"Scorch marks throughout the site indicate that this was no accident; ");
                  strcat(story,"we are working closely with the Fire Department to track down the arsonist.  ");
                  strcat(story,"Fortunately, firemen were able to respond before the fire could spread to other buildings");
               }
               else
               {
                  if(ns.crime[1]>1)strcat(story,"The murdered were reporters ");
                  else strcat(story,"The murdered was a reporter ");
                  strcat(story,"working for this very paper. ");
                  strcat(story,"This is clearly the work of conservative butchers enforcing the prohibition on a free press");
               }
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
         strcat(story,"according to a police spokesperson.");
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
   int picnum,dimx,dimy;

   int numbytes;
   FILE *h;

   h=LCSOpenFile("largecap.cpc", "rb", LCSIO_PRE_ART);
   if(h!=NULL)
   {

      numbytes=fread(&picnum,sizeof(int),1,h);
      numbytes=fread(&dimx,sizeof(int),1,h);
      numbytes=fread(&dimy,sizeof(int),1,h);
      for(int p=0;p<picnum;p++)
      {
         for(int x=0;x<dimx;x++)
         {
            for(int y=0;y<dimy;y++)
            {
               numbytes=fread(&bigletters[p][x][y][0],sizeof(char),4,h);
            }
         }
      }
      LCSCloseFile(h);
   }

   h=LCSOpenFile("newstops.cpc", "rb", LCSIO_PRE_ART);
   if(h!=NULL)
   {

      numbytes=fread(&picnum,sizeof(int),1,h);
      numbytes=fread(&dimx,sizeof(int),1,h);
      numbytes=fread(&dimy,sizeof(int),1,h);
      for(int p=0;p<picnum;p++)
      {
         for(int x=0;x<dimx;x++)
         {
            for(int y=0;y<dimy;y++)
            {
               numbytes=fread(&newstops[p][x][y][0],sizeof(char),4,h);
            }
         }
      }
      LCSCloseFile(h);
   }


  h=LCSOpenFile("newspic.cpc", "rb", LCSIO_PRE_ART);
   if(h!=NULL)
   {

      numbytes=fread(&picnum,sizeof(int),1,h);
      numbytes=fread(&dimx,sizeof(int),1,h);
      numbytes=fread(&dimy,sizeof(int),1,h);
      for(int p=0;p<picnum;p++)
      {
         for(int x=0;x<dimx;x++)
         {
            for(int y=0;y<dimy;y++)
            {
               numbytes=fread(&newspic[p][x][y][0],sizeof(char),4,h);
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
   int i=0;

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
   int i;
   int n=0;

   int writers=0;

   for(i=0;i<pool.size();i++)
   {
      if(pool[i]->alive&&pool[i]->activity.type==ACTIVITY_WRITE_GUARDIAN)
      {
         if(pool[i]->location!=-1&&
            location[pool[i]->location]->compound_walls & COMPOUND_PRINTINGPRESS)
         {
            pool[i]->train(SKILL_WRITING,LCSrandom(3)); // Experience gain
            writers+=pool[i]->skillval(SKILL_WRITING); // Record the writer on this topic
            criminalize(*pool[i],LAWFLAG_SPEECH); // Record possibly illegal speech activity
         }
         else pool[i]->activity.type=ACTIVITY_NONE;
      }
   }

   //Conservative Crime Squad Strikes!
   if(endgamestate<ENDGAME_CCS_DEFEATED && LCSrandom(30)<endgamestate && canseethings)
   {
      newsstoryst *ns=new newsstoryst;

      // 10% chance of CCS squad wipe
      if(LCSrandom(10))ns->type=NEWSSTORY_CCS_SITE;
      else ns->type=NEWSSTORY_CCS_KILLED_SITE;

      // 20% chance of rampage
      ns->positive=LCSrandom(5);
      if(ns->positive)ns->positive=1;

      do
      {
         ns->loc=LCSrandom(location.size());
      } while(location[ns->loc]->renting!=-1);

      newsstory.push_back(ns);
   }

   //Stalinist Comrade Squad Strikes!
   if(stalinendgamestate<ENDGAME_STALIN_DEFEATED && LCSrandom(30) && canseethings)
   {
      newsstoryst *ns=new newsstoryst;

      // 10% chance of Stalin squad wipe
      if(LCSrandom(10))ns->type=NEWSSTORY_STALIN_SITE;
      else ns->type=NEWSSTORY_STALIN_KILLED_SITE;

      // 20% chance of Stalin rampage
      ns->positive=LCSrandom(5);
      if(ns->positive)ns->positive=1;

switch(LCSrandom(5))
{
	case 0:ns->loc=SITE_INDUSTRY_SWEATSHOP;
	case 1:ns->loc=SITE_CORPORATE_HOUSE;
	case 2:ns->loc=SITE_INDUSTRY_POLLUTER;
	case 3:ns->loc=SITE_RESIDENTIAL_APARTMENT_UPSCALE;
	case 4:ns->loc=SITE_CORPORATE_HEADQUARTERS;
}
		newsstory.push_back(ns);
      }
//The "Social Revolutionary Wing" of the Stalinist Comrade Squad Strikes Against Reactionary Liberalism!
   if(endgamestate<ENDGAME_STALIN_DEFEATED && LCSrandom(30) && canseethings)
   {
      newsstoryst *ns=new newsstoryst;

      // 10% chance of CCS squad wipe
      if(LCSrandom(10))ns->type=NEWSSTORY_STALIN_CON_SITE;
      else ns->type=NEWSSTORY_STALIN_CON_KILLED_SITE;

      // 20% chance of rampage
      ns->positive=LCSrandom(5);
      if(ns->positive)ns->positive=1;

switch(LCSrandom(5))
{
            case 0:ns->loc=SITE_INDUSTRY_NUCLEAR;
            case 1:ns->loc=SITE_GOVERNMENT_COURTHOUSE;
            case 2:ns->loc=SITE_GOVERNMENT_POLICESTATION;
            case 3:ns->loc=SITE_GOVERNMENT_PRISON;
            case 4:ns->loc=SITE_GOVERNMENT_INTELLIGENCEHQ;
         }
		newsstory.push_back(ns);
      }
   //SET UP MAJOR EVENTS
   if(!LCSrandom(60))
   {
      newsstoryst *ns=new newsstoryst;
         ns->type=NEWSSTORY_MAJOREVENT;
         do
         {
            ns->view=LCSrandom(VIEWNUM-3);
            ns->positive=LCSrandom(2);

            // Skip issues that we have no news stories for
            if(ns->view==VIEW_IMMIGRATION)continue;
            if(ns->view==VIEW_DRUGS)continue;
            //if(ns->view==VIEW_MILITARY)continue;
            if(ns->view==VIEW_CIVILRIGHTS)continue;
            //if(ns->view==VIEW_POLITICALVIOLENCE)continue;

            //NO ABORTION
            if(ns->view==VIEW_WOMEN&&ns->positive&&law[LAW_ABORTION]==-2)continue;
            //NO PARTIAL BIRTH ABORTION
            if(ns->view==VIEW_WOMEN&&!ns->positive&&law[LAW_ABORTION]<2)continue;
            //NO DEATH PENALTY
            if(ns->view==VIEW_DEATHPENALTY&&law[LAW_DEATHPENALTY]==2)continue;
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
         newsstory[n]->crime.size()==0) // Low content ignored
      {
         delete newsstory[n];
         newsstory.erase(newsstory.begin() + n);
         continue;
      }

      if(newsstory[n]->type==NEWSSTORY_CARTHEFT ||
         newsstory[n]->type==NEWSSTORY_NUDITYARREST||
         newsstory[n]->type==NEWSSTORY_WANTEDARREST||
         newsstory[n]->type==NEWSSTORY_DRUGARREST||
         newsstory[n]->type==NEWSSTORY_GRAFFITIARREST||
         newsstory[n]->type==NEWSSTORY_BURIALARREST)
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

      // Suppress news about sieges that aren't police actions
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
                     generate_name(bname);
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
                     generate_name(bname);
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
                  case VIEW_WOMEN:
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
      for(n=newsstory.size()-1;n>=0;n--)
      {
         setpriority(*newsstory[n]);
         // Suppress squad actions that aren't worth a story
         if(newsstory[n]->type==NEWSSTORY_SQUAD_SITE &&
            ((newsstory[n]->priority<50 &&
            newsstory[n]->claimed==0)||
            newsstory[n]->priority<4))
         {
            delete newsstory[n];
            newsstory.erase(newsstory.begin() + n);
            continue;
         }
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

   
   for(int p=0;p<pool.size();p++)
   {
      //Letters to the editor
      //Yes, crappy letters to the editor may backfire
      if(pool[p]->activity.type==ACTIVITY_WRITE_LETTERS)
      {
         background_liberal_influence[randomissue()]+=pool[p]->skillval(SKILL_WRITING)-LCSrandom(3);
         pool[p]->train(SKILL_WRITING,LCSrandom(5)+1);
      }

      //Guardian Essays
      //Basically letters to the editor, but thrice as potent
      // Should change this to have a further multiplier based on # of printing presses
      if(pool[p]->activity.type==ACTIVITY_WRITE_GUARDIAN)
      {
         background_liberal_influence[randomissue()]+=3*(pool[p]->skillval(SKILL_WRITING)-LCSrandom(3));
         pool[p]->train(SKILL_WRITING,LCSrandom(5)+1);
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
         newsstory[n]->type==NEWSSTORY_WANTEDARREST||
         newsstory[n]->type==NEWSSTORY_GRAFFITIARREST||
         newsstory[n]->type==NEWSSTORY_CCS_SITE||
         newsstory[n]->type==NEWSSTORY_CCS_KILLED_SITE||
         newsstory[n]->type==NEWSSTORY_STALIN_SITE||
         newsstory[n]->type==NEWSSTORY_STALIN_KILLED_SITE||
         newsstory[n]->type==NEWSSTORY_STALIN_CON_SITE||
         newsstory[n]->type==NEWSSTORY_STALIN_CON_KILLED_SITE)
	  {
         power=newsstory[n]->priority;
	  }
         if(newsstory[n]->type==NEWSSTORY_CCS_SITE||
            newsstory[n]->type==NEWSSTORY_CCS_KILLED_SITE||
            newsstory[n]->type==NEWSSTORY_STALIN_CON_SITE||
            newsstory[n]->type==NEWSSTORY_STALIN_CON_KILLED_SITE)
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

         short violence_threshhold;

         if(attitude[VIEW_POLITICALVIOLENCE]<15)violence_threshhold=1;
         else if(attitude[VIEW_POLITICALVIOLENCE]<25)violence_threshhold=2;
         else if(attitude[VIEW_POLITICALVIOLENCE]<35)violence_threshhold=3;
         else if(attitude[VIEW_POLITICALVIOLENCE]<45)violence_threshhold=4;
         else if(attitude[VIEW_POLITICALVIOLENCE]<55)violence_threshhold=5;
         else if(attitude[VIEW_POLITICALVIOLENCE]<65)violence_threshhold=6;
         else if(attitude[VIEW_POLITICALVIOLENCE]<75)violence_threshhold=7;
         else if(attitude[VIEW_POLITICALVIOLENCE]<85)violence_threshhold=8;
         else if(attitude[VIEW_POLITICALVIOLENCE]<95)violence_threshhold=9;
         else violence_threshhold=10;

         if(newsstory[n]->violence_level / (newsstory[n]->politics_level+1) > violence_threshhold)
         {
            change_public_opinion(VIEW_POLITICALVIOLENCE,power);
         }
         else
         {
            change_public_opinion(VIEW_POLITICALVIOLENCE,-power);
         }

         char colored=0;
         if(!(newsstory[n]->type==NEWSSTORY_CCS_SITE)&&
            !(newsstory[n]->type==NEWSSTORY_CCS_KILLED_SITE)&&
            !(newsstory[n]->type==NEWSSTORY_STALIN_SITE)&&
            !(newsstory[n]->type==NEWSSTORY_STALIN_KILLED_SITE)&&
            !(newsstory[n]->type==NEWSSTORY_STALIN_CON_SITE)&&
            !(newsstory[n]->type==NEWSSTORY_STALIN_CON_KILLED_SITE))
         {
            change_public_opinion(VIEW_LIBERALCRIMESQUAD,2+power);
            if(newsstory[n]->positive)
            {
               colored=1;
            }
            else power=-power;

            change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,power);
         }
         if(newsstory[n]->type==NEWSSTORY_CCS_SITE||
            newsstory[n]->type==NEWSSTORY_CCS_KILLED_SITE)
         {
            if(newsstory[n]->positive)
            {
               colored=-1;
            }
            else power=-power;
            
            change_public_opinion(VIEW_CONSERVATIVECRIMESQUAD,power);
         }
         if(newsstory[n]->type==NEWSSTORY_STALIN_SITE||
            newsstory[n]->type==NEWSSTORY_STALIN_KILLED_SITE)
         {
            if(newsstory[n]->positive)
            {
               colored=1;
            }
            else power=-power;
            change_public_opinion(VIEW_STALIN,-power);
         if(newsstory[n]->type==NEWSSTORY_STALIN_CON_SITE||
            newsstory[n]->type==NEWSSTORY_STALIN_CON_KILLED_SITE)
         {
            if(newsstory[n]->positive)
            {
               colored=-1;
            }
            else power=-power;
            change_public_opinion(VIEW_STALIN,power);
		switch(location[newsstory[n]->loc]->type)
         {
            case SITE_LABORATORY_COSMETICS:
               change_public_opinion(VIEW_ANIMALRESEARCH,power,colored,power*10);
               change_public_opinion(VIEW_WOMEN,power,colored,power*10);
               break;
            case SITE_LABORATORY_GENETIC:
               change_public_opinion(VIEW_ANIMALRESEARCH,power,colored,power*10);
               change_public_opinion(VIEW_GENETICS,power,colored,power*10);
               break;
            case SITE_GOVERNMENT_POLICESTATION:
               change_public_opinion(VIEW_POLICEBEHAVIOR,power,colored,power*10);
               change_public_opinion(VIEW_DRUGS,power,colored,power*10);
               break;
            case SITE_GOVERNMENT_COURTHOUSE:
               change_public_opinion(VIEW_DEATHPENALTY,power,colored,power*10);
               change_public_opinion(VIEW_JUSTICES,power,colored,power*10);
               change_public_opinion(VIEW_FREESPEECH,power,colored,power*10);
               change_public_opinion(VIEW_GAY,power,colored,power*10);
               change_public_opinion(VIEW_WOMEN,power,colored,power*10);
               change_public_opinion(VIEW_CIVILRIGHTS,power,colored,power*10);
               break;
            case SITE_GOVERNMENT_PRISON:
               change_public_opinion(VIEW_DEATHPENALTY,power,colored,power*10);
               change_public_opinion(VIEW_PRISONS,power,colored,power*10);
               change_public_opinion(VIEW_DRUGS,power,colored,power*10);
               break;
            case SITE_GOVERNMENT_INTELLIGENCEHQ:
               change_public_opinion(VIEW_INTELLIGENCE,power,colored,power*10);
               break;
            case SITE_INDUSTRY_SWEATSHOP:
               change_public_opinion(VIEW_SWEATSHOPS,power,colored,power*10);
               change_public_opinion(VIEW_IMMIGRATION,power,colored,power*10);
               break;
            case SITE_INDUSTRY_POLLUTER:
               change_public_opinion(VIEW_SWEATSHOPS,power,colored,power*10);
               change_public_opinion(VIEW_POLLUTION,power,colored,power*10);
               break;
            case SITE_INDUSTRY_NUCLEAR:
               change_public_opinion(VIEW_NUCLEARPOWER,power,colored,power*10);
               break;
            case SITE_CORPORATE_HEADQUARTERS:
					change_public_opinion(VIEW_TAXES,power,colored,power*10);
               change_public_opinion(VIEW_CORPORATECULTURE,power,colored,power*10);
               change_public_opinion(VIEW_WOMEN,power,colored,power*10);
               break;
            case SITE_CORPORATE_HOUSE:
					change_public_opinion(VIEW_TAXES,power,colored,power*10);
					change_public_opinion(VIEW_CEOSALARY,power,colored,power*10);
               break;
            case SITE_MEDIA_AMRADIO:
               change_public_opinion(VIEW_AMRADIO,power,colored,power*10);
               change_public_opinion(VIEW_FREESPEECH,power,colored,power*10);
               change_public_opinion(VIEW_GAY,power,colored,power*10);
               change_public_opinion(VIEW_WOMEN,power,colored,power*10);
               change_public_opinion(VIEW_CIVILRIGHTS,power,colored,power*10);
               break;
            case SITE_MEDIA_CABLENEWS:
               change_public_opinion(VIEW_CABLENEWS,power,colored,power*10);
               change_public_opinion(VIEW_FREESPEECH,power,colored,power*10);
               change_public_opinion(VIEW_GAY,power,colored,power*10);
               change_public_opinion(VIEW_WOMEN,power,colored,power*10);
               change_public_opinion(VIEW_CIVILRIGHTS,power,colored,power*10);
               break;
            case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
					change_public_opinion(VIEW_TAXES,power,colored,power*10);
					change_public_opinion(VIEW_CEOSALARY,power,colored,power*10);
					change_public_opinion(VIEW_GUNCONTROL,power,colored,power*10);
               break;
            case SITE_BUSINESS_CIGARBAR:
					change_public_opinion(VIEW_TAXES,power,colored,power*10);
					change_public_opinion(VIEW_CEOSALARY,power,colored,power*10);
               change_public_opinion(VIEW_WOMEN,power,colored,power*10);
               break;
         }
      }
   }
 }
   //DELETE STORIES
   for(n=0;n<newsstory.size();n++)delete newsstory[n];
   newsstory.clear();
}