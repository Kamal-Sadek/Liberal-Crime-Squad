/*
    File created by Chris Johnson.
    These were previously all in game.cpp.

    This file includes all the the enums, defines, and included headers required globally by all components.

    All code released under GNU GPL.
*/
#ifndef INCLUDES_H_INCLUDED
#define INCLUDES_H_INCLUDED

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
   #include "curses.h"
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

struct creaturest
{
   char name[40];
   char propername[40];
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

struct highscorest
{
   char valid;
   char endtype;

   char slogan[80];
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

/*
    Every function prototype is here:
*/
void set_color(short f,short b,char bright);
void translategetch(int &c);
void translategetch_cap(int &c);

void mode_title(void);
void mode_base(void);
void mode_site(long loc);
void mode_site(void);
void printparty(void);
void printlocation(long loc);
void burnflag(void);
void getslogan(void);
void review(void);
void review_mode(short mode);
void assemblesquad(squadst *cursquad);
void squadlessbaseassign(void);
void promoteliberals(void);
void sortbyhire(vector<creaturest *> &temppool,vector<int> &level);
void orderparty(void);
void stopevil(void);
void initliberal(creaturest &cr);
void printliberalstats(creaturest &cr,char smll);
void initsite(locationst &loc);
void getactivity(char *str,activityst &act);
long getsquad(long id);
long id_getcar(long id);
void printsitemap(int x,int y,int z);
void printblock(int x,int y,int z,int px,int py);
void prepareencounter(short type,char sec);
void printencounter(void);
void printchaseencounter(void);
void makecreature(creaturest &cr,short type);
void generateroom(int rx,int ry,int dx,int dy,int z=0);
void knowmap(int locx,int locy,int locz);
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
void delenc(short e,char loot);
void youattack(void);
void enemyattack(void);
void attack(creaturest &a,creaturest &t,char mistake,char &actual);
void creatureadvance(void);
void advancecreature(creaturest &cr);
void damagemod(creaturest &t,char &damtype,int &damamount,int mod);
void makeloot(creaturest &cr,vector<itemst *> &loot);
void severloot(creaturest &cr,vector<itemst *> &loot);
void equip(vector<itemst *> &loot,int loc);
void consolidateloot(vector<itemst *> &loot);
char itemcompare(itemst *a,itemst *b);
short ammotype(int type);
void save(void);
void autosave(void);
char load(void);
void choose_buyer(short &buyer);
void conservatise(creaturest &cr);
char talk(creaturest &a,int t);
void liberalize(creaturest &cr);
void kidnapattempt(void);
char kidnap(creaturest &a,creaturest &t,char &amateur);
void kidnaptransfer(creaturest &cr);
void namecreature(creaturest &cr);
void name(char *str);
void firstname(char *str);
void lastname(char *str);
void noticecheck(int exclude);
void disguisecheck(void);
int disguiseskill(void);
char weaponcheck(creaturest &cr,short type);
char hasdisguise(creaturest &cr,short type);
char alienationcheck(char mistake);
void survey(creaturest *cr);
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
void clearcommandarea(void);
void clearmessagearea(void);
void clearmaparea(void);
char unlock(short type,char &actual);
char bash(short type,char &actual);
long bashstrengthmod(int t);
char hack(short type,char &actual);
char liberalagenda(char won);
char confirmdisband(void);
char radio_broadcast(void);
char news_broadcast(void);
void change_public_opinion(int v,int power,char affect,char cap=1);
void resolvesite(void);
void passmonth(char &clearformess,char canseethings);
void dispersalcheck(char &clearformess);
void locatesquad(squadst *st,long loc);
void basesquad(squadst *st,long loc);
int monthday(void);
void advanceday(char &clearformess,char canseethings);
void majornewspaper(char &clearformess,char canseethings);
void removesquadinfo(creaturest &cr);
void investlocation(void);
void cleangonesquads(void);
void advancelocations(void);
char securityable(int type);
void initlocation(locationst &loc);
void loadhighscores(void);

void savehighscore(char endtype);
void viewhighscores(void);
void reset(void);
void elections(char clearformess,char canseethings);
void supremecourt(char clearformess,char canseethings);
void congress(char clearformess,char canseethings);
char wincheck(void);
int publicmood(int l);
void getview(char *str,short view);
void getlaw(char *str,int l);
void chooseview(short &view);
void guardianupdate(char size,char power);
int choosespecialedition(char &clearformess);
unsigned long fenceselect(void);
unsigned long fencevalue(itemst &it);
void moveloot(vector<itemst *> &dest,vector<itemst *> &source);
void tossjustices(char canseethings);
void reaganify(char canseethings);
char ratify(int level,int view,int lawview,char congress,char canseethings);
void amendmentheading(void);
void romannumeral(int amendnum);
char incapacitated(creaturest &a,char noncombat,char &printed);
void printhealthstat(creaturest &g,int y,int x,char smll);
void healthmodroll(int &aroll,creaturest &a);
int clinictime(creaturest &g);
void siegecheck(char canseethings);
void siegeturn(char clearformess);
void giveup(void);
int fooddaysleft(int loc);
int numbereating(int loc);
void escape_engage(void);
void autopromote(int loc);
void escapesiege(char won);
char addsiegeencounter(char type);
void conquertext(void);
void criminalizeparty(short crime);
void criminalizepool(short crime,long exclude=-1,short loc=-1);
void juiceparty(long juice);
void addjuice(creaturest &cr,long juice,long cap=1000);
void statebrokenlaws(int loc);
char rangedweapon(weaponst &w);
void trial(creaturest &g);
char prison(creaturest &g);
void partyrescue(void);
void penalize(creaturest &g,char lenient);
void printnews(short l,short newspaper);
void adddeathmessage(creaturest &cr);
void fullstatus(int p);
void makecharacter(void);
void hospital(int loc);
void pawnshop(int loc);
void deptstore(int loc);
void halloweenstore(int loc);
void locheader(void);
void activate(void);
void activatebulk(void);
void printcreatureinfo(creaturest *cr);
void activate(creaturest *cr);
void makedelimiter(int y,int x);
int getpoolcreature(long id);
void select_tendhostage(creaturest *cr);
long select_hostagefundinglevel(creaturest *cr,creaturest *hs);
void select_makeclothing(creaturest *cr);
long select_troublefundinglevel(creaturest *cr);
char select_view(creaturest *cr,long &v);
void tendhostage(creaturest *cr,char &clearformess);
char disguisesite(long type);
void armordamage(armorst &armor,int bp);
void bloodblast(armorst &armor);
char squadhasitem(squadst &sq,int type,int subtype);
int getrandomcreaturetype(int cr[CREATURENUM]);
long armor_makedifficulty(int type,creaturest *cr);
long armor_makeprice(int type);
void repairarmor(creaturest &cr,char &clearformess);
void makearmor(creaturest &cr,char &clearformess);
void funds_and_trouble(char &clearformess);
void fundreport(char &clearformess);
void getcarfull(char *str,int type);
void getcar(char *str,int type);
void getcarfull(char *str,vehiclest &car,char halffull=0);
char stealcar(creaturest &cr,char &clearformess);
void getwheelchair(creaturest &cr,char &clearformess);
long difficulty_carfind(int type);
char carselect(creaturest &cr,short &cartype);
void getcarcolor(char *str,int type);
short naturalcarcolor(int type);
char maskselect(creaturest *cr,short &mask);
long sensealarmchance(int ct);
long touchalarmchance(int ct);
void setvehicles(void);
void squadgrab_immobile(char dead);
void freehostage(creaturest &cr,char situation);
void capturecreature(creaturest &t);
char chasesequence(void);
char footchase(void);
char chasesequence(creaturest &cr,vehiclest &v);
char footchase(creaturest &cr);
char endcheck(short cause=-1);
void makechasers(long sitetype,long sitecrime);
void evasivedrive(void);
void evasiverun(void);
long driveskill(creaturest &cr,vehiclest *v);
void drivingupdate(short &obstacle);
void obstacledrive(short obstacle,char choice);
void dodgedrive(void);
void crashfriendlycar(int v);
void crashenemycar(int v);
void chase_giveup(void);
void verifyworklocation(creaturest &cr);
void sleepereffect(creaturest &cr,char &clearformess,char canseethings,int *libpower);
void setpriority(newsstoryst &ns);
void displaystory(newsstoryst &ns);
void loadgraphics(void);
void displaycenterednewsfont(char *str,int y);
void displaycenteredsmallnews(char *str,int y);
void displaynewspicture(int p,int y);
void constructeventstory(char *story,short view,char positive);
void displaynewsstory(char *story,short *storyx_s,short *storyx_e,int y);
void cityname(char *story);
char talkreceptive(creaturest &cr);
void generatefiller(char *story,int amount);
char kid(creaturest &cr);
char completevacation(datest &d,int p,char &clearformess);
char completedate(datest &d,int p,char &clearformess);
void updateworld_laws(short *law,short *oldlaw);
void reloadparty(void);
void imprison(creaturest &g);
void addlocationname(locationst *loc);
void loadinitfile(void);
void removecarprefs_pool(long carid);
bool iscriminal(creaturest &cr);
int testclearallsquads();
int testsquadclear(squadst &thissquad, int obase);
void hospitalize(int loc, creaturest &patient);
int skillatt(int skill);


#endif // INCLUDES_H_INCLUDED
