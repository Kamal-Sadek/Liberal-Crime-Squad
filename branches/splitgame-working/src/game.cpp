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

//#define NOENEMYATTACK
//#define SHITLAWS
//#define GIVEBLOODYARMOR
//#define HIGHFUNDS
//#define AUTOENLIGHTEN
//#define SHOWWAIT

#include <includes.h>


CursesMoviest movie;
unsigned char bigletters[27][5][7][4];
unsigned char newstops[5][80][5][4];
unsigned char newspic[20][78][18][4];



unsigned long seed;

int LawflagHeat[LAWFLAGNUM] =
{
   128, // LAWFLAG_KIDNAPPING,
   64, // LAWFLAG_MURDER,
   4, // LAWFLAG_THEFT,
   1, // LAWFLAG_BREAKING,
   64, // LAWFLAG_TERRORISM,
   32, // LAWFLAG_JURY,
   128, // LAWFLAG_TREASON,
   64, // LAWFLAG_ESCAPED,
   32, // LAWFLAG_HELPESCAPE,
   16, // LAWFLAG_RESIST,
   1, // LAWFLAG_BURNFLAG,
   4, // LAWFLAG_SPEECH,
   1, // LAWFLAG_VANDALISM,
   16, // LAWFLAG_ASSAULT,
   8, // LAWFLAG_CARTHEFT,
   4, // LAWFLAG_INFORMATION,
   1, // LAWFLAG_COMMERCE,
   8, // LAWFLAG_CCFRAUD,
   4, // LAWFLAG_BROWNIES,
   4, // LAWFLAG_BURIAL,
   1, // LAWFLAG_PROSTITUTION,
   1, // LAWFLAG_DISTURBANCE,
   1, // LAWFLAG_HIREILLEGAL,
   8, // LAWFLAG_RACKETEERING,
   0, // LAWFLAG_LOITERING,
   16 // LAWFLAG_GUNCARRY
};




long curcreatureid=0;
vector<itemst *> groundloot;
vector<locationst *> location;


vector<vehiclest *> vehicle;
long curcarid=0;
char showcarprefs=1;


siteblockst map[MAPX][MAPY][MAPZ];


   



chaseseqst chaseseq;

char slogan[80];

vector<creaturest *> pool;

vector<squadst *> squad;
squadst *activesquad=NULL;
long cursquadid=0;

char disbanding=0;
int disbandtime=0;
char cantseereason;


creaturest encounter[ENCMAX];


char loaded=0;

int mode=GAMEMODE_TITLE;

short offended_cops=0;
short offended_corps=0;
short offended_cia=0;
short offended_amradio=0;
short offended_cablenews=0;
int police_heat=0;
unsigned long attorneyseed;
long selectedsiege=-1;
char lcityname[80];
char newscherrybusted=0;

int month=1;
int year=2005;
int amendnum=28;


short attitude[VIEWNUM];

short newspaper_topicwork[VIEWNUM];


short law[LAWNUM];

short house[435];
short senate[100];
short court[9];
char courtname[9][80];


short exec[EXECNUM];
short execterm=1;
char execname[EXECNUM][80];

unsigned long version=31100;
unsigned long lowestloadversion=31005;
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

vector<datest *> date;



vector<newsstoryst *> newsstory;
newsstoryst *sitestory=NULL;


vector<blogpostst *> blogpost;


#define SCORENUM 5
highscorest score[SCORENUM];
int yourscore=-1;








int main(int argc, char* argv[])
{
   //start curses
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
   law[LAW_GUNCONTROL]=-1;

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

   int guardianpower=0;
   if(nploc.size()>0&&!disbanding)
   {
      /*short power=nploc.size();
      
      //CRIMINALIZE THE PRINTERS
      long writingpower=0;
      for(int l=0;l<nploc.size();l++)
      {
         criminalizepool(LAWFLAG_SPEECH,-1,nploc[l]);
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
      }*/

      /*
      //DO EMPHASIS OF SITE EVENTS
      int vpower,val;
      for(int v=0;v<VIEWNUM;v++)
      {
         val=0; // *JDS* Newspaper's emphasis strength starts at 0, increases with writing power
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
      */

      //DO SPECIAL EDITIONS
      int loottype=choosespecialedition(clearformess);

      if(loottype!=-1)
      {
         guardianpower+=10*nploc.size();
         printnews(loottype,nploc.size());

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
               criminalizepool(LAWFLAG_TREASON,-1,nploc[l]);
            }
         }
      }
   }

   int libpower[VIEWNUM]={0};
   int computernum=0;
   /*

   for(int dude=0;dude<pool.size();dude++)
   {
      if(pool[dude]->alive&&pool[dude]->align==1&&
         pool[dude]->clinic==0&&pool[dude]->dating==0&&
         pool[dude]->hiding==0&&pool[dude]->skill[SKILL_COMPUTERS])
      {
         if(pool[dude]->location!=-1)
         {
            if(location[pool[dude]->location]->type!=SITE_GOVERNMENT_PRISON&&
               location[pool[dude]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
               location[pool[dude]->location]->type!=SITE_GOVERNMENT_POLICESTATION)
            {
               computernum++;
            }
         }
      }
   }*/

   if(blogpost.size()&&!disbanding)
   {
      blogpostst* bp=blogpost.back();
      while(blogpost.size())
      {
         if(bp->type==REPORT_NEWS)
         {
            guardianpower+=bp->power;
            libpower[bp->issue]+=bp->power;
         }
         else
         {
            // Persuasive arguments tend to popularize the LCS
            if(bp->power>=3)
            {
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,2,0);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,1,0);
            }
            else if(bp->power>0&&(attitude[VIEW_LIBERALCRIMESQUAD]<20||!LCSrandom(20)))
            {
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,1,0);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,LCSrandom(2),0);
            }

            if(bp->type==REPORT_ATTACK)
            {
               // Offensive or libellous attack articles by shoddy writers
               // on topics that the public largely disagrees with the LCS
               // on tend to scandalize the public and hurt the cause for
               // free speech (not to mention offend their view of the LCS)
               if(bp->power<0&&attitude[bp->issue]<50)
               {
                  change_public_opinion(VIEW_FREESPEECH,(attitude[bp->issue]-50)/25,0);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,(attitude[bp->issue]-50)/10,0);
               }
               // But they are more effective
               bp->power+=1;
            }

            // Modify power by lcs popularity
            bp->power+=(attitude[VIEW_LIBERALCRIMESQUADPOS]-50)/26;
            bp->power*=static_cast<long>((attitude[VIEW_LIBERALCRIMESQUAD]*(nploc.size()+1))/100.0f);
            
            if(bp->issue<VIEWNUM-2)
            {
               if(bp->power>0)
               {
                  if(attitude[bp->issue]<30+newspaper_topicwork[bp->issue]+LCSrandom(20))
                  {
                     guardianpower+=bp->power;
                     libpower[bp->issue]+=3*bp->power;
                  }
               }
               else
               {
                  if(attitude[bp->issue]>70-LCSrandom(20))
                  {
                     guardianpower+=bp->power;
                     libpower[bp->issue]+=3*bp->power;
                  }
               }
            }
            else 
            {
               guardianpower+=bp->power;
               libpower[bp->issue]+=3*bp->power;
            }
         }
         delete bp;
         blogpost.pop_back();
         bp=blogpost.back();
      }
      /*for(int i=0;i<VIEWNUM;i++)
      {
         libpower[i]+=guardianpower;
      }*/
      clearformess=1;

      guardianupdate(nploc.size(),guardianpower);
   }

   //STORIES STALE EVEN IF NOT PRINTED
   for(v=0;v<VIEWNUM;v++)newspaper_topicwork[v]=0;

   int conspower=200-attitude[VIEW_AMRADIO]-attitude[VIEW_CABLENEWS];
   
   //HAVING SLEEPERS
   for(int pl=0;pl<pool.size();pl++)
   {
      if(pool[pl]->alive&&(pool[pl]->flag & CREATUREFLAG_SLEEPER))
      {
         sleepereffect(*pool[pl],clearformess,canseethings,libpower);
      }
   }
   
   //PUBLIC OPINION NATURAL MOVES
   for(v=0;v<VIEWNUM;v++)
   {
      
      if(v==VIEW_LIBERALCRIMESQUADPOS)continue;
      if(v==VIEW_LIBERALCRIMESQUAD)continue;
      if(v!=VIEW_AMRADIO&&v!=VIEW_CABLENEWS)
      {
         //DRIFTS DOWN DEPENDING ON AM RADIO AND CABLE NEWS
         
         
         if(LCSrandom(200)>conspower)
         {
            change_public_opinion(v,LCSrandom(2)*2-1,0);
         }
         else
         {
            change_public_opinion(v,-1,0);

            //Crushing conservative media silences liberal influence
            if(LCSrandom(200)<conspower/2)
            {
               libpower[v]-=75;
            }
         }
      }
      if(LCSrandom(200)<libpower[v])
      {
         change_public_opinion(v,1,0);
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
                           pool[p2]->lawflag[LAWFLAG_RACKETEERING]++;
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
               addstr("Conservative         ");
            }
            else if(senate[s]==0)
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               addstr("moderate               ");
            }
            else if(senate[s]==1)
            {
               set_color(COLOR_BLUE,COLOR_BLACK,1);
               addstr("Liberal                ");
            }
            else
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               addstr("Elite Liberal       ");
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
         addstr("Press any key to continue the elections.   ");

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
            case LAW_GUNCONTROL:
               if(propdir[p]==1)addstr("Prevent Gun Violence");
               else addstr("Assert our Second Amendment Rights");
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
            case LAW_GUNCONTROL:
               if(scasedir[c]==1)addstr("Prevent Gun Violence");
               else addstr("Protect our Second Amendment Rights");
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
            case LAW_GUNCONTROL:
               if(billdir[c]==1)addstr("Prevent Gun Violence");
               else addstr("Protect our Second Amendment Rights");
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
         addstr("Press any key to watch the President.                   ");

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
               addstr("They'll round up the last of you eventually.  All is lost.");
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
         addstr("Press any key to breathe a sigh of relief.                   ");
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

      if(canseethings)
         nodelay(stdscr,FALSE);

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
         addstr("Press any key to watch the State votes unfold.              ");
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
            if(canseethings)
               addstr("Yea");
         }
         else
            if(canseethings)
               addstr("Nay");

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

      if(canseethings)
         set_color(COLOR_WHITE,COLOR_BLACK,1);

      if(yesstate>=37)
      {
         ratified=1;
      }

      if(canseethings)
         nodelay(stdscr,FALSE);

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

      sum/=(VIEWNUM-2);

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
         case LAW_GUNCONTROL:
               if(!disbanding)
                  return (attitude[VIEW_POLICEBEHAVIOR]+
                          attitude[VIEW_PRISONS]+
                          attitude[VIEW_DEATHPENALTY]+
                          ((attitude[VIEW_LIBERALCRIMESQUADPOS]-50)*
                            attitude[VIEW_LIBERALCRIMESQUAD]         )/100)/3;
               else
                  return (attitude[VIEW_POLICEBEHAVIOR]+
                          attitude[VIEW_PRISONS]+
                          attitude[VIEW_DEATHPENALTY])/3;
      }
   }

   return 50;
}



void guardianupdate(char size, char power)
{
   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(5,2);
   if(size)
   {
      addstr("The monthly Liberal Guardian newspaper");
   }
   else
   {
      addstr("The monthly Liberal Guardian online newsletter");
   }

   addstr(" is published.");

   move(7,2);
   if(power<0)
   {
      addstr("The only readers are Conservatives, who seem to think it's funny.");
   }
   else if(power==0)
   {
      addstr("Unfortunately, nobody seems interested.");
   }
   else if(power<5)
   {
      addstr("Very few people seem to be interested.");
   }
   else if(power<50)
   {
      addstr("A fair number of people are reading it.");
   }
   else if(power<100)
   {
      addstr("Many people are reading it.");
   }
   else if(power<250)
   {
      addstr("The response is very strong. People are changing their minds.");
   }
   else
   {
      addstr("The response is electric. Everyone is talking about this month's");
      move(8,2);
      addstr("Liberal Guardian.");
   }

   getch();
   return;
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
         else if(interface_pgup=='.')
         {
            addstr("; - Previous");
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
         else if(interface_pgup=='.')
         {
            addstr(": - Next");
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

   if(!iscriminal(g))g.lawflag[LAWFLAG_LOITERING]++;

   int typenum=0;
   bool breaker[LAWFLAGNUM] = {0};
   int scarefactor=0;
      // *JDS* Scarefactor is the severity of the case against you; if you're a really
      // nasty person with a wide variety of major charges against you, then scarefactor
      // can get up there

   for(int i=0;i<LAWFLAGNUM;i++)
   {
      if(g.lawflag[i])
      {
         typenum++;
         breaker[i]=1;
         scarefactor+=LawflagHeat[i]>>4;
      }
   }

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

      if(breaker[LAWFLAG_TREASON])
      {
         addstr("treason");
         breaker[LAWFLAG_TREASON]=0;
      }
      else if(breaker[LAWFLAG_MURDER])
      {
         addstr("murder");
         breaker[LAWFLAG_MURDER]=0;
      }
      else if(breaker[LAWFLAG_TERRORISM])
      {
         addstr("terrorism");
         breaker[LAWFLAG_TERRORISM]=0;
      }
      else if(breaker[LAWFLAG_KIDNAPPING])
      {
         addstr("kidnapping");
         breaker[LAWFLAG_KIDNAPPING]=0;
      }
      else if(breaker[LAWFLAG_BURNFLAG])
      {
         if(law[LAW_FLAGBURNING]==-2)
            addstr("Flag Murder");
         else if(law[LAW_FLAGBURNING]==-1)
            addstr("felony flag burning");
         else if(law[LAW_FLAGBURNING]==0)
            addstr("flag burning");
         breaker[LAWFLAG_BURNFLAG]=0;
      }
      else if(breaker[LAWFLAG_SPEECH])
      {
         addstr("harmful speech");
         breaker[LAWFLAG_SPEECH]=0;
      }
      else if(breaker[LAWFLAG_BROWNIES])
      {
         addstr("selling brownies");
         breaker[LAWFLAG_BROWNIES]=0;
      }
      else if(breaker[LAWFLAG_ESCAPED])
      {
         addstr("escaping prison");
         breaker[LAWFLAG_ESCAPED]=0;
      }
      else if(breaker[LAWFLAG_HELPESCAPE])
      {
         addstr("aiding a prison escape");
         breaker[LAWFLAG_HELPESCAPE]=0;
      }
      else if(breaker[LAWFLAG_JURY])
      {
         addstr("jury tampering");
         breaker[LAWFLAG_JURY]=0;
      }
      else if(breaker[LAWFLAG_RACKETEERING])
      {
         addstr("racketeering");
         breaker[LAWFLAG_RACKETEERING]=0;
      }
      else if(breaker[LAWFLAG_ASSAULT])
      {
         addstr("assault");
         breaker[LAWFLAG_ASSAULT]=0;
      }
      else if(breaker[LAWFLAG_GUNCARRY])
      {
         if(x>=1){x=1;y++;move(y,1);}
         addstr("carrying an illegal weapon");
         breaker[LAWFLAG_GUNCARRY]=0;
      }
      else if(breaker[LAWFLAG_CARTHEFT])
      {
         addstr("car theft");
         breaker[LAWFLAG_CARTHEFT]=0;
      }
      else if(breaker[LAWFLAG_CCFRAUD])
      {
         addstr("credit card fraud");
         breaker[LAWFLAG_CCFRAUD]=0;
      }
      else if(breaker[LAWFLAG_THEFT])
      {
         addstr("theft");
         breaker[LAWFLAG_THEFT]=0;
      }
      else if(breaker[LAWFLAG_PROSTITUTION])
      {
         addstr("prostitution");
         breaker[LAWFLAG_PROSTITUTION]=0;
      }
      else if(breaker[LAWFLAG_HIREILLEGAL])
      {
         addstr("hiring an illegal alien");
         breaker[LAWFLAG_HIREILLEGAL]=0;
      }
      else if(breaker[LAWFLAG_COMMERCE])
      {
         if(x>=1){x=1;y++;move(y,1);}
         addstr("interference with interstate commerce");
         breaker[LAWFLAG_COMMERCE]=0;
      }
      else if(breaker[LAWFLAG_INFORMATION])
      {
         if(x>=1){x=1;y++;move(y,1);}
         addstr("unlawful access of an information system");
         breaker[LAWFLAG_INFORMATION]=0;
      }
      else if(breaker[LAWFLAG_BURIAL])
      {
         addstr("unlawful burial");
         breaker[LAWFLAG_BURIAL]=0;
      }
      else if(breaker[LAWFLAG_BREAKING])
      {
         addstr("breaking and entering");
         breaker[LAWFLAG_BREAKING]=0;
      }
      else if(breaker[LAWFLAG_VANDALISM])
      {
         addstr("vandalism");
         breaker[LAWFLAG_VANDALISM]=0;
      }
      else if(breaker[LAWFLAG_RESIST])
      {
         addstr("resisting arrest");
         breaker[LAWFLAG_RESIST]=0;
      }
      else if(breaker[LAWFLAG_DISTURBANCE])
      {
         addstr("disturbing the peace");
         breaker[LAWFLAG_DISTURBANCE]=0;
      }
      else if(breaker[LAWFLAG_LOITERING])
      {
         addstr("loitering");
         breaker[LAWFLAG_LOITERING]=0;
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
   else if(funds>=5000) // *JDS* hiring a good attorney is now more expensive
   {
      move(y,1);y++;
      addstr("D - Use $5000 to hire ace Liberal attorney ");
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
      else if(c=='d'&&funds>=5000)
      {
         funds-=5000;
         stat_spent+=5000;
         defense=3;
         moneylost_legal+=5000;
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
      int jury=LCSrandom(61)-30; // *JDS* reduced the effect of the jury, and the effect sleeper judges have on the jury
      if(sleeperjudge)jury-=20;
      if(jury<=-29)
      {
         switch(LCSrandom(4))
         {
            case 0:addstr(g.name);addstr("'s best friend from childhood is a juror.");break;
            case 1:addstr("The jury is Flaming Liberal.");break;
            case 2:addstr("Four of the jurors are closet Socialists.");break;
            case 3:addstr("One of the jurors flashes a SECRET LIBERAL HAND SIGNAL when no one is looking.");break;
         }
      }
      else if(jury<=-15)addstr("The jury is fairly Liberal.");
      else if(jury<15)addstr("The jury is quite moderate.");
      else if(jury<29)addstr("The jury is a bit Conservative.");
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
      int prosecution;
      if(scarefactor>6)prosecution=LCSrandom(101)+50; // *JDS* If you have a long list of crimes, the prosecution won't suck
      else prosecution=LCSrandom(101);
      if(sleeperjudge)prosecution>>=1;

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(7,1);

      if(prosecution<=25)addstr("The prosecution's presentation is terrible.");
      else if(prosecution<=50)addstr("The prosecution's arguments are shaky.");
      else if(prosecution<=75)addstr("The prosecution gives a standard presentation.");
      else if(prosecution<=100)addstr("The prosecution's case is solid.");
      else if(prosecution<=150)addstr("The prosecution makes an airtight case.");
      else addstr("The prosecution's case seems insurmountable.");

      refresh();
      getch();

      jury+=LCSrandom(prosecution/2+1)+prosecution/2;

      //DEFENSE MESSAGE
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(9,1);

      int defensepower=0;
      if(defense==0||defense==3)
      {
         if(defense==0)defensepower=LCSrandom(71); // *JDS* gave you a better shake with the court-appointed attorney
         else
         {
            defensepower=LCSrandom(101)+50;
         }

         if(defensepower<=15)addstr("The defense attorney accidentally said \"My client is GUILTY!\" during closing.");
         else if(defensepower<=25)addstr("The defense is totally lame.");
         else if(defensepower<=50)addstr("The defense was lackluster.");
         else if(defensepower<=75)addstr("Defense arguments were pretty good.");
         else if(defensepower<=100)addstr("The defense was really slick.");
         else if(defensepower<=145)
         {
            if(prosecution<100)addstr("The defense makes the prosecution look like amateurs.");
            else
            {
               addstr("But the defense is just as impressive.");
            }
         }
         else
         {
            if(prosecution<100)
            {
               addstr(attorneyname);
               addstr("'s arguments made several of the jurors stand up");
               move(10,1);
               addstr("and shout \"NOT GUILTY!\" before deliberations even began.");
            }
            else
            {
               addstr("But ");
               addstr(attorneyname);
               addstr(" conducts an incredible defense!");
            }
         }
      }
      if(defense==1)
      {
         // *JDS* LEGAL SELF-REPRESENTATION: Your intelligence, heart, and twice your charisma
         // form the baseline the jury will evaluate your arguments from, and after that it's
         // up to your roll. To succeed here, you really need to have two skills be
         // high: persuasion and law, with law being 1.5 times as influential. You can't have
         // just one or just the other, and that's a change I've made which will make this
         // much harder. Even if you're a very persuasive person, the court will eat you alive
         // if you can't sound intelligent when talking about the relevant charges, and you
         // won't be able to fool the jury into letting you go if you aren't persuasive, as no
         // matter how encyclopedic your legal knowledge is, it's all in the pitch. After these
         // skills are multiplied together, you make a roll on this "defense skill" value; this curves your
         // results toward a central value, and away from extreme good and bad performances.
         // All in all, it is significantly *harder* to defend yourself after these changes,
         // and a very powerful case will require someone have lawyerly skills to begin with.
         // Major note: Defense skill is capped at 100. Once you're rolling d200, adding more
         // would be really unrealistic. You can only be so persuasive.
         //
         // EXAMPLE: Suppose your Liberal has 4 persuasion and 6 law -- the average starting
         // skills for a seduced lawyer. This results in a defenseskill of 51. If their intelligence,
         // heart, and charisma are each four, that makes their roll 16+LCSrandom(102).
         // This is significantly weaker than a paid or Sleeper attorney, but much stronger
         // than the public defender, and about on par with the prosecutor so long as it isn't
         // a high-profile case.

         // A character build spefically to be strong in this area *will* still start out
         // slightly stronger than the public defender (and will be notably better once they
         // hit activist level).
         int defenseskill=(g.skill[SKILL_PERSUASION]+1)*(((g.skill[SKILL_LAW]*3)/2)+1)+1;
         defensepower+=g.attval(ATTRIBUTE_INTELLIGENCE);
         defensepower+=g.attval(ATTRIBUTE_HEART);
         defensepower+=g.attval(ATTRIBUTE_CHARISMA)*2;
         defensepower+=LCSrandom(min(defenseskill*2,200));
         g.skill_ip[SKILL_PERSUASION]+=50;
         g.skill_ip[SKILL_LAW]+=50;

         addstr(g.name);
         if(defensepower<=15)addstr("'s defense looks like Colin Ferguson's.");
         else if(defensepower<=25)addstr("'s case really sucked.");
         else if(defensepower<=50)addstr(" did all right, but made some mistakes.");
         else if(defensepower<=75)addstr("'s arguments were pretty good.");
         else if(defensepower<=100)addstr(" worked the jury very well.");
         else if(defensepower<=150)addstr(" made a very powerful case.");
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
         if(LCSrandom(2)||scarefactor>10)
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
      else if(defensepower>jury)
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
      else if(defensepower/3>=jury/4 || sleeperjudge) // *JDS* sleeper judge will always reduce the sentence
      {
         penalize(g,1);
      }
      else
      {
         penalize(g,0);
      }
      //CLEAN UP LAW FLAGS
      for(int i=0;i<LAWFLAGNUM;i++)
      {
         g.lawflag[i]=0;
      }
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
      for(int i=0;i<LAWFLAGNUM;i++)
      {
         g.lawflag[i]=0;
      }
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



void penalize(creaturest &g,char lenient)
{
   set_color(COLOR_RED,COLOR_BLACK,1);
   move(3,1);
   addstr("GUILTY!");
   refresh();
   getch();

   g.deathpenalty=0;

   if((g.lawflag[LAWFLAG_MURDER])||(g.lawflag[LAWFLAG_TREASON])||
      ((g.lawflag[LAWFLAG_BURNFLAG])&&law[LAW_FLAGBURNING]==-2)||
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
      if((g.lawflag[LAWFLAG_KIDNAPPING])&&g.sentence!=-1)g.sentence+=36+LCSrandom(18);
      if((g.lawflag[LAWFLAG_THEFT])&&g.sentence!=-1)g.sentence+=3+LCSrandom(8);
      if((g.lawflag[LAWFLAG_GUNCARRY])&&g.sentence!=-1)g.sentence+=12+LCSrandom(48);
      if((g.lawflag[LAWFLAG_CARTHEFT])&&g.sentence!=-1)g.sentence+=6+LCSrandom(7);
      if((g.lawflag[LAWFLAG_INFORMATION])&&g.sentence!=-1)g.sentence+=1+LCSrandom(13);
      if((g.lawflag[LAWFLAG_COMMERCE])&&g.sentence!=-1)g.sentence+=1+LCSrandom(13);
      if((g.lawflag[LAWFLAG_CCFRAUD])&&g.sentence!=-1)g.sentence+=6+LCSrandom(25);
      if((g.lawflag[LAWFLAG_BURIAL])&&g.sentence!=-1)g.sentence+=3+LCSrandom(12);
      if((g.lawflag[LAWFLAG_PROSTITUTION])&&g.sentence!=-1)g.sentence+=1+LCSrandom(6);
      if((g.lawflag[LAWFLAG_DISTURBANCE])&&g.sentence!=-1)g.sentence+=1;
      if((g.lawflag[LAWFLAG_LOITERING])&&g.sentence!=-1)g.sentence+=1;
      if((g.lawflag[LAWFLAG_HIREILLEGAL])&&g.sentence!=-1)g.sentence+=1;
      if((g.lawflag[LAWFLAG_RACKETEERING])&&g.sentence!=-1)g.sentence+=6+LCSrandom(100);
      if((g.lawflag[LAWFLAG_BROWNIES])&&g.sentence!=-1)
      {
         if(LCSrandom(3))g.sentence+=3+LCSrandom(12);
         else
         {
            if(LCSrandom(3))g.sentence+=3+LCSrandom(120);
            else g.sentence+=3+LCSrandom(360);
         }
      }
      if((g.lawflag[LAWFLAG_BREAKING])&&g.sentence!=-1)g.sentence+=1;
      if((g.lawflag[LAWFLAG_TERRORISM])&&g.sentence!=-1)g.sentence+=60+LCSrandom(181);
      if((g.lawflag[LAWFLAG_JURY])&&g.sentence!=-1)g.sentence+=30+LCSrandom(61);
      if((g.lawflag[LAWFLAG_ESCAPED])&&g.sentence!=-1)g.sentence=-1;
      if((g.lawflag[LAWFLAG_HELPESCAPE])&&g.sentence!=-1)g.sentence+=30+LCSrandom(61);
      if((g.lawflag[LAWFLAG_RESIST])&&g.sentence!=-1)g.sentence+=1+LCSrandom(1);
      if((g.lawflag[LAWFLAG_BURNFLAG])&&g.sentence!=-1)
      {
         if(law[LAW_FLAGBURNING]==-2)
         {
            if(!LCSrandom(2))g.sentence+=120+LCSrandom(241);
            else g.sentence=-1;
         }
         else if(law[LAW_FLAGBURNING]==-1)g.sentence+=36;
         else if(law[LAW_FLAGBURNING]==0)g.sentence+=1;
      }
      if((g.lawflag[LAWFLAG_SPEECH])&&g.sentence!=-1)g.sentence+=4+LCSrandom(3);
      if((g.lawflag[LAWFLAG_VANDALISM])&&g.sentence!=-1)g.sentence+=1;
      if((g.lawflag[LAWFLAG_ASSAULT])&&g.sentence!=-1)g.sentence+=3+LCSrandom(1);
      if((g.lawflag[LAWFLAG_MURDER])&&g.sentence!=-1)
      {
         if(!LCSrandom(2))g.sentence+=120+LCSrandom(241);
         else g.sentence=-1;
      }
      if((g.lawflag[LAWFLAG_TREASON])&&g.sentence!=-1)g.sentence=-1;
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
      addstr("Liberal Crime Squad:   Monthly Action Report");

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
         addstr("Brownies   . . . . . . . . . . . . . . . . . . . . . . . .");

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
         addstr("Hustling   . . . . . . . . . . . . . . . . . . . . . . . .");

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
         addstr("Thievery   . . . . . . . . . . . . . . . . . . . . . . . .");

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
         addstr("Liberal Disobedience   . . . . . . . . . . . . . . . . . .");

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
         addstr("Rent   . . . . . . . . . . . . . . . . . . . . . . . . . .");

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
         addstr("Legal Fees   . . . . . . . . . . . . . . . . . . . . . . .");

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
         addstr("Infrastructure   . . . . . . . . . . . . . . . . . . . . .");

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



void getmaskdesc(char *str,short mask)
{
   switch(mask)
   {
      //LIMIT-------------------------------------:--------------------------------------:
      case MASK_ASHCROFT:            strcpy(str,"U.S. Senator, Attorney General");break;
      case MASK_BARR:               strcpy(str,"U.S. Representative");break;
      case MASK_BLAIR:            strcpy(str,"British Prime Minister");break;
      case MASK_BUSH_BARBARA:         strcpy(str,"U.S. First Lady");break;
      case MASK_BUSH_GEORGE:         strcpy(str,"U.S. President");break;
      case MASK_BUSH_GEORGE2:         strcpy(str,"U.S. President");break;
      case MASK_CARTER:            strcpy(str,"U.S. President");break;
      case MASK_CHENEY:            strcpy(str,"U.S. Vice President, CEO");break;
      case MASK_CLINTON_BILL:         strcpy(str,"U.S. President");break;
      case MASK_CLINTON_HILLARY:      strcpy(str,"U.S. Senator, First Lady");break;
      case MASK_FALWELL:            strcpy(str,"Reverend");break;
      case MASK_FORD:               strcpy(str,"U.S. President");break;
      case MASK_GORE_AL:            strcpy(str,"U.S. Vice President");break;
      //LIMIT-------------------------------------:--------------------------------------:
      case MASK_GORE_TIPPER:         strcpy(str,"U.S. Second Lady");break;
      case MASK_HELMS:            strcpy(str,"U.S. Senator");break;
      case MASK_JASON:            strcpy(str,"A Hockey Mask a la Friday the 13th");break;
      case MASK_LINCOLN:            strcpy(str,"U.S. President, Assassinated");break;
      case MASK_KENNEDY_JOHN:         strcpy(str,"U.S. President, Assassinated");break;
      case MASK_KENNEDY_ROBERT:      strcpy(str,"U.S. Senator, Assassinated");break;
      case MASK_KISSINGER:         strcpy(str,"U.S. Secretary of State");break;
      case MASK_NIXON:            strcpy(str,"U.S. President, Resigned");break;
      case MASK_POWELL:            strcpy(str,"U.S. Secretary of State, General");break;
      case MASK_REAGAN_NANCY:         strcpy(str,"U.S. First Lady");break;
      case MASK_REAGAN_RONALD:      strcpy(str,"U.S. President");break;
      case MASK_RICE:               strcpy(str,"U.S. National Security Advisor");break;
      case MASK_ROBERTSON:         strcpy(str,"Televangelist");break;
      case MASK_RUMSFELD:            strcpy(str,"U.S. Secretary of Defense");break;
      case MASK_SATAN:            strcpy(str,"Prince of Darkness, Red with Horns");break;
      case MASK_STARR:            strcpy(str,"Independent Counsel");break;
      case MASK_THURMOND:            strcpy(str,"U.S. Senator");break;
      case MASK_WASHINGTON:         strcpy(str,"U.S. President");break;
      case MASK_WAXMAN:            strcpy(str,"U.S. Representative");break;
      case MASK_JESUS:            strcpy(str,"The Son of God, The Lamb, He died 4 U");break;
      case MASK_COLEMAN_GARY:         strcpy(str,"Actor, Candidate for Governor");break;
      case MASK_MADONNA:            strcpy(str,"Singer, Actor");break;
      case MASK_SPEARS:            strcpy(str,"Singer, Actor");break;
      case MASK_EMINEM:            strcpy(str,"Rapper, Actor");break;
      case MASK_AGUILERA:            strcpy(str,"Singer");break;
      case MASK_WAHLBERG:            strcpy(str,"Actor, Rapper");break;
      //LIMIT-------------------------------------:--------------------------------------:
      case MASK_IGGYPOP:            strcpy(str,"Singer, Actor");break;
      case MASK_CASH:               strcpy(str,"Singer");break;
      case MASK_BINLADEN:            strcpy(str,"Terrorist");break;
      case MASK_LORDS:            strcpy(str,"Actor");break;
      case MASK_SHIELDS:            strcpy(str,"Actor");break;
      case MASK_JACKSON_MICHAEL:      strcpy(str,"Singer");break;
      case MASK_CRUTHERS:            strcpy(str,"Singer, Actor");break;
      case MASK_KING_DON:            strcpy(str,"Boxing Promoter");break;
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






















/**********************************************************************
** *JDS*
** ----- The sleeper system has been completely reworked.
** - Sleepers no longer directly inflence the issues. They now affect
** the broad "liberal power" stats across many issues, which are used
** as a kind of monthly liberal roll akin to AM Radio and Cable News.
** - Each sleeper can affect one or more issue, throwing their power
** into the "abstracted debate" on that issue.
** - After all of the sleepers have contributed to the liberal power
** stats, a roll is made on each issue to see whether the liberals
** make background progress on those issues. 
** - Several sleepers have special abilities. Lawyers and Judges, as
** always, can aid your people in the legal system. Police officers,
** corporate managers, CEOs, and agents can all now leak secret
** documents of the appropriate types, and they will make a check
** each month. This will only happen if the homeless shelter is not
** under siege, and "canseethings" is enabled (eg, you're not in prison
** or disbanded or some other situation where your sleeper can't get
** in touch with anyone in your squad).
** - News Anchors and Radio Personalities remain the two most powerful
** sleepers.
**********************************************************************/
void sleepereffect(creaturest &cr,char &clearformess,char canseethings,int *libpower)
{
   int power=(cr.attval(ATTRIBUTE_CHARISMA)+
               cr.attval(ATTRIBUTE_HEART)+cr.attval(ATTRIBUTE_INTELLIGENCE)+
               cr.skill[SKILL_PERSUASION]);

   // Adjust power for sleepers
   switch(cr.type)
   {
      case CREATURE_CORPORATE_CEO:
      case CREATURE_DEATHSQUAD:
      case CREATURE_EDUCATOR:
         power*=4;
         break;
      case CREATURE_SCIENTIST_EMINENT:
      case CREATURE_ACTOR:
      case CREATURE_GANGUNIT:
         power*=3;
         break;
      default:
         power*=2;
         break;
   }
   int homes=-1; // find homeless shelter
   for(int l=0;l<location.size();l++)
   {
      if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
      {
         homes=l;
      }
   }
   switch(cr.type)
   {
      /* Cultural leaders block - small influence on everything */
      case CREATURE_RADIOPERSONALITY:
         libpower[VIEW_AMRADIO]+=power;
         break;
      case CREATURE_NEWSANCHOR:
         libpower[VIEW_CABLENEWS]+=power;
         break;
      case CREATURE_PAINTER:
      case CREATURE_SCULPTOR:
      case CREATURE_AUTHOR:
      case CREATURE_JOURNALIST:
      case CREATURE_MUSICIAN:
      case CREATURE_CRITIC_ART:
      case CREATURE_CRITIC_MUSIC:
      case CREATURE_ACTOR:
         for(int i=0;i<VIEWNUM;i++)
         {
            libpower[i]+=power/4;
         }
         break;
      /* Legal block - influences an array of social issues */
      case CREATURE_LAWYER:
      case CREATURE_JUDGE_CONSERVATIVE:
         libpower[VIEW_ABORTION]+=power;
         libpower[VIEW_GAY]+=power;
         libpower[VIEW_DEATHPENALTY]+=power;
         libpower[VIEW_FREESPEECH]+=power;
         libpower[VIEW_JUSTICES]+=power;
         libpower[VIEW_INTELLIGENCE]+=power;
         libpower[VIEW_ANIMALRESEARCH]+=power;
         break;
      /* Scientists block */
      case CREATURE_SCIENTIST_LABTECH:
      case CREATURE_SCIENTIST_EMINENT:
         libpower[VIEW_NUCLEARPOWER]+=power;
         libpower[VIEW_ANIMALRESEARCH]+=power;
         libpower[VIEW_GENETICS]+=power;
         break;
      /* Corporate block */
      case CREATURE_CORPORATE_CEO:
         // CEO can leak corporate files to you
         if(!LCSrandom(10)&&!location[homes]->siege.siege&&canseethings) 
         {
            itemst *it=new itemst;
            it->type=ITEM_LOOT;
            it->loottype=LOOT_CORPFILES;
            location[homes]->loot.push_back(it);

            erase();
            move(6,1);
            addstr("Sleeper ");
            addstr(cr.name);
            addstr(" has leaked secret corporate documents.");
            move(7,1);
            addstr("They are stashed at the homeless shelter.");
         }
         
         libpower[VIEW_CEOSALARY]+=power;
         libpower[VIEW_TAXES]+=power;
         libpower[VIEW_CORPORATECULTURE]+=power;
         break;
      case CREATURE_CORPORATE_MANAGER:
         // Corporate manager can leak corporate files to you
         if(!LCSrandom(70)&&!location[homes]->siege.siege&&canseethings) 
         {
            itemst *it=new itemst;
            it->type=ITEM_LOOT;
            it->loottype=LOOT_CORPFILES;
            location[homes]->loot.push_back(it);

            erase();
            move(6,1);
            addstr("Sleeper ");
            addstr(cr.name);
            addstr(" has leaked secret corporate documents.");
            move(7,1);
            addstr("They are stashed at the homeless shelter.");
         }
         libpower[VIEW_CEOSALARY]+=power;
         libpower[VIEW_TAXES]+=power;
         libpower[VIEW_CORPORATECULTURE]+=power;
         break;
      /* Law enforcement block */
      case CREATURE_DEATHSQUAD:
         libpower[VIEW_DEATHPENALTY]+=power; // No break
      case CREATURE_SWAT:
      case CREATURE_COP:
      case CREATURE_GANGUNIT:
         // Cops can leak police files to you
         if(!LCSrandom(70)&&!location[homes]->siege.siege&&canseethings)
         {
            itemst *it=new itemst;
            it->type=ITEM_LOOT;
            it->loottype=LOOT_POLICERECORDS;
            location[homes]->loot.push_back(it);

            erase();
            move(6,1);
            addstr("Sleeper ");
            addstr(cr.name);
            addstr(" has leaked secret police records.");
            move(7,1);
            addstr("They are stashed at the homeless shelter.");
         }
         libpower[VIEW_POLICEBEHAVIOR]+=power;
         break;
      /* Prison block */
      case CREATURE_EDUCATOR:
         libpower[VIEW_DEATHPENALTY]+=power; // no break intended
      case CREATURE_PRISONGUARD:
      case CREATURE_PRISONER:
         libpower[VIEW_PRISONS]+=power;
         break;
      /* Intelligence block */
      case CREATURE_AGENT:
         // Agents can leak intelligence files to you
         if(!LCSrandom(20)&&!location[homes]->siege.siege&&canseethings) 
         {
            itemst *it=new itemst;
            it->type=ITEM_LOOT;
            it->loottype=LOOT_SECRETDOCUMENTS;
            location[homes]->loot.push_back(it);

            erase();
            move(6,1);
            addstr("Sleeper ");
            addstr(cr.name);
            addstr(" has leaked secret intelligence files.");
            move(7,1);
            addstr("They are stashed at the homeless shelter.");
         }
         libpower[VIEW_INTELLIGENCE]+=power;
         break;
      /* No influence at all block - for people were liberal anyway, or have no way of doing any good */
      case CREATURE_WORKER_FACTORY_CHILD:
      case CREATURE_GENETIC:
      case CREATURE_GUARDDOG:
      case CREATURE_JUROR:
      case CREATURE_BUM:
      case CREATURE_CRACKHEAD:
      case CREATURE_TANK:
      case CREATURE_HIPPIE: // too liberal to be a proper sleeper
      case CREATURE_WORKER_FACTORY_UNION: // same
      case CREATURE_JUDGE_LIBERAL: // more again
      case CREATURE_MUTANT:
         return;
      /* Talk up LCS block -- includes everyone else */
      default:
         libpower[VIEW_LIBERALCRIMESQUAD]+=power;
         libpower[VIEW_LIBERALCRIMESQUADPOS]+=power;
   }
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
      for(int i=0;i<LAWFLAGNUM;i++)
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
      closed=0;
      highsecurity=0;
      mapseed=seed;r_num();
      compound_walls=0;
      compound_stores=0;
      front_business=-1;
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
   
