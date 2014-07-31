
#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include "includes.h"

class date
{
public:
   int day;
   int month;
   int year;
};



#define HOUSENUM 435
#define SENATENUM 100
#define COURTNUM 9

class executivebranch
{
public:
   signed char alignment[EXECNUM];
   string name[EXECNUM];
   bool secondterm;
   bool party; // should make organization ID?
};

class house
{
public:
   signed char alignment[HOUSENUM];
};

class senate
{
public:
   signed char alignment[SENATENUM];
};

class supremecourt
{
public:
   signed char alignment[COURTNUM];
   string name[COURTNUM];
};

class constitution
{
public:
   int AmendmentNumber;
};

class government
{
public:
   short attitude[VIEWNUM];

   class law            Law[LAWNUM];
   class constitution   Constitution;
   class house          House;
   class senate         Senate;
   class supremecourt   SupremeCourt;
};

enum Income
{
   INCOME_DONATIONS,
   INCOME_SALEOFBROWNIES,
   INCOME_SALEOFGOODS,
   INCOME_CREDITCARDFRAUD,
   INCOME_HUSTLING,
   INCOME_PROSTITUTION,
   INCOME_EXTORTION,
   INCOME_THIEVERY,
   INCOMENUM
};

enum Expense
{
   EXPENSE_PURCHASEOFGOODS,
   EXPENSE_ACTIVISM,
   EXPENSE_RENT,
   EXPENSE_TRAINING,
   EXPENSE_MANUFACTURING,
   EXPENSE_LEGALCOSTS,
   EXPENSE_GROCERIES,
   EXPENSE_DATING,
   EXPENSE_COMPOUNDINVESTMENTS,
   EXPENSE_HOSTAGETENDING,
   EXPENSENUM
};

class accounting
{
private:
   int IncomeTable[INCOMENUM];
   int ExpenseTable[EXPENSENUM];
   int Balance;
public:
   int operator[](enum Expense expense_type)
   {
      return ExpenseTable[expense_type];
   }
   int operator[](enum Income income_type)
   {
      return IncomeTable[income_type];
   }

   bool CanAfford(int amount)
   {
      if(Balance >= amount)return true;
      else return false;
   }

   bool TrySpendMoney(enum Expense expense_type, int amount)
   {
      if(CanAfford(amount)==false)
         return false;
      else
         SpendMoney(expense_type,amount);
         return true;
   }

   void SpendMoney(enum Expense expense_type, int amount)
   {
      ExpenseTable[expense_type]+=amount;
      Balance-=amount;
   }
   void MakeMoney(enum Income income_type, int amount)
   {
      IncomeTable[income_type]+=amount;
      Balance+=amount;
   }
   int CurrentBalance()
   {
      return Balance;
   }
};

class statistics
{
public:
   int recruits;
   int dead;
   int kills;
   int kidnappings;
   int spent;
   int buys;
   int burns;
};

class crimesquad
{
public:
   string Slogan;
   class statistics Statistics;
   class accounting Accounting;

   vector<vehiclest *>     VehiclePool;
   vector<creaturest *>    LiberalPool;
   vector<squadst *>       SquadPool;

   vector<datest *>        DateList;
   vector<recruitst *>     RecruitList;
};

class gamestate
{
public:
   char EndGameState;
   int CurrentCarID;
   char ShowCarPrefs;
   int CurrentCreatureID;
   int CurrentSquadID;
   bool OffendedCops;
   bool OffendedCorps;
   bool OffendedCIA;
   bool OffendedAMRadio;
   bool OffendedCableNews;
   int SelectedSiege;
   int CCSKills;
   short PartyStatus;
   template<class Archive>
   void serialize(Archive & ar, const unsigned int /* file_version */)
   {
      ar & CCSKills & EndGameState & CurrentCarID & ShowCarPrefs & CurrentCreatureID & CurrentSquadID;
      ar & OffendedCops & OffendedCorps & OffendedCIA & OffendedAMRadio & OffendedCableNews;
      ar & SelectedSiege & PartyStatus;
   }
};

class city
{
public:
   bool policestation_closed;
   bool amradio_closed;
   bool cablenews_closed;

   vector<locationst *>    Locations;

   string CityName;
   int AttorneySeed;

   template<class Archive>
   void serialize(Archive & ar, const unsigned int /* file_version */)
   {
      ar & policestation_closed & amradio_closed & cablenews_closed;
      ar & Locations & CityName & AttorneySeed;
   }
};

class media
{
public:
   bool NewsCherryBusted;
   vector<newsstoryst *> NewsStories;
   short PublicInterest[VIEWNUM];
   short BackgroundLiberalInfluence[VIEWNUM];

   template<class Archive>
   void serialize(Archive & ar, const unsigned int /* file_version */)
   {
      ar & NewsCherryBusted & NewsStories & PublicInterest;
   }
};

class sitemode
{
public:
   sitemode() : activesquad(NULL),sitestory(NULL) { }
   siteblockst levelmap[MAPX][MAPY][MAPZ];
   squadst *activesquad;
   newsstoryst *sitestory;
   chaseseqst chaseseq;
   creaturest encounter[ENCMAX];
   int locx,locy,locz;
   short sitetype;
   short sitealienate;
   short sitealarm;
   short sitealarmtimer;
   short postalarmtimer;
   long sitecrime;
   long cursite;
};

struct globals
{
   globals()
   {
      ustat_recruits=0;
      ustat_kidnappings=0;
      ustat_dead=0;
      ustat_kills=0;
      ustat_funds=0;
      ustat_spent=0;
      ustat_buys=0;
      ustat_burns=0;

      loaded=0;

      disbanding=0;
      disbandtime=0;

      interface_pgup='[';
      interface_pgdn=']';
   }
   int seed;
   int mode;
   class date        Date;
   class government  Government;
   class media       Media;
   class statistics  Statistics;

   class city        City;
   class crimesquad  CrimeSquad;

   // Do not serialize below this line
   class sitemode    SiteMode;

   CursesMoviest movie;
   unsigned char bigletters[27][5][7][4];
   unsigned char newstops[6][80][5][4];
   unsigned char newspic[20][78][18][4];

   char disbanding;
   int disbandtime;
   char cantseereason;

   char loaded;

   unsigned long ustat_recruits;
   unsigned long ustat_kidnappings;
   unsigned long ustat_dead;
   unsigned long ustat_kills;
   unsigned long ustat_funds;
   unsigned long ustat_spent;
   unsigned long ustat_buys;
   unsigned long ustat_burns;

   short interface_pgup;
   short interface_pgdn;
};

#endif //GLOBALS_H_INCLUDED
