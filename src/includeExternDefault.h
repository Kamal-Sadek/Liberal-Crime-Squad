#include "includeActivateB.h"

extern string savefile_name;

//extern char homedir[MAX_PATH_SIZE];
//extern char artdir[MAX_PATH_SIZE];

extern CursesMoviest movie;
extern unsigned char bigletters[27][5][7][4];
extern unsigned char newstops[6][80][5][4];
extern unsigned char newspic[20][78][18][4];


extern vector<configSiteMap *> sitemaps; // stores site map info read in from config file

//extern bool multipleCityMode;
extern int oldMapMode;
extern unsigned long seed[RNG_SIZE];


extern bool autosave;

#ifdef WIN32
extern bool fixcleartype;
#endif

extern int day;


extern short background_liberal_influence[VIEWNUM];

//extern char endgamestate;
extern char ccsexposure;

extern char ccs_kills;
//extern int ccs_siege_kills;
//extern int ccs_boss_kills;


extern char loaded;

extern short wincondition;

extern bool notermlimit;
extern bool nocourtpurge;
extern bool stalinmode;

extern short offended_corps;
extern short offended_cia;
extern short offended_amradio;
extern short offended_cablenews;
extern short offended_firemen;
extern int police_heat;
extern unsigned long attorneyseed[RNG_SIZE];
extern char lcityname[CITY_NAMELEN];

extern int month;
//extern int year;
extern int amendnum;

extern bool termlimits;
extern bool deagle;
extern bool m249;

extern UniqueCreatures uniqueCreatures;

extern short party_status;

extern vector<newsstoryst *> newsstory;

extern long curcreatureid;
extern chaseseqst chaseseq;


extern char disbanding;
extern int disbandtime;
extern char cantseereason;


extern vector<ClipType *> cliptype;
extern vector<WeaponType *> weapontype;
//extern vector<LootType *> loottype;
extern vector<CreatureType *> creaturetype;


extern vector<VehicleType *> vehicletype;
extern vector<Vehicle *> vehicle;
extern char showcarprefs;

extern highscorest score[SCORENUM];
extern int yourscore;

//The game log. All events and stuff are output here.
//NO debugging out. Make a debugging log for that.
extern Log xmllog;

