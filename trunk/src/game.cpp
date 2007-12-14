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


CursesMoviest movie;
unsigned char bigletters[27][5][7][4];
unsigned char newstops[6][80][5][4];
unsigned char newspic[20][78][18][4];



uint32 seed;

int32 curcreatureid=0;
vector<itemst *> groundloot;
vector<locationst *> location;


vector<vehiclest *> vehicle;
int32 curcarid=0;
char showcarprefs=1;


siteblockst map[MAPX][MAPY][MAPZ];

chaseseqst chaseseq;

char slogan[SLOGAN_LEN+1];

vector<creaturest *> pool;

vector<squadst *> squad;
squadst *activesquad=NULL;
int32 cursquadid=0;

char disbanding=0;
int32 disbandtime=0;
char cantseereason;


creaturest encounter[ENCMAX];


char loaded=0;

int32 mode=GAMEMODE_TITLE;

int16 offended_cops=0;
int16 offended_corps=0;
int16 offended_cia=0;
int16 offended_amradio=0;
int16 offended_cablenews=0;
int32 police_heat=0;
uint32 attorneyseed;
int32 selectedsiege=-1;
char lcityname[80];
char newscherrybusted=0;

int32 month=1;
int32 year=2005;
int32 amendnum=28;


int16 attitude[VIEWNUM];

// Topicwork1 is editorials that are waiting to be written
// Topicwork2 is editorials that have been written
int16 newspaper_topicwork1[VIEWNUM];
int16 newspaper_topicwork2[VIEWNUM];


int16 law[LAWNUM];

int16 house[435];
int16 senate[100];
int16 court[9];
char courtname[9][80];


int16 exec[EXECNUM];
int16 execterm=1;
char execname[EXECNUM][80];


uint32 stat_recruits=0;
uint32 stat_kidnappings=0;
uint32 stat_dead=0;
uint32 stat_kills=0;
uint32 stat_funds=0;
uint32 stat_spent=0;
uint32 stat_buys=0;
uint32 stat_burns=0;

uint32 ustat_recruits=0;
uint32 ustat_kidnappings=0;
uint32 ustat_dead=0;
uint32 ustat_kills=0;
uint32 ustat_funds=0;
uint32 ustat_spent=0;
uint32 ustat_buys=0;
uint32 ustat_burns=0;

int32 locx,locy,locz;

int16 sitetype;
int16 sitealienate;
int16 sitealarm;
int16 sitealarmtimer;
int16 postalarmtimer;
int32 sitecrime;
int32 cursite;

int16 interface_pgup='[';
int16 interface_pgdn=']';

int32 day=1;

#ifdef HIGHFUNDS
uint32 funds=100000;
#else
uint32 funds=7;
#endif

int32 moneygained_donate=0;
int32 moneygained_brownies=0;
int32 moneygained_goods=0;
int32 moneygained_ccfraud=0;
int32 moneygained_hustling=0;
int32 moneygained_thievery=0;
int32 moneylost_goods=0;
int32 moneylost_trouble=0;
int32 moneylost_rent=0;
int32 moneylost_manufacture=0;
int32 moneylost_legal=0;
int32 moneylost_compound=0;
int32 moneylost_hostage=0;

int16 party_status=-1;

vector<datest *> date;



vector<newsstoryst *> newsstory;
newsstoryst *sitestory=NULL;


#define SCORENUM 5
highscorest score[SCORENUM];
int32 yourscore=-1;




int32 main(int32 argc, char* argv[])
{
   //start curses
   initscr();

   noecho();

   //initialize curses color
   start_color();

   seed=getSeed();

   //initialize the array of color pairs
   for(int32 i=0;i<8;i++)
   {
      for(int32 j=0;j<8;j++)
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

   for(int32 v=0;v<VIEWNUM;v++)
   {
      attitude[v]=45;
      newspaper_topicwork1[v]=0;
      newspaper_topicwork2[v]=0;
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
   law[LAW_GUNCONTROL]=-1;

#ifdef SHITLAWS
   for(int32 l=0;l<LAWNUM;l++)law[l]=-2;
#endif

   for(int32 s=0;s<100;s++)
   {
      if(s<20)senate[s]=-2;
      else if(s<45)senate[s]=-1;
      else if(s<70)senate[s]=0;
      else if(s<90)senate[s]=1;
      else senate[s]=2;
   }

   for(int32 h=0;h<435;h++)
   {
      if(h<30)house[h]=-2;
      else if(h<200)house[h]=-1;
      else if(h<300)house[h]=0;
      else if(h<400)house[h]=1;
      else house[h]=2;
   }

   for(int32 c=0;c<9;c++)
   {
      if(c<2)court[c]=-2;
      else if(c<5)court[c]=-1;
      else if(c<7)court[c]=0;
      else if(c<8)court[c]=1;
      else court[c]=2;
      name(courtname[c]);
   }

   for(int32 e=0;e<EXECNUM;e++)
   {
      exec[e]=-1;
      name(execname[e]);
   }

   attorneyseed=getSeed();
   cityname(lcityname);

   loaded=load();

   mode_title();

   //deinitialize curses
   end_game();

   return 0;
}



//picks a random number from 0 to max-1
int32 LCSrandom(uint32 max)
{
   r_num();

   double rand_y;
   double rand_i;

   rand_i = 2147483648UL;

   rand_y = max*((double)seed/rand_i);

   return((int32)rand_y);
}

//sets seed to a random number from 0 to 2 billion
uint32 r_num(void)
{
   seed=(seed*907725L+99979777UL)%2147483648UL;
   return seed;
}

int32 creaturest::attval(int16 a,char usejuice)
{
   int32 ret=att[a];

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

   int32 disfigs=0;
   if(special[SPECIALWOUND_TEETH]<TOOTHNUM)disfigs++;
   if(special[SPECIALWOUND_TEETH]<TOOTHNUM/2)disfigs++;
   if(special[SPECIALWOUND_TEETH]==0)disfigs++;
   if(special[SPECIALWOUND_RIGHTEYE]==0)disfigs+=2;
   if(special[SPECIALWOUND_LEFTEYE]==0)disfigs+=2;
   if(special[SPECIALWOUND_TONGUE]==0)disfigs+=3;
   if(special[SPECIALWOUND_NOSE]==0)disfigs+=3;

   int32 legok=2;
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

void vehiclest::init(int32 t)
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

void creaturest::creatureinit(void)
   {
      hireid=-1;
      worklocation=0;
      juice=0;
      flag=0;
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
      for(int32 i=0;i<LAWFLAGNUM;i++)
         lawflag[i]=0;
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
      for(int32 w=0;w<BODYPARTNUM;w++)wound[w]=0;
      weapon.type=WEAPON_NONE;
      weapon.ammo=0;
      armor.type=ARMOR_CLOTHES;
      armor.quality='1';
      armor.flag=0;
      for(int32 a=0;a<ATTNUM;a++)
      {
         att[a]=1;
      }
      int32 attnum=32;
      while(attnum>0)
      {
         int32 a=LCSrandom(ATTNUM);
         if(att[a]<10)
         {
            att[a]++;
            attnum--;
         }
      }
      for(int32 s=0;s<SKILLNUM;s++)
      {
         skill[s]=0;
         skill_ip[s]=0;
      }
      for(int32 c=0;c<CLIPNUM;c++)clip[c]=0;
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
      closed=0;
      highsecurity=0;
      mapseed=seed;r_num();
      compound_walls=0;
      compound_stores=0;
      front_business=-1;
   }

   void chaseseqst::clean(void)
   {
      for(int32 v=0;v<enemycar.size();v++)
      {
         delete enemycar[v];
      }
      enemycar.clear();

      friendcar.clear();
   }


   /* Free memory and exit the game */
   void end_game(int32 err)
   {
	   for(vector<locationst*>::iterator it=location.begin();it!=location.end();++it)
	   {
		   delete (*it);
	   }
	   int32 i=0;
	   for(i=0;i<squad.size();++i)
	   {
		   delete squad[i];
	   }
	   endwin();
	   exit(err);
   }

