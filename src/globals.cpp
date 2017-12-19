/**
 * Global variables.
 *
 * Kill this with prejudice.
 */
/*
 * Copyright (c) 2002,2003,2004 by Tarn Adams
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
#include "externs.h"
#include "creature/creaturetypecache.h"

Log                  gamelog; //The gamelog.
Log                  xmllog; // Log for xml errors or bad values.

char                 artdir[MAX_PATH_SIZE];
unsigned long        seed[RNG_SIZE];
short                mode = GAMEMODE_TITLE;

string               savefile_name;

Alignment            law[LAWNUM];

Alignment            house[HOUSENUM];
Alignment            senate[SENATENUM];
Alignment            court[COURTNUM];
char                 courtname[COURTNUM][POLITICIAN_NAMELEN];

Alignment            exec[EXECNUM];
short                execterm=1;
char                 execname[EXECNUM][POLITICIAN_NAMELEN];
short                presparty=CONSERVATIVE_PARTY;
char                 oldPresidentName[POLITICIAN_NAMELEN];
int                  amendnum = 28;

short                attitude[VIEWNUM];
short                public_interest[VIEWNUM];
short                background_liberal_influence[VIEWNUM];

#ifdef THEFUTURE
static constexpr int starting_year = 2100;
#else
static constexpr int starting_year = 2009;
#endif
int                  day   = 1;
int                  month = 1;
int                  year  = starting_year;

vector<ClipType*>    cliptype;
vector<WeaponType*>  weapontype;
vector<ArmorType*>   armortype;
vector<LootType*>    loottype;
vector<AugmentType*> augmenttype;
vector<VehicleType*> vehicletype;
CreatureTypeCache    creature_type_cache; // @TODO move me into pandora's box

vector<Creature*>    pool;
vector<Vehicle*>     vehicle;
UniqueCreatures      uniqueCreatures;
long                 curcreatureid = 0;
Creature             encounter[ENCMAX];
chaseseqst           chaseseq;

char                 slogan[SLOGAN_LEN];
Ledger               ledger;
vector<datest*>      date;
vector<recruitst*>   recruit;

vector<Location*>    location;
siteblockst          levelmap[MAPX][MAPY][MAPZ];
char                 lcityname[CITY_NAMELEN];

vector<squadst*>     squad;
squadst*             activesquad = nullptr;
long                 cursquadid = 0;
short                party_status = -1;
char                 foughtthisround = 0;

vector<configSiteMap*> sitemaps; // stores site map info read in from config file
short                cursite;
short                sitetype;
short                postalarmtimer;
short                sitealarm;
short                sitealarmtimer;
short                sitealienate;
int                  sitecrime;
short                siteonfire;
int                  locx;
int                  locy;
int                  locz;
vector<Item*>        groundloot;

int                  selectedsiege = -1;
int                  police_heat = 0;
short                offended_amradio = 0;
short                offended_cablenews = 0;
short                offended_cia = 0;
short                offended_corps = 0;
short                offended_firemen = 0;

CursesMoviest        movie;
newsstoryst*         sitestory = nullptr;
vector<newsstoryst*> newsstory;
char                 newscherrybusted = 0;

short                wincondition = WINCONDITION_ELITE;
bool                 encounterwarnings = false;
char                 cantseereason;
int                  oldMapMode = 0; // -1 if we're using the old map generation functions.
bool                 stalinmode;
bool                 termlimits = false;
bool                 notermlimit;           //These determine if ELAs can take place --kviiri
bool                 nocourtpurge;
char                 disbanding = 0;
int                  disbandtime = 0;
bool                 mapshowing = false;
bool                 multipleCityMode;
char                 showcarprefs = 1;
char                 endgamestate = ENDGAME_NONE;
char                 ccsexposure = CCSEXPOSURE_NONE;
char                 ccs_kills = 0;
int                  ccs_siege_kills = 0;
int                  ccs_boss_kills = 0;
short                fieldskillrate = FIELDSKILLRATE_FAST;
bool                 deagle = false;
bool                 m249 = false;

short                interface_pgup = '[';
short                interface_pgdn = ']';
short                activesortingchoice[SORTINGCHOICENUM];

MusicClass           music;

highscorest          score[SCORENUM];
int                  yourscore = -1;
int                  stat_recruits = 0;
int                  stat_kidnappings = 0;
int                  stat_dead = 0;
int                  stat_kills = 0;
int                  stat_buys = 0;
int                  stat_burns = 0;

int                  ustat_recruits = 0;
int                  ustat_kidnappings = 0;
int                  ustat_dead = 0;
int                  ustat_kills = 0;
int                  ustat_funds = 0;
int                  ustat_spent = 0;
int                  ustat_buys = 0;
int                  ustat_burns = 0;

