//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//Copyright (c) 2002,2003,2004 by Tarn Adams                                            //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////
//This file is part of Liberal Crime Squad.                                             //
//                                                                                      //
//      Liberal Crime Squad is free software; you can redistribute it and/or modify     //
//      it under the terms of the GNU General Public License as published by            //
//      the Free Software Foundation; either version 2 of the License, or               //
//      (at your option) any later version.                                             //
//                                                                                      //
//      Liberal Crime Squad is distributed in the hope that it will be useful,          //
//      but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the                  //
//      GNU General Public License for more details.                                    //
//                                                                                      //
//      You should have received a copy of the GNU General Public License               //
//      along with Liberal Crime Squad; if not, write to the Free Software              //
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   02111-1307   USA     //
//////////////////////////////////////////////////////////////////////////////////////////

//outstanding issues
   //site trucker-type bug still happens (latte-stand)
      //might have to do with missing location in the verifyworklocation() loop
         //but cannot locate instance of this
   //can have hostage in court?

//changes 3.10 alpha 1
   //added AZERTY keyboard support
   //added bullet-proof vests
   //added SWAT units
   //reduced chance of hitting bystanders
   //added multiple hit chances for automatic weapons
   //added extra hit probability for shotguns
   //removed sever types from handguns and knives
   //fixed bug in newspaper site emphasis
   //reduced fence values for armor
   //sleepers can warn before raids
   //courts made more difficult
   //hacking harder, a little less rewarding
   //sleeper overhaul
   //dating conservatives, kidnapping on dates more risky
   //made it easier to maintain LCS integrity when someone high up is arrested
   //changed version variable to 31001

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

#include <includes.h>

Log gamelog; //The gamelog.
Log xmllog; // Log for xml errors or bad values.

CursesMoviest movie;
unsigned char bigletters[27][5][7][4];
unsigned char newstops[6][80][5][4];
unsigned char newspic[20][78][18][4];

extern char artdir[MAX_PATH_SIZE];

vector<configSiteMap *> sitemaps; // stores site map info read in from config file

bool multipleCityMode;
unsigned long seed[RNG_SIZE];

vector<ClipType *> cliptype;
vector<WeaponType *> weapontype;
vector<ArmorType *> armortype;
vector<LootType *> loottype;
vector<CreatureType *> creaturetype;
vector<VehicleType *> vehicletype;

template<class Type>
bool populate_from_xml(vector<Type*>& types,string file,Log& log);
bool populate_masks_from_xml(vector<ArmorType*>& masks,string file,Log& log);

long curcreatureid=0;
vector<Item *> groundloot;
vector<Location *> location;

vector<Vehicle *> vehicle;
char showcarprefs=1;

int oldMapMode=0; // -1 if we're using the old map generation functions.

siteblockst levelmap[MAPX][MAPY][MAPZ];

chaseseqst chaseseq;

char slogan[SLOGAN_LEN];

vector<Creature *> pool;

vector<squadst *> squad;
squadst *activesquad=NULL;
long cursquadid=0;

char disbanding=0;
int disbandtime=0;
char cantseereason;

short activesortingchoice[SORTINGCHOICENUM];

Creature encounter[ENCMAX];


char loaded=0;

short mode=GAMEMODE_TITLE;

short offended_corps=0;
short offended_cia=0;
short offended_amradio=0;
short offended_cablenews=0;
short offended_firemen=0;
int police_heat=0;
unsigned long attorneyseed[RNG_SIZE];
int selectedsiege=-1;
char lcityname[CITY_NAMELEN];
char newscherrybusted=0;

int month=1;

#ifdef THEFUTURE
int year=2100;
#else
int year=2009;
#endif
int amendnum=28;

bool termlimits=false;
bool deagle=false;
bool m249=false;

UniqueCreatures uniqueCreatures;

short attitude[VIEWNUM];

short public_interest[VIEWNUM];
short background_liberal_influence[VIEWNUM];


short law[LAWNUM];

short house[HOUSENUM];
short senate[SENATENUM];
short court[COURTNUM];
char courtname[COURTNUM][POLITICIAN_NAMELEN];


signed char exec[EXECNUM];
short execterm=1;
char execname[EXECNUM][POLITICIAN_NAMELEN];
short presparty=CONSERVATIVE_PARTY;
char oldPresidentName[POLITICIAN_NAMELEN];

int stat_recruits=0;
int stat_kidnappings=0;
int stat_dead=0;
int stat_kills=0;
int stat_buys=0;
int stat_burns=0;

int ustat_recruits=0;
int ustat_kidnappings=0;
int ustat_dead=0;
int ustat_kills=0;
int ustat_funds=0;
int ustat_spent=0;
int ustat_buys=0;
int ustat_burns=0;

int locx;
int locy;
int locz;

short sitetype;
short sitealienate;
short sitealarm;
short sitealarmtimer;
short postalarmtimer;
short siteonfire;
int sitecrime;
short cursite;
bool mapshowing=false;

char encounterwarnings=0;

char foughtthisround=0;

short interface_pgup='[';
short interface_pgdn=']';

bool autosave=true;

int day=1;

class Ledger ledger;

short party_status=-1;

short wincondition=WINCONDITION_ELITE;

bool notermlimit;           //These determine if ELAs can take place --kviiri
bool nocourtpurge;
bool stalinmode;

char endgamestate=ENDGAME_NONE;
char ccsexposure=CCSEXPOSURE_NONE;
char ccs_kills=0;
int ccs_siege_kills=0;
int ccs_boss_kills=0;

vector<datest *> date;
vector<recruitst *> recruit;

vector<newsstoryst *> newsstory;
newsstoryst *sitestory=NULL;

#define SCORENUM 5
highscorest score[SCORENUM];
int yourscore=-1;

#ifdef WIN32
bool fixcleartype=false;
#endif

int main(int argc, char* argv[])
{
   init_console(); // do this FIRST
   //start curses
   initscr();

   gamelog.initialize(GAMELOG_FILEPATH, OVERWRITE_GAMELOG, NEWLINEMODE_GAMELOG); //Initialize the gamelog.

   //For formatting.
   //To let the user know a new instance of the program was started.
   //TODO: Make this output the current date and time so that the user knows when they
   //did the play session that follows.
   gamelog.log("\n\n\n---------- PROGRAM STARTED ----------\n\n\n");

   // set window title
   char wtitle[50];
   strcpy(wtitle,"Liberal Crime Squad ");
   strcat(wtitle,PACKAGE_VERSION);
   set_title (wtitle);

   noecho();

   //initialize curses color
   start_color();

   initMainRNG();

   //initialize the array of color pairs
   for(int i=0;i<8;i++)
      for(int j=0;j<8;j++)
      {
         if(i==0&&j==0)
         {
            init_pair(7*8,0,0);
            continue;
         }
         if(i==7&&j==0) continue;
         init_pair(i*8+j,i,j);
      }

   //turns off cursor
   curs_set(0);

   //begin the game loop
   keypad(stdscr,TRUE);

   raw_output(TRUE);

   //addstr("Loading Graphics... ");
   //getkey();

   loadgraphics();

   //addstr("Loading Init File Options... ");
   //getkey();

   loadinitfile();

   //addstr("Loading sitemaps.txt... ");
   //getkey();

   oldMapMode=!readConfigFile("sitemaps.txt"); // load site map data
   if (oldMapMode)
   {
      addstr("Failed to load sitemaps.txt! Reverting to old map mode.");

      getkey();
   }

   //move(1,0);
   //addstr("Setting initial game data... ");
   //getkey();

   strcpy(slogan,"We need a slogan!");
   if(!LCSrandom(20))
   {
      switch(LCSrandom(7))
      {
      case 0: strcpy(slogan,"To Rogues and Revolution!"); break;
      case 1: strcpy(slogan,"Hell yes, LCS!"); break;
      case 2: strcpy(slogan,"Striking high, standing tall!"); break;
      case 3: strcpy(slogan,"Revolution never comes with a warning!"); break;
      case 4: strcpy(slogan,"True Liberal Justice!"); break;
      case 5: strcpy(slogan,"Laissez ain't fair!"); break;
      case 6: strcpy(slogan,"This is a slogan!"); break;
      }
   }

   //Initialize sorting choices.
   for(int s=0;s<SORTINGCHOICENUM;s++)
      activesortingchoice[s]=SORTING_NONE;

   for(int v=0;v<VIEWNUM;v++)
   {
      attitude[v]=30+LCSrandom(25);
      public_interest[v]=0;
      background_liberal_influence[v]=0;
   }
   attitude[VIEW_LIBERALCRIMESQUAD]=0;
   attitude[VIEW_LIBERALCRIMESQUADPOS]=5;
   //attitude[VIEW_POLITICALVIOLENCE]=5;

#ifdef REVOLUTIONNOW
   for(int v=0;v<VIEWNUM;v++)
      attitude[v]=100;
#endif

   law[LAW_ABORTION]=1;
   law[LAW_ANIMALRESEARCH]=-1;
   law[LAW_POLICEBEHAVIOR]=-1;
   law[LAW_PRIVACY]=-1;
   law[LAW_DEATHPENALTY]=-1;
   law[LAW_NUCLEARPOWER]=-1;
   law[LAW_POLLUTION]=-1;
   law[LAW_LABOR]=0;
   law[LAW_GAY]=1;
   law[LAW_CORPORATE]=0;
   law[LAW_FREESPEECH]=0;
   law[LAW_FLAGBURNING]=1;
   law[LAW_GUNCONTROL]=-1;
   law[LAW_TAX]=0;
   law[LAW_WOMEN]=1;
   law[LAW_CIVILRIGHTS]=1;
   law[LAW_DRUGS]=-1;
   law[LAW_IMMIGRATION]=0;
   law[LAW_ELECTIONS]=0;
   law[LAW_MILITARY]=-1;
   law[LAW_PRISONS]=0;
   law[LAW_TORTURE]=-1;

#ifdef SHITLAWS
   for(int l=0;l<LAWNUM;l++) law[l]=-2;
#endif

#ifdef PERFECTLAWS
   for(int l=0;l<LAWNUM;l++) law[l]=2;
#endif

   for(int s=0;s<SENATENUM;s++)
   {
      if(s<25) senate[s]=-2;
      else if(s<60) senate[s]=-1;
      else if(s<80) senate[s]=0;
      else if(s<95) senate[s]=1;
      else senate[s]=2;
   }

   for(int h=0;h<HOUSENUM;h++)
   {
      if(h<50) house[h]=-2;
      else if(h<250) house[h]=-1;
      else if(h<350) house[h]=0;
      else if(h<400) house[h]=1;
      else house[h]=2;
   }

   for(int c=0;c<COURTNUM;c++)
   {
      if(c<3) court[c]=-2;
      else if(c<5) court[c]=-1;
      else if(c<5) court[c]=0;
      else if(c<8) court[c]=1;
      else court[c]=2;
      do
      {
         if(court[c]==-2) generate_name(courtname[c],GENDER_WHITEMALEPATRIARCH);
         else generate_name(courtname[c]);
      } while(len(courtname[c])>20);
   }

   for(int e=0;e<EXECNUM;e++)
   {
      exec[e]=-2;
      generate_name(execname[e],GENDER_WHITEMALEPATRIARCH);
   }

   initOtherRNG(attorneyseed);
   strcpy(lcityname,cityname());

   xmllog.initialize("xmllog",true,1);
   bool xml_loaded_ok = true;
   xml_loaded_ok&=populate_from_xml(vehicletype,"vehicles.xml",xmllog);
   xml_loaded_ok&=populate_from_xml(cliptype,"clips.xml",xmllog);
   xml_loaded_ok&=populate_from_xml(weapontype,"weapons.xml",xmllog);
   xml_loaded_ok&=populate_from_xml(armortype,"armors.xml",xmllog);
   xml_loaded_ok&=populate_masks_from_xml(armortype,"masks.xml",xmllog);
   xml_loaded_ok&=populate_from_xml(loottype,"loot.xml",xmllog);
   xml_loaded_ok&=populate_from_xml(creaturetype,"creatures.xml",xmllog);
   if(!xml_loaded_ok) end_game();

   //addstr("Attempting to load saved game... ");
   //getkey();

   loaded=load();

   //addstr("Setup complete!");
   //getkey();

   clear();

   mode_title();

   //deinitialize curses
   end_game();

   return 0;
}

/* Free memory and exit the game */
void end_game(int err)
{
   #ifdef WIN32
   end_cleartype_fix(); // won't do anything unless fixcleartype is true
   #endif

   delete_and_clear(location);
   delete_and_clear(squad);
   delete_and_clear(weapontype);
   delete_and_clear(cliptype);
   delete_and_clear(armortype);
   delete_and_clear(loottype);
   delete_and_clear(creaturetype);
   delete_and_clear(vehicletype);
   delete_and_clear(vehicle);
   delete_and_clear(pool);
   delete_and_clear(sitemaps);
   delete_and_clear(recruit);
   delete_and_clear(date);
   delete_and_clear(groundloot);

   endwin();
   exit(err);
}

template<class Type>
bool populate_from_xml(vector<Type*>& types,string file,Log& log)
{
   CMarkup xml;
   if(!xml.Load(string(artdir)+file))
   { // File is missing or not valid XML.
      addstr("Failed to load "+file+"!");
      log.log("Failed to load "+file+"!");

      getkey();

      // Will cause abort here or else if file is missing all unrecognized types
      // loaded from a saved game will be deleted. Also, you probably don't want
      // to play with a whole category of things missing anyway. If the file
      // does not have valid xml, then behaviour is kind of undefined so it's
      // best to abort then too.
      return false;
   }

   xml.FindElem();
   xml.IntoElem();
   while(xml.FindElem()) types.push_back(new Type(xml.GetSubDoc()));
   return true;
}

bool populate_masks_from_xml(vector<ArmorType*>& masks,string file,Log& log)
{
   CMarkup xml;
   if(!xml.Load(string(artdir)+file))
   { //File is missing or not valid XML.
      addstr("Failed to load "+file+"!",log);

      getkey();

      return false; //Abort.
   }

   xml.FindElem();
   xml.IntoElem();
   int defaultindex;
   if(xml.FindElem("default")) defaultindex=getarmortype(xml.GetData());
   else
   {
      addstr("Default missing for masks!",log);

      getkey();

      return false; //Abort.
   }
   if(defaultindex==-1)
   {
      addstr("Default for masks is not a known armor type!",log);

      getkey();

      return false; //Abort.
   }

   xml.ResetMainPos();
   while(xml.FindElem("masktype")) armortype.push_back(new ArmorType(*armortype[defaultindex],xml.GetSubDoc()));
   return true;
}
