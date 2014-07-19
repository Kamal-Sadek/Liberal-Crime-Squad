#ifndef LOCATIONS_H_INCLUDED
#define LOCATIONS_H_INCLUDED

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

   SITE_RESIDENTIAL_SHELTER, // this has to be in this spot - sites before this cannot be mapped and sites after this can be mapped

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
   SPECIALNUM,
   SPECIAL_NONE = -1
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
   char siege;
   //Puzz:  Temporary siege thing for organizations
   int orgID;
   short siegetype;
   char underattack;
   int attacktime;
   short kills;
   short tanks;
   short escalationstate;
   char lights_off;
   char cameras_off;
   short timeuntillocated;
   short timeuntilcorps;
   short timeuntilcia;
   short timeuntilccs;
   short timeuntilfiremen;

   siegest()
   {
      siege=0;
      siegetype=-1;
      underattack=0;
      escalationstate=0;
      timeuntillocated=-1;
      timeuntilcorps=-1;
      timeuntilcia=-1;
      timeuntilfiremen=-1;
      timeuntilccs=-1;
   }
};

struct siteblockst
{
   short special;
   int flag;
   char siegeflag;
};

struct sitechangest
{
   char x;
   char y;
   char z;
   int flag;
   sitechangest() {}
   sitechangest(char x_, char y_, char z_, int flag_) :
      x(x_), y(y_), z(z_), flag(flag_) {}
};

#define MAPX 70
#define MAPY 23
#define MAPZ 10

class Location
{
public:
   char name[40];
   char shortname[20];
   char type;
   int city;
   int area;
   int parent;
   int id;

   vector<Item *> loot;
   vector<sitechangest> changes;
   int renting;
   char newrental;
   char needcar;
   int closed;
   bool hidden;
   bool mapped;
   bool upgradable;
   int highsecurity;
   siegest siege;
   int heat;
   int heat_protection;
   int compound_walls;
   int compound_stores;
   char front_business;
   char front_name[40];
   char front_shortname[20];
   bool haveflag;

   int mapseed;

   Location(int type, int parent=-1);
   Location() { }
   Location* addchild(int type);
   ~Location() { delete_and_clear(loot); }
   void init();
   void update_heat_protection();
   bool duplicatelocation();
   bool can_be_upgraded();
   bool can_be_fortified();
   bool fortified();
   bool can_be_trapped();
   bool trapped();
   bool can_install_tanktraps();
   bool tank_traps();
   bool can_have_businessfront();
   bool has_business_front();
   bool bomb_resistant();
   bool part_of_justice_system();
   bool is_lcs_safehouse();
   bool is_ccs_safehouse();
   bool is_city();
   std::string getname(int shortname=false, bool include_city=false);
   void rename(const char* name, const char* shortname);
   char* city_description();
   void getloot(vector<Item *>& loot);
};

/*******************************************************************************
*
*                        Location Data
*                        Folder: "locations"
*
*******************************************************************************/

/*
 world.cpp
*/
Location* find_site_by_id(int id);
Location* find_site_in_city(int site, int city);
int find_site_index_in_city(int site, int city);
/* find local versions of these locations */
int find_police_station(const Creature& cr);
int find_police_station(int site);
int find_clinic(const Creature& cr);
int find_clinic(int site);
int find_homeless_shelter(const Creature& cr);
int find_homeless_shelter(int site);
int find_courthouse(const Creature& cr);
int find_courthouse(int site);
/* sets up the list of locations */
void make_world(bool hasmaps);

#endif //LOCATIONS_H_INCLUDED
