#include "includes.h"

std::string tostring(long i)
{
   std::ostringstream os;
   os << i;
   return os.str();
}

int stringtobool(std::string boolstr)
{
   std::transform(boolstr.begin(), boolstr.end(), boolstr.begin(), ::tolower);
   if (boolstr == "true" || boolstr == "1")
      return 1;
   else if (boolstr == "false" || boolstr == "0")
      return 0;
   else
      return -1;
}

short creaturetype_string_to_enum(const std::string& ctname)
{
   if (ctname == "CREATURE_BOUNCER")
      return CREATURE_BOUNCER;
   else if (ctname == "CREATURE_SECURITYGUARD")
      return CREATURE_SECURITYGUARD;
   else if (ctname == "CREATURE_SCIENTIST_LABTECH")
      return CREATURE_SCIENTIST_LABTECH;
   else if (ctname == "CREATURE_SCIENTIST_EMINENT")
      return CREATURE_SCIENTIST_EMINENT;
   else if (ctname == "CREATURE_CORPORATE_MANAGER")
      return CREATURE_CORPORATE_MANAGER;
   else if (ctname == "CREATURE_CORPORATE_CEO")
      return CREATURE_CORPORATE_CEO;
   else if (ctname == "CREATURE_WORKER_SERVANT")
      return CREATURE_WORKER_SERVANT;
   else if (ctname == "CREATURE_WORKER_JANITOR")
      return CREATURE_WORKER_JANITOR;
   else if (ctname == "CREATURE_WORKER_SWEATSHOP")
      return CREATURE_WORKER_SWEATSHOP;
   else if (ctname == "CREATURE_WORKER_FACTORY_NONUNION")
      return CREATURE_WORKER_FACTORY_NONUNION;
   else if (ctname == "CREATURE_WORKER_FACTORY_CHILD")
      return CREATURE_WORKER_FACTORY_CHILD;
   else if (ctname == "CREATURE_WORKER_SECRETARY")
      return CREATURE_WORKER_SECRETARY;
   else if (ctname == "CREATURE_WORKER_FACTORY_UNION")
      return CREATURE_WORKER_FACTORY_UNION;
   else if (ctname == "CREATURE_LANDLORD")
      return CREATURE_LANDLORD;
   else if (ctname == "CREATURE_TEENAGER")
      return CREATURE_TEENAGER;
   else if (ctname == "CREATURE_COP")
      return CREATURE_COP;
   else if (ctname == "CREATURE_SWAT")
      return CREATURE_SWAT;
   else if (ctname == "CREATURE_DEATHSQUAD")
      return CREATURE_DEATHSQUAD;
   else if (ctname == "CREATURE_FIREFIGHTER")
      return CREATURE_FIREFIGHTER;
   else if (ctname == "CREATURE_EDUCATOR")
      return CREATURE_EDUCATOR;
   else if (ctname == "CREATURE_GANGUNIT")
      return CREATURE_GANGUNIT;
   else if (ctname == "CREATURE_JUDGE_LIBERAL")
      return CREATURE_JUDGE_LIBERAL;
   else if (ctname == "CREATURE_JUDGE_CONSERVATIVE")
      return CREATURE_JUDGE_CONSERVATIVE;
   else if (ctname == "CREATURE_AGENT")
      return CREATURE_AGENT;
   else if (ctname == "CREATURE_RADIOPERSONALITY")
      return CREATURE_RADIOPERSONALITY;
   else if (ctname == "CREATURE_NEWSANCHOR")
      return CREATURE_NEWSANCHOR;
   else if (ctname == "CREATURE_GENETIC")
      return CREATURE_GENETIC;
   else if (ctname == "CREATURE_GUARDDOG")
      return CREATURE_GUARDDOG;
   else if (ctname == "CREATURE_PRISONER")
      return CREATURE_PRISONER;
   else if (ctname == "CREATURE_JUROR")
      return CREATURE_JUROR;
   else if (ctname == "CREATURE_LAWYER")
      return CREATURE_LAWYER;
   else if (ctname == "CREATURE_SEWERWORKER")
      return CREATURE_SEWERWORKER;
   else if (ctname == "CREATURE_COLLEGESTUDENT")
      return CREATURE_COLLEGESTUDENT;
   else if (ctname == "CREATURE_MUSICIAN")
      return CREATURE_MUSICIAN;
   else if (ctname == "CREATURE_MATHEMATICIAN")
      return CREATURE_MATHEMATICIAN;
   else if (ctname == "CREATURE_TEACHER")
      return CREATURE_TEACHER;
   else if (ctname == "CREATURE_HSDROPOUT")
      return CREATURE_HSDROPOUT;
   else if (ctname == "CREATURE_BUM")
      return CREATURE_BUM;
   else if (ctname == "CREATURE_GANGMEMBER")
      return CREATURE_GANGMEMBER;
   else if (ctname == "CREATURE_CRACKHEAD")
      return CREATURE_CRACKHEAD;
   else if (ctname == "CREATURE_PRIEST")
      return CREATURE_PRIEST;
   else if (ctname == "CREATURE_ENGINEER")
      return CREATURE_ENGINEER;
   else if (ctname == "CREATURE_FASTFOODWORKER")
      return CREATURE_FASTFOODWORKER;
   else if (ctname == "CREATURE_TELEMARKETER")
      return CREATURE_TELEMARKETER;
   else if (ctname == "CREATURE_OFFICEWORKER")
      return CREATURE_OFFICEWORKER;
   else if (ctname == "CREATURE_FOOTBALLCOACH")
      return CREATURE_FOOTBALLCOACH;
   else if (ctname == "CREATURE_PROSTITUTE")
      return CREATURE_PROSTITUTE;
   else if (ctname == "CREATURE_MAILMAN")
      return CREATURE_MAILMAN;
   else if (ctname == "CREATURE_GARBAGEMAN")
      return CREATURE_GARBAGEMAN;
   else if (ctname == "CREATURE_PLUMBER")
      return CREATURE_PLUMBER;
   else if (ctname == "CREATURE_CHEF")
      return CREATURE_CHEF;
   else if (ctname == "CREATURE_CONSTRUCTIONWORKER")
      return CREATURE_CONSTRUCTIONWORKER;
   else if (ctname == "CREATURE_AMATEURMAGICIAN")
      return CREATURE_AMATEURMAGICIAN;
   else if (ctname == "CREATURE_TANK")
      return CREATURE_TANK;
   else if (ctname == "CREATURE_MERC")
      return CREATURE_MERC;
   else if (ctname == "CREATURE_HICK")
      return CREATURE_HICK;
   else if (ctname == "CREATURE_SOLDIER")
      return CREATURE_SOLDIER;
   else if (ctname == "CREATURE_VETERAN")
      return CREATURE_VETERAN;
   else if (ctname == "CREATURE_HARDENED_VETERAN")
      return CREATURE_HARDENED_VETERAN;
   else if (ctname == "CREATURE_PRISONGUARD")
      return CREATURE_PRISONGUARD;
   else if (ctname == "CREATURE_HIPPIE")
      return CREATURE_HIPPIE;
   else if (ctname == "CREATURE_CRITIC_ART")
      return CREATURE_CRITIC_ART;
   else if (ctname == "CREATURE_CRITIC_MUSIC")
      return CREATURE_CRITIC_MUSIC;
   else if (ctname == "CREATURE_SOCIALITE")
      return CREATURE_SOCIALITE;
   else if (ctname == "CREATURE_PROGRAMMER")
      return CREATURE_PROGRAMMER;
   else if (ctname == "CREATURE_RETIREE")
      return CREATURE_RETIREE;
   else if (ctname == "CREATURE_PAINTER")
      return CREATURE_PAINTER;
   else if (ctname == "CREATURE_SCULPTOR")
      return CREATURE_SCULPTOR;
   else if (ctname == "CREATURE_AUTHOR")
      return CREATURE_AUTHOR;
   else if (ctname == "CREATURE_JOURNALIST")
      return CREATURE_JOURNALIST;
   else if (ctname == "CREATURE_DANCER")
      return CREATURE_DANCER;
   else if (ctname == "CREATURE_PHOTOGRAPHER")
      return CREATURE_PHOTOGRAPHER;
   else if (ctname == "CREATURE_CAMERAMAN")
      return CREATURE_CAMERAMAN;
   else if (ctname == "CREATURE_HAIRSTYLIST")
      return CREATURE_HAIRSTYLIST;
   else if (ctname == "CREATURE_FASHIONDESIGNER")
      return CREATURE_FASHIONDESIGNER;
   else if (ctname == "CREATURE_CLERK")
      return CREATURE_CLERK;
   else if (ctname == "CREATURE_THIEF")
      return CREATURE_THIEF;
   else if (ctname == "CREATURE_ACTOR")
      return CREATURE_ACTOR;
   else if (ctname == "CREATURE_YOGAINSTRUCTOR")
      return CREATURE_YOGAINSTRUCTOR;
   else if (ctname == "CREATURE_MARTIALARTIST")
      return CREATURE_MARTIALARTIST;
   else if (ctname == "CREATURE_ATHLETE")
      return CREATURE_ATHLETE;
   else if (ctname == "CREATURE_BIKER")
      return CREATURE_BIKER;
   else if (ctname == "CREATURE_TRUCKER")
      return CREATURE_TRUCKER;
   else if (ctname == "CREATURE_TAXIDRIVER")
      return CREATURE_TAXIDRIVER;
   else if (ctname == "CREATURE_NUN")
      return CREATURE_NUN;
   else if (ctname == "CREATURE_MUTANT")
      return CREATURE_MUTANT;
   else if (ctname == "CREATURE_DOCTOR")
      return CREATURE_DOCTOR;
   else if (ctname == "CREATURE_NURSE")
      return CREATURE_NURSE;
   else if (ctname == "CREATURE_CCS_VIGILANTE")
      return CREATURE_CCS_VIGILANTE;
   else if (ctname == "CREATURE_CCS_ARCHCONSERVATIVE")
      return CREATURE_CCS_ARCHCONSERVATIVE;
   else if (ctname == "CREATURE_POLITICALACTIVIST")
      return CREATURE_POLITICALACTIVIST;
   else if (ctname == "CREATURE_CCS_MOLOTOV")
      return CREATURE_CCS_MOLOTOV;
   else if (ctname == "CREATURE_CCS_SNIPER")
      return CREATURE_CCS_SNIPER;
   else if (ctname == "CREATURE_PSYCHOLOGIST")
      return CREATURE_PSYCHOLOGIST;
   else if (ctname == "CREATURE_LOCKSMITH")
      return CREATURE_LOCKSMITH;
   else if (ctname == "CREATURE_MILITARYPOLICE")
      return CREATURE_MILITARYPOLICE;
   else if (ctname == "CREATURE_SEAL")
      return CREATURE_SEAL;
   else if (ctname == "CREATURE_MILITARYOFFICER")
      return CREATURE_MILITARYOFFICER;
   else if (ctname == "CREATURE_CARSALESMAN")
      return CREATURE_CARSALESMAN;
   else if (ctname == "CREATURE_BANK_TELLER")
      return CREATURE_BANK_TELLER;
   else if (ctname == "CREATURE_BANK_MANAGER")
      return CREATURE_BANK_MANAGER;
   else if (ctname == "CREATURE_SECRET_SERVICE")
      return CREATURE_SECRET_SERVICE;
   else if (ctname == "CREATURE_POLITICIAN")
      return CREATURE_POLITICIAN;
   else
      return -1;
}

int attribute_string_to_enum(const std::string& attribute)
{
   if (attribute == "strength")
      return ATTRIBUTE_STRENGTH;
   else if (attribute == "intelligence")
      return ATTRIBUTE_INTELLIGENCE;
   else if (attribute == "wisdom")
      return ATTRIBUTE_WISDOM;
   else if (attribute == "agility")
      return ATTRIBUTE_AGILITY;
   else if (attribute == "health")
      return ATTRIBUTE_HEALTH;
   else if (attribute == "charisma")
      return ATTRIBUTE_CHARISMA;
   else if (attribute == "heart")
      return ATTRIBUTE_HEART;
   else
      return -1;
}

int skill_string_to_enum(std::string skillname)
{
   std::transform(skillname.begin(), skillname.end(), skillname.begin(), ::tolower);
   if (skillname == "handtohand")
      return SKILL_HANDTOHAND;
   else if (skillname == "knife")
      return SKILL_KNIFE;
   else if (skillname == "sword")
      return SKILL_SWORD;
   else if (skillname == "club")
      return SKILL_CLUB;
   else if (skillname == "axe")
      return SKILL_AXE;
   else if (skillname == "throwing")
      return SKILL_THROWING;
   else if (skillname == "pistol")
      return SKILL_PISTOL;
   else if (skillname == "rifle")
      return SKILL_RIFLE;
   else if (skillname == "smg")
      return SKILL_SMG;
   else if (skillname == "shotgun")
      return SKILL_SHOTGUN;
   else if (skillname == "persuasion")
      return SKILL_PERSUASION;
   else if (skillname == "psychology")
      return SKILL_PSYCHOLOGY;
   else if (skillname == "law")
      return SKILL_LAW;
   else if (skillname == "security")
      return SKILL_SECURITY;
   else if (skillname == "disguise")
      return SKILL_DISGUISE;
   else if (skillname == "computers")
      return SKILL_COMPUTERS;
   else if (skillname == "tailoring")
      return SKILL_TAILORING;
   else if (skillname == "driving")
      return SKILL_DRIVING;
   else if (skillname == "writing")
      return SKILL_WRITING;
   else if (skillname == "music")
      return SKILL_MUSIC;
   else if (skillname == "art")
      return SKILL_ART;
   else if (skillname == "religion")
      return SKILL_RELIGION;
   else if (skillname == "science")
      return SKILL_SCIENCE;
   else if (skillname == "business")
      return SKILL_BUSINESS;
   else if (skillname == "stealth")
      return SKILL_STEALTH;
   else if (skillname == "teaching")
      return SKILL_TEACHING;
   else if (skillname == "streetsense")
      return SKILL_STREETSENSE;
   else if (skillname == "seduction")
      return SKILL_SEDUCTION;
   else if (skillname == "firstaid")
      return SKILL_FIRSTAID;
   else if (skillname == "dodge")
      return SKILL_DODGE;
   else if (skillname == "heavyweapons")
      return SKILL_HEAVYWEAPONS;
   else
      return -1;
}

int gender_string_to_enum(const std::string& gender)
{
   if (gender == "NEUTRAL")
      return GENDER_NEUTRAL;
   else if (gender == "MALE")
      return GENDER_MALE;
   else if (gender == "FEMALE")
      return GENDER_FEMALE;
   else if (gender == "GENDER_WHITEMALEPATRIARCH")
      return GENDER_WHITEMALEPATRIARCH;
   else if (gender == "MALE BIAS")
      return GENDER_MALE_BIAS;
   else if (gender == "FEMALE BIAS")
      return GENDER_FEMALE_BIAS;
   else if (gender == "RANDOM")
      return GENDER_RANDOM;
   else
      return -1;
}

int severtype_string_to_enum(const std::string& severtype)
{
   if (severtype == "NASTY")
      return WOUND_NASTYOFF;
   else if (severtype == "CLEAN")
      return WOUND_CLEANOFF;
   else if (severtype == "NONE")
      return 0;
   else
      return -1;
}
