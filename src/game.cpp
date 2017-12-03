/*
 * Copyright (c) 2002,2003,2004 by Tarn Adams
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

//outstanding issues
   //site trucker-type bug still happens (latte-stand)
      //might have to do with missing location in the verifyworklocation() loop
         //but cannot locate instance of this
   //can have hostage in court?

//changes 3.10 alpha 1
   //added AZERTY keyboard support
   //added bullet-proof vests
   //added SWAT units
   //reduced chance of hitting bystanders
   //added multiple hit chances for automatic weapons
   //added extra hit probability for shotguns
   //removed sever types from handguns and knives
   //fixed bug in newspaper site emphasis
   //reduced fence values for armor
   //sleepers can warn before raids
   //courts made more difficult
   //hacking harder, a little less rewarding
   //sleeper overhaul
   //dating conservatives, kidnapping on dates more risky
   //made it easier to maintain LCS integrity when someone high up is arrested
   //changed version variable to 31001

//changes 3.09
   //udistrict -> outoftown for the outskirts location
   //in advanceday(), made people going on vacation drop off squad loot
      //if they are the last person in their squad (or else it would be deleted)
   //added a function to every instance where a vehicle is deleted that
      //makes sure that the liberal car preferences are nullified
   //update version number on the title screen
   //changed version variable to 30900
   //changed activesquad to squad[sq] in one portion of the squad movement code
   //added a tweak to verifyworklocation(), but problem probably still exists

//changes 3.08
   //siegetype occurred many times where it shouldn't have in a majornewspaper() if statement
   //added loitering offense if no other offense exists
   //fixed the wheelchair bug in assemblesquad()

//OUTSTANDING ISSUES:
//possible bug with hauling people
//somebody claims saving works only 3/4 of the time (no confirmation)
//somebody claims squads don't move (sounds like older version bug, they haven't told me version)

#include <externs.h>
#include <ctime>
#include "creature/creaturetypecache.h"
#include "news/news.h"


template<class Type>
  bool
  populate_from_xml(vector<Type*>& types,string file,Log& log);

extern CreatureTypeCache creature_type_cache; // @TODO remove me

void
populate_from_xml2(CreatureTypeCache& ctc, string const& file, Log& log);

bool
populate_masks_from_xml(vector<ArmorType*>& masks,string file,Log& log);

void
end_game();

bool autosave;

#ifdef _WIN32
bool fixcleartype=false;
#endif

int
main(int argc, char* argv[])
{
   init_console(); // do this FIRST
   //start curses
   initscr();

   // register cleanup
   atexit(end_game);

   gamelog.initialize(GAMELOG_FILEPATH, OVERWRITE_GAMELOG, NEWLINEMODE_GAMELOG); //Initialize the gamelog (and also initialize artdir and homedir)

   time_t t = time(0);
   struct tm *now = localtime(&t); //Do not need to deallocate this. Statically allocated by system
   char datetime[41];
   sprintf(datetime, "---------%i-%02i-%02i %02i:%02i:%02i---------\n\n\n",
      now->tm_year+1900, now->tm_mon+1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec); //YYYY-MM-DD HH:MM:SS format

   gamelog.log(string("\n\n\n---------- PROGRAM STARTED ----------\n") + datetime);

   music.play(MUSIC_TITLEMODE); // initialize music and play title mode song (do this BEFORE displaying anything on the screen, but AFTER initializing artdir and homedir)

   // set window title
   char wtitle[50];
   strcpy(wtitle,"Liberal Crime Squad ");
   strcat(wtitle,PACKAGE_VERSION);
   set_title(wtitle);

   noecho();

   //initialize curses color
   start_color();

   initMainRNG();

   //initialize the array of color pairs
   for(int i=0;i<8;i++)
      for(int j=0;j<8;j++)
      {
         if(i==0&&j==0)
         {
            init_pair(7*8,0,0);
            continue;
         }
         if(i==7&&j==0) continue;
         init_pair(i*8+j,i,j);
      }

   //turns off cursor
   curs_set(0);

   //begin the game loop
   keypad(stdscr,TRUE);

   raw_output(TRUE);

   //addstr("Loading Graphics... ");
   //getkey();

   loadgraphics();

   //addstr("Loading Init File Options... ");
   //getkey();

   loadinitfile();

   //addstr("Loading sitemaps.txt... ");
   //getkey();

   oldMapMode=!readConfigFile("sitemaps.txt"); // load site map data
   if(oldMapMode)
   {
      addstr("Failed to load sitemaps.txt! Reverting to old map mode.",gamelog);
      gamelog.nextMessage();

      getkey();
   }

   //move(1,0);
   //addstr("Setting initial game data... ");
   //getkey();

   strcpy(slogan,"We need a slogan!");
   if(!LCSrandom(20))
   {
      switch(LCSrandom(7))
      {
      case 0: strcpy(slogan,"To Rogues and Revolution!"); break;
      case 1: strcpy(slogan,"Hell yes, LCS!"); break;
      case 2: strcpy(slogan,"Striking high, standing tall!"); break;
      case 3: strcpy(slogan,"Revolution never comes with a warning!"); break;
      case 4: strcpy(slogan,"True Liberal Justice!"); break;
      case 5: strcpy(slogan,"Laissez ain't fair!"); break;
      case 6: strcpy(slogan,"This is a slogan!"); break;
      }
   }

   //Initialize sorting choices.
   for(int s=0;s<SORTINGCHOICENUM;s++)
      activesortingchoice[s]=SORTING_NONE;

   for(int v=0;v<VIEWNUM;v++)
   {
      attitude[v]=30+LCSrandom(25);
      public_interest[v]=0;
      background_liberal_influence[v]=0;
   }
   attitude[VIEW_LIBERALCRIMESQUAD]=0;
   attitude[VIEW_LIBERALCRIMESQUADPOS]=5;
   //attitude[VIEW_POLITICALVIOLENCE]=5;

#ifdef REVOLUTIONNOW
   for(int v=0;v<VIEWNUM;v++)
      attitude[v]=100;
#endif

   law[LAW_ABORTION] = Alignment::LIBERAL;
   law[LAW_ANIMALRESEARCH] = Alignment::CONSERVATIVE;
   law[LAW_POLICEBEHAVIOR] = Alignment::CONSERVATIVE;
   law[LAW_PRIVACY] = Alignment::CONSERVATIVE;
   law[LAW_DEATHPENALTY] = Alignment::CONSERVATIVE;
   law[LAW_NUCLEARPOWER] = Alignment::CONSERVATIVE;
   law[LAW_POLLUTION] = Alignment::CONSERVATIVE;
   law[LAW_LABOR] = Alignment::MODERATE;
   law[LAW_GAY] = Alignment::LIBERAL;
   law[LAW_CORPORATE] = Alignment::MODERATE;
   law[LAW_FREESPEECH] = Alignment::MODERATE;
   law[LAW_FLAGBURNING] = Alignment::LIBERAL;
   law[LAW_GUNCONTROL] = Alignment::CONSERVATIVE;
   law[LAW_TAX] = Alignment::MODERATE;
   law[LAW_WOMEN] = Alignment::LIBERAL;
   law[LAW_CIVILRIGHTS] = Alignment::LIBERAL;
   law[LAW_DRUGS] = Alignment::CONSERVATIVE;
   law[LAW_IMMIGRATION] = Alignment::MODERATE;
   law[LAW_ELECTIONS] = Alignment::MODERATE;
   law[LAW_MILITARY] = Alignment::CONSERVATIVE;
   law[LAW_PRISONS] = Alignment::MODERATE;
   law[LAW_TORTURE] = Alignment::CONSERVATIVE;

#ifdef SHITLAWS
   for(int l=0;l<LAWNUM;l++) law[l]=-2;
#endif

#ifdef PERFECTLAWS
   for(int l=0;l<LAWNUM;l++) law[l]=2;
#endif

   for(int s=0; s<SENATENUM; s++)
   {
      if      (s<25) senate[s] = Alignment::ARCH_CONSERVATIVE;
      else if (s<60) senate[s] = Alignment::CONSERVATIVE;
      else if (s<80) senate[s] = Alignment::MODERATE;
      else if (s<95) senate[s] = Alignment::LIBERAL;
      else           senate[s] = Alignment::ELITE_LIBERAL;
   }

   for(int h=0; h<HOUSENUM; h++)
   {
      if      (h<50)  house[h] = Alignment::ARCH_CONSERVATIVE;
      else if (h<250) house[h] = Alignment::CONSERVATIVE;
      else if (h<350) house[h] = Alignment::MODERATE;
      else if (h<400) house[h] = Alignment::LIBERAL;
      else            house[h] = Alignment::ELITE_LIBERAL;
   }

   for(int c=0; c<COURTNUM; c++)
   {
      if      (c<3) court[c] = Alignment::ARCH_CONSERVATIVE;
      else if (c<5) court[c] = Alignment::CONSERVATIVE;
      else if (c<5) court[c] = Alignment::MODERATE;
      else if (c<8) court[c] = Alignment::LIBERAL;
      else          court[c] = Alignment::ELITE_LIBERAL;
      do
      {
         if (to_right_of(court[c], Alignment::MODERATE))
             generate_name(courtname[c],GENDER_WHITEMALEPATRIARCH);
         else
         generate_name(courtname[c]);
      } while(len(courtname[c])>20);
   }

   for(int e=0;e<EXECNUM;e++)
   {
      exec[e] = Alignment::ARCH_CONSERVATIVE;
      generate_name(execname[e],GENDER_WHITEMALEPATRIARCH);
   }

   strcpy(lcityname,cityname());

   xmllog.initialize("xmllog",true,1);
   bool xml_loaded_ok = true;
   xml_loaded_ok&=populate_from_xml(vehicletype,"vehicles.xml",xmllog);
   xml_loaded_ok&=populate_from_xml(cliptype,"clips.xml",xmllog);
   xml_loaded_ok&=populate_from_xml(weapontype,"weapons.xml",xmllog);
   xml_loaded_ok&=populate_from_xml(armortype,"armors.xml",xmllog);
   xml_loaded_ok&=populate_masks_from_xml(armortype,"masks.xml",xmllog);
   xml_loaded_ok&=populate_from_xml(loottype,"loot.xml",xmllog);
   xml_loaded_ok&=populate_from_xml(augmenttype,"augmentations.xml",xmllog);
   if(!xml_loaded_ok) exit(EXIT_FAILURE);

   populate_from_xml2(creature_type_cache, "creatures.xml", xmllog);

   //addstr("Attempting to load saved game... ");
   //getkey();

   //addstr("Setup complete!");
   //getkey();

   clear();
   mode_title();

   return EXIT_SUCCESS;
}

/* Free memory and exit the game */
void
end_game()
{
   #ifdef _WIN32
   end_cleartype_fix(); // won't do anything unless fixcleartype is true
   #endif

   delete_and_clear(location);
   delete_and_clear(squad);
   delete_and_clear(weapontype);
   delete_and_clear(cliptype);
   delete_and_clear(armortype);
   delete_and_clear(loottype);
   delete_and_clear(augmenttype);
   delete_and_clear(vehicletype);
   delete_and_clear(vehicle);
   delete_and_clear(pool);
   delete_and_clear(sitemaps);
   delete_and_clear(recruit);
   delete_and_clear(date);
   delete_and_clear(groundloot);

   music.quit(); // shut down music

   endwin();
}


template<class Type>
  bool
  populate_from_xml(vector<Type*>& types, string file, Log& log)
  {
     CMarkup xml;
     if(!xml.Load(string(artdir)+file))
     { // File is missing or not valid XML.
        addstr("Failed to load "+file+"!",log);

        getkey();

        // Will cause abort here or else if file is missing all unrecognized types
        // loaded from a saved game will be deleted. Also, you probably don't want
        // to play with a whole category of things missing anyway. If the file
        // does not have valid xml, then behaviour is kind of undefined so it's
        // best to abort then too.
        return false;
     }

     xml.FindElem();
     xml.IntoElem();
     while(xml.FindElem()) types.push_back(new Type(xml.GetSubDoc()));
     return true;
  }


void
populate_from_xml2(CreatureTypeCache& ctc, string const& file, Log& log)
{
  std::ifstream istr(std::string(artdir) + file);
  if (!istr)
  {
    addstr("Failed to load "+file+"!",log);
    getkey();
    return;
  }
  std::string xml((std::istreambuf_iterator<char>(istr)),
                   std::istreambuf_iterator<char>());

  ctc.load_from_xml_string(xml);
}


bool
populate_masks_from_xml(vector<ArmorType*>& masks, string file, Log& log)
{
   CMarkup xml;
   if(!xml.Load(string(artdir)+file))
   { //File is missing or not valid XML.
      addstr("Failed to load "+file+"!",log);

      getkey();

      return false; //Abort.
   }

   xml.FindElem();
   xml.IntoElem();
   int defaultindex;
   if(xml.FindElem("default")) defaultindex=getarmortype(xml.GetData());
   else
   {
      addstr("Default missing for masks!",log);

      getkey();

      return false; //Abort.
   }
   if(defaultindex==-1)
   {
      addstr("Default for masks is not a known armor type!",log);

      getkey();

      return false; //Abort.
   }

   xml.ResetMainPos();
   while(xml.FindElem("masktype")) armortype.push_back(new ArmorType(*armortype[defaultindex],xml.GetSubDoc()));
   return true;
}
