/*

Copyright (c) 2002,2003,2004 by Tarn Adams                                            //
//
This file is part of Liberal Crime Squad.                                             //
//
Liberal Crime Squad is free software; you can redistribute it and/or modify     //
it under the terms of the GNU General Public License as published by            //
the Free Software Foundation; either version 2 of the License, or               //
(at your option) any later version.                                             //
//
Liberal Crime Squad is distributed in the hope that it will be useful,          //
but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the                  //
GNU General Public License for more details.                                    //
//
You should have received a copy of the GNU General Public License               //
along with Liberal Crime Squad; if not, write to the Free Software              //
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   02111-1307   USA     //
*/

/*
This file was created by Chris Johnson (grundee@users.sourceforge.net)
by copying code from game.cpp.
To see descriptions of files and functions, see the list at
the bottom of includes.h in the top src folder.
*/

//#include <includes.h>
#include <externs.h>

void getactivity(char *str,activityst &act)
{
   switch(act.type)
   {
   case ACTIVITY_HOSTAGETENDING:
      strcpy(str,"Tending to ");
      {
         int pl=getpoolcreature(act.arg);
         if(pl!=-1)strcat(str,pool[pl]->name);
         else strcat(str,"a bug");
      }
      break;
   case ACTIVITY_RECRUITING:
      strcpy(str,"Recruiting");break;
   case ACTIVITY_REPAIR_ARMOR:
      strcpy(str,"Repairing Clothing");break;
   case ACTIVITY_WHEELCHAIR:
      strcpy(str,"Procuring a Wheelchair");break;
   case ACTIVITY_STEALCARS:
      strcpy(str,"Stealing a Car");break;
   case ACTIVITY_POLLS:
      strcpy(str,"Gathering Opinion Info");break;
   case ACTIVITY_MAKE_ARMOR:
      strcpy(str,"Making ");
      strcat(str,armortype[act.arg]->get_shortname().c_str());
      break;
   case ACTIVITY_TROUBLE:
      strcpy(str,"Causing Trouble");break;
   case ACTIVITY_PROSTITUTION:
      strcpy(str,"Prostituting");break;
   case ACTIVITY_COMMUNITYSERVICE:
      strcpy(str,"Volunteering");break;
   case ACTIVITY_GRAFFITI:
      strcpy(str,"Making Graffiti");break;
   case ACTIVITY_CCFRAUD:
      strcpy(str,"Credit Card Fraud");break;
   case ACTIVITY_DOS_RACKET:
      strcpy(str,"Extorting Websites");break;
   case ACTIVITY_DOS_ATTACKS:
      strcpy(str,"Attacking Websites");break;
   case ACTIVITY_HACKING:
      strcpy(str,"Hacking Networks");break;
   case ACTIVITY_SELL_TSHIRTS:
      strcpy(str,"Selling T-Shirts");break;
   case ACTIVITY_SELL_ART:
      strcpy(str,"Selling Art");break;
   case ACTIVITY_TEACH_POLITICS:
      strcpy(str,"Teaching Politics");break;
   case ACTIVITY_TEACH_FIGHTING:
      strcpy(str,"Teaching Fighting");break;
   case ACTIVITY_TEACH_COVERT:
      strcpy(str,"Teaching Covert Ops");break;
   case ACTIVITY_SELL_MUSIC:
      strcpy(str,"Selling Music");break;
   case ACTIVITY_BURY:
      strcpy(str,"Disposing of Bodies");break;
   case ACTIVITY_DONATIONS:
      strcpy(str,"Soliciting Donations");break;
   case ACTIVITY_SELL_DRUGS:
      strcpy(str,"Selling Brownies");break;
   case ACTIVITY_VISIT:
      strcpy(str,"Going to ");
      strcat(str,location[act.arg]->getname(!location[act.arg]->is_city()).c_str());
      break;
   case ACTIVITY_HEAL:
      strcpy(str,"Tending to Injuries");break;
   case ACTIVITY_NONE:
      strcpy(str,"Laying Low");break;
   case ACTIVITY_WRITE_LETTERS:
      strcpy(str,"Writing letters");break;
   case ACTIVITY_WRITE_GUARDIAN:
      strcpy(str,"Writing news");break;
   case ACTIVITY_CLINIC:
      strcpy(str,"Going to Free CLINIC");break;
   case ACTIVITY_STUDY_DEBATING:
   case ACTIVITY_STUDY_MARTIAL_ARTS:
   case ACTIVITY_STUDY_DRIVING:
   case ACTIVITY_STUDY_PSYCHOLOGY:
   case ACTIVITY_STUDY_FIRST_AID:
   case ACTIVITY_STUDY_LAW:
   case ACTIVITY_STUDY_DISGUISE:
   case ACTIVITY_STUDY_SCIENCE:
   case ACTIVITY_STUDY_BUSINESS:
   //case ACTIVITY_STUDY_COOKING:
   case ACTIVITY_STUDY_GYMNASTICS:
   case ACTIVITY_STUDY_ART:
   case ACTIVITY_STUDY_MUSIC:
   case ACTIVITY_STUDY_TEACHING:
   case ACTIVITY_STUDY_WRITING:
   case ACTIVITY_STUDY_LOCKSMITHING:
      strcpy(str,"Attending Classes");break;
   case ACTIVITY_SLEEPER_LIBERAL:
      strcpy(str,"Promoting Liberalism");break;
   case ACTIVITY_SLEEPER_CONSERVATIVE:
      strcpy(str,"Spouting Conservatism");break;
   case ACTIVITY_SLEEPER_SPY:
      strcpy(str,"Snooping Around");break;
   case ACTIVITY_SLEEPER_RECRUIT:
      strcpy(str,"Recruiting Sleepers");break;
   case ACTIVITY_SLEEPER_JOINLCS:
      strcpy(str,"Quitting Job");break;
   case ACTIVITY_SLEEPER_SCANDAL:
      strcpy(str,"Creating a Scandal");break;
   case ACTIVITY_SLEEPER_EMBEZZLE:
      strcpy(str,"Embezzling Funds");break;
   case ACTIVITY_SLEEPER_STEAL:
      strcpy(str,"Stealing Equipment");break;
   default:
      strcpy(str,"Reporting Bugs to the Dev Team");break;
   }
}

void getrecruitcreature(char *str,int type)
{
   switch(type)
   {
   case CREATURE_BOUNCER:strcpy(str,"Club Security");break;
   case CREATURE_SECURITYGUARD:strcpy(str,"Security Guard");break;
   case CREATURE_SCIENTIST_LABTECH:strcpy(str,"Lab Tech");break;
   case CREATURE_SCIENTIST_EMINENT:strcpy(str,"Eminent Scientist");break;
   case CREATURE_CORPORATE_MANAGER:strcpy(str,"Corporate Manager");break;
   case CREATURE_CORPORATE_CEO:strcpy(str,"Corporate CEO");break;
   case CREATURE_POLITICIAN:strcpy(str,"Politician");break;
   case CREATURE_WORKER_SERVANT:
      if(law[LAW_LABOR]==-2&&law[LAW_CORPORATE]==-2)strcpy(str,"Slave");
      else strcpy(str,"Servant");
      break;
   case CREATURE_WORKER_JANITOR:
      if(law[LAW_LABOR]==2)strcpy(str,"Custodian");
      else strcpy(str,"Janitor");
      break;
   case CREATURE_WORKER_SWEATSHOP:
      if(law[LAW_IMMIGRATION]==2&&law[LAW_LABOR]==2)strcpy(str,"Migrant Worker");
      else strcpy(str,"Sweatshop Worker");
      break;
   case CREATURE_WORKER_FACTORY_NONUNION:strcpy(str,"Factory Worker");break;
   case CREATURE_WORKER_FACTORY_CHILD:strcpy(str,"Child Worker");break;
   case CREATURE_WORKER_SECRETARY:strcpy(str,"Secretary");break;
   case CREATURE_WORKER_FACTORY_UNION:strcpy(str,"Factory Worker");break;
   case CREATURE_LANDLORD:strcpy(str,"Landlord");break;
   case CREATURE_COP:strcpy(str,"Police Officer");break;
   case CREATURE_SWAT:strcpy(str,"SWAT Officer");break;
   case CREATURE_DEATHSQUAD:strcpy(str,"DethSquad Officer");break;
   case CREATURE_GANGUNIT:strcpy(str,"Police Gang Unit");break;
   case CREATURE_FIREFIGHTER:
      if(law[LAW_FREESPEECH]==-2)strcpy(str,"Fireman");
      else strcpy(str,"Firefighter");
      break;
   case CREATURE_PRISONGUARD:strcpy(str,"Prison Guard");break;
   case CREATURE_EDUCATOR:strcpy(str,"Educator");break;
   case CREATURE_MERC:strcpy(str,"Elite Security");break;
   case CREATURE_HICK:strcpy(str,"Redneck");break;
   case CREATURE_SOLDIER:strcpy(str,"Soldier");break;
   case CREATURE_VETERAN:strcpy(str,"Veteran");break;
   case CREATURE_HARDENED_VETERAN:strcpy(str,"Hardened Veteran");break;
   case CREATURE_JUDGE_LIBERAL:strcpy(str,"Liberal Judge");break;
   case CREATURE_JUDGE_CONSERVATIVE:strcpy(str,"Hangin' Judge");break;
   case CREATURE_AGENT:strcpy(str,"Agent");break;
   case CREATURE_RADIOPERSONALITY:strcpy(str,"Radio Personality");break;
   case CREATURE_NEWSANCHOR:strcpy(str,"Cable News Anchor");break;
   case CREATURE_LAWYER:strcpy(str,"Lawyer");break;
   case CREATURE_SEWERWORKER:strcpy(str,"Sewer Worker");break;
   case CREATURE_COLLEGESTUDENT:strcpy(str,"College Student");break;
   case CREATURE_MUSICIAN:strcpy(str,"Musician");break;
   case CREATURE_MATHEMATICIAN:strcpy(str,"Mathematician");break;
   case CREATURE_TEACHER:strcpy(str,"Teacher");break;
   case CREATURE_HSDROPOUT:strcpy(str,"Highschool Dropout");break;
   case CREATURE_BUM:strcpy(str,"Transient");break;
   case CREATURE_MUTANT:strcpy(str,"Mutant");break;
   case CREATURE_GANGMEMBER:strcpy(str,"Gang Member");break;
   case CREATURE_CRACKHEAD:strcpy(str,"Crackhead");break;
   case CREATURE_PRIEST:strcpy(str,"Priest");break;
   case CREATURE_ENGINEER:strcpy(str,"Engineer");break;
   case CREATURE_FASTFOODWORKER:strcpy(str,"Fast Food Worker");break;
   case CREATURE_TELEMARKETER:strcpy(str,"Telemarketer");break;
   case CREATURE_CARSALESMAN:
      if(law[LAW_WOMEN]==-2)strcpy(str,"Car Salesman");
      else strcpy(str,"Car Salesperson");
      break;
   case CREATURE_OFFICEWORKER:strcpy(str,"Office Worker");break;
   case CREATURE_FOOTBALLCOACH:strcpy(str,"Football Coach");break;
   case CREATURE_PROSTITUTE:strcpy(str,"Prostitute");break;
   case CREATURE_MAILMAN:strcpy(str,"Mail Carrier");break;
   case CREATURE_GARBAGEMAN:strcpy(str,"Garbage Collector");break;
   case CREATURE_PLUMBER:strcpy(str,"Plumber");break;
   case CREATURE_CHEF:strcpy(str,"Chef");break;
   case CREATURE_CONSTRUCTIONWORKER:strcpy(str,"Construction Worker");break;
   case CREATURE_AMATEURMAGICIAN:strcpy(str,"Amateur Magician");break;
   case CREATURE_HIPPIE:strcpy(str,"Hippie");break;
   case CREATURE_CRITIC_ART:strcpy(str,"Art Critic");break;
   case CREATURE_CRITIC_MUSIC:strcpy(str,"Music Critic");break;
   case CREATURE_AUTHOR:strcpy(str,"Author");break;
   case CREATURE_JOURNALIST:strcpy(str,"Journalist");break;
   case CREATURE_SOCIALITE:strcpy(str,"Socialite");break;
   case CREATURE_BIKER:strcpy(str,"Biker");break;
   case CREATURE_TRUCKER:strcpy(str,"Trucker");break;
   case CREATURE_TAXIDRIVER:strcpy(str,"Taxi Driver");break;
   case CREATURE_PROGRAMMER:strcpy(str,"Programmer");break;
   case CREATURE_NUN:strcpy(str,"Nun");break;
   case CREATURE_RETIREE:strcpy(str,"Retiree");break;
   case CREATURE_PAINTER:strcpy(str,"Painter");break;
   case CREATURE_SCULPTOR:strcpy(str,"Sculptor");break;
   case CREATURE_DANCER:strcpy(str,"Dancer");break;
   case CREATURE_PHOTOGRAPHER:strcpy(str,"Photographer");break;
   case CREATURE_CAMERAMAN:strcpy(str,"Cameraman");break;
   case CREATURE_HAIRSTYLIST:strcpy(str,"Hairstylist");break;
   case CREATURE_FASHIONDESIGNER:strcpy(str,"Fashion Designer");break;
   case CREATURE_CLERK:strcpy(str,"Clerk");break;
   case CREATURE_THIEF:strcpy(str,"Professional Thief");break;
   case CREATURE_ACTOR:strcpy(str,"Actor");break;
   case CREATURE_YOGAINSTRUCTOR:strcpy(str,"Yoga Instructor");break;
   case CREATURE_MARTIALARTIST:strcpy(str,"Black Belt");break;
   case CREATURE_ATHLETE:strcpy(str,"Athlete");break;
   case CREATURE_TEENAGER:strcpy(str,"Teenager");break;
   case CREATURE_PRISONER:strcpy(str,"Prisoner");break;
   case CREATURE_DOCTOR:strcpy(str,"Doctor");break;
   case CREATURE_PSYCHOLOGIST:strcpy(str,"Psychologist");break;
   case CREATURE_NURSE:strcpy(str,"Nurse");break;
   case CREATURE_LOCKSMITH:strcpy(str,"Locksmith");break;
   case CREATURE_MILITARYPOLICE:strcpy(str,"Military Police");break;
   case CREATURE_MILITARYOFFICER:strcpy(str,"Military Officer");break;
   case CREATURE_SEAL:strcpy(str,"Navy SEAL");break;
   case CREATURE_BANK_TELLER:strcpy(str,"Bank Teller");break;
   case CREATURE_BANK_MANAGER:strcpy(str,"Bank Manager");break;
   case CREATURE_GUARDDOG:strcpy(str,"Canine Hero");break;
   case CREATURE_GENETIC:strcpy(str,"Genetic Monster");break;
   case CREATURE_TANK:strcpy(str,"M1 Abrams Tank");break;
   case CREATURE_CCS_MOLOTOV:strcpy(str,"Arsonist");break;
   case CREATURE_CCS_SNIPER:strcpy(str,"Sniper");break;
   case CREATURE_CCS_VIGILANTE:strcpy(str,"Vigilante");break;
   case CREATURE_CCS_ARCHCONSERVATIVE:strcpy(str,"CCS Officer");break;
   case CREATURE_POLITICALACTIVIST:strcpy(str,"Political Activist");break;
   case CREATURE_SECRET_SERVICE:strcpy(str,"Secret Service");break;
   default:strcpy(str,"Beta Tester");break;
   }
}

void gettitle(char *str,Creature &cr)
{
   if(cr.align==-1)
   {
      if(cr.juice<=-50)
      {
         if(law[LAW_FREESPEECH]==-2)strcpy(str,"[Darn] Worthless");
         else strcpy(str,"Damn Worthless");
      }
      else if(cr.juice<=-10)strcpy(str,"Conservative Dregs");
      else if(cr.juice<0)strcpy(str,"Conservative Punk");
      else if(cr.juice<10)strcpy(str,"Mindless Conservative");
      else if(cr.juice<50)strcpy(str,"Wrong-Thinker");
      else if(cr.juice<100)
      {
         if(law[LAW_FREESPEECH]==-2)strcpy(str,"Stubborn as [Heck]");
         else strcpy(str,"Stubborn as Hell");
      }
      else if(cr.juice<200)
      {
         if(law[LAW_FREESPEECH]==-2)strcpy(str,"Heartless [Jerk]");
         else strcpy(str,"Heartless Bastard");
      }
      else if(cr.juice<500)strcpy(str,"Insane Vigilante");
      else if(cr.juice<1000)strcpy(str,"Arch-Conservative");
      else strcpy(str,"Evil Incarnate");
   }
   else if(cr.align==0)
   {
      if(cr.juice<=-50)
      {
         if(law[LAW_FREESPEECH]==-2)strcpy(str,"[Darn] Worthless");
         else strcpy(str,"Damn Worthless");
      }
      else if(cr.juice<=-10)strcpy(str,"Society's Dregs");
      else if(cr.juice<0)strcpy(str,"Non-Liberal Punk");
      else if(cr.juice<10)strcpy(str,"Non-Liberal");
      else if(cr.juice<50)strcpy(str,"Hard Working");
      else if(cr.juice<100)strcpy(str,"Respected");
      else if(cr.juice<200)strcpy(str,"Upstanding Citizen");
      else if(cr.juice<500)strcpy(str,"Great Person");
      else if(cr.juice<1000) strcpy(str,"Peacemaker");
      else strcpy(str,"Peace Prize Winner");
   }
   else
   {
      if(cr.juice<=-50)
      {
         if(law[LAW_FREESPEECH]==-2)strcpy(str,"[Darn] Worthless");
         else strcpy(str,"Damn Worthless");
      }
      else if(cr.juice<=-10)strcpy(str,"Society's Dregs");
      else if(cr.juice<0)strcpy(str,"Punk");
      else if(cr.juice<10)strcpy(str,"Civilian");
      else if(cr.juice<50)strcpy(str,"Activist");
      else if(cr.juice<100)strcpy(str,"Socialist Threat");
      else if(cr.juice<200)strcpy(str,"Revolutionary");
      else if(cr.juice<500)strcpy(str,"Urban Commando");
      else if(cr.juice<1000)strcpy(str,"Liberal Guardian");
      else strcpy(str,"Elite Liberal");
   }
}

void getview(char *str,short view)
{
   switch(view)
   {
   case VIEW_GAY:strcpy(str,"LGBT Rights");break;
   case VIEW_DEATHPENALTY:strcpy(str,"The Death Penalty");break;
   case VIEW_TAXES:strcpy(str,"Taxes");break;
   case VIEW_NUCLEARPOWER:strcpy(str,"Nuclear Power");break;
   case VIEW_ANIMALRESEARCH:strcpy(str,"Animal Cruelty");break;
   case VIEW_POLICEBEHAVIOR:strcpy(str,"The Police");break;
   case VIEW_TORTURE:strcpy(str,"Torture");break;
   case VIEW_PRISONS:strcpy(str,"The Prison System");break;
   case VIEW_INTELLIGENCE:strcpy(str,"Privacy");break;
   case VIEW_FREESPEECH:strcpy(str,"Free Speech");break;
   case VIEW_GENETICS:strcpy(str,"Genetics");break;
   case VIEW_JUSTICES:strcpy(str,"The Judiciary");break;
   case VIEW_GUNCONTROL:strcpy(str,"Gun Control");break;
   case VIEW_SWEATSHOPS:strcpy(str,"Labor");break;
   case VIEW_POLLUTION:strcpy(str,"Pollution");break;
   case VIEW_CORPORATECULTURE:strcpy(str,"Corporate Culture");break;
   case VIEW_CEOSALARY:strcpy(str,"CEO Compensation");break;
   case VIEW_WOMEN:strcpy(str,"Gender Equality");break;
   case VIEW_CIVILRIGHTS:strcpy(str,"Racial Equality");break;
   case VIEW_DRUGS:strcpy(str,"Drugs");break;
   case VIEW_IMMIGRATION:strcpy(str,"Immigration");break;
   case VIEW_MILITARY:strcpy(str,"The Military");break;
   case VIEW_AMRADIO:strcpy(str,"AM Radio");break;
   case VIEW_CABLENEWS:strcpy(str,"Cable News");break;
   //case VIEW_POLITICALVIOLENCE:strcpy(str,"The Need For Action");break;
   case VIEW_LIBERALCRIMESQUAD:strcpy(str,"Who We Are");break;
   case VIEW_LIBERALCRIMESQUADPOS:strcpy(str,"Why We Rock");break;
   case VIEW_CONSERVATIVECRIMESQUAD:strcpy(str,"The CCS Criminals");break;
   default:strcpy(str,"Buggy Software");break;
   }
}



void getviewsmall(char *str,short view)
{
   switch(view)
   {
   case VIEW_GAY:strcpy(str,"LGBT rights");break;
   case VIEW_DEATHPENALTY:strcpy(str,"the death penalty");break;
   case VIEW_TAXES:strcpy(str,"taxes");break;
   case VIEW_NUCLEARPOWER:strcpy(str,"nuclear power");break;
   case VIEW_ANIMALRESEARCH:strcpy(str,"animal cruelty");break;
   case VIEW_POLICEBEHAVIOR:strcpy(str,"the cops");break;
   case VIEW_TORTURE:strcpy(str,"torture");break;
   case VIEW_PRISONS:strcpy(str, "the prison system");break;
   case VIEW_INTELLIGENCE:strcpy(str,"privacy");break;
   case VIEW_FREESPEECH:strcpy(str,"free speech");break;
   case VIEW_GENETICS:strcpy(str,"genetic research");break;
   case VIEW_JUSTICES:strcpy(str,"judges");break;
   case VIEW_GUNCONTROL:strcpy(str,"gun control");break;
   case VIEW_SWEATSHOPS:strcpy(str,"labor rights");break;
   case VIEW_POLLUTION:strcpy(str,"pollution");break;
   case VIEW_CORPORATECULTURE:strcpy(str,"corporations");break;
   case VIEW_CEOSALARY:strcpy(str,"CEO compensation");break;
   case VIEW_WOMEN:strcpy(str,"gender equality");break;
   case VIEW_CIVILRIGHTS:strcpy(str,"racial equality");break;
   case VIEW_DRUGS:strcpy(str,"drugs");break;
   case VIEW_IMMIGRATION:strcpy(str,"immigration");break;
   case VIEW_MILITARY:strcpy(str,"the military");break;
   case VIEW_AMRADIO:strcpy(str,"AM radio");break;
   case VIEW_CABLENEWS:strcpy(str,"cable news");break;
   //case VIEW_POLITICALVIOLENCE:strcpy(str,"the need for action");break;
   case VIEW_LIBERALCRIMESQUAD:
   case VIEW_LIBERALCRIMESQUADPOS:strcpy(str,"the LCS");break;
   case VIEW_CONSERVATIVECRIMESQUAD:strcpy(str,"the CCS");break;
   default:strcpy(str,"bugs");break;
   }
}



void getlaw(char *str,int l)
{
   switch(l)
   {
   case LAW_ABORTION:strcpy(str,"Abortion Rights");break;
   case LAW_ANIMALRESEARCH:strcpy(str,"Animal Research");break;
   case LAW_POLICEBEHAVIOR:strcpy(str,"Police Behavior");break;
   case LAW_PRIVACY:strcpy(str,"Privacy");break;
   case LAW_DEATHPENALTY:strcpy(str,"Capital Punishment");break;
   case LAW_NUCLEARPOWER:strcpy(str,"Nuclear Power");break;
   case LAW_POLLUTION:strcpy(str,"Pollution");break;
   case LAW_LABOR:strcpy(str,"Workers' Rights");break;
   case LAW_GAY:strcpy(str,"Homosexual Rights");break;
   case LAW_CORPORATE:strcpy(str,"Corporate Ethics");break;
   case LAW_FREESPEECH:strcpy(str,"Freedom of Speech");break;
   case LAW_TAX:strcpy(str,"Tax Law");break;
   case LAW_FLAGBURNING:strcpy(str,"Flag Burning");break;
   case LAW_GUNCONTROL:strcpy(str,"Gun Control");break;
   case LAW_WOMEN:strcpy(str,"Women's Rights");break;
   case LAW_CIVILRIGHTS:strcpy(str,"Civil Rights");break;
   case LAW_DRUGS:strcpy(str,"Drug Law");break;
   case LAW_IMMIGRATION:strcpy(str,"Immigration");break;
   case LAW_ELECTIONS:strcpy(str,"Election Reform");break;
   case LAW_MILITARY:strcpy(str,"Military Spending");break;
   case LAW_TORTURE:strcpy(str,"Human Rights");break;
   case LAW_PRISONS:strcpy(str,"Prison Regulation");break;
   default:strcpy(str,"Software Bugs");break;
   }
}

void cityname(char *story)
{
   static const char *city_names[] =
   {
      "San Francisco, CA",
      "Boston, MA",
      "Los Angeles, CA",
      "Detroit, MI",
      "Cleveland, OH",
      "Cincinnati, OH",
      "New York, NY",
      "Chicago, IL",
      "Trenton, NJ",
      "Denver, CO",
      "Phoenix, AZ",
      "Little Rock, AR",
      "Houston, TX",
      "Dallas, TX",
      "Hartford, CT",
      "Miami, FL",
      "Baton Rouge, LA",
      "Seattle, WA",
      "Salt Lake City, UT",
      "Philadelphia, PA",
      "San Antonio, TX",
      "Columbus, OH",
      "Atlanta, GA",
      "Buffalo, NY",
      "Orlando, FL",
      "Syracuse, NY",
      "Baltimore, MD",
      //"Washington, DC", // Removed because White House is modeled as a "distant location" in game
      "Memphis, TN",
      "Brooklyn, NY",
      "New Orleans, LA",
      "Albany, NY",
      "Jackson, MS",
      "Waco, TX",
      "Oklahoma, OK",
      "Austin, TX",
      "Nashville, TN",
      "Philadelphia, PA",
      "Tampa, FL",
      "San Diego, CA",
      "El Paso, TX",
      "Baton Rouge, LA"
   };
   strcpy(story,selectRandomString(city_names,ARRAY_ELEMENTS(city_names)));
}

/* Allow the player to enter a name with an optional default */
void enter_name(char *name, int len, char* defname)
{
   keypad(stdscr,FALSE);
   raw_output(FALSE);
   echo();
   curs_set(1);
   getnstr(name,len-1);//-1 because 'len' is normally the full space available and we need one for a terminator.
   curs_set(0);
   noecho();
   raw_output(TRUE);
   keypad(stdscr,TRUE);
   if((defname!=NULL)&&(strncmp(name,"",len-1)==0))strncpy(name,defname,len-1);
   name[len-1]='\0';
}

void getlawflag(char *str,int type)
{
   switch(type)
   {
   case LAWFLAG_TREASON:strcpy(str,"Treason");break;
   case LAWFLAG_TERRORISM:strcpy(str,"Terrorism");break;
   case LAWFLAG_MURDER:strcpy(str,"Murder");break;
   case LAWFLAG_KIDNAPPING:strcpy(str,"Kidnapping");break;
   case LAWFLAG_BANKROBBERY:strcpy(str,"Bank robbery");break;
   case LAWFLAG_ARSON:strcpy(str,"Arson");break;
   case LAWFLAG_BURNFLAG:strcpy(str,(law[LAW_FLAGBURNING]==-2?"Flag Murder":"Flag burning"));break;
   case LAWFLAG_SPEECH:strcpy(str,"Harmful speech");break;
   case LAWFLAG_BROWNIES:strcpy(str,"Drug dealing");break;
   case LAWFLAG_ESCAPED:strcpy(str,"Escaping prison");break;
   case LAWFLAG_HELPESCAPE:strcpy(str,"Releasing prisoners");break;
   case LAWFLAG_JURY:strcpy(str,"Jury tampering");break;
   case LAWFLAG_RACKETEERING:strcpy(str,"Racketeering");break;
   case LAWFLAG_EXTORTION:strcpy(str,"Extortion");break;
   case LAWFLAG_ARMEDASSAULT:strcpy(str,"Armed assault");break;
   case LAWFLAG_ASSAULT:strcpy(str,"Assault");break;
   case LAWFLAG_CARTHEFT:strcpy(str,"Grand theft auto");break;
   case LAWFLAG_CCFRAUD:strcpy(str,"Credit card fraud");break;
   case LAWFLAG_THEFT:strcpy(str,"Theft");break;
   case LAWFLAG_PROSTITUTION:strcpy(str,"Prostitution");break;
   case LAWFLAG_HIREILLEGAL:strcpy(str,(law[LAW_IMMIGRATION]<1?"Hiring illegal aliens":"Hiring undocumented workers"));break;
   //case LAWFLAG_GUNUSE:strcpy(str,"Firing illegal weapons");break;
   //case LAWFLAG_GUNCARRY:strcpy(str,"Carrying illegal weapons");break;
   case LAWFLAG_COMMERCE:strcpy(str,"Electronic sabotage");break;
   case LAWFLAG_INFORMATION:strcpy(str,"Hacking");break;
   case LAWFLAG_BURIAL:strcpy(str,"Unlawful burial");break;
   case LAWFLAG_BREAKING:strcpy(str,"Breaking and entering");break;
   case LAWFLAG_VANDALISM:strcpy(str,"Vandalism");break;
   case LAWFLAG_RESIST:strcpy(str,"Resisting arrest");break;
   case LAWFLAG_DISTURBANCE:strcpy(str,"Disturbing the peace");break;
   case LAWFLAG_PUBLICNUDITY:strcpy(str,"Public nudity");break;
   case LAWFLAG_LOITERING:strcpy(str,"Loitering");break;
   default:strcpy(str,"Finding bugs");break;
   }
}

// shortname defaults to false
std::string getmonth(int month, bool shortname)
{
   switch(month)
   {
   case  1:return (shortname?"Jan.":"January");
   case  2:return (shortname?"Feb.":"February");
   case  3:return (shortname?"Mar.":"March");
   case  4:return (shortname?"Apr.":"April");
   case  5:return "May";
   case  6:return "June";
   case  7:return "July";
   case  8:return (shortname?"Aug.":"August");
   case  9:return (shortname?"Sep.":"September");
   case 10:return (shortname?"Oct.":"October");
   case 11:return (shortname?"Nov.":"November");
   case 12:return (shortname?"Dec.":"December");
   default:return (shortname?"Bug.":"Bugtober");
   }
}
