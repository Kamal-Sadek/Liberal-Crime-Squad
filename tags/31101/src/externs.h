#ifndef EXTERNS_H_INCLUDED
#define EXTERNS_H_INCLUDED
#include "includes.h"


extern CursesMoviest movie;
extern unsigned char bigletters[27][5][7][4];
extern unsigned char newstops[5][80][5][4];
extern unsigned char newspic[20][78][18][4];

extern unsigned long seed;
extern short law[LAWNUM];

extern short house[435];
extern short senate[100];
extern short court[9];
extern char courtname[9][80];



extern short exec[EXECNUM];
extern short execterm;
extern char execname[EXECNUM][80];


extern unsigned long stat_recruits;
extern unsigned long stat_kidnappings;
extern unsigned long stat_dead;
extern unsigned long stat_kills;
extern unsigned long stat_funds;
extern unsigned long stat_spent;
extern unsigned long stat_buys;
extern unsigned long stat_burns;

extern unsigned long ustat_recruits;
extern unsigned long ustat_kidnappings;
extern unsigned long ustat_dead;
extern unsigned long ustat_kills;
extern unsigned long ustat_funds;
extern unsigned long ustat_spent;
extern unsigned long ustat_buys;
extern unsigned long ustat_burns;

extern int locx;
extern int locy;
extern int locz;

extern short sitetype;
extern short sitealienate;
extern short sitealarm;
extern short sitealarmtimer;
extern short postalarmtimer;
extern long sitecrime;
extern long cursite;

extern short interface_pgup;
extern short interface_pgdn;

extern int day;

extern short attitude[VIEWNUM];

extern short newspaper_topicwork[VIEWNUM];

extern unsigned long funds;

extern char loaded;

extern int mode;

extern short offended_cops;
extern short offended_corps;
extern short offended_cia;
extern short offended_amradio;
extern short offended_cablenews;
extern int police_heat;
extern unsigned long attorneyseed;
extern long selectedsiege;
extern char lcityname[80];
extern char newscherrybusted;

extern int month;
extern int year;
extern int amendnum;

extern long moneygained_donate;
extern long moneygained_brownies;
extern long moneygained_goods;
extern long moneygained_ccfraud;
extern long moneygained_hustling;
extern long moneygained_thievery;
extern long moneylost_goods;
extern long moneylost_trouble;
extern long moneylost_rent;
extern long moneylost_manufacture;
extern long moneylost_legal;
extern long moneylost_compound;
extern long moneylost_hostage;

extern short party_status;


extern vector<datest *> date;

extern vector<locationst *> location;

extern vector<newsstoryst *> newsstory;
extern newsstoryst *sitestory;

extern vector<itemst *> groundloot;
extern long curcreatureid;
extern chaseseqst chaseseq;

extern char slogan[80];

extern vector<creaturest *> pool;

extern vector<blogpostst *> blogpost;

extern vector<squadst *> squad;
extern squadst *activesquad;
extern long cursquadid;

extern char disbanding;
extern int disbandtime;
extern char cantseereason;

extern vector<vehiclest *> vehicle;
extern long curcarid;
extern char showcarprefs;

extern siteblockst map[MAPX][MAPY][MAPZ];
extern creaturest encounter[ENCMAX];
extern highscorest score[SCORENUM];
extern int yourscore;

#endif // EXTERNS_H_INCLUDED
