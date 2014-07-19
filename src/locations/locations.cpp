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

#include "externs.h"

// Finds a location with the corresponding type and returns
// its index in the location array
int findlocation(int type, int city=-1)
{
   if(!multipleCityMode) city = -1;
   for(int i=0; i<(int)location.size(); i++)
   {
      Location& loc = *location[i];
      if(loc.type == type && (loc.city == city || city == -1))
         return i;
   }
   return -1;
}

int findlocation_id(int id)
{
   for(int i=0; i<(int)location.size(); i++)
   {
      if(location[i]->id == id)
         return i;
   }
   return -1;
}

// Locations - Construct a new location with the specified parameters
Location::Location(int type_, int parent_)
{
   this->type = type_;
   this->needcar=false;
   this->hidden=false;
   this->upgradable=false;
   this->renting=RENTING_NOCONTROL;
   this->city = -1;
   if(parent_==-1)
      this->parent = -1;
   else
   {
      this->parent = parent_;
      this->needcar = location[this->parent]->needcar;
      this->mapped = location[this->parent]->mapped;
      this->area = location[this->parent]->area;
      this->city = location[this->parent]->city;
   }
   if(this->city < 0 && multipleCityMode)
   {
      this->city = this->type;
   }
   initlocation(*this);
}

Location* Location::addchild(int type_)
{
   Location *newloc = new Location(type_, findlocation(this->type, this->city));
   location.push_back(newloc);
   return newloc;
}

void Location::init()
{
   haveflag=0;
   newrental=0;
   heat=0;
   heat_protection=0;
   closed=0;
   mapped=0;
   highsecurity=0;
   mapseed=seed;r_num();
   changes.clear();
   compound_walls=0;
   compound_stores=0;
   front_business=-1;
}

bool Location::is_city()
{
   if(type == city) return true;
   else return false;
}

/* Settings for shortname (true is 1 and false is 0, by the way):
 * -1: entire name is long, no matter what
 *  0: first part of place name is long, and if there's a city at the end it's short
 *  1: first part of the name is short unless the place itself is a city in which case it's long, and if there's a city at the end it's short
 *  2: entire name is short, no matter what */
std::string Location::getname(int shortname, bool include_city)
{
   std::string str;
   if(!multipleCityMode) include_city = false;

   if((shortname >=1 && type != city) || shortname>=2) {
      if(this->front_business != -1)
         str = this->front_shortname;
      else
         str = this->shortname;
   } else {
      if(this->front_business != -1)
         str = this->front_name;
      else
         str = this->name;
   }

   if(include_city && type != city) {
      std::string cityname = location[findlocation(city, city)]->getname(shortname+2);
      if(str == "Downtown")
         return str + " " + cityname;
      if(str == "University District" || str == "U-District" || str == "Industrial District" || str == "I-District" ||
         str == "Shopping" || str == "Outskirts" || str == "Seaport Area" || str == "Seaport" || str == "Outskirts & Orange County")
         return cityname + " " + str;
      if(str == "City Outskirts")
         return cityname + " Outskirts";
      if(str == "Arlington")
         return str + (shortname<0?", Virginia":", VA");
      if(str == "Hollywood" || str == "Greater Hollywood")
         return str + (shortname<0?", California":", CA");
      if(str == "Manhattan" || str == "Manhattan Island" || str == "Brooklyn & Queens" || str == "Long Island" || str == "The Bronx")
         return str + (shortname<0?", New York":", NY");
      str += ", " + cityname;
   }
   return str;
}

char* Location::city_description()
{
   switch(type)
   {
   case SITE_CITY_SEATTLE: return (char*)"Birthplace of the LCS.";
   case SITE_CITY_LOS_ANGELES: return (char*)"Hollywood and Trade.";
   case SITE_CITY_NEW_YORK: return (char*)"Wall Street and Big Media.";
   case SITE_CITY_WASHINGTON_DC: return (char*)"The Nation's Capital.";
   case SITE_CITY_CHICAGO:
   case SITE_CITY_DETROIT:
   case SITE_CITY_ATLANTA:
   case SITE_CITY_MIAMI:
   default: return (char*)"";
   }
}

bool Location::can_be_upgraded()
{
   if(upgradable)
   {
      return true;
   }
   else return false;
}

bool Location::can_be_fortified()
{
   if(!upgradable) return false;
   switch(type)
   {
   default:
      return !fortified();
   case SITE_OUTDOOR_BUNKER:
   case SITE_BUSINESS_BARANDGRILL:
      return false;
   }
}

bool Location::fortified()
{
   switch(type)
   {
   default:
      return compound_walls & COMPOUND_BASIC;
   case SITE_OUTDOOR_BUNKER:
      return true;
   }
}

bool Location::can_be_trapped()
{
   if(!upgradable) return false;
   return !trapped();
}

bool Location::trapped()
{
   return compound_walls & COMPOUND_TRAPS;
}

bool Location::can_install_tanktraps()
{
   if(!upgradable) return false;
   switch(type)
   {
   default:
      return !tank_traps();
   case SITE_BUSINESS_BARANDGRILL:
   case SITE_OUTDOOR_BUNKER:
   case SITE_RESIDENTIAL_BOMBSHELTER:
      return false;
   }
}

bool Location::tank_traps()
{
   switch(type)
   {
   default:
      return compound_walls & COMPOUND_TANKTRAPS;
   case SITE_OUTDOOR_BUNKER:
   case SITE_RESIDENTIAL_BOMBSHELTER:
      return true;
   }
}

bool Location::can_have_businessfront()
{
   if(!upgradable) return false;
   switch(type)
   {
   default:
      return !has_business_front();
   case SITE_BUSINESS_BARANDGRILL:
   case SITE_OUTDOOR_BUNKER:
   case SITE_RESIDENTIAL_BOMBSHELTER:
      return false;
   }
}

bool Location::has_business_front()
{
   switch(type)
   {
   default:
      if(front_business == -1) return false;
      else return true;
   case SITE_BUSINESS_BARANDGRILL:
      return true;
   }
}

bool Location::bomb_resistant()
{
   switch(type)
   {
   default:
      return false;
   case SITE_RESIDENTIAL_BOMBSHELTER:
      return true;
   }
}

bool Location::part_of_justice_system()
{
   if(type==SITE_GOVERNMENT_POLICESTATION ||
      type==SITE_GOVERNMENT_COURTHOUSE ||
      type==SITE_GOVERNMENT_PRISON)
   {
      return true;
   }
   else return false;
}

bool Location::is_lcs_safehouse() { return renting >= 0; }
bool Location::is_ccs_safehouse() { return renting == RENTING_CCS; }

bool Location::duplicatelocation() {
   for(int l = 0; l < (int)location.size(); l++)
   {
      if(location[l] == this)
         continue;

      if(type!=SITE_RESIDENTIAL_SHELTER && !strcmp(location[l]->name, this->name))
         return true;

      if (location[l]->front_business != -1 &&
            this->front_business != -1 &&
            !strcmp (location[l]->front_shortname, this->front_shortname))
         return true;
   }
   return 0;
}

void Location::update_heat_protection()
{
   int l;
   for(l=0;l<(int)location.size();l++)
   {
      if(location[l]==this)
         break;
   }
   if(l==(int)location.size())
   {
      heat_protection=0;
      return;
   }
   int numpres=0;
   int heatprotection=0;
   for(int p=0;p<(int)pool.size();p++)
   {
      if(pool[p]->location!=l)continue; // People not at this base don't count
      if(!pool[p]->alive)continue; // Dead people don't count
      numpres++;
   }

   // Determine how effective your current safehouse
   // is at keeping the police confused
   switch(location[l]->type)
   {
   case SITE_INDUSTRY_WAREHOUSE:
      if(location[l]->front_business!=-1)
         heatprotection+=12; // Business front -- high protection
      else
         heatprotection+=0; // Abandoned warehouse -- no protection
      break;
   case SITE_RESIDENTIAL_SHELTER:
      heatprotection+=0; // Homeless shelter -- no protection
      break;
   case SITE_RESIDENTIAL_TENEMENT:
      heatprotection+=4; // Lower class housing -- low protection
      break;
   case SITE_RESIDENTIAL_APARTMENT:
      heatprotection+=8; // Middle class housing -- medium protection
      break;
   case SITE_RESIDENTIAL_BOMBSHELTER:
   case SITE_OUTDOOR_BUNKER:
   case SITE_BUSINESS_BARANDGRILL:
   case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
      heatprotection+=12; // Upper class housing -- high protection
      break;
   }

   if(law[LAW_FLAGBURNING]==-2&&location[l]->haveflag) {heatprotection+=6;} // More protection if the flag is sacred
   else if(law[LAW_FLAGBURNING]!=-2&&location[l]->haveflag) {heatprotection+=2;} // Some if the flag isn't
   else if(law[LAW_FLAGBURNING]==-2&&!(location[l]->haveflag)) {heatprotection-=2;} // Lose some if it is and you have no flag
   else {heatprotection+=0;} // None if it isn't and you have no flag

   //Protection varies with how many people in the safehouse
   if(numpres>60)heatprotection-=20;
   if(numpres>40)heatprotection-=12;
   if(numpres>20)heatprotection-=6;
   if(numpres<10)heatprotection+=1;
   if(numpres<4)heatprotection+=2;
   if(numpres<2)heatprotection+=3;

   if(heatprotection<0)heatprotection=0;

   heat_protection=heatprotection*5;
   if(heat_protection>=100)
      heat_protection=95;
}

void Location::rename(const char* name_, const char* shortname_)
{
   strcpy(this->name, name_);
   strcpy(this->shortname, shortname_);
}

/* daily - seeds and names a site (will re-seed and rename if used after start) */
void initlocation(Location &loc)
{
   loc.init();

   char str[80];

   switch(loc.type)
   {
   case SITE_CITY_SEATTLE: loc.rename("Seattle", "SEA"); break;
   case SITE_CITY_LOS_ANGELES: loc.rename("Los Angeles", "LA"); break;
   case SITE_CITY_NEW_YORK: loc.rename("New York", "NYC"); break;
   case SITE_CITY_CHICAGO: loc.rename("Chicago", "CHI"); break;
   case SITE_CITY_DETROIT: loc.rename("Detroit", "DET"); break;
   case SITE_CITY_ATLANTA: loc.rename("Atlanta", "ATL"); break;
   case SITE_CITY_MIAMI: loc.rename("Miami", "MI"); break;
   case SITE_CITY_WASHINGTON_DC: loc.rename("Washington, DC", "DC"); break;
   case SITE_DOWNTOWN: loc.rename("Downtown", "Downtown"); break;
   case SITE_UDISTRICT: loc.rename("University District", "U-District"); break;
   case SITE_COMMERCIAL: loc.rename("Shopping", "Shopping"); break;
   case SITE_INDUSTRIAL: loc.rename("Industrial District", "I-District"); break;
   case SITE_OUTOFTOWN: loc.rename("City Outskirts", "Outskirts"); break;
   case SITE_TRAVEL: loc.rename("Travel", "Travel"); break;
   case SITE_GOVERNMENT_POLICESTATION: loc.rename("Police Station", "Police Station"); break;
   case SITE_GOVERNMENT_COURTHOUSE:
      if(law[LAW_DEATHPENALTY]==-2) {
         loc.rename("Halls of Ultimate Judgment", "Judge Hall");
      } else {
         loc.rename("Courthouse", "Courthouse");
      } break;
   case SITE_GOVERNMENT_FIRESTATION:
      if(law[LAW_FREESPEECH]==-2) {
         loc.rename("Fireman HQ", "Fireman HQ");
         loc.hidden = false;
      } else {
         loc.rename("Fire Station", "Fire Station");
         loc.hidden = true;
      } break;
   case SITE_GOVERNMENT_PRISON:
      if(law[LAW_PRISONS]==-2)
      {
         switch(LCSrandom(5))
         {
            case 0:strcpy(loc.name,"Happy");break;
            case 1:strcpy(loc.name,"Cheery");break;
            case 2:strcpy(loc.name,"Quiet");break;
            case 3:strcpy(loc.name,"Green");break;
            case 4:strcpy(loc.name,"Nectar");break;
         }
         strcat(loc.name," ");
         switch(LCSrandom(5))
         {
            case 0:strcat(loc.name,"Valley");break;
            case 1:strcat(loc.name,"Meadow");break;
            case 2:strcat(loc.name,"Hills");break;
            case 3:strcat(loc.name,"Glade");break;
            case 4:strcat(loc.name,"Forest");break;
         }
         strcat(loc.name," Forced Labor Camp");
         strcpy(loc.shortname,"Labor Camp");
      }
      else
      {
         lastname(loc.name,true);
         strcat(loc.name," Prison");
         strcpy(loc.shortname,"Prison");
      }
      break;
   case SITE_INDUSTRY_NUCLEAR:
      if(law[LAW_NUCLEARPOWER]==2) {
         loc.rename("Nuclear Waste Center", "NWaste Center");
      } else {
         loc.rename("Nuclear Power Plant", "NPower Plant");
      } break;
   case SITE_GOVERNMENT_INTELLIGENCEHQ:
      if(law[LAW_PRIVACY]==-2 && law[LAW_POLICEBEHAVIOR]==-2) {
         loc.rename("Ministry of Love", "Min. Love");
      } else {
         loc.rename("Intelligence HQ", "Int. HQ");
      } break;
   case SITE_GOVERNMENT_ARMYBASE:
      lastname(loc.name,true);
      strcat(loc.name," Army Base");
      strcpy(loc.shortname,"Army Base");
      break;
   case SITE_GOVERNMENT_WHITE_HOUSE:
      loc.rename("White House", "White House");
      break;
   case SITE_CORPORATE_HEADQUARTERS:
      loc.rename("Corporate HQ","Corp. HQ");
      break;
   case SITE_BUSINESS_BANK:
      loc.rename("American Bank Corp", "Bank");
      break;
   case SITE_BUSINESS_PAWNSHOP:
      lastname(loc.name,true);
      if(law[LAW_GUNCONTROL]==ALIGN_ELITELIBERAL)
         strcat(loc.name,"'s Pawnshop");
      else
         strcat(loc.name," Pawn & Gun");
      strcpy(loc.shortname,"Pawnshop");
      break;
   case SITE_CORPORATE_HOUSE:
      if(law[LAW_CORPORATE]==-2&&law[LAW_TAX]==-2)
         loc.rename("CEO Castle", "CEO Castle");
      else
         loc.rename("CEO Residence", "CEO House");
      break;
   case SITE_RESIDENTIAL_SHELTER:
      loc.rename("Homeless Shelter", "Shelter");
      break;
   case SITE_INDUSTRY_WAREHOUSE:
      do {
         strcpy(loc.name,"Abandoned ");
         /*lastname(loc.name,true);
         strcat(loc.name," ");*/

         switch(LCSrandom(10))
         {
         case 0:
            strcat(loc.name,"Meat Plant");
            strcpy(loc.shortname,"Meat Plant");
            break;
         case 1:
            strcat(loc.name,"Warehouse");
            strcpy(loc.shortname,"Warehouse");
            break;
         case 2:
            strcat(loc.name,"Paper Mill");
            strcpy(loc.shortname,"Paper Mill");
            break;
         case 3:
            strcat(loc.name,"Cement Factory");
            strcpy(loc.shortname,"Cement");
            break;
         case 4:
            strcat(loc.name,"Fertilizer Plant");
            strcpy(loc.shortname,"Fertilizer");
            break;
         case 5:
            strcat(loc.name,"Drill Factory");
            strcpy(loc.shortname,"Drill");
            break;
         case 6:
            strcat(loc.name,"Steel Plant");
            strcpy(loc.shortname,"Steel");
            break;
         case 7:
            strcat(loc.name,"Packing Plant");
            strcpy(loc.shortname,"Packing");
            break;
         case 8:
            strcat(loc.name,"Toy Factory");
            strcpy(loc.shortname,"Toy");
            break;
         case 9:
            strcat(loc.name,"Building Site");
            strcpy(loc.shortname,"Building");
            break;
         }
      } while (loc.duplicatelocation());
      break;
   case SITE_INDUSTRY_POLLUTER:
      switch(LCSrandom(5))
      {
      case 0:
         loc.rename("Aluminum Factory", "Alum Fact");
         break;
      case 1:
         loc.rename("Plastic Factory", "Plast Fact");
         break;
      case 2:
         loc.rename("Oil Refinery", "Refinery");
         break;
      case 3:
         loc.rename("Auto Plant", "Auto Plant");
         break;
      case 4:
         loc.rename("Chemical Factory", "Chem Fact");
         break;
      }
      break;
   case SITE_MEDIA_CABLENEWS:
      loc.rename("Cable News Station", "News Station");
      break;
   case SITE_MEDIA_AMRADIO:
      loc.rename("AM Radio Station", "Radio Station");
      break;
   case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
      do {
         lastname(loc.name,true);
         strcpy(loc.shortname,loc.name);
         strcat(loc.name," Condominiums");
         strcat(loc.shortname," Condos");
      } while (loc.duplicatelocation());
      break;
   case SITE_RESIDENTIAL_APARTMENT:
      do {
         lastname(loc.name,true);
         strcpy(loc.shortname,loc.name);
         strcat(loc.name," Apartments");
         strcat(loc.shortname," Apts");
      } while (loc.duplicatelocation());
      break;
   case SITE_RESIDENTIAL_TENEMENT:
      do {
         do {
            lastname(loc.name,true);
         } while(strlen(loc.name)>7);
         strcat(loc.name," St. Housing Projects");
         strcpy (loc.shortname, "Projects");
      } while (loc.duplicatelocation());
      break;
   case SITE_HOSPITAL_UNIVERSITY:
      loc.rename("The University Hospital", "U Hospital");
      break;
   case SITE_HOSPITAL_CLINIC:
      loc.rename("The Free Clinic", "Clinic");
      break;
   case SITE_LABORATORY_GENETIC:
      lastname(loc.name,true);
      strcat(loc.name," Genetics");
      strcpy(loc.shortname,"Genetics Lab");
      break;
   case SITE_LABORATORY_COSMETICS:
      lastname(loc.name,true);
      strcat(loc.name," Cosmetics");
      strcpy(loc.shortname,"Cosmetics Lab");
      break;
   case SITE_BUSINESS_CARDEALERSHIP:
      generate_name(loc.name,GENDER_WHITEMALEPATRIARCH);
      strcat(loc.name,"'s Used Cars");
      strcpy(loc.shortname,"Used Cars");
      break;
   case SITE_BUSINESS_DEPTSTORE:
      lastname(loc.name,true);
      strcat(loc.name,"'s Department Store");
      strcpy(loc.shortname,"Dept. Store");
      break;
   case SITE_BUSINESS_HALLOWEEN:
      loc.rename("The Oubliette", "Oubliette");
      break;
   case SITE_INDUSTRY_SWEATSHOP:
      lastname(loc.name,true);
      strcat(loc.name," Garment Makers");
      strcpy(loc.shortname,"Sweatshop");
      break;
   case SITE_BUSINESS_CRACKHOUSE:
   do {
      lastname(loc.name,true);
      strcat(loc.name," St. ");

      if (law[LAW_DRUGS]==2)
      {
         switch (LCSrandom (4))
         {
         case 0:
            strcat (loc.name, "Recreational Drugs Center");
            strcpy (loc.shortname, "Drugs Center");
            break;
         case 1:
            strcat (loc.name, "Coffee House");
            strcpy (loc.shortname, "Coffee House");
            break;
         case 2:
            strcat (loc.name, "Cannabis Lounge");
            strcpy (loc.shortname, "Cannabis Lounge");
            break;
         case 3:
            strcat (loc.name, "Marijuana Dispensary");
            strcpy (loc.shortname, "Dispensary");
            break;
         }
      }
      else
      {
         strcat (loc.name, "Crack House");
         strcpy (loc.shortname, "Crack House");
      }
   } while (loc.duplicatelocation());
   break;

   case SITE_BUSINESS_JUICEBAR:
      strcpy(loc.name,"");
      switch(LCSrandom(5))
      {
         case 0:strcat(loc.name,"Natural");break;
         case 1:strcat(loc.name,"Harmonious");break;
         case 2:strcat(loc.name,"Restful");break;
         case 3:strcat(loc.name,"Healthy");break;
         case 4:strcat(loc.name,"New You");break;
      }
      strcat(loc.name," ");
      switch(LCSrandom(5))
      {
         case 0:strcat(loc.name,"Diet");break;
         case 1:strcat(loc.name,"Methods");break;
         case 2:strcat(loc.name,"Plan");break;
         case 3:strcat(loc.name,"Orange");break;
         case 4:strcat(loc.name,"Carrot");break;
      }
      strcat(loc.name," Juice Bar");
      strcpy(loc.shortname,"Juice Bar");
      break;
   case SITE_BUSINESS_VEGANCOOP:
      strcpy(loc.name,"");
      switch(LCSrandom(5))
      {
         case 0:strcat(loc.name,"Asparagus");break;
         case 1:strcat(loc.name,"Tofu");break;
         case 2:strcat(loc.name,"Broccoli");break;
         case 3:strcat(loc.name,"Radish");break;
         case 4:strcat(loc.name,"Eggplant");break;
      }
      strcat(loc.name," ");
      switch(LCSrandom(5))
      {
         case 0:strcat(loc.name,"Forest");break;
         case 1:strcat(loc.name,"Rainbow");break;
         case 2:strcat(loc.name,"Garden");break;
         case 3:strcat(loc.name,"Farm");break;
         case 4:strcat(loc.name,"Meadow");break;
      }
      strcat(loc.name," Vegan Co-op");
      strcpy(loc.shortname,"Vegan Co-op");
      break;
   case SITE_BUSINESS_INTERNETCAFE:
      strcpy(loc.name,"");
      switch(LCSrandom(5))
      {
         case 0:strcat(loc.name,"Electric");break;
         case 1:strcat(loc.name,"Wired");break;
         case 2:strcat(loc.name,"Nano");break;
         case 3:strcat(loc.name,"Micro");break;
         case 4:strcat(loc.name,"Techno");break;
      }
      strcat(loc.name," ");
      switch(LCSrandom(5))
      {
         case 0:strcat(loc.name,"Panda");break;
         case 1:strcat(loc.name,"Troll");break;
         case 2:strcat(loc.name,"Latte");break;
         case 3:strcat(loc.name,"Unicorn");break;
         case 4:strcat(loc.name,"Pixie");break;
      }
      strcat(loc.name," Internet Cafe");
      strcpy(loc.shortname,"Net Cafe");
      break;
   case SITE_BUSINESS_CIGARBAR:
      lastname(str,true);
      strcpy(loc.name,"The ");
      strcat(loc.name,str);
      strcat(loc.name," Gentlemen's Club");
      strcpy(loc.shortname,"Cigar Bar");
      break;
   case SITE_BUSINESS_LATTESTAND:
      strcpy(loc.name,"");
      switch(LCSrandom(5))
      {
         case 0:strcat(loc.name,"Frothy");break;
         case 1:strcat(loc.name,"Milky");break;
         case 2:strcat(loc.name,"Caffeine");break;
         case 3:strcat(loc.name,"Morning");break;
         case 4:strcat(loc.name,"Evening");break;
      }
      strcat(loc.name," ");
      switch(LCSrandom(5))
      {
         case 0:strcat(loc.name,"Mug");break;
         case 1:strcat(loc.name,"Cup");break;
         case 2:strcat(loc.name,"Jolt");break;
         case 3:strcat(loc.name,"Wonder");break;
         case 4:strcat(loc.name,"Express");break;
      }
      strcat(loc.name," Latte Stand");
      strcpy(loc.shortname,"Latte Stand");
      break;
   case SITE_OUTDOOR_PUBLICPARK:
      lastname(loc.name,true);
      strcat(loc.name," Park");
      strcpy(loc.shortname,"Park");
      break;
   case SITE_RESIDENTIAL_BOMBSHELTER:
      loc.rename("Fallout Shelter", "Bomb Shelter");
      break;
   case SITE_BUSINESS_BARANDGRILL:
      loc.rename("Desert Eagle Bar & Grill", "Bar & Grill");
      break;
   case SITE_OUTDOOR_BUNKER:
      loc.rename("Robert E. Lee Bunker", "Bunker");
      break;
   case SITE_BUSINESS_ARMSDEALER:
      loc.rename("Black Market", "Black Market");
      break;
   }
}

/* transfer all loot from some source (such as a squad or another location) to a location, and deal with money properly */
void Location::getloot(vector<Item *>& loot)
{
   for(int l=loot.size()-1;l>=0;l--)
      if(loot[l]->is_money())
      {
         Money* m = static_cast<Money*>(loot[l]); //cast -XML
         ledger.add_funds(m->get_amount(),INCOME_THIEVERY);
         delete loot[l];
      }
      else
      {  // Empty squad inventory into base inventory
         this->loot.push_back(loot[l]);
      }
   loot.clear();
}
