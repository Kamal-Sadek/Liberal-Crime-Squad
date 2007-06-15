//////////////////////////////////////////////////////////////////////////////////////////
//																						//
//Copyright (c) 2002,2003,2004 by Tarn Adams											//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////
//This file is part of Liberal Crime Squad.												//
//																						//
//    Liberal Crime Squad is free software; you can redistribute it and/or modify		//
//    it under the terms of the GNU General Public License as published by				//
//    the Free Software Foundation; either version 2 of the License, or					//
//    (at your option) any later version.												//
//																						//
//    Liberal Crime Squad is distributed in the hope that it will be useful,			//
//    but WITHOUT ANY WARRANTY; without even the implied warranty of					//
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						//
//    GNU General Public License for more details.										//
//																						//
//    You should have received a copy of the GNU General Public License					//
//    along with Liberal Crime Squad; if not, write to the Free Software				//
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA			//
//////////////////////////////////////////////////////////////////////////////////////////

/* Windows will use original Windows API Code unless GO_PORTABLE is defined
 * (this will need to be defined in you Makefile/Visual C++ Project), in which
 * case the portable routines will be used instead. */

  
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
    #define CPLUSPLUS  1
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
CursesMoviest movie;
unsigned char bigletters[27][5][7][4];
unsigned char newstops[5][80][5][4];
unsigned char newspic[20][78][18][4];

//outstanding issues
	//site trucker-type bug still happens (latte-stand)
		//might have to do with missing location in the verifyworklocation() loop
			//but cannot locate instance of this
	//can have hostage in court?

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

//#define NOENEMYATTACK
//#define SHITLAWS
//#define GIVEBLOODYARMOR
//#define HIGHFUNDS
//#define AUTOENLIGHTEN
//#define SHOWWAIT

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

unsigned long seed;

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
	WEAPON_SEMIRIFLE_M16,
	WEAPON_SEMIRIFLE_AK47,
	WEAPON_SHOTGUN,
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
			case WEAPON_SEMIRIFLE_M16:
			case WEAPON_SEMIRIFLE_AK47:
			case WEAPON_SHOTGUN:
				return 1;
			}
		return 0;
		}
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

#define LAWFLAG_KIDNAPPER BIT1
//#define BIT2
#define LAWFLAG_MURDERER BIT3
#define LAWFLAG_THIEF BIT4
#define LAWFLAG_BREAKING BIT5
#define LAWFLAG_TERRORISM BIT6
#define LAWFLAG_JURY BIT7
#define LAWFLAG_TREASON BIT8
#define LAWFLAG_ESCAPED BIT9
#define LAWFLAG_HELPESCAPE BIT10
#define LAWFLAG_RESIST BIT11
#define LAWFLAG_BURNFLAG BIT12
#define LAWFLAG_SPEECH BIT13
#define LAWFLAG_VANDALISM BIT14
#define LAWFLAG_ASSAULT BIT15
//#define BIT16
#define LAWFLAG2_CARTHEFT BIT1
#define LAWFLAG2_INFORMATION BIT2
#define LAWFLAG2_COMMERCE BIT3
#define LAWFLAG2_CCFRAUD BIT4
#define LAWFLAG2_BROWNIES BIT5
#define LAWFLAG2_BURIAL BIT6
#define LAWFLAG2_PROSTITUTION BIT7
#define LAWFLAG2_DISTURBANCE BIT8
#define LAWFLAG2_HIREILLEGAL BIT9
#define LAWFLAG2_RACKETEERING BIT10
#define LAWFLAG2_LOITERING BIT11

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
	ACTIVITYNUM
};

struct activityst
{
	long type;
	long arg;
	long arg2;
};

long curcreatureid=0;

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

	unsigned short lawflag;
	unsigned short lawflag2;
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
	void creatureinit(void)
		{
		hireid=-1;
		worklocation=0;
		juice=0;
		flag=0;
		carid=-1;
		is_driver=0;
		pref_carid=-1;
		pref_is_driver=0;
		id=curcreatureid;curcreatureid++;
		joindays=0;
		deathdays=0;
		squadid=-1;
		cantbluff=0;
		location=0;
		base=0;
		activity.type=ACTIVITY_NONE;
		lawflag=0;
		lawflag2=0;
		clinic=0;
		dating=0;
		hiding=0;
		trainingtime=0;
		trainingsubject=-1;
		specialattack=-1;
		animalgloss=0;
		prisoner=NULL;
		alive=1;
		blood=100;
		for(int w=0;w<BODYPARTNUM;w++)wound[w]=0;
		weapon.type=WEAPON_NONE;
		weapon.ammo=0;
		armor.type=ARMOR_CLOTHES;
		armor.quality='1';
		armor.flag=0;
		for(int a=0;a<ATTNUM;a++)
			{
			att[a]=1;
			}
		int attnum=32;
		while(attnum>0)
			{
			int a=LCSrandom(ATTNUM);
			if(att[a]<10)
				{
				att[a]++;
				attnum--;
				}
			}
		for(int s=0;s<SKILLNUM;s++)
			{
			skill[s]=0;
			skill_ip[s]=0;
			}
		for(int c=0;c<CLIPNUM;c++)clip[c]=0;
		special[SPECIALWOUND_TEETH]=TOOTHNUM;
		special[SPECIALWOUND_RIGHTEYE]=1;
		special[SPECIALWOUND_LEFTEYE]=1;
		special[SPECIALWOUND_NOSE]=1;
		special[SPECIALWOUND_TONGUE]=1;
		special[SPECIALWOUND_RIGHTLUNG]=1;
		special[SPECIALWOUND_LEFTLUNG]=1;
		special[SPECIALWOUND_HEART]=1;
		special[SPECIALWOUND_LIVER]=1;
		special[SPECIALWOUND_STOMACH]=1;
		special[SPECIALWOUND_RIGHTKIDNEY]=1;
		special[SPECIALWOUND_LEFTKIDNEY]=1;
		special[SPECIALWOUND_SPLEEN]=1;
		special[SPECIALWOUND_RIBS]=RIBNUM;
		special[SPECIALWOUND_NECK]=1;
		special[SPECIALWOUND_UPPERSPINE]=1;
		special[SPECIALWOUND_LOWERSPINE]=1;
		forceinc=0;
		sentence=0;
		deathpenalty=0;
		}
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

vector<itemst *> groundloot;

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

	siegest()
		{
		siege=0;
		siegetype=-1;
		underattack=0;
		escalationstate=0;
		timeuntillocated=-1;
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
	void init(void)
		{
		haveflag=0;
		newrental=0;
		closed=0;
		highsecurity=0;
		mapseed=seed;r_num();
		compound_walls=0;
		compound_stores=0;
		front_business=-1;
		}
};
vector<locationst *> location;

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
vector<vehiclest *> vehicle;
long curcarid=0;
char showcarprefs=1;

enum CarChaseObstacles
{
	CARCHASE_OBSTACLE_FRUITSTAND,
	CARCHASE_OBSTACLENUM
};

#define MAPX 70
#define MAPY 23
#define MAPZ 10

siteblockst map[MAPX][MAPY][MAPZ];

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

	void chaseseqst::clean(void)
		{
		for(int v=0;v<enemycar.size();v++)
			{
			delete enemycar[v];
			}
		enemycar.clear();

		friendcar.clear(); 
		}



chaseseqst chaseseq;

char slogan[80];

vector<creaturest *> pool;

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
		for(int l=0;l<loot.size();l++)
			{
			delete loot[l];
			}

		loot.clear();
		}
};
vector<squadst *> squad;
squadst *activesquad=NULL;
long cursquadid=0;

char disbanding=0;
int disbandtime=0;
char cantseereason;

#define ENCMAX 18
creaturest encounter[ENCMAX];

enum GameModes
{
	GAMEMODE_TITLE,
	GAMEMODE_BASE,
	GAMEMODE_SITE,
	GAMEMODE_CHASECAR,
	GAMEMODE_CHASEFOOT
};

char loaded=0;

int mode=GAMEMODE_TITLE;

short offended_cops=0;
short offended_corps=0;
short offended_cia=0;
short offended_amradio=0;
short offended_cablenews=0;
unsigned long attorneyseed;
long selectedsiege=-1;
char lcityname[80];
char newscherrybusted=0;

int month=1;
int year=2005;
int amendnum=28;

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
	VIEW_AMRADIO,
	VIEW_CABLENEWS,
	//THESE TWO MUST BE LAST FOR VIEWNUM-2 TO WORK IN PLACES
	VIEW_LIBERALCRIMESQUAD,
	VIEW_LIBERALCRIMESQUADPOS,
	VIEWNUM
};

short attitude[VIEWNUM];

short newspaper_topicwork[VIEWNUM];

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
	LAW_TAX,
	LAW_FLAGBURNING,
	LAWNUM
};

short law[LAWNUM];

short house[435];
short senate[100];
short court[9];
char courtname[9][80];

enum Execs
{
	EXEC_PRESIDENT,
	EXEC_VP,
	EXEC_STATE,
	EXEC_ATTORNEY,
	EXECNUM
};

short exec[EXECNUM];
short execterm=1;
char execname[EXECNUM][80];

unsigned long version=30900;
unsigned long lowestloadversion=30500;
unsigned long lowestloadscoreversion=30001;

unsigned long stat_recruits=0;
unsigned long stat_kidnappings=0;
unsigned long stat_dead=0;
unsigned long stat_kills=0;
unsigned long stat_funds=0;
unsigned long stat_spent=0;
unsigned long stat_buys=0;
unsigned long stat_burns=0;

unsigned long ustat_recruits=0;
unsigned long ustat_kidnappings=0;
unsigned long ustat_dead=0;
unsigned long ustat_kills=0;
unsigned long ustat_funds=0;
unsigned long ustat_spent=0;
unsigned long ustat_buys=0;
unsigned long ustat_burns=0;

int locx,locy,locz;

short sitetype;
short sitealienate;
short sitealarm;
short sitealarmtimer;
short postalarmtimer;
long sitecrime;
long cursite;

short interface_pgup='[';
short interface_pgdn=']';

int day=1;

#ifdef HIGHFUNDS
unsigned long funds=100000;
#else
unsigned long funds=7;
#endif

long moneygained_donate=0;
long moneygained_brownies=0;
long moneygained_goods=0;
long moneygained_ccfraud=0;
long moneygained_hustling=0;
long moneygained_thievery=0;
long moneylost_goods=0;
long moneylost_trouble=0;
long moneylost_rent=0;
long moneylost_manufacture=0;
long moneylost_legal=0;
long moneylost_compound=0;
long moneylost_hostage=0;

short party_status=-1;

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
		for(int d=0;d<date.size();d++)delete date[d];
		date.clear();
		}
};
vector<datest *> date;

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
vector<newsstoryst *> newsstory;
newsstoryst *sitestory=NULL;

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
highscorest score[SCORENUM];
int yourscore=-1;

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
void criminalizeparty(short crime,char flag2=0);
void criminalizepool(short crime,long exclude=-1,char flag2=0,short loc=-1);
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
void sleepereffect(creaturest &cr,char &clearformess);
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
 *     based on the value of a MACRO GO_PORTABLE.
 * 
 * compat.cpp is the place for non-trivial or more global functions.
 *--------------------------------------------------------------------------*/
 
 unsigned long getSeed(void)
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
  





int main(int argc, char* argv[])
{
	//start curses
	initscr();

	//initialize curses color
	start_color();

	seed=getSeed();
	
	//initialize the array of color pairs
	for(int i=0;i<8;i++)
		{
		for(int j=0;j<8;j++)
			{
			if(i==0&&j==0)
				{
				init_pair(7*8,0,0);
				continue;
				}
			if(i==7&&j==0)continue;
			init_pair(i*8+j,i,j);
			}
		}

	//turns off cursor
	curs_set(0);

	//begin the game loop
	keypad(stdscr,TRUE);
	
	raw_output(TRUE);

	loadgraphics();

	loadinitfile();

	strcpy(slogan,"We need a slogan!");

	for(int v=0;v<VIEWNUM;v++)
		{
		attitude[v]=45;
		newspaper_topicwork[v]=0;
		}
	attitude[VIEW_LIBERALCRIMESQUAD]=0;
	attitude[VIEW_LIBERALCRIMESQUADPOS]=20;

	law[LAW_ABORTION]=2;
	law[LAW_ANIMALRESEARCH]=-2;
	law[LAW_POLICEBEHAVIOR]=0;
	law[LAW_PRIVACY]=0;
	law[LAW_DEATHPENALTY]=-1;
	law[LAW_NUCLEARPOWER]=-1;
	law[LAW_POLLUTION]=-1;
	law[LAW_LABOR]=0;
	law[LAW_GAY]=1;
	law[LAW_CORPORATE]=0;
	law[LAW_FREESPEECH]=0;
	law[LAW_FLAGBURNING]=1;
	law[LAW_TAX]=0;

#ifdef SHITLAWS
	for(int l=0;l<LAWNUM;l++)law[l]=-2;
#endif

	for(int s=0;s<100;s++)
		{
		if(s<20)senate[s]=-2;
		else if(s<45)senate[s]=-1;
		else if(s<70)senate[s]=0;
		else if(s<90)senate[s]=1;
		else senate[s]=2;
		}

	for(int h=0;h<435;h++)
		{
		if(h<30)house[h]=-2;
		else if(h<200)house[h]=-1;
		else if(h<300)house[h]=0;
		else if(h<400)house[h]=1;
		else house[h]=2;
		}

	for(int c=0;c<9;c++)
		{
		if(c<2)court[c]=-2;
		else if(c<5)court[c]=-1;
		else if(c<7)court[c]=0;
		else if(c<8)court[c]=1;
		else court[c]=2;
		name(courtname[c]);
		}

	for(int e=0;e<EXECNUM;e++)
		{
		exec[e]=-1;
		name(execname[e]);
		}

	attorneyseed=getSeed();
	cityname(lcityname);

	loaded=load();

	mode_title();

	//deinitialize curses
	endwin();

	return 0;
}

//sets current color to desired setting
void set_color(short f,short b,char bright)
{
	//color swap required for PDcurses
	if(f==7&&b==0)
		{
		f=0;b=0;
		}
	else if(f==0&&b==0)
		{
		f=7;b=0;
		}

	//pick color pair based on foreground and background
	if(bright)attrset(A_BOLD | COLOR_PAIR(f*8+b));
	else
		{
		attrset(COLOR_PAIR(f*8+b));
		}
}

//picks a random number from 0 to max-1
long LCSrandom(unsigned long max)
{
	r_num();

	long double rand_y;
	long double rand_i;

	rand_i = 2147483648UL;

	rand_y = max*((long double)seed/rand_i);

	return((long)rand_y);
}

//sets seed to a random number from 0 to 2 billion
unsigned long r_num(void)
{
	seed=(seed*907725L+99979777UL)%2147483648UL;
	return seed;
}

//IN CASE FUNKY ARROW KEYS ARE SENT IN, TRANSLATE THEM BACK
void translategetch(int &c)
{
	//if(c==-63)c='7';
	//if(c==-62)c='8';
	//if(c==-61)c='9';
	//if(c==-60)c='4';
	//if(c==-59)c='5';
	//if(c==-58)c='6';
	//if(c==-57)c='1';
	//if(c==-56)c='2';
	//if(c==-55)c='3';

	if(c==-6)c='0';
	if(c==-50)c='.';
	if(c==-53)c=10;
	if(c==-47)c='+';
	if(c==-48)c='-';
	if(c==-49)c='*';
	if(c==-54)c='/';

	/*
	if(c==2)c='2';
	if(c==3)c='8';
	if(c==4)c='4';
	if(c==5)c='6';
	*/

	if(c>='A'&&c<='Z'){c-='A';c+='a';}

	/* Support Cursor Keys...*/
	if(c==KEY_LEFT)c='a';
	if(c==KEY_RIGHT)c='d';
	if(c==KEY_UP)c='w';
	if(c==KEY_DOWN)c='x';

}
void translategetch_cap(int &c)
{
	//if(c==-63)c='7';
	//if(c==-62)c='8';
	//if(c==-61)c='9';
	//if(c==-60)c='4';
	//if(c==-59)c='5';
	//if(c==-58)c='6';
	//if(c==-57)c='1';
	//if(c==-56)c='2';
	//if(c==-55)c='3';

	if(c==-6)c='0';
	if(c==-50)c='.';
	if(c==-53)c=10;
	if(c==-47)c='+';
	if(c==-48)c='-';
	if(c==-49)c='*';
	if(c==-54)c='/';

	/*
	if(c==2)c='2';
	if(c==3)c='8';
	if(c==4)c='4';
	if(c==5)c='6';
	*/
}

void mode_title(void)
{
	//title screen
	erase();

	char str[100];

	set_color(COLOR_WHITE,COLOR_BLACK,1);
	strcpy(str,"Liberal Crime Squad");
	move(4,39-((strlen(str)-1)>>1));
	addstr(str);

	set_color(COLOR_WHITE,COLOR_BLACK,1);
	strcpy(str,"Inspired by the 1983 version of Oubliette");
	move(6,39-((strlen(str)-1)>>1));
	addstr(str);

	if(loaded)
		{
		char num[20];

		int l=strlen(slogan);
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(8,40-(l>>1));
		addstr(slogan);

		move(9,10);
		addstr("Liberals Indoctrinated: ");
		itoa(stat_recruits,num,10);
		addstr(num);

		move(10,10);
		addstr("Liberals Martyred: ");
		itoa(stat_dead,num,10);
		addstr(num);

		move(11,10);
		addstr("Conservatives Killed: ");
		itoa(stat_kills,num,10);
		addstr(num);

		move(9,47);
		addstr("Funds Taxed: ");
		itoa(stat_funds,num,10);
		addstr(num);

		move(10,47);
		addstr("Funds Spent: ");
		itoa(stat_spent,num,10);
		addstr(num);

		move(11,47);
		addstr("Conservatives Kidnapped: ");
		itoa(stat_kidnappings,num,10);
		addstr(num);
		}

	set_color(COLOR_WHITE,COLOR_BLACK,1);
	strcpy(str,"v3.09 Copyright (C) 2002-4, Tarn Adams");
	move(13,39-((strlen(str)-1)>>1));
	addstr(str);
	strcpy(str,"A Bay 12 Games Production");
	move(14,39-((strlen(str)-1)>>1));
	addstr(str);
	strcpy(str,"www.bay12games.com");
	move(15,39-((strlen(str)-1)>>1));
	addstr(str);
	strcpy(str,"Press ESC now to quit.  Quitting later causes your progress to be saved.");
	move(20,39-((strlen(str)-1)>>1));
	addstr(str);
	strcpy(str,"Press any other key to pursue your Liberal Agenda!");
	move(22,39-((strlen(str)-1)>>1));
	addstr(str);

	int c=getch();
	translategetch(c);
	if(c==27)
		{
		endwin();
		exit(0);
		}

	viewhighscores();

	if(!loaded)makecharacter();
	mode=GAMEMODE_BASE;
	mode_base();

	save();
}

void mode_base(void)
{
	short advanced=0;
	short directing=0;
	short in_newspaper=0;
	short in_halloween=0;
	short in_halloween2=0;
	short in_training=0;
	short in_compound=0;

	short investing=0;
	short investing_newspaper=0;
	short investing_halloween=0;
	short investing_training=0;
	short investing_compound=0;
	short investing_stores=0;

	short buyer=0;

	char forcewait,canseethings;
	long nonsighttime=0;
	#ifdef SHOWWAIT
		int oldforcemonth=month;
	#endif
	
	int length=0;

	long l = 0;

	do
		{
		forcewait=1;
		canseethings=0;
		cantseereason=3;
		if(!disbanding)
			{
			for(int p=0;p<pool.size();p++)
				{
				if(pool[p]->alive&&
					pool[p]->align==1&&
					pool[p]->dating==0&&
					pool[p]->hiding==0&&
					!(pool[p]->flag & CREATUREFLAG_SLEEPER))
					{
					if(location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON&&
						location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
						location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION)
						{
						canseethings=1;
						if(pool[p]->clinic==0){forcewait=0;break;}
						}
					}
				else
					{
					if(pool[p]->dating==1&&cantseereason>1)cantseereason=1;
					else if(pool[p]->hiding==1&&cantseereason>2)cantseereason=2;
					}
				}
			}
		else
			{
			cantseereason=4;
			}

		#ifdef SHOWWAIT
		if(forcewait&&oldforcemonth!=month)
			{
			oldforcemonth=month;
			erase();
			move(0,0);
			char num[20];
			itoa(year,num,10);
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			addstr(num);


	int y=2;

	for(int v=0;v<VIEWNUM;v++)
		{
		if(attitude[VIEW_LIBERALCRIMESQUAD]==0&&
			v==VIEW_LIBERALCRIMESQUADPOS)continue;

		if(attitude[v]==-1)set_color(COLOR_BLACK,COLOR_BLACK,1);
		else if(attitude[v]<10)set_color(COLOR_RED,COLOR_BLACK,1);
		else if(attitude[v]<30)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
		else if(attitude[v]<50)set_color(COLOR_YELLOW,COLOR_BLACK,1);
		else if(attitude[v]<70)set_color(COLOR_BLUE,COLOR_BLACK,1);
		else if(attitude[v]<90)set_color(COLOR_CYAN,COLOR_BLACK,1);
		else set_color(COLOR_GREEN,COLOR_BLACK,1);

		move(y,0);
		if(attitude[v]==-1)
			{
			addstr("Unknown how many ");
			}
		else
			{
			itoa(attitude[v],num,10);
			addstr(num);
			addstr("% ");
			}

		switch(v)
			{
			case VIEW_ABORTION:addstr("supported abortion.");break;
			case VIEW_GAY:addstr("were in favor of equal rights for homosexuals.");break;
			case VIEW_DEATHPENALTY:addstr("opposed the death penalty.");break;
			case VIEW_TAXES:addstr("were against cutting taxes.");break;
			case VIEW_NUCLEARPOWER:addstr("were terrified of nuclear power.");break;
			case VIEW_ANIMALRESEARCH:addstr("deplored animal research.");break;
			case VIEW_POLICEBEHAVIOR:addstr("were critical of the police.");break;
			case VIEW_INTELLIGENCE:addstr("thought the intelligence community invades privacy.");break;
			case VIEW_FREESPEECH:addstr("believed in unfettered free speech.");break;
			case VIEW_GENETICS:addstr("abhorred genetically altered food products.");break;
			case VIEW_JUSTICES:addstr("were for the appointment of Liberal justices.");break;
			case VIEW_SWEATSHOPS:addstr("would boycott companies that used sweatshops.");break;
			case VIEW_POLLUTION:addstr("thought industry should lower pollution.");break;
			case VIEW_CORPORATECULTURE:addstr("were disgusted by corporate malfeasance.");break;
			case VIEW_CEOSALARY:addstr("believed that CEO salaries are too great.");break;
			case VIEW_LIBERALCRIMESQUAD:addstr("had heard of the Liberal Crime Squad.");break;
			case VIEW_LIBERALCRIMESQUADPOS:addstr("of these held the Liberal Crime Squad in high regard.");break;
			case VIEW_PRISONS:addstr("think the prison system needs reform.");break;
			case VIEW_AMRADIO:addstr("do not like AM radio.");break;
			case VIEW_CABLENEWS:addstr("have a negative opinion of cable news programs.");break;
			}
		y++;
		}

			refresh();
			}
		#endif

		if(!forcewait)
			{
			if(nonsighttime>=365*4)
				{
				erase();
				char str[100];
				if(nonsighttime>=365*16)
					{
					strcpy(str,"How long since you've heard these sounds...  times have changed.");
					}
				else if(nonsighttime>=365*8)
					{
					strcpy(str,"It has been a long time.  A lot must have changed...");
					}
				else
					{
					strcpy(str,"It sure has been a while.  Things might have changed a bit.");
					}
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(12,39-((strlen(str)-1)>>1));
				addstr(str);

				refresh();
				getch();
				}

			nonsighttime=0;
			}

		int partysize=0;
		int partydead=0;
		if(activesquad!=NULL)
			{
			for(int p=0;p<6;p++)
				{
				if(activesquad->squad[p]!=NULL)partysize++;
				else
					{
					if(p==buyer)buyer=0;
					continue;
					}

				if(!activesquad->squad[p]->alive)partydead++;
				}
			}

		long safenumber=0;
		for(l=0;l<location.size();l++)
			{
			if(location[l]->renting>=0)safenumber++;
			}

		siegest *siege=NULL;
		if(selectedsiege!=-1)siege=&location[selectedsiege]->siege;
		if(activesquad!=NULL)siege=&location[activesquad->squad[0]->location]->siege;
		char sieged=0;
		if(siege!=NULL)sieged=siege->siege;
		char underattack=0;
		if(siege!=NULL)
			{
			if(sieged)underattack=siege->underattack;
			}
		
		char haveflag=0;
		if(selectedsiege!=-1)haveflag=location[selectedsiege]->haveflag;
		if(activesquad!=NULL)haveflag=location[activesquad->squad[0]->location]->haveflag;

		char cannotwait=0;
		for(l=0;l<location.size();l++)
			{
			if(!location[l]->siege.siege)continue;

			if(location[l]->siege.underattack)
				{
				cannotwait=1;
				break;
				}
			//NOTE: returns -1 if no eaters, so is okay
			if(fooddaysleft(l)==0)
				{
				cannotwait=1;
				break;
				}
			}

		if(!forcewait)
			{
			erase();

			if(activesquad!=NULL)selectedsiege=-1;

			locheader();
			if(selectedsiege!=-1)
				{
				printlocation(selectedsiege);

				if(location[selectedsiege]->type==SITE_INDUSTRY_WAREHOUSE&&
					!location[selectedsiege]->siege.siege)
					{
					set_color(COLOR_WHITE,COLOR_BLACK,0);
					move(8,1);
					addstr("I - Invest in this location");
					}
				}
			else if(activesquad!=NULL)printparty();
			else makedelimiter(8,0);

			if(sieged)
				{
				move(8,1);
				if(underattack)
					{
					set_color(COLOR_RED,COLOR_BLACK,1);
					addstr("Under Attack");
					}
				else
					{
					set_color(COLOR_YELLOW,COLOR_BLACK,1);
					addstr("Under Siege");
					}
				}

			if(haveflag)
				{
				for(int p=0;p<7;p++)
					{
					move(p+17,32);
					if(p<3)
						{
						set_color(COLOR_WHITE,COLOR_BLUE,1);
						move(p+17,32);
						addstr("::::::");
						set_color(COLOR_WHITE,COLOR_RED,1);
						move(p+17,38);
						for(int i=0;i<10;i++)addch(CH_LOWER_HALF_BLOCK);
						}
					else
						{
						if(p<6)set_color(COLOR_WHITE,COLOR_RED,1);
						else set_color(COLOR_RED,COLOR_BLACK,0);
						for(int i=0;i<16;i++)
							{
							if(p==6)addch(CH_UPPER_HALF_BLOCK);
							else addch(CH_LOWER_HALF_BLOCK);
							}
						}
					}
				}

			if(partysize>0&&!underattack)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,1);
			addstr("E - Equipment");
			if(pool.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(12,1);
			addstr("R - Review and reorganize Liberals");
			if(partysize>1)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(12,40);
			addstr("O - Change the squad's Liberal order");

			if(squad.size()>1||(activesquad==NULL&&squad.size()>0))set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(13,40);
			addstr("TAB - Next Squad");

			if(safenumber>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(13,62);
			addstr("Z - Next Location");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(14,40);
			addstr("L - The Status of the Liberal Agenda");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(13,1);
			addstr("A - Activate the Uninvolved");

			if(partysize>0)
				{
				if(activesquad->activity.type!=ACTIVITY_NONE)set_color(COLOR_WHITE,COLOR_BLACK,0);
				else set_color(COLOR_BLACK,COLOR_BLACK,1);
				}
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(14,1);
			addstr("C - Cancel this Squad's Departure");

			if(sieged)
				{
				if(partysize>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
				else set_color(COLOR_BLACK,COLOR_BLACK,1);
				move(10,1);
				addstr("F - Escape/Engage");

				set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(10,23);
				addstr("G - Give Up");
				}
			else
				{
				if(partysize>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
				else set_color(COLOR_BLACK,COLOR_BLACK,1);
				move(10,1);
				addstr("F - Go forth to stop EVIL");
				}
			if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,40);
			addstr("# - Check the status of a squad Liberal");
			if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,40);
			addstr("0 - Show the squad's Liberal status");
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(15,1);
			addstr("X - Live to fight EVIL another day");
			move(16,1);
			if(cannotwait)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,0);
				addstr("Cannot Wait until Siege Resolved");
				}
			else
				{
				move(16,1);
				set_color(COLOR_WHITE,COLOR_BLACK,0);
				addstr("W - Wait a day");
				if(day==monthday())addstr(" (next month)");
				}

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(16,40);
			addstr("S - FREE SPEECH: the Liberal Slogan");
			if(vehicle.size()>0&&partysize>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,23);
			addstr("V - Vehicles");
			move(15,40);
			if(haveflag)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,0);
				addstr("P - PROTEST: burn the flag");
				}
			else
				{
				if(funds>=20&&!sieged&&
					(selectedsiege!=-1||activesquad!=NULL))set_color(COLOR_WHITE,COLOR_BLACK,0);
				else set_color(COLOR_BLACK,COLOR_BLACK,1);
				addstr("P - PATRIOTISM: fly a flag here ($20)");
				}

			length=strlen(slogan);
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			if(haveflag)move(24,40-(length>>1));
			else move(20,40-(length>>1));
			addstr(slogan);

			refresh();
			}

		int c='w';
		if(!forcewait)
			{
			c=getch();
			translategetch(c);
			}

		if(c=='x')break;

		if(c=='i'&&selectedsiege!=-1)
			{
			if(location[selectedsiege]->type==SITE_INDUSTRY_WAREHOUSE&&
				!location[selectedsiege]->siege.siege)
				{
				investlocation();
				}
			}

		if(c=='l')
			{
			disbanding=liberalagenda(0);
			}

		if(c=='g'&&sieged)
			{
			giveup();
			cleangonesquads();
			}

		if(c=='f'&&partysize>0)
			{
			//NOTE THAT THERE ARE TWO MORE OF THESE
				//IDENTICAL LINES BELOW
			if(!sieged)
				{
				stopevil();
				}
			else
				{
				escape_engage();
				cleangonesquads();
				}
			}

		if(c=='o'&&partysize>1)orderparty();

		if(c=='c'&&partysize>0)
			{
			activesquad->activity.type=ACTIVITY_NONE;
			}

		if(c=='a')
			{
			activate();
			}

		if(c==9&&squad.size()>0)
			{
			if(activesquad==NULL)activesquad=squad[0];
			else
				{
				for(int sq=0;sq<squad.size();sq++)
					{
					if(squad[sq]==activesquad)
						{
						if(sq==squad.size()-1)activesquad=squad[0];
						else activesquad=squad[sq+1];
						break;
						}
					}
				}
			}

		if(c=='z'&&safenumber>0)
			{
			activesquad=NULL;
			long sl;
			if(selectedsiege==-1)sl=0;
			else sl=selectedsiege+1;

			for(long l=sl;l<location.size();l++)
				{
				if(location[l]->renting>=0)
					{
					selectedsiege=l;
					break;
					}
				else if(l==location.size()-1)l=-1;
				}
			}

		if(c=='e'&&partysize>0&&!underattack)
			{
			if(location[activesquad->squad[0]->location]->renting>=0)
				{
				equip(activesquad->loot,activesquad->squad[0]->location);
				}
			else equip(activesquad->loot,-1);
			}

		if(c=='r'&&pool.size()>0)review();

		if(c=='w'&&(forcewait||!cannotwait))
			{
			char clearformess=forcewait;
			if(!canseethings)nonsighttime++;
			advanceday(clearformess,canseethings);
			if(day>monthday())passmonth(clearformess,canseethings);
			advancelocations();
			}

		if(c=='v'&&vehicle.size()>0&&partysize>0)setvehicles();

		if(c=='p'&&haveflag)
			{
			burnflag();
			stat_burns++;
			if(selectedsiege!=-1)
				{
				location[selectedsiege]->haveflag=0;
				criminalizepool(LAWFLAG_BURNFLAG,-1,0,selectedsiege);
				}
			if(activesquad!=NULL)
				{
				location[activesquad->squad[0]->base]->haveflag=0;
				criminalizepool(LAWFLAG_BURNFLAG,-1,0,activesquad->squad[0]->base);
				}

			//PUBLICITY IF BURN FLAG DURING SIEGE
				//ESPECIALLY IF IT IS REALLY ILLEGAL
			if(sieged)
				{
				change_public_opinion(VIEW_LIBERALCRIMESQUAD,1,0);
				change_public_opinion(VIEW_FREESPEECH,1,1);
				if(law[LAW_FLAGBURNING]<=0)
					{
					change_public_opinion(VIEW_LIBERALCRIMESQUAD,1,0);
					change_public_opinion(VIEW_FREESPEECH,1,1);
					}
				if(law[LAW_FLAGBURNING]<=-1)
					{
					change_public_opinion(VIEW_LIBERALCRIMESQUAD,5,0);
					change_public_opinion(VIEW_FREESPEECH,2,1);
					}
				if(law[LAW_FLAGBURNING]==-2)
					{
					change_public_opinion(VIEW_LIBERALCRIMESQUAD,15,0);
					change_public_opinion(VIEW_FREESPEECH,5,1);
					}
				}
			}
		else if(c=='p'&&funds>=20&&!sieged&&
					(selectedsiege!=-1||activesquad!=NULL))
			{
			funds-=20;
			stat_spent+=20;
			if(selectedsiege!=-1)location[selectedsiege]->haveflag=1;
			if(activesquad!=NULL)location[activesquad->squad[0]->base]->haveflag=1;
			stat_buys++;
			moneylost_compound+=20;
			}

		if(c=='s')getslogan();

		if(c=='0')party_status=-1;

		if(c>='1'&&c<='6'&&activesquad!=NULL)
			{
			if(activesquad->squad[c-'1']!=NULL)
				{
				if(party_status==c-'1')fullstatus(party_status);
				else party_status=c-'1';
				}
			}

		}while(1);
}

void initliberal(creaturest &cr)
{
	cr.creatureinit();

	cr.prisoner=NULL;

	cr.align=1;
	cr.type=CREATURE_BUM;
	cr.money=0;

	namecreature(cr);

	//STARTING SKILLS
	int startsknum=cr.attval(ATTRIBUTE_WISDOM);

	while(startsknum>0)
		{
		cr.skill[LCSrandom(SKILLNUM)]++;
		startsknum--;
		}
}

void printliberalstats(creaturest &cr,char smll)
{
	set_color(COLOR_WHITE,COLOR_BLACK,0);

	char num[20],str[200];

	move(2,0);
	addstr("Name: ");
	addstr(cr.name);
	addstr(", ");
	gettitle(str,cr);
	addstr(str);

	move(4,25);addstr("Juice: ");
	itoa(cr.juice,num,10);
	addstr(num);
	if(cr.juice<500)
		{
		move(5,25);addstr("Next: ");
		if(cr.juice<0)addstr("0");
		else if(cr.juice<10)addstr("10");
		else if(cr.juice<50)addstr("50");
		else if(cr.juice<100)addstr("100");
		else if(cr.juice<200)addstr("200");
		else addstr("500");
		}

	move(4,0);addstr("Heart: ");
	itoa(cr.attval(ATTRIBUTE_HEART),num,10);
	addstr(num);
	move(5,0);addstr("Intelligence: ");
	itoa(cr.attval(ATTRIBUTE_INTELLIGENCE),num,10);
	addstr(num);
	move(6,0);addstr("Wisdom: ");
	itoa(cr.attval(ATTRIBUTE_WISDOM),num,10);
	addstr(num);
	move(7,0);addstr("Health: ");
	itoa(cr.attval(ATTRIBUTE_HEALTH),num,10);
	addstr(num);
	move(8,0);addstr("Agility: ");
	itoa(cr.attval(ATTRIBUTE_AGILITY),num,10);
	addstr(num);
	move(9,0);addstr("Strength: ");
	itoa(cr.attval(ATTRIBUTE_STRENGTH),num,10);
	addstr(num);
	move(10,0);addstr("Charisma: ");
	itoa(cr.attval(ATTRIBUTE_CHARISMA),num,10);
	addstr(num);

	char used[SKILLNUM];
	memset(used,0,sizeof(char)*SKILLNUM);

	int snum=14;
	if(smll)snum=7;
	char printed=1;

	while(snum>0&&printed)
		{
		printed=0;

		unsigned long max=0;
		long maxs=-1;
		for(int s=0;s<SKILLNUM;s++)
			{
			if(cr.skill[s]>max && !used[s])
				{
				max=cr.skill[s];
				maxs=s;
				}
			}

		if(maxs!=-1)
			{
			used[maxs]=1;
			printed=1;

			if(cr.skill_ip[maxs]>=100)set_color(COLOR_WHITE,COLOR_BLACK,1);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);

			if(!smll)move(4+14-snum,40);
			else move(4+7-snum,40);
			getskill(str,maxs);
			strcat(str,": ");
			itoa(cr.skill[maxs],num,10);
			strcat(str,num);
			addstr(str);

			if((snum==14&&!smll)||(snum==7&&smll))
				{
				set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(2,40);
				getrecruitcreature(str,cr.type);
				addstr(str);
				}
			}

		snum--;
		}

	set_color(COLOR_WHITE,COLOR_BLACK,0);

	if(!smll)
		{
		move(12,0);
		addstr("Weapon: ");
		getweaponfull(str,cr.weapon.type);
		addstr(str);

		if(ammotype(cr.weapon.type)!=-1)
			{
			char num[20];
			itoa(cr.weapon.ammo,num,10);
			addstr(" (");
			addstr(num);
			int at=ammotype(cr.weapon.type);
			itoa(cr.clip[at],num,10);
			addstr("/");
			addstr(num);
			addstr(")");
			}

		move(13,0);
		addstr("Armor: ");
		getarmorfull(str,cr.armor,1);
		addstr(str);

		move(14,0);
		addstr("Transport: ");
		long v=-1;
		if(showcarprefs==1)v=id_getcar(cr.pref_carid);
		else v=id_getcar(cr.carid);
		if(v!=-1&&showcarprefs!=-1)
			{
			getcarfull(str,*vehicle[v]);
			char d;
			if(showcarprefs==1)d=cr.pref_is_driver;
			else d=cr.is_driver;
			if(d)strcat(str,"-D");
			}
		else
			{
			int legok=2;
			if((cr.wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
				 (cr.wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
			if((cr.wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
				 (cr.wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;
			if(cr.flag & CREATUREFLAG_WHEELCHAIR)strcpy(str,"Wheelchair");
			else if(legok>=1)strcpy(str,"On Foot");
			else strcpy(str,"On \"Foot\"");
			}
		addstr(str);

		int woundsum=0;
		for(int w=0;w<BODYPARTNUM;w++)
			{
			if(cr.wound[w]!=0)woundsum++;
			}

		if(woundsum>0)
			{
			for(int w=0;w<BODYPARTNUM;w++)
				{
				if(cr.wound[w] & WOUND_BLEEDING)set_color(COLOR_RED,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);

				move(16+w,0);
				switch(w)
					{
					case BODYPART_HEAD:addstr("Head:");break;
					case BODYPART_BODY:addstr("Body:");break;
					case BODYPART_ARM_RIGHT:addstr("Right Arm:");break;
					case BODYPART_ARM_LEFT:addstr("Left Arm:");break;
					case BODYPART_LEG_RIGHT:addstr("Right Leg:");break;
					case BODYPART_LEG_LEFT:addstr("Left Leg:");break;
					}

				move(16+w,12);
				if(cr.wound[w] & WOUND_NASTYOFF)addstr("Ripped off (Scared Straight)");
				else if(cr.wound[w] & WOUND_CLEANOFF)addstr("Cleanly severed");
				else
					{
					int sum=0;

					if(cr.wound[w] & WOUND_SHOT)sum++;
					if(cr.wound[w] & WOUND_CUT)sum++;
					if(cr.wound[w] & WOUND_BRUISED)sum++;
					if(cr.wound[w] & WOUND_BURNED)sum++;
					if(cr.wound[w] & WOUND_TORN)sum++;

					if(sum==0)
						{
						set_color(COLOR_GREEN,COLOR_BLACK,1);
						addstr("Liberal");
						}

					if(cr.wound[w] & WOUND_SHOT){addstr("Shot");sum--;if(sum>0)addstr(",");}
					if(cr.wound[w] & WOUND_BRUISED){addstr("Bruised");sum--;if(sum>0)addstr(",");}
					if(cr.wound[w] & WOUND_CUT){addstr("Cut");sum--;if(sum>0)addstr(",");}
					if(cr.wound[w] & WOUND_TORN){addstr("Torn");sum--;if(sum>0)addstr(",");}
					if(cr.wound[w] & WOUND_BURNED){addstr("Burned");sum--;if(sum>0)addstr(",");}
					}
				}
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			}
		}

	//SPECIAL WOUNDS
	if(!smll)
		{
		set_color(COLOR_GREEN,COLOR_BLACK,0);

		move(3,62);
		if(cr.special[SPECIALWOUND_HEART]!=1)addstr("Heart Punctured");
		move(4,62);
		if(cr.special[SPECIALWOUND_RIGHTLUNG]!=1)addstr("R. Lung Collapsed");
		move(5,62);
		if(cr.special[SPECIALWOUND_LEFTLUNG]!=1)addstr("L. Lung Collapsed");
		move(6,62);
		if(cr.special[SPECIALWOUND_NECK]!=1)addstr("Broken Neck");
		move(7,62);
		if(cr.special[SPECIALWOUND_UPPERSPINE]!=1)addstr("Broken Up Spine");
		move(8,62);
		if(cr.special[SPECIALWOUND_LOWERSPINE]!=1)addstr("Broken Lw Spine");
		move(9,62);
		if(cr.special[SPECIALWOUND_RIGHTEYE]!=1)addstr("No Right Eye");
		move(10,62);
		if(cr.special[SPECIALWOUND_LEFTEYE]!=1)addstr("No Left Eye");
		move(11,62);
		if(cr.special[SPECIALWOUND_NOSE]!=1)addstr("No Nose");
		move(12,62);
		if(cr.special[SPECIALWOUND_TONGUE]!=1)addstr("No Tongue");
		move(13,62);
		if(cr.special[SPECIALWOUND_TEETH]==0)addstr("No Teeth");
		else if(cr.special[SPECIALWOUND_TEETH]==TOOTHNUM-1)addstr("Missing a Tooth");
		else if(cr.special[SPECIALWOUND_TEETH]<TOOTHNUM)addstr("No Teeth");
		move(14,62);
		if(cr.special[SPECIALWOUND_LIVER]!=1)addstr("Liver Damaged");
		move(15,62);
		if(cr.special[SPECIALWOUND_RIGHTKIDNEY]!=1)addstr("R. Kidney Damaged");
		move(16,62);
		if(cr.special[SPECIALWOUND_LEFTKIDNEY]!=1)addstr("L. Kidney Damaged");
		move(17,62);
		if(cr.special[SPECIALWOUND_STOMACH]!=1)addstr("Stomach Injured");
		move(18,62);
		if(cr.special[SPECIALWOUND_SPLEEN]!=1)addstr("Busted Spleen");
		move(19,62);
		if(cr.special[SPECIALWOUND_RIBS]==0)addstr("All Ribs Broken");
		else if(cr.special[SPECIALWOUND_RIBS]==RIBNUM-1)addstr("Broken Rib");
		else if(cr.special[SPECIALWOUND_RIBS]<RIBNUM)addstr("Broken Ribs");

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		}
}

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

void review(void)
{
	int page=0;

	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addstr("Review your Liberals and Assemble Squads");

		move(1,0);
		addstr("----SQUAD NAME-----------------LOCATION------------ACTIVITY----------------------");

		int y=2;
		for(int p=page*19;p<squad.size()+REVIEWMODENUM&&p<page*19+19;p++)
			{
			if(p<squad.size())
				{
				if(activesquad==squad[p])set_color(COLOR_WHITE,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(y,0);
				addch(y+'A'-2);addstr(" - ");
				addstr(squad[p]->name);

				move(y,31);
				if(squad[p]->squad[0]!=NULL)
					{
					addstr(location[squad[p]->squad[0]->location]->shortname);
					}

				move(y,51);
				if(squad[p]->squad[0]!=NULL)
					{
					char str[80];
					getactivity(str,squad[p]->activity);

					if(squad[p]->activity.type==ACTIVITY_NONE)
						{
						int count=0;char haveact=0;
						for(int p2=0;p2<6;p2++)
							{
							if(squad[p]->squad[p2]==NULL)continue;
							count++;
							if(squad[p]->squad[p2]->activity.type!=ACTIVITY_NONE)
								{
								getactivity(str,squad[p]->squad[p2]->activity);
								haveact=1;
								}
							}
						if(haveact&&count>1)strcpy(str,"Acting Individually");
						}

					addstr(str);
					}
				}
			else if(p==squad.size())
				{
				set_color(COLOR_GREEN,COLOR_BLACK,1);
				move(y,0);
				addch('1');addstr(" - ");
				addstr("Active Liberals");
				}
			else if(p==squad.size()+1)
				{
				set_color(COLOR_RED,COLOR_BLACK,1);
				move(y,0);
				addch('2');addstr(" - ");
				addstr("Hostages");
				}
			else if(p==squad.size()+2)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(y,0);
				addch('3');addstr(" - ");
				addstr("CLINIC");
				}
			else if(p==squad.size()+3)
				{
				set_color(COLOR_YELLOW,COLOR_BLACK,1);
				move(y,0);
				addch('4');addstr(" - ");
				addstr("Justice System");
				}
			else if(p==squad.size()+4)
				{
				set_color(COLOR_MAGENTA,COLOR_BLACK,1);
				move(y,0);
				addch('5');addstr(" - ");
				addstr("Sleepers");
				}
			else if(p==squad.size()+5)
				{
				set_color(COLOR_BLACK,COLOR_BLACK,1);
				move(y,0);
				addch('6');addstr(" - ");
				addstr("The Dead");
				}
			else if(p==squad.size()+6)
				{
				set_color(COLOR_BLUE,COLOR_BLACK,1);
				move(y,0);
				addch('7');addstr(" - ");
				addstr("Away");
				}

			y++;
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(22,0);
		addstr("Press a Letter to select a squad.  1-7 to view Liberal groups.");
		move(23,0);
		if(interface_pgup=='[')
			{
			addstr("[] to view other Liberal pages.  Press U to Promote Liberals.");
			}
		else
			{
			addstr("PGUP/PGDN to view other Liberal pages.  Press U to Promote Liberals.");
			}
		move(24,0);
		addstr("Press Z to Assemble a New Squad.  Press T to Assign New Bases to the Squadless.");

		refresh();

		int c=getch();
		translategetch(c);

		if(c==interface_pgup&&page>0)page--;
		if(c==interface_pgdn&&(page+1)*19<squad.size()+REVIEWMODENUM)page++;

		if(c==10)return;

		if(c>='a'&&c<='s')
			{
			int sq=page*19+(int)(c-'a');
			if(sq<squad.size()&&sq>=0)
				{
				if(squad[sq]==activesquad)assemblesquad(squad[sq]);
				else activesquad=squad[sq];
				}
			}
		if(c>='1'&&c<='7')review_mode((int)(c-'1'));
		if(c=='z')
			{
			assemblesquad(NULL);
			if(activesquad==NULL&&squad.size()>0)
				{
				activesquad=squad[squad.size()-1];
				}
			}
		if(c=='t')squadlessbaseassign();
		if(c=='u')promoteliberals();
		}while(1);
}

void assemblesquad(squadst *cursquad)
{
	long culloc=-1;
	int p;
	if(cursquad!=NULL)
		{
		culloc=cursquad->squad[0]->location;
		}

	char newsquad=0;
	if(cursquad==NULL)
		{
		cursquad=new squadst;
		cursquad->id=cursquadid;
		cursquadid++;
		newsquad=1;
		}

	vector<creaturest *> temppool;
	for(p=0;p<pool.size();p++)
		{
		if(pool[p]->alive==1&&
			pool[p]->align==1&&
			pool[p]->clinic==0&&
			pool[p]->dating==0&&
			pool[p]->hiding==0)
			{
			if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
				location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
				location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON&&
				(pool[p]->location==culloc||culloc==-1))temppool.push_back(pool[p]);
			}
		}

	//BUILD LIST OF BASES FOR EACH SQUAD IN CASE IT ENDS UP EMPTY
		//THEN WILL DROP ITS LOOT THERE
	vector<int> squadloc;
	squadloc.resize(squad.size());
	for(int sl=0;sl<squad.size();sl++)
		{
		squadloc[sl]=squad[sl]->squad[0]->location;
		if(squadloc[sl]!=-1)
			{
			if(location[squadloc[sl]]->renting==-1)
				{
				squadloc[sl]=-1;
				}
			}
		}

	short page=0;

	int squadsize;

	char num[20];

	do
		{
		squadsize=0;
		for(p=0;p<6;p++)if(cursquad->squad[p]!=NULL)squadsize++;

		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		if(squadsize<6)addstr("Assemble the squad!");
		else addstr("The squad is full.");

		if(newsquad)
			{
			move(0,71);
			addstr("New Squad");
			}
		else
			{
			move(0,73-strlen(cursquad->name));
			addstr("Squad: ");
			addstr(cursquad->name);
			}

		move(1,0);
		addstr("----CODE NAME------------SKILL---HEALTH-----------PROFESSION--------------------");

		int y=2;
		for(p=page*19;p<temppool.size()&&p<page*19+19;p++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addch(y+'A'-2);addstr(" - ");
			addstr(temppool[p]->name);

			char bright=0;
			unsigned long skill=0;
			for(int sk=0;sk<SKILLNUM;sk++)
				{
				skill+=(unsigned long)temppool[p]->skill[sk];
				if(temppool[p]->skill_ip[sk]>=100)bright=1;
				}

			set_color(COLOR_WHITE,COLOR_BLACK,bright);

			move(y,25);
			itoa(skill,num,10);
			addstr(num);

			printhealthstat(*temppool[p],y,33,FALSE);

			if(temppool[p]->squadid==cursquad->id)
				{
				set_color(COLOR_GREEN,COLOR_BLACK,1);
				move(y,75);
				addstr("SQUAD");
				}
			else if(temppool[p]->squadid!=-1)
				{
				set_color(COLOR_YELLOW,COLOR_BLACK,0);
				move(y,75);
				addstr("SQUAD");
				}
			else if(cursquad->squad[0]!=NULL)
				{
				if(cursquad->squad[0]->location!=temppool[p]->location)
					{
					set_color(COLOR_BLACK,COLOR_BLACK,1);
					move(y,75);
					addstr("AWAY");
					}
				}

			char str[200];
			if(temppool[p]->align==-1)set_color(COLOR_RED,COLOR_BLACK,1);
			else if(temppool[p]->align==0)set_color(COLOR_WHITE,COLOR_BLACK,1);
			else set_color(COLOR_GREEN,COLOR_BLACK,1);
			move(y,50);
			getrecruitcreature(str,temppool[p]->type);
			addstr(str);
			y++;
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(22,0);
		addstr("Press a Letter to add or remove a Liberal from the squad.");
		move(23,0);
		if(interface_pgup=='[')
			{
			addstr("[] to view other Liberal pages.");
			}
		else
			{
			addstr("PGUP/PGDN to view other Liberal pages.");
			}
		move(24,0);
		if(squadsize>0)addstr("1 - The squad is ready.");
		else addstr("1 - I need no squad!");
		if(squadsize>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(24,40);
		addstr("9 - Dissolve the squad.");

		refresh();

		int c=getch();
		translategetch(c);

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*19<temppool.size())page++;

		if(c>='a'&&c<='s')
			{
			int p=page*19+(int)(c-'a');
			if(p<temppool.size())
				{
				char conf=1;
				if(cursquad->squad[0]!=NULL)
					{
					if(cursquad->squad[0]->location!=temppool[p]->location)
						{
						set_color(COLOR_RED,COLOR_BLACK,1);
						move(22,0);
						addstr("                                                                                ");
						move(23,0);
						addstr("           Liberals must be in the same location to form a Squad.               ");
						move(24,0);
						addstr("                                                                                ");
						refresh();
						getch();

						conf=0;
						}
					}
				if(!temppool[p]->canwalk()&&
					!(temppool[p]->flag & CREATUREFLAG_WHEELCHAIR))
					{
					set_color(COLOR_RED,COLOR_BLACK,1);
					move(22,0);
					addstr("                                                                                ");
					move(23,0);
					addstr("                  Squad Liberals must be able to move around.                   ");
					move(24,0);
					addstr("                    Have this Liberal procure a wheelchair.                     ");
					refresh();
					getch();

					conf=0;
					}
				if(conf)
					{
					if(temppool[p]->squadid==cursquad->id)
						{
						char flipstart=0;
						for(int pt=0;pt<6;pt++)
							{
							if(cursquad->squad[pt]==temppool[p])
								{
								flipstart=1;
								cursquad->squad[pt]->squadid=-1;
								}
							if(flipstart&&pt<5)cursquad->squad[pt]=cursquad->squad[pt+1];
							}
						if(flipstart)cursquad->squad[5]=NULL;
						}
					else if(squadsize<6)
						{
						for(int pt=0;pt<6;pt++)
							{
							if(cursquad->squad[pt]==NULL)
								{
								removesquadinfo(*temppool[p]);
								cursquad->squad[pt]=temppool[p];
								temppool[p]->squadid=cursquad->id;
								break;
								}
							}
						}
					}
				}
			}
		if(c=='1')
			{
			//CHECK IF GOOD
			char good=1;
			char care=0;

			for(int p=0;p<6;p++)
				{
				if(cursquad->squad[p]!=NULL)
					{
					if(cursquad->squad[p]->align==1)
						{
						care=1;
						break;
						}
					else good=0;
					}
				}

			if(good||care)break;
			else
				{
				set_color(COLOR_RED,COLOR_BLACK,1);

				move(22,0);
				addstr("                                                                               ");
				move(23,0);
				addstr("You cannot form a Squad with only Conservatives!                               ");
				move(24,0);
				addstr("                                                                               ");

				refresh();
				getch();
				}
			}
		if(c=='9')
			{
			for(int p=0;p<6;p++)
				{
				if(cursquad->squad[p]!=NULL)
					{
					cursquad->squad[p]->squadid=-1;
					cursquad->squad[p]=NULL;
					}
				}
			}
		}while(1);

	//FINALIZE NEW SQUADS
	char hasmembers=0;

	for(p=0;p<6;p++)
		{
		if(cursquad->squad[p]!=NULL)
			{
			hasmembers=1;
			break;
			}
		}

	if(newsquad)
		{
		if(hasmembers)
			{
			move(22,0);
			addstr("                                                                               ");
			move(23,0);
			addstr("What shall we designate this Liberal squad?                                    ");
			move(24,0);
			addstr("                                                                               ");

			keypad(stdscr,FALSE);
			raw_output(FALSE);
			echo();
			curs_set(1);
			mvgetstr(24,0,cursquad->name);
			curs_set(0);
			noecho();
			raw_output(TRUE);
			keypad(stdscr,TRUE);

			squad.push_back(cursquad);
			}
		else delete cursquad;
		}

	//NUKE ALL EMPTY SQUADS
	for(int sq=squad.size()-1;sq>=0;sq--)
		{
		hasmembers=0;

		for(int p=0;p<6;p++)
			{
			if(squad[sq]->squad[p]!=NULL)
				{
				hasmembers=1;
				break;
				}
			}

		if(!hasmembers)
			{
			if(squadloc[sq]!=-1)
				{
				for(int l=0;l<squad[sq]->loot.size();l++)
					{
					location[squadloc[sq]]->loot.push_back(squad[sq]->loot[l]);
					}
				squad[sq]->loot.clear();
				}

			if(activesquad==squad[sq])activesquad=NULL;
			delete squad[sq];
			squad.erase(squad.begin() + sq);
			}
		}
}

void orderparty(void)
{
	if(activesquad==NULL)return;

	int partysize=0;
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)partysize++;
		}

	if(partysize<=1)return;

	int spot=0;

	do
		{
		printparty();

		move(8,20);
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		addstr("Choose a Liberal squad member for Place ");
		char num[20];
		itoa(spot+1,num,10);
		addstr(num);
		addstr(".");

		refresh();

		int c=getch();
		translategetch(c);

		if(c==10)return;

		if(c>=spot+'1'&&c<=partysize+'1'-1)
			{
			creaturest *swap=activesquad->squad[spot];
			activesquad->squad[spot]=activesquad->squad[c-'1'];
			activesquad->squad[c-'1']=swap;
			spot++;
			}
		}while(spot<partysize-1);
}

void initsite(locationst &loc)
{
 int x = 0;
 
	//PREP
	if(activesquad==NULL)return;

	for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;

	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			activesquad->squad[p]->forceinc=0;
			}
		}

	for(int l=0;l<groundloot.size();l++)
		{
		delete groundloot[l];
		}
	groundloot.clear();


	//MAKE MAP
	unsigned long oldseed=seed;
	seed=loc.mapseed;
	
	for(x=0;x<MAPX;x++)
		{
		for(int y=0;y<MAPY;y++)
			{
			for(int z=0;z<MAPZ;z++)
				{
				map[x][y][z].flag=SITEBLOCK_BLOCK;
				map[x][y][z].special=-1;
				map[x][y][z].siegeflag=0;
				}
			}
		}

	map[MAPX>>1][0][0].flag=SITEBLOCK_EXIT;
	map[(MAPX>>1)+1][0][0].flag=SITEBLOCK_EXIT;
	map[(MAPX>>1)+1][1][0].flag=SITEBLOCK_EXIT;
	map[(MAPX>>1)-1][0][0].flag=SITEBLOCK_EXIT;
	map[(MAPX>>1)-1][1][0].flag=SITEBLOCK_EXIT;

	map[MAPX>>1][1][0].flag=0;

	map[MAPX>>1][2][0].flag=SITEBLOCK_DOOR;

	if(loc.type==SITE_RESIDENTIAL_APARTMENT_UPSCALE||
		loc.type==SITE_RESIDENTIAL_APARTMENT||
		loc.type==SITE_RESIDENTIAL_TENEMENT)
		{
		map[MAPX>>1][1][0].special=SPECIAL_APARTMENT_SIGN;
		short height;
		int floors=LCSrandom(6)+1;
		int swap;
		for(int z=0;z<floors;z++)
			{
			for(int y=3;y<MAPY-3;y++)
				{
				map[MAPX>>1][y][z].flag=0;
				if(y%4==0)
					{
					height=y+LCSrandom(3)-1;
					map[(MAPX>>1)-1][height][z].flag=SITEBLOCK_DOOR;
					generateroom((MAPX>>1)-8,y-1,7,3,z);

					height=y+LCSrandom(3)-1;
					map[(MAPX>>1)+1][height][z].flag=SITEBLOCK_DOOR;
					generateroom((MAPX>>1)+2,y-1,7,3,z);
					if(y==4&&z==0)
						{
						map[(MAPX>>1)+2][height][z].flag=0;
						map[(MAPX>>1)+2][height][z].special=SPECIAL_APARTMENT_LANDLORD;
						}
					}
				}
			swap=(z%2)*2-1;
			if(z>0)
				{
				map[(MAPX>>1)+1*swap][MAPY-4][z].flag=0;
				map[(MAPX>>1)+1*swap][MAPY-4][z].special=SPECIAL_STAIRS_DOWN;
				}
			if(z<floors-1)
				{
				map[(MAPX>>1)-1*swap][MAPY-4][z].flag=0;
				map[(MAPX>>1)-1*swap][MAPY-4][z].special=SPECIAL_STAIRS_UP;
				}
			}
		}
	else
		{
		switch(loc.type)
			{
			case SITE_BUSINESS_LATTESTAND:
				{
				for(int x=(MAPX>>1)-4;x<=(MAPX>>1)+4;x++)
					{
					for(int y=0;y<7;y++)
						{
						if(x==(MAPX>>1)-4||x==(MAPX>>1)+4||
							y==0||y==6)map[x][y][0].flag=SITEBLOCK_EXIT;
						else map[x][y][0].flag=0;
						map[x][y][0].special=-1;
						map[x][y][0].siegeflag=0;
						}
					}
				break;
				}
			case SITE_BUSINESS_JUICEBAR:
			case SITE_BUSINESS_CIGARBAR:
			case SITE_BUSINESS_VEGANCOOP:
			case SITE_BUSINESS_INTERNETCAFE:
				{
				for(int x=(MAPX>>1)-4;x<=(MAPX>>1)+4;x++)
					{
					for(int y=3;y<10;y++)
						{
						map[x][y][0].flag=0;
						map[x][y][0].special=-1;
						map[x][y][0].siegeflag=0;
						}
					}
				break;
				}
			case SITE_BUSINESS_CRACKHOUSE:
				{
				int dx=LCSrandom(5)*2+19;
				int dy=LCSrandom(3)*2+7;
				int rx=(MAPX>>1)-(dx>>1);
				int ry=3;
				generateroom(rx,ry,dx,dy,0);
				break;
				}
			default:
				{
				int dx=LCSrandom(5)*2+35;
				int dy=LCSrandom(3)*2+15;
				int rx=(MAPX>>1)-(dx>>1);
				int ry=3;
				generateroom(rx,ry,dx,dy,0);
				break;
				}
			}
		}

	//CLEAR AWAY BLOCKED DOORWAYS
	char block;
	char opennum;
	for(x=0;x<MAPX;x++)
		{
		for(int y=0;y<MAPY;y++)
			{
			for(int z=0;z<MAPZ;z++)
				{
				if(map[x][y][z].flag & SITEBLOCK_DOOR)
					{
					block=BIT1 | BIT2 | BIT3 | BIT4;
					opennum=0;

					if(x>0)if(!(map[x-1][y][z].flag&SITEBLOCK_BLOCK)){block&=~BIT2;opennum++;}
					if(x<MAPX-1)if(!(map[x+1][y][z].flag&SITEBLOCK_BLOCK)){block&=~BIT3;opennum++;}
					if(y>0)if(!(map[x][y-1][z].flag&SITEBLOCK_BLOCK)){block&=~BIT1;opennum++;}
					if(y<MAPY-1)if(!(map[x][y+1][z].flag&SITEBLOCK_BLOCK)){block&=~BIT4;opennum++;}

					if(opennum>=2)continue;

					//BLAST EVERYTHING AROUND TOTALLY BLOCKED DOOR
					if(block==(BIT1 | BIT2 | BIT3 | BIT4))
						{
						if(x>0)map[x-1][y][z].flag&=~SITEBLOCK_BLOCK;
						if(x<MAPX-1)map[x+1][y][z].flag&=~SITEBLOCK_BLOCK;
						if(y>0)map[x][y-1][z].flag&=~SITEBLOCK_BLOCK;
						if(y<MAPY-1)map[x][y+1][z].flag&=~SITEBLOCK_BLOCK;
						}
					//DEAD-END OR OPEN A THREE-BLOCKED DOOR
					else if(!(block&BIT1))
						{
						if(y<MAPY-1)map[x][y+1][z].flag&=~SITEBLOCK_BLOCK;
						else map[x][y+1][z].flag=SITEBLOCK_BLOCK;
						}
					else if(!(block&BIT4))
						{
						if(y>0)map[x][y-1][z].flag&=~SITEBLOCK_BLOCK;
						else map[x][y-1][z].flag=SITEBLOCK_BLOCK;
						}
					else if(!(block&BIT2))
						{
						if(x<MAPX-1)map[x+1][y][z].flag&=~SITEBLOCK_BLOCK;
						else map[x+1][y][z].flag=SITEBLOCK_BLOCK;
						}
					else if(!(block&BIT3))
						{
						if(x>0)map[x-1][y][z].flag&=~SITEBLOCK_BLOCK;
						else map[x-1][y][z].flag=SITEBLOCK_BLOCK;
						}
					}
				}
			}
		}

	//DELETE NON-DOORS
	for(x=0;x<MAPX;x++)
		{
		for(int y=0;y<MAPY;y++)
			{
			for(int z=0;z<MAPZ;z++)
				{
				if(map[x][y][z].flag & SITEBLOCK_DOOR)
					{
					block=BIT1 | BIT2 | BIT3 | BIT4;

					if(x>0)if(!(map[x-1][y][z].flag&SITEBLOCK_BLOCK))block&=~BIT2;
					if(x<MAPX-1)if(!(map[x+1][y][z].flag&SITEBLOCK_BLOCK))block&=~BIT3;
					if(y>0)if(!(map[x][y-1][z].flag&SITEBLOCK_BLOCK))block&=~BIT1;
					if(y<MAPY-1)if(!(map[x][y+1][z].flag&SITEBLOCK_BLOCK))block&=~BIT4;

					if((block & BIT1) && (block & BIT4))continue;
					if((block & BIT2) && (block & BIT3))continue;

					map[x][y][z].flag&=~SITEBLOCK_DOOR;
					map[x][y][z].flag&=~SITEBLOCK_LOCKED;
					}
				}
			}
		}

	//ADD RESTRICTIONS
	switch(loc.type)
		{
		case SITE_LABORATORY_COSMETICS:
		case SITE_LABORATORY_GENETIC:
		case SITE_GOVERNMENT_POLICESTATION:
		case SITE_GOVERNMENT_COURTHOUSE:
		case SITE_GOVERNMENT_PRISON:
		case SITE_GOVERNMENT_INTELLIGENCEHQ:
		case SITE_MEDIA_AMRADIO:
		case SITE_MEDIA_CABLENEWS:
			for(x=2;x<MAPX-2;x++)
				{
				for(int y=2;y<MAPY-2;y++)
					{
					for(int z=0;z<MAPZ;z++)
						{
						map[x][y][z].flag|=SITEBLOCK_RESTRICTED;
						}
					}
				}
			break;
		}

	//NOW CLEAR FIRST FLOOR RESTRICTIONS NEAR TO DOOR
	char acted;
	map[MAPX>>1][2][0].flag&=~SITEBLOCK_RESTRICTED;
	do
		{
		acted=0;

		for(int x=2;x<MAPX-2;x++)
			{
			for(int y=2;y<MAPY-2;y++)
				{
				if(!(map[x][y][0].flag & SITEBLOCK_DOOR)&&
					!(map[x][y][0].flag & SITEBLOCK_BLOCK)&&
					(map[x][y][0].flag & SITEBLOCK_RESTRICTED))
					{
					if(!(map[x-1][y][0].flag & SITEBLOCK_RESTRICTED)||
						!(map[x+1][y][0].flag & SITEBLOCK_RESTRICTED)||
						!(map[x][y-1][0].flag & SITEBLOCK_RESTRICTED)||
						!(map[x][y+1][0].flag & SITEBLOCK_RESTRICTED))
						{
						map[x][y][0].flag&=~SITEBLOCK_RESTRICTED;
						acted=1;
						continue;
						}
					}
				}
			}
		}while(acted);

	//ADD ACCESSORIES
	seed=oldseed;
	for(x=2;x<MAPX-2;x++)
		{
		for(int y=2;y<MAPY-2;y++)
			{
			for(int z=0;z<MAPZ;z++)
				{
				if(map[x][y][z].flag==0&&!LCSrandom(10))
					{
					switch(loc.type)
						{
						case SITE_RESIDENTIAL_SHELTER:
						case SITE_BUSINESS_CRACKHOUSE:
						case SITE_BUSINESS_JUICEBAR:
						case SITE_BUSINESS_CIGARBAR:
						case SITE_BUSINESS_LATTESTAND:
						case SITE_BUSINESS_VEGANCOOP:
						case SITE_BUSINESS_INTERNETCAFE:
						case SITE_INDUSTRY_WAREHOUSE:
							break;
						default:
							map[x][y][z].flag=SITEBLOCK_LOOT;
							break;
						}
					}

				if(map[x][y][z].flag==0&&loc.type==SITE_LABORATORY_COSMETICS&&!LCSrandom(10))
					{
					map[x][y][z].special=SPECIAL_LAB_COSMETICS_CAGEDANIMALS;
					}
				if(map[x][y][z].flag==0&&loc.type==SITE_LABORATORY_GENETIC&&!LCSrandom(10))
					{
					map[x][y][z].special=SPECIAL_LAB_GENETIC_CAGEDANIMALS;
					}
				if(map[x][y][z].flag==0&&loc.type==SITE_INDUSTRY_SWEATSHOP&&!LCSrandom(10))
					{
					map[x][y][z].special=SPECIAL_SWEATSHOP_EQUIPMENT;
					}
				if(map[x][y][z].flag==0&&loc.type==SITE_INDUSTRY_POLLUTER&&!LCSrandom(10))
					{
					map[x][y][z].special=SPECIAL_POLLUTER_EQUIPMENT;
					}
				if(map[x][y][z].flag==0&&
					(loc.type==SITE_BUSINESS_JUICEBAR||
					loc.type==SITE_BUSINESS_CIGARBAR||
					loc.type==SITE_BUSINESS_LATTESTAND||
					loc.type==SITE_BUSINESS_INTERNETCAFE)&&
					!LCSrandom(10))
					{
					map[x][y][z].special=SPECIAL_RESTAURANT_TABLE;
					}
				if(map[x][y][z].flag==0&&loc.type==SITE_BUSINESS_INTERNETCAFE&&
					!LCSrandom(10))
					{
					map[x][y][z].special=SPECIAL_CAFE_COMPUTER;
					}
				}
			}
		}

	int freex,freey,freez=0;

	//ADD FIRST SPECIAL
	int count=100000;

	do
		{
		freex=LCSrandom(MAPX-4)+2;
		freey=LCSrandom(MAPY-4)+2;
		if(freex>=(MAPX>>1)-2&&
			freex<=(MAPX>>1)+2)freey=LCSrandom(MAPY-6)+4;
		count--;
		}while((map[freex][freey][freez].flag!=0 ||
			map[freex][freey][freez].special!=-1)&&count>0);

	switch(loc.type)
		{
		case SITE_INDUSTRY_NUCLEAR:
			map[freex][freey][freez].special=SPECIAL_NUCLEAR_ONOFF;
			break;
		case SITE_GOVERNMENT_POLICESTATION:
			map[freex][freey][freez].special=SPECIAL_POLICESTATION_LOCKUP;
			break;
		case SITE_GOVERNMENT_COURTHOUSE:
			map[freex][freey][freez].special=SPECIAL_COURTHOUSE_LOCKUP;
			break;
		case SITE_GOVERNMENT_PRISON:
			map[freex][freey][freez].special=SPECIAL_PRISON_CONTROL;
			break;
		case SITE_GOVERNMENT_INTELLIGENCEHQ:
			map[freex][freey][freez].special=SPECIAL_INTEL_SUPERCOMPUTER;
			break;
		case SITE_CORPORATE_HEADQUARTERS:
			map[freex][freey][freez].special=SPECIAL_CORPORATE_FILES;
			break;
		case SITE_CORPORATE_HOUSE:
			map[freex][freey][freez].special=SPECIAL_HOUSE_PHOTOS;
			break;
		case SITE_MEDIA_AMRADIO:
			map[freex][freey][freez].special=SPECIAL_RADIO_BROADCASTSTUDIO;
			break;
		case SITE_MEDIA_CABLENEWS:
			map[freex][freey][freez].special=SPECIAL_NEWS_BROADCASTSTUDIO;
			break;
		}

	count=100000;

	//ADD SECOND SPECIAL
	do
		{
		freex=LCSrandom(MAPX-4)+2;
		freey=LCSrandom(MAPY-4)+2;
		if(freex>=(MAPX>>1)-2&&
			freex<=(MAPX>>1)+2)freey=LCSrandom(MAPY-6)+4;
		count--;
		}while((map[freex][freey][freez].flag!=0 ||
			map[freex][freey][freez].special!=-1)&&count>0);

	switch(loc.type)
		{
		case SITE_GOVERNMENT_COURTHOUSE:
			map[freex][freey][freez].special=SPECIAL_COURTHOUSE_JURYROOM;
			break;
		}
}

void generateroom(int rx,int ry,int dx,int dy,int z)
{
	for(int x=rx;x<rx+dx;x++)
		{
		for(int y=ry;y<ry+dy;y++)
			{
			map[x][y][z].flag=0;
			}
		}

	if((dx<=3||dy<=3)&&!LCSrandom(2))return;

	if(dx<=2&&dy<=2)return;

	//LAY DOWN WALL AND ITERATE
	if((!LCSrandom(2)||dy<=2)&&dx>2)
		{
		int wx=rx+LCSrandom(dx-2)+1;

		for(int wy=0;wy<dy;wy++)map[wx][ry+wy][z].flag=SITEBLOCK_BLOCK;
		int rny=LCSrandom(dy);
		map[wx][ry+rny][z].flag=SITEBLOCK_DOOR;
		if(!LCSrandom(3))map[wx][ry+rny][z].flag|=SITEBLOCK_LOCKED;

		generateroom(rx,ry,wx-rx,dy,z);

		generateroom(wx+1,ry,rx+dx-wx-1,dy,z);
		}
	else
		{
		int wy=ry+LCSrandom(dy-2)+1;

		for(int wx=0;wx<dx;wx++)map[rx+wx][wy][z].flag=SITEBLOCK_BLOCK;
		int rnx=LCSrandom(dx);
		map[rx+rnx][wy][z].flag=SITEBLOCK_DOOR;
		if(!LCSrandom(3))map[rx+rnx][wy][z].flag|=SITEBLOCK_LOCKED;

		generateroom(rx,ry,dx,wy-ry,z);

		generateroom(rx,wy+1,dx,ry+dy-wy-1,z);
		}
}

void mode_site(long loc)
{
	sitetype=location[loc]->type;
	cursite=loc;
	sitealarm=0;
	sitealarmtimer=-1;
	postalarmtimer=0;
	sitealienate=0;
	sitecrime=0;
	initsite(*location[loc]);
	sitestory=newsstory[newsstory.size()-1];
	mode=GAMEMODE_SITE;

	if(!location[loc]->siege.siege)
		{
		locx=MAPX>>1;
		locy=1;
		locz=0;
		}
	else
		{
		sitealarm=1;

		location[loc]->siege.attacktime=0;
		location[loc]->siege.kills=0;

		//PLACE YOU
		int maxy=0;

		for(int x=0;x<MAPX;x++)
			{
			for(int y=0;y<MAPY;y++)
				{
				for(int z=0;z<MAPZ;z++)
					{
					if(!location[loc]->siege.lights_off)map[x][y][z].flag|=SITEBLOCK_KNOWN;
					map[x][y][z].flag&=~SITEBLOCK_LOCKED;
					map[x][y][z].flag&=~SITEBLOCK_LOOT;
					}
				if(!(map[x][y][0].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)))maxy=y;
				}
			}

		do
			{
			locx=LCSrandom(MAPX);
			locy=maxy-LCSrandom(3);
			if(locy<3)locy=3;
			locz=0;
			}while(map[locx][locy][locz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR));

		//PLACE LOOT
		int lootnum=location[loc]->loot.size();
		if(lootnum>10)lootnum=10;

		int lx,ly,lz;
		for(int l=0;l<lootnum;l++)
			{
			do
				{
				lx=LCSrandom(MAPX);
				ly=LCSrandom(MAPY);
				lz=0;
				}while(map[lx][ly][lz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT));
			map[lx][ly][lz].flag|=SITEBLOCK_LOOT;
			}

		//PLACE TRAPS
		if(location[loc]->compound_walls & COMPOUND_TRAPS)
			{
			int trapnum=30;
			for(int t=0;t<trapnum;t++)
				{
				do
					{
					lx=LCSrandom(MAPX);
					ly=LCSrandom(MAPY);
					lz=0;
					}while(map[lx][ly][lz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT|SITEBLOCK_LOOT));
				map[lx][ly][lz].siegeflag|=SIEGEFLAG_TRAP;
				}
			}

		//PLACE UNITS
		int unitnum=10;
		int count=50000;
		for(int t=0;t<unitnum;t++)
			{
			do
				{
				lx=LCSrandom(11)+(MAPX/2)-5;
				ly=LCSrandom(8);
				lz=0;
				count--;
				if(count==0)break;
				}while((map[lx][ly][lz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT))||
					(map[lx][ly][lz].siegeflag & (SIEGEFLAG_UNIT|SIEGEFLAG_HEAVYUNIT|SIEGEFLAG_TRAP)));
			map[lx][ly][lz].siegeflag|=SIEGEFLAG_UNIT;
			}

		if(!(location[loc]->compound_walls & COMPOUND_TANKTRAPS)&&
			location[loc]->siege.siegetype==SIEGE_POLICE&&
			location[loc]->siege.escalationstate>=2)
			{
			count=50000;
			int hunitnum=3;
			for(int t=0;t<hunitnum;t++)
				{
				do
					{
					lx=LCSrandom(11)+(MAPX/2)-5;
					ly=LCSrandom(8);
					lz=0;
					count--;
					if(count==0)break;
					}while((map[lx][ly][lz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT))||
						(map[lx][ly][lz].siegeflag & (SIEGEFLAG_UNIT|SIEGEFLAG_HEAVYUNIT|SIEGEFLAG_TRAP)));
				map[lx][ly][lz].siegeflag|=SIEGEFLAG_HEAVYUNIT;
				}
			}
		}

	mode_site();
}

void mode_site(void)
{
	int p, x, u;
	if(activesquad==NULL)return;

	reloadparty();

	showcarprefs=-1;

	char bail_on_base=1;
	if(cursite==activesquad->squad[0]->base)bail_on_base=0;

	knowmap(locx,locy,locz);

	char hostcheck=0;

	do
		{
		int partysize=0;
		int partyalive=0;
		for(p=0;p<6;p++)
			{
			if(activesquad->squad[p]!=NULL)partysize++;
			else continue;

			if(activesquad->squad[p]->alive==1)partyalive++;
			}
		int encsize=0;
		int freeable=0;
		int enemy=0;
		int majorenemy=0;
		int talkers=0;
		for(int e=0;e<ENCMAX;e++)
			{
			if(encounter[e].exists)
				{
				encsize++;
				if(encounter[e].align==-1)enemy++;
				if(encounter[e].type==CREATURE_WORKER_SERVANT||
					encounter[e].type==CREATURE_WORKER_SWEATSHOP)freeable++;
				else if(!encounter[e].cantbluff)talkers++;
				if(encounter[e].type==CREATURE_CORPORATE_CEO||
					encounter[e].type==CREATURE_RADIOPERSONALITY||
					encounter[e].type==CREATURE_NEWSANCHOR||
					encounter[e].type==CREATURE_SCIENTIST_EMINENT||
					encounter[e].type==CREATURE_JUDGE_CONSERVATIVE)majorenemy++;
				}
			}
		int libnum=0;
		for(p=0;p<pool.size();p++)
			{
			if(pool[p]->align==1&&
				pool[p]->alive&&
				pool[p]->location==cursite&&
				!(pool[p]->flag & CREATUREFLAG_SLEEPER))libnum++;
			}

		erase();

		if(location[cursite]->siege.siege)
			{
			set_color(COLOR_RED,COLOR_BLACK,1);
			move(0,0);
			addlocationname(location[cursite]);
			addstr(", Level ");
			char num[20];
			itoa(locz+1,num,10);
			addstr(num);
			addstr(": Escape or Engage");
			}
		else
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(0,0);
			addlocationname(location[cursite]);
			addstr(", Level ");
			char num[20];
			itoa(locz+1,num,10);
			addstr(num);

			if(sitealienate==1)addstr(": ALIENATED MASSES");
			else if(sitealienate==2)addstr(": ALIENATED EVERYONE");
			else if(sitealarm)addstr(": CONSERVATIVES ALARMED");
			else if(sitealarmtimer==0)addstr(": CONSERVATIVES SUSPICIOUS");
			}

		//PRINT PARTY
		if(partyalive==0)party_status=-1;
		printparty();

		//PRINT SITE INSTRUCTIONS
		if(partyalive>0)
			{
			if(!enemy||!sitealarm)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(9,1);
			addstr("W,A,D,X - Move");
			if(partysize>1)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,1);
			addstr("O - Change the squad's Liberal order");
			if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(12,1);
			addstr("# - Check the status of a squad Liberal");
			if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(13,1);
			addstr("0 - Show the squad's Liberal status");

			if(groundloot.size()>0||(map[locx][locy][locz].flag&SITEBLOCK_LOOT))
				{
				set_color(COLOR_WHITE,COLOR_BLACK,0);
				}
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(9,18);
			addstr("G - Get Loot");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(9,35);
			addstr("M - Map");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(10,35);
			addstr("S - Wait");

			if(!enemy||!sitealarm)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,45);
			addstr("L - Reload");

			if(enemy)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(13,45);
			addstr("K - Kidnap");

			if(talkers)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(14,18);
			addstr("T - Talk");

			if(map[locx][locy][locz].special!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,45);
			addstr("U - Use");

			if(enemy&&sitealarm)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(12,45);
			addstr("V - Evade");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(9,45);
			addstr("E - Equip");

			if(enemy)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(14,1);
			addstr("F - Fight!");

			if(!location[cursite]->siege.siege)
				{
				if(freeable&&(!enemy||!sitealarm))set_color(COLOR_WHITE,COLOR_BLACK,0);
				else set_color(COLOR_BLACK,COLOR_BLACK,1);
				move(14,35);
				addstr("R - Release oppressed");
				}
			else
				{
				if(libnum>6)set_color(COLOR_WHITE,COLOR_BLACK,0);
				else set_color(COLOR_BLACK,COLOR_BLACK,1);
				move(14,35);
				addstr("R - Reorganize");
				}
			}
		else
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(9,1);
			addstr("C - Reflect on your Conservative ineptitude");
			}

		//PRINT SITE MAP
		printsitemap(locx,locy,locz);

		//CHECK IF YOU HAVE A SQUIRMING AMATEUR HOSTAGE
			//hostcheck SHOULD ONLY BE 1 WHEN A NEWENC IS CREATED
		if(hostcheck)
			{
			char havehostage=0;
			for(int p=0;p<6;p++)
				{
				if(activesquad->squad[p]!=NULL)
					{
					if(activesquad->squad[p]->prisoner!=NULL&&
						activesquad->squad[p]->weapon.type==WEAPON_NONE)
						{
						if(activesquad->squad[p]->prisoner->squadid==-1)
							{
							activesquad->squad[p]->prisoner->flag|=CREATUREFLAG_KIDNAPPED;
							if(activesquad->squad[p]->type==CREATURE_RADIOPERSONALITY)offended_amradio=1;
							if(activesquad->squad[p]->type==CREATURE_NEWSANCHOR)offended_cablenews=1;
							havehostage=1;
							}
						}
					}
				}

			if(havehostage)
				{
				alienationcheck(1);
				sitecrime+=5;
				criminalizeparty(LAWFLAG_KIDNAPPER);
				}
			hostcheck=0;

			clearmessagearea();
			}

		refresh();

		int c=getch();
		translategetch(c);

		if(partyalive==0&&c=='c')
			{
			//DESTROY ALL CARS BROUGHT ALONG WITH PARTY
			if(!location[cursite]->siege.siege)
				{
				for(int p=0;p<6;p++)
					{
					if(activesquad->squad[p]==NULL)continue;
					if(activesquad->squad[p]->carid!=-1)
						{
						long v=id_getcar(activesquad->squad[p]->carid);
						if(v!=-1)
							{
							removecarprefs_pool(vehicle[v]->id);
							delete vehicle[v];
							vehicle.erase(vehicle.begin() + v);
							}
						}
					}
				}

			for(int p=0;p<6;p++)
				{
				if(activesquad->squad[p]==NULL)continue;

				for(int pl=pool.size()-1;pl>=0;pl--)
					{
					if(pool[pl]==activesquad->squad[p])
						{
						delete pool[pl];
						pool.erase(pool.begin() + pl);
						break;
						}
					}

				activesquad->squad[p]=NULL;
				}

			//END OF GAME CHECK
			if(!endcheck())
				{
				if(location[cursite]->siege.siege)
					{
					if(location[cursite]->siege.underattack)sitestory->type=NEWSSTORY_SQUAD_KILLED_SIEGEATTACK;
					else sitestory->type=NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE;
					}
				else sitestory->type=NEWSSTORY_SQUAD_KILLED_SITE;

				if(!location[cursite]->siege.siege)
					{
					if(!sitealienate)
						{
						long addjuice=sitecrime;if(addjuice>20)addjuice=20;
						juiceparty(addjuice);
						}
					resolvesite();
					}

				mode=GAMEMODE_BASE;

				showcarprefs=0;
				return;
				}
			}

		if(partyalive>0)
			{
			int olocx=locx;
			int olocy=locy;
			int olocz=locz;

			char override=0;

			if(c=='v'&&enemy&&sitealarm)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(16,1);
				addstr("Which way?  (W,A,D, and X to move, ENTER to abort)      ");
				move(17,1);
				addstr("                                                        ");

				refresh();

				do
					{
					int c2=getch();
					translategetch(c2);

					if(c2=='w'||c2=='a'||c2=='d'||c2=='x')
						{
						c=c2;
						override=1;
						break;
						}

					if(c2==10)break;
					}while(1);
				}

			if(c=='w'&&locy>0&&(!enemy||!sitealarm||override))
				{
				if(!(map[locx][locy-1][locz].flag & SITEBLOCK_BLOCK))locy--;
				}
			if(c=='a'&&locx>0&&(!enemy||!sitealarm||override))
				{
				if(!(map[locx-1][locy][locz].flag & SITEBLOCK_BLOCK))locx--;
				}
			if(c=='d'&&locx<MAPX-1&&(!enemy||!sitealarm||override))
				{
				if(!(map[locx+1][locy][locz].flag & SITEBLOCK_BLOCK))locx++;
				}
			if(c=='x'&&locy<MAPY-1&&(!enemy||!sitealarm||override))
				{
				if(!(map[locx][locy+1][locz].flag & SITEBLOCK_BLOCK))locy++;
				}

			if(c=='k'&&enemy)
				{
				kidnapattempt();
				}

			if(c=='u'&&map[locx][locy][locz].special!=-1)
				{
				switch(map[locx][locy][locz].special)
					{
					case SPECIAL_LAB_COSMETICS_CAGEDANIMALS:special_lab_cosmetics_cagedanimals();break;
					case SPECIAL_NUCLEAR_ONOFF:special_nuclear_onoff();break;
					case SPECIAL_LAB_GENETIC_CAGEDANIMALS:special_lab_genetic_cagedanimals();break;
					case SPECIAL_POLICESTATION_LOCKUP:special_policestation_lockup();break;
					case SPECIAL_COURTHOUSE_LOCKUP:special_courthouse_lockup();break;
					case SPECIAL_COURTHOUSE_JURYROOM:special_courthouse_jury();break;
					case SPECIAL_PRISON_CONTROL:special_prison_control();break;
					case SPECIAL_INTEL_SUPERCOMPUTER:special_intel_supercomputer();break;
					case SPECIAL_SWEATSHOP_EQUIPMENT:special_sweatshop_equipment();break;
					case SPECIAL_POLLUTER_EQUIPMENT:special_polluter_equipment();break;
					case SPECIAL_HOUSE_PHOTOS:special_house_photos();break;
					case SPECIAL_CORPORATE_FILES:special_corporate_files();break;
					case SPECIAL_RADIO_BROADCASTSTUDIO:special_radio_broadcaststudio();break;
					case SPECIAL_NEWS_BROADCASTSTUDIO:special_news_broadcaststudio();break;
					case SPECIAL_APARTMENT_SIGN:special_readsign(SPECIAL_APARTMENT_SIGN);break;
					case SPECIAL_STAIRS_UP:locz++;break;
					case SPECIAL_STAIRS_DOWN:locz--;break;
					}
				}

			if(c=='t'&&talkers)
				{
				int forcesp=-1;
				int forcetk=-1;

				for(int p=0;p<6;p++)
					{
					if(activesquad->squad[p]!=NULL)
						{
						if(activesquad->squad[p]->alive)
							{
							if(forcesp==-1)forcesp=p;
							else forcesp=-2;
							}
						}
					}

				for(int e=0;e<ENCMAX;e++)
					{
					if(encounter[e].exists&&encounter[e].alive&&
						!(encounter[e].type==CREATURE_WORKER_SERVANT||
							encounter[e].type==CREATURE_WORKER_SWEATSHOP))
						{
						if(!encounter[e].cantbluff)
							{
							if(forcetk==-1)forcetk=e;
							else forcetk=-2;
							}
						}
					}

				if(forcetk!=-1&&forcesp!=-1)
					{
					int tk=-1,sp=-1;
					if(forcesp==-2)
						{
						clearcommandarea();
						clearmessagearea();
						clearmaparea();

						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(9,1);
						addstr("Which Liberal will speak?");
						move(9,50);
						addstr("Issues");
						move(9,60);
						addstr("Dating");
						move(9,70);
						addstr("Bluff");

						char num[20];
						int y=11;
						for(int p=0;p<6;p++)
							{
							if(activesquad->squad[p]!=NULL)
								{
								if(activesquad->squad[p]->alive)
									{
									move(y,1);
									addch(p+'1');
									addstr(" - ");
									addstr(activesquad->squad[p]->name);
									move(y,50);
									itoa(activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
										activesquad->squad[p]->attval(ATTRIBUTE_HEART)+
										activesquad->squad[p]->skill[SKILL_PERSUASION]*2,num,10);
									addstr(num);
									move(y,60);
									itoa(activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)*2+
										activesquad->squad[p]->skill[SKILL_PERSUASION]*2,num,10);
									addstr(num);
									move(y,70);
									itoa(activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
										activesquad->squad[p]->attval(ATTRIBUTE_WISDOM)+
										activesquad->squad[p]->skill[SKILL_PERSUASION]*2,num,10);
									addstr(num);
									y++;
									}
								}
							}

						do
							{
							refresh();
							int c=getch();
							translategetch(c);
							if(c>='1'&&c<='6')
								{
								sp=c-'1';
								if(activesquad->squad[sp]!=NULL)
									{
									if(activesquad->squad[sp]->alive)break;
									}
								}
							if(c==10||c==27||c==32){sp=-1;break;}
							}while(1);
						}
					else sp=forcesp;

					if(sp!=-1)
						{
						if(forcetk==-2)
							{
							do
								{
								clearcommandarea();
								clearmessagearea();
								clearmaparea();

								set_color(COLOR_WHITE,COLOR_BLACK,1);
								move(9,1);
								addstr("To whom?");

								int x=1,y=11;
								for(int t=0;t<ENCMAX;t++)
									{
									if(encounter[t].exists)
										{
										move(y,x);
										addch(t+'A');
										addstr(" - ");
										addstr(encounter[t].name);

										y++;
										if(y==17)
											{
											y=11;
											x+=30;
											}
										}
									}

								refresh();
								int c=getch();
								translategetch(c);
								if(c>='a'&&c<='z')
									{
									tk=c-'a';
									if(tk<ENCMAX)
										{
										if(encounter[tk].exists)
											{
											if(encounter[tk].alive&&
												!(encounter[tk].type==CREATURE_WORKER_SERVANT||
												encounter[tk].type==CREATURE_WORKER_SWEATSHOP))
												{
												if(encounter[tk].cantbluff==2)
													{
													clearcommandarea();
													clearmessagearea();
													clearmaparea();

													set_color(COLOR_WHITE,COLOR_BLACK,1);
													move(9,1);
													addstr("You can't talk in the middle of a fight.");

													refresh();
													getch();
													}
												else if(encounter[tk].cantbluff==1)
													{
													clearcommandarea();
													clearmessagearea();
													clearmaparea();

													set_color(COLOR_WHITE,COLOR_BLACK,1);
													move(9,1);
													addstr(encounter[tk].name);
													addstr(" won't talk to you.");

													refresh();
													getch();
													}
												else if(encounter[tk].align!=-1&&sitealarm&&enemy)
													{
													clearcommandarea();
													clearmessagearea();
													clearmaparea();

													set_color(COLOR_WHITE,COLOR_BLACK,1);
													move(9,1);
													addstr("You have to deal with the conservatives first.");

													refresh();
													getch();
													}
												else break;
												}
											}
										}
									}
								if(c==10||c==27||c==32){tk=-1;break;}
								}while(1);
							}
						else tk=forcetk;

						if(tk!=-1)
							{
							if(talk(*activesquad->squad[sp],tk))
								{
								if(enemy&&sitealarm)enemyattack();
								else if(enemy)disguisecheck();
								creatureadvance();
								}
							}
						}
					}
				}

			if(c=='l'&&(!enemy||!sitealarm))
				{
				reloadparty();
				printparty();
				refresh();
				creatureadvance();
				}

			if(c=='o'&&partysize>1)orderparty();

			if(c=='0')party_status=-1;

			if(c=='m')
				{
				for(int x=0;x<MAPX;x++)
					{
					for(int y=0;y<MAPY;y++)
						{
						if(map[x][y][locz].flag & SITEBLOCK_KNOWN)
							{
							move(y+1,x+5);
							if(x==locx&&y==locy)
								{
								set_color(COLOR_GREEN,COLOR_BLACK,1);
								addch(1);
								}
							else
								{
								set_color(COLOR_WHITE,COLOR_BLACK,0);
								if(map[x][y][locz].flag & SITEBLOCK_BLOCK)addch(CH_FULL_BLOCK);
								else if(map[x][y][locz].flag & SITEBLOCK_DOOR)addch(CH_LIGHT_SHADE);
								else if((map[x][y][locz].siegeflag & SIEGEFLAG_HEAVYUNIT)&&
									(location[cursite]->compound_walls & COMPOUND_CAMERAS)&&!location[cursite]->siege.cameras_off)
									{
									set_color(COLOR_RED,COLOR_BLACK,1);
									addch(CH_YEN_SIGN);
									}
								else if((map[x][y][locz].siegeflag & SIEGEFLAG_UNIT)&&
									(location[cursite]->compound_walls & COMPOUND_CAMERAS)&&!location[cursite]->siege.cameras_off)
									{
									set_color(COLOR_RED,COLOR_BLACK,1);
									addch(2);
									}
								else if((map[x][y][locz].siegeflag & SIEGEFLAG_UNIT_DAMAGED)&&
									(location[cursite]->compound_walls & COMPOUND_CAMERAS)&&!location[cursite]->siege.cameras_off)
									{
									set_color(COLOR_RED,COLOR_BLACK,0);
									addch(2);
									}
								else if(map[x][y][locz].special==SPECIAL_STAIRS_UP)
									{
									set_color(COLOR_YELLOW,COLOR_BLACK,1);
									addch(24);
									}
								else if(map[x][y][locz].special==SPECIAL_STAIRS_DOWN)
									{
									set_color(COLOR_YELLOW,COLOR_BLACK,1);
									addch(25);
									}
								else if(map[x][y][locz].special!=-1)
									{
									set_color(COLOR_YELLOW,COLOR_BLACK,1);
									addch('!');
									}
								else if(map[x][y][locz].siegeflag & SIEGEFLAG_TRAP)
									{
									set_color(COLOR_YELLOW,COLOR_BLACK,1);
									addch('!');
									}
								else if(map[x][y][locz].flag & SITEBLOCK_LOOT)
									{
									set_color(COLOR_MAGENTA,COLOR_BLACK,1);
									addch('$');
									}
								else addch(' ');
								}
							}
						else
							{
							set_color(COLOR_BLACK,COLOR_BLACK,1);
							move(y+1,x+5);
							addch(CH_FULL_BLOCK);
							}
						}
					}

				refresh();
				getch();
				}

			if(enemy&&c=='f')
				{
				youattack();
				enemyattack();
				creatureadvance();
				}

			if(c=='r'&&location[cursite]->siege.siege&&libnum>6)
				{
				assemblesquad(activesquad);
				autopromote(cursite);
				}

			if(freeable&&(!enemy||!sitealarm)&&c=='r'&&!location[cursite]->siege.siege)
				{
				short followers=0;
				short actgot=0;

				if(enemy)sitealarm=1;
				char freed;
				char flipstart;
				do
					{
					flipstart=0;
					freed=0;
					for(int e=0;e<ENCMAX;e++)
						{
						if(!encounter[e].exists)break;
						if((encounter[e].type==CREATURE_WORKER_SERVANT||
							encounter[e].type==CREATURE_WORKER_SWEATSHOP)&&!flipstart)
							{
							followers++;
							flipstart=1;
							freed=1;

							if(partysize<6)
								{
								creaturest *newcr=new creaturest;
									*newcr=encounter[e];
								namecreature(*newcr);

								newcr->location=activesquad->squad[0]->location;
								newcr->base=activesquad->squad[0]->base;

								pool.push_back(newcr);
								stat_recruits++;

								for(int p=0;p<6;p++)
									{
									if(activesquad->squad[p]==NULL)
										{
										activesquad->squad[p]=newcr;
										newcr->squadid=activesquad->id;
										break;
										}
									}

								actgot++;
								partysize++;
								}
							}
						if(flipstart)
							{
							if(e<ENCMAX-1)encounter[e]=encounter[e+1];
							}
						}
					if(flipstart)encounter[ENCMAX-1].exists=0;

					if(freed)
						{
						int time=20+LCSrandom(10);
						if(time<1)time=1;
						if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;
						}
					}while(freed);

				if(followers>0)
					{
					clearmessagearea();

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(16,1);
					addstr("You free ");
					if(followers>1)addstr("some Oppressed workers");
					else addstr("an Oppressed worker");
					addstr(" from the Conservatives.");

					if(actgot<followers)
						{
						refresh();
						getch();

						clearmessagearea();

						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(16,1);
						if(actgot==0&&followers>1)addstr("They all leave");
						else if(followers-actgot>1)addstr("Some leave");
						else if(actgot==0)addstr("The worker leaves");
						else addstr("One worker leaves");
						addstr(" you, as there are many Liberals here.");

						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(17,1);
						addstr("The last thing you need is more attention.");
						}

					refresh();
					getch();
					}
				}

			if(c>='1'&&c<='6')
				{
				if(activesquad->squad[c-'1']!=NULL)
					{
					if(party_status==c-'1')fullstatus(party_status);
					else party_status=c-'1';
					}
				}

			if(c=='e')
				{
				equip(activesquad->loot,-1);

				if(enemy&&sitealarm)enemyattack();
				else if(enemy)disguisecheck();

				creatureadvance();
				}

			if(c=='g'&&(groundloot.size()>0||(map[locx][locy][locz].flag&SITEBLOCK_LOOT)))
				{
				char tookground=0;

				if(map[locx][locy][locz].flag&SITEBLOCK_LOOT)
					{
					map[locx][locy][locz].flag&=~SITEBLOCK_LOOT;

					if(location[cursite]->siege.siege)
						{
						//GRAB SOME OF THE BASE LOOT
						int lcount=1; //1 FROM THE ONE DELETED ABOVE

						for(int x=0;x<MAPX;x++)
							{
							for(int y=0;y<MAPY;y++)
								{
								for(int z=0;z<MAPZ;z++)
									{
									if(map[x][y][z].flag&SITEBLOCK_LOOT)
										{
										lcount++;
										}
									}
								}
							}

						int lplus=location[cursite]->loot.size()/lcount;
						if(lcount==1)lplus=location[cursite]->loot.size();

						itemst *it;
						int b;

						while(lplus>0)
							{
							b=LCSrandom(location[cursite]->loot.size());
							it=location[cursite]->loot[b];
							activesquad->loot.push_back(it);
							location[cursite]->loot.erase(location[cursite]->loot.begin() + b);

							lplus--;
							}
						}
					else
						{
						int time=20+LCSrandom(10);
						if(time<1)time=1;
						if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

						itemst *item;

						switch(sitetype)
							{
							case SITE_RESIDENTIAL_TENEMENT:
								item=new itemst;item->type=ITEM_LOOT;
									item->loottype=LOOT_TV;
								activesquad->loot.push_back(item);
								break;
							case SITE_RESIDENTIAL_APARTMENT:
							case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
								item=new itemst;item->type=ITEM_LOOT;
									if(!LCSrandom(5))item->loottype=LOOT_CELLPHONE;
									else if(!LCSrandom(4))item->loottype=LOOT_SILVERWARE;
									else if(!LCSrandom(3))item->loottype=LOOT_PRINTER;
									else if(!LCSrandom(2))item->loottype=LOOT_TV;
									else item->loottype=LOOT_COMPUTER;
								activesquad->loot.push_back(item);
								break;
							case SITE_LABORATORY_COSMETICS:
							case SITE_INDUSTRY_NUCLEAR:
							case SITE_LABORATORY_GENETIC:
								item=new itemst;item->type=ITEM_LOOT;
									if(!LCSrandom(2))item->loottype=LOOT_LABEQUIPMENT;
									else if(!LCSrandom(2))item->loottype=LOOT_TV;
									else if(!LCSrandom(5))item->loottype=LOOT_SCANNER;
									else if(!LCSrandom(3))item->loottype=LOOT_PRINTER;
									else if(!LCSrandom(5))item->loottype=LOOT_CHEMICAL;
									else item->loottype=LOOT_COMPUTER;
								activesquad->loot.push_back(item);
								break;
							case SITE_GOVERNMENT_POLICESTATION:
								item=new itemst;item->type=ITEM_LOOT;
									if(!LCSrandom(40))item->loottype=LOOT_POLICERECORDS;
									else if(!LCSrandom(3))item->loottype=LOOT_TV;
									else if(!LCSrandom(2))item->loottype=LOOT_PRINTER;
									else item->loottype=LOOT_COMPUTER;
								activesquad->loot.push_back(item);
								break;
							case SITE_GOVERNMENT_COURTHOUSE:
								item=new itemst;item->type=ITEM_LOOT;
									if(!LCSrandom(30))item->loottype=LOOT_POLICERECORDS;
									else if(!LCSrandom(2))item->loottype=LOOT_TV;
									else item->loottype=LOOT_COMPUTER;
								activesquad->loot.push_back(item);
								break;
							case SITE_GOVERNMENT_PRISON:
								item=new itemst;item->type=ITEM_WEAPON;
								item->weapon.type=WEAPON_SHANK;
								item->weapon.ammo=0;
								activesquad->loot.push_back(item);
								break;
							case SITE_GOVERNMENT_INTELLIGENCEHQ:
								item=new itemst;item->type=ITEM_LOOT;
									if(!LCSrandom(3))item->loottype=LOOT_CELLPHONE;
									else if(!LCSrandom(2))item->loottype=LOOT_TV;
									else if(LCSrandom(199))item->loottype=LOOT_COMPUTER;
									else item->loottype=LOOT_SECRETDOCUMENTS;
								activesquad->loot.push_back(item);
								break;
							case SITE_INDUSTRY_SWEATSHOP:
								item=new itemst;item->type=ITEM_LOOT;
									item->loottype=LOOT_FINECLOTH;
								activesquad->loot.push_back(item);
								break;
							case SITE_INDUSTRY_POLLUTER:
								item=new itemst;item->type=ITEM_LOOT;
									if(!LCSrandom(4))item->loottype=LOOT_PRINTER;
									else if(!LCSrandom(3))item->loottype=LOOT_TV;
									else if(!LCSrandom(2))item->loottype=LOOT_CHEMICAL;
									else item->loottype=LOOT_COMPUTER;
								activesquad->loot.push_back(item);
								break;
							case SITE_CORPORATE_HEADQUARTERS:
								item=new itemst;item->type=ITEM_LOOT;
									if(!LCSrandom(4))item->loottype=LOOT_PRINTER;
									else if(!LCSrandom(3))item->loottype=LOOT_TV;
									else if(!LCSrandom(2))item->loottype=LOOT_CELLPHONE;
									else item->loottype=LOOT_COMPUTER;
								activesquad->loot.push_back(item);
								break;
							case SITE_CORPORATE_HOUSE:
								item=new itemst;item->type=ITEM_LOOT;
									if(!LCSrandom(5))item->loottype=LOOT_CELLPHONE;
									else if(!LCSrandom(4))item->loottype=LOOT_SILVERWARE;
									else if(!LCSrandom(3))item->loottype=LOOT_PRINTER;
									else if(!LCSrandom(2))item->loottype=LOOT_TV;
									else item->loottype=LOOT_COMPUTER;
								activesquad->loot.push_back(item);
								break;
							case SITE_MEDIA_AMRADIO:
								item=new itemst;item->type=ITEM_LOOT;
									if(!LCSrandom(5))item->loottype=LOOT_CELLPHONE;
									else if(!LCSrandom(4))item->loottype=LOOT_BROADCASTINGEQUIPMENT;
									else if(!LCSrandom(3))item->loottype=LOOT_PRINTER;
									else if(!LCSrandom(2))item->loottype=LOOT_TV;
									else item->loottype=LOOT_COMPUTER;
								activesquad->loot.push_back(item);
								break;
							case SITE_MEDIA_CABLENEWS:
								item=new itemst;item->type=ITEM_LOOT;
									if(!LCSrandom(5))item->loottype=LOOT_CELLPHONE;
									else if(!LCSrandom(4))item->loottype=LOOT_TVCAMERA;
									else if(!LCSrandom(3))item->loottype=LOOT_PRINTER;
									else if(!LCSrandom(2))item->loottype=LOOT_TV;
									else item->loottype=LOOT_COMPUTER;
								activesquad->loot.push_back(item);
								break;
							}
						}
					tookground=1;
					}

				//MAKE GROUND LOOT INTO MISSION LOOT
				for(int l=0;l<groundloot.size();l++)
					{
					activesquad->loot.push_back(groundloot[l]);
					}

				groundloot.clear();

				if(enemy&&sitealarm)enemyattack();
				else if(enemy)disguisecheck();

				if(tookground)
					{
					alienationcheck(1);
					noticecheck(-1);
					sitecrime++;
					sitestory->crime.push_back(CRIME_STOLEGROUND);
					criminalizeparty(LAWFLAG_THIEF);
					}

				creatureadvance();
				}

			int cbase=-1;
			if(activesquad->squad[0]!=NULL)
				{
				cbase=activesquad->squad[0]->base;
				}

			if(locx!=olocx||locy!=olocy||locz!=olocz||c=='s'||
				(cbase==cursite&&!location[cursite]->siege.siege))
				{
				//NEED TO GO BACK TO OLD LOCATION IN CASE COMBAT
					//REFRESHES THE SCREEN
				long nlocx=locx;
				long nlocy=locy;
				long nlocz=locz;
				locx=olocx;
				locy=olocy;
				locz=olocz;

				//ENEMIES SHOULD GET FREE SHOTS NOW
				if(enemy&&sitealarm)enemyattack();
				else if(enemy)disguisecheck();

				creatureadvance();

				partyalive=0;
				for(p=0;p<6;p++)
					{
					if(activesquad->squad[p]==NULL)continue;
					if(activesquad->squad[p]->alive==1)partyalive++;
					}

				if(partyalive==0)continue;
				
				//AFTER DEATH CHECK CAN MOVE BACK TO NEW LOCATION
				locx=nlocx;
				locy=nlocy;
				locz=nlocz;
				
				//CHECK FOR EXIT
				if((map[locx][locy][locz].flag & SITEBLOCK_EXIT)||
					(cbase==cursite&&!location[cursite]->siege.siege&&
					bail_on_base))
					{
					//RESET MODE PRIOR TO CHASE
					showcarprefs=0;

					//CHASE SEQUENCE OR FOOT CHASE
					chaseseq.clean();
					chaseseq.location=cursite;
					long level=sitecrime;
					if(sitealarm==0)level=0;
					if(LCSrandom(3)&&level<4)level=0;
					if(LCSrandom(2)&&level<8)level=0;
					if(postalarmtimer<10+LCSrandom(20))level=0;
					else if(postalarmtimer<20+LCSrandom(20)&&LCSrandom(3))level=0;
					else if(postalarmtimer<40+LCSrandom(20)&&!LCSrandom(3))level=0;
					if(location[cursite]->siege.siege)level=1000;

					//MAKE SURE YOU ARE GUILTY OF SOMETHING
					char guilty=0;
					for(p=0;p<6;p++)
						{
						if(activesquad->squad[p]!=NULL)
							{
							if(activesquad->squad[p]->lawflag!=0||
								activesquad->squad[p]->lawflag2!=0)
								{
								guilty=1;
								break;
								}
							}
						}
					if(!guilty)level=0;

					makechasers(sitetype,level);

					char havecar=0;
					for(p=0;p<6;p++)
						{
						if(activesquad->squad[p]!=NULL)
							{
							if(activesquad->squad[p]->carid!=-1)
								{
								havecar=1;
								break;
								}
							}
						}
					char gotout;
					if(havecar)gotout=chasesequence();
					else gotout=footchase();

					if(gotout)
						{
						//DEAL WITH PRISONERS AND STOP BLEEDING
						for(p=0;p<6;p++)
							{
							if(activesquad->squad[p]==NULL)continue;
							if(activesquad->squad[p]->prisoner!=NULL)
								{
								if(activesquad->squad[p]->prisoner->squadid!=-1)
									{
									//RESTORE POOL MEMBER
									activesquad->squad[p]->prisoner->squadid=-1;
									//MUST LOCATE THE MEMBER
									activesquad->squad[p]->prisoner->location=activesquad->squad[p]->base;
									activesquad->squad[p]->prisoner->base=activesquad->squad[p]->base;
									}
								else
									{
									//CONVERT KIDNAP VICTIM
									kidnaptransfer(*activesquad->squad[p]->prisoner);
									delete activesquad->squad[p]->prisoner;
									}
								activesquad->squad[p]->prisoner=NULL;
								}
							}
						for(p=0;p<pool.size();p++)
							{
							pool[p]->flag&=~CREATUREFLAG_JUSTESCAPED;
							for(int w=0;w<BODYPARTNUM;w++)
								{
								pool[p]->wound[w]&=~WOUND_BLEEDING;
								}
							}

						//END SITE MODE
						if(location[cursite]->siege.siege)
							{
							escapesiege(0);
							}
						else
							{
							if(!sitealienate)
								{
								long addjuice=sitecrime;if(addjuice>20)addjuice=20;
								juiceparty(addjuice);
								}
							resolvesite();
							}
						}
					//END OF GAME CHECK
					else if(!endcheck())
						{
						if(location[cursite]->siege.siege)
							{
							if(location[cursite]->siege.underattack)sitestory->type=NEWSSTORY_SQUAD_KILLED_SIEGEATTACK;
							else sitestory->type=NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE;

							location[cursite]->siege.siege=0;
							}
						else sitestory->type=NEWSSTORY_SQUAD_KILLED_SITE;

						if(!location[cursite]->siege.siege)
							{
							if(!sitealienate)
								{
								long addjuice=sitecrime;if(addjuice>20)addjuice=20;
								juiceparty(addjuice);
								}
							resolvesite();
							}
						}

					mode=GAMEMODE_BASE;
					return;
					}

				//SEE IF THERE IS AN ENCOUNTER
				char newenc=0;

				if(!location[cursite]->siege.siege&&!LCSrandom(10))newenc=1;

				for(int e=0;e<ENCMAX;e++)
					{
					if(encounter[e].exists)newenc=0;
					}

				//LOOK FOR SPECIALS
				long makespecial=-1;
				switch(map[locx][locy][locz].special)
					{
					case SPECIAL_APARTMENT_LANDLORD:
					case SPECIAL_RESTAURANT_TABLE:
					case SPECIAL_CAFE_COMPUTER:
						makespecial=map[locx][locy][locz].special;
						newenc=1;
						break;
					}

				//DO DOORS
				if(map[locx][locy][locz].flag & SITEBLOCK_DOOR)
					{
					if((map[locx][locy][locz].flag & SITEBLOCK_LOCKED) &&
						!(map[locx][locy][locz].flag & SITEBLOCK_CLOCK))
						{
						map[locx][locy][locz].flag|=SITEBLOCK_KLOCK;

						do
							{
							clearmessagearea();

							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(16,1);
							addstr("You try the door, but it is locked.");
							move(17,1);
							addstr("Try to pick the lock? (Yes or No)");

							refresh();

							int c=getch();
							translategetch(c);

							if(c=='y')
								{
								char actual;

								if(unlock(UNLOCK_DOOR,actual))
									{
									map[locx][locy][locz].flag&=~SITEBLOCK_LOCKED;
									sitecrime++;
									sitestory->crime.push_back(CRIME_UNLOCKEDDOOR);
									criminalizeparty(LAWFLAG_BREAKING);
									}
								else map[locx][locy][locz].flag|=SITEBLOCK_CLOCK;

								if(actual)
									{
									alienationcheck(1);
									noticecheck(-1);
									}

								break;
								}
							else if(c=='n')break;

							}while(1);
						}
					else if(map[locx][locy][locz].flag & SITEBLOCK_CLOCK)
						{
						do
							{
							clearmessagearea();

							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(16,1);
							addstr("You shake the handle but it is still locked.");
							move(17,1);
							addstr("Force it open? (Yes or No)");

							refresh();
							int c=getch();
							translategetch(c);

							if(c=='y')
								{
								char actual;

								if(bash(BASH_DOOR,actual))
									{
									map[locx][locy][locz].flag&=~SITEBLOCK_DOOR;
									int time=20+LCSrandom(10);
									if(time<1)time=1;
									if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;
									sitecrime++;
									sitestory->crime.push_back(CRIME_BROKEDOWNDOOR);
									criminalizeparty(LAWFLAG_BREAKING);
									}

								if(actual)
									{
									alienationcheck(1);
									noticecheck(-1);
									}

								break;
								}
							else if(c=='n')break;

							}while(1);
						}
					else
						{
						map[locx][locy][locz].flag&=~SITEBLOCK_DOOR;
						}

					locx=olocx;
					locy=olocy;
					locz=olocz;
					if(encsize>0)newenc=0;
					}

				if(location[cursite]->siege.siege)
					{
					if(locx!=olocx||locy!=olocy||locz!=olocz)
						{
						for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;
						}

					//MOVE SIEGE UNITS AROUND
						//MOVE UNITS
					vector<int> unitx;
					vector<int> unity;
					vector<int> unitz;

					for(x=0;x<MAPX;x++)
						{
						for(int y=0;y<MAPY;y++)
							{
							for(int z=0;z<MAPZ;z++)
								{
								if(map[x][y][z].siegeflag & SIEGEFLAG_UNIT)
									{
									unitx.push_back(x);
									unity.push_back(y);
									unitz.push_back(z);
									}
								}
							}
						}

					int sx=0,sy=0,sz=0;
					for(u=0;u<unitx.size();u++)
						{
						sz=0;
						switch(LCSrandom(4))
							{
							case 0:sx=-1;sy=0;
								break;
							case 1:sx=1;sy=0;
								break;
							case 2:sx=0;sy=1;
								break;
							case 3:sx=0;sy=-1;
								break;
							}
						sx=unitx[u]+sx;
						sy=unity[u]+sy;
						sz=unitz[u]+sz;

						if(sx>=0&&sx<MAPX&&sy>=0&&sy<MAPY&&sz>=0&&sz<MAPZ)
							{
							if(!(map[sx][sy][sz].flag & SITEBLOCK_BLOCK))
								{
								if((map[sx][sy][sz].flag & SITEBLOCK_DOOR))
									{
									map[sx][sy][sz].flag&=~SITEBLOCK_DOOR;
									map[sx][sy][sz].flag&=~SITEBLOCK_LOCKED;
									map[sx][sy][sz].flag&=~SITEBLOCK_KLOCK;
									map[sx][sy][sz].flag&=~SITEBLOCK_CLOCK;
									}
								else
									{
									char conf=1;

									//BLOCK PASSAGE
									if(map[sx][sy][sz].siegeflag & SIEGEFLAG_UNIT)conf=0;
									if(map[sx][sy][sz].siegeflag & SIEGEFLAG_HEAVYUNIT)conf=0;

									if(conf)
										{
										map[unitx[u]][unity[u]][unitz[u]].siegeflag&=~SIEGEFLAG_UNIT;

										//BLOW TRAPS
										if(map[sx][sy][sz].siegeflag & SIEGEFLAG_TRAP)
											{
											map[sx][sy][sz].siegeflag&=~SIEGEFLAG_TRAP;
											map[sx][sy][sz].siegeflag|=SIEGEFLAG_UNIT_DAMAGED;
											}
										else
											{
											map[sx][sy][sz].siegeflag|=SIEGEFLAG_UNIT;
											}
										}
									}
								}
							}
						}

					unitx.clear();
					unity.clear();
					unitz.clear();

						//MOVE HEAVY UNITS
					for(x=0;x<MAPX;x++)
						{
						for(int y=0;y<MAPY;y++)
							{
							for(int z=0;z<MAPZ;z++)
								{
								if(map[x][y][z].siegeflag & SIEGEFLAG_HEAVYUNIT)
									{
									unitx.push_back(x);
									unity.push_back(y);
									unitz.push_back(z);
									}
								}
							}
						}

					for(u=0;u<unitx.size();u++)
						{
						sz=0;
						switch(LCSrandom(4))
							{
							case 0:sx=-1;sy=0;
								break;
							case 1:sx=1;sy=0;
								break;
							case 2:sx=0;sy=1;
								break;
							case 3:sx=0;sy=-1;
								break;
							}
						sx=unitx[u]+sx;
						sy=unity[u]+sy;
						sz=unitz[u]+sz;

						if(sx>=0&&sx<MAPX&&sy>=0&&sy<MAPY&&sz>=0&&sz<MAPZ)
							{
							if(!(map[sx][sy][sz].flag & SITEBLOCK_BLOCK))
								{
								if((map[sx][sy][sz].flag & SITEBLOCK_DOOR))
									{
									map[sx][sy][sz].flag&=~SITEBLOCK_DOOR;
									map[sx][sy][sz].flag&=~SITEBLOCK_LOCKED;
									map[sx][sy][sz].flag&=~SITEBLOCK_KLOCK;
									map[sx][sy][sz].flag&=~SITEBLOCK_CLOCK;
									}
								else
									{
									char conf=1;

									//BLOCK PASSAGE
									if(map[sx][sy][sz].siegeflag & SIEGEFLAG_UNIT)conf=0;
									if(map[sx][sy][sz].siegeflag & SIEGEFLAG_HEAVYUNIT)conf=0;

									if(conf)
										{
										map[unitx[u]][unity[u]][unitz[u]].siegeflag&=~SIEGEFLAG_HEAVYUNIT;
										map[sx][sy][sz].siegeflag|=SIEGEFLAG_HEAVYUNIT;

										//BLOW (DIFFUSE) TRAPS
										if(map[sx][sy][sz].siegeflag & SIEGEFLAG_TRAP)
											{
											map[sx][sy][sz].siegeflag&=~SIEGEFLAG_TRAP;
											}
										}
									}
								}
							else
								{
								//BREAK WALLS
								if(sy>=3&&sx>0&&sx<MAPX-1&&sy<MAPY-1)
									{
									map[sx][sy][sz].flag&=~SITEBLOCK_BLOCK;
									}
								}
							}
						}

					unitx.clear();
					unity.clear();
					unitz.clear();

					for(u=0;u<unitx.size();u++)
						{
						sz=0;
						switch(LCSrandom(4))
							{
							case 0:sx=-1;sy=0;
								break;
							case 1:sx=1;sy=0;
								break;
							case 2:sx=0;sy=1;
								break;
							case 3:sx=0;sy=-1;
								break;
							}
						sx=unitx[u]+sx;
						sy=unity[u]+sy;
						sz=unitz[u]+sz;

						if(sx>=0&&sx<MAPX&&sy>=0&&sy<MAPY&&sz>=0&&sz<MAPZ)
							{
							if(!(map[sx][sy][sz].flag & SITEBLOCK_BLOCK))
								{
								if((map[sx][sy][sz].flag & SITEBLOCK_DOOR))
									{
									map[sx][sy][sz].flag&=~SITEBLOCK_DOOR;
									map[sx][sy][sz].flag&=~SITEBLOCK_LOCKED;
									map[sx][sy][sz].flag&=~SITEBLOCK_KLOCK;
									map[sx][sy][sz].flag&=~SITEBLOCK_CLOCK;
									}
								else
									{
									char conf=1;

									//BLOCK PASSAGE
									if(map[sx][sy][sz].siegeflag & SIEGEFLAG_UNIT)conf=0;
									if(map[sx][sy][sz].siegeflag & SIEGEFLAG_HEAVYUNIT)conf=0;
									}
								}
							}
						}

					unitx.clear();
					unity.clear();
					unitz.clear();

					//NEW WAVES
						//IF THERE AREN'T ENOUGH UNITS AROUND
							//AND THEY HAVEN'T BEEN SCARED OFF
								//MORE WAVES WILL ATTACK
									//AND IT GETS WORSE AND WORSE
					location[cursite]->siege.attacktime++;
					if(location[cursite]->siege.attacktime>=100+LCSrandom(10)&&
						(locz!=0||locx<(MAPX/2-3)||locx>(MAPX/2+3)||
						locy>5))
						{
						location[cursite]->siege.attacktime=0;

						//PLACE UNITS
						int lx,ly,lz;
						int unitnum=5;
						int count=10000;
						for(int t=0;t<unitnum;t++)
							{
							do
								{
								lx=LCSrandom(7)+(MAPX/2)-3;
								ly=LCSrandom(5);
								lz=0;
								count--;
								if(count==0)break;
								}while((map[lx][ly][lz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT))||
									(map[lx][ly][lz].siegeflag & (SIEGEFLAG_UNIT|SIEGEFLAG_HEAVYUNIT|SIEGEFLAG_TRAP)));
							map[lx][ly][lz].siegeflag|=SIEGEFLAG_UNIT;
							}

						if(!(location[cursite]->compound_walls & COMPOUND_TANKTRAPS)&&
							location[cursite]->siege.siegetype==SIEGE_POLICE&&
							location[cursite]->siege.escalationstate>=2)
							{
							count=10000;
							int hunitnum=1;
							for(int t=0;t<hunitnum;t++)
								{
								do
									{
									lx=LCSrandom(7)+(MAPX/2)-3;
									ly=LCSrandom(5);
									lz=0;
									count--;
									if(count==0)break;
									}while((map[lx][ly][lz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT))||
										(map[lx][ly][lz].siegeflag & (SIEGEFLAG_UNIT|SIEGEFLAG_HEAVYUNIT|SIEGEFLAG_TRAP)));
								map[lx][ly][lz].siegeflag|=SIEGEFLAG_HEAVYUNIT;
								}
							}
						}

					//CHECK FOR SIEGE UNITS
						//INCLUDING DAMAGED ONES
					if(map[locx][locy][locz].siegeflag & SIEGEFLAG_UNIT)
						{
						if(addsiegeencounter(SIEGEFLAG_UNIT))
							{
							map[locx][locy][locz].siegeflag&=~SIEGEFLAG_UNIT;
							}
						}
					if(map[locx][locy][locz].siegeflag & SIEGEFLAG_HEAVYUNIT)
						{
						if(addsiegeencounter(SIEGEFLAG_HEAVYUNIT))
							{
							map[locx][locy][locz].siegeflag&=~SIEGEFLAG_HEAVYUNIT;
							}
						}
					if(map[locx][locy][locz].siegeflag & SIEGEFLAG_UNIT_DAMAGED)
						{
						if(addsiegeencounter(SIEGEFLAG_UNIT_DAMAGED))
							{
							map[locx][locy][locz].siegeflag&=~SIEGEFLAG_UNIT_DAMAGED;
							}
						}

					//BAIL UPON VICTORY
					if(location[cursite]->siege.kills>=10)
						{
						if(location[cursite]->siege.underattack)sitestory->type=NEWSSTORY_SQUAD_DEFENDED;
						else sitestory->type=NEWSSTORY_SQUAD_BROKESIEGE;

						//DEAL WITH PRISONERS AND STOP BLEEDING
						for(p=0;p<6;p++)
							{
							if(activesquad->squad[p]==NULL)continue;
							if(activesquad->squad[p]->prisoner!=NULL)
								{
								if(activesquad->squad[p]->prisoner->squadid!=-1)
									{
									//RESTORE POOL MEMBER
									activesquad->squad[p]->prisoner->squadid=-1;
									//MUST LOCATE THE MEMBER
									activesquad->squad[p]->prisoner->location=activesquad->squad[p]->base;
									activesquad->squad[p]->prisoner->base=activesquad->squad[p]->base;
									}
								else
									{
									//CONVERT KIDNAP VICTIM
									kidnaptransfer(*activesquad->squad[p]->prisoner);
									delete activesquad->squad[p]->prisoner;
									}
								activesquad->squad[p]->prisoner=NULL;
								}
							}
						for(p=0;p<pool.size();p++)
							{
							pool[p]->flag&=~CREATUREFLAG_JUSTESCAPED;
							for(int w=0;w<BODYPARTNUM;w++)
								{
								pool[p]->wound[w]&=~WOUND_BLEEDING;
								}
							}

						//INFORM
						clearmessagearea();

						set_color(COLOR_GREEN,COLOR_BLACK,1);
						move(16,1);
						addstr("The Conservatives have shrunk back under");
						move(17,1);
						addstr("the power of your Liberal Convictions!");

						refresh();
						getch();

						conquertext();
						escapesiege(1);

						//RESET MODE
						mode=GAMEMODE_BASE;
						showcarprefs=0;
						return;
						}
					}
				else if(newenc)
					{
					switch(makespecial)
						{
						case SPECIAL_CAFE_COMPUTER:
							if(sitealarm||sitealienate)
								{
								clearmessagearea();
								set_color(COLOR_WHITE,COLOR_BLACK,1);
								move(16,1);
								addstr("The computer has been disconnected.");
								map[locx][locy][locz].special=-1;
								refresh();
								getch();
								}
							else
								{
								clearmessagearea();
								set_color(COLOR_WHITE,COLOR_BLACK,1);
								move(16,1);
								addstr("The computer is occupied.");
								map[locx][locy][locz].special=-1;
								refresh();
								getch();

								prepareencounter(sitetype,0);
								for(int e=1;e<ENCMAX;e++)encounter[e].exists=0;
								}
							break;
						case SPECIAL_RESTAURANT_TABLE:
							if(sitealarm||sitealienate)
								{
								clearmessagearea();
								set_color(COLOR_WHITE,COLOR_BLACK,1);
								move(16,1);
								addstr("The table has been abandoned.");
								map[locx][locy][locz].special=-1;
								refresh();
								getch();
								}
							else
								{
								clearmessagearea();
								set_color(COLOR_WHITE,COLOR_BLACK,1);
								move(16,1);
								addstr("The table is occupied.");
								map[locx][locy][locz].special=-1;
								refresh();
								getch();

								prepareencounter(sitetype,0);
								}
							break;
						case SPECIAL_APARTMENT_LANDLORD:
							if(sitealarm||sitealienate||
								location[cursite]->siege.siege)
								{
								clearmessagearea();
								set_color(COLOR_WHITE,COLOR_BLACK,1);
								move(16,1);
								addstr("The landlord is out of the office.");
								map[locx][locy][locz].special=-1;
								refresh();
								getch();
								}
							else
								{
								clearmessagearea();
								set_color(COLOR_WHITE,COLOR_BLACK,1);
								move(16,1);
								addstr("The landlord is in.");
								map[locx][locy][locz].special=-1;
								refresh();
								getch();

								for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;
								makecreature(encounter[0],CREATURE_LANDLORD);
								}
							break;
						default:
							prepareencounter(sitetype,location[cursite]->highsecurity);
							break;
						}
					hostcheck=1;
					}

				if(!location[cursite]->siege.siege)
					{
					if((locx!=olocx||locy!=olocy||locz!=olocz)&&!newenc)
						{
						//PUT BACK SPECIALS
						for(int e=0;e<ENCMAX;e++)
							{
							if(encounter[e].exists)
								{
								if(!encounter[e].cantbluff&&
									encounter[e].type==CREATURE_LANDLORD)
									{
									map[olocx][olocy][olocz].special=SPECIAL_APARTMENT_LANDLORD;
									}
								encounter[e].exists=0;
								}
							}
						}
					}

				if(locx!=olocx||locy!=olocy||locz!=olocz)
					{
					//NUKE GROUND LOOT
					for(int l=0;l<groundloot.size();l++)
						{
						delete groundloot[l];
						}

					groundloot.clear();

					//MOVE BLOOD
					if(map[olocx][olocy][olocz].flag & SITEBLOCK_BLOODY2)
						{
						map[locx][locy][locz].flag|=SITEBLOCK_BLOODY;
						}
					}

				knowmap(locx,locy,locz);
				}
			}
		
		}while(1);
}

void printlocation(long loc)
{
	if(location[loc]->siege.siege)
		{
		if(!location[loc]->siege.underattack)
			{
			set_color(COLOR_YELLOW,COLOR_BLACK,1);
			move(2,1);
			addstr("The police have surrounded this location.");
			}
		else
			{
			set_color(COLOR_RED,COLOR_BLACK,1);
			move(2,1);
			switch(location[loc]->siege.siegetype)
				{
				case SIEGE_POLICE:
					addstr("The police are raiding this location!");break;
				case SIEGE_CIA:
					addstr("The CIA is raiding this location!");break;
				case SIEGE_HICKS:
					addstr("The masses are storming this location!");break;
				case SIEGE_CORPORATE:
					addstr("The Corporations are raiding this location!");break;
				}
			}
		}
	else
		{
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(2,1);
		addstr("You are not under siege...  yet.");
		}

	if(location[loc]->type==SITE_INDUSTRY_WAREHOUSE)
		{
		if(numbereating(loc)>0)
			{
			if(fooddaysleft(loc))
				{
				if(fooddaysleft(loc)<4)
					{
					if(!location[loc]->siege.siege)set_color(COLOR_WHITE,COLOR_BLACK,0);
					else set_color(COLOR_YELLOW,COLOR_BLACK,1);
					move(3,1);
					addstr("This location has food for only a few days.");
					}
				}
			else
				{
				if(!location[loc]->siege.siege)set_color(COLOR_WHITE,COLOR_BLACK,0);
				else set_color(COLOR_RED,COLOR_BLACK,1);
				move(3,1);
				addstr("This location has insufficient food stores.");
				}
			}

		if(location[loc]->compound_walls & COMPOUND_BASIC)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(4,1);
			addstr("FORTIFIED COMPOUND");
			}

		if(location[loc]->compound_walls & COMPOUND_PRINTINGPRESS)
			{
			set_color(COLOR_CYAN,COLOR_BLACK,1);
			move(4,30);
			addstr("PRINTING PRESS");
			}

		if(location[loc]->front_business!=-1)
			{
			set_color(COLOR_MAGENTA,COLOR_BLACK,1);
			move(4,54);
			addstr("BUSINESS FRONT");
			}

		if(location[loc]->compound_walls & COMPOUND_CAMERAS)
			{
			if(location[loc]->siege.siege&&location[loc]->siege.cameras_off)
				{
				set_color(COLOR_RED,COLOR_BLACK,0);
				move(5,1);
				addstr("CAMERAS OFF");
				}
			else
				{
				set_color(COLOR_GREEN,COLOR_BLACK,1);
				move(5,1);
				addstr("CAMERAS ON");
				}
			}

		if(location[loc]->compound_walls & COMPOUND_TRAPS)
			{
			set_color(COLOR_RED,COLOR_BLACK,1);
			move(5,21);
			addstr("BOOBY TRAPS");
			}

		if(location[loc]->compound_walls & COMPOUND_TANKTRAPS)
			{
			set_color(COLOR_YELLOW,COLOR_BLACK,1);
			move(5,41);
			addstr("TANK TRAPS");
			}

		if(location[loc]->siege.siege&&location[loc]->siege.lights_off)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(5,60);
			addstr("LIGHTS OUT");
			}
		else if(location[loc]->compound_walls & COMPOUND_GENERATOR)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(5,61);
			addstr("GENERATOR");
			}

		int eaters=numbereating(loc);
		int days=fooddaysleft(loc);
		char num[20];

		if(eaters>0)
			{
			if(days>=1)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,0);
				itoa(days,num,10);
				move(6,50);
				addstr(num);
				addstr(" Day");
				if(days!=1)addstr("s");
				addstr(" of Food Left");
				}
			else if(days==0)
				{
				set_color(COLOR_RED,COLOR_BLACK,0);
				move(6,50);
				addstr("Not Enough Food");
				}
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		itoa(location[loc]->compound_stores,num,10);
		move(6,1);
		addstr(num);
		addstr(" Daily Ration");
		if(location[loc]->compound_stores!=1)addstr("s");

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		itoa(eaters,num,10);
		move(6,30);
		addstr(num);
		addstr(" Eating");
		}
}

void printparty(void)
{
	creaturest *party[6]={NULL,NULL,NULL,NULL,NULL,NULL};
	if(activesquad!=NULL)
		{
		for(int p=0;p<6;p++)party[p]=activesquad->squad[p];
		}

	set_color(COLOR_WHITE,COLOR_BLACK,0);

	for(int i=2;i<8;i++)
		{
		move(i,0);
		addstr("                                                                                ");
		}

	if(party_status!=-1&&party[party_status]==NULL)party_status=-1;

	if(party_status!=-1)
		{
		printcreatureinfo(party[party_status]);

		char num[20];
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		itoa(party_status+1,num,10);
		move(1,0);
		addstr(num);
		}
	else
		{
		char str[200];
		char num[20];

		move(1,0);
		addstr("#-CODE NAME------------SKILL---WEAPON---------ARMOR----------HEALTH---TRANSPORT-");

		for(int p=0;p<6;p++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(p+2,0);
			addstr("                                                                                ");
			move(p+2,0);
			addch('1'+p);

			if(party[p]!=NULL)
				{
				if(party[p]->prisoner!=NULL)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(p+2,2);
				addstr(party[p]->name);
				if(party[p]->prisoner!=NULL)addstr("+H");

				unsigned long skill=0;
				char bright=0;
				for(int sk=0;sk<SKILLNUM;sk++)
					{
					skill+=(unsigned long)party[p]->skill[sk];
					if(party[p]->skill_ip[sk]>=100)bright=1;
					}

				set_color(COLOR_WHITE,COLOR_BLACK,bright);
				move(p+2,23);
				itoa(skill,num,10);
				addstr(num);
				addstr("/");
				int wsk=SKILL_HANDTOHAND;
				switch(party[p]->weapon.type)
					{
					case WEAPON_SHANK:
					case WEAPON_KNIFE:
					case WEAPON_SYRINGE:
						wsk=SKILL_KNIFE;
						break;
					case WEAPON_CROWBAR:
					case WEAPON_BASEBALLBAT:
					case WEAPON_CHAIN:
					case WEAPON_GAVEL:
					case WEAPON_NIGHTSTICK:
					case WEAPON_HAMMER:
					case WEAPON_MAUL:
					case WEAPON_CROSS:
					case WEAPON_STAFF:
					case WEAPON_TORCH:
						wsk=SKILL_CLUB;
						break;
					case WEAPON_SWORD:
						wsk=SKILL_SWORD;
						break;
					case WEAPON_PITCHFORK:
						wsk=SKILL_SPEAR;
						break;
					case WEAPON_AXE:
						wsk=SKILL_AXE;
						break;
					case WEAPON_REVOLVER_22:
					case WEAPON_REVOLVER_44:
					case WEAPON_SEMIPISTOL_9MM:
					case WEAPON_SEMIPISTOL_45:
						wsk=SKILL_PISTOL;
						break;
					case WEAPON_SHOTGUN:
						wsk=SKILL_RIFLE;
						break;
					case WEAPON_SEMIRIFLE_M16:
					case WEAPON_SEMIRIFLE_AK47:
						wsk=SKILL_ASSAULTRIFLE;
						break;
					}
				itoa(party[p]->skill[wsk],num,10);
				addstr(num);

				set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(p+2,31);
				getweapon(str,party[p]->weapon.type);
				addstr(str);
				if(party[p]->weapon.ammo>0)
					{
					set_color(COLOR_WHITE,COLOR_BLACK,0);
					char num[20];
					itoa(party[p]->weapon.ammo,num,10);
					addstr(" (");
					addstr(num);
					addstr(")");
					}
				else if(ammotype(party[p]->weapon.type)!=-1)
					{
					if(party[p]->clip[ammotype(party[p]->weapon.type)]>0)
						{
						set_color(COLOR_BLACK,COLOR_BLACK,1);
						char num[20];
						itoa(party[p]->clip[ammotype(party[p]->weapon.type)],num,10);
						addstr(" (");
						addstr(num);
						addstr(")");
						}
					else
						{
						set_color(COLOR_BLACK,COLOR_BLACK,1);
						addstr(" (XX)");
						}
					}

				if(mode!=GAMEMODE_SITE)set_color(COLOR_WHITE,COLOR_BLACK,0);
				else if(hasdisguise(*party[p],sitetype))
					{
					if(party[p]->armor.quality!='1'||
						party[p]->armor.flag!=0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
					else set_color(COLOR_GREEN,COLOR_BLACK,1);
					}
				else set_color(COLOR_RED,COLOR_BLACK,1);
				move(p+2,46);
				getarmor(str,party[p]->armor.type,party[p]->armor.subtype);
				addstr(str);

				printhealthstat(*party[p],p+2,61,TRUE);

				set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(p+2,70);

				long v=-1;
				if(showcarprefs==1)v=id_getcar(party[p]->pref_carid);
				else v=id_getcar(party[p]->carid);
				if(v!=-1&&showcarprefs!=-1)
					{
					getcar(str,vehicle[v]->type);
					char d;
					if(showcarprefs==1)d=party[p]->pref_is_driver;
					else d=party[p]->is_driver;
					if(d)strcat(str,"-D");
					}
				else
					{
					int legok=2;
					if((party[p]->wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
						 (party[p]->wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
					if((party[p]->wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
						 (party[p]->wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;
					if(party[p]->flag & CREATUREFLAG_WHEELCHAIR)strcpy(str,"Wheelchair");
					else if(legok>=1)strcpy(str,"On Foot");
					else strcpy(str,"On \"Foot\"");
					}
				addstr(str);
				}
			}
		}

	makedelimiter(8,0);
}

void burnflag(void)
{
	int flagparts=112;
	short flag[16][7][4];
	int x;
	int y;

	for(int p=0;p<7;p++)
		{
		if(p<3)
			{
			for(x=0;x<6;x++)
				{
				flag[x][p][0]=':';
				flag[x][p][1]=COLOR_WHITE;
				flag[x][p][2]=COLOR_BLUE;
				flag[x][p][3]=1;
				}

			for(x=6;x<16;x++)
				{
				flag[x][p][0]=CH_LOWER_HALF_BLOCK;
				flag[x][p][3]=1;
				flag[x][p][1]=COLOR_WHITE;
				flag[x][p][2]=COLOR_RED;
				}
			}
		else
			{
			for(x=0;x<16;x++)
				{
				if(p<6)
					{
					flag[x][p][0]=CH_LOWER_HALF_BLOCK;
					flag[x][p][1]=COLOR_WHITE;
					flag[x][p][2]=COLOR_RED;
					flag[x][p][3]=1;
					}
				else
					{
					flag[x][p][0]=CH_UPPER_HALF_BLOCK;
					flag[x][p][1]=COLOR_RED;
					flag[x][p][2]=COLOR_BLACK;
					flag[x][p][3]=0;
					}
				}
			}
		}

	x=LCSrandom(16);
	y=LCSrandom(7);
	flag[x][y][0]=CH_DARK_SHADE;
	flag[x][y][1]=COLOR_YELLOW;
	flag[x][y][2]=COLOR_BLACK;
	flag[x][y][3]=1;

	char first=1;

	while(flagparts>0)
		{
		if(!first)
			{
			for(x=0;x<16;x++)
				{
				for(y=0;y<7;y++)
					{
					if(flag[x][y][0]==CH_BOX_DRAWINGS_LIGHT_VERTICAL)flag[x][y][0]--;
					else if(flag[x][y][0]==CH_DARK_SHADE)
						{
						flag[x][y][0]=CH_MEDIUM_SHADE;
						flag[x][y][1]=COLOR_RED;
						flag[x][y][2]=COLOR_BLACK;
						flag[x][y][3]=0;
						}
					else if(flag[x][y][0]==CH_MEDIUM_SHADE)
						{
						flag[x][y][0]=CH_LIGHT_SHADE;
						flag[x][y][1]=COLOR_BLACK;
						flag[x][y][2]=COLOR_BLACK;
						flag[x][y][3]=1;
						}
					else if(flag[x][y][0]==CH_LIGHT_SHADE)
						{
						flagparts--;
						flag[x][y][0]=CH_FULL_BLOCK;
						flag[x][y][1]=COLOR_BLACK;
						flag[x][y][2]=COLOR_BLACK;
						flag[x][y][3]=0;
						}
					}
				}
			}
		else first=0;

		for(x=0;x<16;x++)
			{
			for(y=0;y<7;y++)
				{
				move(y+17,x+32);
				set_color(flag[x][y][1],flag[x][y][2],flag[x][y][3]);
				addch(flag[x][y][0]);
				}
			}
		refresh();

		pause_ms(10);

		char gotnew=0;
		while(!gotnew&&flagparts>3)
			{
			x=LCSrandom(16);
			y=LCSrandom(7);
			char conf=0;
			if(flag[x][y][0]==':'||flag[x][y][0]==CH_UPPER_HALF_BLOCK||flag[x][y][0]==220)
				{
				if(x>0)
					{
					if(flag[x-1][y][0]!=':'&&
						flag[x-1][y][0]!=CH_UPPER_HALF_BLOCK&&
						flag[x-1][y][0]!=CH_LOWER_HALF_BLOCK)conf=1;
					}
				if(x<15)
					{
					if(flag[x+1][y][0]!=':'&&
						flag[x+1][y][0]!=CH_UPPER_HALF_BLOCK&&
						flag[x+1][y][0]!=CH_LOWER_HALF_BLOCK)conf=1;
					}
				if(y>0)
					{
					if(flag[x][y-1][0]!=':'&&
						flag[x][y-1][0]!=CH_UPPER_HALF_BLOCK&&
						flag[x][y-1][0]!=CH_LOWER_HALF_BLOCK)conf=1;
					}
				if(y<6)
					{
					if(flag[x][y+1][0]!=':'&&
						flag[x][y+1][0]!=CH_UPPER_HALF_BLOCK&&
						flag[x][y+1][0]!=CH_LOWER_HALF_BLOCK)conf=1;
					}
				if(conf)
					{
					flag[x][y][0]=CH_BOX_DRAWINGS_LIGHT_VERTICAL;
					flag[x][y][1]=COLOR_YELLOW;
					flag[x][y][2]=COLOR_BLACK;
					flag[x][y][3]=1;
					gotnew=1;
					}
				}
			}
		}
}

void getslogan(void)
{
	set_color(COLOR_WHITE,COLOR_BLACK,0);

	move(23,0);
	addstr("What is your new slogan?");
	move(24,0);
	addstr("                                                                                ");

	keypad(stdscr,FALSE);
	raw_output(FALSE);
	echo();
	curs_set(1);
	mvgetstr(24,0,slogan);
	curs_set(0);
	noecho();
	raw_output(TRUE);
	keypad(stdscr,TRUE);
}

void stopevil(void)
{
 int l = 0;
 int p = 0;
 
	if(activesquad==NULL)return;

	char havecar=0;
	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(activesquad->squad[p]->pref_carid!=-1)
				{
				havecar=1;
				break;
				}
			}
		}

	int page=0;
	long loc=-1;

	vector<long> temploc;
	for(l=0;l<location.size();l++)
		{
		if(location[l]->parent==loc&&location[l]->renting>=0)temploc.push_back(l);
		}
	for(l=0;l<location.size();l++)
		{
		if(location[l]->parent==loc&&location[l]->renting==-1)temploc.push_back(l);
		}

	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addstr("Where will the Squad go?");

		printparty();

		if(loc!=-1)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(8,0);
			addlocationname(location[loc]);
			}

		int y=10;
		for(p=page*11;p<temploc.size()&&p<page*11+11;p++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addch(y-10+(int)'A');
			addstr(" - ");

			addlocationname(location[temploc[p]]);

			if(temploc[p]==activesquad->squad[0]->location)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				addstr(" (Current Location)");
				}
			else if(location[temploc[p]]->renting>=0)
				{
				set_color(COLOR_GREEN,COLOR_BLACK,1);
				addstr(" (Safe House)");
				}
			if(location[temploc[p]]->closed>0)
				{
				set_color(COLOR_RED,COLOR_BLACK,1);
				addstr(" (Too Hot)");
				}
			if(location[temploc[p]]->highsecurity)
				{
				set_color(COLOR_MAGENTA,COLOR_BLACK,1);
				addstr(" (High Security)");
				}
			if(location[temploc[p]]->needcar>0&&!havecar)
				{
				set_color(COLOR_YELLOW,COLOR_BLACK,1);
				addstr(" (Need Car)");
				}
			if(location[temploc[p]]->siege.siege>0)
				{
				set_color(COLOR_RED,COLOR_BLACK,0);
				addstr(" (Under Siege)");
				}
			y++;
			}

		//PAGE UP
		if(page>0)
			{
			move(10,60);
			if(interface_pgup=='[')
				{
				addstr("[ - Previous");
				}
			else
				{
				addstr("PGUP - Previous");
				}
			}
		//PAGE DOWN
		if((page+1)*11<temploc.size())
			{
			move(20,60);
			if(interface_pgup=='[')
				{
				addstr("] - Next");
				}
			else
				{
				addstr("PGDN - Next");
				}
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(24,1);
		if(loc!=-1)addstr("Enter - Back one step.");
		else addstr("Enter - The squad is not yet Liberal enough.");

		refresh();

		int c=getch();
		translategetch(c);

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*11<temploc.size())page++;

		if(c>='a'&&c<='k')
			{
			int sq=page*11+(int)(c-'a');
			if(sq<temploc.size()&&sq>=0)
				{
				int oldloc=loc;
				loc=temploc[sq];
				temploc.clear();

				for(l=0;l<location.size();l++)
					{
					if(location[l]->parent==loc&&location[l]->renting>=0)temploc.push_back(l);
					}
				for(l=0;l<location.size();l++)
					{
					if(location[l]->parent==loc&&location[l]->renting==-1)temploc.push_back(l);
					}

				if(temploc.size()==0)
					{
					if(!location[loc]->closed&&
						(location[loc]->needcar==0||havecar))
						{
						activesquad->activity.type=ACTIVITY_VISIT;
						activesquad->activity.arg=loc;
						return;
						}
					else
						{
						loc=oldloc;
						for(l=0;l<location.size();l++)
							{
							if(location[l]->parent==loc&&location[l]->renting>=0)temploc.push_back(l);
							}
						for(l=0;l<location.size();l++)
							{
							if(location[l]->parent==loc&&location[l]->renting==-1)temploc.push_back(l);
							}
						}
					}
				}
			}

		if(c==10&&loc!=-1)
			{
			loc=location[loc]->parent;
			temploc.clear();
			for(l=0;l<location.size();l++)
				{
				if(location[l]->parent==loc&&location[l]->renting>=0)temploc.push_back(l);
				}
			for(l=0;l<location.size();l++)
				{
				if(location[l]->parent==loc&&location[l]->renting==-1)temploc.push_back(l);
				}
			}
		else if(c==10)break;

		}while(1);
}

void printsitemap(int x,int y,int z)
{
	int partysize=0;
	int partyalive=0;
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)partysize++;
		else continue;

		if(activesquad->squad[p]->alive==1)partyalive++;
		}
	int encsize=0;
	int freeable=0;
	int enemy=0;
	int majorenemy=0;
	int talkers=0;
	for(int e=0;e<ENCMAX;e++)
		{
		if(encounter[e].exists)
			{
			encsize++;
			if(encounter[e].align==-1)enemy++;
			if(encounter[e].type==CREATURE_WORKER_SERVANT||
				encounter[e].type==CREATURE_WORKER_SWEATSHOP)freeable++;
			else if(encounter[e].align==0||encounter[e].align==1)talkers++;
			if(encounter[e].type==CREATURE_CORPORATE_CEO||
				encounter[e].type==CREATURE_RADIOPERSONALITY||
				encounter[e].type==CREATURE_NEWSANCHOR||
				encounter[e].type==CREATURE_SCIENTIST_EMINENT||
				encounter[e].type==CREATURE_JUDGE_CONSERVATIVE)majorenemy++;
			}
		}

	set_color(COLOR_WHITE,COLOR_BLACK,0);
	for(int sx=57;sx<80;sx++)
		{
		for(int sy=9;sy<25;sy++)
			{
			move(sy,sx);
			if(sy==24)addch('-');
			else if(sx==57||sx==79)addch('|');
			else addch(' ');
			}
		}

	int px,py;

	//YOUR BLOCK
	px=65;py=14;
	printblock(x,y,z,px,py);

	//UP BLOCK
	px=65;py=9;
	if(y>0)
		{
		if(map[x][y-1][z].flag & SITEBLOCK_BLOCK)
			{
			if(map[x][y][z].flag & SITEBLOCK_BLOODY2)set_color(COLOR_RED,COLOR_BLACK,0);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(py+4,px);addch(CH_FULL_BLOCK);
			move(py+4,px+1);addch(CH_FULL_BLOCK);
			move(py+4,px+2);addch(CH_FULL_BLOCK);
			move(py+4,px+3);addch(CH_FULL_BLOCK);
			move(py+4,px+4);addch(CH_FULL_BLOCK);
			move(py+4,px+5);addch(CH_FULL_BLOCK);
			move(py+4,px+6);addch(CH_FULL_BLOCK);
			}
		else
			{
			printblock(x,y-1,z,px,py);
			}
		}

	//DOWN BLOCK
	px=65;py=19;
	if(y<MAPY-1)
		{
		if(map[x][y+1][z].flag & SITEBLOCK_BLOCK)
			{
			if(map[x][y][z].flag & SITEBLOCK_BLOODY2)set_color(COLOR_RED,COLOR_BLACK,0);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(py,px);addch(CH_FULL_BLOCK);
			move(py,px+1);addch(CH_FULL_BLOCK);
			move(py,px+2);addch(CH_FULL_BLOCK);
			move(py,px+3);addch(CH_FULL_BLOCK);
			move(py,px+4);addch(CH_FULL_BLOCK);
			move(py,px+5);addch(CH_FULL_BLOCK);
			move(py,px+6);addch(CH_FULL_BLOCK);
			}
		else
			{
			printblock(x,y+1,z,px,py);
			}
		}

	//WEST BLOCK
	px=58;py=14;
	if(x>0)
		{
		if(map[x-1][y][z].flag & SITEBLOCK_BLOCK)
			{
			if(map[x][y][z].flag & SITEBLOCK_BLOODY2)set_color(COLOR_RED,COLOR_BLACK,0);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(py,px+6);addch(CH_FULL_BLOCK);
			move(py+1,px+6);addch(CH_FULL_BLOCK);
			move(py+2,px+6);addch(CH_FULL_BLOCK);
			move(py+3,px+6);addch(CH_FULL_BLOCK);
			move(py+4,px+6);addch(CH_FULL_BLOCK);
			}
		else
			{
			printblock(x-1,y,z,px,py);
			}
		}

	//EAST BLOCK
	px=72;py=14;
	if(x<MAPX-1)
		{
		if(map[x+1][y][z].flag & SITEBLOCK_BLOCK)
			{
			if(map[x][y][z].flag & SITEBLOCK_BLOODY2)set_color(COLOR_RED,COLOR_BLACK,0);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(py,px);addch(CH_FULL_BLOCK);
			move(py+1,px);addch(CH_FULL_BLOCK);
			move(py+2,px);addch(CH_FULL_BLOCK);
			move(py+3,px);addch(CH_FULL_BLOCK);
			move(py+4,px);addch(CH_FULL_BLOCK);
			}
		else
			{
			printblock(x+1,y,z,px,py);
			}
		}

	//NE BLOCK
	px=72;py=9;
	if(x<MAPX-1&&y>0)
		{
		if(map[x+1][y-1][z].flag & SITEBLOCK_BLOCK)
			{
			if(!(map[x][y-1][z].flag & SITEBLOCK_BLOCK))
				{
				if(map[x][y-1][z].flag & SITEBLOCK_BLOODY2)set_color(COLOR_RED,COLOR_BLACK,0);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(py,px);addch(CH_FULL_BLOCK);
				move(py+1,px);addch(CH_FULL_BLOCK);
				move(py+2,px);addch(CH_FULL_BLOCK);
				move(py+3,px);addch(CH_FULL_BLOCK);
				move(py+4,px);addch(CH_FULL_BLOCK);
				}
			if(!(map[x+1][y][z].flag & SITEBLOCK_BLOCK))
				{
				if(map[x+1][y][z].flag & SITEBLOCK_BLOODY2)set_color(COLOR_RED,COLOR_BLACK,0);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(py+4,px);addch(CH_FULL_BLOCK);
				move(py+4,px+1);addch(CH_FULL_BLOCK);
				move(py+4,px+2);addch(CH_FULL_BLOCK);
				move(py+4,px+3);addch(CH_FULL_BLOCK);
				move(py+4,px+4);addch(CH_FULL_BLOCK);
				move(py+4,px+5);addch(CH_FULL_BLOCK);
				move(py+4,px+6);addch(CH_FULL_BLOCK);
				}
			}
		else
			{
			printblock(x+1,y-1,z,px,py);
			}
		}

	//SE BLOCK
	px=72;py=19;
	if(x<MAPX-1&&y<MAPY-1)
		{
		if(map[x+1][y+1][z].flag & SITEBLOCK_BLOCK)
			{
			if(!(map[x][y+1][z].flag & SITEBLOCK_BLOCK))
				{
				if(map[x][y+1][z].flag & SITEBLOCK_BLOODY2)set_color(COLOR_RED,COLOR_BLACK,0);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(py,px);addch(CH_FULL_BLOCK);
				move(py+1,px);addch(CH_FULL_BLOCK);
				move(py+2,px);addch(CH_FULL_BLOCK);
				move(py+3,px);addch(CH_FULL_BLOCK);
				move(py+4,px);addch(CH_FULL_BLOCK);
				}
			if(!(map[x+1][y][z].flag & SITEBLOCK_BLOCK))
				{
				if(map[x+1][y][z].flag & SITEBLOCK_BLOODY2)set_color(COLOR_RED,COLOR_BLACK,0);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(py,px);addch(CH_FULL_BLOCK);
				move(py,px+1);addch(CH_FULL_BLOCK);
				move(py,px+2);addch(CH_FULL_BLOCK);
				move(py,px+3);addch(CH_FULL_BLOCK);
				move(py,px+4);addch(CH_FULL_BLOCK);
				move(py,px+5);addch(CH_FULL_BLOCK);
				move(py,px+6);addch(CH_FULL_BLOCK);
				}
			}
		else
			{
			printblock(x+1,y+1,z,px,py);
			}
		}

	//NW BLOCK
	px=58;py=9;
	if(x>0&&y>0)
		{
		if(map[x-1][y-1][z].flag & SITEBLOCK_BLOCK)
			{
			if(!(map[x][y-1][z].flag & SITEBLOCK_BLOCK))
				{
				if(map[x][y-1][z].flag & SITEBLOCK_BLOODY2)set_color(COLOR_RED,COLOR_BLACK,0);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(py,px+6);addch(CH_FULL_BLOCK);
				move(py+1,px+6);addch(CH_FULL_BLOCK);
				move(py+2,px+6);addch(CH_FULL_BLOCK);
				move(py+3,px+6);addch(CH_FULL_BLOCK);
				move(py+4,px+6);addch(CH_FULL_BLOCK);
				}
			if(!(map[x-1][y][z].flag & SITEBLOCK_BLOCK))
				{
				if(map[x-1][y][z].flag & SITEBLOCK_BLOODY2)set_color(COLOR_RED,COLOR_BLACK,0);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(py+4,px);addch(CH_FULL_BLOCK);
				move(py+4,px+1);addch(CH_FULL_BLOCK);
				move(py+4,px+2);addch(CH_FULL_BLOCK);
				move(py+4,px+3);addch(CH_FULL_BLOCK);
				move(py+4,px+4);addch(CH_FULL_BLOCK);
				move(py+4,px+5);addch(CH_FULL_BLOCK);
				move(py+4,px+6);addch(CH_FULL_BLOCK);
				}
			}
		else
			{
			printblock(x-1,y-1,z,px,py);
			}
		}

	//SW BLOCK
	px=58;py=19;
	if(x>0&&y<MAPY-1)
		{
		if(map[x-1][y+1][z].flag & SITEBLOCK_BLOCK)
			{
			if(!(map[x][y+1][z].flag & SITEBLOCK_BLOCK))
				{
				if(map[x][y+1][z].flag & SITEBLOCK_BLOODY2)set_color(COLOR_RED,COLOR_BLACK,0);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(py,px+6);addch(CH_FULL_BLOCK);
				move(py+1,px+6);addch(CH_FULL_BLOCK);
				move(py+2,px+6);addch(CH_FULL_BLOCK);
				move(py+3,px+6);addch(CH_FULL_BLOCK);
				move(py+4,px+6);addch(CH_FULL_BLOCK);
				}
			if(!(map[x-1][y][z].flag & SITEBLOCK_BLOCK))
				{
				if(map[x-1][y][z].flag & SITEBLOCK_BLOODY2)set_color(COLOR_RED,COLOR_BLACK,0);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(py,px);addch(CH_FULL_BLOCK);
				move(py,px+1);addch(CH_FULL_BLOCK);
				move(py,px+2);addch(CH_FULL_BLOCK);
				move(py,px+3);addch(CH_FULL_BLOCK);
				move(py,px+4);addch(CH_FULL_BLOCK);
				move(py,px+5);addch(CH_FULL_BLOCK);
				move(py,px+6);addch(CH_FULL_BLOCK);
				}
			}
		else
			{
			printblock(x-1,y+1,z,px,py);
			}
		}

	//PRINT SPECIAL
	char str[200];
	switch(map[locx][locy][locz].special)
		{
		case SPECIAL_LAB_COSMETICS_CAGEDANIMALS:strcpy(str,"Caged Animals");break;
		case SPECIAL_NUCLEAR_ONOFF:strcpy(str,"The On/Off Switch");break;
		case SPECIAL_LAB_GENETIC_CAGEDANIMALS:strcpy(str,"Caged \"Animals\"");break;
		case SPECIAL_POLICESTATION_LOCKUP:strcpy(str,"Police Detention Room");break;
		case SPECIAL_COURTHOUSE_LOCKUP:strcpy(str,"Court House Jail");break;
		case SPECIAL_COURTHOUSE_JURYROOM:strcpy(str,"Jury Room");break;
		case SPECIAL_PRISON_CONTROL:strcpy(str,"Prison Control Room");break;
		case SPECIAL_INTEL_SUPERCOMPUTER:strcpy(str,"Super-computer");break;
		case SPECIAL_SWEATSHOP_EQUIPMENT:strcpy(str,"Textile Equipment");break;
		case SPECIAL_POLLUTER_EQUIPMENT:strcpy(str,"Factory Equipment");break;
		case SPECIAL_HOUSE_PHOTOS:strcpy(str,"Safe");break;
		case SPECIAL_CORPORATE_FILES:strcpy(str,"Safe");break;
		case SPECIAL_RADIO_BROADCASTSTUDIO:strcpy(str,"Radio Broadcast Room");break;
		case SPECIAL_NEWS_BROADCASTSTUDIO:strcpy(str,"News Broadcast Studio");break;
		case SPECIAL_APARTMENT_LANDLORD:strcpy(str,"Landlord's Office");break;
		case SPECIAL_APARTMENT_SIGN:strcpy(str,"Sign");break;
		case SPECIAL_STAIRS_UP:strcpy(str,"Stairs Up");break;
		case SPECIAL_STAIRS_DOWN:strcpy(str,"Stairs Down");break;
		case SPECIAL_RESTAURANT_TABLE:strcpy(str,"Table");break;
		case SPECIAL_CAFE_COMPUTER:strcpy(str,"Computer");break;
		}
	if(map[locx][locy][locz].special!=-1)
		{
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(24,68-(strlen(str)>>1));
		addstr(str);
		}

	//PRINT PARTY
	if(partyalive>0)set_color(COLOR_GREEN,COLOR_BLACK,1);
	else set_color(COLOR_BLACK,COLOR_BLACK,1);
	move(16,66);
	addstr("SQUAD");

	//PRINT ANY OPPOSING FORCE INFO
	if(encsize>0)
		{
		set_color(COLOR_YELLOW,COLOR_BLACK,1);
		move(17,65);
		if(map[locx][locx][locz].siegeflag & SIEGEFLAG_HEAVYUNIT)
			{
			addstr("ENC+ARM");
			}
		else if(map[locx][locx][locz].siegeflag & SIEGEFLAG_UNIT)
			{
			addstr("ENC+ENM");
			}
		else if(map[locx][locx][locz].siegeflag & SIEGEFLAG_UNIT_DAMAGED)
			{
			addstr("ENC+enm");
			}
		else addstr("ENCNTER");

		printencounter();
		}

	if(groundloot.size()>0)
		{
		set_color(COLOR_MAGENTA,COLOR_BLACK,1);
		move(18,66);
		addstr("LOOT!");

		printencounter();
		}
}

void printblock(int x,int y,int z,int px,int py)
{
	if(map[x][y][z].flag & SITEBLOCK_RESTRICTED)
		{
		set_color(COLOR_BLACK,COLOR_BLACK,1);
		for(int px2=px;px2<px+7;px2++)
			{
			for(int py2=py;py2<py+5;py2++)
				{
				move(py2,px2);
				addstr("x");
				}
			}
		}

	if(map[x][y][z].flag & SITEBLOCK_BLOODY2)
		{
		set_color(COLOR_RED,COLOR_BLACK,0);
		move(py+3,px+2);
		addch('\\');
		move(py+1,px+1);
		addch('%');
		move(py+4,px+5);
		addch('&');
		move(py+2,px+1);
		addch('.');
		move(py+1,px+2);
		addch('.');
		move(py+4,px+4);
		addch('.');
		}
	else if(map[x][y][z].flag & SITEBLOCK_BLOODY)
		{
		set_color(COLOR_RED,COLOR_BLACK,0);
		move(py+2,px+1);
		addch('.');
		move(py+1,px+2);
		addch('.');
		move(py+4,px+4);
		addch('.');
		}

	if(map[x][y][z].flag & SITEBLOCK_EXIT)
		{
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(py+1,px+1);
		addstr("EXIT");
		}
	else if(map[x][y][z].flag & SITEBLOCK_DOOR)
		{
		if((map[x][y][z].flag & SITEBLOCK_CLOCK) &&
			(map[x][y][z].flag & SITEBLOCK_LOCKED))
			{
			set_color(COLOR_RED,COLOR_BLACK,0);
			move(py+1,px);
			addstr("L. DOOR");
			}
		else if((map[x][y][z].flag & SITEBLOCK_KLOCK) &&
			(map[x][y][z].flag & SITEBLOCK_LOCKED))
			{
			set_color(COLOR_YELLOW,COLOR_BLACK,0);
			move(py+1,px);
			addstr("L. DOOR");
			}
		else
			{
			set_color(COLOR_YELLOW,COLOR_BLACK,0);
			move(py+1,px+1);
			addstr("DOOR");
			}
		}
	else if(map[x][y][z].flag & SITEBLOCK_LOOT)
		{
		set_color(COLOR_MAGENTA,COLOR_BLACK,1);
		move(py+1,px+1);
		addstr("GOODS");
		}

	if(map[x][y][z].siegeflag & SIEGEFLAG_TRAP)
		{
		set_color(COLOR_YELLOW,COLOR_BLACK,1);
		move(py,px+1);
		addstr("TRAP!");
		}
	else if(map[x][y][z].siegeflag & SIEGEFLAG_UNIT_DAMAGED)
		{
		set_color(COLOR_RED,COLOR_BLACK,0);
		move(py,px+1);
		addstr("enemy");
		}
	else if(map[x][y][z].special!=-1)
		{
		set_color(COLOR_YELLOW,COLOR_BLACK,1);
		move(py,px);

		switch(map[x][y][z].special)
			{
			case SPECIAL_LAB_COSMETICS_CAGEDANIMALS:addstr("ANIMALS");break;
			case SPECIAL_NUCLEAR_ONOFF:addstr("ON/OFF!");break;
			case SPECIAL_LAB_GENETIC_CAGEDANIMALS:addstr("ANIMALS");break;
			case SPECIAL_POLICESTATION_LOCKUP:addstr("LOCK-UP");break;
			case SPECIAL_COURTHOUSE_LOCKUP:addstr("LOCK-UP");break;
			case SPECIAL_COURTHOUSE_JURYROOM:addstr(" JURY!");break;
			case SPECIAL_PRISON_CONTROL:addstr("CONTROL");break;
			case SPECIAL_INTEL_SUPERCOMPUTER:addstr("COMPUTR");break;
			case SPECIAL_SWEATSHOP_EQUIPMENT:addstr("MACHINE");break;
			case SPECIAL_POLLUTER_EQUIPMENT:addstr("MACHINE");break;
			case SPECIAL_HOUSE_PHOTOS:addstr(" SAFE!");break;
			case SPECIAL_CORPORATE_FILES:addstr(" SAFE!");break;
			case SPECIAL_RADIO_BROADCASTSTUDIO:addstr("STUDIO!");break;
			case SPECIAL_NEWS_BROADCASTSTUDIO:addstr("STUDIO!");break;
			case SPECIAL_APARTMENT_LANDLORD:addstr("LANDLRD");break;
			case SPECIAL_APARTMENT_SIGN:addstr(" SIGN!");break;
			case SPECIAL_STAIRS_UP:addstr("STAIRS");addch(24);break;
			case SPECIAL_STAIRS_DOWN:addstr("STAIRS");addch(25);break;
			case SPECIAL_RESTAURANT_TABLE:addstr(" TABLE");break;
			case SPECIAL_CAFE_COMPUTER:addstr("COMPUTR");break;
			}
		}
	if(map[x][y][z].siegeflag & SIEGEFLAG_HEAVYUNIT)
		{
		set_color(COLOR_RED,COLOR_BLACK,1);
		move(py+3,px+1);
		addstr("ARMOR");
		}
	else if(map[x][y][z].siegeflag & SIEGEFLAG_UNIT)
		{
		set_color(COLOR_RED,COLOR_BLACK,1);
		move(py+3,px+1);
		addstr("ENEMY");
		}
}

void prepareencounter(short type,char sec)
{
	int encslot=0;

	for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;

	int creaturearray[CREATURENUM];
	memset(creaturearray,0,CREATURENUM*sizeof(int));

	switch(type)
		{
		case SITE_BUSINESS_CRACKHOUSE:
			{
			creaturearray[CREATURE_TEENAGER]=100;
			creaturearray[CREATURE_MUSICIAN]=1;
			creaturearray[CREATURE_MATHEMATICIAN]=1;
			creaturearray[CREATURE_HSDROPOUT]=30;
			creaturearray[CREATURE_BUM]=200;
			if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
			if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=2;
			if(law[LAW_POLLUTION]==-2&&
				law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=50;
			creaturearray[CREATURE_GANGMEMBER]=200;
			creaturearray[CREATURE_CRACKHEAD]=200;
			creaturearray[CREATURE_PROSTITUTE]=200;
			creaturearray[CREATURE_BIKER]=5;
			creaturearray[CREATURE_PAINTER]=1;
			creaturearray[CREATURE_SCULPTOR]=1;
			creaturearray[CREATURE_THIEF]=3;
			creaturearray[CREATURE_ACTOR]=1;
			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_BUSINESS_JUICEBAR:
			{
			creaturearray[CREATURE_TEENAGER]=10;
			creaturearray[CREATURE_JUDGE_LIBERAL]=1;
			creaturearray[CREATURE_COLLEGESTUDENT]=10;
			creaturearray[CREATURE_MUSICIAN]=2;
			creaturearray[CREATURE_MATHEMATICIAN]=1;
			creaturearray[CREATURE_TEACHER]=1;
			creaturearray[CREATURE_HSDROPOUT]=1;
			creaturearray[CREATURE_ENGINEER]=1;
			creaturearray[CREATURE_FASTFOODWORKER]=1;
			creaturearray[CREATURE_TELEMARKETER]=1;
			creaturearray[CREATURE_OFFICEWORKER]=1;
			creaturearray[CREATURE_MAILMAN]=1;
			creaturearray[CREATURE_CHEF]=1;
			creaturearray[CREATURE_AMATEURMAGICIAN]=1;
			creaturearray[CREATURE_HIPPIE]=6;
			creaturearray[CREATURE_CRITIC_ART]=1;
			creaturearray[CREATURE_CRITIC_MUSIC]=1;
			creaturearray[CREATURE_AUTHOR]=1;
			creaturearray[CREATURE_JOURNALIST]=1;
			creaturearray[CREATURE_SOCIALITE]=2;
			creaturearray[CREATURE_PROGRAMMER]=1;
			creaturearray[CREATURE_RETIREE]=1;
			creaturearray[CREATURE_PAINTER]=1;
			creaturearray[CREATURE_SCULPTOR]=1;
			creaturearray[CREATURE_DANCER]=1;
			creaturearray[CREATURE_PHOTOGRAPHER]=1;
			creaturearray[CREATURE_CAMERAMAN]=1;
			creaturearray[CREATURE_HAIRSTYLIST]=1;
			creaturearray[CREATURE_FASHIONDESIGNER]=1;
			creaturearray[CREATURE_CLERK]=1;
			creaturearray[CREATURE_THIEF]=1;
			creaturearray[CREATURE_ACTOR]=1;
			creaturearray[CREATURE_YOGAINSTRUCTOR]=1;
			creaturearray[CREATURE_ATHLETE]=1;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_BUSINESS_CIGARBAR:
			{
			if(sec)creaturearray[CREATURE_SECURITYGUARD]=1000;
			else creaturearray[CREATURE_SECURITYGUARD]=100;
			creaturearray[CREATURE_SCIENTIST_EMINENT]=1;
			creaturearray[CREATURE_CORPORATE_MANAGER]=30;
			creaturearray[CREATURE_CORPORATE_CEO]=1;
			creaturearray[CREATURE_COP]=5;
			if(law[LAW_DEATHPENALTY]==-2&&
			   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_DEATHSQUAD]=2;
			if(law[LAW_POLICEBEHAVIOR]<=-1)creaturearray[CREATURE_GANGUNIT]=2;
			creaturearray[CREATURE_JUDGE_CONSERVATIVE]=1;
			creaturearray[CREATURE_RADIOPERSONALITY]=1;
			creaturearray[CREATURE_NEWSANCHOR]=1;
			creaturearray[CREATURE_LAWYER]=15;
			creaturearray[CREATURE_MUSICIAN]=1;
			creaturearray[CREATURE_ENGINEER]=10;
			creaturearray[CREATURE_FOOTBALLCOACH]=1;
			creaturearray[CREATURE_CRITIC_ART]=1;
			creaturearray[CREATURE_CRITIC_MUSIC]=1;
			creaturearray[CREATURE_AUTHOR]=1;
			creaturearray[CREATURE_JOURNALIST]=1;
			creaturearray[CREATURE_SOCIALITE]=2;
			creaturearray[CREATURE_RETIREE]=1;
			creaturearray[CREATURE_PAINTER]=1;
			creaturearray[CREATURE_SCULPTOR]=1;
			creaturearray[CREATURE_DANCER]=1;
			creaturearray[CREATURE_PHOTOGRAPHER]=1;
			creaturearray[CREATURE_FASHIONDESIGNER]=1;
			creaturearray[CREATURE_ACTOR]=1;
			creaturearray[CREATURE_ATHLETE]=1;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_BUSINESS_LATTESTAND:
			{
			creaturearray[CREATURE_SECURITYGUARD]=5;
			creaturearray[CREATURE_SCIENTIST_LABTECH]=10;
			creaturearray[CREATURE_SCIENTIST_EMINENT]=1;
			creaturearray[CREATURE_CORPORATE_MANAGER]=10;
			creaturearray[CREATURE_CORPORATE_CEO]=1;
			creaturearray[CREATURE_WORKER_JANITOR]=5;
			if(law[LAW_LABOR]<2)creaturearray[CREATURE_WORKER_FACTORY_NONUNION]=5;
			creaturearray[CREATURE_WORKER_SECRETARY]=15;
			if(law[LAW_LABOR]>=0)creaturearray[CREATURE_WORKER_FACTORY_UNION]=5;
			creaturearray[CREATURE_TEENAGER]=5;
			creaturearray[CREATURE_COP]=5;
			if(law[LAW_DEATHPENALTY]==-2&&
			   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_DEATHSQUAD]=2;
			if(law[LAW_POLICEBEHAVIOR]<=-1)creaturearray[CREATURE_GANGUNIT]=2;
			creaturearray[CREATURE_JUDGE_LIBERAL]=1;
			creaturearray[CREATURE_JUDGE_CONSERVATIVE]=1;
			creaturearray[CREATURE_AGENT]=1;
			creaturearray[CREATURE_RADIOPERSONALITY]=1;
			creaturearray[CREATURE_NEWSANCHOR]=1;
			creaturearray[CREATURE_LAWYER]=10;
			creaturearray[CREATURE_SEWERWORKER]=1;
			creaturearray[CREATURE_COLLEGESTUDENT]=30;
			creaturearray[CREATURE_MUSICIAN]=5;
			creaturearray[CREATURE_MATHEMATICIAN]=5;
			creaturearray[CREATURE_TEACHER]=5;
			creaturearray[CREATURE_HSDROPOUT]=1;
			creaturearray[CREATURE_BUM]=1;
			if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=1;
			if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=1;
			if(law[LAW_POLLUTION]==-2&&
				law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
			creaturearray[CREATURE_GANGMEMBER]=1;
			creaturearray[CREATURE_CRACKHEAD]=1;
			creaturearray[CREATURE_PRIEST]=1;
			creaturearray[CREATURE_ENGINEER]=5;
			creaturearray[CREATURE_FASTFOODWORKER]=5;
			creaturearray[CREATURE_TELEMARKETER]=5;
			creaturearray[CREATURE_OFFICEWORKER]=10;
			creaturearray[CREATURE_FOOTBALLCOACH]=1;
			creaturearray[CREATURE_PROSTITUTE]=1;
			creaturearray[CREATURE_MAILMAN]=1;
			creaturearray[CREATURE_GARBAGEMAN]=1;
			creaturearray[CREATURE_PLUMBER]=1;
			creaturearray[CREATURE_CHEF]=1;
			creaturearray[CREATURE_CONSTRUCTIONWORKER]=3;
			creaturearray[CREATURE_AMATEURMAGICIAN]=1;
			creaturearray[CREATURE_MERC]=1;
			creaturearray[CREATURE_SOLDIER]=1;
			if(law[LAW_DEATHPENALTY]==-2&&
			   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]=1;
			else creaturearray[CREATURE_PRISONGUARD]=1;
			creaturearray[CREATURE_HIPPIE]=1;
			creaturearray[CREATURE_CRITIC_ART]=1;
			creaturearray[CREATURE_CRITIC_MUSIC]=1;
			creaturearray[CREATURE_AUTHOR]=1;
			creaturearray[CREATURE_JOURNALIST]=1;
			creaturearray[CREATURE_SOCIALITE]=1;
			creaturearray[CREATURE_BIKER]=1;
			creaturearray[CREATURE_TRUCKER]=1;
			creaturearray[CREATURE_TAXIDRIVER]=1;
			creaturearray[CREATURE_PROGRAMMER]=5;
			creaturearray[CREATURE_RETIREE]=3;
			creaturearray[CREATURE_PAINTER]=1;
			creaturearray[CREATURE_SCULPTOR]=1;
			creaturearray[CREATURE_DANCER]=1;
			creaturearray[CREATURE_PHOTOGRAPHER]=1;
			creaturearray[CREATURE_CAMERAMAN]=1;
			creaturearray[CREATURE_HAIRSTYLIST]=1;
			creaturearray[CREATURE_FASHIONDESIGNER]=1;
			creaturearray[CREATURE_CLERK]=1;
			creaturearray[CREATURE_THIEF]=1;
			creaturearray[CREATURE_ACTOR]=1;
			creaturearray[CREATURE_YOGAINSTRUCTOR]=1;
			creaturearray[CREATURE_ATHLETE]=1;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_BUSINESS_VEGANCOOP:
			{
			creaturearray[CREATURE_TEENAGER]=5;
			creaturearray[CREATURE_JUDGE_LIBERAL]=1;
			creaturearray[CREATURE_COLLEGESTUDENT]=50;
			creaturearray[CREATURE_MUSICIAN]=20;
			creaturearray[CREATURE_MATHEMATICIAN]=1;
			creaturearray[CREATURE_TEACHER]=1;
			creaturearray[CREATURE_HSDROPOUT]=10;
			creaturearray[CREATURE_BUM]=1;
			if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=1;
			if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=1;
			if(law[LAW_POLLUTION]==-2&&
				law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
			creaturearray[CREATURE_HIPPIE]=50;
			creaturearray[CREATURE_CRITIC_ART]=1;
			creaturearray[CREATURE_CRITIC_MUSIC]=1;
			creaturearray[CREATURE_AUTHOR]=1;
			creaturearray[CREATURE_JOURNALIST]=1;
			creaturearray[CREATURE_RETIREE]=1;
			creaturearray[CREATURE_PAINTER]=1;
			creaturearray[CREATURE_SCULPTOR]=1;
			creaturearray[CREATURE_DANCER]=1;
			creaturearray[CREATURE_PHOTOGRAPHER]=1;
			creaturearray[CREATURE_YOGAINSTRUCTOR]=2;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_BUSINESS_INTERNETCAFE:
			{
			creaturearray[CREATURE_SCIENTIST_LABTECH]=5;
			creaturearray[CREATURE_CORPORATE_MANAGER]=3;
			creaturearray[CREATURE_TEENAGER]=15;
			creaturearray[CREATURE_LAWYER]=3;
			creaturearray[CREATURE_COLLEGESTUDENT]=25;
			creaturearray[CREATURE_MUSICIAN]=2;
			creaturearray[CREATURE_MATHEMATICIAN]=1;
			creaturearray[CREATURE_TEACHER]=5;
			creaturearray[CREATURE_ENGINEER]=15;
			creaturearray[CREATURE_OFFICEWORKER]=15;
			creaturearray[CREATURE_WORKER_SECRETARY]=5;
			creaturearray[CREATURE_HIPPIE]=1;
			creaturearray[CREATURE_PROGRAMMER]=15;
			creaturearray[CREATURE_RETIREE]=5;
			creaturearray[CREATURE_PAINTER]=1;
			creaturearray[CREATURE_SCULPTOR]=1;
			creaturearray[CREATURE_DANCER]=1;
			creaturearray[CREATURE_PHOTOGRAPHER]=1;
			creaturearray[CREATURE_CAMERAMAN]=1;
			creaturearray[CREATURE_CLERK]=1;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_RESIDENTIAL_SHELTER:
			{
			creaturearray[CREATURE_WORKER_JANITOR]=5;
			creaturearray[CREATURE_TEENAGER]=20;
			creaturearray[CREATURE_MUSICIAN]=3;
			creaturearray[CREATURE_MATHEMATICIAN]=1;
			creaturearray[CREATURE_BUM]=200;
			if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
			if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=2;
			if(law[LAW_POLLUTION]==-2&&
				law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=50;
			creaturearray[CREATURE_GANGMEMBER]=20;
			creaturearray[CREATURE_CRACKHEAD]=50;
			creaturearray[CREATURE_PROSTITUTE]=20;
			creaturearray[CREATURE_AMATEURMAGICIAN]=1;
			creaturearray[CREATURE_HIPPIE]=1;
			creaturearray[CREATURE_BIKER]=1;
			creaturearray[CREATURE_PAINTER]=1;
			creaturearray[CREATURE_SCULPTOR]=1;
			creaturearray[CREATURE_DANCER]=1;
			creaturearray[CREATURE_PHOTOGRAPHER]=1;
			creaturearray[CREATURE_THIEF]=1;
			creaturearray[CREATURE_ACTOR]=1;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_RESIDENTIAL_TENEMENT:
			{
			creaturearray[CREATURE_SECURITYGUARD]=1;
			creaturearray[CREATURE_SCIENTIST_LABTECH]=1;
			creaturearray[CREATURE_WORKER_JANITOR]=3;
			if(law[LAW_LABOR]<2)creaturearray[CREATURE_WORKER_FACTORY_NONUNION]=1;
			creaturearray[CREATURE_WORKER_SECRETARY]=2;
			if(law[LAW_LABOR]>=0)creaturearray[CREATURE_WORKER_FACTORY_UNION]=1;
			creaturearray[CREATURE_TEENAGER]=5;
			creaturearray[CREATURE_SEWERWORKER]=1;
			creaturearray[CREATURE_COLLEGESTUDENT]=1;
			creaturearray[CREATURE_MUSICIAN]=1;
			creaturearray[CREATURE_MATHEMATICIAN]=1;
			creaturearray[CREATURE_TEACHER]=1;
			creaturearray[CREATURE_HSDROPOUT]=3;
			creaturearray[CREATURE_BUM]=3;
			if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
			if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=2;
			if(law[LAW_POLLUTION]==-2&&
				law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=5;
			creaturearray[CREATURE_GANGMEMBER]=3;
			creaturearray[CREATURE_CRACKHEAD]=3;
			creaturearray[CREATURE_FASTFOODWORKER]=1;
			creaturearray[CREATURE_TELEMARKETER]=1;
			creaturearray[CREATURE_OFFICEWORKER]=1;
			creaturearray[CREATURE_PROSTITUTE]=3;
			creaturearray[CREATURE_MAILMAN]=1;
			creaturearray[CREATURE_GARBAGEMAN]=1;
			creaturearray[CREATURE_CONSTRUCTIONWORKER]=1;
			creaturearray[CREATURE_AMATEURMAGICIAN]=1;
			creaturearray[CREATURE_HICK]=1;
			creaturearray[CREATURE_SOLDIER]=1;
			if(law[LAW_DEATHPENALTY]==-2&&
			   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]=1;
			else creaturearray[CREATURE_PRISONGUARD]=1;
			creaturearray[CREATURE_HIPPIE]=1;
			creaturearray[CREATURE_BIKER]=1;
			creaturearray[CREATURE_TAXIDRIVER]=1;
			creaturearray[CREATURE_RETIREE]=1;
			creaturearray[CREATURE_PAINTER]=1;
			creaturearray[CREATURE_SCULPTOR]=1;
			creaturearray[CREATURE_DANCER]=1;
			creaturearray[CREATURE_PHOTOGRAPHER]=1;
			creaturearray[CREATURE_HAIRSTYLIST]=1;
			creaturearray[CREATURE_CLERK]=1;
			creaturearray[CREATURE_THIEF]=1;
			creaturearray[CREATURE_ACTOR]=1;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_RESIDENTIAL_APARTMENT:
			{
			creaturearray[CREATURE_SECURITYGUARD]=1;
			creaturearray[CREATURE_SCIENTIST_LABTECH]=1;
			creaturearray[CREATURE_CORPORATE_MANAGER]=1;
			creaturearray[CREATURE_WORKER_JANITOR]=1;
			if(law[LAW_LABOR]<2)creaturearray[CREATURE_WORKER_FACTORY_NONUNION]=1;
			creaturearray[CREATURE_WORKER_SECRETARY]=1;
			if(law[LAW_LABOR]>=0)creaturearray[CREATURE_WORKER_FACTORY_UNION]=1;
			creaturearray[CREATURE_TEENAGER]=3;
			creaturearray[CREATURE_COP]=1;
			if(law[LAW_DEATHPENALTY]==-2&&
			   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_DEATHSQUAD]=1;
			if(law[LAW_POLICEBEHAVIOR]<=-1)creaturearray[CREATURE_GANGUNIT]=1;
			creaturearray[CREATURE_LAWYER]=1;
			creaturearray[CREATURE_SEWERWORKER]=1;
			creaturearray[CREATURE_COLLEGESTUDENT]=1;
			creaturearray[CREATURE_MUSICIAN]=1;
			creaturearray[CREATURE_MATHEMATICIAN]=1;
			creaturearray[CREATURE_TEACHER]=1;
			creaturearray[CREATURE_PRIEST]=1;
			creaturearray[CREATURE_ENGINEER]=1;
			creaturearray[CREATURE_FASTFOODWORKER]=1;
			creaturearray[CREATURE_TELEMARKETER]=1;
			creaturearray[CREATURE_OFFICEWORKER]=1;
			creaturearray[CREATURE_FOOTBALLCOACH]=1;
			creaturearray[CREATURE_MAILMAN]=1;
			creaturearray[CREATURE_GARBAGEMAN]=1;
			creaturearray[CREATURE_PLUMBER]=1;
			creaturearray[CREATURE_CHEF]=1;
			creaturearray[CREATURE_CONSTRUCTIONWORKER]=1;
			creaturearray[CREATURE_AMATEURMAGICIAN]=1;
			creaturearray[CREATURE_SOLDIER]=1;
			if(law[LAW_DEATHPENALTY]==-2&&
			   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]=1;
			else creaturearray[CREATURE_PRISONGUARD]=1;
			creaturearray[CREATURE_HIPPIE]=1;
			creaturearray[CREATURE_CRITIC_ART]=1;
			creaturearray[CREATURE_CRITIC_MUSIC]=1;
			creaturearray[CREATURE_AUTHOR]=1;
			creaturearray[CREATURE_JOURNALIST]=1;
			creaturearray[CREATURE_TAXIDRIVER]=1;
			creaturearray[CREATURE_PROGRAMMER]=1;
			creaturearray[CREATURE_RETIREE]=1;
			creaturearray[CREATURE_PAINTER]=1;
			creaturearray[CREATURE_SCULPTOR]=1;
			creaturearray[CREATURE_DANCER]=1;
			creaturearray[CREATURE_PHOTOGRAPHER]=1;
			creaturearray[CREATURE_CAMERAMAN]=1;
			creaturearray[CREATURE_HAIRSTYLIST]=1;
			creaturearray[CREATURE_CLERK]=1;
			creaturearray[CREATURE_THIEF]=1;
			creaturearray[CREATURE_ACTOR]=1;
			creaturearray[CREATURE_YOGAINSTRUCTOR]=1;
			creaturearray[CREATURE_ATHLETE]=1;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
			{
			if(sec)creaturearray[CREATURE_SECURITYGUARD]=100;
			else creaturearray[CREATURE_SECURITYGUARD]=10;
			creaturearray[CREATURE_SCIENTIST_EMINENT]=1;
			creaturearray[CREATURE_CORPORATE_MANAGER]=5;
			creaturearray[CREATURE_WORKER_JANITOR]=5;
			creaturearray[CREATURE_WORKER_SECRETARY]=1;
			creaturearray[CREATURE_TEENAGER]=3;
			creaturearray[CREATURE_JUDGE_LIBERAL]=1;
			creaturearray[CREATURE_JUDGE_CONSERVATIVE]=1;
			creaturearray[CREATURE_RADIOPERSONALITY]=1;
			creaturearray[CREATURE_NEWSANCHOR]=1;
			creaturearray[CREATURE_LAWYER]=5;
			creaturearray[CREATURE_COLLEGESTUDENT]=1;
			creaturearray[CREATURE_MUSICIAN]=1;
			creaturearray[CREATURE_PROSTITUTE]=3;
			creaturearray[CREATURE_MAILMAN]=1;
			creaturearray[CREATURE_CRITIC_ART]=1;
			creaturearray[CREATURE_CRITIC_MUSIC]=1;
			creaturearray[CREATURE_AUTHOR]=1;
			creaturearray[CREATURE_JOURNALIST]=1;
			creaturearray[CREATURE_SOCIALITE]=2;
			creaturearray[CREATURE_PAINTER]=1;
			creaturearray[CREATURE_SCULPTOR]=1;
			creaturearray[CREATURE_DANCER]=1;
			creaturearray[CREATURE_PHOTOGRAPHER]=1;
			creaturearray[CREATURE_FASHIONDESIGNER]=1;
			creaturearray[CREATURE_ACTOR]=1;
			creaturearray[CREATURE_ATHLETE]=1;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_LABORATORY_COSMETICS:
			{
			if(sec)creaturearray[CREATURE_SECURITYGUARD]=100;
			else creaturearray[CREATURE_SECURITYGUARD]=10;
			creaturearray[CREATURE_SCIENTIST_LABTECH]=10;
			creaturearray[CREATURE_SCIENTIST_EMINENT]=1;
			creaturearray[CREATURE_CORPORATE_MANAGER]=1;
			creaturearray[CREATURE_WORKER_JANITOR]=10;
			creaturearray[CREATURE_WORKER_SECRETARY]=10;
			creaturearray[CREATURE_OFFICEWORKER]=10;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_INDUSTRY_NUCLEAR:
			{
			if(sec)creaturearray[CREATURE_SECURITYGUARD]=100;
			else creaturearray[CREATURE_SECURITYGUARD]=10;
			creaturearray[CREATURE_SCIENTIST_LABTECH]=10;
			creaturearray[CREATURE_SCIENTIST_EMINENT]=1;
			creaturearray[CREATURE_CORPORATE_MANAGER]=1;
			creaturearray[CREATURE_WORKER_JANITOR]=10;
			creaturearray[CREATURE_WORKER_SECRETARY]=10;
			creaturearray[CREATURE_OFFICEWORKER]=10;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_LABORATORY_GENETIC:
			{
			if(sec)creaturearray[CREATURE_SECURITYGUARD]=10000;
			else creaturearray[CREATURE_SECURITYGUARD]=1000;
			creaturearray[CREATURE_SCIENTIST_LABTECH]=1000;
			creaturearray[CREATURE_SCIENTIST_EMINENT]=100;
			creaturearray[CREATURE_CORPORATE_MANAGER]=100;
			creaturearray[CREATURE_WORKER_JANITOR]=1000;
			creaturearray[CREATURE_WORKER_SECRETARY]=1000;
			creaturearray[CREATURE_OFFICEWORKER]=1000;
			creaturearray[CREATURE_GENETIC]=1;
			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_GOVERNMENT_POLICESTATION:
			{
			creaturearray[CREATURE_SCIENTIST_LABTECH]=1;
			creaturearray[CREATURE_CORPORATE_MANAGER]=1;
			creaturearray[CREATURE_WORKER_JANITOR]=50;
			if(law[LAW_LABOR]<2)creaturearray[CREATURE_WORKER_FACTORY_NONUNION]=1;
			creaturearray[CREATURE_WORKER_SECRETARY]=1;
			if(law[LAW_LABOR]>=0)creaturearray[CREATURE_WORKER_FACTORY_UNION]=1;
			creaturearray[CREATURE_TEENAGER]=5;
			if(sec)creaturearray[CREATURE_COP]=2000;
			else creaturearray[CREATURE_COP]=1000;
			if(law[LAW_DEATHPENALTY]==-2&&
			   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_DEATHSQUAD]=400;
			if(law[LAW_POLICEBEHAVIOR]<=-1)creaturearray[CREATURE_GANGUNIT]=400;
			creaturearray[CREATURE_JUDGE_LIBERAL]=1;
			creaturearray[CREATURE_JUDGE_CONSERVATIVE]=1;
			creaturearray[CREATURE_AGENT]=1;
			creaturearray[CREATURE_RADIOPERSONALITY]=1;
			creaturearray[CREATURE_NEWSANCHOR]=1;
			creaturearray[CREATURE_LAWYER]=1;
			creaturearray[CREATURE_SEWERWORKER]=1;
			creaturearray[CREATURE_COLLEGESTUDENT]=1;
			creaturearray[CREATURE_MUSICIAN]=1;
			creaturearray[CREATURE_MATHEMATICIAN]=1;
			creaturearray[CREATURE_TEACHER]=1;
			creaturearray[CREATURE_HSDROPOUT]=5;
			creaturearray[CREATURE_BUM]=5;
			if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
			if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=2;
			if(law[LAW_POLLUTION]==-2&&
				law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=5;
			creaturearray[CREATURE_GANGMEMBER]=5;
			creaturearray[CREATURE_CRACKHEAD]=5;
			creaturearray[CREATURE_PRIEST]=5;
			creaturearray[CREATURE_ENGINEER]=1;
			creaturearray[CREATURE_FASTFOODWORKER]=1;
			creaturearray[CREATURE_TELEMARKETER]=1;
			creaturearray[CREATURE_OFFICEWORKER]=1;
			creaturearray[CREATURE_FOOTBALLCOACH]=1;
			creaturearray[CREATURE_PROSTITUTE]=5;
			creaturearray[CREATURE_MAILMAN]=1;
			creaturearray[CREATURE_GARBAGEMAN]=1;
			creaturearray[CREATURE_PLUMBER]=1;
			creaturearray[CREATURE_CHEF]=1;
			creaturearray[CREATURE_CONSTRUCTIONWORKER]=1;
			creaturearray[CREATURE_AMATEURMAGICIAN]=1;
			creaturearray[CREATURE_HICK]=1;
			creaturearray[CREATURE_SOLDIER]=1;
			if(law[LAW_DEATHPENALTY]==-2&&
			   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]=1;
			else creaturearray[CREATURE_PRISONGUARD]=1;
			creaturearray[CREATURE_HIPPIE]=1;
			creaturearray[CREATURE_CRITIC_ART]=1;
			creaturearray[CREATURE_CRITIC_MUSIC]=1;
			creaturearray[CREATURE_AUTHOR]=1;
			creaturearray[CREATURE_JOURNALIST]=1;
			creaturearray[CREATURE_SOCIALITE]=1;
			creaturearray[CREATURE_BIKER]=5;
			creaturearray[CREATURE_TRUCKER]=1;
			creaturearray[CREATURE_TAXIDRIVER]=1;
			creaturearray[CREATURE_PROGRAMMER]=1;
			creaturearray[CREATURE_NUN]=1;
			creaturearray[CREATURE_RETIREE]=1;
			creaturearray[CREATURE_PAINTER]=1;
			creaturearray[CREATURE_SCULPTOR]=1;
			creaturearray[CREATURE_DANCER]=1;
			creaturearray[CREATURE_PHOTOGRAPHER]=1;
			creaturearray[CREATURE_CAMERAMAN]=1;
			creaturearray[CREATURE_HAIRSTYLIST]=1;
			creaturearray[CREATURE_FASHIONDESIGNER]=1;
			creaturearray[CREATURE_CLERK]=1;
			creaturearray[CREATURE_THIEF]=5;
			creaturearray[CREATURE_ACTOR]=1;
			creaturearray[CREATURE_YOGAINSTRUCTOR]=1;
			creaturearray[CREATURE_ATHLETE]=1;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_GOVERNMENT_COURTHOUSE:
			{
			if(sec)creaturearray[CREATURE_SECURITYGUARD]=2000;
			else creaturearray[CREATURE_SECURITYGUARD]=200;
			creaturearray[CREATURE_SCIENTIST_LABTECH]=1;
			creaturearray[CREATURE_SCIENTIST_EMINENT]=1;
			creaturearray[CREATURE_CORPORATE_MANAGER]=1;
			creaturearray[CREATURE_CORPORATE_CEO]=1;
			creaturearray[CREATURE_WORKER_JANITOR]=50;
			if(law[LAW_LABOR]<2)creaturearray[CREATURE_WORKER_FACTORY_NONUNION]=1;
			creaturearray[CREATURE_WORKER_SECRETARY]=50;
			if(law[LAW_LABOR]>=0)creaturearray[CREATURE_WORKER_FACTORY_UNION]=1;
			creaturearray[CREATURE_TEENAGER]=1;
			creaturearray[CREATURE_COP]=200;
			if(law[LAW_DEATHPENALTY]==-2&&
			   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_DEATHSQUAD]=80;
			if(law[LAW_POLICEBEHAVIOR]<=-1)creaturearray[CREATURE_GANGUNIT]=80;
			creaturearray[CREATURE_JUDGE_LIBERAL]=20;
			creaturearray[CREATURE_JUDGE_CONSERVATIVE]=20;
			creaturearray[CREATURE_AGENT]=1;
			creaturearray[CREATURE_RADIOPERSONALITY]=1;
			creaturearray[CREATURE_NEWSANCHOR]=1;
			creaturearray[CREATURE_LAWYER]=200;
			creaturearray[CREATURE_SEWERWORKER]=1;
			creaturearray[CREATURE_COLLEGESTUDENT]=1;
			creaturearray[CREATURE_MUSICIAN]=1;
			creaturearray[CREATURE_MATHEMATICIAN]=1;
			creaturearray[CREATURE_TEACHER]=1;
			creaturearray[CREATURE_HSDROPOUT]=1;
			creaturearray[CREATURE_BUM]=1;
			if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=1;
			if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=1;
			if(law[LAW_POLLUTION]==-2&&
				law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
			creaturearray[CREATURE_GANGMEMBER]=1;
			creaturearray[CREATURE_CRACKHEAD]=1;
			creaturearray[CREATURE_PRIEST]=1;
			creaturearray[CREATURE_ENGINEER]=1;
			creaturearray[CREATURE_FASTFOODWORKER]=1;
			creaturearray[CREATURE_TELEMARKETER]=1;
			creaturearray[CREATURE_OFFICEWORKER]=50;
			creaturearray[CREATURE_FOOTBALLCOACH]=1;
			creaturearray[CREATURE_PROSTITUTE]=1;
			creaturearray[CREATURE_MAILMAN]=1;
			creaturearray[CREATURE_GARBAGEMAN]=1;
			creaturearray[CREATURE_PLUMBER]=1;
			creaturearray[CREATURE_CHEF]=1;
			creaturearray[CREATURE_CONSTRUCTIONWORKER]=1;
			creaturearray[CREATURE_AMATEURMAGICIAN]=1;
			creaturearray[CREATURE_HICK]=1;
			creaturearray[CREATURE_SOLDIER]=1;
			if(law[LAW_DEATHPENALTY]==-2&&
			   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]=1;
			else creaturearray[CREATURE_PRISONGUARD]=1;
			creaturearray[CREATURE_HIPPIE]=1;
			creaturearray[CREATURE_CRITIC_ART]=1;
			creaturearray[CREATURE_CRITIC_MUSIC]=1;
			creaturearray[CREATURE_AUTHOR]=1;
			creaturearray[CREATURE_JOURNALIST]=1;
			creaturearray[CREATURE_SOCIALITE]=1;
			creaturearray[CREATURE_BIKER]=1;
			creaturearray[CREATURE_TRUCKER]=1;
			creaturearray[CREATURE_TAXIDRIVER]=1;
			creaturearray[CREATURE_PROGRAMMER]=1;
			creaturearray[CREATURE_NUN]=1;
			creaturearray[CREATURE_RETIREE]=1;
			creaturearray[CREATURE_PAINTER]=1;
			creaturearray[CREATURE_SCULPTOR]=1;
			creaturearray[CREATURE_DANCER]=1;
			creaturearray[CREATURE_PHOTOGRAPHER]=1;
			creaturearray[CREATURE_CAMERAMAN]=1;
			creaturearray[CREATURE_HAIRSTYLIST]=1;
			creaturearray[CREATURE_FASHIONDESIGNER]=1;
			creaturearray[CREATURE_CLERK]=1;
			creaturearray[CREATURE_THIEF]=1;
			creaturearray[CREATURE_ACTOR]=1;
			creaturearray[CREATURE_YOGAINSTRUCTOR]=1;
			creaturearray[CREATURE_ATHLETE]=1;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_GOVERNMENT_PRISON:
			{
			creaturearray[CREATURE_PRISONER]=8;
			if(sec)
				{
				if(law[LAW_DEATHPENALTY]==-2&&
				   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]=3;
				else creaturearray[CREATURE_PRISONGUARD]=3;
				}
			else
				{
				if(law[LAW_DEATHPENALTY]==-2&&
				   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]=2;
				else creaturearray[CREATURE_PRISONGUARD]=2;
				}
			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_GOVERNMENT_INTELLIGENCEHQ:
			{
			if(sec)creaturearray[CREATURE_SECURITYGUARD]=1000;
			else creaturearray[CREATURE_SECURITYGUARD]=100;
			creaturearray[CREATURE_WORKER_JANITOR]=50;
			creaturearray[CREATURE_WORKER_SECRETARY]=50;
			creaturearray[CREATURE_AGENT]=100;
			creaturearray[CREATURE_GUARDDOG]=50;
			creaturearray[CREATURE_MATHEMATICIAN]=5;
			creaturearray[CREATURE_PROGRAMMER]=5;

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_INDUSTRY_SWEATSHOP:
			{
			if(sec)creaturearray[CREATURE_SECURITYGUARD]=2000;
			else creaturearray[CREATURE_SECURITYGUARD]=200;
			creaturearray[CREATURE_CORPORATE_MANAGER]=5;
			creaturearray[CREATURE_WORKER_SWEATSHOP]=800;
			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_INDUSTRY_POLLUTER:
			{
			if(sec)creaturearray[CREATURE_SECURITYGUARD]=1000;
			else creaturearray[CREATURE_SECURITYGUARD]=100;
			creaturearray[CREATURE_CORPORATE_MANAGER]=5;
			creaturearray[CREATURE_WORKER_JANITOR]=50;
			creaturearray[CREATURE_WORKER_SECRETARY]=100;

			if(law[LAW_LABOR]==-2)
				{
				creaturearray[CREATURE_WORKER_FACTORY_NONUNION]=200;
				creaturearray[CREATURE_WORKER_FACTORY_CHILD]=1400;
				}
			else if(law[LAW_LABOR]==-1)
				{
				creaturearray[CREATURE_WORKER_FACTORY_NONUNION]=1600;
				creaturearray[CREATURE_WORKER_FACTORY_CHILD]=5;
				}
			else if(law[LAW_LABOR]==0)
				{
				creaturearray[CREATURE_WORKER_FACTORY_NONUNION]=800;
				creaturearray[CREATURE_WORKER_FACTORY_UNION]=800;
				}
			else if(law[LAW_LABOR]==1)
				{
				creaturearray[CREATURE_WORKER_FACTORY_NONUNION]=500;
				creaturearray[CREATURE_WORKER_FACTORY_UNION]=1100;
				}
			else creaturearray[CREATURE_WORKER_FACTORY_UNION]=1600;
			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_CORPORATE_HEADQUARTERS:
			{
			if(sec)creaturearray[CREATURE_SECURITYGUARD]=4000;
			else creaturearray[CREATURE_SECURITYGUARD]=400;
			creaturearray[CREATURE_CORPORATE_MANAGER]=200;
			creaturearray[CREATURE_CORPORATE_CEO]=1;
			creaturearray[CREATURE_WORKER_JANITOR]=200;
			creaturearray[CREATURE_WORKER_SECRETARY]=400;
			creaturearray[CREATURE_JUDGE_CONSERVATIVE]=1;
			creaturearray[CREATURE_LAWYER]=200;
			creaturearray[CREATURE_PRIEST]=1;
			creaturearray[CREATURE_OFFICEWORKER]=800;
			creaturearray[CREATURE_PROSTITUTE]=1;
			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_CORPORATE_HOUSE:
			{
			if(sec)creaturearray[CREATURE_SECURITYGUARD]=10000;
			else creaturearray[CREATURE_SECURITYGUARD]=1000;
			creaturearray[CREATURE_CORPORATE_CEO]=10;
			creaturearray[CREATURE_WORKER_SERVANT]=3000;
			creaturearray[CREATURE_WORKER_SECRETARY]=10;
			creaturearray[CREATURE_TEENAGER]=10;
			creaturearray[CREATURE_GENETIC]=1;
			creaturearray[CREATURE_GUARDDOG]=1000;
			creaturearray[CREATURE_LAWYER]=10;
			creaturearray[CREATURE_PRIEST]=1;
			creaturearray[CREATURE_PROSTITUTE]=10;
			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_MEDIA_AMRADIO:
			{
			if(sec)creaturearray[CREATURE_SECURITYGUARD]=2000;
			else creaturearray[CREATURE_SECURITYGUARD]=200;
			creaturearray[CREATURE_CORPORATE_MANAGER]=50;
			creaturearray[CREATURE_WORKER_JANITOR]=200;
			creaturearray[CREATURE_WORKER_SECRETARY]=200;
			creaturearray[CREATURE_RADIOPERSONALITY]=20;
			creaturearray[CREATURE_ENGINEER]=400;
			creaturearray[CREATURE_OFFICEWORKER]=400;
			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		case SITE_MEDIA_CABLENEWS:
			{
			if(sec)creaturearray[CREATURE_SECURITYGUARD]=2000;
			else creaturearray[CREATURE_SECURITYGUARD]=200;
			creaturearray[CREATURE_CORPORATE_MANAGER]=50;
			creaturearray[CREATURE_WORKER_JANITOR]=200;
			creaturearray[CREATURE_WORKER_SECRETARY]=200;
			creaturearray[CREATURE_NEWSANCHOR]=20;
			creaturearray[CREATURE_ENGINEER]=400;
			creaturearray[CREATURE_OFFICEWORKER]=400;
			creaturearray[CREATURE_PHOTOGRAPHER]=50;
			creaturearray[CREATURE_CAMERAMAN]=50;

	#ifdef SHOWWAIT
		creaturearray[CREATURE_NEWSANCHOR]=20000;
	#endif

			for(int n=0;n<LCSrandom(6)+1;n++)
				{
				makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
				encslot++;
				}
			break;
			}
		}
}

void printchaseencounter(void)
{
	if(chaseseq.enemycar.size()>0)
		{
		for(int i=19;i<=24;i++)
			{
			move(i,1);
			addstr("                                                                              ");
			}

		int carsy[4]={20,20,20,20};

		char str[80];
		for(int v=0;v<chaseseq.enemycar.size();v++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(19,v*20+1);
			getcarfull(str,*chaseseq.enemycar[v],1);
			addstr(str);
			}

		for(int e=0;e<ENCMAX;e++)
			{
			if(encounter[e].exists)
				{
				for(int v=0;v<chaseseq.enemycar.size();v++)
					{
					if(chaseseq.enemycar[v]->id==encounter[e].carid)
						{
						set_color(COLOR_RED,COLOR_BLACK,1);
						move(carsy[v],v*20+1);
						addstr(encounter[e].name);
						if(encounter[e].is_driver)addstr("-D");
						carsy[v]++;
						}
					}
				}
			}
		}
	else printencounter();
}

void printencounter(void)
{
	for(int i=19;i<=24;i++)
		{
		move(i,1);
		addstr("                                                        ");
		}

	int px=1,py=19;
	for(int e=0;e<ENCMAX;e++)
		{
		if(encounter[e].exists)
			{
			if(!encounter[e].alive)set_color(COLOR_BLACK,COLOR_BLACK,1);
			else if(encounter[e].align==0)set_color(COLOR_WHITE,COLOR_BLACK,1);
			else if(encounter[e].align==1)set_color(COLOR_GREEN,COLOR_BLACK,1);
			else set_color(COLOR_RED,COLOR_BLACK,1);
			move(py,px);
			addstr(encounter[e].name);
			}

		px+=18;
		if(px>37)
			{
			px=1;
			py++;
			}
		}
}

void makecreature(creaturest &cr,short type)
{
 int a = 0;
	cr.creatureinit();

	cr.exists=1;
	cr.squadid=-1;
	cr.type=type;
	getrecruitcreature(cr.name,type);
	cr.armor.type=ARMOR_CLOTHES;
	cr.money=LCSrandom(21)+20;
	cr.align=LCSrandom(3)-1;
	cr.worklocation=cursite;
	verifyworklocation(cr);

	int randomskills=LCSrandom(5)+5;

	int redistatts=0;
	int attcap[ATTNUM];
	for(a=0;a<ATTNUM;a++)attcap[a]=10;

	int sk;

	switch(type)
		{
		case CREATURE_SECURITYGUARD:
			cr.weapon.type=WEAPON_REVOLVER_22;
			cr.weapon.ammo=6;
			cr.clip[CLIP_22]=3;
			cr.armor.type=ARMOR_SECURITYUNIFORM;
			sk=LCSrandom(2)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
			cr.align=LCSrandom(2)-1;
			break;
		case CREATURE_SCIENTIST_LABTECH:
			if(!LCSrandom(2))cr.weapon.type=WEAPON_SYRINGE;
			cr.armor.type=ARMOR_LABCOAT;
			cr.align=-1;
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=28;
			cr.att[ATTRIBUTE_INTELLIGENCE]=5;
			sk=LCSrandom(3);cr.skill[SKILL_COMPUTERS]=sk;randomskills-=sk;
			break;
		case CREATURE_JUDGE_CONSERVATIVE:
			strcpy(cr.name,"Hangin' Judge");
			if(!LCSrandom(2))cr.weapon.type=WEAPON_GAVEL;
			cr.armor.type=ARMOR_BLACKROBE;
			cr.money=LCSrandom(41)+20;
			cr.align=-1;

			sk=LCSrandom(6)+5;cr.skill[SKILL_LAW]=sk;randomskills-=sk;
			sk=LCSrandom(3)+1;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=19;
			cr.att[ATTRIBUTE_INTELLIGENCE]=5;
			cr.att[ATTRIBUTE_WISDOM]=10;
			cr.att[ATTRIBUTE_HEART]=1;attcap[ATTRIBUTE_HEART]=1;
			break;
		case CREATURE_JUDGE_LIBERAL:
			strcpy(cr.name,"Liberal Judge");
			if(!LCSrandom(2))cr.weapon.type=WEAPON_GAVEL;
			cr.armor.type=ARMOR_BLACKROBE;
			cr.money=LCSrandom(41)+20;
			cr.align=1;

			sk=LCSrandom(6)+5;cr.skill[SKILL_LAW]=sk;randomskills-=sk;
			sk=LCSrandom(3)+1;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=19;
			cr.att[ATTRIBUTE_INTELLIGENCE]=5;
			cr.att[ATTRIBUTE_HEART]=10;
			break;
		case CREATURE_SCIENTIST_EMINENT:
			if(!LCSrandom(2))cr.weapon.type=WEAPON_SYRINGE;
			cr.armor.type=ARMOR_LABCOAT;
			cr.money=LCSrandom(41)+20;
			cr.align=-1;

			sk=LCSrandom(3)+1;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=23;
			cr.att[ATTRIBUTE_INTELLIGENCE]=10;
			break;
		case CREATURE_CORPORATE_MANAGER:
			cr.armor.type=ARMOR_CHEAPSUIT;
			cr.money=LCSrandom(41)+40;
			cr.align=-1;

			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=28;
			cr.att[ATTRIBUTE_WISDOM]=5;
			break;
		case CREATURE_CORPORATE_CEO:
			cr.armor.type=ARMOR_EXPENSIVESUIT;
			cr.money=LCSrandom(121)+120;
			cr.align=-1;

			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=21;
			cr.att[ATTRIBUTE_WISDOM]=12;
			break;
		case CREATURE_WORKER_SERVANT:
			cr.money=0;
			cr.align=1;
			break;
		case CREATURE_WORKER_JANITOR:
			break;
		case CREATURE_WORKER_SWEATSHOP:
			cr.armor.type=ARMOR_NONE;
			cr.align=1;
			cr.juice=-20;
			cr.flag|=CREATUREFLAG_ILLEGALALIEN;
			sk=LCSrandom(6)+5;cr.skill[SKILL_GARMENTMAKING]=sk;randomskills-=sk;
			break;
		case CREATURE_WORKER_FACTORY_NONUNION:
			strcpy(cr.name,"Nonunion Worker");
			cr.weapon.type=WEAPON_CHAIN;
			cr.armor.type=ARMOR_WORKCLOTHES;
			cr.align=LCSrandom(2)-1;

			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=28;
			cr.att[ATTRIBUTE_STRENGTH]=5;
			break;
		case CREATURE_WORKER_SECRETARY:
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_LANDLORD:
			strcpy(cr.name,"Landlord");
			cr.money=LCSrandom(121)+120;
			cr.align=0;
			break;
		case CREATURE_TEENAGER:
			cr.align=LCSrandom(3)-1;
			randomskills>>=1;
			break;
		case CREATURE_LAWYER:
			strcpy(cr.name,"Lawyer");
			cr.armor.type=ARMOR_CHEAPSUIT;
			cr.money=LCSrandom(51)+50;
			cr.align=LCSrandom(3)-1;

			sk=LCSrandom(4)+4;cr.skill[SKILL_LAW]=sk;randomskills-=sk;
			break;
		case CREATURE_WORKER_FACTORY_UNION:
			strcpy(cr.name,"Union Worker");
			cr.weapon.type=WEAPON_CHAIN;
			cr.armor.type=ARMOR_WORKCLOTHES;
			cr.align=1;

			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=28;
			cr.att[ATTRIBUTE_STRENGTH]=5;
			break;
		case CREATURE_TANK:
			strcpy(cr.name,"Tank");
			cr.animalgloss=ANIMALGLOSS_TANK;
			cr.armor.type=ARMOR_NONE;
			cr.specialattack=ATTACK_CANNON;
			cr.align=-1;
			break;
		case CREATURE_MERC:
			strcpy(cr.name,"Mercenary");
			if(!LCSrandom(2))cr.weapon.type=WEAPON_SEMIRIFLE_AK47;
			else cr.weapon.type=WEAPON_SEMIRIFLE_M16;
			cr.clip[CLIP_ASSAULT]=6;
			cr.weapon.ammo=30;
			cr.armor.type=ARMOR_MILITARY;
			cr.money=0;
			cr.align=-1;

			sk=LCSrandom(4)+1;cr.skill[SKILL_ASSAULTRIFLE]=sk;randomskills-=sk;
			sk=LCSrandom(3);cr.skill[SKILL_SECURITY]=sk;randomskills-=sk;
			sk=LCSrandom(3);cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
			sk=LCSrandom(3);cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
			sk=LCSrandom(2);cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			break;
		case CREATURE_HICK:
			switch(LCSrandom(5))
				{
				case 0:strcpy(cr.name,"Country Boy");break;
				case 1:strcpy(cr.name,"Hick");break;
				case 2:strcpy(cr.name,"Redneck");break;
				case 3:strcpy(cr.name,"Rube");break;
				case 4:strcpy(cr.name,"Yokel");break;
				}
			if(!LCSrandom(2))cr.weapon.type=WEAPON_TORCH;
			else cr.weapon.type=WEAPON_PITCHFORK;

			if(!LCSrandom(2))cr.armor.type=ARMOR_OVERALLS;
			else cr.armor.type=ARMOR_WIFEBEATER;

			cr.money=LCSrandom(6)+6;
			cr.align=-1;
			break;
		case CREATURE_SOLDIER:
			strcpy(cr.name,"Soldier");
			cr.weapon.type=WEAPON_SEMIRIFLE_M16;
			cr.clip[CLIP_ASSAULT]=6;
			cr.weapon.ammo=30;
			cr.armor.type=ARMOR_MILITARY;
			cr.money=0;
			cr.align=-1;

			sk=LCSrandom(4)+1;cr.skill[SKILL_ASSAULTRIFLE]=sk;randomskills-=sk;
			sk=LCSrandom(3);cr.skill[SKILL_SECURITY]=sk;randomskills-=sk;
			sk=LCSrandom(3);cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
			sk=LCSrandom(3);cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
			sk=LCSrandom(2);cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			break;
		case CREATURE_COP:
			if(!LCSrandom(3))
				{
				cr.weapon.type=WEAPON_SEMIPISTOL_9MM;
				cr.clip[CLIP_9]=3;
				cr.weapon.ammo=15;
				}
			else if(!LCSrandom(2))
				{
				cr.weapon.type=WEAPON_SHOTGUN;
				cr.clip[CLIP_BUCKSHOT]=6;
				cr.weapon.ammo=2;
				}
			else cr.weapon.type=WEAPON_NIGHTSTICK;
			cr.armor.type=ARMOR_POLICEUNIFORM;
			cr.money=LCSrandom(21)+20;
			cr.align=-1;

			sk=LCSrandom(4)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
			sk=LCSrandom(3)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_CLUB]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			break;
		case CREATURE_DEATHSQUAD:
			cr.weapon.type=WEAPON_SEMIRIFLE_M16;
			cr.clip[CLIP_ASSAULT]=6;
			cr.weapon.ammo=30;
			cr.armor.type=ARMOR_POLICEUNIFORM;
			cr.money=LCSrandom(21)+20;
			cr.align=-1;

			sk=LCSrandom(4)+1;cr.skill[SKILL_ASSAULTRIFLE]=sk;randomskills-=sk;
			sk=LCSrandom(3)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
			sk=LCSrandom(3)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_CLUB]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			break;
		case CREATURE_GANGUNIT:
			if(!LCSrandom(3))
				{
				cr.weapon.type=WEAPON_SEMIPISTOL_9MM;
				cr.clip[CLIP_9]=3;
				cr.weapon.ammo=15;
				}
			else if(!LCSrandom(2))
				{
				cr.weapon.type=WEAPON_SHOTGUN;
				cr.clip[CLIP_BUCKSHOT]=6;
				cr.weapon.ammo=2;
				}
			else cr.weapon.type=WEAPON_NIGHTSTICK;
			cr.armor.type=ARMOR_POLICEUNIFORM;
			cr.money=LCSrandom(21)+20;
			cr.align=-1;

			sk=LCSrandom(4)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
			sk=LCSrandom(3)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_CLUB]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			break;
		case CREATURE_PRISONGUARD:
			strcpy(cr.name,"Prison Guard");
			if(!LCSrandom(3))
				{
				cr.weapon.type=WEAPON_SHOTGUN;
				cr.clip[CLIP_BUCKSHOT]=6;
				cr.weapon.ammo=2;
				}
			else cr.weapon.type=WEAPON_NIGHTSTICK;
			cr.armor.type=ARMOR_PRISONGUARD;
			cr.money=LCSrandom(21)+20;
			cr.align=-1;

			sk=LCSrandom(4)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
			sk=LCSrandom(3)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_CLUB]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
			sk=LCSrandom(2);cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			break;
		case CREATURE_EDUCATOR:
			strcpy(cr.name,"Educator");
			if(!LCSrandom(3))
				{
				cr.weapon.type=WEAPON_SEMIPISTOL_9MM;
				cr.clip[CLIP_9]=3;
				cr.weapon.ammo=15;
				}
			else cr.weapon.type=WEAPON_SYRINGE;
			cr.armor.type=ARMOR_LABCOAT;
			cr.money=LCSrandom(21)+20;
			cr.align=-1;

			sk=LCSrandom(4)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
			sk=LCSrandom(3)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_CLUB]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
			sk=LCSrandom(2);cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			break;
		case CREATURE_AGENT:
			strcpy(cr.name,"Agent");
			switch(LCSrandom(7))
				{
				case 0:
					cr.weapon.type=WEAPON_REVOLVER_22;
					cr.clip[CLIP_22]=3;
					cr.weapon.ammo=6;
					break;
				case 1:
					cr.weapon.type=WEAPON_REVOLVER_44;
					cr.clip[CLIP_44]=3;
					cr.weapon.ammo=6;
					break;
				case 2:
					cr.weapon.type=WEAPON_SEMIPISTOL_9MM;
					cr.clip[CLIP_9]=3;
					cr.weapon.ammo=15;
					break;
				case 3:
					cr.weapon.type=WEAPON_SEMIPISTOL_45;
					cr.clip[CLIP_45]=3;
					cr.weapon.ammo=15;
					break;
				case 4:
					cr.weapon.type=WEAPON_SEMIRIFLE_M16;
					cr.clip[CLIP_ASSAULT]=3;
					cr.weapon.ammo=30;
					break;
				case 5:
					cr.weapon.type=WEAPON_SEMIRIFLE_AK47;
					cr.clip[CLIP_ASSAULT]=3;
					cr.weapon.ammo=30;
					break;
				case 6:
					cr.weapon.type=WEAPON_SHOTGUN;
					cr.clip[CLIP_BUCKSHOT]=6;
					cr.weapon.ammo=2;
					break;
				}
			cr.armor.type=ARMOR_BLACKSUIT;
			cr.align=-1;

			sk=LCSrandom(8)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
			sk=LCSrandom(8)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
			sk=LCSrandom(8)+1;cr.skill[SKILL_ASSAULTRIFLE]=sk;randomskills-=sk;
			sk=LCSrandom(8)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
			sk=LCSrandom(8)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=20;
			cr.att[ATTRIBUTE_STRENGTH]=5;
			cr.att[ATTRIBUTE_AGILITY]=5;
			cr.att[ATTRIBUTE_HEALTH]=5;
			break;
		case CREATURE_RADIOPERSONALITY:
			strcpy(cr.name,"Radio Personality");
			cr.armor.type=ARMOR_EXPENSIVESUIT;
			cr.money=LCSrandom(51)+50;
			cr.align=-1;

			sk=LCSrandom(8)+1;cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=19;
			cr.att[ATTRIBUTE_CHARISMA]=10;
			cr.att[ATTRIBUTE_WISDOM]=5;
			break;
		case CREATURE_NEWSANCHOR:
			strcpy(cr.name,"News Anchor");
			cr.armor.type=ARMOR_EXPENSIVESUIT;
			cr.money=LCSrandom(51)+50;

			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=19;
			cr.att[ATTRIBUTE_CHARISMA]=10;
			cr.att[ATTRIBUTE_WISDOM]=5;
			cr.align=-1;
			break;
		case CREATURE_GENETIC:
			switch(LCSrandom(10))
				{
				case 0:strcpy(cr.name,"Genetic Monster");break;
				case 1:
					{
					strcpy(cr.name,"Flaming Rabbit");
					cr.specialattack=ATTACK_FLAME;
					break;
					}
				case 2:strcpy(cr.name,"Genetic Nightmare");break;
				case 3:strcpy(cr.name,"Mad Cow");break;
				case 4:
					{
					strcpy(cr.name,"Giant Mosquito");
					cr.specialattack=ATTACK_SUCK;
					break;
					}
				case 5:strcpy(cr.name,"Six-legged Pig");break;
				case 6:strcpy(cr.name,"Purple Gorilla");break;
				case 7:strcpy(cr.name,"Warped Bear");break;
				case 8:strcpy(cr.name,"Writhing Mass");break;
				case 9:strcpy(cr.name,"Something Bad");break;
				}
			cr.animalgloss=ANIMALGLOSS_ANIMAL;
			cr.armor.type=ARMOR_NONE;
			cr.align=-1;
			break;
		case CREATURE_GUARDDOG:
			strcpy(cr.name,"Guard Dog");
			cr.animalgloss=ANIMALGLOSS_ANIMAL;
			cr.armor.type=ARMOR_NONE;
			cr.align=-1;
			break;
		case CREATURE_PRISONER:
			strcpy(cr.name,"Prisoner");
			if(!LCSrandom(2))cr.weapon.type=WEAPON_SHANK;
			cr.armor.type=ARMOR_PRISONER;
			cr.money=0;
			cr.align=LCSrandom(3)-1;

			sk=LCSrandom(5)+1;cr.skill[SKILL_SECURITY]=sk;randomskills-=sk;
			break;
		case CREATURE_JUROR:
			strcpy(cr.name,"Angry Juror");
			if(!LCSrandom(5))cr.weapon.type=WEAPON_KNIFE;
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=0;
			cr.align=-1;
			break;
		case CREATURE_WORKER_FACTORY_CHILD:
			strcpy(cr.name,"Child Worker");
			cr.weapon.type=WEAPON_CHAIN;
			cr.armor.type=ARMOR_WORKCLOTHES;
			cr.money=0;
			cr.align=LCSrandom(2);
			randomskills=1;
			break;
		case CREATURE_SEWERWORKER:
			strcpy(cr.name,"Sewer Worker");
			cr.armor.type=ARMOR_WORKCLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_COLLEGESTUDENT:
			strcpy(cr.name,"College Student");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_MUSICIAN:
			strcpy(cr.name,"Musician");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(11);
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_MATHEMATICIAN:
			strcpy(cr.name,"Mathematician");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;

			sk=LCSrandom(3);cr.skill[SKILL_COMPUTERS]=sk;randomskills-=sk;
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=26;
			cr.att[ATTRIBUTE_INTELLIGENCE]=7;
			break;
		case CREATURE_TEACHER:
			strcpy(cr.name,"Teacher");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_HSDROPOUT:
			strcpy(cr.name,"Highschool Dropout");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_BUM:
			strcpy(cr.name,"Transient");
			if(!LCSrandom(5))cr.weapon.type=WEAPON_KNIFE;
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(2);
			break;
		case CREATURE_MUTANT:
			strcpy(cr.name,"Mutant");
			if(!LCSrandom(5))cr.weapon.type=WEAPON_KNIFE;
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=1;
			for(a=0;a<ATTNUM;a++)
				{
				cr.att[a]=1;
				attcap[a]=50;
				}
			redistatts=LCSrandom(55);
			break;
		case CREATURE_GANGMEMBER:
			strcpy(cr.name,"Gang Member");

			if(!LCSrandom(20))
				{
				cr.weapon.type=WEAPON_SEMIRIFLE_AK47;
				cr.weapon.ammo=30;
				cr.clip[CLIP_ASSAULT]=2;
				}
			else if(!LCSrandom(16))
				{
				cr.weapon.type=WEAPON_REVOLVER_44;
				cr.weapon.ammo=6;
				cr.clip[CLIP_44]=3;
				}
			else if(!LCSrandom(15))
				{
				cr.weapon.type=WEAPON_SEMIPISTOL_45;
				cr.weapon.ammo=15;
				cr.clip[CLIP_45]=3;
				}
			else if(!LCSrandom(10))
				{
				cr.weapon.type=WEAPON_SHOTGUN;
				cr.weapon.ammo=2;
				cr.clip[CLIP_BUCKSHOT]=3;
				}
			else if(!LCSrandom(4))
				{
				cr.weapon.type=WEAPON_SEMIPISTOL_9MM;
				cr.weapon.ammo=15;
				cr.clip[CLIP_9]=3;
				}
			else if(!LCSrandom(2))
				{
				cr.weapon.type=WEAPON_REVOLVER_22;
				cr.weapon.ammo=6;
				cr.clip[CLIP_22]=3;
				}

			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(2);

			sk=LCSrandom(2)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_ASSAULTRIFLE]=sk;randomskills-=sk;
			sk=LCSrandom(2)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
			break;
		case CREATURE_CRACKHEAD:
			strcpy(cr.name,"Crack Head");
			if(!LCSrandom(5))cr.weapon.type=WEAPON_KNIFE;
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(2);

			//NOTE: DO NOT REDISTRIBUTE
			cr.att[ATTRIBUTE_INTELLIGENCE]=1;
			cr.att[ATTRIBUTE_HEALTH]>>=1;cr.att[ATTRIBUTE_HEALTH]++;
			break;
		case CREATURE_PRIEST:
			strcpy(cr.name,"Priest");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_ENGINEER:
			strcpy(cr.name,"Engineer");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;

			sk=LCSrandom(2)+1;cr.skill[SKILL_COMPUTERS]=sk;randomskills-=sk;
			break;
		case CREATURE_FASTFOODWORKER:
			strcpy(cr.name,"Fast Food Worker");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_TELEMARKETER:
			strcpy(cr.name,"Telemarketer");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_OFFICEWORKER:
			strcpy(cr.name,"Office Worker");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_FOOTBALLCOACH:
			strcpy(cr.name,"Football Coach");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;

			if(LCSrandom(2))
				{
				for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=20;
				cr.att[ATTRIBUTE_HEALTH]=5;
				cr.att[ATTRIBUTE_AGILITY]=5;
				cr.att[ATTRIBUTE_STRENGTH]=5;
				}
			break;
		case CREATURE_PROSTITUTE:
			strcpy(cr.name,"Prostitute");
			if(LCSrandom(2))cr.armor.type=ARMOR_CHEAPDRESS;
			else cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(2);
			break;
		case CREATURE_MAILMAN:
			strcpy(cr.name,"Mail Carrier");
			cr.armor.type=ARMOR_WORKCLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_GARBAGEMAN:
			strcpy(cr.name,"Garbage Collector");
			cr.armor.type=ARMOR_WORKCLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			sk=LCSrandom(2)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			break;
		case CREATURE_PLUMBER:
			strcpy(cr.name,"Plumber");
			cr.armor.type=ARMOR_WORKCLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_CHEF:
			strcpy(cr.name,"Chef");
			cr.armor.type=ARMOR_WORKCLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_CONSTRUCTIONWORKER:
			strcpy(cr.name,"Construction Worker");
			cr.armor.type=ARMOR_WORKCLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;

			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=20;
			cr.att[ATTRIBUTE_STRENGTH]=7;
			cr.att[ATTRIBUTE_HEALTH]=7;
			sk=LCSrandom(2)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			break;
		case CREATURE_AMATEURMAGICIAN:
			strcpy(cr.name,"Amateur Magician");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			break;
		case CREATURE_HIPPIE:
			strcpy(cr.name,"Hippie");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=1;

			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=32;
			cr.att[ATTRIBUTE_WISDOM]=1;attcap[ATTRIBUTE_WISDOM]=1;
			break;
		case CREATURE_AUTHOR:
			cr.armor.type=ARMOR_CHEAPSUIT;
			cr.money=LCSrandom(131)+100;
			cr.align=LCSrandom(3)-1;

			sk=LCSrandom(4)+2;cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
			sk=LCSrandom(4)+2;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
			break;
		case CREATURE_JOURNALIST:
			cr.armor.type=ARMOR_CHEAPSUIT;
			cr.money=LCSrandom(131)+100;
			cr.align=LCSrandom(3)-1;

			sk=LCSrandom(4)+2;cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
			sk=LCSrandom(4)+2;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
			break;
		case CREATURE_CRITIC_ART:
			cr.armor.type=ARMOR_CHEAPSUIT;
			cr.money=LCSrandom(131)+100;
			cr.align=LCSrandom(3)-1;

			sk=LCSrandom(4)+2;cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
			sk=LCSrandom(4)+2;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
			break;
		case CREATURE_CRITIC_MUSIC:
			cr.armor.type=ARMOR_CHEAPSUIT;
			cr.money=LCSrandom(131)+100;
			cr.align=LCSrandom(3)-1;

			sk=LCSrandom(4)+2;cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
			sk=LCSrandom(4)+2;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
			break;
		case CREATURE_SOCIALITE:
			strcpy(cr.name,"Socialite");
			if(LCSrandom(2))cr.armor.type=ARMOR_EXPENSIVEDRESS;
			else cr.armor.type=ARMOR_EXPENSIVESUIT;
			cr.money=LCSrandom(131)+100;
			cr.align=LCSrandom(3)-1;

			sk=LCSrandom(3)+1;cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
			break;
		case CREATURE_BIKER:
			strcpy(cr.name,"Biker");
			cr.armor.type=ARMOR_TRENCHCOAT;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			sk=LCSrandom(3)+3;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			break;
		case CREATURE_TRUCKER:
			strcpy(cr.name,"Truck Driver");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			sk=LCSrandom(3)+3;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			break;
		case CREATURE_TAXIDRIVER:
			strcpy(cr.name,"Taxi Driver");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;
			sk=LCSrandom(3)+3;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			break;
		case CREATURE_PROGRAMMER:
			strcpy(cr.name,"Programmer");
			cr.armor.type=ARMOR_CLOTHES;
			cr.money=LCSrandom(31)+20;
			cr.align=LCSrandom(3)-1;

			sk=LCSrandom(5)+4;cr.skill[SKILL_COMPUTERS]=sk;randomskills-=sk;
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=30;
			cr.att[ATTRIBUTE_INTELLIGENCE]=3;
			break;
		case CREATURE_NUN:
			break;
		case CREATURE_RETIREE:
			break;
		case CREATURE_PAINTER:
			break;
		case CREATURE_SCULPTOR:
			break;
		case CREATURE_DANCER:
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=11;
			cr.att[ATTRIBUTE_AGILITY]=10;
			cr.att[ATTRIBUTE_STRENGTH]=4;
			cr.att[ATTRIBUTE_HEALTH]=10;
			break;
		case CREATURE_PHOTOGRAPHER:
			break;
		case CREATURE_CAMERAMAN:
			break;
		case CREATURE_HAIRSTYLIST:
			break;
		case CREATURE_FASHIONDESIGNER:
			sk=LCSrandom(6)+5;cr.skill[SKILL_GARMENTMAKING]=sk;randomskills-=sk;
			break;
		case CREATURE_CLERK:
			break;
		case CREATURE_THIEF:
			switch(LCSrandom(5))
				{
				case 0:getrecruitcreature(cr.name,CREATURE_SOCIALITE);break;
				case 1:getrecruitcreature(cr.name,CREATURE_CLERK);break;
				case 2:getrecruitcreature(cr.name,CREATURE_OFFICEWORKER);break;
				case 3:getrecruitcreature(cr.name,CREATURE_CRITIC_ART);break;
				case 4:getrecruitcreature(cr.name,CREATURE_CRITIC_MUSIC);break;
				}
			sk=LCSrandom(5)+3;cr.skill[SKILL_SECURITY]=sk;randomskills-=sk;
			sk=LCSrandom(5)+3;cr.skill[SKILL_DISGUISE]=sk;randomskills-=sk;
			break;
		case CREATURE_ACTOR:
			sk=LCSrandom(5);cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
			sk=LCSrandom(5)+3;cr.skill[SKILL_DISGUISE]=sk;randomskills-=sk;
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=24;
			cr.att[ATTRIBUTE_WISDOM]=9;
			break;
		case CREATURE_YOGAINSTRUCTOR:
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=11;
			cr.att[ATTRIBUTE_AGILITY]=10;
			cr.att[ATTRIBUTE_STRENGTH]=4;
			cr.att[ATTRIBUTE_HEALTH]=10;
			break;
		case CREATURE_ATHLETE:
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=11;
			cr.att[ATTRIBUTE_AGILITY]=8;
			cr.att[ATTRIBUTE_STRENGTH]=8;
			cr.att[ATTRIBUTE_HEALTH]=8;
			break;
		}

	if(redistatts>0)
		{
		while(redistatts>0)
			{
			a=LCSrandom(ATTNUM);
			if(cr.att[a]<attcap[a])
				{
				cr.att[a]++;
				redistatts--;
				}
			}
		}

	//RANDOM STARTING SKILLS
	while(randomskills>0)
		{
		cr.skill[LCSrandom(SKILLNUM)]++;
		randomskills--;
		}

	//ALIENATION
	if(sitealienate>=1&&cr.align==0)conservatise(cr);
	if(sitealienate==2&&cr.align==1)conservatise(cr);
}

void knowmap(int locx,int locy,int locz)
{
	map[locx][locy][locz].flag|=SITEBLOCK_KNOWN;

	if(locx>0)map[locx-1][locy][locz].flag|=SITEBLOCK_KNOWN;
	if(locx<MAPX-1)map[locx+1][locy][locz].flag|=SITEBLOCK_KNOWN;
	if(locy>0)map[locx][locy-1][locz].flag|=SITEBLOCK_KNOWN;
	if(locy<MAPY-1)map[locx][locy+1][locz].flag|=SITEBLOCK_KNOWN;

	if(locx>0&&locy>0)
		{
		if(!(map[locx-1][locy][locz].flag & SITEBLOCK_BLOCK)||
			!(map[locx][locy-1][locz].flag & SITEBLOCK_BLOCK))
			{
			map[locx-1][locy-1][locz].flag|=SITEBLOCK_KNOWN;
			}
		}
	if(locx<MAPX-1&&locy>0)
		{
		if(!(map[locx+1][locy][locz].flag & SITEBLOCK_BLOCK)||
			!(map[locx][locy-1][locz].flag & SITEBLOCK_BLOCK))
			{
			map[locx+1][locy-1][locz].flag|=SITEBLOCK_KNOWN;
			}
		}
	if(locx>0&&locy<MAPY-1)
		{
		if(!(map[locx-1][locy][locz].flag & SITEBLOCK_BLOCK)||
			!(map[locx][locy+1][locz].flag & SITEBLOCK_BLOCK))
			{
			map[locx-1][locy+1][locz].flag|=SITEBLOCK_KNOWN;
			}
		}
	if(locx<MAPX-1&&locy<MAPY-1)
		{
		if(!(map[locx+1][locy][locz].flag & SITEBLOCK_BLOCK)||
			!(map[locx][locy+1][locz].flag & SITEBLOCK_BLOCK))
			{
			map[locx+1][locy+1][locz].flag|=SITEBLOCK_KNOWN;
			}
		}
}

void getweapon(char *str,int type)
{
	strcpy(str,"");

	switch(type)
		{
		//LIMIT ----------------------------------------:----: (-5 for guns)
		case WEAPON_NONE:			strcpy(str,"None");break;
		case WEAPON_CROWBAR:		strcpy(str,"Crowbar");break;
		case WEAPON_BASEBALLBAT:	strcpy(str,"Baseball Bat");break;
		case WEAPON_KNIFE:			strcpy(str,"Knife");break;
		case WEAPON_SHANK:			strcpy(str,"Shank");break;
		case WEAPON_SYRINGE:		strcpy(str,"Syringe");break;
		case WEAPON_REVOLVER_22:	strcpy(str,".22 Rev.");break;
		case WEAPON_REVOLVER_44:	strcpy(str,".44 Mag.");break;
		case WEAPON_SEMIPISTOL_9MM:	strcpy(str,"9mm Semi.");break;
		case WEAPON_SEMIPISTOL_45:	strcpy(str,".45 Semi.");break;
		case WEAPON_SEMIRIFLE_M16:	strcpy(str,"M16");break;
		case WEAPON_SEMIRIFLE_AK47:	strcpy(str,"AK47");break;
		case WEAPON_SHOTGUN:		strcpy(str,"Shotgun");break;
		case WEAPON_SWORD:			strcpy(str,"Morfiegr Sword");break;
		case WEAPON_AXE:			strcpy(str,"Axe of Kirgar");break;
		case WEAPON_HAMMER:			strcpy(str,"Dwarven Hammer");break;
		case WEAPON_MAUL:			strcpy(str,"Maul of Anrin");break;
		case WEAPON_CROSS:			strcpy(str,"Silver Cross");break;
		case WEAPON_STAFF:			strcpy(str,"Wizard's Staff");break;
		case WEAPON_CHAIN:			strcpy(str,"Chain");break;
		case WEAPON_NIGHTSTICK:		strcpy(str,"Nightstick");break;
		case WEAPON_GAVEL:			strcpy(str,"Gavel");break;
		case WEAPON_PITCHFORK:		strcpy(str,"Pitchfork");break;
		case WEAPON_TORCH:			strcpy(str,"Torch");break;
		}
}

void getweaponfull(char *str,int type)
{
	strcpy(str,"");

	switch(type)
		{
		//LIMIT -----------------------------------------------:----: (-5 for guns)
		case WEAPON_NONE:			strcpy(str,"None");break;
		case WEAPON_CROWBAR:		strcpy(str,"Crowbar");break;
		case WEAPON_BASEBALLBAT:	strcpy(str,"Baseball Bat");break;
		case WEAPON_KNIFE:			strcpy(str,"Knife");break;
		case WEAPON_SHANK:			strcpy(str,"Shank");break;
		case WEAPON_SYRINGE:		strcpy(str,"Syringe");break;
		case WEAPON_REVOLVER_22:	strcpy(str,".22 Revolver");break;
		case WEAPON_REVOLVER_44:	strcpy(str,".44 Magnum");break;
		case WEAPON_SEMIPISTOL_9MM:	strcpy(str,"9mm Semiauto");break;
		case WEAPON_SEMIPISTOL_45:	strcpy(str,".45 Semiauto");break;
		case WEAPON_SEMIRIFLE_M16:	strcpy(str,"M16");break;
		case WEAPON_SEMIRIFLE_AK47:	strcpy(str,"AK47");break;
		case WEAPON_SHOTGUN:		strcpy(str,"Shotgun");break;
		case WEAPON_SWORD:			strcpy(str,"Sword of Morfiegor");break;
		case WEAPON_AXE:			strcpy(str,"Axe of Kirgar");break;
		case WEAPON_HAMMER:			strcpy(str,"Dwarven Hammer");break;
		case WEAPON_MAUL:			strcpy(str,"Maul of Anrin");break;
		case WEAPON_CROSS:			strcpy(str,"Silver Cross");break;
		case WEAPON_STAFF:			strcpy(str,"Wizard's Staff");break;
		case WEAPON_CHAIN:			strcpy(str,"Chain");break;
		case WEAPON_NIGHTSTICK:		strcpy(str,"Nightstick");break;
		case WEAPON_GAVEL:			strcpy(str,"Gavel");break;
		case WEAPON_PITCHFORK:		strcpy(str,"Pitchfork");break;
		case WEAPON_TORCH:			strcpy(str,"Torch");break;
		}
}

void getarmor(char *str,int type,int subtype)
{
	switch(type)
		{
		//LIMIT ---------------------------------------------:
		case ARMOR_NONE:			strcpy(str,"Naked");break;
		case ARMOR_CLOTHES:			strcpy(str,"Clothes");break;
		case ARMOR_OVERALLS:		strcpy(str,"Overalls");break;
		case ARMOR_WIFEBEATER:		strcpy(str,"Wife Beater");break;
		case ARMOR_TRENCHCOAT:		strcpy(str,"Trenchcoat");break;
		case ARMOR_WORKCLOTHES:		strcpy(str,"Work Clothes");break;
		case ARMOR_SECURITYUNIFORM:	strcpy(str,"Security Unif.");break;
		case ARMOR_POLICEUNIFORM:	strcpy(str,"Police Uniform");break;
		case ARMOR_CHEAPSUIT:		strcpy(str,"Cheap Suit");break;
		case ARMOR_EXPENSIVESUIT:	strcpy(str,"Expensive Suit");break;
		case ARMOR_BLACKSUIT:		strcpy(str,"Black Suit");break;
		case ARMOR_CHEAPDRESS:		strcpy(str,"Cheap Dress");break;
		case ARMOR_EXPENSIVEDRESS:	strcpy(str,"Expensve Dress");break;
		case ARMOR_BLACKDRESS:		strcpy(str,"Black Dress");break;
		case ARMOR_LABCOAT:			strcpy(str,"Lab Coat");break;
		case ARMOR_BLACKROBE:		strcpy(str,"Black Robe");break;
		case ARMOR_CLOWNSUIT:		strcpy(str,"Clown Suit");break;
		case ARMOR_BONDAGEGEAR:		strcpy(str,"Bondage Gear");break;
		case ARMOR_MASK:
			switch(subtype)
				{
		case MASK_ASHCROFT:			strcpy(str,"Ashcroft Mask");break;
		case MASK_BARR:				strcpy(str,"Bob Barr Mask");break;
		case MASK_BLAIR:			strcpy(str,"Blair Mask");break;
		case MASK_BUSH_BARBARA:		strcpy(str,"Brb. Bush Mask");break;
		case MASK_BUSH_GEORGE:		strcpy(str,"G HW Bush Mask");break;
		case MASK_BUSH_GEORGE2:		strcpy(str,"G W. Bush Mask");break;
		//LIMIT ---------------------------------------------:
		case MASK_CARTER:			strcpy(str,"Carter Mask");break;
		case MASK_CHENEY:			strcpy(str,"Cheney Mask");break;
		case MASK_CLINTON_BILL:		strcpy(str,"B Clinton Mask");break;
		case MASK_CLINTON_HILLARY:	strcpy(str,"H Clinton Mask");break;
		case MASK_FALWELL:			strcpy(str,"Falwell Mask");break;
		case MASK_FORD:				strcpy(str,"Gld. Ford Mask");break;
		case MASK_GORE_AL:			strcpy(str,"Al Gore Mask");break;
		case MASK_GORE_TIPPER:		strcpy(str,"Tpr. Gore Mask");break;
		case MASK_HELMS:			strcpy(str,"Helms Mask");break;
		case MASK_JASON:			strcpy(str,"Jason Mask");break;
		case MASK_LINCOLN:			strcpy(str,"Lincoln Mask");break;
		case MASK_KENNEDY_JOHN:		strcpy(str,"JFK Mask");break;
		case MASK_KENNEDY_ROBERT:	strcpy(str,"RFK Mask");break;
		case MASK_KISSINGER:		strcpy(str,"Kissinger Mask");break;
		case MASK_NIXON:			strcpy(str,"Nixon Mask");break;
		case MASK_POWELL:			strcpy(str,"Powell Mask");break;
		case MASK_REAGAN_NANCY:		strcpy(str,"N. Reagan Mask");break;
		case MASK_REAGAN_RONALD:	strcpy(str,"R. Reagan Mask");break;
		case MASK_RICE:				strcpy(str,"Cn. Rice Mask");break;
		case MASK_ROBERTSON:		strcpy(str,"Robertson Mask");break;
		case MASK_RUMSFELD:			strcpy(str,"Rumsfeld Mask");break;
		case MASK_SATAN:			strcpy(str,"Satan Mask");break;
		case MASK_STARR:			strcpy(str,"Ken Starr Mask");break;
		case MASK_THURMOND:			strcpy(str,"Thurmond Mask");break;
		case MASK_WASHINGTON:		strcpy(str,"Washngton Mask");break;
		case MASK_WAXMAN:			strcpy(str,"Waxman Mask");break;
		//LIMIT ---------------------------------------------:
		case MASK_JESUS:			strcpy(str,"Jesus Mask");break;
		case MASK_COLEMAN_GARY:		strcpy(str,"G Coleman Mask");break;
		case MASK_MADONNA:			strcpy(str,"Madonna Mask");break;
		case MASK_SPEARS:			strcpy(str,"B Spears Mask");break;
		case MASK_EMINEM:			strcpy(str,"Eminem Mask");break;
		case MASK_AGUILERA:			strcpy(str,"C Aguilra Mask");break;
		case MASK_WAHLBERG:			strcpy(str,"M Wahlbrg Mask");break;
		case MASK_IGGYPOP:			strcpy(str,"Iggy Pop Mask");break;
		case MASK_CASH:				strcpy(str,"Jny. Cash Mask");break;
		case MASK_BINLADEN:			strcpy(str,"bin Laden Mask");break;
		case MASK_LORDS:			strcpy(str,"Tr. Lords Mask");break;
		case MASK_SHIELDS:			strcpy(str,"Br. Shlds Mask");break;
		case MASK_JACKSON_MICHAEL:	strcpy(str,"M Jackson Mask");break;
		case MASK_CRUTHERS:			strcpy(str,"Cruthers Mask");break;
		case MASK_KING_DON:			strcpy(str,"Don King Mask");break;
		//LIMIT ---------------------------------------------:
		default:
									strcpy(str,"Mask");
				}
			break;
		case ARMOR_MILITARY:		strcpy(str,"Army Uniform");break;
		case ARMOR_PRISONGUARD:		strcpy(str,"Guard Uniform");break;
		case ARMOR_PRISONER:		strcpy(str,"Orange Jmpsuit");break;
		case ARMOR_TOGA:			strcpy(str,"Toga");break;
		case ARMOR_MITHRIL:			strcpy(str,"Mithril Mail");break;
		}
}

void getarmorfull(char *str,int type,int subtype)
{
	switch(type)
		{
		//LIMIT --------------------------------------------------:
		case ARMOR_NONE:			strcpy(str,"Naked");break;
		case ARMOR_CLOTHES:			strcpy(str,"Clothes");break;
		case ARMOR_OVERALLS:		strcpy(str,"Overalls");break;
		case ARMOR_WIFEBEATER:		strcpy(str,"Wife Beater");break;
		case ARMOR_TRENCHCOAT:		strcpy(str,"Trenchcoat");break;
		case ARMOR_WORKCLOTHES:		strcpy(str,"Work Clothes");break;
		case ARMOR_SECURITYUNIFORM:	strcpy(str,"Security Uniform");break;
		case ARMOR_POLICEUNIFORM:	strcpy(str,"Police Uniform");break;
		case ARMOR_CHEAPSUIT:		strcpy(str,"Cheap Suit");break;
		case ARMOR_EXPENSIVESUIT:	strcpy(str,"Expensive Suit");break;
		case ARMOR_BLACKSUIT:		strcpy(str,"Black Suit");break;
		case ARMOR_CHEAPDRESS:		strcpy(str,"Cheap Dress");break;
		case ARMOR_EXPENSIVEDRESS:	strcpy(str,"Expensve Dress");break;
		case ARMOR_BLACKDRESS:		strcpy(str,"Black Dress");break;
		case ARMOR_LABCOAT:			strcpy(str,"Lab Coat");break;
		case ARMOR_BLACKROBE:		strcpy(str,"Black Robe");break;
		case ARMOR_CLOWNSUIT:		strcpy(str,"Clown Suit");break;
		case ARMOR_BONDAGEGEAR:		strcpy(str,"Bondage Gear");break;
		case ARMOR_MASK:
			switch(subtype)
				{
		case MASK_ASHCROFT:			strcpy(str,"John Ashcroft Mask");break;
		case MASK_BARR:				strcpy(str,"Bob Barr Mask");break;
		case MASK_BLAIR:			strcpy(str,"Tony Blair Mask");break;
		case MASK_BUSH_BARBARA:		strcpy(str,"Barbara Bush Mask");break;
		case MASK_BUSH_GEORGE:		strcpy(str,"George HW Bush Mask");break;
		case MASK_BUSH_GEORGE2:		strcpy(str,"George W. Bush Mask");break;
		//LIMIT --------------------------------------------------:
		case MASK_CARTER:			strcpy(str,"Jimmy Carter Mask");break;
		case MASK_CHENEY:			strcpy(str,"Dick Cheney Mask");break;
		case MASK_CLINTON_BILL:		strcpy(str,"Bill Clinton Mask");break;
		case MASK_CLINTON_HILLARY:	strcpy(str,"Hillary Clinton Msk");break;
		case MASK_FALWELL:			strcpy(str,"Jerry Falwell Mask");break;
		case MASK_FORD:				strcpy(str,"Gerald Ford Mask");break;
		case MASK_GORE_AL:			strcpy(str,"Al Gore Mask");break;
		case MASK_GORE_TIPPER:		strcpy(str,"Tipper Gore Mask");break;
		case MASK_HELMS:			strcpy(str,"Jesse Helms Mask");break;
		case MASK_JASON:			strcpy(str,"Jason Mask");break;
		case MASK_LINCOLN:			strcpy(str,"Abraham Lincoln Msk");break;
		case MASK_KENNEDY_JOHN:		strcpy(str,"John F Kennedy Mask");break;
		case MASK_KENNEDY_ROBERT:	strcpy(str,"Robert Kennedy Mask");break;
		case MASK_KISSINGER:		strcpy(str,"Henry Kissinger Msk");break;
		case MASK_NIXON:			strcpy(str,"Richard Nixon Mask");break;
		case MASK_POWELL:			strcpy(str,"Colin Powell Mask");break;
		case MASK_REAGAN_NANCY:		strcpy(str,"Nancy Reagan Mask");break;
		case MASK_REAGAN_RONALD:	strcpy(str,"Ronald Reagan Mask");break;
		case MASK_RICE:				strcpy(str,"Condoleezza Rice Mk");break;
		case MASK_ROBERTSON:		strcpy(str,"Pat Robertson Mask");break;
		case MASK_RUMSFELD:			strcpy(str,"Donald Rumsfeld Msk");break;
		case MASK_SATAN:			strcpy(str,"Satan Mask");break;
		case MASK_STARR:			strcpy(str,"Ken Starr Mask");break;
		case MASK_THURMOND:			strcpy(str,"Strom Thurmond Mask");break;
		case MASK_WASHINGTON:		strcpy(str,"George Washngtn Msk");break;
		case MASK_WAXMAN:			strcpy(str,"Henry Waxman Mask");break;
		//LIMIT --------------------------------------------------:
		case MASK_JESUS:			strcpy(str,"Jesus Christ Mask");break;
		case MASK_COLEMAN_GARY:		strcpy(str,"Gary Coleman Mask");break;
		case MASK_MADONNA:			strcpy(str,"Madonna Mask");break;
		case MASK_SPEARS:			strcpy(str,"Britney Spears Mask");break;
		case MASK_EMINEM:			strcpy(str,"Eminem Mask");break;
		case MASK_AGUILERA:			strcpy(str,"Chrstna Aguilera Mk");break;
		case MASK_WAHLBERG:			strcpy(str,"Mark Wahlbrg Mask");break;
		case MASK_IGGYPOP:			strcpy(str,"Iggy Pop Mask");break;
		case MASK_CASH:				strcpy(str,"Johnny Cash Mask");break;
		case MASK_BINLADEN:			strcpy(str,"Osama bin Laden Msk");break;
		case MASK_LORDS:			strcpy(str,"Traci Lords Mask");break;
		case MASK_SHIELDS:			strcpy(str,"Brooke Shields Mask");break;
		case MASK_JACKSON_MICHAEL:	strcpy(str,"Michael Jackson Msk");break;
		case MASK_CRUTHERS:			strcpy(str,"Scatman Cruthers Mk");break;
		case MASK_KING_DON:			strcpy(str,"Don King Mask");break;
		//LIMIT --------------------------------------------------:
		default:
									strcpy(str,"Mask");
				}
			break;
		case ARMOR_MILITARY:		strcpy(str,"Army Uniform");break;
		case ARMOR_PRISONGUARD:		strcpy(str,"Guard Uniform");break;
		case ARMOR_PRISONER:		strcpy(str,"Orange Jumpsuit");break;
		case ARMOR_TOGA:			strcpy(str,"Toga");break;
		case ARMOR_MITHRIL:			strcpy(str,"Mithril Mail");break;
		}
}

void getarmorfull(char *str,armorst &armor,char superfull)
{
	if(superfull)getarmorfull(str,armor.type,armor.subtype);
	else getarmor(str,armor.type,armor.subtype);

	if(armor.flag!=0||armor.quality!='1')
		{
		strcat(str,"[");
		if(armor.quality!='1')
			{
			char c[2];
			c[0]=armor.quality;
			c[1]='\x0';
			strcat(str,c);
			}
		if(armor.flag & ARMORFLAG_BLOODY)
			{
			strcat(str,"B");
			}
		if(armor.flag & ARMORFLAG_DAMAGED)
			{
			strcat(str,"D");
			}
		strcat(str,"]");
		}
}

void getskill(char *str,int type)
{
	strcpy(str,"");

	switch(type)
		{
		case SKILL_HANDTOHAND:strcpy(str,"Hand-to-Hand");break;
		case SKILL_KNIFE:strcpy(str,"Knife");break;
		case SKILL_SWORD:strcpy(str,"Sword");break;
		case SKILL_AXE:strcpy(str,"Axe");break;
		case SKILL_SPEAR:strcpy(str,"Spear");break;
		case SKILL_CLUB:strcpy(str,"Club");break;
		case SKILL_PISTOL:strcpy(str,"Pistol");break;
		case SKILL_RIFLE:strcpy(str,"Rifle");break;
		case SKILL_ASSAULTRIFLE:strcpy(str,"Assault Rifle");break;
		case SKILL_PERSUASION:strcpy(str,"Persuasion");break;
		case SKILL_SECURITY:strcpy(str,"Security");break;
		case SKILL_DISGUISE:strcpy(str,"Disguise");break;
		case SKILL_COMPUTERS:strcpy(str,"Computers");break;
		case SKILL_LAW:strcpy(str,"Law");break;
		case SKILL_GARMENTMAKING:strcpy(str,"Garment Making");break;
		case SKILL_DRIVING:strcpy(str,"Driving");break;
		case SKILL_WRITING:strcpy(str,"Writing");break;
		}
}

void delenc(short e,char loot)
{
	//MAKE GROUND LOOT
	if(mode!=GAMEMODE_SITE)loot=0;
	if(loot)makeloot(encounter[e],groundloot);

	//BURY IT
	for(int en=e;en<ENCMAX;en++)
		{
		if(!encounter[en].exists)break;
		if(en<ENCMAX-1)encounter[en]=encounter[en+1];
		}
	encounter[ENCMAX-1].exists=0;
}

void severloot(creaturest &cr,vector<itemst *> &loot)
{
	int armok=2;
	if((cr.wound[BODYPART_ARM_RIGHT] & WOUND_NASTYOFF)||
		 (cr.wound[BODYPART_ARM_RIGHT] & WOUND_CLEANOFF))armok--;
	if((cr.wound[BODYPART_ARM_LEFT] & WOUND_NASTYOFF)||
		 (cr.wound[BODYPART_ARM_LEFT] & WOUND_CLEANOFF))armok--;
	if(cr.special[SPECIALWOUND_NECK]!=1)armok=0;
	if(cr.special[SPECIALWOUND_UPPERSPINE]!=1)armok=0;

	if(cr.weapon.type!=WEAPON_NONE && armok==0)
		{
		clearmessagearea();
		set_color(COLOR_YELLOW,COLOR_BLACK,1);
		move(16,1);
		addstr("The ");
		addstr(" ");
		char str[200];
		getweaponfull(str,cr.weapon.type);
		addstr(str);
		move(17,1);
		addstr(cr.name);
		addstr("'s grasp.");
		refresh();
		getch();

		if(mode==GAMEMODE_SITE)
			{
			itemst *newi=new itemst;
				newi->type=ITEM_WEAPON;
				newi->weapon=cr.weapon;
			loot.push_back(newi);
			}

		cr.weapon.type=WEAPON_NONE;
		cr.weapon.ammo=0;

		for(int c=0;c<CLIPNUM;c++)
			{
			if(mode==GAMEMODE_SITE)
				{
				for(int p=0;p<cr.clip[c];p++)
					{
					itemst *newi=new itemst;
						newi->type=ITEM_CLIP;
						newi->cliptype=c;
					loot.push_back(newi);
					}
				}

			cr.clip[c]=0;
			}
		}

	if(((cr.wound[BODYPART_BODY] & WOUND_CLEANOFF)||
		(cr.wound[BODYPART_BODY] & WOUND_NASTYOFF))&&
		cr.armor.type!=ARMOR_NONE&&
		cr.armor.type!=ARMOR_MASK)
		{
		clearmessagearea();
		set_color(COLOR_YELLOW,COLOR_BLACK,1);
		move(16,1);
		addstr(cr.name);
		addstr("'s ");
		char str[80];
		getarmorfull(str,cr.armor.type,cr.armor.subtype);
		addstr(str);
		addstr(" has been destroyed.");
		refresh();
		getch();

		cr.armor.type=ARMOR_NONE;
		cr.armor.quality='1';
		cr.armor.flag=0;
		}

	if((cr.wound[BODYPART_HEAD] & WOUND_NASTYOFF)&&
		cr.armor.type==ARMOR_MASK)
		{
		clearmessagearea();
		set_color(COLOR_YELLOW,COLOR_BLACK,1);
		move(16,1);
		addstr(cr.name);
		addstr("'s ");
		char str[80];
		getarmorfull(str,cr.armor.type,cr.armor.subtype);
		addstr(str);
		addstr(" has been destroyed.");
		refresh();
		getch();

		cr.armor.type=ARMOR_NONE;
		cr.armor.quality='1';
		cr.armor.flag=0;
		}
}

void makeloot(creaturest &cr,vector<itemst *> &loot)
{
	if(cr.weapon.type!=WEAPON_NONE)
		{
		itemst *newi=new itemst;
			newi->type=ITEM_WEAPON;
			newi->weapon=cr.weapon;
		loot.push_back(newi);

		cr.weapon.type=WEAPON_NONE;
		cr.weapon.ammo=0;
		}

	if(cr.armor.type!=ARMOR_NONE)
		{
		itemst *newi=new itemst;
			newi->type=ITEM_ARMOR;
			newi->armor=cr.armor;
		loot.push_back(newi);

		cr.armor.type=ARMOR_NONE;
		cr.armor.quality='1';
		cr.armor.flag=0;
		}

	if(cr.money>0)
		{
		itemst *newi=new itemst;
			newi->type=ITEM_MONEY;
			newi->money=cr.money;
		loot.push_back(newi);

		cr.money=0;
		}

	for(int c=0;c<CLIPNUM;c++)
		{
		for(int p=0;p<cr.clip[c];p++)
			{
			itemst *newi=new itemst;
				newi->type=ITEM_CLIP;
				newi->cliptype=c;
			loot.push_back(newi);
			}

		cr.clip[c]=0;
		}
}

void youattack(void)
{
	short wasalarm=sitealarm;

	for(int e=0;e<ENCMAX;e++)
		{
		encounter[e].cantbluff=2;
		}

	sitealarm=1;

	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]==NULL)continue;
		if(!activesquad->squad[p]->alive)continue;

		if(mode==GAMEMODE_CHASECAR&&
			!activesquad->squad[p]->weapon.ranged())continue;

		vector<int> goodtarg;
		vector<int> badtarg;

		for(int e=0;e<ENCMAX;e++)
			{
			if(encounter[e].alive&&encounter[e].exists)
				{
				if(encounter[e].align==-1)goodtarg.push_back(e);
				else badtarg.push_back(e);
				}
			}

		if(goodtarg.size()==0)return;

		int target=goodtarg[LCSrandom(goodtarg.size())];

		char mistake=0;

		if(badtarg.size()>0 && !LCSrandom(10))
			{
			target=badtarg[LCSrandom(badtarg.size())];
			mistake=1;
			addjuice(*activesquad->squad[p],-5);
			}

		char actual;
		attack(*activesquad->squad[p],encounter[target],mistake,actual);

		if(actual)alienationcheck(mistake);

		if(mistake)
			{
			sitestory->crime.push_back(CRIME_ATTACKED_MISTAKE);
			sitecrime+=10;
			}
		else if(!wasalarm)
			{
			sitestory->crime.push_back(CRIME_ATTACKED);
			sitecrime+=3;
			}

		criminalizeparty(LAWFLAG_ASSAULT);

		if(!encounter[target].alive)delenc(target,1);
		}

	//COVER FIRE
	if(location[cursite]->siege.siege)
		{
		for(int p=0;p<pool.size();p++)
			{
			if(!pool[p]->alive)continue;
			if(pool[p]->align!=1)continue;
			if(pool[p]->squadid!=-1)continue;
			if(pool[p]->location!=cursite)continue;

			if(rangedweapon(pool[p]->weapon))
				{
				char conf=0;
				if(pool[p]->weapon.ammo>0)conf=1;
				if(ammotype(pool[p]->weapon.type)!=-1)
					{
					if(pool[p]->clip[ammotype(pool[p]->weapon.type)]>0)conf=1;
					}

				if(conf)
					{
					vector<int> goodtarg;
					vector<int> badtarg;

					for(int e=0;e<ENCMAX;e++)
						{
						if(encounter[e].alive&&encounter[e].exists)
							{
							if(encounter[e].align==-1)goodtarg.push_back(e);
							else badtarg.push_back(e);
							}
						}

					if(goodtarg.size()==0)return;

					int target=goodtarg[LCSrandom(goodtarg.size())];

					char mistake=0;

					if(badtarg.size()>0 && !LCSrandom(10))
						{
						target=badtarg[LCSrandom(badtarg.size())];
						mistake=1;
						}

					char actual;
					attack(*pool[p],encounter[target],mistake,actual);

					if(actual)alienationcheck(mistake);

					if(mistake)
						{
						sitestory->crime.push_back(CRIME_ATTACKED_MISTAKE);
						sitecrime+=10;
						}
					else if(!wasalarm)
						{
						sitestory->crime.push_back(CRIME_ATTACKED);
						sitecrime+=3;
						}

					criminalizeparty(LAWFLAG_ASSAULT);

					if(!encounter[target].alive)delenc(target,1);
					}
				}
			}
		}
}

char rangedweapon(weaponst &w)
{
	switch(w.type)
		{
		case WEAPON_REVOLVER_22:
		case WEAPON_REVOLVER_44:
		case WEAPON_SEMIPISTOL_9MM:
		case WEAPON_SEMIPISTOL_45:
		case WEAPON_SEMIRIFLE_M16:
		case WEAPON_SEMIRIFLE_AK47:
		case WEAPON_SHOTGUN:
			return 1;
		}

	return 0;
}

void enemyattack(void)
{
#ifdef NOENEMYATTACK
	return;
#endif

	int e2, e;
	char printed;
	for(e=0;e<ENCMAX;e++)
		{
		if(!encounter[e].exists)continue;
		if(!encounter[e].alive)continue;

		if(mode==GAMEMODE_CHASECAR&&
			!encounter[e].weapon.ranged())continue;

		encounter[e].cantbluff=2;
		if(encounter[e].align!=-1)
			{
			if(!incapacitated(encounter[e],0,printed))
				{
				if(printed)
					{
					printparty();
					if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
					else printencounter();

					refresh();
					getch();
					}

				clearmessagearea();

				move(16,1);
				addstr(encounter[e].name);
				if((encounter[e].wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
					(encounter[e].wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF)||
					(encounter[e].wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
					(encounter[e].wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))
					{
					switch(LCSrandom(3))
						{
						case 0:addstr(" crawls off moaning...");break;
						case 1:addstr(" crawls off wimpering...");break;
						case 2:addstr(" crawls off trailing blood...");break;
						}
					}
				else
					{
					switch(LCSrandom(5))
						{
						case 0:addstr(" runs off screaming!");break;
						case 1:addstr(" escapes crying!");break;
						case 2:addstr(" runs away!");break;
						case 3:addstr(" gets out of there!");break;
						case 4:addstr(" runs hollering!");break;
						}
					}

				delenc(e,0);
				e--;

				printparty();
				if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
				else printencounter();

				refresh();
				getch();
				}
			continue;
			}

		vector<int> goodtarg;
		vector<int> badtarg;

		for(int p=0;p<6;p++)
			{
			if(activesquad->squad[p]!=NULL)
				{
				if(activesquad->squad[p]->alive)goodtarg.push_back(p);
				}
			}

		for(e2=0;e2<ENCMAX;e2++)
			{
			if(!encounter[e].exists)continue;
			if(!encounter[e].alive)continue;
			if(encounter[e].align==-1)continue;

			badtarg.push_back(e2);
			}

		if(goodtarg.size()==0)return;

		int target=goodtarg[LCSrandom(goodtarg.size())];

		char canmistake=1;

		int encnum=0;
		for(e2=0;e2<ENCMAX;e2++)if(encounter[e2].exists)encnum++;

		if((encounter[e].type==CREATURE_SCIENTIST_EMINENT||
			encounter[e].type==CREATURE_JUDGE_LIBERAL||
			encounter[e].type==CREATURE_JUDGE_CONSERVATIVE||
			encounter[e].type==CREATURE_CORPORATE_CEO||
			encounter[e].type==CREATURE_RADIOPERSONALITY||
			encounter[e].type==CREATURE_NEWSANCHOR)&&encnum<ENCMAX)canmistake=0;

		char actual;
		if(canmistake)
			{
			if(activesquad->squad[target]->prisoner!=NULL && !LCSrandom(2))
				{
				attack(encounter[e],*activesquad->squad[target]->prisoner,1,actual);
				if(!activesquad->squad[target]->prisoner->alive)
					{
					if(activesquad->squad[target]->prisoner->squadid==-1)
						{
						clearmessagearea();
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(16,1);
						addstr(activesquad->squad[target]->name);
						addstr(" drops ");
						addstr(activesquad->squad[target]->prisoner->name);
						addstr("'s body.");

						sitecrime+=10;
						sitestory->crime.push_back(CRIME_KILLEDSOMEBODY);
						criminalizeparty(LAWFLAG_MURDERER);

						if(activesquad->squad[target]->prisoner->type==CREATURE_CORPORATE_CEO||
							activesquad->squad[target]->prisoner->type==CREATURE_RADIOPERSONALITY||
							activesquad->squad[target]->prisoner->type==CREATURE_NEWSANCHOR||
							activesquad->squad[target]->prisoner->type==CREATURE_SCIENTIST_EMINENT||
							activesquad->squad[target]->prisoner->type==CREATURE_JUDGE_CONSERVATIVE)sitecrime+=30;

						makeloot(*activesquad->squad[target]->prisoner,groundloot);

						refresh();
						getch();

						delete activesquad->squad[target]->prisoner;
						activesquad->squad[target]->prisoner=NULL;
						}
					}
				continue;
				}

			if(!LCSrandom(10)&&badtarg.size()>0)
				{
				int target=badtarg[LCSrandom(badtarg.size())];
				attack(encounter[e],encounter[target],1,actual);
				if(!encounter[target].alive)delenc(target,1);
				continue;
				}
			}

		attack(encounter[e],*activesquad->squad[target],0,actual);
		}
}

void attack(creaturest &a,creaturest &t,char mistake,char &actual)
{
	actual=0;

	char str[200],str2[200];

	clearmessagearea();

	//INCAPACITATED
	char incaprint;
	a.forceinc=0;
	if(incapacitated(a,0,incaprint))
		{
		if(incaprint)
			{
			printparty();
			if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
			else printencounter();

			refresh();
			getch();
			}

		a.forceinc=1;

		return;
		}

	//SPECIAL ATTACK!
	int encnum=0;
	for(int e=0;e<ENCMAX;e++)if(encounter[e].exists)encnum++;

	if((a.type==CREATURE_SCIENTIST_EMINENT||
		a.type==CREATURE_JUDGE_LIBERAL||
		a.type==CREATURE_JUDGE_CONSERVATIVE||
		a.type==CREATURE_CORPORATE_CEO||
		a.type==CREATURE_RADIOPERSONALITY||
		a.type==CREATURE_NEWSANCHOR)&&!mistake&&a.align==-1)
		{
		if(encnum<ENCMAX)
			{
			int resist=0;

			strcpy(str,a.name);
			strcat(str," ");

			int attack=LCSrandom(a.attval(ATTRIBUTE_WISDOM))+
				t.attval(ATTRIBUTE_WISDOM,0);

			switch(a.type)
				{
				case CREATURE_JUDGE_CONSERVATIVE:
				case CREATURE_JUDGE_LIBERAL:
					switch(LCSrandom(4))
						{
						case 0:strcat(str,"debates the death penalty with");break;
						case 1:strcat(str,"debates gay rights with");break;
						case 2:strcat(str,"debates free speech with");break;
						case 3:strcat(str,"debates the Second Amendment with");break;
						}
					strcat(str," ");
					strcat(str,t.name);
					strcat(str,"!");
					resist=t.attval(ATTRIBUTE_INTELLIGENCE,0)+
						t.attval(ATTRIBUTE_HEART,0)+t.skill[SKILL_LAW];
					attack+=LCSrandom(a.attval(ATTRIBUTE_INTELLIGENCE)/2+1)+LCSrandom(a.skill[SKILL_LAW]+1);
					break;
				case CREATURE_SCIENTIST_EMINENT:
					switch(LCSrandom(3))
						{
						case 0:strcat(str,"debates scientific ethics with");break;
						case 1:strcat(str,"explains the benefits of research to");break;
						case 2:strcat(str,"discusses the scientific method with");break;
						}
					strcat(str," ");
					strcat(str,t.name);
					strcat(str,"!");
					resist=t.attval(ATTRIBUTE_INTELLIGENCE,0)+
						t.attval(ATTRIBUTE_HEART,0);
					attack+=LCSrandom(a.attval(ATTRIBUTE_INTELLIGENCE));
					break;
				case CREATURE_CORPORATE_CEO:
					switch(LCSrandom(10))
						{
						case 0:strcat(str,"explains the derivatives market to");break;
						case 1:strcat(str,"justifies voodoo economics to");break;
						case 2:strcat(str,"extols the Reagan presidency to");break;
						case 3:strcat(str,"argues about tax cuts with");break;
						case 4:strcat(str,"explains conservative philosophy to");break;
						case 5:strcat(str,"extends a dinner invitation to");break;
						case 6:strcat(str,"offers a VP position to");break;
						case 7:strcat(str,"shows a $1000 bill to");break;
						case 8:strcat(str,"debates fiscal policy with");break;
						case 9:strcat(str,"offers stock options to");break;
						}
					strcat(str," ");
					strcat(str,t.name);
					strcat(str,"!");
					resist=t.attval(ATTRIBUTE_HEART,0);
					attack+=LCSrandom(a.skill[SKILL_PERSUASION]+1);
					break;
				case CREATURE_RADIOPERSONALITY:
				case CREATURE_NEWSANCHOR:
					switch(LCSrandom(5))
						{
						case 0:strcat(str,"winks at");break;
						case 1:strcat(str,"smiles at");break;
						case 2:strcat(str,"smirks at");break;
						case 3:strcat(str,"chats warmly with");break;
						case 4:strcat(str,"yells slogans at");break;
						}
					strcat(str," ");
					strcat(str,t.name);
					strcat(str,"!");
					resist=t.attval(ATTRIBUTE_CHARISMA,0)+
						t.attval(ATTRIBUTE_HEART,0);
					attack+=LCSrandom(a.attval(ATTRIBUTE_CHARISMA));
					break;
				}

			move(16,1);
			addstr(str);

			if(attack>resist)
				{
				if(t.juice>=100)
					{
					move(17,1);
					addstr(t.name);
					addstr(" loses juice!");
					addjuice(t,-100);
					}
				else if(LCSrandom(2))
					{
					move(17,1);
					addstr(t.name);
					addstr(" becomes Wiser!");
					t.att[ATTRIBUTE_WISDOM]++;
					}
				else
					{
					move(17,1);
					addstr(t.name);
					addstr(" is turned Conservative");
					if(t.prisoner!=NULL)
						{
						freehostage(t,0);
						}
					addstr("!");

					for(int e=0;e<ENCMAX;e++)
						{
						if(encounter[e].exists==0)
							{
							encounter[e]=t;
							encounter[e].exists=1;
							encounter[e].align=-1;
							encounter[e].squadid=-1;
							break;
							}
						}

					char flipstart=0;
					for(int p=0;p<6;p++)
						{
						if(activesquad->squad[p]==&t)
							{
							for(int pl=pool.size()-1;pl>=0;pl--)
								{
								if(pool[pl]==activesquad->squad[p])
									{
									delete pool[pl];
									pool.erase(pool.begin() + pl);
									break;
									}
								}

							activesquad->squad[p]=NULL;
							flipstart=1;
							}
						if(flipstart&&p<5)activesquad->squad[p]=activesquad->squad[p+1];
						}
					if(flipstart)activesquad->squad[5]=NULL;
					}
				}
			else
				{
				move(17,1);
				addstr(t.name);
				addstr(" remains strong.");
				}

			printparty();
			if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
			else printencounter();

			refresh();
			getch();

			return;
			}
		}

	//RELOAD
	if(ammotype(a.weapon.type)!=-1&&a.weapon.ammo==0)
		{
		if(a.clip[ammotype(a.weapon.type)]>0)
			{
			switch(ammotype(a.weapon.type))
				{
				case CLIP_9:a.weapon.ammo+=15;break;
				case CLIP_45:a.weapon.ammo+=15;break;
				case CLIP_ASSAULT:a.weapon.ammo+=30;break;
				case CLIP_22:a.weapon.ammo+=6;break;
				case CLIP_44:a.weapon.ammo+=6;break;
				case CLIP_BUCKSHOT:a.weapon.ammo+=2;break;
				}

			a.clip[ammotype(a.weapon.type)]--;

			strcpy(str,a.name);
			strcat(str," reloads.");

			move(16,1);
			addstr(str);

			printparty();
			if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
			else printencounter();

			refresh();
			getch();

			return;
			}
		}

	strcpy(str,a.name);
	strcat(str," ");
	if(mistake)strcat(str,"MISTAKENLY ");
	switch(a.weapon.type)
		{
		case WEAPON_NONE:
			{
			if(!a.animalgloss)strcat(str,"jabs at");
			else
				{
				if(a.specialattack==ATTACK_CANNON)strcat(str,"blasts at");
				else if(a.specialattack==ATTACK_FLAME)strcat(str,"breathes fire at");
				else if(a.specialattack==ATTACK_SUCK)strcat(str,"stabs at");
				else strcat(str,"claws at");
				}
			break;
			}
		case WEAPON_SWORD:
		case WEAPON_KNIFE:
		case WEAPON_SHANK:
			strcat(str,"slashes at");break;
		case WEAPON_SYRINGE:strcat(str,"pokes at");break;
		case WEAPON_REVOLVER_22:
		case WEAPON_REVOLVER_44:
		case WEAPON_SEMIPISTOL_9MM:
		case WEAPON_SEMIPISTOL_45:
		case WEAPON_SEMIRIFLE_M16:
		case WEAPON_SEMIRIFLE_AK47:
		case WEAPON_SHOTGUN:
			{
			if(a.weapon.ammo>0)strcat(str,"shoots at");
			else strcat(str,"swings at");
			break;
			}
		case WEAPON_CHAIN:
		case WEAPON_NIGHTSTICK:
		case WEAPON_CROWBAR:
		case WEAPON_BASEBALLBAT:
		case WEAPON_GAVEL:
		case WEAPON_AXE:
		case WEAPON_HAMMER:
		case WEAPON_MAUL:
		case WEAPON_CROSS:
		case WEAPON_STAFF:
		case WEAPON_TORCH:
			strcat(str,"swings at");break;
		case WEAPON_PITCHFORK:
			strcat(str,"stabs at");break;
		}
	strcat(str," ");
	strcat(str,t.name);
	move(16,1);
	addstr(str);

	strcpy(str,"");

	if(a.weapon.type!=WEAPON_NONE)
		{
		strcat(str,"with a ");
		getweaponfull(str2,a.weapon.type);
		strcat(str,str2);
		strcat(str," and ");
		}
	else
		{
		strcat(str,"and ");
		}

	//BASIC ROLL
	int aroll=LCSrandom(20)+1+LCSrandom(a.attval(ATTRIBUTE_AGILITY));
	if(a.prisoner!=NULL)aroll-=LCSrandom(10);
	int droll=LCSrandom(20)+1+LCSrandom(t.attval(ATTRIBUTE_AGILITY));
	if(t.prisoner!=NULL)droll-=LCSrandom(10);

	healthmodroll(aroll,a);
	healthmodroll(droll,t);

	if(aroll<0)aroll=1;
	if(droll<0)droll=1;

	//SKILL EFFECTS
	switch(a.weapon.type)
		{
		case WEAPON_NONE:
			aroll+=LCSrandom(a.skill[SKILL_HANDTOHAND]+1);
			a.skill_ip[SKILL_HANDTOHAND]+=droll;
			break;
		case WEAPON_KNIFE:
		case WEAPON_SHANK:
		case WEAPON_SYRINGE:
			aroll+=LCSrandom(a.skill[SKILL_KNIFE]+1);
			a.skill_ip[SKILL_KNIFE]+=droll;
			break;
		case WEAPON_CROWBAR:
		case WEAPON_BASEBALLBAT:
		case WEAPON_CHAIN:
		case WEAPON_GAVEL:
		case WEAPON_NIGHTSTICK:
		case WEAPON_HAMMER:
		case WEAPON_MAUL:
		case WEAPON_CROSS:
		case WEAPON_STAFF:
		case WEAPON_TORCH:
			aroll+=LCSrandom(a.skill[SKILL_CLUB]+1);
			a.skill_ip[SKILL_CLUB]+=droll;
			break;
		case WEAPON_SWORD:
			aroll+=LCSrandom(a.skill[SKILL_SWORD]+1);
			a.skill_ip[SKILL_SWORD]+=droll;
			break;
		case WEAPON_PITCHFORK:
			aroll+=LCSrandom(a.skill[SKILL_SPEAR]+1);
			a.skill_ip[SKILL_SPEAR]+=droll;
			break;
		case WEAPON_AXE:
			aroll+=LCSrandom(a.skill[SKILL_AXE]+1);
			a.skill_ip[SKILL_AXE]+=droll;
			break;
		case WEAPON_REVOLVER_22:
		case WEAPON_REVOLVER_44:
		case WEAPON_SEMIPISTOL_9MM:
		case WEAPON_SEMIPISTOL_45:
			aroll+=LCSrandom(a.skill[SKILL_PISTOL]+1);
			a.skill_ip[SKILL_PISTOL]+=droll;
			break;
		case WEAPON_SHOTGUN:
			aroll+=LCSrandom(a.skill[SKILL_RIFLE]+1);
			a.skill_ip[SKILL_PISTOL]+=droll;
			break;
		case WEAPON_SEMIRIFLE_M16:
		case WEAPON_SEMIRIFLE_AK47:
			aroll+=LCSrandom(a.skill[SKILL_ASSAULTRIFLE]+1);
			a.skill_ip[SKILL_ASSAULTRIFLE]+=droll;
			break;
		}

	//USE BULLETS
	if(a.weapon.ammo>0)
		{
		switch(a.weapon.type)
			{
			case WEAPON_REVOLVER_22:
			case WEAPON_REVOLVER_44:
			case WEAPON_SEMIPISTOL_9MM:
			case WEAPON_SEMIPISTOL_45:
			case WEAPON_SEMIRIFLE_M16:
			case WEAPON_SEMIRIFLE_AK47:
			case WEAPON_SHOTGUN:
				a.weapon.ammo--;
				break;
			}
		}

	//HIT!
	if(aroll>droll)
		{
		strcat(str,"hits the ");
		int w;
		
		do
			{
			w=LCSrandom(BODYPARTNUM);
			}while((t.wound[w]&WOUND_CLEANOFF) || (t.wound[w]&WOUND_NASTYOFF));

		if(t.animalgloss==ANIMALGLOSS_TANK)
			{
			switch(w)
				{
				case BODYPART_HEAD:strcat(str,"turret");break;
				case BODYPART_BODY:strcat(str,"front");break;
				case BODYPART_ARM_RIGHT:strcat(str,"right side");break;
				case BODYPART_ARM_LEFT:strcat(str,"left side");break;
				case BODYPART_LEG_RIGHT:strcat(str,"right tread");break;
				case BODYPART_LEG_LEFT:strcat(str,"left tread");break;
				}
			}
		else if(t.animalgloss==ANIMALGLOSS_ANIMAL)
			{
			switch(w)
				{
				case BODYPART_HEAD:strcat(str,"head");break;
				case BODYPART_BODY:strcat(str,"body");break;
				case BODYPART_ARM_RIGHT:strcat(str,"right front leg");break;
				case BODYPART_ARM_LEFT:strcat(str,"left front leg");break;
				case BODYPART_LEG_RIGHT:strcat(str,"right rear leg");break;
				case BODYPART_LEG_LEFT:strcat(str,"left rear leg");break;
				}
			}
		else
			{
			switch(w)
				{
				case BODYPART_HEAD:strcat(str,"head");break;
				case BODYPART_BODY:strcat(str,"body");break;
				case BODYPART_ARM_RIGHT:strcat(str,"right arm");break;
				case BODYPART_ARM_LEFT:strcat(str,"left arm");break;
				case BODYPART_LEG_RIGHT:strcat(str,"right leg");break;
				case BODYPART_LEG_LEFT:strcat(str,"left leg");break;
				}
			}

		char damtype=0;
		int damamount=0;
		char strengthmod=0;
		int severtype=-1;

		char damagearmor=0;

		switch(a.weapon.type)
			{
			case WEAPON_NONE:
				damamount=LCSrandom(5)+1;
				if(!a.animalgloss)damtype|=WOUND_BRUISED;
				else
					{
					if(a.specialattack==ATTACK_CANNON)
						{
						damamount=LCSrandom(500)+500;
						damtype|=WOUND_BURNED;
						damtype|=WOUND_TORN;
						}
					else if(a.specialattack==ATTACK_FLAME)damtype|=WOUND_BURNED;
					else if(a.specialattack==ATTACK_SUCK)damtype|=WOUND_CUT;
					else damtype|=WOUND_TORN;
					}
				strengthmod=1;
				severtype=WOUND_NASTYOFF;
				break;
			case WEAPON_CROWBAR:
				damtype|=WOUND_BRUISED;
				damamount=LCSrandom(80)+10;
				strengthmod=1;
				break;
			case WEAPON_MAUL:
			case WEAPON_BASEBALLBAT:
				damtype|=WOUND_BRUISED;
				damamount=LCSrandom(100)+10;
				strengthmod=1;
				break;
			case WEAPON_PITCHFORK:
				damtype|=WOUND_CUT;
				damtype|=WOUND_BLEEDING;
				damamount=LCSrandom(50)+5;
				strengthmod=1;
				damagearmor=1;
				break;
			case WEAPON_TORCH:
				damtype|=WOUND_BURNED;
				damamount=LCSrandom(10)+1;
				strengthmod=1;
				damagearmor=1;
				break;
			case WEAPON_SHANK:
			case WEAPON_KNIFE:
				damtype|=WOUND_CUT;
				damtype|=WOUND_BLEEDING;
				damamount=LCSrandom(10)+1;
				strengthmod=1;
				severtype=WOUND_CLEANOFF;
				damagearmor=1;
				break;
			case WEAPON_SYRINGE:
				damtype|=WOUND_CUT;
				damamount=LCSrandom(5)+1;
				strengthmod=1;
				break;
			case WEAPON_REVOLVER_22:
				if(a.weapon.ammo>0)
					{
					damtype|=WOUND_SHOT;
					damtype|=WOUND_BLEEDING;
					damamount=LCSrandom(150)+10;
					severtype=WOUND_NASTYOFF;
					damagearmor=1;
					}
				else
					{
					damtype|=WOUND_BRUISED;
					damamount=LCSrandom(20)+5;
					strengthmod=1;
					}
				break;
			case WEAPON_REVOLVER_44:
				if(a.weapon.ammo>0)
					{
					damtype|=WOUND_SHOT;
					damtype|=WOUND_BLEEDING;
					damamount=LCSrandom(300)+10;
					severtype=WOUND_NASTYOFF;
					damagearmor=1;
					}
				else
					{
					damtype|=WOUND_BRUISED;
					damamount=LCSrandom(20)+5;
					strengthmod=1;
					}
				break;
			case WEAPON_SEMIPISTOL_9MM:
				if(a.weapon.ammo>0)
					{
					damtype|=WOUND_SHOT;
					damtype|=WOUND_BLEEDING;
					damamount=LCSrandom(200)+10;
					severtype=WOUND_NASTYOFF;
					damagearmor=1;
					}
				else
					{
					damtype|=WOUND_BRUISED;
					damamount=LCSrandom(20)+5;
					strengthmod=1;
					}
				break;
			case WEAPON_SEMIPISTOL_45:
				if(a.weapon.ammo>0)
					{
					damtype|=WOUND_SHOT;
					damtype|=WOUND_BLEEDING;
					damamount=LCSrandom(200)+10;
					severtype=WOUND_NASTYOFF;
					damagearmor=1;
					}
				else
					{
					damtype|=WOUND_BRUISED;
					damamount=LCSrandom(20)+5;
					strengthmod=1;
					}
				break;
			case WEAPON_SEMIRIFLE_M16:
				if(a.weapon.ammo>0)
					{
					damtype|=WOUND_SHOT;
					damtype|=WOUND_BLEEDING;
					damamount=LCSrandom(300)+10;
					severtype=WOUND_NASTYOFF;
					damagearmor=1;
					}
				else
					{
					damtype|=WOUND_BRUISED;
					damamount=LCSrandom(30)+5;
					strengthmod=1;
					}
				break;
			case WEAPON_SEMIRIFLE_AK47:
				if(a.weapon.ammo>0)
					{
					damtype|=WOUND_SHOT;
					damtype|=WOUND_BLEEDING;
					damamount=LCSrandom(300)+10;
					severtype=WOUND_NASTYOFF;
					damagearmor=1;
					}
				else
					{
					damtype|=WOUND_BRUISED;
					damamount=LCSrandom(30)+5;
					strengthmod=1;
					}
				break;
			case WEAPON_SHOTGUN:
				if(a.weapon.ammo>0)
					{
					damtype|=WOUND_SHOT;
					damtype|=WOUND_BLEEDING;
					damamount=LCSrandom(300)+10;
					severtype=WOUND_NASTYOFF;
					damagearmor=1;
					}
				else
					{
					damtype|=WOUND_BRUISED;
					damamount=LCSrandom(30)+5;
					strengthmod=1;
					}
				break;
			case WEAPON_AXE:
			case WEAPON_SWORD:
				damtype|=WOUND_CUT;
				damtype|=WOUND_BLEEDING;
				damamount=LCSrandom(100)+10;
				strengthmod=1;
				severtype=WOUND_CLEANOFF;
				damagearmor=1;
				break;
			case WEAPON_HAMMER:
			case WEAPON_CROSS:
			case WEAPON_STAFF:
			case WEAPON_CHAIN:
				damtype|=WOUND_BRUISED;
				damamount=LCSrandom(30)+5;
				strengthmod=1;
				break;
			case WEAPON_NIGHTSTICK:
				damtype|=WOUND_BRUISED;
				damamount=LCSrandom(30)+5;
				strengthmod=1;
				break;
			case WEAPON_GAVEL:
				damtype|=WOUND_BRUISED;
				damamount=LCSrandom(20)+5;
				strengthmod=1;
				break;
			}

		int mod=0;

		if(strengthmod)
			{
			mod+=a.attval(ATTRIBUTE_STRENGTH)-5;
			}

		//SKILL BONUS FOR GOOD ROLL
		mod+=aroll-droll-5;

		//DO THE HEALTH MOD ON THE WOUND
		mod-=a.attval(ATTRIBUTE_HEALTH)-5;

		damagemod(t,damtype,damamount,mod);

		if(damamount>0)
			{
			t.wound[w]|=damtype;

			int severamount=100;

			switch(w)
				{
				case BODYPART_HEAD:severamount=100;break;
				case BODYPART_BODY:severamount=1000;break;
				case BODYPART_ARM_RIGHT:severamount=200;break;
				case BODYPART_ARM_LEFT:severamount=200;break;
				case BODYPART_LEG_RIGHT:severamount=400;break;
				case BODYPART_LEG_LEFT:severamount=400;break;
				}

			if(severtype!=-1 && damamount>=severamount)
				{
				t.wound[w]|=(char)severtype;
				}

			if(w!=BODYPART_HEAD && w!=BODYPART_BODY && t.blood-damamount<=0 &&
				t.blood>0)
				{
				do
					{
					damamount>>=1;
					}while(t.blood-damamount<=0);
				}

			if(damagearmor)armordamage(t.armor,w);

			t.blood-=damamount;

			map[locx][locy][locz].flag|=SITEBLOCK_BLOODY;

			if((t.wound[BODYPART_HEAD] & WOUND_CLEANOFF)||
				(t.wound[BODYPART_BODY] & WOUND_CLEANOFF)||
				(t.wound[BODYPART_HEAD] & WOUND_NASTYOFF)||
				(t.wound[BODYPART_BODY] & WOUND_NASTYOFF)||
				t.blood<=0)
				{
				if((t.wound[BODYPART_HEAD] & WOUND_NASTYOFF)||
					(t.wound[BODYPART_BODY] & WOUND_NASTYOFF))bloodblast(t.armor);

				char alreadydead=!t.alive;
				
				if(!alreadydead)
					{
					t.blood=0;
					t.alive=0;
					if(t.squadid!=-1)
						{
						if(t.align==1)stat_dead++;
						}
					else if(t.align==-1&&t.animalgloss!=ANIMALGLOSS_ANIMAL)
						{
						stat_kills++;
						if(location[cursite]->siege.siege)location[cursite]->siege.kills++;
						}
					if(t.squadid==-1)
						{
						sitecrime+=10;
						sitestory->crime.push_back(CRIME_KILLEDSOMEBODY);
						criminalizeparty(LAWFLAG_MURDERER);
						}
					}

				if(t.wound[BODYPART_HEAD] & WOUND_CLEANOFF)strcat(str," CUTTING IT OFF!");
				else if(t.wound[BODYPART_BODY] & WOUND_CLEANOFF)strcat(str," CUTTING IT IN HALF!");
				else if(t.wound[BODYPART_HEAD] & WOUND_NASTYOFF)strcat(str," BLOWING IT APART!");
				else if(t.wound[BODYPART_BODY] & WOUND_NASTYOFF)strcat(str," BLOWING IT IN HALF!");
				else strcat(str,".");
				move(17,1);
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				addstr(str);

				refresh();
				getch();

				if(!alreadydead)
					{
					severloot(t,groundloot);

					clearmessagearea();
					adddeathmessage(t);

					refresh();
					getch();

					if(t.prisoner!=NULL)
						{
						freehostage(t,1);
						}
					}
				}
			else
				{
				if(t.wound[w] & WOUND_CLEANOFF)strcat(str," CUTTING IT OFF!");
				else if(t.wound[w] & WOUND_NASTYOFF)strcat(str," BLOWING IT OFF!");
				else strcat(str,".");
				move(17,1);
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				addstr(str);

				if(t.wound[w] & WOUND_NASTYOFF)bloodblast(t.armor);

				printparty();
				if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
				else printencounter();

				refresh();
				getch();

				//SPECIAL WOUNDS
				if(!(t.wound[w] & WOUND_CLEANOFF)&&
					!(t.wound[w] & WOUND_NASTYOFF)&&
					!t.animalgloss)
					{
					char heavydam=0;
					char breakdam=0;
					char pokedam=0;
					if(damamount>=6)
						{
						if(damtype & WOUND_SHOT)heavydam=1;
						if(damtype & WOUND_BURNED)heavydam=1;
						if(damtype & WOUND_TORN)heavydam=1;
						if(damtype & WOUND_CUT)heavydam=1;
						}

					if(damamount>=5)
						{
						if(damtype & WOUND_SHOT)pokedam=1;
						if(damtype & WOUND_TORN)pokedam=1;
						if(damtype & WOUND_CUT)pokedam=1;
						}

					if((damtype & WOUND_BRUISED)&&damamount>=50)breakdam=1;
					if((damtype & WOUND_SHOT)&&damamount>=50)breakdam=1;
					if((damtype & WOUND_TORN)&&damamount>=50)breakdam=1;
					if((damtype & WOUND_CUT)&&damamount>=50)breakdam=1;

					if(w==BODYPART_HEAD)
						{
						clearmessagearea();
						set_color(COLOR_MAGENTA,COLOR_BLACK,1);

						switch(LCSrandom(7))
							{
							case 0:
								if((t.special[SPECIALWOUND_RIGHTEYE]||
									t.special[SPECIALWOUND_LEFTEYE]||
									t.special[SPECIALWOUND_NOSE])&&
									heavydam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s face is blasted off!");
									else if(damtype & WOUND_BURNED)addstr("'s face is burned away!");
									else if(damtype & WOUND_TORN)addstr("'s face is torn off!");
									else if(damtype & WOUND_CUT)addstr("'s face is cut away!");
									else addstr("'s face is removed!");
									refresh();
									getch();

									t.special[SPECIALWOUND_RIGHTEYE]=0;
									t.special[SPECIALWOUND_LEFTEYE]=0;
									t.special[SPECIALWOUND_NOSE]=0;
									if(t.blood>20)t.blood=20;
									}
								break;
							case 1:
								if(t.special[SPECIALWOUND_TEETH]>0)
									{
									int teethminus=LCSrandom(TOOTHNUM)+1;
									if(teethminus>t.special[SPECIALWOUND_TEETH])teethminus=t.special[SPECIALWOUND_TEETH];
									char num[20];
									itoa(teethminus,num,10);

									move(16,1);
									if(teethminus>1)
										{
										addstr(num);
										addstr(" of ");
										addstr(t.name);
										addstr("'s teeth are ");
										}
									else if(t.special[SPECIALWOUND_TEETH]>1)
										{
										addstr("One of ");
										addstr(t.name);
										addstr("'s teeth is ");
										}
									else
										{
										addstr(t.name);
										addstr("'s last tooth is ");
										}

									if(damtype & WOUND_SHOT)addstr("shot out!");
									else if(damtype & WOUND_BURNED)addstr("burned away!");
									else if(damtype & WOUND_TORN)addstr("gouged out!");
									else if(damtype & WOUND_CUT)addstr("cut out!");
									else addstr("knocked out!");
									refresh();
									getch();

									t.special[SPECIALWOUND_TEETH]-=teethminus;
									}
								break;
							case 2:
								if(t.special[SPECIALWOUND_RIGHTEYE]&&
									heavydam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s right eye is blasted out!");
									else if(damtype & WOUND_BURNED)addstr("'s right eye is burned away!");
									else if(damtype & WOUND_TORN)addstr("'s right eye is torn out!");
									else if(damtype & WOUND_CUT)addstr("'s right eye is poked out!");
									else addstr("'s right eye is removed!");
									refresh();
									getch();

									t.special[SPECIALWOUND_RIGHTEYE]=0;
									if(t.blood>50)t.blood=50;
									}
								break;
							case 3:
								if(t.special[SPECIALWOUND_LEFTEYE]&&
									heavydam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s left eye is blasted out!");
									else if(damtype & WOUND_BURNED)addstr("'s left eye is burned away!");
									else if(damtype & WOUND_TORN)addstr("'s left eye is torn out!");
									else if(damtype & WOUND_CUT)addstr("'s left eye is poked out!");
									else addstr("'s left eye is removed!");
									refresh();
									getch();

									t.special[SPECIALWOUND_LEFTEYE]=0;
									if(t.blood>50)t.blood=50;
									}
								break;
							case 4:
								if(t.special[SPECIALWOUND_TONGUE]&&
									heavydam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s tongue is blasted off!");
									else if(damtype & WOUND_BURNED)addstr("'s tongue is burned away!");
									else if(damtype & WOUND_TORN)addstr("'s tongue is torn out!");
									else if(damtype & WOUND_CUT)addstr("'s tongue is cut off!");
									else addstr("'s tongue is removed!");
									refresh();
									getch();

									t.special[SPECIALWOUND_TONGUE]=0;
									if(t.blood>50)t.blood=50;
									}
								break;
							case 5:
								if(t.special[SPECIALWOUND_NOSE]&&
									heavydam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s nose is blasted off!");
									else if(damtype & WOUND_BURNED)addstr("'s nose is burned away!");
									else if(damtype & WOUND_TORN)addstr("'s nose is torn off!");
									else if(damtype & WOUND_CUT)addstr("'s nose is cut off!");
									else addstr("'s nose is removed!");
									refresh();
									getch();

									t.special[SPECIALWOUND_NOSE]=0;
									if(t.blood>50)t.blood=50;
									}
								break;
							case 6:
								if(t.special[SPECIALWOUND_NECK]&&
									breakdam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s neck bones are shattered!");
									else addstr("'s neck is broken!");
									refresh();
									getch();

									t.special[SPECIALWOUND_NECK]=0;
									if(t.blood>20)t.blood=20;
									}
								break;
							}
						}
					if(w==BODYPART_BODY)
						{
						clearmessagearea();
						set_color(COLOR_MAGENTA,COLOR_BLACK,1);

						switch(LCSrandom(11))
							{
							case 0:
								if(t.special[SPECIALWOUND_UPPERSPINE]&&
									breakdam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s upper spine is shattered!");
									else addstr("'s upper spine is broken!");
									refresh();
									getch();

									t.special[SPECIALWOUND_UPPERSPINE]=0;
									if(t.blood>20)t.blood=20;
									}
								break;
							case 1:
								if(t.special[SPECIALWOUND_LOWERSPINE]&&
									breakdam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s lower spine is shattered!");
									else addstr("'s lower spine is broken!");
									refresh();
									getch();

									t.special[SPECIALWOUND_LOWERSPINE]=0;
									if(t.blood>20)t.blood=20;
									}
								break;
							case 2:
								if(t.special[SPECIALWOUND_RIGHTLUNG]&&
									pokedam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s right lung is blasted!");
									else if(damtype & WOUND_TORN)addstr("'s right lung is torn!");
									else addstr("'s right lung is punctured!");
									refresh();
									getch();

									t.special[SPECIALWOUND_RIGHTLUNG]=0;
									if(t.blood>20)t.blood=20;
									}
								break;
							case 3:
								if(t.special[SPECIALWOUND_LEFTLUNG]&&
									pokedam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s left lung is blasted!");
									else if(damtype & WOUND_TORN)addstr("'s left lung is torn!");
									else addstr("'s left lung is punctured!");
									refresh();
									getch();

									t.special[SPECIALWOUND_LEFTLUNG]=0;
									if(t.blood>20)t.blood=20;
									}
								break;
							case 4:
								if(t.special[SPECIALWOUND_HEART]&&
									pokedam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s heart is blasted!");
									else if(damtype & WOUND_TORN)addstr("'s heart is torn!");
									else addstr("'s heart is punctured!");
									refresh();
									getch();

									t.special[SPECIALWOUND_HEART]=0;
									if(t.blood>3)t.blood=3;
									}
								break;
							case 5:
								if(t.special[SPECIALWOUND_LIVER]&&
									pokedam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s liver is blasted!");
									else if(damtype & WOUND_TORN)addstr("'s liver is torn!");
									else addstr("'s liver is punctured!");
									refresh();
									getch();

									t.special[SPECIALWOUND_LIVER]=0;
									if(t.blood>50)t.blood=50;
									}
								break;
							case 6:
								if(t.special[SPECIALWOUND_STOMACH]&&
									pokedam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s stomach is blasted!");
									else if(damtype & WOUND_TORN)addstr("'s stomach is torn!");
									else addstr("'s stomach is punctured!");
									refresh();
									getch();

									t.special[SPECIALWOUND_STOMACH]=0;
									if(t.blood>50)t.blood=50;
									}
								break;
							case 7:
								if(t.special[SPECIALWOUND_RIGHTKIDNEY]&&
									pokedam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s right kidney is blasted!");
									else if(damtype & WOUND_TORN)addstr("'s right kidney is torn!");
									else addstr("'s right kidney is punctured!");
									refresh();
									getch();

									t.special[SPECIALWOUND_RIGHTKIDNEY]=0;
									if(t.blood>50)t.blood=50;
									}
								break;
							case 8:
								if(t.special[SPECIALWOUND_LEFTKIDNEY]&&
									pokedam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s left kidney is blasted!");
									else if(damtype & WOUND_TORN)addstr("'s left kidney is torn!");
									else addstr("'s left kidney is punctured!");
									refresh();
									getch();

									t.special[SPECIALWOUND_LEFTKIDNEY]=0;
									if(t.blood>50)t.blood=50;
									}
								break;
							case 9:
								if(t.special[SPECIALWOUND_SPLEEN]&&
									pokedam)
									{
									move(16,1);
									addstr(t.name);
									if(damtype & WOUND_SHOT)addstr("'s spleen is blasted!");
									else if(damtype & WOUND_TORN)addstr("'s spleen is torn!");
									else addstr("'s spleen is punctured!");
									refresh();
									getch();

									t.special[SPECIALWOUND_SPLEEN]=0;
									if(t.blood>50)t.blood=50;
									}
								break;
							case 10:
								if(t.special[SPECIALWOUND_RIBS]>0&&
									breakdam)
									{
									int ribminus=LCSrandom(RIBNUM)+1;
									if(ribminus>t.special[SPECIALWOUND_RIBS])ribminus=t.special[SPECIALWOUND_RIBS];
									char num[20];
									itoa(ribminus,num,10);

									move(16,1);
									if(ribminus>1)
										{
										addstr(num);
										addstr(" of ");
										addstr(t.name);
										addstr("'s ribs are ");
										}
									else if(t.special[SPECIALWOUND_RIBS]>1)
										{
										addstr("One of ");
										addstr(t.name);
										addstr("'s rib is ");
										}
									else
										{
										addstr(t.name);
										addstr("'s last unbroken rib is ");
										}

									if(damtype & WOUND_SHOT)addstr("shot apart!");
									else addstr("broken!");
									refresh();
									getch();

									t.special[SPECIALWOUND_RIBS]-=ribminus;
									}
								break;
							}
						}

					severloot(t,groundloot);
					}

				set_color(COLOR_WHITE,COLOR_BLACK,1);				
				}
			}
		else
			{
			strcat(str," to no effect.");
			move(17,1);
			addstr(str);

			printparty();
			if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
			else printencounter();

			refresh();
			getch();
			}
		}
	else
		{
		strcat(str,"misses.");
		move(17,1);
		addstr(str);

		printparty();
		if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
		else printencounter();

		refresh();
		getch();
		}

	actual=1;
	return;
}

void damagemod(creaturest &t,char &damtype,int &damamount,int mod)
{
	int prot=0;

	switch(t.armor.type)
		{
		case ARMOR_CLOTHES:prot=1;break;
		case ARMOR_OVERALLS:prot=1;break;
		case ARMOR_WIFEBEATER:prot=1;break;
		case ARMOR_TRENCHCOAT:prot=2;break;
		case ARMOR_WORKCLOTHES:prot=1;break;
		case ARMOR_SECURITYUNIFORM:prot=1;break;
		case ARMOR_POLICEUNIFORM:prot=1;break;
		case ARMOR_CHEAPSUIT:prot=1;break;
		case ARMOR_EXPENSIVESUIT:prot=1;break;
		case ARMOR_BLACKSUIT:prot=1;break;
		case ARMOR_CHEAPDRESS:prot=1;break;
		case ARMOR_EXPENSIVEDRESS:prot=1;break;
		case ARMOR_BLACKDRESS:prot=1;break;
		case ARMOR_LABCOAT:prot=1;break;
		case ARMOR_BLACKROBE:prot=1;break;
		case ARMOR_CLOWNSUIT:prot=1;break;
		case ARMOR_BONDAGEGEAR:prot=1;break;
		case ARMOR_MASK:prot=0;break;
		case ARMOR_MILITARY:prot=1;break;
		case ARMOR_PRISONGUARD:prot=1;break;
		case ARMOR_PRISONER:prot=1;break;
		case ARMOR_TOGA:prot=0;break;
		case ARMOR_MITHRIL:prot=0;break;
		}

	mod-=prot;

	if(mod<=-4)damamount>>=4;
	else if(mod<=-3)damamount>>=3;
	else if(mod<=-2)damamount>>=2;
	else if(mod<=-1)damamount>>=1;
	else if(mod>=10)damamount=(int)((float)damamount * mod);
	else if(mod>=9)damamount=(int)((float)damamount * 8.0f);
	else if(mod>=8)damamount=(int)((float)damamount * 6.0f);
	else if(mod>=7)damamount=(int)((float)damamount * 4.0f);
	else if(mod>=6)damamount=(int)((float)damamount * 2.0f);
	else if(mod>=5)damamount=(int)((float)damamount * 1.75f);
	else if(mod>=4)damamount=(int)((float)damamount * 1.5f);
	else if(mod>=3)damamount=(int)((float)damamount * 1.4f);
	else if(mod>=2)damamount=(int)((float)damamount * 1.3f);
	else if(mod>=1)damamount=(int)((float)damamount * 1.2f);

	if(damamount<0)damamount=0;

	if(t.animalgloss==ANIMALGLOSS_TANK)damamount=0;
}

void creatureadvance(void)
{
	int e;
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]==NULL)continue;
		if(!activesquad->squad[p]->alive)continue;

		advancecreature(*activesquad->squad[p]);
		if(activesquad->squad[p]->prisoner!=NULL)
			{
			advancecreature(*activesquad->squad[p]->prisoner);
			if(!activesquad->squad[p]->prisoner->alive)
				{
				if(activesquad->squad[p]->prisoner->squadid==-1)
					{
					clearmessagearea();
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(16,1);
					addstr(activesquad->squad[p]->name);
					addstr(" drops ");
					addstr(activesquad->squad[p]->prisoner->name);
					addstr("'s body.");

					makeloot(*activesquad->squad[p]->prisoner,groundloot);

					refresh();
					getch();

					sitecrime+=10;
					sitestory->crime.push_back(CRIME_KILLEDSOMEBODY);
					criminalizeparty(LAWFLAG_MURDERER);

					if(activesquad->squad[p]->prisoner->type==CREATURE_CORPORATE_CEO||
						activesquad->squad[p]->prisoner->type==CREATURE_RADIOPERSONALITY||
						activesquad->squad[p]->prisoner->type==CREATURE_NEWSANCHOR||
						activesquad->squad[p]->prisoner->type==CREATURE_SCIENTIST_EMINENT||
						activesquad->squad[p]->prisoner->type==CREATURE_JUDGE_CONSERVATIVE)sitecrime+=30;

					delete activesquad->squad[p]->prisoner;
					activesquad->squad[p]->prisoner=NULL;
					}
				}
			}
		}

	if(location[cursite]->siege.siege)
		{
		for(int p=0;p<pool.size();p++)
			{
			if(!pool[p]->alive)continue;
			if(pool[p]->squadid!=-1)continue;
			if(pool[p]->location!=cursite)continue;

			advancecreature(*pool[p]);
			}

		autopromote(cursite);
		}

	for(e=0;e<ENCMAX;e++)
		{
		if(!encounter[e].exists)continue;
		if(!encounter[e].alive)continue;

		advancecreature(encounter[e]);
		}

	if(mode!=GAMEMODE_CHASECAR)
		{
		//TAKE THE INJURED WITH YOU
		squadgrab_immobile(0);

		//TAKE THE DEAD WITH YOU
		squadgrab_immobile(1);
		}

	for(e=ENCMAX-1;e>=0;e--)
		{
		if(!encounter[e].exists)continue;
		if(!encounter[e].alive)delenc(e,1);
		}

	if(mode==GAMEMODE_SITE)
		{
		if(sitealarm)postalarmtimer++;
		if(sitealarmtimer>0&&!sitealarm)
			{
			sitealarmtimer--;
			if(sitealarmtimer<=0)
				{
				sitealarmtimer=0;

				clearmessagearea();

				set_color(COLOR_YELLOW,COLOR_BLACK,1);
				move(16,1);
				addstr("The Squad smells Conservative panic.");

				if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
				else printencounter();

				refresh();
				getch();
				}
			}
		}
}

void advancecreature(creaturest &cr)
{
	if(!cr.alive)return;

	char incaprint;
	if(incapacitated(cr,1,incaprint))
		{
		if(incaprint)
			{
			printparty();
			if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
			else printencounter();

			refresh();
			getch();
			}
		}

	int bleed=0;

	for(int w=0;w<BODYPARTNUM;w++)
		{
		if(cr.wound[w] & WOUND_BLEEDING)bleed++;
		}

	if(bleed>0)
		{
		clearmessagearea();

		cr.blood-=bleed;

		map[locx][locy][locz].flag|=SITEBLOCK_BLOODY;

		if(cr.armor.type!=ARMOR_NONE)cr.armor.flag|=ARMORFLAG_BLOODY;

		char str[200];

		if(cr.blood<=0)
			{
			cr.blood=0;
			cr.alive=0;

			if(cr.squadid!=-1)
				{
				if(cr.align==1)stat_dead++;
				}
			else if(cr.align==-1&&cr.animalgloss!=ANIMALGLOSS_ANIMAL)
				{
				stat_kills++;
				if(location[cursite]->siege.siege)location[cursite]->siege.kills++;
				}
			if(cr.squadid==-1)
				{
				sitecrime+=10;
				sitestory->crime.push_back(CRIME_KILLEDSOMEBODY);
				criminalizeparty(LAWFLAG_MURDERER);
				}
			adddeathmessage(cr);

			refresh();
			getch();

			if(cr.prisoner!=NULL)
				{
				freehostage(cr,1);
				}
			}
		else
			{
			set_color(COLOR_RED,COLOR_BLACK,0);
			move(16,1);
			strcpy(str,cr.name);
			strcat(str," bleeds.");
			addstr(str);

			refresh();
			getch();
			}
		}
}

void equip(vector<itemst *> &loot,int loc)
{
	if(activesquad==NULL)return;

	consolidateloot(loot);
	if(loc!=-1)consolidateloot(location[loc]->loot);

	int page=0;

	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addstr("Equip the Squad");

		printparty();

		int x=1,y=10;
		char str[200],str2[200];

		for(int l=page*18;l<loot.size()&&l<page*18+18;l++)
			{
			if(loot[l]->type==ITEM_WEAPON)
				{
				getweaponfull(str2,loot[l]->weapon.type);
				if(loot[l]->weapon.ammo>0)
					{
					char num[20];
					itoa(loot[l]->weapon.ammo,num,10);
					strcat(str2," (");
					strcat(str2,num);
					strcat(str2,")");
					}
				}
			if(loot[l]->type==ITEM_ARMOR)
				{
				getarmorfull(str2,loot[l]->armor,0);
				}
			if(loot[l]->type==ITEM_CLIP)
				{
				getclip(str2,loot[l]->cliptype);
				}
			if(loot[l]->type==ITEM_LOOT)
				{
				getloot(str2,loot[l]->loottype);
				}
			if(loot[l]->type==ITEM_MONEY)
				{
				strcpy(str2,"$");
				char num[20];
				itoa(loot[l]->money,num,10);
				strcat(str2,num);
				}
			if(loot[l]->number>1)
				{
				char num[20];
				itoa(loot[l]->number,num,10);
				strcat(str2," ");
				strcat(str2,"x");
				strcat(str2,num);
				}
			str[0]=l-page*18+'A';
			str[1]='\x0';
			strcat(str," - ");
			strcat(str,str2);

			move(y,x);
			addstr(str);

			x+=26;
			if(x>53)
				{
				x=1;
				y++;
				}
			}

		//PAGE UP
		if(page>0)
			{
			move(17,1);
			if(interface_pgup=='[')
				{
				addstr("[ - Previous");
				}
			else
				{
				addstr("PGUP - Previous");
				}
			}
		//PAGE DOWN
		if((page+1)*18<loot.size())
			{
			move(17,53);
			if(interface_pgup=='[')
				{
				addstr("] - Next");
				}
			else
				{
				addstr("PGDN - Next");
				}
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(19,1);
		addstr("Press a letter to equip a Liberal item");
		move(20,1);
		addstr("Press a number to drop that Squad member's Conservative weapon");
		move(21,1);
		addstr("S - Liberally Strip a Squad member");

		if(loc!=-1)
			{
			if(location[loc]->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(22,1);
			addstr("Y - Get things from ");
			addstr(location[loc]->shortname);

			if(loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(22,40);
			addstr("Z - Stash things at ");
			addstr(location[loc]->shortname);
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(24,1);
		addstr("X - Done");

		refresh();

		int c=getch();
		translategetch(c);

		if(c>='a'&&c<='r')
			{
			int slot=c-'a'+page*18;

			if(slot>=0&&slot<loot.size())
				{
				move(8,20);
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				addstr("Choose a Liberal squad member to receive it.");

				refresh();

				int c=getch();
				translategetch(c);

				if(c>='1'&&c<='6')
					{
					if(activesquad->squad[c-'1']!=NULL)
						{
						int armok=2;
						if((activesquad->squad[c-'1']->wound[BODYPART_ARM_RIGHT] & WOUND_NASTYOFF)||
							(activesquad->squad[c-'1']->wound[BODYPART_ARM_RIGHT] & WOUND_CLEANOFF))armok--;
						if((activesquad->squad[c-'1']->wound[BODYPART_ARM_LEFT] & WOUND_NASTYOFF)||
							(activesquad->squad[c-'1']->wound[BODYPART_ARM_LEFT] & WOUND_CLEANOFF))armok--;
						if(activesquad->squad[c-'1']->special[SPECIALWOUND_NECK]!=1)armok=0;
						if(activesquad->squad[c-'1']->special[SPECIALWOUND_UPPERSPINE]!=1)armok=0;

						if(loot[slot]->type==ITEM_WEAPON && armok)
							{
							if(activesquad->squad[c-'1']->weapon.type!=WEAPON_NONE)
								{
								itemst *newloot=new itemst;
									newloot->type=ITEM_WEAPON;
									newloot->weapon=activesquad->squad[c-'1']->weapon;
								loot.push_back(newloot);
								}

							activesquad->squad[c-'1']->weapon=loot[slot]->weapon;

							loot[slot]->number--;
							if(loot[slot]->number==0)
								{
								delete loot[slot];
								loot.erase(loot.begin() + slot);
								}

							//DROP ALL CLIPS THAT DON'T WORK
							for(int cl=0;cl<CLIPNUM;cl++)
								{
								if(cl==ammotype(activesquad->squad[c-'1']->weapon.type))continue;

								for(int p2=0;p2<activesquad->squad[c-'1']->clip[cl];p2++)
									{
									itemst *newi=new itemst;
										newi->type=ITEM_CLIP;
										newi->cliptype=cl;
									loot.push_back(newi);
									}

								activesquad->squad[c-'1']->clip[cl]=0;
								}

							if(page*18>=loot.size()&&page!=0)page--;
							}
						else if(loot[slot]->type==ITEM_ARMOR)
							{
							if(activesquad->squad[c-'1']->armor.type!=ARMOR_NONE)
								{
								itemst *newloot=new itemst;
									newloot->type=ITEM_ARMOR;
									newloot->armor=activesquad->squad[c-'1']->armor;
								loot.push_back(newloot);
								}

							activesquad->squad[c-'1']->armor=loot[slot]->armor;

							loot[slot]->number--;
							if(loot[slot]->number==0)
								{
								delete loot[slot];
								loot.erase(loot.begin() + slot);
								}

							if(page*18>=loot.size()&&page!=0)page--;
							}
						else if(loot[slot]->type==ITEM_CLIP && armok)
							{
							if(ammotype(activesquad->squad[c-'1']->weapon.type)==loot[slot]->cliptype&&
								activesquad->squad[c-'1']->clip[ammotype(activesquad->squad[c-'1']->weapon.type)]<9)
								{
								activesquad->squad[c-'1']->clip[ammotype(activesquad->squad[c-'1']->weapon.type)]++;

								loot[slot]->number--;
								if(loot[slot]->number==0)
									{
									delete loot[slot];
									loot.erase(loot.begin() + slot);
									}

								if(page*18>=loot.size()&&page!=0)page--;
								}
							}

						consolidateloot(loot);
						}
					}
				}
			}
		if(c=='s')
			{
			move(8,20);
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			addstr("Choose a Liberal squad member to strip down.");

			refresh();

			int c=getch();
			translategetch(c);

			if(c>='1'&&c<='6')
				{
				if(activesquad->squad[c-'1']!=NULL)
					{
					if(activesquad->squad[c-'1']->armor.type!=ARMOR_NONE)
						{
						itemst *newi=new itemst;
							newi->type=ITEM_ARMOR;
							newi->armor=activesquad->squad[c-'1']->armor;
						loot.push_back(newi);

						activesquad->squad[c-'1']->armor.type=ARMOR_NONE;
						activesquad->squad[c-'1']->armor.quality='1';
						activesquad->squad[c-'1']->armor.flag=0;

						consolidateloot(loot);
						}
					}
				}
			}

		if(c=='x')return;

		if(loc!=-1)
			{
			if(c=='y'&&location[loc]->loot.size()>0)moveloot(loot,location[loc]->loot);
			if(c=='z'&&loot.size()>0)moveloot(location[loc]->loot,loot);
			}

		if(c>='1'&&c<='6')
			{
			int p=c-'1';
			if(activesquad->squad[p]!=NULL)
				{
				if(activesquad->squad[p]->weapon.type!=WEAPON_NONE)
					{
					itemst *i=new itemst;
						i->type=ITEM_WEAPON;
						i->weapon=activesquad->squad[p]->weapon;
					loot.push_back(i);

					activesquad->squad[p]->weapon.type=WEAPON_NONE;
					activesquad->squad[p]->weapon.ammo=0;

					//DROP ALL CLIPS
					for(int c=0;c<CLIPNUM;c++)
						{
						for(int p2=0;p2<activesquad->squad[p]->clip[c];p2++)
							{
							itemst *newi=new itemst;
								newi->type=ITEM_CLIP;
								newi->cliptype=c;
							loot.push_back(newi);
							}

						activesquad->squad[p]->clip[c]=0;
						}

					consolidateloot(loot);
					}
				}
			}

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*18<loot.size())page++;

		}while(1);
}

short ammotype(int type)
{
	switch(type)
		{
		case WEAPON_REVOLVER_22:
			return CLIP_22;
		case WEAPON_REVOLVER_44:
			return CLIP_44;
		case WEAPON_SEMIPISTOL_9MM:
			return CLIP_9;
		case WEAPON_SEMIPISTOL_45:
			return CLIP_45;
		case WEAPON_SEMIRIFLE_M16:
		case WEAPON_SEMIRIFLE_AK47:
			return CLIP_ASSAULT;
		case WEAPON_SHOTGUN:
			return CLIP_BUCKSHOT;
		}

	return -1;
}

void getclip(char *str,int clip)
{
	strcpy(str,"");

	switch(clip)
		{
		case CLIP_9:strcat(str,"9MM Clip");break;
		case CLIP_45:strcat(str,".45 Clip");break;
		case CLIP_ASSAULT:strcat(str,"Aslt.Rifle Clip");break;
		case CLIP_22:strcat(str,".22 Speedloader");break;
		case CLIP_44:strcat(str,".44 Speedloader");break;
		case CLIP_BUCKSHOT:strcat(str,"Pair of Buckshots");break;
		}
}


void savegame(char *str)
{
	char dummy_c;
	int dummy;
	DWORD numbytes;
	HANDLE h;
	int l;
	
	h=LCSCreateFile(str, LCSIO_WRITE);
	if(h!=NULL)
		{
		WriteFile(h,&version,sizeof(unsigned long),&numbytes,NULL);

		WriteFile(h,&seed,sizeof(unsigned long),&numbytes,NULL);

		WriteFile(h,&mode,sizeof(int),&numbytes,NULL);

		WriteFile(h,&day,sizeof(int),&numbytes,NULL);
		WriteFile(h,&month,sizeof(int),&numbytes,NULL);
		WriteFile(h,&year,sizeof(int),&numbytes,NULL);
		WriteFile(h,&execterm,sizeof(short),&numbytes,NULL);
		WriteFile(h,&amendnum,sizeof(int),&numbytes,NULL);

		WriteFile(h,&stat_recruits,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&stat_dead,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&stat_kills,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&stat_kidnappings,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&stat_funds,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&stat_spent,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&stat_buys,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&stat_burns,sizeof(unsigned long),&numbytes,NULL);

		WriteFile(h,&curcarid,sizeof(long),&numbytes,NULL);
		WriteFile(h,&showcarprefs,sizeof(char),&numbytes,NULL);
		WriteFile(h,&curcreatureid,sizeof(long),&numbytes,NULL);
		WriteFile(h,&cursquadid,sizeof(long),&numbytes,NULL);
		WriteFile(h,&offended_cops,sizeof(short),&numbytes,NULL);
		WriteFile(h,&offended_corps,sizeof(short),&numbytes,NULL);
		WriteFile(h,&offended_cia,sizeof(short),&numbytes,NULL);
		WriteFile(h,&offended_amradio,sizeof(short),&numbytes,NULL);
		WriteFile(h,&offended_cablenews,sizeof(short),&numbytes,NULL);
		WriteFile(h,&attorneyseed,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&selectedsiege,sizeof(long),&numbytes,NULL);
		WriteFile(h,lcityname,sizeof(char)*80,&numbytes,NULL);
		WriteFile(h,&newscherrybusted,sizeof(char),&numbytes,NULL);
		WriteFile(h,newspaper_topicwork,sizeof(short)*VIEWNUM,&numbytes,NULL);
		WriteFile(h,&moneygained_donate,sizeof(long),&numbytes,NULL);
		WriteFile(h,&moneygained_brownies,sizeof(long),&numbytes,NULL);
		WriteFile(h,&moneygained_goods,sizeof(long),&numbytes,NULL);
		WriteFile(h,&moneygained_ccfraud,sizeof(long),&numbytes,NULL);
		WriteFile(h,&moneygained_hustling,sizeof(long),&numbytes,NULL);
		WriteFile(h,&moneygained_thievery,sizeof(long),&numbytes,NULL);
		WriteFile(h,&moneylost_goods,sizeof(long),&numbytes,NULL);
		WriteFile(h,&moneylost_trouble,sizeof(long),&numbytes,NULL);
		WriteFile(h,&moneylost_rent,sizeof(long),&numbytes,NULL);
		WriteFile(h,&moneylost_manufacture,sizeof(long),&numbytes,NULL);
		WriteFile(h,&moneylost_legal,sizeof(long),&numbytes,NULL);
		WriteFile(h,&moneylost_compound,sizeof(long),&numbytes,NULL);
		WriteFile(h,&moneylost_hostage,sizeof(long),&numbytes,NULL);

		WriteFile(h,slogan,sizeof(char)*80,&numbytes,NULL);
		WriteFile(h,&funds,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&party_status,sizeof(short),&numbytes,NULL);

		WriteFile(h,attitude,sizeof(short)*VIEWNUM,&numbytes,NULL);
		WriteFile(h,law,sizeof(short)*LAWNUM,&numbytes,NULL);
		WriteFile(h,house,sizeof(short)*435,&numbytes,NULL);
		WriteFile(h,senate,sizeof(short)*100,&numbytes,NULL);
		WriteFile(h,court,sizeof(short)*9,&numbytes,NULL);
		WriteFile(h,courtname,sizeof(char)*9*80,&numbytes,NULL);
		WriteFile(h,exec,sizeof(short)*EXECNUM,&numbytes,NULL);
		WriteFile(h,execname,sizeof(char)*EXECNUM*80,&numbytes,NULL);

		//LOCATIONS
		dummy=location.size();
		WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
		for(l=0;l<location.size();l++)
			{
			dummy=location[l]->loot.size();
			WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
			for(int l2=0;l2<location[l]->loot.size();l2++)
				{
				WriteFile(h,location[l]->loot[l2],sizeof(itemst),&numbytes,NULL);
				}

			WriteFile(h,location[l]->name,sizeof(char)*40,&numbytes,NULL);
			WriteFile(h,location[l]->shortname,sizeof(char)*20,&numbytes,NULL);
			WriteFile(h,&location[l]->type,sizeof(short),&numbytes,NULL);
			WriteFile(h,&location[l]->parent,sizeof(long),&numbytes,NULL);
			WriteFile(h,&location[l]->renting,sizeof(long),&numbytes,NULL);
			WriteFile(h,&location[l]->newrental,sizeof(char),&numbytes,NULL);
			WriteFile(h,&location[l]->needcar,sizeof(char),&numbytes,NULL);
			WriteFile(h,&location[l]->closed,sizeof(short),&numbytes,NULL);
			WriteFile(h,&location[l]->highsecurity,sizeof(char),&numbytes,NULL);
			WriteFile(h,&location[l]->siege,sizeof(siegest),&numbytes,NULL);
			WriteFile(h,&location[l]->compound_walls,sizeof(char),&numbytes,NULL);
			WriteFile(h,&location[l]->compound_stores,sizeof(long),&numbytes,NULL);
			WriteFile(h,&location[l]->front_business,sizeof(short),&numbytes,NULL);
			WriteFile(h,location[l]->front_name,sizeof(char)*40,&numbytes,NULL);
			WriteFile(h,&location[l]->haveflag,sizeof(char),&numbytes,NULL);
			WriteFile(h,&location[l]->mapseed,sizeof(unsigned long),&numbytes,NULL);
			}

		//VEHICLES
		dummy=vehicle.size();
		WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
		for(l=0;l<vehicle.size();l++)
			{
			WriteFile(h,vehicle[l],sizeof(vehiclest),&numbytes,NULL);
			}

		//POOL
		dummy=pool.size();
		WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
		for(int pl=0;pl<pool.size();pl++)
			{
			WriteFile(h,pool[pl],sizeof(creaturest),&numbytes,NULL);
			}

		//SQUADS
		dummy=squad.size();
		WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
		for(int sq=0;sq<squad.size();sq++)
			{
			WriteFile(h,squad[sq]->name,sizeof(char)*40,&numbytes,NULL);
			WriteFile(h,&squad[sq]->activity,sizeof(activityst),&numbytes,NULL);
			WriteFile(h,&squad[sq]->id,sizeof(int),&numbytes,NULL);

			for(int pos=0;pos<6;pos++)
				{
				if(squad[sq]->squad[pos]==NULL)dummy_c=0;
				else dummy_c=1;
				WriteFile(h,&dummy_c,sizeof(char),&numbytes,NULL);

				if(squad[sq]->squad[pos]!=NULL)
					{
					WriteFile(h,&squad[sq]->squad[pos]->id,sizeof(long),&numbytes,NULL);
					}
				}

			dummy=squad[sq]->loot.size();
			WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
			for(int l2=0;l2<squad[sq]->loot.size();l2++)
				{
				WriteFile(h,squad[sq]->loot[l2],sizeof(itemst),&numbytes,NULL);
				}
			}

		if(activesquad==NULL)dummy_c=0;
		else dummy_c=1;
		WriteFile(h,&dummy_c,sizeof(char),&numbytes,NULL);
		if(activesquad!=NULL)
			{
			WriteFile(h,&activesquad->id,sizeof(long),&numbytes,NULL);
			}

		//DATES
		dummy=date.size();
		WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
		for(int dt=0;dt<date.size();dt++)
			{
			WriteFile(h,&date[dt]->mac_id,sizeof(long),&numbytes,NULL);
			WriteFile(h,&date[dt]->timeleft,sizeof(short),&numbytes,NULL);
			dummy=date[dt]->date.size();
			WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
			for(int dt2=0;dt2<date[dt]->date.size();dt2++)
				{
				WriteFile(h,date[dt]->date[dt2],sizeof(creaturest),&numbytes,NULL);
				}
			}

		//NEWS STORIES
		dummy=newsstory.size();
		WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
		for(int ns=0;ns<newsstory.size();ns++)
			{
			WriteFile(h,&newsstory[ns]->type,sizeof(short),&numbytes,NULL);
			WriteFile(h,&newsstory[ns]->view,sizeof(short),&numbytes,NULL);

			WriteFile(h,&newsstory[ns]->loc,sizeof(long),&numbytes,NULL);
			WriteFile(h,&newsstory[ns]->priority,sizeof(long),&numbytes,NULL);
			WriteFile(h,&newsstory[ns]->page,sizeof(long),&numbytes,NULL);
			WriteFile(h,&newsstory[ns]->positive,sizeof(char),&numbytes,NULL);
			WriteFile(h,&newsstory[ns]->siegetype,sizeof(short),&numbytes,NULL);

			if(newsstory[ns]->cr==NULL)dummy_c=0;
			else dummy_c=1;
			WriteFile(h,&dummy_c,sizeof(char),&numbytes,NULL);
			if(newsstory[ns]->cr!=NULL)
				{
				WriteFile(h,&newsstory[ns]->cr->id,sizeof(long),&numbytes,NULL);
				}

			dummy=newsstory[ns]->crime.size();
			WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
			for(int dt2=0;dt2<newsstory[ns]->crime.size();dt2++)
				{
				WriteFile(h,&newsstory[ns]->crime[dt2],sizeof(int),&numbytes,NULL);
				}
			}

		CloseHandle(h);
		}
}

void save(void)
{
 savegame("save.dat");
}

void autosave(void)
{
 savegame("autosave.dat");
}


char load(void)
{
	//LOAD FILE
	unsigned long loadversion;
    int l;
	char dummy_c;
	int dummy;
	long dummy_l;
	DWORD numbytes;
	HANDLE h;
	
	h=LCSCreateFile("save.dat", LCSIO_READ);
		
	if(h!=NULL)
		{
		ReadFile(h,&loadversion,sizeof(unsigned long),&numbytes,NULL);

		//NUKE INVALID SAVE GAMES
		if(loadversion<lowestloadversion)
			{
			CloseHandle(h);

			reset();

			return 0;
			}

		ReadFile(h,&seed,sizeof(unsigned long),&numbytes,NULL);

		ReadFile(h,&mode,sizeof(int),&numbytes,NULL);

		ReadFile(h,&day,sizeof(int),&numbytes,NULL);
		ReadFile(h,&month,sizeof(int),&numbytes,NULL);
		ReadFile(h,&year,sizeof(int),&numbytes,NULL);
		ReadFile(h,&execterm,sizeof(short),&numbytes,NULL);
		ReadFile(h,&amendnum,sizeof(int),&numbytes,NULL);

		ReadFile(h,&stat_recruits,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&stat_dead,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&stat_kills,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&stat_kidnappings,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&stat_funds,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&stat_spent,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&stat_buys,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&stat_burns,sizeof(unsigned long),&numbytes,NULL);

		ReadFile(h,&curcarid,sizeof(long),&numbytes,NULL);
		ReadFile(h,&showcarprefs,sizeof(char),&numbytes,NULL);
		ReadFile(h,&curcreatureid,sizeof(long),&numbytes,NULL);
		ReadFile(h,&cursquadid,sizeof(long),&numbytes,NULL);
		ReadFile(h,&offended_cops,sizeof(short),&numbytes,NULL);
		ReadFile(h,&offended_corps,sizeof(short),&numbytes,NULL);
		ReadFile(h,&offended_cia,sizeof(short),&numbytes,NULL);
		ReadFile(h,&offended_amradio,sizeof(short),&numbytes,NULL);
		ReadFile(h,&offended_cablenews,sizeof(short),&numbytes,NULL);
		ReadFile(h,&attorneyseed,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&selectedsiege,sizeof(long),&numbytes,NULL);
		ReadFile(h,lcityname,sizeof(char)*80,&numbytes,NULL);
		ReadFile(h,&newscherrybusted,sizeof(char),&numbytes,NULL);
		ReadFile(h,newspaper_topicwork,sizeof(short)*VIEWNUM,&numbytes,NULL);
		ReadFile(h,&moneygained_donate,sizeof(long),&numbytes,NULL);
		ReadFile(h,&moneygained_brownies,sizeof(long),&numbytes,NULL);
		ReadFile(h,&moneygained_goods,sizeof(long),&numbytes,NULL);
		ReadFile(h,&moneygained_ccfraud,sizeof(long),&numbytes,NULL);
		ReadFile(h,&moneygained_hustling,sizeof(long),&numbytes,NULL);
		ReadFile(h,&moneygained_thievery,sizeof(long),&numbytes,NULL);
		ReadFile(h,&moneylost_goods,sizeof(long),&numbytes,NULL);
		ReadFile(h,&moneylost_trouble,sizeof(long),&numbytes,NULL);
		ReadFile(h,&moneylost_rent,sizeof(long),&numbytes,NULL);
		ReadFile(h,&moneylost_manufacture,sizeof(long),&numbytes,NULL);
		ReadFile(h,&moneylost_legal,sizeof(long),&numbytes,NULL);
		ReadFile(h,&moneylost_compound,sizeof(long),&numbytes,NULL);
		ReadFile(h,&moneylost_hostage,sizeof(long),&numbytes,NULL);

		ReadFile(h,slogan,sizeof(char)*80,&numbytes,NULL);
		ReadFile(h,&funds,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&party_status,sizeof(short),&numbytes,NULL);

		ReadFile(h,attitude,sizeof(short)*VIEWNUM,&numbytes,NULL);
		ReadFile(h,law,sizeof(short)*LAWNUM,&numbytes,NULL);
		ReadFile(h,house,sizeof(short)*435,&numbytes,NULL);
		ReadFile(h,senate,sizeof(short)*100,&numbytes,NULL);
		ReadFile(h,court,sizeof(short)*9,&numbytes,NULL);
		ReadFile(h,courtname,sizeof(char)*9*80,&numbytes,NULL);
		ReadFile(h,exec,sizeof(short)*EXECNUM,&numbytes,NULL);
		ReadFile(h,execname,sizeof(char)*EXECNUM*80,&numbytes,NULL);

		//LOCATIONS
		ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
		location.resize(dummy);
		for(l=0;l<location.size();l++)
			{
			location[l]=new locationst;

			ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
			location[l]->loot.resize(dummy);
			for(int l2=0;l2<location[l]->loot.size();l2++)
				{
				location[l]->loot[l2]=new itemst;
				ReadFile(h,location[l]->loot[l2],sizeof(itemst),&numbytes,NULL);
				}

			ReadFile(h,location[l]->name,sizeof(char)*40,&numbytes,NULL);
			ReadFile(h,location[l]->shortname,sizeof(char)*20,&numbytes,NULL);
			ReadFile(h,&location[l]->type,sizeof(short),&numbytes,NULL);
			ReadFile(h,&location[l]->parent,sizeof(long),&numbytes,NULL);
			ReadFile(h,&location[l]->renting,sizeof(long),&numbytes,NULL);
			ReadFile(h,&location[l]->newrental,sizeof(char),&numbytes,NULL);
			ReadFile(h,&location[l]->needcar,sizeof(char),&numbytes,NULL);
			ReadFile(h,&location[l]->closed,sizeof(short),&numbytes,NULL);
			ReadFile(h,&location[l]->highsecurity,sizeof(char),&numbytes,NULL);
			ReadFile(h,&location[l]->siege,sizeof(siegest),&numbytes,NULL);
			ReadFile(h,&location[l]->compound_walls,sizeof(char),&numbytes,NULL);
			ReadFile(h,&location[l]->compound_stores,sizeof(long),&numbytes,NULL);
			ReadFile(h,&location[l]->front_business,sizeof(short),&numbytes,NULL);
			ReadFile(h,location[l]->front_name,sizeof(char)*40,&numbytes,NULL);
			ReadFile(h,&location[l]->haveflag,sizeof(char),&numbytes,NULL);
			ReadFile(h,&location[l]->mapseed,sizeof(unsigned long),&numbytes,NULL);
			}

		//VEHICLES
		ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
		vehicle.resize(dummy);
		for(l=0;l<vehicle.size();l++)
			{
			vehicle[l]=new vehiclest;
			ReadFile(h,vehicle[l],sizeof(vehiclest),&numbytes,NULL);
			}

		//POOL
		ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
		pool.resize(dummy);
		for(int pl=0;pl<pool.size();pl++)
			{
			pool[pl]=new creaturest;
			ReadFile(h,pool[pl],sizeof(creaturest),&numbytes,NULL);
			}

		//SQUADS
		ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
		squad.resize(dummy);
		for(int sq=0;sq<squad.size();sq++)
			{
			squad[sq]=new squadst;

			ReadFile(h,squad[sq]->name,sizeof(char)*40,&numbytes,NULL);
			ReadFile(h,&squad[sq]->activity,sizeof(activityst),&numbytes,NULL);
			ReadFile(h,&squad[sq]->id,sizeof(int),&numbytes,NULL);

			for(int pos=0;pos<6;pos++)
				{
				ReadFile(h,&dummy_c,sizeof(char),&numbytes,NULL);

				//REBUILD SQUAD FROM POOL
				squad[sq]->squad[pos]=NULL;
				if(dummy_c)
					{
					ReadFile(h,&dummy_l,sizeof(long),&numbytes,NULL);
					for(int pl=0;pl<pool.size();pl++)
						{
						if(pool[pl]->id==dummy_l)
							{
							squad[sq]->squad[pos]=pool[pl];
							}
						}
					}
				}

			ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
			squad[sq]->loot.resize(dummy);
			for(int l2=0;l2<squad[sq]->loot.size();l2++)
				{
				squad[sq]->loot[l2]=new itemst;
				ReadFile(h,squad[sq]->loot[l2],sizeof(itemst),&numbytes,NULL);
				}
			}

		activesquad=NULL;
		ReadFile(h,&dummy_c,sizeof(char),&numbytes,NULL);
		if(dummy_c)
			{
			ReadFile(h,&dummy_l,sizeof(long),&numbytes,NULL);
			for(int sq=0;sq<squad.size();sq++)
				{
				if(squad[sq]->id==dummy_l)
					{
					activesquad=squad[sq];
					break;
					}
				}
			}

		//DATES
		ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
		date.resize(dummy);
		for(int dt=0;dt<date.size();dt++)
			{
			date[dt]=new datest;

			ReadFile(h,&date[dt]->mac_id,sizeof(long),&numbytes,NULL);
			ReadFile(h,&date[dt]->timeleft,sizeof(short),&numbytes,NULL);

			ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
			date[dt]->date.resize(dummy);
			for(int dt2=0;dt2<date[dt]->date.size();dt2++)
				{
				date[dt]->date[dt2]=new creaturest;
				ReadFile(h,date[dt]->date[dt2],sizeof(creaturest),&numbytes,NULL);
				}
			}

		//NEWS STORIES
		ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
		newsstory.resize(dummy);
		for(int ns=0;ns<newsstory.size();ns++)
			{
			newsstory[ns]=new newsstoryst;

			ReadFile(h,&newsstory[ns]->type,sizeof(short),&numbytes,NULL);
			ReadFile(h,&newsstory[ns]->view,sizeof(short),&numbytes,NULL);

			ReadFile(h,&newsstory[ns]->loc,sizeof(long),&numbytes,NULL);
			ReadFile(h,&newsstory[ns]->priority,sizeof(long),&numbytes,NULL);
			ReadFile(h,&newsstory[ns]->page,sizeof(long),&numbytes,NULL);
			ReadFile(h,&newsstory[ns]->positive,sizeof(char),&numbytes,NULL);
			ReadFile(h,&newsstory[ns]->siegetype,sizeof(short),&numbytes,NULL);

			ReadFile(h,&dummy_c,sizeof(char),&numbytes,NULL);
			if(dummy_c)
				{
				WriteFile(h,&dummy_l,sizeof(long),&numbytes,NULL);
				for(int pl=0;pl<pool.size();pl++)
					{
					if(pool[pl]->id==dummy_l)
						{
						newsstory[ns]->cr=pool[pl];
						break;
						}
					}
				}

			ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
			newsstory[ns]->crime.resize(dummy);
			for(int dt2=0;dt2<newsstory[ns]->crime.size();dt2++)
				{
				ReadFile(h,&newsstory[ns]->crime[dt2],sizeof(int),&numbytes,NULL);
				}
			}

		CloseHandle(h);

		return 1;
		}

	return 0;
}

void choose_buyer(short &buyer)
{
	if(activesquad==NULL)return;

	party_status=-1;

	int partysize=0;
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)partysize++;
		}

	if(partysize<=1)return;

	do
		{
		printparty();

		move(8,20);
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		addstr("Choose a Liberal squad member to SPEND.");

		refresh();

		int c=getch();
		translategetch(c);

		if(c==10)return;

		if(c>='1'&&c<=partysize+'1'-1)
			{
			buyer=c-'1';
			return;
			}
		}while(1);
}

void conservatise(creaturest &cr)
{
	cr.align=-1;

	switch(cr.type)
		{
		case CREATURE_WORKER_FACTORY_UNION:
			strcpy(cr.name,"Ex-union Worker");
			break;
		case CREATURE_JUDGE_LIBERAL:
			strcpy(cr.name,"Jaded Liberal Judge");
			break;
		}
}

enum TalkModes
{
	TALKMODE_START,
	TALKMODE_RENTING,
	TALKMODE_ISSUES,
	TALKMODENUM
};

char talk(creaturest &a,int t)
{
	//BLUFFING
	if((sitealarm||location[cursite]->siege.siege)&&encounter[t].align==-1)
		{
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		if(location[cursite]->siege.siege)
			{
			addstr(a.name);
			addstr(" ");
			switch(location[cursite]->siege.siegetype)
				{
				case SIEGE_POLICE:
					addstr("pretends to be part of a police raid.");
					break;
				case SIEGE_CIA:
					addstr("pretends to be a Secret Agent.");
					break;
				case SIEGE_HICKS:
					switch(LCSrandom(2))
						{
						case 0:
							addstr("pretends to be Mountain");
							move(17,1);
							addstr("like Patrick Swayze in Next of Kin.");
							break;
						case 1:
							addstr("squeals like Ned Beatty");
							move(17,1);
							addstr("in Deliverance.");
							break;
						}
					break;
				case SIEGE_CORPORATE:
					addstr("pretends to be a mercenary.");
					break;
				}
			}
		else
			{
			addstr("talks like a Conservative");
			move(17,1);
			addstr("and pretends to belong here.");
			}

		refresh();
		getch();

		vector<int> noticer;
		for(int e=0;e<ENCMAX;e++)
			{
			if(encounter[e].exists&&encounter[e].alive&&
				encounter[e].align==-1)
				{
				noticer.push_back(e);
				}
			}

		if(noticer.size()>0)
			{
			int disguise=disguiseskill();

			char noticed=0,bluff=0;
			int n,an;

			do
				{
				an=LCSrandom(noticer.size());
				n=noticer[an];
				noticer.erase(noticer.begin() + an);

				int chance=encounter[n].attval(ATTRIBUTE_WISDOM)+
					encounter[n].attval(ATTRIBUTE_INTELLIGENCE);

				if(chance>(int)LCSrandom(21)+disguise)
					{
					noticed=1;
					break;
					}

				}while(noticer.size()>0);

			//NOW MUST BLUFF
			if(!noticed)
				{
				short aroll=LCSrandom(21)+a.attval(ATTRIBUTE_CHARISMA)+
					a.attval(ATTRIBUTE_WISDOM)+LCSrandom(a.skill[SKILL_PERSUASION]*2+1);

				int maxtroll=0,troll;
				for(int e=0;e<ENCMAX;e++)
					{
					if(encounter[e].exists&&encounter[e].alive&&
						encounter[e].align==-1)
						{
						troll=encounter[e].attval(ATTRIBUTE_WISDOM)+
							encounter[e].attval(ATTRIBUTE_INTELLIGENCE);
						if(troll>maxtroll)
							{
							n=e;
							maxtroll=troll;
							}
						}
					}

				maxtroll+=LCSrandom(21);
				a.skill_ip[SKILL_PERSUASION]+=(maxtroll>>2)+1;

				if(maxtroll>aroll)
					{
					clearmessagearea();

					set_color(COLOR_RED,COLOR_BLACK,1);
					move(16,1);
					if(location[cursite]->siege.siege&&
						location[cursite]->siege.siegetype==SIEGE_HICKS)
						{
						addstr("But ");
						addstr(encounter[n].name);
						addstr(" weren't born yesterday.");
						}
					else
						{
						addstr(encounter[n].name);
						addstr(" is not fooled by that crap.");
						}

					refresh();
					getch();
					}
				else
					{
					clearmessagearea();

					set_color(COLOR_GREEN,COLOR_BLACK,1);
					move(16,1);
					addstr("The Enemy is fooled and departs.");
					bluff=1;

					refresh();
					getch();
					}
				}
			else
				{
				clearmessagearea();

				if(!location[cursite]->siege.siege&&!sitealarm)
					{
					set_color(COLOR_RED,COLOR_BLACK,1);
					move(16,1);
					addstr(encounter[n].name);
					addstr(" looks at the Squad with Intolerance");
					move(17,1);
					addstr("and lets forth a piercing Conservative alarm cry!");

					sitealarm=1;
					}
				else
					{
					set_color(COLOR_RED,COLOR_BLACK,1);
					move(16,1);
					addstr(encounter[n].name);
					addstr(" looks at the Squad and isn't fooled.");
					}

				refresh();
				getch();
				}

			if(bluff)
				{
				for(int e=ENCMAX-1;e>=0;e--)
					{
					if(encounter[e].exists&&encounter[e].alive&&
						encounter[e].align==-1)
						{
						delenc(e,0);
						}
					}
				}
			}

		return 1;
		}

	//TALKING
	short talkmode=TALKMODE_START;

	creaturest *tk=&encounter[t];

	do
		{
		clearcommandarea();
		clearmessagearea();
		clearmaparea();

		int c='a';
		if(talkmode!=TALKMODE_ISSUES)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(9,1);
			addstr(a.name);
			addstr(" talks to ");
			addstr(tk->name);
			addstr(":");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			switch(talkmode)
				{
				case TALKMODE_RENTING:
					move(11,1);
					addstr("A - Accept.");
					move(12,1);
					addstr("B - Decline.");
					break;
				case TALKMODE_START:
					move(11,1);
					addstr("A - Strike up a conversation about the issues");
					if(a.armor.type==ARMOR_NONE)addstr(" while naked");
					addstr(".");
					move(12,1);
					if(!kid(*tk)&&!kid(a))set_color(COLOR_WHITE,COLOR_BLACK,0);
					else set_color(COLOR_BLACK,COLOR_BLACK,1);
					addstr("B - Drop a pickup line");
					if(a.armor.type==ARMOR_NONE)addstr(" while naked");
					addstr(".");
					set_color(COLOR_WHITE,COLOR_BLACK,0);
					move(13,1);
					addstr("C - On second thought, don't say anything");
					if(a.armor.type==ARMOR_NONE)addstr(" while naked");
					addstr(".");
					if(encounter[t].type==CREATURE_LANDLORD&&
						location[cursite]->renting==-1)
						{
						move(14,1);
						addstr("D - Rent a room");
						if(a.armor.type==ARMOR_NONE)addstr(" while naked");
						addstr(".");
						}
					break;
				}

			c=getch();
			translategetch(c);
			}

		if(c>='a'&&c<='g')
			{
			switch(talkmode)
				{
				case TALKMODE_ISSUES:
					{
					clearcommandarea();
					clearmessagearea();
					clearmaparea();

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(9,1);
					addstr("What issue will ");
					addstr(a.name);
					addstr(" discuss");
					if(a.armor.type==ARMOR_NONE)addstr(" while naked");
					addstr("?");

					int lw=-1;
					char str[50];
					int x=1,y=11;
					for(int l2=0;l2<LAWNUM;l2++)
						{
						if(law[l2]<2)set_color(COLOR_WHITE,COLOR_BLACK,1);
						else set_color(COLOR_BLACK,COLOR_BLACK,1);
						move(y,x);
						addch(l2+'A');
						addstr(" - ");
						getlaw(str,l2);
						addstr(str);

						y++;
						if(y==17)
							{
							y=11;
							x+=30;
							}
						}

					do
						{
						refresh();
						int c=getch();
						translategetch(c);
						if(c>='a'&&c<='z')
							{
							lw=c-'a';
							if(lw<LAWNUM)
								{
								if(law[lw]<2)break;
								}
							}
						if(c==10||c==27||c==32){lw=-1;break;}
						}while(1);

					if(lw!=-1)
						{
						short aroll=LCSrandom(21)+a.attval(ATTRIBUTE_CHARISMA)+
							a.attval(ATTRIBUTE_HEART)+LCSrandom(a.skill[SKILL_PERSUASION]*2+1);
						char badthing=0;
						if(aroll<15)badthing=1;
						if(a.armor.type==ARMOR_NONE)aroll-=30;
						short troll=LCSrandom(21)+tk->attval(ATTRIBUTE_CHARISMA)+
							tk->attval(ATTRIBUTE_WISDOM);
						a.skill_ip[SKILL_PERSUASION]+=LCSrandom(2)+1;

						clearcommandarea();clearmessagearea();clearmaparea();
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(9,1);addstr(a.name);addstr(" says,");
						set_color(COLOR_GREEN,COLOR_BLACK,1);
						int y=10;
						move(y,1);y++;
						if(badthing)
							{
							switch(lw)
								{
								case LAW_ABORTION:
									addstr("\"Conservatives make women turn to coat hangers.\"");
									break;
								case LAW_ANIMALRESEARCH:
									addstr("\"Richard Gere put a gerbil in his butt!\"");
									break;
								case LAW_POLICEBEHAVIOR:
									addstr("\"The cops suck!\"");
									break;
								case LAW_PRIVACY:
									addstr("\"The government, like, knows things about you.\"");
									break;
								case LAW_DEATHPENALTY:
									addstr("\"They executed this one dude, and like, his head caught on fire.\"");
									break;
								case LAW_NUCLEARPOWER:
									addstr("\"Have you seen Godzilla?  Nuclear power is bad, yo.\"");
									break;
								case LAW_POLLUTION:
									addstr("\"You wanna look like the Toxic Avenger?  Oppose pollution!\"");
									break;
								case LAW_LABOR:
									addstr("\"Bad people wanna make babies work and stuff.\"");
									break;
								case LAW_GAY:
									addstr("\"Lots of people don't like homosexuals.\"");
									break;
								case LAW_CORPORATE:
									addstr("\"The corporations are putting you down, dude.\"");
									break;
								case LAW_FREESPEECH:
									addstr("\"Better watch what you say.  They've got ears everywhere.\"");
									break;
								case LAW_TAX:
									addstr("\"Rich people, like, have money, man.\"");
									break;
								case LAW_FLAGBURNING:
									addstr("\"The flag is stupid.\"");
									break;
								}
							}
						else
							{
							switch(lw)
								{
								case LAW_ABORTION:
									addstr("\"The government is systematically trying to rob women of the right");
									move(y,1);y++;
									addstr("to control their own destinies.\"");
									break;
								case LAW_ANIMALRESEARCH:
									addstr("\"Animals are routinely subjected to inhumane treatment in labs in this");
									move(y,1);y++;
									addstr("country.\"");
									break;
								case LAW_POLICEBEHAVIOR:
									addstr("\"The police regularly torture minority suspects during interrogations.\"");
									break;
								case LAW_PRIVACY:
									addstr("\"Files are being kept on innocent citizens whose only crime is to");
									move(y,1);y++;
									addstr("speak out against a system that is trying to farm them like beasts.\"");
									break;
								case LAW_DEATHPENALTY:
									addstr("\"Over thirty innocent people have been executed over the past decade.\"");
									break;
								case LAW_NUCLEARPOWER:
									addstr("\"Radioactive waste is being stored all over the country, and it poses");
									move(y,1);y++;
									addstr("a serious threat to many families, even in this neighborhood.\"");
									break;
								case LAW_POLLUTION:
									addstr("\"Industries that stop at nothing to become more profitable are polluting");
									move(y,1);y++;
									addstr("the environment in ways that hurt not only animals, but people too.");
									break;
								case LAW_LABOR:
									addstr("\"Have you noticed how people are working more and more hours for less and");
									move(y,1);y++;
									addstr("less money?  It's all part of a plan to keep you enslaved, man.\"");
									break;
								case LAW_GAY:
									addstr("\"Homosexuals are people like anyone else, and yet they are treated in this");
									move(y,1);y++;
									addstr("country as if they are deviants fit only for cheap entertainment.\"");
									break;
								case LAW_CORPORATE:
									addstr("\"Corporate executives use giant corporations as a means to become parasites");
									move(y,1);y++;
									addstr("that suck wealth out of this country and put it into their pockets.\"");
									break;
								case LAW_FREESPEECH:
									addstr("\"Protests and demonstrations are regularly and often brutally suppressed in\"");
									move(y,1);y++;
									addstr("this country.  People have to watch what they write -- even what they read.\"");
									break;
								case LAW_TAX:
									addstr("\"The tax code has been designed to perpetuate an unjust class");
									move(y,1);y++;
									addstr("structure that is keeping you oppressed.\"");
									break;
								case LAW_FLAGBURNING:
									addstr("\"Burning a piece of cloth is actually stigmatized in this country.");
									move(y,1);y++;
									addstr("You love freedom and still hate our government stands for.\"");
									break;
								}
							}
						refresh();
						getch();

						if(aroll>troll)
							{
							y++;
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(y,1);y++;addstr(tk->name);addstr(" responds,");
							set_color(COLOR_BLUE,COLOR_BLACK,1);
							move(y,1);y++;
							if(tk->type==CREATURE_MUTANT&&
								tk->attval(ATTRIBUTE_INTELLIGENCE)<3)
								{
								addstr("\"Aaaahhh...\"");
								}
							else addstr("\"That *is* disturbing!  What can I do?\"");
							refresh();
							getch();

							y++;move(y,1);y++;
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							addstr("After more discussion, ");
							addstr(tk->name);
							addstr(" agrees to come by later tonight.");
							refresh();
							getch();

							creaturest *newcr=new creaturest;
								*newcr=*tk;
							namecreature(*newcr);

							newcr->location=a.location;
							newcr->base=a.base;
							newcr->hireid=a.id;
							newcr->align=1;

							pool.push_back(newcr);
							stat_recruits++;

							delenc(t,0);
							return 1;
							}
						else
							{
							y++;
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(y,1);y++;addstr(tk->name);addstr(" responds,");
							set_color(COLOR_BLUE,COLOR_BLACK,1);
							move(y,1);y++;
							if(tk->type==CREATURE_MUTANT&&
								tk->attval(ATTRIBUTE_INTELLIGENCE)<3)
								{
								addstr("\"Ugh.  Pfft.\"");
								}
							else addstr("\"Whatever.\"");
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							addstr(" <turns away>");
							refresh();
							getch();

							encounter[t].cantbluff=1;
							return 1;
							}
						}
					else
						{
						clearcommandarea();clearmessagearea();clearmaparea();
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(9,1);addstr(a.name);addstr(" says,");
						set_color(COLOR_GREEN,COLOR_BLACK,1);
						move(10,1);addstr("\"Nevermind.\"");
						refresh();
						getch();

						return 1;
						}
					break;
					}
				case TALKMODE_RENTING:
					if(c=='a')
						{
						clearcommandarea();clearmessagearea();clearmaparea();
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(9,1);addstr(a.name);addstr(" says,");
						set_color(COLOR_GREEN,COLOR_BLACK,1);
						move(10,1);
						addstr("\"I'll take it.\"");
						refresh();
						getch();

						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(12,1);addstr(tk->name);addstr(" responds,");
						set_color(COLOR_BLUE,COLOR_BLACK,1);
						move(13,1);
						addstr("\"Rent is due by the third of every month.");
						move(14,1);
						addstr("We'll start next month.\"");
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						addstr(" <turns away>");
						refresh();
						getch();

						long rent=200;
						switch(location[cursite]->type)
							{
							case SITE_RESIDENTIAL_APARTMENT:rent=650;break;
							case SITE_RESIDENTIAL_APARTMENT_UPSCALE:rent=1500;break;
							}

						funds-=rent;
						stat_spent+=rent;
						location[cursite]->renting=rent;
						location[cursite]->newrental=1;
						moneylost_rent+=rent;

						basesquad(activesquad,cursite);
						return 1;
						}
					if(c=='b')
						{
						clearcommandarea();clearmessagearea();clearmaparea();
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(9,1);addstr(a.name);addstr(" says,");
						set_color(COLOR_GREEN,COLOR_BLACK,1);
						move(10,1);
						addstr("\"Whoa, I was looking for something cheaper.\"");
						refresh();
						getch();

						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(12,1);addstr(tk->name);addstr(" responds,");
						set_color(COLOR_BLUE,COLOR_BLACK,1);
						move(13,1);
						addstr("\"Not my problem...\"");
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						addstr(" <turns away>");
						refresh();
						getch();

						return 1;
						}
					break;
				case TALKMODE_START:
					if(c=='a')
						{
						clearcommandarea();clearmessagearea();clearmaparea();

						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(9,1);addstr(a.name);addstr(" says,");
						set_color(COLOR_GREEN,COLOR_BLACK,1);
						move(10,1);
						addstr("\"Do you want to hear something disturbing?\"");
						refresh();
						getch();

						if(talkreceptive(*tk))
							{
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(12,1);addstr(tk->name);addstr(" responds,");
							set_color(COLOR_BLUE,COLOR_BLACK,1);
							move(13,1);
							addstr("\"What?\"");
							refresh();
							getch();

							talkmode=TALKMODE_ISSUES;
							break;
							}
						else
							{
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(12,1);addstr(tk->name);addstr(" responds,");
							set_color(COLOR_BLUE,COLOR_BLACK,1);
							move(13,1);
							addstr("\"No.\"");
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							addstr(" <turns away>");
							refresh();
							getch();

							return 1;
							}
						}
					if(c=='b'&&!kid(*tk)&&!kid(a))
						{
						int y=12;
						clearcommandarea();clearmessagearea();clearmaparea();
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(9,1);addstr(a.name);addstr(" says,");
						set_color(COLOR_GREEN,COLOR_BLACK,1);
						move(10,1);
						int line;
						if(law[LAW_FREESPEECH]==-2)
							{
							line=LCSrandom(3);
							switch(line)
								{
								case 0:addstr("\"[What church do you go to?]\"");break;
								case 1:addstr("\"[Will you marry me?]\"");break;
								case 2:addstr("\"[Do you believe in abstinence education?]\"");break;
								}
							}
						else
							{
							line=LCSrandom(44);
						switch(line)
							{
//LIMIT         :-----------------------------------------------------------------------------:
case 0:addstr("\"Hey baby, you're kinda ugly.  I like that.\"");break;
case 1:addstr("\"I lost my phone number.  Could I have yours?\"");break;
case 2:addstr("\"Hey, you wanna go rub one off?\"");break;
case 3:addstr("\"Hot damn.  You're built like a brick shithouse, honey.\"");break;
case 4:
	   addstr("\"I know I've seen you on the back of a milk carton,");
	   move(11,1);y++;
	   addstr("cuz you've been missing from my life.\"");
	   break;
case 5:addstr("\"I'm big where it counts.\"");break;
case 6:addstr("\"Daaaaaamn girl, I want to wrap your legs around my face and");
	   move(11,1);y++;
	   addstr("wear you like a feed bag!\"");
	   break;
case 7:addstr("\"Let's play squirrel.  I'll bust a nut in your hole.\"");break;
case 8:addstr("\"You know, if I were you, I'd have sex with me.\"");break;
case 9:addstr("\"You don't sweat much for a fat chick.\"");break;
case 10:addstr("\"Fuck me if I'm wrong but you want to kiss me, right?\"");break;
case 11:addstr("\"Your parents must be retarded, because you are special.\"");break;
case 12:addstr("\"Let's play trains...  you can sit on my face and I will chew chew chew.\"");break;
case 13:addstr("\"Is it hot in here or is it just you?\"");break;
case 14:addstr("\"I may not be Fred Flintstone, but I can make your bed rock!\"");break;
case 15:addstr("\"What do you say we go behind a rock and get a little boulder?\"");break;
case 16:addstr("\"Do you have stars on your panties?  Your ass is outta this world!\"");break;
case 17:addstr("\"Those pants would look great on the floor of my bedroom.\"");break;
case 18:addstr("\"If I said you had a nice body, would you hold it against me?\"");break;
case 19:addstr("\"Are you tired?  You've been running around in my thoughts all day.\"");break;
case 20:addstr("\"If I could change the alphabet baby, I would put the U and I together!\"");break;
case 21:addstr("\"Your lips look sweet.  Can I taste them?\"");break;
case 22:addstr("\"Nice shoes.  Wanna fuck?\"");break;
case 23:addstr("\"Your sexuality makes me nervous and this frustrates me.\"");break;
case 24:addstr("\"Are you Jamaican?  Cuz Jamaican me horny.\"");break;
case 25:addstr("\"Hey pop tart, fancy coming in my toaster of love?\"");break;
case 26:addstr("\"Wanna play army?  You lie down and I'll blow you away.\"");break;
case 27:addstr("\"Can I lick your forehead?\"");break;
case 28:addstr("\"I have a genital rash.  Will you rub this ointment on me?\"");break;
case 29:addstr("\"What's your sign?\"");break;
case 30:addstr("\"Do you work for the post office?");
		move(11,1);y++;
		addstr("Because I could have sworn you were checking out my package.\"");
		break;
case 31:addstr("\"I'm not the most attractive person in here,");
		move(11,1);y++;
		addstr("but I'm the only one talking to you.\"");
		break;
case 32:addstr("\"Hi.  I suffer from amnesia.  Do I come here often?\"");break;
case 33:addstr("\"I'm new in town.  Could you give me directions to your apartment?\"");break;
case 34:addstr("\"Stand still so I can pick you up!\"");break;
case 35:addstr("\"Your daddy must have been a baker, cuz you've got a nice set of buns.\"");break;
case 36:addstr("\"If you were a laser, you'd be set on 'stunning'.\"");break;
case 37:addstr("\"Is that a keg in your pants?  Cuz I'd love to tap that ass.\"");break;
case 38:addstr("\"If I could be anything, I'd love to be your bathwater.\"");break;
case 39:addstr("\"Stop, drop and roll, baby.  You are on fire.\"");break;
case 40:addstr("\"Do you want to see something swell?\"");break;
case 41:addstr("\"Excuse me.  Do you want to fuck or should I apologize?\"");break;
case 42:addstr("\"Say, did we go to different schools together?\"");break;
case 43:addstr("\"You smell...  Let's go take a shower.\"");break;
							}
							}
						refresh();
						getch();

						short aroll=LCSrandom(21)+a.attval(ATTRIBUTE_CHARISMA)*2+LCSrandom(a.skill[SKILL_PERSUASION]*2+1);
						if(a.armor.type==ARMOR_NONE)aroll-=30;
						short troll=LCSrandom(21)+tk->attval(ATTRIBUTE_CHARISMA)+tk->attval(ATTRIBUTE_WISDOM);
						a.skill_ip[SKILL_PERSUASION]+=LCSrandom(2)+1;

						if(aroll>troll)
							{
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(y,1);y++;addstr(tk->name);addstr(" responds,");
							set_color(COLOR_BLUE,COLOR_BLACK,1);
							move(y,1);y++;

							if(law[LAW_FREESPEECH]==-2)
								{
							switch(line)
								{
								case 0:addstr("\"[I go to your church.]\"");break;
								case 1:addstr("\"[Yes.]\"");break;
								case 2:addstr("\"[Yes.  Yes, I do.]\"");break;
								}
								}
							else
							{
						switch(line)
							{
//LIMIT         :-----------------------------------------------------------------------------:
case 0:addstr("\"You're not so cute yourself.  Wanna get a room?\"");break;
case 1:addstr("\"How sweet!  You can call me tonight...\"");break;
case 2:addstr("\"You bet, baby.\"");break;
case 3:addstr("\"He he, I'll let that one slide.  Besides, I like country folk...\"");break;
case 4:addstr("\"That's sick.  I can do sick tonight.\"");break;
case 5:addstr("\"Oooo, let me see!\"");break;
case 6:addstr("\"Wow, looks like I'm going to have to reward creativity tonight!\"");break;
case 7:addstr("\"Winter's coming.  You'd better bust more than one.\"");break;
case 8:addstr("\"But you're not, so the pleasure's all mine.\"");break;
case 9:addstr("\"Just wait until tonight, baby.\"");break;
case 10:addstr("\"You're wrong.\"");break;
case 11:addstr("\"I can drool on you if you like it that way.\"");break;
case 12:addstr("\"Oooo, all aboard baby!\"");break;
case 13:addstr("\"Not as hot as we'll be tonight you slut.\"");break;
case 14:addstr("\"Goober.  You wanna hook up tonight?\"");break;
case 15:addstr("\"Oooo, we should get stoned too!  He he.\"");break;
case 16:addstr("\"You'll have to whip out your rocket to get some.  Let's do it.\"");break;
case 17:addstr("\"So would my underwear.\"");break;
case 18:addstr("\"Yeah, and you're going to repay me tonight.\"");break;
case 19:addstr("\"Then stop *thinking* about it and come over tonight.\"");break;
case 20:addstr("\"As long as you put a condom between them, I'm all for it.\"");break;
case 21:addstr("\"Sure, but you can't use your mouth.\"");break;
case 22:addstr("\"I hope you don't have a foot fetish, but I'm game.\"");break;
case 23:addstr("\"My sex could do even more.\"");break;
case 24:addstr("\"Let me invite you to visit my island paradise.  Tonight.\"");break;
case 25:addstr("\"Oh, man...  just don't tell anybody I'm seeing you.\"");break;
case 26:addstr("\"I hope we're shooting blanks, soldier.  I'm out of condoms.\"");break;
case 27:addstr("\"You can lick all my decals off, baby.\"");break;
case 28:addstr("\"Only if I'm not allowed to use my hands.\"");break;
case 29:addstr("\"The one that says 'Open All Night'.\"");break;
case 30:addstr("\"It looks like a letter bomb to me.  Let me blow it up.\"");break;
case 31:addstr("\"Hey, I could do better.  But I'm feeling cheap tonight.\"");break;
case 32:addstr("\"Yeah.  I hope you remember the lube this time.\"");break;
case 33:addstr("\"But if we use a hotel, you won't get shot by an angry spouse tonight.\"");break;
case 34:addstr("\"I think you'll appreciate the way I move after tonight.\"");break;
case 35:addstr("\"They make a yummy bedtime snack.\"");break;
case 36:addstr("\"Oh..  oh, God.  I can't believe I'm going to date a Trekkie.\"");break;
case 37:addstr("\"Oh, it isn't safe for you to drive like that.  You'd better stay the night.\"");break;
case 38:addstr("\"Come over tonight and I can show you what it's like.\"");break;
case 39:addstr("\"I'll stop, drop and roll if you do it with me.\"");break;
case 40:addstr("\"I'd rather feel something swell.\"");break;
case 41:addstr("\"You can apologize later if it isn't any good.\"");break;
case 42:addstr("\"Yeah, and we tonight can try different positions together.\"");break;
case 43:addstr("\"Don't you like it dirty?\"");break;
							}
							}
							refresh();
							getch();

							y++;
							move(y,1);
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(y,1);y++;
							addstr(a.name);
							addstr(" and ");
							addstr(tk->name);
							addstr(" make plans for tonight");
							if(tk->type==CREATURE_PRISONER)
								{
								addstr(", and ");
								move(y,1);y++;
								addstr(tk->name);
								addstr(" breaks for the exit");
								tk->lawflag|=LAWFLAG_ESCAPED;
								}
							addstr(".  ");
							refresh();
							getch();

							int olddate=0;
							datest *newd=NULL;
							for(int d=0;d<date.size();d++)
								{
								if(date[d]->mac_id==a.id)
									{
									newd=date[d];
									break;
									}
								}

							if(newd==NULL)
								{
								newd=new datest;
									newd->mac_id=a.id;
								date.push_back(newd);
								}

							creaturest *newcr=new creaturest;
							*newcr=*tk;

							newcr->location=a.location;
							newcr->base=a.base;

							newd->date.push_back(newcr);

							delenc(t,0);
							}
						else
							{
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(y,1);y++;addstr(tk->name);addstr(" responds,");
							set_color(COLOR_BLUE,COLOR_BLACK,1);
							move(y,1);y++;
							addstr("\"Jesus...\"");
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							addstr(" <turns away>");
							refresh();
							getch();

							encounter[t].cantbluff=1;
							}

						return 1;
						}
					if(c=='c')return 0;
					if(encounter[t].type==CREATURE_LANDLORD&&
						location[cursite]->renting==-1)
						{
						if(c=='d')
							{
							clearcommandarea();clearmessagearea();clearmaparea();
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(9,1);addstr(a.name);addstr(" says,");
							set_color(COLOR_GREEN,COLOR_BLACK,1);
							move(10,1);
							addstr("\"I'd like to rent a room.\"");
							refresh();
							getch();

							if(a.armor.type==ARMOR_NONE)
								{
								set_color(COLOR_WHITE,COLOR_BLACK,1);
								move(12,1);addstr(tk->name);addstr(" responds,");
								set_color(COLOR_BLUE,COLOR_BLACK,1);
								move(13,1);
								addstr("\"Put some clothes on before I call the cops.\"");
								refresh();
								getch();
								return 1;
								}

							long rent=200;
							switch(location[cursite]->type)
								{
								case SITE_RESIDENTIAL_APARTMENT:rent=650;break;
								case SITE_RESIDENTIAL_APARTMENT_UPSCALE:rent=1500;break;
								}

							char num[20];
							itoa(rent,num,10);

							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(12,1);addstr(tk->name);addstr(" responds,");
							set_color(COLOR_BLUE,COLOR_BLACK,1);
							move(13,1);
							addstr("\"It'll be $");
							addstr(num);
							addstr(" a month.");
							move(14,1);
							addstr("I'll need $");
							addstr(num);
							addstr(" now as a security deposit.\"");
							refresh();
							getch();

							if(funds>=rent)
								{
								talkmode=TALKMODE_RENTING;
								}
							else
								{
								clearcommandarea();clearmessagearea();clearmaparea();
								set_color(COLOR_WHITE,COLOR_BLACK,1);
								move(9,1);addstr(a.name);addstr(" says,");
								set_color(COLOR_GREEN,COLOR_BLACK,1);
								move(10,1);
								addstr("\"Whoa, I was looking for something cheaper.\"");
								refresh();
								getch();

								set_color(COLOR_WHITE,COLOR_BLACK,1);
								move(12,1);addstr(tk->name);addstr(" responds,");
								set_color(COLOR_BLUE,COLOR_BLACK,1);
								move(13,1);
								addstr("\"Not my problem...\"");
								set_color(COLOR_WHITE,COLOR_BLACK,1);
								addstr(" <turns away>");
								refresh();
								getch();
								}
							}
						}
					break;
				}
			}
		}while(1);
}

void liberalize(creaturest &cr)
{
	cr.align=1;

	switch(cr.type)
		{
		case CREATURE_WORKER_FACTORY_NONUNION:
			strcpy(cr.name,"New Union Worker");
			break;
		}
}

void kidnapattempt(void)
{
	short kidnapper=-1;

	party_status=-1;

	int available=0;
	char availslot[6]={0,0,0,0,0,0};
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(activesquad->squad[p]->alive&&activesquad->squad[p]->prisoner==NULL)
				{
				available++;
				availslot[p]=1;
				}
			}
		}

	if(!available)
		{
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("No one can do the job.                                  ");
		move(17,1);
		addstr("                                                        ");

		refresh();
		getch();

		return;
		}

	do
		{
		printparty();

		move(8,20);
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		addstr("Choose a Liberal squad member to do the job.");

		refresh();

		int c=getch();
		translategetch(c);

		if(c==10)return;

		if(c>='1'&&c<='6')
			{
			if(availslot[c-'1'])
				{
				kidnapper=c-'1';
				break;
				}
			}

		}while(1);

	if(kidnapper>=0)
		{
		vector<int> target;

		for(int e=0;e<ENCMAX;e++)
			{
			if(encounter[e].exists&&encounter[e].alive&&encounter[e].align==-1&&!encounter[e].animalgloss)
				{
				if(encounter[e].weapon.type!=WEAPON_NONE&&
					encounter[e].weapon.type!=WEAPON_SYRINGE&&
					encounter[e].weapon.type!=WEAPON_GAVEL&&
					encounter[e].blood>20)continue;
				target.push_back(e);
				}
			}

		if(target.size()>0)
			{
			int t=target[0];

			if(target.size()>1)
				{
				do
					{
					clearcommandarea();
					clearmessagearea();
					clearmaparea();

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(9,1);
					addstr("Kidnap whom?");

					int x=1,y=11;
					for(int t2=0;t2<target.size();t2++)
						{
						move(y,x);
						addch(t2+'A');
						addstr(" - ");
						addstr(encounter[target[t2]].name);

						y++;
						if(y==17)
							{
							y=11;
							x+=30;
							}
						}

					refresh();
					int c=getch();
					translategetch(c);
					if(c>='a'&&c<='z')
						{
						t=c-'a';
						if(t>=target.size())t=-1;
						else {t=target[t];}break;
						}
					if(c==10||c==27||c==32){t=-1;break;}
					}while(1);
				}

			if(t!=-1)
				{
				char amateur=0;

				if(kidnap(*activesquad->squad[kidnapper],encounter[t],amateur))
					{
					delenc(t,0);

					int time=20+LCSrandom(10);
					if(time<1)time=1;
					if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;
					}
				else sitealarm=1;

				if(amateur)
					{
					char present=0;
					for(int e=0;e<ENCMAX;e++)
						{
						if(encounter[e].exists&&encounter[e].alive){present=1;break;}
						}

					if(present)
						{
						alienationcheck(1);
						sitecrime+=5;
						criminalizeparty(LAWFLAG_KIDNAPPER);
						if(activesquad->squad[kidnapper]->prisoner!=NULL)
							{
							if(activesquad->squad[kidnapper]->prisoner->type==CREATURE_RADIOPERSONALITY)offended_amradio=1;
							if(activesquad->squad[kidnapper]->prisoner->type==CREATURE_NEWSANCHOR)offended_cablenews=1;
							}
						else
							{
							if(encounter[t].type==CREATURE_RADIOPERSONALITY)offended_amradio=1;
							if(encounter[t].type==CREATURE_NEWSANCHOR)offended_cablenews=1;
							}
						}
					}

				if(sitealarm)enemyattack();
				creatureadvance();
				}
			}
		else
			{
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(16,1);
			addstr("All of the targets are too dangerous.                   ");
			move(17,1);
			addstr("                                                        ");

			refresh();
			getch();
			}
		}
	else
		{
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("No one can do the job.                                  ");
		move(17,1);
		addstr("                                                        ");

		refresh();
		getch();
		}
}

char kidnap(creaturest &a,creaturest &t,char &amateur)
{
	if(a.weapon.type==WEAPON_NONE)
		{
		amateur=1;

		//BASIC ROLL
		int aroll=LCSrandom(15)+1+LCSrandom(a.attval(ATTRIBUTE_AGILITY));
		int droll=LCSrandom(20)+1+LCSrandom(t.attval(ATTRIBUTE_AGILITY));

		aroll+=LCSrandom(a.skill[SKILL_HANDTOHAND]+1);
		a.skill_ip[SKILL_HANDTOHAND]+=droll;

		clearmessagearea();

		//HIT!
		if(aroll>droll)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(16,1);
			addstr(a.name);
			addstr(" snatches ");
			addstr(t.name);
			addstr("!");

			a.prisoner=new creaturest;
			*a.prisoner=t;

			refresh();
			getch();

			set_color(COLOR_RED,COLOR_BLACK,1);
			move(17,1);
			addstr(t.name);
			addstr(" is struggling and screaming!");

			refresh();
			getch();

			return 1;
			}
		else
			{
			set_color(COLOR_MAGENTA,COLOR_BLACK,1);
			move(16,1);
			addstr(a.name);
			addstr(" grabs at ");
			addstr(t.name);
			move(17,1);
			addstr("but ");
			addstr(t.name);
			addstr(" writhes away!");

			refresh();
			getch();

			return 0;
			}
		}
	else
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr(a.name);
		addstr(" shows ");
		addstr(t.name);
		addstr(" the ");
		char str[30];
		getweaponfull(str,a.weapon.type);
		addstr(str);
		move(17,1);
		addstr("and says, ");
		set_color(COLOR_GREEN,COLOR_BLACK,1);
		if(law[LAW_FREESPEECH]==-2)addstr("\"[Please], be cool.\"");
		else addstr("\"Bitch, be cool.\"");

		a.prisoner=new creaturest;
		*a.prisoner=t;

		refresh();
		getch();

		return 1;
		}
}

void kidnaptransfer(creaturest &cr)
{
	creaturest *newcr=new creaturest;
		*newcr=cr;
	namecreature(*newcr);

	newcr->location=activesquad->squad[0]->location;
	newcr->base=activesquad->squad[0]->base;
	newcr->flag|=CREATUREFLAG_MISSING;

	erase();

	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(0,0);
	addstr("The Education of ");
	addstr(newcr->propername);

	move(2,0);
	set_color(COLOR_WHITE,COLOR_BLACK,0);
	addstr("What name will you use for this ");
	char str[80];
	getrecruitcreature(str,newcr->type);
	addstr(str);
	addstr(" in its presence?");

	keypad(stdscr,FALSE);
	raw_output(FALSE);
	echo();
	curs_set(1);
	mvgetstr(3,0,newcr->name);
	curs_set(0);
	noecho();
	raw_output(TRUE);
	keypad(stdscr,TRUE);

	pool.push_back(newcr);
	stat_kidnappings++;
}

void namecreature(creaturest &cr)
{
	name(cr.name);
	strcpy(cr.propername,cr.name);
}

void name(char *str)
{
	strcpy(str,"");

	char first[80];
	firstname(first);
	strcat(str,first);

	strcat(str," ");

	char last[80];
	lastname(last);
	strcat(str,last);
}

void firstname(char *str)
{
	strcpy(str,"");

	switch(LCSrandom(100))
		{
		case 0:strcat(str,"Ryan");break;
		case 1:strcat(str,"Sergio");break;
		case 2:strcat(str,"Laura");break;
		case 3:strcat(str,"Anne");break;
		case 4:strcat(str,"Bill");break;
		case 5:strcat(str,"James");break;
		case 6:strcat(str,"Marty");break;
		case 7:strcat(str,"Jessica");break;
		case 8:strcat(str,"Lisa");break;
		case 9:strcat(str,"Bonita");break;
		case 10:strcat(str,"Angel");break;
		case 11:strcat(str,"Pat");break;
		case 12:strcat(str,"Toshiro");break;
		case 13:strcat(str,"Yan-ping");break;
		case 14:strcat(str,"Tetsuo");break;
		case 15:strcat(str,"Akira");break;
		case 16:strcat(str,"Jimmy");break;
		case 17:strcat(str,"Carlos");break;
		case 18:strcat(str,"William");break;
		case 19:strcat(str,"Billy Bob");break;
		case 20:strcat(str,"Carol");break;
		case 21:strcat(str,"Jenny");break;
		case 22:strcat(str,"Jennifer");break;
		case 23:strcat(str,"Manuela");break;
		case 24:strcat(str,"Douglas");break;
		case 25:strcat(str,"Kristin");break;
		case 26:strcat(str,"Steven");break;
		case 27:strcat(str,"Bonnie");break;
		case 28:strcat(str,"Howard");break;
		case 29:strcat(str,"Donald");break;
		case 30:strcat(str,"Barry");break;
		case 31:strcat(str,"Thomas");break;
		case 32:strcat(str,"Joann");break;
		case 33:strcat(str,"Derek");break;
		case 34:strcat(str,"Gary");break;
		case 35:strcat(str,"Archie");break;
		case 36:strcat(str,"Mayumi");break;
		case 37:strcat(str,"Felicia");break;
		case 38:strcat(str,"Sherry");break;
		case 39:strcat(str,"Judy");break;
		case 40:strcat(str,"Elinor");break;
		case 41:strcat(str,"Ned");break;
		case 42:strcat(str,"Randy");break;
		case 43:strcat(str,"Taylor");break;
		case 44:strcat(str,"Kim");break;
		case 45:strcat(str,"Ruthanne");break;
		case 46:strcat(str,"Roger");break;
		case 47:strcat(str,"Raymond");break;
		case 48:strcat(str,"Harvey");break;
		case 49:strcat(str,"Robert");break;
		case 50:strcat(str,"Michael");break;
		case 51:strcat(str,"Aaron");break;
		case 52:strcat(str,"George");break;
		case 53:strcat(str,"Noel");break;
		case 54:strcat(str,"Adrienne");break;
		case 55:strcat(str,"Lex");break;
		case 56:strcat(str,"Linda");break;
		case 57:strcat(str,"Chuck");break;
		case 58:strcat(str,"Charlie");break;
		case 59:strcat(str,"Charles");break;
		case 60:strcat(str,"Malcolm");break;
		case 61:strcat(str,"Martin");break;
		case 62:strcat(str,"Sean");break;
		case 63:strcat(str,"Raven");break;
		case 64:strcat(str,"Wolf");break;
		case 65:strcat(str,"Miguel");break;
		case 66:strcat(str,"Pablo");break;
		case 67:strcat(str,"Paul");break;
		case 68:strcat(str,"Jesus");break;
		case 69:strcat(str,"Ali");break;
		case 70:strcat(str,"Ingrid");break;
		case 71:strcat(str,"Kweisi");break;
		case 72:strcat(str,"Susanna");break;
		case 73:strcat(str,"Sharon");break;
		case 74:strcat(str,"Marion");break;
		case 75:strcat(str,"Kathy");break;
		case 76:strcat(str,"Bruce");break;
		case 77:strcat(str,"Dick");break;
		case 78:strcat(str,"Phillip");break;
		case 79:strcat(str,"Kirk");break;
		case 80:strcat(str,"Kurt");break;
		case 81:strcat(str,"John");break;
		case 82:strcat(str,"Alexander");break;
		case 83:strcat(str,"David");break;
		case 84:strcat(str,"Beau");break;
		case 85:strcat(str,"Elsie");break;
		case 86:strcat(str,"Satya");break;
		case 87:strcat(str,"Mumtaz");break;
		case 88:strcat(str,"Diwakar");break;
		case 89:strcat(str,"Dale");break;
		case 90:strcat(str,"Woody");break;
		case 91:strcat(str,"Ariel");break;
		case 92:strcat(str,"Hans");break;
		case 93:strcat(str,"Barbara");break;
		case 94:strcat(str,"Jun");break;
		case 95:strcat(str,"Rosemary");break;
		case 96:strcat(str,"Chin-Yuan");break;
		case 97:strcat(str,"Aiko");break;
		case 98:strcat(str,"Vithara");break;
		case 99:strcat(str,"Deepak");break;
		}
}

void lastname(char *str)
{
	strcpy(str,"");

	switch(LCSrandom(100))
		{
		case 0:strcat(str,"King");break;
		case 1:strcat(str,"Lewis");break;
		case 2:strcat(str,"Black");break;
		case 3:strcat(str,"White");break;
		case 4:strcat(str,"Ames");break;
		case 5:strcat(str,"Warner");break;
		case 6:strcat(str,"Simpson");break;
		case 7:strcat(str,"Parker");break;
		case 8:strcat(str,"Suave");break;
		case 9:strcat(str,"Mifune");break;
		case 10:strcat(str,"Gu");break;
		case 11:strcat(str,"Bolger");break;
		case 12:strcat(str,"Ross");break;
		case 13:strcat(str,"Ramirez");break;
		case 14:strcat(str,"Kurosawa");break;
		case 15:strcat(str,"Johnson");break;
		case 16:strcat(str,"Buchanan");break;
		case 17:strcat(str,"Adamson");break;
		case 18:strcat(str,"Hendrix");break;
		case 19:strcat(str,"Rojo");break;
		case 20:strcat(str,"Villa");break;
		case 21:strcat(str,"Fields");break;
		case 22:strcat(str,"Templeton");break;
		case 23:strcat(str,"Ivanson");break;
		case 24:strcat(str,"Blitzer");break;
		case 25:strcat(str,"Muhammed");break;
		case 26:strcat(str,"Stone");break;
		case 27:strcat(str,"Cho");break;
		case 28:strcat(str,"Childress");break;
		case 29:strcat(str,"Africa");break;
		case 30:strcat(str,"Balgos");break;
		case 31:strcat(str,"Baird");break;
		case 32:strcat(str,"Bailey");break;
		case 33:strcat(str,"Diaz");break;
		case 34:strcat(str,"Decker");break;
		case 35:strcat(str,"Ericson");break;
		case 36:strcat(str,"Loeb");break;
		case 37:strcat(str,"Meffert");break;
		case 38:strcat(str,"McLeod");break;
		case 39:strcat(str,"Tucker");break;
		case 40:strcat(str,"Takayoshi");break;
		case 41:strcat(str,"Tanner");break;
		case 42:strcat(str,"Lipman");break;
		case 43:strcat(str,"Little");break;
		case 44:strcat(str,"Logsdon");break;
		case 45:strcat(str,"Krasow");break;
		case 46:strcat(str,"Krieger");break;
		case 47:strcat(str,"Dahmer");break;
		case 48:strcat(str,"Gacy");break;
		case 49:strcat(str,"Krishna");break;
		case 50:strcat(str,"la Russa");break;
		case 51:strcat(str,"Savedra");break;
		case 52:strcat(str,"Scardino");break;
		case 53:strcat(str,"Keitel");break;
		case 54:strcat(str,"Wallace");break;
		case 55:strcat(str,"Buckman");break;
		case 56:strcat(str,"Fulsom");break;
		case 57:strcat(str,"Smith");break;
		case 58:strcat(str,"Venus");break;
		case 59:strcat(str,"Straley");break;
		case 60:strcat(str,"Purcell");break;
		case 61:strcat(str,"al Fadil");break;
		case 62:strcat(str,"Storm");break;
		case 63:strcat(str,"Patterson");break;
		case 64:strcat(str,"Pelton");break;
		case 65:strcat(str,"Ng");break;
		case 66:strcat(str,"Filler");break;
		case 67:strcat(str,"Buttman");break;
		case 68:strcat(str,"Fingleton");break;
		case 69:strcat(str,"Fenoglio");break;
		case 70:strcat(str,"de la Cruz");break;
		case 71:strcat(str,"Delgado");break;
		case 72:strcat(str,"Hatcher");break;
		case 73:strcat(str,"Jameson");break;
		case 74:strcat(str,"Franklin");break;
		case 75:strcat(str,"Washington");break;
		case 76:strcat(str,"Jefferson");break;
		case 77:strcat(str,"Strossen");break;
		case 78:strcat(str,"Hannemann");break;
		case 79:strcat(str,"Hammond");break;
		case 80:strcat(str,"Logan");break;
		case 81:strcat(str,"Hutchison");break;
		case 82:strcat(str,"Jimison");break;
		case 83:strcat(str,"Sawyer");break;
		case 84:strcat(str,"Santiago");break;
		case 85:strcat(str,"Rudkin");break;
		case 86:strcat(str,"Bump");break;
		case 87:strcat(str,"Simon");break;
		case 88:strcat(str,"Davis");break;
		case 89:strcat(str,"Reagan");break;
		case 90:strcat(str,"Bush");break;
		case 91:strcat(str,"Bradshaw");break;
		case 92:strcat(str,"Yamaguchi");break;
		case 93:strcat(str,"Roy");break;
		case 94:strcat(str,"Colt");break;
		case 95:strcat(str,"Rothstein");break;
		case 96:strcat(str,"Spears");break;
		case 97:strcat(str,"Lopez");break;
		case 98:strcat(str,"Aguilera");break;
		case 99:strcat(str,"Carey");break;
		}
}

void disguisecheck(void)
{
	if(sitealarm)return;

	if(sitealarmtimer==-1)
		{
		if(!disguisesite(location[cursite]->type)&&
			!(map[locx][locy][locz].flag & SITEBLOCK_RESTRICTED))return;
		}

	char noticed=0;
	vector<int> noticer;
	for(int e=0;e<ENCMAX;e++)
		{
		if(encounter[e].type==CREATURE_PRISONER)continue;
		if(encounter[e].exists&&encounter[e].alive&&
			encounter[e].align==-1)
			{
			noticer.push_back(e);
			}
		}

	if(noticer.size()>0)
		{
		int disguise=disguiseskill();

		int n,an;

		do
			{
			an=LCSrandom(noticer.size());
			n=noticer[an];
			noticer.erase(noticer.begin() + an);

			int chance=encounter[n].attval(ATTRIBUTE_WISDOM)+
				encounter[n].attval(ATTRIBUTE_INTELLIGENCE);
			if(chance>(int)LCSrandom(21)+disguise)
				{
				noticed=1;
				break;
				}

			}while(noticer.size()>0);

		if(!noticed)return;

		clearmessagearea();

		set_color(COLOR_RED,COLOR_BLACK,1);
		move(16,1);
		addstr(encounter[n].name);
		if(sitealarmtimer!=0)
			{
			addstr(" looks at the Squad suspiciously.");

			int time=20+LCSrandom(10)-encounter[n].attval(ATTRIBUTE_INTELLIGENCE)-encounter[n].attval(ATTRIBUTE_CHARISMA);
			if(time<1)time=1;

			if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;
			}
		else
			{
			addstr(" looks at the Squad with Intolerance");
			move(17,1);
			addstr("and lets forth a piercing Conservative alarm cry!");

			sitealarm=1;
			}

		refresh();
		getch();
		}
}

void noticecheck(int exclude)
{
	if(sitealarm)return;

	char noticed=0;
	vector<int> noticer;
	for(int e=0;e<ENCMAX;e++)
		{
		if(e==exclude)continue;
		if(encounter[e].type==CREATURE_PRISONER)continue;
		if(encounter[e].exists&&encounter[e].alive&&
			encounter[e].align==-1)
			{
			noticer.push_back(e);
			}
		}

	if(noticer.size()>0)
		{
		int n,an;

		do
			{
			an=LCSrandom(noticer.size());
			n=noticer[an];
			noticer.erase(noticer.begin() + an);

			int chance=encounter[n].attval(ATTRIBUTE_WISDOM)+encounter[n].attval(ATTRIBUTE_INTELLIGENCE);
			if(chance>LCSrandom(21))
				{
				noticed=1;
				break;
				}

			}while(noticer.size()>0);

		if(!noticed)return;

		clearmessagearea();

		set_color(COLOR_RED,COLOR_BLACK,1);
		move(16,1);
		addstr(encounter[n].name);
		addstr(" observes your Liberal activity");
		move(17,1);
		addstr("and lets forth a piercing Conservative alarm cry!");

		sitealarm=1;

		refresh();
		getch();
		}
}

void getloot(char *str,int loot)
{
	strcpy(str,"");

	switch(loot)
		{
		case LOOT_LABEQUIPMENT:strcat(str,"Lab Equipment");break;
		case LOOT_COMPUTER:strcat(str,"Computer");break;
		case LOOT_TV:strcat(str,"TV");break;
		case LOOT_SECRETDOCUMENTS:strcat(str,"Secret Documents");break;
		case LOOT_POLICERECORDS:strcat(str,"Police Records");break;
		case LOOT_VCR:strcat(str,"VCR");break;
		case LOOT_CELLPHONE:strcat(str,"Cellphone");break;
		case LOOT_TVCAMERA:strcat(str,"TV Camera");break;
		case LOOT_BROADCASTINGEQUIPMENT:strcat(str,"Broadcasting Equip.");break;
		case LOOT_SILVERWARE:strcat(str,"Silverware");break;
		case LOOT_SCANNER:strcat(str,"Scanner");break;
		case LOOT_PRINTER:strcat(str,"Printer");break;
		case LOOT_FINECLOTH:strcat(str,"Fine Cloth");break;
		case LOOT_CHEMICAL:strcat(str,"Weird Chemical");break;
		case LOOT_CEOPHOTOS:strcat(str,"CEO Photos");break;
		case LOOT_INTHQDISK:strcat(str,"Intel. HQ Data Disk");break;
		case LOOT_CORPFILES:strcat(str,"Secret Corporate Files");break;
		}
}

void survey(creaturest *cr)
{
	int v;
	int creatureskill=cr->skill[SKILL_COMPUTERS]+cr->attval(ATTRIBUTE_INTELLIGENCE);
	int misschance=0,noise=2;
	if(creatureskill<3){noise=15;misschance=(30-creatureskill)*3;}
	else if(creatureskill<7){noise=7;misschance=(20-creatureskill)*3;}
	else if(creatureskill<10){noise=5;misschance=(20-creatureskill)*2;}
	else if(creatureskill<15){noise=4;misschance=20-creatureskill;}
	else if(creatureskill<20){noise=3;misschance=20-creatureskill;}

	int survey[VIEWNUM];

	for(v=0;v<VIEWNUM;v++)
		{
		survey[v]=attitude[v];

		//MAKE SURVEY ACCURATE IF DEBUGGING
		#ifdef NDEBUG
			survey[v]+=LCSrandom(noise*2+1)-noise;

			if(!LCSrandom(20))
				{
				survey[v]+=LCSrandom(noise*2+1)-noise;
				if(!LCSrandom(20))
					{
					survey[v]+=LCSrandom(noise*2+1)-noise;
					if(!LCSrandom(20))
						{
						survey[v]+=LCSrandom(noise*2+1)-noise;
						if(!LCSrandom(20))
							{
							survey[v]+=LCSrandom(noise*2+1)-noise;
							}
						}
					}
				}
		#endif

		if(survey[v]<0)survey[v]=0;
		if(survey[v]>100)survey[v]=100;

		#ifdef NDEBUG
			if(LCSrandom(100)<misschance)survey[v]=-1;
		#endif

		if(v==VIEW_LIBERALCRIMESQUAD&&attitude[v]==0)survey[v]=0;
		}

	erase();

	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(0,0);
	addstr("Survey of Public Opinion, According to Recent Polls");

	int y=2;

	char num[20];

	for(v=0;v<VIEWNUM;v++)
		{
		if(survey[VIEW_LIBERALCRIMESQUAD]==0&&
			v==VIEW_LIBERALCRIMESQUADPOS)continue;

		if(survey[v]==-1)set_color(COLOR_BLACK,COLOR_BLACK,1);
		else if(survey[v]<10)set_color(COLOR_RED,COLOR_BLACK,1);
		else if(survey[v]<30)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
		else if(survey[v]<50)set_color(COLOR_YELLOW,COLOR_BLACK,1);
		else if(survey[v]<70)set_color(COLOR_BLUE,COLOR_BLACK,1);
		else if(survey[v]<90)set_color(COLOR_CYAN,COLOR_BLACK,1);
		else set_color(COLOR_GREEN,COLOR_BLACK,1);

		move(y,0);
		if(survey[v]==-1)
			{
			addstr("Unknown how many ");
			}
		else
			{
			itoa(survey[v],num,10);
			addstr(num);
			addstr("% ");
			}

		switch(v)
			{
			case VIEW_ABORTION:addstr("supported abortion.");break;
			case VIEW_GAY:addstr("were in favor of equal rights for homosexuals.");break;
			case VIEW_DEATHPENALTY:addstr("opposed the death penalty.");break;
			case VIEW_TAXES:addstr("were against cutting taxes.");break;
			case VIEW_NUCLEARPOWER:addstr("were terrified of nuclear power.");break;
			case VIEW_ANIMALRESEARCH:addstr("deplored animal research.");break;
			case VIEW_POLICEBEHAVIOR:addstr("were critical of the police.");break;
			case VIEW_INTELLIGENCE:addstr("thought the intelligence community invades privacy.");break;
			case VIEW_FREESPEECH:addstr("believed in unfettered free speech.");break;
			case VIEW_GENETICS:addstr("abhorred genetically altered food products.");break;
			case VIEW_JUSTICES:addstr("were for the appointment of Liberal justices.");break;
			case VIEW_SWEATSHOPS:addstr("would boycott companies that used sweatshops.");break;
			case VIEW_POLLUTION:addstr("thought industry should lower pollution.");break;
			case VIEW_CORPORATECULTURE:addstr("were disgusted by corporate malfeasance.");break;
			case VIEW_CEOSALARY:addstr("believed that CEO salaries are too great.");break;
			case VIEW_LIBERALCRIMESQUAD:addstr("had heard of the Liberal Crime Squad.");break;
			case VIEW_LIBERALCRIMESQUADPOS:addstr("of these held the Liberal Crime Squad in high regard.");break;
			case VIEW_PRISONS:addstr("think the prison system needs reform.");break;
			case VIEW_AMRADIO:addstr("do not like AM radio.");break;
			case VIEW_CABLENEWS:addstr("have a negative opinion of cable news programs.");break;
			}
		y++;
		}

	set_color(COLOR_WHITE,COLOR_BLACK,0);
	move(23,0);
	addstr("Results are +/- ");
	itoa(noise,num,10);
	addstr(num);
	addstr(" Liberal percentage points.");
	move(24,0);
	addstr("Press any key to reflect on these results.");

	refresh();
	getch();
}

void special_lab_cosmetics_cagedanimals(void)
{
	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You see fluffy white rabbits in a locked cage.");
		move(17,1);
		addstr("Free them? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			char actual;

			if(unlock(UNLOCK_CAGE,actual))
				{
				int time=20+LCSrandom(10);
				if(time<1)time=1;
				if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

				sitecrime++;
				sitestory->crime.push_back(CRIME_FREE_RABBITS);
				criminalizeparty(LAWFLAG_VANDALISM);
				}
			
			if(actual)
				{
				alienationcheck(0);
				noticecheck(-1);
				map[locx][locy][locz].special=-1;
				}

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void special_nuclear_onoff(void)
{
	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You see a big red button that says OFF.");
		move(17,1);
		addstr("Press it? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			clearmessagearea();
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(16,1);
			addstr("The lights dim...  power must be out state-wide.");

			change_public_opinion(VIEW_NUCLEARPOWER,5,0);

			refresh();
			getch();

			sitealarm=1;
			alienationcheck(1);
			map[locx][locy][locz].special=-1;
			sitecrime+=5;
			sitestory->crime.push_back(CRIME_SHUTDOWNREACTOR);
			criminalizeparty(LAWFLAG_TERRORISM);

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void special_lab_genetic_cagedanimals(void)
{
	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You see horrible misshapen creatures in a locked cage.");
		move(17,1);
		addstr("Free them? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			char actual;

			if(unlock(UNLOCK_CAGE,actual))
				{
				int time=20+LCSrandom(10);
				if(time<1)time=1;
				if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

				sitecrime++;
				sitestory->crime.push_back(CRIME_FREE_BEASTS);
				criminalizeparty(LAWFLAG_VANDALISM);

				if(!LCSrandom(2))
					{
					clearmessagearea();

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(16,1);
					addstr("Uh, maybe that idea was Conservative in retrospect...");

					int numleft=LCSrandom(6)+1;
					for(int e=0;e<ENCMAX;e++)
						{
						if(!encounter[e].exists)
							{
							makecreature(encounter[e],CREATURE_GENETIC);
							numleft--;
							}
						if(numleft==0)break;
						}

					if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
					else printencounter();

					refresh();
					getch();

					sitealarm=1;
					alienationcheck(0);
					}
				else
					{
					alienationcheck(0);
					noticecheck(-1);
					}
				}
			else if(actual)
				{
				alienationcheck(0);
				noticecheck(-1);
				}

			if(actual)
				{
				map[locx][locy][locz].special=-1;
				}

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void special_policestation_lockup(void)
{
	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You see prisoners in the detention room.");
		move(17,1);
		addstr("Free them? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			char actual;

			if(unlock(UNLOCK_DOOR,actual))
				{
				int numleft=LCSrandom(8)+2;
				for(int e=0;e<ENCMAX;e++)
					{
					if(!encounter[e].exists)
						{
						makecreature(encounter[e],CREATURE_PRISONER);
						numleft--;
						}
					if(numleft==0)break;
					}

				int time=20+LCSrandom(10);
				if(time<1)time=1;
				if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

				if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
				else printencounter();
				refresh();

				partyrescue();
				}

			if(actual)
				{
				alienationcheck(1);
				noticecheck(-1);
				map[locx][locy][locz].special=-1;
				sitecrime+=3;
				sitestory->crime.push_back(CRIME_POLICE_LOCKUP);
				criminalizeparty(LAWFLAG_HELPESCAPE);
				}

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void special_courthouse_lockup(void)
{
	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You see prisoners in the Court House jail.");
		move(17,1);
		addstr("Free them? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			char actual;

			if(unlock(UNLOCK_DOOR,actual))
				{
				int numleft=LCSrandom(8)+2;
				for(int e=0;e<ENCMAX;e++)
					{
					if(!encounter[e].exists)
						{
						makecreature(encounter[e],CREATURE_PRISONER);
						numleft--;
						}
					if(numleft==0)break;
					}

				int time=20+LCSrandom(10);
				if(time<1)time=1;
				if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

				if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
				else printencounter();
				refresh();

				partyrescue();
				}

			if(actual)
				{
				alienationcheck(1);
				noticecheck(-1);
				map[locx][locy][locz].special=-1;
				sitecrime+=3;
				sitestory->crime.push_back(CRIME_COURTHOUSE_LOCKUP);
				criminalizeparty(LAWFLAG_HELPESCAPE);
				}

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void special_courthouse_jury(void)
{
	int p;
	if(sitealarm||sitealienate)
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("It appears as if this room has been");
		move(17,1);
		addstr("vacated in a hurry.");

		refresh();
		getch();
		return;
		}

	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You've found a Jury in deliberations!");
		move(17,1);
		addstr("Attempt to influence them? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			map[locx][locy][locz].special=-1;

			char succeed=0;

			int maxattack=0;

			for(p=0;p<6;p++)
				{
				if(activesquad->squad[p]!=NULL)
					{
					if(activesquad->squad[p]->alive)
						{
						if((activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
							activesquad->squad[p]->attval(ATTRIBUTE_HEART)+
							activesquad->squad[p]->skill[SKILL_PERSUASION]+
							activesquad->squad[p]->skill[SKILL_LAW]*2)>maxattack)
							{
							maxattack=activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
								activesquad->squad[p]->attval(ATTRIBUTE_HEART)+
								activesquad->squad[p]->skill[SKILL_PERSUASION]+
								activesquad->squad[p]->skill[SKILL_LAW]*2;
							}
						}
					}
				}

			vector<int> goodp;

			for(p=0;p<6;p++)
				{
				if(activesquad->squad[p]!=NULL)
					{
					if(activesquad->squad[p]->alive)
						{
						if((activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
							activesquad->squad[p]->attval(ATTRIBUTE_HEART)+
							activesquad->squad[p]->skill[SKILL_PERSUASION]+
							activesquad->squad[p]->skill[SKILL_LAW]*2)==maxattack)
							{
							goodp.push_back(p);
							}
						}
					}
				}

			if(goodp.size()>0)
				{
				int p=goodp[LCSrandom(goodp.size())];

				short aroll=LCSrandom(21)+activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
					activesquad->squad[p]->attval(ATTRIBUTE_HEART)+LCSrandom(activesquad->squad[p]->skill[SKILL_PERSUASION]+1)+
					LCSrandom(activesquad->squad[p]->skill[SKILL_LAW]+1)*2;
				short troll=LCSrandom(21)+20;
				activesquad->squad[p]->skill_ip[SKILL_PERSUASION]+=troll;
				activesquad->squad[p]->skill_ip[SKILL_LAW]+=troll;

				if(aroll>troll)succeed=1;

				if(succeed)
					{
					clearmessagearea();

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(16,1);
					addstr(activesquad->squad[p]->name);
					addstr(" works the room like Twelve Angry Men, and the jury");
					move(17,1);
					addstr("concludes that the ");
					switch(LCSrandom(10))
						{
						case 0:addstr("murder");break;
						case 1:addstr("assault");break;
						case 2:addstr("theft");break;
						case 3:addstr("mugging");break;
						case 4:addstr("burglary");break;
						case 5:addstr("property destruction");break;
						case 6:addstr("vandalism");break;
						case 7:addstr("libel");break;
						case 8:addstr("slander");break;
						case 9:addstr("sodomy");break;
						}
					addstr(" wasn't really wrong here.");

					refresh();
					getch();

					alienationcheck(0);
					noticecheck(-1);

					//INSTANT JUICE BONUS
					juiceparty(10);
					}
				else
					{
					clearmessagearea();

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(16,1);
					addstr(activesquad->squad[p]->name);

					addstr(" wasn't quite convincing...");

					refresh();
					getch();

					int numleft=12;
					for(int e=0;e<ENCMAX;e++)
						{
						if(!encounter[e].exists)
							{
							makecreature(encounter[e],CREATURE_JUROR);
							numleft--;
							}
						if(numleft==0)break;
						}

					if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
					else printencounter();
					refresh();

					sitealarm=1;
					sitealienate=2;
					sitecrime+=10;
					sitestory->crime.push_back(CRIME_JURYTAMPERING);

					criminalizeparty(LAWFLAG_JURY);
					}
				}

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void special_prison_control(void)
{
	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You've found the prison control room.");
		move(17,1);
		addstr("Free the prisoners? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			int numleft=LCSrandom(8)+2;
			for(int e=0;e<ENCMAX;e++)
				{
				if(!encounter[e].exists)
					{
					makecreature(encounter[e],CREATURE_PRISONER);
					numleft--;
					}
				if(numleft==0)break;
				}

			int time=20+LCSrandom(10);
			if(time<1)time=1;
			if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

			if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
			else printencounter();
			refresh();

			partyrescue();

			alienationcheck(1);
			noticecheck(-1);
			map[locx][locy][locz].special=-1;
			sitecrime+=3;
			sitestory->crime.push_back(CRIME_PRISON_RELEASE);
			criminalizeparty(LAWFLAG_HELPESCAPE);

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void special_intel_supercomputer(void)
{
	if(sitealarm||sitealienate)
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("The security alert has caused the");
		move(17,1);
		addstr("computer to shut down.");

		refresh();
		getch();
		return;
		}

	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You've found the Intelligence Super Computer.");
		move(17,1);
		addstr("Hack it? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			char actual;

			if(hack(HACK_SUPERCOMPUTER,actual))
				{
				clearmessagearea();

				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(16,1);
				addstr("The Squad obtains sensitive information.");

				itemst *it=new itemst;
					it->type=ITEM_LOOT;
					it->loottype=LOOT_INTHQDISK;
				activesquad->loot.push_back(it);

				refresh();
				getch();
				}

			if(actual)
				{
				int time=20+LCSrandom(10);
				if(time<1)time=1;
				if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

				alienationcheck(1);
				noticecheck(-1);
				map[locx][locy][locz].special=-1;
				sitecrime+=3;
				sitestory->crime.push_back(CRIME_HACK_INTEL);

				criminalizeparty(LAWFLAG_TREASON);
				}

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void special_sweatshop_equipment(void)
{
	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You see some textile equipment.");
		move(17,1);
		addstr("Destroy it? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			int time=20+LCSrandom(10);
			if(time<1)time=1;
			if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;
			
			alienationcheck(0);
			noticecheck(-1);
			map[locx][locy][locz].special=-1;
			sitecrime++;
			sitestory->crime.push_back(CRIME_BREAK_SWEATSHOP);

			criminalizeparty(LAWFLAG_VANDALISM);

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void special_polluter_equipment(void)
{
	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You see some industrial equipment.");
		move(17,1);
		addstr("Destroy it? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			int time=20+LCSrandom(10);
			if(time<1)time=1;
			if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

			change_public_opinion(VIEW_POLLUTION,2,0);
			
			alienationcheck(1);
			noticecheck(-1);
			map[locx][locy][locz].special=-1;
			sitecrime+=2;
			sitestory->crime.push_back(CRIME_BREAK_FACTORY);

			criminalizeparty(LAWFLAG_VANDALISM);

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void special_house_photos(void)
{
	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You've found a safe.");
		move(17,1);
		addstr("Open it? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			char actual;

			if(unlock(UNLOCK_SAFE,actual))
				{
				clearmessagearea();

				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(16,1);
				addstr("The Squad has found some... very comprising photos.");

				itemst *it=new itemst;
					it->type=ITEM_LOOT;
					it->loottype=LOOT_CEOPHOTOS;
				activesquad->loot.push_back(it);

				int time=20+LCSrandom(10);
				if(time<1)time=1;
				if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

				refresh();
				getch();
				}

			if(actual)
				{
				alienationcheck(1);
				noticecheck(-1);
				map[locx][locy][locz].special=-1;
				sitecrime+=3;
				sitestory->crime.push_back(CRIME_HOUSE_PHOTOS);

				criminalizeparty(LAWFLAG_THIEF);
				}

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void special_corporate_files(void)
{
	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You've found a safe.");
		move(17,1);
		addstr("Open it? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			char actual;

			if(unlock(UNLOCK_SAFE,actual))
				{
				clearmessagearea();

				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(16,1);
				addstr("The Squad has found some very interesting files.");

				itemst *it=new itemst;
					it->type=ITEM_LOOT;
					it->loottype=LOOT_CORPFILES;
				activesquad->loot.push_back(it);

				int time=20+LCSrandom(10);
				if(time<1)time=1;
				if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

				refresh();
				getch();
				}

			if(actual)
				{
				alienationcheck(1);
				noticecheck(-1);
				map[locx][locy][locz].special=-1;
				sitecrime+=3;
				sitestory->crime.push_back(CRIME_CORP_FILES);

				criminalizeparty(LAWFLAG_THIEF);
				}

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void special_radio_broadcaststudio(void)
{
	if(sitealarm||sitealienate)
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("This studio has been abandoned in the");
		move(17,1);
		addstr("commotion.");

		refresh();
		getch();
		return;
		}

	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You've found a radio broadcasting room.");
		move(17,1);
		addstr("Interrupt this evening's programming? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			if(radio_broadcast())map[locx][locy][locz].special=-1;

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void special_news_broadcaststudio(void)
{
	if(sitealarm||sitealienate)
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("This studio has been abandoned in the");
		move(17,1);
		addstr("commotion.");

		refresh();
		getch();
		return;
		}

	do
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You've found a Cable News broadcasting studio.");
		move(17,1);
		addstr("Start an impromptu news program? (Yes or No)");

		refresh();

		int c=getch();
		translategetch(c);

		if(c=='y')
			{
			if(news_broadcast())map[locx][locy][locz].special=-1;

			return;
			}
		else if(c=='n')return;

		}while(1);
}

void clearmaparea(void)
{
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	for(int y=9;y<24;y++)
		{
		move(y,57);
		addstr("                       ");
		}
}

void clearmessagearea(void)
{
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(16,1);
	addstr("                                                        ");
	move(17,1);
	addstr("                                                        ");
}

void clearcommandarea(void)
{
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	for(int y=9;y<16;y++)
		{
		move(y,1);
		addstr("                                                        ");
		}
}

char unlock(short type,char &actual)
{
	int p;
	int difficulty=0;

	switch(type)
		{
		case UNLOCK_DOOR:difficulty=10;break;
		case UNLOCK_CAGE:difficulty=10;break;
		case UNLOCK_SAFE:difficulty=20;break;
		}

	int maxattack=0;

	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(activesquad->squad[p]->alive)
				{
				if((activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE)+
					activesquad->squad[p]->skill[SKILL_SECURITY])>maxattack)
					{
					maxattack=activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE)+
						activesquad->squad[p]->skill[SKILL_SECURITY];
					}
				}
			}
		}

	vector<int> goodp;

	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(activesquad->squad[p]->alive)
				{
				if((activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE)+
					activesquad->squad[p]->skill[SKILL_SECURITY])==maxattack)
					{
					goodp.push_back(p);
					}
				}
			}
		}

	if(goodp.size()>0)
		{
		int p=goodp[LCSrandom(goodp.size())];

		int aroll=LCSrandom(11)+maxattack;
		activesquad->squad[p]->skill_ip[SKILL_SECURITY]+=difficulty;

		if(aroll>difficulty)
			{
			clearmessagearea();
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(16,1);
			addstr(activesquad->squad[p]->name);
			addstr(" ");
			switch(type)
				{
				case UNLOCK_DOOR:addstr("unlocks the door");break;
				case UNLOCK_CAGE:addstr("unlocks the cage");break;
				case UNLOCK_SAFE:addstr("cracks the safe");break;
				}
			addstr("!");
			refresh();
			getch();

			actual=1;
			return 1;
			}
		else
			{
			clearmessagearea();
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(16,1);
			addstr(activesquad->squad[p]->name);
			addstr(" doesn't succeed.");
			refresh();
			getch();

			actual=1;
			return 0;
			}
		}
	else
		{
		clearmessagearea();
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You can't find anyone to do the job.");
		refresh();
		getch();
		}

	actual=0;
	return 0;
}

char bash(short type,char &actual)
{
	int difficulty=0;
	char crowable=0;
	int p = 0;

	switch(type)
		{
		case BASH_DOOR:difficulty=10;crowable=1;break;
		}

	if(crowable)
		{
		if(!squadhasitem(*activesquad,ITEM_WEAPON,WEAPON_CROWBAR))crowable=0;
		}

	int maxattack=0;

	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(activesquad->squad[p]->alive)
				{
				if(activesquad->squad[p]->attval(ATTRIBUTE_STRENGTH)+
					bashstrengthmod(activesquad->squad[p]->weapon.type)>maxattack)
					{
					maxattack=activesquad->squad[p]->attval(ATTRIBUTE_STRENGTH)+
						bashstrengthmod(activesquad->squad[p]->weapon.type);
					}
				}
			}
		}

	vector<int> goodp;

	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(activesquad->squad[p]->alive)
				{
				if(activesquad->squad[p]->attval(ATTRIBUTE_STRENGTH)+
					bashstrengthmod(activesquad->squad[p]->weapon.type)==maxattack)
					{
					goodp.push_back(p);
					}
				}
			}
		}

	if(goodp.size()>0)
		{
		int p=goodp[LCSrandom(goodp.size())];

		int aroll=LCSrandom(11)+maxattack;

		if(aroll>difficulty||crowable)
			{
			clearmessagearea();
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(16,1);
			addstr(activesquad->squad[p]->name);
			addstr(" ");
			switch(type)
				{
				case BASH_DOOR:
					{
					if(crowable)addstr("uses a crowbar on the door");
					else addstr("bashes in the door");
					break;
					}
				}
			addstr("!");
			refresh();
			getch();

			actual=1;
			return 1;
			}
		else
			{
			clearmessagearea();
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(16,1);
			addstr(activesquad->squad[p]->name);
			switch(type)
				{
				case BASH_DOOR:addstr(" bounces off of the door");break;
				}
			addstr(".");
			refresh();
			getch();

			actual=1;
			return 0;
			}
		}
	else
		{
		clearmessagearea();
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You can't find anyone to do the job.");
		refresh();
		getch();
		}

	actual=0;
	return 0;
}

char hack(short type,char &actual)
{
	int difficulty=0;
	int p;

	switch(type)
		{
		case HACK_SUPERCOMPUTER:difficulty=20;break;
		}

	int maxattack=0;
	char blind=0;

	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(activesquad->squad[p]->alive)
				{
				if((activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE)+
					activesquad->squad[p]->skill[SKILL_COMPUTERS])>maxattack)
					{
					if(activesquad->squad[p]->skill[SKILL_COMPUTERS]>0)
						{
						if(!activesquad->squad[p]->special[SPECIALWOUND_RIGHTEYE]&&
							!activesquad->squad[p]->special[SPECIALWOUND_LEFTEYE])
							{
							blind=1;
							}
						else
							{
							maxattack=activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE)+
								activesquad->squad[p]->skill[SKILL_COMPUTERS];
							}
						}
					}
				}
			}
		}

	vector<int> goodp;

	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(activesquad->squad[p]->alive)
				{
				if((activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE)+
					activesquad->squad[p]->skill[SKILL_COMPUTERS])==maxattack)
					{
					if(activesquad->squad[p]->skill[SKILL_COMPUTERS]>0)
						{
						if(activesquad->squad[p]->special[SPECIALWOUND_RIGHTEYE]||
							activesquad->squad[p]->special[SPECIALWOUND_LEFTEYE])
							{
							goodp.push_back(p);
							}
						}
					}
				}
			}
		}

	if(goodp.size()>0)
		{
		int p=goodp[LCSrandom(goodp.size())];

		int aroll=LCSrandom(11)+maxattack;
		activesquad->squad[p]->skill_ip[SKILL_COMPUTERS]+=difficulty;

		if(aroll>difficulty)
			{
			clearmessagearea();
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(16,1);
			addstr(activesquad->squad[p]->name);
			addstr(" ");
			switch(type)
				{
				case HACK_SUPERCOMPUTER:addstr("has hacked into the Super Computer!");break;
				}
			addstr("!");
			refresh();
			getch();

			actual=1;
			return 1;
			}
		else
			{
			clearmessagearea();
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(16,1);
			addstr(activesquad->squad[p]->name);
			switch(type)
				{
				case BASH_DOOR:addstr(" fails to hack the computer.");break;
				}
			addstr(".");
			refresh();
			getch();

			actual=1;
			return 0;
			}
		}
	else
		{
		clearmessagearea();
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("You can't find anyone to do the job.");
		if(blind)
			{
			refresh();
			getch();
			move(17,1);
			addstr("Including the UNSIGHTED HACKER you brought.");
			}
		refresh();
		getch();
		}

	actual=0;
	return 0;
}

char alienationcheck(char mistake)
{
	if(location[cursite]->siege.siege)return 0;

	char alienate=0;

	int oldsitealienate=sitealienate;

	for(int e=0;e<ENCMAX;e++)
		{
		if(encounter[e].type==CREATURE_PRISONER)continue;
		if(encounter[e].alive&&encounter[e].exists&&
			(encounter[e].align==0||(encounter[e].align==1&&mistake)))
			{
			conservatise(encounter[e]);
			if(encounter[e].cantbluff==0)encounter[e].cantbluff=1;
			alienate=1;
			}
		}

	if(alienate&&mistake)sitealienate=2;
	if(alienate&&!mistake&&sitealienate!=2)sitealienate=1;

	if(oldsitealienate<sitealienate)
		{
		set_color(COLOR_YELLOW,COLOR_BLACK,1);

		move(16,1);
		if(sitealienate==1)addstr("We've alienated the masses here!                        ");
		else addstr("We've alienated absolutely everyone here!               ");
		move(17,1);
		addstr("                                                        ");

		sitealarm=1;

		if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
		else printencounter();
		refresh();
		getch();
		}

	return alienate;
}

int disguiseskill(void)
{
	int lowest=10000;

	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(!activesquad->squad[p]->alive)return 0;

			if(activesquad->squad[p]->prisoner!=NULL)return 0;

			int skill=activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE)+
				activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
				activesquad->squad[p]->skill[SKILL_DISGUISE];

			//ALSO NEED APPROPRIATE UNIFORM
			char uniformed=hasdisguise(*activesquad->squad[p],sitetype);

			if(!uniformed)
				{
				skill>>=2;
				//NAKEDNESS MAJOR PENALTY
				if(activesquad->squad[p]->armor.type==ARMOR_NONE)skill=-100;
				}
			else
				{
				activesquad->squad[p]->skill_ip[SKILL_DISGUISE]+=2;
				}

			if(activesquad->squad[p]->armor.quality!='1')
				{
				if(activesquad->squad[p]->armor.quality=='2')skill--;
				else if(activesquad->squad[p]->armor.quality=='3')skill>>=1;
				else if(activesquad->squad[p]->armor.quality=='4')skill>>=2;
				}

			if(activesquad->squad[p]->armor.flag & ARMORFLAG_BLOODY)
				{
				skill>>=1;
				}

			if(activesquad->squad[p]->armor.flag & ARMORFLAG_DAMAGED)
				{
				skill>>=1;
				}

			if(lowest>skill)lowest=skill;
			}
		}

	return lowest;
}

char hasdisguise(creaturest &cr,short type)
{
	char uniformed=0;

	if(location[cursite]->siege.siege)
		{
		switch(location[cursite]->siege.siegetype)
			{
			case SIEGE_CIA:
				{
				if(cr.armor.type==ARMOR_BLACKSUIT)uniformed=1;
				break;
				}
			case SIEGE_CORPORATE:
				{
				if(cr.armor.type==ARMOR_MILITARY)uniformed=1;
				break;
				}
			case SIEGE_HICKS:
				{
				if(cr.armor.type==ARMOR_CLOTHES)uniformed=1;
				if(cr.armor.type==ARMOR_OVERALLS)uniformed=1;
				if(cr.armor.type==ARMOR_WIFEBEATER)uniformed=1;
				break;
				}
			case SIEGE_POLICE:
				{
				if(cr.armor.type==ARMOR_POLICEUNIFORM)uniformed=1;
				if(cr.armor.type==ARMOR_MILITARY&&
					location[cursite]->siege.escalationstate>0)uniformed=1;
				break;
				}
			}
		}
	else
		{
		switch(type)
			{
			case SITE_INDUSTRY_WAREHOUSE:
			case SITE_RESIDENTIAL_SHELTER:
				uniformed=1;
				break;
			case SITE_LABORATORY_COSMETICS:
			case SITE_LABORATORY_GENETIC:
				if(map[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
					{
					if(cr.armor.type==ARMOR_LABCOAT)uniformed=1;
					if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
					}
				else if(cr.armor.type!=ARMOR_NONE)uniformed=1;
				break;
			case SITE_GOVERNMENT_POLICESTATION:
				if(map[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
					{
					if(cr.armor.type==ARMOR_POLICEUNIFORM)uniformed=1;
					}
				else if(cr.armor.type!=ARMOR_NONE)uniformed=1;
				break;
			case SITE_GOVERNMENT_COURTHOUSE:
				if(map[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
					{
					if(cr.armor.type==ARMOR_BLACKROBE)uniformed=1;
					if(cr.armor.type==ARMOR_POLICEUNIFORM)uniformed=1;
					if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
					}
				else if(cr.armor.type!=ARMOR_NONE)uniformed=1;
				break;
			case SITE_GOVERNMENT_PRISON:
				if(map[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
					{
					if(law[LAW_DEATHPENALTY]==-2&&
					   law[LAW_POLICEBEHAVIOR]==-2)
						{
						if(cr.armor.type==ARMOR_LABCOAT)uniformed=1;
						}
					else if(cr.armor.type==ARMOR_PRISONGUARD)uniformed=1;
					if(cr.armor.type==ARMOR_PRISONER)uniformed=1;
					}
				else if(cr.armor.type!=ARMOR_NONE)uniformed=1;
				break;
			case SITE_GOVERNMENT_INTELLIGENCEHQ:
				if(map[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
					{
					if(cr.armor.type==ARMOR_BLACKSUIT)uniformed=1;
					if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
					}
				else if(cr.armor.type!=ARMOR_NONE)uniformed=1;
				break;
			case SITE_BUSINESS_CIGARBAR:
				if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
				if(cr.armor.type==ARMOR_EXPENSIVESUIT)uniformed=1;
				if(cr.armor.type==ARMOR_CHEAPSUIT)uniformed=1;
				if(cr.armor.type==ARMOR_EXPENSIVEDRESS)uniformed=1;
				if(cr.armor.type==ARMOR_CHEAPDRESS)uniformed=1;
				break;
			case SITE_INDUSTRY_SWEATSHOP:
				if(cr.armor.type==ARMOR_NONE)uniformed=1;
				if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
				break;
			case SITE_INDUSTRY_POLLUTER:
				if(cr.armor.type==ARMOR_WORKCLOTHES)uniformed=1;
				if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
				break;
			case SITE_INDUSTRY_NUCLEAR:
				if(cr.armor.type==ARMOR_LABCOAT)uniformed=1;
				if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
				break;
			case SITE_CORPORATE_HEADQUARTERS:
				if(cr.armor.type==ARMOR_EXPENSIVESUIT)uniformed=1;
				if(cr.armor.type==ARMOR_CHEAPSUIT)uniformed=1;
				if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
				if(cr.armor.type==ARMOR_EXPENSIVEDRESS)uniformed=1;
				if(cr.armor.type==ARMOR_CHEAPDRESS)uniformed=1;
				break;
			case SITE_CORPORATE_HOUSE:
				if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
				break;
			case SITE_MEDIA_AMRADIO:
				if(map[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
					{
					if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
					}
				else if(cr.armor.type!=ARMOR_NONE)uniformed=1;
				break;
			case SITE_MEDIA_CABLENEWS:
				if(map[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
					{
					if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
					}
				else if(cr.armor.type!=ARMOR_NONE)uniformed=1;
				break;
			default:
				if(cr.armor.type!=ARMOR_NONE)uniformed=1;
				break;
			}
		}

	return uniformed;
}

char liberalagenda(char won)
{
	erase();

	if(won==1)
		{
		set_color(COLOR_GREEN,COLOR_BLACK,1);
		move(0,0);
		addstr("The Triumph of the Liberal Agenda");
		}
	else if(won==-1)
		{
		set_color(COLOR_RED,COLOR_BLACK,1);
		move(0,0);
		addstr("The Abject Failure of the Liberal Agenda");
		}
	else
		{
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(0,0);
		addstr("The Status of the Liberal Agenda");
		}

	if(exec[EXEC_PRESIDENT]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
	else if(exec[EXEC_PRESIDENT]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
	else if(exec[EXEC_PRESIDENT]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
	else if(exec[EXEC_PRESIDENT]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
	else set_color(COLOR_GREEN,COLOR_BLACK,1);
	move(2,0);
	if(won!=-1)addstr("President: ");
	else addstr("King ");
	addstr(execname[EXEC_PRESIDENT]);addstr(", ");
	switch(exec[EXEC_PRESIDENT])
		{
		case -2:addstr("Arch-Conservative");break;
		case -1:addstr("Conservative");break;
		case 0:addstr("moderate");break;
		case 1:addstr("Liberal");break;
		case 2:addstr("Elite Liberal");break;
		}
	if(won!=-1)
		{
		if(execterm==1)addstr(", 1st Term");
		else addstr(", 2nd Term");
		}
	if(exec[EXEC_VP]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
	else if(exec[EXEC_VP]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
	else if(exec[EXEC_VP]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
	else if(exec[EXEC_VP]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
	else set_color(COLOR_GREEN,COLOR_BLACK,1);
	move(3,0);
	addstr("Vice President: ");addstr(execname[EXEC_VP]);addstr(", ");
	switch(exec[EXEC_VP])
		{
		case -2:addstr("Arch-Conservative");break;
		case -1:addstr("Conservative");break;
		case 0:addstr("moderate");break;
		case 1:addstr("Liberal");break;
		case 2:addstr("Elite Liberal");break;
		}
	if(exec[EXEC_STATE]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
	else if(exec[EXEC_STATE]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
	else if(exec[EXEC_STATE]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
	else if(exec[EXEC_STATE]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
	else set_color(COLOR_GREEN,COLOR_BLACK,1);
	move(4,0);
	addstr("Secretary of State: ");addstr(execname[EXEC_STATE]);addstr(", ");
	switch(exec[EXEC_STATE])
		{
		case -2:addstr("Arch-Conservative");break;
		case -1:addstr("Conservative");break;
		case 0:addstr("moderate");break;
		case 1:addstr("Liberal");break;
		case 2:addstr("Elite Liberal");break;
		}
	if(exec[EXEC_ATTORNEY]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
	else if(exec[EXEC_ATTORNEY]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
	else if(exec[EXEC_ATTORNEY]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
	else if(exec[EXEC_ATTORNEY]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
	else set_color(COLOR_GREEN,COLOR_BLACK,1);
	move(5,0);
	addstr("Attorney General: ");addstr(execname[EXEC_ATTORNEY]);addstr(", ");
	switch(exec[EXEC_ATTORNEY])
		{
		case -2:addstr("Arch-Conservative");break;
		case -1:addstr("Conservative");break;
		case 0:addstr("moderate");break;
		case 1:addstr("Liberal");break;
		case 2:addstr("Elite Liberal");break;
		}

	int y=22-LAWNUM;
	for(int l=0;l<LAWNUM;l++)
		{
		if(won==-1)set_color(COLOR_RED,COLOR_BLACK,1);
		else
			{
			if(law[l]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
			else if(law[l]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
			else if(law[l]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
			else if(law[l]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
			else set_color(COLOR_GREEN,COLOR_BLACK,1);
			}

		move(y,0);

		switch(l)
			{
			case LAW_TAX:
				if(won==-1)addstr("There are no taxes, yet most people have no money.");
				else if(law[l]==-2)addstr("The tax code is a nightmare designed to maintain class structure.");
				else if(law[l]==-1)addstr("A flat tax is in effect.");
				else if(law[l]==0)addstr("Taxes are moderate, and the code has loop-holes.");
				else if(law[l]==1)addstr("Taxes are very high and steeply graded.");
				else addstr("Rich people are virtually unheard of, due to taxation.");
				break;
			case LAW_ABORTION:
				if(won==-1)addstr("Use of contraception and abortion are capital offenses.");
				else if(law[l]==-2)addstr("Abortion is illegal in all cases.");
				else if(law[l]==-1)addstr("Abortion is illegal except in extreme circumstances.");
				else if(law[l]==0)addstr("Abortion is illegal in the second and third trimesters.");
				else if(law[l]==1)addstr("Abortion is illegal in the third trimester.");
				else addstr("Abortion is legal.");
				break;
			case LAW_ANIMALRESEARCH:
				if(won==-1)addstr("All forms of human experimentation on the poor are encouraged.");
				else if(law[l]==-2)addstr("Animal research is completely unregulated.");
				else if(law[l]==-1)addstr("Animal research is lightly regulated.");
				else if(law[l]==0)addstr("Animal research is moderately regulated.");
				else if(law[l]==1)addstr("Animal research is stiffly regulated.");
				else addstr("Animal research is illegal in all cases.");
				break;
			case LAW_POLICEBEHAVIOR:
				if(won==-1)addstr("Policing is administered by corporations and has a draft.");
				else if(law[l]==-2)addstr("Law enforcement is given free reign.");
				else if(law[l]==-1)addstr("Law enforcement is lightly regulated.");
				else if(law[l]==0)addstr("Law enforcement is moderately regulated.");
				else if(law[l]==1)addstr("Law enforcement is strictly controlled.");
				else addstr("All law enforcement positions are subject to election and recall.");
				break;
			case LAW_PRIVACY:
				if(won==-1)addstr("Files on each citizen are easily accessible to corporations.");
				else if(law[l]==-2)addstr("Any corporation requesting private information is granted access.");
				else if(law[l]==-1)addstr("Privacy laws are weak.");
				else if(law[l]==0)addstr("Privacy laws are moderate.");
				else if(law[l]==1)addstr("Privacy laws are strong.");
				else addstr("Individual privacy is sacred.");
				break;
			case LAW_DEATHPENALTY:
				if(won==-1)addstr("Poor criminals receive mandatory death sentences.");
				else if(law[l]==-2)addstr("People can be put to death for minor offenses.");
				else if(law[l]==-1)addstr("The death penalty is actively enforced in many states.");
				else if(law[l]==0)addstr("The death penalty is in effect but under scrutiny.");
				else if(law[l]==1)addstr("The death penalty is not permitted in many circumstances.");
				else addstr("The death penalty is considered barbaric and never practiced.");
				break;
			case LAW_NUCLEARPOWER:
				if(won==-1)addstr("Nuclear power plants are ubiquitous.");
				else if(law[l]==-2)addstr("Nuclear power is proliferating with no controls.");
				else if(law[l]==-1)addstr("Nuclear power is a preferred energy source.");
				else if(law[l]==0)addstr("Nuclear power is often an energy source.");
				else if(law[l]==1)addstr("Nuclear power is intensely regulated and seldom used.");
				else addstr("Nuclear power is illegal.");
				break;
			case LAW_POLLUTION:
				if(won==-1)addstr("Deformed children are the norm in industrial zones.");
				else if(law[l]==-2)addstr("Industry may pollute as much as they like.");
				else if(law[l]==-1)addstr("Industry voluntarily regulates pollution.");
				else if(law[l]==0)addstr("Industry is subject to moderate pollution regulations.");
				else if(law[l]==1)addstr("Industry is subject to strict pollution regulations.");
				else addstr("Industry is subject to zero-tolerance pollution regulations.");
				break;
			case LAW_LABOR:
				if(won==-1)addstr("People are bred in pens to be farmed out to corporations like beasts.");
				else if(law[l]==-2)addstr("There is no weekend and children are forced to work.");
				else if(law[l]==-1)addstr("Working conditions are miserable and the minimum wage is low.");
				else if(law[l]==0)addstr("Workers still require some benefits.");
				else if(law[l]==1)addstr("Workers are fairly compensated and have benefits.");
				else addstr("There are universal workers' rights and high wages.");
				break;
			case LAW_GAY:
				if(won==-1)addstr("Homosexuals are executed regularly.");
				else if(law[l]==-2)addstr("Homosexuals are routinely persecuted with no recourse.");
				else if(law[l]==-1)addstr("Homosexuals are not tolerated.");
				else if(law[l]==0)addstr("Homosexuals are grudgingly tolerated but have few equal rights.");
				else if(law[l]==1)addstr("Homosexuals have many rights shared by heterosexuals.");
				else addstr("Homosexuals have equal rights.");
				break;
			case LAW_CORPORATE:
				if(won==-1)addstr("Corporations under the King run the country in a feudal system.");
				else if(law[l]==-2)addstr("Corporations essentially run the country in a feudal system.");
				else if(law[l]==-1)addstr("Corporate culture is corrupt and there is a great disparity in wages.");
				else if(law[l]==0)addstr("Corporations are moderately regulated, although wages are still unfair.");
				else if(law[l]==1)addstr("Corporations are stiffly regulated, and executives are fairly compensated.");
				else addstr("Corporations are subject to intense regulation, and there is a maximum wage law.");
				break;
			case LAW_FREESPEECH:
				if(won==-1)addstr("Unacceptable speech is a capital crime.");
				else if(law[l]==-2)addstr("Speech is routinely suppressed.");
				else if(law[l]==-1)addstr("Some individuals are harassed because of their speech.");
				else if(law[l]==0)addstr("Free speech is tolerated.");
				else if(law[l]==1)addstr("Free speech is encouraged.");
				else addstr("Free speech is universally supported.");
				break;
			case LAW_FLAGBURNING:
				if(won==-1)addstr("Images or words describing flag burning are punished by death.");
				else if(law[l]==-2)addstr("Burning the flag is a crime on par with murder.");
				else if(law[l]==-1)addstr("Burning the flag is a felony.");
				else if(law[l]==0)addstr("Flag-burning is a misdemeanor.");
				else if(law[l]==1)addstr("Flag-burning is legal but stigmatized.");
				else addstr("Flag-burning is legal.");
				break;
			}

		y++;
		}

	int housemake[5]={0,0,0,0,0};
	for(int h=0;h<435;h++)
		{
		housemake[house[h]+2]++;
		}
	int lsum=housemake[3]+housemake[4]
		-housemake[0]-housemake[1];
	if(lsum<=-145)set_color(COLOR_RED,COLOR_BLACK,1);
	else if(lsum<0)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
	else if(lsum<145)set_color(COLOR_YELLOW,COLOR_BLACK,1);
	else if(housemake[4]<290)set_color(COLOR_BLUE,COLOR_BLACK,1);
	else set_color(COLOR_GREEN,COLOR_BLACK,1);
	char num[20];
	if(won!=-1)
		{
		move(6,0);
		addstr("House: ");
		itoa(housemake[4],num,10);
		addstr(num);addstr("Lib+, ");
		itoa(housemake[3],num,10);
		addstr(num);addstr("Lib, ");
		itoa(housemake[2],num,10);
		addstr(num);addstr("Mod, ");
		itoa(housemake[1],num,10);
		addstr(num);addstr("Cons, ");
		itoa(housemake[0],num,10);
		addstr(num);addstr("Cons+");
		}

	int senatemake[5]={0,0,0,0,0};
	for(int s=0;s<100;s++)
		{
		senatemake[senate[s]+2]++;
		}
	lsum=senatemake[3]+senatemake[4]
		-senatemake[0]-senatemake[1];
	if(lsum<=-33)set_color(COLOR_RED,COLOR_BLACK,1);
	else if(lsum<0)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
	else if(lsum<33)set_color(COLOR_YELLOW,COLOR_BLACK,1);
	else if(senatemake[4]<67)set_color(COLOR_BLUE,COLOR_BLACK,1);
	else set_color(COLOR_GREEN,COLOR_BLACK,1);
	if(won!=-1)
		{
		move(7,0);
		addstr("Senate: ");
		itoa(senatemake[4],num,10);
		addstr(num);addstr("Lib+, ");
		itoa(senatemake[3],num,10);
		addstr(num);addstr("Lib, ");
		itoa(senatemake[2],num,10);
		addstr(num);addstr("Mod, ");
		itoa(senatemake[1],num,10);
		addstr(num);addstr("Cons, ");
		itoa(senatemake[0],num,10);
		addstr(num);addstr("Cons+");
		}
	else
		{
		set_color(COLOR_RED,COLOR_BLACK,1);
		move(7,0);
		addstr("The Congress consists of CEOs and televangelists.");
		}

	int elibjudge=0;
	for(int c=0;c<9;c++)
		{
		if(court[c]==2)elibjudge++;
		}

	if(won==-1)set_color(COLOR_RED,COLOR_BLACK,1);
	else if(won==1||elibjudge>=5)set_color(COLOR_GREEN,COLOR_BLACK,1);
	else set_color(COLOR_WHITE,COLOR_BLACK,1);

	move(0,56);addch('S');
	move(1,56);addch('U');
	move(2,56);addch('P');
	move(3,56);addch('R');
	move(4,56);addch('E');
	move(5,56);addch('M');
	move(6,56);addch('E');

	move(0,58);addch('C');
	move(1,58);addch('O');
	move(2,58);addch('U');
	move(3,58);addch('R');
	move(4,58);addch('T');

	if(won!=-1)
		{
		y=0;

		for(int c=0;c<9;c++)
			{
			if(court[c]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
			else if(court[c]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
			else if(court[c]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
			else if(court[c]==1)set_color(COLOR_BLUE,COLOR_BLACK,1);
			else set_color(COLOR_GREEN,COLOR_BLACK,1);

			move(y,60);
			addstr(courtname[c]);

			y++;
			}
		}
	else
		{
		set_color(COLOR_RED,COLOR_BLACK,1);
		move(0,60);
		addstr("   Replaced");
		move(1,60);
		addstr(" By Corporate");
		move(2,60);
		addstr("Ethics Officers");
		}

	if(won==1)
		{
		set_color(COLOR_GREEN,COLOR_BLACK,1);
		move(23,0);
		addstr("The country has achieved Elite Liberal status.");
		move(24,0);
		addstr("Press 'L' to view the high score list.");

		do
			{
			refresh();
			int c=getch();
			translategetch(c);

			if(c=='l')break;
			}while(1);
		}
	else if(won==-1)
		{
		set_color(COLOR_RED,COLOR_BLACK,1);
		move(23,0);
		addstr("The country has been Reaganified.");
		move(24,0);
		addstr("Press 'L' to view the high score list.");

		do
			{
			refresh();
			int c=getch();
			translategetch(c);

			if(c=='l')break;
			}while(1);
		}
	else
		{
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(23,0);
		addstr("Once these are Green, the country will have achieved Elite Liberal status.");
		move(24,0);
		addstr("Press D to disband and wait.  Press any other key to consider the situation.");

		do
			{
			refresh();
			int c=getch();
			translategetch(c);

			if(c=='d')
				{
				return confirmdisband();
				}

			break;
			}while(1);
		}

	return 0;
}

char radio_broadcast(void)
{
	sitealarm=1;

	int enemy=0;
	for(int e=0;e<ENCMAX;e++)
		{
		if(encounter[e].exists&&encounter[e].alive)
			{
			if(encounter[e].align==-1)enemy++;
			}
		}

	if(enemy>0)
		{
		clearmessagearea();
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("The Conservatives in the room hurry the Squad, so");
		move(17,1);
		addstr("the broadcast never happens.");

		refresh();
		getch();
		return 0;
		}

	criminalizeparty(LAWFLAG2_DISTURBANCE,1);

	clearmessagearea();
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(16,1);
	addstr("The Squad takes control of the microphone and");
	move(17,1);
	int viewhit=LCSrandom(VIEWNUM);
	switch(viewhit)
		{
		case VIEW_ABORTION:addstr("discusses abortion.");break;
		case VIEW_GAY:addstr("discusses homosexual rights.");break;
		case VIEW_DEATHPENALTY:addstr("examines the death penalty.");break;
		case VIEW_TAXES:addstr("advocates taxes - high and fair.");break;
		case VIEW_NUCLEARPOWER:addstr("runs down nuclear power.");break;
		case VIEW_ANIMALRESEARCH:addstr("discusses the horrors of animal research.");break;
		case VIEW_POLICEBEHAVIOR:addstr("goes over cases of police brutality.");break;
		case VIEW_PRISONS:addstr("discusses the prison system.");break;
		case VIEW_INTELLIGENCE:addstr("debates privacy law.");break;
		case VIEW_FREESPEECH:addstr("talks about free speech.");break;
		case VIEW_GENETICS:addstr("discusses the implications of genetic research.");break;
		case VIEW_JUSTICES:addstr("talks about the record of a Conservative judge.");break;
		case VIEW_SWEATSHOPS:addstr("brings details about sweatshops to light.");break;
		case VIEW_POLLUTION:addstr("does a show on industrial pollution.");break;
		case VIEW_CORPORATECULTURE:addstr("jokes about corporate culture.");break;
		case VIEW_CEOSALARY:addstr("gives examples of CEO excesses.");break;
		case VIEW_AMRADIO:addstr("discusses other AM radio shows.");break;
		case VIEW_CABLENEWS:addstr("talks about Cable News.");break;
		case VIEW_LIBERALCRIMESQUAD:addstr("lets people know about the Liberal Crime Squad.");break;
		case VIEW_LIBERALCRIMESQUADPOS:addstr("extols the virtues of the Liberal Crime Squad.");break;
		}

	refresh();
	getch();

	int segmentpower=0;
	int usegmentpower;
	int partysize=0;
	int p = 0;

	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(!activesquad->squad[p]->alive)continue;

			segmentpower+=LCSrandom(activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE));
			segmentpower+=activesquad->squad[p]->attval(ATTRIBUTE_HEART);
			segmentpower+=LCSrandom(activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA));
			segmentpower+=activesquad->squad[p]->skill[SKILL_PERSUASION];
			activesquad->squad[p]->skill_ip[SKILL_PERSUASION]+=50;
			partysize++;
			}
		}

	if(partysize>1)segmentpower/=partysize;

	clearmessagearea();

	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(16,1);
	if(segmentpower<5)addstr("The Squad sounds wholly insane.");
	else if(segmentpower<7)addstr("The show really sucks.");
	else if(segmentpower<10)addstr("It is a very boring hour.");
	else if(segmentpower<15)addstr("It is mediocre radio.");
	else if(segmentpower<20)addstr("The show was all right.");
	else if(segmentpower<25)addstr("The Squad put on a good show.");
	else if(segmentpower<30)addstr("It was thought-provoking, even humorous.");
	else addstr("It was the best hour of AM radio EVER.");

	refresh();
	getch();

	//CHECK PUBLIC OPINION
	change_public_opinion(VIEW_LIBERALCRIMESQUAD,10,0);
	change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,(segmentpower-10)/2,0,0);
	if(viewhit!=VIEW_LIBERALCRIMESQUAD)change_public_opinion(viewhit,(segmentpower-10)/2,1,0);
	else change_public_opinion(viewhit,segmentpower/2,1,0);

	//PRISONER PARTS
	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(activesquad->squad[p]->prisoner!=NULL)
				{
				if(activesquad->squad[p]->prisoner->type==CREATURE_RADIOPERSONALITY)
					{
					viewhit=LCSrandom(VIEWNUM);
					clearmessagearea();

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(16,1);
					addstr("The hostage ");
					addstr(activesquad->squad[p]->prisoner->name);
					addstr(" is forced on to");
					move(17,1);
					switch(viewhit)
						{
						case VIEW_ABORTION:addstr("discuss abortion.");break;
						case VIEW_GAY:addstr("discuss homosexual rights.");break;
						case VIEW_DEATHPENALTY:addstr("examine the death penalty.");break;
						case VIEW_TAXES:addstr("advocate taxes - high and fair.");break;
						case VIEW_NUCLEARPOWER:addstr("run down nuclear power.");break;
						case VIEW_ANIMALRESEARCH:addstr("discuss the horrors of animal research.");break;
						case VIEW_POLICEBEHAVIOR:addstr("go over cases of police brutality.");break;
						case VIEW_PRISONS:addstr("discuss the prison system.");break;
						case VIEW_INTELLIGENCE:addstr("advocate privacy laws.");break;
						case VIEW_FREESPEECH:addstr("talk about free speech.");break;
						case VIEW_GENETICS:addstr("discuss the implications of genetic research.");break;
						case VIEW_JUSTICES:addstr("talk about the record of a Conservative judge.");break;
						case VIEW_SWEATSHOPS:addstr("bring details about sweatshops to light.");break;
						case VIEW_POLLUTION:addstr("do a segment on industrial pollution.");break;
						case VIEW_CORPORATECULTURE:addstr("joke about corporate culture.");break;
						case VIEW_CEOSALARY:addstr("give examples of CEO excesses.");break;
						case VIEW_AMRADIO:addstr("make fun of its own AM radio show.");break;
						case VIEW_CABLENEWS:addstr("talk about Cable News.");break;
						case VIEW_LIBERALCRIMESQUAD:addstr("let people know about the Liberal Crime Squad.");break;
						case VIEW_LIBERALCRIMESQUADPOS:addstr("extol the virtues of the Liberal Crime Squad.");break;
						}

					usegmentpower=10; //FAME BONUS
					usegmentpower+=LCSrandom(activesquad->squad[p]->prisoner->attval(ATTRIBUTE_INTELLIGENCE));
					usegmentpower+=activesquad->squad[p]->prisoner->attval(ATTRIBUTE_HEART);
					usegmentpower+=LCSrandom(activesquad->squad[p]->prisoner->attval(ATTRIBUTE_CHARISMA));
					usegmentpower+=activesquad->squad[p]->prisoner->skill[SKILL_PERSUASION];

					if(viewhit!=VIEW_LIBERALCRIMESQUAD)change_public_opinion(viewhit,(usegmentpower-10)/2,1,0);
					else change_public_opinion(viewhit,usegmentpower/2,1,0);

					segmentpower+=usegmentpower;

					refresh();
					getch();
					}
				else
					{
					clearmessagearea();

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(16,1);
					addstr(activesquad->squad[p]->prisoner->name);
					addstr(", the hostage, is kept off-air.");

					refresh();
					getch();
					}
				}
			}
		}

	if(sitealienate>=1&&segmentpower>=75)
		{
		sitealienate=0;

		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("Moderates at the station appreciated the show.");
		move(17,1);
		addstr("They no longer feel alienated.");

		refresh();
		getch();
		}

	//POST-SECURITY BLITZ IF IT SUCKED
	if(segmentpower<100)
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("Security is waiting for the Squad");
		move(17,1);
		addstr("after the show!");

		refresh();
		getch();

		int numleft=LCSrandom(8)+2;
		for(int e=0;e<ENCMAX;e++)
			{
			if(!encounter[e].exists)
				{
				makecreature(encounter[e],CREATURE_SECURITYGUARD);
				numleft--;
				}
			if(numleft==0)break;
			}
		}
	else
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("The show was so good that security listened to it");
		move(17,1);
		addstr("at their desks.  The Squad might yet escape.");

		refresh();
		getch();
		}

	return 1;
}

char news_broadcast(void)
{
	sitealarm=1;
	int p;

	int enemy=0;
	for(int e=0;e<ENCMAX;e++)
		{
		if(encounter[e].exists&&encounter[e].alive)
			{
			if(encounter[e].align==-1)enemy++;
			}
		}

	if(enemy>0)
		{
		clearmessagearea();
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("The Conservatives in the room hurry the Squad, so");
		move(17,1);
		addstr("the broadcast never happens.");

		refresh();
		getch();
		return 0;
		}

	criminalizeparty(LAWFLAG2_DISTURBANCE,1);

	clearmessagearea();
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(16,1);
	addstr("The Squad steps in front of the cameras and");
	move(17,1);
	int viewhit=LCSrandom(VIEWNUM);
	switch(viewhit)
		{
		case VIEW_ABORTION:addstr("discusses abortion.");break;
		case VIEW_GAY:addstr("discusses homosexual rights.");break;
		case VIEW_DEATHPENALTY:addstr("examines the death penalty.");break;
		case VIEW_TAXES:addstr("advocates taxes - high and fair.");break;
		case VIEW_NUCLEARPOWER:addstr("runs down nuclear power.");break;
		case VIEW_ANIMALRESEARCH:addstr("discusses the horrors of animal research.");break;
		case VIEW_POLICEBEHAVIOR:addstr("goes over cases of police brutality.");break;
		case VIEW_PRISONS:addstr("discusses the prison system.");break;
		case VIEW_INTELLIGENCE:addstr("debates privacy law.");break;
		case VIEW_FREESPEECH:addstr("talks about free speech.");break;
		case VIEW_GENETICS:addstr("discusses the implications of genetic research.");break;
		case VIEW_JUSTICES:addstr("talks about the record of a Conservative judge.");break;
		case VIEW_SWEATSHOPS:addstr("brings details about sweatshops to light.");break;
		case VIEW_POLLUTION:addstr("does a show on industrial pollution.");break;
		case VIEW_CORPORATECULTURE:addstr("jokes about corporate culture.");break;
		case VIEW_CEOSALARY:addstr("gives examples of CEO excesses.");break;
		case VIEW_AMRADIO:addstr("discusses AM radio shows.");break;
		case VIEW_CABLENEWS:addstr("talks about Cable News.");break;
		case VIEW_LIBERALCRIMESQUAD:addstr("lets people know about the Liberal Crime Squad.");break;
		case VIEW_LIBERALCRIMESQUADPOS:addstr("extols the virtues of the Liberal Crime Squad.");break;
		}

	refresh();
	getch();

	int segmentpower=0;
	int usegmentpower;
	int partysize=0;
	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(!activesquad->squad[p]->alive)continue;

			segmentpower+=LCSrandom(activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE));
			segmentpower+=activesquad->squad[p]->attval(ATTRIBUTE_HEART);
			segmentpower+=LCSrandom(activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA));
			segmentpower+=activesquad->squad[p]->skill[SKILL_PERSUASION];
			activesquad->squad[p]->skill_ip[SKILL_PERSUASION]+=50;
			partysize++;
			}
		}

	if(partysize>1)segmentpower/=partysize;

	clearmessagearea();

	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(16,1);
	if(segmentpower<5)addstr("The Squad looks completely insane.");
	else if(segmentpower<7)addstr("The show really sucks.");
	else if(segmentpower<10)addstr("It is a very boring hour.");
	else if(segmentpower<15)addstr("It is mediocre TV.");
	else if(segmentpower<20)addstr("The show was all right.");
	else if(segmentpower<25)addstr("The Squad put on a good show.");
	else if(segmentpower<30)addstr("It was thought-provoking, even humorous.");
	else addstr("It was the best hour of Cable TV EVER.");

	refresh();
	getch();

	//CHECK PUBLIC OPINION
	change_public_opinion(VIEW_LIBERALCRIMESQUAD,10,0);
	change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,(segmentpower-50)/10,0,0);
	if(viewhit!=VIEW_LIBERALCRIMESQUAD)change_public_opinion(viewhit,(segmentpower-50)/10,1,0);
	else change_public_opinion(viewhit,segmentpower/10,1,0);

	//PRISONER PARTS
	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(activesquad->squad[p]->prisoner!=NULL)
				{
				if(activesquad->squad[p]->prisoner->type==CREATURE_NEWSANCHOR)
					{
					viewhit=LCSrandom(VIEWNUM);
					clearmessagearea();

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(16,1);
					addstr("The hostage ");
					addstr(activesquad->squad[p]->prisoner->name);
					addstr(" is forced on to");
					move(17,1);
					switch(viewhit)
						{
						case VIEW_ABORTION:addstr("discuss abortion.");break;
						case VIEW_GAY:addstr("discuss homosexual rights.");break;
						case VIEW_DEATHPENALTY:addstr("examine the death penalty.");break;
						case VIEW_TAXES:addstr("advocate taxes - high and fair.");break;
						case VIEW_NUCLEARPOWER:addstr("run down nuclear power.");break;
						case VIEW_ANIMALRESEARCH:addstr("discuss the horrors of animal research.");break;
						case VIEW_POLICEBEHAVIOR:addstr("go over cases of police brutality.");break;
						case VIEW_PRISONS:addstr("discuss the prison system.");break;
						case VIEW_INTELLIGENCE:addstr("advocate privacy laws.");break;
						case VIEW_FREESPEECH:addstr("talk about free speech.");break;
						case VIEW_GENETICS:addstr("discuss the implications of genetic research.");break;
						case VIEW_JUSTICES:addstr("talk about the record of a Conservative judge.");break;
						case VIEW_SWEATSHOPS:addstr("bring details about sweatshops to light.");break;
						case VIEW_POLLUTION:addstr("do a segment on industrial pollution.");break;
						case VIEW_CORPORATECULTURE:addstr("joke about corporate culture.");break;
						case VIEW_CEOSALARY:addstr("give examples of CEO excesses.");break;
						case VIEW_AMRADIO:addstr("talk about AM radio.");break;
						case VIEW_CABLENEWS:addstr("make fun of its own Cable News show.");break;
						case VIEW_LIBERALCRIMESQUAD:addstr("let people know about the Liberal Crime Squad.");break;
						case VIEW_LIBERALCRIMESQUADPOS:addstr("extol the virtues of the Liberal Crime Squad.");break;
						}

					usegmentpower=10; //FAME BONUS
					usegmentpower+=LCSrandom(activesquad->squad[p]->prisoner->attval(ATTRIBUTE_INTELLIGENCE));
					usegmentpower+=activesquad->squad[p]->prisoner->attval(ATTRIBUTE_HEART);
					usegmentpower+=LCSrandom(activesquad->squad[p]->prisoner->attval(ATTRIBUTE_CHARISMA));
					usegmentpower+=activesquad->squad[p]->prisoner->skill[SKILL_PERSUASION];

					if(viewhit!=VIEW_LIBERALCRIMESQUAD)change_public_opinion(viewhit,(usegmentpower-10)/2,1,0);
					else change_public_opinion(viewhit,usegmentpower/2,1,0);

					segmentpower+=usegmentpower;

					refresh();
					getch();
					}
				else
					{
					clearmessagearea();

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(16,1);
					addstr(activesquad->squad[p]->prisoner->name);
					addstr(", the hostage, is kept off-air.");

					refresh();
					getch();
					}
				}
			}
		}

	if(sitealienate>=1&&segmentpower>=75)
		{
		sitealienate=0;

		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("Moderates at the station appreciated the show.");
		move(17,1);
		addstr("They no longer feel alienated.");

		refresh();
		getch();
		}

	//POST-SECURITY BLITZ IF IT SUCKED
	if(segmentpower<100)
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("Security is waiting for the Squad");
		move(17,1);
		addstr("after the show!");

		refresh();
		getch();

		int numleft=LCSrandom(8)+2;
		for(int e=0;e<ENCMAX;e++)
			{
			if(!encounter[e].exists)
				{
				makecreature(encounter[e],CREATURE_SECURITYGUARD);
				numleft--;
				}
			if(numleft==0)break;
			}
		}
	else
		{
		clearmessagearea();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(16,1);
		addstr("The show was so entertaining that security watched it");
		move(17,1);
		addstr("at their desks.  The Squad might yet escape.");

		refresh();
		getch();
		}

	return 1;
}

void change_public_opinion(int v,int power,char affect,char cap)
{
	if(v==VIEW_LIBERALCRIMESQUAD)affect=0;
	if(v==VIEW_LIBERALCRIMESQUADPOS)affect=0;

	int effpower=power;
	if(affect)
		{
		int aff=attitude[VIEW_LIBERALCRIMESQUAD];
		int rawpower=(int)((float)power * (float)(100-aff)/100.0f);
		int affectedpower=power-rawpower;

		if(affectedpower>0)
			{
			int dist=attitude[VIEW_LIBERALCRIMESQUADPOS]-50;
			if(dist<0)dist*=-1;
			affectedpower=(int)(((float)affectedpower*(float)dist)/10.0f);

			if(attitude[VIEW_LIBERALCRIMESQUADPOS]<50)affectedpower*=-1;
			}

		effpower=rawpower+affectedpower;
		}

	if(v==VIEW_LIBERALCRIMESQUAD)
		{
		if(effpower<-50)effpower=-50;
		if(effpower>50)effpower=50;
		}
	else if(v==VIEW_LIBERALCRIMESQUADPOS)
		{
		if(effpower<-80)effpower=-80;
		if(effpower>80)effpower=80;
		}
	else if(cap)
		{
		if(effpower<-10)effpower=-10;
		if(effpower>10)effpower=10;
		}

	attitude[v]+=effpower;
	if(attitude[v]<0)attitude[v]=0;
	if(attitude[v]>100)attitude[v]=100;
}

void passmonth(char &clearformess,char canseethings)
{
	short oldlaw[LAWNUM];
	memmove(oldlaw,law,sizeof(short)*LAWNUM);
	int l, v, p;

	//TIME ADVANCE
	day=1;
	month++;
	if(month==13)
		{
		month=1;
		year++;
		}

	//CLEAR RENT EXEMPTIONS
	for(l=0;l<location.size();l++)location[l]->newrental=0;

	//YOUR PAPER AND PUBLIC OPINION AND STUFF
	vector<int> nploc;
	for(l=0;l<location.size();l++)
		{
		if((location[l]->compound_walls & COMPOUND_PRINTINGPRESS)&&
			!location[l]->siege.siege)nploc.push_back(l);
		}

	if(nploc.size()>0&&!disbanding)
		{
		short power=nploc.size();

		//CRIMINALIZE THE PRINTERS
		long writingpower=0;
		for(int l=0;l<nploc.size();l++)
			{
			criminalizepool(LAWFLAG_SPEECH,-1,0,nploc[l]);
			for(int p=0;p<pool.size();p++)
				{
				if(pool[p]->location==nploc[l]&&
					pool[p]->alive&&
					pool[p]->align==1)
					{
					writingpower+=LCSrandom(pool[p]->skill[SKILL_PERSUASION]+
						pool[p]->skill[SKILL_WRITING]*3+
						pool[p]->attval(ATTRIBUTE_INTELLIGENCE)+
						pool[p]->attval(ATTRIBUTE_CHARISMA)+
						pool[p]->attval(ATTRIBUTE_HEART)*2+1);
					pool[p]->skill_ip[SKILL_WRITING]+=LCSrandom(2)+1;
					pool[p]->skill_ip[SKILL_PERSUASION]+=LCSrandom(2)+1;
					}
				}
			}

		//DO EMPHASIS OF SITE EVENTS
		int vpower,val;
		for(int v=0;v<VIEWNUM;v++)
			{
			val=newspaper_topicwork[v];
			if(LCSrandom(100)<writingpower)val++;
			if(LCSrandom(100)<writingpower)val++;
			if(LCSrandom(100)<writingpower)val++;
			if(LCSrandom(1000)<writingpower)val++;
			if(LCSrandom(1000)<writingpower)val++;
			if(LCSrandom(1000)<writingpower)val++;
			if(LCSrandom(10000)<writingpower)val++;
			if(LCSrandom(10000)<writingpower)val++;
			if(LCSrandom(10000)<writingpower)val++;
			if(LCSrandom(10000)<writingpower)val++;
			if(val>newspaper_topicwork[v])val=newspaper_topicwork[v];
			vpower=power*val;
			if(vpower>0)
				{
				change_public_opinion(v,vpower,1,0);
				}
			}

		change_public_opinion(VIEW_LIBERALCRIMESQUAD,power*5,0,0);

		//DO SPECIAL EDITIONS
		int loottype=choosespecialedition(clearformess);

		if(loottype!=-1)
			{
			printnews(loottype,power);

			for(int p=0;p<pool.size();p++)
				{
				if(pool[p]->alive&&pool[p]->align==1&&
					pool[p]->clinic==0&&pool[p]->dating==0&&
					pool[p]->hiding==0)
					{
					if(pool[p]->location!=-1)
						{
						if(location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON&&
							location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
							location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION)
							{
							if(loottype==LOOT_INTHQDISK||
								loottype==LOOT_SECRETDOCUMENTS)addjuice(*pool[p],100);
							else addjuice(*pool[p],50);
							}
						}
					}
				}

			if(loottype==LOOT_INTHQDISK||
				loottype==LOOT_SECRETDOCUMENTS)
				{
				for(int l=0;l<nploc.size();l++)
					{
					criminalizepool(LAWFLAG_TREASON,-1,0,nploc[l]);
					}
				}
			}
		}

	//STORIES STALE EVEN IF NOT PRINTED
	for(v=0;v<VIEWNUM;v++)newspaper_topicwork[v]=0;

	//HAVING SLEEPERS
	for(int pl=0;pl<pool.size();pl++)
		{
		if(pool[pl]->alive&&(pool[pl]->flag & CREATUREFLAG_SLEEPER))
			{
			sleepereffect(*pool[pl],clearformess);
			}
		}

	//PUBLIC OPINION NATURAL MOVES
	for(v=0;v<VIEWNUM;v++)
		{
		if(v==VIEW_LIBERALCRIMESQUADPOS)continue;
		if(v==VIEW_LIBERALCRIMESQUAD)continue;
		if(!LCSrandom(3)&&v!=VIEW_AMRADIO&&v!=VIEW_CABLENEWS)
			{
			//DRIFTS DOWN DEPENDING ON AM RADIO AND CABLE NEWS
			int conspower=200-attitude[VIEW_AMRADIO]-attitude[VIEW_CABLENEWS];

			if(LCSrandom(200)>conspower)change_public_opinion(v,LCSrandom(2)*2-1,0);
			else change_public_opinion(v,-1,0);
			}

		if(v==VIEW_AMRADIO||v==VIEW_CABLENEWS)
			{
			if(!LCSrandom(3))change_public_opinion(v,-1,0);
			}
		}

	//ELECTIONS
	if(month==11){elections(clearformess,canseethings);clearformess=1;}

	//SUPREME COURT
	if(month==6){supremecourt(clearformess,canseethings);clearformess=1;}

	//CONGRESS
	if(month==3){congress(clearformess,canseethings);clearformess=1;}
	if(month==9){congress(clearformess,canseethings);clearformess=1;}

	//REAGANIFY?
	if(publicmood(-1)<=8)
		{
		reaganify(canseethings);
		clearformess=1;
		}

	//DID YOU WIN?
	if(wincheck())
		{
		liberalagenda(1);
		savehighscore(END_WON);
		reset();
		viewhighscores();
		endwin();
		exit(0);
		}

	//CONTROL LONG DISBANDS
	if(disbanding&&year-disbandtime>=50)
		{
		set_color(COLOR_WHITE,COLOR_BLACK,1);

		erase();
		move(12,10);
		addstr("You disappeared safely, but you hadn't done enough.");
		refresh();
		getch();

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		erase();
		move(12,12);
		addstr("The Conservatives have made the world in their image.");
		refresh();
		getch();

		set_color(COLOR_BLACK,COLOR_BLACK,1);
		erase();
		move(12,14);
		addstr("They'll round the last of you up eventually.  All is lost.");
		refresh();
		getch();

		savehighscore(END_DISBANDLOSS);
		reset();
		viewhighscores();
		endwin();
		exit(0);
		}

	//UPDATE THE WORLD IN CASE THE LAWS HAVE CHANGED
	updateworld_laws(law,oldlaw);

	//THE SYSTEM!
	for(p=pool.size()-1;p>=0;p--)
		{
		if(disbanding)break;

		if(!pool[p]->alive)continue;
		if(pool[p]->flag & CREATUREFLAG_SLEEPER)continue;
		if(pool[p]->location==-1)continue;

		if(location[pool[p]->location]->type==SITE_GOVERNMENT_POLICESTATION)
			{
			if(clearformess)
				{
				erase();
				}
			else
				{
				makedelimiter(8,0);
				}

			if(pool[p]->flag & CREATUREFLAG_ILLEGALALIEN)
				{
				set_color(COLOR_MAGENTA,COLOR_BLACK,1);
				move(8,1);
				addstr(pool[p]->name);
				addstr(" has been shipped out to the INS to face deportation.");

				refresh();
				getch();

				removesquadinfo(*pool[p]);
				delete pool[p];
				pool.erase(pool.begin() + p);
				continue;
				}
			else
				{
				//TRY TO GET RACKETEERING CHARGE
				int copstrength=100;
				if(law[LAW_POLICEBEHAVIOR]==-2)copstrength=200;
				if(law[LAW_POLICEBEHAVIOR]==-1)copstrength=150;
				if(law[LAW_POLICEBEHAVIOR]==1)copstrength=75;
				if(law[LAW_POLICEBEHAVIOR]==2)copstrength=50;

				if(LCSrandom(copstrength)>pool[p]->juice+pool[p]->attval(ATTRIBUTE_HEART,0)*5&&pool[p]->hireid!=-1)
					{
					if(pool[p]->hireid!=-1)
						{
						for(int p2=0;p2<pool.size();p2++)
							{
							if(pool[p2]->alive==1&&pool[p2]->id==pool[p]->hireid)
								{
								char conf=0;
								if(pool[p2]->location==-1)conf=1;
								else if(location[pool[p2]->location]->type!=SITE_GOVERNMENT_PRISON)conf=1;

								if(conf)
									{
									pool[p2]->lawflag2|=LAWFLAG2_RACKETEERING;
									}
								}
							}
						}

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(8,1);
					addstr(pool[p]->name);
					addstr(" has broken under the pressure and ratted you out!");

					refresh();
					getch();

					removesquadinfo(*pool[p]);
					delete pool[p];
					pool.erase(pool.begin() + p);
					continue;
					}
				else
					{
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(8,1);
					addstr(pool[p]->name);
					addstr(" is moved to the courthouse for trial.");

					refresh();
					getch();
					}

				for(int l=0;l<location.size();l++)
					{
					if(location[l]->type==SITE_GOVERNMENT_COURTHOUSE)
						{
						pool[p]->location=l;
						}
					}
				pool[p]->armor.type=ARMOR_PRISONER;
				}
			}
		else if(location[pool[p]->location]->type==SITE_GOVERNMENT_COURTHOUSE)
			{
			trial(*pool[p]);
			clearformess=1;
			}
		else if(location[pool[p]->location]->type==SITE_GOVERNMENT_PRISON)
			{
			if(prison(*pool[p]))clearformess=1;
			}
		}

	//NUKE EXECUTION VICTIMS
	for(p=pool.size()-1;p>=0;p--)
		{
		if(pool[p]->location==-1)continue;

		if(location[pool[p]->location]->type==SITE_GOVERNMENT_PRISON&&!pool[p]->alive)
			{
			removesquadinfo(*pool[p]);
			delete pool[p];
			pool.erase(pool.begin() + p);
			}
		}

	//MUST DO AN END OF GAME CHECK HERE BECAUSE OF EXECUTIONS
	endcheck(END_EXECUTED);

	//DISPERSAL CHECK
	dispersalcheck(clearformess);

	//FUND REPORTS
	fundreport(clearformess);

	//HEAL CLINIC PEOPLE AND TRAIN
	for(p=0;p<pool.size();p++)
		{
		if(disbanding)break;

		if(pool[p]->clinic>0)
			{
			pool[p]->clinic--;

			for(int w=0;w<BODYPARTNUM;w++)
				{
				if((pool[p]->wound[w] & WOUND_NASTYOFF)||
					(pool[p]->wound[w] & WOUND_CLEANOFF))
					{
					pool[p]->wound[w]=(char)WOUND_CLEANOFF;
					}
				else pool[p]->wound[w]=0;
				}

			if(pool[p]->special[SPECIALWOUND_RIGHTLUNG]!=1)
				{
				pool[p]->special[SPECIALWOUND_RIGHTLUNG]=1;
				if(LCSrandom(2))
					{
					pool[p]->att[ATTRIBUTE_HEALTH]--;
					if(pool[p]->att[ATTRIBUTE_HEALTH]<=0)
						{
						pool[p]->att[ATTRIBUTE_HEALTH]=1;
						}
					}
				}
			if(pool[p]->special[SPECIALWOUND_LEFTLUNG]!=1)
				{
				pool[p]->special[SPECIALWOUND_LEFTLUNG]=1;
				if(LCSrandom(2))
					{
					pool[p]->att[ATTRIBUTE_HEALTH]--;
					if(pool[p]->att[ATTRIBUTE_HEALTH]<=0)
						{
						pool[p]->att[ATTRIBUTE_HEALTH]=1;
						}
					}
				}
			if(pool[p]->special[SPECIALWOUND_HEART]!=1)
				{
				pool[p]->special[SPECIALWOUND_HEART]=1;
				if(LCSrandom(3))
					{
					pool[p]->att[ATTRIBUTE_HEALTH]--;
					if(pool[p]->att[ATTRIBUTE_HEALTH]<=0)
						{
						pool[p]->att[ATTRIBUTE_HEALTH]=1;
						}
					}
				}
			pool[p]->special[SPECIALWOUND_LIVER]=1;
			pool[p]->special[SPECIALWOUND_STOMACH]=1;
			pool[p]->special[SPECIALWOUND_RIGHTKIDNEY]=1;
			pool[p]->special[SPECIALWOUND_LEFTKIDNEY]=1;
			pool[p]->special[SPECIALWOUND_SPLEEN]=1;
			pool[p]->special[SPECIALWOUND_RIBS]=RIBNUM;
			if(!pool[p]->special[SPECIALWOUND_NECK])
				{
				pool[p]->special[SPECIALWOUND_NECK]=2;
				}
			if(!pool[p]->special[SPECIALWOUND_UPPERSPINE])
				{
				pool[p]->special[SPECIALWOUND_UPPERSPINE]=2;
				}
			if(!pool[p]->special[SPECIALWOUND_LOWERSPINE])
				{
				pool[p]->special[SPECIALWOUND_LOWERSPINE]=2;
				}

			if(pool[p]->blood<=20&&pool[p]->clinic<=2)pool[p]->blood=50;
			if(pool[p]->blood<=50&&pool[p]->clinic<=1)pool[p]->blood=75;

			if(pool[p]->clinic==0)
				{
				pool[p]->blood=100;
				if(clearformess)
					{
					erase();
					}
				else
					{
					makedelimiter(8,0);
					}

				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(8,1);
				addstr(pool[p]->name);
				addstr(" has left ");
				addstr(location[pool[p]->location]->name);
				addstr(".");

				int hs=-1;
				for(int l=0;l<location.size();l++)
					{
					if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
						{
						hs=l;
						break;
						}
					}
				if (hs==-1)
				{
					//TODO: Error unable to find location
					hs=0;
				}

				if(location[pool[p]->base]->siege.siege)
					{
					pool[p]->base=hs;
					}

				pool[p]->location=pool[p]->base;

				refresh();
				getch();
				}
			}
		}
}

void elections(char clearformess,char canseethings)
{
	char num[20];
	int l, p, c;

	if(canseethings)
		{
		if(clearformess)
			{
			erase();
			}
		else
			{
			makedelimiter(8,0);
			}
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(8,1);
		addstr("The Elections are being held today!");

		refresh();
		getch();
		}

	int mood=publicmood(-1);

	//PRESIDENTIAL
	if(year%4==0)
		{
		if(canseethings)
			{
			erase();

			set_color(COLOR_WHITE,COLOR_BLACK,1);

			move(0,0);
			addstr("Presidential Election ");
			itoa(year,num,10);
			addstr(num);
			}

		char candidate[5][80];
		int votes[5]={0,0,0,0,0};
		int oldwinnerspot=-1;
		for(c=0;c<5;c++)
			{
			if(c==0)set_color(COLOR_RED,COLOR_BLACK,1);
			else if(c==1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
			else if(c==2)set_color(COLOR_YELLOW,COLOR_BLACK,1);
			else if(c==3)set_color(COLOR_BLUE,COLOR_BLACK,1);
			else set_color(COLOR_GREEN,COLOR_BLACK,1);

			if(execterm==1&&c==exec[EXEC_PRESIDENT]+2)
				{
				oldwinnerspot=c;
				strcpy(candidate[c],execname[EXEC_PRESIDENT]);
				}
			else name(candidate[c]);

			if(canseethings)
				{
				move(6-c,0);
				addstr(candidate[c]);
				if(c==oldwinnerspot)addstr(", Incumbent");
				}
			}

		if(canseethings)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(8,0);
			addstr("Press any key to watch the election unfold.");

			refresh();
			getch();

			nodelay(stdscr,TRUE);
			}

		int winner=-1;
		char recount=0;
		char oldwinner=0;
		int vote;
		for(int l=0;l<100;l++)
			{
			vote=0;

			if(LCSrandom(100)<mood)vote++;
			if(LCSrandom(100)<mood)vote++;
			if(LCSrandom(100)<mood)vote++;
			if(LCSrandom(100)<mood)vote++;

			votes[vote]++;

			if(l==99)
				{
				int maxvote=0;

				for(c=0;c<5;c++)
					{
					if(votes[c]>maxvote)maxvote=votes[c];
					}

				vector<int> eligible;

				for(c=0;c<5;c++)
					{
					if(votes[c]==maxvote)
						{
						eligible.push_back(c);
						}
					}

				if(eligible.size()>1)
					{
					winner=eligible[LCSrandom(eligible.size())];
					recount=1;
					}
				else winner=eligible[0];
				}

			if(canseethings)
				{
				for(int c=0;c<5;c++)
					{
					if(winner>=0&&c!=winner)set_color(COLOR_BLACK,COLOR_BLACK,1);
					else if(c==winner)set_color(COLOR_WHITE,COLOR_BLACK,1);
					else set_color(COLOR_WHITE,COLOR_BLACK,0);
					move(6-c,40);
					itoa(votes[c],num,10);
					addstr(num);
					addch('%');
					if(c==winner&&recount)addstr(" (After Recount)");
					}

				refresh();

				pause_ms(50);
				getch();
				}
			}

		if(canseethings)nodelay(stdscr,FALSE);

		if(winner==oldwinnerspot)oldwinner=1;

		if(canseethings)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(8,0);
			addstr("Press any key to continue the elections.   ");

			refresh();
			getch();
			}

		//CONSTRUCT EXECUTIVE BRANCH
		if(oldwinner)
			{
			execterm=2;
			}
		else
			{
			execterm=1;
			exec[EXEC_PRESIDENT]=winner-2;
			strcpy(execname[EXEC_PRESIDENT],candidate[winner]);

			for(int e=0;e<EXECNUM;e++)
				{
				if(e==EXEC_PRESIDENT)continue;
				if(winner-2==-2)exec[e]=-2;
				else if(winner-2==2)exec[e]=2;
				else exec[e]=winner+LCSrandom(3)-3;
				name(execname[e]);
				}
			}
		}

	//SENATE
	if(year%2==0)
		{
		int senmod=-1;
		if(year%6==0)
			{
			senmod=0;
			}
		if(year%6==2)
			{
			senmod=1;
			}
		if(year%6==4)
			{
			senmod=2;
			}
			
		if(canseethings)
			{
			erase();

			set_color(COLOR_WHITE,COLOR_BLACK,1);

			move(0,0);
			addstr("Senate Elections ");
			itoa(year,num,10);
			addstr(num);
			}

		int x=0,y=2, s=0;

		for(s=0;s<100;s++)
			{
			if(s%3!=senmod)continue;

			if(canseethings)
				{
				move(y,x);

				if(senate[s]==-2)
					{
					set_color(COLOR_RED,COLOR_BLACK,1);
					addstr("Arch-Conservative");
					}
				else if(senate[s]==-1)
					{
					set_color(COLOR_MAGENTA,COLOR_BLACK,1);
					addstr("Conservative");
					}
				else if(senate[s]==0)
					{
					set_color(COLOR_YELLOW,COLOR_BLACK,1);
					addstr("moderate");
					}
				else if(senate[s]==1)
					{
					set_color(COLOR_BLUE,COLOR_BLACK,1);
					addstr("Liberal");
					}
				else
					{
					set_color(COLOR_GREEN,COLOR_BLACK,1);
					addstr("Elite Liberal");
					}
				}

			x+=20;
			if(x>70)
				{
				x=0;
				y++;
				}
			}

		if(canseethings)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(20,0);
			addstr("Press any key to watch the elections unfold.");

			refresh();
			getch();

			nodelay(stdscr,TRUE);
			}

		int vote;

		x=0;
		y=2;

		for(s=0;s<100;s++)
			{
			if(s%3!=senmod)continue;

			vote=0;

			if(LCSrandom(100)<mood)vote++;
			if(LCSrandom(100)<mood)vote++;
			if(LCSrandom(100)<mood)vote++;
			if(LCSrandom(100)<mood)vote++;

			senate[s]=vote-2;

			if(canseethings)
				{
				move(y,x);

				if(senate[s]==-2)
					{
					set_color(COLOR_RED,COLOR_BLACK,1);
					addstr("Arch-Conservative");
					}
				else if(senate[s]==-1)
					{
					set_color(COLOR_MAGENTA,COLOR_BLACK,1);
					addstr("Conservative      ");
					}
				else if(senate[s]==0)
					{
					set_color(COLOR_YELLOW,COLOR_BLACK,1);
					addstr("moderate          ");
					}
				else if(senate[s]==1)
					{
					set_color(COLOR_BLUE,COLOR_BLACK,1);
					addstr("Liberal           ");
					}
				else
					{
					set_color(COLOR_GREEN,COLOR_BLACK,1);
					addstr("Elite Liberal     ");
					}
				}

			x+=20;
			if(x>70)
				{
				x=0;
				y++;
				}

			if(canseethings)
				{
				refresh();
				pause_ms(50);

				getch();
				}
			}

		if(canseethings)
			{
			nodelay(stdscr,FALSE);

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(20,0);
			addstr("Press any key to continue the elections.    ");

			refresh();
			getch();
			}
		}

	//HOUSE
	if(year%2==0)
		{
		if(canseethings)
			{
			erase();

			set_color(COLOR_WHITE,COLOR_BLACK,1);

			move(0,0);
			addstr("House Elections ");
			itoa(year,num,10);
			addstr(num);
			}

		int x=0,y=2, h=0;

		for(h=0;h<435;h++)
			{
			if(canseethings)
				{
				move(y,x);

				if(house[h]==-2)
					{
					set_color(COLOR_RED,COLOR_BLACK,1);
					addstr("C+");
					}
				else if(house[h]==-1)
					{
					set_color(COLOR_MAGENTA,COLOR_BLACK,1);
					addstr("C ");
					}
				else if(house[h]==0)
					{
					set_color(COLOR_YELLOW,COLOR_BLACK,1);
					addstr("m ");
					}
				else if(house[h]==1)
					{
					set_color(COLOR_BLUE,COLOR_BLACK,1);
					addstr("L ");
					}
				else
					{
					set_color(COLOR_GREEN,COLOR_BLACK,1);
					addstr("L+");
					}
				}

			x+=3;
			if(x>78)
				{
				x=0;
				y++;
				}
			}

		if(canseethings)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(20,0);
			addstr("Press any key to watch the elections unfold.");

			refresh();
			getch();

			nodelay(stdscr,TRUE);
			}

		int vote;

		x=0;
		y=2;

		for(h=0;h<435;h++)
			{
			vote=0;

			if(LCSrandom(100)<mood)vote++;
			if(LCSrandom(100)<mood)vote++;
			if(LCSrandom(100)<mood)vote++;
			if(LCSrandom(100)<mood)vote++;

			house[h]=vote-2;

			if(canseethings)
				{
				move(y,x);

				if(house[h]==-2)
					{
					set_color(COLOR_RED,COLOR_BLACK,1);
					addstr("C+");
					}
				else if(house[h]==-1)
					{
					set_color(COLOR_MAGENTA,COLOR_BLACK,1);
					addstr("C ");
					}
				else if(house[h]==0)
					{
					set_color(COLOR_YELLOW,COLOR_BLACK,1);
					addstr("m ");
					}
				else if(house[h]==1)
					{
					set_color(COLOR_BLUE,COLOR_BLACK,1);
					addstr("L ");
					}
				else
					{
					set_color(COLOR_GREEN,COLOR_BLACK,1);
					addstr("L+");
					}
				}

			x+=3;
			if(x>78)
				{
				x=0;
				y++;
				}

			if(canseethings)
				{
				refresh();

				pause_ms(10);

				getch();
				}
			}

		if(canseethings)
			{
			nodelay(stdscr,FALSE);

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(20,0);
			addstr("Press any key to continue the elections.    ");

			refresh();
			getch();
			}
		}

	//PROPOSITIONS
	if(canseethings)
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,1);

		move(0,0);
		addstr("Important Propositions ");
		itoa(year,num,10);
		addstr(num);
		}

	vector<int> prop;
	vector<int> propdir;
	int pnum=LCSrandom(5)+2;
	char lawtaken[LAWNUM];
	memset(lawtaken,0,LAWNUM*sizeof(char));

	char lawpriority[LAWNUM];
	memset(lawpriority,0,LAWNUM*sizeof(char));
	char lawdir[LAWNUM];
	memset(lawdir,0,LAWNUM*sizeof(char));
	//DETERMINE PROPS
	int pmood,pvote;
	for(l=0;l<LAWNUM;l++)
		{
		pmood=publicmood(l);
		pvote=0;
		if(LCSrandom(100)<pmood)pvote++;
		if(LCSrandom(100)<pmood)pvote++;
		if(LCSrandom(100)<pmood)pvote++;
		if(LCSrandom(100)<pmood)pvote++;
		pvote-=2;

		if(law[l]<pvote)lawdir[l]=1;
		if(law[l]>=pvote)lawdir[l]=-1;
		if(law[l]==-2)lawdir[l]=1;
		if(law[l]==2)lawdir[l]=-1;

		//CALC PRIORITY
		if(law[l]==-2)pvote=0;
		else if(law[l]==-1)pvote=25;
		else if(law[l]==0)pvote=50;
		else if(law[l]==1)pvote=75;
		else pvote=100;

		lawpriority[l]=abs(pvote-pmood)+LCSrandom(10);
		}

	vector<int> canlaw;

	prop.resize(pnum);
	propdir.resize(pnum);
	for(p=0;p<pnum;p++)
		{
		int maxprior=0;
		for(l=0;l<LAWNUM;l++)
			{
			if(lawpriority[l]>maxprior&&!lawtaken[l])maxprior=lawpriority[l];
			}
		canlaw.clear();
		for(l=0;l<LAWNUM;l++)
			{
			if(lawpriority[l]==maxprior&&!lawtaken[l])
				{
				canlaw.push_back(l);
				}
			}

		prop[p]=canlaw[LCSrandom(canlaw.size())];

		lawtaken[prop[p]]=1;

		propdir[p]=lawdir[prop[p]];

		if(canseethings)
			{
			move(p*3+2,0);
			int propnum=0;
			switch(p)
				{
				case 0:propnum=2*(17-LCSrandom(2)*6)*(19-LCSrandom(2)*6);break;
				case 1:propnum=7*(17-LCSrandom(2)*6)*(19-LCSrandom(2)*6);break;
				case 2:propnum=3*(17-LCSrandom(2)*6)*(19-LCSrandom(2)*6);break;
				case 3:propnum=5*(17-LCSrandom(2)*6)*(2-LCSrandom(2)*1);break;
				case 4:propnum=11*(17-LCSrandom(2)*6)*(2-LCSrandom(2)*1);break;
				case 5:propnum=13*(17-LCSrandom(2)*6)*(2-LCSrandom(2)*1);break;
				}
			itoa(propnum,num,10);
			addstr("Proposition ");addstr(num);addstr(":");
			move(p*3+2,18);
			addstr("To ");
			switch(prop[p])
				{
				case LAW_ABORTION:
					if(propdir[p]==1)addstr("Strengthen Abortion Rights");
					else addstr("Protect the Unborn Child");
					break;
				case LAW_ANIMALRESEARCH:
					if(propdir[p]==1)addstr("Limit Animal Cruelty");
					else addstr("Expand Animal Research");
					break;
				case LAW_POLICEBEHAVIOR:
					if(propdir[p]==1)addstr("Curtail Police Misconduct");
					else addstr("Stop Harassment of Police Officers");
					break;
				case LAW_PRIVACY:
					if(propdir[p]==1)addstr("Enhance Privacy Protection");
					else addstr("Allow Corporations Access to Information");
					break;
				case LAW_DEATHPENALTY:
					if(propdir[p]==1)addstr("Limit the Death Penalty");
					else addstr("Expand Capital Punishment");
					break;
				case LAW_NUCLEARPOWER:
					if(propdir[p]==1)addstr("Limit Nuclear Power");
					else addstr("Expand Nuclear Power");
					break;
				case LAW_POLLUTION:
					if(propdir[p]==1)addstr("Punish Polluters");
					else addstr("Reward Industry");
					break;
				case LAW_LABOR:
					if(propdir[p]==1)addstr("Enhance Labor Standards");
					else addstr("End Undue Union Influence");
					break;
				case LAW_GAY:
					if(propdir[p]==1)addstr("Expand Homosexual Rights");
					else addstr("Support the Sanctity of Marriage");
					break;
				case LAW_CORPORATE:
					if(propdir[p]==1)addstr("Stop Corporate Criminals");
					else addstr("Reward Small Businesses");
					break;
				case LAW_FREESPEECH:
					if(propdir[p]==1)addstr("Protect Free Speech");
					else addstr("Limit Hurtful Speech");
					break;
				case LAW_TAX:
					if(propdir[p]==1)addstr("Punish the Wealthy");
					else addstr("Stimulate Economic Growth");
					break;
				case LAW_FLAGBURNING:
					if(propdir[p]==1)addstr("Limit Prohibitions on Flag Burning");
					else addstr("Protect the Symbol of Our Nation");
					break;
				}

			refresh();
			}
		}

	if(canseethings)
		{
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(20,0);
		addstr("Press any key to watch the elections unfold.");

		refresh();
		getch();

		nodelay(stdscr,TRUE);
		}

	for(p=0;p<pnum;p++)
		{
		char yeswin=0;
		int yesvotes=0;
		char recount=0;
		int vote;
		mood=publicmood(prop[p]);
		for(int l=0;l<100;l++)
			{
			vote=0;
			if(LCSrandom(100)<mood)vote++;
			if(LCSrandom(100)<mood)vote++;
			if(LCSrandom(100)<mood)vote++;
			if(LCSrandom(100)<mood)vote++;
			vote-=2;

			if(law[prop[p]]>vote && propdir[p]==-1)yesvotes++;
			if(law[prop[p]]<vote && propdir[p]==1)yesvotes++;

			if(l==99)
				{
				if(yesvotes>50)yeswin=1;
				else if(yesvotes==50)
					{
					if(!LCSrandom(2))yeswin=1;
					recount=1;
					}
				}

			if(canseethings)
				{
				if(l==99&&yeswin)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(l==99)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(p*3+2,72);
				itoa(yesvotes,num,10);
				addstr(num);
				addstr("% Yes");

				if(l==99&&!yeswin)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(l==99)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(p*3+3,72);
				itoa(l+1-yesvotes,num,10);
				addstr(num);
				addstr("% No");

				refresh();

				pause_ms(20);
				getch();
				}
			}

		if(canseethings)
			{
			if(recount)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(p*3+3,0);
				addstr("A Recount was Necessary");
				}
			}

		if(yeswin)law[prop[p]]+=propdir[p];
		}

	if(canseethings)
		{
		nodelay(stdscr,FALSE);

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(20,0);
		addstr("Press any key to reflect on what has happened.");

		refresh();
		getch();
		}
}

void supremecourt(char clearformess,char canseethings)
{
	int c;
	if(canseethings)
		{
		if(clearformess)
			{
			erase();
			}
		else
			{
			makedelimiter(8,0);
			}
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(8,1);
		addstr("The Supreme court is handing down decisions!");

		refresh();
		getch();
		}

	//CHANGE THINGS AND REPORT
	char num[20];
	if(canseethings)
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,1);

		move(0,0);
		addstr("Supreme Court Watch ");
		itoa(year,num,10);
		addstr(num);
		}

	vector<int> scase;
	vector<int> scasedir;
	int cnum=LCSrandom(5)+2;
	char lawtaken[LAWNUM];
	memset(lawtaken,0,LAWNUM*sizeof(char));

	scase.resize(cnum);
	scasedir.resize(cnum);
	for(c=0;c<cnum;c++)
		{
		do
			{
			scase[c]=LCSrandom(LAWNUM);
			}while(lawtaken[scase[c]]);

		lawtaken[scase[c]]=1;

		if(law[scase[c]]==2)scasedir[c]=-1;
		else if(law[scase[c]]==-2)scasedir[c]=1;
		else
			{
			if(!LCSrandom(2))scasedir[c]=1;
			else scasedir[c]=-1;
			}

		if(canseethings)
			{
			move(c*3+2,0);
			char name1[80];
			char name2[80];
			if(!LCSrandom(3))strcpy(name1,"The People");
			else lastname(name1);
			addstr(name1);
			addstr(" vs. ");
			lastname(name2);
			addstr(name2);

			move(c*3+3,0);
			addstr("A Decision could ");
			switch(scase[c])
				{
				case LAW_ABORTION:
					if(scasedir[c]==1)addstr("Strengthen Abortion Rights");
					else addstr("Protect the Unborn Child");
					break;
				case LAW_ANIMALRESEARCH:
					if(scasedir[c]==1)addstr("Limit Animal Cruelty");
					else addstr("Expand Animal Research");
					break;
				case LAW_POLICEBEHAVIOR:
					if(scasedir[c]==1)addstr("Curtail Police Misconduct");
					else addstr("Stop Harassment of Police Officers");
					break;
				case LAW_PRIVACY:
					if(scasedir[c]==1)addstr("Enhance Privacy Protection");
					else addstr("Allow Corporations Access to Information");
					break;
				case LAW_DEATHPENALTY:
					if(scasedir[c]==1)addstr("Limit the Death Penalty");
					else addstr("Expand Capital Punishment");
					break;
				case LAW_NUCLEARPOWER:
					if(scasedir[c]==1)addstr("Limit Nuclear Power");
					else addstr("Expand Nuclear Power");
					break;
				case LAW_POLLUTION:
					if(scasedir[c]==1)addstr("Punish Polluters");
					else addstr("Reward Industry");
					break;
				case LAW_LABOR:
					if(scasedir[c]==1)addstr("Enhance Labor Standards");
					else addstr("End Undue Union Influence");
					break;
				case LAW_GAY:
					if(scasedir[c]==1)addstr("Expand Homosexual Rights");
					else addstr("Support the Sanctity of Marriage");
					break;
				case LAW_CORPORATE:
					if(scasedir[c]==1)addstr("Stop Corporate Criminals");
					else addstr("Reward Small Businesses");
					break;
				case LAW_FREESPEECH:
					if(scasedir[c]==1)addstr("Protect Free Speech");
					else addstr("Limit Hurtful Speech");
					break;
				case LAW_TAX:
					if(scasedir[c]==1)addstr("Punish the Wealthy");
					else addstr("Stimulate Economic Growth");
					break;
				case LAW_FLAGBURNING:
					if(scasedir[c]==1)addstr("Limit Prohibitions on Flag Burning");
					else addstr("Protect the Symbol of Our Nation");
					break;
				}

			refresh();
			}
		}

	if(canseethings)
		{
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(20,0);
		addstr("Press any key to watch the decisions unfold.");

		refresh();
		getch();

		nodelay(stdscr,TRUE);
		}

	for(c=0;c<cnum;c++)
		{
		char yeswin=0;
		int yesvotes=0;

		int vote;

		for(int l=0;l<9;l++)
			{
			vote=court[l];
			if(vote>=-1&&vote<=1)vote+=LCSrandom(3)-1;

			if(law[scase[c]]>vote && scasedir[c]==-1)yesvotes++;
			if(law[scase[c]]<vote && scasedir[c]==1)yesvotes++;

			if(l==8)
				{
				if(yesvotes>=5)yeswin=1;
				}

			if(canseethings)
				{
				if(l==8&&yeswin)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(l==8)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(c*3+2,63);
				itoa(yesvotes,num,10);
				addstr(num);
				addstr(" for Change");

				if(l==8&&!yeswin)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(l==8)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(c*3+3,63);
				itoa(l+1-yesvotes,num,10);
				addstr(num);
				addstr(" for Status Quo");

				refresh();

				pause_ms(120);

				getch();
				}
			}

		if(yeswin)law[scase[c]]+=scasedir[c];
		}

	if(canseethings)
		{
		nodelay(stdscr,FALSE);

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(20,0);
		addstr("Press any key to reflect on what has happened.");

		refresh();
		getch();
		}

	//CHANGE A JUSTICE 40% OF THE TIME
	if(LCSrandom(10)>=6)
		{
		if(canseethings)
			{
			erase();

			set_color(COLOR_WHITE,COLOR_BLACK,1);

			move(0,0);
			addstr("Changing the Guard!");
			}

		int j=LCSrandom(9);

		if(canseethings)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(2,0);
			addstr("Justice ");
			addstr(courtname[j]);
			addstr(", ");
			switch(court[j])
				{
				case -2:addstr("Arch-Conservative");break;
				case -1:addstr("Conservative");break;
				case 0:addstr("moderate");break;
				case 1:addstr("Liberal");break;
				case 2:addstr("Elite Liberal");break;
				}
			addstr(", is stepping down.");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(7,0);
			addstr("Press any key to see what happens.");

			refresh();
			getch();
			}

		name(courtname[j]);
		float president=exec[EXEC_PRESIDENT];
		float sen=0;
		for(int s=0;s<100;s++)sen+=senate[s];
		sen/=100.0f;

		float consensus=(president+sen)*.5f;

		if(consensus<-1.5f)court[j]=-2;
		else if(consensus<-.5f)court[j]=-1;
		else if(consensus<.5f)court[j]=0;
		else if(consensus<1.5f)court[j]=1;
		else court[j]=2;

		if(canseethings)
			{
			move(4,0);
			addstr("After much debate and televised testimony, a new justice,");
			move(5,0);
			addstr("the Honorable ");
			addstr(courtname[j]);
			addstr(", ");
			switch(court[j])
				{
				case -2:addstr("Arch-Conservative");break;
				case -1:addstr("Conservative");break;
				case 0:addstr("moderate");break;
				case 1:addstr("Liberal");break;
				case 2:addstr("Elite Liberal");break;
				}
			addstr(", is appointed to the bench.");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(7,0);
			addstr("Press any key to reflect on what has happened.");

			refresh();
			getch();
			}
		}
}

void congress(char clearformess,char canseethings)
{
	int l, c;
	if(canseethings)
		{
		if(clearformess)
			{
			erase();
			}
		else
			{
			makedelimiter(8,0);
			}
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(8,1);
		addstr("Congress is acting on legislation!");

		refresh();
		getch();
		}

	//CHANGE THINGS AND REPORT
	char num[20];
	if(canseethings)
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,1);

		move(0,0);
		addstr("Legislative Agenda ");
		itoa(year,num,10);
		addstr(num);
		}

	vector<int> bill;
	vector<int> billdir;
	vector<int> killbill;
	int cnum=LCSrandom(5)+2;
	char lawtaken[LAWNUM];
	memset(lawtaken,0,LAWNUM*sizeof(char));

	int lawpriority[LAWNUM];
	memset(lawpriority,0,LAWNUM*sizeof(int));
	char lawdir[LAWNUM];
	memset(lawdir,0,LAWNUM*sizeof(char));

	//DETERMINE BILLS
	int pup,pdown,pprior;
	for(l=0;l<LAWNUM;l++)
		{
		pup=0;
		pdown=0;
		pprior=0;

		if(!LCSrandom(3))
			{
			for(int cl=0;cl<435;cl++)
				{
				if(law[l]<house[cl])pup=1;
				else if(law[l]>house[cl])pdown=1;
				pprior+=abs(house[cl]-law[l]);
				}
			}
		else if(LCSrandom(2))
			{
			for(int sl=0;sl<100;sl++)
				{
				if(law[l]<senate[sl])pup++;
				else if(law[l]>senate[sl])pdown++;
				pprior+=abs(senate[sl]-law[l]);
				}
			}
		else
			{
			for(int cl=0;cl<435;cl++)
				{
				if(law[l]<house[cl])pup++;
				else if(law[l]>house[cl])pdown++;
				pprior+=abs(house[cl]-law[l]);
				}
			for(int sl=0;sl<100;sl++)
				{
				if(law[l]<senate[sl])pup+=4;
				else if(law[l]>senate[sl])pdown+=4;
				pprior+=abs(senate[sl]-law[l])*4;
				}
			}

		if(pup>pdown)lawdir[l]=1;
		else if(pup==pdown)lawdir[l]=LCSrandom(2)*2-1;
		else lawdir[l]=-1;
		if(law[l]==-2)lawdir[l]=1;
		if(law[l]==2)lawdir[l]=-1;

		//CALC PRIORITY
		lawpriority[l]=pprior;
		}

	vector<int> canlaw;

	bill.resize(cnum);
	billdir.resize(cnum);
	killbill.resize(cnum);
	for(c=0;c<cnum;c++)
		{
		killbill[c]=0;

		int maxprior=0;
		for(l=0;l<LAWNUM;l++)
			{
			if(lawpriority[l]>maxprior&&!lawtaken[l])maxprior=lawpriority[l];
			}
		canlaw.clear();
		for(l=0;l<LAWNUM;l++)
			{
			if(lawpriority[l]==maxprior&&!lawtaken[l])
				{
				canlaw.push_back(l);
				}
			}

		bill[c]=canlaw[LCSrandom(canlaw.size())];

		lawtaken[bill[c]]=1;

		billdir[c]=lawdir[bill[c]];

		if(canseethings)
			{
			move(c*3+2,0);
			addstr("Joint Resolution ");
			itoa(year,num,10);
			addstr(num);
			addstr("-");
			itoa(c+1,num,10);
			addstr(num);

			move(c*3+3,0);
			addstr("To ");
			switch(bill[c])
				{
				case LAW_ABORTION:
					if(billdir[c]==1)addstr("Strengthen Abortion Rights");
					else addstr("Protect the Unborn Child");
					break;
				case LAW_ANIMALRESEARCH:
					if(billdir[c]==1)addstr("Limit Animal Cruelty");
					else addstr("Expand Animal Research");
					break;
				case LAW_POLICEBEHAVIOR:
					if(billdir[c]==1)addstr("Curtail Police Misconduct");
					else addstr("Stop Harassment of Police Officers");
					break;
				case LAW_PRIVACY:
					if(billdir[c]==1)addstr("Enhance Privacy Protection");
					else addstr("Allow Corporations Information Access");
					break;
				case LAW_DEATHPENALTY:
					if(billdir[c]==1)addstr("Limit the Death Penalty");
					else addstr("Expand Capital Punishment");
					break;
				case LAW_NUCLEARPOWER:
					if(billdir[c]==1)addstr("Limit Nuclear Power");
					else addstr("Expand Nuclear Power");
					break;
				case LAW_POLLUTION:
					if(billdir[c]==1)addstr("Punish Polluters");
					else addstr("Reward Industry");
					break;
				case LAW_LABOR:
					if(billdir[c]==1)addstr("Enhance Labor Standards");
					else addstr("End Undue Union Influence");
					break;
				case LAW_GAY:
					if(billdir[c]==1)addstr("Expand Homosexual Rights");
					else addstr("Support the Sanctity of Marriage");
					break;
				case LAW_CORPORATE:
					if(billdir[c]==1)addstr("Stop Corporate Criminals");
					else addstr("Reward Small Businesses");
					break;
				case LAW_FREESPEECH:
					if(billdir[c]==1)addstr("Protect Free Speech");
					else addstr("Limit Hurtful Speech");
					break;
				case LAW_TAX:
					if(billdir[c]==1)addstr("Punish the Wealthy");
					else addstr("Stimulate Economic Growth");
					break;
				case LAW_FLAGBURNING:
					if(billdir[c]==1)addstr("Limit Prohibitions on Flag Burning");
					else addstr("Protect the Symbol of Our Nation");
					break;
				}

			refresh();
			}
		}

	if(canseethings)
		{
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(20,0);
		addstr("Press any key to watch the votes unfold.");

		refresh();
		getch();

		move(0,62);
		addstr("House");

		move(0,70);
		addstr("Senate");

		nodelay(stdscr,TRUE);
		}

	for(c=0;c<cnum;c++)
		{
		char yeswin_h=0;
		char yeswin_s=0;
		int yesvotes_h=0;
		int yesvotes_s=0;

		int vote;
		int s=-1;

		for(int l=0;l<435;l++)
			{
			vote=house[l];
			if(vote>=-1&&vote<=1)vote+=LCSrandom(3)-1;

			if(law[bill[c]]>vote && billdir[c]==-1)yesvotes_h++;
			if(law[bill[c]]<vote && billdir[c]==1)yesvotes_h++;

			if(l==434)
				{
				if(yesvotes_h>=218)yeswin_h=1;
				}

			if(canseethings)
				{
				if(l==434&&yeswin_h)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(l==434)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(c*3+2,62);
				itoa(yesvotes_h,num,10);
				addstr(num);
				addstr(" Yea");

				if(l==434&&!yeswin_h)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(l==434)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(c*3+3,62);
				itoa(l+1-yesvotes_h,num,10);
				addstr(num);
				addstr(" Nay");
				}

			if(l%4==0&&s<99)
				{
				s++;

				vote=senate[s];
				if(vote>=-1&&vote<=1)vote+=LCSrandom(3)-1;

				if(law[bill[c]]>vote && billdir[c]==-1)yesvotes_s++;
				if(law[bill[c]]<vote && billdir[c]==1)yesvotes_s++;
				}

			if(l==434)
				{
				if(yesvotes_s>=51)yeswin_s=1;
				if(yesvotes_s==50)
					{
					//TIE BREAKER
					int vote=(exec[EXEC_PRESIDENT]+
						exec[EXEC_VP]+
						exec[EXEC_STATE]+
						exec[EXEC_ATTORNEY]+LCSrandom(9)-4)/4;

					if(law[bill[c]]>vote && billdir[c]==-1)yeswin_s=1;
					if(law[bill[c]]<vote && billdir[c]==1)yeswin_s=1;

					//ASSURED SIGNING BY PRESIDENT IF VP VOTED YES
					if(yeswin_s)killbill[c]=-1;
					}
				}

			if(canseethings)
				{
				if(l==434&&yeswin_s)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(l==434)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(c*3+2,70);
				itoa(yesvotes_s,num,10);
				addstr(num);
				addstr(" Yea");

				if(l==434&&yesvotes_s==50&&yeswin_s)
					{
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(c*3+2,78);
					addstr("VP");
					}

				if(l==434&&!yeswin_s)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(l==434)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(c*3+3,70);
				itoa(s+1-yesvotes_s,num,10);
				addstr(num);
				addstr(" Nay");

				if(l==434&&yesvotes_s==50&&!yeswin_s)
					{
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(c*3+3,78);
					addstr("VP");
					}

				if(l%5==0)
					{
					refresh();

					pause_ms(10);
					}

				getch();
				}
			}

		if(!yeswin_h)killbill[c]=1;
		if(!yeswin_s)killbill[c]=1;
		}

	if(canseethings)nodelay(stdscr,FALSE);

	int havebill=0;
	for(c=0;c<cnum;c++)
		{
		if(killbill[c]<=0)havebill++;
		}

	if(havebill>0)
		{
		if(canseethings)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(20,0);
			addstr("Press any key to watch the President.                 ");

			refresh();
			getch();

			move(0,35);
			addstr("President");
			refresh();

			nodelay(stdscr,TRUE);

			pause_ms(500);
			}

		for(int c=0;c<bill.size();c++)
			{
			char sign=0;
			if(killbill[c]==1)sign=-1;
			else
				{
				int vote=(exec[EXEC_PRESIDENT]+
					exec[EXEC_VP]+
					exec[EXEC_STATE]+
					exec[EXEC_ATTORNEY]+(short)LCSrandom(9)-4)/4;
				if(exec[EXEC_PRESIDENT]==2)vote=2;
				if(exec[EXEC_PRESIDENT]==-2)vote=-2;

				if(law[bill[c]]>vote && billdir[c]==-1)sign=1;
				if(law[bill[c]]<vote && billdir[c]==1)sign=1;
				if(killbill[c]==-1)sign=1;
				}

			if(canseethings)
				{
				move(c*3+2,35);
				if(sign==1)
					{
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					addstr(execname[EXEC_PRESIDENT]);
					}
				else if(sign==-1)
					{
					set_color(COLOR_BLACK,COLOR_BLACK,1);
					addstr("Dead in Congress");
					}
				else if(!sign)
					{
					set_color(COLOR_RED,COLOR_BLACK,1);
					addstr("*** VETO ***");
					}

				refresh();

				pause_ms(500);
				}

			if(sign==1)law[bill[c]]+=billdir[c];
			}

		if(canseethings)
			{
			nodelay(stdscr,FALSE);

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(20,0);
			addstr("Press any key to reflect on what has happened.    ");

			refresh();
			getch();
			}
		}
	else
		{
		if(canseethings)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(20,0);
			addstr("None of the items made it to the President's desk.");
			move(21,0);
			addstr("Press any key to reflect on what has happened.    ");

			refresh();
			getch();
			}
		}

	//CONGRESS CONSTITUTION CHANGES
	int housemake[5]={0,0,0,0,0};
	for(int h=0;h<435;h++)
		{
		housemake[house[h]+2]++;
		}

	int senatemake[5]={0,0,0,0,0};
	for(int s=0;s<100;s++)
		{
		senatemake[senate[s]+2]++;
		}

	//TOSS JUSTICES?
	char tossj=0;
	for(int j=0;j<9;j++)
		{
		if(court[j]<=1)
			{
			tossj=1;
			break;
			}
		}

	if(housemake[4]>=290&&senatemake[4]>=67&&tossj)
		{
		tossjustices(canseethings);
		}
}

void tossjustices(char canseethings)
{
	int j;
	if(canseethings)
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,1);

		move(12,6);
		addstr("The Elite Liberal Congress is proposing an ELITE LIBERAL AMENDMENT!");

		refresh();
		getch();
		}

	//STATE THE AMENDMENT
	if(canseethings)
		{
		int tossnum=0;
		for(j=0;j<9;j++)
			{
			if(court[j]<=1)tossnum++;
			}

		amendmentheading();

		move(2,5);
		addstr("The following former citizen");
		if(tossnum!=1)addstr("s are");
		else addstr(" is");
		addstr(" branded Arch-Conservative:");

		int y=4;

		for(j=0;j<9;j++)
			{
			if(court[j]!=2)
				{
				move(y,0);
				addstr(courtname[j]);
				y++;
				}
			}

		move(y+1,5);
		addstr("In particular, the aforementioned former citizens may");
		move(y+2,0);
		addstr("not serve on the Supreme Court.  Said former citizens will");
		move(y+3,0);
		addstr("be deported to Conservative countries of the President's");
		move(y+4,0);
		addstr("choosing to be replaced by Proper Justices, also of");
		move(y+5,0);
		addstr("the President's choosing.");

		move(24,0);
		addstr("Press 'C' to watch the ratification process unfold.");
		do
			{
			refresh();
			int c=getch();
			translategetch(c);
			if(c=='c')break;
			}while(1);
		}

	if(ratify(2,-1,-1,1,canseethings))
		{
		//BLAST JUSTICES
		for(int j=0;j<9;j++)
			{
			if(court[j]!=2)
				{
				name(courtname[j]);
				court[j]=2;
				}
			}

		amendnum++;
		}

	if(canseethings)
		{
		move(24,0);
		addstr("Press any key to reflect on what has happened.");
		refresh();
		getch();
		}
}

void reaganify(char canseethings)
{
	if(canseethings)
		{
		set_color(COLOR_WHITE,COLOR_BLACK,1);

		erase();
		move(12,6);
		addstr("Two Thirds of the States have convened a Constitutional Convention!");
		refresh();
		getch();

		erase();
		move(12,15);
		addstr("They have proposed an ARCH-CONSERVATIVE AMENDMENT!");
		refresh();
		getch();

		//STATE THE AMENDMENT
		amendmentheading();

		move(2,5);
		addstr("In recognition of the fact that society is degenerating under");
		move(3,0);
		addstr("the pressure of the elite liberal threat, we the People hereby");
		move(4,0);
		addstr("repeal the Constitition.  The former United States are to be");
		move(5,0);
		addstr("reorganized into the Confederated States of America, with new");
		move(6,0);
		addstr("boundaries to be determined by leading theologians.");
		move(8,5);
		addstr("Ronald Reagan is to be King, forever, even after death.");
		move(10,5);
		addstr("The following Executive Officers are also chosen in perpetuity:");
		move(11,0);
		addstr("Vice President Strom Thurmond, Secretary of State Jesse Helms, and");
		move(12,0);
		addstr("Attorney General Jerry Falwell.");
		move(14,5);
		addstr("In the event of the deaths of any of the aforementioned");
		move(15,0);
		addstr("persons, though they shall still nominally hold these posts,");
		move(16,0);
		addstr("actual decisions shall be made by business representatives,");
		move(17,0);
		addstr("chosen by respected business leaders.");
		move(19,5);
		addstr("People may petition Jesus for a redress of grievances, as");
		move(20,0);
		addstr("He will be the only one listening.");
		move(22,5);
		addstr("Have a nice day.");

		move(24,0);
		addstr("Press 'C' to watch the ratification process unfold.");
		do
			{
			refresh();
			int c=getch();
			translategetch(c);
			if(c=='c')break;
			}while(1);
		}

	if(ratify(-2,-1,-1,0,canseethings))
		{
		if(canseethings)
			{
			move(24,0);
			addstr("Press any key to reflect on what has happened ONE LAST TIME.");
			refresh();
			getch();
			}

		//REAGANIFY
		if(canseethings)
			{
			strcpy(execname[EXEC_PRESIDENT],"Ronald Reagan");
			strcpy(execname[EXEC_VP],"Strom Thurmond");
			strcpy(execname[EXEC_STATE],"Jesse Helms");
			strcpy(execname[EXEC_ATTORNEY],"Jerry Falwell");
			for(int e=0;e<EXECNUM;e++)exec[e]=-2;
			liberalagenda(-1);
			savehighscore(END_REAGAN);
			}
		else
			{
			switch(cantseereason)
				{
				case 1:
					//DATING AND REAGANIFIED
					set_color(COLOR_WHITE,COLOR_BLACK,1);

					erase();
					move(12,10);
					addstr("You went on vacation when the country was on the verge of collapse.");
					refresh();
					getch();

					set_color(COLOR_WHITE,COLOR_BLACK,0);
					erase();
					move(12,12);
					addstr("The Conservatives have made the world in their image.");
					refresh();
					getch();

					set_color(COLOR_BLACK,COLOR_BLACK,1);
					erase();
					move(12,14);
					addstr("They'll round the last of you up eventually.  All is lost.");
					refresh();
					getch();

					savehighscore(END_DATING);
					break;
				case 2:
					//HIDING AND REAGANIFIED
					set_color(COLOR_WHITE,COLOR_BLACK,1);

					erase();
					move(12,10);
					addstr("You went into hiding when the country was on the verge of collapse.");
					refresh();
					getch();

					set_color(COLOR_WHITE,COLOR_BLACK,0);
					erase();
					move(12,12);
					addstr("The Conservatives have made the world in their image.");
					refresh();
					getch();

					set_color(COLOR_BLACK,COLOR_BLACK,1);
					erase();
					move(12,14);
					addstr("They'll round the last of you up eventually.  All is lost.");
					refresh();
					getch();

					savehighscore(END_HIDING);
					break;
				case 3:
					//IF YOU ARE ALL IN PRISON, JUST PASS AWAY QUIETLY
					set_color(COLOR_WHITE,COLOR_BLACK,1);

					erase();
					move(12,10);
					addstr("While you were on the inside, the country degenerated...");
					refresh();
					getch();

					set_color(COLOR_WHITE,COLOR_BLACK,0);
					erase();
					move(12,12);
					addstr("Your kind are never released these days.");
					refresh();
					getch();

					set_color(COLOR_BLACK,COLOR_BLACK,1);
					erase();
					move(12,14);
					addstr("Ain't no sunshine...");
					refresh();
					getch();
					savehighscore(END_PRISON);
					break;
				case 4:
					//DISBANDED AND REAGANIFIED
					set_color(COLOR_WHITE,COLOR_BLACK,1);

					erase();
					move(12,10);
					addstr("You disappeared safely, but you hadn't done enough.");
					refresh();
					getch();

					set_color(COLOR_WHITE,COLOR_BLACK,0);
					erase();
					move(12,12);
					addstr("The Conservatives have made the world in their image.");
					refresh();
					getch();

					set_color(COLOR_BLACK,COLOR_BLACK,1);
					erase();
					move(12,14);
					addstr("They'll round the last of you up eventually.  All is lost.");
					refresh();
					getch();

					savehighscore(END_DISBANDLOSS);
					break;
				}
			}
		reset();
		viewhighscores();
		endwin();
		exit(0);
		}
	else
		{
		if(canseethings)
			{
			move(24,0);
			addstr("Press any key to breathe a sigh of relief.                 ");
			refresh();
			getch();
			}
		}
}

void amendmentheading(void)
{
	erase();

	set_color(COLOR_WHITE,COLOR_BLACK,1);

	move(0,0);
	addstr("Proposed Amendment ");
	romannumeral(amendnum);
	addstr(" to the United States Constitution:");
}

void romannumeral(int amendnum)
{
	while(amendnum>=1000)
		{
		amendnum-=1000;
		addch('M');
		}
	if(amendnum>=900)
		{
		amendnum-=900;
		addch('C');
		addch('M');
		}
	if(amendnum>=500)
		{
		amendnum-=500;
		addch('D');
		}
	if(amendnum>=400)
		{
		amendnum-=400;
		addch('C');
		addch('D');
		}
	while(amendnum>=100)
		{
		amendnum-=100;
		addch('C');
		}
	if(amendnum>=90)
		{
		amendnum-=90;
		addch('X');
		addch('C');
		}
	if(amendnum>=50)
		{
		amendnum-=50;
		addch('L');
		}
	if(amendnum>=40)
		{
		amendnum-=40;
		addch('X');
		addch('L');
		}
	while(amendnum>=10)
		{
		amendnum-=10;
		addch('X');
		}
	if(amendnum>=9)
		{
		amendnum-=9;
		addch('I');
		addch('X');
		}
	if(amendnum>=5)
		{
		amendnum-=5;
		addch('V');
		}
	if(amendnum>=4)
		{
		amendnum-=4;
		addch('I');
		addch('V');
		}
	while(amendnum>=1)
		{
		amendnum-=1;
		addch('I');
		}
}

char ratify(int level,int view,int lawview,char congress,char canseethings)
{
	if(canseethings)
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,1);

		move(0,0);
		addstr("The Ratification Process:");
		}

	//THE STATE VOTE WILL BE BASED ON VIEW OF LAW
	int mood=publicmood(lawview);
	//OR OF A PARTICULAR ISSUE
	if(view!=-1)mood=attitude[view];

	//CONGRESS
	char num[20];
	char ratified=0;

	int y=0;

	if(congress)
		{
		ratified=1;

		if(canseethings)
			{
			move(0,62);
			addstr("House");

			move(0,70);
			addstr("Senate");

			move(24,0);
			addstr("Press any key to watch the Congressional votes unfold.     ");
			refresh();
			getch();

			nodelay(stdscr,TRUE);
			}

		char yeswin_h=0;
		char yeswin_s=0;
		int yesvotes_h=0;
		int yesvotes_s=0;

		int vote;
		int s=-1;

		for(int l=0;l<435;l++)
			{
			vote=house[l];
			if(vote>=-1&&vote<=1)vote+=LCSrandom(3)-1;

			if(level==vote)yesvotes_h++;

			if(l==434)
				{
				if(yesvotes_h>=290)yeswin_h=1;
				}

			if(canseethings)
				{
				if(l==434&&yeswin_h)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(l==434)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(2,62);
				itoa(yesvotes_h,num,10);
				addstr(num);
				addstr(" Yea");

				if(l==434&&!yeswin_h)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(l==434)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(3,62);
				itoa(l+1-yesvotes_h,num,10);
				addstr(num);
				addstr(" Nay");
				}

			if(l%4==0&&s<99)
				{
				s++;

				vote=senate[s];
				if(vote>=-1&&vote<=1)vote+=LCSrandom(3)-1;

				if(level==vote)yesvotes_s++;
				}

			if(l==434)
				{
				if(yesvotes_s>=67)yeswin_s=1;
				}

			if(canseethings)
				{
				if(l==434&&yeswin_s)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(l==434)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(2,70);
				itoa(yesvotes_s,num,10);
				addstr(num);
				addstr(" Yea");

				if(l==434&&!yeswin_s)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(l==434)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(3,70);
				itoa(s+1-yesvotes_s,num,10);
				addstr(num);
				addstr(" Nay");

				if(l%5==0)
					{
					refresh();

					pause_ms(10);
					}

				getch();
				}
			}

		if(!yeswin_h)ratified=0;
		if(!yeswin_s)ratified=0;

		if(canseethings)nodelay(stdscr,FALSE);

		y+=4;
		}
	else ratified=1;

	//STATES
	if(ratified)
		{
		ratified=0;

		int yesstate=0;

		if(canseethings)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,1);

			for(int s=0;s<50;s++)
				{
				if(s<17)move(5+s,0);
				else if(s<34)move(5+s-17,27);
				else move(5+s-34,54);

				switch(s)
					{
					case 0:addstr("Alabama");break;
					case 1:addstr("Alaska");break;
					case 2:addstr("Arkansas");break;
					case 3:addstr("Arizona");break;
					case 4:addstr("California");break;
					case 5:addstr("Colorado");break;
					case 6:addstr("Connecticut");break;
					case 7:addstr("Delaware");break;
					case 8:addstr("Florida");break;
					case 9:addstr("Georgia");break;
					case 10:addstr("Hawaii");break;
					case 11:addstr("Idaho");break;
					case 12:addstr("Illinois");break;
					case 13:addstr("Indiana");break;
					case 14:addstr("Iowa");break;
					case 15:addstr("Kansas");break;
					case 16:addstr("Kentucky");break;
					case 17:addstr("Louisiana");break;
					case 18:addstr("Maine");break;
					case 19:addstr("Maryland");break;
					case 20:addstr("Massachusetts");break;
					case 21:addstr("Michigan");break;
					case 22:addstr("Minnesota");break;
					case 23:addstr("Mississippi");break;
					case 24:addstr("Missouri");break;
					case 25:addstr("Montana");break;
					case 26:addstr("Nebraska");break;
					case 27:addstr("Nevada");break;
					case 28:addstr("New Hampshire");break;
					case 29:addstr("New Jersey");break;
					case 30:addstr("New Mexico");break;
					case 31:addstr("New York");break;
					case 32:addstr("North Carolina");break;
					case 33:addstr("North Dakota");break;
					case 34:addstr("Ohio");break;
					case 35:addstr("Oklahoma");break;
					case 36:addstr("Oregon");break;
					case 37:addstr("Pennsylvania");break;
					case 38:addstr("Rhode Island");break;
					case 39:addstr("South Carolina");break;
					case 40:addstr("South Dakota");break;
					case 41:addstr("Tennessee");break;
					case 42:addstr("Texas");break;
					case 43:addstr("Utah");break;
					case 44:addstr("Vermont");break;
					case 45:addstr("Virginia");break;
					case 46:addstr("Washington");break;
					case 47:addstr("West Virginia");break;
					case 48:addstr("Wisconsin");break;
					case 49:addstr("Wyoming");break;
					}
				}

			move(24,0);
			addstr("Press any key to watch the State votes unfold.             ");
			refresh();
			getch();

			nodelay(stdscr,TRUE);
			}

		int vote;
		int smood;
		for(int s=0;s<50;s++)
			{
			smood=mood;

			switch(s)
				{
				case 0:smood-=10;break;
				case 4:smood=100;break;
				case 9:smood-=10;break;
				case 11:smood-=10;break;
				case 20:smood+=25;break;
				case 23:smood-=10;break;
				case 39:smood-=10;break;
				case 42:smood-=10;break;
				case 43:smood=0;break;
				}

			vote=0;
			if((short)LCSrandom(100)<smood)vote++;
			if((short)LCSrandom(100)<smood)vote++;
			if((short)LCSrandom(100)<smood)vote++;
			if((short)LCSrandom(100)<smood)vote++;
			vote-=2;

			if(canseethings)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				if(s<17)move(5+s,22);
				else if(s<34)move(5+s-17,49);
				else move(5+s-34,76);
				}
			if(vote==level&&s!=44)
				{
				yesstate++;
				if(canseethings)addstr("Yea");
				}
			else if(canseethings)addstr("Nay");

			if(canseethings)
				{
				if(s==49&&yesstate>=37)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(s==49)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(23,50);
				itoa(yesstate,num,10);
				addstr(num);
				addstr(" Yea");

				if(s==49&&yesstate<37)set_color(COLOR_WHITE,COLOR_BLACK,1);
				else if(s==49)set_color(COLOR_BLACK,COLOR_BLACK,1);
				else set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(23,60);
				itoa(s+1-yesstate,num,10);
				addstr(num);
				addstr(" Nay");

				refresh();
				pause_ms(50);
				}
			}

		if(canseethings)set_color(COLOR_WHITE,COLOR_BLACK,1);

		if(yesstate>=37)
			{
			ratified=1;
			}

		if(canseethings)nodelay(stdscr,FALSE);

		if(canseethings)
			{
			if(!ratified)
				{
				move(23,0);
				addstr("AMENDMENT REJECTED.");
				}
			else
				{
				move(23,0);
				addstr("AMENDMENT ADOPTED.");
				}
			}
		}
	else
		{
		if(canseethings)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(23,0);
			addstr("AMENDMENT REJECTED.");
			}
		}

	return ratified;
}

char wincheck(void)
{
	for(int e=0;e<EXECNUM;e++)
		{
		if(exec[e]!=2)return 0;
		}

	for(int l=0;l<LAWNUM;l++)
		{
		if(law[l]!=2)return 0;
		}

	int housemake[5]={0,0,0,0,0};
	for(int h=0;h<435;h++)
		{
		housemake[house[h]+2]++;
		}
	if(housemake[4]<290)return 0;

	int senatemake[5]={0,0,0,0,0};
	for(int s=0;s<100;s++)
		{
		senatemake[senate[s]+2]++;
		}
	if(senatemake[4]<67)return 0;

	int elibjudge=0;
	for(int c=0;c<9;c++)
		{
		if(court[c]==2)elibjudge++;
		}

	if(elibjudge<5)return 0;

	return 1;
}

void loadhighscores(void)
{
	for(int s=0;s<SCORENUM;s++)score[s].valid=0;

	//LOAD FILE
	unsigned long loadversion;

	DWORD numbytes;
	HANDLE h;

	h =LCSCreateFile("score.dat", LCSIO_READ);
	
	if(h!=NULL)
		{	
		ReadFile(h,&loadversion,sizeof(unsigned long),&numbytes,NULL);

		if(loadversion<lowestloadscoreversion)
			{
			CloseHandle(h);
			return;
			}

		ReadFile(h,&ustat_recruits,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&ustat_dead,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&ustat_kills,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&ustat_kidnappings,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&ustat_funds,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&ustat_spent,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&ustat_buys,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&ustat_burns,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,score,sizeof(highscorest)*SCORENUM,&numbytes,NULL);

		CloseHandle(h);
		}
}

void savehighscore(char endtype)
{
	loadhighscores();

	//MERGE THE STATS
	ustat_recruits+=stat_recruits;
	ustat_dead+=stat_dead;
	ustat_kills+=stat_kills;
	ustat_kidnappings+=stat_kidnappings;
	ustat_funds+=stat_funds;
	ustat_spent+=stat_spent;
	ustat_buys+=stat_buys;
	ustat_burns+=stat_burns;

	//PLACE THIS HIGH SCORE BY DATE IF NECESSARY
	yourscore=-1;

	for(int s=0;s<SCORENUM;s++)
		{
		if((endtype==END_WON&&score[s].endtype==END_WON&&
			year==score[s].year&&month==score[s].month&&
			stat_spent+stat_funds>score[s].stat_spent+score[s].stat_funds)||

			(endtype==END_WON&&score[s].endtype==END_WON
			&&(year<score[s].year ||
			(year==score[s].year && month<score[s].month)))||

			(endtype==END_WON&&score[s].endtype!=END_WON)||

			(endtype!=END_WON&&score[s].endtype!=END_WON&&
			stat_spent+stat_funds>score[s].stat_spent+score[s].stat_funds)||

			score[s].valid==0)
			{
			for(int s2=SCORENUM-1;s2>=s+1;s2--)
				{
				score[s2]=score[s2-1];
				}

			strcpy(score[s].slogan,slogan);
			score[s].month=month;
			score[s].year=year;
			score[s].stat_recruits=stat_recruits;
			score[s].stat_dead=stat_dead;
			score[s].stat_kills=stat_kills;
			score[s].stat_kidnappings=stat_kidnappings;
			score[s].stat_funds=stat_funds;
			score[s].stat_spent=stat_spent;
			score[s].stat_buys=stat_buys;
			score[s].stat_burns=stat_burns;
			score[s].valid=1;
			score[s].endtype=endtype;

			yourscore=s;
			break;
			}
		}


	DWORD numbytes;
	HANDLE h;
	h=LCSCreateFile("score.dat", LCSIO_WRITE);
	if(h!=NULL)
		{
		WriteFile(h,&version,sizeof(unsigned long),&numbytes,NULL);

		WriteFile(h,&ustat_recruits,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&ustat_dead,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&ustat_kills,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&ustat_kidnappings,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&ustat_funds,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&ustat_spent,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&ustat_buys,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,&ustat_burns,sizeof(unsigned long),&numbytes,NULL);
		WriteFile(h,score,sizeof(highscorest)*SCORENUM,&numbytes,NULL);

		CloseHandle(h);
		}
}

void viewhighscores(void)
{
	int s;
	loadhighscores();

	short validsum=0;
	for(s=0;s<SCORENUM;s++)
		{
		if(score[s].valid)validsum++;
		}

	if(!validsum)return;

	erase();

	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(0,0);
	addstr("The Liberal ELITE");

	char num[20];

	int y=2;
	for(s=0;s<SCORENUM;s++)
		{
		if(score[s].valid)
			{
			if(yourscore==s&&score[s].endtype==END_WON)set_color(COLOR_GREEN,COLOR_BLACK,1);
			else if(yourscore==s)set_color(COLOR_RED,COLOR_BLACK,1);
			else set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(y,0);
			addstr(score[s].slogan);
			if(yourscore==s&&score[s].endtype==END_WON)set_color(COLOR_GREEN,COLOR_BLACK,0);
			else if(yourscore==s)set_color(COLOR_RED,COLOR_BLACK,0);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y+1,0);
			switch(score[s].endtype)
				{
				case END_WON:
					addstr("The Liberal Crime Squad liberalized the country in ");
					break;
				case END_POLICE:
					addstr("The Liberal Crime Squad was brought to justice in ");
					break;
				case END_CIA:
					addstr("The Liberal Crime Squad was blotted out in ");
					break;
				case END_HICKS:
					addstr("The Liberal Crime Squad was mobbed in ");
					break;
				case END_CORP:
					addstr("The Liberal Crime Squad was downsized in ");
					break;
				case END_DEAD:
					addstr("The Liberal Crime Squad was KIA in ");
					break;
				case END_REAGAN:
					addstr("The country was Reaganified in ");
					break;
				case END_PRISON:
					addstr("The Liberal Crime Squad died in prison in ");
					break;
				case END_EXECUTED:
					addstr("The Liberal Crime Squad was executed in ");
					break;
				case END_DATING:
					addstr("The Liberal Crime Squad was on vacation in ");
					break;
				case END_HIDING:
					addstr("The Liberal Crime Squad was in permanent hiding in ");
					break;
				case END_DISBANDLOSS:
					addstr("The Liberal Crime Squad was hunted down in ");
					break;
				case END_DISPERSED:
					addstr("The Liberal Crime Squad was scattered in ");
					break;
				}
			switch(score[s].month)
				{
				case 1:addstr("January");break;
				case 2:addstr("February");break;
				case 3:addstr("March");break;
				case 4:addstr("April");break;
				case 5:addstr("May");break;
				case 6:addstr("June");break;
				case 7:addstr("July");break;
				case 8:addstr("August");break;
				case 9:addstr("September");break;
				case 10:addstr("October");break;
				case 11:addstr("November");break;
				case 12:addstr("December");break;
				}
			addstr(" ");
			itoa(score[s].year,num,10);
			addstr(num);
			addstr(". ");

			move(y+2,0);
			addstr("Recruits: ");
			itoa(score[s].stat_recruits,num,10);
			addstr(num);

			move(y+3,0);
			addstr("Martyrs: ");
			itoa(score[s].stat_dead,num,10);
			addstr(num);

			move(y+2,20);
			addstr("Kills: ");
			itoa(score[s].stat_kills,num,10);
			addstr(num);

			move(y+3,20);
			addstr("Kidnappings: ");
			itoa(score[s].stat_kidnappings,num,10);
			addstr(num);

			move(y+2,40);
			addstr("$ Taxed: ");
			itoa(score[s].stat_funds,num,10);
			addstr(num);

			move(y+3,40);
			addstr("$ Spent: ");
			itoa(score[s].stat_spent,num,10);
			addstr(num);

			move(y+2,60);
			addstr("Flags Bought: ");
			itoa(score[s].stat_buys,num,10);
			addstr(num);

			move(y+3,60);
			addstr("Flags Burned: ");
			itoa(score[s].stat_burns,num,10);
			addstr(num);

			y+=4;
			}
		}

	set_color(COLOR_GREEN,COLOR_BLACK,1);

	//UNIVERSAL STATS
	move(22,0);
	addstr("Universal Liberal Statistics:");

	move(23,0);
	addstr("Recruits: ");
	itoa(ustat_recruits,num,10);
	addstr(num);

	move(24,0);
	addstr("Martyrs: ");
	itoa(ustat_dead,num,10);
	addstr(num);

	move(23,20);
	addstr("Kills: ");
	itoa(ustat_kills,num,10);
	addstr(num);

	move(24,20);
	addstr("Kidnappings: ");
	itoa(ustat_kidnappings,num,10);
	addstr(num);

	move(23,40);
	addstr("$ Taxed: ");
	itoa(ustat_funds,num,10);
	addstr(num);

	move(24,40);
	addstr("$ Spent: ");
	itoa(ustat_spent,num,10);
	addstr(num);

	move(23,60);
	addstr("Flags Bought: ");
	itoa(ustat_buys,num,10);
	addstr(num);

	move(24,60);
	addstr("Flags Burned: ");
	itoa(ustat_burns,num,10);
	addstr(num);

	refresh();
	getch();
}

void reset(void)
{
	unlink("save.dat");
}

int publicmood(int l)
{
	if(l==-1)
		{
		int sum=0;

		for(int v=0;v<VIEWNUM;v++)
			{
			if(v==VIEW_LIBERALCRIMESQUAD)continue;
			if(v==VIEW_LIBERALCRIMESQUADPOS)continue;

			sum+=attitude[v];
			}

		sum/=(VIEWNUM-1);

		return sum;
		}
	else
		{
		switch(l)
			{
			case LAW_ABORTION:return attitude[VIEW_ABORTION];
			case LAW_ANIMALRESEARCH:return attitude[VIEW_ANIMALRESEARCH];
			case LAW_POLICEBEHAVIOR:return attitude[VIEW_POLICEBEHAVIOR];
			case LAW_PRIVACY:return attitude[VIEW_INTELLIGENCE];
			case LAW_DEATHPENALTY:return attitude[VIEW_DEATHPENALTY];
			case LAW_NUCLEARPOWER:return attitude[VIEW_NUCLEARPOWER];
			case LAW_POLLUTION:return attitude[VIEW_POLLUTION];
			case LAW_LABOR:return attitude[VIEW_SWEATSHOPS];
			case LAW_GAY:return attitude[VIEW_GAY];
			case LAW_CORPORATE:return (attitude[VIEW_CORPORATECULTURE]+
								   attitude[VIEW_CEOSALARY])/2;
			case LAW_FREESPEECH:return attitude[VIEW_FREESPEECH];
			case LAW_TAX:return attitude[VIEW_TAXES];
			case LAW_FLAGBURNING:return attitude[VIEW_FREESPEECH];
			}
		}

	return 50;
}

void getview(char *str,short view)
{
	strcpy(str,"");

	switch(view)
		{
		case VIEW_ABORTION:strcat(str,"Abortion");break;
		case VIEW_GAY:strcat(str,"Homosexual Rights");break;
		case VIEW_DEATHPENALTY:strcat(str,"Death Penalty");break;
		case VIEW_TAXES:strcat(str,"Taxes");break;
		case VIEW_NUCLEARPOWER:strcat(str,"Nuclear Power");break;
		case VIEW_ANIMALRESEARCH:strcat(str,"Animal Cruelty");break;
		case VIEW_POLICEBEHAVIOR:strcat(str,"The Police");break;
		case VIEW_PRISONS:strcat(str,"Prisons");break;
		case VIEW_INTELLIGENCE:strcat(str,"Privacy");break;
		case VIEW_FREESPEECH:strcat(str,"Free Speech");break;
		case VIEW_GENETICS:strcat(str,"Genetics");break;
		case VIEW_JUSTICES:strcat(str,"The Judiciary");break;
		case VIEW_SWEATSHOPS:strcat(str,"Labor");break;
		case VIEW_POLLUTION:strcat(str,"Pollution");break;
		case VIEW_CORPORATECULTURE:strcat(str,"Corporate Culture");break;
		case VIEW_CEOSALARY:strcat(str,"CEO Compensation");break;
		case VIEW_AMRADIO:strcat(str,"AM Radio");break;
		case VIEW_CABLENEWS:strcat(str,"Cable News");break;
		case VIEW_LIBERALCRIMESQUAD:strcat(str,"Who We Are");break;
		case VIEW_LIBERALCRIMESQUADPOS:strcat(str,"Why We Rock");break;
		}
}

int choosespecialedition(char &clearformess)
{
	int page=0;

	char havetype[LOOTNUM];
	for(int l=0;l<LOOTNUM;l++)havetype[l]=0;
	vector<int> loottype;

	//FIND ALL LOOT TYPES
	for(int loc=0;loc<location.size();loc++)
		{
		if(location[loc]->renting==-1)continue;

		for(int l=0;l<location[loc]->loot.size();l++)
			{
			if(location[loc]->loot[l]->type!=ITEM_LOOT)continue;

			if(location[loc]->loot[l]->loottype!=LOOT_CEOPHOTOS&&
				location[loc]->loot[l]->loottype!=LOOT_INTHQDISK&&
				location[loc]->loot[l]->loottype!=LOOT_CORPFILES&&
				location[loc]->loot[l]->loottype!=LOOT_SECRETDOCUMENTS&&
				location[loc]->loot[l]->loottype!=LOOT_POLICERECORDS)continue;

			if(!havetype[location[loc]->loot[l]->loottype])
				{
				loottype.push_back(location[loc]->loot[l]->loottype);
				havetype[location[loc]->loot[l]->loottype]=1;
				}
			}
		}
	for(int sq=0;sq<squad.size();sq++)
		{
		for(int l=0;l<squad[sq]->loot.size();l++)
			{
			if(squad[sq]->loot[l]->type!=ITEM_LOOT)continue;

			if(squad[sq]->loot[l]->loottype!=LOOT_CEOPHOTOS&&
				squad[sq]->loot[l]->loottype!=LOOT_INTHQDISK&&
				squad[sq]->loot[l]->loottype!=LOOT_CORPFILES&&
				squad[sq]->loot[l]->loottype!=LOOT_SECRETDOCUMENTS&&
				squad[sq]->loot[l]->loottype!=LOOT_POLICERECORDS)continue;

			if(!havetype[squad[sq]->loot[l]->loottype])
				{
				loottype.push_back(squad[sq]->loot[l]->loottype);
				havetype[squad[sq]->loot[l]->loottype]=1;
				}
			}
		}

	if(loottype.size()==0)return -1;

	clearformess=1;
	
	//PICK ONE
	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addstr("Do you want to run a special edition?");

		int x=1,y=10;
		char str[200],str2[200];

		for(int l=page*18;l<loottype.size()&&l<page*18+18;l++)
			{
			getloot(str2,loottype[l]);
			str[0]=l-page*18+'A';
			str[1]='\x0';
			strcat(str," - ");
			strcat(str,str2);

			move(y,x);
			addstr(str);

			x+=26;
			if(x>53)
				{
				x=1;
				y++;
				}
			}

		//PAGE UP
		if(page>0)
			{
			move(17,1);
			if(interface_pgup=='[')
				{
				addstr("[ - Previous");
				}
			else
				{
				addstr("PGUP - Previous");
				}
			}
		//PAGE DOWN
		if((page+1)*18<loottype.size())
			{
			move(17,53);
			if(interface_pgup=='[')
				{
				addstr("] - Next");
				}
			else
				{
				addstr("PGDN - Next");
				}
			}

		move(24,1);
		addstr("X - Not in this month's Liberal Guardian");

		refresh();

		int c=getch();
		translategetch(c);

		if(c>='a'&&c<='r')
			{
			int slot=c-'a'+page*18;

			if(slot>=0&&slot<loottype.size())
				{
				//DELETE THE ITEM
				for(int loc=0;loc<location.size();loc++)
					{
					if(location[loc]->renting==-1)continue;

					for(int l=0;l<location[loc]->loot.size();l++)
						{
						if(location[loc]->loot[l]->type!=ITEM_LOOT)continue;

						if(location[loc]->loot[l]->loottype==loottype[slot])
							{
							delete location[loc]->loot[l];
							location[loc]->loot.erase(location[loc]->loot.begin() + l);
							return loottype[slot];
							}
						}
					}
				for(int sq=0;sq<squad.size();sq++)
					{
					for(int l=0;l<squad[sq]->loot.size();l++)
						{
						if(squad[sq]->loot[l]->type!=ITEM_LOOT)continue;

						if(squad[sq]->loot[l]->loottype==loottype[slot])
							{
							delete squad[sq]->loot[l];
							squad[sq]->loot.erase(squad[sq]->loot.begin() + l);
							return loottype[slot];
							}
						}
					}

				//WHOOPS!
				return loottype[slot];
				}
			}

		if(c=='x')
			{
			return -1;
			}

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*18<loottype.size())page++;

		}while(1);

	return -1;
}

unsigned long fenceselect(void)
{
	unsigned long ret=0;

	consolidateloot(activesquad->loot);

	int page=0;

	vector<char> selected;
	selected.resize(activesquad->loot.size());
	for(int s=0;s<selected.size();s++)selected[s]=0;

	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addstr("What will you sell?");

		if(ret>0)
			{
			char num[20];
			move(0,30);
			addstr("Estimated Liberal Amount: $");
			itoa(ret,num,10);
			addstr(num);
			}

		printparty();

		int x=1,y=10;
		char str[200],str2[200];

		for(int l=page*18;l<activesquad->loot.size()&&l<page*18+18;l++)
			{
			if(selected[l])set_color(COLOR_GREEN,COLOR_BLACK,1);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);
			if(activesquad->loot[l]->type==ITEM_WEAPON)
				{
				getweaponfull(str2,activesquad->loot[l]->weapon.type);
				if(activesquad->loot[l]->weapon.ammo>0)
					{
					char num[20];
					itoa(activesquad->loot[l]->weapon.ammo,num,10);
					strcat(str2," (");
					strcat(str2,num);
					strcat(str2,")");
					}
				}
			if(activesquad->loot[l]->type==ITEM_ARMOR)
				{
				getarmorfull(str2,activesquad->loot[l]->armor,0);
				}
			if(activesquad->loot[l]->type==ITEM_CLIP)
				{
				getclip(str2,activesquad->loot[l]->cliptype);
				}
			if(activesquad->loot[l]->type==ITEM_LOOT)
				{
				getloot(str2,activesquad->loot[l]->loottype);
				}
			if(activesquad->loot[l]->number>1)
				{
				char num[20];
				strcat(str2," ");
				if(selected[l]>0)
					{
					itoa(selected[l],num,10);
					strcat(str2,num);
					strcat(str2,"/");
					}
				else strcat(str2,"x");
				itoa(activesquad->loot[l]->number,num,10);
				strcat(str2,num);
				}

			str[0]=l-page*18+'A';
			str[1]='\x0';
			strcat(str," - ");
			strcat(str,str2);

			move(y,x);
			addstr(str);

			x+=26;
			if(x>53)
				{
				x=1;
				y++;
				}
			}

		//PAGE UP
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		if(page>0)
			{
			move(17,1);
			if(interface_pgup=='[')
				{
				addstr("[ - Previous");
				}
			else
				{
				addstr("PGUP - Previous");
				}
			}
		//PAGE DOWN
		if((page+1)*18<activesquad->loot.size())
			{
			move(17,53);
			if(interface_pgup=='[')
				{
				addstr("] - Next");
				}
			else
				{
				addstr("PGDN - Next");
				}
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(23,1);
		addstr("Press a letter to select an item to sell.");
		move(24,1);
		addstr("X - Done");

		refresh();

		int c=getch();
		translategetch(c);

		if(c>='a'&&c<='r')
			{
			int slot=c-'a'+page*18;

			if(slot>=0&&slot<activesquad->loot.size())
				{
				if(selected[slot])
					{
					ret-=fencevalue(*activesquad->loot[slot])*selected[slot];
					selected[slot]=0;
					}
				else
					{
					char bad=0;
					if(activesquad->loot[slot]->type==ITEM_ARMOR)
						{
						if(activesquad->loot[slot]->armor.quality!='1')bad=1;
						if(activesquad->loot[slot]->armor.flag!=0)bad=1;
						}

					if(bad)
						{
						printparty();

						move(8,15);
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						addstr(" You can't sell damaged or second-rate goods.");

						refresh();
						getch();
						}
					else
						{
						if(activesquad->loot[slot]->number>1)
							{
							selected[slot]=1;

							printparty();

							move(8,15);
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							addstr("     How many?       ");

							refresh();

							char str[100];

							keypad(stdscr,FALSE);
							raw_output(FALSE);
							echo();
							curs_set(1);
							mvgetstr(8,30,str);
							curs_set(0);
							noecho();
							raw_output(TRUE);
							keypad(stdscr,TRUE);

							selected[slot]=atoi(str);
							if(selected[slot]<0)selected[slot]=0;
							else if(selected[slot]>activesquad->loot[slot]->number)selected[slot]=activesquad->loot[slot]->number;
							}
						else selected[slot]=1;
						ret+=fencevalue(*activesquad->loot[slot])*selected[slot];
						}
					}
				}
			}

		if(c=='x')break;

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*18<activesquad->loot.size())page++;

		}while(1);

	for(int l=activesquad->loot.size()-1;l>=0;l--)
		{
		if(selected[l]>0)
			{
			activesquad->loot[l]->number-=selected[l];
			if(activesquad->loot[l]->number<=0)
				{
				delete activesquad->loot[l];
				activesquad->loot.erase(activesquad->loot.begin() + l);
				}
			}
		}

  	return ret;
}

unsigned long fencevalue(itemst &it)
{
	unsigned long fenceamount=0;

	switch(it.type)
		{
		case ITEM_WEAPON:
			switch(it.weapon.type)
				{
				case WEAPON_CROWBAR:fenceamount=10;break;
				case WEAPON_BASEBALLBAT:fenceamount=20;break;
				case WEAPON_KNIFE:fenceamount=10;break;
				case WEAPON_SHANK:fenceamount=5;break;
				case WEAPON_SYRINGE:fenceamount=10;break;
				case WEAPON_REVOLVER_22:fenceamount=50;break;
				case WEAPON_REVOLVER_44:fenceamount=100;break;
				case WEAPON_SEMIPISTOL_9MM:fenceamount=100;break;
				case WEAPON_SEMIPISTOL_45:fenceamount=100;break;
				case WEAPON_SEMIRIFLE_M16:fenceamount=250;break;
				case WEAPON_SEMIRIFLE_AK47:fenceamount=250;break;
				case WEAPON_SHOTGUN:fenceamount=100;break;
				case WEAPON_AXE:fenceamount=100;break;
				case WEAPON_HAMMER:fenceamount=100;break;
				case WEAPON_MAUL:fenceamount=100;break;
				case WEAPON_CROSS:fenceamount=100;break;
				case WEAPON_STAFF:fenceamount=100;break;
				case WEAPON_SWORD:fenceamount=100;break;
				case WEAPON_CHAIN:fenceamount=10;break;
				case WEAPON_NIGHTSTICK:fenceamount=20;break;
				case WEAPON_GAVEL:fenceamount=20;break;
				case WEAPON_PITCHFORK:fenceamount=20;break;
				case WEAPON_TORCH:fenceamount=2;break;
				}
			break;
		case ITEM_ARMOR:
			switch(it.armor.type)
				{
				case ARMOR_CLOTHES:fenceamount=20;break;
				case ARMOR_OVERALLS:fenceamount=10;break;
				case ARMOR_WIFEBEATER:fenceamount=5;break;
				case ARMOR_TRENCHCOAT:fenceamount=30;break;
				case ARMOR_WORKCLOTHES:fenceamount=20;break;
				case ARMOR_SECURITYUNIFORM:fenceamount=100;break;
				case ARMOR_POLICEUNIFORM:fenceamount=200;break;
				case ARMOR_CHEAPSUIT:fenceamount=150;break;
				case ARMOR_EXPENSIVESUIT:fenceamount=1500;break;
				case ARMOR_BLACKSUIT:fenceamount=200;break;
				case ARMOR_CHEAPDRESS:fenceamount=150;break;
				case ARMOR_EXPENSIVEDRESS:fenceamount=1500;break;
				case ARMOR_BLACKDRESS:fenceamount=200;break;
				case ARMOR_LABCOAT:fenceamount=80;break;
				case ARMOR_BLACKROBE:fenceamount=80;break;
				case ARMOR_CLOWNSUIT:fenceamount=80;break;
				case ARMOR_BONDAGEGEAR:fenceamount=150;break;
				case ARMOR_MASK:fenceamount=5;break;
				case ARMOR_MILITARY:fenceamount=200;break;
				case ARMOR_PRISONGUARD:fenceamount=150;break;
				case ARMOR_PRISONER:fenceamount=100;break;
				case ARMOR_TOGA:fenceamount=40;break;
				case ARMOR_MITHRIL:fenceamount=20;break;
				}
			break;
		case ITEM_CLIP:
			switch(it.cliptype)
				{
				case CLIP_22:fenceamount=2;break;
				case CLIP_44:fenceamount=3;break;
				case CLIP_45:fenceamount=4;break;
				case CLIP_9:fenceamount=4;break;
				case CLIP_ASSAULT:fenceamount=8;break;
				case CLIP_BUCKSHOT:fenceamount=2;break;
				}
			break;
		case ITEM_LOOT:
			switch(it.loottype)
				{
				case LOOT_LABEQUIPMENT:fenceamount=50;break;
				case LOOT_COMPUTER:fenceamount=200;break;
				case LOOT_TV:fenceamount=50;break;
				case LOOT_VCR:fenceamount=50;break;
				case LOOT_CELLPHONE:fenceamount=20;break;
				case LOOT_TVCAMERA:fenceamount=50;break;
				case LOOT_BROADCASTINGEQUIPMENT:fenceamount=50;break;
				case LOOT_SILVERWARE:fenceamount=20;break;
				case LOOT_SCANNER:fenceamount=50;break;
				case LOOT_PRINTER:fenceamount=50;break;
				case LOOT_FINECLOTH:fenceamount=20;break;
				case LOOT_CHEMICAL:fenceamount=20;break;
				case LOOT_CEOPHOTOS:fenceamount=1000;break;
				case LOOT_SECRETDOCUMENTS:fenceamount=1500;break;
				case LOOT_INTHQDISK:fenceamount=1500;break;
				case LOOT_CORPFILES:fenceamount=1000;break;
				case LOOT_POLICERECORDS:fenceamount=750;break;
				}
			break;
		}

	return fenceamount;
}

void getrecruitcreature(char *str,int type)
{
	strcpy(str,"");

	switch(type)
		{
		case CREATURE_SECURITYGUARD:strcat(str,"Security Guard");break;
		case CREATURE_SCIENTIST_LABTECH:strcat(str,"Lab Tech");break;
		case CREATURE_SCIENTIST_EMINENT:strcat(str,"Eminent Scientist");break;
		case CREATURE_CORPORATE_MANAGER:strcat(str,"Corporate Manager");break;
		case CREATURE_CORPORATE_CEO:strcat(str,"CEO of a Corporation");break;
		case CREATURE_WORKER_SERVANT:
			if(law[LAW_LABOR]==-2&&
			   law[LAW_CORPORATE]==-2)strcpy(str,"Slave");
			else strcat(str,"Servant");
			break;
		case CREATURE_WORKER_JANITOR:strcat(str,"Janitor");break;
		case CREATURE_WORKER_SWEATSHOP:strcat(str,"Sweatshop Worker");break;
		case CREATURE_WORKER_FACTORY_NONUNION:strcat(str,"Factory Worker");break;
		case CREATURE_WORKER_FACTORY_CHILD:strcat(str,"Child Worker");break;
		case CREATURE_WORKER_SECRETARY:strcat(str,"Secretary");break;
		case CREATURE_WORKER_FACTORY_UNION:strcat(str,"Factory Worker");break;
		case CREATURE_LANDLORD:strcat(str,"Landlord");break;
		case CREATURE_COP:strcat(str,"Police Officer");break;
		case CREATURE_DEATHSQUAD:strcat(str,"Death Squad");break;
		case CREATURE_GANGUNIT:strcat(str,"Police Gang Unit");break;
		case CREATURE_PRISONGUARD:strcat(str,"Prison Guard");break;
		case CREATURE_EDUCATOR:strcat(str,"Educator");break;
		case CREATURE_MERC:strcat(str,"Mercenary");break;
		case CREATURE_HICK:strcat(str,"Redneck");break;
		case CREATURE_SOLDIER:strcat(str,"Soldier");break;
		case CREATURE_JUDGE_LIBERAL:strcat(str,"Judge");break;
		case CREATURE_JUDGE_CONSERVATIVE:strcat(str,"Judge");break;
		case CREATURE_AGENT:strcat(str,"Agent");break;
		case CREATURE_RADIOPERSONALITY:strcat(str,"Radio Personality");break;
		case CREATURE_NEWSANCHOR:strcat(str,"Cable News Anchor");break;
		case CREATURE_LAWYER:strcat(str,"Lawyer");break;
		case CREATURE_SEWERWORKER:strcat(str,"Sewer Worker");break;
		case CREATURE_COLLEGESTUDENT:strcat(str,"College Student");break;
		case CREATURE_MUSICIAN:strcat(str,"Musician");break;
		case CREATURE_MATHEMATICIAN:strcat(str,"Mathematician");break;
		case CREATURE_TEACHER:strcat(str,"Teacher");break;
		case CREATURE_HSDROPOUT:strcat(str,"Highschool Dropout");break;
		case CREATURE_BUM:strcat(str,"Transient");break;
		case CREATURE_MUTANT:strcat(str,"Mutant");break;
		case CREATURE_GANGMEMBER:strcat(str,"Gang Member");break;
		case CREATURE_CRACKHEAD:strcat(str,"Crack Head");break;
		case CREATURE_PRIEST:strcat(str,"Priest");break;
		case CREATURE_ENGINEER:strcat(str,"Engineer");break;
		case CREATURE_FASTFOODWORKER:strcat(str,"Fast Food Worker");break;
		case CREATURE_TELEMARKETER:strcat(str,"Telemarketer");break;
		case CREATURE_OFFICEWORKER:strcat(str,"Office Worker");break;
		case CREATURE_FOOTBALLCOACH:strcat(str,"Football Coach");break;
		case CREATURE_PROSTITUTE:strcat(str,"Prostitute");break;
		case CREATURE_MAILMAN:strcat(str,"Mail Carrier");break;
		case CREATURE_GARBAGEMAN:strcat(str,"Garbage Collector");break;
		case CREATURE_PLUMBER:strcat(str,"Plumber");break;
		case CREATURE_CHEF:strcat(str,"Chef");break;
		case CREATURE_CONSTRUCTIONWORKER:strcat(str,"Construction Worker");break;
		case CREATURE_AMATEURMAGICIAN:strcat(str,"Amateur Magician");break;
		case CREATURE_HIPPIE:strcat(str,"Hippie");break;
		case CREATURE_CRITIC_ART:strcat(str,"Art Critic");break;
		case CREATURE_CRITIC_MUSIC:strcat(str,"Music Critic");break;
		case CREATURE_AUTHOR:strcat(str,"Author");break;
		case CREATURE_JOURNALIST:strcat(str,"Journalist");break;
		case CREATURE_SOCIALITE:strcat(str,"Socialite");break;
		case CREATURE_BIKER:strcat(str,"Biker");break;
		case CREATURE_TRUCKER:strcat(str,"Trucker");break;
		case CREATURE_TAXIDRIVER:strcat(str,"Taxi Driver");break;
		case CREATURE_PROGRAMMER:strcat(str,"Programmer");break;
		case CREATURE_NUN:strcat(str,"Nun");break;
		case CREATURE_RETIREE:strcat(str,"Retiree");break;
		case CREATURE_PAINTER:strcat(str,"Painter");break;
		case CREATURE_SCULPTOR:strcat(str,"Sculptor");break;
		case CREATURE_DANCER:strcat(str,"Dancer");break;
		case CREATURE_PHOTOGRAPHER:strcat(str,"Photographer");break;
		case CREATURE_CAMERAMAN:strcat(str,"Cameraman");break;
		case CREATURE_HAIRSTYLIST:strcat(str,"Hairstylist");break;
		case CREATURE_FASHIONDESIGNER:strcat(str,"Fashion Designer");break;
		case CREATURE_CLERK:strcat(str,"Clerk");break;
		case CREATURE_THIEF:strcat(str,"Professional Thief");break;
		case CREATURE_ACTOR:strcat(str,"Actor");break;
		case CREATURE_YOGAINSTRUCTOR:strcat(str,"Yoga Instructor");break;
		case CREATURE_ATHLETE:strcat(str,"Athlete");break;
		case CREATURE_TEENAGER:strcat(str,"Teenager");break;
		case CREATURE_PRISONER:strcat(str,"Prisoner");break;
		default:
			strcat(str,"Liberal");
			break;
		}
}

char incapacitated(creaturest &a,char noncombat,char &printed)
{
	printed=0;

	set_color(COLOR_WHITE,COLOR_BLACK,1);

	if(a.animalgloss==ANIMALGLOSS_TANK)
		{
		if(a.blood<=20||(a.blood<=50&&(LCSrandom(2)||a.forceinc)))
			{
			a.forceinc=0;
			if(noncombat)
				{
				clearmessagearea();

				move(16,1);
				addstr("The ");
				addstr(a.name);
				switch(LCSrandom(3))
					{
					case 0:addstr(" smokes...");
						break;
					case 1:addstr(" smolders.");
						break;
					case 2:addstr(" burns...");
						break;
					}

				printed=1;
				}
			return 1;
			}

		return 0;
		}

	if(a.animalgloss==ANIMALGLOSS_ANIMAL)
		{
		if(a.blood<=20||(a.blood<=50&&(LCSrandom(2)||a.forceinc)))
			{
			a.forceinc=0;
			if(noncombat)
				{
				clearmessagearea();

				move(16,1);
				addstr("The ");
				addstr(a.name);
				switch(LCSrandom(3))
					{
					case 0:addstr(" yelps in pain...");
						break;
					case 1:
						if(law[LAW_FREESPEECH]==-2)addstr(" [makes a stinky].");
						else addstr(" soils the floor.");
						break;
					case 2:addstr(" yowls pitifully...");
						break;
					}

				printed=1;
				}
			return 1;
			}

		return 0;
		}

	if(a.blood<=20||(a.blood<=50&&(LCSrandom(2)||a.forceinc)))
		{
		a.forceinc=0;
		if(noncombat)
			{
			clearmessagearea();

			move(16,1);
			addstr(a.name);
			switch(LCSrandom(54))
				{
				case 0:
					addstr(" desperately cries out to Jesus.");
					break;
				case 1:
					if(law[LAW_FREESPEECH]==-2)addstr(" [makes a stinky].");
					else addstr(" soils the floor.");
					break;
				case 2:
					addstr(" whimpers in a corner.");
					break;
				case 3:
					addstr(" begins to weep.");
					break;
				case 4:
					addstr(" vomits.");
					break;
				case 5:
					addstr(" chortles...");
					break;
				case 6:
					addstr(" screams in pain.");
					break;
				case 7:
					addstr(" asks for mother.");
					break;
				case 8:
					addstr(" prays softly...");
					break;
				case 9:
					addstr(" clutches at the wounds.");
					break;
				case 10:
					addstr(" reaches out and moans.");
					break;
				case 11:
					addstr(" hollers in pain.");
					break;
				case 12:
					addstr(" groans in agony.");
					break;
				case 13:
					addstr(" begins hyperventilating.");
					break;
				case 14:
					addstr(" shouts a prayer.");
					break;
				case 15:
					addstr(" coughs up blood.");
					break;
				case 16:
					if(mode!=GAMEMODE_CHASECAR)addstr(" stumbles against a wall.");
					else addstr(" leans against the door.");
					break;
				case 17:
					addstr(" begs for forgiveness.");
					break;
				case 18:
					addstr(" shouts \"Why have you forsaken me?\"");
					break;
				case 19:
					addstr(" murmurs \"Why Lord?  Why?\"");
					break;
				case 20:
					addstr(" whispers \"Am I dead?\"");
					break;
				case 21:
					if(law[LAW_FREESPEECH]==-2)addstr(" [makes a mess], moaning.");
					else addstr(" pisses on the floor, moaning.");
					break;
				case 22:
					addstr(" whispers incoherently.");
					break;
				case 23:
					if(a.special[SPECIALWOUND_RIGHTEYE]&&
						a.special[SPECIALWOUND_LEFTEYE])
						{
						addstr(" stares off into space.");
						}
					else if(a.special[SPECIALWOUND_RIGHTEYE]||
						a.special[SPECIALWOUND_LEFTEYE])
						{
						addstr(" stares into space with one empty eye.");
						}
					else
						{
						addstr(" stares out with hollow sockets.");
						}
					break;
				case 24:
					addstr(" cries softly.");
					break;
				case 25:
					addstr(" yells until the scream cracks dry.");
					break;
				case 26:
					if(a.special[SPECIALWOUND_TEETH]>1)addstr("'s teeth start chattering.");
					else if(a.special[SPECIALWOUND_TEETH]==1)addstr("'s tooth starts chattering.");
					else addstr("'s gums start chattering.");
					break;
				case 27:
					addstr(" starts shaking uncontrollably.");
					break;
				case 28:
					addstr(" looks strangly calm.");
					break;
				case 29:
					addstr(" nods off for a moment.");
					break;
				case 30:
					addstr(" starts drooling.");
					break;
				case 31:
					addstr(" seems lost in memories.");
					break;
				case 32:
					addstr(" shakes with fear.");
					break;
				case 33:
					addstr(" murmurs \"I'm so afraid...\"");
					break;
				case 34:
					addstr(" cries \"It can't be like this...\"");
					break;
				case 35:
					if(a.type==CREATURE_TEENAGER||
						a.type==CREATURE_WORKER_FACTORY_CHILD)
						{
						addstr(" cries \"Mommy!\"");
						}
					else addstr(" murmurs \"What about my children?\"");
					break;
				case 36:
					addstr(" shudders quietly.");
					break;
				case 37:
					addstr(" yowls pitifully.");
					break;
				case 38:
					addstr(" begins losing faith in God.");
					break;
				case 39:
					addstr(" muses quietly about death.");
					break;
				case 40:
					addstr(" asks for a blanket.");
					break;
				case 41:
					addstr(" shivers softly.");
					break;
				case 42:
					if(law[LAW_FREESPEECH]==-2)addstr(" [makes a mess].");
					else addstr(" vomits up a clot of blood.");
					break;
				case 43:
					if(law[LAW_FREESPEECH]==-2)addstr(" [makes a mess].");
					else addstr(" spits up a cluster of bloody bubbles.");
					break;
				case 44:
					addstr(" pleads for mercy.");
					break;
				case 45:
					addstr(" quietly asks for coffee.");
					break;
				case 46:
					addstr(" looks resigned.");
					break;
				case 47:
					addstr(" scratches at the air.");
					break;
				case 48:
					addstr(" starts to giggle uncontrollably.");
					break;
				case 49:
					addstr(" wears a look of pain.");
					break;
				case 50:
					addstr(" questions God.");
					break;
				case 51:
					addstr(" whispers \"Mama baby.  Baby loves mama.\"");
					break;
				case 52:
					addstr(" asks for childhood toys frantically.");
					break;
				case 53:
					addstr(" murmurs \"But I go to church...\"");
					break;
				}

			printed=1;
			}

		return 1;
		}

	if(a.special[SPECIALWOUND_NECK]==2||
		a.special[SPECIALWOUND_UPPERSPINE]==2)
		{
		if(!noncombat)
			{
			clearmessagearea();

			move(16,1);
			addstr(a.name);
			switch(LCSrandom(5))
				{
				case 0:
					addstr(" looks on with authority.");
					break;
				case 1:
					addstr(" waits patiently.");
					break;
				case 2:
					addstr(" sits in thought.");
					break;
				case 3:
					addstr(" breathes slowly.");
					break;
				case 4:
					addstr(" considers the situation.");
					break;
				}

			printed=1;
			}

		return 1;
		}

	return 0;
}

void printhealthstat(creaturest &g,int y,int x,char smll)
{
	short woundsum=0;
	char bleeding=0;
	for(int w=0;w<BODYPARTNUM;w++)
		{
		if(g.wound[w]!=0)woundsum++;
		if(g.wound[w] & WOUND_BLEEDING)bleeding=1;
		}

	int armok=2;
	int legok=2;
	if((g.wound[BODYPART_ARM_RIGHT] & WOUND_NASTYOFF)||
		 (g.wound[BODYPART_ARM_RIGHT] & WOUND_CLEANOFF))armok--;
	if((g.wound[BODYPART_ARM_LEFT] & WOUND_NASTYOFF)||
		 (g.wound[BODYPART_ARM_LEFT] & WOUND_CLEANOFF))armok--;
	if((g.wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
		 (g.wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
	if((g.wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
		 (g.wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;

	move(y,x);
	if(bleeding)set_color(COLOR_RED,COLOR_BLACK,1);
	else set_color(COLOR_WHITE,COLOR_BLACK,1);
	if(!g.alive)
		{
		set_color(COLOR_BLACK,COLOR_BLACK,1);
		addstr("Deceased");
		}
	else if(g.blood<=20)
		{
		if(smll)addstr("NearDETH");
		else addstr("Near Death");
		}
	else if(g.blood<=50)
		{
		if(smll)addstr("BadWound");
		else addstr("Badly Wounded");
		}
	else if(g.blood<=75)
		{
		addstr("Wounded");
		}
	else if(g.blood<100)
		{
		if(smll)addstr("LtWound");
		else addstr("Lightly Wounded");
		}
	else if(g.special[SPECIALWOUND_NECK]==2)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		if(smll)addstr("NckBroke");
		else addstr("Neck Broken");
		}
	else if(g.special[SPECIALWOUND_UPPERSPINE]==2)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		if(smll)addstr("Quadpleg");
		else addstr("Quadraplegic");
		}
	else if(g.special[SPECIALWOUND_LOWERSPINE]==2)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		if(smll)addstr("Parapleg");
		else addstr("Paraplegic");
		}
	else if(g.special[SPECIALWOUND_RIGHTEYE]==0&&
		g.special[SPECIALWOUND_LEFTEYE]==0&&
		g.special[SPECIALWOUND_NOSE]==0)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		if(smll)addstr("FaceGone");
		else addstr("Face Gone");
		}
	else if(legok==0&&armok==0)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		addstr("No Limbs");
		}
	else if((legok==1&&armok==0)||(armok==1&&legok==0))
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		addstr("One Limb");
		}
	else if(legok==2&&armok==0)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		addstr("No Arms");
		}
	else if(legok==0&&armok==2)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		addstr("No Legs");
		}
	else if(legok==1&&armok==1)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		if(smll)addstr("1Arm1Leg");
		else addstr("One Arm, One Leg");
		}
	else if(armok==1)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		addstr("One Arm");
		}
	else if(legok==1)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		addstr("One Arm");
		}
	else if(g.special[SPECIALWOUND_RIGHTEYE]==0&&
		g.special[SPECIALWOUND_LEFTEYE]==0)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		addstr("Blind");
		}
	else if((g.special[SPECIALWOUND_RIGHTEYE]==0||
		g.special[SPECIALWOUND_LEFTEYE]==0)&&
		g.special[SPECIALWOUND_NOSE]==0)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		if(smll)addstr("FaceMutl");
		else addstr("Face Mutilated");
		}
	else if(g.special[SPECIALWOUND_NOSE]==0)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		if(smll)addstr("NoseGone");
		else addstr("Missing Nose");
		}
	else if(g.special[SPECIALWOUND_RIGHTEYE]==0||
		g.special[SPECIALWOUND_LEFTEYE]==0)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		if(smll)addstr("One Eye");
		else addstr("Missing Eye");
		}
	else if(g.special[SPECIALWOUND_TONGUE]==0)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		if(smll)addstr("NoTongue");
		else addstr("No Tongue");
		}
	else if(g.special[SPECIALWOUND_TEETH]==0)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		addstr("No Teeth");
		}
	else if(g.special[SPECIALWOUND_TEETH]<TOOTHNUM)
		{
		if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
		if(smll)addstr("MisTeeth");
		else addstr("Missing Teeth");
		}
	else
		{
		if(g.align==-1)
			{
			set_color(COLOR_RED,COLOR_BLACK,1);
			if(smll)addstr("Consrvtv");
			else addstr("Conservative");
			}
		else if(g.align==0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			addstr("Moderate");
			}
		else
			{
			set_color(COLOR_GREEN,COLOR_BLACK,1);
			addstr("Liberal");
			}
		}
}

void healthmodroll(int &aroll,creaturest &a)
{
	if(a.special[SPECIALWOUND_RIGHTEYE]!=1)aroll-=LCSrandom(2);
	if(a.special[SPECIALWOUND_LEFTEYE]!=1)aroll-=LCSrandom(2);
	if(a.special[SPECIALWOUND_RIGHTEYE]!=1&&
		a.special[SPECIALWOUND_LEFTEYE]!=1)aroll-=LCSrandom(20);
	if(a.special[SPECIALWOUND_RIGHTLUNG]!=1)aroll-=LCSrandom(8);
	if(a.special[SPECIALWOUND_LEFTLUNG]!=1)aroll-=LCSrandom(8);
	if(a.special[SPECIALWOUND_HEART]!=1)aroll-=LCSrandom(10);
	if(a.special[SPECIALWOUND_LIVER]!=1)aroll-=LCSrandom(5);
	if(a.special[SPECIALWOUND_STOMACH]!=1)aroll-=LCSrandom(5);
	if(a.special[SPECIALWOUND_RIGHTKIDNEY]!=1)aroll-=LCSrandom(5);
	if(a.special[SPECIALWOUND_LEFTKIDNEY]!=1)aroll-=LCSrandom(5);
	if(a.special[SPECIALWOUND_SPLEEN]!=1)aroll-=LCSrandom(4);
	if(a.special[SPECIALWOUND_LOWERSPINE]!=1)aroll-=LCSrandom(100);
	if(a.special[SPECIALWOUND_UPPERSPINE]!=1)aroll-=LCSrandom(200);
	if(a.special[SPECIALWOUND_NECK]!=1)aroll-=LCSrandom(300);
	if(a.special[SPECIALWOUND_RIBS]<RIBNUM)aroll-=LCSrandom(5);
	if(a.special[SPECIALWOUND_RIBS]<RIBNUM/2)aroll-=LCSrandom(5);
	if(a.special[SPECIALWOUND_RIBS]==0)aroll-=LCSrandom(5);
}

int clinictime(creaturest &g)
{
	int time=0;

	for(int w=0;w<BODYPARTNUM;w++)
		{
		if((g.wound[w] & WOUND_NASTYOFF)||
			(g.wound[w] & WOUND_CLEANOFF))
			{
			time++;
			}
		}
	if(g.blood<=10)time++;
	if(g.blood<=50)time++;
	if(g.blood<100)time++;
	if(!g.special[SPECIALWOUND_RIGHTLUNG])time++;
	if(!g.special[SPECIALWOUND_LEFTLUNG])time++;
	if(!g.special[SPECIALWOUND_HEART])time+=2;
	if(!g.special[SPECIALWOUND_LIVER])time++;
	if(!g.special[SPECIALWOUND_STOMACH])time++;
	if(!g.special[SPECIALWOUND_RIGHTKIDNEY])time++;
	if(!g.special[SPECIALWOUND_LEFTKIDNEY])time++;
	if(!g.special[SPECIALWOUND_SPLEEN])time++;
	if(g.special[SPECIALWOUND_RIBS]<RIBNUM)time++;
	if(!g.special[SPECIALWOUND_NECK])time++;
	if(!g.special[SPECIALWOUND_UPPERSPINE])time++;
	if(!g.special[SPECIALWOUND_LOWERSPINE])time++;

	return time;
}

void siegecheck(char canseethings)
{
	if(disbanding)return;

	//FIRST, THE COPS
	int numpres;
	for(int l=0;l<location.size();l++)
		{
		if(location[l]->siege.siege)continue;
		if(location[l]->renting==-1)continue;
		numpres=0;

		if(location[l]->siege.timeuntillocated==-2)
			{
			//IF JUST SIEGED, BUY SOME TIME
			location[l]->siege.timeuntillocated=-1;
			}
		else
			{
			//HUNTING
			if(location[l]->siege.timeuntillocated>0)
				{
				if(location[l]->front_business==-1||LCSrandom(2))
					{
					location[l]->siege.timeuntillocated--;
					if(offended_cops&&
						location[l]->siege.timeuntillocated>1&&!LCSrandom(2))
						{
						location[l]->siege.timeuntillocated--;
						}
					}
				}

			//CHECK FOR CRIMINALS AT THIS BASE
			char crimes=0;
			for(int p=0;p<pool.size();p++)
				{
				if(!pool[p]->alive)continue;
				if(pool[p]->location!=l)continue;
				if(pool[p]->align!=1)continue;
				numpres++;

				if(pool[p]->flag & CREATUREFLAG_KIDNAPPED)crimes++;

				if(pool[p]->lawflag2 & LAWFLAG2_CARTHEFT)crimes++;
				if(pool[p]->lawflag2 & LAWFLAG2_INFORMATION)crimes++;
				if(pool[p]->lawflag2 & LAWFLAG2_COMMERCE)crimes++;
				if(pool[p]->lawflag2 & LAWFLAG2_CCFRAUD)crimes++;
				if(pool[p]->lawflag2 & LAWFLAG2_BROWNIES)crimes++;
				if(pool[p]->lawflag2 & LAWFLAG2_BURIAL)crimes++;
				if(pool[p]->lawflag2 & LAWFLAG2_PROSTITUTION)crimes++;
				if(pool[p]->lawflag2 & LAWFLAG2_DISTURBANCE)crimes++;
				if(pool[p]->lawflag2 & LAWFLAG2_HIREILLEGAL)crimes++;
				if(pool[p]->lawflag2 & LAWFLAG2_RACKETEERING)crimes++;
				if(pool[p]->lawflag2 & LAWFLAG2_LOITERING)crimes++;
				if(pool[p]->lawflag & LAWFLAG_KIDNAPPER)crimes++;
				if(pool[p]->lawflag & LAWFLAG_MURDERER)crimes++;
				if(pool[p]->lawflag & LAWFLAG_THIEF)crimes++;
				if(pool[p]->lawflag & LAWFLAG_BREAKING)crimes++;
				if(pool[p]->lawflag & LAWFLAG_TERRORISM)crimes++;
				if(pool[p]->lawflag & LAWFLAG_JURY)crimes++;
				if(pool[p]->lawflag & LAWFLAG_TREASON)crimes++;
				if(pool[p]->lawflag & LAWFLAG_ESCAPED)crimes++;
				if(pool[p]->lawflag & LAWFLAG_HELPESCAPE)crimes++;
				if(pool[p]->lawflag & LAWFLAG_RESIST)crimes++;
				if(pool[p]->lawflag & LAWFLAG_BURNFLAG)
					{
					if(law[LAW_FLAGBURNING]<=0)crimes++;
					else pool[p]->lawflag&=~LAWFLAG_BURNFLAG;
					}
				if(pool[p]->lawflag & LAWFLAG_SPEECH)
					{
					if(law[LAW_FREESPEECH]==-2)crimes++;
					else pool[p]->lawflag&=~LAWFLAG_SPEECH;
					}
				if(pool[p]->lawflag & LAWFLAG_VANDALISM)crimes++;
				if(pool[p]->lawflag & LAWFLAG_ASSAULT)crimes++;
				}

			if(crimes)
				{
				if(location[l]->siege.timeuntillocated==-1)
					{
					location[l]->siege.timeuntillocated=LCSrandom(10)+10;
					if(location[l]->front_business!=-1)location[l]->siege.timeuntillocated+=100+LCSrandom(101);
					if(location[l]->type==SITE_RESIDENTIAL_TENEMENT)location[l]->siege.timeuntillocated+=50+LCSrandom(51);
					if(location[l]->type==SITE_RESIDENTIAL_APARTMENT)location[l]->siege.timeuntillocated+=150+LCSrandom(151);
					if(location[l]->type==SITE_RESIDENTIAL_APARTMENT_UPSCALE)location[l]->siege.timeuntillocated+=350+LCSrandom(351);
					}
				}

			//COPS RAID THIS LOCATION
			if(!location[l]->siege.timeuntillocated)
				{
				location[l]->siege.timeuntillocated=-2;

				if(numpres>0)
					{
					erase();
					set_color(COLOR_WHITE,COLOR_BLACK,1);

					if(location[l]->compound_walls & COMPOUND_BASIC)
						{
						move(8,1);
						addstr("The cops have surrounded the ");
						addlocationname(location[l]);
						addstr(".");
						location[l]->siege.underattack=0;
						}
					else
						{
						move(8,1);
						addstr("The cops are raiding the ");
						addlocationname(location[l]);
						addstr("!");
						location[l]->siege.underattack=1;
						}

					//MENTION ESCALATION STATE
					if(location[l]->siege.escalationstate==1)
						{
						move(9,1);
						addstr("Some national guard troops have joined them.");
						}
					if(location[l]->siege.escalationstate==2)
						{
						move(9,1);
						addstr("You hear tanks rolling around outside.");
						}
					if(location[l]->siege.escalationstate>=3)
						{
						move(9,1);
						addstr("You hear tanks and aircraft outside.");
						}

					refresh();
					getch();

					statebrokenlaws(l);

					location[l]->siege.siege=1;
					location[l]->siege.siegetype=SIEGE_POLICE;
					location[l]->siege.lights_off=0;
					location[l]->siege.cameras_off=0;
					}
				else
					{
					erase();
					set_color(COLOR_WHITE,COLOR_BLACK,1);

					move(8,1);
					addstr("The cops have raided the ");
					addlocationname(location[l]);
					addstr(", an unoccupied safehouse.");

					refresh();
					getch();

					int y=9;

					for(int p=pool.size()-1;p>=0;p--)
						{
						if(pool[p]->location!=l)continue;
						if(!pool[p]->alive)
							{
							move(y,1);y++;
							addstr(pool[p]->name);
							addstr("'s corpse has been recovered.");
							refresh();
							getch();

							delete pool[p];
							pool.erase(pool.begin() + p);
							continue;
							}
						if(pool[p]->align!=1)
							{
							move(y,1);y++;
							addstr(pool[p]->name);
							addstr(" has been rescued.");
							refresh();
							getch();

							delete pool[p];
							pool.erase(pool.begin() + p);
							continue;
							}
						}
					for(int l2=0;l2<location[l]->loot.size();l2++)
						{
						delete location[l]->loot[l2];
						}
					location[l]->loot.clear();

					for(int v=(int)vehicle.size()-1;v>=0;v--)
						{
						if(vehicle[v]->location==l)
							{
							removecarprefs_pool(vehicle[v]->id);
							delete vehicle[v];
							vehicle.erase(vehicle.begin() + v);
							}
						}
					}
				}
			}

		//OTHER OFFENDABLE ENTITIES
			//CORPS
		if(!location[l]->siege.siege&&offended_corps&&!LCSrandom(600)&&numpres>0)
			{
			erase();
			set_color(COLOR_WHITE,COLOR_BLACK,1);

			move(8,1);
			addstr("The Corporations are raiding the ");
			addlocationname(location[l]);
			addstr("!");

			refresh();
			getch();

			location[l]->siege.siege=1;
			location[l]->siege.siegetype=SIEGE_CORPORATE;
			location[l]->siege.underattack=1;
			location[l]->siege.lights_off=0;
			location[l]->siege.cameras_off=0;
			}
			//CIA
		if(!location[l]->siege.siege&&offended_cia&&!LCSrandom(600)&&numpres>0)
			{
			erase();
			set_color(COLOR_WHITE,COLOR_BLACK,1);

			move(8,1);
			addstr("The CIA is raiding the ");
			addlocationname(location[l]);
			addstr("!");

			if(location[l]->compound_walls & COMPOUND_CAMERAS)
				{
				move(9,1);
				addstr("Through some form of high technology, they've managed");
				move(10,1);
				addstr("to shut off the lights and the cameras.");
				}
			else if(location[l]->compound_walls & COMPOUND_GENERATOR)
				{
				move(9,1);
				addstr("Through some form of high technology, they've managed");
				move(10,1);
				addstr("to shut off the lights.");
				}
			else
				{
				move(9,1);
				addstr("They've shut off the lights.");
				}

			refresh();
			getch();

			location[l]->siege.siege=1;
			location[l]->siege.siegetype=SIEGE_CIA;
			location[l]->siege.underattack=1;
			location[l]->siege.lights_off=1;
			location[l]->siege.cameras_off=1;
			}
			//HICKS
		if(!location[l]->siege.siege&&offended_amradio&&attitude[VIEW_AMRADIO]<=35&&!LCSrandom(600)&&numpres>0)
			{
			erase();
			set_color(COLOR_WHITE,COLOR_BLACK,1);

			move(8,1);
			addstr("Masses dissatisfied with your lack of respect for AM Radio");
			move(9,1);
			addstr("are storming the ");
			addlocationname(location[l]);
			addstr("!");

			refresh();
			getch();

			location[l]->siege.siege=1;
			location[l]->siege.siegetype=SIEGE_HICKS;
			location[l]->siege.underattack=1;
			location[l]->siege.lights_off=0;
			location[l]->siege.cameras_off=0;
			}
		if(!location[l]->siege.siege&&offended_cablenews&&attitude[VIEW_CABLENEWS]<=35&&!LCSrandom(600)&&numpres>0)
			{
			erase();
			set_color(COLOR_WHITE,COLOR_BLACK,1);

			move(8,1);
			addstr("Masses dissatisfied with your lack of respect for Cable News");
			move(9,1);
			addstr("are storming the ");
			addlocationname(location[l]);
			addstr("!");

			refresh();
			getch();

			location[l]->siege.siege=1;
			location[l]->siege.siegetype=SIEGE_HICKS;
			location[l]->siege.underattack=1;
			location[l]->siege.lights_off=0;
			location[l]->siege.cameras_off=0;
			}
		}
}

void siegeturn(char clearformess)
{
	if(disbanding)return;

	for(int l=0;l<location.size();l++)
		{
		if(location[l]->siege.siege&&
			!location[l]->siege.underattack)
			{

	//EAT
	int eat=numbereating(l);
	if(location[l]->compound_stores>=eat)location[l]->compound_stores-=eat;
	else location[l]->compound_stores=0;

	//ATTACK!
	char attack=0;
	if(!LCSrandom(150))attack=1;

	if(attack)
		{
		if(clearformess)
			{
			erase();
			}
		else
			{
			makedelimiter(8,0);
			}
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(8,1);
		addstr("The cops are coming!");

		refresh();
		getch();

		location[l]->siege.underattack=1;
		}
	else
		{
		char no_bad=1;

		//CUT LIGHTS
		if(!location[l]->siege.lights_off &&
			!(location[l]->compound_walls & COMPOUND_GENERATOR) && !LCSrandom(5))
			{
			no_bad=0;

			if(clearformess)
				{
				erase();
				}
			else
				{
				makedelimiter(8,0);
				}
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(8,1);
			addstr("The police have cut the lights.");

			refresh();
			getch();

			location[l]->siege.lights_off=1;
			}

		//SNIPER
		if(!LCSrandom(20))
			{
			no_bad=0;

			vector<int> pol;
			for(int p=0;p<pool.size();p++)
				{
				if(pool[p]->alive&&pool[p]->location==l)
					{
					pol.push_back(p);
					}
				}

			if(pol.size()>0)
				{
				if(clearformess)erase();
				else makedelimiter(8,0);
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(8,1);
				int targ=pol[LCSrandom(pol.size())];
				if(LCSrandom(100)>pool[targ]->juice)
					{
					addstr("A police sniper takes out ");
					addstr(pool[targ]->name);
					addstr("!");

					if(pool[targ]->align==1)stat_dead++;

					removesquadinfo(*pool[targ]);
					delete pool[targ];
					pool.erase(pool.begin() + targ);
					}
				else
					{
					addstr("A police sniper nearly hits ");
					addstr(pool[targ]->name);
					addstr("!");
					}
				refresh();
				getch();
				}
			}
	
		if(location[l]->siege.escalationstate>=3 && !LCSrandom(5))
			{
			no_bad=0;

			//AIR STRIKE!
			char hit=!LCSrandom(3);
			if(!(location[l]->compound_walls & COMPOUND_GENERATOR))hit=0;

			if(clearformess)
				{
				erase();
				}
			else
				{
				makedelimiter(8,0);
				}
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(8,1);
			addstr("You hear planes streak over head!");
			refresh();
			getch();
			if(clearformess)
				{
				erase();
				}
			else
				{
				makedelimiter(8,0);
				}
			move(8,1);
			addstr("Explosions rock the compound!");
			refresh();
			getch();

			if(hit)
				{
				if(clearformess)erase();
				else makedelimiter(8,0);
				move(8,1);
				addstr("The generator has been destroyed!");
				refresh();
				getch();
				if(clearformess)erase();
				else makedelimiter(8,0);
				move(8,1);
				addstr("The lights fade and all is dark. ");
				refresh();
				getch();
				}

			if(!LCSrandom(20))
				{
				vector<int> pol;
				for(int p=0;p<pool.size();p++)
					{
					if(pool[p]->alive&&pool[p]->location==l)
						{
						pol.push_back(p);
						}
					}

				if(pol.size()>0)
					{
					if(clearformess)erase();
					else makedelimiter(8,0);
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(8,1);
					int targ=pol[LCSrandom(pol.size())];
					if(LCSrandom(100)>pool[targ]->juice)
						{
						addstr(pool[targ]->name);
						addstr(" died in the bombing!");

						if(pool[targ]->align==1)stat_dead++;

						removesquadinfo(*pool[targ]);
						delete pool[targ];
						pool.erase(pool.begin() + targ);
						}
					else
						{
						addstr(pool[targ]->name);
						addstr(" narrowly avoided death!");
						}
					refresh();
					getch();
					}
				}
			else
				{
				if(clearformess)erase();
				else makedelimiter(8,0);
				move(8,1);
				addstr("Fortunately, no one is hurt.");
				refresh();
				getch();
				}

			if(hit)
				{
				location[l]->compound_walls&=~COMPOUND_GENERATOR;
				location[l]->siege.lights_off=1;
				}
			}
		if((location[l]->compound_walls & COMPOUND_TANKTRAPS) &&
			location[l]->siege.escalationstate>=3 && !LCSrandom(5))
			{
			no_bad=0;

			//ENGINEERS
			if(clearformess)
				{
				erase();
				}
			else
				{
				makedelimiter(8,0);
				}
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(8,1);
			addstr("Engineers have removed your tank traps.");
			refresh();
			getch();

			location[l]->compound_walls&=~COMPOUND_TANKTRAPS;
			}

		//NEED GOOD THINGS TO BALANCE THE BAD
		int livingpool=0;
		for(int p=0;p<pool.size();p++)
			{
			if(!pool[p]->alive)continue;
			if(pool[p]->align!=1)continue;
			if(pool[p]->location!=l)continue;
			livingpool++;
			}

		if(!LCSrandom(50)&&no_bad&&livingpool)
			{
			char repname[200];
			name(repname);

			set_color(COLOR_WHITE,COLOR_BLACK,0);

			erase();
			move(1,1);
			addstr("Elitist ");
			addstr(repname);
			addstr(" from the ");
			switch(LCSrandom(5))
				{
				case 0:addstr("news program");break;
				case 1:addstr("news magazine");break;
				case 2:addstr("website");break;
				case 3:addstr("scandal rag");break;
				case 4:addstr("newspaper");break;
				}
			addstr(" ");
			switch(LCSrandom(11))
				{
				case 0:addstr("Daily");break;
				case 1:addstr("Nightly");break;
				case 2:addstr("Current");break;
				case 3:addstr("Pressing");break;
				case 4:addstr("Socialist");break;
				case 5:addstr("American");break;
				case 6:addstr("National");break;
				case 7:addstr("Union");break;
				case 8:addstr("Foreign");break;
				case 9:addstr("Associated");break;
				case 10:addstr("International");break;
				}
			addstr(" ");
			switch(LCSrandom(11))
				{
				case 0:addstr("Reporter");break;
				case 1:addstr("Issue");break;
				case 2:addstr("Take");break;
				case 3:addstr("Constitution");break;
				case 4:addstr("Times");break;
				case 5:addstr("Post");break;
				case 6:addstr("News");break;
				case 7:addstr("Affair");break;
				case 8:addstr("Statesman");break;
				case 9:addstr("Star");break;
				case 10:addstr("Inquirer");break;
				}
			move(2,1);
			addstr("got into the compound somehow!");
			refresh();
			getch();

			int best=-1,bestvalue=0,sum;
			for(int p=0;p<pool.size();p++)
				{
				if(!pool[p]->alive)continue;
				if(pool[p]->align!=1)continue;
				if(pool[p]->location!=l)continue;

				sum=0;
				sum+=pool[p]->attval(ATTRIBUTE_INTELLIGENCE);
				sum+=pool[p]->attval(ATTRIBUTE_HEART);
				sum+=pool[p]->attval(ATTRIBUTE_CHARISMA)*2;
				sum+=pool[p]->skill[SKILL_PERSUASION]*3;

				if(sum>bestvalue||best==-1)
					{
					best=p;
					bestvalue=sum;
					}
				}

			move(4,1);
			addstr(pool[best]->name);
			addstr(" decides to give an interview.");
			refresh();
			getch();

			move(6,1);
			addstr("The interview is wide-ranging, covering a variety of topics.");
			refresh();
			getch();

			int segmentpower=LCSrandom(bestvalue*2+1);

			move(8,1);
			if(segmentpower<10)
				{
				addstr(repname);
				addstr(" cancelled the interview halfway through");
				move(9,1);
				addstr("and later used the material for a Broadway play called");
				move(10,1);
				switch(LCSrandom(11))
					{
					case 0:addstr("Flaming");break;
					case 1:addstr("Retarded");break;
					case 2:addstr("Insane");break;
					case 3:addstr("Crazy");break;
					case 4:addstr("Loopy");break;
					case 5:addstr("Idiot");break;
					case 6:addstr("Empty-Headed");break;
					case 7:addstr("Nutty");break;
					case 8:addstr("Half-Baked");break;
					case 9:addstr("Pot-Smoking");break;
					case 10:addstr("Stoner");break;
					}
				addstr(" ");
				switch(LCSrandom(10))
					{
					case 0:addstr("Liberal");break;
					case 1:addstr("Socialist");break;
					case 2:addstr("Anarchist");break;
					case 3:addstr("Communist");break;
					case 4:addstr("Marxist");break;
					case 5:addstr("Green");break;
					case 6:addstr("Elite");break;
					case 7:addstr("Guerilla");break;
					case 8:addstr("Commando");break;
					case 9:addstr("Soldier");break;
					}
				addstr(".");
				}
			else if(segmentpower<15)
				{
				addstr("But the interview is so boring that ");
				addstr(repname);
				addstr(" falls asleep.");
				}
			else if(segmentpower<20)addstr("But the interview sucked.");
			else if(segmentpower<25)addstr("It was nothing special, though.");
			else if(segmentpower<32)addstr("It went pretty well.");
			else if(segmentpower<40)addstr("The discussion was exciting and dynamic.");
			else if(segmentpower<50)addstr("It was almost perfect.");
			else
				{
				addstr(repname);
				addstr(" later went on to win a Pulitzer for it.");
				move(9,1);
				addstr("Virtually everyone in America was moved by ");
				addstr(pool[best]->name);
				addstr("'s words.");
				}
			refresh();
			getch();

			//CHECK PUBLIC OPINION
			change_public_opinion(VIEW_LIBERALCRIMESQUAD,20,0,0);
			change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,(segmentpower-25)/2,0,0);
			int viewhit;
			for(int v=0;v<5;v++)
				{
				do
					{
					viewhit=LCSrandom(VIEWNUM);
					}while(viewhit==VIEW_LIBERALCRIMESQUADPOS);
				if(viewhit!=VIEW_LIBERALCRIMESQUAD)change_public_opinion(viewhit,(segmentpower-25)/2,1,0);
				else change_public_opinion(viewhit,segmentpower/4,1,0);
				}
			}
		}
			}
		}
}

void giveup(void)
{
	int loc=-1;
	if(selectedsiege!=-1)loc=selectedsiege;
	if(activesquad!=NULL)loc=activesquad->squad[0]->location;
	if(loc==-1)return;

	if(location[loc]->renting>1)location[loc]->renting=-1;

	//IF POLICE, END SIEGE
	if(location[loc]->siege.siegetype==SIEGE_POLICE)
		{
		int polsta=-1;
		for(int l=0;l<location.size();l++)
			{
			if(location[l]->type==SITE_GOVERNMENT_POLICESTATION)
				{
				polsta=l;
				break;
				}
			}

		//END SIEGE
		erase();
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(1,1);
		addstr("The police confiscate everything, including Squad weapons.");

		int kcount=0;
		int pcount=0;
		char kname[100];
		char pname[100];
		char pcname[100];
		int icount=0;
		int p;
		for(p=pool.size()-1;p>=0;p--)
			{
			if(pool[p]->location!=loc)continue;
			if(!pool[p]->alive)continue;

			if(pool[p]->flag & CREATUREFLAG_ILLEGALALIEN)icount++;

			if(pool[p]->flag & CREATUREFLAG_MISSING)
				{
				kcount++;
				strcpy(kname,pool[p]->propername);
				if(pool[p]->type==CREATURE_RADIOPERSONALITY)offended_amradio=1;
				if(pool[p]->type==CREATURE_NEWSANCHOR)offended_cablenews=1;
				}
			}

		//CRIMINALIZE POOL IF FOUND WITH KIDNAP VICTIM OR ALIEN
		if(kcount>0)criminalizepool(LAWFLAG_KIDNAPPER,-1,0,loc);
		if(icount>0)criminalizepool(LAWFLAG2_HIREILLEGAL,-1,1,loc);

		//LOOK FOR PRISONERS (MUST BE AFTER CRIMINALIZATION ABOVE)
		for(p=pool.size()-1;p>=0;p--)
			{
			if(pool[p]->location!=loc)continue;
			if(!pool[p]->alive)continue;

			if((pool[p]->lawflag!=0||
				pool[p]->lawflag2!=0)&&!(pool[p]->flag & CREATUREFLAG_MISSING))
				{
				pcount++;
				strcpy(pname,pool[p]->propername);
				strcpy(pcname,pool[p]->name);
				}
			}

		if(kcount==1)
			{
			move(3,1);
			addstr(kname);
			addstr(" is rehabilitated and freed.");
			}
		if(kcount>1)
			{
			move(3,1);
			addstr("The kidnap victims are rehabilitated and freed.");
			}
		if(pcount==1)
			{
			move(5,1);
			addstr(pname);
			addstr(", aka ");
			addstr(pcname);
			addstr(",");
			move(6,1);
			addstr("is taken to the police station.");
			}
		if(pcount>1)
			{
			char num[20];
			move(5,1);
			itoa(pcount,num,10);
			addstr(num);
			addstr(" Liberals are taken to the police station.");
			}
		if(funds>0)
			{
			move(8,1);
			addstr("Fortunately, your funds remain intact.");
			}
		if(location[loc]->compound_walls)
			{
			move(10,1);
			addstr("The compound is dismantled.");
			location[loc]->compound_walls=0;
			}
		if(location[loc]->front_business!=-1)
			{
			move(12,1);
			addstr("Materials relating to the business front have been taken.");
			location[loc]->front_business=-1;
			}

		refresh();
		getch();

		location[loc]->siege.siege=0;

		char clearformess=1;

		for(p=pool.size()-1;p>=0;p--)
			{
			if(pool[p]->location!=loc)continue;

			//ALL KIDNAP VICTIMS FREED REGARDLESS OF CRIMES
			if((pool[p]->flag & CREATUREFLAG_MISSING)||
				!pool[p]->alive)
				{
				removesquadinfo(*pool[p]);
				delete pool[p];
				pool.erase(pool.begin() + p);
				continue;
				}

			//TAKE SQUAD EQUIPMENT
			if(pool[p]->squadid!=-1)
				{
				long sq=getsquad(pool[p]->squadid);
				if(sq!=-1)
					{
					for(int l=0;l<squad[sq]->loot.size();l++)delete squad[sq]->loot[l];
					squad[sq]->loot.clear();
					}
				}

			pool[p]->weapon.type=WEAPON_NONE;
			pool[p]->weapon.ammo=0;
			if(pool[p]->lawflag!=0||
				pool[p]->lawflag2!=0)
				{
				removesquadinfo(*pool[p]);
				pool[p]->location=polsta;
				pool[p]->activity.type=ACTIVITY_NONE;
				}
			}
		}
	else
		{
		//OTHERWISE IT IS SUICIDE
		int killnumber=0;
		for(int p=pool.size()-1;p>=0;p--)
			{
			if(pool[p]->location!=loc)continue;

			if(pool[p]->alive&&pool[p]->align==1)stat_dead++;

			killnumber++;
			removesquadinfo(*pool[p]);
			delete pool[p];
			pool.erase(pool.begin() + p);
			}

		erase();
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(1,1);
		addstr("Everyone in the ");
		addlocationname(location[loc]);
		addstr(" is slain.");
		refresh();
		getch();

		newsstoryst *ns=new newsstoryst;
			ns->type=NEWSSTORY_MASSACRE;
			ns->loc=loc;
			ns->crime.push_back(location[loc]->siege.siegetype);
			ns->crime.push_back(killnumber);
		newsstory.push_back(ns);

		//MUST SET cursite TO SATISFY endcheck() CODE
		cursite=loc;
		endcheck();
		cursite=-1;

		location[loc]->siege.siege=0;
		}

	//CONFISCATE MATERIAL
	for(int l=0;l<location[loc]->loot.size();l++)delete location[loc]->loot[l];
	location[loc]->loot.clear();
	for(int v=(int)vehicle.size()-1;v>=0;v--)
		{
		if(vehicle[v]->location==loc)
			{
			removecarprefs_pool(vehicle[v]->id);
			delete vehicle[v];
			vehicle.erase(vehicle.begin() + v);
			}
		}
}

int fooddaysleft(int loc)
{
	int eaters=numbereating(loc);

	if(eaters==0)return -1;

	int days=location[loc]->compound_stores/eaters;
	if((location[loc]->compound_stores%eaters)>eaters/2)days++;
	return days;
}

int numbereating(int loc)
{
	int eaters=0;

	for(int p=0;p<pool.size();p++)
		{
		if(pool[p]->location!=loc)continue;
		if(pool[p]->alive)eaters++;
		}

	return eaters;
}

void escape_engage(void)
{
	//GIVE INFO SCREEN
	erase();
	set_color(COLOR_RED,COLOR_BLACK,1);
	move(1,26);
	addstr("UNDER SIEGE: ESCAPE OR ENGAGE");

	set_color(COLOR_WHITE,COLOR_BLACK,0);
	move(3,16);
	addstr("You are about to engage Conservative forces in battle.");
	move(4,11);
	addstr("You will find yourself in the Command Center, and it will be");
	move(5,11);
	addstr("swarming with Conservative units.  The Liberal Crime Squad");
	move(6,11);
	addstr("will be located far from the entrance to the LCC.  It is");
	move(7,11);
	addstr("your task to bring your squad out to safety, or fight off");
	move(8,11);
	addstr("the Conservatives within the perimeter.  Either way you");
	move(9,11);
	addstr("choose, any equipment from the LCC which isn't held by a");
	move(10,11);
	addstr("Liberal will be scattered about the compound.  Save what");
	move(11,11);
	addstr("you can.  You might notice your Squad has filled out to");
	move(12,11);
	addstr("six members if any were available.  If you have a larger pool");
	move(13,11);
	addstr("of Liberals, they will be traveling behind the Squad.");
	move(14,11);
	addstr("There is a new button, (R)eorganize, which reflects this.");
	move(15,11);
	addstr("Squad members in the back with firearms can provide cover");
	move(16,11);
	addstr("fire.  If you have at least six people total, then six must");
	move(17,11);
	addstr("be in the Squad.  If less than six, then they all must.");
	
	int loc=-1;
	if(selectedsiege!=-1)loc=selectedsiege;
	if(activesquad!=NULL)loc=activesquad->squad[0]->location;
	if(loc==-1)return;

	if(location[loc]->compound_walls & COMPOUND_CAMERAS)
		{
		move(18,16);
		addstr("Your security cameras let you see units on the (M)ap.");
		}
	if(location[loc]->compound_walls & COMPOUND_TRAPS)
		{
		move(19,16);
		addstr("Your traps will harass the enemy, but not the Squad.");
		}

	set_color(COLOR_RED,COLOR_BLACK,1);
	move(23,11);
	addstr("Press any key to Confront the Conservative Aggressors");

	refresh();
	getch();

	//CRIMINALIZE
	if(location[loc]->siege.siegetype==SIEGE_POLICE)criminalizepool(LAWFLAG_RESIST,-1,0,loc);

	//DELETE ALL SQUADS IN THIS AREA UNLESS THEY ARE THE activesquad
	for(int sq=squad.size()-1;sq>=0;sq--)
		{
		if(squad[sq]==activesquad)continue;
		if(squad[sq]->squad[0]!=NULL)
			{
			if(squad[sq]->squad[0]->location==loc)
				{
				for(int p=0;p<6;p++)
					{
					if(squad[sq]->squad[p]==NULL)continue;
					squad[sq]->squad[p]->squadid=-1;
					}
				delete squad[sq];
				squad.erase(squad.begin() + sq);
				}
			}
		}

	//MAKE SURE PARTY IS ORGANIZED
	autopromote(loc);

	//START FIGHTING
	newsstoryst *ns=new newsstoryst;
		if(location[loc]->siege.underattack)ns->type=NEWSSTORY_SQUAD_FLEDATTACK;
		else ns->type=NEWSSTORY_SQUAD_ESCAPED;
		ns->positive=1;
		ns->loc=loc;
		ns->siegetype=location[loc]->siege.siegetype;
	newsstory.push_back(ns);
	mode_site(loc);
}

void autopromote(int loc)
{
	if(activesquad==NULL)return;

	int partysize=0;
	int partydead=0;
	int p;
	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)partysize++;
		else continue;

		if(!activesquad->squad[p]->alive)partydead++;
		}

	int libnum=0;
	for(int pl=0;pl<pool.size();pl++)
		{
		if(pool[pl]->location!=loc)continue;
		if(pool[pl]->alive&&pool[pl]->align==1)libnum++;
		}

	if(partysize==6&&partydead==0)return;
	if(partysize==libnum)return;

	char conf;
	for(p=0;p<6;p++)
		{
		conf=0;
		if(activesquad->squad[p]==NULL)conf=1;
		else if(!activesquad->squad[p]->alive)conf=1;

		if(conf)
			{
			for(int pl=0;pl<pool.size();pl++)
				{
				if(pool[pl]->location!=loc)continue;
				if(pool[pl]->alive&&pool[pl]->squadid==-1&&
					pool[pl]->align==1)
					{
					if(activesquad->squad[p]!=NULL)activesquad->squad[p]->squadid=-1;
					activesquad->squad[p]=pool[pl];
					activesquad->squad[p]->squadid=activesquad->id;
					break;
					}
				}
			}
		}
}

void escapesiege(char won)
{
	//TEXT IF DIDN'T WIN
	if(!won)
		{
		//GIVE INFO SCREEN
		erase();
		set_color(COLOR_YELLOW,COLOR_BLACK,1);
		move(1,32);
		addstr("You have escaped!");

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(3,16);
		addstr("The Conservatives thought that the Liberal Crime Squad was");
		move(4,11);
		addstr("finished, but once again, Conservative Thinking has proven");
		move(5,11);
		addstr("itself to be based on Unsound Notions.");
		move(6,16);
		addstr("However, all is not well.  In your haste to escape you have");
		move(7,11);
		addstr("lost everything that you've left behind.  You'll have");
		move(8,11);
		addstr("to start from scratch in a new safe house.  Your");
		move(9,11);
		addstr("funds remain under your control, fortunately.  Your flight has");
		move(10,11);
		addstr("given you some time to regroup, but the Conservatives will");
		move(11,11);
		addstr("doubtless be preparing another assault.");

		int homes=-1;
		for(int l=0;l<location.size();l++)
			{
			if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
				{
				homes=l;
				}
			}

		set_color(COLOR_YELLOW,COLOR_BLACK,1);
		move(13,11);
		addstr("Press any key to split up and lay low for a few days");

		refresh();
		getch();

		//GET RID OF DEAD, etc.
		if(location[cursite]->renting>1)location[cursite]->renting=-1;

		for(int p=pool.size()-1;p>=0;p--)
			{
			if(pool[p]->location!=cursite)continue;
			if(!pool[p]->alive)
				{
				delete pool[p];
				pool.erase(pool.begin() + p);
				continue;
				}

			//BASE EVERYONE LEFT AT HOMELESS SHELTER
			removesquadinfo(*pool[p]);
			pool[p]->hiding=LCSrandom(3)+2;
			pool[p]->location=-1;
			pool[p]->base=homes;
			}
		for(int l2=0;l2<location[cursite]->loot.size();l2++)
			{
			delete location[cursite]->loot[l2];
			}
		location[cursite]->loot.clear();

		for(int v=(int)vehicle.size()-1;v>=0;v--)
			{
			if(vehicle[v]->location==cursite)
				{
				removecarprefs_pool(vehicle[v]->id);
				delete vehicle[v];
				vehicle.erase(vehicle.begin() + v);
				}
			}

		location[cursite]->compound_walls=0;
		location[cursite]->compound_stores=0;
		location[cursite]->front_business=-1;
		initlocation(*location[cursite]);
		}

	//SET UP NEW SIEGE CHARACTERISTICS, INCLUDING TIMING
	location[cursite]->siege.siege=0;
	if(won)
		{
		if(location[cursite]->siege.siegetype==SIEGE_POLICE)
			{
			location[cursite]->siege.timeuntillocated=LCSrandom(4)+4;
			location[cursite]->siege.escalationstate++;
			}
		}
}

char addsiegeencounter(char type)
{
	int num;
	int freeslots=0;
	for(int e=0;e<ENCMAX;e++)
		{
		if(!encounter[e].exists)freeslots++;
		}

	switch(type)
		{
		case SIEGEFLAG_UNIT:
		case SIEGEFLAG_UNIT_DAMAGED:
			{
			if(freeslots<6)return 0;

			num=LCSrandom(3)+4;

			for(int e=0;e<ENCMAX;e++)
				{
				if(encounter[e].exists)continue;

				switch(location[cursite]->siege.siegetype)
					{
					case SIEGE_POLICE:
						if(location[cursite]->siege.escalationstate==0||LCSrandom(2))
							{
							if(law[LAW_DEATHPENALTY]==-2&&
							   law[LAW_POLICEBEHAVIOR]==-2)makecreature(encounter[e],CREATURE_DEATHSQUAD);
							else if(law[LAW_POLICEBEHAVIOR]<=-1)makecreature(encounter[e],CREATURE_GANGUNIT);
							else makecreature(encounter[e],CREATURE_COP);
							}
						else makecreature(encounter[e],CREATURE_SOLDIER);
						break;
					case SIEGE_CIA:
						makecreature(encounter[e],CREATURE_AGENT);
						break;
					case SIEGE_HICKS:
						makecreature(encounter[e],CREATURE_HICK);
						break;
					case SIEGE_CORPORATE:
						makecreature(encounter[e],CREATURE_MERC);
						break;
					}

				if(type==SIEGEFLAG_UNIT_DAMAGED)
					{
					encounter[e].blood=LCSrandom(50)+1;
					}

				num--;
				if(num==0)break;
				}
			break;
			}
		case SIEGEFLAG_HEAVYUNIT:
			{
			if(freeslots<1)return 0;

			num=1;

			for(int e=0;e<ENCMAX;e++)
				{
				if(encounter[e].exists)continue;

				makecreature(encounter[e],CREATURE_TANK);

				num--;
				if(num==0)break;
				}
			break;
			}
		}

	return 1;
}

void conquertext(void)
{
	//GIVE INFO SCREEN
	erase();
	set_color(COLOR_GREEN,COLOR_BLACK,1);
	move(1,26);
	addstr("* * * * *  VICTORY  * * * * *");

	if(location[cursite]->siege.siegetype==SIEGE_POLICE)
		{
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(3,16);
		addstr("The Conservative automatons have been driven back -- for");
		move(4,11);
		addstr("the time being.  While they are regrouping, you might consider");
		move(5,11);
		addstr("abandoning this safe house for a safer location.");
		}
	else
		{
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(3,16);
		addstr("The Conservative automatons have been driven back.");
		move(4,11);
		addstr("Unfortunately, you will never truely be safe from");
		move(5,11);
		addstr("this filth until the Liberal Agenda is realized.");
		}

	move(7,19);
	addstr("Press C to Continue Liberally.");

	do
		{
		refresh();
		int c=getch();
		translategetch(c);

		if(c=='c')break;
		}while(1);
}

void criminalizeparty(short crime,char flag2)
{
	if(activesquad==NULL)return;
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(!activesquad->squad[p]->alive)continue;
			if(flag2)activesquad->squad[p]->lawflag2|=crime;
			else activesquad->squad[p]->lawflag|=crime;
			}
		}
}

void criminalizepool(short crime,long exclude,char flag2,short loc)
{
	for(int p=0;p<pool.size();p++)
		{
		if(p==exclude)continue;
		if(loc!=-1&&pool[p]->location!=loc)continue;

		if(flag2)pool[p]->lawflag2|=crime;
		else pool[p]->lawflag|=crime;
		}
}

void statebrokenlaws(int loc)
{
	short breakercount[16];
	short breakercount2[16];
	memset(breakercount,0,sizeof(short)*16);
	memset(breakercount2,0,sizeof(short)*16);
	int typenum=0;
	int criminalcount=0;
	char kname[100];
	int kidnapped=0;

	for(int p=0;p<pool.size();p++)
		{
		if(!pool[p]->alive)continue;
		if(pool[p]->location!=loc)continue;

		if(pool[p]->flag & CREATUREFLAG_KIDNAPPED)
			{
			strcpy(kname,pool[p]->propername);
			kidnapped++;
			}

		if((pool[p]->lawflag!=0||
			pool[p]->lawflag2!=0))criminalcount++;

		if(pool[p]->lawflag & LAWFLAG_KIDNAPPER){breakercount[0]++;typenum++;}
		if(pool[p]->lawflag & LAWFLAG_MURDERER){breakercount[2]++;typenum++;}
		if(pool[p]->lawflag & LAWFLAG_THIEF){breakercount[3]++;typenum++;}
		if(pool[p]->lawflag & LAWFLAG_BREAKING){breakercount[4]++;typenum++;}
		if(pool[p]->lawflag & LAWFLAG_TERRORISM){breakercount[5]++;typenum++;}
		if(pool[p]->lawflag & LAWFLAG_JURY){breakercount[6]++;typenum++;}
		if(pool[p]->lawflag & LAWFLAG_TREASON){breakercount[7]++;typenum++;}
		if(pool[p]->lawflag & LAWFLAG_ESCAPED){breakercount[8]++;typenum++;}
		if(pool[p]->lawflag & LAWFLAG_HELPESCAPE){breakercount[9]++;typenum++;}
		if(pool[p]->lawflag & LAWFLAG_RESIST){breakercount[10]++;typenum++;}
		if(pool[p]->lawflag & LAWFLAG_BURNFLAG){breakercount[11]++;typenum++;}
		if(pool[p]->lawflag & LAWFLAG_SPEECH){breakercount[12]++;typenum++;}
		if(pool[p]->lawflag & LAWFLAG_VANDALISM){breakercount[13]++;typenum++;}
		if(pool[p]->lawflag & LAWFLAG_ASSAULT){breakercount[14]++;typenum++;}
		if(pool[p]->lawflag2 & LAWFLAG2_CARTHEFT){breakercount2[0]++;typenum++;}
		if(pool[p]->lawflag2 & LAWFLAG2_INFORMATION){breakercount2[1]++;typenum++;}
		if(pool[p]->lawflag2 & LAWFLAG2_COMMERCE){breakercount2[2]++;typenum++;}
		if(pool[p]->lawflag2 & LAWFLAG2_CCFRAUD){breakercount2[3]++;typenum++;}
		if(pool[p]->lawflag2 & LAWFLAG2_BROWNIES){breakercount2[4]++;typenum++;}
		if(pool[p]->lawflag2 & LAWFLAG2_BURIAL){breakercount2[5]++;typenum++;}
		if(pool[p]->lawflag2 & LAWFLAG2_PROSTITUTION){breakercount2[6]++;typenum++;}
		if(pool[p]->lawflag2 & LAWFLAG2_DISTURBANCE){breakercount2[7]++;typenum++;}
		if(pool[p]->lawflag2 & LAWFLAG2_HIREILLEGAL){breakercount2[8]++;typenum++;}
		if(pool[p]->lawflag2 & LAWFLAG2_RACKETEERING){breakercount2[9]++;typenum++;}
		if(pool[p]->lawflag2 & LAWFLAG2_LOITERING){breakercount2[10]++;typenum++;}
		}

	erase();

	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(1,1);
	if(location[loc]->siege.underattack)addstr("You hear shouts:");
	else addstr("You hear a blaring voice on a loudspeaker:");

	move(3,1);
	addstr("Surrender yourselves!");

	//KIDNAP VICTIM
	if(kidnapped)
		{
		move(4,1);
		addstr("Release ");
		addstr(kname);
		if(kidnapped>1)addstr(" and the others");
		addstr(" unharmed!");
		}
	//TREASON
	else if(breakercount[7])
		{
		move(4,1);
		addstr("You are wanted for treason");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//MURDERER
	else if(breakercount[2])
		{
		move(4,1);
		addstr("You are wanted for murder");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//TERRORISM
	else if(breakercount[5])
		{
		move(4,1);
		addstr("You are wanted for terrorism");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//KIDNAPPER
	else if(breakercount[0])
		{
		move(4,1);
		addstr("You are wanted for kidnapping");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//BURN FLAG
	else if(breakercount[11])
		{
		move(4,1);
		addstr("You are wanted for flag burning");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//SPEECH
	else if(breakercount[12])
		{
		move(4,1);
		addstr("You are wanted for harmful speech");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//BROWNIES
	else if(breakercount2[4])
		{
		move(4,1);
		addstr("You are wanted for selling brownies");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//ESCAPED
	else if(breakercount[8])
		{
		move(4,1);
		addstr("You are wanted for escaping prison");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//HELP ESCAPED
	else if(breakercount[9])
		{
		move(4,1);
		addstr("You are wanted for aiding a prison escape");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//JURY
	else if(breakercount[6])
		{
		move(4,1);
		addstr("You are wanted for jury tampering");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//RACKETEERING
	else if(breakercount2[9])
		{
		move(4,1);
		addstr("You are wanted for racketeering");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//ASSAULT
	else if(breakercount[14])
		{
		move(4,1);
		addstr("You are wanted for assault");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//CAR THEFT
	else if(breakercount2[0])
		{
		move(4,1);
		addstr("You are wanted for car theft");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//CC FRAUD
	else if(breakercount2[2])
		{
		move(4,1);
		addstr("You are wanted for credit card fraud");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//THIEF
	else if(breakercount[3])
		{
		move(4,1);
		addstr("You are wanted for theft");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//PROSTITUTION
	else if(breakercount2[6])
		{
		move(4,1);
		addstr("You are wanted for prostitution");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//HIRE ILLEGAL
	else if(breakercount2[8])
		{
		move(4,1);
		addstr("You are wanted for hiring an illegal alien");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//COMMERCE
	else if(breakercount2[3])
		{
		move(4,1);
		addstr("You are wanted for intefering with interstate commerce");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//INFORMATION
	else if(breakercount2[1])
		{
		move(4,1);
		addstr("You are wanted for unlawfully accessing an information system");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//BREAKING
	else if(breakercount2[5])
		{
		move(4,1);
		addstr("You are wanted for unlawful burial");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//BREAKING
	else if(breakercount[4])
		{
		move(4,1);
		addstr("You are wanted for breaking and entering");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//VANDALISM
	else if(breakercount[13])
		{
		move(4,1);
		addstr("You are wanted for vandalism");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//RESIST
	else if(breakercount[10])
		{
		move(4,1);
		addstr("You are wanted for resisting arrest");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}
	//DISTURB
	else if(breakercount2[7])
		{
		move(4,1);
		addstr("You are wanted for disturbing the peace");
		if(typenum>1)addstr(" and other crimes");
		addstr(".");
		}

	refresh();
	getch();
}

void trial(creaturest &g)
{
	g.sentence=0;
	g.deathpenalty=0;
	g.location=g.base;

	erase();

	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(1,1);
	addstr(g.name);
	addstr(" is standing trial.");
	refresh();
	getch();

	set_color(COLOR_WHITE,COLOR_BLACK,0);

	if(g.lawflag==0&&g.lawflag2==0)g.lawflag2|=LAWFLAG2_LOITERING;

	short breaker=g.lawflag;
	short breaker2=g.lawflag2;
	int typenum=0;

	if(g.lawflag & LAWFLAG_KIDNAPPER)typenum++;
	if(g.lawflag & LAWFLAG_MURDERER)typenum++;
	if(g.lawflag & LAWFLAG_THIEF)typenum++;
	if(g.lawflag & LAWFLAG_BREAKING)typenum++;
	if(g.lawflag & LAWFLAG_TERRORISM)typenum++;
	if(g.lawflag & LAWFLAG_JURY)typenum++;
	if(g.lawflag & LAWFLAG_TREASON)typenum++;
	if(g.lawflag & LAWFLAG_ESCAPED)typenum++;
	if(g.lawflag & LAWFLAG_HELPESCAPE)typenum++;
	if(g.lawflag & LAWFLAG_RESIST)typenum++;
	if(g.lawflag & LAWFLAG_BURNFLAG)typenum++;
	if(g.lawflag & LAWFLAG_SPEECH)typenum++;
	if(g.lawflag & LAWFLAG_VANDALISM)typenum++;
	if(g.lawflag & LAWFLAG_ASSAULT)typenum++;
	if(g.lawflag2 & LAWFLAG2_CARTHEFT)typenum++;
	if(g.lawflag2 & LAWFLAG2_INFORMATION)typenum++;
	if(g.lawflag2 & LAWFLAG2_COMMERCE)typenum++;
	if(g.lawflag2 & LAWFLAG2_CCFRAUD)typenum++;
	if(g.lawflag2 & LAWFLAG2_BROWNIES)typenum++;
	if(g.lawflag2 & LAWFLAG2_BURIAL)typenum++;
	if(g.lawflag2 & LAWFLAG2_PROSTITUTION)typenum++;
	if(g.lawflag2 & LAWFLAG2_DISTURBANCE)typenum++;
	if(g.lawflag2 & LAWFLAG2_HIREILLEGAL)typenum++;
	if(g.lawflag2 & LAWFLAG2_RACKETEERING)typenum++;
	if(g.lawflag2 & LAWFLAG2_LOITERING)typenum++;

	//CHECK FOR SLEEPERS
	vector<int> sjudge;
	char sleeperlawyer=0;
	char sleeperjudge=0;
	char *sleeperjname=NULL;
	char *sleepername=NULL;
	for(int p=0;p<pool.size();p++)
		{
		if(pool[p]->alive&&(pool[p]->flag & CREATUREFLAG_SLEEPER))
			{
			if(pool[p]->type==CREATURE_JUDGE_CONSERVATIVE||
				pool[p]->type==CREATURE_JUDGE_LIBERAL)
				{
				sjudge.push_back(p);
				}
			if(pool[p]->type==CREATURE_LAWYER&&!sleeperlawyer)
				{
				sleeperlawyer=1;
				sleepername=pool[p]->name;
				}
			}
		}

	if(LCSrandom(10)<sjudge.size())
		{
		sleeperjudge=1;
		sleeperjname=pool[sjudge[LCSrandom(sjudge.size())]]->name;
		}

	//STATE CHARGES
	set_color(COLOR_WHITE,COLOR_BLACK,0);
	move(3,1);
	if(sleeperjudge)
		{
		addstr("Sleeper ");
		addstr(sleeperjname);
		addstr(" reads the charges, trying to hide a smile:");
		}
	else addstr("The judge reads the charges:");
	set_color(COLOR_RED,COLOR_BLACK,1);
	move(5,1);
	addstr("The defendant, ");
	addstr(g.propername);
	addstr(", is charged with ");
	int x=2,y=5;
	while(typenum>0)
		{
		typenum--;

		x++;
		if(x>=3){x=0;y++;move(y,1);}

		if(breaker & LAWFLAG_TREASON)
			{
			addstr("treason");
			breaker&=~LAWFLAG_TREASON;
			}
		else if(breaker & LAWFLAG_MURDERER)
			{
			addstr("murder");
			breaker&=~LAWFLAG_MURDERER;
			}
		else if(breaker & LAWFLAG_TERRORISM)
			{
			addstr("terrorism");
			breaker&=~LAWFLAG_TERRORISM;
			}
		else if(breaker & LAWFLAG_KIDNAPPER)
			{
			addstr("kidnapping");
			breaker&=~LAWFLAG_KIDNAPPER;
			}
		else if(breaker & LAWFLAG_BURNFLAG)
			{
			if(law[LAW_FLAGBURNING]==-2)addstr("Flag Murder");
			else if(law[LAW_FLAGBURNING]==-1)addstr("felony flag burning");
			else if(law[LAW_FLAGBURNING]==0)addstr("flag burning");
			breaker&=~LAWFLAG_BURNFLAG;
			}
		else if(breaker & LAWFLAG_SPEECH)
			{
			addstr("harmful speech");
			breaker&=~LAWFLAG_SPEECH;
			}
		else if(breaker2 & LAWFLAG2_BROWNIES)
			{
			addstr("selling brownies");
			breaker2&=~LAWFLAG2_BROWNIES;
			}
		else if(breaker & LAWFLAG_ESCAPED)
			{
			addstr("escaping prison");
			breaker&=~LAWFLAG_ESCAPED;
			}
		else if(breaker & LAWFLAG_HELPESCAPE)
			{
			addstr("aiding a prison escape");
			breaker&=~LAWFLAG_HELPESCAPE;
			}
		else if(breaker & LAWFLAG_JURY)
			{
			addstr("jury tampering");
			breaker&=~LAWFLAG_JURY;
			}
		else if(breaker2 & LAWFLAG2_RACKETEERING)
			{
			addstr("racketeering");
			breaker2&=~LAWFLAG2_RACKETEERING;
			}
		else if(breaker & LAWFLAG_ASSAULT)
			{
			addstr("assault");
			breaker&=~LAWFLAG_ASSAULT;
			}
		else if(breaker2 & LAWFLAG2_CARTHEFT)
			{
			addstr("car theft");
			breaker2&=~LAWFLAG2_CARTHEFT;
			}
		else if(breaker2 & LAWFLAG2_CCFRAUD)
			{
			addstr("credit card fraud");
			breaker2&=~LAWFLAG2_CCFRAUD;
			}
		else if(breaker & LAWFLAG_THIEF)
			{
			addstr("theft");
			breaker&=~LAWFLAG_THIEF;
			}
		else if(breaker2 & LAWFLAG2_PROSTITUTION)
			{
			addstr("prostitution");
			breaker2&=~LAWFLAG2_PROSTITUTION;
			}
		else if(breaker2 & LAWFLAG2_HIREILLEGAL)
			{
			addstr("hiring an illegal alien");
			breaker2&=~LAWFLAG2_HIREILLEGAL;
			}
		else if(breaker2 & LAWFLAG2_COMMERCE)
			{
			if(x>=1){x=1;y++;move(y,1);}
			addstr("interference with interstate commerce");
			breaker2&=~LAWFLAG2_COMMERCE;
			}
		else if(breaker2 & LAWFLAG2_INFORMATION)
			{
			if(x>=1){x=1;y++;move(y,1);}
			addstr("unlawful access of an information system");
			breaker2&=~LAWFLAG2_INFORMATION;
			}
		else if(breaker2 & LAWFLAG2_BURIAL)
			{
			addstr("unlawful burial");
			breaker2&=~LAWFLAG2_BURIAL;
			}
		else if(breaker & LAWFLAG_BREAKING)
			{
			addstr("breaking and entering");
			breaker&=~LAWFLAG_BREAKING;
			}
		else if(breaker & LAWFLAG_VANDALISM)
			{
			addstr("vandalism");
			breaker&=~LAWFLAG_VANDALISM;
			}
		else if(breaker & LAWFLAG_RESIST)
			{
			addstr("resisting arrest");
			breaker&=~LAWFLAG_RESIST;
			}
		else if(breaker2 & LAWFLAG2_DISTURBANCE)
			{
			addstr("disturbing the peace");
			breaker2&=~LAWFLAG2_DISTURBANCE;
			}
		else if(breaker2 & LAWFLAG2_LOITERING)
			{
			addstr("loitering");
			breaker2&=~LAWFLAG2_LOITERING;
			}

		if(typenum>1)addstr(", ");
		if(typenum==1)addstr(" and ");
		if(typenum==0)addstr(".");

		refresh();
		getch();
		}

	//CHOOSE DEFENSE
	set_color(COLOR_WHITE,COLOR_BLACK,0);
	move(y+2,1);
	addstr("How will you conduct the defense?");

	char attorneyname[200];
	unsigned long oldseed=seed;
	seed=attorneyseed;
	name(attorneyname);
	seed=oldseed;

	y+=4;
	move(y,1);y++;
	addstr("A - Use a court-appointed attorney.");
	move(y,1);y++;
	addstr("B - Defend self!");
	move(y,1);y++;
	addstr("C - Plead guilty.");
	if(sleeperlawyer)
		{
		move(y,1);y++;
		addstr("D - Use Sleeper ");
		addstr(sleepername);
		addstr(" as your attorney.");
		}
	else if(funds>=1000)
		{
		move(y,1);y++;
		addstr("D - Use $1000 to hire Liberal attorney ");
		addstr(attorneyname);
		addstr(".");
		}

	short defense;

	int c;
	do
		{
		refresh();
		c=getch();
		translategetch(c);
		if(c=='a'){defense=0;break;}
		if(c=='b'){defense=1;break;}
		if(c=='c'){defense=2;break;}
		if(c=='d'&&sleeperlawyer)
			{
			defense=3;
			break;
			}
		else if(c=='d'&&funds>=1000)
			{
			funds-=1000;
			stat_spent+=1000;
			defense=3;
			moneylost_legal+=1000;
			break;
			}
		}while(1);

	//TRIAL
	if(defense!=2)
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(1,1);
		addstr(g.name);
		addstr(" is standing trial.");

		//TRIAL MESSAGE
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(3,1);
		addstr("The trial proceeds.  Jury selection is first.");
		refresh();
		getch();

		//JURY MAKEUP MESSAGE
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(5,1);
		int jury=LCSrandom(101)-50;
		if(sleeperjudge)jury-=50;
		if(jury<=-40)
			{
			switch(LCSrandom(4))
				{
				case 0:addstr(g.name);addstr("'s best friend from childhood is a juror.");break;
				case 1:addstr("The jury is Flaming Liberal.");break;
				case 2:addstr("Four of the jurors are closet Socialists.");break;
				case 3:addstr("One of the jurors flashes a SECRET LIBERAL HAND SIGNAL when no one is looking.");break;
				}
			}
		else if(jury<=-20)addstr("The jury is fairly Liberal.");
		else if(jury<20)addstr("The jury is quite moderate.");
		else if(jury<40)addstr("The jury is a bit Conservative.");
		else
			{
			switch(LCSrandom(4))
				{
				case 0:addstr("Such a collection of Conservative jurors has never before been assembled.");break;
				case 1:addstr("One of the accepted jurors is a Conservative activist.");break;
				case 2:addstr("Three of the jurors are members of the KKK.");break;
				case 3:addstr("The jury is frighteningly Conservative.");break;
				}
			}
		refresh();
		getch();

		//PROSECUTION MESSAGE
		int prosecution=LCSrandom(100);
		if(sleeperjudge)prosecution>>=1;

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(7,1);

		if(prosecution<=15)addstr("The prosecution's presentation is terrible.");
		else if(prosecution<=25)addstr("The prosecution's arguments are shaky.");
		else if(prosecution<=50)addstr("The prosecution gives a standard presentation.");
		else if(prosecution<=75)addstr("The prosecution's case is solid.");
		else addstr("The prosecution makes an airtight case.");

		refresh();
		getch();

		jury+=LCSrandom(prosecution/2+1)+prosecution/2;

		//DEFENSE MESSAGE
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(9,1);

		int defensepower=0;
		if(defense==0||defense==3)
			{
			if(defense==0)defensepower=LCSrandom(51);
			else
				{
				defensepower=LCSrandom(101)+50;
				}

			if(defensepower<=15)addstr("The defense attorney accidentally said \"My client is GUILTY!\" during closing.");
			else if(defensepower<=25)addstr("The defense is totally lame.");
			else if(defensepower<=50)addstr("The defense was lackluster.");
			else if(defensepower<=75)addstr("Defense arguments were pretty good.");
			else if(defensepower<=100)addstr("The defense was really slick.");
			else if(defensepower<=125)addstr("The defense made the prosecution look like amateurs.");
			else
				{
				addstr(attorneyname);
				addstr("'s arguments made several of the jurors stand up");
				move(10,1);
				addstr("and shout \"NOT GUILTY!\" before deliberations even began.");
				}
			}
		if(defense==1)
			{
			defensepower+=g.attval(ATTRIBUTE_INTELLIGENCE);
			defensepower+=g.attval(ATTRIBUTE_HEART);
			defensepower+=g.attval(ATTRIBUTE_CHARISMA)*2;
			defensepower+=LCSrandom(g.skill[SKILL_PERSUASION]*7+1);
			defensepower+=LCSrandom(g.skill[SKILL_LAW]*7+1);
			g.skill_ip[SKILL_PERSUASION]+=50;
			g.skill_ip[SKILL_LAW]+=50;

			addstr(g.name);
			if(defensepower<=15)addstr("'s defense looks like Colin Ferguson's.");
			else if(defensepower<=25)addstr("'s case really sucked.");
			else if(defensepower<=50)addstr(" did all right, but made some mistakes.");
			else if(defensepower<=75)addstr("'s arguments were pretty good.");
			else if(defensepower<=100)addstr(" worked the jury very well.");
			else if(defensepower<=125)addstr(" made a very powerful case.");
			else
				{
				addstr(" had the jurors crying for freedom.");
				}
			}

		refresh();
		getch();

		//DELIBERATION MESSAGE
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(12,1);
		addstr("The jury leaves to consider the case.");
		refresh();
		getch();

		//JURY RETURN MESSAGE
		erase();
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(1,1);
		addstr("The jury has returned from deliberations.");
		refresh();
		getch();

		//HUNG JURY
		if(defensepower==jury)
			{
			set_color(COLOR_YELLOW,COLOR_BLACK,1);
			move(3,1);
			addstr("But they can't reach a verdict!");
			refresh();
			getch();

			//RE-TRY
			if(LCSrandom(2))
				{
				set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(5,1);
				addstr("The case will be re-tried next month.");
				refresh();
				getch();

				long ps=-1;
				for(long l=0;l<location.size();l++)
					{
					if(location[l]->type==SITE_GOVERNMENT_COURTHOUSE)
						{
						ps=l;
						}
					}
				g.location=ps;
				}
			//NO RE-TRY
			else
				{
				set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(5,1);
				addstr("The prosecution declines to re-try the case.");
				set_color(COLOR_GREEN,COLOR_BLACK,1);
				move(7,1);
				addstr(g.name);
				addstr(" is free!");
				refresh();
				getch();
				}
			}
		//ACQUITTAL!
		if(defensepower>jury)
			{
			set_color(COLOR_GREEN,COLOR_BLACK,1);
			move(3,1);
			addstr("NOT GUILTY!");
			refresh();
			getch();
			set_color(COLOR_GREEN,COLOR_BLACK,1);
			move(5,1);
			addstr(g.name);
			addstr(" is free!");
			refresh();
			getch();
			}
		//LENIENCE
		else if(defensepower/3>=jury/4)
			{
			penalize(g,1);
			}
		else
			{
			penalize(g,0);
			}
		//CLEAN UP LAW FLAGS
		g.lawflag=0;
		g.lawflag2=0;
		//PLACE PRISONER
		if(g.sentence!=0)
			{
			imprison(g);
			}
		else
			{
			g.armor.type=ARMOR_CLOTHES;
			g.armor.quality='1';
			g.armor.flag=0;
			}
		}
	//GUILTY PLEA
	else
		{
		erase();
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(1,1);
		addstr("The court accepts the plea.");
		refresh();
		getch();

		penalize(g,LCSrandom(2));
		//CLEAN UP LAW FLAGS
		g.lawflag=0;
		g.lawflag2=0;
		//PLACE PRISONER
		if(g.sentence!=0)
			{
			imprison(g);
			}
		else
			{
			g.armor.type=ARMOR_CLOTHES;
			g.armor.quality='1';
			g.armor.flag=0;
			}
		}
}

//RETURNS IF SCREEN WAS ERASED
char prison(creaturest &g)
{
	char showed=0;

	if(g.sentence>0)
		{
		//COMMUTE DEATH IN RIGHT CLIMATE
		if(g.deathpenalty&&law[LAW_DEATHPENALTY]==2)
			{
			erase();
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(8,1);
			addstr(g.name);
			addstr("'s death sentence has been commuted to life,");
			move(9,1);
			addstr("due to the abolition of the death penalty.");
			refresh();
			getch();

			g.sentence=-1;
			g.deathpenalty=0;
			return 1;
			}

		//ADVANCE SENTENCE
		g.sentence--;
		if(g.sentence==0)
			{
			//EXECUTE
			if(g.deathpenalty)
				{
				erase();
				set_color(COLOR_RED,COLOR_BLACK,1);
				move(8,1);
				addstr("FOR SHAME:");
				move(9,1);
				addstr("Today, the Conservative Machine executed ");
				addstr(g.name);
				move(10,1);
				addstr("by ");
				if(law[LAW_DEATHPENALTY]==-2)
					{
					switch(LCSrandom(23))
						{
						case 0:addstr("beheading");break;
						case 1:addstr("drawing and quartering");break;
						case 2:addstr("disemboweling");break;
						case 3:addstr("one thousand cuts");break;
						case 4:addstr("feeding the lions");break;
						case 5:addstr("repeated gladiatory death matches");break;
						case 6:addstr("burning");break;
						case 7:addstr("crucifixion");break;
						case 8:addstr("head-squishing");break;
						case 9:addstr("piranha tank swimming exhibition");break;
						case 10:addstr("forced sucking of Ronald Reagan's ass");break;
						case 11:addstr("covering with peanut butter and letting rats eat");break;
						case 12:addstr("burying up to the neck in a fire ant nest");break;
						case 13:addstr("running truck over the head");break;
						case 14:addstr("drowning in a sewage digestor vat");break;
						case 15:addstr("chipper-shredder");break;
						case 16:addstr("use in lab research");break;
						case 17:addstr("blood draining");break;
						case 18:addstr("chemical weapons test");break;
						case 19:addstr("sale to a furniture maker");break;
						case 20:addstr("sale to a CEO as a personal pleasure toy");break;
						case 21:addstr("sale to foreign slave traders");break;
						case 22:addstr("exposure to degenerate Bay 12 Curses games");break;
						}
					}
				else if(law[LAW_DEATHPENALTY]==-1||law[LAW_DEATHPENALTY]==0)
					{
					switch(LCSrandom(4))
						{
						case 0:addstr("lethal injection");break;
						case 1:addstr("hanging");break;
						case 2:addstr("firing squad");break;
						case 3:addstr("electrocution");break;
						}
					}
				else
					{
					addstr("lethal injection");
					}
				addstr(".");
				refresh();
				getch();

				g.alive=0;
				stat_dead++;
				showed=1;
				}
			//SET FREE
			else
				{
				erase();
				set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(8,1);
				addstr(g.name);
				addstr(" has been released from prison.");
				move(9,1);
				addstr("No doubt there are some mental scars, but the Liberal is back.");
				refresh();
				getch();

				g.armor.type=ARMOR_CLOTHES;
				g.armor.quality='1';
				g.armor.flag=0;
				g.location=g.base;
				showed=1;
				}
			}
		//NOTIFY OF IMPENDING THINGS
		if(g.sentence==1)
			{
			if(g.deathpenalty)
				{
				erase();
				set_color(COLOR_YELLOW,COLOR_BLACK,1);
				move(8,1);
				addstr(g.name);
				addstr(" is due to be executed next month.");
				refresh();
				getch();

				showed=1;
				}
			else
				{
				erase();
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(8,1);
				addstr(g.name);
				addstr(" is due to be released next month.");
				refresh();
				getch();

				showed=1;
				}
			}
		}

	return showed;
}

void partyrescue(void)
{
	int freeslots=0;
	int p, pl;
	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]==NULL)freeslots++;
		}
	int hostslots=0;
	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(activesquad->squad[p]->alive&&activesquad->squad[p]->prisoner==NULL)
				{
				hostslots++;
				}
			}
		}

	for(pl=0;pl<pool.size();pl++)
		{
		if(pool[pl]->location==cursite&&
			!(pool[pl]->flag & CREATUREFLAG_SLEEPER))
			{
			if(LCSrandom(2)&&freeslots)
				{
				for(int p=0;p<6;p++)
					{
					if(activesquad->squad[p]==NULL)
						{
						activesquad->squad[p]=pool[pl];
						activesquad->squad[p]->squadid=activesquad->id;
						activesquad->squad[p]->lawflag|=LAWFLAG_ESCAPED;
						activesquad->squad[p]->flag|=CREATUREFLAG_JUSTESCAPED;
						break;
						}
					}
				hostslots++;
				freeslots--;

				clearmessagearea();

				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(16,1);
				addstr("You've rescued ");
				addstr(pool[pl]->name);
				addstr(" from the Conservatives.");

				printparty();
				refresh();
				getch();

				pool[pl]->location=-1;
				pool[pl]->base=activesquad->squad[0]->base;
				}
			}
		}
	for(pl=0;pl<pool.size();pl++)
		{
		if(pool[pl]->location==cursite&&
			!(pool[pl]->flag & CREATUREFLAG_SLEEPER))
			{
			if(hostslots)
				{
				for(int p=0;p<6;p++)
					{
					if(activesquad->squad[p]!=NULL)
						{
						if(activesquad->squad[p]->alive&&activesquad->squad[p]->prisoner==NULL)
							{
							activesquad->squad[p]->prisoner=pool[pl];
							pool[pl]->squadid=activesquad->id;
							pool[pl]->lawflag|=LAWFLAG_ESCAPED;
							pool[pl]->flag|=CREATUREFLAG_JUSTESCAPED;

							clearmessagearea();

							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(16,1);
							addstr("You've rescued ");
							addstr(pool[pl]->name);
							addstr(" from the Conservatives.");

							refresh();
							getch();

							clearmessagearea();
							move(16,1);
							addstr(pool[pl]->name);
							addstr(" ");
							switch(LCSrandom(3))
								{
								case 0:addstr("was tortured recently");break;
								case 1:addstr("was beaten severely yesterday");break;
								case 2:addstr("was on a hunger strike");break;
								}
							move(17,1);
							addstr("so ");
							addstr(activesquad->squad[p]->name);
							addstr(" will have to haul a Liberal.");

							pool[pl]->location=-1;
							pool[pl]->base=activesquad->squad[p]->base;

							printparty();
							refresh();
							getch();
							break;
							}
						}
					}

				hostslots--;
				}
			if(!hostslots)break;
			}
		}

	int stillpcount=0;
	char stillpname[200];
	for(pl=0;pl<pool.size();pl++)
		{
		if(pool[pl]->location==cursite&&
			!(pool[pl]->flag & CREATUREFLAG_SLEEPER))
			{
			stillpcount++;
			if(stillpcount==1)strcpy(stillpname,pool[pl]->name);
			}
		}

	if(stillpcount==1)
		{
		clearmessagearea();
		set_color(COLOR_YELLOW,COLOR_BLACK,1);
		move(16,1);
		addstr("There's nobody left to carry ");
		addstr(stillpname);
		addstr(".");
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(17,1);
		addstr("You'll have to come back later.");
		refresh();
		getch();
		}
	else if(stillpcount>1)
		{
		clearmessagearea();
		set_color(COLOR_YELLOW,COLOR_BLACK,1);
		move(16,1);
		addstr("There's nobody left to carry the others.");
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(17,1);
		addstr("You'll have to come back later.");
		refresh();
		getch();
		}
}

void penalize(creaturest &g,char lenient)
{
	set_color(COLOR_RED,COLOR_BLACK,1);
	move(3,1);
	addstr("GUILTY!");
	refresh();
	getch();

	g.deathpenalty=0;

	if((g.lawflag & LAWFLAG_MURDERER)||(g.lawflag & LAWFLAG_TREASON)||
		((g.lawflag & LAWFLAG_BURNFLAG)&&law[LAW_FLAGBURNING]==-2)||
		law[LAW_DEATHPENALTY]==-2)
		{
		if(law[LAW_DEATHPENALTY]==-2)g.deathpenalty=1;
		if(law[LAW_DEATHPENALTY]==-1)g.deathpenalty=LCSrandom(3);
		if(law[LAW_DEATHPENALTY]==0)g.deathpenalty=LCSrandom(2);
		if(law[LAW_DEATHPENALTY]==1)g.deathpenalty=!LCSrandom(5);
		if(law[LAW_DEATHPENALTY]==2)g.deathpenalty=0;
		}

	//CALC TIME
	if(!g.deathpenalty)
		{
		if((g.lawflag & LAWFLAG_KIDNAPPER)&&g.sentence!=-1)g.sentence+=36+LCSrandom(18);
		if((g.lawflag & LAWFLAG_THIEF)&&g.sentence!=-1)g.sentence+=3+LCSrandom(8);
		if((g.lawflag2 & LAWFLAG2_CARTHEFT)&&g.sentence!=-1)g.sentence+=6+LCSrandom(7);
		if((g.lawflag2 & LAWFLAG2_INFORMATION)&&g.sentence!=-1)g.sentence+=1+LCSrandom(13);
		if((g.lawflag2 & LAWFLAG2_COMMERCE)&&g.sentence!=-1)g.sentence+=1+LCSrandom(13);
		if((g.lawflag2 & LAWFLAG2_CCFRAUD)&&g.sentence!=-1)g.sentence+=6+LCSrandom(25);
		if((g.lawflag2 & LAWFLAG2_BURIAL)&&g.sentence!=-1)g.sentence+=3+LCSrandom(12);
		if((g.lawflag2 & LAWFLAG2_PROSTITUTION)&&g.sentence!=-1)g.sentence+=1+LCSrandom(6);
		if((g.lawflag2 & LAWFLAG2_DISTURBANCE)&&g.sentence!=-1)g.sentence+=1;
		if((g.lawflag2 & LAWFLAG2_LOITERING)&&g.sentence!=-1)g.sentence+=1;
		if((g.lawflag2 & LAWFLAG2_HIREILLEGAL)&&g.sentence!=-1)g.sentence+=1;
		if((g.lawflag2 & LAWFLAG2_RACKETEERING)&&g.sentence!=-1)g.sentence+=6+LCSrandom(100);
		if((g.lawflag2 & LAWFLAG2_BROWNIES)&&g.sentence!=-1)
			{
			if(LCSrandom(3))g.sentence+=3+LCSrandom(12);
			else
				{
				if(LCSrandom(3))g.sentence+=3+LCSrandom(120);
				else g.sentence+=3+LCSrandom(360);
				}
			}
		if((g.lawflag & LAWFLAG_BREAKING)&&g.sentence!=-1)g.sentence+=1;
		if((g.lawflag & LAWFLAG_TERRORISM)&&g.sentence!=-1)g.sentence+=60+LCSrandom(181);
		if((g.lawflag & LAWFLAG_JURY)&&g.sentence!=-1)g.sentence+=30+LCSrandom(61);
		if((g.lawflag & LAWFLAG_ESCAPED)&&g.sentence!=-1)g.sentence=-1;
		if((g.lawflag & LAWFLAG_HELPESCAPE)&&g.sentence!=-1)g.sentence+=30+LCSrandom(61);
		if((g.lawflag & LAWFLAG_RESIST)&&g.sentence!=-1)g.sentence+=1+LCSrandom(1);
		if((g.lawflag & LAWFLAG_BURNFLAG)&&g.sentence!=-1)
			{
			if(law[LAW_FLAGBURNING]==-2)
				{
				if(!LCSrandom(2))g.sentence+=120+LCSrandom(241);
				else g.sentence=-1;
				}
			else if(law[LAW_FLAGBURNING]==-1)g.sentence+=36;
			else if(law[LAW_FLAGBURNING]==0)g.sentence+=1;
			}
		if((g.lawflag & LAWFLAG_SPEECH)&&g.sentence!=-1)g.sentence+=4+LCSrandom(3);
		if((g.lawflag & LAWFLAG_VANDALISM)&&g.sentence!=-1)g.sentence+=1;
		if((g.lawflag & LAWFLAG_ASSAULT)&&g.sentence!=-1)g.sentence+=3+LCSrandom(1);
		if((g.lawflag & LAWFLAG_MURDERER)&&g.sentence!=-1)
			{
			if(!LCSrandom(2))g.sentence+=120+LCSrandom(241);
			else g.sentence=-1;
			}
		if((g.lawflag & LAWFLAG_TREASON)&&g.sentence!=-1)g.sentence=-1;
		if(lenient&&g.sentence!=-1)g.sentence/=2;
		if(lenient&&g.sentence==-1)g.sentence=240+LCSrandom(120);
		}
	//LENIENCY AND CAPITAL PUNISHMENT DON'T MIX
	else if(g.deathpenalty&&lenient)
		{
		g.deathpenalty=0;
		g.sentence=-1;
		}

	//MENTION LENIENCY
	if(lenient)
		{
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(5,1);
		addstr("During sentencing, the judge grants some leniency.");
		refresh();
		getch();
		}

	//MENTION SENTENCE
	if(g.deathpenalty)
		{
		g.sentence=3;
		set_color(COLOR_RED,COLOR_RED,1);
		move(7,1);
		addstr(g.propername);
		addstr(", you are sentenced to DEATH!");
		refresh();
		getch();
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(9,1);
		addstr("The execution is scheduled to occur three months from now.");
		refresh();
		getch();
		}
	else if(g.sentence==0)
		{
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(7,1);
		addstr(g.propername);
		addstr(", consider this a warning.  You are free to go.");
		refresh();
		getch();
		}
	else
		{
		if(g.sentence>=36)g.sentence-=g.sentence%12;

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(7,1);
		addstr(g.propername);
		addstr(", you are sentenced to ");
		if(g.sentence==-1)
			{
			addstr("life in prison.");
			}
		else if(g.sentence>=36)
			{
			char num[20];
			itoa(g.sentence/12,num,10);
			addstr(num);
			addstr(" year");
			if(g.sentence/12>1)addstr("s");
			addstr(" in prison.");
			}
		else
			{
			char num[20];
			itoa(g.sentence,num,10);
			addstr(num);
			addstr(" month");
			if(g.sentence>1)addstr("s");
			addstr(" in prison.");
			}
		refresh();
		getch();
		}
}

void printnews(short l,short newspaper)
{
	erase();
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	switch(l)
		{
		case LOOT_CEOPHOTOS:
			move(6,1);
			addstr("The Liberal Guardian runs a story featuring photos of a major CEO");
			move(7,1);
			switch(LCSrandom(10))
				{
				case 0:
					addstr("engaging in lude behavior with animals.");
					change_public_opinion(VIEW_ANIMALRESEARCH,15,0,0);
					break;
				case 1:addstr("digging up graves and sleeping with the dead.");break;
				case 2:
					addstr("participating in a murder.");
					change_public_opinion(VIEW_POLICEBEHAVIOR,15,0,0);
					change_public_opinion(VIEW_JUSTICES,10,0,0);
					break;
				case 3:addstr("engaging in heavy bondage.  A cucumber was involved in some way.");break;
				case 4:addstr("tongue-kissing an infamous dictator.");break;
				case 5:
					addstr("making out with an FDA official overseeing the CEO's products.");
					change_public_opinion(VIEW_GENETICS,10,0,0);
					change_public_opinion(VIEW_POLLUTION,10,0,0);
					break;
				case 6:addstr("castrating himself.");break;
				case 7:addstr("waving a Nazi flag at a supremacist rally.");break;
				case 8:
					addstr("torturing an employee with a hot iron.");
					change_public_opinion(VIEW_SWEATSHOPS,10,0,0);
					break;
				case 9:addstr("playing with feces and urine.");break;
				}
			move(8,1);
			addstr("The major networks and publications take it up and run it for weeks.");
			move(9,1);
			addstr("This is bound to get the Corporations a little riled up.");

			change_public_opinion(VIEW_LIBERALCRIMESQUAD,newspaper*10,0,0);
			change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,newspaper*10,0,0);
			change_public_opinion(VIEW_CEOSALARY,50,0,0);
			change_public_opinion(VIEW_CORPORATECULTURE,50,0,0);
			offended_corps=1;
			break;
		case LOOT_CORPFILES:
			move(6,1);
			addstr("The Liberal Guardian runs a story featuring Corporate files");
			move(7,1);
			switch(LCSrandom(5))
				{
				case 0:
					addstr("describing a genetic monster created in a lab.");
					change_public_opinion(VIEW_GENETICS,50,0,0);
					break;
				case 1:
					addstr("with a list of gay employees entitled \"Homo-workers\".");
					change_public_opinion(VIEW_GAY,50,0,0);
					break;
				case 2:
					addstr("containing a memo: \"Terminate the pregnancy, I terminate you.\"");
					change_public_opinion(VIEW_ABORTION,50,0,0);
					break;
				case 3:
					addstr("cheerfully describing foreign corporate sweatshops.");
					change_public_opinion(VIEW_SWEATSHOPS,50,0,0);
					break;
				case 4:
					addstr("describing an intricate tax scheme.");
					change_public_opinion(VIEW_TAXES,50,0,0);
					break;
				}
			move(8,1);
			addstr("The major networks and publications take it up and run it for weeks.");
			move(9,1);
			addstr("This is bound to get the Corporations a little riled up.");

			change_public_opinion(VIEW_LIBERALCRIMESQUAD,newspaper*10,0,0);
			change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,newspaper*10,0,0);
			change_public_opinion(VIEW_CEOSALARY,50,0,0);
			change_public_opinion(VIEW_CORPORATECULTURE,50,0,0);
			offended_corps=1;
			break;
		case LOOT_INTHQDISK:
		case LOOT_SECRETDOCUMENTS:
			move(6,1);
			addstr("The Liberal Guardian runs a story featuring CIA and other intelligence files");
			move(7,1);
			switch(LCSrandom(6))
				{
				case 0:addstr("documenting the overthrow of a government.");break;
				case 1:
					addstr("documenting the planned assassination of a Liberal federal judge.");
					change_public_opinion(VIEW_JUSTICES,50,0,0);
					break;
				case 2:addstr("containing private information on innocent citizens.");break;
				case 3:
					addstr("documenting \"harmful speech\" made by innocent citizens.");
					change_public_opinion(VIEW_FREESPEECH,50,0,0);
					break;
				case 4:
					addstr("used to keep tabs on gay citizens.");
					change_public_opinion(VIEW_GAY,50,0,0);
					break;
				case 5:
					addstr("documenting the infiltration of a pro-choice group.");
					change_public_opinion(VIEW_ABORTION,50,0,0);
					break;
				}
			move(8,1);
			addstr("The major networks and publications take it up and run it for weeks.");
			move(9,1);
			addstr("This is bound to get the Government a little riled up.");

			change_public_opinion(VIEW_LIBERALCRIMESQUAD,newspaper*10,0,0);
			change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,newspaper*10,0,0);
			change_public_opinion(VIEW_INTELLIGENCE,50,0,0);
			offended_cia=1;
			break;
		case LOOT_POLICERECORDS:
			move(6,1);
			addstr("The Liberal Guardian runs a story featuring police records");
			move(7,1);
			switch(LCSrandom(6))
				{
				case 0:addstr("documenting human rights abuses by the force.");break;
				case 1:addstr("documenting a police torture case.");break;
				case 2:
					addstr("documenting a systematic invasion of privacy by the force.");
					change_public_opinion(VIEW_INTELLIGENCE,15,0,0);
					break;
				case 3:addstr("documenting a forced confession.");break;
				case 4:addstr("documenting widespread corruption in the force.");break;
				case 5:
					addstr("documenting gladiatory matches held between prisoners by guards.");
					change_public_opinion(VIEW_PRISONS,50,0,0);
					break;
				}
			move(8,1);
			addstr("The major networks and publications take it up and run it for weeks.");
			move(9,1);
			addstr("This is bound to get the police a little riled up.");

			change_public_opinion(VIEW_LIBERALCRIMESQUAD,newspaper*10,0,0);
			change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,newspaper*10,0,0);
			change_public_opinion(VIEW_POLICEBEHAVIOR,50,0,0);
			offended_cops=1;
			break;
		}
	refresh();
	getch();
}

void adddeathmessage(creaturest &cr)
{
	set_color(COLOR_YELLOW,COLOR_BLACK,1);

	move(16,1);
	char str[200];

	if((cr.wound[BODYPART_HEAD] & WOUND_CLEANOFF)||
		(cr.wound[BODYPART_HEAD] & WOUND_NASTYOFF))
		{
		strcpy(str,cr.name);
		switch(LCSrandom(4))
			{
			case 0:
				strcat(str," reaches once where there");
				addstr(str);
				move(17,1);
				if(mode!=GAMEMODE_CHASECAR)
					{
					addstr("is no head, and falls.");
					}
				else addstr("is no head, and slumps over.");
				break;
			case 1:
				if(mode!=GAMEMODE_CHASECAR)
					{
					strcat(str," stands headless for a");
					}
				else strcat(str," sits headless for a");
				addstr(str);
				move(17,1);
				addstr("moment then crumples over.");
				break;
			case 2:
				strcat(str," squirts ");
				if(law[LAW_FREESPEECH]==-2)addstr("[red water]");
				else strcat(str,"blood");
				strcat(str," out of the ");
				addstr(str);
				move(17,1);
				if(mode!=GAMEMODE_CHASECAR)
					{
					addstr("neck and runs down the hall.");
					}
				else addstr("neck and falls to the side.");
				break;
			case 3:
				strcat(str," sucks a last breath through");
				addstr(str);
				move(17,1);
				addstr("the neck hole, then is quiet.");
				break;
			}
		}
	else if((cr.wound[BODYPART_BODY] & WOUND_CLEANOFF)||
		(cr.wound[BODYPART_BODY] & WOUND_NASTYOFF))
		{
		strcpy(str,cr.name);
		switch(LCSrandom(2))
			{
			case 0:strcat(str," falls into pieces.");break;
			case 1:strcat(str," breaks apart and is dead.");break;
			}
		addstr(str);
		}
	else
		{
		strcpy(str,cr.name);
		switch(LCSrandom(10))
			{
			case 0:
				strcat(str," cries out one last time");
				addstr(str);
				move(17,1);
				addstr("then is quiet.");
				break;
			case 1:
				strcat(str," gasps a last breath and");
				addstr(str);
				move(17,1);
				if(law[LAW_FREESPEECH]==-2)addstr("[makes a mess].");
				else addstr("soils the floor.");
				break;
			case 2:
				strcat(str," murmur quietly, breathing softly.");
				addstr(str);
				move(17,1);
				addstr("Then all is silent.");
				break;
			case 3:
				strcat(str," shouts \"FATHER!  Why have you");
				addstr(str);
				move(17,1);
				addstr("forsaken me?\" and dies in a heap.");
				break;
			case 4:
				strcat(str," cries silently for mother,");
				addstr(str);
				move(17,1);
				addstr("breathing slowly, then not at all.");
				break;
			case 5:
				strcat(str," breathes heavily, coughing up");
				addstr(str);
				move(17,1);
				addstr("blood...  then is quiet.");
				break;
			case 6:
				strcat(str," silently drifts away, and");
				addstr(str);
				move(17,1);
				addstr("is gone.");
				break;
			case 7:
				strcat(str," sweats profusely, murmurs");
				addstr(str);
				move(17,1);
				if(law[LAW_FREESPEECH]==-2)addstr("something [good] about Jesus, and dies.");
				else addstr("something about Jesus, and dies.");
				break;
			case 8:
				strcat(str," whines loudly, voice crackling,");
				addstr(str);
				move(17,1);
				addstr("then curls into a ball, unmoving.");
				break;
			case 9:
				strcat(str," shivers silently, whispering");
				addstr(str);
				move(17,1);
				addstr("a prayer, then all is still.");
				break;
			}
		}
}

void fullstatus(int p)
{
	if(activesquad==NULL)return;

	do
		{
		erase();

		set_color(COLOR_GREEN,COLOR_BLACK,1);
		move(0,0);
		addstr("Profile of a Liberal");

		printliberalstats(*activesquad->squad[p],0);

		move(23,0);
		addstr("Press N to change this Liberal's Code Name");
		if(activesquad->squad[1]!=NULL)
			{
			if(interface_pgup=='[')
				{
				addstr(", [] to view the others");
				}
			else
				{
				addstr(", PGUP/PGDN to view the others");
				}
			}
		move(24,0);
		addstr("Press any other key to continue the Struggle");

		refresh();
		int c=getch();
		translategetch(c);

		if(activesquad->squad[1]!=NULL&&(c==interface_pgup||c==interface_pgdn))
			{
			int sx=1;
			if(c==interface_pgup)sx=-1;
			do
				{
				p=(p+6+sx)%6;
				}while(activesquad->squad[p]==NULL);
			continue;
			}

		if(c=='n')
			{
			move(23,0);
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			addstr("What is the new code name?                          ");
			move(24,0);
			addstr("                                                    ");

			keypad(stdscr,FALSE);
			raw_output(FALSE);
			echo();
			curs_set(1);
			mvgetstr(24,0,activesquad->squad[p]->name);
			curs_set(0);
			noecho();
			raw_output(TRUE);
			keypad(stdscr,TRUE);
			}
		else break;
		}while(1);
}

void consolidateloot(vector<itemst *> &loot)
{
	int l,l2;

	//PUT THINGS TOGETHER
	for(l=loot.size()-1;l>=1;l--)
		{
		for(l2=l-1;l2>=0;l2--)
			{
			if(loot[l]->type==loot[l2]->type)
				{
				char conf=0;
				switch(loot[l]->type)
					{
					case ITEM_WEAPON:
						if(loot[l]->weapon.type==loot[l2]->weapon.type&&
							loot[l]->weapon.ammo==loot[l2]->weapon.ammo)conf=1;
						break;
					case ITEM_ARMOR:
						if(loot[l]->armor.type==loot[l2]->armor.type&&
							loot[l]->armor.subtype==loot[l2]->armor.subtype&&
							loot[l]->armor.quality==loot[l2]->armor.quality&&
							loot[l]->armor.flag==loot[l2]->armor.flag)conf=1;
						break;
					case ITEM_CLIP:
						if(loot[l]->cliptype==loot[l2]->cliptype)conf=1;
						break;
					case ITEM_LOOT:
						if(loot[l]->loottype==loot[l2]->loottype&&
							loot[l]->loottype!=LOOT_CEOPHOTOS&&
							loot[l]->loottype!=LOOT_INTHQDISK&&
							loot[l]->loottype!=LOOT_CORPFILES&&
							loot[l]->loottype!=LOOT_SECRETDOCUMENTS&&
							loot[l]->loottype!=LOOT_POLICERECORDS)continue;
						break;
					case ITEM_MONEY:
						conf=1;
						break;
					}
				if(conf)
					{
					if(loot[l2]->type==ITEM_MONEY)loot[l2]->money+=loot[l]->money;
					else loot[l2]->number+=loot[l]->number;
					delete loot[l];
					loot.erase(loot.begin() + l);
					break;
					}
				}
			}
		}

	//NOW DO A LAME-ASS SORT
	itemst *pt;
	for(l=loot.size()-2;l>=0;l--)
		{
		for(l2=l+1;l2<loot.size();l2++)
			{
			if(itemcompare(loot[l],loot[l2])>=0)
				{
				pt=loot[l];
				loot.erase(loot.begin() + l);
				loot.insert(loot.begin()+l2-1,pt);
				break;
				}
			}
		if(l2==loot.size())
			{
			pt=loot[l];
			loot.erase(loot.begin() + l);
			loot.push_back(pt);
			}
		}
}

char itemcompare(itemst *a,itemst *b)
{
	if(a->type<b->type)return 1;
	else if(a->type>b->type)return -1;
	else
		{
		switch(a->type)
			{
			case ITEM_WEAPON:
				if(a->weapon.type<b->weapon.type)return 1;
				else if(a->weapon.type>b->weapon.type)return -1;
				else
					{
					if(a->weapon.ammo>b->weapon.ammo)return 1;
					else if(a->weapon.ammo<b->weapon.ammo)return -1;
					}
				break;
			case ITEM_ARMOR:
				if(a->armor.type==b->armor.type)
					{
					if(a->armor.subtype==b->armor.subtype)
						{
						if(a->armor.quality==b->armor.quality)
							{
							if(a->armor.flag<b->armor.flag)return 1;
							else if(a->armor.flag>b->armor.flag)return -1;
							}
						else if(a->armor.quality<b->armor.quality)return 1;
						else return -1;
						}
					else if(a->armor.subtype<b->armor.subtype)return 1;
					else return -1;
					}
				else if(a->armor.type<b->armor.type)return 1;
				else return -1;
				break;
			case ITEM_CLIP:
				if(a->cliptype<b->cliptype)return 1;
				else return -1;
				break;
			case ITEM_LOOT:
				if(a->loottype<b->loottype)return 1;
				else return -1;
				break;
			}
		}

	return 0;
}

long creaturest::attval(short a,char usejuice)
{
	long ret=att[a];

	if(a!=ATTRIBUTE_WISDOM&&usejuice)
		{
		if(juice<=-50)ret=1;
		else if(juice<=-10)ret-=2;
		else if(juice<0)ret--;
		else if(juice>=10)
			{
			if(juice<50)ret++;
			else if(juice<100)ret+=2;
			else if(juice<200)ret+=4;
			else if(juice<500)ret+=8;
			else ret+=15;
			}
		if(ret<1)ret=1;
		}

	long disfigs=0;
	if(special[SPECIALWOUND_TEETH]<TOOTHNUM)disfigs++;
	if(special[SPECIALWOUND_TEETH]<TOOTHNUM/2)disfigs++;
	if(special[SPECIALWOUND_TEETH]==0)disfigs++;
	if(special[SPECIALWOUND_RIGHTEYE]==0)disfigs+=2;
	if(special[SPECIALWOUND_LEFTEYE]==0)disfigs+=2;
	if(special[SPECIALWOUND_TONGUE]==0)disfigs+=3;
	if(special[SPECIALWOUND_NOSE]==0)disfigs+=3;

	int legok=2;
	if((wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
		 (wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
	if((wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
		 (wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;

	switch(a)
		{
		case ATTRIBUTE_STRENGTH:
			if(special[SPECIALWOUND_NECK]!=1||
				special[SPECIALWOUND_UPPERSPINE]!=1)
				{
				ret=1;
				}
			else if(special[SPECIALWOUND_LOWERSPINE]!=1)ret>>=2;
			break;
		case ATTRIBUTE_AGILITY:
			if(special[SPECIALWOUND_NECK]!=1||
				special[SPECIALWOUND_UPPERSPINE]!=1)
				{
				ret=1;
				}
			else if(special[SPECIALWOUND_LOWERSPINE]!=1)ret>>=2;
			else if(legok==0)ret>>=2;
			else if(legok==1)ret>>=1;
			break;
		case ATTRIBUTE_HEALTH:
			if(special[SPECIALWOUND_NECK]!=1||
				special[SPECIALWOUND_UPPERSPINE]!=1)
				{
				ret=1;
				}
			else if(special[SPECIALWOUND_LOWERSPINE]!=1)ret>>=2;
			break;
		case ATTRIBUTE_CHARISMA:
			ret-=disfigs;
			break;
		}

	if(a==ATTRIBUTE_STRENGTH||
		a==ATTRIBUTE_AGILITY||
		a==ATTRIBUTE_CHARISMA)
		{
		if(blood<=20)ret>>=1;
		else if(blood<=50){ret*=3;ret>>=2;}
		else if(blood<=75){ret*=7;ret>>=3;}
		}

	if(ret<1)ret=1;

	return ret;
}

void makecharacter(void)
{
	creaturest *newcr=new creaturest;
	initliberal(*newcr);

	newcr->att[ATTRIBUTE_HEART]=5;
	newcr->att[ATTRIBUTE_WISDOM]=1;
	newcr->att[ATTRIBUTE_INTELLIGENCE]=1;
	newcr->att[ATTRIBUTE_AGILITY]=4;
	newcr->att[ATTRIBUTE_STRENGTH]=4;
	newcr->att[ATTRIBUTE_HEALTH]=4;
	for(int sk=0;sk<SKILLNUM;sk++)newcr->skill[sk]=0;

	name(newcr->propername);

	for(int q=0;q<10;q++)
		{
		erase();
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(0,0);
		addstr("Insight into a Revolution:  My Traumatic Childhood");
		set_color(COLOR_WHITE,COLOR_BLACK,0);

		switch(q)
			{
			case 0:
				move(2,0);addstr("The day I was born in 1984...");
				move(5,0);addstr("A - was the day the Sandinista Front won the elections in Nicaragua.");
				//ATTRIBUTE_CHARISMA 2
				//SKILL_PERSUASION 2
				//ATTRIBUTE_HEART 1
				move(7,0);addstr("B - was the 3rd anniversary of the assassination attempt on Ronald Reagan.");
				//SKILL_PISTOL 2
				//ATTRIBUTE_STRENGTH 1
				//ATTRIBUTE_AGILITY 1
				//ATTRIBUTE_HEALTH 1
				move(9,0);addstr("C - was the day the Macintosh was introduced.");
				//ATTRIBUTE_INTELLIGENCE 3
				//SKILL_COMPUTERS 2

				move(12,0);
				addstr("My parents named me ");
				addstr(newcr->propername);
				addstr(".");
				break;
			case 1:
				move(2,0);addstr("When I was bad...");
				move(5,0);addstr("A - my parents argued with each other about me, but I was never punished.");
				//SKILL_PERSUASION 2
				//ATTRIBUTE_CHARISMA 1
				move(7,0);addstr("B - my father beat me.  I learned to take a punch earlier than most.");
				//SKILL_HANDTOHAND 1
				//ATTRIBUTE_HEALTH 1
				//ATTRIBUTE_STRENGTH 1
				move(9,0);addstr("C - I was sent to my room, where I studied quietly by myself, alone.");
				//ATTRIBUTE_INTELLIGENCE 1
				//SKILL_WRITING 1
				//SKILL_COMPUTERS 1
				break;
			case 2:
				move(2,0);addstr("In elementary school...");
				move(5,0);addstr("A - I was the class clown.  I even had some friends.");
				//ATTRIBUTE_PERSUASION 2
				//ATTRIBUTE_CHARISMA 1
				move(7,0);addstr("B - I was unruly and often fought with the other children.");
				//SKILL_HANDTOHAND 1
				//ATTRIBUTE_STRENGTH 1
				//ATTRIBUTE_AGILITY 1
				move(9,0);addstr("C - I was at the head of the class, and I worked very hard.");
				//ATTRIBUTE_INTELLIGENCE 1
				//SKILL_WRITING 1
				//SKILL_COMPUTERS 1
				break;
			case 3:
				move(2,0);addstr("When I turned 10...");
				move(5,0);addstr("A - my parents divorced.  Acrimoniously.  I once tripped over the paperwork!");
				//ATTRIBUTE_INTELLIGENCE 1
				//SKILL_WRITING 1
				//SKILL_LAW 1
				move(7,0);addstr("B - my parents divorced.  Violently.");
				//SKILL_CLUB 1
				//ATTRIBUTE_STRENGTH 1
				//ATTRIBUTE_AGILITY 1
				move(9,0);addstr("C - my parents divorced.  My studies suffered but I continued working.");
				//ATTRIBUTE_INTELLIGENCE 2
				//SKILL_COMPUTERS 1
				break;
			case 4:
				move(2,0);addstr("When I hit junior high school...");
				move(5,0);addstr("A - I broke into lockers and was into punk rock.");
				//SKILL_SECURITY 1
				//ATTRIBUTE_CHARISMA 1
				move(7,0);addstr("B - I was into knives and broke things with bats.  My anger was unmanaged.");
				//SKILL_KNIFE 1
				//SKILL_CLUB 1
				move(9,0);addstr("C - I got into chess and go.  I was a total outcast.");
				//ATTRIBUTE_INTELLIGENCE 2
				break;
			case 5:
				move(2,0);addstr("Things were getting really bad...");
				move(5,0);addstr("A - when I stole my first car.  I got a few blocks before I totalled it.");
				//SKILL_DRIVING 1
				//SKILL_SECURITY 1
				//ATTRIBUTE_INTELLIGENCE 1
				move(7,0);addstr("B - and I went to live with my dad.  He had been in Nam and he still drank.");
				//SKILL_RIFLE 1
				//SKILL_ASSAULTRIFLE 1
				//ATTRIBUTE_AGILITY 1
				move(9,0);addstr("C - and I went completely goth.  I had no friends and made costumes by myself.");
				//SKILL_DISGUISE 2
				//SKILL_GARMENTMAKING 1
				break;
			case 6:
				move(2,0);addstr("Well, you know it has reached a crescendo when...");
				move(5,0);addstr("A - you steal a cop car and you're only 14.  I went to juvie for 6 months.");
				//SKILL_DRIVING 1
				//SKILL_SECURITY 2
				move(7,0);addstr("B - your mom shoots her ex-husband, your dad, with a shotgun.  She got off.");
				//SKILL_RIFLE 1
				//SKILL_ASSAULTRIFLE 1
				//SKILL_PISTOL 1
				move(9,0);addstr("C - you try sports for a quarter, desperate to fit in.");
				//ATTRIBUTE_STRENGTH 1
				//ATTRIBUTE_AGILITY 2
				break;
			case 7:
				move(2,0);addstr("I was only 15 when I ran away, and...");
				move(5,0);addstr("A - I started robbing houses:  rich people only.  I was fed up with their crap.");
				//SKILL_SECURITY 1
				//SKILL_DISGUISE 1
				//ATTRIBUTE_INTELLIGENCE 1
				move(7,0);addstr("B - Hung out with thugs and beat the shit out of people.");
				//ATTRIBUTE_STRENGTH 1
				//ATTRIBUTE_HEALTH 1
				//SKILL_HANDTOHAND 1
				move(9,0);addstr("C - volunteered for a left-wing candidate.  It wasn't *real*, though, you know?");
				//ATTRIBUTE_HEART 1
				//SKILL_LAW 1
				//SKILL_PERSUASION 1
				break;
			case 8:
				move(2,0);addstr("Life went on.  For my 18th birthday...");
				move(5,0);addstr("A - I stole a security uniform.");
				move(7,0);addstr("B - I bought myself an assault rifle.");
				move(9,0);addstr("C - I celebrated.  I'd saved a thousand bucks!");
				break;
			case 9:
				move(2,0);addstr("For the past few years, I've been...");
				move(5,0);addstr("A - stealing from Corporations, but I want to do more.");
				//ATTRIBUTE_INTELLIGENCE 2
				//ATTRIBUTE_AGILITY 2
				//ATTRIBUTE_HEALTH 1
				//SKILL_DISGUISE 2
				//SKILL_SECURITY 2
				//SKILL_DRIVING 1
				move(7,0);addstr("B - a violent criminal.  Nothing can change that.");
				//SKILL_RIFLE 1
				//SKILL_ASSAULTRIFLE 1
				//SKILL_PISTOL 1
				//SKILL_KNIFE 1
				//SKILL_CLUB 1
				//ATTRIBUTE_AGILITY 2
				//ATTRIBUTE_STRENGTH 1
				//ATTRIBUTE_HEALTH 2
				move(9,0);addstr("C - volunteering for prominent liberals, but I know there's a better way.");
				//ATTRIBUTE_INTELLIGENCE 2
				//ATTRIBUTE_HEART 2
				//ATTRIBUTE_HEALTH 1
				//SKILL_LAW 2
				//SKILL_WRITING 2
				//SKILL_PERSUASION 1
				move(12,0);addstr("I live in ");
				addstr(lcityname);addstr(", and it's about to experience real change.");
				break;
			}

		refresh();

		int c;
		do
			{
			c=getch();
			translategetch(c);
			}while(c!='a'&&c!='b'&&c!='c');

		switch(q)
			{
			case 0:
				if(c=='a')
					{
					newcr->att[ATTRIBUTE_CHARISMA]+=2;
					newcr->att[ATTRIBUTE_HEART]+=1;
					newcr->skill[SKILL_PERSUASION]+=2;
					}
				if(c=='b')
					{
					newcr->skill[SKILL_PISTOL]+=2;
					newcr->att[ATTRIBUTE_STRENGTH]+=1;
					newcr->att[ATTRIBUTE_AGILITY]+=1;
					newcr->att[ATTRIBUTE_HEALTH]+=1;
					}
				if(c=='c')
					{
					newcr->att[ATTRIBUTE_INTELLIGENCE]+=3;
					newcr->skill[SKILL_COMPUTERS]+=2;
					}
				break;
			case 1:
				if(c=='a')
					{
					newcr->skill[SKILL_PERSUASION]+=2;
					newcr->att[ATTRIBUTE_CHARISMA]+=1;
					}
				if(c=='b')
					{
					newcr->skill[SKILL_HANDTOHAND]+=1;
					newcr->att[ATTRIBUTE_HEALTH]+=1;
					newcr->att[ATTRIBUTE_STRENGTH]+=1;
					}
				if(c=='c')
					{
					newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
					newcr->skill[SKILL_WRITING]+=1;
					newcr->skill[SKILL_COMPUTERS]+=1;
					}
				break;
			case 2:
				if(c=='a')
					{
					newcr->skill[SKILL_PERSUASION]+=2;
					newcr->att[ATTRIBUTE_CHARISMA]+=1;
					}
				if(c=='b')
					{
					newcr->skill[SKILL_HANDTOHAND]+=1;
					newcr->att[ATTRIBUTE_STRENGTH]+=1;
					newcr->att[ATTRIBUTE_AGILITY]+=1;
					}
				if(c=='c')
					{
					newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
					newcr->skill[SKILL_WRITING]+=1;
					newcr->skill[SKILL_COMPUTERS]+=1;
					}
				break;
			case 3:
				if(c=='a')
					{
					newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
					newcr->skill[SKILL_WRITING]+=1;
					newcr->skill[SKILL_LAW]+=1;
					}
				if(c=='b')
					{
					newcr->skill[SKILL_CLUB]+=1;
					newcr->att[ATTRIBUTE_STRENGTH]+=1;
					newcr->att[ATTRIBUTE_AGILITY]+=1;
					}
				if(c=='c')
					{
					newcr->att[ATTRIBUTE_INTELLIGENCE]+=2;
					newcr->skill[SKILL_COMPUTERS]+=1;
					}
				break;
			case 4:
				if(c=='a')
					{
					newcr->skill[SKILL_SECURITY]+=1;
					newcr->att[ATTRIBUTE_CHARISMA]+=1;
					}
				if(c=='b')
					{
					newcr->skill[SKILL_KNIFE]+=1;
					newcr->skill[SKILL_CLUB]+=1;
					}
				if(c=='c')
					{
					newcr->att[ATTRIBUTE_INTELLIGENCE]+=2;
					}
				break;
			case 5:
				if(c=='a')
					{
					newcr->skill[SKILL_DRIVING]+=1;
					newcr->skill[SKILL_SECURITY]+=1;
					newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
					}
				if(c=='b')
					{
					newcr->skill[SKILL_RIFLE]+=1;
					newcr->skill[SKILL_ASSAULTRIFLE]+=1;
					newcr->att[ATTRIBUTE_AGILITY]+=1;
					}
				if(c=='c')
					{
					newcr->skill[SKILL_DISGUISE]+=2;
					newcr->skill[SKILL_GARMENTMAKING]+=1;
					}
				break;
			case 6:
				if(c=='a')
					{
					newcr->skill[SKILL_DRIVING]+=1;
					newcr->skill[SKILL_SECURITY]+=2;
					}
				if(c=='b')
					{
					newcr->skill[SKILL_RIFLE]+=1;
					newcr->skill[SKILL_ASSAULTRIFLE]+=1;
					newcr->skill[SKILL_PISTOL]+=1;
					}
				if(c=='c')
					{
					newcr->att[ATTRIBUTE_STRENGTH]+=1;
					newcr->att[ATTRIBUTE_AGILITY]+=2;
					}
				break;
			case 7:
				if(c=='a')
					{
					newcr->skill[SKILL_SECURITY]+=1;
					newcr->skill[SKILL_DISGUISE]+=1;
					newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
					}
				if(c=='b')
					{
					newcr->att[ATTRIBUTE_STRENGTH]+=1;
					newcr->att[ATTRIBUTE_HEALTH]+=1;
					newcr->skill[SKILL_HANDTOHAND]+=1;
					}
				if(c=='c')
					{
					newcr->att[ATTRIBUTE_HEART]+=1;
					newcr->skill[SKILL_LAW]+=1;
					newcr->skill[SKILL_PERSUASION]+=1;
					}
				break;
			case 8:
				if(c=='a')
					{
					newcr->armor.type=ARMOR_SECURITYUNIFORM;
					newcr->armor.quality='1';
					newcr->armor.flag=0;
					}
				if(c=='b')
					{
					newcr->weapon.type=WEAPON_SEMIRIFLE_AK47;
					newcr->weapon.ammo=30;
					newcr->clip[CLIP_ASSAULT]=9;
					}
				if(c=='c')
					{
					funds=1000;
					}
				break;
			case 9:
				if(c=='a')
					{
					newcr->att[ATTRIBUTE_INTELLIGENCE]+=2;
					newcr->att[ATTRIBUTE_AGILITY]+=2;
					newcr->att[ATTRIBUTE_HEALTH]+=1;
					newcr->skill[SKILL_DISGUISE]+=2;
					newcr->skill[SKILL_SECURITY]+=2;
					newcr->skill[SKILL_DRIVING]+=1;
					}
				if(c=='b')
					{
					newcr->skill[SKILL_RIFLE]+=1;
					newcr->skill[SKILL_ASSAULTRIFLE]+=1;
					newcr->skill[SKILL_PISTOL]+=1;
					newcr->skill[SKILL_KNIFE]+=1;
					newcr->skill[SKILL_CLUB]+=1;
					newcr->att[ATTRIBUTE_AGILITY]+=2;
					newcr->att[ATTRIBUTE_STRENGTH]+=1;
					newcr->att[ATTRIBUTE_HEALTH]+=2;
					}
				if(c=='c')
					{
					newcr->att[ATTRIBUTE_INTELLIGENCE]+=2;
					newcr->att[ATTRIBUTE_HEART]+=2;
					newcr->att[ATTRIBUTE_HEALTH]+=1;
					newcr->skill[SKILL_LAW]+=2;
					newcr->skill[SKILL_WRITING]+=2;
					newcr->skill[SKILL_PERSUASION]+=1;
					}
				break;
			}
		}

	erase();
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(0,0);
	addstr("What is your name to the People?");
	set_color(COLOR_WHITE,COLOR_BLACK,0);

	keypad(stdscr,FALSE);
	raw_output(FALSE);
	echo();
	curs_set(1);
	mvgetstr(1,0,newcr->name);
	curs_set(0);
	noecho();
	raw_output(TRUE);
	keypad(stdscr,TRUE);

	pool.push_back(newcr);

	//MAKE LOCATIONS
	locationst *newl;

	newl=new locationst;
		strcpy(newl->name,"Downtown");
		strcpy(newl->shortname,"Downtown");
		newl->type=SITE_DOWNTOWN;
		newl->parent=-1;
	location.push_back(newl);

	newl=new locationst;
		strcpy(newl->name,"The University District");
		strcpy(newl->shortname,"U-District");
		newl->type=SITE_UDISTRICT;
		newl->parent=-1;
	location.push_back(newl);

	newl=new locationst;
		strcpy(newl->name,"The Industrial District");
		strcpy(newl->shortname,"I-District");
		newl->type=SITE_INDUSTRIAL;
		newl->parent=-1;
	location.push_back(newl);

	newl=new locationst;
		strcpy(newl->name,"On the Outskirts of the City");
		strcpy(newl->shortname,"Outskirts");
		newl->type=SITE_OUTOFTOWN;
		newl->parent=-1;
		newl->needcar=1;
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_GOVERNMENT_POLICESTATION;
		newl->parent=0;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_GOVERNMENT_COURTHOUSE;
		newl->parent=0;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_GOVERNMENT_PRISON;
		newl->parent=3;
		newl->needcar=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_INDUSTRY_NUCLEAR;
		newl->parent=3;
		newl->needcar=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_GOVERNMENT_INTELLIGENCEHQ;
		newl->parent=3;
		newl->needcar=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_CORPORATE_HEADQUARTERS;
		newl->parent=3;
		newl->needcar=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_CORPORATE_HOUSE;
		newl->parent=3;
		newl->needcar=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_RESIDENTIAL_SHELTER;
		newl->parent=2;
		newl->renting=0;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_INDUSTRY_WAREHOUSE;
		newl->parent=2;
		newl->renting=0;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_INDUSTRY_WAREHOUSE;
		newl->parent=2;
		newl->renting=0;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_INDUSTRY_WAREHOUSE;
		newl->parent=2;
		newl->renting=0;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_INDUSTRY_POLLUTER;
		newl->parent=2;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_INDUSTRY_POLLUTER;
		newl->parent=2;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_INDUSTRY_POLLUTER;
		newl->parent=2;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_MEDIA_CABLENEWS;
		newl->parent=0;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_MEDIA_AMRADIO;
		newl->parent=0;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_RESIDENTIAL_APARTMENT_UPSCALE;
		newl->parent=0;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_RESIDENTIAL_APARTMENT_UPSCALE;
		newl->parent=0;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_RESIDENTIAL_APARTMENT;
		newl->parent=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_RESIDENTIAL_APARTMENT;
		newl->parent=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_RESIDENTIAL_TENEMENT;
		newl->parent=2;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_RESIDENTIAL_TENEMENT;
		newl->parent=2;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_HOSPITAL_UNIVERSITY;
		newl->parent=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_HOSPITAL_CLINIC;
		newl->parent=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_LABORATORY_GENETIC;
		newl->parent=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_LABORATORY_COSMETICS;
		newl->parent=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_BUSINESS_PAWNSHOP;
		newl->parent=2;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_BUSINESS_DEPTSTORE;
		newl->parent=0;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_BUSINESS_HALLOWEEN;
		newl->parent=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_INDUSTRY_SWEATSHOP;
		newl->parent=2;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_INDUSTRY_SWEATSHOP;
		newl->parent=2;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_BUSINESS_CRACKHOUSE;
		newl->parent=2;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_BUSINESS_CRACKHOUSE;
		newl->parent=2;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_BUSINESS_CRACKHOUSE;
		newl->parent=2;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_BUSINESS_VEGANCOOP;
		newl->parent=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_BUSINESS_JUICEBAR;
		newl->parent=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_BUSINESS_INTERNETCAFE;
		newl->parent=1;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_BUSINESS_CIGARBAR;
		newl->parent=0;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_BUSINESS_LATTESTAND;
		newl->parent=0;
		initlocation(*newl);
	location.push_back(newl);

	newl=new locationst;
		newl->type=SITE_BUSINESS_LATTESTAND;
		newl->parent=1;
		initlocation(*newl);
	location.push_back(newl);

	squadst *newsq=new squadst;
		newsq->id=0;cursquadid++;
		newsq->squad[0]=newcr;
		newcr->squadid=0;
		strcpy(newsq->name,"The Liberal Crime Squad");
		for(int l=0;l<location.size();l++)
			{
			if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
				{
				newcr->base=l;
				newcr->location=l;

				#ifdef GIVEBLOODYARMOR
				itemst *newi=new itemst;
					newi->type=ITEM_ARMOR;
					newi->armor.flag=1;
					newi->armor.quality='1';
					newi->armor.type=ARMOR_CLOTHES;
					newi->number=1;
				location[l]->loot.push_back(newi);
				#endif
				break;
				}
			}
		newcr->juice=0;
	squad.push_back(newsq);
	activesquad=newsq;
}

int monthday(void)
{
	switch(month)
		{
		case 4:return 30;
		case 6:return 30;
		case 9:return 30;
		case 11:return 30;
		case 2:
			if(year%4==0)return 29;
			else return 28;
		default:
			return 31;
		}
}

void advanceday(char &clearformess,char canseethings)
{
	int p;
	showcarprefs=0;
	int w=0;
	int l2;

	//Save the game to autosave.dat each day.
	autosave();

	//CLEAR CAR STATES
	vector<long> caridused;
	for(p=0;p<pool.size();p++)pool[p]->carid=-1;

	//SHUFFLE AROUND THE SQUADLESS
	int homes=-1;
	for(int l=0;l<location.size();l++)
		{
		if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
			{
			homes=l;
			break;
			}
		}
		if (homes==-1)
			{
			//TODO: Error unable to find location
			homes=0;
			}
		
	for(p=0;p<pool.size();p++)
		{
		if(disbanding)break;

		if(!pool[p]->alive)continue;
		if(pool[p]->clinic)continue;
		if(pool[p]->dating)continue;
		if(pool[p]->hiding)continue;
		if(pool[p]->squadid!=-1)continue;
		if(pool[p]->location==-1)continue;

		if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
						location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
						location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON)
			{

			if(location[pool[p]->base]->siege.siege)
				{
				pool[p]->base=homes;
				}

			pool[p]->location=pool[p]->base;
			}
		}

	//ADVANCE SQUADS
	squadst *oactivesquad=activesquad;
	for(int sq=0;sq<squad.size();sq++)
		{
		if(disbanding)break;

		//MAKE SURE MEMBERS DON'T ACT IF SQUAD DOES
		if(squad[sq]->activity.type!=ACTIVITY_NONE)
			{
			for(int p=0;p<6;p++)
				{
				if(squad[sq]->squad[p]!=NULL)
					{
					if(squad[sq]->squad[p]->activity.type!=ACTIVITY_NONE)
						{
						if(clearformess)erase();
						else
							{
							makedelimiter(8,0);
							}
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(8,1);
						addstr(squad[sq]->squad[p]->name);
						addstr(" acted with ");
						addstr(squad[sq]->name);
						addstr(" instead of ");
						char str[200];
						getactivity(str,squad[sq]->squad[p]->activity);
						addstr(str);
						addstr(".");

						refresh();
						getch();

						squad[sq]->squad[p]->activity.type=ACTIVITY_NONE;
						}
					}
				}
			}

		if(squad[sq]->activity.type==ACTIVITY_VISIT)
			{
			//TURN AWAY SQUADS FROM RECENTLY CLOSED OR SIEGED SITES
			if(location[squad[sq]->activity.arg]->closed||
				location[squad[sq]->activity.arg]->siege.siege)
				{
				if(clearformess)erase();
				else
					{
					makedelimiter(8,0);
					}
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(8,1);
				addstr(squad[sq]->name);
				addstr(" decided ");
				addlocationname(location[squad[sq]->activity.arg]);
				addstr(" was too hot to risk.");

				refresh();
				getch();

				//ON TO THE NEXT SQUAD
				squad[sq]->activity.type=ACTIVITY_NONE;
				continue;
				}

			//CAR UP AS NECESSARY
			vector<long> wantcar;

			for(p=0;p<6;p++)
				{
				if(squad[sq]->squad[p]!=NULL)
					{
					long wid=squad[sq]->squad[p]->pref_carid;
					if(wid==-1)continue;
					for(w=0;w<wantcar.size();w++)
						{
						if(wantcar[w]==wid)break;
						}
					if(w==wantcar.size())wantcar.push_back(wid);
					}
				}

			if(wantcar.size()>0)
				{
				//CULL UNAVAILABLE CARS
				for(int c=wantcar.size()-1;c>=0;c--)
					{
					for(int c2=0;c2<caridused.size();c2++)
						{
						if(wantcar[c]==caridused[c2])
							{
							if(clearformess)erase();
							else
								{
								makedelimiter(8,0);
								}
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(8,1);
							addstr(squad[sq]->name);
							addstr(" couldn't use the ");
							long v=id_getcar(caridused[c2]);
							char str[80];
							getcarfull(str,*vehicle[v]);
							addstr(str);
							addstr(".");

							refresh();
							getch();

							wantcar.erase(wantcar.begin() + c);
							}
						}
					}
				}

			//ASSIGN AVAILABLE CARS
			if(wantcar.size()>0)
				{
				vector<int> driver;
				vector<int> passenger;
				for(w=0;w<wantcar.size();w++)
					{
					driver.clear();
					passenger.clear();
					caridused.push_back(wantcar[w]);

					//FILL CAR WITH DESIGNATED DRIVERS AND PASSENGERS
					for(p=0;p<6;p++)
						{
						if(squad[sq]->squad[p]!=NULL)
							{
							if(squad[sq]->squad[p]->pref_carid==wantcar[w])
								{
								squad[sq]->squad[p]->carid=wantcar[w];
								squad[sq]->squad[p]->is_driver=squad[sq]->squad[p]->pref_is_driver;
								if(squad[sq]->squad[p]->is_driver)driver.push_back(p);
								else passenger.push_back(p);
								}
							}
						}

					//NO DRIVER?
					if(driver.size()==0)
						{
						//MAKE BEST DRIVING PASSENGER INTO A DRIVER
						if(passenger.size()>0)
							{
							int max=0;
							for(p=0;p<passenger.size();p++)
								{
								long v=id_getcar(squad[sq]->squad[passenger[p]]->carid);
								if(v >= 0) 
								  {
								    if(driveskill(*squad[sq]->squad[passenger[p]],vehicle[v])>max&&
								  	  squad[sq]->squad[passenger[p]]->canwalk())
								  	  {
								  	    max=driveskill(*squad[sq]->squad[passenger[p]],vehicle[v]);
									  }
								  }
								}
							vector<int> goodp;
							for(p=0;p<passenger.size();p++)
								{
								long v=id_getcar(squad[sq]->squad[passenger[p]]->carid);
								if(v >= 0) 
								  {
								    if(driveskill(*squad[sq]->squad[passenger[p]],vehicle[v])==max&&
									  squad[sq]->squad[passenger[p]]->canwalk())
									  {
									  goodp.push_back(passenger[p]);
									  }
								  }
								}

							if(goodp.size()>0)
								{
								int p=goodp[LCSrandom(goodp.size())];
								squad[sq]->squad[p]->is_driver=1;
								}
							}
						}
					//TOO MANY DRIVERS?
					else if(driver.size()>1)
						{
						//TOSS ALL BUT THE BEST
						int max=0;
						for(p=0;p<driver.size();p++)
							{
							long v=id_getcar(squad[sq]->squad[driver[p]]->carid);
							if (v >= 0)
							  {
							  if(driveskill(*squad[sq]->squad[driver[p]],vehicle[v])>max)
								{
								  max=driveskill(*squad[sq]->squad[driver[p]],vehicle[v]);
								}
							  }
							}
						vector<int> goodp;
						for(p=0;p<driver.size();p++)
							{
							long v=id_getcar(squad[sq]->squad[driver[p]]->carid);
							if (v >= 0)
							  {
							  if(driveskill(*squad[sq]->squad[driver[p]],vehicle[v])==max)
								  {
								  goodp.push_back(p);
								  }
							  }
							}

						if(goodp.size()>0)
							{
							int p=goodp[LCSrandom(goodp.size())];
							for(int p2=0;p2<driver.size();p2++)
								{
								if(p2==p)continue;
								squad[sq]->squad[driver[p2]]->is_driver=0;
								}
							}
						}
					}

				//PUT PEOPLE WITHOUT CARS INTO RANDOM CARS
					//THESE PEOPLE WILL NOT DRIVE
				for(int p=0;p<6;p++)
					{
					if(squad[sq]->squad[p]!=NULL)
						{
						if(squad[sq]->squad[p]->carid==-1)
							{
							squad[sq]->squad[p]->carid=wantcar[LCSrandom(wantcar.size())];
							squad[sq]->squad[p]->is_driver=0;
							}
						}
					}
				}

			//IF NEED CAR AND DON'T HAVE ONE...
				//NOTE: SQUADS DON'T TAKE FREE CARS
			if(location[squad[sq]->activity.arg]->needcar&&
				squad[sq]->squad[0]!=NULL)
				{
				if(squad[sq]->squad[0]->carid==-1)
					{
					if(clearformess)erase();
					else
						{
						makedelimiter(8,0);
						}
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(8,1);
					addstr(squad[sq]->name);
					addstr(" didn't have a car to get to ");
					addlocationname(location[squad[sq]->activity.arg]);
					addstr(".");

					refresh();
					getch();

					//ON TO THE NEXT SQUAD
					squad[sq]->activity.type=ACTIVITY_NONE;
					continue;
					}
				}

			//GO PLACES
			switch(location[squad[sq]->activity.arg]->type)
				{
				case SITE_BUSINESS_DEPTSTORE:
				case SITE_BUSINESS_HALLOWEEN:
				case SITE_BUSINESS_PAWNSHOP:
					if(clearformess)erase();
					else
						{
						makedelimiter(8,0);
						}
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(8,1);
					addstr(squad[sq]->name);
					addstr(" has arrived at ");
					addlocationname(location[squad[sq]->activity.arg]);
					addstr(".");

					refresh();
					getch();

					activesquad=squad[sq];
					showcarprefs=-1;
					switch(location[squad[sq]->activity.arg]->type)
						{
						case SITE_BUSINESS_DEPTSTORE:
							deptstore(squad[sq]->activity.arg);
							break;
						case SITE_BUSINESS_HALLOWEEN:
							halloweenstore(squad[sq]->activity.arg);
							break;
						case SITE_BUSINESS_PAWNSHOP:
							pawnshop(squad[sq]->activity.arg);
							break;
						}
					showcarprefs=0;
					if(activesquad->squad[0]!=NULL)
						{
						locatesquad(activesquad,activesquad->squad[0]->base);
						}
					clearformess=1;
					break;
				case SITE_HOSPITAL_UNIVERSITY:
				case SITE_HOSPITAL_CLINIC:
					if(clearformess)erase();
					else
						{
						makedelimiter(8,0);
						}
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(8,1);
					addstr(squad[sq]->name);
					addstr(" has arrived at ");
					addlocationname(location[squad[sq]->activity.arg]);
					addstr(".");

					refresh();
					getch();

					activesquad=squad[sq];
					hospital(squad[sq]->activity.arg);
					if(activesquad->squad[0]!=NULL)
						{
						locatesquad(activesquad,activesquad->squad[0]->base);
						}
					clearformess=1;
					break;
				default:
					{
					if(clearformess)erase();
					else
						{
						makedelimiter(8,0);
						}

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(8,1);
					if(squad[sq]->squad[0]->base==squad[sq]->activity.arg)
						{
						addstr(squad[sq]->name);
						addstr(" looks around ");
						addlocationname(location[squad[sq]->activity.arg]);
						addstr(".");
						}
					else
						{
						addstr(squad[sq]->name);
						addstr(" has arrived at ");
						addlocationname(location[squad[sq]->activity.arg]);
						addstr(".");
						}

					refresh();
					getch();

					if(clearformess)erase();
					else
						{
						makedelimiter(8,0);
						}

					int c='t';

					if(location[squad[sq]->activity.arg]->renting>=0&&
						location[squad[sq]->activity.arg]->type==SITE_INDUSTRY_WAREHOUSE)
						{
						c='s';
						}
					else if(location[squad[sq]->activity.arg]->renting>=0&&
						squad[sq]->squad[0]->base!=squad[sq]->activity.arg)
						{
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(8,1);
						addstr("Why is the squad here?  (S)afe House, to cause (T)rouble, or (B)oth?");

						c=getch();
						translategetch(c);
						}

					if(c=='s'||c=='b')basesquad(squad[sq],squad[sq]->activity.arg);
					if(c=='t'||c=='b')
						{
						activesquad=squad[sq];
						newsstoryst *ns=new newsstoryst;
							ns->type=NEWSSTORY_SQUAD_SITE;
							ns->positive=1;
							ns->loc=squad[sq]->activity.arg;
						newsstory.push_back(ns);
						mode_site(squad[sq]->activity.arg);
						}
					if(squad[sq]->squad[0]!=NULL)
						{
						locatesquad(squad[sq],squad[sq]->squad[0]->base);
						}

					clearformess=1;
					break;
					}
				}
			squad[sq]->activity.type=ACTIVITY_NONE;
			}
		}
	activesquad=oactivesquad;

	//HOSTAGES
	for(p=pool.size()-1;p>=0;p--)
		{
		if(disbanding)break;

		if(!pool[p]->alive)continue;
		if(pool[p]->align!=1)
			{
			tendhostage(pool[p],clearformess);
			}
		}

	//ACTIVITIES FOR INDIVIDUALS
	for(p=0;p<pool.size();p++)
		{
		if(disbanding)break;

		if(!pool[p]->alive)continue;
		if(pool[p]->clinic)continue;
		if(pool[p]->dating)continue;
		if(pool[p]->hiding)continue;

		//CLEAR ACTIONS FOR PEOPLE UNDER SIEGE
		if(location[pool[p]->location]->siege.siege)
			{
			pool[p]->activity.type=ACTIVITY_NONE;
			}
		switch(pool[p]->activity.type)
			{
			case ACTIVITY_REPAIR_ARMOR:
				repairarmor(*pool[p],clearformess);
				break;
			case ACTIVITY_MAKE_ARMOR:
				makearmor(*pool[p],clearformess);
				pool[p]->activity.type=ACTIVITY_NONE;
				break;
			case ACTIVITY_WHEELCHAIR:
				getwheelchair(*pool[p],clearformess);
				if(pool[p]->flag & CREATUREFLAG_WHEELCHAIR)pool[p]->activity.type=ACTIVITY_NONE;
				break;
			case ACTIVITY_STEALCARS:
				if(stealcar(*pool[p],clearformess))
					{
					pool[p]->activity.type=ACTIVITY_NONE;
					}
				break;
			case ACTIVITY_POLLS:
				if(clearformess)erase();
				else
					{
					makedelimiter(8,0);
					}

				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(8,1);
				addstr(pool[p]->name);
				addstr(" surfs the Net for recent opinion polls.");

				refresh();
				getch();

				pool[p]->skill_ip[SKILL_COMPUTERS]++;

				survey(pool[p]);
				pool[p]->activity.type=ACTIVITY_NONE;

				clearformess=1;
				break;
			}
		}

	funds_and_trouble(clearformess);

	//DISPERSAL CHECK
	dispersalcheck(clearformess);

	//DO RENT
	if(day==3&&!disbanding)
		{
		for(int l=0;l<location.size();l++)
			{
			if(location[l]->renting>0&&
				!location[l]->newrental)
				{
				if(funds>=location[l]->renting)
					{
					funds-=location[l]->renting;
					moneylost_rent+=location[l]->renting;
					stat_spent+=location[l]->renting;
					}
				//EVICTED!!!!!!!!!
				else
					{
					if(clearformess)erase();
					else
						{
						makedelimiter(8,0);
						}

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(8,1);
					addstr("EVICTION NOTICE: ");
					addstr(location[l]->name);
					addstr(".  Possessions go to the shelter.");

					refresh();
					getch();

					location[l]->renting=-1;

					//MOVE ALL ITEMS AND SQUAD MEMBERS
					int hs=0;
					for(l2=0;l2<location.size();l2++)
						{
						if(location[l2]->type==SITE_RESIDENTIAL_SHELTER)
							{
							hs=l2;
							break;
							}
						}
					for(int p=0;p<pool.size();p++)
						{
						if(pool[p]->location==l)pool[p]->location=hs;
						if(pool[p]->base==l)pool[p]->base=hs;
						}
					for(l2=0;l2<location[l]->loot.size();l2++)
						{
						location[hs]->loot.push_back(location[l]->loot[l2]);
						}
					location[l]->loot.clear();

					location[l]->compound_walls=0;
					location[l]->compound_stores=0;
					location[l]->front_business=-1;
					}
				}
			}
		}

	//DO DATES
	for(int d=date.size()-1;d>=0;d--)
		{
		if(disbanding)break;

		int p=getpoolcreature(date[d]->mac_id);
		if(p!=-1)
			{
			//VACATION
			if(date[d]->timeleft>0)
				{
				date[d]->timeleft--;

				pool[p]->dating=date[d]->timeleft;

				if(date[d]->timeleft==0)
					{
					int hs=-1;
					for(int l=0;l<location.size();l++)
						{
						if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
							{
							hs=l;
							break;
							}
						}
					if (hs==-1)
					{
						//TODO: Error unable to find location
						hs=0;
					}

					if(location[pool[p]->base]->siege.siege)
						{
						pool[p]->base=hs;
						}

					pool[p]->location=pool[p]->base;

					if(completevacation(*date[d],p,clearformess))
						{
						delete date[d];
						date.erase(date.begin() + d);
						continue;
						}
					}
				}
			//DO A DATE
			else
				{
				//TERMINATE NULL DATES
				if(location[pool[p]->location]->siege.siege)
					{
					delete date[d];
					date.erase(date.begin() + d);
					continue;
					}
				//DO DATE
				else if(completedate(*date[d],p,clearformess))
					{
					delete date[d];
					date.erase(date.begin() + d);
					continue;
					}
				else
					{
					pool[p]->dating=date[d]->timeleft;
					if(pool[p]->dating>0)
						{
						//IF YOU ARE THE LAST PERSON IN YOUR SQUAD
							//YOU HAVE TO DROP OFF THE EQUIPMENT WHEREVER YOUR BASE IS
							//BECAUSE YOUR SQUAD IS ABOUT TO BE DESTROYED
						if(pool[p]->squadid!=-1)
							{
							long sq=getsquad(pool[p]->squadid);
							if(sq!=-1)
								{
								char hasmembers=0;
								int p2,l;
								for(p2=0;p2<6;p2++)
									{
									if(squad[sq]->squad[p2]!=NULL)hasmembers++;
									}
								if(hasmembers<=1)
									{
									if(pool[p]->location!=-1)
										{
										for(l=0;l<squad[sq]->loot.size();l++)
											{
											location[pool[p]->location]->loot.push_back(squad[sq]->loot[l]);
											}
										squad[sq]->loot.clear();
										}
									}
								}
							}

						//NOW KICK THE DATER OUT OF THE SQUAD AND LOCATION
						removesquadinfo(*pool[p]);
						pool[p]->location=-1;
						}
					}
				}
			}
		else
			{
			delete date[d];
			date.erase(date.begin() + d);
			continue;
			}
		}

	//AGE THINGS
	day++;
	for(p=0;p<pool.size();p++)
		{
		if(pool[p]->hiding>0)
			{
			pool[p]->hiding--;
			if(pool[p]->hiding==0)
				{
				pool[p]->location=pool[p]->base;

				if(clearformess)erase();
				else
					{
					makedelimiter(8,0);
					}

				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(8,1);
				addstr(pool[p]->name);
				addstr(" slinks back to the homeless shelter.");

				refresh();
				getch();
				}
			}

		if((pool[p]->flag & CREATUREFLAG_MISSING)&&
			!(pool[p]->flag & CREATUREFLAG_KIDNAPPED))
			{
			if(LCSrandom(10)+2<pool[p]->joindays)
				{
				pool[p]->flag|=CREATUREFLAG_KIDNAPPED;

				newsstoryst *ns=new newsstoryst;
					ns->type=NEWSSTORY_KIDNAPREPORT;
					ns->cr=pool[p];
				newsstory.push_back(ns);
				}
			}

		pool[p]->joindays++;

		if(!pool[p]->alive)
			{
			pool[p]->deathdays++;
			continue;
			}

		for(int s=0;s<SKILLNUM;s++)
			{
			if(pool[p]->skill_ip[s]>=100)
				{
				pool[p]->skill[s]+=pool[p]->skill_ip[s]/100;
				pool[p]->skill_ip[s]%=100;
				}
			}
		}
	
	//DO REPORTING BY MAJOR NEWSPAPERS
	majornewspaper(clearformess,canseethings);

	//CLEAN UP GONE SQUADS
	cleangonesquads();

	//SIEGE?
	siegeturn(clearformess);
	siegecheck(canseethings);

	//CLEAN GONE SQUADS AGAIN
	cleangonesquads();

	showcarprefs=1;
}

void cleangonesquads(void)
{
	char hasmembers;
	for(int sq=squad.size()-1;sq>=0;sq--)
		{
		//NUKE SQUAD IF IT IS GONE
		hasmembers=0;
		for(int p=0;p<6;p++)
			{
			if(squad[sq]->squad[p]!=NULL)hasmembers=1;
			}
		if(!hasmembers)
			{
			//SQUAD LOOT WILL BE DESTROYED
			if(activesquad==squad[sq])activesquad=NULL;
			delete squad[sq];
			squad.erase(squad.begin() + sq);
			}
		//OTHERWISE YOU CAN TAKE ITS MONEY
		else
			{
			for(int l=squad[sq]->loot.size()-1;l>=0;l--)
				{
				if(squad[sq]->loot[l]->type==ITEM_MONEY)
					{
					funds+=squad[sq]->loot[l]->money;
					stat_funds+=squad[sq]->loot[l]->money;
					moneygained_thievery+=squad[sq]->loot[l]->money;
					delete squad[sq]->loot[l];
					squad[sq]->loot.erase(squad[sq]->loot.begin() + l);
					}
				}
			}
		}
}

void locatesquad(squadst *st,long loc)
{
	for(int p=0;p<6;p++)
		{
		if(st->squad[p]!=NULL)
			{
			st->squad[p]->location=loc;

			if(st->squad[p]->carid!=-1)
				{
				long v=id_getcar(st->squad[p]->carid);
				if(v!=-1)
					{
					vehicle[v]->location=loc;
					}
				}
			}
		}
}

void basesquad(squadst *st,long loc)
{
	for(int p=0;p<6;p++)
		{
		if(st->squad[p]!=NULL)
			{
			st->squad[p]->base=loc;
			}
		}
}

void getactivity(char *str,activityst &act)
{
	strcpy(str,"");

	switch(act.type)
		{
		case ACTIVITY_HOSTAGETENDING:
			{
			strcat(str,"Tending to ");
			int pl=getpoolcreature(act.arg);
			if(pl!=-1)
				{
				strcat(str,pool[pl]->name);
				}
			break;
			}
		case ACTIVITY_REPAIR_ARMOR:
			strcat(str,"Repairing Clothing");
			break;
		case ACTIVITY_WHEELCHAIR:
			strcat(str,"Procuring a Wheelchair");
			break;
		case ACTIVITY_STEALCARS:
			strcat(str,"Stealing a Car");
			break;
		case ACTIVITY_POLLS:
			strcat(str,"Gathering Opinion Info");
			break;
		case ACTIVITY_MAKE_ARMOR:
			{
			strcat(str,"Making ");
			char str2[80];
			getarmor(str2,act.arg);
			strcat(str,str2);
			break;
			}
		case ACTIVITY_TROUBLE:
			strcat(str,"Causing Trouble");
			break;
		case ACTIVITY_BURY:
			strcat(str,"Disposing of Bodies");
			break;
		case ACTIVITY_FUNDS_LEGAL:
			strcat(str,"Soliciting Donations");
			break;
		case ACTIVITY_FUNDS_ILLEGAL:
			strcat(str,"Selling Brownies");
			break;
		case ACTIVITY_VISIT:
			{
			strcat(str,"Going to ");
			strcat(str,location[act.arg]->shortname);
			break;
			}
		case ACTIVITY_NONE:
			strcat(str,"Hanging Out");
			break;
		}
}

void special_readsign(int sign)
{
	clearmessagearea();
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(16,1);
	addstr("The landlord's office is the first door");
	move(17,1);
	addstr("on the left.");
	getch();
}

void review_mode(short mode)
{
	vector<creaturest *> temppool;
	for(int p=0;p<pool.size();p++)
		{
		switch(mode)
			{
			case REVIEWMODE_LIBERALS:
				if(pool[p]->alive==1&&
					pool[p]->align==1&&
					pool[p]->clinic==0&&
					pool[p]->dating==0&&
					pool[p]->hiding==0&&
					!(pool[p]->flag & CREATUREFLAG_SLEEPER))
					{
					if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
						location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
						location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON)temppool.push_back(pool[p]);
					}
				break;
			case REVIEWMODE_HOSTAGES:
				if(pool[p]->align!=1&&
					pool[p]->alive==1)temppool.push_back(pool[p]);
				break;
			case REVIEWMODE_CLINIC:
				if(pool[p]->clinic>0&&
					pool[p]->alive==1)temppool.push_back(pool[p]);
				break;
			case REVIEWMODE_JUSTICE:
				if(!(pool[p]->flag & CREATUREFLAG_SLEEPER)&&
					pool[p]->clinic==0&&
					pool[p]->dating==0&&
					pool[p]->hiding==0&&
					pool[p]->alive==1)
					{
					if(location[pool[p]->location]->type==SITE_GOVERNMENT_POLICESTATION||
						location[pool[p]->location]->type==SITE_GOVERNMENT_COURTHOUSE||
						location[pool[p]->location]->type==SITE_GOVERNMENT_PRISON)temppool.push_back(pool[p]);
					}
				break;
			case REVIEWMODE_SLEEPERS:
				if(pool[p]->alive==1&&
					pool[p]->align==1&&
					pool[p]->clinic==0&&
					pool[p]->dating==0&&
					pool[p]->hiding==0&&
					(pool[p]->flag & CREATUREFLAG_SLEEPER))temppool.push_back(pool[p]);
				break;
			case REVIEWMODE_DEAD:
				if(!pool[p]->alive)temppool.push_back(pool[p]);
				break;
			case REVIEWMODE_AWAY:
				if((pool[p]->dating>0||
					pool[p]->hiding>0)&&
					pool[p]->alive==1)temppool.push_back(pool[p]);
				break;
			}
		}

	if(temppool.size()==0)return;

	short page=0;

	char num[20];

	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		switch(mode)
			{
			case REVIEWMODE_LIBERALS:
				addstr("Active Liberals");
				break;
			case REVIEWMODE_HOSTAGES:
				addstr("Conservative Automatons in Captivity");
				break;
			case REVIEWMODE_CLINIC:
				addstr("Liberals in CLINICS");
				break;
			case REVIEWMODE_JUSTICE:
				addstr("Liberals and the Justice System");
				break;
			case REVIEWMODE_SLEEPERS:
				addstr("Sleepers");
				break;
			case REVIEWMODE_DEAD:
				addstr("Liberal Martyrs and Dead Bodies");
				break;
			case REVIEWMODE_AWAY:
				addstr("Liberals that are Away");
				break;
			}
		move(1,0);
		addstr("----CODE NAME------------SKILL---HEALTH---LOCATION------------------------------");

		move(1,57);
		switch(mode)
			{
			case REVIEWMODE_LIBERALS:
				addstr("SQUAD / ACTIVITY");
				break;
			case REVIEWMODE_HOSTAGES:
				addstr("DAYS IN CAPTIVITY");
				break;
			case REVIEWMODE_JUSTICE:
				addstr("MONTHS LEFT");
				break;
			case REVIEWMODE_CLINIC:
				addstr("PROGNOSIS");
				break;
			case REVIEWMODE_SLEEPERS:
				addstr("PROFESSION");
				break;
			case REVIEWMODE_DEAD:
				addstr("DAYS SINCE PASSING");
				break;
			case REVIEWMODE_AWAY:
				addstr("DAYS UNTIL RETURN");
				break;
			}

		int y=2;
		for(int p=page*19;p<temppool.size()&&p<page*19+19;p++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addch(y+'A'-2);addstr(" - ");
			addstr(temppool[p]->name);

			char bright=0;
			unsigned long skill=0;
			for(int sk=0;sk<SKILLNUM;sk++)
				{
				skill+=(unsigned long)temppool[p]->skill[sk];
				if(temppool[p]->skill_ip[sk]>=100)bright=1;
				}

			set_color(COLOR_WHITE,COLOR_BLACK,bright);

			move(y,25);
			itoa(skill,num,10);
			addstr(num);

			printhealthstat(*temppool[p],y,33,TRUE);

			if(mode==REVIEWMODE_JUSTICE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,42);
			if(temppool[p]->location==-1)
				{
				addstr("Away");
				}
			else
				{
				addstr(location[temppool[p]->location]->shortname);
				}

			move(y,57);
			switch(mode)
				{
				case REVIEWMODE_LIBERALS:
					{
					char usepers=1;
					if(temppool[p]->squadid!=-1)
						{
						int sq=getsquad(temppool[p]->squadid);
						if(sq!=-1)
							{
							if(squad[sq]->activity.type!=ACTIVITY_NONE)
								{
								set_color(COLOR_GREEN,COLOR_BLACK,1);
								addstr("SQUAD");
								usepers=0;
								}
							}
						}
					if(usepers)
						{
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						char str[80];
						getactivity(str,temppool[p]->activity);
						addstr(str);
						}
					break;
					}
				case REVIEWMODE_HOSTAGES:
					{
					set_color(COLOR_MAGENTA,COLOR_BLACK,1);
					char num[20];
					itoa(temppool[p]->joindays,num,10);
					addstr(num);
					addstr(" ");
					if(temppool[p]->joindays>1)addstr("Days");
					else addstr("Day");
					break;
					}
				case REVIEWMODE_JUSTICE:
					{
					if(temppool[p]->deathpenalty&&temppool[p]->sentence!=0)
						{
						set_color(COLOR_RED,COLOR_BLACK,1);
						addstr("DEATH ROW: ");
						itoa(temppool[p]->sentence,num,10);
						addstr(num);
						addstr(" ");
						if(temppool[p]->sentence>1)addstr("Months");
						else addstr("Month");
						}
					else if(temppool[p]->sentence==-1)
						{
						set_color(COLOR_WHITE,COLOR_BLACK,0);
						addstr("Life Sentence");
						}
					else if(temppool[p]->sentence!=0)
						{
						set_color(COLOR_YELLOW,COLOR_BLACK,1);
						char num[20];
						itoa(temppool[p]->sentence,num,10);
						addstr(num);
						addstr(" ");
						if(temppool[p]->sentence>1)addstr("Months");
						else addstr("Month");
						}
					else
						{
						set_color(COLOR_BLACK,COLOR_BLACK,1);
						addstr("-------");
						}
					break;
					}
				case REVIEWMODE_CLINIC:
					{
					set_color(COLOR_CYAN,COLOR_BLACK,1);
					char num[20];
					addstr("Out in ");
					itoa(temppool[p]->clinic,num,10);
					addstr(num);
					addstr(" ");
					if(temppool[p]->clinic>1)addstr("Months");
					else addstr("Month");
					break;
					}
				case REVIEWMODE_SLEEPERS:
					{
					char str[200];
					if(temppool[p]->align==-1)set_color(COLOR_RED,COLOR_BLACK,1);
					else if(temppool[p]->align==0)set_color(COLOR_WHITE,COLOR_BLACK,1);
					else set_color(COLOR_GREEN,COLOR_BLACK,1);
					getrecruitcreature(str,temppool[p]->type);
					addstr(str);
					break;
					}
				case REVIEWMODE_DEAD:
					{
					set_color(COLOR_MAGENTA,COLOR_BLACK,1);
					char num[20];
					itoa(temppool[p]->deathdays,num,10);
					addstr(num);
					addstr(" ");
					if(temppool[p]->deathdays>1)addstr("Days");
					else addstr("Day");
					break;
					}
				case REVIEWMODE_AWAY:
					{
					set_color(COLOR_BLUE,COLOR_BLACK,1);
					char num[20];
					itoa(temppool[p]->dating+
						temppool[p]->hiding,num,10);
					addstr(num);
					addstr(" ");
					if(temppool[p]->dating+
						temppool[p]->hiding>1)addstr("Days");
					else addstr("Day");
					break;
					}
				}

			y++;
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(22,0);
		addstr("Press a Letter to View Status.");
		move(23,0);
		if(interface_pgup=='[')
			{
			addstr("[] to view other Liberal pages.");
			}
		else
			{
			addstr("PGUP/PGDN to view other Liberal pages.");
			}

		refresh();

		int c=getch();
		translategetch(c);

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*19<temppool.size())page++;

		if(c>='a'&&c<='s')
			{
			int p=page*19+(int)(c-'a');
			if(p<temppool.size())
				{
				do
					{
					erase();

					move(0,0);
					if(temppool[p]->align!=1)
						{
						set_color(COLOR_RED,COLOR_BLACK,1);
						addstr("Profile of an Automaton");
						}
					else
						{
						set_color(COLOR_GREEN,COLOR_BLACK,1);
						addstr("Profile of a Liberal");
						}

					printliberalstats(*temppool[p],0);

					move(23,0);
					if(temppool[p]->align!=1)addstr("Press N to change this Automaton's Code Name");
					else addstr("Press N to change this Liberal's Code Name");
					if(temppool.size()>1)
						{
						if(interface_pgup=='[')
							{
							addstr(", [] to view the others");
							}
						else
							{
							addstr(", PGUP/PGDN to view the others");
							}
						}
					move(24,0);
					addstr("Press any other key to continue the Struggle");

					refresh();
					int c=getch();
					translategetch(c);

					if(temppool.size()>0&&(c==interface_pgup||c==interface_pgdn))
						{
						int sx=1;
						if(c==interface_pgup)sx=-1;
						p=(p+(int)temppool.size()+sx)%((int)temppool.size());
						continue;
						}

					if(c=='n')
						{
						move(23,0);
						set_color(COLOR_WHITE,COLOR_BLACK,0);
						addstr("What is the new code name?                          ");
						move(24,0);
						addstr("                                                    ");

						keypad(stdscr,FALSE);
						raw_output(FALSE);
						echo();
						curs_set(1);
						mvgetstr(24,0,temppool[p]->name);
						curs_set(0);
						noecho();
						raw_output(TRUE);
						keypad(stdscr,TRUE);
						}
					else break;
					}while(1);
				}
			}

		if(c==10)break;
		}while(1);
}

long getsquad(long id)
{
	for(int sq=0;sq<squad.size();sq++)
		{
		if(squad[sq]->id==id)return sq;
		}

	return -1;
}

void hospital(int loc)
{
	locatesquad(activesquad,loc);

	int partysize=0;
	int partydead=0;
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			partysize++;
			}
		}

	do
		{
		erase();

		locheader();
		printparty();

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(10,1);
		addstr("F - Go in and fix up Conservative wounds");

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(12,1);
		addstr("Enter - Leave");

		if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(13,1);
		addstr("# - Check the status of a squad Liberal");
		if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(14,1);
		addstr("0 - Show the squad's Liberal status");

		int c=getch();
		translategetch(c);

		if(c==10)break;

		if(c=='0')party_status=-1;

		if(c>='1'&&c<='6'&&activesquad!=NULL)
			{
			if(activesquad->squad[c-'1']!=NULL)
				{
				if(party_status==c-'1')fullstatus(party_status);
				else party_status=c-'1';
				}
			}

		if(c=='f')
			{
			for(int p=5;p>=0;p--)
				{
				if(activesquad->squad[p]==NULL)continue;
				if(!activesquad->squad[p]->alive)continue;

				int time=clinictime(*activesquad->squad[p]);

				if(time>0)
					{
					activesquad->squad[p]->clinic=time;
					activesquad->squad[p]->squadid=-1;
					activesquad->squad[p]->location=loc;

					char num[20];

					makedelimiter(8,0);
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(8,1);
					addstr(activesquad->squad[p]->name);
					addstr(" will be at ");
					addstr(location[loc]->name);
					addstr(" for ");
					itoa(time,num,10);
					addstr(num);
					addstr(" ");
					if(time>1)addstr("months");
					else addstr("month");
					addstr(".");

					refresh();
					getch();

					int obase=activesquad->squad[p]->base;

					activesquad->squad[p]=NULL;

					for(int i=0;i<5;i++)
						{
						for(int p2=1;p2<6;p2++)
							{
							if(activesquad->squad[p2-1]==NULL&&activesquad->squad[p2]!=NULL)
								{
								activesquad->squad[p2-1]=activesquad->squad[p2];
								activesquad->squad[p2]=NULL;
								}
							}
						}

					//SPLIT IF SQUAD IS GONE
					char hasmembers=0;
					for(int p2=0;p2<6;p2++)
						{
						if(activesquad->squad[p2]!=NULL)hasmembers=1;
						}
					if(!hasmembers)
						{
						//LOSE ALL CARS
						for(int p2=0;p2<6;p2++)
							{
							if(activesquad->squad[p2]==NULL)continue;
							if(activesquad->squad[p2]->carid!=-1)
								{
								long v=id_getcar(activesquad->squad[p2]->carid);
								if(v!=-1)
									{
									removecarprefs_pool(vehicle[v]->id);
									delete vehicle[v];
									vehicle.erase(vehicle.begin() + v);
									}
								}
							}

						//RETURN ALL LOOT ITEMS TO BASE
						for(int l=0;l<activesquad->loot.size();l++)
							{
							location[obase]->loot.push_back(activesquad->loot[l]);
							}
						activesquad->loot.clear();
						return;
						}
					}
				}
			}
		}while(1);
}

void deptstore(int loc)
{
	short buyer=0;

	locatesquad(activesquad,loc);

	int partysize=0;
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			partysize++;
			}
		}

	do
		{
		int weaponbought=-1;
		int armorbought=-1;

		erase();

		locheader();
		printparty();

		move(8,60);
		addstr("Buyer: ");
		addstr(activesquad->squad[buyer]->name);

		if(funds>=400)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(10,1);
		addstr("C - Buy a Cheap Suit         ($400)");

		if(funds>=5000)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(11,1);
		addstr("V - Buy an Expensive Suit   ($5000)");

		if(funds>=500)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(12,1);
		addstr("S - Buy a Black Suit         ($500)");

		if(funds>=400)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(10,40);
		addstr("H - Buy a Cheap Dress        ($400)");

		if(funds>=5000)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(11,40);
		addstr("D - Buy an Expensive Dress  ($5000)");

		if(funds>=500)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(12,40);
		addstr("R - Buy a Black Dress        ($500)");

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(15,1);
		addstr("E - Look over Equipment");
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(15,30);
		addstr("Enter - Leave");
		if(partysize>=2)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(15,50);
		addstr("B - Choose a buyer");
		if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(14,1);
		addstr("0 - Show the squad's Liberal status");
		if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(14,40);
		addstr("# - Check the status of a squad Liberal");

		int c=getch();
		translategetch(c);

		if(c==10)break;

		if(funds>=400&&c=='c')
			{
			armorbought=ARMOR_CHEAPSUIT;
			funds-=400;
			stat_spent+=400;
			moneylost_goods+=400;
			}
		if(funds>=5000&&c=='v')
			{
			armorbought=ARMOR_EXPENSIVESUIT;
			funds-=5000;
			stat_spent+=5000;
			moneylost_goods+=5000;
			}
		if(funds>=500&&c=='s')
			{
			armorbought=ARMOR_BLACKSUIT;
			funds-=500;
			stat_spent+=500;
			moneylost_goods+=500;
			}
		if(funds>=400&&c=='h')
			{
			armorbought=ARMOR_CHEAPDRESS;
			funds-=400;
			stat_spent+=400;
			moneylost_goods+=400;
			}
		if(funds>=5000&&c=='d')
			{
			armorbought=ARMOR_EXPENSIVEDRESS;
			funds-=5000;
			stat_spent+=5000;
			moneylost_goods+=5000;
			}
		if(funds>=500&&c=='r')
			{
			armorbought=ARMOR_BLACKDRESS;
			funds-=500;
			stat_spent+=500;
			moneylost_goods+=500;
			}

		if(armorbought!=-1)
			{
			armorst swap=activesquad->squad[buyer]->armor;
			activesquad->squad[buyer]->armor.type=armorbought;

			if(swap.type!=ARMOR_NONE)
				{
				itemst *newi=new itemst;
					newi->type=ITEM_ARMOR;
					newi->armor=swap;
				activesquad->loot.push_back(newi);
				}
			}
		if(weaponbought!=-1)
			{
			weaponst swap=activesquad->squad[buyer]->weapon;
			activesquad->squad[buyer]->weapon.type=weaponbought;
			activesquad->squad[buyer]->weapon.ammo=0;

			if(swap.type!=WEAPON_NONE)
				{
				itemst *newi=new itemst;
					newi->type=ITEM_WEAPON;
					newi->weapon=swap;
				activesquad->loot.push_back(newi);
				}

			//DROP ALL CLIPS THAT DON'T WORK
			for(int cl=0;cl<CLIPNUM;cl++)
				{
				if(cl==ammotype(activesquad->squad[buyer]->weapon.type))continue;

				for(int p2=0;p2<activesquad->squad[buyer]->clip[cl];p2++)
					{
					itemst *newi=new itemst;
						newi->type=ITEM_CLIP;
						newi->cliptype=cl;
					activesquad->loot.push_back(newi);
					}

				activesquad->squad[buyer]->clip[cl]=0;
				}
			}

		if(c=='e')equip(activesquad->loot,-1);
		if(c=='b')choose_buyer(buyer);
		if(c=='0')party_status=-1;
		if(c>='1'&&c<='6'&&activesquad!=NULL)
			{
			if(activesquad->squad[c-'1']!=NULL)
				{
				if(party_status==c-'1')fullstatus(party_status);
				else party_status=c-'1';
				}
			}

		}while(1);
}

void halloweenstore(int loc)
{
	short buyer=0;
	short in_halloween=0;

	locatesquad(activesquad,loc);

	int partysize=0;
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			partysize++;
			}
		}

	do
		{
		int weaponbought=-1;
		int armorbought=-1,armorbought2=-1;

		erase();

		locheader();
		printparty();

		move(8,60);
		addstr("Buyer: ");
		addstr(activesquad->squad[buyer]->name);

		if(in_halloween==1)
			{
			if(funds>=70)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,1);
			addstr("T - Buy a Trench Coat         ($70)");

			if(funds>=50)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,1);
			addstr("W - Buy Work Clothes          ($50)");

			if(funds>=200)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(12,1);
			addstr("L - Buy a Lab Coat           ($200)");

			if(funds>=200)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(13,1);
			addstr("R - Buy a Black Judge's Robe ($200)");

			if(funds>=200)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,40);
			addstr("C - Buy a Clown Suit         ($200)");

			if(funds>=350)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,40);
			addstr("G - Buy Bondage Gear         ($350)");

			if(funds>=15)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(12,40);
			addstr("M - Buy a Mask                ($15)");

			if(funds>=90)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(13,40);
			addstr("O - Buy a Toga                ($90)");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(15,30);
			addstr("Enter - Done");
			}
		else if(in_halloween==2)
			{
			if(funds>=20)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,1);
			addstr("K - Buy a Knife                 ($20)");

			if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,1);
			addstr("S - Buy the Sword of Morfiegor ($250)");

			if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(12,1);
			addstr("A - Buy the Axe of Kirgar      ($250)");

			if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(13,1);
			addstr("H - Buy a Dwarven Hammer       ($250)");

			if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,40);
			addstr("M - Buy the Maul of Anrin      ($250)");

			if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,40);
			addstr("C - Buy a Silver Cross         ($250)");

			if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(12,40);
			addstr("W - Buy a Wizard's Staff       ($250)");

			if(funds>=10000)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(13,40);
			addstr("! - Buy Mithril Mail         ($10000)");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(15,30);
			addstr("Enter - Done");
			}
		else
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(10,1);
			addstr("C - Purchase Halloween Costumes");
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(11,1);
			addstr("M - Purchase Medieval Gear");
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(15,30);
			addstr("Enter - Leave");
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(15,1);
		addstr("E - Look over Equipment");
		if(partysize>=2)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(15,50);
		addstr("B - Choose a buyer");
		if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(14,1);
		addstr("0 - Show the squad's Liberal status");
		if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(14,40);
		addstr("# - Check the status of a squad Liberal");

		int c=getch();
		translategetch(c);

		if(in_halloween==1)
			{
			if(c==10)in_halloween=0;

			if(c=='t'&&funds>=70)
				{
				armorbought=ARMOR_TRENCHCOAT;
				funds-=70;
				stat_spent+=70;
				moneylost_goods+=70;
				}

			if(c=='w'&&funds>=50)
				{
				armorbought=ARMOR_WORKCLOTHES;
				funds-=50;
				stat_spent+=50;
				moneylost_goods+=50;
				}

			if(c=='l'&&funds>=200)
				{
				armorbought=ARMOR_LABCOAT;
				funds-=200;
				stat_spent+=200;
				moneylost_goods+=200;
				}

			if(c=='r'&&funds>=200)
				{
				armorbought=ARMOR_BLACKROBE;
				funds-=200;
				stat_spent+=200;
				moneylost_goods+=200;
				}

			if(c=='c'&&funds>=200)
				{
				armorbought=ARMOR_CLOWNSUIT;
				funds-=200;
				stat_spent+=200;
				moneylost_goods+=200;
				}

			if(c=='g'&&funds>=350)
				{
				armorbought=ARMOR_BONDAGEGEAR;
				funds-=350;
				stat_spent+=350;
				moneylost_goods+=350;
				}

			if(c=='m'&&funds>=15)
				{
				short mask;
				if(maskselect(activesquad->squad[buyer],mask))
					{
					armorbought=ARMOR_MASK;
					armorbought2=mask;
					funds-=15;
					stat_spent+=15;
					moneylost_goods+=15;
					}
				}

			if(c=='o'&&funds>=90)
				{
				armorbought=ARMOR_TOGA;
				funds-=90;
				stat_spent+=90;
				moneylost_goods+=90;
				}
			}
		else if(in_halloween==2)
			{
			if(c==10)in_halloween=0;

			if(c=='k'&&funds>=20)
				{
				weaponbought=WEAPON_KNIFE;
				funds-=20;
				stat_spent+=20;
				moneylost_goods+=20;
				}

			if(c=='s'&&funds>=250)
				{
				weaponbought=WEAPON_SWORD;
				funds-=250;
				stat_spent+=250;
				moneylost_goods+=250;
				}

			if(c=='a'&&funds>=250)
				{
				weaponbought=WEAPON_AXE;
				funds-=250;
				stat_spent+=250;
				moneylost_goods+=250;
				}

			if(c=='h'&&funds>=250)
				{
				weaponbought=WEAPON_HAMMER;
				funds-=250;
				stat_spent+=250;
				moneylost_goods+=250;
				}

			if(c=='m'&&funds>=250)
				{
				weaponbought=WEAPON_MAUL;
				funds-=250;
				stat_spent+=250;
				moneylost_goods+=250;
				}

			if(c=='c'&&funds>=250)
				{
				weaponbought=WEAPON_CROSS;
				funds-=250;
				stat_spent+=250;
				moneylost_goods+=250;
				}

			if(c=='w'&&funds>=250)
				{
				weaponbought=WEAPON_STAFF;
				funds-=250;
				stat_spent+=250;
				moneylost_goods+=250;
				}

			if(c=='!'&&funds>=10000)
				{
				armorbought=ARMOR_MITHRIL;
				funds-=10000;
				stat_spent+=10000;
				moneylost_goods+=10000;
				}
			}
		else
			{
			if(c==10)break;
			if(c=='c')in_halloween=1;
			if(c=='m')in_halloween=2;
			}

		if(armorbought!=-1)
			{
			armorst swap=activesquad->squad[buyer]->armor;
			activesquad->squad[buyer]->armor.type=armorbought;
			activesquad->squad[buyer]->armor.subtype=armorbought2;

			if(swap.type!=ARMOR_NONE)
				{
				itemst *newi=new itemst;
					newi->type=ITEM_ARMOR;
					newi->armor=swap;
				activesquad->loot.push_back(newi);
				}
			}
		if(weaponbought!=-1)
			{
			weaponst swap=activesquad->squad[buyer]->weapon;
			activesquad->squad[buyer]->weapon.type=weaponbought;
			activesquad->squad[buyer]->weapon.ammo=0;

			if(swap.type!=WEAPON_NONE)
				{
				itemst *newi=new itemst;
					newi->type=ITEM_WEAPON;
					newi->weapon=swap;
				activesquad->loot.push_back(newi);
				}

			//DROP ALL CLIPS THAT DON'T WORK
			for(int cl=0;cl<CLIPNUM;cl++)
				{
				if(cl==ammotype(activesquad->squad[buyer]->weapon.type))continue;

				for(int p2=0;p2<activesquad->squad[buyer]->clip[cl];p2++)
					{
					itemst *newi=new itemst;
						newi->type=ITEM_CLIP;
						newi->cliptype=cl;
					activesquad->loot.push_back(newi);
					}

				activesquad->squad[buyer]->clip[cl]=0;
				}
			}

		if(c=='e')equip(activesquad->loot,-1);
		if(c=='b')choose_buyer(buyer);
		if(c=='0')party_status=-1;
		if(c>='1'&&c<='6'&&activesquad!=NULL)
			{
			if(activesquad->squad[c-'1']!=NULL)
				{
				if(party_status==c-'1')fullstatus(party_status);
				else party_status=c-'1';
				}
			}

		}while(1);
}

void pawnshop(int loc)
{
	short buyer=0;
	short in_tools=0;
	short in_gunshop=0;
	short in_fence=0;
	int l;

	locatesquad(activesquad,loc);

	int partysize=0;
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			partysize++;
			}
		}

	do
		{
		erase();

		locheader();
		printparty();

		move(8,60);
		addstr("Buyer: ");
		addstr(activesquad->squad[buyer]->name);

		if(in_fence==1)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(10,1);
			addstr("E - Look over Equipment");

			if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,40);
			addstr("F - Pawn Selectively");

			if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,1);
			addstr("P - Pawn all Weapons");

			if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,40);
			addstr("R - Pawn all Armor");

			if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(12,1);
			addstr("W - Pawn all extra Weapons");

			if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(12,40);
			addstr("A - Pawn all extra Armor");

			if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(13,1);
			addstr("C - Pawn all extra Clips");

			if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(13,40);
			addstr("L - Pawn Loot");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(15,40);
			addstr("Enter - Done pawning");
			}
		else if(in_gunshop==2)
			{
			if(funds>=15)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,1);
			addstr("T - Buy a .22 Speedloader     ($15)");
			if(funds>=20)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,40);
			addstr("M - Buy a .44 Speedloader     ($20)");
			if(funds>=30)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,1);
			addstr("N - Buy a 9mm Clip            ($30)");
			if(funds>=30)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,40);
			addstr("F - Buy a .45 Clip            ($30)");
			if(funds>=10)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(12,1);
			addstr("P - Buy a Pair of Buckshots   ($10)");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(15,40);
			addstr("Enter - Done buying Liberal clips");
			}
		else if(in_gunshop==1)
			{
			if(funds>=150)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,1);
			addstr("T - Buy a .22 Revolver        ($150)");
			if(funds>=300)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,40);
			addstr("M - Buy a .44 Magnum          ($300)");
			if(funds>=300)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,1);
			addstr("N - Buy a 9mm Semi-automatic  ($300)");
			if(funds>=300)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,40);
			addstr("F - Buy a .45 Semi-automatic  ($300)");
			if(funds>=300)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(12,1);
			addstr("G - Buy a Shotgun             ($300)");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(15,40);
			addstr("Enter - Done buying Liberal guns");
			}
		else if(in_tools==1)
			{
			if(funds>=20)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,1);
			addstr("C - Buy a Crowbar              ($20)");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(15,40);
			addstr("Enter - Leave");
			}
		else
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(10,1);
			addstr("G - Buy a Liberal gun");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(10,40);
			addstr("C - Buy Liberal clips");

			move(11,1);
			addstr("T - Buy Liberal tools");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(12,1);
			addstr("E - Look over Equipment");

			set_color(COLOR_WHITE,COLOR_BLACK,0);

			if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(13,1);
			addstr("S - Sell something");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(15,40);
			addstr("Enter - Leave");
			}

		if(partysize>=2)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(15,1);
		addstr("B - Choose a buyer");

		if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(14,1);
		addstr("0 - Show the squad's Liberal status");
		if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(14,40);
		addstr("# - Check the status of a squad Liberal");

		int c=getch();
		translategetch(c);

		if(in_fence)
			{
			if(c==10)in_fence=0;

			if(c=='e')equip(activesquad->loot,-1);

			if((c=='p'||c=='r'||c=='w'||c=='a'||c=='l'||c=='f')&&
				activesquad->loot.size()>0)
				{
				unsigned long fenceamount=0;

				if(c=='f')fenceamount=fenceselect();
				else
					{
					int fenceweapon[WEAPONNUM];
					int fencearmor[ARMORNUM];
					int fenceclip[CLIPNUM];
					int fenceloot[LOOTNUM];

					memset(fenceweapon,0,WEAPONNUM*sizeof(int));
					memset(fencearmor,0,ARMORNUM*sizeof(int));
					memset(fenceclip,0,CLIPNUM*sizeof(int));
					memset(fenceloot,0,LOOTNUM*sizeof(int));

					for(l=activesquad->loot.size()-1;l>=0;l--)
						{
						switch(activesquad->loot[l]->type)
							{
							case ITEM_WEAPON:fenceweapon[activesquad->loot[l]->weapon.type]+=activesquad->loot[l]->number;break;
							case ITEM_ARMOR:
								if(activesquad->loot[l]->armor.quality!='1')break;
								if(activesquad->loot[l]->armor.flag!=0)break;
								fencearmor[activesquad->loot[l]->armor.type]+=activesquad->loot[l]->number;
								break;
							case ITEM_CLIP:fenceclip[activesquad->loot[l]->cliptype]+=activesquad->loot[l]->number;break;
							case ITEM_LOOT:fenceloot[activesquad->loot[l]->loottype]+=activesquad->loot[l]->number;break;
							}
						}

					if(c=='l'||c=='a'||c=='r'||c=='c')memset(fenceweapon,0,WEAPONNUM*sizeof(int));
					else if(c=='w')
						{
						for(int w=0;w<WEAPONNUM;w++)
							{
							if(fenceweapon[w]>6)fenceweapon[w]-=6;
							else fenceweapon[w]=0;
							}
						}

					if(c=='l'||c=='p'||c=='w'||c=='c')memset(fencearmor,0,ARMORNUM*sizeof(int));
					else if(c=='a')
						{
						for(int a=0;a<ARMORNUM;a++)
							{
							if(fencearmor[a]>6)fencearmor[a]-=6;
							else fencearmor[a]=0;
							}
						}

					if(c!='c')memset(fenceclip,0,CLIPNUM*sizeof(int));
					else
						{
						for(int c=0;c<CLIPNUM;c++)
							{
							if(fenceclip[c]>54)fenceclip[c]-=54;
							else fenceclip[c]=0;
							}
						}

					if(c!='l')memset(fenceloot,0,LOOTNUM*sizeof(int));
					else
						{
						fenceloot[LOOT_CEOPHOTOS]=0;
						fenceloot[LOOT_INTHQDISK]=0;
						fenceloot[LOOT_CORPFILES]=0;
						fenceloot[LOOT_SECRETDOCUMENTS]=0;
						fenceloot[LOOT_POLICERECORDS]=0;
						}

					for(l=activesquad->loot.size()-1;l>=0;l--)
						{
						switch(activesquad->loot[l]->type)
							{
							case ITEM_WEAPON:
								if(fenceweapon[activesquad->loot[l]->weapon.type]>0)
									{
									long numbersold=fenceweapon[activesquad->loot[l]->weapon.type];
									if(numbersold>activesquad->loot[l]->number)numbersold=activesquad->loot[l]->number;
									fenceweapon[activesquad->loot[l]->weapon.type]-=numbersold;
									activesquad->loot[l]->number-=numbersold;
									fenceamount+=fencevalue(*activesquad->loot[l])*numbersold;

									if(activesquad->loot[l]->number==0)
										{
										delete activesquad->loot[l];
										activesquad->loot.erase(activesquad->loot.begin() + l);
										}
									}
								break;
							case ITEM_ARMOR:
								if(activesquad->loot[l]->armor.quality!='1')break;
								if(activesquad->loot[l]->armor.flag!=0)break;
								if(fencearmor[activesquad->loot[l]->armor.type]>0)
									{
									long numbersold=fencearmor[activesquad->loot[l]->armor.type];
									if(numbersold>activesquad->loot[l]->number)numbersold=activesquad->loot[l]->number;
									fencearmor[activesquad->loot[l]->armor.type]-=numbersold;
									activesquad->loot[l]->number-=numbersold;
									fenceamount+=fencevalue(*activesquad->loot[l])*numbersold;

									if(activesquad->loot[l]->number==0)
										{
										delete activesquad->loot[l];
										activesquad->loot.erase(activesquad->loot.begin() + l);
										}
									}
								break;
							case ITEM_CLIP:
								if(fenceclip[activesquad->loot[l]->cliptype]>0)
									{
									long numbersold=fenceclip[activesquad->loot[l]->cliptype];
									if(numbersold>activesquad->loot[l]->number)numbersold=activesquad->loot[l]->number;
									fenceclip[activesquad->loot[l]->cliptype]-=numbersold;
									activesquad->loot[l]->number-=numbersold;
									fenceamount+=fencevalue(*activesquad->loot[l])*numbersold;

									if(activesquad->loot[l]->number==0)
										{
										delete activesquad->loot[l];
										activesquad->loot.erase(activesquad->loot.begin() + l);
										}
									}
								break;
							case ITEM_LOOT:
								if(fenceloot[activesquad->loot[l]->loottype]>0)
									{
									long numbersold=fenceloot[activesquad->loot[l]->loottype];
									if(numbersold>activesquad->loot[l]->number)numbersold=activesquad->loot[l]->number;
									fenceloot[activesquad->loot[l]->loottype]-=numbersold;
									activesquad->loot[l]->number-=numbersold;
									fenceamount+=fencevalue(*activesquad->loot[l])*numbersold;

									if(activesquad->loot[l]->number==0)
										{
										delete activesquad->loot[l];
										activesquad->loot.erase(activesquad->loot.begin() + l);
										}
									}
								break;
							}
						}
					}

				if(fenceamount>0)
					{
					set_color(COLOR_WHITE,COLOR_BLACK,1);

					char num[20];

					move(8,1);
					addstr("You add $");
					itoa(fenceamount,num,10);
					addstr(num);addstr(" to Liberal Funds.");

					refresh();
					getch();

					funds+=fenceamount;
					stat_funds+=fenceamount;
					moneygained_goods+=fenceamount;
					}
				}
			}
		else if(in_gunshop==1)
			{
			int gunbought=-1;

			if(funds>=150&&c=='t')
				{
				gunbought=WEAPON_REVOLVER_22;
				funds-=150;
				stat_spent+=150;
				moneylost_goods+=150;
				}
			if(funds>=300&&c=='m')
				{
				gunbought=WEAPON_REVOLVER_44;
				funds-=300;
				stat_spent+=300;
				moneylost_goods+=300;
				}
			if(funds>=300&&c=='n')
				{
				gunbought=WEAPON_SEMIPISTOL_9MM;
				funds-=300;
				stat_spent+=300;
				moneylost_goods+=300;
				}
			if(funds>=300&&c=='f')
				{
				gunbought=WEAPON_SEMIPISTOL_45;
				funds-=300;
				stat_spent+=300;
				moneylost_goods+=300;
				}
			if(funds>=300&&c=='g')
				{
				gunbought=WEAPON_SHOTGUN;
				funds-=300;
				stat_spent+=300;
				moneylost_goods+=300;
				}

			if(gunbought!=-1)
				{
				weaponst swap=activesquad->squad[buyer]->weapon;
				activesquad->squad[buyer]->weapon.type=gunbought;
				activesquad->squad[buyer]->weapon.ammo=0;

				if(swap.type!=WEAPON_NONE)
					{
					itemst *newi=new itemst;
						newi->type=ITEM_WEAPON;
						newi->weapon=swap;
					activesquad->loot.push_back(newi);
					}

				//DROP ALL CLIPS THAT DON'T WORK
				for(int cl=0;cl<CLIPNUM;cl++)
					{
					if(cl==ammotype(activesquad->squad[buyer]->weapon.type))continue;

					for(int p2=0;p2<activesquad->squad[buyer]->clip[cl];p2++)
						{
						itemst *newi=new itemst;
							newi->type=ITEM_CLIP;
							newi->cliptype=cl;
						activesquad->loot.push_back(newi);
						}

					activesquad->squad[buyer]->clip[cl]=0;
					}
				}

			if(c==10)in_gunshop=0;
			}
		else if(in_gunshop==2)
			{
			int clipbought=-1;

			if(funds>=15&&c=='t')
				{
				clipbought=CLIP_22;
				funds-=15;
				stat_spent+=15;
				moneylost_goods+=15;
				}
			if(funds>=20&&c=='m')
				{
				clipbought=CLIP_44;
				funds-=20;
				stat_spent+=20;
				moneylost_goods+=20;
				}
			if(funds>=30&&c=='n')
				{
				clipbought=CLIP_9;
				funds-=30;
				stat_spent+=30;
				moneylost_goods+=30;
				}
			if(funds>=30&&c=='f')
				{
				clipbought=CLIP_45;
				funds-=30;
				stat_spent+=30;
				moneylost_goods+=30;
				}
			if(funds>=10&&c=='p')
				{
				clipbought=CLIP_BUCKSHOT;
				funds-=10;
				stat_spent+=10;
				moneylost_goods+=10;
				}

			char conf=1;

			if(ammotype(activesquad->squad[buyer]->weapon.type)==clipbought&&clipbought!=-1)
				{
				if(activesquad->squad[buyer]->clip[clipbought]<9)
					{
					activesquad->squad[buyer]->clip[clipbought]++;
					conf=0;
					}
				}

			if(conf&&clipbought!=-1)
				{
				itemst *newi=new itemst;
					newi->type=ITEM_CLIP;
					newi->cliptype=clipbought;
				activesquad->loot.push_back(newi);
				}

			if(c==10)in_gunshop=0;
			}
		else if(in_tools==1)
			{
			if(c=='c'&&funds>=20)
				{
				funds-=20;
				stat_spent+=20;
				moneylost_goods+=20;

				itemst *newi=new itemst;
					newi->type=ITEM_WEAPON;
					newi->weapon.type=WEAPON_CROWBAR;
				activesquad->loot.push_back(newi);
				}
			if(c==10)in_tools=0;
			}
		else
			{
			if(c==10)break;
			if(c=='c')in_gunshop=2;
			if(c=='g')in_gunshop=1;
			if(c=='e')equip(activesquad->loot,-1);
			if(c=='s')in_fence=1;
			if(c=='t')in_tools=1;
			}

		if(c=='b')choose_buyer(buyer);

		if(c=='0')party_status=-1;

		if(c>='1'&&c<='6'&&activesquad!=NULL)
			{
			if(activesquad->squad[c-'1']!=NULL)
				{
				if(party_status==c-'1')fullstatus(party_status);
				else party_status=c-'1';
				}
			}

		}while(1);
}

void locheader(void)
{
	char num[20];

	if(activesquad!=NULL)
		{
		if(location[activesquad->squad[0]->location]->siege.siege)
			{
			if(location[activesquad->squad[0]->location]->siege.underattack)set_color(COLOR_RED,COLOR_BLACK,1);
			else set_color(COLOR_YELLOW,COLOR_BLACK,1);
			}
		else set_color(COLOR_WHITE,COLOR_BLACK,0);
		}
	else if(selectedsiege!=-1)
		{
		if(location[selectedsiege]->siege.siege)
			{
			if(location[selectedsiege]->siege.underattack)set_color(COLOR_RED,COLOR_BLACK,1);
			else set_color(COLOR_YELLOW,COLOR_BLACK,1);
			}
		else set_color(COLOR_WHITE,COLOR_BLACK,0);
		}
	else set_color(COLOR_WHITE,COLOR_BLACK,0);
	move(0,0);

	if(activesquad!=NULL)
		{
		addlocationname(location[activesquad->squad[0]->location]);
		addstr(", ");
		}
	else
		{
		if(selectedsiege==-1)
			{
			addstr("No Squad Selected");
			addstr(", ");
			}
		else
			{
			addlocationname(location[selectedsiege]);
			addstr(", ");
			}
		}

	switch(month)
		{
		case 1:addstr("Jan");break;
		case 2:addstr("Feb");break;
		case 3:addstr("Mar");break;
		case 4:addstr("Apr");break;
		case 5:addstr("May");break;
		case 6:addstr("Jun");break;
		case 7:addstr("Jul");break;
		case 8:addstr("Aug");break;
		case 9:addstr("Sep");break;
		case 10:addstr("Oct");break;
		case 11:addstr("Nov");break;
		case 12:addstr("Dec");break;
		}
	addstr(". ");
	itoa(day,num,10);
	addstr(num);
	addstr(", ");
	itoa(year,num,10);
	addstr(num);

	set_color(COLOR_GREEN,COLOR_BLACK,1);
	itoa(funds,num,10);
	move(0,78-strlen(num));
	addstr("$");
	addstr(num);

	if(activesquad!=NULL)
		{
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		char str[80];
		getactivity(str,activesquad->activity);

		if(activesquad->activity.type==ACTIVITY_NONE)
			{
			int count=0;char haveact=0;
			for(int p=0;p<6;p++)
				{
				if(activesquad->squad[p]==NULL)continue;
				count++;
				if(activesquad->squad[p]->activity.type!=ACTIVITY_NONE)
					{
					getactivity(str,activesquad->squad[p]->activity);
					haveact=1;
					}
				}
			if(haveact&&count>1)strcpy(str,"Acting Individually");
			}

		move(0,46);
		addstr(str);
		}
}

void moveloot(vector<itemst *> &dest,vector<itemst *> &source)
{
	unsigned long ret=0;

	int page=0;

	vector<char> selected;
	selected.resize(source.size());
	for(int s=0;s<selected.size();s++)selected[s]=0;

	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addstr("Select Objects");

		printparty();

		int x=1,y=10;
		char str[200],str2[200];

		for(int l=page*18;l<source.size()&&l<page*18+18;l++)
			{
			if(selected[l])set_color(COLOR_GREEN,COLOR_BLACK,1);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);
			if(source[l]->type==ITEM_WEAPON)
				{
				getweaponfull(str2,source[l]->weapon.type);
				if(source[l]->weapon.ammo>0)
					{
					char num[20];
					itoa(source[l]->weapon.ammo,num,10);
					strcat(str2," (");
					strcat(str2,num);
					strcat(str2,")");
					}
				}
			if(source[l]->type==ITEM_ARMOR)
				{
				getarmorfull(str2,source[l]->armor,0);
				}
			if(source[l]->type==ITEM_CLIP)
				{
				getclip(str2,source[l]->cliptype);
				}
			if(source[l]->type==ITEM_LOOT)
				{
				getloot(str2,source[l]->loottype);
				}
			if(source[l]->number>1)
				{
				char num[20];
				strcat(str2," ");
				if(selected[l]>0)
					{
					itoa(selected[l],num,10);
					strcat(str2,num);
					strcat(str2,"/");
					}
				else strcat(str2,"x");
				itoa(source[l]->number,num,10);
				strcat(str2,num);
				}

			str[0]=l-page*18+'A';
			str[1]='\x0';
			strcat(str," - ");
			strcat(str,str2);

			move(y,x);
			addstr(str);

			x+=26;
			if(x>53)
				{
				x=1;
				y++;
				}
			}

		//PAGE UP
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		if(page>0)
			{
			move(17,1);
			if(interface_pgup=='[')
				{
				addstr("[ - Previous");
				}
			else
				{
				addstr("PGUP - Previous");
				}
			}
		//PAGE DOWN
		if((page+1)*18<source.size())
			{
			move(17,53);
			if(interface_pgup=='[')
				{
				addstr("] - Next");
				}
			else
				{
				addstr("PGDN - Next");
				}
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(23,1);
		addstr("Press a letter to select an item.");
		move(24,1);
		addstr("X - Done");

		refresh();

		int c=getch();
		translategetch(c);

		if(c>='a'&&c<='r')
			{
			int slot=c-'a'+page*18;

			if(slot>=0&&slot<source.size())
				{
				if(selected[slot])
					{
					selected[slot]=0;
					}
				else
					{
					if(source[slot]->number>1)
						{
						selected[slot]=1;

						printparty();

						move(8,15);
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						addstr("     How many?       ");

						refresh();

						char str[100];

						keypad(stdscr,FALSE);
						raw_output(FALSE);
						echo();
						curs_set(1);
						mvgetstr(8,30,str);
						curs_set(0);
						noecho();
						raw_output(TRUE);
						keypad(stdscr,TRUE);

						selected[slot]=atoi(str);
						if(selected[slot]<0)selected[slot]=0;
						else if(selected[slot]>source[slot]->number)selected[slot]=source[slot]->number;
						}
					else selected[slot]=1;
					}
				}
			}

		if(c=='x')break;

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*18<source.size())page++;

		}while(1);

	for(int l=source.size()-1;l>=0;l--)
		{
		if(selected[l]>0)
			{
			itemst *newit=new itemst;
				*newit=*source[l];
				newit->number=selected[l];
			dest.push_back(newit);

			source[l]->number-=selected[l];
			if(source[l]->number<=0)
				{
				delete source[l];
				source.erase(source.begin() + l);
				}
			}
		}
}

void activate(void)
{
	vector<creaturest *> temppool;
	int sq;
	for(int p=0;p<pool.size();p++)
		{
		if(pool[p]->alive&&
			pool[p]->align==1&&
			pool[p]->clinic==0&&
			pool[p]->dating==0&&
			pool[p]->hiding==0&&
			!(pool[p]->flag & CREATUREFLAG_SLEEPER))
			{
			if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
				location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
				location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON)
				{
				if(pool[p]->squadid!=-1)
					{
					sq=getsquad(pool[p]->squadid);
					if(sq!=-1)
						{
						if(squad[sq]->activity.type!=ACTIVITY_NONE)continue;
						}
					}
				temppool.push_back(pool[p]);
				}
			}
		}

	if(temppool.size()==0)return;

	short page=0;

	char str[80];
	char num[20];

	do
		{
		erase();

		set_color(COLOR_GREEN,COLOR_BLACK,1);
		itoa(funds,num,10);
		move(0,78-strlen(num));
		addstr("$");
		addstr(num);

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addstr("Activate Uninvolved Liberals");
		move(1,0);
		addstr("----CODE NAME------------SKILL---HEALTH---LOCATION------------------------------");
		move(1,57);
		addstr("ACTIVITY");

		int y=2;
		for(int p=page*19;p<temppool.size()&&p<page*19+19;p++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addch(y+'A'-2);addstr(" - ");
			addstr(temppool[p]->name);

			char bright=0;
			unsigned long skill=0;
			for(int sk=0;sk<SKILLNUM;sk++)
				{
				skill+=(unsigned long)temppool[p]->skill[sk];
				if(temppool[p]->skill_ip[sk]>=100)bright=1;
				}

			set_color(COLOR_WHITE,COLOR_BLACK,bright);

			move(y,25);
			itoa(skill,num,10);
			addstr(num);

			printhealthstat(*temppool[p],y,33,TRUE);

			if(mode==REVIEWMODE_JUSTICE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,42);
			addstr(location[temppool[p]->location]->shortname);

			move(y,57);
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			getactivity(str,temppool[p]->activity);
			addstr(str);

			y++;
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(22,0);
		addstr("Press a Letter to Assign an Activity.");
		move(23,0);
		if(interface_pgup=='[')
			{
			addstr("[] to view other Liberal pages.");
			}
		else
			{
			addstr("PGUP/PGDN to view other Liberal pages.");
			}
		move(24,0);
		addstr("Press Z to assign simple tasks in bulk.");

		refresh();

		int c=getch();
		translategetch(c);

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*19<temppool.size())page++;

		if(c>='a'&&c<='s')
			{
			int p=page*19+(int)(c-'a');
			if(p<temppool.size())
				{
				activate(temppool[p]);
				}
			}

		if(c=='z')
			{
			activatebulk();
			}

		if(c==10)break;
		}while(1);
}

void activatebulk(void)
{
	vector<creaturest *> temppool;
	int sq;
	for(int p=0;p<pool.size();p++)
		{
		if(pool[p]->alive&&
			pool[p]->align==1&&
			pool[p]->clinic==0&&
			pool[p]->dating==0&&
			pool[p]->hiding==0&&
			!(pool[p]->flag & CREATUREFLAG_SLEEPER))
			{
			if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
				location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
				location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON)
				{
				if(pool[p]->squadid!=-1)
					{
					sq=getsquad(pool[p]->squadid);
					if(sq!=-1)
						{
						if(squad[sq]->activity.type!=ACTIVITY_NONE)continue;
						}
					}
				temppool.push_back(pool[p]);
				}
			}
		}

	if(temppool.size()==0)return;

	short page=0;

	char str[80];
	char num[20];

	int selectedactivity=0;

	do
		{
		erase();

		set_color(COLOR_GREEN,COLOR_BLACK,1);
		itoa(funds,num,10);
		move(0,78-strlen(num));
		addstr("$");
		addstr(num);

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addstr("Activate Uninvolved Liberals");
		move(1,0);
		addstr("----CODE NAME------------CURRENT ACTIVITY---------------------------------------");
		move(1,51);
		addstr("BULK ACTIVITY");

		if(selectedactivity==0)set_color(COLOR_WHITE,COLOR_BLACK,1);
		else set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(2,51);
		addstr("1 - Causing Trouble ($0).");
		if(selectedactivity==1)set_color(COLOR_WHITE,COLOR_BLACK,1);
		else set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(3,51);
		addstr("2 - Causing Trouble ($20).");
		if(selectedactivity==2)set_color(COLOR_WHITE,COLOR_BLACK,1);
		else set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(4,51);
		addstr("3 - Causing Trouble ($50).");
		if(selectedactivity==3)set_color(COLOR_WHITE,COLOR_BLACK,1);
		else set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(5,51);
		addstr("4 - Causing Trouble ($100).");
		if(selectedactivity==4)set_color(COLOR_WHITE,COLOR_BLACK,1);
		else set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(6,51);
		addstr("5 - Causing Trouble ($500).");
		if(selectedactivity==5)set_color(COLOR_WHITE,COLOR_BLACK,1);
		else set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(7,51);
		addstr("6 - Soliciting Donations.");
		if(selectedactivity==6)set_color(COLOR_WHITE,COLOR_BLACK,1);
		else set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(8,51);
		addstr("7 - Selling Brownies.");
		if(selectedactivity==7)set_color(COLOR_WHITE,COLOR_BLACK,1);
		else set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(9,51);
		addstr("8 - Stealing Cars.");

		int y=2;
		for(int p=page*19;p<temppool.size()&&p<page*19+19;p++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addch(y+'A'-2);addstr(" - ");
			addstr(temppool[p]->name);

			move(y,25);
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			getactivity(str,temppool[p]->activity);
			addstr(str);
			if(temppool[p]->activity.type==ACTIVITY_TROUBLE)
				{
				addstr(" ($");
				itoa(temppool[p]->activity.arg,num,10);
				addstr(num);
				addstr(")");
				}

			y++;
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(22,0);
		addstr("Press a Letter to Assign an Activity.  Press a Number to select an Activity.");
		move(23,0);
		if(interface_pgup=='[')
			{
			addstr("[] to view other Liberal pages.");
			}
		else
			{
			addstr("PGUP/PGDN to view other Liberal pages.");
			}

		refresh();

		int c=getch();
		translategetch(c);

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*19<temppool.size())page++;

		if(c>='a'&&c<='s')
			{
			int p=page*19+(int)(c-'a');
			if(p<temppool.size())
				{
				switch(selectedactivity)
					{
					case 0:
						temppool[p]->activity.type=ACTIVITY_TROUBLE;
						temppool[p]->activity.arg=0;
						break;
					case 1:
						temppool[p]->activity.type=ACTIVITY_TROUBLE;
						temppool[p]->activity.arg=20;
						break;
					case 2:
						temppool[p]->activity.type=ACTIVITY_TROUBLE;
						temppool[p]->activity.arg=50;
						break;
					case 3:
						temppool[p]->activity.type=ACTIVITY_TROUBLE;
						temppool[p]->activity.arg=100;
						break;
					case 4:
						temppool[p]->activity.type=ACTIVITY_TROUBLE;
						temppool[p]->activity.arg=500;
						break;
					case 5:
						temppool[p]->activity.type=ACTIVITY_FUNDS_LEGAL;
						break;
					case 6:
						temppool[p]->activity.type=ACTIVITY_FUNDS_ILLEGAL;
						break;
					case 7:
						temppool[p]->activity.type=ACTIVITY_STEALCARS;
						break;
					}
				}
			}
		if(c>='1'&&c<='8')
			{
			selectedactivity=c-'1';
			}

		if(c==10)break;
		}while(1);
}

void activate(creaturest *cr)
{
	int hostagecount=0;
	char havedead=0;
	for(int p=0;p<pool.size();p++)
		{
		if(pool[p]->alive&&pool[p]->align!=1)hostagecount++;
		if(!pool[p]->alive)havedead=1;
		}

	char num[20];

	do
		{
		erase();

		set_color(COLOR_GREEN,COLOR_BLACK,1);
		itoa(funds,num,10);
		move(0,78-strlen(num));
		addstr("$");
		addstr(num);

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addstr("Taking Action:  What will ");
		addstr(cr->name);
		addstr(" be doing today?");

		printcreatureinfo(cr);

		makedelimiter(8,0);

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(10,1);
		addstr("A - Perpetrating random acts of Liberal Disobedience.");

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(11,1);
		addstr("D - Soliciting Donations.");

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(11,40);
		addstr("B - Selling Brownies.");

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(12,1);
		addstr("C - Making Clothing.");

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(12,40);
		addstr("R - Repairing Clothing.");

		if(hostagecount>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(13,1);
		addstr("H - Tending to a Conservative hostage.");

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(13,40);
		addstr("P - Surfing the Net for opinion polls.");

		move(14,1);
		if(cr->canwalk())
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			addstr("S - Stealing a Car.");
			}
		else
			{
			if(!(cr->flag & CREATUREFLAG_WHEELCHAIR))set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			addstr("S - Procuring a Wheelchair.");
			}

		if(havedead)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(15,1);
		addstr("Z - Dispose of bodies.");

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(17,1);
		addstr("Enter - Nothing for now.");

		refresh();
		int c=getch();
		translategetch(c);

		if(c=='a')
			{
			long flevel=select_troublefundinglevel(cr);

			if(flevel>=0)
				{
				cr->activity.type=ACTIVITY_TROUBLE;
				cr->activity.arg=flevel;
				return;
				}
			}
		if(c=='d')
			{
			cr->activity.type=ACTIVITY_FUNDS_LEGAL;
			break;
			}
		if(c=='b')
			{
			cr->activity.type=ACTIVITY_FUNDS_ILLEGAL;
			break;
			}
		if(c=='h'&&hostagecount>0)
			{
			activityst oact=cr->activity;
			cr->activity.type=ACTIVITY_NONE;
			select_tendhostage(cr);
			if(cr->activity.type==ACTIVITY_HOSTAGETENDING)break;
			else cr->activity=oact;
			}
		if(c=='c')
			{
			activityst oact=cr->activity;
			cr->activity.type=ACTIVITY_NONE;
			select_makeclothing(cr);
			if(cr->activity.type==ACTIVITY_MAKE_ARMOR)break;
			else cr->activity=oact;
			}
		if(c=='r')
			{
			cr->activity.type=ACTIVITY_REPAIR_ARMOR;
			break;
			}
		if(c=='p')
			{
			cr->activity.type=ACTIVITY_POLLS;
			break;
			}
		if(c=='s')
			{
			if(cr->canwalk())
				{
				cr->activity.type=ACTIVITY_STEALCARS;
				break;
				}
			else if(!(cr->flag & CREATUREFLAG_WHEELCHAIR))
				{
				cr->activity.type=ACTIVITY_WHEELCHAIR;
				break;
				}
			}
		if(c=='z'&&havedead)
			{
			cr->activity.type=ACTIVITY_BURY;
			break;
			}

		if(c==10)
			{
			cr->activity.type=ACTIVITY_NONE;
			break;
			}
		}while(1);
}

void select_makeclothing(creaturest *cr)
{
	vector<int> armortype;
	for(int a=0;a<ARMORNUM;a++)
		{
		switch(a)
			{
			case ARMOR_NONE:
			case ARMOR_MITHRIL:
			case ARMOR_MASK:
				break;
			default:
				armortype.push_back(a);
				break;
			}
		}

	short page=0;

	char str[200];
	char num[20];

	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(0,0);
		addstr("Which will ");
		addstr(cr->name);
		addstr(" try to make?  (Note: Half Cost if you have cloth)");
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(1,0);
		addstr("----NAME-----------------------------DIFFICULTY-------------COST----------------");

		int y=2,difficulty;
		for(int p=page*19;p<armortype.size()&&p<page*19+19;p++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addch(y+'A'-2);addstr(" - ");
			getarmorfull(str,armortype[p]);
			addstr(str);

			move(y,37);
			difficulty=armor_makedifficulty(armortype[p],cr);
			switch(difficulty)
				{
				case 0:
					set_color(COLOR_GREEN,COLOR_BLACK,1);
					addstr("Simple");
					break;
				case 1:
					set_color(COLOR_CYAN,COLOR_BLACK,1);
					addstr("Very Easy");
					break;
				case 2:
					set_color(COLOR_CYAN,COLOR_BLACK,0);
					addstr("Easy");
					break;
				case 3:
					set_color(COLOR_BLUE,COLOR_BLACK,1);
					addstr("Below Average");
					break;
				case 4:
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					addstr("Average");
					break;
				case 5:
					set_color(COLOR_WHITE,COLOR_BLACK,0);
					addstr("Above Average");
					break;
				case 6:
					set_color(COLOR_YELLOW,COLOR_BLACK,1);
					addstr("Hard");
					break;
				case 7:
					set_color(COLOR_MAGENTA,COLOR_BLACK,0);
					addstr("Very Hard");
					break;
				case 8:
					set_color(COLOR_MAGENTA,COLOR_BLACK,1);
					addstr("Extremely Difficult");
					break;
				case 9:
					set_color(COLOR_RED,COLOR_BLACK,0);
					addstr("Nearly Impossible");
					break;
				default:
					set_color(COLOR_RED,COLOR_BLACK,1);
					addstr("Impossible");
					break;
				}

			set_color(COLOR_GREEN,COLOR_BLACK,1);
			itoa(armor_makeprice(armortype[p]),num,10);
			move(y,63-strlen(num));
			addch('$');addstr(num);

			y++;
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(22,0);
		addstr("Press a Letter to select a Type of Clothing");
		move(23,0);
		if(interface_pgup=='[')
			{
			addstr("[] to view other Liberal pages.");
			}
		else
			{
			addstr("PGUP/PGDN to view other Liberal pages.");
			}

		refresh();

		int c=getch();
		translategetch(c);

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*19<armortype.size())page++;

		if(c>='a'&&c<='s')
			{
			int p=page*19+(int)(c-'a');
			if(p<armortype.size())
				{
				cr->activity.type=ACTIVITY_MAKE_ARMOR;
				cr->activity.arg=armortype[p];
				return;
				}
			}

		if(c==10)break;
		}while(1);
}

void select_tendhostage(creaturest *cr)
{
	vector<creaturest *> temppool;
	for(int p=0;p<pool.size();p++)
		{
		if(pool[p]->align!=1&&
			pool[p]->alive)
			{
			temppool.push_back(pool[p]);
			}
		}

	if(temppool.size()==0)return;

	short page=0;

	char num[20];

	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addstr("Which hostage will ");
		addstr(cr->name);
		addstr(" be watching over?");
		move(1,0);
		addstr("----CODE NAME------------SKILL---HEALTH---LOCATION------------------------------");
		move(1,57);
		addstr("DAYS IN CAPTIVITY");

		int y=2;
		for(int p=page*19;p<temppool.size()&&p<page*19+19;p++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addch(y+'A'-2);addstr(" - ");
			addstr(temppool[p]->name);

			char bright=0;
			unsigned long skill=0;
			for(int sk=0;sk<SKILLNUM;sk++)
				{
				skill+=(unsigned long)temppool[p]->skill[sk];
				if(temppool[p]->skill_ip[sk]>=100)bright=1;
				}

			set_color(COLOR_WHITE,COLOR_BLACK,bright);

			move(y,25);
			itoa(skill,num,10);
			addstr(num);

			printhealthstat(*temppool[p],y,33,TRUE);

			if(mode==REVIEWMODE_JUSTICE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,42);
			addstr(location[temppool[p]->location]->shortname);

			move(y,57);
			set_color(COLOR_MAGENTA,COLOR_BLACK,1);
			char num[20];
			itoa(temppool[p]->joindays,num,10);
			addstr(num);
			addstr(" ");
			if(temppool[p]->joindays>1)addstr("Days");
			else addstr("Day");

			y++;
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(22,0);
		addstr("Press a Letter to select a Conservative");
		move(23,0);
		if(interface_pgup=='[')
			{
			addstr("[] to view other Liberal pages.");
			}
		else
			{
			addstr("PGUP/PGDN to view other Liberal pages.");
			}

		refresh();

		int c=getch();
		translategetch(c);

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*19<temppool.size())page++;

		if(c>='a'&&c<='s')
			{
			int p=page*19+(int)(c-'a');
			if(p<temppool.size())
				{
				long flevel=select_hostagefundinglevel(cr,temppool[p]);

				if(flevel>=0)
					{
					cr->activity.type=ACTIVITY_HOSTAGETENDING;
					cr->activity.arg=temppool[p]->id;
					cr->activity.arg2=flevel;
					return;
					}
				}
			}

		if(c==10)break;
		}while(1);
}

long select_hostagefundinglevel(creaturest *cr,creaturest *hs)
{
	long flevel=-1;

	erase();

	set_color(COLOR_WHITE,COLOR_BLACK,0);
	move(0,0);
	addstr("Select a Funding Level for this Operation:");

	move(2,0);
	addstr("A - Don't spend anything.  ");
	addstr(cr->name);
	addstr(" is just on watch duty.");

	move(3,0);
	addstr("B - Don't spend anything.  ");
	addstr(cr->name);
	addstr(" will turn the prisoner over time.");

	move(4,0);
	addstr("C - $20 per day.  Enough for some props.");

	move(5,0);
	addstr("D - $50 per day.  ");
	addstr(cr->name);
	addstr(" will go for a thrifty indoctrination.");

	move(6,0);
	addstr("E - $100 per day.  ");
	addstr(cr->name);
	addstr(" needs enough freedom to be creative.");

	move(7,0);
	addstr("F - $500 per day.  It is imperative that the Conservative be turned quickly.");

	move(8,0);
	addstr("K - This Conservative has become a liability and needs to be terminated.");

	move(10,0);
	addstr("X - On second thought, this isn't a job for ");
	addstr(cr->name);
	addstr(".");

	refresh();
	int c=getch();
	translategetch(c);

	if(c=='a')flevel=0;
	if(c=='b')flevel=1;
	if(c=='c')flevel=20;
	if(c=='d')flevel=50;
	if(c=='e')flevel=100;
	if(c=='f')flevel=500;
	if(c=='k')flevel=666;

	return flevel;
}

void printcreatureinfo(creaturest *cr)
{
	char num[20],str[200];

	makedelimiter(1,0);

	set_color(COLOR_WHITE,COLOR_BLACK,0);
	move(1,2);
	addstr(cr->name);
	addstr(", ");
	gettitle(str,*cr);
	addstr(str);

	if(cr->prisoner!=NULL)
		{
		addstr(", holding ");
		switch(cr->prisoner->type)
			{
			case CREATURE_CORPORATE_CEO:addstr("a squirming CEO");break;
			case CREATURE_RADIOPERSONALITY:addstr("a crying Radio Personality");break;
			case CREATURE_NEWSANCHOR:addstr("smarmy News Anchor");break;
			case CREATURE_SCIENTIST_EMINENT:addstr("a frightened Eminent Scientist");break;
			case CREATURE_JUDGE_CONSERVATIVE:addstr("an angry Hangin' Judge");break;
			default:
				addstr(cr->prisoner->name);
				break;
			}
		}

	move(2,0);addstr("Hrt:   ");
	itoa(cr->attval(ATTRIBUTE_HEART),num,10);
	addstr(num);
	move(3,0);addstr("Int:   ");
	itoa(cr->attval(ATTRIBUTE_INTELLIGENCE),num,10);
	addstr(num);
	move(4,0);addstr("Wis:   ");
	itoa(cr->attval(ATTRIBUTE_WISDOM),num,10);
	addstr(num);
	move(5,0);addstr("Hlth:  ");
	itoa(cr->attval(ATTRIBUTE_HEALTH),num,10);
	addstr(num);
	move(2,11);addstr("Agi:   ");
	itoa(cr->attval(ATTRIBUTE_AGILITY),num,10);
	addstr(num);
	move(3,11);addstr("Str:   ");
	itoa(cr->attval(ATTRIBUTE_STRENGTH),num,10);
	addstr(num);
	move(4,11);addstr("Char:  ");
	itoa(cr->attval(ATTRIBUTE_CHARISMA),num,10);
	addstr(num);
	move(5,11);
	addstr("Trans: ");
	long v=-1;
	if(showcarprefs==1)v=id_getcar(cr->pref_carid);
	else v=id_getcar(cr->carid);
	if(v!=-1&&showcarprefs!=-1)
		{
		getcar(str,vehicle[v]->type);
		char d;
		if(showcarprefs==1)d=cr->pref_is_driver;
		else d=cr->is_driver;
		if(d)strcat(str,"-D");
		}
	else
		{
		int legok=2;
		if((cr->wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
			 (cr->wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
		if((cr->wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
			 (cr->wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;
		if(cr->flag & CREATUREFLAG_WHEELCHAIR)strcpy(str,"Wheelchair");
		else if(legok>=1)strcpy(str,"On Foot");
		else strcpy(str,"On \"Foot\"");
		}
	addstr(str);
	move(6,0);
	addstr("Weapon: ");
	getweaponfull(str,cr->weapon.type);
	addstr(str);
	if(ammotype(cr->weapon.type)!=-1)
		{
		char num[20];
		itoa(cr->weapon.ammo,num,10);
		addstr(" (");
		addstr(num);
		int at=ammotype(cr->weapon.type);
		itoa(cr->clip[at],num,10);
		addstr("/");
		addstr(num);
		addstr(")");
		}

	if(mode!=GAMEMODE_SITE)set_color(COLOR_WHITE,COLOR_BLACK,0);
	else if(hasdisguise(*cr,sitetype))
		{
		if(cr->armor.quality!='1'||
			cr->armor.flag>0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
		else set_color(COLOR_GREEN,COLOR_BLACK,1);
		}
	else set_color(COLOR_RED,COLOR_BLACK,1);
	move(7,0);
	addstr("Armor: ");
	getarmorfull(str,cr->armor,0);
	addstr(str);

	set_color(COLOR_WHITE,COLOR_BLACK,0);

	char used[SKILLNUM];
	memset(used,0,sizeof(char)*SKILLNUM);

	int snum=5;
	char printed=1;

	while(snum>0&&printed)
		{
		printed=0;

		unsigned long max=0;
		long maxs=-1;
		for(int s=0;s<SKILLNUM;s++)
			{
			if(cr->skill[s]>max && !used[s])
				{
				max=cr->skill[s];
				maxs=s;
				}
			}

		if(maxs!=-1)
			{
			used[maxs]=1;
			printed=1;

			if(cr->skill_ip[maxs]>=100)set_color(COLOR_WHITE,COLOR_BLACK,1);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);

			move(3+5-snum,31);
			getskill(str,maxs);
			strcat(str,": ");
			itoa(cr->skill[maxs],num,10);
			strcat(str,num);
			addstr(str);

			if(snum==5&&printed)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(2,31);
				addstr("Top Skills:");
				}
			}

		snum--;
		}

	int woundsum=0;
	for(int w=0;w<BODYPARTNUM;w++)
		{
		if(cr->wound[w]!=0)woundsum++;
		}

	printhealthstat(*cr,1,49,FALSE);

	if(woundsum>0)
		{
		for(int w=0;w<BODYPARTNUM;w++)
			{
			if(cr->wound[w] & WOUND_BLEEDING)set_color(COLOR_RED,COLOR_BLACK,1);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);

			move(2+w,49);
			switch(w)
				{
				case BODYPART_HEAD:addstr("Head:");break;
				case BODYPART_BODY:addstr("Body:");break;
				case BODYPART_ARM_RIGHT:addstr("Right Arm:");break;
				case BODYPART_ARM_LEFT:addstr("Left Arm:");break;
				case BODYPART_LEG_RIGHT:addstr("Right Leg:");break;
				case BODYPART_LEG_LEFT:addstr("Left Leg:");break;
				}

			move(2+w,61);
			if(cr->wound[w] & WOUND_NASTYOFF)addstr("Ripped off");
			else if(cr->wound[w] & WOUND_CLEANOFF)addstr("Clean sever");
			else
				{
				int sum=0;

				if(cr->wound[w] & WOUND_SHOT)sum++;
				if(cr->wound[w] & WOUND_CUT)sum++;
				if(cr->wound[w] & WOUND_BRUISED)sum++;
				if(cr->wound[w] & WOUND_BURNED)sum++;
				if(cr->wound[w] & WOUND_TORN)sum++;

				if(sum==0)
					{
					set_color(COLOR_GREEN,COLOR_BLACK,1);
					addstr("Liberal");
					}

				if(cr->wound[w] & WOUND_SHOT){addstr("Sht");sum--;if(sum>0)addstr(",");}
				if(cr->wound[w] & WOUND_BRUISED){addstr("Brs");sum--;if(sum>0)addstr(",");}
				if(cr->wound[w] & WOUND_CUT){addstr("Cut");sum--;if(sum>0)addstr(",");}
				if(cr->wound[w] & WOUND_TORN){addstr("Trn");sum--;if(sum>0)addstr(",");}
				if(cr->wound[w] & WOUND_BURNED){addstr("Brn");sum--;if(sum>0)addstr(",");}
				}
			}
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		}
}

void makedelimiter(int y,int x)
{
	set_color(COLOR_WHITE,COLOR_BLACK,0);
	move(y,x);
	addstr("--------------------------------------------------------------------------------");
}

int getpoolcreature(long id)
{
	for(int pl=0;pl<pool.size();pl++)
		{
		if(pool[pl]->id==id)return pl;
		}
	return -1;
}

void tendhostage(creaturest *cr,char &clearformess)
{
	vector<creaturest *> temppool;
	int p;

	long hfunds=0;
	char notender=1;
	char terminatehostage=0;
	creaturest *killer=NULL;

	for(p=0;p<pool.size();p++)
		{
		if(!pool[p]->alive)continue;
		if(terminatehostage)break;
		if(pool[p]->activity.type==ACTIVITY_HOSTAGETENDING)
			{
			if(pool[p]->activity.arg==cr->id)
				{
				notender=0;
				if(pool[p]->activity.arg2>=1)
					{
					temppool.push_back(pool[p]);
					if(pool[p]->activity.arg2==666)
						{
						terminatehostage=1;
						killer=pool[p];
						}
					else if(pool[p]->activity.arg2>=20)hfunds+=pool[p]->activity.arg2;
					}
				}
			}
		}

	if(temppool.size()==0)
		{
		//CHECK FOR HOSTAGE ESCAPE
		if(notender&&
			LCSrandom(200)<cr->attval(ATTRIBUTE_INTELLIGENCE)+
			cr->attval(ATTRIBUTE_AGILITY)+
			cr->attval(ATTRIBUTE_STRENGTH)&&
			cr->joindays>=2)
			{
			for(int p=0;p<pool.size();p++)
				{
				if(pool[p]==cr)
					{
					if(clearformess)erase();
					else
						{
						makedelimiter(8,0);
						}

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(8,1);
					addstr(cr->name);
					addstr(" has escaped!");
					refresh();
					getch();

					delete pool[p];
					pool.erase(pool.begin() + p);
					break;
					}
				}
			}

		return;
		}

	clearformess=1;

	erase();
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(0,0);
	if(terminatehostage)addstr("The Final Education of ");
	else addstr("The Education of ");
	addstr(cr->name);
	addstr(": Day ");
	char num[20];
	itoa(cr->joindays,num,10);
	addstr(num);

	refresh();
	getch();

	set_color(COLOR_WHITE,COLOR_BLACK,0);

	char turned=0;

	int y=2;

	if(terminatehostage)
		{
		if(LCSrandom(10)<killer->juice&&
			LCSrandom(9)+1>=killer->attval(ATTRIBUTE_HEART,0))
			{
			set_color(COLOR_MAGENTA,COLOR_BLACK,0);
			cr->alive=0;
			stat_kills++;
			move(y,0);y++;
			addstr(killer->name);
			addstr(" executes ");
			addstr(cr->name);
			addstr(" by ");
			switch(LCSrandom(5))
				{
				case 0:addstr("strangling it to death.");break;
				case 1:addstr("beating it to death.");break;
				case 2:addstr("burning photos of Reagan in front of it.");break;
				case 3:addstr("telling it that taxes have been increased.");break;
				case 4:addstr("telling it its parents wanted to abort it.");break;
				}
			refresh();getch();
			if(killer->att[ATTRIBUTE_HEART]>1)
				{
				set_color(COLOR_GREEN,COLOR_BLACK,1);
				move(y,0);y++;
				addstr(killer->name);
				addstr(" feels sick to the stomach afterward and ");
				killer->att[ATTRIBUTE_HEART]--;
				move(y,0);y++;
				switch(LCSrandom(3))
					{
					case 0:addstr("throws up in a trash can.");break;
					case 1:addstr("gets drunk, eventually falling asleep.");break;
					case 2:addstr("curls up in a ball, crying softly.");break;
					}
				}
			else if(!LCSrandom(3))
				{
				set_color(COLOR_CYAN,COLOR_BLACK,1);
				move(y,0);y++;
				addstr(killer->name);
				addstr(" grows colder.");
				killer->att[ATTRIBUTE_WISDOM]++;
				}
			}
		else
			{
			set_color(COLOR_YELLOW,COLOR_BLACK,0);
			move(y,0);y++;
			addstr(killer->name);
			addstr(" is unable to get up the nerve to");
			move(y,0);y++;
			addstr("execute ");
			addstr(cr->name);
			addstr(" in cold blood.");
			}
		}
	else
		{
	if(hfunds>funds)hfunds=funds;
	funds-=hfunds;
	stat_spent+=hfunds;
	moneylost_hostage+=hfunds;

	if(hfunds==0)
		{
		//BEAT IT UP OR TALK IT DOWN
		if(!LCSrandom(2))
			{
			int maxattack=0;

			for(p=0;p<temppool.size();p++)
				{
				if(temppool[p]!=NULL)
					{
					if(temppool[p]->alive)
						{
						if((temppool[p]->attval(ATTRIBUTE_CHARISMA)+
							temppool[p]->attval(ATTRIBUTE_HEART)+
							temppool[p]->skill[SKILL_PERSUASION])>maxattack)
							{
							maxattack=temppool[p]->attval(ATTRIBUTE_CHARISMA)+
								temppool[p]->attval(ATTRIBUTE_HEART)+
								temppool[p]->skill[SKILL_PERSUASION];
							}
						}
					}
				}

			vector<int> goodp;

			for(p=0;p<temppool.size();p++)
				{
				if(temppool[p]!=NULL)
					{
					if(temppool[p]->alive)
						{
						if((temppool[p]->attval(ATTRIBUTE_CHARISMA)+
							temppool[p]->attval(ATTRIBUTE_HEART)+
							temppool[p]->skill[SKILL_PERSUASION])==maxattack)
							{
							goodp.push_back(p);
							}
						}
					}
				}

			if(goodp.size()>0)
				{
				creaturest *a=temppool[goodp[LCSrandom(goodp.size())]];

				long aroll=LCSrandom(a->attval(ATTRIBUTE_CHARISMA)+
					a->attval(ATTRIBUTE_HEART)+
					a->skill[SKILL_PERSUASION]+1)+LCSrandom(10)+temppool.size();
				long troll=LCSrandom(cr->attval(ATTRIBUTE_CHARISMA)+
					cr->attval(ATTRIBUTE_WISDOM)+
					cr->skill[SKILL_PERSUASION]+1)+LCSrandom(10);

				move(y,0);
				addstr(a->name);
				addstr(" discusses the issues with ");
				addstr(cr->name);
				addstr(".");
				y++;

				refresh();
				getch();
			
				if(aroll>troll)
					{
					if(cr->att[ATTRIBUTE_WISDOM]>1)
						{
						cr->att[ATTRIBUTE_WISDOM]--;
						if(LCSrandom(11)>(cr->att[ATTRIBUTE_WISDOM]*5)-cr->joindays)turned=1;
						}
					else turned=1;

					move(y,0);
					addstr(cr->name);
					addstr("'s resolve weakens...");
					y++;
					}
				else if(aroll>(troll>>1))
					{
					move(y,0);
					addstr(cr->name);
					addstr(" holds firm.");
					y++;
					}
				else
					{
					a->att[ATTRIBUTE_WISDOM]++;

					move(y,0);
					addstr(cr->name);
					addstr(" turns the tables on ");
					addstr(a->name);
					addstr("!");
					y++;

					refresh();
					getch();
			
					move(y,0);
					addstr(a->name);
					addstr(" has gained wisdom!");
					y++;
					}

				a->skill_ip[SKILL_PERSUASION]+=(troll>>2)+1;
				}
			}
		else
			{
			move(y,0);
			if(temppool.size()==1)
				{
				addstr(temppool[0]->name);
				addstr(" beats");
				}
			else if(temppool.size()==2)
				{
				addstr(temppool[0]->name);
				addstr(" and ");
				addstr(temppool[1]->name);
				addstr(" beat");
				}
			else
				{
				addstr(cr->name);
				addstr("'s guards beat");
				}
			addstr(" the Automaton, yelling its");
			y++;
			move(y,0);
			addstr("favorite corporation's name.");
			y++;

			refresh();
			getch();

			long forceroll=LCSrandom(((long)temppool.size())*10+1);

			if(forceroll>=cr->attval(ATTRIBUTE_HEALTH))
				{
				if(cr->att[ATTRIBUTE_WISDOM]>1)
					{
					cr->att[ATTRIBUTE_WISDOM]--;
					if(LCSrandom(11)>(cr->att[ATTRIBUTE_WISDOM]*5)-cr->joindays)turned=1;
					}
				else turned=1;

				move(y,0);
				addstr(cr->name);
				addstr("'s resolve weakens...");
				y++;

				if(forceroll>=LCSrandom(cr->attval(ATTRIBUTE_HEALTH)*5+1))
					{
					refresh();
					getch();

					if(LCSrandom(5)&&cr->att[ATTRIBUTE_HEALTH]>1)
						{
						cr->att[ATTRIBUTE_HEALTH]--;
						move(y,0);
						addstr(cr->name);
						addstr("'s health is damaged...");
						y++;
						}
					else
						{
						cr->alive=0;
						stat_kills++;
						move(y,0);
						addstr(cr->name);
						addstr(" dies from the savage attack!");
						y++;
						}
					}
				}
			else
				{
				move(y,0);
				addstr(cr->name);
				addstr(" takes it well.");
				y++;
				}
			}
		}
	else
		{
		int maxattack=0;

		for(int p=0;p<temppool.size();p++)
			{
			if(temppool[p]!=NULL)
				{
				if(temppool[p]->alive)
					{
					if((temppool[p]->attval(ATTRIBUTE_CHARISMA)+
						temppool[p]->attval(ATTRIBUTE_HEART)+
						temppool[p]->skill[SKILL_PERSUASION])>maxattack)
						{
						maxattack=temppool[p]->attval(ATTRIBUTE_CHARISMA)+
							temppool[p]->attval(ATTRIBUTE_HEART)+
							temppool[p]->skill[SKILL_PERSUASION];
						}
					}
				}
			}
		maxattack+=hfunds/20+temppool.size();

		long aroll=LCSrandom(maxattack)+LCSrandom(10);
		long troll=LCSrandom(cr->attval(ATTRIBUTE_WISDOM)*2)+LCSrandom(15);

		if(hfunds<=20)
			{
			move(y,0);
			addstr(cr->name);
			switch(LCSrandom(2))
				{
				case 0:addstr(" is forced to listen to FM radio in lieu of sleep");break;
				case 1:addstr(" is forced to watch network news tapes in lieu of sleep");break;
				}
			addstr(".");
			y++;

			refresh();
			getch();
			}
		else if(hfunds<=50)
			{
			move(y,0);
			addstr(cr->name);
			switch(LCSrandom(2))
				{
				case 0:
					addstr(" is forced to ingest psychotropic drugs.");
					y++;
					move(y,0);
					addstr("Liberal tracts are then read to it");
					break;
				case 1:addstr(" is forced to watch controversial avant-garde films");break;
				}
			addstr(".");
			y++;

			refresh();
			getch();
			}
		else if(hfunds<=100)
			{
			move(y,0);
			addstr(cr->name);
			addstr(" tied to a chair while Origin of the Species blares on a PA");
			y++;
			move(y,0);
			addstr("and a silent video of ");
			switch(LCSrandom(3))
				{
				case 0:addstr("atrocities in Cambodia");break;
				case 1:addstr("chickens in a factory farm");break;
				case 2:addstr("young sweatshop workers making toys");break;
				}
			addstr(" is played.");
			y++;

			refresh();
			getch();
			}
		else
			{
			move(y,0);
			addstr(cr->name);
			addstr(" is kept in a dark room and");
			y++;
			move(y,0);
			addstr("made to burn confederate flags for light,");
			y++;
			move(y,0);
			addstr("only to find upon illumination of the room that");
			switch(LCSrandom(3))
				{
				case 0:
					addstr(" the walls have been");y++;move(y,0);
					addstr("plastered with scenes from anime film Bible Black");break;
				case 1:
					addstr(" the walls have been");y++;move(y,0);
					addstr("papered with anarchy symbols");break;
				case 2:
					addstr(" the walls have been");y++;move(y,0);
					addstr("papered with illustrations of apes evolving into humans");break;
				}
			addstr(".");
			y++;

			refresh();
			getch();
			}

		if(aroll>troll)
			{
			if(aroll>troll*4&&cr->att[ATTRIBUTE_WISDOM]>1)cr->att[ATTRIBUTE_WISDOM]--;
			if(aroll>troll*3&&cr->att[ATTRIBUTE_WISDOM]>1)cr->att[ATTRIBUTE_WISDOM]--;
			if(aroll>troll*2&&cr->att[ATTRIBUTE_WISDOM]>1)cr->att[ATTRIBUTE_WISDOM]--;

			if(cr->att[ATTRIBUTE_WISDOM]>1)
				{
				cr->att[ATTRIBUTE_WISDOM]--;
				if(LCSrandom(11)>(cr->att[ATTRIBUTE_WISDOM]*5)-cr->joindays)turned=1;
				}
			else turned=1;

			move(y,0);
			addstr(cr->name);
			addstr("'s resolve weakens...");
			y++;
			}
		else
			{
			move(y,0);
			addstr(cr->name);
			addstr(" holds firm.");
			y++;
			}
		}

	#ifdef AUTOENLIGHTEN
		turned=1;
	#endif

	if(turned)
		{
		refresh();
		getch();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		y++;
		move(y,0);
		addstr("The Automaton has been Enlightened!  Your Liberal ranks are swelling!");
		y+=2;
		cr->align=1;
		stat_recruits++;

		if(!(cr->flag & CREATUREFLAG_KIDNAPPED))
			{
			set_color(COLOR_CYAN,COLOR_BLACK,1);
			move(y,0);y++;
			addstr(cr->name);
			addstr("'s disappearance has not yet been reported.");
			move(y,0);y+=2;
			addstr("You now have a sleeper infiltrating Conservative Society.");
			cr->flag&=~CREATUREFLAG_MISSING;
			cr->flag|=CREATUREFLAG_SLEEPER;
			cr->location=cr->worklocation;
			cr->base=cr->worklocation;
			}
		}}

	if(cr->align==1||!cr->alive)
		{
		for(int p=0;p<pool.size();p++)
			{
			if(!pool[p]->alive)continue;
			if(pool[p]->activity.type==ACTIVITY_HOSTAGETENDING)
				{
				if(pool[p]->activity.arg==cr->id)
					{
					pool[p]->activity.type=ACTIVITY_NONE;
					}
				}
			}
		}

	refresh();
	getch();

	set_color(COLOR_WHITE,COLOR_BLACK,0);
	move(24,0);
	addstr("Press any key to reflect on this.");

	refresh();
	getch();
}

char disguisesite(long type)
{
	switch(type)
		{
		case SITE_LABORATORY_COSMETICS:
		case SITE_LABORATORY_GENETIC:
		case SITE_GOVERNMENT_PRISON:
		case SITE_GOVERNMENT_INTELLIGENCEHQ:
		case SITE_INDUSTRY_SWEATSHOP:
		case SITE_INDUSTRY_POLLUTER:
		case SITE_INDUSTRY_NUCLEAR:
		case SITE_CORPORATE_HEADQUARTERS:
		case SITE_CORPORATE_HOUSE:
		case SITE_BUSINESS_CIGARBAR:
			return 1;
		}

	return 0;
}

void armordamage(armorst &armor,int bp)
{
	switch(armor.type)
		{
		case ARMOR_NONE:
			return;
		case ARMOR_SECURITYUNIFORM:
		case ARMOR_POLICEUNIFORM:
		case ARMOR_BONDAGEGEAR:
		case ARMOR_MILITARY:
			armor.flag|=ARMORFLAG_DAMAGED;
			break;
		case ARMOR_MASK:
			if(bp==BODYPART_HEAD)armor.flag|=ARMORFLAG_DAMAGED;
			break;
		case ARMOR_TOGA:
		case ARMOR_MITHRIL:
		case ARMOR_WIFEBEATER:
			if(bp==BODYPART_BODY)armor.flag|=ARMORFLAG_DAMAGED;
			break;
		case ARMOR_OVERALLS:
			if(bp==BODYPART_BODY)armor.flag|=ARMORFLAG_DAMAGED;
			if(bp==BODYPART_LEG_RIGHT)armor.flag|=ARMORFLAG_DAMAGED;
			if(bp==BODYPART_LEG_LEFT)armor.flag|=ARMORFLAG_DAMAGED;
			break;
		default:
			if(bp!=BODYPART_HEAD)armor.flag|=ARMORFLAG_DAMAGED;
			break;
		}
}

void bloodblast(armorst &armor)
{
	//GENERAL
	if(armor.type!=ARMOR_NONE)armor.flag|=ARMORFLAG_BLOODY;

	if(mode!=GAMEMODE_SITE)return;

	map[locx][locy][locz].flag|=SITEBLOCK_BLOODY2;

	//HIT EVERYTHING
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]==NULL)continue;
		if(activesquad->squad[p]->armor.type!=ARMOR_NONE)
			{
			if(!LCSrandom(2))activesquad->squad[p]->armor.flag|=ARMORFLAG_BLOODY;
			}
		}

	for(int e=0;e<ENCMAX;e++)
		{
		if(!encounter[e].exists)continue;
		if(encounter[e].armor.type!=ARMOR_NONE)
			{
			if(!LCSrandom(2))encounter[e].armor.flag|=ARMORFLAG_BLOODY;
			}
		}

	//REFRESH THE SCREEN
	printsitemap(locx,locy,locz);
	refresh();
}

char squadhasitem(squadst &sq,int type,int subtype)
{
	for(int p=0;p<6;p++)
		{
		if(sq.squad[p]!=NULL)
			{
			switch(type)
				{
				case ITEM_WEAPON:
					if(sq.squad[p]->weapon.type==subtype)return 1;
					break;
				}
			}
		}

	for(int l=0;l<sq.loot.size();l++)
		{
		if(sq.loot[l]->type!=type)continue;
		switch(type)
			{
			case ITEM_WEAPON:
				if(sq.loot[l]->weapon.type==subtype)return 1;
				break;
			}
		}

	return 0;
}

int getrandomcreaturetype(int cr[CREATURENUM])
{
	int sum=0;
	for(int c=0;c<CREATURENUM;c++)sum+=cr[c];

	if(sum>0)
		{
		int roll=LCSrandom(sum);
		int sel=0;
		while(roll>=0){roll-=cr[sel];sel++;}
		return sel-1;
		}
	else return -1;
}

long armor_makedifficulty(int type,creaturest *cr)
{
	long basedif;

	switch(type)
		{
		case ARMOR_TOGA:
		case ARMOR_WIFEBEATER:
			basedif=2;
			break;
		case ARMOR_CLOTHES:
		case ARMOR_OVERALLS:
		case ARMOR_WORKCLOTHES:
			basedif=3;
			break;
		case ARMOR_CLOWNSUIT:
		case ARMOR_PRISONER:
		case ARMOR_CHEAPDRESS:
		case ARMOR_TRENCHCOAT:
		case ARMOR_LABCOAT:
		case ARMOR_BLACKROBE:
		case ARMOR_BONDAGEGEAR:
			basedif=4;
			break;
		case ARMOR_SECURITYUNIFORM:
		case ARMOR_PRISONGUARD:
		case ARMOR_MILITARY:
		case ARMOR_POLICEUNIFORM:
			basedif=5;
			break;
		case ARMOR_CHEAPSUIT:
			basedif=6;
			break;
		case ARMOR_BLACKSUIT:
		case ARMOR_BLACKDRESS:
			basedif=7;
			break;
		case ARMOR_EXPENSIVESUIT:
		case ARMOR_EXPENSIVEDRESS:
			basedif=9;
			break;
		default:
			basedif=10;
			break;
		}

	basedif-=cr->skill[SKILL_GARMENTMAKING]-3;
	if(basedif<0)basedif=0;

	return basedif;
}

long armor_makeprice(int type)
{
	long price=0;

	switch(type)
		{
		case ARMOR_TOGA:
		case ARMOR_WIFEBEATER:
			price=5;
			break;
		case ARMOR_CLOTHES:
		case ARMOR_OVERALLS:
		case ARMOR_WORKCLOTHES:
			price=10;
			break;
		case ARMOR_CLOWNSUIT:
		case ARMOR_PRISONER:
		case ARMOR_CHEAPDRESS:
		case ARMOR_TRENCHCOAT:
		case ARMOR_LABCOAT:
		case ARMOR_BLACKROBE:
			price=20;
			break;
		case ARMOR_BONDAGEGEAR:
			price=30;
			break;
		case ARMOR_SECURITYUNIFORM:
		case ARMOR_PRISONGUARD:
		case ARMOR_MILITARY:
		case ARMOR_POLICEUNIFORM:
			price=40;
			break;
		case ARMOR_CHEAPSUIT:
			price=50;
			break;
		case ARMOR_BLACKSUIT:
		case ARMOR_BLACKDRESS:
			price=60;
			break;
		case ARMOR_EXPENSIVESUIT:
		case ARMOR_EXPENSIVEDRESS:
			price=300;
			break;
		}

	return price;
}

void repairarmor(creaturest &cr,char &clearformess)
{
	armorst *it=NULL;
	itemst *itm=NULL;
	vector<itemst *> *lar=NULL;

	if(cr.armor.type!=ARMOR_NONE&&
		((cr.armor.flag & ARMORFLAG_DAMAGED)||
		(cr.armor.flag & ARMORFLAG_BLOODY)))
		{
		it=&cr.armor;
		}
	else if(cr.squadid!=-1)
		{
		int sq=getsquad(cr.squadid);
		for(int l=0;l<squad[sq]->loot.size();l++)
			{
			if(squad[sq]->loot[l]->type==ITEM_ARMOR&&
				((squad[sq]->loot[l]->armor.flag & ARMORFLAG_DAMAGED)||
				(squad[sq]->loot[l]->armor.flag & ARMORFLAG_BLOODY)))
				{
				it=&squad[sq]->loot[l]->armor;
				itm=squad[sq]->loot[l];
				lar=&squad[sq]->loot;
				break;
				}
			}
		}
	if(it==NULL&&cr.location!=-1)
		{
		for(int l=0;l<location[cr.location]->loot.size();l++)
			{
			if(location[cr.location]->loot[l]->type==ITEM_ARMOR&&
				((location[cr.location]->loot[l]->armor.flag & ARMORFLAG_DAMAGED)||
				(location[cr.location]->loot[l]->armor.flag & ARMORFLAG_BLOODY)))
				{
				it=&location[cr.location]->loot[l]->armor;
				itm=location[cr.location]->loot[l];
				lar=&location[cr.location]->loot;
				break;
				}
			}
		}

	if(it!=NULL)
		{
		if(clearformess)erase();
		else
			{
			makedelimiter(8,0);
			}

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(8,1);
		addstr(cr.name);
		if(it->flag & ARMORFLAG_DAMAGED)addstr(" repairs ");
		else addstr(" cleans ");
		char str[80];
		getarmorfull(str,it->type,it->subtype);
		addstr(str);

		if(itm!=NULL)
			{
			if(itm->number>1)
				{
				itemst *newi=new itemst;
					*newi=*itm;
				lar->push_back(newi);
				newi->number=itm->number-1;
				itm->number=1;
				}
			}

		it->flag&=~ARMORFLAG_BLOODY;
		it->flag&=~ARMORFLAG_DAMAGED;

		long dif=(armor_makedifficulty(it->type,&cr)>>1);
		cr.skill_ip[SKILL_GARMENTMAKING]+=dif+1;

		if((LCSrandom(10)<dif||LCSrandom(10)<dif)&&it->quality!='4'&&
			(it->flag & ARMORFLAG_DAMAGED))
			{
			addstr(" but it is not quite the same.");
			it->quality++;
			if((LCSrandom(10)<dif||LCSrandom(10)<dif)&&it->quality!='4')it->quality++;
			if((LCSrandom(10)<dif||LCSrandom(10)<dif)&&it->quality!='4')it->quality++;
			}
		else addstr(".");

		refresh();
		getch();
		}
}

void makearmor(creaturest &cr,char &clearformess)
{
	long at=cr.activity.arg;

	long cost=armor_makeprice(at);
	long hcost=(armor_makeprice(at)>>1)+1;
	long dif=armor_makedifficulty(at,&cr);

	if(funds<hcost)
		{
		if(clearformess)erase();
		else
			{
			makedelimiter(8,0);
			}

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(8,1);
		addstr(cr.name);
		addstr(" cannot afford material for clothing.");

		refresh();
		getch();
		return;
		}
	else
		{
		char foundcloth=0;

		if(cr.squadid!=-1)
			{
			int sq=getsquad(cr.squadid);
			for(int l=0;l<squad[sq]->loot.size();l++)
				{
				if(squad[sq]->loot[l]->type==ITEM_LOOT&&
					squad[sq]->loot[l]->loottype==LOOT_FINECLOTH)
					{
					delete squad[sq]->loot[l];
					squad[sq]->loot.erase(squad[sq]->loot.begin() + l);
					foundcloth=1;
					break;
					}
				}
			}
		if(!foundcloth)
			{
			for(int l=0;l<location[cr.location]->loot.size();l++)
				{
				if(location[cr.location]->loot[l]->type==ITEM_LOOT&&
					location[cr.location]->loot[l]->loottype==LOOT_FINECLOTH)
					{
					delete location[cr.location]->loot[l];
					location[cr.location]->loot.erase(location[cr.location]->loot.begin() + l);
					foundcloth=1;
					break;
					}
				}
			}

		if(!foundcloth&&funds<cost)
			{
			if(clearformess)erase();
			else
				{
				makedelimiter(8,0);
				}

			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(8,1);
			addstr(cr.name);
			addstr(" cannot find enough cloth to reduce clothing costs.");

			refresh();
			getch();
			}
		else
			{
			if(foundcloth)
				{
				funds-=hcost;
				stat_spent+=hcost;
				moneylost_manufacture+=hcost;
				}
			else
				{
				funds-=cost;
				stat_spent+=cost;
				moneylost_manufacture+=cost;
				}

			cr.skill_ip[SKILL_GARMENTMAKING]+=dif*2+1;

			itemst *it=new itemst;
				it->type=ITEM_ARMOR;
				it->armor.type=at;
				it->armor.quality='1';
				it->armor.flag=0;
			location[cr.location]->loot.push_back(it);

			if(LCSrandom(10)<dif||LCSrandom(10)<dif)
				{
				it->armor.quality='2';
				if(LCSrandom(10)<dif||LCSrandom(10)<dif)
					{
					it->armor.quality='3';
					if(LCSrandom(10)<dif||LCSrandom(10)<dif)
						{
						it->armor.quality='4';
						}
					}
				}

			if(clearformess)erase();
			else
				{
				makedelimiter(8,0);
				}

			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(8,1);
			addstr(cr.name);
			addstr(" has made a ");
			switch(it->armor.quality)
				{
				case '1':addstr("first-rate");break;
				case '2':addstr("second-rate");break;
				case '3':addstr("third-rate");break;
				case '4':addstr("fourth-rate");break;
				}
			addstr(" ");
			char str[80];
			getarmorfull(str,at);
			addstr(str);
			addstr(".");

			refresh();
			getch();
			}
		}
}

void funds_and_trouble(char &clearformess)
{
	int s;
	//FIND A POLICE STATION
	long ps=-1;
	for(long l=0;l<location.size();l++)
		{
		if(location[l]->type==SITE_GOVERNMENT_POLICESTATION)
			{
			ps=l;
			}
		}

	//ACTIVITIES FOR INDIVIDUALS
	vector<creaturest *> trouble;
	long tfund=0;
	vector<creaturest *> hack;
	long hfund=0;
	vector<creaturest *> bury;
	vector<creaturest *> solicit;
	vector<creaturest *> brownies;

	for(int p=0;p<pool.size();p++)
		{
		if(!pool[p]->alive)continue;
		switch(pool[p]->activity.type)
			{
			case ACTIVITY_TROUBLE:
				if(pool[p]->skill[SKILL_COMPUTERS]>=2&&
					pool[p]->skill[SKILL_COMPUTERS]+
					pool[p]->attval(ATTRIBUTE_INTELLIGENCE)>=7&&
					pool[p]->juice>=0)
					{
					hack.push_back(pool[p]);
					if(funds>=pool[p]->activity.arg)
						{
						hfund+=pool[p]->activity.arg;
						funds-=pool[p]->activity.arg;
						stat_spent+=pool[p]->activity.arg;
						moneylost_trouble+=pool[p]->activity.arg;
						}
					else
						{
						hfund+=funds;
						stat_spent+=funds;
						moneylost_trouble+=funds;
						funds=0;
						}
					}
				else
					{
					trouble.push_back(pool[p]);
					if(pool[p]->juice>=0)
						{
						if(funds>=pool[p]->activity.arg)
							{
							tfund+=pool[p]->activity.arg;
							funds-=pool[p]->activity.arg;
							stat_spent+=pool[p]->activity.arg;
							moneylost_trouble+=pool[p]->activity.arg;
							}
						else
							{
							tfund+=funds;
							moneylost_trouble+=funds;
							stat_spent+=funds;
							funds=0;
							}
						}
					}
				break;
			case ACTIVITY_FUNDS_LEGAL:
				if(pool[p]->juice<0)trouble.push_back(pool[p]);
				else solicit.push_back(pool[p]);
				break;
			case ACTIVITY_FUNDS_ILLEGAL:
				if(pool[p]->juice<0)trouble.push_back(pool[p]);
				else brownies.push_back(pool[p]);
				break;
			case ACTIVITY_BURY:
				bury.push_back(pool[p]);
				pool[p]->activity.type=ACTIVITY_NONE;
				break;
			}
		}

	long money;

	//SOLICITORS
	for(s=0;s<solicit.size();s++)
		{
		money=LCSrandom(solicit[s]->skill[SKILL_PERSUASION]+
			solicit[s]->attval(ATTRIBUTE_CHARISMA)+
			solicit[s]->attval(ATTRIBUTE_HEART)+1);
		if(LCSrandom(100)>attitude[VIEW_LIBERALCRIMESQUADPOS])money=0;
		funds+=money;
		stat_funds+=money;
		moneygained_donate+=money;
		if(solicit[s]->skill[SKILL_PERSUASION]<3)solicit[s]->skill_ip[SKILL_PERSUASION]+=LCSrandom(2)+1;
		}

	//BROWNIES
	long dodgelawroll;
	for(s=0;s<brownies.size();s++)
		{
		money=LCSrandom(brownies[s]->skill[SKILL_PERSUASION]*5+
			brownies[s]->attval(ATTRIBUTE_CHARISMA)*5+
			brownies[s]->attval(ATTRIBUTE_INTELLIGENCE)*5+1);
		funds+=money;
		stat_funds+=money;
		moneygained_brownies+=money;
		brownies[s]->skill_ip[SKILL_PERSUASION]+=LCSrandom(2)+1;

		dodgelawroll=LCSrandom(brownies[s]->skill[SKILL_PERSUASION]+
			brownies[s]->skill[SKILL_DISGUISE]+
			brownies[s]->attval(ATTRIBUTE_CHARISMA)+
			brownies[s]->attval(ATTRIBUTE_AGILITY)+
			brownies[s]->attval(ATTRIBUTE_INTELLIGENCE)+1);

		if(dodgelawroll==0)
			{
			if(clearformess)erase();
			else
				{
				makedelimiter(8,0);
				}

			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(8,1);
			addstr(brownies[s]->name);
			addstr(" has been arrested while selling brownies.");

			refresh();
			getch();

			removesquadinfo(*brownies[s]);
			brownies[s]->carid=-1;
			brownies[s]->location=ps;
			brownies[s]->weapon.type=WEAPON_NONE;
			brownies[s]->weapon.ammo=0;
			brownies[s]->activity.type=ACTIVITY_NONE;
			brownies[s]->lawflag2|=LAWFLAG2_BROWNIES;
			}
		}

	//HACKING
	if(hack.size()>0)
		{
		long hskill=0;
		for(int h=0;h<hack.size();h++)
			{
			hskill+=hack[h]->skill[SKILL_COMPUTERS];
			hskill+=hack[h]->attval(ATTRIBUTE_INTELLIGENCE);
			hack[h]->skill_ip[SKILL_COMPUTERS]+=LCSrandom(2)+1;
			}

		//MAJOR HACKING EVENT
		if(LCSrandom(10000)<=hfund+hskill*20)
			{
			if(clearformess)erase();
			else
				{
				makedelimiter(8,0);
				}

			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(8,1);
			if(hack.size()>1)addstr("Your Hackers have ");
			else {addstr(hack[0]->name);addstr(" has ");}

			int trackdif=0;
			char lawflag=1;
			unsigned short crime=0;

			long juiceval=0;

			switch(LCSrandom(1))
				{
				case 0:
					addstr("pilfered files from a Corporate server.");

					itemst *it=new itemst;
						it->type=ITEM_LOOT;
						it->loottype=LOOT_CORPFILES;
					location[hack[0]->location]->loot.push_back(it);

					trackdif=20;
					lawflag=2;
					crime=LAWFLAG2_INFORMATION;
					juiceval=10;
					break;
				}

			if(trackdif>LCSrandom(hskill+1)+LCSrandom(10))
				{
				for(int h=0;h<hack.size();h++)
					{
					if(lawflag==1)hack[h]->lawflag|=crime;
					else hack[h]->lawflag2|=crime;
					}
				}

			for(int h=0;h<hack.size();h++)
				{
				addjuice(*hack[h],juiceval,20);
				}

			refresh();
			getch();
			}
		//MINOR HACKING EVENT
		else if(LCSrandom(1000)<=hfund+hskill*20)
			{
			if(clearformess)erase();
			else
				{
				makedelimiter(8,0);
				}

			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(8,1);
			if(hack.size()>1)addstr("Your Hackers have ");
			else {addstr(hack[0]->name);addstr(" has ");}

			int trackdif=0;
			char lawflag=1;
			unsigned short crime=0;

			long juiceval=0;

			switch(LCSrandom(2))
				{
				case 0:
					addstr("launched a denial of service attack on a Corporate site.");

					trackdif=5;
					lawflag=2;
					crime=LAWFLAG2_COMMERCE;
					juiceval=2;
					break;
				case 1:
					{
					addstr("stolen some credit card numbers, netting $");
					char num[20];
					long fundgain=LCSrandom(1001)+1000;
					funds+=fundgain;
					stat_funds+=fundgain;
					moneygained_ccfraud+=fundgain;
					itoa(fundgain,num,10);
					addstr(num);
					addstr(".");

					trackdif=10;
					lawflag=2;
					crime=LAWFLAG2_CCFRAUD;
					break;
					}
				}

			if(trackdif>LCSrandom(hskill+1)+LCSrandom(10))
				{
				for(int h=0;h<hack.size();h++)
					{
					if(lawflag==1)hack[h]->lawflag|=crime;
					else hack[h]->lawflag2|=crime;
					}
				}

			for(int h=0;h<hack.size();h++)
				{
				addjuice(*hack[h],juiceval,20);
				}

			refresh();
			getch();
			}
		}

	//TROUBLE
	if(trouble.size()>0)
		{
		//FIRST DEAL WITH LAME-Os
		char num[20];
		for(int t=trouble.size()-1;t>=0;t--)
			{
			if(trouble[t]->juice<0)
				{
				long fundgain=0;
				char caught=0;

				if(!LCSrandom(4))
					{
					if(clearformess)erase();
					else
						{
						makedelimiter(8,0);
						}

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(8,1);
					addstr(trouble[t]->name);
					addstr(" gives it up for $");
					if(trouble[t]->juice<=-50)fundgain=LCSrandom(21)+20;
					else if(trouble[t]->juice<=-50)fundgain=LCSrandom(21)+20;
					else fundgain=LCSrandom(21)+20;
					itoa(fundgain,num,10);
					addstr(num);
					addstr("!");

					addjuice(*trouble[t],-3);

					refresh();
					getch();


					if(!LCSrandom(4))
						{
						if(clearformess)erase();
						else
							{
							makedelimiter(8,0);
							}

						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(8,1);
						addstr(trouble[t]->name);
						addstr(" has been arrested in a prostitution sting.");

						addjuice(*trouble[t],-7);

						refresh();
						getch();

						caught=1;

						removesquadinfo(*trouble[t]);
						trouble[t]->carid=-1;
						trouble[t]->location=ps;
						trouble[t]->weapon.type=WEAPON_NONE;
						trouble[t]->weapon.ammo=0;
						trouble[t]->activity.type=ACTIVITY_NONE;
						trouble[t]->lawflag2|=LAWFLAG2_PROSTITUTION;
						}
					}
				else
					{
					if(clearformess)erase();
					else
						{
						makedelimiter(8,0);
						}

					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(8,1);
					addstr(trouble[t]->name);
					switch(LCSrandom(2))
						{
						case 0:addstr(" made some money playing shell games.");break;
						case 1:addstr(" found some money on the ground.");break;
						}

					refresh();
					getch();

					fundgain=LCSrandom(11)+10;
					}

				if(!caught)
					{
					funds+=fundgain;
					stat_funds+=fundgain;
					moneygained_hustling+=fundgain;
					}

				trouble.erase(trouble.begin() + t);
				}
			}

		//NOW DO THE REST
		if(trouble.size()>0)
			{
			long juiceval=0;
			char done=0;
			char lawflag=1;
			unsigned short crime=0;

			if(clearformess)erase();
			else
				{
				makedelimiter(8,0);
				}

			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(8,1);
			if(trouble.size()>1)addstr("Your Activists have ");
			else {addstr(trouble[0]->name);addstr(" has ");}

			long power=0;
			for(int t=0;t<trouble.size();t++)
				{
				power+=trouble[t]->skill[SKILL_PERSUASION]+
					trouble[t]->attval(ATTRIBUTE_CHARISMA)+
					trouble[t]->attval(ATTRIBUTE_AGILITY)+
					trouble[t]->attval(ATTRIBUTE_HEART)+
					trouble[t]->attval(ATTRIBUTE_STRENGTH)+
					trouble[t]->attval(ATTRIBUTE_INTELLIGENCE);
				}
			power+=tfund/10;

			long mod=1;
			if(LCSrandom(100)<power)mod++;
			if(LCSrandom(100)<power)mod++;
			if(LCSrandom(1000)<power)mod++;
			if(LCSrandom(1000)<power)mod++;
			if(LCSrandom(10000)<power)mod++;
			if(LCSrandom(10000)<power)mod++;

			do
				{
				switch(LCSrandom(8))
					{
					case 0:
						addstr("run around uptown splashing paint on fur coats!");
						juiceval=2;
						lawflag=1;
						crime=LAWFLAG_ASSAULT;
						change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
						change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,0);
						if(attitude[VIEW_ANIMALRESEARCH]<75)change_public_opinion(VIEW_ANIMALRESEARCH,mod,0);
						done=1;
						break;
					case 1:
						{
						if(law[LAW_GAY]<2)
							{
							addstr("disrupted a traditional wedding at a church!");
							change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
							change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,0);
							if(attitude[VIEW_GAY]<75)change_public_opinion(VIEW_GAY,mod,0);
							juiceval=2;
							lawflag=2;
							crime=LAWFLAG2_DISTURBANCE;
							done=1;
							}
						break;
						}
					case 2:
						{
						if(law[LAW_ABORTION]<2)
							{
							addstr("posted horrifying dead abortion doctor pictures downtown!");
							change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
							change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,0);
							if(attitude[VIEW_ABORTION]<75)change_public_opinion(VIEW_ABORTION,mod,0);
							juiceval=1;
							done=1;
							}
						break;
						}
					case 3:
						{
						addstr("distributed fliers graphically illustrating prison life!");
						change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
						change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,0);
						if(attitude[VIEW_PRISONS]<75)change_public_opinion(VIEW_PRISONS,mod,0);
						juiceval=1;
						done=1;
						break;
						}
					case 4:
						{
						if(law[LAW_POLICEBEHAVIOR]<2)
							{
							addstr("gone downtown and reenacted a police beating!");
							change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
							change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,0);
							if(attitude[VIEW_POLICEBEHAVIOR]<75)change_public_opinion(VIEW_POLICEBEHAVIOR,mod,0);
							juiceval=2;
							lawflag=2;
							crime=LAWFLAG2_DISTURBANCE;
							done=1;
							}
						break;
						}
					case 5:
						{
						if(law[LAW_NUCLEARPOWER]<2)
							{
							if(trouble.size()>1)addstr("dressed up and pretended to be radioactive mutants!");
							else addstr("dressed up and pretended to be a radioactive mutant!");
							change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
							change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,0);
							if(attitude[VIEW_NUCLEARPOWER]<75)change_public_opinion(VIEW_NUCLEARPOWER,mod,0);
							juiceval=2;
							lawflag=2;
							crime=LAWFLAG2_DISTURBANCE;
							done=1;
							}
						break;
						}
					case 6:
						{
						if(law[LAW_POLLUTION]<2)
							{
							addstr("squirted business people with fake polluted water!");
							change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
							change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,0);
							if(attitude[VIEW_POLLUTION]<75)change_public_opinion(VIEW_POLLUTION,mod,0);
							juiceval=2;
							lawflag=2;
							crime=LAWFLAG2_DISTURBANCE;
							done=1;
							}
						break;
						}
					case 7:
						{
						if(law[LAW_DEATHPENALTY]<2)
							{
							addstr("distributed fliers graphically illustrating executions!");
							change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
							change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,0);
							if(attitude[VIEW_DEATHPENALTY]<75)change_public_opinion(VIEW_DEATHPENALTY,mod,0);
							juiceval=1;
							done=1;
							}
						break;
						}
					}
				}while(!done);

			refresh();
			getch();

			if(crime!=0)
				{
				for(int t=0;t<trouble.size();t++)
					{
					dodgelawroll=LCSrandom(trouble[t]->skill[SKILL_PERSUASION]+
						trouble[t]->skill[SKILL_DISGUISE]+
						trouble[t]->attval(ATTRIBUTE_CHARISMA)+
						trouble[t]->attval(ATTRIBUTE_AGILITY)+
						trouble[t]->attval(ATTRIBUTE_INTELLIGENCE)+1)+
						LCSrandom(tfund/100+1);

					if(dodgelawroll==0)
						{
						if(clearformess)erase();
						else
							{
							makedelimiter(8,0);
							}

						if(LCSrandom(2))
							{
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(8,1);
							addstr(trouble[t]->name);
							addstr(" has been arrested.");

							refresh();
							getch();

							removesquadinfo(*trouble[t]);
							trouble[t]->carid=-1;
							trouble[t]->location=ps;
							trouble[t]->weapon.type=WEAPON_NONE;
							trouble[t]->weapon.ammo=0;
							trouble[t]->activity.type=ACTIVITY_NONE;
							if(lawflag==1)trouble[t]->lawflag|=crime;
							else trouble[t]->lawflag2|=crime;
							}
						else
							{
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(8,1);
							addstr(trouble[t]->name);
							addstr(" is cornered by a mob of angry rednecks.");

							refresh();
							getch();

							trouble[t]->activity.type=ACTIVITY_NONE;

							if(clearformess)erase();
							else
								{
								makedelimiter(8,0);
								}

							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(8,1);
							addstr(trouble[t]->name);
							addstr(" is severely beaten before the mob is broken up.");

							refresh();
							getch();

							if(trouble[t]->juice>=100||LCSrandom(100)<trouble[t]->juice)
								{
								addjuice(*trouble[t],-5);
								if(trouble[t]->blood>50)trouble[t]->blood=50;
								}
							else
								{
								addjuice(*trouble[t],-10);
								if(trouble[t]->blood>10)trouble[t]->blood=10;

								if(!LCSrandom(5))
									{
									switch(LCSrandom(15))
										{
										case 0:
											if(trouble[t]->special[SPECIALWOUND_LOWERSPINE]==1)
												{
												set_color(COLOR_WHITE,COLOR_BLACK,1);
												move(8,1);
												addstr(trouble[t]->name);
												addstr("'s lower spine has been broken!");
												trouble[t]->special[SPECIALWOUND_LOWERSPINE]=0;
												refresh();
												getch();
												}
											break;
										case 1:
											if(trouble[t]->special[SPECIALWOUND_UPPERSPINE]==1)
												{
												set_color(COLOR_WHITE,COLOR_BLACK,1);
												move(8,1);
												addstr(trouble[t]->name);
												addstr("'s upper spine has been broken!");
												trouble[t]->special[SPECIALWOUND_UPPERSPINE]=0;
												refresh();
												getch();
												}
											break;
										case 2:
											if(trouble[t]->special[SPECIALWOUND_NECK]==1)
												{
												set_color(COLOR_WHITE,COLOR_BLACK,1);
												move(8,1);
												addstr(trouble[t]->name);
												addstr("'s neck has been broken!");
												trouble[t]->special[SPECIALWOUND_NECK]=0;
												refresh();
												getch();
												}
											break;
										case 3:
											if(trouble[t]->special[SPECIALWOUND_TEETH]>0)
												{
												set_color(COLOR_WHITE,COLOR_BLACK,1);
												move(8,1);
												addstr(trouble[t]->name);
												if(trouble[t]->special[SPECIALWOUND_TEETH]>1)addstr("'s teeth have been smashed out on the curb.");
												else addstr("'s tooth has been pulled out with pliers!");
												trouble[t]->special[SPECIALWOUND_TEETH]=0;
												refresh();
												getch();
												}
											break;
										default:
											{
											if(trouble[t]->special[SPECIALWOUND_RIBS]>0)
												{
												int ribminus=LCSrandom(RIBNUM)+1;
												if(ribminus>trouble[t]->special[SPECIALWOUND_RIBS])ribminus=trouble[t]->special[SPECIALWOUND_RIBS];
												char num[20];
												itoa(ribminus,num,10);

												move(8,1);
												if(ribminus>1)
													{
													addstr(num);
													addstr(" of ");
													addstr(trouble[t]->name);
													addstr("'s ribs are ");
													}
												else if(trouble[t]->special[SPECIALWOUND_RIBS]>1)
													{
													addstr("One of ");
													addstr(trouble[t]->name);
													addstr("'s rib is ");
													}
												else
													{
													addstr(trouble[t]->name);
													addstr("'s last unbroken rib is ");
													}

												addstr("broken!");
												refresh();
												getch();

												trouble[t]->special[SPECIALWOUND_RIBS]-=ribminus;
												}
											break;
											}
										}
									}
								}
							}
						}
					}
				}

			for(int h=0;h<trouble.size();h++)
				{
				addjuice(*trouble[h],juiceval,20);
				}
			}
		}

	//BURY THE DEAD
	if(bury.size()>0)
		{
		char haveburied=0;
		char caught=0;

		for(int p=pool.size()-1;p>=0;p--)
			{
			if(pool[p]->alive)continue;

			//MAKE BASE LOOT
			makeloot(*pool[p],location[bury[0]->base]->loot);

			//BURY
			delete pool[p];
			pool.erase(pool.begin() + p);

			if(!caught)
				{
				long dodgelawroll=0,ndodgelawroll;

				for(int b=0;b<bury.size();b++)
					{
					ndodgelawroll=LCSrandom(bury[b]->skill[SKILL_PERSUASION]+
						bury[b]->skill[SKILL_DISGUISE]+
						bury[b]->attval(ATTRIBUTE_CHARISMA)+
						bury[b]->attval(ATTRIBUTE_AGILITY)+
						bury[b]->attval(ATTRIBUTE_INTELLIGENCE)+1);
					if(ndodgelawroll>dodgelawroll)dodgelawroll=ndodgelawroll;
					}
				if(dodgelawroll==0)caught=1;
				}
			}

		if(caught)
			{
			if(clearformess)erase();
			else
				{
				makedelimiter(8,0);
				}

			if(bury.size()>0)addstr("Some Liberals have");
			else {addstr(bury[0]->name);addstr(" has");}
			addstr(" been while disposing of bodies.");

			refresh();
			getch();

			for(int b=0;b<bury.size();b++)
				{
				removesquadinfo(*bury[b]);
				bury[b]->carid=-1;
				bury[b]->location=ps;
				bury[b]->weapon.type=WEAPON_NONE;
				bury[b]->weapon.ammo=0;
				bury[b]->activity.type=ACTIVITY_NONE;
				bury[b]->lawflag2|=LAWFLAG2_BURIAL;
				}
			}
		}
}

void fundreport(char &clearformess)
{
	if(disbanding)return;

	//MUST HAVE CATEGORIES FOR ALL FUND CHANGES
	if(moneygained_donate>0||
		moneygained_brownies>0||
		moneylost_trouble>0||
		moneylost_manufacture>0||
		moneylost_rent>0||
		moneylost_legal>0||
		moneygained_goods>0||
		moneygained_ccfraud>0||
		moneygained_hustling>0||
		moneygained_thievery>0||
		moneylost_goods>0||
		moneylost_compound>0||
		moneylost_hostage>0)
		{
		clearformess=1;
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,1);

		move(0,0);
		addstr("Liberal Crime Squad:  Monthly Action Report");

		int y=2;

		long totalmoney=0;

		//DONATIONS
		if(moneygained_donate>0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addstr("Donations . . . . . . . . . . . . . . . . . . . . . . . .");

			set_color(COLOR_GREEN,COLOR_BLACK,0);
			move(y,60);
			char num[20];
			itoa(moneygained_donate,num,10);
			addstr("$");
			addstr(num);

			y++;

			totalmoney+=moneygained_donate;
			}

		//BROWNIES
		if(moneygained_brownies>0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addstr("Brownies  . . . . . . . . . . . . . . . . . . . . . . . .");

			set_color(COLOR_GREEN,COLOR_BLACK,0);
			move(y,60);
			char num[20];
			itoa(moneygained_brownies,num,10);
			addstr("$");
			addstr(num);

			y++;

			totalmoney+=moneygained_brownies;
			}

		//CCFRAUD
		if(moneygained_ccfraud>0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addstr("Credit Card Fraud . . . . . . . . . . . . . . . . . . . .");

			set_color(COLOR_GREEN,COLOR_BLACK,0);
			move(y,60);
			char num[20];
			itoa(moneygained_ccfraud,num,10);
			addstr("$");
			addstr(num);

			y++;

			totalmoney+=moneygained_ccfraud;
			}

		//HUSTLING
		if(moneygained_hustling>0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addstr("Hustling  . . . . . . . . . . . . . . . . . . . . . . . .");

			set_color(COLOR_GREEN,COLOR_BLACK,0);
			move(y,60);
			char num[20];
			itoa(moneygained_hustling,num,10);
			addstr("$");
			addstr(num);

			y++;

			totalmoney+=moneygained_hustling;
			}

		//THIEVERY
		if(moneygained_thievery>0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addstr("Thievery  . . . . . . . . . . . . . . . . . . . . . . . .");

			set_color(COLOR_GREEN,COLOR_BLACK,0);
			move(y,60);
			char num[20];
			itoa(moneygained_thievery,num,10);
			addstr("$");
			addstr(num);

			y++;

			totalmoney+=moneygained_thievery;
			}

		//SALE
		if(moneygained_goods>0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addstr("Sale of Goods . . . . . . . . . . . . . . . . . . . . . .");

			set_color(COLOR_GREEN,COLOR_BLACK,0);
			move(y,60);
			char num[20];
			itoa(moneygained_goods,num,10);
			addstr("$");
			addstr(num);

			y++;

			totalmoney+=moneygained_goods;
			}

		//PURCHASE
		if(moneylost_goods>0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addstr("Purchase of Goods . . . . . . . . . . . . . . . . . . . .");

			set_color(COLOR_RED,COLOR_BLACK,0);
			move(y,60);
			char num[20];
			itoa(moneylost_goods,num,10);
			addstr("$");
			addstr(num);

			y++;

			totalmoney-=moneylost_goods;
			}

		//TROUBLE
		if(moneylost_trouble>0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addstr("Liberal Disobedience  . . . . . . . . . . . . . . . . . .");

			set_color(COLOR_RED,COLOR_BLACK,0);
			move(y,60);
			char num[20];
			itoa(moneylost_trouble,num,10);
			addstr("$");
			addstr(num);

			y++;

			totalmoney-=moneylost_trouble;
			}

		//RENT
		if(moneylost_rent>0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addstr("Rent  . . . . . . . . . . . . . . . . . . . . . . . . . .");

			set_color(COLOR_RED,COLOR_BLACK,0);
			move(y,60);
			char num[20];
			itoa(moneylost_rent,num,10);
			addstr("$");
			addstr(num);

			y++;

			totalmoney-=moneylost_rent;
			}

		//MANUFACTURE
		if(moneylost_manufacture>0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addstr("Manufacture . . . . . . . . . . . . . . . . . . . . . . .");

			set_color(COLOR_RED,COLOR_BLACK,0);
			move(y,60);
			char num[20];
			itoa(moneylost_manufacture,num,10);
			addstr("$");
			addstr(num);

			y++;

			totalmoney-=moneylost_manufacture;
			}

		//LEGAL
		if(moneylost_legal>0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addstr("Legal Fees  . . . . . . . . . . . . . . . . . . . . . . .");

			set_color(COLOR_RED,COLOR_BLACK,0);
			move(y,60);
			char num[20];
			itoa(moneylost_legal,num,10);
			addstr("$");
			addstr(num);

			y++;

			totalmoney-=moneylost_legal;
			}

		//COMPOUND
		if(moneylost_compound>0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addstr("Infrastructure  . . . . . . . . . . . . . . . . . . . . .");

			set_color(COLOR_RED,COLOR_BLACK,0);
			move(y,60);
			char num[20];
			itoa(moneylost_compound,num,10);
			addstr("$");
			addstr(num);

			y++;

			totalmoney-=moneylost_compound;
			}

		//HOSTAGES
		if(moneylost_hostage>0)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addstr("Hostage Tending . . . . . . . . . . . . . . . . . . . . .");

			set_color(COLOR_RED,COLOR_BLACK,0);
			move(y,60);
			char num[20];
			itoa(moneylost_hostage,num,10);
			addstr("$");
			addstr(num);

			y++;

			totalmoney-=moneylost_hostage;
			}

		//TOTAL
		y++;
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(y,0);
		addstr("Total:");

		if(totalmoney>0)set_color(COLOR_GREEN,COLOR_BLACK,1);
		else if(totalmoney<0)set_color(COLOR_RED,COLOR_BLACK,1);
		else set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(y,60);
		char num[20];
		if(totalmoney<0)itoa(-totalmoney,num,10);
		else itoa(totalmoney,num,10);
		addstr("$");
		addstr(num);

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(24,0);
		addstr("Press any key to reflect on the report.");

		refresh();
		getch();

		moneygained_donate=0;
		moneygained_brownies=0;
		moneygained_goods=0;
		moneygained_ccfraud=0;
		moneygained_hustling=0;
		moneygained_thievery=0;
		moneylost_goods=0;
		moneylost_trouble=0;
		moneylost_manufacture=0;
		moneylost_rent=0;
		moneylost_legal=0;
		moneylost_compound=0;
		moneylost_hostage=0;
		}
}

long select_troublefundinglevel(creaturest *cr)
{
	long flevel=-1;

	erase();

	set_color(COLOR_WHITE,COLOR_BLACK,0);
	move(0,0);
	addstr("Select a Funding Level for this Operation:");

	move(2,0);
	addstr("A - Don't spend anything.  ");
	addstr(cr->name);
	addstr(" just needs something constructive to do.");

	move(3,0);
	addstr("B - $20 per day.  Some minor purchases are needed.");

	move(4,0);
	addstr("C - $50 per day.  Disobedience costs money.");

	move(5,0);
	addstr("D - $100 per day.  Enough to be really disobedient.");

	move(6,0);
	addstr("E - $500 per day.  The Machine will buckle under the weight of");
	move(7,0);
	addstr("    ");
	addstr(cr->name);
	addstr("'s Numerous and Varied Liberal Acts.");

	move(9,0);
	addstr("X - On second thought, this isn't a job for ");
	addstr(cr->name);
	addstr(".");

	refresh();
	int c=getch();
	translategetch(c);

	if(c=='a')flevel=0;
	if(c=='b')flevel=20;
	if(c=='c')flevel=50;
	if(c=='d')flevel=100;
	if(c=='e')flevel=500;

	return flevel;
}

char carselect(creaturest &cr,short &cartype)
{
	cartype=-1;

	vector<int> cart;
	for(int a=0;a<VEHICLENUM;a++)
		{
		if(difficulty_carfind(a)<10)cart.push_back(a);
		}

	short page=0;

	char str[200];

	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(0,0);
		addstr("What type of car will ");
		addstr(cr.name);
		addstr(" try to find and steal today?");
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(1,0);
		addstr("----TYPE-----------------------------------------DIFFICULTY TO FIND UNATTENDED--");

		int y=2,difficulty;
		for(int p=page*19;p<cart.size()&&p<page*19+19;p++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addch(y+'A'-2);addstr(" - ");
			getcarfull(str,cart[p]);
			addstr(str);

			move(y,49);
			difficulty=difficulty_carfind(cart[p]);
			switch(difficulty)
				{
				case 0:
					set_color(COLOR_GREEN,COLOR_BLACK,1);
					addstr("Simple");
					break;
				case 1:
					set_color(COLOR_CYAN,COLOR_BLACK,1);
					addstr("Very Easy");
					break;
				case 2:
					set_color(COLOR_CYAN,COLOR_BLACK,0);
					addstr("Easy");
					break;
				case 3:
					set_color(COLOR_BLUE,COLOR_BLACK,1);
					addstr("Below Average");
					break;
				case 4:
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					addstr("Average");
					break;
				case 5:
					set_color(COLOR_WHITE,COLOR_BLACK,0);
					addstr("Above Average");
					break;
				case 6:
					set_color(COLOR_YELLOW,COLOR_BLACK,1);
					addstr("Hard");
					break;
				case 7:
					set_color(COLOR_MAGENTA,COLOR_BLACK,0);
					addstr("Very Hard");
					break;
				case 8:
					set_color(COLOR_MAGENTA,COLOR_BLACK,1);
					addstr("Extremely Difficult");
					break;
				case 9:
					set_color(COLOR_RED,COLOR_BLACK,0);
					addstr("Nearly Impossible");
					break;
				default:
					set_color(COLOR_RED,COLOR_BLACK,1);
					addstr("Impossible");
					break;
				}

			y++;
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(22,0);
		addstr("Press a Letter to select a Type of Car");
		move(23,0);
		if(interface_pgup=='[')
			{
			addstr("[] to view other Liberal pages.");
			}
		else
			{
			addstr("PGUP/PGDN to view other Liberal pages.");
			}

		refresh();

		int c=getch();
		translategetch(c);

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*19<cart.size())page++;

		if(c>='a'&&c<='s')
			{
			int p=page*19+(int)(c-'a');
			if(p<cart.size())
				{
				cartype=cart[p];
				return 1;
				}
			}

		if(c==10)break;
		}while(1);

	return 0;
}

void getwheelchair(creaturest &cr,char &clearformess)
{
	if(clearformess)
		{
		erase();
		}
	else
		{
		makedelimiter(8,0);
		}

	if(LCSrandom(2))
		{
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(8,1);
		addstr(cr.name);
		addstr(" has procured a wheelchair.");

		cr.flag|=CREATUREFLAG_WHEELCHAIR;
		}
	else
		{
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(8,1);
		addstr(cr.name);
		addstr(" was unable to get a wheelchair.  Maybe tomorrow...");
		}

	refresh();
	getch();
}

char stealcar(creaturest &cr,char &clearformess)
{
	clearformess=1;

	short cartype;
	char str[80];

	if(carselect(cr,cartype))
		{
		long diff=difficulty_carfind(cartype);

		vehiclest *v=new vehiclest;

		int old=cartype;

		//ROUGH DAY
		if(LCSrandom(10)<diff)
			{
			do
				{
				cartype=LCSrandom(VEHICLENUM);
				if(LCSrandom(10)<difficulty_carfind(cartype))cartype=old;
				}while(cartype==old);
			}

		v->init(cartype);

		//THEFT SEQUENCE
		erase();
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(0,0);
		addstr("Adventures in Liberal Car Theft");
		printcreatureinfo(&cr);
		makedelimiter(8,0);

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(10,0);
		addstr(cr.name);
		addstr(" looks around for an accessible vehicle...");

		refresh();
		getch();

		if(old!=cartype)
			{
			move(11,0);
			addstr(cr.name);
			addstr(" was unable to find a ");
			getcarfull(str,old);
			addstr(str);
			addstr(" but did find a ");
			getcarfull(str,cartype);
			addstr(str);
			addstr(".");

			refresh();
			getch();
			}
		else
			{
			move(11,0);
			addstr(cr.name);
			addstr(" found a ");
			getcarfull(str,cartype);
			addstr(str);
			addstr(".");

			refresh();
			getch();
			}

		//APPROACH?
		erase();
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(0,0);
		addstr("Adventures in Liberal Car Theft");
		printcreatureinfo(&cr);
		makedelimiter(8,0);

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(10,0);
		addstr(cr.name);
		addstr(" looks from a distance at an empty ");
		getcarfull(str,*v);
		addstr(str);
		addstr(".");

		move(12,0);
		addstr("A - Approach the driver's side door.");
		move(13,0);
		addstr("X - Call it a day.");

		refresh();

		int c;

		do
			{
			c=getch();
			translategetch(c);
			if(c=='a')break;
			if(c=='x'){delete v;return 1;}
			}while(1);

		//SECURITY?
		char alarmon=0;

		char sensealarm=0;
		if(LCSrandom(100)<sensealarmchance(cartype))sensealarm=1;
		char touchalarm=0;
		if(LCSrandom(100)<touchalarmchance(cartype))touchalarm=1;

		do
			{
			erase();
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(0,0);
			addstr("Adventures in Liberal Car Theft");
			printcreatureinfo(&cr);
			makedelimiter(8,0);

			getcarfull(str,*v);

			if(alarmon)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(10,0);
				if(sensealarm)addstr("THE VIPER");
				else addstr(str);
				addstr(":  ");
				set_color(COLOR_RED,COLOR_BLACK,1);
				if(sensealarm)addstr("STAND AWAY FROM THE VEHICLE!  <BEEP!!> <BEEP!!>");
				else addstr("<BEEP!!> <BEEP!!> <BEEP!!> <BEEP!!>");
				}
			else if(sensealarm)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(10,0);
				addstr("THE VIPER:  ");
				set_color(COLOR_RED,COLOR_BLACK,1);
				addstr("THIS IS THE VIPER!  STAND AWAY!");
				}
			else
				{
				set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(10,0);
				addstr(cr.name);
				addstr(" stands by the ");
				addstr(str);
				addstr(".");
				}

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(12,0);
			addstr("A - Pick the lock.");
			move(13,0);
			addstr("B - Break the window.");
			move(14,0);
			if(!sensealarm)addstr("X - Call it a day.");
			else
				{
				if(!alarmon)
					{
					addstr("X - The Viper?  ");
					addstr(cr.name);
					addstr(" is deterred.");
					}
				else
					{
					addstr("X - Yes, the Viper has deterred ");
					addstr(cr.name);
					addstr(".");
					}
				}

			refresh();

			char method=0;

			do
				{
				c=getch();
				translategetch(c);
				if(c=='a')break;
				if(c=='b'){method=1;break;}
				if(c=='x'){delete v;return 1;}
				}while(1);

			char entered=0;

			//PICK LOCK
			if(method==0)
				{
				int attack=cr.attval(ATTRIBUTE_INTELLIGENCE)+
					cr.skill[SKILL_SECURITY];

				cr.skill_ip[SKILL_SECURITY]+=2;

				if(LCSrandom(11)<attack)
					{
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(16,0);
					addstr(cr.name);
					addstr(" jimmies the car door open.");
					refresh();getch();

					entered=1;
					}
				else
					{
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(16,0);
					addstr(cr.name);
					addstr(" fiddles with the lock with no luck.");
					refresh();getch();
					}
				}
			//BREAK WINDOW
			if(method==1)
				{
				int attack=cr.attval(ATTRIBUTE_STRENGTH)+
					bashstrengthmod(cr.weapon.type);

				if(LCSrandom(11)<attack)
					{
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(16,0);
					addstr(cr.name);
					addstr(" smashes the window");
					if(bashstrengthmod(cr.weapon.type)>0)
						{
						addstr(" with a ");
						char str[80];
						getweaponfull(str,cr.weapon.type);
						addstr(str);
						}
					addstr(".");
					refresh();getch();

					entered=1;
					}
				else
					{
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(16,0);
					addstr(cr.name);
					addstr(" wacks the window");
					if(bashstrengthmod(cr.weapon.type)>0)
						{
						addstr(" with a ");
						char str[80];
						getweaponfull(str,cr.weapon.type);
						addstr(str);
						}
					addstr(" but it is still somewhat intact.");
					refresh();getch();
					}
				}

			//ALARM CHECK
			int y=17;

			if(touchalarm||sensealarm)
				{
				if(!alarmon)
					{
					set_color(COLOR_YELLOW,COLOR_BLACK,1);
					move(y,0);y++;
					addstr("An alarm suddenly starts blaring!");
					refresh();
					getch();
					alarmon=1;
					}
				}

			//NOTICE CHECK
			if(!LCSrandom(50)||(!LCSrandom(5)&&alarmon))
				{
				set_color(COLOR_RED,COLOR_BLACK,1);
				move(y,0);y++;
				addstr(cr.name);
				addstr(" has been spotted by a passerby!");
				refresh();
				getch();

				//FOOT CHASE
				chaseseq.clean();
				chaseseq.location=0;
				cr.lawflag2|=LAWFLAG2_CARTHEFT;
				newsstoryst *ns=new newsstoryst;
					ns->type=NEWSSTORY_CARTHEFT;
				newsstory.push_back(ns);
				sitestory=ns;
				makechasers(-1,5);

				if(footchase(cr)){
					mode=GAMEMODE_BASE;
					delete v;return 1;}
				else {
					mode=GAMEMODE_BASE;
					delete v;return 0;}
				}

			if(entered)break;
			}while(1);

		//START CAR
		char keys_in_car=0;
		if(!LCSrandom(5))keys_in_car=1;

		do
			{
			erase();
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(0,0);
			addstr("Adventures in Liberal Car Theft");
			printcreatureinfo(&cr);
			makedelimiter(8,0);

			getcarfull(str,*v);

			int y=10;

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);y++;
			addstr(cr.name);
			addstr(" is behind the wheel of a ");
			addstr(str);
			addstr(".");

			if(alarmon)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(y,0);y++;
				if(sensealarm)addstr("THE VIPER");
				else addstr(str);
				addstr(":  ");
				set_color(COLOR_RED,COLOR_BLACK,1);
				if(sensealarm)addstr("REMOVE YOURSELF FROM THE VEHICLE!  <BEEP!!> <BEEP!!>");
				else addstr("<BEEP!!> <BEEP!!> <BEEP!!> <BEEP!!>");
				}

			y++;

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);y++;
			addstr("A - Hotwire the car.");
			move(y,0);y++;
			addstr("B - Desperately search for keys.");
			move(y,0);y++;
			if(!sensealarm)addstr("X - Call it a day.");
			else {addstr("X - The Viper has finally deterred ");addstr(cr.name);addstr(".");}

			refresh();

			y++;

			char method=0;

			do
				{
				c=getch();
				translategetch(c);
				if(c=='a')break;
				if(c=='b'){method=1;break;}
				if(c=='x'){delete v;return 1;}
				}while(1);

			char started=0;

			//HOTWIRE CAR
			if(method==0)
				{
				int attack=cr.attval(ATTRIBUTE_INTELLIGENCE)+
					cr.skill[SKILL_SECURITY];

				cr.skill_ip[SKILL_SECURITY]+=2;

				if(LCSrandom(11)<attack)
					{
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(y,0);y++;
					addstr(cr.name);
					addstr(" hotwires the car!");
					refresh();getch();

					started=1;
					}
				else
					{
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(y,0);y++;
					addstr(cr.name);
					addstr(" fiddles with ignition, but the car doesn't start.");
					refresh();getch();
					}
				}
			//KEYS
			if(method==1)
				{
				int attack=cr.attval(ATTRIBUTE_INTELLIGENCE);

				if(LCSrandom(11)<attack&&keys_in_car)
					{
					set_color(COLOR_GREEN,COLOR_BLACK,1);
					move(y,0);y++;
					if(law[LAW_FREESPEECH]==-2)addstr("Holy [Car Keys]!  ");
					else addstr("Holy shit!  ");
					addstr(cr.name);
					addstr(" found the keys ");
					switch(LCSrandom(5))
						{
						case 0:addstr("in the ignition.  Damn.");break;
						case 1:addstr("under the front seat");break;
						case 2:addstr("under the back seat");break;
						case 3:addstr("above the pull-down sunblock thingy");break;
						case 4:addstr("in the glove compartment.");break;
						}
					addstr("!");
					refresh();getch();

					started=1;
					}
				else
					{
					set_color(COLOR_WHITE,COLOR_BLACK,1);
					move(y,0);y++;
					addstr(cr.name);
					addstr(": <rummaging> ");
					set_color(COLOR_GREEN,COLOR_BLACK,1);
					switch(LCSrandom(5))
						{
						case 0:addstr("They've gotta be in here somewhere...");break;
						case 1:
							if(law[LAW_FREESPEECH]==-2)addstr("[Shoot]!  Where are they?!");
							else addstr("Fuck!  Where are they?!");
							break;
						case 2:addstr("Come on, baby, come to me...");break;
						case 3:
							if(law[LAW_FREESPEECH]==-2)addstr("[Darn] it...");
							else addstr("Dammit...");
							break;
						case 4:addstr("I wish I could hotwire this thing...");break;
						}
					refresh();getch();
					}
				}

			//NOTICE CHECK
			if(!started&&(!LCSrandom(50)||(!LCSrandom(5)&&alarmon)))
				{
				set_color(COLOR_RED,COLOR_BLACK,1);
				move(y,0);y++;
				addstr(cr.name);
				addstr(" has been spotted by a passerby!");
				refresh();
				getch();

				//FOOT CHASE
				chaseseq.clean();
				chaseseq.location=0;
				cr.lawflag2|=LAWFLAG2_CARTHEFT;
				newsstoryst *ns=new newsstoryst;
					ns->type=NEWSSTORY_CARTHEFT;
				newsstory.push_back(ns);
				sitestory=ns;
				makechasers(-1,5);

				if(footchase(cr)){
					mode=GAMEMODE_BASE;
					delete v;return 1;}
				else {
					mode=GAMEMODE_BASE;
					delete v;return 0;}
				}

			if(started)break;
			}while(1);

		//CHASE SEQUENCE
			//CAR IS OFFICIAL, THOUGH CAN BE DELETE BY chasesequence()
		if(v->type==VEHICLE_SUV||
			v->type==VEHICLE_POLICECAR)addjuice(cr,2,20);

		chaseseq.clean();
		chaseseq.location=0;
		int chaselev=!LCSrandom(4);
		if(chaselev>0)
			{
			chaselev=1;
			cr.lawflag2|=LAWFLAG2_CARTHEFT;
			newsstoryst *ns=new newsstoryst;
				ns->type=NEWSSTORY_CARTHEFT;
			newsstory.push_back(ns);
			sitestory=ns;
			makechasers(-1,chaselev);
			}

		vehicle.push_back(v);
		if(chasesequence(cr,*v))
			{
			v->location=cr.base;
			return 1;
			}
		else return 0;//do not need to delete vehicle
		}

	return 0;
}

void getcarfull(char *str,int type)
{
	strcpy(str,"");

	switch(type)
		{
		case VEHICLE_VAN:strcat(str,"Van");break;
		case VEHICLE_STATIONWAGON:strcat(str,"Station Wagon");break;
		case VEHICLE_SPORTSCAR:strcat(str,"Sportscar");break;
		case VEHICLE_BUG:strcat(str,"Bug");break;
		case VEHICLE_PICKUP:strcat(str,"Pickup Truck");break;
		case VEHICLE_POLICECAR:strcat(str,"Police Cruiser");break;
		case VEHICLE_TAXICAB:strcat(str,"Taxicab");break;
		case VEHICLE_SUV:strcat(str,"SUV");break;
		case VEHICLE_AGENTCAR:strcat(str,"Car");break;
		case VEHICLE_JEEP:strcat(str,"Jeep");break;
		}
}

void getcar(char *str,int type)
{
	strcpy(str,"");
	switch(type)
		{
		//LIMIT-------------------------------------:-----:
		case VEHICLE_VAN:				strcat(str,"Van");break;
		case VEHICLE_STATIONWAGON:		strcat(str,"Wagon");break;
		case VEHICLE_SPORTSCAR:			strcat(str,"Sport");break;
		case VEHICLE_BUG:				strcat(str,"Bug");break;
		case VEHICLE_PICKUP:			strcat(str,"Pickup");break;
		case VEHICLE_POLICECAR:			strcat(str,"Cruiser");break;
		case VEHICLE_TAXICAB:			strcat(str,"Taxicab");break;
		case VEHICLE_SUV:				strcat(str,"SUV");break;
		case VEHICLE_AGENTCAR:			strcat(str,"Car");break;
		case VEHICLE_JEEP:				strcat(str,"Jeep");break;
		}
}

void getcarfull(char *str,vehiclest &car,char halffull)
{
	char str2[80],num[20];
	strcpy(str,"");
	if(car.color!=naturalcarcolor(car.type))
		{
		getcarcolor(str2,car.color);
		strcat(str,str2);
		strcat(str," ");
		}
	if(car.myear!=-1)
		{
		itoa(car.myear,num,10);
		strcat(str,num);
		strcat(str," ");
		}
	if(halffull)getcar(str2,car.type);
	else getcarfull(str2,car.type);
	strcat(str,str2);
}

short naturalcarcolor(int type)
{
	switch(type)
		{
		case VEHICLE_POLICECAR:return VEHICLECOLOR_POLICE;
		case VEHICLE_TAXICAB:return VEHICLECOLOR_TAXI;
		}

	return -1;
}

void getcarcolor(char *str,int type)
{
	strcpy(str,"");
	switch(type)
		{
		case VEHICLECOLOR_RED:strcat(str,"Red");break;
		case VEHICLECOLOR_WHITE:strcat(str,"White");break;
		case VEHICLECOLOR_BLUE:strcat(str,"Blue");break;
		case VEHICLECOLOR_BEIGE:strcat(str,"Beige");break;
		case VEHICLECOLOR_POLICE:strcat(str,"Police-Marked");break;
		case VEHICLECOLOR_TAXI:strcat(str,"Taxi-Striped");break;
		case VEHICLECOLOR_BLACK:strcat(str,"Black");break;
		}
}

long difficulty_carfind(int type)
{
	switch(type)
		{
		case VEHICLE_VAN:return 2;
		case VEHICLE_STATIONWAGON:return 1;
		case VEHICLE_SPORTSCAR:return 3;
		case VEHICLE_BUG:return 2;
		case VEHICLE_PICKUP:return 1;
		case VEHICLE_POLICECAR:return 7;
		case VEHICLE_TAXICAB:return 8;
		case VEHICLE_SUV:return 1;
		case VEHICLE_JEEP:return 6;
		}

	return 10;
}

char maskselect(creaturest *cr,short &mask)
{
	mask=-1;

	vector<int> masktype;
	for(int a=0;a<MASKNUM;a++)
		{
		switch(a)
			{
			case MASK_JESUS:
			case MASK_COLEMAN_GARY:
			case MASK_MADONNA:
			case MASK_SPEARS:
			case MASK_EMINEM:
			case MASK_AGUILERA:
			case MASK_WAHLBERG:
			case MASK_IGGYPOP:
			case MASK_CASH:
			case MASK_BINLADEN:
			case MASK_LORDS:
			case MASK_SHIELDS:
			case MASK_JACKSON_MICHAEL:
			case MASK_CRUTHERS:
			case MASK_KING_DON:
				break;
			default:
				masktype.push_back(a);
			}
		}

	short page=0;

	char str[200];

	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(0,0);
		addstr("Which mask will ");
		addstr(cr->name);
		addstr(" buy?");
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(1,0);
		addstr("----PRODUCT NAME-----------------------DESCRIPTION------------------------------");

		int y=2;
		for(int p=page*19;p<masktype.size()&&p<page*19+19;p++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addch(y+'A'-2);addstr(" - ");
			getarmorfull(str,ARMOR_MASK,masktype[p]);
			addstr(str);

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,39);
			getmaskdesc(str,masktype[p]);
			addstr(str);

			y++;
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(22,0);
		addstr("Press a Letter to select a Mask");
		move(23,0);
		if(interface_pgup=='[')
			{
			addstr("[] to view other Liberal pages.");
			}
		else
			{
			addstr("PGUP/PGDN to view other Liberal pages.");
			}
		move(24,0);
		addstr("Z - Surprise ");
		addstr(cr->name);
		addstr(" With a Random Mask");

		refresh();

		int c=getch();
		translategetch(c);

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*19<masktype.size())page++;

		if(c>='a'&&c<='s')
			{
			int p=page*19+(int)(c-'a');
			if(p<masktype.size())
				{
				mask=masktype[p];
				return 1;
				}
			}
		if(c=='z')
			{
			mask=LCSrandom(MASKNUM);
			return 1;
			}

		if(c==10)break;
		}while(1);

	return 0;
}

void getmaskdesc(char *str,short mask)
{
	switch(mask)
		{
		//LIMIT-------------------------------------:--------------------------------------:
		case MASK_ASHCROFT:				strcpy(str,"U.S. Senator, Attorney General");break;
		case MASK_BARR:					strcpy(str,"U.S. Representative");break;
		case MASK_BLAIR:				strcpy(str,"British Prime Minister");break;
		case MASK_BUSH_BARBARA:			strcpy(str,"U.S. First Lady");break;
		case MASK_BUSH_GEORGE:			strcpy(str,"U.S. President");break;
		case MASK_BUSH_GEORGE2:			strcpy(str,"U.S. President");break;
		case MASK_CARTER:				strcpy(str,"U.S. President");break;
		case MASK_CHENEY:				strcpy(str,"U.S. Vice President, CEO");break;
		case MASK_CLINTON_BILL:			strcpy(str,"U.S. President");break;
		case MASK_CLINTON_HILLARY:		strcpy(str,"U.S. Senator, First Lady");break;
		case MASK_FALWELL:				strcpy(str,"Reverend");break;
		case MASK_FORD:					strcpy(str,"U.S. President");break;
		case MASK_GORE_AL:				strcpy(str,"U.S. Vice President");break;
		//LIMIT-------------------------------------:--------------------------------------:
		case MASK_GORE_TIPPER:			strcpy(str,"U.S. Second Lady");break;
		case MASK_HELMS:				strcpy(str,"U.S. Senator");break;
		case MASK_JASON:				strcpy(str,"A Hockey Mask a la Friday the 13th");break;
		case MASK_LINCOLN:				strcpy(str,"U.S. President, Assassinated");break;
		case MASK_KENNEDY_JOHN:			strcpy(str,"U.S. President, Assassinated");break;
		case MASK_KENNEDY_ROBERT:		strcpy(str,"U.S. Senator, Assassinated");break;
		case MASK_KISSINGER:			strcpy(str,"U.S. Secretary of State");break;
		case MASK_NIXON:				strcpy(str,"U.S. President, Resigned");break;
		case MASK_POWELL:				strcpy(str,"U.S. Secretary of State, General");break;
		case MASK_REAGAN_NANCY:			strcpy(str,"U.S. First Lady");break;
		case MASK_REAGAN_RONALD:		strcpy(str,"U.S. President");break;
		case MASK_RICE:					strcpy(str,"U.S. National Security Advisor");break;
		case MASK_ROBERTSON:			strcpy(str,"Televangelist");break;
		case MASK_RUMSFELD:				strcpy(str,"U.S. Secretary of Defense");break;
		case MASK_SATAN:				strcpy(str,"Prince of Darkness, Red with Horns");break;
		case MASK_STARR:				strcpy(str,"Independent Counsel");break;
		case MASK_THURMOND:				strcpy(str,"U.S. Senator");break;
		case MASK_WASHINGTON:			strcpy(str,"U.S. President");break;
		case MASK_WAXMAN:				strcpy(str,"U.S. Representative");break;
		case MASK_JESUS:				strcpy(str,"The Son of God, The Lamb, He died 4 U");break;
		case MASK_COLEMAN_GARY:			strcpy(str,"Actor, Candidate for Governor");break;
		case MASK_MADONNA:				strcpy(str,"Singer, Actor");break;
		case MASK_SPEARS:				strcpy(str,"Singer, Actor");break;
		case MASK_EMINEM:				strcpy(str,"Rapper, Actor");break;
		case MASK_AGUILERA:				strcpy(str,"Singer");break;
		case MASK_WAHLBERG:				strcpy(str,"Actor, Rapper");break;
		//LIMIT-------------------------------------:--------------------------------------:
		case MASK_IGGYPOP:				strcpy(str,"Singer, Actor");break;
		case MASK_CASH:					strcpy(str,"Singer");break;
		case MASK_BINLADEN:				strcpy(str,"Terrorist");break;
		case MASK_LORDS:				strcpy(str,"Actor");break;
		case MASK_SHIELDS:				strcpy(str,"Actor");break;
		case MASK_JACKSON_MICHAEL:		strcpy(str,"Singer");break;
		case MASK_CRUTHERS:				strcpy(str,"Singer, Actor");break;
		case MASK_KING_DON:				strcpy(str,"Boxing Promoter");break;
		}
}

void vehiclest::init(int t)
{
	id=curcarid;curcarid++;

	location=-1;
	type=t;
	switch(t)
		{
		case VEHICLE_JEEP:
			myear=year+1-LCSrandom(41);
			break;
		case VEHICLE_VAN:
			myear=1969+LCSrandom(6);
			break;
		case VEHICLE_STATIONWAGON:
			myear=year+1-LCSrandom(41);
			break;
		case VEHICLE_SPORTSCAR:
			myear=year+1-LCSrandom(21);
			break;
		case VEHICLE_BUG:
			myear=1969+LCSrandom(6);
			break;
		case VEHICLE_PICKUP:
			myear=year+1-LCSrandom(41);
			break;
		case VEHICLE_POLICECAR:
			myear=year+1-LCSrandom(21);
			break;
		case VEHICLE_TAXICAB:
			myear=year+1-LCSrandom(41);
			break;
		case VEHICLE_SUV:
			myear=1995+LCSrandom(year-1995+1);
			break;
		}
	switch(t)
		{
		case VEHICLE_POLICECAR:
			color=VEHICLECOLOR_POLICE;
			break;
		case VEHICLE_AGENTCAR:
			color=VEHICLECOLOR_BLACK;
			break;
		case VEHICLE_TAXICAB:
			color=VEHICLECOLOR_TAXI;
			break;
		case VEHICLE_JEEP:
		case VEHICLE_VAN:
		case VEHICLE_STATIONWAGON:
		case VEHICLE_SPORTSCAR:
		case VEHICLE_BUG:
		case VEHICLE_PICKUP:
		case VEHICLE_SUV:
			switch(LCSrandom(5))
				{
				case 0:color=VEHICLECOLOR_RED;break;
				case 1:color=VEHICLECOLOR_WHITE;break;
				case 2:color=VEHICLECOLOR_BLUE;break;
				case 3:color=VEHICLECOLOR_BEIGE;break;
				case 4:color=VEHICLECOLOR_BLACK;break;
				}
			break;
		}
}

long sensealarmchance(int ct)
{
	switch(ct)
		{
		case VEHICLE_SPORTSCAR:
			return 15;
		case VEHICLE_SUV:
			return 4;
		case VEHICLE_JEEP:
		case VEHICLE_VAN:
		case VEHICLE_STATIONWAGON:
		case VEHICLE_BUG:
		case VEHICLE_PICKUP:
			return 1;
		default:
			return 0;
		}
}

long touchalarmchance(int ct)
{
	switch(ct)
		{
		case VEHICLE_SPORTSCAR:
			return 95;
		case VEHICLE_SUV:
			return 85;
		case VEHICLE_JEEP:
		case VEHICLE_VAN:
		case VEHICLE_STATIONWAGON:
		case VEHICLE_BUG:
		case VEHICLE_PICKUP:
			return 10;
		default:
			return 0;
		}
}

long bashstrengthmod(int t)
{
	switch(t)
		{
		case WEAPON_BASEBALLBAT:
		case WEAPON_SWORD:
		case WEAPON_AXE:
		case WEAPON_MAUL:
			return 5;
		case WEAPON_CROWBAR:
		case WEAPON_SHOTGUN:
		case WEAPON_STAFF:
		case WEAPON_NIGHTSTICK:
		case WEAPON_REVOLVER_22:
		case WEAPON_REVOLVER_44:
		case WEAPON_SEMIPISTOL_9MM:
		case WEAPON_SEMIPISTOL_45:
		case WEAPON_SEMIRIFLE_M16:
		case WEAPON_SEMIRIFLE_AK47:
		case WEAPON_PITCHFORK:
			return 3;
		case WEAPON_KNIFE:
		case WEAPON_SHANK:
		case WEAPON_GAVEL:
		case WEAPON_CHAIN:
		case WEAPON_HAMMER:
		case WEAPON_CROSS:
		case WEAPON_TORCH:
			return 1;
		default:
			return 0;
		}
}

void setvehicles(void)
{
	int p, l;
	if(activesquad==NULL)return;

	int page=0;

	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(0,0);
		addstr("Choosing the Right Liberal Vehicle");

		printparty();

		int x=1,y=10;
		char str[200],str2[200];

		for(l=page*18;l<vehicle.size()&&l<page*18+18;l++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			for(p=0;p<6;p++)
				{
				if(activesquad->squad[p]==NULL)continue;
				if(activesquad->squad[p]->alive&&
					activesquad->squad[p]->pref_carid==vehicle[l]->id)
					{
					set_color(COLOR_GREEN,COLOR_BLACK,1);
					break;
					}
				}
			for(p=0;p<pool.size();p++)
				{
				if(pool[p]->squadid!=-1&&pool[p]->alive&&
					pool[p]->pref_carid==vehicle[l]->id&&pool[p]->squadid!=activesquad->id)
					{
					set_color(COLOR_YELLOW,COLOR_BLACK,1);
					break;
					}
				}

			getcarfull(str2,*vehicle[l],1);
			str[0]=l-page*18+'A';
			str[1]='\x0';
			strcat(str," - ");
			strcat(str,str2);

			move(y,x);
			addstr(str);

			x+=26;
			if(x>53)
				{
				x=1;
				y++;
				}
			}

		//PAGE UP
		if(page>0)
			{
			move(17,1);
			if(interface_pgup=='[')
				{
				addstr("[ - Previous");
				}
			else
				{
				addstr("PGUP - Previous");
				}
			}
		//PAGE DOWN
		if((page+1)*18<vehicle.size())
			{
			move(17,53);
			if(interface_pgup=='[')
				{
				addstr("] - Next");
				}
			else
				{
				addstr("PGDN - Next");
				}
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(19,1);
		addstr("Press a letter to specify passengers for that Liberal vehicle");
		move(20,1);
		addstr("Capitalize the letter to designate a driver.");
		move(21,1);
		addstr("Note:  Vehicles in yellow have already been selected by another squad");
		move(22,1);
		addstr("       These cars may be used by both squads but not on the same day.");

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(24,1);
		addstr("X - Done");

		refresh();

		int c=getch();
		translategetch_cap(c);

		if(c>='A'&&c<='R')
			{
			int slot=c-'A'+page*18;

			if(slot>=0&&slot<vehicle.size())
				{
				move(8,20);
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				addstr("Choose a Liberal squad member drive it.");

				refresh();

				int c=getch();
				translategetch(c);

				if(c>='1'&&c<='6')
					{
					if(activesquad->squad[c-'1']!=NULL)
						{
						activesquad->squad[c-'1']->pref_carid=vehicle[slot]->id;
						if(activesquad->squad[c-'1']->canwalk())
							{
							activesquad->squad[c-'1']->pref_is_driver=1;
							}
						else activesquad->squad[c-'1']->pref_is_driver=0;
						}
					}
				}
			}

		if(c>='a'&&c<='r')
			{
			int slot=c-'a'+page*18;

			if(slot>=0&&slot<vehicle.size())
				{
				move(8,20);
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				addstr("Choose a Liberal squad member be a passenger.");

				refresh();

				int c=getch();
				translategetch(c);

				if(c>='1'&&c<='6')
					{
					if(activesquad->squad[c-'1']!=NULL)
						{
						activesquad->squad[c-'1']->pref_carid=vehicle[slot]->id;
						activesquad->squad[c-'1']->pref_is_driver=0;
						}
					}
				}
			}

		if(c=='x')return;

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*18<vehicle.size())page++;

		}while(1);
}

long id_getcar(long id)
{
	for(long v=0;v<vehicle.size();v++)
		{
		if(vehicle[v]->id==id)return v;
		}
	return -1;
}

void squadgrab_immobile(char dead)
{
	int p;
	//DRAGGING PEOPLE OUT IF POSSIBLE
	int hostslots=0;
	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if(activesquad->squad[p]->alive&&
				(activesquad->squad[p]->canwalk()||
				(activesquad->squad[p]->flag & CREATUREFLAG_WHEELCHAIR))&&
				activesquad->squad[p]->prisoner==NULL)
				{
				hostslots++;
				}
			else if((!activesquad->squad[p]->alive||
				(!activesquad->squad[p]->canwalk()&&
				!(activesquad->squad[p]->flag & CREATUREFLAG_WHEELCHAIR)))&&
				activesquad->squad[p]->prisoner!=NULL)
				{
				clearmessagearea();
				set_color(COLOR_YELLOW,COLOR_BLACK,1);
				move(16,1);
				addstr(activesquad->squad[p]->name);
				addstr(" can no longer handle ");
				addstr(activesquad->squad[p]->prisoner->name);
				addstr(".");
				refresh();
				getch();

				freehostage(*activesquad->squad[p]->prisoner,1);
				}
			}
		}

	for(p=5;p>=0;p--)
		{
		if(activesquad->squad[p]!=NULL)
			{
			if((!activesquad->squad[p]->alive&&dead)||
				(activesquad->squad[p]->alive&&
				!(activesquad->squad[p]->flag & CREATUREFLAG_WHEELCHAIR)&&
				!activesquad->squad[p]->canwalk()&&!dead))
				{
				if(hostslots==0)
					{
					if(!activesquad->squad[p]->alive)
						{
						clearmessagearea();
						set_color(COLOR_YELLOW,COLOR_BLACK,1);
						move(16,1);
						addstr("Nobody can carry Martyr ");
						addstr(activesquad->squad[p]->name);
						addstr(".");

						//DROP LOOT
						makeloot(*activesquad->squad[p],groundloot);

						//MUST DELETE PARTY MEMBER FROM POOL COMPLETELY
						for(int pl=0;pl<pool.size();pl++)
							{
							if(pool[pl]==activesquad->squad[p])
								{
								delete pool[pl];
								pool.erase(pool.begin() + pl);
								break;
								}
							}
						}
					else
						{
						clearmessagearea();
						set_color(COLOR_YELLOW,COLOR_BLACK,1);
						move(16,1);
						addstr(activesquad->squad[p]->name);
						addstr(" is left to be captured.");

						capturecreature(*activesquad->squad[p]);
						}
					}
				else
					{
					for(int p2=0;p2<6;p2++)
						{
						if(p2==p)continue;
						if(activesquad->squad[p2]!=NULL)
							{
							if(activesquad->squad[p2]->alive&&
								(activesquad->squad[p2]->canwalk()||
								(activesquad->squad[p2]->flag & CREATUREFLAG_WHEELCHAIR))&&
								activesquad->squad[p2]->prisoner==NULL)
								{
								activesquad->squad[p2]->prisoner=activesquad->squad[p];

								clearmessagearea();
								set_color(COLOR_YELLOW,COLOR_BLACK,1);
								move(16,1);
								addstr(activesquad->squad[p2]->name);
								addstr(" hauls ");
								addstr(activesquad->squad[p]->name);
								addstr(".");
								break;
								}
							}
						}

					hostslots--;
					}

				//SHUFFLE SQUAD
				char flipstart=0;
				for(int pt=0;pt<6;pt++)
					{
					if(pt==p-1)continue;
					if(pt==p){flipstart=1;}
					if(flipstart&&pt<5)activesquad->squad[pt]=activesquad->squad[pt+1];
					}
				if(flipstart)activesquad->squad[5]=NULL;

				printparty();
				refresh();
				getch();
				}
			}
		}
}

void freehostage(creaturest &cr,char situation)
{
	if(cr.prisoner==NULL)return;

	if(cr.prisoner->alive)
		{
		if(situation==0)
			{
			if(cr.prisoner->squadid==-1)addstr(" and a hostage is freed");
			else
				{
				addstr(" and ");
				addstr(cr.prisoner->name);
				if(cr.prisoner->flag & CREATUREFLAG_JUSTESCAPED)addstr(" is recaptured");
				else addstr(" is captured");
				}
			}
		else if(situation==1)
			{
			clearmessagearea();
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(16,1);
			if(cr.prisoner->squadid==-1)addstr("A hostage escapes!");
			else
				{
				addstr(cr.prisoner->name);
				if(cr.prisoner->flag & CREATUREFLAG_JUSTESCAPED)addstr(" is recaptured.");
				else addstr(" is captured.");
				}
			}

		if(cr.prisoner->squadid==-1)
			{
			for(int e=0;e<ENCMAX;e++)
				{
				if(encounter[e].exists==0)
					{
					encounter[e]=*cr.prisoner;
					encounter[e].exists=1;
					encounter[e].align=-1;
					break;
					}
				}
			delete cr.prisoner;
			}
		else capturecreature(*cr.prisoner);
		}
	else
		{
		if(cr.prisoner->squadid!=-1)
			{
			//MUST DELETE PARTY MEMBER FROM POOL COMPLETELY
			for(int pl=0;pl<pool.size();pl++)
				{
				if(pool[pl]==cr.prisoner)
					{
					removesquadinfo(*pool[pl]);
					delete pool[pl];
					pool.erase(pool.begin() + pl);
					break;
					}
				}
			}
		}

	cr.prisoner=NULL;

	if(situation==1)
		{
		printparty();
		if(mode==GAMEMODE_CHASECAR||
						mode==GAMEMODE_CHASEFOOT)printchaseencounter();
		else printencounter();
		refresh();
		getch();
		}
}

void capturecreature(creaturest &t)
{
	if(t.flag & CREATUREFLAG_JUSTESCAPED)
		{
		t.location=cursite;
		if(sitetype==SITE_GOVERNMENT_PRISON||
			sitetype==SITE_GOVERNMENT_COURTHOUSE)
			{
			t.armor.type=ARMOR_PRISONER;
			}
		if(sitetype==SITE_GOVERNMENT_PRISON)
			{
			t.lawflag=0;
			t.lawflag2=0;
			}
		}
	else
		{
		for(int l=0;l<location.size();l++)
			{
			if(location[l]->type==SITE_GOVERNMENT_POLICESTATION)
				{
				t.location=l;
				}
			}
		}

	t.squadid=-1;
}

char chasesequence(void)
{
	int p = 0;

	reloadparty();

	//NOTE: THIS FUNCTION RETURNS 1 IF ANYBODY ESCAPES
		//IT CAN DELETE CREATURES AND VEHICLES, BUT SHOULD NOT DELETE SQUADS
			//AS SQUADS MAY BE FICTITIOUS AND WILL BE DELETED LATER ANYWAY

	//BAIL IF NO CHASERS
	int chasenum=0;
	int v2=0;
	for(int e=0;e<ENCMAX;e++)
		{
		if(encounter[e].exists)chasenum++;
		}
	if(chasenum==0)return 1;

	chaseseq.friendcar.clear();
	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]==NULL)continue;

		if(activesquad->squad[p]->carid!=-1)
			{
			long v=id_getcar(activesquad->squad[p]->carid);
			if(v!=-1)
				{
				for(v2=0;v2<chaseseq.friendcar.size();v2++)
					{
					if(chaseseq.friendcar[v2]->id==vehicle[v]->id)break;
					}
				if(v2==chaseseq.friendcar.size())chaseseq.friendcar.push_back(vehicle[v]);
				}
			}
		}

	mode=GAMEMODE_CHASECAR;

	erase();
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(0,0);
	addstr("As you pull away from the site, you notice that you are");
	move(1,0);
	addstr("being followed by Conservative swine!");
	refresh();
	getch();

	if(location[chaseseq.location]->parent!=-1)
		{
		chaseseq.location=location[chaseseq.location]->parent;
		}

	short obstacle=-1;

	do
		{
		int partysize=0;
		int partyalive=0;
		for(p=0;p<6;p++)
			{
			if(activesquad->squad[p]!=NULL)partysize++;
			else continue;

			if(activesquad->squad[p]->alive==1)partyalive++;
			}
		int encsize=0;
		for(int e=0;e<ENCMAX;e++)
			{
			if(encounter[e].exists)
				{
				encsize++;
				}
			}

		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addlocationname(location[chaseseq.location]);

		//PRINT PARTY
		if(partyalive==0)party_status=-1;
		printparty();

		if(partyalive>0)
			{
			//PRINT DRIVING SITUATION AND INSTRUCTIONS
			if(partysize>1)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(9,40);
			addstr("O - Change the squad's Liberal order");
			if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,40);
			addstr("# - Check the status of a squad Liberal");
			if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,40);
			addstr("0 - Show the squad's Liberal status");

			int y=12;
			if(obstacle==-1)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,0);
				move(9,1);
				addstr("D - Try to lose them!");
				move(10,1);
				addstr("E - Equip");
				move(11,1);
				addstr("F - Fight!");
				}
			else
				{
				switch(obstacle)
					{
					case CARCHASE_OBSTACLE_FRUITSTAND:
						set_color(COLOR_MAGENTA,COLOR_BLACK,1);
						move(9,1);
						//LIMIT :------------------------------------:
						addstr("You are speeding toward a fruit-stand!");
						move(10,1);
						addstr("D - Avoid it!");
						move(11,1);
						addstr("F - Drive on through!");
						break;
					}
				}
			move(y,1);
			addstr("B - Bail out and run!");
			if(chaseseq.canpullover)
				{
				move(y+1,1);
				addstr("P - Pull over");
				}
			}
		else
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(9,1);
			addstr("C - Reflect on your lack of skill.");
			}

		//PRINT ENEMIES
		printchaseencounter();

		refresh();

		int c=getch();
		translategetch(c);

		if(partyalive==0&&c=='c')
			{
			//DESTROY ALL CARS BROUGHT ALONG WITH PARTY
			for(p=0;p<6;p++)
				{
				if(activesquad->squad[p]==NULL)continue;
				if(activesquad->squad[p]->carid!=-1)
					{
					long v=id_getcar(activesquad->squad[p]->carid);
					if(v!=-1)
						{
						removecarprefs_pool(vehicle[v]->id);
						delete vehicle[v];
						vehicle.erase(vehicle.begin() + v);
						}
					}
				}

			for(p=0;p<6;p++)
				{
				if(activesquad->squad[p]==NULL)continue;

				for(int pl=pool.size()-1;pl>=0;pl--)
					{
					if(pool[pl]==activesquad->squad[p])
						{
						delete pool[pl];
						pool.erase(pool.begin() + pl);
						break;
						}
					}

				activesquad->squad[p]=NULL;
				}

			if(!endcheck())
				{
				mode=GAMEMODE_BASE;
				return 0;
				}
			}

		if(partyalive>0)
			{
			if(c=='o'&&partysize>1)orderparty();

			if(c=='0')party_status=-1;

			if(c>='1'&&c<='6')
				{
				if(activesquad->squad[c-'1']!=NULL)
					{
					if(party_status==c-'1')fullstatus(party_status);
					else party_status=c-'1';
					}
				}

			if(c=='b')
				{
				for(int v=0;v<chaseseq.friendcar.size();v++)
					{
					for(int v2=(int)vehicle.size()-1;v2>=0;v2--)
						{
						if(vehicle[v2]==chaseseq.friendcar[v])
							{
							removecarprefs_pool(vehicle[v2]->id);
							delete vehicle[v2];
							vehicle.erase(vehicle.begin() + v2);
							}
						}
					}
				chaseseq.friendcar.clear();
				for(int p=0;p<6;p++)
					{
					if(activesquad->squad[p]==NULL)continue;
					activesquad->squad[p]->carid=-1;
					}

				return footchase();
				}
			else if(c=='p')
				{
				if(chaseseq.canpullover)
					{
					chase_giveup();
					return 0;
					}
				}
			else if(obstacle==-1)
				{
				if(c=='d')
					{
					if(encounter[0].exists&&
						encounter[0].type==CREATURE_COP)
						{
						sitestory->crime.push_back(CRIME_CARCHASE);
						criminalizeparty(LAWFLAG_RESIST);
						}
					evasivedrive();
					enemyattack();
					creatureadvance();
					drivingupdate(obstacle);
					}

				if(c=='f')
					{
					if(encounter[0].exists&&
						encounter[0].type==CREATURE_COP)
						{
						sitestory->crime.push_back(CRIME_CARCHASE);
						criminalizeparty(LAWFLAG_RESIST);
						}
					youattack();
					enemyattack();
					creatureadvance();
					drivingupdate(obstacle);
					chaseseq.canpullover=0;
					}

				if(c=='e')
					{
					equip(activesquad->loot,-1);
					}
				}
			else
				{
				switch(obstacle)
					{
					case CARCHASE_OBSTACLE_FRUITSTAND:
						if(c=='d')
							{
							obstacledrive(obstacle,0);
							creatureadvance();
							drivingupdate(obstacle);
							}
						if(c=='f')
							{
							obstacledrive(obstacle,1);
							creatureadvance();
							drivingupdate(obstacle);
							}
						break;
					}
				}

			//HAVE YOU LOST ALL OF THEM?
				//THEN LEAVE
			partysize=0;
			partyalive=0;
			for(int p=0;p<6;p++)
				{
				if(activesquad->squad[p]!=NULL)partysize++;
				else continue;

				if(activesquad->squad[p]->alive==1)partyalive++;
				}
			int baddiecount=0;
			for(int e=0;e<ENCMAX;e++)
				{
				if(encounter[e].carid!=-1&&
					encounter[e].align==-1&&
					encounter[e].alive&&
					encounter[e].exists)baddiecount++;
				}
			if(partyalive>0&&baddiecount==0)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				clearmessagearea();
				move(16,1);
				addstr("It looks like you've lost them!");
				refresh();
				getch();
				for(int p=0;p<pool.size();p++)
					{
					for(int w=0;w<BODYPARTNUM;w++)
						{
						pool[p]->wound[w]&=~WOUND_BLEEDING;
						}
					}
				mode=GAMEMODE_BASE;
				return 1;
				}
			}
		
		}while(1);

	mode=GAMEMODE_BASE;
	return 1;
}

char footchase(void)
{
	//NOTE: THIS FUNCTION RETURNS 1 IF ANYBODY ESCAPES
		//IT CAN DELETE CREATURES, BUT SHOULD NOT DELETE SQUADS
			//AS SQUADS MAY BE FICTITIOUS AND WILL BE DELETED LATER ANYWAY
    int p;
	reloadparty();

	//NUKE ALL CARS
	for(int v=0;v<chaseseq.enemycar.size();v++)
		{
		delete chaseseq.enemycar[v];
		}
	chaseseq.enemycar.clear();

	//BAIL IF NO CHASERS
	int chasenum=0;
	for(int e=0;e<ENCMAX;e++)
		{
		if(encounter[e].exists)chasenum++;
		encounter[e].carid=-1;
		}
	if(chasenum==0)return 1;

	mode=GAMEMODE_CHASEFOOT;

	do
		{
		int partysize=0;
		int partyalive=0;
		for(p=0;p<6;p++)
			{
			if(activesquad->squad[p]!=NULL)partysize++;
			else continue;

			if(activesquad->squad[p]->alive==1)partyalive++;
			}
		int encsize=0;
		for(int e=0;e<ENCMAX;e++)
			{
			if(encounter[e].exists)
				{
				encsize++;
				}
			}

		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addlocationname(location[chaseseq.location]);

		//PRINT PARTY
		if(partyalive==0)party_status=-1;
		printparty();

		if(partyalive>0)
			{
			//PRINT DRIVING SITUATION AND INSTRUCTIONS
			if(partysize>1)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(9,40);
			addstr("O - Change the squad's Liberal order");
			if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,40);
			addstr("# - Check the status of a squad Liberal");
			if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,40);
			addstr("0 - Show the squad's Liberal status");

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(9,1);
			addstr("D - Try to lose them!");
			move(10,1);
			addstr("E - Equip");
			move(11,1);
			addstr("F - Fight!");
			if(chaseseq.canpullover)
				{
				move(12,1);
				addstr("G - Give Up");
				}
			}
		else
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(9,1);
			addstr("C - Reflect on your lack of skill.");
			}

		//PRINT ENEMIES
		printchaseencounter();

		refresh();

		int c=getch();
		translategetch(c);

		if(partyalive==0&&c=='c')
			{
			//DESTROY ALL CARS BROUGHT ALONG WITH PARTY
			for(p=0;p<6;p++)
				{
				if(activesquad->squad[p]==NULL)continue;
				if(activesquad->squad[p]->carid!=-1)
					{
					long v=id_getcar(activesquad->squad[p]->carid);
					if(v!=-1)
						{
						removecarprefs_pool(vehicle[v]->id);
						delete vehicle[v];
						vehicle.erase(vehicle.begin() + v);
						}
					}
				}

			for(p=0;p<6;p++)
				{
				if(activesquad->squad[p]==NULL)continue;

				for(int pl=pool.size()-1;pl>=0;pl--)
					{
					if(pool[pl]==activesquad->squad[p])
						{
						delete pool[pl];
						pool.erase(pool.begin() + pl);
						break;
						}
					}

				activesquad->squad[p]=NULL;
				}

			if(!endcheck())
				{
				mode=GAMEMODE_BASE;
				return 0;
				}
			}

		if(partyalive>0)
			{
			if(c=='o'&&partysize>1)orderparty();

			if(c=='0')party_status=-1;

			if(c>='1'&&c<='6')
				{
				if(activesquad->squad[c-'1']!=NULL)
					{
					if(party_status==c-'1')fullstatus(party_status);
					else party_status=c-'1';
					}
				}

			if(c=='g')
				{
				if(chaseseq.canpullover)
					{
					chase_giveup();
					return 0;
					}
				}
			if(c=='d')
				{
				if(encounter[0].exists&&
					encounter[0].type==CREATURE_COP)
					{
					sitestory->crime.push_back(CRIME_FOOTCHASE);
					}
				criminalizeparty(LAWFLAG_RESIST);
				evasiverun();
				enemyattack();
				creatureadvance();
				}

			if(c=='f')
				{
				if(encounter[0].exists&&
					encounter[0].type==CREATURE_COP)
					{
					sitestory->crime.push_back(CRIME_FOOTCHASE);
					}
				criminalizeparty(LAWFLAG_RESIST);
				youattack();
				enemyattack();
				creatureadvance();
				chaseseq.canpullover=0;
				}

			if(c=='e')
				{
				equip(activesquad->loot,-1);
				}

			//HAVE YOU LOST ALL OF THEM?
				//THEN LEAVE
			partysize=0;
			partyalive=0;
			for(int p=0;p<6;p++)
				{
				if(activesquad->squad[p]!=NULL)partysize++;
				else continue;

				if(activesquad->squad[p]->alive==1)partyalive++;
				}
			int baddiecount=0;
			for(int e=0;e<ENCMAX;e++)
				{
				if(encounter[e].align==-1&&
					encounter[e].alive&&
					encounter[e].exists)baddiecount++;
				}
			if(partyalive>0&&baddiecount==0)
				{
				set_color(COLOR_WHITE,COLOR_BLACK,1);
				clearmessagearea();
				move(16,1);
				addstr("It looks like you've lost them!");
				refresh();
				getch();
				for(int p=0;p<pool.size();p++)
					{
					for(int w=0;w<BODYPARTNUM;w++)
						{
						pool[p]->wound[w]&=~WOUND_BLEEDING;
						}
					}
				mode=GAMEMODE_BASE;
				return 1;
				}
			}
		
		}while(1);

	mode=GAMEMODE_BASE;
	return 1;
}

char chasesequence(creaturest &cr,vehiclest &v)
{
	long oldsqid=cr.squadid;
	squadst *sq=new squadst;
		sq->squad[0]=&cr;
		sq->id=cursquadid;cursquadid++;
		cr.squadid=sq->id;
		cr.carid=v.id;
		cr.is_driver=1;

	squadst *oact=activesquad;
	long ops=party_status;
	activesquad=sq;
	party_status=0;
	char ret=chasesequence();
	activesquad=oact;
	party_status=ops;

	delete sq;

	if(ret)
		{
		cr.squadid=oldsqid;
		cr.carid=-1;
		}
	return ret;
}

char footchase(creaturest &cr)
{
	long oldsqid=cr.squadid;
	squadst *sq=new squadst;
		sq->squad[0]=&cr;
		sq->squad[0]->id=cursquadid;cursquadid++;
		cr.squadid=sq->squad[0]->id;
		cr.carid=-1;

	squadst *oact=activesquad;
	long ops=party_status;
	activesquad=sq;
	party_status=0;
	char ret=footchase();
	activesquad=oact;
	party_status=ops;

	delete sq;

	if(ret)
		{
		cr.squadid=oldsqid;
		}
	return ret;
}

char endcheck(short cause)
{
	char dead=1;
	for(int p=0;p<pool.size();p++)
		{
		if(pool[p]->alive&&
			pool[p]->align==1&&
			!(pool[p]->flag & CREATUREFLAG_SLEEPER))
			{
			dead=0;
			break;
			}
		}

	if(dead)
		{
		if(cause==-1)
			{
			if(location[cursite]->siege.siege)
				{
				switch(location[cursite]->siege.siegetype)
					{
					case SIEGE_POLICE:savehighscore(END_POLICE);break;
					case SIEGE_CIA:savehighscore(END_CIA);break;
					case SIEGE_HICKS:savehighscore(END_HICKS);break;
					case SIEGE_CORPORATE:savehighscore(END_CORP);break;
					}
				} 
			else savehighscore(END_DEAD);
			}
		else savehighscore(cause);

		reset();
		viewhighscores();
		endwin();
		exit(0);
		}

	return 0;
}

void makechasers(long sitetype,long sitecrime)
{
	for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;

	if(sitecrime==0)return;

	short encslot=0;
	int n;

	long cartype=-1;
	long pnum;

	chaseseq.canpullover=0;

	switch(sitetype)
		{
		case SITE_GOVERNMENT_INTELLIGENCEHQ:
			cartype=VEHICLE_AGENTCAR;
			pnum=LCSrandom(sitecrime/5 + 1)+1;
			if(pnum>6)pnum=6;
			for(n=0;n<pnum;n++)
				{
				makecreature(encounter[encslot],CREATURE_AGENT);
				encslot++;
				}
			break;
		case SITE_CORPORATE_HEADQUARTERS:
		case SITE_CORPORATE_HOUSE:
			if(LCSrandom(2))cartype=VEHICLE_SUV;
			else cartype=VEHICLE_JEEP;
			pnum=LCSrandom(sitecrime/5 + 1)+1;
			if(pnum>6)pnum=6;
			for(n=0;n<pnum;n++)
				{
				makecreature(encounter[encslot],CREATURE_MERC);
				encslot++;
				}
			break;
		case SITE_MEDIA_AMRADIO:
		case SITE_MEDIA_CABLENEWS:
			cartype=VEHICLE_PICKUP;
			pnum=LCSrandom(sitecrime/5 + 1)+1;
			if(pnum>18)pnum=18;
			for(n=0;n<pnum;n++)
				{
				makecreature(encounter[encslot],CREATURE_HICK);
				encslot++;
				}
			break;
		default:
			chaseseq.canpullover=1;
			cartype=VEHICLE_POLICECAR;
			pnum=LCSrandom(sitecrime/5 + 1)+1;
			if(pnum>6)pnum=6;
			for(n=0;n<pnum;n++)
				{
				if(law[LAW_DEATHPENALTY]==-2&&
					law[LAW_POLICEBEHAVIOR]==-2){makecreature(encounter[encslot],CREATURE_DEATHSQUAD);chaseseq.canpullover=0;}
				else if(law[LAW_POLICEBEHAVIOR]<=-1)makecreature(encounter[encslot],CREATURE_GANGUNIT);
				else makecreature(encounter[encslot],CREATURE_COP);
				encslot++;
				}
			break;
		}

	for(n=0;n<pnum;n++)encounter[n].align=-1;

	//ASSIGN CARS TO CREATURES
	long carnum;
	if(pnum<=2)carnum=1;
	else if(pnum<=3)carnum=LCSrandom(2)+1;
	else if(pnum<=5)carnum=LCSrandom(2)+2;
	else if(pnum<=7)carnum=LCSrandom(2)+3;
	else carnum=4;

	for(int c=0;c<carnum;c++)
		{
		vehiclest *v=new vehiclest;
			v->init(cartype);
		chaseseq.enemycar.push_back(v);

		for(n=0;n<pnum;n++)
			{
			if(encounter[n].carid==-1)
				{
				encounter[n].carid=v->id;
				encounter[n].is_driver=1;
				break;
				}
			}
		}

	long load[4]={0,0,0,0};

	for(n=0;n<pnum;n++)
		{
		if(encounter[n].carid==-1)
			{
			int v;
			do
				{
				v=LCSrandom(chaseseq.enemycar.size());
				encounter[n].carid=chaseseq.enemycar[v]->id;
				encounter[n].is_driver=0;
				}while(load[v]>=4);
			load[v]++;
			}
		}
}

void evasivedrive(void)
{
	int e;
	vector<long> yourrolls;
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]==NULL)continue;
		if(activesquad->squad[p]->alive&&
			activesquad->squad[p]->is_driver)
			{
			long v=id_getcar(activesquad->squad[p]->carid);
			yourrolls.push_back(driveskill(*activesquad->squad[p],vehicle[v])+LCSrandom(10));
			}
		}

	vector<long> theirrolls;
	vector<long> theirrolls_id;
	vector<long> theirrolls_drv;
	for(e=0;e<ENCMAX;e++)
		{
		if(encounter[e].carid!=-1&&
			encounter[e].align==-1&&
			encounter[e].alive&&
			encounter[e].exists&&
			encounter[e].is_driver)
			{
			for(int v=0;v<chaseseq.enemycar.size();v++)
				{
				if(chaseseq.enemycar[v]->id==encounter[e].carid)
					{
					theirrolls.push_back(driveskill(encounter[e],chaseseq.enemycar[v])+LCSrandom(10));
					theirrolls_id.push_back(encounter[e].carid);
					theirrolls_drv.push_back(encounter[e].id);
					}
				}
			}
		}

	clearmessagearea();
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(16,1);
	addstr("You swerve around the next corner!");
	refresh();
	getch();

	int cnt;
	for(int i=0;i<theirrolls.size();i++)
		{
		cnt=yourrolls[LCSrandom(yourrolls.size())];
		if(theirrolls[i]<cnt)
			{
			clearmessagearea();
			set_color(COLOR_CYAN,COLOR_BLACK,1);
			move(16,1);
			for(e=0;e<ENCMAX;e++)
				{
				if(encounter[e].id==theirrolls_drv[i])
					{
					addstr(encounter[e].name);
					break;
					}
				}
			addstr(" fails to negotiate the turn!");

			for(e=0;e<ENCMAX;e++)
				{
				if(encounter[e].carid==theirrolls_id[i])
					{
					for(int e2=e;e2<ENCMAX-1;e2++)
						{
						encounter[e2]=encounter[e2+1];
						}
					encounter[ENCMAX-1].exists=0;
					e--;
					}
				}
			for(int v=0;v<chaseseq.enemycar.size();v++)
				{
				if(chaseseq.enemycar[v]->id==theirrolls_id[i])
					{
					delete chaseseq.enemycar[v];
					chaseseq.enemycar.erase(chaseseq.enemycar.begin() + v);
					break;
					}
				}

			printchaseencounter();
			refresh();
			getch();
			}
		else
			{
			clearmessagearea();
			set_color(COLOR_YELLOW,COLOR_BLACK,1);
			move(16,1);
			for(int e=0;e<ENCMAX;e++)
				{
				if(encounter[e].id==theirrolls_drv[i])
					{
					addstr(encounter[e].name);
					break;
					}
				}
			addstr(" is still on your tail!");
			refresh();
			getch();
			}
		}
}

long driveskill(creaturest &cr,vehiclest *v)
{
	long vbonus=0;
	switch(v->type)
		{
		case VEHICLE_PICKUP:
		case VEHICLE_SUV:
		case VEHICLE_TAXICAB:
		case VEHICLE_JEEP:
			vbonus=1;
			break;
		case VEHICLE_POLICECAR:
			vbonus=3;
			break;
		case VEHICLE_SPORTSCAR:
		case VEHICLE_AGENTCAR:
			vbonus=5;
			break;
		}
	return cr.attval(ATTRIBUTE_AGILITY)+cr.skill[SKILL_DRIVING]*3+vbonus;
}

void drivingupdate(short &obstacle)
{
	//CHECK TO SEE WHICH CARS ARE BEING DRIVEN
	vector<int> passenger;
	int driver;
	int v;
	int p;
	for(v=chaseseq.friendcar.size()-1;v>=0;v--)
		{
		passenger.clear();
		driver=-1;
		for(p=0;p<6;p++)
			{
			if(activesquad->squad[p]==NULL)continue;
			if(activesquad->squad[p]->carid==chaseseq.friendcar[v]->id)
				{
				if(activesquad->squad[p]->is_driver)
					{
					if(activesquad->squad[p]->canwalk())driver=p;
					else activesquad->squad[p]->is_driver=0;
					}
				passenger.push_back(p);
				}
			}

		if(passenger.size()>0&&driver==-1)
			{
			//MAKE BEST DRIVING PASSENGER INTO A DRIVER
			vector<int> goodp;
			int max=0;
			for(p=0;p<passenger.size();p++)
				{
				if(driveskill(*activesquad->squad[passenger[p]],chaseseq.friendcar[v])>max&&
					activesquad->squad[passenger[p]]->canwalk())
					{
					max=driveskill(*activesquad->squad[passenger[p]],chaseseq.friendcar[v]);
					}
				}
			for(p=0;p<passenger.size();p++)
				{
				if(driveskill(*activesquad->squad[passenger[p]],chaseseq.friendcar[v])==max&&
					activesquad->squad[passenger[p]]->canwalk())
					{
					goodp.push_back(passenger[p]);
					}
				}

			if(goodp.size()>0)
				{
				int p=goodp[LCSrandom(goodp.size())];
				activesquad->squad[p]->is_driver=1;
				driver=p;

				clearmessagearea();
				set_color(COLOR_YELLOW,COLOR_BLACK,1);
				move(16,1);
				addstr(activesquad->squad[p]->name);
				addstr(" takes over the wheel.");
				printparty();
				refresh();
				getch();
				}
			}
		if(driver==-1)
			{
			crashfriendlycar(v);
			sitestory->crime.push_back(CRIME_CARCHASE);
			}
		}

	for(v=chaseseq.enemycar.size()-1;v>=0;v--)
		{
		passenger.clear();
		driver=-1;
		for(p=0;p<ENCMAX;p++)
			{
			if(!encounter[p].exists)continue;
			if(encounter[p].carid==chaseseq.enemycar[v]->id)
				{
				if(encounter[p].is_driver)
					{
					if(encounter[p].canwalk())driver=p;
					else encounter[p].is_driver=0;
					}
				passenger.push_back(p);
				}
			}

		if(passenger.size()>0&&driver==-1)
			{
			//MAKE BEST DRIVING PASSENGER INTO A DRIVER
			vector<int> goodp;
			int max=0;
			for(p=0;p<passenger.size();p++)
				{
				if(driveskill(encounter[passenger[p]],chaseseq.enemycar[v])>max&&
					encounter[passenger[p]].canwalk())
					{
					max=driveskill(encounter[passenger[p]],chaseseq.enemycar[v]);
					}
				}
			for(p=0;p<passenger.size();p++)
				{
				if(driveskill(encounter[passenger[p]],chaseseq.enemycar[v])==max&&
					encounter[passenger[p]].canwalk())
					{
					goodp.push_back(passenger[p]);
					}
				}

			if(goodp.size()>0)
				{
				int p=goodp[LCSrandom(goodp.size())];
				encounter[p].is_driver=1;
				driver=p;

				clearmessagearea();
				set_color(COLOR_YELLOW,COLOR_BLACK,1);
				move(16,1);
				addstr(encounter[p].name);
				addstr(" takes over the wheel.");
				printchaseencounter();
				refresh();
				getch();
				}
			}
		if(driver==-1)
			{
			crashenemycar(v);
			sitestory->crime.push_back(CRIME_CARCHASE);
			}
		}

	//SET UP NEXT OBSTACLE
	if(!LCSrandom(3))
		{
		obstacle=LCSrandom(CARCHASE_OBSTACLENUM);
		}
	else obstacle=-1;
}

void obstacledrive(short obstacle,char choice)
{
	switch(obstacle)
		{
		case CARCHASE_OBSTACLE_FRUITSTAND:
			if(choice==0)
				{
				dodgedrive();
				}
			else if(choice==1)
				{
				clearmessagearea();
				set_color(COLOR_YELLOW,COLOR_BLACK,1);
				move(16,1);
				addstr("You plow through the fruit stand!");
				refresh();
				getch();

				if(!LCSrandom(5))
					{
					set_color(COLOR_RED,COLOR_BLACK,1);
					move(17,1);
					addstr("The fruit-seller has been squashed!");
					refresh();
					getch();

					criminalizeparty(LAWFLAG_MURDERER);
					}
				}
			break;
		}
}

void dodgedrive(void)
{
	int v;
	clearmessagearea();
	set_color(COLOR_YELLOW,COLOR_BLACK,1);
	move(16,1);
	addstr("You swerve to avoid the obstacle!");
	refresh();
	getch();

	int driver;
	for(v=chaseseq.friendcar.size()-1;v>=0;v--)
		{
		driver=-1;
		for(int p=0;p<6;p++)
			{
			if(activesquad->squad[p]==NULL)continue;
			if(activesquad->squad[p]->carid==chaseseq.friendcar[v]->id)
				{
				if(activesquad->squad[p]->is_driver)
					{
					driver=p;
					break;
					}
				}
			}

		if(driver!=-1)
			{
			if(LCSrandom(11)>driveskill(*activesquad->squad[driver],chaseseq.friendcar[v]))
				{
				crashfriendlycar(v);
				sitestory->crime.push_back(CRIME_CARCHASE);
				}
			}
		}

	for(v=chaseseq.enemycar.size()-1;v>=0;v--)
		{
		driver=-1;
		for(int p=0;p<ENCMAX;p++)
			{
			if(!encounter[p].exists)continue;
			if(encounter[p].carid==chaseseq.enemycar[v]->id)
				{
				if(encounter[p].is_driver)
					{
					driver=p;
					break;
					}
				}
			}

		if(driver!=-1)
			{
			if(LCSrandom(11)>driveskill(encounter[driver],chaseseq.enemycar[v]))
				{
				crashenemycar(v);
				sitestory->crime.push_back(CRIME_CARCHASE);
				}
			}
		}
}

void crashfriendlycar(int v)
{
	char str[80];
	getcarfull(str,*chaseseq.friendcar[v]);

	int victimsum=0;

	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]==NULL)continue;
		if(activesquad->squad[p]->carid==chaseseq.friendcar[v]->id)
			{
			victimsum++;
			if(activesquad->squad[p]->alive&&
				activesquad->squad[p]->align==1)stat_dead++;
			activesquad->squad[p]->alive=0;
			if(activesquad->squad[p]->prisoner!=NULL)
				{
				victimsum++;
				activesquad->squad[p]->prisoner->alive=0;
				if(activesquad->squad[p]->prisoner->squadid!=-1)
					{
					if(activesquad->squad[p]->prisoner->alive&&
						activesquad->squad[p]->prisoner->align==1)stat_dead++;
					for(int pl=pool.size()-1;pl>=0;pl--)
						{
						if(pool[pl]==activesquad->squad[p]->prisoner)
							{
							delete pool[pl];
							pool.erase(pool.begin() + pl);
							break;
							}
						}
					}
				else delete activesquad->squad[p]->prisoner;
				activesquad->squad[p]->prisoner=NULL;
				}
			for(int pl=pool.size()-1;pl>=0;pl--)
				{
				if(pool[pl]==activesquad->squad[p])
					{
					delete pool[pl];
					pool.erase(pool.begin() + pl);
					break;
					}
				}
			activesquad->squad[p]=NULL;
			}
		}

	//CONSOLIDATE SQUAD
	for(int i=0;i<5;i++)
		{
		char flipstart=0;
		for(int pt=0;pt<6;pt++)
			{
			if(activesquad->squad[pt]==NULL&&!flipstart)flipstart=1;
			if(flipstart&&pt<5)activesquad->squad[pt]=activesquad->squad[pt+1];
			}
		if(flipstart)activesquad->squad[5]=NULL;
		}

	//GET RID OF CAR
	for(int v2=(int)vehicle.size()-1;v2>=0;v2--)
		{
		if(vehicle[v2]==chaseseq.friendcar[v])
			{
			removecarprefs_pool(vehicle[v2]->id);
			delete vehicle[v2];
			vehicle.erase(vehicle.begin() + v2);
			break;
			}
		}

	chaseseq.friendcar.erase(chaseseq.friendcar.begin() + v);

	//CRASH CAR
	clearmessagearea();
	set_color(COLOR_MAGENTA,COLOR_BLACK,1);
	move(16,1);
	addstr("Your ");
	addstr(str);
	switch(LCSrandom(3))
		{
		case 0:addstr(" slams into a building.");break;
		case 1:
			addstr(" spins out and crashes.");
			move(17,1);
			if(victimsum>1)addstr("Everyone inside is peeled off against the pavement.");
			else if(victimsum==1)addstr("The person inside is squashed into a cube.");
			break;
		case 2:addstr(" hits a parked car and flips over.");break;
		}
	printparty();
	refresh();
	getch();
}

void crashenemycar(int v)
{
	char str[80];
	getcarfull(str,*chaseseq.enemycar[v]);

	int victimsum=0;

	for(int p=ENCMAX-1;p>=0;p--)
		{
		if(!encounter[p].exists)continue;
		if(encounter[p].carid==chaseseq.enemycar[v]->id)
			{
			victimsum++;
			delenc(p,0);
			}
		}

	delete chaseseq.enemycar[v];
	chaseseq.enemycar.erase(chaseseq.enemycar.begin() + v);

	//CRASH CAR
	clearmessagearea();
	set_color(COLOR_CYAN,COLOR_BLACK,1);
	move(16,1);
	addstr("The ");
	addstr(str);
	switch(LCSrandom(3))
		{
		case 0:addstr(" slams into a building.");break;
		case 1:
			addstr(" spins out and crashes.");
			move(17,1);
			if(victimsum>1)addstr("Everyone inside is peeled off against the pavement.");
			else if(victimsum==1)addstr("The person inside is squashed into a cube.");
			break;
		case 2:addstr(" hits a parked car and flips over.");break;
		}
	printchaseencounter();
	refresh();
	getch();
}

void chase_giveup(void)
{
	int p;
	long ps=-1;
	for(long l=0;l<location.size();l++)
		{
		if(location[l]->type==SITE_GOVERNMENT_POLICESTATION)
			{
			ps=l;
			}
		}

	for(int v=0;v<chaseseq.friendcar.size();v++)
		{
		for(int v2=(int)vehicle.size()-1;v2>=0;v2--)
			{
			if(vehicle[v2]==chaseseq.friendcar[v])
				{
				removecarprefs_pool(vehicle[v2]->id);
				delete vehicle[v2];
				vehicle.erase(vehicle.begin() + v2);
				}
			}
		}
	chaseseq.friendcar.clear();
	for(p=0;p<6;p++)
		{
		if(activesquad->squad[p]==NULL)continue;
		activesquad->squad[p]->squadid=-1;
		activesquad->squad[p]->carid=-1;
		activesquad->squad[p]->location=ps;
		activesquad->squad[p]->weapon.type=WEAPON_NONE;
		activesquad->squad[p]->weapon.ammo=0;
		activesquad->squad[p]->activity.type=ACTIVITY_NONE;
		activesquad->squad[p]=NULL;
		}
	for(p=0;p<pool.size();p++)
		{
		for(int w=0;w<BODYPARTNUM;w++)
			{
			pool[p]->wound[w]&=~WOUND_BLEEDING;
			}
		}

	clearmessagearea();
	set_color(COLOR_MAGENTA,COLOR_BLACK,1);
	move(16,1);
	if(mode!=GAMEMODE_CHASECAR)addstr("You stop and are arrested.");
	else addstr("You pull over and are arrested.");
	refresh();
	getch();
}

void evasiverun(void)
{
	vector<long> yourrolls;
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]==NULL)continue;
		if(activesquad->squad[p]->alive)
			{
			yourrolls.push_back(activesquad->squad[p]->attval(ATTRIBUTE_AGILITY)+
				activesquad->squad[p]->attval(ATTRIBUTE_HEALTH)+LCSrandom(10));
			}
		}

	clearmessagearea();
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(16,1);
	addstr("You suddenly dart into an alley!");
	refresh();
	getch();

	int cnt;
	for(int e=0;e<ENCMAX;e++)
		{
		if(!encounter[e].exists)continue;
		cnt=yourrolls[LCSrandom(yourrolls.size())];
		if(encounter[e].attval(ATTRIBUTE_AGILITY)+
			encounter[e].attval(ATTRIBUTE_HEALTH)+LCSrandom(10)<cnt)
			{
			clearmessagearea();
			set_color(COLOR_CYAN,COLOR_BLACK,1);
			move(16,1);
			addstr(encounter[e].name);
			addstr(" can't keep up!");

			delenc(e,0);
			e--;

			printchaseencounter();
			refresh();
			getch();
			}
		else
			{
			clearmessagearea();
			set_color(COLOR_YELLOW,COLOR_BLACK,1);
			move(16,1);
			addstr(encounter[e].name);
			addstr(" is still on your tail!");
			refresh();
			getch();
			}
		}
}

void gettitle(char *str,creaturest &cr)
{
	if(cr.align!=1)
		{
		strcpy(str,"Hostage");
		}
	else
		{
		if(cr.juice<=-50)
			{
			if(law[LAW_FREESPEECH]==-2)strcpy(str,"[Darn] Worthless");
			else strcpy(str,"Damn Worthless");
			}
		else if(cr.juice<=-10)strcpy(str,"Society's Dregs");
		else if(cr.juice<0)strcpy(str,"Punk");
		else if(cr.juice<10)strcpy(str,"Civilian");
		else if(cr.juice<50)strcpy(str,"Activist");
		else if(cr.juice<100)strcpy(str,"Socialist Threat");
		else if(cr.juice<200)strcpy(str,"Revolutionary");
		else if(cr.juice<500)strcpy(str,"Urban Commando");
		else strcpy(str,"Elite Liberal");
		}
}

void juiceparty(long juice)
{
	if(activesquad!=NULL)
		{
		for(int p=0;p<6;p++)
			{
			if(activesquad->squad[p]!=NULL)
				{
				if(activesquad->squad[p]->alive)
					{
					addjuice(*activesquad->squad[p],juice);
					}
				}
			}
		}
}

void addjuice(creaturest &cr,long juice,long cap)
{
	if(cr.juice>=cap)return;

	if(cr.juice+juice>cap)cr.juice=cap;
	else cr.juice+=juice;
	if(cr.juice>1000)cr.juice=1000;
}

void resolvesite(void)
{
	if(sitealienate)sitestory->positive=0;
	if(sitealarm==1&&location[cursite]->renting==-1)
		{
		location[cursite]->closed=LCSrandom(3)+2;
		}
}

void advancelocations(void)
{
	//ADVANCE LOCATIONS
	for(int l=0;l<location.size();l++)
		{
		if(location[l]->closed>0)
			{
			location[l]->closed--;
			if(location[l]->closed==0)
				{
				switch(LCSrandom(2))
					{
					case 0:
						if(securityable(location[l]->type))location[l]->highsecurity=1;
						else initlocation(*location[l]);
						break;
					case 1:
						initlocation(*location[l]);
						break;
					}
				}
			}
		}
}

void initlocation(locationst &loc)
{
	loc.init();

	char str[80];

	switch(loc.type)
		{
		case SITE_GOVERNMENT_POLICESTATION:
			strcpy(loc.name,"Police Station");
			strcpy(loc.shortname,"Police Station");
			break;
		case SITE_GOVERNMENT_COURTHOUSE:
			if(law[LAW_DEATHPENALTY]==-2)
				{
				strcpy(loc.name,"Halls of Ultimate Judgement");
				strcpy(loc.shortname,"Judge Hall");
				}
			else
				{
				strcpy(loc.name,"Court House");
				strcpy(loc.shortname,"Court House");
				}
			break;
		case SITE_GOVERNMENT_PRISON:
			if(law[LAW_DEATHPENALTY]==-2&&
				law[LAW_POLICEBEHAVIOR]==-2)
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
				strcat(loc.name," Re-education Camp");
				strcpy(loc.shortname,"Re-ed Camp");
				}
			else
				{
				lastname(str);
				strcpy(loc.name,str);
				strcat(loc.name," Prison");
				strcpy(loc.shortname,"Prison");
				}
			break;
		case SITE_INDUSTRY_NUCLEAR:
			strcpy(loc.name,"Nuclear Power Plant");
			strcpy(loc.shortname,"NPower Plant");
			break;
		case SITE_GOVERNMENT_INTELLIGENCEHQ:
			if(law[LAW_PRIVACY]==-2&&
				law[LAW_POLICEBEHAVIOR]==-2)
				{
				strcpy(loc.name,"Ministry of Love");
				strcpy(loc.shortname,"Min. Love");
				}
			else
				{
				strcpy(loc.name,"Intelligence HQ");
				strcpy(loc.shortname,"Int. HQ");
				}
			break;
		case SITE_CORPORATE_HEADQUARTERS:
			strcpy(loc.name,"Corporate HQ");
			strcpy(loc.shortname,"Corp. HQ");
			break;
		case SITE_CORPORATE_HOUSE:
			if(law[LAW_CORPORATE]==-2&&
				law[LAW_TAX]==-2)
				{
				strcpy(loc.name,"CEO Castle");
				strcpy(loc.shortname,"CEO Castle");
				}
			else
				{
				strcpy(loc.name,"CEO Residence");
				strcpy(loc.shortname,"CEO House");
				}
			break;
		case SITE_RESIDENTIAL_SHELTER:
			strcpy(loc.name,"Homeless Shelter");
			strcpy(loc.shortname,"Shelter");
			break;
		case SITE_INDUSTRY_WAREHOUSE:
			{
			strcpy(loc.name,"Abandoned ");
			char str[50];
			lastname(str);
			strcat(loc.name,str);
			strcat(loc.name," ");

			switch(LCSrandom(5))
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
				}
			break;
			}
		case SITE_INDUSTRY_POLLUTER:
			switch(LCSrandom(5))
				{
				case 0:
					strcpy(loc.name,"Aluminum Factory");
					strcpy(loc.shortname,"Alum Fact");
					break;
				case 1:
					strcpy(loc.name,"Plastic Factory");
					strcpy(loc.shortname,"Plast Fact");
					break;
				case 2:
					strcpy(loc.name,"Oil Refinery");
					strcpy(loc.shortname,"Refinery");
					break;
				case 3:
					strcpy(loc.name,"Auto Plant");
					strcpy(loc.shortname,"Auto Plant");
					break;
				case 4:
					strcpy(loc.name,"Chemical Factory");
					strcpy(loc.shortname,"Chem Fact");
					break;
				}
			break;
		case SITE_MEDIA_CABLENEWS:
			strcpy(loc.name,"Cable News Station");
			strcpy(loc.shortname,"News Station");
			break;
		case SITE_MEDIA_AMRADIO:
			strcpy(loc.name,"AM Radio Station");
			strcpy(loc.shortname,"Radio Station");
			break;
		case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
			lastname(str);
			strcpy(loc.name,str);
			strcat(loc.name," Apartments");
			strcpy(loc.shortname,str);
			strcat(loc.shortname," Apts");
			break;
		case SITE_RESIDENTIAL_APARTMENT:
			lastname(str);
			strcpy(loc.name,str);
			strcat(loc.name," Apartments");
			strcpy(loc.shortname,str);
			strcat(loc.shortname," Apts");
			break;
		case SITE_RESIDENTIAL_TENEMENT:
			lastname(str);
			strcpy(loc.name,str);
			strcat(loc.name," Apartments");
			strcpy(loc.shortname,str);
			strcat(loc.shortname," Apts");
			break;
		case SITE_HOSPITAL_UNIVERSITY:
			strcpy(loc.name,"The University Hospital");
			strcpy(loc.shortname,"U Hospital");
			break;
		case SITE_HOSPITAL_CLINIC:
			strcpy(loc.name,"The Free CLINIC");
			strcpy(loc.shortname,"CLINIC");
			break;
		case SITE_LABORATORY_GENETIC:
			lastname(str);
			strcpy(loc.name,str);
			strcat(loc.name," Genetics");
			strcpy(loc.shortname,"Genetics Lab");
			break;
		case SITE_LABORATORY_COSMETICS:
			lastname(str);
			strcpy(loc.name,str);
			strcat(loc.name," Cosmetics");
			strcpy(loc.shortname,"Cosmetics Lab");
			break;
		case SITE_BUSINESS_PAWNSHOP:
			lastname(str);
			strcpy(loc.name,str);
			strcat(loc.name," Cash & Loans");
			strcpy(loc.shortname,"Pawnshop");
			break;
		case SITE_BUSINESS_DEPTSTORE:
			lastname(str);
			strcpy(loc.name,str);
			strcat(loc.name,"'s Department Store");
			strcpy(loc.shortname,"Dept. Store");
			break;
		case SITE_BUSINESS_HALLOWEEN:
			lastname(str);
			strcpy(loc.name,"The Oubliette");
			strcpy(loc.shortname,"Oubliette");
			break;
		case SITE_INDUSTRY_SWEATSHOP:
			lastname(str);
			strcpy(loc.name,str);
			strcat(loc.name," Garment Makers");
			strcpy(loc.shortname,"Sweatshop");
			break;
		case SITE_BUSINESS_CRACKHOUSE:
			lastname(str);
			strcpy(loc.name,str);
			strcat(loc.name," St. Crack House");
			strcpy(loc.shortname,"Crack House");
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
			lastname(str);
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
		}
}

char securityable(int type)
{
	switch(type)
		{
		case SITE_BUSINESS_CIGARBAR:
		case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
		case SITE_LABORATORY_COSMETICS:
		case SITE_INDUSTRY_NUCLEAR:
		case SITE_LABORATORY_GENETIC:
		case SITE_GOVERNMENT_POLICESTATION:
		case SITE_GOVERNMENT_COURTHOUSE:
		case SITE_GOVERNMENT_PRISON:
		case SITE_GOVERNMENT_INTELLIGENCEHQ:
		case SITE_INDUSTRY_SWEATSHOP:
		case SITE_INDUSTRY_POLLUTER:
		case SITE_CORPORATE_HEADQUARTERS:
		case SITE_CORPORATE_HOUSE:
		case SITE_MEDIA_AMRADIO:
		case SITE_MEDIA_CABLENEWS:
			return 1;
		}

	return 0;
}

char select_view(creaturest *cr,long &v)
{
	int page=0;
	char str[80];

	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(0,0);
		addstr("Which topic will ");
		addstr(cr->name);
		addstr(" write about?");
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(1,0);
		addstr("----TOPIC-----------------------------------TOPIC-------------------------------");

		int y=2,x=0;
		for(int p=page*26;p<VIEWNUM-2&&p<page*26+26;p++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,x);
			addch((p-page*26)+'A');addstr(" - ");
			getview(str,p);
			addstr(str);

			y++;
			if(y==16)
				{
				y=2;
				x=40;
				}
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(22,0);
		addstr("Press a Letter to select a Topic");
		move(23,0);
		if(interface_pgup=='[')
			{
			addstr("[] to view other Liberal pages.");
			}
		else
			{
			addstr("PGUP/PGDN to view other Liberal pages.");
			}

		refresh();

		int c=getch();
		translategetch(c);

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*26<VIEWNUM-2)page++;

		if(c>='a'&&c<='s')
			{
			int p=page*26+(int)(c-'a');
			if(p<VIEWNUM-2)
				{
				v=p;
				return 1;
				}
			}

		if(c==10)break;
		}while(1);

	return 0;
}

void investlocation(void)
{
	int loc=selectedsiege;

	do
		{
		erase();

		locheader();
		printlocation(loc);

		if(!(location[loc]->compound_walls & COMPOUND_BASIC))
			{
			if(funds>=2000)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(8,1);
			addstr("W - Fortify the Compound for a Siege ($2000)");
			}

		if(!(location[loc]->compound_walls & COMPOUND_CAMERAS))
			{
			if(funds>=2000)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(9,1);
			addstr("C - Place Security Cameras around the Compound ($2000)");
			}

		if(!(location[loc]->compound_walls & COMPOUND_TRAPS))
			{
			if(funds>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(10,1);
			addstr("B - Place Booby Traps throughout the Compound ($3000)");
			}

		if(!(location[loc]->compound_walls & COMPOUND_TANKTRAPS))
			{
			if(funds>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(11,1);
			addstr("T - Ring the Compound with Tank Traps ($3000)");
			}

		if(!(location[loc]->compound_walls & COMPOUND_GENERATOR))
			{
			if(funds>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(12,1);
			addstr("G - Buy a Generator for Electricity ($3000)");
			}

		if(!(location[loc]->compound_walls & COMPOUND_PRINTINGPRESS))
			{
			if(funds>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(13,1);
			addstr("P - Setup a Printing Press ($3000)");
			}

		if(location[loc]->front_business==-1)
			{
			if(funds>=3000)set_color(COLOR_WHITE,COLOR_BLACK,0);
			else set_color(COLOR_BLACK,COLOR_BLACK,1);
			move(14,1);
			addstr("F - Setup a Business Front ($3000)");
			}

		if(funds>=150)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(15,1);
		addstr("R - Buy 20 more daily rations ($150)");

		move(16,1);
		addstr("Enter - Done");

		int c=getch();
		translategetch(c);

		if(c==10)break;

		if(c=='w')
			{
			if(!(location[loc]->compound_walls & COMPOUND_BASIC)&&funds>=2000)
				{
				funds-=2000;
				stat_spent+=2000;
				moneylost_compound+=2000;
				location[loc]->compound_walls|=COMPOUND_BASIC;
				}
			}

		if(c=='c')
			{
			if(!(location[loc]->compound_walls & COMPOUND_CAMERAS)&&funds>=2000)
				{
				funds-=2000;
				stat_spent+=2000;
				moneylost_compound+=2000;
				location[loc]->compound_walls|=COMPOUND_CAMERAS;
				}
			}

		if(c=='b')
			{
			if(!(location[loc]->compound_walls & COMPOUND_TRAPS)&&funds>=3000)
				{
				funds-=3000;
				stat_spent+=3000;
				moneylost_compound+=3000;
				location[loc]->compound_walls|=COMPOUND_TRAPS;
				}
			}

		if(c=='t')
			{
			if(!(location[loc]->compound_walls & COMPOUND_TANKTRAPS)&&funds>=3000)
				{
				funds-=3000;
				stat_spent+=3000;
				moneylost_compound+=3000;
				location[loc]->compound_walls|=COMPOUND_TANKTRAPS;
				}
			}

		if(c=='g')
			{
			if(!(location[loc]->compound_walls & COMPOUND_GENERATOR)&&funds>=3000)
				{
				funds-=3000;
				stat_spent+=3000;
				moneylost_compound+=3000;
				location[loc]->compound_walls|=COMPOUND_GENERATOR;
				}
			}

		if(c=='p')
			{
			if(!(location[loc]->compound_walls & COMPOUND_PRINTINGPRESS)&&funds>=3000)
				{
				funds-=3000;
				stat_spent+=3000;
				moneylost_compound+=3000;
				location[loc]->compound_walls|=COMPOUND_PRINTINGPRESS;
				}
			}

		if(c=='r')
			{
			if(funds>=150)
				{
				funds-=150;
				stat_spent+=150;
				moneylost_compound+=150;
				location[loc]->compound_stores+=20;
				}
			}

		if(c=='f')
			{
			if(location[loc]->front_business==-1&&funds>=3000)
				{
				funds-=3000;
				stat_spent+=3000;
				moneylost_compound+=3000;
				location[loc]->front_business=LCSrandom(BUSINESSFRONTNUM);
				lastname(location[loc]->front_name);
				strcat(location[loc]->front_name," ");
				switch(location[loc]->front_business)
					{
					case BUSINESSFRONT_INSURANCE:
						switch(LCSrandom(3))
							{
							case 0:strcat(location[loc]->front_name,"Auto");break;
							case 1:strcat(location[loc]->front_name,"Life");break;
							case 2:strcat(location[loc]->front_name,"Health");break;
							}
						strcat(location[loc]->front_name," Insurance");
						break;
					case BUSINESSFRONT_TEMPAGENCY:
						switch(LCSrandom(2))
							{
							case 0:strcat(location[loc]->front_name,"Temp Agency");break;
							case 1:strcat(location[loc]->front_name,"Manpower, LLC");break;
							}
						break;
					}
				}
			}
		}while(1);
}

void removesquadinfo(creaturest &cr)
{
	if(cr.squadid!=-1)
		{
		long sq=getsquad(cr.squadid);
		if(sq!=-1)
			{
			char flipstart=0;
			for(int pt=0;pt<6;pt++)
				{
				if(squad[sq]->squad[pt]==&cr)flipstart=1;
				if(flipstart&&pt<5)squad[sq]->squad[pt]=squad[sq]->squad[pt+1];
				}
			if(flipstart)squad[sq]->squad[5]=NULL;
			}
		cr.squadid=-1;
		}
}

void verifyworklocation(creaturest &cr)
{
	char okaysite[SITENUM];
	memset(okaysite,0,SITENUM*sizeof(char));

	switch(cr.type)
		{
		case CREATURE_CORPORATE_CEO:
			okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
			break;
		case CREATURE_SECURITYGUARD:
			okaysite[SITE_RESIDENTIAL_APARTMENT_UPSCALE]=1;
			okaysite[SITE_LABORATORY_COSMETICS]=1;
			okaysite[SITE_LABORATORY_GENETIC]=1;
			okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
			okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
			okaysite[SITE_INDUSTRY_SWEATSHOP]=1;
			okaysite[SITE_INDUSTRY_POLLUTER]=1;
			okaysite[SITE_INDUSTRY_NUCLEAR]=1;
			okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
			okaysite[SITE_CORPORATE_HOUSE]=1;
			okaysite[SITE_MEDIA_AMRADIO]=1;
			okaysite[SITE_MEDIA_CABLENEWS]=1;
			okaysite[SITE_BUSINESS_CIGARBAR]=1;
			break;
		case CREATURE_SCIENTIST_LABTECH:
			okaysite[SITE_LABORATORY_COSMETICS]=1;
			okaysite[SITE_LABORATORY_GENETIC]=1;
			break;
		case CREATURE_SCIENTIST_EMINENT:
			okaysite[SITE_LABORATORY_COSMETICS]=1;
			okaysite[SITE_LABORATORY_GENETIC]=1;
			break;
		case CREATURE_CORPORATE_MANAGER:
			okaysite[SITE_LABORATORY_COSMETICS]=1;
			okaysite[SITE_LABORATORY_GENETIC]=1;
			okaysite[SITE_INDUSTRY_SWEATSHOP]=1;
			okaysite[SITE_INDUSTRY_POLLUTER]=1;
			okaysite[SITE_INDUSTRY_NUCLEAR]=1;
			okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
			okaysite[SITE_MEDIA_AMRADIO]=1;
			okaysite[SITE_MEDIA_CABLENEWS]=1;
			break;
		case CREATURE_WORKER_SERVANT:
			okaysite[SITE_CORPORATE_HOUSE]=1;
			break;
		case CREATURE_WORKER_JANITOR:
			okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
			okaysite[SITE_RESIDENTIAL_APARTMENT]=1;
			okaysite[SITE_RESIDENTIAL_APARTMENT_UPSCALE]=1;
			okaysite[SITE_LABORATORY_COSMETICS]=1;
			okaysite[SITE_LABORATORY_GENETIC]=1;
			okaysite[SITE_HOSPITAL_CLINIC]=1;
			okaysite[SITE_HOSPITAL_UNIVERSITY]=1;
			okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
			okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
			okaysite[SITE_GOVERNMENT_PRISON]=1;
			okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
			okaysite[SITE_INDUSTRY_POLLUTER]=1;
			okaysite[SITE_INDUSTRY_NUCLEAR]=1;
			okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
			okaysite[SITE_MEDIA_AMRADIO]=1;
			okaysite[SITE_MEDIA_CABLENEWS]=1;
			okaysite[SITE_BUSINESS_PAWNSHOP]=1;
			okaysite[SITE_BUSINESS_CRACKHOUSE]=1;
			okaysite[SITE_BUSINESS_JUICEBAR]=1;
			okaysite[SITE_BUSINESS_CIGARBAR]=1;
			okaysite[SITE_BUSINESS_LATTESTAND]=1;
			okaysite[SITE_BUSINESS_VEGANCOOP]=1;
			okaysite[SITE_BUSINESS_INTERNETCAFE]=1;
			okaysite[SITE_BUSINESS_DEPTSTORE]=1;
			okaysite[SITE_BUSINESS_HALLOWEEN]=1;
			break;
		case CREATURE_WORKER_SWEATSHOP:
			okaysite[SITE_INDUSTRY_SWEATSHOP]=1;
			break;
		case CREATURE_WORKER_FACTORY_NONUNION:
			okaysite[SITE_INDUSTRY_POLLUTER]=1;
			break;
		case CREATURE_WORKER_FACTORY_CHILD:
			okaysite[SITE_INDUSTRY_POLLUTER]=1;
			break;
		case CREATURE_WORKER_SECRETARY:
			okaysite[SITE_LABORATORY_COSMETICS]=1;
			okaysite[SITE_LABORATORY_GENETIC]=1;
			okaysite[SITE_HOSPITAL_CLINIC]=1;
			okaysite[SITE_HOSPITAL_UNIVERSITY]=1;
			okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
			okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
			okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
			okaysite[SITE_INDUSTRY_POLLUTER]=1;
			okaysite[SITE_INDUSTRY_NUCLEAR]=1;
			okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
			okaysite[SITE_CORPORATE_HOUSE]=1;
			okaysite[SITE_MEDIA_AMRADIO]=1;
			okaysite[SITE_MEDIA_CABLENEWS]=1;
			break;
		case CREATURE_WORKER_FACTORY_UNION:
			okaysite[SITE_INDUSTRY_POLLUTER]=1;
			break;
		case CREATURE_LANDLORD:
			okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
			okaysite[SITE_RESIDENTIAL_APARTMENT]=1;
			okaysite[SITE_RESIDENTIAL_APARTMENT_UPSCALE]=1;
			break;
		case CREATURE_TEENAGER:
			okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
			okaysite[SITE_RESIDENTIAL_APARTMENT]=1;
			okaysite[SITE_RESIDENTIAL_APARTMENT_UPSCALE]=1;
			okaysite[SITE_RESIDENTIAL_SHELTER]=1;
			okaysite[SITE_CORPORATE_HOUSE]=1;
			break;
		case CREATURE_COP:
			okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
			break;
		case CREATURE_DEATHSQUAD:
			okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
			break;
		case CREATURE_GANGUNIT:
			okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
			break;
		case CREATURE_JUDGE_LIBERAL:
			okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
			break;
		case CREATURE_JUDGE_CONSERVATIVE:
			okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
			break;
		case CREATURE_AGENT:
			okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
			break;
		case CREATURE_RADIOPERSONALITY:
			okaysite[SITE_MEDIA_AMRADIO]=1;
			break;
		case CREATURE_NEWSANCHOR:
			okaysite[SITE_MEDIA_CABLENEWS]=1;
			break;
		case CREATURE_GENETIC:
			okaysite[SITE_LABORATORY_GENETIC]=1;
			break;
		case CREATURE_GUARDDOG:
			okaysite[SITE_GOVERNMENT_PRISON]=1;
			okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
			okaysite[SITE_CORPORATE_HOUSE]=1;
			break;
		case CREATURE_PRISONER:
			okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
			okaysite[SITE_RESIDENTIAL_SHELTER]=1;
			break;
		case CREATURE_JUROR:
			okaysite[SITE_RESIDENTIAL_APARTMENT]=1;
			break;
		case CREATURE_LAWYER:
			okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
			break;
		case CREATURE_SEWERWORKER:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_COLLEGESTUDENT:
			okaysite[SITE_UDISTRICT]=1;
			break;
		case CREATURE_MUSICIAN:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_MATHEMATICIAN:
			okaysite[SITE_UDISTRICT]=1;
			break;
		case CREATURE_TEACHER:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_HSDROPOUT:
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_BUM:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			okaysite[SITE_RESIDENTIAL_SHELTER]=1;
			break;
		case CREATURE_GANGMEMBER:
			okaysite[SITE_BUSINESS_CRACKHOUSE]=1;
			break;
		case CREATURE_CRACKHEAD:
			okaysite[SITE_BUSINESS_CRACKHOUSE]=1;
			break;
		case CREATURE_PRIEST:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_ENGINEER:
			okaysite[SITE_MEDIA_AMRADIO]=1;
			okaysite[SITE_MEDIA_CABLENEWS]=1;
			break;
		case CREATURE_FASTFOODWORKER:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_TELEMARKETER:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_OFFICEWORKER:
			okaysite[SITE_LABORATORY_COSMETICS]=1;
			okaysite[SITE_LABORATORY_GENETIC]=1;
			okaysite[SITE_HOSPITAL_CLINIC]=1;
			okaysite[SITE_HOSPITAL_UNIVERSITY]=1;
			okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
			okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
			okaysite[SITE_MEDIA_AMRADIO]=1;
			okaysite[SITE_MEDIA_CABLENEWS]=1;
			okaysite[SITE_BUSINESS_DEPTSTORE]=1;
			break;
		case CREATURE_FOOTBALLCOACH:
			okaysite[SITE_UDISTRICT]=1;
			break;
		case CREATURE_PROSTITUTE:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_MAILMAN:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_GARBAGEMAN:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_PLUMBER:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_CHEF:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			okaysite[SITE_BUSINESS_CIGARBAR]=1;
			break;
		case CREATURE_CONSTRUCTIONWORKER:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_AMATEURMAGICIAN:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_TANK:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_MERC:
			okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
			break;
		case CREATURE_HICK:
			okaysite[SITE_MEDIA_AMRADIO]=1;
			okaysite[SITE_MEDIA_CABLENEWS]=1;
			break;
		case CREATURE_SOLDIER:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_EDUCATOR:
		case CREATURE_PRISONGUARD:
			okaysite[SITE_GOVERNMENT_PRISON]=1;
			break;
		case CREATURE_HIPPIE:
			okaysite[SITE_BUSINESS_VEGANCOOP]=1;
			break;
		case CREATURE_CRITIC_ART:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_CRITIC_MUSIC:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_SOCIALITE:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_PROGRAMMER:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
			okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
			break;
		case CREATURE_RETIREE:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_PAINTER:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_SCULPTOR:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_AUTHOR:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_JOURNALIST:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_DANCER:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_PHOTOGRAPHER:
			okaysite[SITE_MEDIA_CABLENEWS]=1;
			break;
		case CREATURE_CAMERAMAN:
			okaysite[SITE_MEDIA_CABLENEWS]=1;
			break;
		case CREATURE_HAIRSTYLIST:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_FASHIONDESIGNER:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_CLERK:
			okaysite[SITE_BUSINESS_JUICEBAR]=1;
			okaysite[SITE_BUSINESS_LATTESTAND]=1;
			okaysite[SITE_BUSINESS_INTERNETCAFE]=1;
			okaysite[SITE_BUSINESS_DEPTSTORE]=1;
			okaysite[SITE_BUSINESS_HALLOWEEN]=1;
			break;
		case CREATURE_THIEF:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_ACTOR:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_YOGAINSTRUCTOR:
			okaysite[SITE_BUSINESS_VEGANCOOP]=1;
			break;
		case CREATURE_ATHLETE:
			okaysite[SITE_UDISTRICT]=1;
			break;
		case CREATURE_BIKER:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_TRUCKER:
			okaysite[SITE_OUTOFTOWN]=1;
			break;
		case CREATURE_TAXIDRIVER:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		case CREATURE_NUN:
			okaysite[SITE_DOWNTOWN]=1;
			okaysite[SITE_UDISTRICT]=1;
			okaysite[SITE_INDUSTRIAL]=1;
			break;
		}

	char swap=0;
	if(cr.worklocation==-1)swap=1;
	else
		{
		if(!okaysite[location[cr.worklocation]->type])swap=1;
		}

	if(swap)
		{
		//PICK A TYPE OF WORK LOCATION
		do
			{
			cr.worklocation=LCSrandom(SITENUM);
			}while(!okaysite[cr.worklocation]);

		//FIND ONE OF THESE
		vector<int> goodlist;

		for(int l=0;l<location.size();l++)
			{
			if(location[l]->type==cr.worklocation)
				{
				goodlist.push_back(l);
				}
			}
// Sadler - This line sometimes causes a memory fault

//          Only thing I can think of is if loop above didn'

//          find any locations of type == to cr.worklocation

//          My hunch is that some locations, such as the 1st four

//          are special and cannot be used here..

//		

//	TODO There was a bug in the makecharacter() code where th

//	SITE_OUTOFTOWN was not set properly. This was fixed but the bug here

//	is still occuring, normally at the Latte Bar Downtown ;

	if (goodlist.size()==0)

              {

               cr.worklocation=0;

              }

            else

              {

	   cr.worklocation=goodlist[LCSrandom(goodlist.size())];

              }





		}
}

void sleepereffect(creaturest &cr,char &clearformess)
{
	int power=(cr.attval(ATTRIBUTE_CHARISMA)+
					cr.attval(ATTRIBUTE_HEART)+cr.attval(ATTRIBUTE_INTELLIGENCE)+
					cr.skill[SKILL_PERSUASION]);
	int view=-1;

	switch(cr.type)
		{
		case CREATURE_CORPORATE_CEO:
		case CREATURE_RADIOPERSONALITY:
		case CREATURE_NEWSANCHOR:
		case CREATURE_DEATHSQUAD:
		case CREATURE_EDUCATOR:
			power>>=1;
			power++;
			break;
		case CREATURE_ACTOR:
		case CREATURE_GANGUNIT:
			power>>=2;
			power++;
			break;
		default:
			power>>=3;
			power++;
			break;
		}

	switch(cr.type)
		{
		case CREATURE_CORPORATE_CEO:
			switch(LCSrandom(3))
				{
				case 0:view=VIEW_CEOSALARY;break;
				case 1:view=VIEW_CORPORATECULTURE;break;
				case 2:view=VIEW_TAXES;break;
				}
			break;
		case CREATURE_RADIOPERSONALITY:
		case CREATURE_NEWSANCHOR:
		case CREATURE_CRITIC_ART:
		case CREATURE_CRITIC_MUSIC:
		case CREATURE_ACTOR:
			view=LCSrandom(VIEWNUM-2);
			break;
		case CREATURE_SCIENTIST_EMINENT:
			switch(LCSrandom(3))
				{
				case 0:view=VIEW_NUCLEARPOWER;break;
				case 1:view=VIEW_ANIMALRESEARCH;break;
				case 2:view=VIEW_GENETICS;break;
				}
			break;
		case CREATURE_LAWYER:
		case CREATURE_JUDGE_CONSERVATIVE:
			switch(LCSrandom(8))
				{
				case 0:view=VIEW_DEATHPENALTY;break;
				case 1:view=VIEW_FREESPEECH;break;
				case 2:view=VIEW_GAY;break;
				case 3:view=VIEW_JUSTICES;break;
				case 4:view=VIEW_ABORTION;break;
				case 5:view=VIEW_INTELLIGENCE;break;
				case 6:view=VIEW_POLICEBEHAVIOR;break;
				case 7:view=VIEW_PRISONS;break;
				}
			break;
		case CREATURE_CORPORATE_MANAGER:
			switch(LCSrandom(3))
				{
				case 0:view=VIEW_CEOSALARY;break;
				case 1:view=VIEW_CORPORATECULTURE;break;
				case 2:view=VIEW_TAXES;break;
				}
			break;
		case CREATURE_COP:
		case CREATURE_GANGUNIT:
			view=VIEW_POLICEBEHAVIOR;
			break;
		case CREATURE_DEATHSQUAD:
			switch(LCSrandom(2))
				{
				case 0:view=VIEW_DEATHPENALTY;break;
				case 1:view=VIEW_POLICEBEHAVIOR;break;
				}
			break;
		case CREATURE_EDUCATOR:
			switch(LCSrandom(2))
				{
				case 0:view=VIEW_DEATHPENALTY;break;
				case 1:view=VIEW_PRISONS;break;
				}
			break;
		case CREATURE_PRISONGUARD:
			view=VIEW_PRISONS;
			break;
		case CREATURE_AGENT:
			view=VIEW_INTELLIGENCE;
			break;
		case CREATURE_PRISONER:
			view=VIEW_PRISONS;
			break;
		}

	if(LCSrandom(3))power>>=1;
	if(LCSrandom(3))power>>=1;
	if(power==0)return;
	if(view==-1)return;

	change_public_opinion(view,power,0);
}

void majornewspaper(char &clearformess,char canseethings)
{
 int n = 0 ;
 
	//SET UP MAJOR EVENTS
	if(!LCSrandom(VIEWNUM*5))
		{
		newsstoryst *ns=new newsstoryst;
			ns->type=NEWSSTORY_MAJOREVENT;
			do
				{
				ns->view=LCSrandom(VIEWNUM-2);
				ns->positive=LCSrandom(2);

				//NO ABORTION
				if(ns->view==VIEW_ABORTION&&ns->positive&&law[LAW_ABORTION]==-2)continue;
				//NO PARTIAL BIRTH ABORTION
				if(ns->view==VIEW_ABORTION&&!ns->positive&&law[LAW_ABORTION]<2)continue;
				//NO DEATH PENALTY
				if(ns->view==VIEW_DEATHPENALTY&&law[LAW_ABORTION]==2)continue;
				//NO NUCLEAR POWER
				if(ns->view==VIEW_NUCLEARPOWER&&ns->positive&&law[LAW_NUCLEARPOWER]==2)continue;
				//NO ANIMAL RESEARCH
				if(ns->view==VIEW_ANIMALRESEARCH&&law[LAW_ANIMALRESEARCH]==2)continue;
				//NO BAD COPS
				if(ns->view==VIEW_POLICEBEHAVIOR&&ns->positive&&law[LAW_POLICEBEHAVIOR]==2)continue;
				//NO PRIVACY VIOLATIONS
				if(ns->view==VIEW_INTELLIGENCE&&ns->positive&&law[LAW_PRIVACY]==2)continue;
				//NO SWEATSHOPS
				if(ns->view==VIEW_SWEATSHOPS&&ns->positive&&law[LAW_LABOR]==2)continue;
				//NO POLLUTION
				if(ns->view==VIEW_POLLUTION&&ns->positive&&law[LAW_POLLUTION]>=1)continue;
				//NO ENRONS
				if(ns->view==VIEW_CORPORATECULTURE&&ns->positive&&law[LAW_CORPORATE]==2)continue;
				//NO CEOS
				if(ns->view==VIEW_CEOSALARY&&ns->positive&&law[LAW_CORPORATE]==2)continue;
				//NO FREEDOM OF SPEECH
				if(ns->view==VIEW_AMRADIO&&!ns->positive&&law[LAW_FREESPEECH]==-2)continue;

				break;
				}while(1);
		newsstory.push_back(ns);

		if(ns->positive)change_public_opinion(ns->view,20,0,0);
		else change_public_opinion(ns->view,-20,0,0);
		}

	//DELETE STORIES THAT HAVE NO CONTENT
	for(n=newsstory.size()-1;n>=0;n--)
		{
		if(newsstory[n]->type==NEWSSTORY_SQUAD_SITE&&
			newsstory[n]->crime.size()==0)
			{
			delete newsstory[n];
			newsstory.erase(newsstory.begin() + n);
			continue;
			}

		if(newsstory[n]->type==NEWSSTORY_CARTHEFT)
			{
			char conf=0;
			for(int c=0;c<newsstory[n]->crime.size();c++)
				{
				if(newsstory[n]->crime[c]==CRIME_KILLEDSOMEBODY)
					{
					conf=1;
					break;
					}
				}
			if(!conf)
				{
				delete newsstory[n];
				newsstory.erase(newsstory.begin() + n);
				continue;
				}
			}

		if((newsstory[n]->type==NEWSSTORY_SQUAD_ESCAPED||
			newsstory[n]->type==NEWSSTORY_SQUAD_FLEDATTACK||
			newsstory[n]->type==NEWSSTORY_SQUAD_DEFENDED||
			newsstory[n]->type==NEWSSTORY_SQUAD_BROKESIEGE||
			newsstory[n]->type==NEWSSTORY_SQUAD_KILLED_SIEGEATTACK||
			newsstory[n]->type==NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE)&&
			newsstory[n]->siegetype!=SIEGE_POLICE)
			{
			delete newsstory[n];
			newsstory.erase(newsstory.begin() + n);
			continue;
			}
		}

	//BAIL IF NO STORIES
	if(newsstory.size()==0)return;

	clearformess=1;

	//DO TELEVISION AND OTHER NON-NEWS STORIES, THEN DELETE THEM
	if(canseethings)
		{
		char del;
		for(int n=newsstory.size()-1;n>=0;n--)
			{
			del=0;
			if(newsstory[n]->type==NEWSSTORY_MAJOREVENT)
				{
				if(newsstory[n]->positive)
					{
					switch(newsstory[n]->view)
						{
						case VIEW_POLICEBEHAVIOR:
							movie.loadmovie("art\\lacops.cmv");
							movie.playmovie(0,0);
							nodelay(stdscr,FALSE);

							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(19,13);
							addstr("/----------------------------------------------------\\");
							move(20,13);
							addstr("|     The  police  have  beaten  a  black  man  in   |");
							move(21,13);
							addstr("|   Los Angeles again.  This time, the incident is   |");
							move(22,13);
							addstr("|   taped by  a passerby  and saturates  the news.   |");
							move(23,13);
							addstr("\\----------------------------------------------------/");

							refresh();
							getch();

							del=1;
							break;
						case VIEW_CABLENEWS:
							{
							char str[80];
							strcpy(str,"Tonight on a Cable News channel: ");
							switch(LCSrandom(5))
								{
								case 0:strcat(str,"Cross");break;
								case 1:strcat(str,"Hard");break;
								case 2:strcat(str,"Lightning");break;
								case 3:strcat(str,"Washington");break;
								case 4:strcat(str,"Capital");break;
								}
							switch(LCSrandom(5))
								{
								case 0:strcat(str," Fire");break;
								case 1:strcat(str," Ball");break;
								case 2:strcat(str," Talk");break;
								case 3:strcat(str," Insider");break;
								case 4:strcat(str," Gang");break;
								}
							strcat(str," with ");
							char bname[80];
							name(bname);
							strcat(str,bname);

							erase();
							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(0,39-((strlen(str)-1)>>1));
							addstr(str);

							move(16,20);
							addstr(bname);
							move(17,20);
							addstr("Washington D.C.");

							move(16,41);
							name(bname);
							addstr(bname);
							move(17,41);
							switch(LCSrandom(3))
								{
								case 0:addstr("Eugene, OR");break;
								case 1:addstr("San Francisco, CA");break;
								case 2:addstr("Cambridge, MA");break;
								}

							movie.loadmovie("art\\newscast.cmv");

							movie.playmovie(1,1);
							nodelay(stdscr,FALSE);

							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(19,13);
							addstr("/----------------------------------------------------\\");
							move(20,13);
							addstr("|     A  Cable  News  anchor  accidentally  let  a   |");
							move(21,13);
							addstr("|   bright Liberal guest  finish a sentence.  Many   |");
							move(22,13);
							addstr("|   viewers  across  the  nation  were  listening.   |");
							move(23,13);
							addstr("\\----------------------------------------------------/");

							refresh();
							getch();

							del=1;
							break;
							}
						}
					}
				else
					{
					switch(newsstory[n]->view)
						{
						case VIEW_CEOSALARY:
							movie.loadmovie("art\\glamshow.cmv");
							movie.playmovie(0,0);
							nodelay(stdscr,FALSE);

							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(19,13);
							addstr("/----------------------------------------------------\\");
							move(20,13);
							addstr("|     A new show glamorizing the lives of the rich   |");
							move(21,13);
							addstr("|   begins airing  this week.  With the nationwide   |");
							move(22,13);
							addstr("|   advertising  blitz, it's bound  to be popular.   |");
							move(23,13);
							addstr("\\----------------------------------------------------/");

							refresh();
							getch();

							del=1;
							break;
						case VIEW_CABLENEWS:
							movie.loadmovie("art\\anchor.cmv");
							movie.playmovie(0,0);
							nodelay(stdscr,FALSE);

							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(19,13);
							addstr("/----------------------------------------------------\\");
							move(20,13);
							addstr("|     A major Cable News channel has hired a slick   |");
							move(21,13);
							addstr("|   new anchor for one of  its news shows.  Guided   |");
							move(22,13);
							addstr("|   by impressive  advertising, America  tunes in.   |");
							move(23,13);
							addstr("\\----------------------------------------------------/");

							refresh();
							getch();

							del=1;
							break;
						case VIEW_ABORTION:
							erase();

							movie.loadmovie("art\\abort.cmv");
							movie.playmovie(0,0);
							nodelay(stdscr,FALSE);

							set_color(COLOR_WHITE,COLOR_BLACK,1);
							move(19,13);
							addstr("/----------------------------------------------------\\");
							move(20,13);
							addstr("|     A  failed partial  birth abortion  goes on a   |");
							move(21,13);
							addstr("|   popular  afternoon   talk  show.   The  studio   |");
							move(22,13);
							addstr("|   audience and viewers nationwide feel its pain.   |");
							move(23,13);
							addstr("\\----------------------------------------------------/");

							refresh();
							getch();

							del=1;
							break;
						}
					}
				}
			if(del)
				{
				delete newsstory[n];
				newsstory.erase(newsstory.begin() + n);
				}
			}
		}

	//BAIL IF NO STORIES
	if(newsstory.size()==0)return;

	//ASSIGN PAGE NUMBERS TO STORIES BASED ON THEIR PRIORITY
	for(n=0;n<newsstory.size();n++)
		{
		setpriority(*newsstory[n]);
		newsstory[n]->page=-1;
		}
	char acted;
	int curpage=1;
	do
		{
		acted=0;
		int maxn=-1;
		int maxp=-1;
		for(n=0;n<newsstory.size();n++)
			{
			if(newsstory[n]->priority>maxp&&
				newsstory[n]->page==-1)
				{
				maxn=n;
				maxp=newsstory[n]->priority;
				}
			}
		if(maxn!=-1)
			{
			if(newsstory[maxn]->priority<30&&curpage==1)curpage=2;
			if(newsstory[maxn]->priority<25&&curpage<3)curpage=3+LCSrandom(2);
			if(newsstory[maxn]->priority<20&&curpage<5)curpage=5+LCSrandom(5);
			if(newsstory[maxn]->priority<15&&curpage<10)curpage=10+LCSrandom(10);
			if(newsstory[maxn]->priority<10&&curpage<20)curpage=20+LCSrandom(10);
			if(newsstory[maxn]->priority<5&&curpage<30)curpage=30+LCSrandom(20);

			newsstory[maxn]->page=curpage;
			curpage++;
			acted=1;
			}
		}while(acted);

	//DISPLAY PAPER
	if(canseethings)
		{
		for(n=0;n<newsstory.size();n++)
			{
			displaystory(*newsstory[n]);
			}
		}

	//CHANGE FOR SQUAD ACTS PUBLIC OPINION BASED ON PAGE NUMBERS
		//AND OVERALL POWER OF THE STORY
	int power;
	for(n=0;n<newsstory.size();n++)
		{
		if(newsstory[n]->type==NEWSSTORY_SQUAD_SITE||
			newsstory[n]->type==NEWSSTORY_SQUAD_ESCAPED||
			newsstory[n]->type==NEWSSTORY_SQUAD_FLEDATTACK||
			newsstory[n]->type==NEWSSTORY_SQUAD_DEFENDED||
			newsstory[n]->type==NEWSSTORY_SQUAD_BROKESIEGE||
			newsstory[n]->type==NEWSSTORY_SQUAD_KILLED_SIEGEATTACK||
			newsstory[n]->type==NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE||
			newsstory[n]->type==NEWSSTORY_SQUAD_KILLED_SITE)
			{
			power=newsstory[n]->priority;

			//PAGE BONUS
			if(newsstory[n]->page==1)power*=5;
			else if(newsstory[n]->page==2)power*=3;
			else if(newsstory[n]->page==3)power*=2;

			int maxpower;
			if(newsstory[n]->page==1)maxpower=600;
			else if(newsstory[n]->page<5)maxpower=100*(5-newsstory[n]->page);
			else if(newsstory[n]->page<10)maxpower=50;
			else if(newsstory[n]->page<20)maxpower=30;
			else if(newsstory[n]->page<30)maxpower=10;
			else if(newsstory[n]->page<40)maxpower=5;
			else maxpower=1;

			if(power>maxpower)power=maxpower;
			power/=10;
			power++;
			change_public_opinion(VIEW_LIBERALCRIMESQUAD,2+power,0);
			if(newsstory[n]->positive)change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,power,0);
			else change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,-power,0);

			char colored=0;
			if(newsstory[n]->positive)colored=1;

			switch(location[newsstory[n]->loc]->type)
				{
				case SITE_LABORATORY_COSMETICS:
					change_public_opinion(VIEW_ANIMALRESEARCH,power/2+1,colored);
					newspaper_topicwork[VIEW_ANIMALRESEARCH]+=power/2+1;
					break;
				case SITE_LABORATORY_GENETIC:
					change_public_opinion(VIEW_GENETICS,power/2+1,colored);
					newspaper_topicwork[VIEW_GENETICS]+=power/2+1;
					break;
				case SITE_GOVERNMENT_POLICESTATION:
					change_public_opinion(VIEW_POLICEBEHAVIOR,power/2+1,colored);
					newspaper_topicwork[VIEW_POLICEBEHAVIOR]+=power/2+1;
					break;
				case SITE_GOVERNMENT_COURTHOUSE:
					change_public_opinion(VIEW_DEATHPENALTY,power/2+1,colored);
					change_public_opinion(VIEW_JUSTICES,power/2+1,colored);
					change_public_opinion(VIEW_FREESPEECH,power/2+1,colored);
					newspaper_topicwork[VIEW_DEATHPENALTY]+=power/2+1;
					newspaper_topicwork[VIEW_JUSTICES]+=power/2+1;
					newspaper_topicwork[VIEW_FREESPEECH]+=power/2+1;
					break;
				case SITE_GOVERNMENT_PRISON:
					change_public_opinion(VIEW_PRISONS,power/2+1,colored);
					newspaper_topicwork[VIEW_PRISONS]+=power/2+1;
					break;
				case SITE_GOVERNMENT_INTELLIGENCEHQ:
					change_public_opinion(VIEW_INTELLIGENCE,power/2+1,colored);
					newspaper_topicwork[VIEW_INTELLIGENCE]+=power/2+1;
					break;
				case SITE_INDUSTRY_SWEATSHOP:
					change_public_opinion(VIEW_SWEATSHOPS,power/2+1,colored);
					newspaper_topicwork[VIEW_SWEATSHOPS]+=power/2+1;
					break;
				case SITE_INDUSTRY_POLLUTER:
					change_public_opinion(VIEW_POLLUTION,power/2+1,colored);
					newspaper_topicwork[VIEW_POLLUTION]+=power/2+1;
					break;
				case SITE_INDUSTRY_NUCLEAR:
					change_public_opinion(VIEW_NUCLEARPOWER,power/2+1,colored);
					newspaper_topicwork[VIEW_NUCLEARPOWER]+=power/2+1;
					break;
				case SITE_CORPORATE_HEADQUARTERS:
					change_public_opinion(VIEW_TAXES,power/2+1,colored);
					change_public_opinion(VIEW_CORPORATECULTURE,power/2+1,colored);
					newspaper_topicwork[VIEW_TAXES]+=power/2+1;
					newspaper_topicwork[VIEW_CORPORATECULTURE]+=power/2+1;
					break;
				case SITE_CORPORATE_HOUSE:
					change_public_opinion(VIEW_TAXES,power/4+1,colored);
					change_public_opinion(VIEW_CEOSALARY,power/4+1,colored);
					newspaper_topicwork[VIEW_TAXES]+=power/2+1;
					newspaper_topicwork[VIEW_CEOSALARY]+=power/2+1;
					break;
				case SITE_MEDIA_AMRADIO:
					change_public_opinion(VIEW_AMRADIO,power/4+1,colored);
					newspaper_topicwork[VIEW_AMRADIO]+=power/2+1;
					break;
				case SITE_MEDIA_CABLENEWS:
					change_public_opinion(VIEW_CABLENEWS,power/4+1,colored);
					newspaper_topicwork[VIEW_CABLENEWS]+=power/2+1;
					break;
				case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
					change_public_opinion(VIEW_TAXES,power/4+1,colored);
					change_public_opinion(VIEW_CEOSALARY,power/4+1,colored);
					newspaper_topicwork[VIEW_TAXES]+=power/2+1;
					newspaper_topicwork[VIEW_CEOSALARY]+=power/2+1;
					break;
				case SITE_BUSINESS_CIGARBAR:
					change_public_opinion(VIEW_TAXES,power/4+1,colored);
					change_public_opinion(VIEW_CORPORATECULTURE,power/4+1,colored);
					newspaper_topicwork[VIEW_TAXES]+=power/2+1;
					newspaper_topicwork[VIEW_CORPORATECULTURE]+=power/2+1;
					break;
				}
			}
		}

	//DELETE STORIES
	for(n=0;n<newsstory.size();n++)delete newsstory[n];
	newsstory.clear();
}

void setpriority(newsstoryst &ns)
{
	switch(ns.type)
		{
		case NEWSSTORY_MAJOREVENT:
			ns.priority=30000;
			break;
		case NEWSSTORY_SQUAD_SITE:
		case NEWSSTORY_SQUAD_ESCAPED:
		case NEWSSTORY_SQUAD_FLEDATTACK:
		case NEWSSTORY_SQUAD_DEFENDED:
		case NEWSSTORY_SQUAD_BROKESIEGE:
		case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
		case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
		case NEWSSTORY_SQUAD_KILLED_SITE:
		case NEWSSTORY_CARTHEFT:
			{
			ns.priority=0;

			long crime[CRIMENUM];
			memset(crime,0,CRIMENUM*sizeof(long));
			for(int c=0;c<ns.crime.size();c++)
				{
				crime[ns.crime[c]]++;
				}
			if(crime[CRIME_STOLEGROUND]>10)crime[CRIME_STOLEGROUND]=10;
			if(crime[CRIME_UNLOCKEDDOOR]>10)crime[CRIME_UNLOCKEDDOOR]=10;
			if(crime[CRIME_BROKEDOWNDOOR]>10)crime[CRIME_BROKEDOWNDOOR]=10;
			if(crime[CRIME_ATTACKED_MISTAKE]>10)crime[CRIME_ATTACKED_MISTAKE]=10;
			if(crime[CRIME_ATTACKED]>10)crime[CRIME_ATTACKED]=10;
			if(crime[CRIME_BREAK_SWEATSHOP]>10)crime[CRIME_BREAK_SWEATSHOP]=10;
			if(crime[CRIME_BREAK_FACTORY]>10)crime[CRIME_BREAK_FACTORY]=10;
			if(crime[CRIME_FREE_RABBITS]>10)crime[CRIME_BREAK_FACTORY]=10;
			if(crime[CRIME_FREE_BEASTS]>10)crime[CRIME_BREAK_FACTORY]=10;

			ns.priority+=crime[CRIME_STOLEGROUND];
			ns.priority+=crime[CRIME_UNLOCKEDDOOR];
			ns.priority+=crime[CRIME_BROKEDOWNDOOR];
			ns.priority+=crime[CRIME_ATTACKED_MISTAKE]*7;
			ns.priority+=crime[CRIME_ATTACKED]*4;
			ns.priority+=crime[CRIME_KILLEDSOMEBODY]*30;
			ns.priority+=crime[CRIME_SHUTDOWNREACTOR]*100;
			ns.priority+=crime[CRIME_POLICE_LOCKUP]*30;
			ns.priority+=crime[CRIME_COURTHOUSE_LOCKUP]*30;
			ns.priority+=crime[CRIME_PRISON_RELEASE]*50;
			ns.priority+=crime[CRIME_JURYTAMPERING]*30;
			ns.priority+=crime[CRIME_HACK_INTEL]*100;
			ns.priority+=crime[CRIME_BREAK_SWEATSHOP]*2;
			ns.priority+=crime[CRIME_BREAK_FACTORY]*2;
			ns.priority+=crime[CRIME_HOUSE_PHOTOS]*100;
			ns.priority+=crime[CRIME_CORP_FILES]*100;
			ns.priority+=crime[CRIME_FREE_RABBITS]*2;
			ns.priority+=crime[CRIME_FREE_BEASTS]*3;

			switch(ns.type)
				{
				case NEWSSTORY_SQUAD_ESCAPED:
					ns.priority+=10+attitude[VIEW_LIBERALCRIMESQUAD]/3;
					break;
				case NEWSSTORY_SQUAD_FLEDATTACK:
					ns.priority+=15+attitude[VIEW_LIBERALCRIMESQUAD]/3;
					break;
				case NEWSSTORY_SQUAD_DEFENDED:
					ns.priority+=30+attitude[VIEW_LIBERALCRIMESQUAD]/3;
					break;
				case NEWSSTORY_SQUAD_BROKESIEGE:
					ns.priority+=45+attitude[VIEW_LIBERALCRIMESQUAD]/3;
					break;
				case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
					ns.priority+=10+attitude[VIEW_LIBERALCRIMESQUAD]/3;
					break;
				case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
					ns.priority+=15+attitude[VIEW_LIBERALCRIMESQUAD]/3;
					break;
				case NEWSSTORY_SQUAD_KILLED_SITE:
					ns.priority+=10+attitude[VIEW_LIBERALCRIMESQUAD]/3;
					break;
				}

			if(ns.priority>20000)ns.priority=20000;
			break;
			}
		case NEWSSTORY_KIDNAPREPORT:
			ns.priority=20;
			if(ns.cr->type==CREATURE_CORPORATE_CEO||
				ns.cr->type==CREATURE_RADIOPERSONALITY||
				ns.cr->type==CREATURE_NEWSANCHOR||
				ns.cr->type==CREATURE_SCIENTIST_EMINENT||
				ns.cr->type==CREATURE_JUDGE_CONSERVATIVE)ns.priority=40;
			break;
		case NEWSSTORY_MASSACRE:
			ns.priority=10 + ns.crime[1]*5;
			break;
		}
}

void displaystory(newsstoryst &ns)
{
	int it2;
	for(int x=0;x<80;x++)
		{
		for(int y=0;y<25;y++)
			{
			move(y,x);
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			addch(CH_FULL_BLOCK);
			}
		}

	if(ns.page==1)
		{
		//TOP
		int pap=LCSrandom(5);
		for(int x=0;x<80;x++)
			{
			for(int y=0;y<5;y++)
				{
				move(y,x);
				set_color(newstops[pap][x][y][1],
					newstops[pap][x][y][2],
					newstops[pap][x][y][3]);
				addch(translateGraphicsChar(newstops[pap][x][y][0]));
				}
			}

		//DATE
		set_color(COLOR_BLACK,COLOR_WHITE,0);
		char num[20];
		if(day>=10)move(3,66);
		else move(3,67);
		switch(month)
			{
			case 1:addstr("Jan");break;
			case 2:addstr("Feb");break;
			case 3:addstr("Mar");break;
			case 4:addstr("Apr");break;
			case 5:addstr("May");break;
			case 6:addstr("Jun");break;
			case 7:addstr("Jul");break;
			case 8:addstr("Aug");break;
			case 9:addstr("Sep");break;
			case 10:addstr("Oct");break;
			case 11:addstr("Nov");break;
			case 12:addstr("Dec");break;
			}
		addstr(". ");
		itoa(day,num,10);
		addstr(num);
		addstr(", ");
		itoa(year,num,10);
		addstr(num);
		}
	else
		{
		//PAGE
		set_color(COLOR_BLACK,COLOR_WHITE,0);
		char num[20];
		itoa(ns.page,num,10);
		move(0,76);
		addstr(num);
		}

	char story[5000];
	short storyx_s[25];
	short storyx_e[25];
	for(it2=0;it2<25;it2++)storyx_s[it2]=1;
	for(it2=0;it2<25;it2++)storyx_e[it2]=78;
	int adnumber=0;
	if(ns.page>=10)adnumber++;
	if(ns.page>=20)adnumber+=LCSrandom(2)+1;
	if(ns.page>=30)adnumber+=LCSrandom(2)+1;
	if(ns.page>=40)adnumber+=LCSrandom(2)+1;
	if(ns.page>=50)adnumber+=LCSrandom(2)+1;
	char addplace[2][3]={0,0,0,0,0,0};
	if(adnumber>6)adnumber=6;
	while(adnumber>0)
		{
		int x,y;
		do
			{
			x=LCSrandom(2);y=LCSrandom(3);
			}while(addplace[x][y]);
		addplace[x][y]=1;
		adnumber--;

		int sx=0,ex=0,sy=0,ey=0;
		if(x==0)
			{
			sx=0;
			ex=23+LCSrandom(4);
			}
		if(x==1)
			{
			sx=57-LCSrandom(4);
			ex=79;
			}
		if(y==0)
			{
			sy=2;
			ey=9;
			}
		if(y==1)
			{
			sy=10;
			ey=17;
			}
		if(y==2)
			{
			sy=18;
			ey=24;
			}

		unsigned char ch='?';
		switch(LCSrandom(6))
			{
			case 0:ch=CH_LIGHT_SHADE;break;
			case 1:ch=CH_MEDIUM_SHADE;break;
			case 2:ch=CH_DARK_SHADE;break;
			case 3:ch=CH_FULL_BLOCK;break;
			case 4:ch=CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_HORIZONTAL;break;
			case 5:ch='*';break;
			}

		for(y=sy;y<=ey;y++)
			{
			if(storyx_s[y]<ex&&ex<43)storyx_s[y]=ex+2;
			if(storyx_e[y]>sx&&sx>43)storyx_e[y]=sx-2;

			if(ey==9&&y==9)continue;
			if(ey==17&&y==17)continue;
			for(x=sx;x<=ex;x++)
				{
				if(y==sy||y==8||y==16||y==24||x==sx||x==ex)
					{
					move(y,x);
					addch(translateGraphicsChar(ch));
					}
				}
			}

		//AD CONTENT
		short storyx_s[25];
		short storyx_e[25];
		for(it2=0;it2<25;it2++)storyx_s[it2]=40;
		for(it2=0;it2<25;it2++)storyx_e[it2]=40;
		for(it2=sy+1;it2<=ey-1;it2++)storyx_s[it2]=sx+1;
		for(it2=sy+1;it2<=ey-1;it2++)storyx_e[it2]=ex-1;

		char ad[500];
		switch(LCSrandom(5))
			{
			case 0:
				strcpy(ad,"&cNo Fee&r");
				strcat(ad,"&cConsignment Program&r&r");
				strcat(ad,"&cCall for Details&r");
				break;
			case 1:
				strcpy(ad,"&cFine Leather Chairs&r&r");
				strcat(ad,"&cSpecial Purchase&r");
				strcat(ad,"&cNow $599&r");
				break;
			case 2:
				strcpy(ad,"&cParis Flea Market&r&r");
				strcat(ad,"&cSale&r");
				strcat(ad,"&c50% Off&r");
				break;
			case 3:
				strcpy(ad,"&cQuality Pre-Owned&r");
				strcat(ad,"&cVehicles&r");
				strcat(ad,"&c1999 Lexus GS 300&r");
				strcat(ad,"&cSedan 4D&r");
				strcat(ad,"&cOnly $21,988&r");
				break;
			case 4:
				strcpy(ad,"&cSpa&r");
				strcat(ad,"&cHealth, Beauty&r");
				strcat(ad,"&cand Fitness&r&r");
				strcat(ad,"&c7 Days a Week&r");
				break;
			}

		displaynewsstory(ad,storyx_s,storyx_e,sy+1);
		}

	switch(ns.type)
		{
		case NEWSSTORY_MAJOREVENT:
			if(ns.positive)
				{
				switch(ns.view)
					{
					case VIEW_ABORTION:
						displaycenterednewsfont("CLINIC MURDER",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_GAY:
						displaycenterednewsfont("CRIME OF HATE",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_DEATHPENALTY:
						displaycenterednewsfont("JUSTICE DEAD",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_TAXES:
						{
						displaycenterednewsfont("REAGAN FLAWED",5);
						char str[80];
						strcpy(str,"");
						switch(LCSrandom(5))
							{
							case 0:strcat(str,"Shadow");break;
							case 1:strcat(str,"Dark");break;
							case 2:strcat(str,"Abyssal");break;
							case 3:strcat(str,"Orwellian");break;
							case 4:strcat(str,"Craggy");break;
							}
						strcat(str," ");
						switch(LCSrandom(5))
							{
							case 0:strcat(str,"Actor");break;
							case 1:strcat(str,"Lord");break;
							case 2:strcat(str,"Emperor");break;
							case 3:strcat(str,"Puppet");break;
							case 4:strcat(str,"Dementia");break;
							}
						strcat(str,": A new book further documenting the other side of Reagan.");
						displaycenteredsmallnews(str,12);
						displaynewspicture(2,13);
						break;
						}
					case VIEW_NUCLEARPOWER:
						displaycenterednewsfont("MELTDOWN",5);
						displaycenteredsmallnews("A nuclear power plant suffers a catastrophic meltdown.",12);
						displaynewspicture(3,13);
						break;
					case VIEW_ANIMALRESEARCH:
						displaycenterednewsfont("HELL ON EARTH",5);
						displaycenteredsmallnews("A mutant animal has escaped from a lab and killed thirty people.",12);
						displaynewspicture(0,13);
						break;
					case VIEW_PRISONS:
						displaycenterednewsfont("ON THE INSIDE",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_INTELLIGENCE:
						displaycenterednewsfont("THE FBI FILES",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_FREESPEECH:
						displaycenterednewsfont("BOOK BANNED",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_GENETICS:
						displaycenterednewsfont("KILLER FOOD",5);
						displaycenteredsmallnews("Over a hundred people become sick from genetically modified food.",12);
						displaynewspicture(4,13);
						break;
					case VIEW_JUSTICES:
						displaycenterednewsfont("IN CONTEMPT",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_SWEATSHOPS:
						displaycenterednewsfont("CHILD'S PLEA",5);
						displaycenteredsmallnews("A T-shirt in a store is found scrawled with a message from a sweatshop worker.",12);
						displaynewspicture(12,13);
						break;
					case VIEW_POLLUTION:
						displaycenterednewsfont("RING OF FIRE",5);
						displaycenteredsmallnews("The Ohio River caught on fire again.",12);
						displaynewspicture(5,13);
						break;
					case VIEW_CORPORATECULTURE:
						displaycenterednewsfont("BELLY UP",5);
						displaycenteredsmallnews("An enormous company files for bankruptcy, shattering the previous record.",12);
						displaynewspicture(6,13);
						break;
					case VIEW_CEOSALARY:
						{
						displaycenterednewsfont("AMERICAN CEO",5);
						char str[80];
			strcpy(str,"This major CEO ");
			switch(LCSrandom(10))
				{
				case 0:
					strcat(str,"regularly visits prostitutes.");
					break;
				case 1:
					strcat(str,"seeks the aid of psychics.");
					break;
				case 2:
					strcat(str,"donated millions to the KKK.");
					break;
				case 3:
					strcat(str,"evaded paying taxes.");
					break;
				case 4:
					strcat(str,"took out a contract on his wife.");
					break;
				case 5:
					strcat(str,"doesn't know what his company does.");
					break;
				case 6:
					strcat(str,"has a zoo of imported exotic worms.");
					break;
				case 7:
					strcat(str,"paid millions for high-tech bondage gear.");
					break;
				case 8:
					strcat(str,"installed a camera in an office bathroom.");
					break;
				case 9:
					strcat(str,"owns slaves in another country.");
					break;
				}
						displaycenteredsmallnews(str,12);
						displaynewspicture(1,13);
						break;
						}
					case VIEW_AMRADIO:
						displaycenterednewsfont("AM IMPLOSION",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					}
				}
			else
				{
				switch(ns.view)
					{
					case VIEW_GAY:
						displaycenterednewsfont("KINKY WINKY",5);
						displaycenteredsmallnews("Jerry Falwell explains the truth about Tinky Winky.  Again.",12);
						displaynewspicture(7,13);
						break;
					case VIEW_DEATHPENALTY:
						displaycenterednewsfont("LET'S FRY 'EM",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_TAXES:
						{
						displaycenterednewsfont("REAGAN THE MAN",5);
						char str[80];
						strcpy(str,"");
						switch(LCSrandom(5))
							{
							case 0:strcat(str,"Great");break;
							case 1:strcat(str,"Noble");break;
							case 2:strcat(str,"True");break;
							case 3:strcat(str,"Pure");break;
							case 4:strcat(str,"Golden");break;
							}
						strcat(str," ");
						switch(LCSrandom(5))
							{
							case 0:strcat(str,"Leadership");break;
							case 1:strcat(str,"Courage");break;
							case 2:strcat(str,"Pioneer");break;
							case 3:strcat(str,"Communicator");break;
							case 4:strcat(str,"Faith");break;
							}
						strcat(str,": A new book lauding Reagan and the greatest generation.");
						displaycenteredsmallnews(str,12);
						displaynewspicture(2,13);
						break;
						}
					case VIEW_NUCLEARPOWER:
						displaycenterednewsfont("OIL CRUNCH",5);
						displaycenteredsmallnews("OPEC cuts oil production sharply in response to a US foreign policy decision.",12);
						displaynewspicture(8,13);
						break;
					case VIEW_ANIMALRESEARCH:
						displaycenterednewsfont("APE EXPLORERS",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_POLICEBEHAVIOR:
						displaycenterednewsfont("BASTARDS",5);
						displaynewspicture(9,13);
						break;
					case VIEW_PRISONS:
						displaycenterednewsfont("HOSTAGE SLAIN",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_INTELLIGENCE:
						displaycenterednewsfont("DODGED BULLET",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_FREESPEECH:
						displaycenterednewsfont("HATE RALLY",5);
						displaycenteredsmallnews("Free speech advocates fight hard to let a white supremacist rally take place.",12);
						displaynewspicture(10,13);
						break;
					case VIEW_GENETICS:
						displaycenterednewsfont("GM FOOD FAIRE",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_JUSTICES:
						displaycenterednewsfont("JUSTICE AMOK",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_SWEATSHOPS:
						displaycenterednewsfont("THEY ARE HERE",5);
						if(month>=8&&month<=11)displaycenteredsmallnews("Fall fashions hit the stores across the country.",12);
						else displaycenteredsmallnews("Fall fashions are previewed in stores across the country.",12);
						displaynewspicture(11,13);
						break;
					case VIEW_POLLUTION:
						displaycenterednewsfont("LOOKING UP",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_CORPORATECULTURE:
						displaycenterednewsfont("NEW JOBS",5);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,13);
						break;
					case VIEW_AMRADIO:
						displaycenterednewsfont("THE DEATH",5);
						displaycenterednewsfont("OF CULTURE",13);
						constructeventstory(story,ns.view,ns.positive);
						displaynewsstory(story,storyx_s,storyx_e,21);
						break;
					}
				}
			break;
		case NEWSSTORY_SQUAD_SITE:
		case NEWSSTORY_SQUAD_ESCAPED:
		case NEWSSTORY_SQUAD_FLEDATTACK:
		case NEWSSTORY_SQUAD_DEFENDED:
		case NEWSSTORY_SQUAD_BROKESIEGE:
		case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
		case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
		case NEWSSTORY_SQUAD_KILLED_SITE:
		case NEWSSTORY_CARTHEFT:
			{
			int y=2;
			if(ns.page==1)
				{
				y=21;
				
				switch(ns.type)
					{
					case NEWSSTORY_CARTHEFT:
						displaycenterednewsfont("POLICE KILLED",5);
						y=13;
						break;
					case NEWSSTORY_SQUAD_ESCAPED:
					case NEWSSTORY_SQUAD_FLEDATTACK:
						displaycenterednewsfont("LCS ESCAPES",5);
						displaycenterednewsfont("POLICE SIEGE",13);
						break;
					case NEWSSTORY_SQUAD_DEFENDED:
					case NEWSSTORY_SQUAD_BROKESIEGE:
						displaycenterednewsfont("LCS FIGHTS",5);
						displaycenterednewsfont("OFF COPS",13);
						break;
					case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
					case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
						displaycenterednewsfont("LCS SIEGE",5);
						displaycenterednewsfont("TRAGIC END",13);
						break;
					default:
						if(ns.positive)
							{
							if(newscherrybusted)
								{
								displaycenterednewsfont("LCS STRIKES",5);
								y=13;
								}
							else
								{
								displaycenterednewsfont("LIBERAL CRIME",5);
								displaycenterednewsfont("SQUAD STRIKES",13);
								}
							}
						else
							{
							if(newscherrybusted)
								{
								displaycenterednewsfont("LCS RAMPAGE",5);
								y=13;
								}
							else
								{
								displaycenterednewsfont("LIBERAL CRIME",5);
								displaycenterednewsfont("SQUAD RAMPAGE",13);
								}
							}
						break;
					}
				}

			strcpy(story,lcityname);
			strcat(story," - ");

			switch(ns.type)
				{
				case NEWSSTORY_CARTHEFT:
					{
					int crime[CRIMENUM];
					memset(crime,0,sizeof(int)*CRIMENUM);
					for(int c=0;c<ns.crime.size();c++)
						{
						crime[ns.crime[c]]++;
						}

					strcat(story,"A routine arrest went horribly wrong yesterday, ");
					strcat(story,"according to a spokesperson from the police department.");
					strcat(story,"&r");
					strcat(story,"   A suspect, whose identify is unclear, ");
					strcat(story,"killed ");
					if(crime[CRIME_KILLEDSOMEBODY]>1)
						{
						char num[20];
						itoa(crime[CRIME_KILLEDSOMEBODY],num,10);
						strcat(story,num);
						strcat(story," police officers");
						}
					else strcat(story,"a police officer");
					strcat(story," that were attempting to perform an arrest.  A passerby had allegedly spotted the suspect committing a car theft.  ");
					strcat(story,"The names of the officers have not been released pending notification of their families.");
					strcat(story,"&r");
					break;
					}
				case NEWSSTORY_SQUAD_ESCAPED:
					strcat(story,"Members of the Liberal Crime Squad ");
					strcat(story,"escaped from a police siege yesterday, according ");
					strcat(story,"to a spokesperson from the police department.");
					strcat(story,"&r");
					break;
				case NEWSSTORY_SQUAD_FLEDATTACK:
					strcat(story,"Members of the Liberal Crime Squad ");
					strcat(story,"escaped from police officers during a raid yesterday, according ");
					strcat(story,"to a spokesperson from the police department.");
					strcat(story,"&r");
					break;
				case NEWSSTORY_SQUAD_DEFENDED:
					strcat(story,"Members of the Liberal Crime Squad ");
					strcat(story,"fought off a police raid yesterday, according ");
					strcat(story,"to a spokesperson from the police department.");
					strcat(story,"&r");
					break;
				case NEWSSTORY_SQUAD_BROKESIEGE:
					strcat(story,"Members of the Liberal Crime Squad ");
					strcat(story,"violently broke a police siege yesterday, according ");
					strcat(story,"to a spokesperson from the police department.");
					strcat(story,"&r");
					break;
				case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
					strcat(story,"Members of the Liberal Crime Squad were ");
					strcat(story,"slain during a police raid yesterday, according ");
					strcat(story,"to a spokesperson from the police department.");
					strcat(story,"&r");
					break;
				case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
					strcat(story,"Members of the Liberal Crime Squad were ");
					strcat(story,"slain trying to escape from a police siege yesterday, according ");
					strcat(story,"to a spokesperson from the police department.");
					strcat(story,"&r");
					break;
				default:
					{
					if(ns.type==NEWSSTORY_SQUAD_SITE)
						{
						if(!newscherrybusted)
							{
							if(ns.positive)
								{
								strcat(story,"A group calling itself the Liberal Crime Squad ");
								strcat(story,"burst onto the scene of political activism yesterday, according ");
								strcat(story,"to a spokesperson from the police department.");
								strcat(story,"&r");
								}
							else
								{
								strcat(story,"A group of thugs calling itself the Liberal Crime Squad ");
								strcat(story,"went on a rampage yesterday, according ");
								strcat(story,"to a spokesperson from the police department.");
								}
							}
						else
							{
							if(ns.positive)
								{
								strcat(story,"The Liberal Crime Squad has struck again.  ");
								strcat(story,"&r");
								}
							else
								{
								strcat(story,"The Liberal Crime Squad has gone on a rampage.  ");
								strcat(story,"&r");
								}
							}
						}
					else
						{
						if(!newscherrybusted)
							{
							if(ns.positive)
								{
								strcat(story,"A group calling itself the Liberal Crime Squad ");
								strcat(story,"burst briefly onto the scene of political activism yesterday, according ");
								strcat(story,"to a spokesperson from the police department.  ");
								strcat(story,"&r");
								}
							else
								{
								strcat(story,"A group of thugs calling itself the Liberal Crime Squad ");
								strcat(story,"went on a suicidal rampage yesterday, according ");
								strcat(story,"to a spokesperson from the police department.");
								}
							}
						else
							{
							if(ns.positive)
								{
								strcat(story,"The Liberal Crime Squad has struck again, albeit with a tragic end.  ");
								strcat(story,"&r");
								}
							else
								{
								strcat(story,"The Liberal Crime Squad has gone on a rampage, and they got what they deserved.  ");
								strcat(story,"&r");
								}
							}
						}

					strcat(story,"   The events took place at the ");
					strcat(story,location[ns.loc]->name);
					strcat(story,".  ");

					if(ns.type==NEWSSTORY_SQUAD_KILLED_SITE)
						{
						if(ns.positive)strcat(story,"Everyone in the LCS group was killed by conservatives.  ");
						else strcat(story,"Everyone in the LCS group was killed by brave citizens.  ");
						}
					strcat(story,"&r");

					int crime[CRIMENUM];
					memset(crime,0,sizeof(int)*CRIMENUM);
					int typesum=0;
					for(int c=0;c<ns.crime.size();c++)
						{
						crime[ns.crime[c]]++;

						if(ns.crime[c]==CRIME_HOUSE_PHOTOS)continue;
						if(ns.crime[c]==CRIME_CORP_FILES)continue;
						if(ns.crime[c]==CRIME_SHUTDOWNREACTOR)continue;
						if(ns.crime[c]==CRIME_POLICE_LOCKUP)continue;
						if(ns.crime[c]==CRIME_COURTHOUSE_LOCKUP)continue;
						if(ns.crime[c]==CRIME_PRISON_RELEASE)continue;
						if(ns.crime[c]==CRIME_JURYTAMPERING)continue;
						if(ns.crime[c]==CRIME_HACK_INTEL)continue;
						if(ns.crime[c]==CRIME_HOUSE_PHOTOS)continue;
						if(ns.crime[c]==CRIME_CORP_FILES)continue;
						if(ns.crime[c]==CRIME_CARCHASE)continue;
						if(ns.crime[c]==CRIME_CARCRASH)continue;
						if(ns.crime[c]==CRIME_FOOTCHASE)continue;
						if(crime[ns.crime[c]]==1)typesum++;
						}

					if(crime[CRIME_SHUTDOWNREACTOR])
						{
						strcat(story,"   Accorded to sources that were at the scene, ");
						strcat(story,"the Liberal Crime Squad caused the power out that struck the state ");
						strcat(story,"yesterday by tampering with equipment on the site.");
						strcat(story,"&r");
						}
					if(crime[CRIME_POLICE_LOCKUP])
						{
						strcat(story,"   Accorded to sources that were at the scene, ");
						strcat(story,"the Liberal Crime Squad allegedly freed or attempted to free prisoners from the police lockup.");
						strcat(story,"&r");
						}
					if(crime[CRIME_COURTHOUSE_LOCKUP])
						{
						strcat(story,"   Accorded to sources that were at the scene, ");
						strcat(story,"the Liberal Crime Squad allegedly freed or attempted to free prisoners from the court house lockup.");
						strcat(story,"&r");
						}
					if(crime[CRIME_PRISON_RELEASE])
						{
						strcat(story,"   Accorded to sources that were at the scene, ");
						strcat(story,"the Liberal Crime Squad allegedly freed prisoners while in the facility.");
						strcat(story,"&r");
						}
					if(crime[CRIME_JURYTAMPERING])
						{
						strcat(story,"   Accorded to police sources that were at the scene, ");
						strcat(story,"the Liberal Crime Squad allegedly violated the sacred ");
						strcat(story,"trust and attempted to influence a jury.");
						strcat(story,"&r");
						}
					if(crime[CRIME_HACK_INTEL])
						{
						strcat(story,"   Accorded to police sources that were at the scene, ");
						strcat(story,"intelligence officials seemed very nervous about something.");
						strcat(story,"&r");
						}
					if(crime[CRIME_HOUSE_PHOTOS])
						{
						strcat(story,"   Accorded to police sources that were at the scene, ");
						strcat(story,"the owner of the house seemed very frantic about some missing property.");
						strcat(story,"&r");
						}
					if(crime[CRIME_CORP_FILES])
						{
						strcat(story,"   Accorded to police sources that were at the scene, ");
						strcat(story,"executives on the scene seemed very nervous about something.");
						strcat(story,"&r");
						}

					if(typesum>0)
						{
						strcat(story,"   Further details are sketchy, but police sources suggest that the LCS ");
						strcat(story,"engaged in ");
						if(crime[CRIME_KILLEDSOMEBODY])
							{
							strcat(story,"murder");
							if(typesum>=3)strcat(story,", ");
							else if(typesum==2)strcat(story," and ");
							typesum--;
							}
						if(crime[CRIME_ATTACKED_MISTAKE])
							{
							strcat(story,"violence");
							if(typesum>=3)strcat(story,", ");
							else if(typesum==2)strcat(story," and ");
							typesum--;
							}
						if(crime[CRIME_ATTACKED])
							{
							if(crime[CRIME_ATTACKED_MISTAKE])strcat(story,"more violence");
							else strcat(story,"violence");
							if(typesum>=3)strcat(story,", ");
							else if(typesum==2)strcat(story," and ");
							typesum--;
							}
						if(crime[CRIME_STOLEGROUND])
							{
							strcat(story,"theft");
							if(typesum>=3)strcat(story,", ");
							else if(typesum==2)strcat(story," and ");
							typesum--;
							}
						if(crime[CRIME_FREE_RABBITS]||crime[CRIME_FREE_BEASTS])
							{
							strcat(story,"tampering with lab animals");
							if(typesum>=3)strcat(story,", ");
							else if(typesum==2)strcat(story," and ");
							typesum--;
							}
						if(crime[CRIME_BREAK_SWEATSHOP]||crime[CRIME_BREAK_FACTORY])
							{
							strcat(story,"destruction of private property");
							if(typesum>=3)strcat(story,", ");
							else if(typesum==2)strcat(story," and ");
							typesum--;
							}
						if(crime[CRIME_BROKEDOWNDOOR])
							{
							strcat(story,"breaking and entering");
							if(typesum>=3)strcat(story,", ");
							else if(typesum==2)strcat(story," and ");
							typesum--;
							}
						if(crime[CRIME_UNLOCKEDDOOR])
							{
							strcat(story,"unlawful entry");
							if(typesum>=3)strcat(story,", ");
							else if(typesum==2)strcat(story," and ");
							typesum--;
							}
						strcat(story,".&r");
						}

					if(crime[CRIME_CARCHASE])
						{
						strcat(story,"   It is known that there was a high-speed chase ");
						strcat(story,"following the incident.  ");

						if(crime[CRIME_CARCRASH])
							{
							if(crime[CRIME_CARCRASH]>1)
								{
								char num[20];
								itoa(crime[CRIME_CARCRASH],num,10);
								strcat(story," vehicles crashed.");
								}
							else strcat(story,"One vehicle crashed.");
							strcat(story,"Details about injuries were not released.  ");
							}

						if(crime[CRIME_FOOTCHASE])
							{
							strcat(story,"There was also a foot chase when the suspect or suspects bailed out after the high-speed pursuit.  ");
							}
						strcat(story,"&r");
						}

					break;
					}
				}

			generatefiller(story,200);
			displaynewsstory(story,storyx_s,storyx_e,y);

			newscherrybusted=1;
			break;
			}
		case NEWSSTORY_MASSACRE:
			{
			int y=3;
			if(ns.page==1)
				{
				y=21;
				displaycenterednewsfont("MYSTERIOUS",5);
				displaycenterednewsfont("MASSACRE",13);
				}

			strcpy(story,lcityname);
			strcat(story," - ");
			if(ns.crime[1]>2)
				{
				char num[20];
				itoa(ns.crime[1],num,10);
				strcat(story,num);
				strcat(story," bodies were ");
				}
			else if(ns.crime[1]>1)strcat(story," Two bodies were ");
			else strcat(story," A body was ");
			strcat(story," found in the ");
			strcat(story,location[ns.loc]->name);
			strcat(story," yesterday.  According to a spokesperson for ");
			strcat(story,"the police department, the matter is under investigation as a homicide.");
			strcat(story,"&r");
			strcat(story,"   Privately, sources in the department confide that there aren't any leads.  ");
			strcat(story,"According to one person familiar with the case, \"");
			switch(ns.crime[0])
				{
				case SIEGE_CIA:
					if(ns.crime[1]>1)strcat(story,"The bodies had no faces or ");
					else strcat(story,"The body had no face or ");
					strcat(story,"fingerprints.  Like, it was all smooth.  ");
					strcat(story,"D*mnd*st thing I've ever seen");
					break;
				case SIEGE_POLICE:
				case SIEGE_HICKS:
					strcat(story,"Burned...  stabbed with, maybe, pitchforks.  There may have been bite marks.  Nothing recognizable left");
					break;
				case SIEGE_CORPORATE:
					strcat(story,"It was execution style.  Professional.  We've got nothing");
					break;
				}
			strcat(story,".\"");
			strcat(story,"&r");

			generatefiller(story,200);
			displaynewsstory(story,storyx_s,storyx_e,y);
			break;
			}
		case NEWSSTORY_KIDNAPREPORT:
			{
			int y=2;
			if(ns.page==1)
				{
				y=21;
				switch(ns.cr->type)
					{
					case CREATURE_CORPORATE_CEO:
						displaycenterednewsfont("CEO",5);
						displaycenterednewsfont("KIDNAPPED",13);break;
					case CREATURE_RADIOPERSONALITY:
						displaycenterednewsfont("RADIO HOST",5);
						displaycenterednewsfont("KIDNAPPED",13);break;
					case CREATURE_NEWSANCHOR:
						displaycenterednewsfont("NEWS ANCHOR",5);
						displaycenterednewsfont("KIDNAPPED",13);break;
					case CREATURE_SCIENTIST_EMINENT:
						displaycenterednewsfont("SCIENTIST",5);
						displaycenterednewsfont("KIDNAPPED",13);break;
					case CREATURE_JUDGE_CONSERVATIVE:
						displaycenterednewsfont("JUDGE",5);
						displaycenterednewsfont("KIDNAPPED",13);break;
					default:
						displaycenterednewsfont("PERSON",5);
						displaycenterednewsfont("KIDNAPPED",13);break;
					}
				}

			strcpy(story,lcityname);
			strcat(story," - The disappearance of ");
			strcat(story,ns.cr->propername);
			strcat(story," is now considered a kidnapping, ");
			strcat(story,"accorded to a police spokesperson.");
			strcat(story,"&r");

			char dstr[200],dstr2[200];
			strcat(story,"   ");
			firstname(dstr);lastname(dstr2);
			strcat(story,dstr);
			strcat(story," ");
			strcat(story,dstr2);
			strcat(story,", speaking on behalf of the police department, stated ");
			strcat(story,"\"We now believe that ");
			strcat(story,ns.cr->propername);
			strcat(story," was taken ");
			char num[20];
			itoa(ns.cr->joindays-1,num,10);
			strcat(story,num);
			strcat(story," days ago, by a person or persons as yet undetermined.  ");
			strcat(story,"We have several leads and are confident that we will ");
			strcat(story,"bring ");
			strcat(story,ns.cr->propername);
			strcat(story," back home and bring the kidnappers to justice.  ");
			strcat(story,"As the investigation is ongoing, I cannot be more specific at this time.  ");
			strcat(story,"To the citizens, please contact the department if you have any additional information.");
			strcat(story,"\"");
			strcat(story,"&r");
			strcat(story,"   According to sources, ");
			strcat(story,ns.cr->propername);
			strcat(story,"'s last known location was the ");
			strcat(story,location[ns.cr->worklocation]->name);
			strcat(story,".  Police were seen searching the surrounding area yesterday.");
			strcat(story,"&r");

			generatefiller(story,200);
			displaynewsstory(story,storyx_s,storyx_e,y);
			break;
			}
		}

	int c;
	do
		{
		refresh();
		c=getch();
		translategetch(c);
		}while(c!=27&&c!=10&&c!=32);
}

void loadgraphics(void)
{
	unsigned long picnum,dimx,dimy;

	DWORD numbytes;
	HANDLE h;

	h=LCSCreateFile("art\\largecap.cpc", LCSIO_READ);
	
	if(h!=NULL)
		{
		ReadFile(h,&picnum,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&dimx,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&dimy,sizeof(unsigned long),&numbytes,NULL);
		for(int p=0;p<picnum;p++)
			{
			for(int x=0;x<dimx;x++)
				{
				for(int y=0;y<dimy;y++)
					{
					ReadFile(h,&bigletters[p][x][y][0],sizeof(unsigned char)*4,&numbytes,NULL);
					}
				}
			}
		CloseHandle(h);
		}

	h=LCSCreateFile("art\\newstops.cpc", LCSIO_READ);
	if(h!=NULL)
		{
		ReadFile(h,&picnum,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&dimx,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&dimy,sizeof(unsigned long),&numbytes,NULL);
		for(int p=0;p<picnum;p++)
			{
			for(int x=0;x<dimx;x++)
				{
				for(int y=0;y<dimy;y++)
					{
					ReadFile(h,&newstops[p][x][y][0],sizeof(unsigned char)*4,&numbytes,NULL);
					}
				}
			}
		CloseHandle(h);
		}

	h=LCSCreateFile("art\\newspic.cpc", LCSIO_READ);
	
	if(h!=NULL)
		{
		ReadFile(h,&picnum,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&dimx,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&dimy,sizeof(unsigned long),&numbytes,NULL);
		for(int p=0;p<picnum;p++)
			{
			for(int x=0;x<dimx;x++)
				{
				for(int y=0;y<dimy;y++)
					{
					ReadFile(h,&newspic[p][x][y][0],sizeof(unsigned char)*4,&numbytes,NULL);
					}
				}
			}
		CloseHandle(h);
		}
}

void displaycenterednewsfont(char *str,int y)
{
	int width=-1;
	int s;
	for(s=0;s<strlen(str);s++)
		{
		if(str[s]>='A'&&str[s]<='Z')width+=6;
		else if(str[s]=='\'')width+=4;
		else width+=3;
		}

	int x=39-width/2;

	for(s=0;s<strlen(str);s++)
		{
		if((str[s]>='A'&&str[s]<='Z')||str[s]=='\'')
			{
			int p;
			if(str[s]>='A'&&str[s]<='Z')p=str[s]-'A';
			else p=26;
			int lim=6;
			if(str[s]=='\'')lim=4;
			if(s==strlen(str)-1)lim--;
			for(int x2=0;x2<lim;x2++)
				{
				for(int y2=0;y2<7;y2++)
					{
					move(y+y2,x+x2);
					if(x2==5)
						{
						set_color(COLOR_WHITE,COLOR_BLACK,0);
						addch(CH_FULL_BLOCK);
						}
					else
						{
						set_color(bigletters[p][x2][y2][1],
							bigletters[p][x2][y2][2],
							bigletters[p][x2][y2][3]);
						addch(translateGraphicsChar(bigletters[p][x2][y2][0]));
						}
					}
				}
			x+=lim;
			}
		else
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			for(int x2=0;x2<3;x2++)
				{
				for(int y2=0;y2<7;y2++)
					{
					move(y+y2,x+x2);
					addch(CH_FULL_BLOCK);
					}
				}
			x+=3;
			}
		}
}

void displaycenteredsmallnews(char *str,int y)
{
	int x=39-((strlen(str)-1)>>1);
	move(y,x);
	set_color(COLOR_BLACK,COLOR_WHITE,0);
	addstr(str);
}

void displaynewspicture(int p,int y)
{
	for(int x2=0;x2<78;x2++)
		{
		for(int y2=0;y2<15;y2++)
			{
			if(y+y2>24)break;
			move(y+y2,1+x2);
			set_color(newspic[p][x2][y2][1],
				newspic[p][x2][y2][2],
				newspic[p][x2][y2][3]);
			addch(translateGraphicsChar(newspic[p][x2][y2][0]));
			}
		}
}

void constructeventstory(char *story,short view,char positive)
{
	strcpy(story,"");

	if(positive)
		{
		switch(view)
			{
			case VIEW_ABORTION:
				{
				char str[200],str2[200];
				char gen[50];
				cityname(story);
				strcat(story," - ");
				strcat(story,"A doctor that routinely performed abortions was ruthlessly ");
				strcat(story,"gunned down outside of the ");
				lastname(str);strcat(story,str);
				strcat(story," CLINIC yesterday.  ");
				strcat(story,"Dr. ");
				char dstr[200],dstr2[200];
				firstname(dstr);lastname(dstr2);
				strcat(story,dstr);
				strcat(story," ");
				strcat(story,dstr2);
				char gn=LCSrandom(2);
				switch(gn)
					{
					case 0:strcpy(gen,"his");break;
					case 1:strcpy(gen,"her");break;
					}
				strcat(story," was walking to ");strcat(story,gen);
				strcat(story," car when, according to police reports, ");
				strcat(story,"shots were fired from a nearby vehicle.  ");
				strcat(story,dstr2);
				strcat(story," was hit ");
				char num[20];
				itoa(LCSrandom(15)+3,num,10);
				strcat(story,num);
				strcat(story," times and died immediately in the parking lot.  ");
				strcat(story,"The suspected shooter, ");
				firstname(str);strcat(story,str);
				strcat(story," ");
				lastname(str2);strcat(story,str2);
				strcat(story,", is in custody.&r");
				strcat(story,"   Witnesses report that ");
				strcat(story,str2);
				strcat(story," remained at the scene after the shooting, screaming ");
				strcat(story,"verses of the Bible at the stunned onlookers.  Someone ");
				strcat(story,"called the police on a cellphone and they arrived shortly thereafter.  ");
				strcat(story,str2);
				strcat(story," surrendered without a struggle, reportedly saying that God's work ");
				strcat(story,"had been completed.&r");
				strcat(story,"   ");
				strcat(story,dstr2);
				strcat(story," is survived by ");
				strcat(story,gen);
				strcat(story," ");
				char spouse=LCSrandom(2);
				if(law[LAW_GAY]<=1)
					{
					spouse=1-gn;
					}
				switch(spouse)
					{
					case 0:strcat(story,"husband");break;
					case 1:strcat(story,"wife");break;
					}
				strcat(story," and ");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"two");break;
					case 1:strcat(story,"three");break;
					case 2:strcat(story,"five");break;
					}
				strcat(story," children.&r");
				break;
				}
			case VIEW_GAY:
				{
				cityname(story);
				strcat(story," - ");
				char dstr[200],dstr2[200];
				firstname(dstr);lastname(dstr2);
				strcat(story,dstr);
				strcat(story," ");
				strcat(story,dstr2);
				if(law[LAW_GAY]<=-1)strcat(story,", a known homosexual, was ");
				else strcat(story,", a homosexual, was ");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"dragged to death behind a pickup truck");break;
					case 1:strcat(story,"burned alive");break;
					case 2:strcat(story,"beaten to death");break;
					}
				strcat(story," here yesterday.  ");
				strcat(story,"A police spokesperson reported that four suspects ");
				strcat(story,"were apprehended after a high speed chase.  Their names ");
				strcat(story,"have not yet been released.");
				strcat(story,"&r");
				strcat(story,"   Witnesses of the freeway chase described the pickup of the alleged ");
				strcat(story,"murderers swerving wildly, ");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"throwing beer bottles");break;
					case 1:strcat(story,"urinating out the window");break;
					case 2:strcat(story,"taking swipes");break;
					}
				strcat(story," at the pursuing police cruisers.  ");
				strcat(story,"The chase ended when ");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"the suspects ran out of gas, ");break;
					case 1:strcat(story,"the suspects collided with a manure truck, ");break;
					case 2:strcat(story,"the suspects veered into a ditch, ");break;
					}
				strcat(story,"at which point they were taken into custody.  Nobody was seriously injured during the incident.");
				strcat(story,"&r");
				strcat(story,"   Authorities have stated that they will vigorously ");
				strcat(story,"prosecute this case as a hate crime, due to the ");
				strcat(story,"aggravated nature of the offense");
				if(law[LAW_GAY]==-2)strcat(story,", even though being gay is deviant, as we all know.");
				else strcat(story,".");
				strcat(story,"&r");
				break;
				}
			case VIEW_DEATHPENALTY:
				{
				strcat(story,"Texas - ");
				strcat(story,"An innocent citizen has been put to death in the electric chair.  ");
				char dstr[200],dstr2[200],dstr3[200];
				firstname(dstr);firstname(dstr2);lastname(dstr3);
				strcat(story,dstr);
				strcat(story," ");
				strcat(story,dstr2);
				strcat(story," ");
				strcat(story,dstr3);
				strcat(story," was pronounced dead at 7:06PM yesterday at the ");
				char jstr[200];
				lastname(jstr);
				strcat(story,jstr);
				strcat(story," Correctional Facility.&r");
				
				strcat(story,"   ");
				strcat(story,dstr3);
				strcat(story," was convicted in ");
				char num[20];
				itoa(year-LCSrandom(11)-10,num,10);
				strcat(story,num);
				strcat(story," of thirteen serial murders.  ");
				strcat(story,"Since then, numerous pieces of exculpatory evidence ");
				strcat(story,"have been produced, including ");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"a confession from another convict.  ");break;
					case 1:strcat(story,"a battery of negative DNA tests.  ");break;
					case 2:
						strcat(story,"an admission from a former prosecutor that ");
						strcat(story,dstr3);
						strcat(story," was framed.  ");
						break;
					}
				strcat(story,"The state still went through with the execution, with a ");
				strcat(story,"spokesperson for the governor saying, ");
				strcat(story,"\"");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"Let's not forget the convict is colored.  You know how their kind are");break;
					case 1:
						strcat(story,"The convict is always referred to by three names.  ");
						strcat(story,"Assassin, serial killer, either way -- guilty.  ");
						strcat(story,"End of story");
						break;
					case 2:strcat(story,"The family wants closure.  We don't have time for another trial");break;
					}
				strcat(story,".\"");
				strcat(story,"&r");
				strcat(story,"   Candlelight vigils were held throughout the country last night during the execution, ");
				strcat(story,"and more events are expected this evening.  If there is a bright side to be found from this ");
				strcat(story,"tragedy, it will be that our nation is now evaluating the ease with which people ");
				strcat(story,"can be put to death in this country.");
				strcat(story,"&r");
				break;
				}
			case VIEW_PRISONS:
				{
				cityname(story);
				strcat(story," - A former prisoner has written a book describing in horrifying ");
				strcat(story,"detail what goes on behind bars.  ");
				strcat(story,"Although popular culture has used, or perhaps overused, the ");
				strcat(story,"prison theme lately in its offerings for mass consumption, rarely ");
				strcat(story,"have these works been as poignant as ");
				char dstr[200],dstr2[200];
				firstname(dstr);lastname(dstr2);
				strcat(story,dstr);
				strcat(story," ");
				strcat(story,dstr2);
				strcat(story,"'s new tour-de-force, _");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"Nightmare");break;
					case 1:strcat(story,"Primal");break;
					case 2:strcat(story,"Animal");break;
					case 3:strcat(story,"American");break;
					case 4:strcat(story,"Solitary");break;
					}
				strcat(story,"_");
				switch(LCSrandom(7))
					{
					case 0:strcat(story,"Packer");break;
					case 1:strcat(story,"Soap");break;
					case 2:strcat(story,"Punk");break;
					case 3:strcat(story,"Kid");break;
					case 4:strcat(story,"Cell");break;
					case 5:strcat(story,"Shank");break;
					case 6:strcat(story,"Lockdown");break;
					case 7:strcat(story,"Buttlord");break;
					}
				strcat(story,"_.&r");
				strcat(story,"   Take this excerpt, \"");

				strcat(story,"The steel bars grated forward in their rails, ");
				strcat(story,"coming to a halt with a deafening clang that said it all -- ");
				strcat(story,"I was trapped with them now.  There were three, looking me over ");
				strcat(story,"with dark glares of bare lust, as football players might stare at a stupified, drunken, helpless teenager.  ");
				strcat(story,"My shank's under the mattress.  Better to be a man and fight or a punk and let them take it?  ");
				strcat(story,"Maybe lose an eye the one way, maybe catch AIDS the other.  A helluva choice, ");
				strcat(story,"and I would only have a few seconds before they made it for me");
				strcat(story,".\"");
				strcat(story,"&r");
				break;
				}
			case VIEW_INTELLIGENCE:
				{
				strcat(story,"Washington D.C. - The FBI might be keeping tabs on you.  ");
				strcat(story,"This newspaper yesterday received a collection of files from a source in the Federal Bureau of Investigations.  ");
				strcat(story,"The files contain information on which people have been attending demonstrations, organizing ");
				strcat(story,"unions, working for liberal organizations -- even ");
				switch(LCSrandom(2))
					{
					case 0:strcat(story,"buying music with 'Explicit Lyrics' labels.");break;
					case 1:strcat(story,"helping homeless people");break;
					}
				strcat(story,".");
				strcat(story,"&r");
				strcat(story,"   More disturbingly, the files make reference to a plan to ");
				strcat(story,"\"deal with the undesirables\", although this phrase is not clarified.  ");
				strcat(story,"&r");
				strcat(story,"   The FBI refused to comment initially, but when confronted with the information, ");
				strcat(story,"a spokesperson stated, \"");
				strcat(story,"Well, you know, there's privacy, and there's privacy.  ");
				strcat(story,"It might be a bit presumptive to assume that ");
				strcat(story,"these files deal with the one and not the other.  ");
				strcat(story,"You think about that before you continue slanging accusations");
				strcat(story,".\"");
				strcat(story,"&r");
				break;
				}
			case VIEW_FREESPEECH:
				{
				cityname(story);
				strcat(story," - A children's story has been removed from libraries here after ");
				strcat(story,"the city bowed to pressure from religious groups.");
				strcat(story,"&r");
				strcat(story,"   The book, ");
				strcat(story,"_");
				char dstr[200],nstr[200];
				firstname(nstr);
				strcat(story,nstr);
				strcat(story,"_");
				lastname(dstr);
				strcat(story,dstr);
				strcat(story,"_and_the_");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"Mysterious");break;
					case 1:strcat(story,"Magical");break;
					case 2:strcat(story,"Golden");break;
					case 3:strcat(story,"Invisible");break;
					case 4:strcat(story,"Wondrous");break;
					}
				strcat(story,"_");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"Thing");break;
					case 1:strcat(story,"Stuff");break;
					case 2:strcat(story,"Object");break;
					case 3:strcat(story,"Whatever");break;
					case 4:strcat(story,"Something");break;
					}
				strcat(story,"_, is the third in an immensely popular series by ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"British");break;
					case 1:strcat(story,"Indian");break;
					case 2:strcat(story,"Chinese");break;
					case 3:strcat(story,"Rwandan");break;
					case 4:strcat(story,"Palestinian");break;
					}
				strcat(story," author ");
				char c[2];
				c[0]='A'+LCSrandom(26);
				c[1]='\x0';
				strcat(story,c);
				strcat(story,".");
				c[0]='A'+LCSrandom(26);
				c[1]='\x0';
				strcat(story,c);
				strcat(story,". ");
				lastname(dstr);
				strcat(story,dstr);
				strcat(story,".  ");
				strcat(story,"Although the series is adored by children worldwide, ");
				strcat(story,"some conservatives feel that the books ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"glorify Satan worship and are spawned by demons from the pit.  ");break;
					case 1:strcat(story,"teach children to kill their parents and hate life.  ");break;
					case 2:strcat(story,"cause violence in schools and are a gateway to cocaine use.  ");break;
					case 3:strcat(story,"breed demonic thoughts that manifest themselves as dreams of murder.  ");break;
					case 4:strcat(story,"contain step-by-step instructions to summon the Prince of Darkness.  ");break;
					}
				strcat(story,"In their complaint, the groups cited an incident involving ");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"a child that swore in class");break;
					case 1:strcat(story,"a child that said a magic spell at her parents");break;
					case 2:strcat(story,"a child that pushed her older sister");break;
					}
				strcat(story," as key evidence of the dark nature of the book.");
				strcat(story,"&r");
				strcat(story,"   When the decision to ban the book was announced yesterday, ");
				strcat(story,"many area children spontoneously broke into tears.  One child was ");
				strcat(story,"heard saying, \"");
				switch(LCSrandom(2))
					{
					case 0:
						strcat(story,"Mamma, is ");
						strcat(story,nstr);
						strcat(story," dead?");
						break;
					case 1:
						strcat(story,"Mamma, why did they kill ");
						strcat(story,nstr);
						strcat(story,"?");
						break;
					}
				strcat(story,"\"");
				strcat(story,"&r");
				break;
				}
			case VIEW_JUSTICES:
				{
				cityname(story);
				strcat(story," - Conservative federal judge ");
				char dstr[200],dstr2[200];
				firstname(dstr);lastname(dstr2);
				strcat(story,dstr);
				strcat(story," ");
				strcat(story,dstr2);
				strcat(story," has resigned in disgrace after being caught with a prostitute.");
				strcat(story,"&r");
				strcat(story,"   ");
				strcat(story,dstr2);
				strcat(story,", who once ");
				switch(LCSrandom(2))
					{
					case 0:strcat(story,"defied the federal government by putting a Ten Commandments monument in the local federal building");break;
					case 1:strcat(story,"stated that, \"Segregation wasn't the bad idea everybody makes it out to be these days\"");break;
					}
				strcat(story,", was found with ");
				char pstr[200],pstr2[200];
				firstname(pstr);lastname(pstr2);
				strcat(story,pstr);
				strcat(story," ");
				strcat(story,pstr2);
				strcat(story," last week in a hotel during a police sting operation.  ");
				strcat(story,"According to sources familiar with the particulars, ");
				strcat(story,"when police broke into the hotel room they saw ");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"\"the most perverse and spine-tingling debauchery imaginable, at least with only two people.\"");break;
					case 1:strcat(story,"the judge relieving himself on the prostitute.");break;
					case 2:strcat(story,"the prostitute hollering like a cowboy astride the judge.");break;
					}
				strcat(story,"  ");
				strcat(story,pstr2);
				strcat(story," reportedly offered ");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"the arresting officers money");break;
					case 1:strcat(story,"to let the officers join in");break;
					case 2:strcat(story,"the arresting officers \"favors\"");break;
					}
				strcat(story," in exchange for their silence.");
				strcat(story,"&r");
				strcat(story,"   ");
				strcat(story,dstr2);
				strcat(story," could not be reached for comment, although an aid stated that ");
				strcat(story,"the judge would be going on a Bible retreat for a few weeks to ");
				strcat(story,"\"Make things right with the Almighty Father.\"  ");
				strcat(story,"&r");
				break;
				}
			case VIEW_AMRADIO:
				{
				cityname(story);
				strcat(story," - Well-known AM radio personality ");
				char dstr[200],dstr2[200];
				firstname(dstr);lastname(dstr2);
				strcat(story,dstr);
				strcat(story," ");
				strcat(story,dstr2);
				strcat(story," went off for fifteen minutes in an inexplicable rant ");
				strcat(story,"two nights ago during the syndicated radio program \"");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"Straight");break;
					case 1:strcat(story,"Real");break;
					case 2:strcat(story,"True");break;
					}
				strcat(story," ");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"Talk");break;
					case 1:strcat(story,"Chat");break;
					case 2:strcat(story,"Discussion");break;
					}
				strcat(story,"\".");
				strcat(story,"&r");
				strcat(story,"   ");
				strcat(story,dstr2);
				strcat(story,"'s monologue for the evening began the way that fans ");
				strcat(story,"had come to expect, with attacks on the \"liberal media establishment\" and ");
				strcat(story,"the \"elite liberal agenda\".  But when the radio icon said, \"");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"and the Grays are going to take over the planet in the End Times");break;
					case 1:strcat(story,"a liberal chupacabra will suck the blood from us like a goat, a goat!, a goat!");break;
					case 2:strcat(story,"I feel translucent rods passing through my body...  it's like making love to the future");break;
					}
				strcat(story,"\", a former fan of the show, ");
				char nstr[200],nstr2[200];
				firstname(nstr);lastname(nstr2);
				strcat(story,nstr);
				strcat(story," ");
				strcat(story,nstr2);
				strcat(story,", knew that \"");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"my old hero");break;
					case 1:strcat(story,"my old idol");break;
					case 2:strcat(story,"the legend");break;
					}
				strcat(story," had ");
				switch(LCSrandom(3))
					{
					case 0:
						strcat(story,"lost ");
						switch(LCSrandom(2))
							{
							case 0:strcat(story,"his");break;
							case 1:strcat(story,"her");break;
							}
						strcat(story," g*dd*mn mind");
						break;
					case 1:strcat(story,"maybe gone a little off the deep end");break;
					case 2:strcat(story,"probably been listening to Art Bell in the next studio a little too long");break;
					}
				strcat(story,".  After that, it just got worse and worse.\"");
				strcat(story,"&r");
				strcat(story,"   ");
				strcat(story,dstr2);
				strcat(story," issued an apology later in the program, but ");
				strcat(story,"the damage might already be done.  ");
				strcat(story,"According to a poll completed yesterday, ");
				strcat(story,"fully half of the host's most loyal supporters ");
				strcat(story,"have decided to leave the program for saner ");
				strcat(story,"pastures.  Of these, many said that they would be switching over ");
				strcat(story,"to the FM band.");
				strcat(story,"&r");
				break;
				}
			}
		}
	else
		{
		switch(view)
			{
			case VIEW_DEATHPENALTY:
				{
				cityname(story);
				strcat(story," - Perhaps parents can rest easier tonight.  ");
				strcat(story,"The authorities have apprehended their primary suspect in the ");
				strcat(story,"string of brutal child killings that has kept everyone in the area on edge, ");
				strcat(story,"according to a spokesperson for the police department here.  ");
				char dstr[200],dstr2[200],dstr3[200];
				firstname(dstr);firstname(dstr2);lastname(dstr3);
				strcat(story,dstr);
				strcat(story," ");
				strcat(story,dstr2);
				strcat(story," ");
				strcat(story,dstr3);
				strcat(story," was detained yesterday afternoon, reportedly in possession of ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"pieces of another victim");break;
					case 1:strcat(story,"bloody toys");break;
					case 2:strcat(story,"a child's clothing stained with DNA evidence");break;
					case 3:strcat(story,"seven junior high school yearbooks");break;
					case 4:strcat(story,"two small backpacks");break;
					}
				strcat(story,".  Over twenty children in the past two years have gone missing, ");
				strcat(story,"only to turn up later dead and ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"carved with satanic symbols");break;
					case 1:strcat(story,"sexually mutilated");break;
					case 2:strcat(story,"missing all of their teeth");break;
					case 3:strcat(story,"missing all of their fingers");break;
					case 4:strcat(story,"without eyes");break;
					}
				strcat(story,".  Sources say that the police got a break in the case when ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"a victim called 911 just prior to being slain while still on the phone");break;
					case 1:strcat(story,"the suspect allegedly carved an address into one of the bodies");break;
					case 2:strcat(story,"an eye witness allegedly spotted the suspect luring a victim into a car");break;
					case 3:strcat(story,"a blood trail was found on a road that led them to the suspect's car trunk");break;
					case 4:strcat(story,"they found a victim in a ditch, still clinging to life");break;
					}
				strcat(story,".");
				strcat(story,"&r");
				strcat(story,"   The district attorney's office has already repeatedly said it will be ");
				strcat(story,"seeking the death penalty in this case.");
				strcat(story,"&r");
				break;
				}
			case VIEW_ANIMALRESEARCH:
				{
				cityname(story);
				strcat(story," - Researchers here report that they have discovered an amazing new wonder drug.  ");
				strcat(story,"Called ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"Anal");break;
					case 1:strcat(story,"Colo");break;
					case 2:strcat(story,"Lacta");break;
					case 3:strcat(story,"Pur");break;
					case 4:strcat(story,"Loba");break;
					}
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"nephrin");break;
					case 1:strcat(story,"tax");break;
					case 2:strcat(story,"zac");break;
					case 3:strcat(story,"thium");break;
					case 4:strcat(story,"drene");break;
					}
				strcat(story,", the drug apparently ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"boosts intelligence in chimpanzees");break;
					case 1:strcat(story,"corrects erectile dysfunction in chimpanzees");break;
					case 2:strcat(story,"allows chimpanzees to move blocks with their minds");break;
					case 3:strcat(story,"allows chimpanzees to fly short distances");break;
					case 4:strcat(story,"increases the attention span of young chimpanzees");break;
					}
				strcat(story,".  ");
				strcat(story,"&r");
				strcat(story,"   Along with bonobos, chimpanzees are our closest cousins");
				if(law[LAW_FREESPEECH]==-2)strcat(story,", at least according to the now-discredited theory of evolution");
				else strcat(story,".  ");
				strcat(story,"Fielding questions about the ethics of their experiments from reporters during a press conference yesterday, ");
				strcat(story,"a spokesperson for the research team stated that, \"It really isn't so bad as all that.  Chimpanzees are very resilient creatures.  ");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"The ones that survived are all doing very well");break;
					case 1:strcat(story,"They hardly notice when you drill their brains out, if you're fast");break;
					case 2:strcat(story,"When we started muffling the screams of our subjects, the other chimps all calmed down quite a bit");break;
					}
				strcat(story,".  We have a very experienced research team.  ");
				strcat(story,"While we understand your concerns, any worries are entirely unfounded.  ");
				strcat(story,"I think the media should be focusing on the enormous benefits of this drug.");
				strcat(story,"\"");
				strcat(story,"&r");
				strcat(story,"   The first phase of human trials is slated to begin in a few months.");
				strcat(story,"&r");
				break;
				}
			case VIEW_PRISONS:
				{
				cityname(story);
				strcat(story," - The hostage crisis at the ");
				char jstr[200];
				lastname(jstr);
				strcat(story,jstr);
				strcat(story," Correctional Facility ended tragically yesterday with the ");
				strcat(story,"death of both the prison guard being held hostage and ");
				switch(LCSrandom(2))
					{
					case 0:strcat(story,"his");break;
					case 1:strcat(story,"her");break;
					}
				strcat(story," captor.");
				strcat(story,"&r");
				strcat(story,"   Two weeks ago, convicted rapist ");
				char dstr[200],dstr2[200];
				firstname(dstr);lastname(dstr2);
				strcat(story,dstr);
				strcat(story," ");
				strcat(story,dstr2);
				strcat(story,", an inmate at ");
				strcat(story,jstr);
				strcat(story,", overpowered ");
				char gstr[200],gstr2[200];
				firstname(gstr);lastname(gstr2);
				strcat(story,gstr);
				strcat(story," ");
				strcat(story,gstr2);
				strcat(story," and barricaded ");
				switch(LCSrandom(2))
					{
					case 0:strcat(story,"himself");break;
					case 1:strcat(story,"herself");break;
					}
				strcat(story," with the guard in a prison tower.  ");
				strcat(story,"Authorities locked down the prison and ");
				strcat(story,"attempted to negotiate by phone for 13 days, ");
				strcat(story,"but talks were cut short when ");
				strcat(story,dstr2);
				strcat(story," reportedly screamed into the receiver \"");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"Ah, f*ck this sh*t.  This punk b*tch is f*ckin' dead!");break;
					case 1:strcat(story,"F*ck a m*th*f*ck*n' bull.  I'm killin' this pig sh*t.");break;
					case 2:strcat(story,"Why the f*ck am I talkin' to you?  I'd rather kill this pig.");break;
					}
				strcat(story,"\"");
				strcat(story,"  The tower was breached in an attempt to reach ");
				strcat(story,"the hostage, but ");
				strcat(story,dstr2);
				strcat(story," had already ");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"slit the guard's throat with a shank");break;
					case 1:strcat(story,"strangled the guard to death with a knotted bed sheet");break;
					case 2:strcat(story,"chewed out the guard's throat");break;
					}
				strcat(story,".  The prisoner was beaten to death while ");
				strcat(story,"\"resisting capture\", according to a prison spokesperson.");
				strcat(story,"&r");
				break;
				}
			case VIEW_INTELLIGENCE:
				{
				strcat(story,"Washington D.C. - The CIA announced yesterday that it has averted a terror attack that ");
				strcat(story,"would have occurred on American soil.");
				strcat(story,"&r");
				strcat(story,"   According to a spokesperson for the agency, ");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"white supremacists");break;
					case 1:strcat(story,"Islamic fundamentalists");break;
					case 2:strcat(story,"outcast goths from a suburban highschool");break;
					}
				strcat(story," planned to ");
				switch(LCSrandom(9))
					{
					case 0:strcat(story,"fly planes into skyscrapers");break;
					case 1:strcat(story,"detonate a fertilizer bomb at a federal building");break;
					case 2:strcat(story,"ram a motorboat loaded with explosives into a warship");break;
					case 3:strcat(story,"detonate explosives on a school bus");break;
					case 4:strcat(story,"blow out a section of a major bridge");break;
					case 5:strcat(story,"kidnap the president");break;
					case 6:strcat(story,"assassinate the president");break;
					case 7:strcat(story,"destroy the Capitol Building");break;
					case 8:strcat(story,"detonate a nuclear bomb in New York");break;
					}
				strcat(story,".  However, intelligence garnered from deep within the mysterious ");
				strcat(story,"terrorist organization allowed the plot to be foiled just days before it ");
				strcat(story,"was to occur.");
				strcat(story,"&r");
				strcat(story,"   The spokesperson further stated, \"");
				strcat(story,"I won't compromise our sources and methods, but let me just say ");
				strcat(story,"that we are grateful to the Congress and this Administration for ");
				strcat(story,"providing us with the tools we need to neutralize these enemies of ");
				strcat(story,"civilization before they can destroy American families.  ");
				strcat(story,"However, let me also say that there's more that needs to be done.  ");
				strcat(story,"The Head of the Agency will be sending a request to Congress ");
				strcat(story,"for what we feel are the essential tools for combating terrorism in ");
				strcat(story,"this new age.");
				strcat(story,"\"");
				strcat(story,"&r");
				break;
				}
			case VIEW_GENETICS:
				{
				cityname(story);
				strcat(story," - The genetic foods industry staged a major event here yesterday ");
				strcat(story,"to showcase its upcoming products.  Over thirty companies set up ");
				strcat(story,"booths and gave talks to wide-eyed onlookers.");
				strcat(story,"&r");
				strcat(story,"   One such corporation, ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"Altered");break;
					case 1:strcat(story,"Gene-tech");break;
					case 2:strcat(story,"DNA");break;
					case 3:strcat(story,"Proteomic");break;
					case 4:strcat(story,"Genomic");break;
					}
				strcat(story," ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"Foods");break;
					case 1:strcat(story,"Agriculture");break;
					case 2:strcat(story,"Meals");break;
					case 3:strcat(story,"Farming");break;
					case 4:strcat(story,"Living");break;
					}
				strcat(story,", presented their product, \"");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"Mega");break;
					case 1:strcat(story,"Epic");break;
					case 2:strcat(story,"Overlord");break;
					case 3:strcat(story,"Franken");break;
					case 4:strcat(story,"Transcendent");break;
					}
				strcat(story," ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"Rice");break;
					case 1:strcat(story,"Beans");break;
					case 2:strcat(story,"Corn");break;
					case 3:strcat(story,"Wheat");break;
					case 4:strcat(story,"Potatoes");break;
					}
				strcat(story,"\", during an afternoon Power Point presentation.  ");
				strcat(story,"According to the public relations representative speaking, ");
				strcat(story,"this amazing new product actually ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"extends human life by a few minutes every bite");break;
					case 1:strcat(story,"mends split-ends upon digestion.  Hair is also made glossier and thicker");break;
					case 2:strcat(story,"allows people to see in complete darkness");break;
					case 3:strcat(story,"causes a person to slowly attain their optimum weight with repeated use");break;
					case 4:strcat(story,"cures the common cold");break;
					}
				strcat(story,".");
				strcat(story,"&r");
				strcat(story,"   Spokespeople for the GM corporations were universal ");
				strcat(story,"in their dismissal of the criticism which often follows the industry.  ");
				strcat(story,"One in particular said, \"");
				strcat(story,"Look, these products are safe.  That thing about the ");
				switch(LCSrandom(4))
					{
					case 0:strcat(story,"guy going on a killing spree");break;
					case 1:strcat(story,"gal turning blue and exploding");break;
					case 2:strcat(story,"guy speaking in tongues and worshipping Satan");break;
					case 3:strcat(story,"gal having a ruptured intestine");break;
					}
				strcat(story," is just a load of ");
				switch(LCSrandom(4))
					{
					case 0:strcat(story,"hooey");break;
					case 1:strcat(story,"poppycock");break;
					case 2:strcat(story,"horse radish");break;
					case 3:strcat(story,"skunk weed");break;
					}
				strcat(story,".  Would we stake the reputation of our company on unsafe products?  ");
				strcat(story,"No.  That's just ridiculous.  I mean, sure companies have put unsafe products out, ");
				strcat(story,"but the GM industry operates at a higher ethical standard.  That goes without saying.");
				strcat(story,"\"");
				strcat(story,"&r");
				break;
				}
			case VIEW_JUSTICES:
				{
				cityname(story);
				strcat(story," - The conviction of confessed serial killer ");
				char dstr[200],dstr2[200],dstr3[200];
				firstname(dstr);firstname(dstr2);lastname(dstr3);
				strcat(story,dstr);
				strcat(story," ");
				strcat(story,dstr2);
				strcat(story," ");
				strcat(story,dstr3);
				strcat(story," was overturned by a federal judge yesterday.  ");
				strcat(story,"Justice ");
				char jstr[200],jstr2[200];
				firstname(jstr);lastname(jstr2);
				strcat(story,jstr);
				strcat(story," ");
				strcat(story,jstr2);
				strcat(story," of the notoriously liberal circuit of appeals here ");
				strcat(story,"made the decision based on ");
				char gen[20];
				switch(LCSrandom(2))
					{
					case 0:strcpy(gen,"his");break;
					case 1:strcpy(gen,"her");break;
					}
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"ten-year-old eye witness testimony");break;
					case 1:strcat(story,gen);strcat(story," general feeling about police corruption");break;
					case 2:strcat(story,gen);strcat(story," belief that the crimes were a right-wing conspiracy");break;
					case 3:
						strcat(story,gen);
						strcat(story," belief that ");
						strcat(story,dstr3);
						strcat(story," deserved another chance");
						break;
					case 4:
						strcat(story,gen);
						strcat(story," personal philosophy of liberty");break;
					}
				strcat(story,", despite the confession of ");
				strcat(story,dstr3);
				strcat(story,", which even Justice ");
				strcat(story,jstr2);
				strcat(story," grants was not coerced in any way.&r");

				strcat(story,"   Ten years ago, ");
				strcat(story,dstr3);
				strcat(story," was convicted of the now-infamous ");
				char sstr[200];
				lastname(sstr);
				strcat(story,sstr);
				strcat(story," slayings.  ");
				strcat(story,"After an intensive manhunt, ");
				strcat(story,dstr3);
				strcat(story," was found with the murder weapon, ");
				strcat(story,"covered in the victims' blood.  ");
				strcat(story,dstr3);
				strcat(story," confessed and was sentenced to life, saying \"");
				strcat(story,"Thank you for saving me from myself.  ");
				strcat(story,"If I were to be released, I would surely kill again.\"&r");
				strcat(story,"   A spokesperson for the district attorney ");
				strcat(story,"has stated that the case will not be retried, due ");
				strcat(story,"to the current economic doldrums that have left the state ");
				strcat(story,"completely strapped for cash.&r");
				break;
				}
			case VIEW_POLLUTION:
				cityname(story);
				strcat(story," - Pollution might not be so bad after all.  The ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"American");break;
					case 1:strcat(story,"United");break;
					case 2:strcat(story,"Patriot");break;
					case 3:strcat(story,"Family");break;
					case 4:strcat(story,"Children's");break;
					}
				strcat(story," ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"Heritage");break;
					case 1:strcat(story,"Enterprise");break;
					case 2:strcat(story,"Freedom");break;
					case 3:strcat(story,"Liberty");break;
					case 4:strcat(story,"Charity");break;
					}
				strcat(story," ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"Partnership");break;
					case 1:strcat(story,"Institute");break;
					case 2:strcat(story,"Consortium");break;
					case 3:strcat(story,"Forum");break;
					case 4:strcat(story,"Center");break;
					}
				strcat(story," recently released a wide-ranging report detailing recent trends ");
				strcat(story,"and the latest science on the issue.  ");
				strcat(story,"Among the most startling of the think tank's findings is that ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"a modest intake of radiactive waste");break;
					case 1:strcat(story,"a healthy dose of radiation");break;
					case 2:strcat(story,"a bath in raw sewage");break;
					case 3:strcat(story,"watching animals die in oil slicks");break;
					case 4:strcat(story,"inhaling carbon monoxide");break;
					}
				strcat(story," might actually ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"purify the soul");break;
					case 1:strcat(story,"increase test scores");break;
					case 2:strcat(story,"increase a child's attention span");break;
					case 3:strcat(story,"make children behave better");break;
					case 4:strcat(story,"make shy children fit in");break;
					}
				strcat(story,".");
				strcat(story,"&r");
				strcat(story,"   When questioned about the science behind these results, ");
				strcat(story,"a spokesperson stated that, \"");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"Research is complicated, and there are always two ways to think about things");break;
					case 1:strcat(story,"The jury is still out on pollution.  You really have to keep an open mind");break;
					case 2:strcat(story,"They've got their scientists, and we have ours.  The issue of pollution is wide open as it stands today");break;
					}
				strcat(story,".  You have to realize that ");
				switch(LCSrandom(3))
					{
					case 0:strcat(story,"the elitist liberal media often distorts");break;
					case 1:strcat(story,"the vast left-wing education machine often distorts");break;
					case 2:strcat(story,"the fruits, nuts, and flakes of the environmentalist left often distort");break;
					}
				strcat(story," these issues to their own advantage.  ");
				strcat(story,"All we've done is introduced a little clarity into the ongoing debate.  ");
				strcat(story,"Why is there contention on the pollution question?  It's because ");
				strcat(story,"there's work left to be done.  We should study much more ");
				strcat(story,"before we urge any action.  Society really just ");
				strcat(story,"needs to take a breather on this one.  We don't see why there's such a rush to judgement here.  ");
				strcat(story,"&r");
				break;
			case VIEW_CORPORATECULTURE:
				cityname(story);
				strcat(story," - Several major companies have announced ");
				strcat(story,"at a joint news conference here that they ");
				strcat(story,"will be expanding their work forces considerably ");
				strcat(story,"during the next quarter.  Over thirty thousand jobs ");
				strcat(story,"are expected in the first month, with ");
				strcat(story,"tech giant ");
				switch(LCSrandom(10))
					{
					case 0:strcat(story,"Ameri");break;
					case 1:strcat(story,"Gen");break;
					case 2:strcat(story,"Oro");break;
					case 3:strcat(story,"Amelia");break;
					case 4:strcat(story,"Vivo");break;
					case 5:strcat(story,"Benji");break;
					case 6:strcat(story,"Amal");break;
					case 7:strcat(story,"Ply");break;
					case 8:strcat(story,"Seli");break;
					case 9:strcat(story,"Rio");break;
					}
				switch(LCSrandom(10))
					{
					case 0:strcat(story,"tech");break;
					case 1:strcat(story,"com");break;
					case 2:strcat(story,"zap");break;
					case 3:strcat(story,"cor");break;
					case 4:strcat(story,"dyne");break;
					case 5:strcat(story,"bless");break;
					case 6:strcat(story,"chip");break;
					case 7:strcat(story,"co");break;
					case 8:strcat(story,"wire");break;
					case 9:strcat(story,"rex");break;
					}
				strcat(story," increasing its payrolls by over ten thousand workers alone.  ");
				strcat(story,"Given the state of the economy recently and in ");
				strcat(story,"light of the tendency ");
				strcat(story,"of large corporations to export jobs overseas these days, ");
				strcat(story,"this welcome news is bound to be a pleasant surprise to those in the unemployment lines.  ");
				strcat(story,"The markets reportedly responded to the announcement with mild interest, ");
				strcat(story,"although the dampened movement might be expected due to the uncertain ");
				strcat(story,"futures of some of the companies in the tech sector.  On the whole, however, ");
				strcat(story,"analysts suggest that not only does the expansion speak to the health ");
				strcat(story,"of the tech industry but is also indicative of a full economic recover.&r");
				break;
			case VIEW_AMRADIO:
				{
				//THIS ONE IS SHORTER BECAUSE OF DOUBLE HEADLINE
				cityname(story);
				strcat(story," - Infamous FM radio shock jock ");
				char dstr[200],dstr2[200];
				firstname(dstr);lastname(dstr2);
				strcat(story,dstr);
				strcat(story," ");
				strcat(story,dstr2);
				strcat(story," has brought radio entertainment to a new low.  During yesterday's ");
				strcat(story,"broadcast of the program \"");
				strcat(story,dstr);
				strcat(story,"'s ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"Morning");break;
					case 1:strcat(story,"Commuter");break;
					case 2:strcat(story,"Jam");break;
					case 3:strcat(story,"Talk");break;
					case 4:strcat(story,"Radio");break;
					}
				strcat(story," ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"Swamp");break;
					case 1:strcat(story,"Jolt");break;
					case 2:strcat(story,"Club");break;
					case 3:strcat(story,"Show");break;
					case 4:strcat(story,"Fandango");break;
					}
				strcat(story,"\", ");
				strcat(story,dstr2);
				strcat(story," reportedly ");
				switch(LCSrandom(5))
					{
					case 0:strcat(story,"had intercourse");break;
					case 1:strcat(story,"encouraged listeners to call in and relieve themselves");break;
					case 2:strcat(story,"screamed \"f*ck the police those g*odd*mn m*th*f*ck*rs.  I got a f*cking ticket this morning and I'm f*cking p*ss*d as sh*t.\"");break;
					case 3:strcat(story,"breastfed from a lactating woman");break;
					case 4:strcat(story,"masturbated");break;
					}
				strcat(story," on the air.  Although ");
				strcat(story,dstr2);
				strcat(story," later apologized, ");
				strcat(story,"the FCC received thousands of complaints ");
				strcat(story,"from irate listeners.  A spokesperson for the FCC ");
				strcat(story,"stated that the incident is under investigation.");
				strcat(story,"&r");
				}
				break;
			}
		}
}

void displaynewsstory(char *story,short *storyx_s,short *storyx_e,int y)
{
	vector<char *> text;
	vector<char> centered;
	
	int totalwidth;
	int curpos=0;

	int addstrcur;
	char addstring[500];

	char content;
	int cury=y;
	int length;
	char endparagraph=0;
	char iscentered=0;
	unsigned int i=0;

	while(curpos<strlen(story)&&cury<25)
		{
		content=0;
		totalwidth=0;
		addstrcur=0;
		length=storyx_e[cury]-storyx_s[cury]+1;
		if(length==0){cury++;if(endparagraph>0)endparagraph--;continue;}

		for(i=curpos;i<strlen(story);i++)
			{
			if(story[i]=='&'&&story[i+1]!='&')
				{
				i++;
				if(story[i]=='c')
					{
					iscentered=1;
					}
				if(story[i]=='r')
					{
					content=1;
					i++;
					addstrcur+=1;
					addstring[addstrcur-1]=' ';
					addstring[addstrcur]='\x0';
					endparagraph=1;
					break;
					}
				}
			else
				{
				content=1;

				if(story[i]=='&')i++;
				addstring[addstrcur]=story[i];
				addstring[addstrcur+1]='\x0';
				totalwidth++;
				if(totalwidth>length)
					{
					while(story[i]!=' '){i--;addstrcur--;}
					while(story[i]==' ')i++;
					addstring[addstrcur]='\x0';
					break;
					}
				addstrcur++;
				}
			}

		if(i==strlen(story))addstring[addstrcur]='\x0';

		if(strlen(addstring)>0&&content)
			{
			int words=0;
			char silent=1;
			vector<int> spacex;
			for(int s2=0;s2<strlen(addstring);s2++)
				{
				if(addstring[s2]==' ')
					{
					if(!silent)
						{
						silent=1;
						words++;
						spacex.push_back(s2);
						}
					}
				else
					{
					if(silent)
						{
						words++;
						silent=0;
						}
					}
				}

			while(!endparagraph&&words>1&&strlen(addstring)<length&&!iscentered)
				{
				int csp=spacex[LCSrandom(spacex.size())];

				for(int x=0;x<spacex.size();x++)
					{
					if(spacex[x]>csp)spacex[x]++;
					}

				int l=strlen(addstring);
				for(int s=l+1;s>csp;s--)
					{
					addstring[s]=addstring[s-1];
					}
				}

			char *news=new char[strlen(addstring)+1];
			strcpy(news,addstring);
			text.push_back(news);
			centered.push_back(iscentered);
			cury++;
			if(endparagraph>0)
				{
				endparagraph--;
				iscentered=0;
				}
			}

		curpos=i;
		}

	set_color(COLOR_BLACK,COLOR_WHITE,0);
	for(int t=0;t<text.size();t++)
		{
		if(y+t>=25)break;
		if(centered[t])
			{
			move(y+t,((storyx_s[y+t]+storyx_e[y+t])>>1)-((strlen(text[t])-1)>>1));
			}
		else move(y+t,storyx_s[y+t]);
		addstr(text[t]);

		delete[] text[t];
		}
	text.clear();
}

void cityname(char *story)
{
	switch(LCSrandom(20))
		{
		case 0:strcpy(story,"San Francisco, CA");break;
		case 1:strcpy(story,"Boston, MA");break;
		case 2:strcpy(story,"Los Angeles, CA");break;
		case 3:strcpy(story,"Detroit, MC");break;
		case 4:strcpy(story,"Cleveland, OH");break;
		case 5:strcpy(story,"Cincinnati, OH");break;
		case 6:strcpy(story,"New York, NY");break;
		case 7:strcpy(story,"Chicago, IL");break;
		case 8:strcpy(story,"Trenton, NJ");break;
		case 9:strcpy(story,"Denver, CO");break;
		case 10:strcpy(story,"Phoenix, AZ");break;
		case 11:strcpy(story,"Little Rock, AR");break;
		case 12:strcpy(story,"Houston, TX");break;
		case 13:strcpy(story,"Dallas, TX");break;
		case 14:strcpy(story,"Hartford, CT");break;
		case 15:strcpy(story,"Miami, FL");break;
		case 16:strcpy(story,"Baton Rouge, LA");break;
		case 17:strcpy(story,"Seattle, WA");break;
		case 18:strcpy(story,"Salt Lake City, UT");break;
		case 19:strcpy(story,"Philadelphia, PA");break;
		}
}

char talkreceptive(creaturest &cr)
{
	if(cr.align==-1)return 0;

	switch(cr.type)
		{
		case CREATURE_WORKER_SERVANT:
		case CREATURE_WORKER_JANITOR:
		case CREATURE_WORKER_SWEATSHOP:
		case CREATURE_WORKER_FACTORY_CHILD:
		case CREATURE_TEENAGER:
		case CREATURE_SEWERWORKER:
		case CREATURE_COLLEGESTUDENT:
		case CREATURE_MUSICIAN:
		case CREATURE_MATHEMATICIAN:
		case CREATURE_TEACHER:
		case CREATURE_HSDROPOUT:
		case CREATURE_BUM:
		case CREATURE_GANGMEMBER:
		case CREATURE_CRACKHEAD:
		case CREATURE_FASTFOODWORKER:
		case CREATURE_TELEMARKETER:
		case CREATURE_PROSTITUTE:
		case CREATURE_GARBAGEMAN:
		case CREATURE_PLUMBER:
		case CREATURE_AMATEURMAGICIAN:
		case CREATURE_HIPPIE:
		case CREATURE_RETIREE:
		case CREATURE_HAIRSTYLIST:
		case CREATURE_CLERK:
		case CREATURE_MUTANT:
			return 1;
		}

	return 0;
}

void generatefiller(char *story,int amount)
{
	char str[80];
	strcat(story,"&r");
	cityname(str);
	strcat(story,str);
	strcat(story," - ");
	int par=0;
	while(amount>0)
		{
		par++;
		for(int i=0;i<LCSrandom(10)+3;i++)strcat(story,"~");
		if(amount>1)strcat(story," ");
		if(par>=50&&!LCSrandom(5)&&amount>20)
			{
			par=0;
			strcat(story,"&r");
			strcat(story,"   ");
			}
		amount--;
		}
	strcat(story,"&r");
}

char kid(creaturest &cr)
{
	switch(cr.type)
		{
		case CREATURE_WORKER_FACTORY_CHILD:
			return 1;
		}

	return 0;
}

void getlaw(char *str,int l)
{
	switch(l)
		{
		case LAW_ABORTION:strcpy(str,"Abortion Rights");break;
		case LAW_ANIMALRESEARCH:strcpy(str,"Animal Research");break;
		case LAW_POLICEBEHAVIOR:strcpy(str,"Police Behavior");break;
		case LAW_PRIVACY:strcpy(str,"Privacy");break;
		case LAW_DEATHPENALTY:strcpy(str,"Capital Punishment");break;
		case LAW_NUCLEARPOWER:strcpy(str,"Nuclear Power");break;
		case LAW_POLLUTION:strcpy(str,"Pollution");break;
		case LAW_LABOR:strcpy(str,"Workers' Rights");break;
		case LAW_GAY:strcpy(str,"Homosexual Rights");break;
		case LAW_CORPORATE:strcpy(str,"Corporate Ethics");break;
		case LAW_FREESPEECH:strcpy(str,"Freedom of Speech");break;
		case LAW_TAX:strcpy(str,"Tax Law");break;
		case LAW_FLAGBURNING:strcpy(str,"Flag Burning");break;
		}
}

char completevacation(datest &d,int p,char &clearformess)
{
	int e=0;

	clearformess=1;

	erase();
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(0,0);
	addstr(pool[p]->name);
	addstr(" is back from vacation.");

	short aroll=LCSrandom(51)+10+pool[p]->attval(ATTRIBUTE_CHARISMA)*2+LCSrandom(pool[p]->skill[SKILL_PERSUASION]*2+1);
	short troll=LCSrandom(21)+d.date[e]->attval(ATTRIBUTE_CHARISMA)+d.date[e]->attval(ATTRIBUTE_WISDOM)*2;
	pool[p]->skill_ip[SKILL_PERSUASION]+=LCSrandom(14)+7;

	int y=2;
	if(aroll>troll)
		{
		set_color(COLOR_BLUE,COLOR_BLACK,1);
		move(y,0);y++;
		addstr(d.date[e]->name);
		addstr(" learned a lot from ");
		addstr(pool[p]->name);
		addstr(" during their time together.");
		refresh();
		getch();

		if(d.date[e]->att[ATTRIBUTE_WISDOM]<=1||
			LCSrandom(20)>d.date[e]->att[ATTRIBUTE_WISDOM])
			{
			set_color(COLOR_GREEN,COLOR_BLACK,1);
			move(y,0);y++;
			addstr("In fact, ");
			addstr(d.date[e]->name);
			addstr(" is ");
			addstr(pool[p]->name);
			addstr("'s totally unconditional love-slave!");
			refresh();
			getch();

			name(d.date[e]->name);
			strcpy(d.date[e]->propername,d.date[e]->name);

			d.date[e]->location=pool[p]->location;
			d.date[e]->base=pool[p]->base;
			d.date[e]->hireid=pool[p]->id;

			erase();

			set_color(COLOR_WHITE,COLOR_BLACK,1);
			move(0,0);
			addstr("The Self-Nullifying Infatuation of ");
			addstr(d.date[e]->propername);

			move(2,0);
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			addstr("What name will you use for this ");
			char str[80];
			getrecruitcreature(str,d.date[e]->type);
			addstr(str);
			addstr(" in its presence?");

			keypad(stdscr,FALSE);
			raw_output(FALSE);
			echo();
			curs_set(1);
			mvgetstr(3,0,d.date[e]->name);
			curs_set(0);
			noecho();
			raw_output(TRUE);
			keypad(stdscr,TRUE);

			pool.push_back(d.date[e]);
			stat_recruits++;

			d.date[e]->location=pool[p]->location;
			d.date[e]->base=pool[p]->base;
			d.date[e]->align=1;
			d.date.erase(d.date.begin() + e);
			
			return 1;
			}
		else
			{
			d.date[e]->att[ATTRIBUTE_WISDOM]-=LCSrandom(5)+1;
			if(d.date[e]->att[ATTRIBUTE_WISDOM]<1)d.date[e]->att[ATTRIBUTE_WISDOM]=1;

			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);y++;
			addstr("They'll meet again tomorrow.");
			refresh();
			getch();

			return 0;
			}
		}
	else
		{
		//WISDOM POSSIBLE INCREASE
		if(d.date[e]->align==-1&&!LCSrandom(2))
			{
			set_color(COLOR_RED,COLOR_BLACK,1);
			move(y,0);y++;
			addstr("Spending time with ");
			addstr(d.date[e]->name);
			addstr(" actually increases ");
			addstr(pool[p]->name);
			addstr("'s wisdom!!!");
			pool[p]->att[ATTRIBUTE_WISDOM]+=LCSrandom(5)+1;

			refresh();
			getch();
			}

		//BREAK UP
		set_color(COLOR_MAGENTA,COLOR_BLACK,1);
		move(y,0);y++;
		addstr(d.date[e]->name);
		addstr(" can sense that things just aren't working out.");
		move(y,0);y++;
		addstr("This relationship is over.");

		refresh();
		getch();

		delete d.date[e];
		d.date.erase(d.date.begin() + e);

		return 1;
		}
}

char completedate(datest &d,int p,char &clearformess)
{
	int e;
	clearformess=1;

	erase();
	set_color(COLOR_WHITE,COLOR_BLACK,1);
	move(0,0);
	addstr(pool[p]->name);
	addstr(" has ");
	if(d.date.size()==1)
		{
		if(pool[p]->clinic)addstr("a \"hot\" date with ");
		else addstr("a hot date with ");
		}
	else addstr("dates to manage with ");
	for(e=0;e<d.date.size();e++)
		{
		addstr(d.date[e]->name);

		if(e<=(int)d.date.size()-3)addstr(", ");
		else if(e==(int)d.date.size()-2)addstr(" and ");
		else
			{
			if(pool[p]->clinic>0)
				{
				addstr(" at ");
				addstr(location[pool[p]->location]->name);
				}
			addstr(".");
			}
		}

	refresh();
	getch();

	if(d.date.size()>1&&!LCSrandom(3))
		{
		move(2,0);
		addstr("Unfortunately, they all know each other and had been discussing");
		move(3,0);
		addstr(pool[p]->name);
		addstr(".  An ambush was set for the lying dog...");
		refresh();
		getch();

		move(5,0);
		addstr(pool[p]->name);
		addstr(" has failed the Liberal Crime Squad.");
		addjuice(*pool[p],-5);
		refresh();
		getch();

		return 1;
		}

	for(e=d.date.size()-1;e>=0;e--)
		{
		erase();
		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(0,0);
		addstr("Seeing ");
		addstr(d.date[e]->name);
		addstr(", ");
		switch(d.date[e]->align)
			{
			case -1:addstr("Conservative");break;
			case 0:addstr("moderate");break;
			case 1:addstr("Liberal");break;
			}

		set_color(COLOR_GREEN,COLOR_BLACK,1);
		move(0,66);
		addstr("$");
		char num[20];
		itoa(funds,num,10);
		addstr(num);

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(2,0);
		addstr("How should ");
		addstr(pool[p]->name);
		addstr(" approach the situation?");

		if(funds>=100&&!pool[p]->clinic)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(4,0);
		addstr("A - Spend a hundred bucks tonight to get the ball rolling.");
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(5,0);
		addstr("B - Try to get through the evening without spending a penny.");
		if(!pool[p]->clinic)set_color(COLOR_WHITE,COLOR_BLACK,0);
		else set_color(COLOR_BLACK,COLOR_BLACK,1);
		move(6,0);
		addstr("C - Spend a week on a cheap vacation (stands up any other dates).");
		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(7,0);
		addstr("D - Break it off.");
		if(d.date[e]->align==-1&&!pool[p]->clinic)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(8,0);
			addstr("E - Just kidnap the Conservative bitch.");
			}

		do
			{
			refresh();
			int c=getch();
			translategetch(c);

			short aroll=LCSrandom(21)+pool[p]->attval(ATTRIBUTE_CHARISMA)*2+LCSrandom(pool[p]->skill[SKILL_PERSUASION]*2+1);
			short troll=LCSrandom(21)+d.date[e]->attval(ATTRIBUTE_CHARISMA)+d.date[e]->attval(ATTRIBUTE_WISDOM)*2;
			pool[p]->skill_ip[SKILL_PERSUASION]+=LCSrandom(2)+1;

			char test=0;
			if(c=='a'&&funds>=100&&!pool[p]->clinic)
				{
				funds-=100;
				stat_spent+=100;
				moneylost_compound+=100;
				aroll+=LCSrandom(10);
				test=1;
				}
			if(c=='b')test=1;

			if(test)
				{
				int y=10;
				if(aroll>troll)
					{
					set_color(COLOR_BLUE,COLOR_BLACK,1);
					move(y,0);y++;
					addstr(d.date[e]->name);
					addstr(" is quite taken with ");
					addstr(pool[p]->name);
					addstr("'s unique life philosophy...");
					refresh();
					getch();

					if(d.date[e]->att[ATTRIBUTE_WISDOM]<=1||
						LCSrandom(10)>d.date[e]->att[ATTRIBUTE_WISDOM])
						{
						set_color(COLOR_GREEN,COLOR_BLACK,1);
						move(y,0);y++;
						addstr("In fact, ");
						addstr(d.date[e]->name);
						addstr(" is ");
						addstr(pool[p]->name);
						addstr("'s totally unconditional love-slave!");
						refresh();
						getch();

						name(d.date[e]->name);
						strcpy(d.date[e]->propername,d.date[e]->name);

						d.date[e]->location=pool[p]->location;
						d.date[e]->base=pool[p]->base;
						d.date[e]->hireid=pool[p]->id;

						erase();
	
						set_color(COLOR_WHITE,COLOR_BLACK,1);
						move(0,0);
						addstr("The Self-Nullifying Infatuation of ");
						addstr(d.date[e]->propername);

						move(2,0);
						set_color(COLOR_WHITE,COLOR_BLACK,0);
						addstr("What name will you use for this ");
						char str[80];
						getrecruitcreature(str,d.date[e]->type);
						addstr(str);
						addstr(" in its presence?");

						keypad(stdscr,FALSE);
						raw_output(FALSE);
						echo();
						curs_set(1);
						mvgetstr(3,0,d.date[e]->name);
						curs_set(0);
						noecho();
						raw_output(TRUE);
						keypad(stdscr,TRUE);

						pool.push_back(d.date[e]);
						stat_recruits++;

						d.date[e]->location=pool[p]->location;
						d.date[e]->base=pool[p]->base;
						d.date[e]->align=1;
						d.date.erase(d.date.begin() + e);
						}
					else
						{
						d.date[e]->att[ATTRIBUTE_WISDOM]--;

						set_color(COLOR_WHITE,COLOR_BLACK,0);
						move(y,0);y++;
						addstr("They'll meet again tomorrow.");
						refresh();
						getch();
						}
					}
				else
					{
					//WISDOM POSSIBLE INCREASE
					if(d.date[e]->align==-1)
						{
						set_color(COLOR_RED,COLOR_BLACK,1);
						move(y,0);y++;
						addstr("Talking with ");
						addstr(d.date[e]->name);
						addstr(" actually increases ");
						addstr(pool[p]->name);
						addstr("'s wisdom!!!");
						pool[p]->att[ATTRIBUTE_WISDOM]++;

						refresh();
						getch();
						}

					//BREAK UP
					set_color(COLOR_MAGENTA,COLOR_BLACK,1);
					move(y,0);y++;
					addstr(d.date[e]->name);
					addstr(" can sense that things just aren't working out.");
					move(y,0);y++;
					addstr("This relationship is over.");

					refresh();
					getch();

					delete d.date[e];
					d.date.erase(d.date.begin() + e);
					}
				break;
				}

			if(c=='c'&&!pool[p]->clinic)
				{
				for(int e2=d.date.size()-1;e2>=0;e2--)
					{
					if(e2==e)continue;
					delete d.date[e2];
					d.date.erase(d.date.begin() + e2);
					}
				d.timeleft=7;
				return 0;
				}
			if(c=='d')
				{
				delete d.date[e];
				d.date.erase(d.date.begin() + e);
				break;
				}
			if(c=='e'&&d.date[e]->align==-1&&!pool[p]->clinic)
				{
				name(d.date[e]->name);
				strcpy(d.date[e]->propername,d.date[e]->name);

				d.date[e]->location=pool[p]->location;
				d.date[e]->base=pool[p]->base;
				d.date[e]->flag|=CREATUREFLAG_MISSING;

				erase();

				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(0,0);
				addstr("The Education of ");
				addstr(d.date[e]->propername);

				move(2,0);
				set_color(COLOR_WHITE,COLOR_BLACK,0);
				addstr("What name will you use for this ");
				char str[80];
				getrecruitcreature(str,d.date[e]->type);
				addstr(str);
				addstr(" in its presence?");

				keypad(stdscr,FALSE);
				raw_output(FALSE);
				echo();
				curs_set(1);
				mvgetstr(3,0,d.date[e]->name);
				curs_set(0);
				noecho();
				raw_output(TRUE);
				keypad(stdscr,TRUE);

				pool.push_back(d.date[e]);
				stat_kidnappings++;
				d.date.erase(d.date.begin() + e);
				break;
				}
			}while(1);
		}

	if(d.date.size()>0)
		{
		d.timeleft=0;
		return 0;
		}
	else return 1;
}

void updateworld_laws(short *law,short *oldlaw)
{
	//RENAME PRISONS
	if(law[LAW_DEATHPENALTY]==-2&&
		law[LAW_POLICEBEHAVIOR]==-2&&
		(oldlaw[LAW_DEATHPENALTY]>-2||
		oldlaw[LAW_POLICEBEHAVIOR]>-2))
		{
		for(int l=0;l<location.size();l++)
			{
			if(location[l]->type==SITE_GOVERNMENT_PRISON)
				{
				initlocation(*location[l]);
				}
			}
		}
	//RENAME CAMPS
	else if(oldlaw[LAW_DEATHPENALTY]==-2&&
		oldlaw[LAW_POLICEBEHAVIOR]==-2&&
		(law[LAW_DEATHPENALTY]>-2||
		law[LAW_POLICEBEHAVIOR]>-2))
		{
		for(int l=0;l<location.size();l++)
			{
			if(location[l]->type==SITE_GOVERNMENT_PRISON)
				{
				initlocation(*location[l]);
				}
			}
		}
}

void reloadparty(void)
{
	for(int p=0;p<6;p++)
		{
		if(activesquad->squad[p]==NULL)continue;
		if(!activesquad->squad[p]->alive)continue;

		if(ammotype(activesquad->squad[p]->weapon.type)!=-1)
			{
			int ammomax=2;
			switch(ammotype(activesquad->squad[p]->weapon.type))
				{
				case CLIP_9:ammomax=15;break;
				case CLIP_45:ammomax=15;break;
				case CLIP_ASSAULT:ammomax=30;break;
				case CLIP_22:ammomax=6;break;
				case CLIP_44:ammomax=6;break;
				case CLIP_BUCKSHOT:ammomax=2;break;
				}

			if(activesquad->squad[p]->weapon.ammo<ammomax&&activesquad->squad[p]->clip[ammotype(activesquad->squad[p]->weapon.type)]>0)
				{
				activesquad->squad[p]->weapon.ammo=ammomax;

				activesquad->squad[p]->clip[ammotype(activesquad->squad[p]->weapon.type)]--;

				}
			}
		}
}

void imprison(creaturest &g)
{
	long ps=-1;
	for(long l=0;l<location.size();l++)
		{
		if(location[l]->type==SITE_GOVERNMENT_PRISON)
			{
			ps=l;
			}
		}
	g.location=ps;
}

char confirmdisband(void)
{
	char word[80];
	int pos=0;

	switch(LCSrandom(5))
		{
		case 0:strcpy(word,"Corporate Accountability");break;
		case 1:strcpy(word,"Free Speech");break;
		case 2:strcpy(word,"Gay Marriage");break;
		case 3:strcpy(word,"Abortion Rights");break;
		case 4:strcpy(word,"Separation Clause");break;
		}

	do
		{
		erase();

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(0,0);
		addstr("Are you sure you want to disband?");

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(2,0);
		addstr("Disbanding ends the game.  The country will be simulated until a resolution");
		move(3,0);
		addstr("is reached.  Any sleepers in place will still be effective, but the rest of");
		move(4,0);
		addstr("your infrastructure will be dismantled.");

		set_color(COLOR_WHITE,COLOR_BLACK,1);
		move(6,0);
		addstr("Type this Liberal phrase to confirm (press a wrong letter to rethink it):");

		for(int x=0;x<strlen(word);x++)
			{
			move(8,x);
			if(x==pos)set_color(COLOR_GREEN,COLOR_BLACK,0);
			else if(x<pos)set_color(COLOR_GREEN,COLOR_BLACK,1);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);
			addch(word[x]);
			}

		int c=getch();
		translategetch(c);

		if((c==word[pos])||((c+'A'-'a')==word[pos]))
			{
			pos++;
			if(word[pos]==' ')pos++;
			if(pos>=strlen(word))
				{
				//SET UP THE DISBAND
				for(int p=0;p<pool.size();p++)
					{
					if(pool[p]->alive&&
						!(pool[p]->flag & CREATUREFLAG_SLEEPER))
						{
						pool[p]->activity.type=ACTIVITY_NONE;
						pool[p]->clinic=0;
						pool[p]->lawflag=0;
						pool[p]->lawflag2=0;
						pool[p]->dating=0;
						pool[p]->hiding=0;
						}
					}

				disbandtime=year;

				return 1;
				}
			}
		else break;

		}while(1);

	return 0;
}

void squadlessbaseassign(void)
{
int p = 0;
int l = 0;
	vector<creaturest *> temppool;
	for(p=0;p<pool.size();p++)
		{
		if(pool[p]->alive&&
			pool[p]->align==1&&
			pool[p]->clinic==0&&
			pool[p]->dating==0&&
			pool[p]->hiding==0&&
			pool[p]->squadid==-1&&
			!(pool[p]->flag & CREATUREFLAG_SLEEPER))
			{
			if(location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
				location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
				location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON)
				{
				temppool.push_back(pool[p]);
				}
			}
		}

	if(temppool.size()==0)return;

	vector<int> temploc;
	for(l=0;l<location.size();l++)
		{
		if(location[l]->renting>=0&&!location[l]->siege.siege)temploc.push_back(l);
		}
	if(temploc.size()==0)return;

	short page_lib=0;
	short page_loc=0;

	char num[20];

	int selectedbase=0;

	do
		{
		erase();

		set_color(COLOR_GREEN,COLOR_BLACK,1);
		itoa(funds,num,10);
		move(0,78-strlen(num));
		addstr("$");
		addstr(num);

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addstr("New Bases for Squadless Liberals");
		move(1,0);
		addstr("----CODE NAME------------CURRENT BASE-------------------------------------------");
		move(1,51);
		addstr("NEW BASE");

		int y=2;
		for(p=page_lib*19;p<temppool.size()&&p<page_lib*19+19;p++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addch(y+'A'-2);addstr(" - ");
			addstr(temppool[p]->name);

			move(y,25);
			addstr(location[temppool[p]->base]->shortname);

			y++;
			}

		y=2;
		for(p=page_loc*9;p<temploc.size()&&p<page_loc*9+9;p++)
			{
			if(p==selectedbase)set_color(COLOR_WHITE,COLOR_BLACK,1);
			else set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,51);
			addch(y+'1'-2);addstr(" - ");
			addstr(location[temploc[p]]->shortname);

			y++;
			}


		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(22,0);
		addstr("Press a Letter to assign a Base.  Press a Number to select a Base.");
		if(temppool.size()>19)
			{
			move(23,0);
			if(interface_pgup=='[')
				{
				addstr("[] to view other Liberal pages.");
				}
			else
				{
				addstr("PGUP/PGDN to view other Liberal pages.");
				}
			}
		if(temploc.size()>9)
			{
			move(24,0);
			addstr(",. to view other Base pages.");
			}

		refresh();

		int c=getch();
		translategetch(c);

		//PAGE UP
		if(c==interface_pgup&&page_lib>0)page_lib--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page_lib+1)*19<temppool.size())page_lib++;

		//PAGE UP
		if(c==','&&page_loc>0)page_loc--;
		//PAGE DOWN
		if(c=='.'&&(page_loc+1)*9<temploc.size())page_loc++;

		if(c>='a'&&c<='s')
			{
			int p=page_lib*19+(int)(c-'a');
			if(p<temppool.size())
				{
				temppool[p]->base=temploc[selectedbase];
				}
			}
		if(c>='1'&&c<='9')
			{
			int p=page_loc*9+(int)(c-'1');
			if(p<temploc.size())
				{
				selectedbase=p;
				}
			}

		if(c==10)break;
		}while(1);
}

void promoteliberals(void)
{
	vector<creaturest *> temppool;
	vector<int> level;
	for(int p=0;p<pool.size();p++)
		{
		if(pool[p]->alive&&
			pool[p]->align==1&&
			!(pool[p]->flag & CREATUREFLAG_SLEEPER))
			{
			temppool.push_back(pool[p]);
			}
		}

	if(temppool.size()==0)return;

	//SORT
	sortbyhire(temppool,level);

	//PROMOTE
	short page=0;

	char num[20];

	do
		{
		erase();

		set_color(COLOR_GREEN,COLOR_BLACK,1);
		itoa(funds,num,10);
		move(0,78-strlen(num));
		addstr("$");
		addstr(num);

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(0,0);
		addstr("Promote the Elite Liberals");
		move(1,0);
		addstr("----CODE NAME--------------CURRENT CONTACT--------------------------------------");
		move(1,54);
		addstr("CONTACT AFTER PROMOTION");

		int y=2;
		for(int p=page*19;p<temppool.size()&&p<page*19+19;p++)
			{
			set_color(COLOR_WHITE,COLOR_BLACK,0);
			move(y,0);
			addch(y+'A'-2);addstr(" - ");

			move(y,27);
			int p2 = 0;
			
			for(p2=0;p2<pool.size();p2++)
				{
				int p3 = 0;
				if(pool[p2]->alive==1&&pool[p2]->id==temppool[p]->hireid)
					{
					addstr(pool[p2]->name);

					move(y,54);
					for(p3=0;p3<pool.size();p3++)
						{
						if(pool[p3]->alive==1&&pool[p3]->id==pool[p2]->hireid)
							{
							addstr(pool[p3]->name);
							break;
							}
						}
					if(p3==pool.size())addstr("<Liberal Comrade>");
					break;
					}
				}
			if(p2==pool.size())addstr("<LCS Founder>");

			move(y,4+level[p]);
			addstr(temppool[p]->name);

			y++;
			}

		set_color(COLOR_WHITE,COLOR_BLACK,0);
		move(22,0);
		addstr("Press a Letter to Promote a Liberal.");
		if(temppool.size()>19)
			{
			move(23,0);
			if(interface_pgup=='[')
				{
				addstr("[] to view other Liberal pages.");
				}
			else
				{
				addstr("PGUP/PGDN to view other Liberal pages.");
				}
			}

		refresh();

		int c=getch();
		translategetch(c);

		//PAGE UP
		if(c==interface_pgup&&page>0)page--;
		//PAGE DOWN
		if(c==interface_pgdn&&(page+1)*19<temppool.size())page++;

		if(c>='a'&&c<='s')
			{
			int p=page*19+(int)(c-'a');
			if(p<temppool.size())
				{
				for(int p2=0;p2<pool.size();p2++)
					{
					if(pool[p2]->alive==1&&pool[p2]->id==temppool[p]->hireid)
						{
						addstr(pool[p2]->name);

						for(int p3=0;p3<pool.size();p3++)
							{
							if(pool[p3]->alive==1&&pool[p3]->id==pool[p2]->hireid)
								{
								temppool[p]->hireid=pool[p2]->hireid;
								sortbyhire(temppool,level);
								break;
								}
							}
						break;
						}
					}
				}
			}

		if(c==10)break;
		}while(1);
}

void sortbyhire(vector<creaturest *> &temppool,vector<int> &level)
{
	vector<creaturest *> newpool;
	level.clear();

	for(int i=temppool.size()-1;i>=0;i--)
		{
		if(temppool[i]->hireid==-1)
			{
			newpool.insert(newpool.begin(),temppool[i]);
			level.insert(level.begin(),0);
			temppool.erase(temppool.begin() + i);
			}
		}

	char changed;

	do
		{
		changed=0;

		for(int i=0;i<newpool.size();i++)
			{
			for(int j=temppool.size()-1;j>=0;j--)
				{
				if(temppool[j]->hireid==newpool[i]->id)
					{
					newpool.insert(newpool.begin()+i+1,temppool[j]);
					level.insert(level.begin()+i+1,level[i]+1);
					temppool.erase(temppool.begin() + j);
					changed=1;
					}
				}
			}
		}while(changed);

	temppool.clear();
	for(int p=0;p<newpool.size();p++)
		{
		temppool.push_back(newpool[p]);
		}
}

void dispersalcheck(char &clearformess)
{
int p = 0;
	//NUKE DISPERSED SQUAD MEMBERS WHOSE MASTERS ARE NOT AVAILABLE
	if(pool.size()>0)
		{
		vector<int> nukeme;
		nukeme.resize(pool.size());
		for(p=pool.size()-1;p>=0;p--)
			{
			if(!pool[p]->alive||pool[p]->hireid==-1)nukeme[p]=0;
			else nukeme[p]=1;
			}

		char changed;

		do
			{
			changed=0;

			char conf;

			for(p=pool.size()-1;p>=0;p--)
				{
				conf=0;
				if(pool[p]->location==-1)conf=1;
				else if(location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON&&
					location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION&&
					location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE)conf=1;
				if(nukeme[p]==0&&pool[p]->alive&&conf)
					{
					for(int p2=pool.size()-1;p2>=0;p2--)
						{
						if(pool[p2]->hireid==pool[p]->id)
							{
							nukeme[p2]=0;
							changed=1;
							}
						}
					nukeme[p]=-1;
					}
				}
			}while(changed);

		for(p=pool.size()-1;p>=0;p--)
			{
			if(nukeme[p]==1)
				{
				if(clearformess)
					{
					erase();
					}
				else
					{
					makedelimiter(8,0);
					}

				set_color(COLOR_WHITE,COLOR_BLACK,1);
				move(8,1);
				addstr(pool[p]->name);
				addstr(" has lost touch with the Liberal Crime Squad.");

				refresh();
				getch();

				removesquadinfo(*pool[p]);
				delete pool[p];
				pool.erase(pool.begin() + p);
				}
			}
		}

	//MUST DO AN END OF GAME CHECK HERE BECAUSE OF DISPERSAL
	endcheck(END_DISPERSED);
}

void addlocationname(locationst *loc)
{
	if(loc->front_business!=-1)
		{
		addstr(loc->front_name);
		}
	else
		{
		addstr(loc->name);
		}
}

void loadinitfile(void)
{
	//LOAD OPTIONS IF POSSIBLE
	::fstream fseed;
	#ifdef WIN32_PRE_DOTNET
	//This uses some non-standard isotream stuff which Microsoft have got
	//rid of in the .NET 2003 version of their C++ library (the 3rd arg and nocreate)
	fseed.open("init.txt",ios::nocreate | ios::in,filebuf::sh_read);
	#else
	fseed.open("init.txt",ios::in);
	#endif
	if(fseed.is_open())
		{
		int count=0;
		char str[200];
		char word[3][205];
		int wordnum,pos;
		char begin;
		while(fseed.getline(str,198)&&count<10000)
			{
			if(str[0]!='#')
				{
				wordnum=0;pos=0;begin=1;
				for(int l=0;l<strlen(str);l++)
					{
					if(str[l]==' '||str[l]=='\t')
						{
						if(str[l+1]!=' '&&str[l+1]!='\t'&&!begin)
							{
							word[wordnum][pos]='\x0';
							wordnum++;
							pos=0;
							}
						continue;
						}
					if(wordnum>2)break;
					word[wordnum][pos]=str[l];
					pos++;
					begin=0;
					}
				if(wordnum<=2)word[wordnum][pos]='\x0';

				if(!stricmp(word[1],"="))
					{
					if(!stricmp(word[0],"pagekeys"))
						{
						if(!stricmp(word[2],"brackets"))
							{
							interface_pgup='[';
							interface_pgdn=']';
							}
						else if(!stricmp(word[2],"page"))
							{
							interface_pgup=-61;
							interface_pgdn=-55;
							}
						}
					}
				}
			count++;
			
			}
		}
	fseed.close();
}

//GETS RID OF CAR PREFERENCES FROM pool LIBERALS, BY CAR ID NUMBER
void removecarprefs_pool(long carid)
{
	for(int p=0;p<pool.size();p++)
		{
		if(pool[p]->pref_carid==carid)
			{
			pool[p]->pref_carid=-1;
			}
		}
}

