
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
	vector<Vehicle *> friendcar, enemycar;
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
		for (int p = 0; p<6; p++) squad[p] = NULL;
		strcpy(name, "");
		activity.type = ACTIVITY_NONE, id = -1, stance = SQUADSTANCE_STANDARD;
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

enum MusicModes
{
	MUSIC_TITLEMODE,
	MUSIC_NEWGAME,
	MUSIC_BASEMODE,
	MUSIC_SIEGE,
	MUSIC_ACTIVATE,
	MUSIC_SLEEPERS,
	MUSIC_STOPEVIL,
	MUSIC_REVIEWMODE,
	MUSIC_LIBERALAGENDA,
	MUSIC_DISBANDED,
	MUSIC_FINANCES,
	MUSIC_CARTHEFT,
	MUSIC_ELECTIONS,
	MUSIC_SHOPPING,
	MUSIC_SITEMODE,
	MUSIC_SUSPICIOUS,
	MUSIC_ALARMED,
	MUSIC_HEAVYCOMBAT,
	MUSIC_DEFENSE,
	MUSIC_CONQUER,
	MUSIC_CARCHASE,
	MUSIC_FOOTCHASE,
	MUSIC_INTERROGATION,
	MUSIC_TRIAL,
	MUSIC_RECRUITING,
	MUSIC_DATING,
	MUSIC_NEWSPAPER,
	MUSIC_LACOPS,
	MUSIC_NEWSCAST,
	MUSIC_GLAMSHOW,
	MUSIC_ANCHOR,
	MUSIC_ABORT,
	MUSIC_VICTORY,
	MUSIC_DEFEAT,
	MUSIC_REAGANIFIED,
	MUSIC_STALINIZED,
	MUSIC_OFF, // this one must come immediately after the ones corresponding to music files (this one is to have silence)
	MUSIC_PREVIOUS, // this one must come after MUSIC_OFF (this one is to play the previous song)
	MUSIC_CURRENT, // this one must come after MUSIC_OFF (this one continues playing the current song)
	MUSIC_RANDOM // this one must come after MUSIC_OFF (this one plays a random song)
};

class MusicClass
{
private:
	bool enabled;
#ifndef DONT_INCLUDE_SDL
	bool songsinitialized;
	bool oggsupport;
	int musicmode, previous;
	Mix_Music* songs[MUSIC_OFF];
	/* helper function for initsongs() ... implemented in misc.cpp */
	void loadsong(int i, const char* filename);
#endif // DONT_INCLUDE_SDL
public:
#ifndef DONT_INCLUDE_SDL
	MusicClass() : enabled(true), songsinitialized(false), oggsupport(true), musicmode(MUSIC_OFF), previous(MUSIC_OFF) { }
#else
	MusicClass() : enabled(true) { }
#endif // DONT_INCLUDE_SDL
	/* find out if music's enabled or disabled */
	bool isEnabled() { return enabled; }
	/* enable or disable music */
	void enableIf(bool e)
	{
		enabled = e;
#ifndef DONT_INCLUDE_SDL
		Mix_VolumeMusic(enabled*(MIX_MAX_VOLUME / 2)); // half volume if music enabled, muted if music disabled
#endif // DONT_INCLUDE_SDL
	}
	/* initialize SDL, SDL_mixer, and songs ... implemented in misc.cpp */
	void init();
	/* shut down SDL, SDL_mixer, and songs ... implemented in misc.cpp */
	void quit();
	/* play music specified by a MusicMode ... implemented in misc.cpp */
	void play(int _musicmode);
};

enum WinConditions
{
	WINCONDITION_ELITE,
	WINCONDITION_EASY
};
enum FieldSkillRates
{
	FIELDSKILLRATE_FAST,
	FIELDSKILLRATE_CLASSIC,
	FIELDSKILLRATE_HARD
};

enum Views
{
	VIEW_STALIN = -2, // this one is -2 and is actually calculated based on views >=0 and <VIEWNUM-3
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
	LAW_STALIN = -2, // not a real law: this is -2 and is actually calculated based on views >=0 and <VIEWNUM-3
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
	datest() : timeleft(0) { }
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
	char level, eagerness1, task;
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
	short type, view;
	char claimed;
	short politics_level, violence_level;
	Creature *cr;
	vector<int> crime;
	long loc, priority, page, guardianpage;
	char positive;
	short siegetype;
	newsstoryst() : claimed(1), politics_level(0), violence_level(0), cr(NULL), loc(-1) { }
};

#include "news/news.h"

#define SLOGAN_LEN 80

struct highscorest
{
	char valid, endtype, slogan[SLOGAN_LEN];
	int month, year, stat_recruits, stat_kidnappings, stat_dead, stat_kills, stat_funds, stat_spent, stat_buys, stat_burns;
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

enum PoliticalParties
{
	LIBERAL_PARTY,
	CONSERVATIVE_PARTY,
	STALINIST_PARTY,
	PARTYNUM
};

// full house (100%) - for looping thru full house
#define HOUSENUM 435
// just over half of house (50%+1) - to pass bills
#define HOUSEMAJORITY 218
// 3/5 of house - has no significance other than in seeing if you won game
#define HOUSECOMFYMAJORITY 261
// 2/3 of house - to override veto or pass constitutional amendment
#define HOUSESUPERMAJORITY 290
// full senate (100%) - for looping thru full senate
#define SENATENUM 100
// just over half of senate(50%+1) - to bass bills
#define SENATEMAJORITY 51
// 3/5 of senate - to break filibuster in real world, but in game, has no significance other than seeing if you won
#define SENATECOMFYMAJORITY 60
// 2/3 of senate - to override veto or pass constitutional amendment
#define SENATESUPERMAJORITY 67
// full court (100%) - for looping thru full court
#define COURTNUM 9
// just over half of court (50%+1) - to make majority rulings
#define COURTMAJORITY 5
// 2/3 of court - has no significance other than seeing if you won game
#define COURTSUPERMAJORITY 6
// all states (100%) - for looping thru all states
#define STATENUM 50
// 3/4 of states (75%) - needed to pass constitutional amendments
#define STATESUPERMAJORITY 38

#define POLITICIAN_NAMELEN 80

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
