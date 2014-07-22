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
// Don't save the game
//#define NOSAVE

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

// Interrogation debug, since SHOWMECHANICS is too verbose
//#define INTERROGATION_DEBUG

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

// Make age not matter for dating or prostitution
//#define ZEROMORAL

// Re-seed the Random Number Generator every time it's called
//#define MORERANDOM

// Allow experimental, incomplete Stalinist Comrade Squad mode to be chosen for new games
#define ALLOWSTALIN


#ifdef HAVE_CONFIG_H
#include <config.h>
#include <math.h>
#include <stdlib.h>
#include <langinfo.h>
#endif

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "4.07.5 beta"
#endif

const int version=40705;
const int lowestloadversion=40705;
const int lowestloadscoreversion=31203;

#include "common.h" /* include this prior to checking if WIN32 is defined */

#ifdef WIN32
   //#include <windows.h>
   #define GO_PORTABLE
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
         #include <ciso646> // alternate keywords included in the ISO C++ standard
                            // but not directly supported by Microsoft Visual Studio C++
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
   //#define CH_USE_ASCII_HACK
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
      #undef erase // FIXME: Umm... Now erase() and clear() don't work in
      #undef clear //       dumpcaps.cpp
   #else
      #if defined(USE_NCURSES)
         #include <ncurses.h>
      #elif defined(USE_NCURSES_W)
         #include <ncursesw/ncurses.h>
      #else
         #error "You must define either USE_NCURSES or USE_NCURSES_W."
      #endif
      // Undo mvaddstr macro and re-implement as function to support overloading
      #ifdef mvaddstr
         #undef mvaddstr
         inline int mvaddstr(int x, int y, const char* text) { move(x, y); return addstr(text); }
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
      /*
      This #undef addstr...It exists only to make the overloaded addstr
         work in linux (because otherwise it clashes with ncurses).
         The normal addstr works fine for me, so I have no idea what's going on.
         I'll just leave this warning here...If addstr breaks or something, it
         might be related to this undefine. At this point in time though, at least
         on my machine, everything's working just fine.

      This still strikes me as being odd, though. :/
      Oh well, I just hope it'll work for everybody.

      Ciprian Ilies, September 26, 2012
      */
      #undef addstr
      #include <unistd.h>
      #include <sys/time.h>
      #include <signal.h>
   #endif
#endif

#include <sstream>
#include <deque>
#include <algorithm>
#include "cmarkup/Markup.h" //For XML.

using namespace std;
//#include "lcsio.h"
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

 inline unsigned int getSeed()
 {
    unsigned int t;
    #ifdef GO_PORTABLE
    t=(unsigned int)time(NULL); /* Seconds since 1970-01-01 00:00:00 */
    #else // WIN32
    t=(unsigned int)GetTickCount(); /* ms since system boot */
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

#define TAB 9
#define ENTER 10
#define ESC 27
#define SPACEBAR 32

class Log;

enum UnlockTypes
{
   UNLOCK_DOOR,
   UNLOCK_CAGE,
   UNLOCK_CAGE_HARD,
   UNLOCK_CELL,
   UNLOCK_SAFE,
   UNLOCK_ARMORY,
   UNLOCK_VAULT,
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
   HACK_VAULT,
   HACKNUM
};

enum SpecialAttacks
{
   ATTACK_CANNON,
   ATTACK_FLAME,
   ATTACK_SUCK,
   ATTACKNUM
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

enum ccsexposure
{
   CCSEXPOSURE_NONE,
   CCSEXPOSURE_LCSGOTDATA,
   CCSEXPOSURE_EXPOSED,
   CCSEXPOSURE_NOBACKERS,
   CCSEXPOSURENUM
};


#define ARMORFLAG_DAMAGED BIT1
#define ARMORFLAG_BLOODY BIT2


/* *JDS* I'm making laws an array instead of a bunch
 * of bits which are either on or off. Each charge can be
 * duplicated, for multiple counts, and intense focus
 * for wanted persons.
 */
enum Lawflags
{
   LAWFLAG_TREASON,      // "Treason"
   LAWFLAG_TERRORISM,    // "Terrorism"
   LAWFLAG_MURDER,       // "Murder"
   LAWFLAG_KIDNAPPING,   // "Kidnapping"
   LAWFLAG_BANKROBBERY,  // "Bank robbery"
   LAWFLAG_ARSON,        // "Arson"
   LAWFLAG_BURNFLAG,     // "Flag burning" / "Flag Murder"
   LAWFLAG_SPEECH,       // "Harmful speech"
   LAWFLAG_BROWNIES,     // "Drug dealing"
   LAWFLAG_ESCAPED,      // "Escaping prison"
   LAWFLAG_HELPESCAPE,   // "Releasing prisoners"
   LAWFLAG_JURY,         // "Jury tampering"
   LAWFLAG_RACKETEERING, // "Racketeering"
   LAWFLAG_EXTORTION,    // "Extortion"
   LAWFLAG_ARMEDASSAULT, // "Armed assault"
   LAWFLAG_ASSAULT,      // "Assault"
   LAWFLAG_CARTHEFT,     // "Grand theft auto"
   LAWFLAG_CCFRAUD,      // "Credit card fraud"
   LAWFLAG_THEFT,        // "Theft"
   LAWFLAG_PROSTITUTION, // "Prostitution"
   LAWFLAG_HIREILLEGAL,  // "Hiring illegal aliens" / "Hiring undocumented workers"
   //LAWFLAG_GUNUSE,     // "Firing illegal weapons"
   //LAWFLAG_GUNCARRY,   // "Carrying illegal weapons"
   LAWFLAG_COMMERCE,     // "Electronic sabotage"
   LAWFLAG_INFORMATION,  // "Hacking"
   LAWFLAG_BURIAL,       // "Unlawful burial"
   LAWFLAG_BREAKING,     // "Breaking and entering"
   LAWFLAG_VANDALISM,    // "Vandalism"
   LAWFLAG_RESIST,       // "Resisting arrest"
   LAWFLAG_DISTURBANCE,  // "Disturbing the peace"
   LAWFLAG_PUBLICNUDITY, // "Public nudity"
   LAWFLAG_LOITERING,    // "Loitering"
   LAWFLAGNUM
};

/* *JDS* In addition to laws being an array,
 * each law will be associated with a "heat" value,
 * indicating how much that particular crime draws
 * police attention to the wanted person, and to the
 * LCS as a whole. This is a global array, available
 * at runtime and indexed to the lawflag enum, so that
 * code can dynamically look up these heat values as
 * needed at runtime.
 */


enum BusinessFronts
{
   BUSINESSFRONT_INSURANCE,
   BUSINESSFRONT_TEMPAGENCY,
   BUSINESSFRONT_RESTAURANT,
   BUSINESSFRONT_MISCELLANEOUS,
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
   ACTIVITY_TEACH_POLITICS,
   ACTIVITY_TEACH_FIGHTING,
   ACTIVITY_TEACH_COVERT,
   ACTIVITY_STUDY_DEBATING,
   ACTIVITY_STUDY_MARTIAL_ARTS,
   ACTIVITY_STUDY_DRIVING,
   ACTIVITY_STUDY_PSYCHOLOGY,
   ACTIVITY_STUDY_FIRST_AID,
   ACTIVITY_STUDY_LAW,
   ACTIVITY_STUDY_DISGUISE,
   ACTIVITY_STUDY_SCIENCE,
   ACTIVITY_STUDY_BUSINESS,
   //ACTIVITY_STUDY_COOKING,
   ACTIVITY_STUDY_GYMNASTICS,
   ACTIVITY_STUDY_MUSIC,
   ACTIVITY_STUDY_ART,
   ACTIVITY_STUDY_TEACHING,
   ACTIVITY_STUDY_WRITING,
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
   ACTIVITY_STUDY_LOCKSMITHING,
   ACTIVITY_RECRUITING,
   ACTIVITYNUM
};

struct activityst
{
   activityst() : type(0),arg(0),arg2(0) {}
   int type;
   long arg,arg2;
};

enum IncomeType
{                       // the below names are used in fundreport() in lcsmonthly.cpp
                        // new types added without updating fundreport will show as "other income"
   INCOME_BROWNIES,     // "Brownies"
   INCOME_CARS,         // "Car Sales"
   INCOME_CCFRAUD,      // "Credit Card Fraud"
   INCOME_DONATIONS,    // "Donations"
   INCOME_SKETCHES,     // "Drawing Sales"
   INCOME_EMBEZZLEMENT, // "Embezzlement"
   INCOME_EXTORTION,    // "Extortion"
   INCOME_HUSTLING,     // "Hustling"
   INCOME_PAWN,         // "Pawning Goods"
   INCOME_PROSTITUTION, // "Prostitution"
   INCOME_BUSKING,      // "Street Music"
   INCOME_THIEVERY,     // "Thievery"
   INCOME_TSHIRTS,      // "T-Shirt Sales"
   INCOMETYPENUM        // Count of Categories
};

enum ExpenseType
{                         // the below names are used in fundreport() in lcsmonthly.cpp
                          // new types added without updating fundreport will show as "other expenses"
   EXPENSE_TROUBLEMAKING, // "Activism"
   EXPENSE_CONFISCATED,   // "Confiscated"
   EXPENSE_DATING,        // "Dating"
   EXPENSE_SKETCHES,      // "Drawing Materials"
   EXPENSE_FOOD,          // "Groceries"
   EXPENSE_HOSTAGE,       // "Hostage Tending"
   EXPENSE_LEGAL,         // "Legal Fees"
   EXPENSE_MANUFACTURE,   // "Manufacturing"
   EXPENSE_CARS,          // "New Cars"
   EXPENSE_SHOPPING,      // "Purchasing Goods"
   EXPENSE_RECRUITMENT,   // "Recruitment"
   EXPENSE_RENT,          // "Rent"
   EXPENSE_COMPOUND,      // "Safehouse Investments"
   EXPENSE_TRAINING,      // "Training"
   EXPENSE_TRAVEL,        // "Travel"
   EXPENSE_TSHIRTS,       // "T-Shirt Materials"
   EXPENSETYPENUM         // Count of Categories
};

class Ledger
{
private:
   int funds;
public:
   int income[INCOMETYPENUM],expense[EXPENSETYPENUM],total_income,total_expense,dailyIncome[INCOMETYPENUM],dailyExpense[EXPENSETYPENUM];
   Ledger() : funds(7),total_income(0),total_expense(0)
   {
      for(int i=0;i<INCOMETYPENUM;i++) income[i]=0,dailyIncome[i]=0;
      for(int e=0;e<EXPENSETYPENUM;e++) expense[e]=0,dailyExpense[e]=0;
   }
   int get_funds() { return funds; }
   void force_funds(int amount) { funds=amount; }
   void add_funds(int amount,int incometype)
   {
        funds+=amount,
        income[incometype]+=amount,
        dailyIncome[incometype]+=amount,
        total_income+=amount;
   }
   void subtract_funds(int amount,int expensetype)
   {
        funds-=amount,
        expense[expensetype]+=amount,
        dailyExpense[expensetype]+=amount,
        total_expense+=amount;
   }
   void resetMonthlyAmounts()
   {
      for(int i=0;i<INCOMETYPENUM;i++) income[i]=0;
      for(int e=0;e<EXPENSETYPENUM;e++) expense[e]=0;
   }
   void resetDailyAmounts()
   {
      for(int i=0;i<INCOMETYPENUM;i++) dailyIncome[i]=0;
      for(int e=0;e<EXPENSETYPENUM;e++) dailyExpense[e]=0;
   }
};

class Interval
{
public:
   int min,max;
   Interval() : min(0),max(0) {}
   Interval(int value) : min(value),max(value) {}
   Interval(int low, int high) : min(low),max(high) {}
   void set_interval(int low, int high) { min=low,max=high; }
   // Sets the interval according to a string that is either a number or two
   // number separated by a dash. Returns false and does not change the
   // interval if the given string is not a valid interval.
   bool set_interval(const string& interval); // implemented in misc.cpp
   int roll() const { return LCSrandom(max-min+1)+min; }
private:
   // Checks if a string is a number. Assumes non-numeric characters other
   // than dashes have already been checked for.
   bool valid(const string& v)
   { return !(v.empty() ||
             (v.length()==1&&v[0]=='-') ||     // Just a dash.
              v.find('-', 1)!=string::npos); } // A dash after the first char.
};

#include "items/itemtype.h"
#include "items/cliptype.h"
#include "items/weapontype.h"
#include "items/armortype.h"
#include "items/loottype.h"
#include "items/item.h"
#include "items/clip.h"
#include "items/weapon.h"
#include "items/armor.h"
#include "items/loot.h"
#include "items/money.h"
#include "creature/creature.h"
#include "creature/creaturetype.h"
#include "vehicle/vehicletype.h"
#include "vehicle/vehicle.h"

#include "locations/locations.h"

#define COMPOUND_BASIC BIT1
#define COMPOUND_CAMERAS BIT2
#define COMPOUND_TANKTRAPS BIT3
#define COMPOUND_TRAPS BIT4
#define COMPOUND_GENERATOR BIT5
#define COMPOUND_PRINTINGPRESS BIT6
#define COMPOUND_AAGUN BIT7



enum CarChaseObstacles
{
   CARCHASE_OBSTACLE_FRUITSTAND,
   CARCHASE_OBSTACLE_TRUCKPULLSOUT,
   CARCHASE_OBSTACLE_CROSSTRAFFIC,
   CARCHASE_OBSTACLE_CHILD,
   CARCHASE_OBSTACLENUM
};

struct chaseseqst
{
   long location;
   vector<Vehicle *> friendcar,enemycar;
   char canpullover;
   void clean() { delete_and_clear(enemycar); friendcar.clear(); }
};

enum SquadStances
{
   SQUADSTANCE_ANONYMOUS,
   SQUADSTANCE_STANDARD,
   SQUADSTANCE_BATTLECOLORS,
   SQUADSTANCE_MAX
};

#define SQUAD_NAMELEN 40

struct squadst
{
   char name[SQUAD_NAMELEN];
   Creature *squad[6];
   activityst activity;
   int id;
   vector<Item *> loot;

   char stance; // Squad's site action stance: high profile, low profile, etc.

   squadst()
   {
      for(int p=0;p<6;p++) squad[p]=NULL;
      strcpy(name,"");
      activity.type=ACTIVITY_NONE,id=-1,stance=SQUADSTANCE_STANDARD;
   }
   ~squadst() { delete_and_clear(loot); }
};


#define ENCMAX 18
// ENCMAX _HAS_ to be 26 or less, or else there aren't enough letters

#include "sitemode/shop.h"

enum GameModes
{
   GAMEMODE_TITLE,
   GAMEMODE_BASE,
   GAMEMODE_SITE,
   GAMEMODE_CHASECAR,
   GAMEMODE_CHASEFOOT
};

enum WinConditions
{
	WINCONDITION_ELITE,
	WINCONDITION_EASY
};

enum Views
{
   VIEW_STALIN=-2, // this one is -2 and is actually calculated based on views >=0 and <VIEWNUM-3
   VIEW_MOOD, // this one is -1 and is likewise calculated based on views >=0 and <VIEWNUM-3
   VIEW_GAY, // view #0, the first one that is actually in the attitude[] array
   VIEW_DEATHPENALTY,
   VIEW_TAXES,
   VIEW_NUCLEARPOWER,
   VIEW_ANIMALRESEARCH,
   VIEW_POLICEBEHAVIOR,
   VIEW_TORTURE,
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
   VIEW_PRISONS,
   //*JDS* I'm using VIEWNUM-5 in a random generator that rolls a
   //random issue, not including the media/politicalviolence ones, and this will
   //break if these stop being the last 4 issues; do a search
   //for VIEWNUM-5 to change it if it needs to be changed.
   VIEW_AMRADIO,
   VIEW_CABLENEWS,
   //VIEW_POLITICALVIOLENCE,
   //THESE THREE MUST BE LAST FOR VIEWNUM-3 TO WORK IN PLACES
   VIEW_LIBERALCRIMESQUAD,
   VIEW_LIBERALCRIMESQUADPOS,
   //THIS ONE MUST BE LAST. randomissue RELIES ON IT BEING LAST TO IGNORE IT IF
   //CCS IS DEAD.
   VIEW_CONSERVATIVECRIMESQUAD,
   VIEWNUM
};

enum Laws
{
   LAW_STALIN=-2, // not a real law: this is -2 and is actually calculated based on views >=0 and <VIEWNUM-3
   LAW_MOOD, // not a real law: this is -1 and is likewise calculated based on views >=0 and <VIEWNUM-3
   LAW_ABORTION, // law #0, the first one that is actually in the law[] array
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
   LAW_PRISONS,
   LAW_TORTURE,
   LAWNUM
};

struct datest
{
   long mac_id;
   vector<Creature *> date;
   short timeleft;
   int city;
   datest() : timeleft(0) {}
   ~datest() { delete_and_clear(date); }
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
   char level,eagerness1,task;
   recruitst();
   ~recruitst();
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
   CRIME_ARMORY,
   CRIME_VANDALISM,
   CRIME_BANKVAULTROBBERY,
   CRIME_BANKTELLERROBBERY,
   CRIME_BANKSTICKUP,
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
   NEWSSTORY_CCS_NOBACKERS,
   NEWSSTORY_CCS_DEFEATED,
   NEWSSTORY_PRESIDENT_IMPEACHED,
   NEWSSTORY_PRESIDENT_BELIEVED_DEAD,
   NEWSSTORY_PRESIDENT_FOUND_DEAD,
   NEWSSTORY_PRESIDENT_FOUND,
   NEWSSTORY_PRESIDENT_KIDNAPPED,
   NEWSSTORY_PRESIDENT_MISSING,
   NEWSSTORY_PRESIDENT_ASSASSINATED,
   NEWSSTORYNUM
};

struct newsstoryst
{
   short type,view;
   char claimed;
   short politics_level,violence_level;
   Creature *cr;
   vector<int> crime;
   long loc,priority,page,guardianpage;
   char positive;
   short siegetype;
   newsstoryst() : claimed(1),politics_level(0),violence_level(0),cr(NULL),loc(-1) {}
};

#define SLOGAN_LEN 80

struct highscorest
{
   char valid,endtype,slogan[SLOGAN_LEN];
   int month,year,stat_recruits,stat_kidnappings,stat_dead,stat_kills,stat_funds,stat_spent,stat_buys,stat_burns;
};

//just a float that is initialized to 0
struct float_zero
{
   float_zero() : n(0.0f) {}
   operator float&() { return n; }
   float n;
};
//Interrogation information for the interrogation system, to be
//dynamically created on capture and deleted when interrogation ends,
//referenced using a pointer typecast into one of the arguments
//of the target's current action.
struct interrogation
{
   interrogation() : druguse(0) { techniques[0]=1,techniques[1]=1,techniques[2]=0,techniques[3]=0,techniques[4]=0,techniques[5]=0; }
   bool techniques[6]; //yesterday's interrogation plan
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
   END_STALIN,
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

enum SortingChoices
{
   SORTING_NONE,
   SORTING_NAME,
   SORTING_LOCATION_AND_NAME,
   SORTING_SQUAD_OR_NAME,
   SORTINGNUM
};

enum ActiveSortingChoices
{
   SORTINGCHOICE_LIBERALS, //They're prefixed SORTINGCHOICE because they're used as
   SORTINGCHOICE_HOSTAGES, //array indices for the array activesortingchoice.
   SORTINGCHOICE_CLINIC,   //activesortingchoice holds the chosen way to sort the lists.
   SORTINGCHOICE_JUSTICE,
   SORTINGCHOICE_SLEEPERS,
   SORTINGCHOICE_DEAD,
   SORTINGCHOICE_AWAY,
   SORTINGCHOICE_ACTIVATE,
   SORTINGCHOICE_ACTIVATESLEEPERS,
   SORTINGCHOICE_ASSEMBLESQUAD,
   SORTINGCHOICE_BASEASSIGN,
   SORTINGCHOICENUM
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
void set_alignment_color(signed char alignment,bool extended_range=false);
/* Sets the text color per activity type */
void set_activity_color(long activity_type);
/* location and squad header */
void locheader();
/* party info at top of screen */
void printparty();
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
/* Full screen character sheet, crime sheet */
void printliberalcrimes(Creature &cr);
/* draws a horizontal line across the screen */
void makedelimiter(int y=8,int x=0);
/* prints a character's health description (One Leg, Liberal, NearDETH...) */
void printhealthstat(Creature &g,int y,int x,char smll);
/* prints amount of money the player has, with optional formatting */
void printfunds(int y=0,int offset=1,const char* prefix="Money: ");
/* prints a short blurb showing how to page forward */
void addnextpagestr();
/* prints a short blurb showing how to page back */
void addprevpagestr();
/* prints a long blurb showing how to page forward and back */
void addpagestr();
/* Various wrappers to addstr() and mvaddstr() which handle permutations of:
   - Including or not including the gamelog for external message logging
   - std::string or c-style char arrays */
int addstr(const char *text,Log &log);
int mvaddstr(int y,int x,const char *text,Log &log);
int addstr(const std::string& text);
int addstr(const std::string& text, Log &log);
int mvaddstr(int y,int x,const std::string& text);
int mvaddstr(int y,int x,const std::string& text,Log &log);
/* These wrappers convert numbers to text */
int addstr(long num);
int addstr(long num, Log &log);
int mvaddstr(int y,int x,long num);
int mvaddstr(int y,int x,long num,Log &log);
/* addstr with formatted output */
int addstr_f(const char * format,...);
/* mvaddstr with formatted output */
int mvaddstr_f(int y,int x,const char * format,...);
/* addstr with formatted output and logging */
int addstr_fl(Log &log,const char * format,...);
/* mvaddstr with formatted output and logging */
int mvaddstr_fl(int y,int x,Log &log,const char * format,...);
/* Variant of addch that works on char instead of chtype, fixing display of extended characters */
int addchar(char ch);
/* Variant of mvaddch that works on char instead of chtype, fixing display of extended characters */
int mvaddchar(int y,int x,char ch);
/* addchar and mvaddchar with logging */
int addchar(char ch,Log &log);
int mvaddchar(int y,int x,char ch,Log &log);

/*
 commonactions.cpp
*/
/* common - test for possible game over */
char endcheck(char cause=-1);
/* common - tests if the person is a wanted criminal */
bool iscriminal(Creature &cr);
/* common - sends somebody to the hospital */
void hospitalize(int loc,Creature &patient);
/* common - determines how long a creature's injuries will take to heal */
int clinictime(Creature &g);
/* common - applies a crime to everyone in the active party */
void criminalizeparty(short crime);
/* common - applies a crime to everyone in a location, or the entire LCS */
void criminalizepool(short crime,long exclude=-1,short loc=-1);
/* common - applies a crime to a person */
void criminalize(Creature &cr,short crime);
/* common - gives juice to everyone in the active party */
void juiceparty(long juice,long cap);
/* common - gives juice to a given creature */
void addjuice(Creature &cr,long juice,long cap);
/* common - removes the liberal from all squads */
void removesquadinfo(Creature &cr);
/* common - purges empty squads from existance */
void cleangonesquads();
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
// Determines the number of recruitment meetings a creature has scheduled
int scheduledmeetings(const Creature& cr);
// Determines the number of dates a creature has scheduled
int scheduleddates(const Creature& cr);
/* common - random issue by public interest */
int randomissue(bool core_only=0);
// Picks a random option, based on the weights provided
int choose_one(const int * weight_list,int number_of_options,int default_value);
// Prompt to convert a new recruit into a sleeper
void sleeperize_prompt(Creature &converted,Creature &recruiter,int y);
/* common - Sort a list of creatures.*/
void sortliberals(std::vector<Creature *>& liberals,short sortingchoice,bool dosortnone=false);
/* common - Functions used when sorting vectors of creatures. */
bool sort_none(Creature* first,Creature* second);
bool sort_name(Creature* first,Creature* second);
bool sort_locationandname(Creature* first,Creature* second);
bool sort_squadorname(Creature* first,Creature* second);
/* common - Prompt to decide how to sort liberals.*/
void sorting_prompt(short listforsorting);
/* common - Returns appropriate sortingchoice enum value for a reviewmode enum value.
            (Is currently unnecessary unless the enums are changed.)*/
short reviewmodeenum_to_sortingchoiceenum(short reviewmode);
/* common - Display a list of options and return choice. */
int choiceprompt(const string &firstline,const string &secondline,
                 const vector<string> &option,const string &optiontypename,
                 bool allowexitwochoice,const string &exitstring="");
/* common - Displays a list of things to buy and returns an int corresponding
            to the index of the chosen thing in the nameprice vector. */
int buyprompt(const string &firstline,const string &secondline,
              const vector< pair<string,int> > &nameprice,int namepaddedlength,
              const string &producttype,const string &exitstring);
/* tells how many total members a squad has (including dead members) */
int squadsize(const squadst *st);
/* tells how many members a squad has who are alive */
int squadalive(const squadst *st);


/*
 consolesupport.cpp
*/
void set_color(short f,short b,bool bright,bool blink=false);
void translategetch(int &c);
void translategetch_cap(int &c);
/* Refreshes the screen, empties the keyboard buffer, waits for a new key to be pressed, and returns the key pressed */
int getkey();
/* Variant of getkey() that doesn't make all letters lowercase */
int getkey_cap();
/* Empties the keyboard buffer, and returns most recent key pressed, if any */
int checkkey();
/* Variant of checkkey() that doesn't make all letters lowercase */
int checkkey_cap();
#ifdef CH_USE_UNICODE
  char setup_unicode();
  int addch_unicode(int c);
#endif
void set_title (char *c);
void init_console();
#ifdef WIN32
void begin_cleartype_fix();
void end_cleartype_fix();
#endif

/*
 getnames.cpp
*/
std::string getactivity(activityst &act);
std::string gettitle(Creature &cr);
std::string getview(short view,bool shortname);
std::string getlaw(int l);
std::string cityname(); /* random city name */
/* Allow player to enter a name with an optional default name */
void enter_name(int y,int x,char *name,int len,const char *defname=NULL);
std::string getlawflag(int type);
std::string getmonth(int month,bool shortname=false);
std::string getalign(signed char alignment,bool capitalize=false);

/*
 translateid.cpp
*/
/* transforms a squad id number into the index of that squad in the global vector */
int getsquad(int id);
/* transforms a car id number into the index of that car in the global vector */
int id_getcar(int id);
/* transforms a creature id number into the index of that person in the pool */
int getpoolcreature(int id);
/* transforms a vehicle type id into the index of that vehicle type in the global vector */
int getvehicletype(int id);
/* transforms a vehicle type idname into the index of that vehicle type in the global vector */
int getvehicletype(const string &idname);
/* transforms a clip type id into the index of that clip type in the global vector */
int getcliptype(int id);
/* transforms a clip type name into the index of that clip type in the global vector */
int getcliptype(const string &idname);
/* transforms a weapon type id into the index of that weapon type in the global vector */
int getweapontype(int id);
/* transforms a weapon type name into the index of that weapon type in the global vector */
int getweapontype(const string &idname);
/* transforms a armor type id into the index of that armor type in the global vector */
int getarmortype(int id);
/* transforms a armor type name into the index of that armor type in the global vector */
int getarmortype(const string &idname);
/* transforms a loot type id into the index of that loot type in the global vector */
int getloottype(int id);
/* transforms a loot type name into the index of that loot type in the global vector */
int getloottype(const string &idname);
/* transforms a CreatureTypes value into a pointer to that creature type */
const CreatureType* getcreaturetype(short crtype);
/* transforms a creature type name into a pointer to that creature type */
const CreatureType* getcreaturetype(const std::string& crtype);

/*
 equipment.cpp
*/
/* prompt user to enter an amount of items to equip, move, or sell */
long prompt_amount(long min, long max);
/* review squad equipment */
void equip(vector<Item *> &loot,int loc);
/* lets you pick stuff to stash/retrieve from one location to another */
void moveloot(vector<Item *> &dest,vector<Item *> &source);
/* equipment - assign new bases to the equipment */
void equipmentbaseassign();
/* combines multiple items of the same type into stacks */
void consolidateloot(vector<Item *> &loot);
/* compares two items, used in sorting gear */
char itemcompare(Item *a,Item *b);
/* check if the squad has a certain weapon */
char squadhasitem(squadst &sq,int type,int subtype);

/*
 stringconversion.cpp
*/
std::string tostring(long i);
const char* toCstring(long i);
/* Tries to determine boolean value of a string. Returns 1 for true, 0 for false
   and -1 if unable to determine. */
int stringtobool(std::string boolstr);
/* These strcpy, strncpy, and strcat wrappers handle std:strings */
char* strcpy(char* dest, const std::string& src);
char* strncpy(char* dest, const std::string& src, size_t maxlen);
char* strcat(char* dest, const std::string& src);
/* These strcpy and strcat wrappers handle numbers */
char* strcpy(char* dest, long src);
char* strcat(char* dest, long src);
/* This wrapper allows atoi to handle std::strings */
int atoi(const std::string& str);
/* This wrapper allows atof to handle std::strings */
double atof(const std::string& str);
short creaturetype_string_to_enum(const std::string& ctname);
int attribute_string_to_enum(const std::string& attribute);
int skill_string_to_enum(std::string skillname);
int gender_string_to_enum(const std::string& gender);
int severtype_string_to_enum(const std::string& severtype);

/*
 creature.cpp
*/
// Add an age estimate to a person's name
void add_age(Creature& person);
/* rolls up a creature's stats and equipment */
void makecreature(Creature &cr,short type);
/* fills a string with a proper name */
void generate_name(char *str, char gender = GENDER_NEUTRAL);
/* get a first and last name for the same person */
void generate_name(char *first, char *last, char gender = GENDER_NEUTRAL);
/* get a first, middle, and last name for the same person */
void generate_long_name(char *first, char *middle, char *last, char gender = GENDER_NEUTRAL);
/* gets a random first name */
void firstname(char *str, char gender = GENDER_NEUTRAL);
/* gets a random last name */
void lastname(char *str, bool archconservative=false);
/* ensures that the creature's work location is appropriate to its type */
bool verifyworklocation(Creature &cr, char test_location=-1, char test_type=-1);
/* turns a creature into a conservative */
void conservatise(Creature &cr);
/* turns a creature into a liberal */
void liberalize(Creature &cr,bool rename=true);
/* gives a cover name to CCS members */
void nameCCSMember(Creature &cr);

/*
 misc.cpp
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
/* return a letter, number, or one of either. */
void chooseLetterOrNumber(char *str,int type);
/* generate a non-vanity plate for the squad. */
void plate(char *str);
/* name of a state (random state, by default) */
const char* statename(int state=-1);
/* endgame - converts an integer into a roman numeral for amendments */
void romannumeral(int amendnum);
/* code for bool Interval::set_interval(const string& interval); is also in misc.cpp */


/*
 help.cpp
*/
/* Generates help page on activities. Just supply Activity type as an int */
void HelpActivities(int activity);


/*******************************************************************************
*
*                             Title Screen and New Game
*                             Folder: "title"
*
*******************************************************************************/

/*
 titlescreen.cpp
*/
void mode_title();

/*
 initfile.cpp
*/
/* Handles a init.txt line */
void setconfigoption(std::string name, std::string value);
/* Loads and parses init.txt */
void loadinitfile();

/*
 highscore.cpp
*/
/* displays the high score board */
void viewhighscores();
/* loads the high scores file */
void loadhighscores();
/* saves a new high score */
void savehighscore(char endtype);

/*
 newgame.cpp
*/
/* new game options screen */
void setup_newgame();
/* creates your founder */
void makecharacter();

/*
 saveload.cpp
*/
/* handles saving */
void savegame(const char *str);
/* loads the game from save.dat */
char load();
/* deletes save.dat (used on endgame and for invalid save version) */
void reset();


/*******************************************************************************
*
*                        The Main Game Screen: Base Mode
*                        Folder: "basemode"
*
*******************************************************************************/

/*
 basemode.cpp
*/
void mode_base();

/*
 baseactions.cpp
*/
/* base - burn the flag */
void burnflag();
/* base - new slogan */
void getslogan();
/* base - reorder party */
void orderparty();
/* base - go forth to stop evil */
void stopevil();
/* base - invest in this location */
void investlocation();
/* base - assign a vehicle to this squad */
void setvehicles();

/*
 liberalagenda.cpp
*/
/* base - liberal agenda */
char liberalagenda(char won);
/* base - liberal agenda - disband */
char confirmdisband();

/*
 activate_sleepers.cpp
*/
/* base - activate the uninvolved */
void activate_sleepers();
void activate_sleeper(Creature *cr);

/*
 activate.cpp
*/
/* base - activate the uninvolved */
void activate();
void activate(Creature *cr);
void activatebulk();
/* base - activate - hostages */
void select_tendhostage(Creature *cr);
long select_hostagefundinglevel(Creature *cr,Creature *hs);
/* activation-based recruitment */
int recruitFindDifficulty(int creatureType);
char* recruitName(int creatureType);
void recruitSelect(Creature &cr);
/* base - activate - make clothing */
void select_makeclothing(Creature *cr);
int armor_makedifficulty(Armor& type,Creature *cr); //Replace with Armor class method? -XML
int armor_makedifficulty(ArmorType& type,Creature *cr); //Replace with Armor class method? -XML
/* base - activate - trouble */
long select_troublefundinglevel(Creature *cr);
/* base - activate - select a topic to write about */
char select_view(Creature *cr,int &v);

/*
 reviewmode.cpp
*/
/* base - review and reorganize liberals */
void review();
void review_mode(short mode);
/* base - review - assemble a squad */
void assemblesquad(squadst *cursquad);
/* base - review - assign new bases to the squadless */
void squadlessbaseassign();
/* base - review - promote liberals */
void promoteliberals();
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
void mode_site();
/* site - determines spin on site news story, "too hot" timer */
void resolvesite();
/* behavior when the player bumps into a door in sitemode */
void open_door(bool restricted);
/* re-create site from seed before squad arrives */
void initsite(Location &loc);
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
void printencounter();
/* prints the names of creatures you see in car chases */
void printchaseencounter();
/* blanks a part of the screen */
void clearcommandarea();
void clearmessagearea(bool redrawmaparea=true);
void clearmaparea(bool lower=true,bool upper=true);

/*
 miscactions.cpp
*/
/* unlock attempt */
char unlock(short type,char &actual);
/* bash attempt */
char bash(short type,char &actual);
/* computer hack attempt */
char hack(short type,char &actual);
/* run a radio broadcast */
char radio_broadcast();
/* run a tv broadcast */
char news_broadcast();
/* rescues people held at the activeparty's current location */
void partyrescue(short special);
/* everybody reload! */
void reloadparty(bool wasteful=false);

/*
 mapspecials.cpp
*/
void special_bouncer_assess_squad();
void special_bouncer_greet_squad();
void special_lab_cosmetics_cagedanimals();
void special_readsign(int sign);
void special_nuclear_onoff();
void special_lab_genetic_cagedanimals();
void special_policestation_lockup();
void special_courthouse_lockup();
void special_courthouse_jury();
void special_prison_control(short prison_control_type);
void special_intel_supercomputer();
void special_sweatshop_equipment();
void special_polluter_equipment();
void special_house_photos();
void special_corporate_files();
void special_radio_broadcaststudio();
void special_news_broadcaststudio();
void special_graffiti();
void special_armory();
void special_display_case();
void special_security_checkpoint();
void special_security_metaldetectors();
void special_security_secondvisit();
void special_bank_teller();
void special_bank_money();
void special_bank_vault();
void special_ccs_boss();
void special_oval_office();

/*
 talk.cpp
*/
/* bluff, date, issues */
char talk(Creature &a,int t);

/*
 stealth.cpp
*/
/* checks if your liberal activity is noticed */
void noticecheck(int exclude,int difficulty=DIFFICULTY_EASY);
/* checks if your liberal behavior/attack alienates anyone */
char alienationcheck(char mistake);
/* checks if conservatives see through your disguise */
void disguisecheck(int encounter_timer);
/* checks if a creature's weapon is suspicious or illegal */
char weaponcheck(const Creature &cr,bool metaldetect=false);
/* checks if a creature's uniform is appropriate to the location */
char hasdisguise(const Creature &cr);
/* returns true if the entire site is not open to public */
char disguisesite(long type);

/*
 advance.cpp
*/
/* handles end of round stuff for everyone */
void creatureadvance();
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
void youattack();
void enemyattack();
/* attack handling for an individual creature and its target */
void attack(Creature &a,Creature &t,char mistake,char &actual,bool force_melee=false);
void specialattack(Creature &a,Creature &t,char &actual);
/* modifies a combat roll based on the creature's critical injuries */
void healthmodroll(int &aroll,Creature &a);
/* adjusts attack damage based on armor, other factors */
void damagemod(Creature &t,char &damtype,int &damamount,char hitlocation,char armorpenetration,int &mod);
/* destroys armor, masks, drops weapons based on severe damage */
void severloot(Creature &cr,vector<Item *> &loot);
/* damages the selected armor if it covers the body part specified */
void armordamage(Armor &armor,int bp);
/* blood explosions */
void bloodblast(Armor* armor);
/* kills the specified creature from the encounter, dropping loot */
void delenc(short e,char loot);
/* generates the loot dropped by a creature when it dies */
void makeloot(Creature &cr,vector<Item *> &loot);
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
bool chasesequence();
bool footchase();
void evasivedrive();
void evasiverun();
int driveskill(Creature &cr,Vehicle &v);
bool drivingupdate(short &obstacle);
void makechasers(long sitetype,long sitecrime);
bool obstacledrive(short obstacle,char choice);
bool dodgedrive();
void crashfriendlycar(int v);
void crashenemycar(int v);
void chase_giveup();
/* the next two functions force a chase sequence with a specific liberal */
bool footchase(Creature &cr);
bool chasesequence(Creature &cr,Vehicle &v);

/*
 haulkidnap.cpp
*/
/* prompt after you've said you want to kidnap someone */
void kidnapattempt();
/* prompt after you've said you want to release someone */
void releasehostage();
/* roll on the kidnap attempt and show the results */
bool kidnap(Creature &a,Creature &t,bool &amateur);
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
bool promotesubordinates(Creature &cr,char &clearformess);
/* daily - manages too hot timer and when a site map should be re-seeded and renamed */
void advancelocations();
/* daily - returns true if the site type supports high security */
char securityable(int type);
/* daily - seeds and names a site (will re-seed and rename if used after start) */
void initlocation(Location &loc);
/* daily - returns the number of days in the current month */
int monthday();

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
bool stealcar(Creature &cr,char &clearformess);
bool carselect(Creature &cr,short &cartype);
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
/* automatic finding recruits from the activity screen */
char recruitment_activity(Creature &cr,char &clearformess);
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
void giveup();
/* siege - checks how many days of food left at the site */
int fooddaysleft(int loc);
/* siege - checks how many people are eating at the site */
int numbereating(int loc);
/* siege - prepares for massed combat outside the safehouse */
void sally_forth();
/* siege - prepares for entering site mode to fight the siege */
void escape_engage();
/* siege - what happens when you escaped the siege */
void escapesiege(char won);
/* siege - flavor text when you fought off the raid */
void conquertext();
/* siege - flavor text when you crush a CCS safe house */
void conquertextccs();
/* siege - "you are wanted for _______ and other crimes..." */
void statebrokenlaws(int loc);
void statebrokenlaws(Creature &cr);

/*******************************************************************************
*
*                             End of Day News Stories
*                             Folder: "news"
*
*******************************************************************************/
/* news - determines the priority of a news story */
void setpriority(newsstoryst &ns);
/* news - show major news story */
void displaystory(newsstoryst &ns,bool liberalguardian,int header);
/* news - graphics */
void loadgraphics();
void displaycenterednewsfont(const std::string& str,int y);
void displaycenteredsmallnews(const std::string& str,int y);
void displaynewspicture(int p,int y);
/* news - constructs non-LCS related event stories */
void constructeventstory(char *story,short view,char positive);
/* news - draws the specified block of text to the screen */
void displaynewsstory(char *story,short *storyx_s,short *storyx_e,int y);
/* news - shows animated news stories */
void run_television_news_stories();
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

/*
 sleeper_update.cpp
*/
/* monthly - sleeper behavior */
void sleepereffect(Creature &cr,char &clearformess,char canseethings,int (&libpower)[VIEWNUM]);
/* assistant functions for specific sleeper tasks */
void sleeper_recruit(Creature &cr,char &clearformess,char canseethings,int (&libpower)[VIEWNUM]);
void sleeper_influence(Creature &cr,char &clearformess,char canseethings,int (&libpower)[VIEWNUM]);
void sleeper_spy(Creature &cr,char &clearformess,char canseethings,int (&libpower)[VIEWNUM]);
void sleeper_scandal(Creature &cr,char &clearformess,char canseethings,int (&libpower)[VIEWNUM]);
void sleeper_embezzle(Creature &cr,char &clearformess,char canseethings,int (&libpower)[VIEWNUM]);
void sleeper_steal(Creature &cr,char &clearformess,char canseethings,int (&libpower)[VIEWNUM]);

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
/* monthly - subjects a liberal to liberal therapy */
void reeducation(Creature &g);
/* monthly - subjects a liberal to hard labor */
void laborcamp(Creature &g);
/* monthly - subjects a liberal to normal prison */
void prisonscene(Creature &g);

/*
 politics.cpp
*/
/* politics - calculate presidential approval */
int presidentapproval();
/* politics -- gets the leaning of an issue voter for an election */
int getswingvoter();
/* politics -- promotes the Vice President to President, and replaces VP */
void promoteVP();
/* politics -- appoints a figure to an executive office, based on the President's alignment */
void fillCabinetPost(int position);
/* politics - causes the people to vote (presidential, congressional, propositions) */
void elections(char clearformess,char canseethings);
void elections_senate(int senmod,char canseethings);
void elections_house(char canseethings);
/* politics - causes the supreme court to hand down decisions */
void supremecourt(char clearformess,char canseethings);
/* politics - causes congress to act on legislation */
void congress(char clearformess,char canseethings);
// letter of amnesty to the LCS from the President (you win)
void amnesty();
/* politics - checks if the game is won */
char wincheck();
/* politics - checks the prevailing attitude on a specific law, or overall */
int publicmood(int l);
/* returns true if Stalinists agree with Elite Liberals on a view/law, false if they strongly disagree with libs  *
 * the input bool islaw, if true, returns Stalinist opinion on laws, if false, returns Stalinist opinion on views */
bool stalinview(short view,bool islaw);

/*
 endgame.cpp
*/
/* endgame - attempts to pass a constitutional amendment to help win the game */
void tossjustices(char canseethings);
/* endgame - attempts to pass a constitutional amendment to help win the game */
void amendment_termlimits(char canseethings);
/* endgame - attempts to pass a constitutional amendment to lose the game */
void reaganify(char canseethings);
/* endgame - attempts to pass a constitutional amendment to lose the game */
void stalinize(char canseethings);
/* endgame - checks if a constitutional amendment is ratified */
char ratify(int level,int view,int lawview,char congress,char canseethings);
/* endgame - header for announcing constitutional amendments */
void amendmentheading();


/*******************************************************************************
*
*                             Logging Stuff
*                             Folder: "log"
*
*******************************************************************************/

//TODO: Make NEWLINEMODE_LOGFILES_DEFAULT, NEWLINEMODE_GAMELOG, and OVERWRITE_GAMELOG set by the cfg.
//Whether or not it should autonewline logfiles by defualt.
#define NEWLINEMODE_LOGFILES_DEFAULT 1
//Whether or not it should autonewline the gamelog.
#define NEWLINEMODE_GAMELOG 2
//Whether or not it should overwrite the gamelog every time the game starts.
#define OVERWRITE_GAMELOG false

//The filepath of the gamelog.
//TODO: Make this be set via the cfg.
#define GAMELOG_FILEPATH "gamelog.txt"

#include "log/log.h"


#endif // INCLUDES_H_INCLUDED
