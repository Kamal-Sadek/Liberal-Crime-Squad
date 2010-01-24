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

#include "includes.h"
#include "configfile.h"
#include "sitemode/sitemap.h"
#include <iostream>

CursesMoviest movie;
unsigned char bigletters[27][5][7][4];
unsigned char newstops[6][80][5][4];
unsigned char newspic[20][78][18][4];

vector<configSiteMap *> sitemaps; // stores site map info read in from config file

unsigned int seed;

long curcreatureid=0;
vector<itemst *> groundloot;
vector<locationst *> location;


vector<vehiclest *> vehicle;
long curcarid=0;
char showcarprefs=1;

int oldMapMode=0; // -1 if we're using the old map generation functions.

siteblockst levelmap[MAPX][MAPY][MAPZ];

chaseseqst chaseseq;

char slogan[SLOGAN_LEN+1];

vector<Creature *> pool;

vector<squadst *> squad;
squadst *activesquad=NULL;
long cursquadid=0;

char disbanding=0;
int disbandtime=0;
char cantseereason;


Creature encounter[ENCMAX];


char loaded=0;

int mode=GAMEMODE_TITLE;

short offended_cops=0;
short offended_corps=0;
short offended_cia=0;
short offended_amradio=0;
short offended_cablenews=0;
short offended_firemen=0;
int police_heat=0;
int attorneyseed;
int selectedsiege=-1;
char lcityname[80];
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

short attitude[VIEWNUM];

short public_interest[VIEWNUM];
short background_liberal_influence[VIEWNUM];


short law[LAWNUM];

short house[435];
short senate[100];
short court[9];
char courtname[9][80];


signed char exec[EXECNUM];
short execterm=1;
char execname[EXECNUM][80];
short presparty=1;

char amradio_closed=0;
char cablenews_closed=0;
char policestation_closed=0;


int stat_recruits=0;
int stat_kidnappings=0;
int stat_dead=0;
int stat_kills=0;
int stat_funds=0;
int stat_spent=0;
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

short interface_pgup='[';
short interface_pgdn=']';

int day=1;

class Ledger ledger;

short party_status=-1;

char endgamestate=ENDGAME_NONE;
char ccs_kills=0;
int ccs_siege_kills=0;

vector<datest *> date;
vector<recruitst *> recruit;

vector<newsstoryst *> newsstory;
newsstoryst *sitestory=NULL;

#define SCORENUM 5
highscorest score[SCORENUM];
int yourscore=-1;

int main(int argc, char* argv[])
{
   //start curses
#ifdef CH_USE_UNICODE
   setup_unicode();
#endif
   initscr();

   noecho();

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
   
   //addstr("Loading Graphics... ");
   //refresh();
   //getch();

   loadgraphics();
   
   //addstr("Loading Init File Options... ");
   //refresh();
   //getch();

   loadinitfile();
   
   //addstr("Loading sitemaps.txt... ");
   //refresh();
   //getch();

   oldMapMode=readConfigFile("sitemaps.txt"); // load site map data
   if (oldMapMode==-1)
   {
      addstr("Failed to load sitemaps.txt! Reverting to old map mode.");
      refresh();
      getch();
   }
   
   //move(1,0);
   //addstr("Setting initial game data... ");
   //refresh();
   //getch();

   strcpy(slogan,"We need a slogan!");
   if(!LCSrandom(20))
   {
      switch(LCSrandom(5))
      {
      case 0:strcpy(slogan,"To Rogues and Revolution!");break;
      case 1:strcpy(slogan,"Lob the light grenades that burst and blind with truth!");break;
      case 2:strcpy(slogan,"No serenade, no fire brigade, just pyromania!");break;
      case 3:strcpy(slogan,"Revolution never comes with a warning!");break;
      case 4:strcpy(slogan,"If you're not affected, you're not paying attention!");break;
      }
   }

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
   {
      attitude[v]=100;
   }
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
   law[LAW_GUNCONTROL]=-1;
   law[LAW_WOMEN]=1;
   law[LAW_CIVILRIGHTS]=1;
   law[LAW_DRUGS]=-1;
   law[LAW_IMMIGRATION]=0;
   law[LAW_ELECTIONS]=0;
   law[LAW_MILITARY]=-1;
   law[LAW_TORTURE]=-1;

#ifdef SHITLAWS
   for(int l=0;l<LAWNUM;l++)law[l]=-2;
#endif

#ifdef PERFECTLAWS
   for(int l=0;l<LAWNUM;l++)law[l]=2;
#endif

   for(int s=0;s<100;s++)
   {
      if(s<25)senate[s]=-2;
      else if(s<60)senate[s]=-1;
      else if(s<80)senate[s]=0;
      else if(s<95)senate[s]=1;
      else senate[s]=2;
   }

   for(int h=0;h<435;h++)
   {
      if(h<50)house[h]=-2;
      else if(h<250)house[h]=-1;
      else if(h<350)house[h]=0;
      else if(h<400)house[h]=1;
      else house[h]=2;
   }

   for(int c=0;c<9;c++)
   {
      if(c<3)court[c]=-2;
      else if(c<5)court[c]=-1;
      else if(c<5)court[c]=0;
      else if(c<8)court[c]=1;
      else court[c]=2;
      generate_name(courtname[c]);
   }

   for(int e=0;e<EXECNUM;e++)
   {
      exec[e]=-2;
      generate_name(execname[e],GENDER_WHITEMALEPATRIARCH);
   }

   attorneyseed=getSeed();
   cityname(lcityname);
   
   //addstr("Attempting to load saved game... ");
   //refresh();
   //getch();

   loaded=load();
   
   //addstr("Setup complete!");
   //refresh();
   //getch();

   clear();

   mode_title();

   //deinitialize curses
   end_game();

   return 0;
}

//picks a random number from 0 to max-1
int LCSrandom(int max)
{
   r_num();

   long double rand_y;
   long double rand_i;

   rand_i = 2147483648UL;

   rand_y = max*((long double)seed/rand_i);

   return((int)rand_y);
}

//sets seed to a random number from 0 to 2 billion
int r_num(void)
{
   seed=(seed*907725L+99979777UL)%2147483648UL;
   return seed;
}

int Creature::attval(short a,char usejuice)
{
   int ret=att[a];
   
   if(a==ATTRIBUTE_WISDOM && align!=ALIGN_CONSERVATIVE)usejuice=false;
   if(a==ATTRIBUTE_HEART  && align!=ALIGN_LIBERAL)usejuice=false;

   if(usejuice)
   {
      if(juice<=-50)ret=1;
      else if(juice<=-10)ret=static_cast<int>(ret*0.6);
      else if(juice<0)ret=static_cast<int>(ret*0.8);
      else if(juice>=10)
      {
         if(juice<50)ret=static_cast<int>(ret+=1);
         else if(juice<100)ret=static_cast<int>(ret*1.1+2);
         else if(juice<200)ret=static_cast<int>(ret*1.2+3);
         else if(juice<500)ret=static_cast<int>(ret*1.3+4);
         else if(juice<1000)ret=static_cast<int>(ret*1.4+5);
         else ret=static_cast<int>(ret*1.5+6);
      }
      if(ret<1)ret=1;
      if(ret>20)ret=20;
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

         if(age<11)ret>>=1;
         else if(age<16)ret-=1;
         else if(age>35)ret-=1;
         else if(age>52)ret-=3;
         else if(age>70)ret-=6;
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

         if(age<11)ret-=2;
         else if(age<16)ret-=1;
         else if(age>35)ret-=1;
         else if(age>52)ret-=3;
         else if(age>70)ret-=6;
         break;
      case ATTRIBUTE_HEALTH:
         if(special[SPECIALWOUND_NECK]!=1||
            special[SPECIALWOUND_UPPERSPINE]!=1)
         {
            ret=1;
         }
         else if(special[SPECIALWOUND_LOWERSPINE]!=1)ret>>=2;

         if(age<11)ret-=2;
         else if(age<16)ret-=1;
         break;
      case ATTRIBUTE_CHARISMA:
         ret-=disfigs;

         if(age<11)ret+=2; // yayay kids
         else if(age<16)ret-=1; // barf teenagers
         else if(age>35)ret+=1;
         else if(age>52)ret+=2;
         else if(age>70)ret+=3;
         break;
      case ATTRIBUTE_INTELLIGENCE:
         if(age<11)ret-=3;
         else if(age<16)ret-=1;
         else if(age>35)ret+=1;
         else if(age>52)ret+=2;
         else if(age>70)ret+=3;
         break;
      case ATTRIBUTE_WISDOM:
         if(age<11)ret-=2;
         else if(age<16)ret-=1;
         else if(age>52)ret+=1;
         else if(age>70)ret+=2;
         break;
      case ATTRIBUTE_HEART:
         if(age<11)ret+=2;
         else if(age<16)ret+=1;
         else if(age>52)ret-=1;
         else if(age>70)ret-=2;
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

void vehiclest::stop_riding_me()
{
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->carid==id)
      {
         pool[p]->carid=-1;
      }
   }
}

void vehiclest::init(int t)
{
   id=curcarid;curcarid++;

   heat=0;
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
      case VEHICLE_AGENTCAR:
         myear=year+1-LCSrandom(11);
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

void Creature::creatureinit(void)
{
   hireid=-1;
   worklocation=0;
   juice=0;
   flag=0;
   age=18+LCSrandom(40);
   gender_liberal = gender_conservative = LCSrandom(2) + 1;
   birthday_month=LCSrandom(12)+1;
   if(birthday_month==4 || birthday_month==6 ||
      birthday_month==9 || birthday_month==11)
   {
      birthday_day=LCSrandom(30)+1;
   }
   else if(birthday_month==2)
   {
      birthday_day=LCSrandom(28)+1;
   }
   else
   {
      birthday_day=LCSrandom(31)+1;
   }
   carid=-1;
   is_driver=0;
   pref_carid=-1;
   pref_is_driver=0;
   id=curcreatureid;
      curcreatureid++;
   joindays=0;
   deathdays=0;
   squadid=-1;
   cantbluff=0;
   location=0;
   base=0;
   activity.type=ACTIVITY_NONE;
   for(int i=0;i<LAWFLAGNUM;i++)
      crimes_suspected[i]=0;
   heat=0;
   confessions=0;
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
   stunned=0;
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

void locationst::init(void)
{
   haveflag=0;
   newrental=0;
   heat=0;
   heat_protection=0.0;
   closed=0;
   interrogated=0;
   highsecurity=0;
   mapseed=seed;r_num();
   changes.clear();
   compound_walls=0;
   compound_stores=0;
   front_business=-1;
}

void locationst::update_heat_protection(void)
{
   int l;
   for(l=0;l<location.size();l++)
   {
      if(location[l]==this)
         break;
   }
   if(l==location.size())
   {
      heat_protection=0.0;
      return;
   }
   int numpres=0;
   int heatprotection=0;
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->location!=l)continue; // People not at this base don't count
      if(!pool[p]->alive)continue; // Dead people don't count
      numpres++;
   }

   // Determine how effective your current safehouse
   // is at keeping the police confused
   switch(location[l]->type)
   {
   case SITE_INDUSTRY_WAREHOUSE:
      if(location[l]->front_business!=-1)
         heatprotection+=12; // Business front -- high protection
      else
         heatprotection+=0; // Abandoned warehouse -- no protection
      break;
   case SITE_RESIDENTIAL_SHELTER:
      heatprotection+=0; // Homeless shelter -- no protection
      break;
   case SITE_RESIDENTIAL_TENEMENT:
      heatprotection+=4; // Lower class housing -- low protection
      break;
   case SITE_RESIDENTIAL_APARTMENT:
      heatprotection+=8; // Middle class housing -- medium protection
      break;
   case SITE_RESIDENTIAL_BOMBSHELTER:
   case SITE_OUTDOOR_BUNKER:
   case SITE_BUSINESS_BARANDGRILL:
   case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
      heatprotection+=12; // Upper class housing -- high protection
      break;
   }

   if(law[LAW_FLAGBURNING]==-2&&location[l]->haveflag) {heatprotection+=6;} // More protection if the flag is sacred
   else if(law[LAW_FLAGBURNING]!=-2&&location[l]->haveflag) {heatprotection+=2;} // Some if the flag isn't
   else if(law[LAW_FLAGBURNING]==2&&!(location[l]->haveflag)) {heatprotection-=2;} // Lose some if it is and you have no flag
   else {heatprotection+=0;} // None if it isn't and you have no flag

   //Protection varies with how many people in the safehouse
   if(numpres>60)heatprotection-=20;
   if(numpres>40)heatprotection-=12;
   if(numpres>20)heatprotection-=6;
   if(numpres<10)heatprotection+=1;
   if(numpres<4)heatprotection+=2;
   if(numpres<2)heatprotection+=3;

   if(heatprotection<0)heatprotection=0;

   heat_protection=heatprotection*0.05;
   if(heat_protection>=1.0)
      heat_protection=0.95;
}

void chaseseqst::clean(void)
{
   for(int v=0;v<enemycar.size();v++)
   {
      delete enemycar[v];
   }
   enemycar.clear();

   friendcar.clear(); 
}


/* Free memory and exit the game */
void end_game(int err)
{
   for(vector<locationst*>::iterator it=location.begin();it!=location.end();++it)
   {
      delete (*it);
   }
   for(int i=0;i<squad.size();++i)
   {
      delete squad[i];
   }
   endwin();
   exit(err);
}

