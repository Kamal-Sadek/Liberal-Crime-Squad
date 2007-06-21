/*
    File created by Chris Johnson.
    These were previously all in game.cpp.

    This file includes all the the enums, defines, and included headers required globally by all components.

    All code released under GNU GPL.
*/
#ifndef INCLUDES_H_INCLUDED
#define INCLUDES_H_INCLUDED

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "3.11.1svn2"
#endif

const unsigned long version=31101;
const unsigned long lowestloadversion=31005;
const unsigned long lowestloadscoreversion=30001;

#ifdef WIN32
   #include <windows.h>
   #include <string.h>
   //Visual C++ .NET (7) includes the STL with vector, so we
   //will use that, otherwise the HP STL Vector.h will be used.
   #ifdef __MINGW32__
      #include <iostream>
      #include <fstream>
      #include <vector>
      #include <io.h> //needed for unlink()
   #else
      #if _MSC_VER > 1200
         #define WIN32_DOTNET
         #include <iostream>
         #include <fstream>
         #include <vector>
      #else
         #define WIN32_PRE_DOTNET
         #include <iostream.h>
         #include <fstream.h>
         #include "vector.h"
      #endif
   #endif

   #include <curses.h>
   //undo PDCurses macros that break vector class
   #undef erase
   #undef clear

   #define CH_USE_CP437
#else
   #include <vector>
   #include <string.h>
   #include <iostream>
   #include <fstream>
   #include <ctype.h>
   #define GO_PORTABLE

   #define CH_USE_ASCII_HACK
   #ifdef HAVE_LIBXCURSES
   	#define XCURSES
   #endif
   #ifdef HAVE_LIBNCURSES
   	#define NCURSES
   #endif
   #ifdef XCURSES
      #define HAVE_PROTO 1
      #define CPLUSPLUS   1
      /* Try these PDCurses/Xcurses options later...
      #define FAST_VIDEO
      #define REGISTERWINDOWS
      */
      #include <xcurses.h> //This is the X11 Port of PDCurses
   //undo PDCurses macros that break vector class
      #undef erase
      #undef clear
   #else
      #ifdef NCURSES
         #include <ncurses.h>
      #else
         #include <curses.h>
      #endif
   #endif
#endif

/* Headers for Portability */
#ifdef GO_PORTABLE
   #include <time.h>

   #ifdef Linux // And BSD and SVr4
      #include <unistd.h>
      #include <sys/time.h>
      #include <signal.h>
   #endif
#endif



using namespace std;
#include "lcsio.h"
#include "compat.h"
#include "cursesmovie.h"
#include "cursesgraphics.h"



/*--------------------------------------------------------------------------
 * Portability Functions
 *
 * These functions are intended to replace explicit calls to Windows API.
 *
 * We can do the following:
 *
 * (a) Write alternative calls for the ports, keep Windows calls.
 * (b) Write portable alternatives for use by Windows and ports.
 * (c) Do (a) and (b) and decide what Windows does (API or portable)
 *       based on the value of a MACRO GO_PORTABLE.
 *
 * compat.cpp is the place for non-trivial or more global functions.
 *--------------------------------------------------------------------------*/

 inline unsigned long getSeed(void)
 {
 unsigned long t;

 #ifdef GO_PORTABLE

 t = (unsigned long)time(NULL); /* Seconds since 1970-01-01 00:00:00 */

 #else // WIN32

 t = (unsigned long)GetTickCount(); /* ms since system boot */

 #endif

 return(t);
 }


 /* raw_output() is provided in PDcurses/Xcurses but is not in ncurses.
   * This function is for compatibility and is currently a do nothing function.
   */
 #ifdef NCURSES
 inline int raw_output(bool bf)
 {
    raw();
 return OK;
 }

 #endif

/*--------------------------------------------------------------------------
 * End of Portability Functions
 *--------------------------------------------------------------------------*/





#ifndef NDEBUG
#define NDEBUG
#endif

#define BIT1 1
#define BIT2 2
#define BIT3 4
#define BIT4 8
#define BIT5 16
#define BIT6 32
#define BIT7 64
#define BIT8 128
#define BIT9 256
#define BIT10 512
#define BIT11 1024
#define BIT12 2048
#define BIT13 4096
#define BIT14 8192
#define BIT15 16384
#define BIT16 32768



unsigned long r_num(void);
long LCSrandom(unsigned long max);

enum UnlockTypes
{
   UNLOCK_DOOR,
   UNLOCK_CAGE,
   UNLOCK_SAFE,
   UNLOCKNUM
};

enum BashTypes
{
   BASH_DOOR,
   BASHNUM
};

enum HackTypes
{
   HACK_SUPERCOMPUTER,
   HACKNUM
};

enum SpecialAttacks
{
   ATTACK_CANNON,
   ATTACK_FLAME,
   ATTACK_SUCK,
   ATTACKNUM
};

enum SiteTypes
{
   SITE_DOWNTOWN,
   SITE_UDISTRICT,
   SITE_OUTOFTOWN,
   SITE_INDUSTRIAL,
   SITE_RESIDENTIAL_TENEMENT,
   SITE_RESIDENTIAL_APARTMENT,
   SITE_RESIDENTIAL_APARTMENT_UPSCALE,
   SITE_RESIDENTIAL_SHELTER,
   SITE_LABORATORY_COSMETICS,
   SITE_LABORATORY_GENETIC,
   SITE_HOSPITAL_CLINIC,
   SITE_HOSPITAL_UNIVERSITY,
   SITE_GOVERNMENT_POLICESTATION,
   SITE_GOVERNMENT_COURTHOUSE,
   SITE_GOVERNMENT_PRISON,
   SITE_GOVERNMENT_INTELLIGENCEHQ,
   SITE_INDUSTRY_SWEATSHOP,
   SITE_INDUSTRY_POLLUTER,
   SITE_INDUSTRY_NUCLEAR,
   SITE_INDUSTRY_WAREHOUSE,
   SITE_CORPORATE_HEADQUARTERS,
   SITE_CORPORATE_HOUSE,
   SITE_MEDIA_AMRADIO,
   SITE_MEDIA_CABLENEWS,
   SITE_BUSINESS_PAWNSHOP,
   SITE_BUSINESS_CRACKHOUSE,
   SITE_BUSINESS_JUICEBAR,
   SITE_BUSINESS_CIGARBAR,
   SITE_BUSINESS_LATTESTAND,
   SITE_BUSINESS_VEGANCOOP,
   SITE_BUSINESS_INTERNETCAFE,
   SITE_BUSINESS_DEPTSTORE,
   SITE_BUSINESS_HALLOWEEN,
   SITENUM
};

enum CreatureTypes
{
   CREATURE_SECURITYGUARD,
   CREATURE_SCIENTIST_LABTECH,
   CREATURE_SCIENTIST_EMINENT,
   CREATURE_CORPORATE_MANAGER,
   CREATURE_CORPORATE_CEO,
   CREATURE_WORKER_SERVANT,
   CREATURE_WORKER_JANITOR,
   CREATURE_WORKER_SWEATSHOP,
   CREATURE_WORKER_FACTORY_NONUNION,
   CREATURE_WORKER_FACTORY_CHILD,
   CREATURE_WORKER_SECRETARY,
   CREATURE_WORKER_FACTORY_UNION,
   CREATURE_LANDLORD,
   CREATURE_TEENAGER,
   CREATURE_COP,
   CREATURE_SWAT,
   CREATURE_DEATHSQUAD,
   CREATURE_EDUCATOR,
   CREATURE_GANGUNIT,
   CREATURE_JUDGE_LIBERAL,
   CREATURE_JUDGE_CONSERVATIVE,
   CREATURE_AGENT,
   CREATURE_RADIOPERSONALITY,
   CREATURE_NEWSANCHOR,
   CREATURE_GENETIC,
   CREATURE_GUARDDOG,
   CREATURE_PRISONER,
   CREATURE_JUROR,
   CREATURE_LAWYER,
   CREATURE_SEWERWORKER,
   CREATURE_COLLEGESTUDENT,
   CREATURE_MUSICIAN,
   CREATURE_MATHEMATICIAN,
   CREATURE_TEACHER,
   CREATURE_HSDROPOUT,
   CREATURE_BUM,
   CREATURE_GANGMEMBER,
   CREATURE_CRACKHEAD,
   CREATURE_PRIEST,
   CREATURE_ENGINEER,
   CREATURE_FASTFOODWORKER,
   CREATURE_TELEMARKETER,
   CREATURE_OFFICEWORKER,
   CREATURE_FOOTBALLCOACH,
   CREATURE_PROSTITUTE,
   CREATURE_MAILMAN,
   CREATURE_GARBAGEMAN,
   CREATURE_PLUMBER,
   CREATURE_CHEF,
   CREATURE_CONSTRUCTIONWORKER,
   CREATURE_AMATEURMAGICIAN,
   CREATURE_TANK,
   CREATURE_MERC,
   CREATURE_HICK,
   CREATURE_SOLDIER,
   CREATURE_PRISONGUARD,
   CREATURE_HIPPIE,
   CREATURE_CRITIC_ART,
   CREATURE_CRITIC_MUSIC,
   CREATURE_SOCIALITE,
   CREATURE_PROGRAMMER,
   CREATURE_RETIREE,
   CREATURE_PAINTER,
   CREATURE_SCULPTOR,
   CREATURE_AUTHOR,
   CREATURE_JOURNALIST,
   CREATURE_DANCER,
   CREATURE_PHOTOGRAPHER,
   CREATURE_CAMERAMAN,
   CREATURE_HAIRSTYLIST,
   CREATURE_FASHIONDESIGNER,
   CREATURE_CLERK,
   CREATURE_THIEF,
   CREATURE_ACTOR,
   CREATURE_YOGAINSTRUCTOR,
   CREATURE_ATHLETE,
   CREATURE_BIKER,
   CREATURE_TRUCKER,
   CREATURE_TAXIDRIVER,
   CREATURE_NUN,
   CREATURE_MUTANT,
   CREATURENUM
};

enum Attributes
{
   ATTRIBUTE_STRENGTH,
   ATTRIBUTE_INTELLIGENCE,
   ATTRIBUTE_WISDOM,
   ATTRIBUTE_AGILITY,
   ATTRIBUTE_HEALTH,
   ATTRIBUTE_CHARISMA,
   ATTRIBUTE_HEART,
   ATTNUM
};

enum Skills
{
   SKILL_HANDTOHAND,
   SKILL_KNIFE,
   SKILL_CLUB,
   SKILL_SWORD,
   SKILL_AXE,
   SKILL_SPEAR,
   SKILL_PISTOL,
   SKILL_RIFLE,
   SKILL_ASSAULTRIFLE,
   SKILL_PERSUASION,
   SKILL_LAW,
   SKILL_SECURITY,
   SKILL_DISGUISE,
   SKILL_COMPUTERS,
   SKILL_GARMENTMAKING,
   SKILL_DRIVING,
   SKILL_WRITING,
   SKILLNUM
};

enum Bodyparts
{
   BODYPART_HEAD,
   BODYPART_BODY,
   BODYPART_ARM_RIGHT,
   BODYPART_ARM_LEFT,
   BODYPART_LEG_RIGHT,
   BODYPART_LEG_LEFT,
   BODYPARTNUM
};

#define WOUND_SHOT BIT1
#define WOUND_CUT BIT2
#define WOUND_BRUISED BIT3
#define WOUND_BURNED BIT4
#define WOUND_BLEEDING BIT5
#define WOUND_TORN BIT6
#define WOUND_NASTYOFF BIT7
#define WOUND_CLEANOFF BIT8

enum Weapons
{
   WEAPON_NONE,
   WEAPON_BASEBALLBAT,
   WEAPON_CROWBAR,
   WEAPON_KNIFE,
   WEAPON_SHANK,
   WEAPON_SYRINGE,
   WEAPON_REVOLVER_22,
   WEAPON_REVOLVER_44,
   WEAPON_SEMIPISTOL_9MM,
   WEAPON_SEMIPISTOL_45,
   WEAPON_AUTORIFLE_M16,
   WEAPON_AUTORIFLE_AK47,
   WEAPON_SEMIRIFLE_AR15,
   WEAPON_CARBINE_M4,
   WEAPON_SMG_MP5,
   WEAPON_SHOTGUN_PUMP,
   WEAPON_SWORD,
   WEAPON_CHAIN,
   WEAPON_NIGHTSTICK,
   WEAPON_GAVEL,
   WEAPON_AXE,
   WEAPON_HAMMER,
   WEAPON_MAUL,
   WEAPON_CROSS,
   WEAPON_STAFF,
   WEAPON_PITCHFORK,
   WEAPON_TORCH,
   WEAPONNUM
};

struct weaponst
{
   short type;
   short ammo;

   char ranged(void)
   {
      switch(type)
      {
         case WEAPON_REVOLVER_22:
         case WEAPON_REVOLVER_44:
         case WEAPON_SEMIPISTOL_9MM:
         case WEAPON_SEMIPISTOL_45:
         case WEAPON_AUTORIFLE_M16:
         case WEAPON_AUTORIFLE_AK47:
         case WEAPON_SEMIRIFLE_AR15:
         case WEAPON_CARBINE_M4:
         case WEAPON_SMG_MP5:
         case WEAPON_SHOTGUN_PUMP:
            return 1;
      }
      return 0;
   }
};

// *JDS* This should be expanded to cover
// any situation in which you want a bullet
// proof vest to act as a proper uniform.
// eg, security, etc...
//
// Yes, it would be in some ways cooler to
// do it the other way around -- have
// body armor be a subtype, and use the
// uniform as your main type, but that
// would necessitate modifying the names
// of the armor pieces across many cases.
//
// Perhaps I will change this system,
// so that there is a flag after the
// armor name (like the bloody and damaged
// flags) which signals a vest is being
// worn underneath, but eh... this should
// work for now.
enum BallisticVestTypes
{
   BVEST_POLICE,
   BVEST_MILITARY
};

enum MaskTypes
{
   MASK_ASHCROFT,
   MASK_BARR,
   MASK_BLAIR,
   MASK_BUSH_BARBARA,
   MASK_BUSH_GEORGE,
   MASK_BUSH_GEORGE2,
   MASK_CARTER,
   MASK_CHENEY,
   MASK_CLINTON_BILL,
   MASK_CLINTON_HILLARY,
   MASK_FALWELL,
   MASK_FORD,
   MASK_GORE_AL,
   MASK_GORE_TIPPER,
   MASK_HELMS,
   MASK_JASON,
   MASK_LINCOLN,
   MASK_KENNEDY_JOHN,
   MASK_KENNEDY_ROBERT,
   MASK_KISSINGER,
   MASK_NIXON,
   MASK_POWELL,
   MASK_REAGAN_NANCY,
   MASK_REAGAN_RONALD,
   MASK_RICE,
   MASK_ROBERTSON,
   MASK_RUMSFELD,
   MASK_SATAN,
   MASK_STARR,
   MASK_THURMOND,
   MASK_WASHINGTON,
   MASK_WAXMAN,
   //SURPRISE MASKS
   MASK_JESUS,
   MASK_COLEMAN_GARY,
   MASK_MADONNA,
   MASK_SPEARS,
   MASK_EMINEM,
   MASK_AGUILERA,
   MASK_WAHLBERG,
   MASK_IGGYPOP,
   MASK_CASH,
   MASK_BINLADEN,
   MASK_LORDS,
   MASK_SHIELDS,
   MASK_JACKSON_MICHAEL,
   MASK_CRUTHERS,
   MASK_KING_DON,
   MASKNUM
};

enum Armors
{
   ARMOR_NONE,
   ARMOR_CLOTHES,
   ARMOR_TRENCHCOAT,
   ARMOR_WORKCLOTHES,
   ARMOR_SECURITYUNIFORM,
   ARMOR_POLICEUNIFORM,
   ARMOR_CHEAPSUIT,
   ARMOR_EXPENSIVESUIT,
   ARMOR_BLACKSUIT,
   ARMOR_CHEAPDRESS,
   ARMOR_EXPENSIVEDRESS,
   ARMOR_BLACKDRESS,
   ARMOR_LABCOAT,
   ARMOR_BLACKROBE,
   ARMOR_CLOWNSUIT,
   ARMOR_BONDAGEGEAR,
   ARMOR_MASK,
   ARMOR_MILITARY,
   ARMOR_PRISONGUARD,
   ARMOR_PRISONER,
   ARMOR_TOGA,
   ARMOR_MITHRIL,
   ARMOR_OVERALLS,
   ARMOR_WIFEBEATER,
   ARMOR_BALLISTICVEST,
   ARMOR_HEAVYBALLISTICVEST,
   ARMORNUM
};

#define ARMORFLAG_DAMAGED BIT1
#define ARMORFLAG_BLOODY BIT2

struct armorst
{
   short type;
   short subtype;
   char quality;
   unsigned short flag;

   armorst()
   {
      quality='1';
      flag=0;
   }
};

enum ClipType
{
   CLIP_9,
   CLIP_45,
   CLIP_ASSAULT,
   CLIP_SMG,
   CLIP_22,
   CLIP_44,
   CLIP_BUCKSHOT,
   CLIPNUM
};

enum LootType
{
   LOOT_LABEQUIPMENT,
   LOOT_COMPUTER,
   LOOT_TV,
   LOOT_SECRETDOCUMENTS,
   LOOT_CEOPHOTOS,
   LOOT_INTHQDISK,
   LOOT_CORPFILES,
   LOOT_POLICERECORDS,
   LOOT_VCR,
   LOOT_CELLPHONE,
   LOOT_TVCAMERA,
   LOOT_BROADCASTINGEQUIPMENT,
   LOOT_SILVERWARE,
   LOOT_SCANNER,
   LOOT_PRINTER,
   LOOT_FINECLOTH,
   LOOT_CHEMICAL,
   LOOTNUM
};

enum SpecialWounds
{
   SPECIALWOUND_TEETH,
   SPECIALWOUND_RIGHTEYE,
   SPECIALWOUND_LEFTEYE,
   SPECIALWOUND_NOSE,
   SPECIALWOUND_TONGUE,
   SPECIALWOUND_RIGHTLUNG,
   SPECIALWOUND_LEFTLUNG,
   SPECIALWOUND_HEART,
   SPECIALWOUND_LIVER,
   SPECIALWOUND_STOMACH,
   SPECIALWOUND_RIGHTKIDNEY,
   SPECIALWOUND_LEFTKIDNEY,
   SPECIALWOUND_SPLEEN,
   SPECIALWOUND_RIBS,
   SPECIALWOUND_NECK,
   SPECIALWOUND_UPPERSPINE,
   SPECIALWOUND_LOWERSPINE,
   SPECIALWOUNDNUM
};

#define RIBNUM 10
#define TOOTHNUM 32

/* *JDS* I'm making laws an array instead of a bunch
 * of bits which are either on or off. Each charge can be
 * duplicated, for multiple counts, and intense focus
 * for wanted persons.
 */
enum Lawflags
{
   LAWFLAG_KIDNAPPING,
   LAWFLAG_MURDER,
   LAWFLAG_THEFT,
   LAWFLAG_BREAKING,
   LAWFLAG_TERRORISM,
   LAWFLAG_JURY,
   LAWFLAG_TREASON,
   LAWFLAG_ESCAPED,
   LAWFLAG_HELPESCAPE,
   LAWFLAG_RESIST,
   LAWFLAG_BURNFLAG,
   LAWFLAG_SPEECH,
   LAWFLAG_VANDALISM,
   LAWFLAG_ASSAULT,
   LAWFLAG_CARTHEFT,
   LAWFLAG_INFORMATION,
   LAWFLAG_COMMERCE,
   LAWFLAG_CCFRAUD,
   LAWFLAG_BROWNIES,
   LAWFLAG_BURIAL,
   LAWFLAG_PROSTITUTION,
   LAWFLAG_DISTURBANCE,
   LAWFLAG_HIREILLEGAL,
   LAWFLAG_RACKETEERING,
   LAWFLAG_LOITERING,
   LAWFLAG_GUNCARRY,
   LAWFLAGNUM
};

/* *JDS* In addition to laws being an array,
 * each law will be associated with a "heat" value,
 * indicating how much that particular crime draws
 * police attention to the wanted person, and to the
 * LCS as a whole. This is a global array, available
 * at runtime and indexed to the lawflag enum, so that
 * code can dynamically look up these hea
 t values as
 * needed at runtime.
 */


enum BusinessFronts
{
   BUSINESSFRONT_INSURANCE,
   BUSINESSFRONT_TEMPAGENCY,
   BUSINESSFRONTNUM
};

enum AnimalGlosses
{
   ANIMALGLOSS_NONE,//IMPORTANT THAT THIS BE HERE AT ZERO
   ANIMALGLOSS_TANK,
   ANIMALGLOSS_ANIMAL,
   ANIMALGLOSSNUM
};

enum Activity
{
   ACTIVITY_NONE,
   ACTIVITY_VISIT,
   ACTIVITY_HOSTAGETENDING,
   ACTIVITY_TROUBLE,
   ACTIVITY_FUNDS_LEGAL,
   ACTIVITY_FUNDS_ILLEGAL,
   ACTIVITY_REPAIR_ARMOR,
   ACTIVITY_MAKE_ARMOR,
   ACTIVITY_POLLS,
   ACTIVITY_STEALCARS,
   ACTIVITY_WHEELCHAIR,
   ACTIVITY_BURY,
   ACTIVITY_WRITE,
   ACTIVITY_CLINIC,
   ACTIVITYNUM
};

struct activityst
{
   long type;
   long arg;
   long arg2;
};



#define CREATUREFLAG_WHEELCHAIR BIT1
#define CREATUREFLAG_JUSTESCAPED BIT2
#define CREATUREFLAG_MISSING BIT3
#define CREATUREFLAG_KIDNAPPED BIT4
#define CREATUREFLAG_SLEEPER BIT5
#define CREATUREFLAG_ILLEGALALIEN BIT6
#define CREATURE_NAMELEN 40
struct creaturest
{
   char name[CREATURE_NAMELEN];
   char propername[CREATURE_NAMELEN];
   long squadid;//REMEMBER, THIS IS ID NUMBER, NOT ARRAY INDEX
   char exists;
   char align;
   char alive;
   short type;
   char animalgloss;
   short specialattack;
   short clinic;
   short dating;
   short hiding;
   short trainingtime;
   short trainingsubject;
   creaturest *prisoner;
   short sentence;
   char deathpenalty;
   long joindays;
   long deathdays;
   long id;
   long hireid;

   char forceinc;

   long att[ATTNUM];
   unsigned long skill[SKILLNUM];
   unsigned long skill_ip[SKILLNUM];

   weaponst weapon;
   armorst armor;
   int clip[CLIPNUM];

   unsigned long money;
   long juice;

   char wound[BODYPARTNUM];
   short blood;
   char special[SPECIALWOUNDNUM];

   unsigned int lawflag[LAWFLAGNUM];
   long location;
   long worklocation;

   char cantbluff;

   long base;
   activityst activity;

   long carid;
   char is_driver;
   long pref_carid;
   char pref_is_driver;
   unsigned short flag;

   creaturest()
   {
      creatureinit();
   }
   ~creaturest()
   {
      if(prisoner!=NULL)
      {
         if(prisoner->squadid!=-1)delete prisoner;
      }
   }
   char canwalk(void)
   {
      if(!alive)return 0;
      int legok=2;
      if((wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
          (wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
      if((wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
          (wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;
      if(special[SPECIALWOUND_NECK]!=1||
         special[SPECIALWOUND_UPPERSPINE]!=1||
         special[SPECIALWOUND_LOWERSPINE]!=1)return 0;
      if(legok==0)return 0;

      return 1;
   }
   void creatureinit(void);
   long attval(short a,char usejuice=1);
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

enum SpecialBlocks
{
   SPECIAL_LAB_COSMETICS_CAGEDANIMALS,
   SPECIAL_LAB_GENETIC_CAGEDANIMALS,
   SPECIAL_POLICESTATION_LOCKUP,
   SPECIAL_COURTHOUSE_LOCKUP,
   SPECIAL_COURTHOUSE_JURYROOM,
   SPECIAL_PRISON_CONTROL,
   SPECIAL_INTEL_SUPERCOMPUTER,
   SPECIAL_SWEATSHOP_EQUIPMENT,
   SPECIAL_POLLUTER_EQUIPMENT,
   SPECIAL_NUCLEAR_ONOFF,
   SPECIAL_HOUSE_PHOTOS,
   SPECIAL_CORPORATE_FILES,
   SPECIAL_RADIO_BROADCASTSTUDIO,
   SPECIAL_NEWS_BROADCASTSTUDIO,
   SPECIAL_APARTMENT_LANDLORD,
   SPECIAL_APARTMENT_SIGN,
   SPECIAL_RESTAURANT_TABLE,
   SPECIAL_CAFE_COMPUTER,
   SPECIAL_STAIRS_UP,
   SPECIAL_STAIRS_DOWN,
   SPECIALNUM
};

#define SIEGEFLAG_UNIT BIT1
#define SIEGEFLAG_TRAP BIT2
#define SIEGEFLAG_HEAVYUNIT BIT3
#define SIEGEFLAG_UNIT_DAMAGED BIT4

struct siteblockst
{
   short special;
   unsigned short flag;
   unsigned char siegeflag;
};

enum ItemTypes
{
   ITEM_WEAPON,
   ITEM_ARMOR,
   ITEM_MONEY,
   ITEM_CLIP,
   ITEM_LOOT,
   ITEMNUM
};

struct itemst
{
   weaponst weapon;
   armorst armor;
   unsigned long money;
   short cliptype;
   short loottype;
   short type;
   long number;

   itemst()
   {
      number=1;
   }
};



enum SiegeTypes
{
   SIEGE_POLICE,
   SIEGE_CIA,
   SIEGE_HICKS,
   SIEGE_CORPORATE,
   SIEGENUM
};

struct siegest
{
   char siege;
   short siegetype;
   char underattack;
   long attacktime;
   short kills;
   short escalationstate;
   char lights_off;
   char cameras_off;
   short timeuntillocated;
   short timeuntilcorps;
   short timeuntilcia;

   siegest()
   {
      siege=0;
      siegetype=-1;
      underattack=0;
      escalationstate=0;
      timeuntillocated=-1;
      timeuntilcorps=-1;
      timeuntilcia=-1;
   }
};

#define COMPOUND_BASIC BIT1
#define COMPOUND_CAMERAS BIT2
#define COMPOUND_TANKTRAPS BIT3
#define COMPOUND_TRAPS BIT4
#define COMPOUND_GENERATOR BIT5
#define COMPOUND_PRINTINGPRESS BIT6

struct locationst
{
   char name[40];
   char shortname[20];
   short type;
   long parent;
   vector<itemst *> loot;
   long renting;
   char newrental;
   char needcar;
   short closed;
   char highsecurity;
   siegest siege;
   int heat;
   char compound_walls;
   long compound_stores;
   short front_business;
   char front_name[40];
   char haveflag;

   unsigned long mapseed;

   locationst()
   {
      init();
      needcar=0;
      renting=-1;
   }
   void init(void);
};


enum VehicleTypes
{
   VEHICLE_VAN,
   VEHICLE_STATIONWAGON,
   VEHICLE_SPORTSCAR,
   VEHICLE_BUG,
   VEHICLE_PICKUP,
   VEHICLE_POLICECAR,
   VEHICLE_TAXICAB,
   VEHICLE_SUV,
   VEHICLE_AGENTCAR,
   VEHICLE_JEEP,
   VEHICLENUM,
};

enum VehicleColors
{
   VEHICLECOLOR_RED,
   VEHICLECOLOR_WHITE,
   VEHICLECOLOR_BLUE,
   VEHICLECOLOR_BEIGE,
   VEHICLECOLOR_POLICE,
   VEHICLECOLOR_TAXI,
   VEHICLECOLOR_BLACK,
   VEHICLECOLORNUM
};


struct vehiclest
{
   short type;
   short color;
   long location;
   int myear;
   long id;

   void init(int t);
};


enum CarChaseObstacles
{
   CARCHASE_OBSTACLE_FRUITSTAND,
   CARCHASE_OBSTACLENUM
};

#define MAPX 70
#define MAPY 23
#define MAPZ 10



//struct chaseseqst
struct chaseseqst
{
   long location;
   vector<vehiclest *> friendcar;
   vector<vehiclest *> enemycar;
   char canpullover;

   //public:
   void clean(void);
};








struct squadst
{
   char name[40];
   creaturest *squad[6];
   activityst activity;
   int id;
   vector<itemst *> loot;

   squadst()
   {
      for(int p=0;p<6;p++)squad[p]=NULL;
      strcpy(name,"");
      activity.type=ACTIVITY_NONE;
      id=-1;
   }
   ~squadst()
   {
      for(unsigned int l=0;l<loot.size();l++)
      {
         delete loot[l];
      }

      loot.clear();
   }
};


#define ENCMAX 18


enum GameModes
{
   GAMEMODE_TITLE,
   GAMEMODE_BASE,
   GAMEMODE_SITE,
   GAMEMODE_CHASECAR,
   GAMEMODE_CHASEFOOT
};



enum Views
{
   VIEW_ABORTION,
   VIEW_GAY,
   VIEW_DEATHPENALTY,
	VIEW_TAXES,
   VIEW_NUCLEARPOWER,
   VIEW_ANIMALRESEARCH,
   VIEW_POLICEBEHAVIOR,
   VIEW_PRISONS,
   VIEW_INTELLIGENCE,
   VIEW_FREESPEECH,
   VIEW_GENETICS,
   VIEW_JUSTICES,
   VIEW_SWEATSHOPS,
   VIEW_POLLUTION,
   VIEW_CORPORATECULTURE,
   VIEW_CEOSALARY,
   //*JDS* I'm using VIEWNUM-4 in a random generator that rolls a
   //random issue, not including the media ones, and this will
   //break if these stop being the last 4 issues; do a search
   //for VIEWNUM-4 to change it if it needs to be changed.
   VIEW_AMRADIO,
   VIEW_CABLENEWS,
   //THESE TWO MUST BE LAST FOR VIEWNUM-2 TO WORK IN PLACES
   VIEW_LIBERALCRIMESQUAD,
   VIEW_LIBERALCRIMESQUADPOS,
   VIEWNUM
};



enum Laws
{
   LAW_ABORTION,
   LAW_ANIMALRESEARCH,
   LAW_POLICEBEHAVIOR,
   LAW_PRIVACY,
   LAW_DEATHPENALTY,
   LAW_NUCLEARPOWER,
   LAW_POLLUTION,
   LAW_LABOR,
   LAW_GAY,
   LAW_CORPORATE,
   LAW_FREESPEECH,
   LAW_FLAGBURNING,
   LAW_GUNCONTROL,
   LAW_TAX,
   LAWNUM
};

struct datest
{
   long mac_id;
   vector<creaturest *> date;
   short timeleft;
   datest()
   {
      timeleft=0;
   }
   ~datest()
   {
      for(unsigned int d=0;d<date.size();d++)delete date[d];
      date.clear();
   }
};

enum Crimes
{
   CRIME_STOLEGROUND,
   CRIME_UNLOCKEDDOOR,
   CRIME_BROKEDOWNDOOR,
   CRIME_ATTACKED_MISTAKE,
   CRIME_ATTACKED,
   CRIME_CARCHASE,
   CRIME_CARCRASH,
   CRIME_FOOTCHASE,
   CRIME_KILLEDSOMEBODY,
   CRIME_SHUTDOWNREACTOR,
   CRIME_POLICE_LOCKUP,
   CRIME_COURTHOUSE_LOCKUP,
   CRIME_PRISON_RELEASE,
   CRIME_JURYTAMPERING,
   CRIME_HACK_INTEL,
   CRIME_BREAK_SWEATSHOP,
   CRIME_BREAK_FACTORY,
   CRIME_HOUSE_PHOTOS,
   CRIME_CORP_FILES,
   CRIME_FREE_RABBITS,
   CRIME_FREE_BEASTS,
   CRIMENUM
};

enum NewsStories
{
   NEWSSTORY_MAJOREVENT,
   NEWSSTORY_SQUAD_SITE,
   NEWSSTORY_SQUAD_ESCAPED,
   NEWSSTORY_SQUAD_FLEDATTACK,
   NEWSSTORY_SQUAD_DEFENDED,
   NEWSSTORY_SQUAD_BROKESIEGE,
   NEWSSTORY_SQUAD_KILLED_SIEGEATTACK,
   NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE,
   NEWSSTORY_SQUAD_KILLED_SITE,
   NEWSSTORY_CARTHEFT,
   NEWSSTORY_MASSACRE,
   NEWSSTORY_KIDNAPREPORT,
   NEWSSTORYNUM
};

struct newsstoryst
{
   short type;
   short view;
   creaturest *cr;
   vector<int> crime;
   long loc,priority,page;
   char positive;
   short siegetype;
   newsstoryst()
   {
      cr=NULL;
   }
};

struct blogpostst
{
   char type;
   char power;
   char issue;
};


#define SLOGAN_LEN 79

struct highscorest
{
   char valid;
   char endtype;

   char slogan[SLOGAN_LEN];
   int month;
   int year;
   unsigned long stat_recruits;
   unsigned long stat_kidnappings;
   unsigned long stat_dead;
   unsigned long stat_kills;
   unsigned long stat_funds;
   unsigned long stat_spent;
   unsigned long stat_buys;
   unsigned long stat_burns;
};

#define SCORENUM 5

enum EndTypes
{
   END_WON,
   END_HICKS,
   END_CIA,
   END_POLICE,
   END_CORP,
   END_REAGAN,
   END_DEAD,
   END_PRISON,
   END_EXECUTED,
   END_DATING,
   END_HIDING,
   END_DISBANDLOSS,
   END_DISPERSED,
   ENDNUM
};
enum ReportTypes
{
   REPORT_NEWS,
   REPORT_OPINION,
   REPORT_ATTACK,
   REPORTNUM
};

enum Execs
{
   EXEC_PRESIDENT,
   EXEC_VP,
   EXEC_STATE,
   EXEC_ATTORNEY,
   EXECNUM
};


enum TalkModes
{
   TALKMODE_START,
   TALKMODE_RENTING,
   TALKMODE_ISSUES,
   TALKMODENUM
};

enum ReviewModes
{
   REVIEWMODE_LIBERALS,
   REVIEWMODE_HOSTAGES,
   REVIEWMODE_CLINIC,
   REVIEWMODE_JUSTICE,
   REVIEWMODE_SLEEPERS,
   REVIEWMODE_DEAD,
   REVIEWMODE_AWAY,
   REVIEWMODENUM
};

/*
	Declarations for every function grouped by folder and file.
	Created by jonathansfox.
*/


/* end the game and clean up */
void end_game(int err=0);

/*******************************************************************************
*
*                                Common Stuff
*                                Folder: "common"
*
*******************************************************************************/

/*
 commondisplay.cpp
*/
/* location and squad header */
void locheader(void);
/* party info at top of screen */
void printparty(void);
/* location info at top of screen */
void printlocation(long loc);
/* character info at top of screen */
void printcreatureinfo(creaturest *cr);
/* full character sheet (with surrounding interface) */
void fullstatus(int p);
/* full screen character sheet */
void printliberalstats(creaturest &cr,char smll);
/* draws a horizontal line across the screen */
void makedelimiter(int y,int x);
/* print location name (is aware of business fronts) */
void addlocationname(locationst *loc);
/* prints a character's health description (One Leg, Liberal, NearDETH...) */
void printhealthstat(creaturest &g,int y,int x,char smll);

/*
 commonactions.cpp
*/
/* common - test for possible game over */
char endcheck(short cause=-1);
/* common - detatches all liberals from a specified car */
void removecarprefs_pool(long carid);
/* common - tests if the person is a wanted criminal */
bool iscriminal(creaturest &cr);
/* common - sends somebody to the hospital */
void hospitalize(int loc, creaturest &patient);
/* common - determines how long a creature's injuries will take to heal */
int clinictime(creaturest &g);
/* common - purges squad of loot and vehicles if it has no members */
int testsquadclear(squadst &thissquad, int obase);
/* common - returns the associated attribute for the given skill */
int skillatt(int skill);
/* common - applies a crime to everyone in the active party */
void criminalizeparty(short crime);
/* common - applies a crime to everyone in a location, or the entire LCS */
void criminalizepool(short crime,long exclude=-1,short loc=-1);
/* common - gives juice to everyone in the active party */
void juiceparty(long juice);
/* common - gives juice to a given creature */
void addjuice(creaturest &cr,long juice,long cap=1000);
/* common - removes the liberal from all squads */
void removesquadinfo(creaturest &cr);
/* common - purges empty squads from existance */
void cleangonesquads(void);
/* common - moves all squad members and their cars to a new location */
void locatesquad(squadst *st,long loc);
/* common - assigns a new base to all members of a squad */
void basesquad(squadst *st,long loc);
/* common - shifts public opinion on an issue */
void change_public_opinion(int v,int power,char affect,char cap=1);
/* returns the amount of heat associated with a given crime */
int lawflagheat(int lawflag);

/*
 consolesupport.cpp
*/
void set_color(short f,short b,char bright);
void translategetch(int &c);
void translategetch_cap(int &c);

/*
 getnames.cpp
*/
void getactivity(char *str,activityst &act);
void getweapon(char *str,int type);
void getweaponfull(char *str,int type);
void getarmor(char *str,int type,int subtype=-1);
void getarmorfull(char *str,armorst &armor,char superfull);
void getarmorfull(char *str,int type,int subtype=-1);
void getmaskdesc(char *str,short mask);
void getskill(char *str,int type);
void getclip(char *str,int clip);
void getloot(char *str,int loot);
void getrecruitcreature(char *str,int type);
void gettitle(char *str,creaturest &cr);
void getview(char *str,short view);
void getlaw(char *str,int l);
void getcarfull(char *str,vehiclest &car,char halffull=0);
void getcarfull(char *str,int type);
void getcar(char *str,int type);
void getcarcolor(char *str,int type);
short naturalcarcolor(int type); /* support function for getcarcolor */
void cityname(char *story); /* random city name */
/* Allow player to enter a name with an optional default name */
void enter_name(char *name, int len, char *defname=NULL);

/*
 translateid.cpp
*/
/* transforms a squad id number into the index of that squad in the global vector */
long getsquad(long id);
/* transforms a car id number into the index of that car in the global vector */
long id_getcar(long id);
/* transforms a creature id number into the index of that person in the pool */
int getpoolcreature(long id);

/*
 equipment.cpp
*/
/* review squad equipment */
void equip(vector<itemst *> &loot,int loc);
/* lets you pick stuff to stash/retrieve from one location to another */
void moveloot(vector<itemst *> &dest,vector<itemst *> &source);
/* combines multiple items of the same type into stacks */
void consolidateloot(vector<itemst *> &loot);
/* compares two items, used in sorting gear */
char itemcompare(itemst *a,itemst *b);
/* returns the type of ammo used by the given weapon, if any */
short ammotype(int type);
/* check if a weapon is ranged */
char rangedweapon(weaponst &w);
/* check if the squad has a certain weapon */
char squadhasitem(squadst &sq,int type,int subtype);

/*
 creature.cpp
*/
/* rolls up a creature's stats and equipment */
void makecreature(creaturest &cr,short type);
/* rolls up a proper name for a creature */
void namecreature(creaturest &cr);
/* fills a string with a proper name */
void name(char *str);
/* gets a random first name */
void firstname(char *str);
/* gets a random last name */
void lastname(char *str);
/* ensures that the creature's work location is appropriate to its type */
void verifyworklocation(creaturest &cr);
/* turns a creature into a conservative */
void conservatise(creaturest &cr);
/* turns a creature into a liberal */
void liberalize(creaturest &cr);

/*******************************************************************************
*
*                             Title Screen and New Game
*                             Folder: "title"
*
*******************************************************************************/

/*
 titlescreen.cpp
*/
void mode_title(void);
void loadinitfile(void);

/*
 highscore.cpp
*/
/* displays the high score board */
void viewhighscores(void);
/* loads the high scores file */
void loadhighscores(void);
/* saves a new high score */
void savehighscore(char endtype);

/*
 newgame.cpp
*/
/* creates your founder */
void makecharacter(void);
/* mostly depricated, but called once by makecharacter */
void initliberal(creaturest &cr);

/*
 saveload.cpp
*/
/* saves the game to save.dat */
void save(void);
/* saves the game to autosave.dat */
void autosave(void);
/* handles saving */
void savegame(char *str);
/* loads the game from save.dat */
char load(void);
/* deletes save.dat (used on endgame and for invalid save version) */
void reset(void);


/*******************************************************************************
*
*                        The Main Game Screen: Base Mode
*                        Folder: "basemode"
*
*******************************************************************************/

/*
 basemode.cpp
*/
void mode_base(void);

/*
 baseactions.cpp
*/
/* base - burn the flag */
void burnflag(void);
/* base - new slogan */
void getslogan(void);
/* base - reorder party */
void orderparty(void);
/* base - go forth to stop evil */
void stopevil(void);
/* base - liberal agenda */
char liberalagenda(char won);
/* base - liberal agenda - disband */
char confirmdisband(void);
/* base - invest in this location */
void investlocation(void);
/* base - assign a vehicle to this squad */
void setvehicles(void);

/*
 activate.cpp
*/
/* base - activate the uninvolved */
void activate(void);
void activate(creaturest *cr);
void activatebulk(void);
/* base - activate - hostages */
void select_tendhostage(creaturest *cr);
long select_hostagefundinglevel(creaturest *cr,creaturest *hs);
/* base - activate - make clothing */
void select_makeclothing(creaturest *cr);
long armor_makedifficulty(int type,creaturest *cr);
long armor_makeprice(int type);
/* base - activate - trouble */
long select_troublefundinglevel(creaturest *cr);
/* base - activate - select a topic to write about (uncalled function!!!) */
char select_view(creaturest *cr,long &v);

/*
 reviewmode.cpp
*/
/* base - review and reorganize liberals */
void review(void);
void review_mode(short mode);
/* base - review - assemble a squad */
void assemblesquad(squadst *cursquad);
/* base - review - assign new bases to the squadless */
void squadlessbaseassign(void);
/* base - review - promote liberals */
void promoteliberals(void);
void sortbyhire(vector<creaturest *> &temppool,vector<int> &level);

/*******************************************************************************
*
*                             Fighting Evil: Site Mode
*                             Folder: "sitemode"
*
*******************************************************************************/

/*
 sitemode.cpp
*/
void mode_site(long loc);
void mode_site(void);
/* site - determines spin on site news story, "too hot" timer */
void resolvesite(void);

/*
 map.cpp
*/
/* re-create site from seed before squad arrives */
void initsite(locationst &loc);
/* recursive dungeon-generating algorithm */
void generateroom(int rx,int ry,int dx,int dy,int z=0);
/* marks the area around the specified tile as explored */
void knowmap(int locx,int locy,int locz);

/*
 newencounter.cpp
*/
/* generates a new random encounter */
void prepareencounter(short type,char sec);
/* generates a new siege encounter */
char addsiegeencounter(char type);
/* rolls up a random creature type according to the passed weighting array */
int getrandomcreaturetype(int cr[CREATURENUM]);

/*
 sitedisplay.cpp
*/
/* prints the 'map graphics' on the bottom right */
void printsitemap(int x,int y,int z);
void printblock(int x,int y,int z,int px,int py);
/* prints the names of creatures you see */
void printencounter(void);
/* prints the names of creatures you see in car chases */
void printchaseencounter(void);
/* blanks a part of the screen */
void clearcommandarea(void);
void clearmessagearea(void);
void clearmaparea(void);

/*
 miscactions.cpp
*/
/* unlock attempt */
char unlock(short type,char &actual);
/* bash attempt */
char bash(short type,char &actual);
/* returns the bash bonus provided by the specified weapon */
long bashstrengthmod(int t);
/* computer hack attempt */
char hack(short type,char &actual);
/* run a radio broadcast */
char radio_broadcast(void);
/* run a tv broadcast */
char news_broadcast(void);
/* rescues people held at the activeparty's current location */
void partyrescue(void);
/* everybody reload! */
void reloadparty(void);

/*
 mapspecials.cpp
*/
void special_lab_cosmetics_cagedanimals(void);
void special_readsign(int sign);
void special_nuclear_onoff(void);
void special_lab_genetic_cagedanimals(void);
void special_policestation_lockup(void);
void special_courthouse_lockup(void);
void special_courthouse_jury(void);
void special_prison_control(void);
void special_intel_supercomputer(void);
void special_sweatshop_equipment(void);
void special_polluter_equipment(void);
void special_house_photos(void);
void special_corporate_files(void);
void special_radio_broadcaststudio(void);
void special_news_broadcaststudio(void);

/*
 talk.cpp
*/
/* bluff, date, issues */
char talk(creaturest &a,int t);
/* are they interested in talking about the issues? */
char talkreceptive(creaturest &cr);
/* is the character too young to be dating? */
char kid(creaturest &cr);

/*
 stealth.cpp
*/
/* checks if your liberal activity is noticed */
void noticecheck(int exclude);
/* checks if your liberal behavior/attack alienates anyone */
char alienationcheck(char mistake);
/* checks if conservatives see through your disguise */
void disguisecheck(void);
/* returns the difficulty of seeing through your squad's disguise */
int disguiseskill(void);
/* checks if a creature's weapon is suspicious or illegal */
char weaponcheck(creaturest &cr,short type);
/* checks if a creature's uniform is appropriate to the location */
char hasdisguise(creaturest &cr,short type);
/* returns true if the entire site is not open to public */
char disguisesite(long type);

/*
 advance.cpp
*/
/* handles end of round stuff for everyone */
void creatureadvance(void);
/* handles end of round stuff for one creature */
void advancecreature(creaturest &cr);

/*******************************************************************************
*
*                        Combat and Chase Sequences
*                        Folder: "combat"
*
*******************************************************************************/

/*
 fight.cpp
*/
/* attack handling for each side as a whole */
void youattack(void);
void enemyattack(void);
/* attack handling for an individual creature and its target */
void attack(creaturest &a,creaturest &t,char mistake,char &actual);
/* modifies a combat roll based on the creature's critical injuries */
void healthmodroll(int &aroll,creaturest &a);
/* adjusts attack damage based on armor, other factors */
void damagemod(creaturest &t,char &damtype,int &damamount,int mod);
/* destroys armor, masks, drops weapons based on severe damage */
void severloot(creaturest &cr,vector<itemst *> &loot);
/* damages the selected armor if it covers the body part specified */
void armordamage(armorst &armor,int bp);
/* blood explosions */
void bloodblast(armorst &armor);
/* kills the specified creature from the encounter, dropping loot */
void delenc(short e,char loot);
/* generates the loot dropped by a creature when it dies */
void makeloot(creaturest &cr,vector<itemst *> &loot);
/* abandoned liberal is captured by conservatives */
void capturecreature(creaturest &t);
/* checks if the creature can fight and prints flavor text if they can't */
char incapacitated(creaturest &a,char noncombat,char &printed);
/* describes a character's death */
void adddeathmessage(creaturest &cr);
/* pushes people into the current squad (used in a siege) */
void autopromote(int loc);

/*
 chase.cpp
*/
char chasesequence(void);
char footchase(void);
void evasivedrive(void);
void evasiverun(void);
long driveskill(creaturest &cr,vehiclest *v);
void drivingupdate(short &obstacle);
void makechasers(long sitetype,long sitecrime);
void obstacledrive(short obstacle,char choice);
void dodgedrive(void);
void crashfriendlycar(int v);
void crashenemycar(int v);
void chase_giveup(void);
/* the next two functions force a chase sequence with a specific liberal */
char footchase(creaturest &cr);
char chasesequence(creaturest &cr,vehiclest &v);

/*
 haulkidnap.cpp
*/
/* prompt after you've said you want to kidnap someone */
void kidnapattempt(void);
/* roll on the kidnap attempt and show the results */
char kidnap(creaturest &a,creaturest &t,char &amateur);
/* hostage freed due to host unable to haul */
void freehostage(creaturest &cr,char situation);
/* haul dead/paralyzed */
void squadgrab_immobile(char dead);
/* names the new hostage and stashes them in your base */
void kidnaptransfer(creaturest &cr);

/*******************************************************************************
*
*              End of Day Events
*              Folder: "daily"
*
*******************************************************************************/

/*
 daily.cpp
*/
void advanceday(char &clearformess,char canseethings);
/* squad members with no chain of command lose contact */
void dispersalcheck(char &clearformess);
/* daily - manages too hot timer and when a site map should be re-seeded and renamed */
void advancelocations(void);
/* daily - returns true if the site type supports high security */
char securityable(int type);
/* daily - seeds and names a site (will re-seed and rename if used after start) */
void initlocation(locationst &loc);
/* daily - returns the number of days in the current month */
int monthday(void);

/*
 activities.cpp
*/
/* adjust blog power */
void adjustblogpower(int &power);
/* hostage tending */
void tendhostage(creaturest *cr,char &clearformess);
/* armor repair */
void repairarmor(creaturest &cr,char &clearformess);
/* armor manufacture */
void makearmor(creaturest &cr,char &clearformess);
/* search for polls */
void survey(creaturest *cr);
/* misc activation related things */
void funds_and_trouble(char &clearformess);
/* steal a car */
char stealcar(creaturest &cr,char &clearformess);
long difficulty_carfind(int type);
char carselect(creaturest &cr,short &cartype);
long sensealarmchance(int ct);
long touchalarmchance(int ct);
/* get a wheelchair */
void getwheelchair(creaturest &cr,char &clearformess);

/*
 shopsnstuff.cpp
*/
/* active squad visits the hospital */
void hospital(int loc);
/* active squad visits the pawn shop */
void pawnshop(int loc);
/* active squad visits the department store */
void deptstore(int loc);
/* active squad visits the oubliette */
void halloweenstore(int loc);
/* oubliette - buy a mask */
char maskselect(creaturest *cr,short &mask);
/* pick stuff to fence */
unsigned long fenceselect(void);
/* value of stuff to fence */
unsigned long fencevalue(itemst &it);
/* choose buyer */
void choose_buyer(short &buyer);

/*
 date.cpp
*/
/* daily - date - dater p gets back from vacation */
char completevacation(datest &d,int p,char &clearformess);
/* daily - date - dater p goes on some dates */
char completedate(datest &d,int p,char &clearformess);

/*
 siege.cpp
*/
/* siege - updates upcoming sieges */
void siegecheck(char canseethings);
/* siege - updates sieges in progress */
void siegeturn(char clearformess);
/* siege - handles giving up */
void giveup(void);
/* siege - checks how many days of food left at the site */
int fooddaysleft(int loc);
/* siege - checks how many people are eating at the site */
int numbereating(int loc);
/* siege - prepares for entering site mode to fight the siege */
void escape_engage(void);
/* siege - what happens when you escaped the siege */
void escapesiege(char won);
/* siege - flavor text when you fought off the raid */
void conquertext(void);
/* siege - "you are wanted for _______ and other crimes..." */
void statebrokenlaws(int loc);

/*
 news.cpp
*/
/* news - determines the priority of a news story */
void setpriority(newsstoryst &ns);
/* news - show major news story */
void displaystory(newsstoryst &ns);
/* news - graphics */
void loadgraphics(void);
void displaycenterednewsfont(char *str,int y);
void displaycenteredsmallnews(char *str,int y);
void displaynewspicture(int p,int y);
/* news - constructs non-LCS related event stories */
void constructeventstory(char *story,short view,char positive);
/* news - draws the specified block of text to the screen */
void displaynewsstory(char *story,short *storyx_s,short *storyx_e,int y);
/* news - make some filler junk */
void generatefiller(char *story,int amount);
/* news - major newspaper reporting on lcs and other topics */
void majornewspaper(char &clearformess,char canseethings);

/*******************************************************************************
*
*                             End of Month Events
*                             Folder: "monthly"
*
*******************************************************************************/

/*
 monthly.cpp
*/
/* does end of month actions */
void passmonth(char &clearformess,char canseethings);
/* rename prison according to the new laws (add more buildings to this) */
void updateworld_laws(short *law,short *oldlaw);

/*
 lcsmonthly.cpp
*/
/* monthly - reports the guardian's power to the player */
void guardianupdate(char size,char power);
/* monthly - lets the player choose a special edition for the guardian */
int choosespecialedition(char &clearformess);
/* monthly - guardian - prints liberal guardian special editions */
void printnews(short l,short newspaper);
/* monthly - LCS finances report */
void fundreport(char &clearformess);
/* monthly - sleeper behavior */
void sleepereffect(creaturest &cr,char &clearformess,char canseethings,int *libpower);

/*
 justice.cpp
*/
/* monthly - hold trial on a liberal */
void trial(creaturest &g);
/* monthly - sentence a liberal */
void penalize(creaturest &g,char lenient);
/* monthly - move a liberal to jail */
void imprison(creaturest &g);
/* monthly - advances a liberal's prison time or executes them */
char prison(creaturest &g);

/*
 politics.cpp
*/
/* politics - causes the people to vote (presidential, congressional, propositions) */
void elections(char clearformess,char canseethings);
/* politics - causes the supreme court to hand down decisions */
void supremecourt(char clearformess,char canseethings);
/* politics - causes congress to act on legislation */
void congress(char clearformess,char canseethings);
/* politics - checks if the game is won */
char wincheck(void);
/* politics - checks the prevailing attitude on a specific law, or overall */
int publicmood(int l);

/*
 endgame.cpp
*/
/* endgame - attempts to pass a constitutional amendment to win the game */
void tossjustices(char canseethings);
/* endgame - attempts to pass a constitutional amendment to lose the game */
void reaganify(char canseethings);
/* endgame - checks if a constitutional amendment is ratified */
char ratify(int level,int view,int lawview,char congress,char canseethings);
/* endgame - header for announcing constitutional amendments */
void amendmentheading(void);
/* endgame - converts an integer into a roman numeral for amendments */
void romannumeral(int amendnum);


// Garbage bin

/* does not exist! */
//int testclearallsquads();
/* does not exist! */
//void chooseview(short &view);


#endif // INCLUDES_H_INCLUDED
