/**
 * Interface for the Locations class.
 */
/*
 * Copyright 2009, 2013 Jonathan Stickles  <jonathansfox@users.sourceforge.net>
 * Copyright 2014 Carlos Gustavos  <blomkvist>
 * Copyright 2013, 2014 Rich McGrew (yetisyny)
 * Copyright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#ifndef LOCATIONS_H_INCLUDED
#define LOCATIONS_H_INCLUDED

#include "common.h"

enum SiteTypes
{
  SITE_CITY_SEATTLE, // first are the cities
  SITE_CITY_LOS_ANGELES,
  SITE_CITY_NEW_YORK,
  SITE_CITY_CHICAGO,
  SITE_CITY_DETROIT,
  SITE_CITY_ATLANTA,
  SITE_CITY_MIAMI,
  SITE_CITY_WASHINGTON_DC,

  SITE_DOWNTOWN, // then are the districts
  SITE_COMMERCIAL,
  SITE_UDISTRICT,
  SITE_OUTOFTOWN,
  SITE_INDUSTRIAL,
  SITE_TRAVEL,

  SITE_HOSPITAL_CLINIC, // then come sites which cannot be mapped
  SITE_HOSPITAL_UNIVERSITY,
  SITE_BUSINESS_PAWNSHOP,
  SITE_BUSINESS_DEPTSTORE,
  SITE_BUSINESS_HALLOWEEN,
  SITE_BUSINESS_ARMSDEALER,
  SITE_BUSINESS_CARDEALERSHIP,

  SITE_RESIDENTIAL_SHELTER, // this has to be in this spot - sites before this cannot be mapped and
                            // sites after this can be mapped

  SITE_RESIDENTIAL_TENEMENT, // sites starting at this point CAN be mapped
  SITE_RESIDENTIAL_APARTMENT,
  SITE_RESIDENTIAL_APARTMENT_UPSCALE,
  SITE_RESIDENTIAL_BOMBSHELTER,
  SITE_LABORATORY_COSMETICS,
  SITE_LABORATORY_GENETIC,
  SITE_GOVERNMENT_POLICESTATION,
  SITE_GOVERNMENT_COURTHOUSE,
  SITE_GOVERNMENT_PRISON,
  SITE_GOVERNMENT_INTELLIGENCEHQ,
  SITE_GOVERNMENT_FIRESTATION,
  SITE_INDUSTRY_SWEATSHOP,
  SITE_INDUSTRY_POLLUTER,
  SITE_INDUSTRY_NUCLEAR,
  SITE_INDUSTRY_WAREHOUSE,
  SITE_CORPORATE_HEADQUARTERS,
  SITE_CORPORATE_HOUSE,
  SITE_MEDIA_AMRADIO,
  SITE_MEDIA_CABLENEWS,
  SITE_BUSINESS_CRACKHOUSE,
  SITE_BUSINESS_JUICEBAR,
  SITE_BUSINESS_CIGARBAR,
  SITE_BUSINESS_LATTESTAND,
  SITE_BUSINESS_VEGANCOOP,
  SITE_BUSINESS_INTERNETCAFE,
  SITE_BUSINESS_BARANDGRILL,
  SITE_OUTDOOR_PUBLICPARK,
  SITE_OUTDOOR_BUNKER,
  SITE_GOVERNMENT_ARMYBASE,
  SITE_BUSINESS_BANK,
  SITE_GOVERNMENT_LIBERAL_PARTY_HQ,
  SITE_GOVERNMENT_WHITE_HOUSE,

  SITENUM // this comes last
};

#define SITEBLOCK_EXIT BIT1
#define SITEBLOCK_BLOCK BIT2
#define SITEBLOCK_DOOR BIT3
#define SITEBLOCK_KNOWN BIT4
#define SITEBLOCK_LOOT BIT5
#define SITEBLOCK_LOCKED BIT6
#define SITEBLOCK_KLOCK BIT7
#define SITEBLOCK_CLOCK BIT8
#define SITEBLOCK_RESTRICTED BIT9
#define SITEBLOCK_BLOODY BIT10
#define SITEBLOCK_BLOODY2 BIT11
#define SITEBLOCK_GRASSY BIT12
#define SITEBLOCK_OUTDOOR BIT13
#define SITEBLOCK_DEBRIS BIT14
#define SITEBLOCK_GRAFFITI BIT15
#define SITEBLOCK_GRAFFITI_CCS BIT16
#define SITEBLOCK_GRAFFITI_OTHER BIT17
#define SITEBLOCK_FIRE_START BIT18
#define SITEBLOCK_FIRE_PEAK BIT19
#define SITEBLOCK_FIRE_END BIT20
#define SITEBLOCK_CHAINLINK BIT21
#define SITEBLOCK_ALARMED BIT22
#define SITEBLOCK_METAL BIT23

enum SpecialBlocks
{
  SPECIAL_NONE = -1,
  SPECIAL_LAB_COSMETICS_CAGEDANIMALS,
  SPECIAL_LAB_GENETIC_CAGEDANIMALS,
  SPECIAL_POLICESTATION_LOCKUP,
  SPECIAL_COURTHOUSE_LOCKUP,
  SPECIAL_COURTHOUSE_JURYROOM,
  SPECIAL_PRISON_CONTROL,
  SPECIAL_PRISON_CONTROL_LOW,
  SPECIAL_PRISON_CONTROL_MEDIUM,
  SPECIAL_PRISON_CONTROL_HIGH,
  SPECIAL_INTEL_SUPERCOMPUTER,
  SPECIAL_SWEATSHOP_EQUIPMENT,
  SPECIAL_POLLUTER_EQUIPMENT,
  SPECIAL_NUCLEAR_ONOFF,
  SPECIAL_HOUSE_PHOTOS,
  SPECIAL_HOUSE_CEO,
  SPECIAL_CORPORATE_FILES,
  SPECIAL_RADIO_BROADCASTSTUDIO,
  SPECIAL_NEWS_BROADCASTSTUDIO,
  SPECIAL_APARTMENT_LANDLORD,
  SPECIAL_SIGN_ONE,
  SPECIAL_RESTAURANT_TABLE,
  SPECIAL_CAFE_COMPUTER,
  SPECIAL_PARK_BENCH,
  SPECIAL_STAIRS_UP,
  SPECIAL_STAIRS_DOWN,
  SPECIAL_CLUB_BOUNCER,
  SPECIAL_CLUB_BOUNCER_SECONDVISIT,
  SPECIAL_ARMORY,
  SPECIAL_DISPLAY_CASE,
  SPECIAL_SIGN_TWO,
  SPECIAL_SIGN_THREE,
  SPECIAL_SECURITY_CHECKPOINT,
  SPECIAL_SECURITY_METALDETECTORS,
  SPECIAL_SECURITY_SECONDVISIT,
  SPECIAL_BANK_VAULT,
  SPECIAL_BANK_TELLER,
  SPECIAL_BANK_MONEY,
  SPECIAL_CCS_BOSS,
  SPECIAL_OVAL_OFFICE_NW,
  SPECIAL_OVAL_OFFICE_NE,
  SPECIAL_OVAL_OFFICE_SW,
  SPECIAL_OVAL_OFFICE_SE,
  SPECIALNUM
};

#define SIEGEFLAG_UNIT BIT1
#define SIEGEFLAG_TRAP BIT2
#define SIEGEFLAG_HEAVYUNIT BIT3
#define SIEGEFLAG_UNIT_DAMAGED BIT4

enum SiegeTypes
{
  SIEGE_POLICE,
  SIEGE_CIA,
  SIEGE_HICKS,
  SIEGE_CORPORATE,
  SIEGE_CCS,
  SIEGE_FIREMEN,
  SIEGE_ORG,
  SIEGENUM
};

struct siegest
{
  char siege{0};
  // Puzz:  Temporary siege thing for organizations
  int   orgID;
  short siegetype{-1};
  char  underattack{0};
  int   attacktime;
  short kills;
  short tanks;
  short escalationstate{0};
  char  lights_off;
  char  cameras_off;
  short timeuntillocated{-1};
  short timeuntilcorps{-1};
  short timeuntilcia{-1};
  short timeuntilccs{-1};
  short timeuntilfiremen{-1};
};

struct siteblockst
{
  short special;
  int   flag;
  char  siegeflag;
};

struct sitechangest
{
  sitechangest()
  {}

  sitechangest(char x_, char y_, char z_, int flag_)
  : x(x_), y(y_), z(z_), flag(flag_)
  {}

  char x, y, z;
  int  flag;
};

#define MAPX 70
#define MAPY 23
#define MAPZ 10

#define COMPOUND_BASIC BIT1
#define COMPOUND_CAMERAS BIT2
#define COMPOUND_TANKTRAPS BIT3
#define COMPOUND_TRAPS BIT4
#define COMPOUND_GENERATOR BIT5
#define COMPOUND_PRINTINGPRESS BIT6
#define COMPOUND_AAGUN BIT7

enum RentingTypes
{
  RENTING_CCS = -2,
  RENTING_NOCONTROL = -1,
  RENTING_PERMANENT = 0
  // positive values of renting are considered monthly rent prices
};


#define CITY_NAMELEN 80
#define LOCATION_NAMELEN 40
#define LOCATION_SHORTNAMELEN 20

class Item;


class Location
{
public:
  Location(char type_, int parent_ = -1);

  Location() {}

  ~Location()
  { delete_and_clear(loot); }

  void
  init();

  Location*
  addchild(char type_);

  void
  update_heat_protection();

  bool
  duplicatelocation();

  bool
  can_be_upgraded()
  { return upgradable; }

  bool
  can_be_fortified();

  bool
  fortified();

  bool
  can_be_trapped();

  bool
  trapped()
  { return compound_walls & COMPOUND_TRAPS; }

  bool
  can_install_tanktraps();

  bool
  tank_traps();

  bool
  can_have_businessfront();

  bool
  has_business_front();

  bool
  bomb_resistant();

  bool
  part_of_justice_system();

  bool
  is_lcs_safehouse()
  { return renting >= 0; }

  bool
  is_ccs_safehouse()
  { return renting == RENTING_CCS; }

  bool
  is_city()
  { return type == city; }

  string
  getname(signed char shortname_ = false, bool include_city = false);

  void
  rename(const char* name_, const char* shortname_);

  string
  city_description();

  void
  getloot(vector<Item*>& loot);

public:
  char                 name[LOCATION_NAMELEN];
  char                 shortname[LOCATION_SHORTNAMELEN];
  char                 type;
  int                  city;
  int                  area;
  int                  parent;
  int                  id;
  vector<Item*>        loot;
  vector<sitechangest> changes;
  int                  renting;
  char                 newrental;
  char                 needcar;
  int                  closed;
  bool                 hidden;
  bool                 mapped;
  bool                 upgradable;
  int                  highsecurity;
  siegest              siege;
  int                  heat;
  int                  heat_protection;
  int                  compound_walls;
  int                  compound_stores;
  char                 front_business;
  char                 front_name[LOCATION_NAMELEN];
  char                 front_shortname[LOCATION_SHORTNAMELEN];
  bool                 haveflag;
  unsigned long        mapseed[RNG_SIZE];
};

#endif // LOCATIONS_H_INCLUDED
