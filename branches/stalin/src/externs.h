#ifndef EXTERNS_H_INCLUDED
#define EXTERNS_H_INCLUDED
#include "includes.h"
#include "sitemode/sitemap.h"

extern CursesMoviest movie;
extern unsigned char bigletters[27][5][7][4];
extern unsigned char newstops[6][80][5][4];
extern unsigned char newspic[20][78][18][4];

extern vector<configSiteMap *> sitemaps; // stores site map info read in from config file

extern int oldMapMode;
extern unsigned int seed;
extern short law[LAWNUM];

extern short house[435];
extern short senate[100];
extern short court[9];
extern char courtname[9][80];


extern signed char exec[EXECNUM];
extern short execterm;
extern char execname[EXECNUM][80];
extern short presparty;

extern int stat_recruits;
extern int stat_kidnappings;
extern int stat_dead;
extern int stat_kills;
extern int stat_funds;
extern int stat_spent;
extern int stat_buys;
extern int stat_burns;

extern int ustat_recruits;
extern int ustat_kidnappings;
extern int ustat_dead;
extern int ustat_kills;
extern int ustat_funds;
extern int ustat_spent;
extern int ustat_buys;
extern int ustat_burns;

extern int locx;
extern int locy;
extern int locz;

extern short sitetype;
extern short sitealienate;
extern short sitealarm;
extern short sitealarmtimer;
extern short postalarmtimer;
extern short siteonfire;
extern int sitecrime;
extern short cursite;

extern short interface_pgup;
extern short interface_pgdn;

extern int day;

extern short attitude[VIEWNUM];

extern short public_interest[VIEWNUM];
extern short background_liberal_influence[VIEWNUM];

extern char policestation_closed;
extern char amradio_closed;
extern char cablenews_closed;

extern char endgamestate;
extern char stalinendgamestate;
extern char ccs_kills;

extern int funds;

extern char loaded;

extern int mode;

extern short offended_cops;
extern short offended_corps;
extern short offended_cia;
extern short offended_amradio;
extern short offended_cablenews;
extern short offended_firemen;
extern short offended_stalin;
extern int police_heat;
extern int attorneyseed;
extern int selectedsiege;
extern char lcityname[80];
extern char newscherrybusted;

extern int month;
extern int year;
extern int amendnum;

extern bool termlimits;

extern long moneygained_donate;
extern long moneygained_brownies;
extern long moneygained_goods;
extern long moneygained_ccfraud;
extern long moneygained_hustling;
extern long moneygained_extortion;
extern long moneygained_thievery;
extern long moneygained_embezzlement;
extern long moneylost_goods;
extern long moneylost_trouble;
extern long moneylost_rent;
extern long moneylost_training;
extern long moneylost_manufacture;
extern long moneylost_legal;
extern long moneylost_food;
extern long moneylost_dating;
extern long moneylost_compound;
extern long moneylost_hostage;
extern long moneylost_confiscated;

extern short party_status;


extern vector<datest *> date;
extern vector<recruitst *> recruit;

extern vector<locationst *> location;

extern vector<newsstoryst *> newsstory;
extern newsstoryst *sitestory;

extern vector<itemst *> groundloot;
extern long curcreatureid;
extern chaseseqst chaseseq;

extern char slogan[80];

extern vector<Creature *> pool;

extern vector<squadst *> squad;
extern squadst *activesquad;
extern long cursquadid;

extern char disbanding;
extern int disbandtime;
extern char cantseereason;

extern vector<vehiclest *> vehicle;
extern long curcarid;
extern char showcarprefs;

extern siteblockst levelmap[MAPX][MAPY][MAPZ];
extern Creature encounter[ENCMAX];
extern highscorest score[SCORENUM];
extern int yourscore;

#endif // EXTERNS_H_INCLUDED
