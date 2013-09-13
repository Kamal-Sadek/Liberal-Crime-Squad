
void saveBoost1(const char * filename) 
{
   std::ofstream ofs(filename);
   boost::archive::text_oarchive oa(ofs);
   //oa & Globals;
};

void loadBoost1(const char * filename) 
{
   std::ifstream ifs(filename);
   boost::archive::text_iarchive ia(ifs);
   //ia & Globals;
};

/* handles saving */
template<typename STREAMTYPE,typename ARCHIVETYPE>
bool saveloadBoost(const char * filename)
{
   STREAMTYPE stream(filename);
   if(!stream.is_open())return false;
   ARCHIVETYPE archive(stream);

   unsigned int lversion=version;
   archive & lversion;
   archive & seed;
   archive & mode;
   archive & day;
   archive & month;
   archive & year;
   archive & execterm;
   archive & presparty;
   archive & amendnum;

   archive & stat_recruits;
   archive & stat_dead;
   archive & stat_kills;
   archive & stat_kidnappings;
   archive & stat_funds;
   archive & stat_spent;
   archive & stat_buys;
   archive & stat_burns;

   archive & policestation_closed;
   archive & amradio_closed;
   archive & cablenews_closed;

   archive & endgamestate;
   archive & ccs_kills;

   archive & curcarid;
   archive & showcarprefs;
   archive & curcreatureid;
   archive & cursquadid;
   archive & offended_cops;
   archive & police_heat;
   archive & offended_corps;
   archive & offended_cia;
   archive & offended_amradio;
   archive & offended_cablenews;
   archive & attorneyseed;
   archive & selectedsiege;
   archive & lcityname;
   archive & newscherrybusted;
   archive & moneygained_donate;
   archive & moneygained_brownies;
   archive & moneygained_goods;
   archive & moneygained_ccfraud;
   archive & moneygained_hustling;
   archive & moneygained_extortion;
   archive & moneygained_thievery;
   archive & moneylost_goods;
   archive & moneylost_trouble;
   archive & moneylost_rent;
   archive & moneylost_training;
   archive & moneylost_manufacture;
   archive & moneylost_legal;
   archive & moneylost_food;
   archive & moneylost_dating;
   archive & moneylost_compound;
   archive & moneylost_hostage;

   archive & slogan;
   archive & funds;
   archive & party_status;

   archive & attitude;
   archive & law;
   archive & house;
   archive & senate;
   archive & court;
   archive & courtname;
   archive & exec;
   archive & execname;

   //LOCATIONS
   archive & location;

   //VEHICLES
   archive & vehicle;

   //POOL
   archive & pool;

   //SQUADS
   archive & squad;

   archive & activesquad;

   //DATES
   archive & date;

   //RECRUITS
   archive & recruit;

   //NEWS STORIES
   archive & newsstory;

   // Liberal Media
   archive & public_interest;
   archive & background_liberal_influence;

   return true;
}