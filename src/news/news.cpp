/**
 * The main news module.
 */
/*
 * Copyright (c) 2002,2003,2004 by Tarn Adams
 * Copyright 2017 Stephen M. Webb
 *
 * This file is part of Liberal Crime Squad.
 *
 * Liberal Crime Squad is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/*
 * This file was created by Chris Johnson (grundee@users.sourceforge.net)
 * by copying code from game.cpp.
 */

//TODO: Add logging for this file? --Addictgamer

#include <externs.h>
#include "news/news.h"
#pragma GCC diagnostic ignored "-Wunused-result"


/**
 * Determine the priority of a news story.
 */
static void
setpriority(newsstoryst &ns)
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

         int crime[CRIMENUM] = { 0 };
         // Record all the crimes in this story
         for(int c=0;c<len(ns.crime);c++)
            crime[ns.crime[c]]++;
         // Cap publicity for more than ten repeats of an action of some type
         if(crime[CRIME_STOLEGROUND]>10) crime[CRIME_STOLEGROUND]=10;
         if(crime[CRIME_BROKEDOWNDOOR]>10) crime[CRIME_BROKEDOWNDOOR]=10;
         if(crime[CRIME_ATTACKED_MISTAKE]>10) crime[CRIME_ATTACKED_MISTAKE]=10;
         if(crime[CRIME_ATTACKED]>10) crime[CRIME_ATTACKED]=10;
         if(crime[CRIME_BREAK_SWEATSHOP]>10) crime[CRIME_BREAK_SWEATSHOP]=10;
         if(crime[CRIME_BREAK_FACTORY]>10) crime[CRIME_BREAK_FACTORY]=10;
         if(crime[CRIME_FREE_RABBITS]>10) crime[CRIME_FREE_RABBITS]=10;
         if(crime[CRIME_FREE_BEASTS]>10) crime[CRIME_FREE_BEASTS]=10;
         if(crime[CRIME_TAGGING]>10) crime[CRIME_TAGGING]=10;

         // Increase news story priority based on the number of instances of
         // various crimes, scaled by a factor dependant on the crime

         // Unique site crimes
         ns.priority+=crime[CRIME_BANKVAULTROBBERY ] * 100;
         ns.priority+=crime[CRIME_BANKSTICKUP      ] * 100;
         ns.priority+=crime[CRIME_SHUTDOWNREACTOR  ] * 100;
         ns.priority+=crime[CRIME_HACK_INTEL       ] * 100;
         ns.priority+=crime[CRIME_ARMORY           ] * 100;
         ns.priority+=crime[CRIME_HOUSE_PHOTOS     ] * 100;
         ns.priority+=crime[CRIME_CORP_FILES       ] * 100;
         ns.priority+=crime[CRIME_PRISON_RELEASE   ] *  50;
         ns.priority+=crime[CRIME_JURYTAMPERING    ] *  30;
         ns.priority+=crime[CRIME_POLICE_LOCKUP    ] *  30;
         ns.priority+=crime[CRIME_COURTHOUSE_LOCKUP] *  30;
         ns.priority+=crime[CRIME_BANKTELLERROBBERY] *  30;

         // Common site crimes
         ns.priority+=crime[CRIME_KILLEDSOMEBODY   ] *  30;
         ns.priority+=crime[CRIME_FREE_BEASTS      ] *  12;
         ns.priority+=crime[CRIME_BREAK_SWEATSHOP  ] *   8;
         ns.priority+=crime[CRIME_BREAK_FACTORY    ] *   8;
         ns.priority+=crime[CRIME_FREE_RABBITS     ] *   8;
         ns.priority+=crime[CRIME_ATTACKED_MISTAKE ] *   7;
         ns.priority+=crime[CRIME_ATTACKED         ] *   4;
         ns.priority+=crime[CRIME_TAGGING          ] *   2;
         ns.priority+=crime[CRIME_VANDALISM        ] *   2;
         //ns.priority+=crime[CRIME_STOLEGROUND      ];
         //ns.priority+=crime[CRIME_BROKEDOWNDOOR    ];

         // Set story's political and violence levels for determining whether
         // a story becomes positive or negative
         if(ns.claimed) ns.politics_level=5;
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
         ns.politics_level+=crime[CRIME_VANDALISM        ] *   5;
         ns.politics_level+=crime[CRIME_TAGGING          ] *   3;

         ns.violence_level=0;

         ns.violence_level+=crime[CRIME_ARMORY           ] * 100;
         ns.violence_level+=crime[CRIME_KILLEDSOMEBODY   ] *  20;
         ns.violence_level+=crime[CRIME_ATTACKED_MISTAKE ] *  12;
         ns.violence_level+=crime[CRIME_ATTACKED         ] *   4;

         /*
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
         */

         //if(ns.violence_level / (ns.politics_level+1) > violence_threshhold)
         //   ns.positive = 0;
         //else ns.positive = 1;

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
            default:
               // Suppress actions at CCS safehouses
               if(ns.loc!=-1 &&
                  location[ns.loc]->renting==RENTING_CCS)
               {
                  ns.priority = 0;
               }
               break;
         }

         // Double profile if the squad moved out in full battle colors
         if(ns.claimed==2) ns.priority*=2;

         // Modify notability by location
         if(ns.loc!=-1)
         {
            switch(location[ns.loc]->type)
            {
               // Not even reported
            case SITE_BUSINESS_CRACKHOUSE:
               if(ns.type == NEWSSTORY_SQUAD_KILLED_SITE ||
                  ns.type == NEWSSTORY_SQUAD_SITE)
               {
                  ns.priority = 0;
                  break;
               }
               // Nobody cares
            case SITE_RESIDENTIAL_TENEMENT:
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
            case SITE_INDUSTRY_NUCLEAR:
            case SITE_GOVERNMENT_POLICESTATION:
            case SITE_GOVERNMENT_COURTHOUSE:
            case SITE_GOVERNMENT_PRISON:
            case SITE_GOVERNMENT_INTELLIGENCEHQ:
            case SITE_GOVERNMENT_ARMYBASE:
            case SITE_GOVERNMENT_FIRESTATION:
            case SITE_CORPORATE_HEADQUARTERS:
            case SITE_CORPORATE_HOUSE:
            case SITE_MEDIA_AMRADIO:
            case SITE_MEDIA_CABLENEWS:
            case SITE_BUSINESS_BANK:
            case SITE_GOVERNMENT_WHITE_HOUSE:
               ns.priority*=2;
               break;
            }
         }

         // Cap news priority, in part so it can't displace major news stories
         if(ns.priority>20000) ns.priority=20000;
         break;
      }
      case NEWSSTORY_KIDNAPREPORT:
         // Kidnappings are higher priority if they're an archconservative
         ns.priority=20;
         if(ns.cr->type==CREATURE_CORPORATE_CEO||
            ns.cr->type==CREATURE_RADIOPERSONALITY||
            ns.cr->type==CREATURE_NEWSANCHOR||
            ns.cr->type==CREATURE_SCIENTIST_EMINENT||
            ns.cr->type==CREATURE_JUDGE_CONSERVATIVE) ns.priority=40;
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
         break;
      case NEWSSTORY_CCS_DEFENDED:
      case NEWSSTORY_CCS_KILLED_SIEGEATTACK:
         ns.priority=40+attitude[VIEW_LIBERALCRIMESQUAD]/3;
         break;
   }
}


/**
 * Show major news story.
 */
static void
displaystory(newsstoryst &ns, bool liberalguardian, int header)
{
   music.play(MUSIC_NEWSPAPER);
   int it2;
   preparepage(ns,liberalguardian);

   char story[5000];
   short storyx_s[25];
   short storyx_e[25];
   for(it2=0;it2<25;it2++) storyx_s[it2]=1;
   for(it2=0;it2<25;it2++) storyx_e[it2]=78;
   displayads(ns,liberalguardian,storyx_s,storyx_e,it2);

   const char *city;
   if(multipleCityMode && ns.loc != -1)
   {
      Location * ns_site = find_site_in_city(location[ns.loc]->city, -1);
      switch(ns_site->type)
      {
      case SITE_CITY_SEATTLE: city = "Seattle, WA"; break;
      case SITE_CITY_NEW_YORK: city = "New York, NY"; break;
      case SITE_CITY_LOS_ANGELES: city = "Los Angeles, CA"; break;
      case SITE_CITY_CHICAGO: city = "Chicago, IL"; break;
      case SITE_CITY_DETROIT: city = "Detroit, MI"; break;
      case SITE_CITY_ATLANTA: city = "Atlanta, GA"; break;
      case SITE_CITY_MIAMI: city = "Miami, FL"; break;
      case SITE_CITY_WASHINGTON_DC: city = "Washington, DC"; break;
      default: city = lcityname;
      }
   }
   else city = lcityname;

   switch(ns.type)
   {
      case NEWSSTORY_MAJOREVENT:
         displaymajoreventstory(ns,story,storyx_s,storyx_e);
         break;
      case NEWSSTORY_CCS_NOBACKERS:
      case NEWSSTORY_CCS_DEFEATED:
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

         strcpy(story,city);
         strcat(story," - ");

         switch(ns.type)
         {
            case NEWSSTORY_CCS_NOBACKERS:
               strcat(story,"The FBI investigation into the Conservative Crime Squad's government connections has led to the arrest of more than ");
               strcat(story,"a dozen elected officials and revealed extensive corruption in law enforcement.");
               strcat(story,"&r");
               strcat(story,"  \"The uphevals in the police force, and arrest of corrupt officials, are only the beginning,\" FBI Chief ");
               strcat(story,"Roberta T. Malton said during a news conference.  \"A major focus ");
               strcat(story,"of our efforts will be on the complete destruction of the Conservative Crime Squad. Within six months, we'll have their ");
               strcat(story,"entire leadership, dead or alive. I personally guarantee it.\"");
               strcat(story,"&r");
               break;
            case NEWSSTORY_CCS_DEFEATED:
               strcat(story,"An elite FBI force conducted simultaneous ");
               strcat(story,"raids on several suspected Conservative Crime Squad safehouses in the early hours. Despite resistance from ");
               strcat(story,"CCS terrorists armed with automatic weapons and body armor, no FBI agents were killed in the raids, and all ");
               strcat(story,"three raids were successful. Seventeen suspects were killed in the fighting, and twenty-three are ");
               strcat(story,"now in custody.");
               strcat(story,"&r");
               strcat(story,"  The Conservative Crime Squad fell on hard times when the alternative newspaper Liberal Guardian published ");
               strcat(story,"1147 pages of documents showing extensive government support for the group. The ensuing scandal ");
               strcat(story,"led to the arrest of twenty-five members of Congress, as well as several leadership figures in the ");
               strcat(story,"Conservative Party's National Committee.");
               strcat(story,"&r");
               strcat(story,"  \"I want parents to rest easy tonight,\" FBI Chief ");
               strcat(story,"Roberta T. Malton said during a news conference to announce the raids.  \"You don't need the Liberal Crime Squad ");
               strcat(story,"to protect you. The Government can handle it.\"");
               strcat(story,"&r");
               break;
            case NEWSSTORY_WANTEDARREST:
            case NEWSSTORY_GRAFFITIARREST:
            {
               int crime[CRIMENUM] = { 0 };
               for(int c=0;c<len(ns.crime);c++)
                  crime[ns.crime[c]]++;
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
               strcat(story,"radical political group known as the Liberal Crime Squad, is believed to have killed ");
               if(crime[CRIME_KILLEDSOMEBODY]>1)
               {
                  strcat(story,crime[CRIME_KILLEDSOMEBODY]);
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
               int crime[CRIMENUM] = { 0 };
               for(int c=0;c<len(ns.crime);c++)
                  crime[ns.crime[c]]++;
               strcat(story,"A routine arrest went horribly wrong yesterday, ");
               strcat(story,"according to a spokesperson from the police department.");
               strcat(story,"&r");
               strcat(story,"  A suspect, whose identity is unclear, ");
               strcat(story,"killed ");
               if(crime[CRIME_KILLEDSOMEBODY]>1)
               {
                  strcat(story,crime[CRIME_KILLEDSOMEBODY]);
                  strcat(story," police officers that were");
               }
               else strcat(story,"a police officer that was");
               strcat(story," attempting to perform an arrest.  ");
               if(ns.type==NEWSSTORY_NUDITYARREST)
                  strcat(story,"The incident apparently occurred as a response to a public nudity complaint.  ");
               else if(ns.type==NEWSSTORY_DRUGARREST)
                  strcat(story,"The suspect was allegedly selling \"pot brownies\".  ");
               else if(ns.type==NEWSSTORY_BURIALARREST)
               {
                  strcat(story,"A passerby allegedly called the authorities after seeing the suspect dragging what ");
                  strcat(story,"appeared to be a corpse through an empty lot.  ");
               }
               else
                  strcat(story,"A passerby had allegedly spotted the suspect committing a car theft.  ");

               if(crime[CRIME_KILLEDSOMEBODY]>1)
                  strcat(story,"The names of the officers have not been released pending notification of their families.");
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

               int crime[CRIMENUM] = { 0 };
               int typesum=0;
               for(int c=0;c<len(ns.crime);c++)
               {
                  // Count crimes of each type
                  crime[ns.crime[c]]++;

                  // Special crimes are described at the start or end of the article;
                  // others should be recorded in the body
                  if(ns.crime[c]==CRIME_HOUSE_PHOTOS) continue;
                  if(ns.crime[c]==CRIME_CORP_FILES) continue;
                  if(ns.crime[c]==CRIME_SHUTDOWNREACTOR) continue;
                  if(ns.crime[c]==CRIME_BANKVAULTROBBERY) continue;
                  if(ns.crime[c]==CRIME_BANKSTICKUP) continue;
                  if(ns.crime[c]==CRIME_POLICE_LOCKUP) continue;
                  if(ns.crime[c]==CRIME_COURTHOUSE_LOCKUP) continue;
                  if(ns.crime[c]==CRIME_PRISON_RELEASE) continue;
                  if(ns.crime[c]==CRIME_JURYTAMPERING) continue;
                  if(ns.crime[c]==CRIME_HACK_INTEL) continue;
                  if(ns.crime[c]==CRIME_ARMORY) continue;
                  if(ns.crime[c]==CRIME_HOUSE_PHOTOS) continue;
                  if(ns.crime[c]==CRIME_CORP_FILES) continue;
                  if(ns.crime[c]==CRIME_CARCHASE) continue;
                  if(ns.crime[c]==CRIME_CARCRASH) continue;
                  if(ns.crime[c]==CRIME_FOOTCHASE) continue;
                  //if(ns.crime[c]==CRIME_TAGGING) continue;
                  if(crime[ns.crime[c]]==1) typesum++;
               }

               if(crime[CRIME_SHUTDOWNREACTOR])
               {
                  if(law[LAW_NUCLEARPOWER]==2)
                  {
                     if(!liberalguardian)
                     {
                        strcat(story,"  According to sources that were at the scene, ");
                        strcat(story,"the Liberal Crime Squad contaminated the state's water supply");
                        strcat(story,"yesterday by tampering with equipment on the site.");
                        strcat(story,"&r");
                     }
                     else
                     {
                        strcat(story,"  The Liberal Crime Squad tampered with the state's water supply yesterday, ");
                        strcat(story,"demonstrating the extreme dangers of Nuclear Waste. ");
                        strcat(story,"&r");
                     }
                  }
                  else
                  {
                     if(!liberalguardian)
                     {
                        strcat(story,"  According to sources that were at the scene, ");
                        strcat(story,"the Liberal Crime Squad nearly caused a catastrophic meltdown of the nuclear ");
                        strcat(story,"reactor.");
                        strcat(story,"&r");
                     }
                     else
                     {
                        strcat(story,"  The Liberal Crime Squad brought the reactor to the verge of a nuclear meltdown, ");
                        strcat(story,"demonstrating the extreme vulnerability and danger of Nuclear Power Plants. ");
                        strcat(story,"&r");
                     }
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
               if(crime[CRIME_BANKVAULTROBBERY])
               {
                  if(!liberalguardian)
                  {
                     strcat(story,"  According to sources that were at the scene, ");
                     strcat(story,"the Liberal Crime Squad opened the bank vault, which held more than $100,000 at the time.");
                     strcat(story,"&r");
                  }
                  else
                  {
                     strcat(story,"  The Liberal Crime Squad opened the bank vault, ");
                     strcat(story,"showing the triumph of Liberal ideals over Conservative economics.");
                     strcat(story,"&r");
                  }
               }
               else if(crime[CRIME_BANKSTICKUP])
               {
                  if(!liberalguardian)
                  {
                     strcat(story,"  According to sources that were at the scene, ");
                     strcat(story,"the Liberal Crime Squad threatened innocent bystanders in order to rob the bank vault.");
                     strcat(story,"&r");
                  }
                  else
                  {
                     strcat(story,"  The Liberal Crime Squad demanded access to the bank vault, ");
                     strcat(story,"hoping to acquire the resources to overcome evil.");
                     strcat(story,"&r");
                  }
               }
               if(crime[CRIME_COURTHOUSE_LOCKUP])
               {
                  if(!liberalguardian)
                  {
                     strcat(story,"  According to sources that were at the scene, ");
                     strcat(story,"the Liberal Crime Squad allegedly freed or attempted to free prisoners from the courthouse lockup.");
                     strcat(story,"&r");
                  }
                  else
                  {
                     strcat(story,"  The Liberal Crime Squad attempted to rescue innocent people from the courthouse lockup, ");
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
                     strcat(story,"  The Liberal Crime Squad attempted to rescue innocent people from the abusive Conservative conditions ");
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
                     strcat(story,"may have interfered with jury deliberations.");
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
                     strcat(story,"  Liberal Crime Squad computer specialists worked to liberate information from CIA computers.");
                     strcat(story,"&r");
                  }
               }
               if(crime[CRIME_ARMORY])
               {
                  if(!liberalguardian)
                  {
                     strcat(story,"  According to sources, ");
                     strcat(story,"the Liberal Crime Squad attempted to break into the armory.");
                     strcat(story,"&r");
                  }
                  else
                  {
                     strcat(story,"  Liberal Crime Squad infiltration specialists worked to liberate weapons from the oppressors.");
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
                  if(crime[CRIME_ATTACKED_MISTAKE]) typesum--;
                  if(crime[CRIME_KILLEDSOMEBODY]) typesum--;
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
                     if(crime[CRIME_STOLEGROUND]||crime[CRIME_BANKTELLERROBBERY])
                     {
                        if(!liberalguardian||ccs)
                        {
                           strcat(story,"theft");
                        }
                        else
                        {
                           strcat(story,"liberated enemy resources");
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
                           strcat(story,"liberated abused animals");
                        }

                        if(typesum>=3)strcat(story,", ");
                        else if(typesum==2)strcat(story," and ");
                        typesum--;
                     }
                     if(crime[CRIME_BREAK_SWEATSHOP]||crime[CRIME_BREAK_FACTORY]||crime[CRIME_VANDALISM])
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
                           strcat(story,"broke down doors");
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
                           strcat(story,"picked locks");
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
                     strcat(story,"  Conservative operatives engaged in a reckless ");
                     strcat(story,"pursuit of the LCS.  ");
                  }

                  if(crime[CRIME_CARCRASH])
                  {
                     if(crime[CRIME_CARCRASH]>1)
                     {
                        strcat(story,crime[CRIME_CARCRASH]);
                        strcat(story," vehicles crashed.  ");
                     }
                     else strcat(story,"One vehicle crashed.  ");
                     if(!liberalguardian||ccs)
                        strcat(story,"Details about injuries were not released.  ");//XXX: Why not turn them into martyrs?
                  }

                  if(crime[CRIME_FOOTCHASE])
                  {
                     if(!liberalguardian||ccs)
                        strcat(story,"There was also a foot chase when the suspect or suspects bailed out after the high-speed pursuit.  ");
                     else
                        strcat(story,"The Liberal Crime Squad ended the dangerous high-speed chase in order to protect the public, and attempted to escape on foot.  ");
                  }
                  strcat(story,"&r");
               }
               if(!ccs)
               {
                  if(!LCSrandom(8))
                  {
                     if(crime[CRIME_TAGGING])
                     {
                        strcat(story,"  The slogan, \"");
                        strcat(story,slogan);
                        strcat(story,"\" was found painted on the walls.");
                     }
                     else
                     {
                        switch(LCSrandom(3))
                        {
                        case 0:
                           if (ns.type==NEWSSTORY_SQUAD_KILLED_SITE)
                           {
                              strcat(story,"  One uttered the words, \"");
                              strcat(story,slogan);
                              strcat(story,"\" before passing out.");
                           }
                           else
                           {
                              strcat(story,"  As they left, they shouted, \"");
                              strcat(story,slogan);
                              strcat(story,"\"");
                           }
                           break;
                        case 1:
                           strcat(story,"  One of them was rumored to have cried out, \"");
                           strcat(story,slogan);
                           strcat(story,"\"");
                           break;
                        case 2:
                           strcat(story,"  Witnesses reported hearing the phrase, \"");
                           strcat(story,slogan);
                           strcat(story,"\"");
                           break;
                        }
                     }
                     strcat(story,"&r");
                  }
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

         strcpy(story,city);
         strcat(story," - ");
         if(ns.crime[1]>2)
         {
            strcat(story,ns.crime[1]);
            strcat(story," bodies were "); //Gruesome pile, large pile.
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
            strcat(story,"unwilling to pursue the case with any serious effort.");
            strcat(story,"&r");
            strcat(story,"  The Liberal Crime Squad has claimed that the ");
            if(ns.crime[1]>1)strcat(story,"victims were members ");
            else strcat(story,"victim was a member ");
            strcat(story,"of the LCS targeted simply due to their political beliefs.  ");
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
                  if(law[LAW_FREESPEECH]==-2)strcat(story,"[Craziest] thing I've ever seen");
                  else if(law[LAW_FREESPEECH]==2)strcat(story,"Damnedest thing I've ever seen");
                  else strcat(story,"D*mnd*st thing I've ever seen");
               }
               else
               {
                  strcat(story,"We have strong evidence that this was an extra-judicial slaughter ");
                  strcat(story,"carried out by the Central Intelligence Agency in retaliation for our ");
                  strcat(story,"previous actions to uncover human rights abuses and corruption in the ");
                  strcat(story,"intelligence community");
               }
               break;
            case SIEGE_POLICE:
            case SIEGE_HICKS:
               if(!liberalguardian)
               {
                  strcat(story,"Burned...  stabbed with, maybe, pitchforks.  There may have ");
                  strcat(story,"been bite marks.  Nothing recognizable left.  Complete carnage.");
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
                  strcat(story,"known to work with several corporations we've had confrontations ");
                  strcat(story,"with in the past.  *When* the police can't figure this one out, they're ");
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
                     displaycenterednewsfont("SOMEONE",5);
                     displaycenterednewsfont("KIDNAPPED",13);break;
               }
            }
         }

         strcpy(story,city);
         strcat(story," - The disappearance of ");
         strcat(story,ns.cr->propername);
         strcat(story," is now considered a kidnapping, ");
         strcat(story,"according to a police spokesperson.");
         strcat(story,"&r");

         char dstr[200],dstr2[200];
         strcat(story,"  ");
         generate_name(dstr,dstr2);
         strcat(story,dstr);
         strcat(story," ");
         strcat(story,dstr2);
         strcat(story,", speaking on behalf of the police department, stated ");
         strcat(story,"\"We now believe that ");
         strcat(story,ns.cr->propername);
         strcat(story," was taken ");
         strcat(story,ns.cr->joindays-1);
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
   do c=getkey(); while(c!='x'&&c!=ESC&&c!=ENTER&&c!=SPACEBAR);
}


/* news - major newspaper reporting on lcs and other topics */
void majornewspaper(char &clearformess,char canseethings)
{
   clearformess = true;

   int n=0;

   generate_random_event_news_stories();

   clean_up_empty_news_stories();

   if(canseethings) run_television_news_stories();

   assign_page_numbers_to_newspaper_stories();
   if(canseethings) display_newspaper();

   //DELETE STORIES
   for(n=0;n<len(newsstory);n++)
      handle_public_opinion_impact(*newsstory[n]);
   delete_and_clear(newsstory);
}

void display_newspaper()
{
   int writers = liberal_guardian_writing_power();

   for(int n=0;n<len(newsstory);n++)
   {
      bool liberalguardian=0;
      int header = -1;
      if(writers&&newsstory[n]->type!=NEWSSTORY_MAJOREVENT)
         liberalguardian=1;

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
               header=VIEW_DEATHPENALTY;
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
            case SITE_BUSINESS_BANK:
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
         case NEWSSTORY_CCS_NOBACKERS:
         case NEWSSTORY_CCS_DEFEATED:
            break;
      }
      if(liberalguardian)
      {
         if(newsstory[n]->type==NEWSSTORY_CCS_SITE||
            newsstory[n]->type==NEWSSTORY_CCS_KILLED_SITE)
         {
            newsstory[n]->positive=0;
         }
         displaystory(*newsstory[n],liberalguardian,header);
         if(newsstory[n]->positive)newsstory[n]->positive+=1;
      }
      else displaystory(*newsstory[n],0,-1);
   }
}

void generate_random_event_news_stories()
{
   //Conservative Crime Squad Strikes!
   if(endgamestate < ENDGAME_CCS_DEFEATED &&
      LCSrandom(30) < endgamestate)
   {
      newsstory.push_back(ccs_strikes_story());
   }

   // The slow defeat of the conservative crime squad...
   if(endgamestate < ENDGAME_CCS_DEFEATED &&
      ccsexposure >= CCSEXPOSURE_EXPOSED &&
      !LCSrandom(60))
   {
      advance_ccs_defeat_storyline();
   }

   // Random major event news stories
   if(!LCSrandom(60)) {
      newsstory.push_back(new_major_event());
   }
}

void advance_ccs_defeat_storyline()
{
   switch(ccsexposure)
   {
   default:
   case CCSEXPOSURE_NONE:
   case CCSEXPOSURE_LCSGOTDATA:
      break;
   case CCSEXPOSURE_EXPOSED:
      newsstory.push_back(ccs_exposure_story());
      break;
   case CCSEXPOSURE_NOBACKERS:
      newsstory.push_back(ccs_fbi_raid_story());
      break;
   }
}

void clean_up_empty_news_stories()
{
   // Delete stories that have no content or shouldn't be reported on
   for(int n=len(newsstory)-1;n>=0;n--)
   {
      // Squad site action stories without crimes
      if(newsstory[n]->type==NEWSSTORY_SQUAD_SITE&&
        !len(newsstory[n]->crime))
      {
         delete_and_remove(newsstory,n);
         continue;
      }

      // Police killed stories without police being killed
      if(newsstory[n]->type==NEWSSTORY_CARTHEFT ||
         newsstory[n]->type==NEWSSTORY_NUDITYARREST ||
         newsstory[n]->type==NEWSSTORY_WANTEDARREST ||
         newsstory[n]->type==NEWSSTORY_DRUGARREST ||
         newsstory[n]->type==NEWSSTORY_GRAFFITIARREST ||
         newsstory[n]->type==NEWSSTORY_BURIALARREST)
      {
         char conf=0;
         for(int c=0;c<len(newsstory[n]->crime);c++)
         {
            if(newsstory[n]->crime[c]==CRIME_KILLEDSOMEBODY)
            {
               conf=1;
               break;
            }
         }
         if(!conf)
         {
            delete_and_remove(newsstory,n);
            continue;
         }
      }

      // Sieges that aren't police actions
      if((newsstory[n]->type==NEWSSTORY_SQUAD_ESCAPED||
          newsstory[n]->type==NEWSSTORY_SQUAD_FLEDATTACK||
          newsstory[n]->type==NEWSSTORY_SQUAD_DEFENDED||
          newsstory[n]->type==NEWSSTORY_SQUAD_BROKESIEGE||
          newsstory[n]->type==NEWSSTORY_SQUAD_KILLED_SIEGEATTACK||
          newsstory[n]->type==NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE)&&
          newsstory[n]->siegetype!=SIEGE_POLICE)
      {
         delete_and_remove(newsstory,n);
         continue;
      }
   }
}

void assign_page_numbers_to_newspaper_stories()
{
   for(int n=len(newsstory)-1;n>=0;n--)
   {
      setpriority(*newsstory[n]);
      // Suppress squad actions that aren't worth a story
      if(newsstory[n]->type==NEWSSTORY_SQUAD_SITE &&
       ((newsstory[n]->priority<50 &&
         newsstory[n]->claimed==0)||
         newsstory[n]->priority<4))
      {
         delete_and_remove(newsstory,n);
         continue;
      }
      newsstory[n]->page=-1;
   }
   char acted;
   int curpage=1,curguardianpage=1;
   do
   {
      acted=0;
      // Sort the major newspapers
      int maxn=-1,maxp=-1;
      for(int n=0;n<len(newsstory);n++)
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
         if(newsstory[maxn]->priority<30&&curpage==1) curpage=2;
         if(newsstory[maxn]->priority<25&&curpage<3) curpage=3+LCSrandom(2);
         if(newsstory[maxn]->priority<20&&curpage<5) curpage=5+LCSrandom(5);
         if(newsstory[maxn]->priority<15&&curpage<10) curpage=10+LCSrandom(10);
         if(newsstory[maxn]->priority<10&&curpage<20) curpage=20+LCSrandom(10);
         if(newsstory[maxn]->priority<5&&curpage<30) curpage=30+LCSrandom(20);

         newsstory[maxn]->page=curpage;
         newsstory[maxn]->guardianpage=curguardianpage;
         curpage++;
         curguardianpage++;
         acted=1;
      }
   } while(acted);
}

void handle_public_opinion_impact(const newsstoryst &ns)
{
   // Check if this function is meant to handle public opinion impact
   // for this type of news story (primarily deals with squad/site actions)
   int okay_types[] = { NEWSSTORY_SQUAD_SITE, NEWSSTORY_SQUAD_ESCAPED, NEWSSTORY_SQUAD_FLEDATTACK,
      NEWSSTORY_SQUAD_DEFENDED, NEWSSTORY_SQUAD_BROKESIEGE, NEWSSTORY_SQUAD_KILLED_SIEGEATTACK,
      NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE, NEWSSTORY_SQUAD_KILLED_SITE, NEWSSTORY_WANTEDARREST,
      NEWSSTORY_GRAFFITIARREST, NEWSSTORY_CCS_SITE, NEWSSTORY_CCS_KILLED_SITE };
   int okay_type_num = len(okay_types);

   int i;
   for(i=0; i < okay_type_num; i++)
   {
      if(okay_types[i] == ns.type)
         break;
   }
   if(i == okay_type_num) return; // No impact for this news story type

   int impact = ns.priority;

   // Magnitude of impact will be affected by which page of the newspaper the story appears on
   if(ns.page==1) impact*=5;
   else if(ns.page==2) impact*=3;
   else if(ns.page==3) impact*=2;

   int maxpower = 1;
   if(ns.page==1) maxpower=100;
   else if(ns.page<5) maxpower=100-10*ns.page;
   else if(ns.page<10) maxpower=40;
   else if(ns.page<20) maxpower=20;
   else if(ns.page<30) maxpower=10;
   else if(ns.page<40) maxpower=5;
   else maxpower=1;

   // Five times effectiveness with the Liberal Guardian
   if(ns.positive==2)
      impact*=5;
   // Cap power
   if(impact>maxpower)
      impact=maxpower;

   impact/=10;
   impact++;

   // Account for squad responsible, rampages, and Liberal Guardian bias
   int impact_direction = ALIGN_LIBERAL;
   if(ns.type==NEWSSTORY_CCS_SITE || ns.type==NEWSSTORY_CCS_KILLED_SITE)
   {
      impact_direction = ALIGN_CONSERVATIVE;
      if(ns.positive)
         change_public_opinion(VIEW_CONSERVATIVECRIMESQUAD,impact,0);
      else
         change_public_opinion(VIEW_CONSERVATIVECRIMESQUAD,-impact,0);
   }
   else
   {
      change_public_opinion(VIEW_LIBERALCRIMESQUAD,2+impact);
      impact_direction = ALIGN_LIBERAL;
      if(ns.positive)
         change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,impact);
      else
         change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,-impact);
   }
   impact *= impact_direction;
   int squad_responsible = impact_direction;
   if(!ns.positive) impact /= 4;

   // Impact gun control issue
   change_public_opinion(VIEW_GUNCONTROL, ABS(impact)/10, 0, ABS(impact)*10);

   if(ns.loc == -1) return;

   // Location-specific issue impact
   std::vector<int> issues;
   switch(location[ns.loc]->type)
   {
   case SITE_LABORATORY_COSMETICS:
      issues.push_back(VIEW_ANIMALRESEARCH);
      issues.push_back(VIEW_WOMEN);
      break;
   case SITE_LABORATORY_GENETIC:
      issues.push_back(VIEW_ANIMALRESEARCH);
      issues.push_back(VIEW_GENETICS);
      break;
   case SITE_GOVERNMENT_POLICESTATION:
      issues.push_back(VIEW_POLICEBEHAVIOR);
      issues.push_back(VIEW_PRISONS);
      issues.push_back(VIEW_DRUGS);
      break;
   case SITE_GOVERNMENT_COURTHOUSE:
      issues.push_back(VIEW_DEATHPENALTY);
      issues.push_back(VIEW_JUSTICES);
      issues.push_back(VIEW_FREESPEECH);
      issues.push_back(VIEW_GAY);
      issues.push_back(VIEW_WOMEN);
      issues.push_back(VIEW_CIVILRIGHTS);
      break;
   case SITE_GOVERNMENT_PRISON:
      issues.push_back(VIEW_DEATHPENALTY);
      issues.push_back(VIEW_DRUGS);
      issues.push_back(VIEW_TORTURE);
      issues.push_back(VIEW_PRISONS);
      break;
   case SITE_GOVERNMENT_ARMYBASE:
      issues.push_back(VIEW_TORTURE);
      issues.push_back(VIEW_MILITARY);
      break;
   case SITE_GOVERNMENT_WHITE_HOUSE:
      break;
   case SITE_GOVERNMENT_INTELLIGENCEHQ:
      issues.push_back(VIEW_INTELLIGENCE);
      issues.push_back(VIEW_TORTURE);
      issues.push_back(VIEW_PRISONS);
      break;
   case SITE_INDUSTRY_SWEATSHOP:
      issues.push_back(VIEW_SWEATSHOPS);
      issues.push_back(VIEW_IMMIGRATION);
      break;
   case SITE_INDUSTRY_POLLUTER:
      issues.push_back(VIEW_SWEATSHOPS);
      issues.push_back(VIEW_POLLUTION);
      break;
   case SITE_INDUSTRY_NUCLEAR:
      issues.push_back(VIEW_NUCLEARPOWER);
      break;
   case SITE_CORPORATE_HEADQUARTERS:
      issues.push_back(VIEW_TAXES);
      issues.push_back(VIEW_CORPORATECULTURE);
      issues.push_back(VIEW_WOMEN);
      break;
   case SITE_CORPORATE_HOUSE:
      issues.push_back(VIEW_TAXES);
      issues.push_back(VIEW_CEOSALARY);
      break;
   case SITE_MEDIA_AMRADIO:
      issues.push_back(VIEW_AMRADIO);
      issues.push_back(VIEW_FREESPEECH);
      issues.push_back(VIEW_GAY);
      issues.push_back(VIEW_WOMEN);
      issues.push_back(VIEW_CIVILRIGHTS);
      break;
   case SITE_MEDIA_CABLENEWS:
      issues.push_back(VIEW_CABLENEWS);
      issues.push_back(VIEW_FREESPEECH);
      issues.push_back(VIEW_GAY);
      issues.push_back(VIEW_WOMEN);
      issues.push_back(VIEW_CIVILRIGHTS);
      break;
   case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
      issues.push_back(VIEW_TAXES);
      issues.push_back(VIEW_CEOSALARY);
      issues.push_back(VIEW_GUNCONTROL);
      break;
   case SITE_BUSINESS_CIGARBAR:
      issues.push_back(VIEW_TAXES);
      issues.push_back(VIEW_CEOSALARY);
      issues.push_back(VIEW_WOMEN);
      break;
   case SITE_BUSINESS_BANK:
      issues.push_back(VIEW_TAXES);
      issues.push_back(VIEW_CEOSALARY);
      issues.push_back(VIEW_CORPORATECULTURE);
      break;
   }
   for(i=0; i<len(issues); i++)
      change_public_opinion(issues[i],impact,squad_responsible,impact*10);
}



/* news - graphics */
void loadgraphics()
{
   int picnum,dimx,dimy;

   FILE *h;

   if((h=LCSOpenFile("largecap.cpc", "rb", LCSIO_PRE_ART))!=NULL)
   {

      fread(&picnum,sizeof(int),1,h);
      fread(&dimx,sizeof(int),1,h);
      fread(&dimy,sizeof(int),1,h);
      for(int p=0;p<picnum;p++)
         for(int x=0;x<dimx;x++)
            for(int y=0;y<dimy;y++)
               fread(&bigletters[p][x][y][0],sizeof(char),4,h);
      LCSCloseFile(h);
   }

   if((h=LCSOpenFile("newstops.cpc", "rb", LCSIO_PRE_ART))!=NULL)
   {

      fread(&picnum,sizeof(int),1,h);
      fread(&dimx,sizeof(int),1,h);
      fread(&dimy,sizeof(int),1,h);
      for(int p=0;p<picnum;p++)
         for(int x=0;x<dimx;x++)
            for(int y=0;y<dimy;y++)
               fread(&newstops[p][x][y][0],sizeof(char),4,h);
      LCSCloseFile(h);
   }

   if((h=LCSOpenFile("newspic.cpc", "rb", LCSIO_PRE_ART))!=NULL)
   {

      fread(&picnum,sizeof(int),1,h);
      fread(&dimx,sizeof(int),1,h);
      fread(&dimy,sizeof(int),1,h);
      for(int p=0;p<picnum;p++)
         for(int x=0;x<dimx;x++)
            for(int y=0;y<dimy;y++)
               fread(&newspic[p][x][y][0],sizeof(char),4,h);
      LCSCloseFile(h);
   }
}

void displaycenterednewsfont(const std::string& str,int y)
{
   int width=-1;
   int s;
   for(s=0;s<len(str);s++)
   {
      if(str[s]>='A'&&str[s]<='Z')width+=6;
      else if(str[s]=='\'')width+=4;
      else width+=3;
   }

   int x=39-width/2;

   for(s=0;s<len(str);s++)
   {
      if((str[s]>='A'&&str[s]<='Z')||str[s]=='\'')
      {
         int p;
         if(str[s]>='A'&&str[s]<='Z') p=str[s]-'A';
         else p=26;
         int lim=6;
         if(str[s]=='\'') lim=4;
         if(s==len(str)-1) lim--;
         for(int x2=0;x2<lim;x2++) for(int y2=0;y2<7;y2++)
         {
            move(y+y2,x+x2);

#ifdef NCURSES
            // Clean the square first.
            set_color(COLOR_BLACK, COLOR_BLACK, 0);
            addchar(' ');
            move(y+y2,x+x2);
#endif

            if(x2==5)
            {
               set_color(COLOR_WHITE,COLOR_WHITE,0);
               addchar(' ');
            }
            else
            {
               set_color(translateGraphicsColor(bigletters[p][x2][y2][1]),
                         translateGraphicsColor(bigletters[p][x2][y2][2]),
                         bigletters[p][x2][y2][3]);
               addchar(bigletters[p][x2][y2][0]);
            }
         }
         x+=lim;
      }
      else
      {
         set_color(COLOR_WHITE,COLOR_WHITE,0);
         for(int x2=0;x2<3;x2++) for(int y2=0;y2<7;y2++)
         {
            move(y+y2,x+x2);
            addchar(' ');
         }
         x+=3;
      }
   }
}

void displaycenteredsmallnews(const std::string& str,int y)
{
   int x=39-((len(str)-1)>>1);
   move(y,x);
   set_color(COLOR_BLACK,COLOR_WHITE,0);
   addstr(str);
}

void displaynewspicture(int p,int y)
{
   for(int x2=0;x2<78;x2++)
      for(int y2=0;y2<15;y2++)
      {
         if(y+y2>24) break;
         move(y+y2,1+x2);
         set_color(translateGraphicsColor(newspic[p][x2][y2][1]),
                   translateGraphicsColor(newspic[p][x2][y2][2]),
                   newspic[p][x2][y2][3]);
         addchar(newspic[p][x2][y2][0]);
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

   while(curpos<len(story)&&cury<25)
   {
      content=0;
      totalwidth=0;
      addstrcur=0;
      length=storyx_e[cury]-storyx_s[cury]+1;
      if(length==0){cury++;if(endparagraph>0)endparagraph--;continue;}

      for(i=curpos;i<len(story);i++)
      {
         if(story[i]=='&'&&story[i+1]!='&')
         {
            i++;
            if(story[i]=='c')iscentered=1;
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
               while(story[i]!=' ') i--,addstrcur--;
               while(story[i]==' ') i++;
               addstring[addstrcur]='\x0';
               break;
            }
            addstrcur++;
         }
      }

      if(i==len(story)) addstring[addstrcur]='\x0';

      if(len(addstring)&&content)
      {
         int words=0;
         char silent=1;
         vector<int> spacex;
         for(int s2=0;s2<len(addstring);s2++)
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

         while(!endparagraph&&words>1&&len(addstring)<length&&!iscentered)
         {
            int csp=pickrandom(spacex);

            for(int x=0;x<len(spacex);x++)
               if(spacex[x]>csp) spacex[x]++;

            int l=len(addstring);
            for(int s=l+1;s>csp;s--)
               addstring[s]=addstring[s-1];
         }

         char *news=new char[len(addstring)+1];
         strcpy(news,addstring);
         text.push_back(news);
         centered.push_back(iscentered);
         cury++;
         if(endparagraph>0)
            endparagraph--,iscentered=0;
      }

      curpos=i;
   }

   set_color(COLOR_BLACK,COLOR_WHITE,0);
   for(int t=0;t<len(text);t++)
   {
      if(y+t>=25) break;
      if(text[t][len(text[t])-1]==' ') // remove trailing space
         text[t][len(text[t])-1]='\x0'; // (necessary for proper centering and to not overwrite borders around an ad)
      if(centered[t])
         move(y+t,((storyx_s[y+t]+storyx_e[y+t]-len(text[t])+1)>>1));
      else move(y+t,storyx_s[y+t]);
      addstr(text[t]);

      delete[] text[t];
   }
   text.clear();
}



/* news - make some filler junk */
void generatefiller(char *story,int amount)
{  //TODO: Use text from filler.cpp
   strcat(story,"&r"+cityname()+" - ");
   for(int par=0;amount>0;amount--)
   {
      par++;
      for(int i=0;i<LCSrandom(10)+3;i++)strcat(story,"~");
      if(amount>1)strcat(story," ");
      if(par>=50&&!LCSrandom(5)&&amount>20)
      {
         par=0;
         strcat(story,"&r  ");
      }
   }
   strcat(story,"&r");
}


newsstoryst* new_major_event()
{
   newsstoryst *ns=new newsstoryst;
   ns->type=NEWSSTORY_MAJOREVENT;
   while(true)
   {
      ns->view=LCSrandom(VIEWNUM-3);
      ns->positive=LCSrandom(2);

      // Skip issues that we have no news stories for
      if(ns->view==VIEW_IMMIGRATION)continue;
      if(ns->view==VIEW_DRUGS)continue;
      if(ns->view==VIEW_MILITARY)continue;
      if(ns->view==VIEW_CIVILRIGHTS)continue;
      if(ns->view==VIEW_TORTURE)continue;
      //if(ns->view==VIEW_POLITICALVIOLENCE)continue;

      // News stories that don't apply when the law is extreme -- covering
      // nuclear power when it's banned, police corruption when it doesn't
      // exist, out of control pollution when it's under control, etc.
      if(ns->positive) {
         if(ns->view==VIEW_WOMEN&&law[LAW_ABORTION]==-2)continue; // Abortion banned
         if(ns->view==VIEW_DEATHPENALTY&&law[LAW_DEATHPENALTY]==2)continue; // Death penalty banned
         if(ns->view==VIEW_NUCLEARPOWER&&law[LAW_NUCLEARPOWER]==2)continue; // Nuclear power banned
         if(ns->view==VIEW_ANIMALRESEARCH&&law[LAW_ANIMALRESEARCH]==2)continue; // Animal research banned
         if(ns->view==VIEW_POLICEBEHAVIOR&&law[LAW_POLICEBEHAVIOR]==2)continue; // Police corruption eliminated
         if(ns->view==VIEW_INTELLIGENCE&&law[LAW_PRIVACY]==2)continue; // Privacy rights respected
         if(ns->view==VIEW_SWEATSHOPS&&law[LAW_LABOR]==2)continue; // Sweatshops nonexistant
         if(ns->view==VIEW_POLLUTION&&law[LAW_POLLUTION]>=1)continue; // Pollution under control
         if(ns->view==VIEW_CORPORATECULTURE&&law[LAW_CORPORATE]==2)continue; // Regulation controls corporate corruption
         if(ns->view==VIEW_CEOSALARY&&law[LAW_CORPORATE]==2)continue; // CEOs aren't rich
      } else {
         if(ns->view==VIEW_WOMEN&&law[LAW_ABORTION]<2)continue; // Partial birth abortion banned
         if(ns->view==VIEW_AMRADIO&&law[LAW_FREESPEECH]==-2)continue; // AM Radio is censored to oblivion
         if(ns->view==VIEW_ANIMALRESEARCH&&law[LAW_ANIMALRESEARCH]==2)continue; // Animal research banned
      }

      break;
   }

   if(ns->positive) change_public_opinion(ns->view,20,0);
   else change_public_opinion(ns->view,-20,0);
   public_interest[ns->view]+=50;
   return ns;
}

int liberal_guardian_writing_power()
{
   int power=0;
   for(int i=0;i<len(pool);i++)
   {
      if(pool[i]->alive&&pool[i]->activity.type==ACTIVITY_WRITE_GUARDIAN)
      {
         if(pool[i]->location!=-1&&
            location[pool[i]->location]->compound_walls & COMPOUND_PRINTINGPRESS)
         {
            pool[i]->train(SKILL_WRITING,LCSrandom(3)); // Experience gain
            power+=pool[i]->skill_roll(SKILL_WRITING); // Record the writer on this topic
            criminalize(*pool[i],LAWFLAG_SPEECH); // Record possibly illegal speech activity
         }
         else pool[i]->activity.type=ACTIVITY_NONE;
      }
   }
   return power;
}

newsstoryst* ccs_strikes_story()
{
   newsstoryst *ns=new newsstoryst;

   // Chance of CCS squad wipe
   if(LCSrandom(10))ns->type=NEWSSTORY_CCS_SITE;
   else ns->type=NEWSSTORY_CCS_KILLED_SITE;

   // Chance of positive CCS story
   ns->positive=true;

   do {
      ns->loc=LCSrandom(len(location));
   } while(location[ns->loc]->renting!=-1);

   return ns;
}

newsstoryst* ccs_exposure_story()
{
   newsstoryst* ns=new newsstoryst;
   ns->type=NEWSSTORY_CCS_NOBACKERS;
   ns->priority=8000;
   ccsexposure=CCSEXPOSURE_NOBACKERS;
   // arrest seventeen representatives and eight senators
   int arrestsleft = 8;
   for(int i=0; i<SENATENUM; i++)
   {
      if((senate[i]==-2 || senate[i]==-1)&&!LCSrandom(4))
      {
         senate[i]=ALIGN_ELITELIBERAL;
         arrestsleft--;
         if(arrestsleft<=0) break;
      }
   }
   arrestsleft = 17;
   for(int i=0; i<HOUSENUM; i++)
   {
      if((house[i]==-2 || house[i]==-1)&&!LCSrandom(4))
      {
         house[i]=ALIGN_ELITELIBERAL;
         arrestsleft--;
         if(arrestsleft<=0) break;
      }
   }
   // change police regulation issue to be more liberal
   law[LAW_POLICEBEHAVIOR] += 2;
   if(law[LAW_POLICEBEHAVIOR] > ALIGN_ELITELIBERAL)
      law[LAW_POLICEBEHAVIOR] = ALIGN_ELITELIBERAL;
   change_public_opinion(VIEW_POLICEBEHAVIOR,50);
   change_public_opinion(VIEW_CONSERVATIVECRIMESQUAD,50);

   return ns;
}

newsstoryst* ccs_fbi_raid_story()
{
   newsstoryst* ns=new newsstoryst;
   ns->type=NEWSSTORY_CCS_DEFEATED;
   ns->priority=8000;
   endgamestate=ENDGAME_CCS_DEFEATED;
   // arrest or kill ccs sleepers
   for(int p=0;p<len(pool);p++)
   {
      if(pool[p]->flag&CREATUREFLAG_SLEEPER)
      {
         if(pool[p]->type==CREATURE_CCS_VIGILANTE||pool[p]->type==CREATURE_CCS_ARCHCONSERVATIVE||
            pool[p]->type==CREATURE_CCS_MOLOTOV||pool[p]->type==CREATURE_CCS_SNIPER)
         {
            pool[p]->flag&=~CREATUREFLAG_SLEEPER;
            criminalize(*pool[p],LAWFLAG_RACKETEERING);
            capturecreature(*pool[p]);
         }
      }
   }
   // hide ccs safehouses
   for(int l=0;l<len(location);l++)
   {
      if(location[l]->renting==RENTING_CCS)
      {
         location[l]->renting=RENTING_NOCONTROL;
         location[l]->hidden=true;
      }
   }
   // go militarized police
   change_public_opinion(VIEW_POLICEBEHAVIOR,-20);
   return ns;
}

