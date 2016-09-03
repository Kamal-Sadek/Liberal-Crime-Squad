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

#include <includeDefault.h>
//#include "configfile.h"
//#include "tinydir.h"
#include <includeEnum.h>
#include <includeCommon.h>

/*
translateid.cpp
*/
#include "common\\translateid.h"

/*
stringconversion.cpp
*/
#include "common\\stringconversion.h"

/*
consolesupport.cpp
*/
#include "common\\consolesupport.h"

#include <includeTitle.h>
//#include <includeNews.h>
//#include <includeFunctions.h>
#include <ctime>

Log gamelog; //The gamelog.
Log xmllog; // Log for xml errors or bad values.

CursesMoviest movie;
unsigned char bigletters[27][5][7][4];
unsigned char newstops[6][80][5][4];
unsigned char newspic[20][78][18][4];

MusicClass music;

char homedir[MAX_PATH_SIZE];
char artdir[MAX_PATH_SIZE];

vector<configSiteMap *> sitemaps; // stores site map info read in from config file

bool multipleCityMode;
unsigned long seed[RNG_SIZE];

vector<ClipType *> cliptype;
vector<WeaponType *> weapontype;
vector<ArmorType *> armortype;
vector<LootType *> loottype;
vector<CreatureType *> creaturetype;
vector<AugmentType *> augmenttype;
vector<VehicleType *> vehicletype;

vector<vector<string>> pickupLines;
vector<vector<string>> no_free_speech_flirt;
vector<vector<string>> lovingly_talk_to_mutant;
vector<vector<string>> normal_talk_to_mutant;
vector<vector<string>> lovingly_talk_to_dog;
vector<vector<string>> normal_talk_to_dog;
vector<vector<string>> real_quote;
vector<string> escape_running;
vector<string> escape_crawling;
vector<string> judge_debate;
vector<string> conservative_ceo_debate;
vector<string> other_ceo_debate;
vector<string> conservative_politician_debate;
vector<string> other_politician_debate;
vector<string> media_debate;
vector<string> military_debate;
vector<string> police_debate;
vector<string> dog_rejection;
vector<string> mutant_rejection;
vector<string> that_is_disturbing;
vector<string> that_is_not_disturbing;

vector<string> execution;
vector<string> feels_sick;
vector<string> low_heart_torture_props;
vector<string> screaming;
vector<string> beat_with_props;
vector<string> words_meaning_screaming;
vector<string> words_to_scream;
vector<string> prays;
vector<string> prays_on_drugs;
vector<string> use_props;
vector<string> resist_drugs;
vector<string> cry_alarm;

vector<string> bleeding_to_death;
vector<string> stunned_text;
vector<string> paralyzed_text;
vector<string> paralyzed_tank;
vector<string> bleeding_animal;
vector<vector<string>> double_line_death;
vector<string> body_falls_apart;

vector<string> robbing_bank;
vector<string> teller_gestures;
vector<string> teller_complies;

vector<string> default_slogans;

vector<string> come_at_me_bro;
vector<string> backs_off;
vector<string> threaten_hostage;
vector<string> please_spare_hostage;
vector<string> who_cares_about_hostage;
vector<string> hostage_negotiation;
vector<string> please_no_more;
vector<string> let_hostages_go;
vector<string> go_ahead_and_die;
vector<string> agree_to_release_hostages;

vector<string> partial_conversion;
vector<string> broods_over_death;
vector<string> self_wounding;

template<class Type>
bool populate_from_xml(vector<Type*>& types,string file,Log& log);
bool populate_from_txt(vector<vector<string>>& types, string file, int dimension, Log& log);
bool populate_from_txt(vector<string> & types, string fileName, Log& log);
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

Alignment law[LAWNUM];

Alignment house[HOUSENUM];
Alignment senate[SENATENUM];
Alignment court[COURTNUM];
char courtname[COURTNUM][POLITICIAN_NAMELEN];

Alignment exec[EXECNUM];
short execterm=1;
char execname[EXECNUM][POLITICIAN_NAMELEN];
PoliticalParties presparty=CONSERVATIVE_PARTY;
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

bool encounterwarnings=false;

char foughtthisround=0;

short interface_pgup='[';
short interface_pgdn=']';

bool autosave=false;
string savefile_name;

int day=1;

class Ledger ledger;

short party_status=-1;

short wincondition=WINCONDITION_ELITE;
short fieldskillrate=FIELDSKILLRATE_FAST;

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

   gamelog.initialize(GAMELOG_FILEPATH, OVERWRITE_GAMELOG, NEWLINEMODE_GAMELOG); //Initialize the gamelog (and also initialize artdir and homedir)

   time_t t = time(0);
   struct tm *now = localtime(&t); //Do not need to deallocate this. Statically allocated by system
   char datetime[41];
   sprintf(datetime, "---------%i-%02i-%02i %02i:%02i:%02i---------\n\n\n", 
      now->tm_year+1900, now->tm_mon+1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec); //YYYY-MM-DD HH:MM:SS format

   gamelog.log(string("\n\n\n---------- PROGRAM STARTED ----------\n") + datetime);



   char file_name[13];
   FILE *file;

   music.play(MUSIC_TITLEMODE); // initialize music and play title mode song (do this BEFORE displaying anything on the screen, but AFTER initializing artdir and homedir)

   // set window title
   char wtitle[50];
   strcpy(wtitle,"Liberal Crime Squad ");
   strcat(wtitle,PACKAGE_VERSION);
   set_title(wtitle);

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
   if(oldMapMode)
   {
      addstr("Failed to load sitemaps.txt! Reverting to old map mode.",gamelog);
      gamelog.nextMessage();

      getkey();
   }

   //move(1,0);
   //addstr("Setting initial game data... ");
   //getkey();

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

   law[LAW_ABORTION]= ALIGN_LIBERAL;
   law[LAW_ANIMALRESEARCH]= ALIGN_CONSERVATIVE;
   law[LAW_POLICEBEHAVIOR]= ALIGN_CONSERVATIVE;
   law[LAW_PRIVACY]= ALIGN_CONSERVATIVE;
   law[LAW_DEATHPENALTY]= ALIGN_CONSERVATIVE;
   law[LAW_NUCLEARPOWER]= ALIGN_CONSERVATIVE;
   law[LAW_POLLUTION]= ALIGN_CONSERVATIVE;
   law[LAW_LABOR]= ALIGN_MODERATE;
   law[LAW_GAY]= ALIGN_LIBERAL;
   law[LAW_CORPORATE]= ALIGN_MODERATE;
   law[LAW_FREESPEECH]= ALIGN_MODERATE;
   law[LAW_FLAGBURNING]= ALIGN_LIBERAL;
   law[LAW_GUNCONTROL]= ALIGN_CONSERVATIVE;
   law[LAW_TAX]= ALIGN_MODERATE;
   law[LAW_WOMEN]= ALIGN_LIBERAL;
   law[LAW_CIVILRIGHTS]= ALIGN_LIBERAL;
   law[LAW_DRUGS]= ALIGN_CONSERVATIVE;
   law[LAW_IMMIGRATION]= ALIGN_MODERATE;
   law[LAW_ELECTIONS]= ALIGN_MODERATE;
   law[LAW_MILITARY]= ALIGN_CONSERVATIVE;
   law[LAW_PRISONS]= ALIGN_MODERATE;
   law[LAW_TORTURE]= ALIGN_CONSERVATIVE;

#ifdef SHITLAWS
   for(int l=0;l<LAWNUM;l++) law[l]=-2;
#endif

#ifdef PERFECTLAWS
   for(int l=0;l<LAWNUM;l++) law[l]=2;
#endif

   for(int s=0;s<SENATENUM;s++)
   {
      if(s<25) senate[s]= ALIGN_ARCHCONSERVATIVE;
      else if(s<60) senate[s]= ALIGN_CONSERVATIVE;
      else if(s<80) senate[s]= ALIGN_MODERATE;
      else if(s<95) senate[s]= ALIGN_LIBERAL;
      else senate[s]= ALIGN_ELITELIBERAL;
   }

   for(int h=0;h<HOUSENUM;h++)
   {
      if(h<50) house[h]= ALIGN_ARCHCONSERVATIVE;
      else if(h<250) house[h]= ALIGN_CONSERVATIVE;
      else if(h<350) house[h]= ALIGN_MODERATE;
      else if(h<400) house[h]= ALIGN_LIBERAL;
      else house[h]= ALIGN_ELITELIBERAL;
   }

   for(int c=0;c<COURTNUM;c++)
   {
      if(c<3) court[c]= ALIGN_ARCHCONSERVATIVE;
      else if(c<5) court[c]= ALIGN_CONSERVATIVE;
      else if(c<5) court[c]= ALIGN_MODERATE;
      else if(c<8) court[c]= ALIGN_LIBERAL;
      else court[c]= ALIGN_ELITELIBERAL;
      do
      {
         if(court[c]==-2) generate_name(courtname[c],GENDER_WHITEMALEPATRIARCH);
         else generate_name(courtname[c]);
      } while(len(courtname[c])>20);
   }

   for(int e=0;e<EXECNUM;e++)
   {
      exec[e]= ALIGN_ARCHCONSERVATIVE;
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
   xml_loaded_ok&=populate_from_xml(augmenttype,"augmentations.xml",xmllog);


   xml_loaded_ok &= populate_from_txt(default_slogans, "default_slogans.txt", xmllog);

   //IsaacG
   // the number five (5) here indicates how many lines a "pickup line" is
   // 2 lines for the initial pickup,
   // 1 line for accepting the proposal,
   // and 2 lines for rejecting
   // 
   /*talk.cpp*/
   string talk = "talk\\";
   xml_loaded_ok&=populate_from_txt(pickupLines, talk + "pickupLines.txt", 5, xmllog);
   xml_loaded_ok &= populate_from_txt(no_free_speech_flirt, talk + "no_free_speech_flirt.txt", 5, xmllog);
   xml_loaded_ok &= populate_from_txt(lovingly_talk_to_mutant, talk + "lovingly_talk_to_mutant.txt", 2, xmllog);
   xml_loaded_ok &= populate_from_txt(normal_talk_to_mutant, talk + "normal_talk_to_mutant.txt", 2, xmllog);
   xml_loaded_ok &= populate_from_txt(lovingly_talk_to_dog, talk + "lovingly_talk_to_dog.txt", 2, xmllog);
   xml_loaded_ok &= populate_from_txt(normal_talk_to_dog, talk + "normal_talk_to_dog.txt", 2, xmllog);
   xml_loaded_ok &= populate_from_txt(dog_rejection, talk + "dog_rejection.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(mutant_rejection, talk + "mutant_rejection.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(that_is_disturbing, talk + "that_is_disturbing.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(that_is_not_disturbing, talk + "that_is_not_disturbing.txt", xmllog);

   xml_loaded_ok &= populate_from_txt(robbing_bank, talk + "robbing_bank.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(teller_gestures, talk + "teller_gestures.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(teller_complies, talk + "teller_complies.txt", xmllog);
   //talk in combat
   string talk_combat = "talk_combat\\";
   xml_loaded_ok &= populate_from_txt(come_at_me_bro, talk_combat + "come_at_me_bro.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(backs_off, talk_combat + "backs_off.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(threaten_hostage, talk_combat + "threaten_hostage.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(please_spare_hostage, talk_combat + "please_spare_hostage.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(who_cares_about_hostage, talk_combat + "who_cares_about_hostage.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(hostage_negotiation, talk_combat + "hostage_negotiation.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(please_no_more, talk_combat + "please_no_more.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(let_hostages_go, talk_combat + "let_hostages_go.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(go_ahead_and_die, talk_combat + "go_ahead_and_die.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(agree_to_release_hostages, talk_combat + "agree_to_release_hostages.txt", xmllog);

   /*titlescreen.cpp*/
   string title = "titlescreen\\";
   xml_loaded_ok &= populate_from_txt(real_quote, title + "real_quote.txt", 4, xmllog);

   

   /*fight.cpp*/
   string fight = "fight\\";
   xml_loaded_ok&=populate_from_txt(escape_running, fight + "escape_running.txt", xmllog);
   xml_loaded_ok&=populate_from_txt(escape_crawling, fight + "escape_crawling.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(judge_debate, fight + "judge_debate.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(conservative_ceo_debate, fight + "conservative_ceo_debate.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(other_ceo_debate, fight + "other_ceo_debate.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(conservative_politician_debate, fight + "conservative_politician_debate.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(other_politician_debate, fight + "other_politician_debate.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(media_debate, fight + "media_debate.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(military_debate, fight + "military_debate.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(police_debate, fight + "police_debate.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(cry_alarm, fight + "cry_alarm.txt", xmllog);

   xml_loaded_ok &= populate_from_txt(bleeding_to_death, fight + "bleeding_to_death.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(stunned_text, fight + "stunned_text.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(paralyzed_text, fight + "paralyzed_text.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(paralyzed_tank, fight + "paralyzed_tank.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(bleeding_animal, fight + "bleeding_animal.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(double_line_death, fight + "double_line_death.txt", 2, xmllog);
   xml_loaded_ok &= populate_from_txt(body_falls_apart, fight + "body_falls_apart.txt", xmllog);

   /*interrogation.cpp*/
   string interrogate = "interrogation\\";
   xml_loaded_ok &= populate_from_txt(execution, interrogate + "execution.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(feels_sick, interrogate + "feels_sick.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(low_heart_torture_props, interrogate + "low_heart_torture_props.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(screaming, interrogate + "screaming.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(beat_with_props, interrogate + "beat_with_props.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(words_meaning_screaming, interrogate + "words_meaning_screaming.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(words_to_scream, interrogate + "words_to_scream.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(prays, interrogate + "prays.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(prays_on_drugs, interrogate + "prays_on_drugs.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(use_props, interrogate + "use_props.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(resist_drugs, interrogate + "resist_drugs.txt", xmllog);

   xml_loaded_ok &= populate_from_txt(partial_conversion, interrogate + "partial_conversion.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(broods_over_death, interrogate + "broods_over_death.txt", xmllog);
   xml_loaded_ok &= populate_from_txt(self_wounding, interrogate + "self_wounding.txt", xmllog);

   //IsaacG
   // Moved this line so that it occurs after the custom text is acquired
   strcpy(slogan, "We need a slogan!");
   if (!LCSrandom(20))
   {
	   strcpy(slogan, pickrandom(default_slogans));
   }


   if(!xml_loaded_ok) end_game(EXIT_FAILURE);
   //TODO procedural String
   //(string(artdir)+file))

   //addstr("Attempting to load saved game... ");
   //getkey();

   //addstr("Setup complete!");
   //getkey();

   clear();

   mode_title();

   //deinitialize curses
   end_game();

   return EXIT_SUCCESS;
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
   delete_and_clear(augmenttype);
   delete_and_clear(vehicletype);
   delete_and_clear(vehicle);
   delete_and_clear(pool);
   delete_and_clear(sitemaps);
   delete_and_clear(recruit);
   delete_and_clear(date);
   delete_and_clear(groundloot);
   // IsaacG
   // TODO Figure out what the problem is here
   //delete_and_clear(pickupLines);

   music.quit(); // shut down music

   endwin();
   exit(err);
}

template<class Type>
bool populate_from_xml(vector<Type*>& types,string file,Log& log)
{
   CMarkup xml;
   if(!xml.Load(string(artdir)+file))
   { // File is missing or not valid XML.
      addstr("Failed to load "+file+"!",log);

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

// populate_from_text extracts plaintext and puts it in a two dimensional vector of strings.
// returns true if file loads, false if not.

bool populate_from_txt(vector< vector<string> >& types, string fileName, int dimension, Log& log)
{
	 
	ifstream txtFile;
	char currentLine[90];
	txtFile.open(string(artdir) + fileName);
	if (txtFile.fail()) {
		addstr("Failed to load " + fileName + "!", log);
		cout << endl;
		getkey();
		return false;
	}
	//cout << endl;
	while (!txtFile.eof()) {
		txtFile.getline(currentLine, 90, '\n');
		/*if (!currentLine[0]) {
			cout << "blank" << endl;
		}
		if (currentLine[0] == '\n') {
			cout << "newLine" << endl;
		}*/
		bool notComment = (currentLine[0] && currentLine[0] != '#');
		if (notComment) {
			vector<string> line;
			line.push_back(currentLine);
			//cout << currentLine << endl;
			//types[whichPickup][whichLine] = string(currentLine);

			for (int i = 0; i < dimension - 1 && !txtFile.eof(); i++) {
				// whether the line is not a comment, and is also the line whose contents are optional
					// Testing until obtain proper data from file
					txtFile.getline(currentLine, 90, '\n');
					line.push_back(currentLine);
					//cout << currentLine << endl;
					//types[whichPickup][whichLine] = string(currentLine);
					if (currentLine[0] == '#') {
						cout << "Comment Found" << endl << currentLine << endl;
						getkey();

					}
				
			}
			types.push_back(line);
		}
	}
	return true;
	}

bool populate_from_txt(vector<string> & types, string fileName, Log& log)
{

	ifstream txtFile;
	char currentLine[90];
	txtFile.open(string(artdir) + fileName);
	if (txtFile.fail()) {
		addstr("Failed to load " + fileName + "!", log);

		getkey();
		return false;
	}
	cout << endl;
	while (!txtFile.eof()) {
		txtFile.getline(currentLine, 90, '\n');
		/*if (!currentLine[0]) {
		cout << "blank" << endl;
		}
		if (currentLine[0] == '\n') {
		cout << "newLine" << endl;
		}*/
		bool notComment = (currentLine[0] && currentLine[0] != '#');
		if (notComment) {
			types.push_back(currentLine);
		}
	}
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
