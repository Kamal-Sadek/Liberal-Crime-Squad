#ifndef EXTERNS_H_INCLUDED
#define EXTERNS_H_INCLUDED
#include "includes.h"


extern CursesMoviest movie;
extern unsigned char bigletters[27][5][7][4];
extern unsigned char newstops[6][80][5][4];
extern unsigned char newspic[20][78][18][4];

extern uint32 seed;
extern int16 law[LAWNUM];

extern int16 house[435];
extern int16 senate[100];
extern int16 court[9];
extern char courtname[9][80];



extern int16 exec[EXECNUM];
extern int16 execterm;
extern char execname[EXECNUM][80];


extern uint32 stat_recruits;
extern uint32 stat_kidnappings;
extern uint32 stat_dead;
extern uint32 stat_kills;
extern uint32 stat_funds;
extern uint32 stat_spent;
extern uint32 stat_buys;
extern uint32 stat_burns;

extern uint32 ustat_recruits;
extern uint32 ustat_kidnappings;
extern uint32 ustat_dead;
extern uint32 ustat_kills;
extern uint32 ustat_funds;
extern uint32 ustat_spent;
extern uint32 ustat_buys;
extern uint32 ustat_burns;

extern int32 locx;
extern int32 locy;
extern int32 locz;

extern int16 sitetype;
extern int16 sitealienate;
extern int16 sitealarm;
extern int16 sitealarmtimer;
extern int16 postalarmtimer;
extern int32 sitecrime;
extern int32 cursite;

extern int16 interface_pgup;
extern int16 interface_pgdn;

extern int32 day;

extern int16 attitude[VIEWNUM];

// Topicwork1 is editorials that are waiting to be written
// Topicwork2 is editorials that have been written
extern int16 newspaper_topicwork1[VIEWNUM];
extern int16 newspaper_topicwork2[VIEWNUM];

extern uint32 funds;

extern char loaded;

extern int32 mode;

extern int16 offended_cops;
extern int16 offended_corps;
extern int16 offended_cia;
extern int16 offended_amradio;
extern int16 offended_cablenews;
extern int32 police_heat;
extern uint32 attorneyseed;
extern int32 selectedsiege;
extern char lcityname[80];
extern char newscherrybusted;

extern int32 month;
extern int32 year;
extern int32 amendnum;

extern int32 moneygained_donate;
extern int32 moneygained_brownies;
extern int32 moneygained_goods;
extern int32 moneygained_ccfraud;
extern int32 moneygained_hustling;
extern int32 moneygained_thievery;
extern int32 moneylost_goods;
extern int32 moneylost_trouble;
extern int32 moneylost_rent;
extern int32 moneylost_manufacture;
extern int32 moneylost_legal;
extern int32 moneylost_compound;
extern int32 moneylost_hostage;

extern int16 party_status;


extern vector<datest *> date;

extern vector<locationst *> location;

extern vector<newsstoryst *> newsstory;
extern newsstoryst *sitestory;

extern vector<itemst *> groundloot;
extern int32 curcreatureid;
extern chaseseqst chaseseq;

extern char slogan[80];

extern vector<creaturest *> pool;

extern vector<squadst *> squad;
extern squadst *activesquad;
extern int32 cursquadid;

extern char disbanding;
extern int32 disbandtime;
extern char cantseereason;

extern vector<vehiclest *> vehicle;
extern int32 curcarid;
extern char showcarprefs;

extern siteblockst map[MAPX][MAPY][MAPZ];
extern creaturest encounter[ENCMAX];
extern highscorest score[SCORENUM];
extern int32 yourscore;

#endif // EXTERNS_H_INCLUDED
