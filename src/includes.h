/*
    File created by Chris Johnson.
    These were previously all in game.cpp.

    This file includes all the the enums, defines, and included headers required globally by all components.

    All code released under GNU GPL.
*/
#ifndef INCLUDES_H_INCLUDED
#define INCLUDES_H_INCLUDED


/*
   DEBUG DEFINES
*/
// Enemies don't attack
//#define NOENEMYATTACK

// Laws start archconservative
//#define SHITLAWS

// Laws start elite liberal
//#define PERFECTLAWS

// Public opinion starts at 100% Liberal
//#define REVOLUTIONNOW

// Gives you bloody armor
//#define GIVEBLOODYARMOR

// Start with lots of money
//#define HIGHFUNDS

// Interrogation always enlightens
//#define AUTOENLIGHTEN

// Show polls when you can't see things
//#define SHOWWAIT

// Show die rolls, 100% accurate poll numbers
//#define SHOWMECHANICS

// Make the year 2100
//#define THEFUTURE

// Make the founder blind
//#define BLIND

// Make the founder unable to walk
//#define NOWALK

// Make the founder have no face
//#define NOFACE

// Make the founder have a severely injured spine
//#define SPINE

// Make the founder have severe internal damage
//#define INTERNAL


#ifdef HAVE_CONFIG_H
#include <config.h>
#include <math.h>
#include <stdlib.h>
#include <langinfo.h>
#endif

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "4.00.5"
#endif

const int version=40005;
const int lowestloadversion=40000;
const int lowestloadscoreversion=31203;

#ifdef WIN32
   #include <windows.h>
   #include <string.h>
   //Visual C++ .NET (7) includes the STL with vector, so we
   //will use that, otherwise the HP STL Vector.h will be used.
   #ifdef __MINGW32__
      #include <iostream>
      #include <fstream>
      #include <vector>
      #include <map>
      #include <io.h> //needed for unlink()
   #else
      #if _MSC_VER > 1200
         #define WIN32_DOTNET
         #include <iostream>
         #include <fstream>
         #include <vector>
         #include <map>
      #else
         #define WIN32_PRE_DOTNET
         #include <iostream.h>
         #include <fstream.h>
         #include "vector.h"
         #include "map.h"
      #endif
   #endif

   #include <curses.h>
   //undo PDCurses macros that break vector class
   #undef erase
   #undef clear

   #define CH_USE_CP437
#else
   #include <vector>
   #include <map>
   #include <string.h>
   #include <iostream>
   #include <fstream>
   #include <ctype.h>
   #define GO_PORTABLE

   #if defined(HAVE_WIDE_NCURSES) && defined(__STDC_ISO_10646__)
     #define CH_USE_UNICODE
   #else
     #define CH_USE_ASCII_HACK
   #endif

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
      #if defined(USE_NCURSES)
         #include <ncurses.h>
      #elif defined(USE_NCURSES_W)
         #include <ncursesw/ncurses.h>
      #else
         #error "You must define either USE_NCURSES or USE_NCURSES_W."
      #endif
   #endif

   #ifdef CH_USE_UNICODE
     // Make sure we don't override addch for consolesupport.cpp,
     // because addch_unicode may use addch internally.
     #ifndef CONSOLE_SUPPORT
        #undef addch
        #define addch(a) addch_unicode(a)
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
#include "politics/alignment.h"



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

 inline unsigned int getSeed(void)
 {
 unsigned int t;

 #ifdef GO_PORTABLE

 t = (unsigned int)time(NULL); /* Seconds since 1970-01-01 00:00:00 */

 #else // WIN32

 t = (unsigned int)GetTickCount(); /* ms since system boot */

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


/* Macro definition */
#ifndef MAX
   #define MAX(a,b) (((a)<(b))?(b):(a))
#endif

#ifndef MIN
   #define MIN(a,b) (((a)>(b))?(b):(a))
#endif



#ifndef NDEBUG
#define NDEBUG
#endif

#define BIT1  (1<<0 )
#define BIT2  (1<<1 )
#define BIT3  (1<<2 )
#define BIT4  (1<<3 )
#define BIT5  (1<<4 )
#define BIT6  (1<<5 )
#define BIT7  (1<<6 )
#define BIT8  (1<<7 )
#define BIT9  (1<<8 )
#define BIT10 (1<<9 )
#define BIT11 (1<<10)
#define BIT12 (1<<11)
#define BIT13 (1<<12)
#define BIT14 (1<<13)
#define BIT15 (1<<14)
#define BIT16 (1<<15)
#define BIT17 (1<<16)
#define BIT18 (1<<17)
#define BIT19 (1<<18)
#define BIT20 (1<<19)
#define BIT21 (1<<20)
#define BIT22 (1<<21)
#define BIT23 (1<<22)
#define BIT24 (1<<23)
#define BIT25 (1<<24)
#define BIT26 (1<<25)
#define BIT27 (1<<26)
#define BIT28 (1<<27)
#define BIT29 (1<<28)
#define BIT30 (1<<29)
#define BIT31 (1<<30)
#define BIT32 (1<<31)



int r_num(void);
int LCSrandom(int max);

enum UnlockTypes
{
   UNLOCK_DOOR,
   UNLOCK_CAGE,
   UNLOCK_CAGE_HARD,
   UNLOCK_CELL,
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
   SITE_COMMERCIAL,
   SITE_UDISTRICT,
   SITE_OUTOFTOWN,
   SITE_INDUSTRIAL,
   SITE_RESIDENTIAL_TENEMENT,
   SITE_RESIDENTIAL_APARTMENT,
   SITE_RESIDENTIAL_APARTMENT_UPSCALE,
   SITE_RESIDENTIAL_SHELTER,
   SITE_RESIDENTIAL_BOMBSHELTER,
   SITE_LABORATORY_COSMETICS,
   SITE_LABORATORY_GENETIC,
   SITE_HOSPITAL_CLINIC,
   SITE_HOSPITAL_UNIVERSITY,
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
   SITE_BUSINESS_PAWNSHOP,
   SITE_BUSINESS_CRACKHOUSE,
   SITE_BUSINESS_JUICEBAR,
   SITE_BUSINESS_CIGARBAR,
   SITE_BUSINESS_LATTESTAND,
   SITE_BUSINESS_VEGANCOOP,
   SITE_BUSINESS_INTERNETCAFE,
   SITE_BUSINESS_DEPTSTORE,
   SITE_BUSINESS_HALLOWEEN,
   SITE_BUSINESS_BARANDGRILL,
   SITE_BUSINESS_ARMSDEALER,
   SITE_BUSINESS_CARDEALERSHIP,
   SITE_OUTDOOR_PUBLICPARK,
   SITE_OUTDOOR_BUNKER,
   SITENUM
};

enum CreatureType
{
   CREATURE_BOUNCER,
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
   CREATURE_FIREFIGHTER,
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
   CREATURE_VETERAN,
   CREATURE_HARDENED_VETERAN,
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
   CREATURE_MARTIALARTIST,
   CREATURE_ATHLETE,
   CREATURE_BIKER,
   CREATURE_TRUCKER,
   CREATURE_TAXIDRIVER,
   CREATURE_NUN,
   CREATURE_MUTANT,
   CREATURE_DOCTOR,
   CREATURE_NURSE,
   CREATURE_CCS_VIGILANTE,
   CREATURE_CCS_ARCHCONSERVATIVE,
   CREATURE_POLITICALACTIVIST,
   CREATURE_CCS_MOLOTOV,
   CREATURE_CCS_SNIPER,
   CREATURE_PSYCHOLOGIST,
   CREATURENUM
};

enum endgame
{
   ENDGAME_NONE,
   ENDGAME_CCS_APPEARANCE,
   ENDGAME_CCS_ATTACKS,
   ENDGAME_CCS_SIEGES,
   ENDGAME_CCS_DEFEATED,
   ENDGAME_MARTIALLAW,
   ENDGAMENUM
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
   SKILL_SWORD,
   SKILL_CLUB,
   SKILL_AXE,
   SKILL_THROWING,
   SKILL_PISTOL,
   SKILL_RIFLE,
   SKILL_SMG,
   SKILL_SHOTGUN,
   SKILL_PERSUASION,
   SKILL_PSYCHOLOGY,
   SKILL_LAW,
   SKILL_SECURITY,
   SKILL_DISGUISE,
   SKILL_COMPUTERS,
   SKILL_TAILORING,
   SKILL_DRIVING,
   SKILL_WRITING,
   SKILL_COOKING,
   SKILL_MUSIC,
   SKILL_ART,
   SKILL_RELIGION,
   SKILL_SCIENCE,
   SKILL_BUSINESS,
   SKILL_STEALTH,
   SKILL_THEFT,
   SKILL_TEACHING,
   SKILL_STREETSENSE,
   SKILL_SEDUCTION,
   SKILL_LEADERSHIP,
   SKILL_FIRSTAID,
   SKILL_DODGE,
   SKILL_FLAMETHROWER,
   //SKILL_SURVIVAL,
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
   WEAPON_REVOLVER_38,
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
   WEAPON_DAISHO,
   WEAPON_HAMMER,
   WEAPON_MAUL,
   WEAPON_CROSS,
   WEAPON_STAFF,
   WEAPON_PITCHFORK,
   WEAPON_TORCH,
   WEAPON_GUITAR,
   WEAPON_SPRAYCAN,
   WEAPON_MOLOTOV,
   WEAPON_AXE,
   WEAPON_FLAMETHROWER,
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
         case WEAPON_REVOLVER_38:
         case WEAPON_REVOLVER_44:
         case WEAPON_SEMIPISTOL_9MM:
         case WEAPON_SEMIPISTOL_45:
         case WEAPON_AUTORIFLE_M16:
         case WEAPON_AUTORIFLE_AK47:
         case WEAPON_SEMIRIFLE_AR15:
         case WEAPON_CARBINE_M4:
         case WEAPON_SMG_MP5:
         case WEAPON_SHOTGUN_PUMP:
         case WEAPON_FLAMETHROWER:
            return 1;
      }
      return 0;
   }

   weaponst()
   {
      ammo=0;
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
   MASK_GUY_FAWKES,
   MASK_TED_STEVENS,
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
   ARMOR_CIVILLIANARMOR,
   ARMOR_POLICEARMOR,
   ARMOR_SWATARMOR,
   ARMOR_ARMYARMOR,
   ARMOR_HEAVYARMOR,
   ARMOR_BUNKERGEAR,
   ARMOR_ELEPHANTSUIT,
   ARMOR_DONKEYSUIT,
   ARMOR_DEATHSQUADUNIFORM,
   ARMORNUM
};

#define ARMORFLAG_DAMAGED BIT1
#define ARMORFLAG_BLOODY BIT2

struct armorst
{
   short type;
   short subtype;
   char quality;
   short flag;

   armorst()
   {
      quality='1';
      flag=0;
   }

   int interrogation_basepower();
   int interrogation_assaultbonus();
   int interrogation_drugbonus();
   int professionalism();
   bool faceconcealed();
};

enum ClipType
{
   CLIP_9,
   CLIP_45,
   CLIP_ASSAULT,
   CLIP_SMG,
   CLIP_38,
   CLIP_44,
   CLIP_BUCKSHOT,
   CLIP_MOLOTOV,
   CLIP_GASOLINE,
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
   LOOT_JUDGEFILES,
   LOOT_RESEARCHFILES,
   LOOT_PRISONFILES,
   LOOT_CABLENEWSFILES,
   LOOT_AMRADIOFILES,
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
   LAWFLAG_ARMEDASSAULT,
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
   LAWFLAG_GUNUSE,
   LAWFLAG_ARSON,
   LAWFLAG_PUBLICNUDITY,
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
   ACTIVITY_GRAFFITI,
   ACTIVITY_COMMUNITYSERVICE,
   ACTIVITY_SELL_ART,
   ACTIVITY_SELL_MUSIC,
   ACTIVITY_SELL_TSHIRTS,
   ACTIVITY_DONATIONS,
   ACTIVITY_SELL_DRUGS,
   ACTIVITY_PROSTITUTION,
   ACTIVITY_POLLS,
   ACTIVITY_CCFRAUD,
   ACTIVITY_DOS_RACKET,
   ACTIVITY_DOS_ATTACKS,
   ACTIVITY_HACKING,
   ACTIVITY_REPAIR_ARMOR,
   ACTIVITY_MAKE_ARMOR,
   ACTIVITY_STEALCARS,
   ACTIVITY_WHEELCHAIR,
   ACTIVITY_BURY,
   ACTIVITY_WRITE_BLOG,
   ACTIVITY_WRITE_LETTERS,
   ACTIVITY_WRITE_GUARDIAN,
   ACTIVITY_TEACH_GENERALED,
   ACTIVITY_TEACH_POLITICS,
   ACTIVITY_TEACH_SURVIVAL,
   ACTIVITY_TEACH_FIGHTING,
   ACTIVITY_TEACH_COVERT,
   ACTIVITY_CLINIC,
   ACTIVITY_HEAL,
   ACTIVITY_SLEEPER_LIBERAL,
   ACTIVITY_SLEEPER_CONSERVATIVE,
   ACTIVITY_SLEEPER_SPY,
   ACTIVITY_SLEEPER_RECRUIT,
   ACTIVITY_SLEEPER_SCANDAL,
   ACTIVITY_SLEEPER_EMBEZZLE,
   ACTIVITY_SLEEPER_STEAL,
   ACTIVITY_SLEEPER_JOINLCS,
   ACTIVITYNUM
};

struct activityst
{
   activityst() : type(0), arg(0), arg2(0) { }
   int type;
   long arg;
   long arg2;
};

#define CREATUREFLAG_WHEELCHAIR BIT1
#define CREATUREFLAG_JUSTESCAPED BIT2
#define CREATUREFLAG_MISSING BIT3
#define CREATUREFLAG_KIDNAPPED BIT4
#define CREATUREFLAG_SLEEPER BIT5
#define CREATUREFLAG_ILLEGALALIEN BIT6
#define CREATUREFLAG_LOVESLAVE BIT7
#define CREATUREFLAG_BRAINWASHED BIT8
#define CREATUREFLAG_CONVERTED BIT9

#define CREATURE_NAMELEN 40

#define MAXSUBORDINATES 30

#define GENDER_NEUTRAL 0
#define GENDER_MALE    1
#define GENDER_FEMALE  2
#define GENDER_WHITEMALEPATRIARCH 3

class Creature
{
private:
   int skill_ip[SKILLNUM]; // Use public training interface!! >:0
public:
   char name[CREATURE_NAMELEN];
   char propername[CREATURE_NAMELEN];
   char gender_conservative;
   char gender_liberal;
   int squadid;//REMEMBER, THIS IS ID NUMBER, NOT ARRAY INDEX
   int age;
   int birthday_month;
   int birthday_day;
   bool exists;
   char align;
   bool alive;
   short type;
   float infiltration;
   char animalgloss;
   short specialattack;
   short clinic;
   short dating;
   short hiding;
   short trainingtime;
   short trainingsubject;
   Creature *prisoner;
   short sentence;
   char confessions;
   char deathpenalty;
   int joindays;
   int deathdays;
   int id;
   int hireid;

   char forceinc;

   int att[ATTNUM]; // allow negatives in raw data, but check through attval()
   int skill[SKILLNUM]; // should never decrease, so never be negative

   void train(int trainedskill, int experience);
   void skill_up();
   int get_skill_ip(int skill);

   bool enemy();

   int stunned;

   weaponst weapon;
   armorst armor;
   int clip[CLIPNUM];

   int money;
   int juice;

   unsigned char wound[BODYPARTNUM];
   short blood;
   char special[SPECIALWOUNDNUM];

   int lawflag[LAWFLAGNUM];
   int heat;
   int location;
   int worklocation;

   char cantbluff;

   int base;
   activityst activity;

   int carid;
   char is_driver;
   int pref_carid;
   char pref_is_driver;
   short flag;

   Creature()
   {
      creatureinit();
   }
   ~Creature()
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
   int attval(short a,char usejuice=1);
   int skillval(int skill_to_lookup) { return skill[skill_to_lookup]; }

   /* are they interested in talking about the issues? */
   bool talkreceptive();
   /* are the characters close enough in age to date? */
   bool can_date(Creature &a);
   /* rolls up a proper name for a creature */
   void namecreature();
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
   SPECIAL_PARK_BENCH,
   SPECIAL_STAIRS_UP,
   SPECIAL_STAIRS_DOWN,
   SPECIAL_CLUB_BOUNCER,
   SPECIAL_CLUB_BOUNCER_SECONDVISIT,
   SPECIALNUM
};

#define SIEGEFLAG_UNIT BIT1
#define SIEGEFLAG_TRAP BIT2
#define SIEGEFLAG_HEAVYUNIT BIT3
#define SIEGEFLAG_UNIT_DAMAGED BIT4

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
   int money;
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
   int parent;
   vector<itemst *> loot;
   vector<sitechangest> changes;
   int renting;
   char newrental;
   char needcar;
   short closed;
   char interrogated;
   int highsecurity;
   siegest siege;
   int heat;
   double heat_protection;
   char compound_walls;
   int compound_stores;
   short front_business;
   char front_name[40];
   char haveflag;
   char hidden;

   int mapseed;

   locationst()
   {
      init();
      needcar=0;
      renting=-1;
      hidden=0;
   }
   void init(void);
   void update_heat_protection(void);
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
   short heat;
   long location;
   int myear;
   long id;

   void init(int t);
};


enum CarChaseObstacles
{
   CARCHASE_OBSTACLE_FRUITSTAND,
   CARCHASE_OBSTACLE_TRUCKPULLSOUT,
   CARCHASE_OBSTACLE_CROSSTRAFFIC,
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

enum SquadStances
{
   SQUADSTANCE_ANONYMOUS,
   SQUADSTANCE_STANDARD,
   SQUADSTANCE_BATTLECOLORS,
   SQUADSTANCE_MAX
};

struct squadst
{
   char name[40];
   Creature *squad[6];
   activityst activity;
   int id;
   vector<itemst *> loot;

   char stance; // Squad's site action stance: high profile, low profile, etc.

   squadst()
   {
      for(int p=0;p<6;p++)squad[p]=NULL;
      strcpy(name,"");
      activity.type=ACTIVITY_NONE;
      id=-1;
      stance=SQUADSTANCE_STANDARD;
   }
   ~squadst()
   {
      for(int l=0;l<loot.size();l++)
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
   VIEW_GAY,
   VIEW_DEATHPENALTY,
   VIEW_TAXES,
   VIEW_NUCLEARPOWER,
   VIEW_ANIMALRESEARCH,
   VIEW_POLICEBEHAVIOR,
   VIEW_PRISONS,//XXX: Please don't code with the nonexistant VIEW_PRISON
   VIEW_INTELLIGENCE,
   VIEW_FREESPEECH,
   VIEW_GENETICS,
   VIEW_JUSTICES,
   VIEW_GUNCONTROL,
   VIEW_SWEATSHOPS,
   VIEW_POLLUTION,
   VIEW_CORPORATECULTURE,
   VIEW_CEOSALARY,
   VIEW_WOMEN,//XXX: VIEW_ABORTION DOES NOT EXIST
   VIEW_CIVILRIGHTS,
   VIEW_DRUGS,
   VIEW_IMMIGRATION,
   VIEW_MILITARY,
   //*JDS* I'm using VIEWNUM-6 in a random generator that rolls a
   //random issue, not including the media/politicalviolence ones, and this will
   //break if these stop being the last 4 issues; do a search
   //for VIEWNUM-6 to change it if it needs to be changed.
   VIEW_AMRADIO,
   VIEW_CABLENEWS,
   VIEW_POLITICALVIOLENCE,
   //THESE THREE MUST BE LAST FOR VIEWNUM-3 TO WORK IN PLACES
   VIEW_LIBERALCRIMESQUAD,
   VIEW_LIBERALCRIMESQUADPOS,
   VIEW_CONSERVATIVECRIMESQUAD,
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
   LAW_WOMEN,
   LAW_CIVILRIGHTS,
   LAW_DRUGS,
   LAW_IMMIGRATION,
   LAW_ELECTIONS,
   LAW_MILITARY,
   LAW_TORTURE,
   LAWNUM
};

struct datest
{
   long mac_id;
   vector<Creature *> date;
   short timeleft;
   datest()
   {
      timeleft=0;
   }
   ~datest()
   {
      for(int d=0;d<date.size();d++)delete date[d];
      date.clear();
   }
};

enum RecruitTasks
{
   TASK_NONE,
   TASK_COMMUNITYSERVICE,
   TASK_ACTIVISM,
   TASK_CRIMES,
   TASK_BUYWEAPON,
   TASK_ARRESTED,
   TASKNUM
};

struct recruitst
{
   long recruiter_id;
   Creature* recruit;
   short timeleft;
   char level;
   char eagerness1;
   char task;
   recruitst();
   char eagerness();
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
   CRIME_ARSON,
   CRIME_TAGGING,
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
   NEWSSTORY_CCS_SITE,
   NEWSSTORY_CCS_DEFENDED,
   NEWSSTORY_CCS_KILLED_SIEGEATTACK,
   NEWSSTORY_CCS_KILLED_SITE,
   NEWSSTORY_CARTHEFT,
   NEWSSTORY_MASSACRE,
   NEWSSTORY_KIDNAPREPORT,
   NEWSSTORY_NUDITYARREST,
   NEWSSTORY_WANTEDARREST,
   NEWSSTORY_DRUGARREST,
   NEWSSTORY_GRAFFITIARREST,
   NEWSSTORY_BURIALARREST,
   NEWSSTORY_RAID_CORPSESFOUND,
   NEWSSTORY_RAID_GUNSFOUND,
   NEWSSTORY_HOSTAGE_RESCUED,
   NEWSSTORY_HOSTAGE_ESCAPES,
   NEWSSTORYNUM
};

struct newsstoryst
{
   short type;
   short view;
   char claimed;
   short politics_level;
   short violence_level;
   Creature *cr;
   vector<int> crime;
   long loc,priority,page,guardianpage;
   char positive;
   short siegetype;
   newsstoryst()
   {
      claimed=1;
      politics_level=0;
      violence_level=0;
      cr=NULL;
   }
};

#define SLOGAN_LEN 79

struct highscorest
{
   char valid;
   char endtype;

   char slogan[SLOGAN_LEN+1];
   int month;
   int year;
   int stat_recruits;
   int stat_kidnappings;
   int stat_dead;
   int stat_kills;
   int stat_funds;
   int stat_spent;
   int stat_buys;
   int stat_burns;
};

//just a float that is initialized to 0
struct float_zero
{
   float_zero() : n(0.0f) {};
   operator float&() { return n; };
   float n;
};
//Interrogation information for the interrogation system, to be
//dynamically created on capture and deleted when interrogation ends,
//referenced using a pointer typecast into one of the arguments
//of the target's current action.
struct interrogation
{
   interrogation() : nofood(0), nowater(0), nosleep(0), nolight(0),
      totalspiritcrush(0), druguse(0)
   {
      techniques[0]=1;
      techniques[1]=1;
      techniques[2]=0;
      techniques[3]=0;
      techniques[4]=0;
      techniques[5]=0;
      techniques[6]=0;
      techniques[7]=0;
      techniques[8]=0;
   };

   int nofood;  //days without food
   int nowater; //days without water
   int nosleep; //days without sleep
   int nolight; //days without light

   bool techniques[9]; //yesterday's interrogation plan

   int totalspiritcrush; //total amount of spirit crush applied over time

   int druguse; //total days of drug use

   //Maps individual characters to the rapport built with them
   map<long,struct float_zero> rapport;
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
   END_CCS,
   END_FIREMEN,
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

#define RENTING_CCS -2
#define RENTING_NOCONTROL -1
#define RENTING_PERMANENT 0

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
// Sets the text color to the thematic color for the given alignment
// extended_range forces colors to be set on a 5 point scale instead
// of just basic liberal-moderate-conservative
void set_alignment_color(signed char alignment, bool extended_range=false);
/* Sets the text color per activity type */
void set_activity_color(long activity_type);
/* location and squad header */
void locheader(void);
/* party info at top of screen */
void printparty(void);
/* location info at top of screen */
void printlocation(long loc);
/* character info at top of screen */
void printcreatureinfo(Creature *cr,unsigned char knowledge=255);
/* full character sheet (with surrounding interface) */
void fullstatus(int p);
/* full screen character sheet, just skills */
void printliberalskills(Creature &cr);
/* full screen character sheet */
void printliberalstats(Creature &cr);
/* draws a horizontal line across the screen */
void makedelimiter(int y,int x);
/* print location name (is aware of business fronts) */
void addlocationname(locationst *loc);
/* prints a character's health description (One Leg, Liberal, NearDETH...) */
void printhealthstat(Creature &g,int y,int x,char smll);
/* prints amount of money the player has, with optional formatting */
void printfunds(int y, int offset, char* prefix=NULL);
/* prints a short blurb showing how to page forward */
void addnextpagestr();
/* prints a short blurb showing how to page back */
void addprevpagestr();
/* prints a long blurb showing how to page forward and back */
void addpagestr();

/*
 commonactions.cpp
*/
/* common - test for possible game over */
char endcheck(char cause=-1);
/* common - detatches all liberals from a specified car */
void removecarprefs_pool(long carid);
/* common - tests if the person is a wanted criminal */
bool iscriminal(Creature &cr);
/* common - sends somebody to the hospital */
void hospitalize(int loc, Creature &patient);
/* common - determines how long a creature's injuries will take to heal */
int clinictime(Creature &g);
/* common - purges squad of loot and vehicles if it has no members */
int testsquadclear(squadst &thissquad, int obase);
/* common - returns the creature's maximum level in the given skill */
int maxskill(int skill, Creature& cr, bool use_juice=true);
/* common - returns the associated skill for the given weapon type */
int weaponskill(int weapon);
/* common - applies a crime to everyone in the active party */
void criminalizeparty(short crime);
/* common - applies a crime to everyone in a location, or the entire LCS */
void criminalizepool(short crime,long exclude=-1,short loc=-1);
/* common - applies a crime to a person */
void criminalize(Creature &cr,short crime);
/* common - gives juice to everyone in the active party */
void juiceparty(long juice);
/* common - gives juice to a given creature */
void addjuice(Creature &cr,long juice,long cap=1000);
/* common - removes the liberal from all squads */
void removesquadinfo(Creature &cr);
/* common - purges empty squads from existance */
void cleangonesquads(void);
/* common - moves all squad members and their cars to a new location */
void locatesquad(squadst *st,long loc);
/* common - assigns a new base to all members of a squad */
void basesquad(squadst *st,long loc);
/* common - shifts public opinion on an issue */
void change_public_opinion(int v,int power,char affect=1,char cap=100);
/* returns the amount of heat associated with a given crime */
int lawflagheat(int lawflag);
/* Determines the number of subordinates a creature may command */
int maxsubordinates(const Creature& cr);
/* Determines the number of subordinates a creature may recruit,
   based on their max and the number they already command */
int subordinatesleft(const Creature& cr);
// Determines the number of love slaves a creature has
int loveslaves(const Creature& cr);
/* Determines the number of loveslaves a creature may recruit,
   based on their max and the number they already command */
int loveslavesleft(const Creature& cr);
/* common - random issue by public interest */
int randomissue(bool core_only=0);
// Picks a random option, based on the weights provided
int choose_one(const int * weight_list, int number_of_options, int default_value);
/* pick a random state */
void makestate(char *str);

/*
 consolesupport.cpp
*/
void set_color(short f,short b,char bright,char blink=0);
void translategetch(int &c);
void translategetch_cap(int &c);
#ifdef CH_USE_UNICODE
  char setup_unicode();
  int addch_unicode(int c);
#endif

/*
 getnames.cpp
*/
void getactivity(char *str,activityst &act);
void getweapon(char *str,int type);
void getweaponfull(char *str,int type,int subtype=0);
void getarmor(char *str,int type,int subtype=-1);
void getarmorfull(char *str,armorst &armor,char superfull);
void getarmorfull(char *str,int type,int subtype=-1);
void getmaskdesc(char *str,short mask);
void getskill(char *str,int type);
void getclip(char *str,int clip);
void getloot(char *str,int loot);
void getrecruitcreature(char *str,int type);
void gettitle(char *str,Creature &cr);
void getview(char *str,short view);
void getviewsmall(char *str,short view);
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
int id_getcar(int id);
/* transforms a creature id number into the index of that person in the pool */
int getpoolcreature(long id);

/*
 equipment.cpp
*/
/* review squad equipment */
void equip(vector<itemst *> &loot,int loc);
/* lets you pick stuff to stash/retrieve from one location to another */
void moveloot(vector<itemst *> &dest,vector<itemst *> &source);
/* equipment - assign new bases to the equipment */
void equipmentbaseassign(void);
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
// Add an age estimate to a person's name
void add_age(Creature& person);
/* rolls up a creature's stats and equipment */
void makecreature(Creature &cr,short type);
/* fills a string with a proper name */
void generate_name(char *str, char gender = GENDER_NEUTRAL);
/* gets a random first name */
void firstname(char *str, char gender = GENDER_NEUTRAL);
/* gets a random last name */
void lastname(char *str);
/* ensures that the creature's work location is appropriate to its type */
bool verifyworklocation(Creature &cr, char test_location=-1, char test_type=-1);
/* turns a creature into a conservative */
void conservatise(Creature &cr);
/* turns a creature into a liberal */
void liberalize(Creature &cr,bool rename=true);
/* gives a cover name to CCS members */
void nameCCSMember(Creature &cr);

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
/* new game options screen */
void setup_newgame(void);
/* creates your founder */
void makecharacter(void);
/* mostly depricated, but called once by makecharacter */
void initliberal(Creature &cr);

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
 activate_sleepers.cpp
*/
/* base - activate the uninvolved */
void activate_sleepers(void);
void activate_sleeper(Creature *cr);

/*
 activate.cpp
*/
/* base - activate the uninvolved */
void activate(void);
void activate(Creature *cr);
void activatebulk(void);
/* base - activate - hostages */
void select_tendhostage(Creature *cr);
long select_hostagefundinglevel(Creature *cr,Creature *hs);
/* base - activate - make clothing */
void select_makeclothing(Creature *cr);
int armor_makedifficulty(int type,Creature *cr);
int armor_makeprice(int type);
/* base - activate - trouble */
long select_troublefundinglevel(Creature *cr);
/* base - activate - select a topic to write about */
char select_view(Creature *cr,int &v);

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
void sortbyhire(vector<Creature *> &temppool,vector<int> &level);

/*******************************************************************************
*
*                             Fighting Evil: Site Mode
*                             Folder: "sitemode"
*
*******************************************************************************/

/*
 sitemode.cpp
*/
void mode_site(short loc);
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
void clearmessagearea(bool redrawmaparea=true);
void clearmaparea(bool lower=true,bool upper=true);

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
void reloadparty(bool wasteful=false);

/*
 mapspecials.cpp
*/
void special_bouncer_assess_squad(void);
void special_bouncer_greet_squad(void);
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
void special_graffiti(void);

/*
 talk.cpp
*/
/* bluff, date, issues */
char talk(Creature &a,int t);

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
/* practices squads disguise skill */
void disguisepractice(int p,int diff);
/* returns the difficulty of spotting the squad if it is sneaking */
int stealthskill(void);
/* practices squads stealth skill */
void stealthpractice(int p,int diff);
/* checks if a creature's weapon is suspicious or illegal */
char weaponcheck(Creature &cr,short type);
/* checks if a creature's uniform is appropriate to the location */
char hasdisguise(Creature &cr,short type);
/* returns true if the entire site is not open to public */
char disguisesite(long type);

/*
 advance.cpp
*/
/* handles end of round stuff for everyone */
void creatureadvance(void);
/* handles end of round stuff for one creature */
void advancecreature(Creature &cr);

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
void attack(Creature &a,Creature &t,char mistake,char &actual,bool force_melee=false);
void specialattack(Creature &a,Creature &t,char &actual);
/* modifies a combat roll based on the creature's critical injuries */
void healthmodroll(int &aroll,Creature &a);
/* adjusts attack damage based on armor, other factors */
void damagemod(Creature &t,char &damtype,int &damamount,char hitlocation,char armorpenetration,int &mod);
/* destroys armor, masks, drops weapons based on severe damage */
void severloot(Creature &cr,vector<itemst *> &loot);
/* damages the selected armor if it covers the body part specified */
void armordamage(armorst &armor,int bp);
/* blood explosions */
void bloodblast(armorst &armor);
/* kills the specified creature from the encounter, dropping loot */
void delenc(short e,char loot);
/* generates the loot dropped by a creature when it dies */
void makeloot(Creature &cr,vector<itemst *> &loot);
/* abandoned liberal is captured by conservatives */
void capturecreature(Creature &t);
/* checks if the creature can fight and prints flavor text if they can't */
char incapacitated(Creature &a,char noncombat,char &printed);
/* describes a character's death */
void adddeathmessage(Creature &cr);
/* pushes people into the current squad (used in a siege) */
void autopromote(int loc);

/*
 chase.cpp
*/
char chasesequence(void);
char footchase(void);
void evasivedrive(void);
void evasiverun(void);
int driveskill(Creature &cr,vehiclest *v);
void drivingupdate(short &obstacle);
void makechasers(long sitetype,long sitecrime);
void obstacledrive(short obstacle,char choice);
void dodgedrive(void);
void crashfriendlycar(int v);
void crashenemycar(int v);
void chase_giveup(void);
/* the next two functions force a chase sequence with a specific liberal */
char footchase(Creature &cr);
char chasesequence(Creature &cr,vehiclest &v);

/*
 haulkidnap.cpp
*/
/* prompt after you've said you want to kidnap someone */
void kidnapattempt(void);
/* roll on the kidnap attempt and show the results */
char kidnap(Creature &a,Creature &t,char &amateur);
/* hostage freed due to host unable to haul */
void freehostage(Creature &cr,char situation);
/* haul dead/paralyzed */
void squadgrab_immobile(char dead);
/* names the new hostage and stashes them in your base */
void kidnaptransfer(Creature &cr);

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
/* promote a subordinate to maintain chain of command when boss is lost */
bool promotesubordinates(Creature &cr, char &clearformess);
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
void tendhostage(Creature *cr,char &clearformess);
/* armor repair */
void repairarmor(Creature &cr,char &clearformess);
/* armor manufacture */
void makearmor(Creature &cr,char &clearformess);
/* search for polls */
void survey(Creature *cr);
/* misc activation related things */
void funds_and_trouble(char &clearformess);
/* steal a car */
char stealcar(Creature &cr,char &clearformess);
int difficulty_carfind(int type);
char carselect(Creature &cr,short &cartype);
int sensealarmchance(int cartype);
int touchalarmchance(int cartype);
/* get a wheelchair */
void getwheelchair(Creature &cr,char &clearformess);

/*
 shopsnstuff.cpp
*/
/* active squad visits the hospital */
void hospital(int loc);
/* active squad visits the pawn shop */
void pawnshop(int loc);
/* active squad visits the car dealership */
void dealership(int loc);
/* active squad visits the arms dealer */
void armsdealer(int loc);
/* active squad visits the department store */
void deptstore(int loc);
/* active squad visits the oubliette */
void halloweenstore(int loc);
/* oubliette - buy a mask */
char maskselect(Creature *cr,short &mask);
/* pick stuff to fence */
int fenceselect(void);
/* value of stuff to fence */
int fencevalue(itemst &it);
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
 recruit.cpp
*/
/* daily - recruit - recruit finishes task */
char completerecruittask(recruitst &d,int p,char &clearformess);
/* daily - recruit - recruit meeting*/
char completerecruitmeeting(recruitst &d,int p,char &clearformess);

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
/* siege - flavor text when you crush a CCS safe house */
void conquertextccs(void);
/* siege - "you are wanted for _______ and other crimes..." */
void statebrokenlaws(int loc);
void statebrokenlaws(Creature & cr);

/*
 news.cpp
*/
/* news - determines the priority of a news story */
void setpriority(newsstoryst &ns);
/* news - show major news story */
void displaystory(newsstoryst &ns, bool liberalguardian, int header);
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
*			      News Related Things
*			      Folder: "news"
*
*******************************************************************************/

/*
 newsfuncs.cpp
*/
/* pick a descriptor acronym */
void sexdesc(char *str);
/* what kind of person? */
void sexwho(char *str);
/* seeking acronym */
void sexseek(char *str);
/* what type of sex? */
void sextype(char *str);
/* generate a vanity plate for the squad. */
//void vanity(char *str);
/* generate a non-vanity plate for the squad. */
void plate(char *str);

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

/*
 sleeper_update.cpp
*/
/* monthly - sleeper behavior */
void sleepereffect(Creature &cr,char &clearformess,char canseethings,int *libpower);
/* assistant functions for specific sleeper tasks */
void sleeper_recruit(Creature &cr,char &clearformess,char canseethings,int *libpower);
void sleeper_influence(Creature &cr,char &clearformess,char canseethings,int *libpower);
void sleeper_spy(Creature &cr,char &clearformess,char canseethings,int *libpower);
void sleeper_scandal(Creature &cr,char &clearformess,char canseethings,int *libpower);
void sleeper_embezzle(Creature &cr,char &clearformess,char canseethings,int *libpower);
void sleeper_steal(Creature &cr,char &clearformess,char canseethings,int *libpower);

/*
 justice.cpp
*/
/* monthly - hold trial on a liberal */
void trial(Creature &g);
/* monthly - sentence a liberal */
void penalize(Creature &g,char lenient);
/* monthly - move a liberal to jail */
void imprison(Creature &g);
/* monthly - advances a liberal's prison time or executes them */
char prison(Creature &g);

/*
 politics.cpp
*/
/* politics - calculate presidential approval */
int presidentapproval();
/* politics -- gets the leaning of an issue voter for an election */
int getswingvoter();
/* politics - causes the people to vote (presidential, congressional, propositions) */
void elections(char clearformess,char canseethings);
void elections_senate(int senmod,char canseethings);
void elections_house(char canseethings);
/* politics - causes the supreme court to hand down decisions */
void supremecourt(char clearformess,char canseethings);
/* politics - causes congress to act on legislation */
void congress(char clearformess,char canseethings);
// letter of amenesty to the LCS from the President (you win)
void amnesty(void);
/* politics - checks if the game is won */
char wincheck(void);
/* politics - checks the prevailing attitude on a specific law, or overall */
int publicmood(int l);

/*
 endgame.cpp
*/
/* endgame - attempts to pass a constitutional amendment to help win the game */
void tossjustices(char canseethings);
/* endgame - attempts to pass a constitutional amendment to help win the game */
void amendment_termlimits(char canseethings);
/* endgame - attempts to pass a constitutional amendment to lose the game */
void reaganify(char canseethings);
/* endgame - checks if a constitutional amendment is ratified */
char ratify(int level,int view,int lawview,char congress,char canseethings);
/* endgame - header for announcing constitutional amendments */
void amendmentheading(void);
/* endgame - converts an integer into a roman numeral for amendments */
void romannumeral(int amendnum);


#endif // INCLUDES_H_INCLUDED
