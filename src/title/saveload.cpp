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

/*
 * This file was created by Chris Johnson (grundee@users.sourceforge.net)
 * by copying code from game.cpp into monthly/endgame.cpp.
 */

#include <externs.h>
#include <sys/stat.h>
#pragma GCC diagnostic ignored "-Wunused-result"

// TODO: It would be really cool to be able to "export" characters.

/* handles saving */

bool file_exists(const std::string& filename)
{
   struct stat buf;
   if (stat(filename.c_str(), &buf) != -1)
   {
      return false;
   }
   return true;
}

void savegame(const string& filename)
{
#ifdef NOSAVE
   return;
#endif

   bool dummy_b;
   int dummy;
   FILE *h;
   int l;

   h=LCSOpenFile(filename.c_str(), "wb", LCSIO_PRE_HOME);

   if(h!=NULL)
   {
      int lversion=version;
      fwrite(&lversion,sizeof(int),1,h);

      fwrite(seed,sizeof(unsigned long),RNG_SIZE,h);

      fwrite(&mode,sizeof(short),1,h);
      fwrite(&wincondition,sizeof(short),1,h);
      fwrite(&fieldskillrate,sizeof(short),1,h);

      fwrite(&day,sizeof(int),1,h);
      fwrite(&month,sizeof(int),1,h);
      fwrite(&year,sizeof(int),1,h);
      fwrite(&execterm,sizeof(short),1,h);
      fwrite(&presparty,sizeof(short),1,h);
      fwrite(&amendnum,sizeof(int),1,h);

      fwrite(&multipleCityMode,sizeof(bool),1,h);
      fwrite(&termlimits,sizeof(bool),1,h);
      fwrite(&deagle,sizeof(bool),1,h);
      fwrite(&m249,sizeof(bool),1,h);
      fwrite(&notermlimit,sizeof(bool),1,h);
      fwrite(&nocourtpurge,sizeof(bool),1,h);
      fwrite(&stalinmode,sizeof(bool),1,h);

      fwrite(&stat_recruits,sizeof(int),1,h);
      fwrite(&stat_dead,sizeof(int),1,h);
      fwrite(&stat_kills,sizeof(int),1,h);
      fwrite(&stat_kidnappings,sizeof(int),1,h);
      fwrite(&stat_buys,sizeof(int),1,h);
      fwrite(&stat_burns,sizeof(int),1,h);

      fwrite(&endgamestate,sizeof(char),1,h);
      fwrite(&ccsexposure,sizeof(char),1,h);
      fwrite(&ccs_kills,sizeof(char),1,h);

      fwrite(&Vehicle::curcarid,sizeof(long),1,h);
      fwrite(&curcreatureid,sizeof(long),1,h);
      fwrite(&cursquadid,sizeof(long),1,h);
      fwrite(&police_heat,sizeof(int),1,h);
      fwrite(&offended_corps,sizeof(short),1,h);
      fwrite(&offended_cia,sizeof(short),1,h);
      fwrite(&offended_amradio,sizeof(short),1,h);
      fwrite(&offended_cablenews,sizeof(short),1,h);
      fwrite(&offended_firemen,sizeof(short),1,h);
      fwrite(attorneyseed,sizeof(unsigned long),RNG_SIZE,h);
      //fwrite(&selectedsiege,sizeof(long),1,h);
      fwrite(lcityname,sizeof(char),CITY_NAMELEN,h);
      fwrite(&newscherrybusted,sizeof(char),1,h);

      fwrite(slogan,sizeof(char),SLOGAN_LEN,h);
      fwrite(&ledger,sizeof(class Ledger),1,h);
      fwrite(&party_status,sizeof(short),1,h);

      fwrite(attitude,sizeof(short),VIEWNUM,h);
      fwrite(law,sizeof(short),LAWNUM,h);
      fwrite(house,sizeof(short),HOUSENUM,h);
      fwrite(senate,sizeof(short),SENATENUM,h);
      fwrite(court,sizeof(short),COURTNUM,h);
      fwrite(courtname,sizeof(char)*POLITICIAN_NAMELEN,9,h);
      fwrite(exec,sizeof(char),EXECNUM,h);
      fwrite(execname,sizeof(char)*POLITICIAN_NAMELEN,EXECNUM,h);
      fwrite(oldPresidentName,sizeof(char),POLITICIAN_NAMELEN,h);

      //LOCATIONS
      dummy=len(location);
      fwrite(&dummy,sizeof(int),1,h);
      for(l=0;l<len(location);l++)
      {
         consolidateloot(location[l]->loot); // consolidate loot before saving
         dummy=len(location[l]->loot);
         fwrite(&dummy,sizeof(int),1,h);
         for(int l2=0;l2<len(location[l]->loot);l2++)
         {
            std::string itemStr = location[l]->loot[l2]->showXml();
            int itemSize = len(itemStr);

            fwrite(&itemSize,sizeof(int),1,h);
            fwrite(itemStr.c_str(),itemSize,1,h);
         }
         dummy=len(location[l]->changes);
         fwrite(&dummy,sizeof(int),1,h);
         for(int l2=0;l2<len(location[l]->changes);l2++)
            fwrite(&location[l]->changes[l2],sizeof(sitechangest),1,h);

         fwrite(location[l]->name,sizeof(char),LOCATION_NAMELEN,h);
         fwrite(location[l]->shortname,sizeof(char),LOCATION_SHORTNAMELEN,h);
         fwrite(&location[l]->type,sizeof(char),1,h);
         fwrite(&location[l]->city,sizeof(int),1,h);
         fwrite(&location[l]->area,sizeof(int),1,h);
         fwrite(&location[l]->parent,sizeof(int),1,h);
         fwrite(&location[l]->id,sizeof(int),1,h);

         fwrite(&location[l]->renting,sizeof(int),1,h);
         fwrite(&location[l]->newrental,sizeof(char),1,h);
         fwrite(&location[l]->needcar,sizeof(char),1,h);
         fwrite(&location[l]->closed,sizeof(int),1,h);
         fwrite(&location[l]->hidden,sizeof(bool),1,h);
         fwrite(&location[l]->mapped,sizeof(bool),1,h);
         fwrite(&location[l]->upgradable,sizeof(bool),1,h);
         fwrite(&location[l]->highsecurity,sizeof(int),1,h);
         fwrite(&location[l]->siege,sizeof(siegest),1,h);
         fwrite(&location[l]->heat,sizeof(int),1,h);
         fwrite(&location[l]->heat_protection,sizeof(int),1,h);
         fwrite(&location[l]->compound_walls,sizeof(int),1,h);
         fwrite(&location[l]->compound_stores,sizeof(int),1,h);
         fwrite(&location[l]->front_business,sizeof(char),1,h);
         fwrite(location[l]->front_name,sizeof(char),LOCATION_NAMELEN,h);
         fwrite(location[l]->front_shortname,sizeof(char),LOCATION_SHORTNAMELEN,h);
         fwrite(&location[l]->haveflag,sizeof(bool),1,h);

         fwrite(location[l]->mapseed,sizeof(unsigned long),RNG_SIZE,h);
      }

      //VEHICLES
      dummy=len(vehicle);
      fwrite(&dummy,sizeof(int),1,h);
      for(l=0;l<len(vehicle);l++)
      {
         std::string vehicleStr = vehicle[l]->showXml();
         int vehicleSize = len(vehicleStr);

         fwrite(&vehicleSize,sizeof(int),1,h);
         fwrite(vehicleStr.c_str(),vehicleSize,1,h);
      }

      //POOL
      dummy=len(pool);
      fwrite(&dummy,sizeof(int),1,h);
      for(int pl=0;pl<len(pool);pl++)
      {
         std::string creatureStr = pool[pl]->showXml();
         int creatureSize = len(creatureStr);

         fwrite(&creatureSize,sizeof(int),1,h);
         fwrite(creatureStr.c_str(),creatureSize,1,h);
         //fwrite(pool[pl],sizeof(Creature),1,h);
         //write extra interrogation data if applicable
         if (pool[pl]->align == Alignment::CONSERVATIVE && pool[pl]->alive)
         {
            interrogation* &intr = pool[pl]->activity.intr();
            fwrite(intr->techniques,sizeof(bool[6]),1,h);
            fwrite(&intr->druguse,sizeof(int),1,h);

            //deep write rapport map
            int size = len(intr->rapport);
            fwrite(&size,sizeof(int),1,h);
            for(map<long,float_zero>::iterator i=intr->rapport.begin();i!=intr->rapport.end();i++)
            {
               fwrite(&i->first,sizeof(long),1,h);
               fwrite(&i->second,sizeof(float_zero),1,h);
            }
         }
      }

      //Unique Creatures
      {
         std::string uniquecreaturesStr = uniqueCreatures.showXml();
         int uniquecreaturesSize = len(uniquecreaturesStr);

         fwrite(&uniquecreaturesSize,sizeof(int),1,h);
         fwrite(uniquecreaturesStr.c_str(),uniquecreaturesSize,1,h);
         //fwrite(&uniqueCreatures,sizeof(UniqueCreatures),1,h);
      }

      //SQUADS
      dummy=len(squad);
      fwrite(&dummy,sizeof(int),1,h);
      for(int sq=0;sq<len(squad);sq++)
      {
         fwrite(squad[sq]->name,sizeof(char),SQUAD_NAMELEN,h);
         fwrite(&squad[sq]->activity,sizeof(activityst),1,h);
         fwrite(&squad[sq]->id,sizeof(int),1,h);

         for(int pos=0;pos<6;pos++)
         {
            dummy_b=squad[sq]->squad[pos];
            fwrite(&dummy_b,sizeof(bool),1,h);
            if(dummy_b)
               fwrite(&squad[sq]->squad[pos]->id,sizeof(int),1,h);
         }

         consolidateloot(squad[sq]->loot); // consolidate loot before saving
         dummy=len(squad[sq]->loot);
         fwrite(&dummy,sizeof(int),1,h);
         for(int l2=0;l2<len(squad[sq]->loot);l2++)
         {
            std::string itemStr = squad[sq]->loot[l2]->showXml();
            int itemSize = len(itemStr);

            fwrite(&itemSize,sizeof(int),1,h);
            fwrite(itemStr.c_str(),itemSize,1,h);
         }
      }

      dummy_b=activesquad;
      fwrite(&dummy_b,sizeof(bool),1,h);
      if(dummy_b)
         fwrite(&activesquad->id,sizeof(int),1,h);

      //DATES
      dummy=len(date);
      fwrite(&dummy,sizeof(int),1,h);
      for(int dt=0;dt<len(date);dt++)
      {
         fwrite(&date[dt]->mac_id,sizeof(long),1,h);
         fwrite(&date[dt]->timeleft,sizeof(short),1,h);
         fwrite(&date[dt]->city,sizeof(int),1,h);

         dummy=len(date[dt]->date);
         fwrite(&dummy,sizeof(int),1,h);
         for(int dt2=0;dt2<len(date[dt]->date);dt2++)
         {
            std::string creatureStr = date[dt]->date[dt2]->showXml();
            int creatureSize = len(creatureStr);

            fwrite(&creatureSize,sizeof(int),1,h);
            fwrite(creatureStr.c_str(),creatureSize,1,h);
            //fwrite(date[dt]->date[dt2],sizeof(Creature),1,h);
         }
      }

      //RECRUITS
      dummy=len(recruit);
      fwrite(&dummy,sizeof(int),1,h);
      for(int rt=0;rt<len(recruit);rt++)
      {
         fwrite(&recruit[rt]->recruiter_id,sizeof(long),1,h);
         fwrite(&recruit[rt]->timeleft,sizeof(short),1,h);
         fwrite(&recruit[rt]->level,sizeof(char),1,h);
         fwrite(&recruit[rt]->eagerness1,sizeof(char),1,h);
         fwrite(&recruit[rt]->task,sizeof(char),1,h);

         std::string creatureStr = recruit[rt]->recruit->showXml();
         int creatureSize = len(creatureStr);

         fwrite(&creatureSize,sizeof(int),1,h);
         fwrite(creatureStr.c_str(),creatureSize,1,h);
         //fwrite(recruit[rt]->recruit,sizeof(Creature),1,h);
      }

      //NEWS STORIES
      dummy=len(newsstory);
      fwrite(&dummy,sizeof(int),1,h);
      for(int ns=0;ns<len(newsstory);ns++)
      {
         fwrite(&newsstory[ns]->type,sizeof(short),1,h);
         fwrite(&newsstory[ns]->view,sizeof(short),1,h);

         fwrite(&newsstory[ns]->loc,sizeof(long),1,h);
         fwrite(&newsstory[ns]->priority,sizeof(long),1,h);
         fwrite(&newsstory[ns]->page,sizeof(long),1,h);
         fwrite(&newsstory[ns]->positive,sizeof(char),1,h);
         fwrite(&newsstory[ns]->siegetype,sizeof(short),1,h);

         dummy_b=newsstory[ns]->cr;
         fwrite(&dummy_b,sizeof(bool),1,h);
         if(dummy_b)
            fwrite(&newsstory[ns]->cr->id,sizeof(long),1,h);

         dummy=len(newsstory[ns]->crime);
         fwrite(&dummy,sizeof(int),1,h);
         for(int dt2=0;dt2<len(newsstory[ns]->crime);dt2++)
            fwrite(&newsstory[ns]->crime[dt2],sizeof(int),1,h);
      }

      // Liberal Media
      fwrite(public_interest,sizeof(public_interest),1,h);
      fwrite(background_liberal_influence,sizeof(background_liberal_influence),1,h);

      // Site mode options
      fwrite(&encounterwarnings,sizeof(bool),1,h);
      bool musicenabled=music.isEnabled();
      fwrite(&musicenabled,sizeof(bool),1,h);

      LCSCloseFile(h);
   }
}


/* Used by load() to create items of the correct class. */
Item* create_item(const std::string& inputXml)
{
   Item* it = NULL;
   CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   string itemclass = xml.GetTagName();
   if (itemclass == "clip")
      it = new Clip(inputXml);
   else if (itemclass == "weapon")
      it = new Weapon(inputXml);
   else if (itemclass == "armor")
      it = new Armor(inputXml);
   else if (itemclass == "loot")
      it = new Loot(inputXml);
   else if (itemclass == "money")
      it = new Money(inputXml);

   return it;
}

/* loads the game from save.dat */
char load(const string& filename)
{
   //LOAD FILE
   int loadversion;
   int l;
   bool dummy_b;
   int dummy;
   long dummy_l;
   FILE *h;

   h=LCSOpenFile(filename.c_str(), "rb", LCSIO_PRE_HOME);
   if(h!=NULL)
   {
      fread(&loadversion,sizeof(int),1,h);

      //NUKE INVALID SAVE GAMES
      if(loadversion<lowestloadversion)
      {
            LCSCloseFile(h);

            reset(savefile_name);

            return 0;
         }

         fread(seed,sizeof(unsigned long),RNG_SIZE,h);

         fread(&mode,sizeof(short),1,h);
         fread(&wincondition,sizeof(short),1,h);
         fread(&fieldskillrate,sizeof(short),1,h);

         fread(&day,sizeof(int),1,h);
         fread(&month,sizeof(int),1,h);
         fread(&year,sizeof(int),1,h);
         fread(&execterm,sizeof(short),1,h);
         fread(&presparty,sizeof(short),1,h);
         fread(&amendnum,sizeof(int),1,h);

         fread(&multipleCityMode,sizeof(bool),1,h);
         fread(&termlimits,sizeof(bool),1,h);
         fread(&deagle,sizeof(bool),1,h);
         fread(&m249,sizeof(bool),1,h);
         fread(&notermlimit,sizeof(bool),1,h);
         fread(&nocourtpurge,sizeof(bool),1,h);
         fread(&stalinmode,sizeof(bool),1,h);

         fread(&stat_recruits,sizeof(int),1,h);
         fread(&stat_dead,sizeof(int),1,h);
         fread(&stat_kills,sizeof(int),1,h);
         fread(&stat_kidnappings,sizeof(int),1,h);
         fread(&stat_buys,sizeof(int),1,h);
         fread(&stat_burns,sizeof(int),1,h);

         fread(&endgamestate,sizeof(char),1,h);
         fread(&ccsexposure,sizeof(char),1,h);
         fread(&ccs_kills,sizeof(char),1,h);

         fread(&Vehicle::curcarid,sizeof(long),1,h);
         fread(&curcreatureid,sizeof(long),1,h);
         fread(&cursquadid,sizeof(long),1,h);
         fread(&police_heat,sizeof(int),1,h);
         fread(&offended_corps,sizeof(short),1,h);
         fread(&offended_cia,sizeof(short),1,h);
         fread(&offended_amradio,sizeof(short),1,h);
         fread(&offended_cablenews,sizeof(short),1,h);
         fread(&offended_firemen,sizeof(short),1,h);
         fread(attorneyseed,sizeof(unsigned long),RNG_SIZE,h);
         //fread(&selectedsiege,sizeof(long),1,h);
         fread(lcityname,sizeof(char),CITY_NAMELEN,h);
         fread(&newscherrybusted,sizeof(char),1,h);

         fread(slogan,sizeof(char),SLOGAN_LEN,h);
         fread(&ledger,sizeof(class Ledger),1,h);
         fread(&party_status,sizeof(short),1,h);

         fread(attitude,sizeof(short),VIEWNUM,h);
         fread(law,sizeof(short),LAWNUM,h);
         fread(house,sizeof(short),HOUSENUM,h);
         fread(senate,sizeof(short),SENATENUM,h);
         fread(court,sizeof(short),COURTNUM,h);
         fread(courtname,sizeof(char)*POLITICIAN_NAMELEN,COURTNUM,h);
         fread(exec,sizeof(char),EXECNUM,h);
         fread(execname,sizeof(char)*POLITICIAN_NAMELEN,EXECNUM,h);
         fread(oldPresidentName,sizeof(char),POLITICIAN_NAMELEN,h);

         //LOCATIONS
         fread(&dummy,sizeof(int),1,h);
         location.resize(dummy);
         for(l=0;l<len(location);l++)
         {
            location[l]=new Location;

            fread(&dummy,sizeof(int),1,h);
            location[l]->loot.resize(dummy);
            for(int l2=0;l2<len(location[l]->loot);l2++)
            {
               int itemLen;
               fread(&itemLen, sizeof(int), 1, h);
               vector<char> vec = vector<char>(itemLen + 1);
               fread(&vec[0], itemLen, 1, h);
               vec[itemLen] = '\0';

               Item* it = create_item(&vec[0]);
               if(it!=NULL)
                  location[l]->loot[l2] = it;
            }
            //Remove items of unknown type.
            for(int l2=len(location[l]->loot)-1; l2>=0; l2--)
            {
               bool del = false;
               if(location[l]->loot[l2]->is_loot())
                  del = (getloottype(location[l]->loot[l2]->get_itemtypename()) == -1);
               else if(location[l]->loot[l2]->is_clip())
                  del = (getcliptype(location[l]->loot[l2]->get_itemtypename()) == -1);
               else if(location[l]->loot[l2]->is_weapon())
                  del = (getweapontype(location[l]->loot[l2]->get_itemtypename()) == -1);
               else if(location[l]->loot[l2]->is_armor())
                  del = (getarmortype(location[l]->loot[l2]->get_itemtypename()) == -1);

               if(del)
               {
                  addstr("Item type ");
                  addstr(location[l]->loot[l2]->get_itemtypename());
                  addstr(" does not exist. Deleting item.");
                  delete_and_remove(location[l]->loot,l2);
               }
            }
            consolidateloot(location[l]->loot); // consolidate loot after loading

            fread(&dummy,sizeof(int),1,h);
            location[l]->changes.resize(dummy);
            for(int l2=0;l2<len(location[l]->changes);l2++)
               fread(&location[l]->changes[l2],sizeof(sitechangest),1,h);

            fread(location[l]->name,sizeof(char),LOCATION_NAMELEN,h);
            fread(location[l]->shortname,sizeof(char),LOCATION_SHORTNAMELEN,h);
            fread(&location[l]->type,sizeof(char),1,h);
            fread(&location[l]->city,sizeof(int),1,h);
            fread(&location[l]->area,sizeof(int),1,h);
            fread(&location[l]->parent,sizeof(int),1,h);
            fread(&location[l]->id,sizeof(int),1,h);

            fread(&location[l]->renting,sizeof(int),1,h);
            fread(&location[l]->newrental,sizeof(char),1,h);
            fread(&location[l]->needcar,sizeof(char),1,h);
            fread(&location[l]->closed,sizeof(int),1,h);
            fread(&location[l]->hidden,sizeof(bool),1,h);
            fread(&location[l]->mapped,sizeof(bool),1,h);
            fread(&location[l]->upgradable,sizeof(bool),1,h);
            fread(&location[l]->highsecurity,sizeof(int),1,h);
            fread(&location[l]->siege,sizeof(siegest),1,h);
            fread(&location[l]->heat,sizeof(int),1,h);
            fread(&location[l]->heat_protection,sizeof(int),1,h);
            fread(&location[l]->compound_walls,sizeof(int),1,h);
            fread(&location[l]->compound_stores,sizeof(int),1,h);
            fread(&location[l]->front_business,sizeof(char),1,h);
            fread(location[l]->front_name,sizeof(char),LOCATION_NAMELEN,h);
            fread(location[l]->front_shortname,sizeof(char),LOCATION_SHORTNAMELEN,h);
            fread(&location[l]->haveflag,sizeof(bool),1,h);

            fread(location[l]->mapseed,sizeof(unsigned long),RNG_SIZE,h);
         }

         //VEHICLES
         fread(&dummy,sizeof(int),1,h);
         vehicle.resize(dummy);
         for(l=0;l<len(vehicle);l++)
         {
            int vehicleLen;
            fread (&vehicleLen, sizeof(int), 1, h);
            vector<char> vec = vector<char> (vehicleLen + 1);
            fread (&vec[0], vehicleLen, 1, h);
            vec[vehicleLen] = '\0';
            vehicle[l] = new Vehicle (&vec[0]);
         }

         //POOL
         fread(&dummy,sizeof(int),1,h);
         pool.resize(dummy);
         for(int pl=0;pl<len(pool);pl++)
         {
            int creatureLen;
            fread (&creatureLen, sizeof(int), 1, h);
            vector<char> vec = vector<char> (creatureLen + 1);
            fread (&vec[0], creatureLen, 1, h);
            vec[creatureLen] = '\0';
            pool[pl] = new Creature(&vec[0]);
            //pool[pl]=new Creature;
            //fread(pool[pl],sizeof(Creature),1,h);
            //read extra interrogation data if applicable
            if (pool[pl]->align == Alignment::CONSERVATIVE && pool[pl]->alive)
            {
               interrogation* &intr = pool[pl]->activity.intr();
               intr = new interrogation;
               fread(intr->techniques,sizeof(bool[6]),1,h);
               fread(&intr->druguse,sizeof(int),1,h);

               intr->rapport.clear();
               int size;
               fread(&size,sizeof(int),1,h);
               for(int i=0;i<size;i++)
               {
                  long id;
                  float_zero value;
                  fread(&id,sizeof(long),1,h);
                  fread(&value,sizeof(float_zero),1,h);
                  intr->rapport[id]=value;
               }
            }
            /*
            //read equipment
            vector<Item*> dump; //Used to catch invalid pointers from creature so they aren't deleted.
            pool[pl]->drop_weapon(&dump);
            pool[pl]->strip(&dump);
            pool[pl]->clips = deque<Clip*>();
            pool[pl]->extra_throwing_weapons = deque<Weapon*>();
            int itemLen;
            fread(&itemLen, sizeof(int), 1, h);
            if(itemLen != 0)
            {
               vector<char> vec = vector<char>(itemLen + 1);
               fread(&vec[0], itemLen, 1, h);
               vec[itemLen] = '\0';

               Weapon w(&vec[0]);
               if(getweapontype(w.get_itemtypename())!=-1) //Check it is a valid weapon type.
                  pool[pl]->give_weapon(w,&dump);
            }
            //pool[pl]->clips.clear();
            fread(&dummy,sizeof(int),1,h);
            for(int nc=0; nc<dummy; nc++)
            {
               fread(&itemLen, sizeof(itemLen), 1, h);
               vector<char> vec = vector<char>(itemLen + 1);
               fread(&vec[0], itemLen, 1, h);
               vec[itemLen] = '\0';

               Clip c(&vec[0]);
               if(getcliptype(c.get_itemtypename())!=-1) //Check it is a valid clip type.
                  pool[pl]->take_clips(c,len(c));
            }
            //pool[pl]->extra_throwing_weapons.clear();
            fread(&dummy,sizeof(int),1,h);
            for(int ne=0; ne<dummy; ne++)
            {
               fread(&itemLen, sizeof(itemLen), 1, h);
               vector<char> vec = vector<char>(itemLen + 1);
               fread(&vec[0], itemLen, 1, h);
               vec[itemLen] = '\0';

               Weapon w(&vec[0]);
               if(getweapontype(w.get_itemtypename())!=-1) //Check it is a valid weapon type.
                  pool[pl]->give_weapon(w,NULL);
            }
            fread(&itemLen, sizeof(itemLen), 1, h);
            if(itemLen != 0)
            {
               vector<char> vec = vector<char>(itemLen + 1);
               fread(&vec[0], itemLen, 1, h);
               vec[itemLen] = '\0';

               Armor a(&vec[0]);
               if(getarmortype(a.get_itemtypename())!=-1) //Check it is a valid armor type.
                  pool[pl]->give_armor(a,&dump);
            }*/
         }

         //Unique Creatures
         {
            int uniquecreaturesLen;
            fread (&uniquecreaturesLen, sizeof(int), 1, h);
            vector<char> vec = vector<char> (uniquecreaturesLen + 1);
            fread (&vec[0], uniquecreaturesLen, 1, h);
            vec[uniquecreaturesLen] = '\0';
            uniqueCreatures = UniqueCreatures(&vec[0]);
            //fread(&uniqueCreatures,sizeof(UniqueCreatures),1,h);
         }

         //SQUADS
         fread(&dummy,sizeof(int),1,h);
         squad.resize(dummy);
         for(int sq=0;sq<len(squad);sq++)
         {
            squad[sq]=new squadst;

            fread(squad[sq]->name,sizeof(char),SQUAD_NAMELEN,h);
            fread(&squad[sq]->activity,sizeof(activityst),1,h);
            fread(&squad[sq]->id,sizeof(int),1,h);

            for(int pos=0;pos<6;pos++)
            {
               //REBUILD SQUAD FROM POOL
               squad[sq]->squad[pos]=NULL;
               fread(&dummy_b,sizeof(bool),1,h);
               if(dummy_b)
               {
                  int dummy_i;
                  fread(&dummy_i,sizeof(int),1,h);
                  for(int pl=0;pl<len(pool);pl++)
                     if(pool[pl]->id==dummy_i)
                        squad[sq]->squad[pos]=pool[pl];
               }
            }

            fread(&dummy,sizeof(int),1,h);
            squad[sq]->loot.resize(dummy);
            for(int l2=0;l2<len(squad[sq]->loot);l2++)
            {
               int itemLen;
               fread(&itemLen, sizeof(int), 1, h);
               vector<char> vec = vector<char>(itemLen + 1);
               fread(&vec[0], itemLen, 1, h);
               vec[itemLen] = '\0';

               Item* it = create_item(&vec[0]);
               //if(it!=NULL) //Assume save file is correct? -XML
                  squad[sq]->loot[l2] = it;
               /*else
                  squad[sq]->loot.erase(loot.begin()+l2--);*/
            }
            //Remove items of unknown type.
            for(int l2=len(squad[sq]->loot)-1; l2>=0; l2--)
            {
               bool del = false;
               if(squad[sq]->loot[l2]->is_loot())
                  del = (getloottype(squad[sq]->loot[l2]->get_itemtypename()) == -1);
               else if(squad[sq]->loot[l2]->is_clip())
                  del = (getcliptype(squad[sq]->loot[l2]->get_itemtypename()) == -1);
               else if(squad[sq]->loot[l2]->is_weapon())
                  del = (getweapontype(squad[sq]->loot[l2]->get_itemtypename()) == -1);
               else if(squad[sq]->loot[l2]->is_armor())
                  del = (getarmortype(squad[sq]->loot[l2]->get_itemtypename()) == -1);

               if(del)
               {
                  addstr("Item type ");
                  addstr(squad[sq]->loot[l2]->get_itemtypename());
                  addstr(" does not exist. Deleting item.");
                  delete_and_remove(squad[sq]->loot,l2);
               }
            }
            consolidateloot(squad[sq]->loot); // consolidate loot after loading
         }

         activesquad=NULL;
         fread(&dummy_b,sizeof(bool),1,h);
         if(dummy_b)
         {
            int dummy_i;
            fread(&dummy_i,sizeof(int),1,h);
            for(int sq=0;sq<len(squad);sq++)
               if(squad[sq]->id==dummy_i)
               {
                  activesquad=squad[sq];
                  break;
               }
         }

         //DATES
         fread(&dummy,sizeof(int),1,h);
         date.resize(dummy);
         for(int dt=0;dt<len(date);dt++)
         {
            date[dt]=new datest;

            fread(&date[dt]->mac_id,sizeof(long),1,h);
            fread(&date[dt]->timeleft,sizeof(short),1,h);
            fread(&date[dt]->city,sizeof(int),1,h);

            fread(&dummy,sizeof(int),1,h);
            date[dt]->date.resize(dummy);
            for(int dt2=0;dt2<len(date[dt]->date);dt2++)
            {
               int creatureLen;
               fread (&creatureLen, sizeof(int), 1, h);
               vector<char> vec = vector<char> (creatureLen + 1);
               fread (&vec[0], creatureLen, 1, h);
               vec[creatureLen] = '\0';
               date[dt]->date[dt2] = new Creature(&vec[0]);

               //date[dt]->date[dt2]=new Creature;
               //fread(date[dt]->date[dt2],sizeof(Creature),1,h);
            }
         }

         //RECRUITS
         fread(&dummy,sizeof(int),1,h);
         recruit.resize(dummy);
         for(int rt=0;rt<len(recruit);rt++)
         {
            recruit[rt]=new recruitst;
            fread(&recruit[rt]->recruiter_id,sizeof(long),1,h);
            fread(&recruit[rt]->timeleft,sizeof(short),1,h);
            fread(&recruit[rt]->level,sizeof(char),1,h);
            fread(&recruit[rt]->eagerness1,sizeof(char),1,h);
            fread(&recruit[rt]->task,sizeof(char),1,h);

            int creatureLen;
            fread (&creatureLen, sizeof(int), 1, h);
            vector<char> vec = vector<char> (creatureLen + 1);
            fread (&vec[0], creatureLen, 1, h);
            vec[creatureLen] = '\0';
            recruit[rt]->recruit = new Creature(&vec[0]);
            //recruit[rt]->recruit = new Creature;
            //fread(recruit[rt]->recruit,sizeof(Creature),1,h);
         }

         //NEWS STORIES
         fread(&dummy,sizeof(int),1,h);
         newsstory.resize(dummy);
         for(int ns=0;ns<len(newsstory);ns++)
         {
            newsstory[ns]=new newsstoryst;

            fread(&newsstory[ns]->type,sizeof(short),1,h);
            fread(&newsstory[ns]->view,sizeof(short),1,h);

            fread(&newsstory[ns]->loc,sizeof(long),1,h);
            fread(&newsstory[ns]->priority,sizeof(long),1,h);
            fread(&newsstory[ns]->page,sizeof(long),1,h);
            fread(&newsstory[ns]->positive,sizeof(char),1,h);
            fread(&newsstory[ns]->siegetype,sizeof(short),1,h);

            newsstory[ns]->cr=NULL;
            fread(&dummy_b,sizeof(bool),1,h);
            if(dummy_b)
            {
               fread(&dummy_l,sizeof(long),1,h);
               for(int pl=0;pl<len(pool);pl++)
                  if(pool[pl]->id==dummy_l)
                  {
                     newsstory[ns]->cr=pool[pl];
                     break;
                  }
            }

            fread(&dummy,sizeof(int),1,h);
            newsstory[ns]->crime.resize(dummy);
            for(int dt2=0;dt2<len(newsstory[ns]->crime);dt2++)
               fread(&newsstory[ns]->crime[dt2],sizeof(int),1,h);
         }

         // Liberal Media
         fread(public_interest,sizeof(public_interest),1,h);
         fread(background_liberal_influence,sizeof(background_liberal_influence),1,h);

         // Site mode options
         fread(&encounterwarnings,sizeof(bool),1,h);

		 // Music resets to on every time the game is loaded
         /*bool musicenabled;
         fread(&musicenabled,sizeof(bool),1,h);
         music.enableIf(musicenabled);*/

         LCSCloseFile(h);

         // Check that vehicles are of existing types.
         for(int v=0;v<len(vehicle);v++)
         {
            if(getvehicletype(vehicle[v]->vtypeidname())==-1)
            { //Remove vehicle of non-existing type.
               addstr("Vehicle type "+vehicle[v]->vtypeidname()+" does not exist. Deleting vehicle.");
               delete_and_remove(vehicle,v--);
            }
         }

         return 1;
      }

   gamelog.log("Could not load");
   return 0;
}

/* deletes save.dat (used on endgame and for invalid save version) */
void reset(const string& filename)
{
	if(file_exists(filename)) LCSDeleteFile(filename.c_str(),LCSIO_PRE_HOME);
}

