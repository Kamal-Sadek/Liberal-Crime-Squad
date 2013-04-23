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
   strcpy(str,"");

   switch(act.type)
   {
      case ACTIVITY_HOSTAGETENDING:
      {
         strcat(str,"Tending to ");
         int pl=getpoolcreature(act.arg);
         if(pl!=-1)
         {
            strcat(str,pool[pl]->name);
         }
         break;
      }
      case ACTIVITY_REPAIR_ARMOR:
         strcat(str,"Repairing Clothing");
         break;
      case ACTIVITY_WHEELCHAIR:
         strcat(str,"Procuring a Wheelchair");
         break;
      case ACTIVITY_STEALCARS:
         strcat(str,"Stealing a Car");
         break;
      case ACTIVITY_POLLS:
         strcat(str,"Gathering Opinion Info");
         break;
      case ACTIVITY_MAKE_ARMOR:
      {
         strcat(str,"Making ");
         strcat(str,armortype[act.arg]->get_shortname().c_str());
         break;
      }
      case ACTIVITY_TROUBLE:
         strcat(str,"Causing Trouble");
         break;
      case ACTIVITY_PROSTITUTION:
         strcat(str,"Prostituting");
         break;
      case ACTIVITY_COMMUNITYSERVICE:
         strcat(str,"Volunteering");
         break;
      case ACTIVITY_GRAFFITI:
         strcat(str,"Making Graffiti");
         break;
      case ACTIVITY_CCFRAUD:
         strcat(str,"Credit Card Fraud");
         break;
      case ACTIVITY_DOS_RACKET:
         strcat(str,"Extorting Websites");
         break;
      case ACTIVITY_DOS_ATTACKS:
         strcat(str,"Attacking Websites");
         break;
      case ACTIVITY_HACKING:
         strcat(str,"Hacking Networks");
         break;
      case ACTIVITY_SELL_TSHIRTS:
         strcat(str,"Selling T-Shirts");
         break;
      case ACTIVITY_SELL_ART:
         strcat(str,"Selling Art");
         break;
      case ACTIVITY_TEACH_POLITICS:
         strcat(str,"Teaching Politics");
         break;
      case ACTIVITY_TEACH_FIGHTING:
         strcat(str,"Teaching Fighting");
         break;
      case ACTIVITY_TEACH_COVERT:
         strcat(str,"Teaching Covert Ops");
         break;
      case ACTIVITY_SELL_MUSIC:
         strcat(str,"Selling Music");
         break;
      case ACTIVITY_BURY:
         strcat(str,"Disposing of Bodies");
         break;
      case ACTIVITY_DONATIONS:
         strcat(str,"Soliciting Donations");
         break;
      case ACTIVITY_SELL_DRUGS:
         strcat(str,"Selling Brownies");
         break;
      case ACTIVITY_VISIT:
      {
         strcat(str,"Going to ");
         if (location[act.arg]->front_business==-1)
            strcat(str,location[act.arg]->shortname);
         else
            strcat(str,location[act.arg]->front_shortname);
         break;
      }
      case ACTIVITY_HEAL:
         strcat(str,"Tending to Injuries");
         break;
      case ACTIVITY_NONE:
         strcat(str,"Laying Low");
         break;
      case ACTIVITY_WRITE_LETTERS:
		  strcat(str,"Writing letters");
         break;
      case ACTIVITY_WRITE_GUARDIAN:
         strcat(str,"Writing news");
         break;
      case ACTIVITY_CLINIC:
         strcat(str,"Going to Free CLINIC");
         break;
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
		  strcat(str,"Attending Classes");
		  break;
      case ACTIVITY_SLEEPER_LIBERAL:
         strcat(str,"Promoting Liberalism");
         break;
      case ACTIVITY_SLEEPER_CONSERVATIVE:
         strcat(str,"Spouting Conservatism");
         break;
      case ACTIVITY_SLEEPER_SPY:
         strcat(str,"Snooping Around");
         break;
      case ACTIVITY_SLEEPER_RECRUIT:
         strcat(str,"Recruiting Sleepers");
         break;
      case ACTIVITY_SLEEPER_JOINLCS:
         strcat(str,"Quitting Job");
         break;
      case ACTIVITY_SLEEPER_SCANDAL:
         strcat(str,"Creating a Scandal");
         break;
      case ACTIVITY_SLEEPER_EMBEZZLE:
         strcat(str,"Embezzling Funds");
         break;
      case ACTIVITY_SLEEPER_STEAL:
         strcat(str,"Stealing Equipment");
         break;
      default:
         strcat(str,"Reporting Bugs to the Dev Team");
         break;
   }
}


void getrecruitcreature(char *str,int type)
{
   strcpy(str,"");

   switch(type)
   {
      case CREATURE_BOUNCER:strcat(str,"Club Security");break;
      case CREATURE_SECURITYGUARD:strcat(str,"Security Guard");break;
      case CREATURE_SCIENTIST_LABTECH:strcat(str,"Lab Tech");break;
      case CREATURE_SCIENTIST_EMINENT:strcat(str,"Eminent Scientist");break;
      case CREATURE_CORPORATE_MANAGER:strcat(str,"Corporate Manager");break;
      case CREATURE_CORPORATE_CEO:strcat(str,"Corporate CEO");break;
      case CREATURE_WORKER_SERVANT:
         if(law[LAW_LABOR]==-2&&
             law[LAW_CORPORATE]==-2)strcpy(str,"Slave");
         else strcat(str,"Servant");
         break;
      case CREATURE_WORKER_JANITOR:
             if(law[LAW_LABOR]==2)strcpy(str,"Custodian");
         else strcat(str,"Janitor");
         break;
      case CREATURE_WORKER_SWEATSHOP:
         if(law[LAW_IMMIGRATION]==2&&
             law[LAW_LABOR]==2)strcpy(str,"Migrant Worker");
         else strcat(str,"Sweatshop Worker");
         break;
      case CREATURE_WORKER_FACTORY_NONUNION:strcat(str,"Factory Worker");break;
      case CREATURE_WORKER_FACTORY_CHILD:strcat(str,"Child Worker");break;
      case CREATURE_WORKER_SECRETARY:strcat(str,"Secretary");break;
      case CREATURE_WORKER_FACTORY_UNION:strcat(str,"Factory Worker");break;
      case CREATURE_LANDLORD:strcat(str,"Landlord");break;
      case CREATURE_COP:strcat(str,"Police Officer");break;
      case CREATURE_SWAT:strcat(str,"SWAT Officer");break;
      case CREATURE_DEATHSQUAD:strcat(str,"DethSquad Officer");break;
      case CREATURE_GANGUNIT:strcat(str,"Police Gang Unit");break;
      case CREATURE_FIREFIGHTER:strcat(str,"Fire Fighter");break;
      case CREATURE_PRISONGUARD:strcat(str,"Prison Guard");break;
      case CREATURE_EDUCATOR:strcat(str,"Educator");break;
      case CREATURE_MERC:strcat(str,"Elite Security");break;
      case CREATURE_HICK:strcat(str,"Redneck");break;
      case CREATURE_SOLDIER:strcat(str,"Soldier");break;
      case CREATURE_VETERAN:strcat(str,"Veteran");break;
      case CREATURE_HARDENED_VETERAN:strcat(str,"Hardened Veteran");break;
      case CREATURE_JUDGE_LIBERAL:strcat(str,"Judge");break;
      case CREATURE_JUDGE_CONSERVATIVE:strcat(str,"Judge");break;
      case CREATURE_AGENT:strcat(str,"Agent");break;
      case CREATURE_RADIOPERSONALITY:strcat(str,"Radio Personality");break;
      case CREATURE_NEWSANCHOR:strcat(str,"Cable News Anchor");break;
      case CREATURE_LAWYER:strcat(str,"Lawyer");break;
      case CREATURE_SEWERWORKER:strcat(str,"Sewer Worker");break;
      case CREATURE_COLLEGESTUDENT:strcat(str,"College Student");break;
      case CREATURE_MUSICIAN:strcat(str,"Musician");break;
      case CREATURE_MATHEMATICIAN:strcat(str,"Mathematician");break;
      case CREATURE_TEACHER:strcat(str,"Teacher");break;
      case CREATURE_HSDROPOUT:strcat(str,"Highschool Dropout");break;
      case CREATURE_BUM:strcat(str,"Transient");break;
      case CREATURE_MUTANT:strcat(str,"Mutant");break;
      case CREATURE_GANGMEMBER:strcat(str,"Gang Member");break;
      case CREATURE_CRACKHEAD:strcat(str,"Crack Head");break;
      case CREATURE_PRIEST:strcat(str,"Priest");break;
      case CREATURE_ENGINEER:strcat(str,"Engineer");break;
      case CREATURE_FASTFOODWORKER:strcat(str,"Fast Food Worker");break;
      case CREATURE_TELEMARKETER:strcat(str,"Telemarketer");break;
	  case CREATURE_CARSALESMAN:
			 if(law[LAW_WOMEN]==-2)strcpy(str,"Car Salesman");
         else strcat(str,"Car Salesperson");
         break;
	  case CREATURE_OFFICEWORKER:strcat(str,"Office Worker");break;
      case CREATURE_FOOTBALLCOACH:strcat(str,"Football Coach");break;
      case CREATURE_PROSTITUTE:strcat(str,"Prostitute");break;
      case CREATURE_MAILMAN:strcat(str,"Mail Carrier");break;
      case CREATURE_GARBAGEMAN:strcat(str,"Garbage Collector");break;
      case CREATURE_PLUMBER:strcat(str,"Plumber");break;
      case CREATURE_CHEF:strcat(str,"Chef");break;
      case CREATURE_CONSTRUCTIONWORKER:strcat(str,"Construction Worker");break;
      case CREATURE_AMATEURMAGICIAN:strcat(str,"Amateur Magician");break;
      case CREATURE_HIPPIE:strcat(str,"Hippie");break;
      case CREATURE_CRITIC_ART:strcat(str,"Art Critic");break;
      case CREATURE_CRITIC_MUSIC:strcat(str,"Music Critic");break;
      case CREATURE_AUTHOR:strcat(str,"Author");break;
      case CREATURE_JOURNALIST:strcat(str,"Journalist");break;
      case CREATURE_SOCIALITE:strcat(str,"Socialite");break;
      case CREATURE_BIKER:strcat(str,"Biker");break;
      case CREATURE_TRUCKER:strcat(str,"Trucker");break;
      case CREATURE_TAXIDRIVER:strcat(str,"Taxi Driver");break;
      case CREATURE_PROGRAMMER:strcat(str,"Programmer");break;
      case CREATURE_NUN:strcat(str,"Nun");break;
      case CREATURE_RETIREE:strcat(str,"Retiree");break;
      case CREATURE_PAINTER:strcat(str,"Painter");break;
      case CREATURE_SCULPTOR:strcat(str,"Sculptor");break;
      case CREATURE_DANCER:strcat(str,"Dancer");break;
      case CREATURE_PHOTOGRAPHER:strcat(str,"Photographer");break;
      case CREATURE_CAMERAMAN:strcat(str,"Cameraman");break;
      case CREATURE_HAIRSTYLIST:strcat(str,"Hairstylist");break;
      case CREATURE_FASHIONDESIGNER:strcat(str,"Fashion Designer");break;
      case CREATURE_CLERK:strcat(str,"Clerk");break;
      case CREATURE_THIEF:strcat(str,"Professional Thief");break;
      case CREATURE_ACTOR:strcat(str,"Actor");break;
      case CREATURE_YOGAINSTRUCTOR:strcat(str,"Yoga Instructor");break;
      case CREATURE_MARTIALARTIST:strcat(str,"Black Belt");break;
      case CREATURE_ATHLETE:strcat(str,"Athlete");break;
      case CREATURE_TEENAGER:strcat(str,"Teenager");break;
      case CREATURE_PRISONER:strcat(str,"Prisoner");break;
      case CREATURE_DOCTOR:strcat(str,"Doctor");break;
      case CREATURE_PSYCHOLOGIST:strcat(str,"Psychologist");break;
      case CREATURE_NURSE:strcat(str,"Nurse");break;
      case CREATURE_LOCKSMITH:strcat(str,"Locksmith");break;
      case CREATURE_MILITARYPOLICE:strcat(str,"Military Police");break;
      case CREATURE_MILITARYOFFICER:strcat(str,"Military Officer");break;
      case CREATURE_SEAL:strcat(str,"Navy SEAL");break;
      case CREATURE_BANK_TELLER:strcat(str,"Bank Teller");break;
      case CREATURE_BANK_MANAGER:strcat(str,"Bank Manager");break;
      case CREATURE_GUARDDOG:strcat(str,"Canine Hero");break;
      case CREATURE_GENETIC:strcat(str,"Genetic Monster");break;
      case CREATURE_TANK:strcat(str,"M1 Abrams Tank");break;
      case CREATURE_CCS_MOLOTOV:strcat(str,"Arsonist");break;
      case CREATURE_CCS_SNIPER:strcat(str,"Sniper");break;
      case CREATURE_CCS_VIGILANTE:strcat(str,"Vigilante");break;
      case CREATURE_CCS_ARCHCONSERVATIVE:strcat(str,"CCS Officer");break;
      case CREATURE_POLITICALACTIVIST:strcat(str,"Political Activist");break;
      default:
         strcat(str,"Liberal");
         break;
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
   strcpy(str,"");

   switch(view)
   {
      case VIEW_GAY:strcat(str,"LGBT Rights");break;
      case VIEW_DEATHPENALTY:strcat(str,"The Death Penalty");break;
      case VIEW_TAXES:strcat(str,"Taxes");break;
      case VIEW_NUCLEARPOWER:strcat(str,"Nuclear Power");break;
      case VIEW_ANIMALRESEARCH:strcat(str,"Animal Cruelty");break;
      case VIEW_POLICEBEHAVIOR:strcat(str,"The Police");break;
      case VIEW_TORTURE:strcat(str,"Torture");break;
      case VIEW_PRISONS:strcat(str,"The Prison System");break;
      case VIEW_INTELLIGENCE:strcat(str,"Privacy");break;
      case VIEW_FREESPEECH:strcat(str,"Free Speech");break;
      case VIEW_GENETICS:strcat(str,"Genetics");break;
      case VIEW_JUSTICES:strcat(str,"The Judiciary");break;
      case VIEW_GUNCONTROL:strcat(str,"Gun Control");break;
      case VIEW_SWEATSHOPS:strcat(str,"Labor");break;
      case VIEW_POLLUTION:strcat(str,"Pollution");break;
      case VIEW_CORPORATECULTURE:strcat(str,"Corporate Culture");break;
      case VIEW_CEOSALARY:strcat(str,"CEO Compensation");break;
      case VIEW_WOMEN:strcat(str,"Gender Equality");break;
      case VIEW_CIVILRIGHTS:strcat(str,"Racial Equality");break;
      case VIEW_DRUGS:strcat(str,"Drugs");break;
      case VIEW_IMMIGRATION:strcat(str,"Immigration");break;
      case VIEW_MILITARY:strcat(str,"The Military");break;
      case VIEW_AMRADIO:strcat(str,"AM Radio");break;
      case VIEW_CABLENEWS:strcat(str,"Cable News");break;
      //case VIEW_POLITICALVIOLENCE:strcat(str,"The Need For Action");break;
      case VIEW_LIBERALCRIMESQUAD:strcat(str,"Who We Are");break;
      case VIEW_LIBERALCRIMESQUADPOS:strcat(str,"Why We Rock");break;
      case VIEW_CONSERVATIVECRIMESQUAD:strcat(str,"The CCS Criminals");break;
   }
}



void getviewsmall(char *str,short view)
{
   strcpy(str,"");

   switch(view)
   {
      case VIEW_GAY:strcat(str,"LGBT rights");break;
      case VIEW_DEATHPENALTY:strcat(str,"the death penalty");break;
      case VIEW_TAXES:strcat(str,"taxes");break;
      case VIEW_NUCLEARPOWER:strcat(str,"nuclear power");break;
      case VIEW_ANIMALRESEARCH:strcat(str,"animal cruelty");break;
      case VIEW_POLICEBEHAVIOR:strcat(str,"the cops");break;
      case VIEW_TORTURE:strcat(str,"torture");break;
      case VIEW_PRISONS:strcat(str, "the prison system");break;
      case VIEW_INTELLIGENCE:strcat(str,"privacy");break;
      case VIEW_FREESPEECH:strcat(str,"free speech");break;
      case VIEW_GENETICS:strcat(str,"genetic research");break;
      case VIEW_JUSTICES:strcat(str,"judges");break;
      case VIEW_GUNCONTROL:strcat(str,"gun control");break;
      case VIEW_SWEATSHOPS:strcat(str,"labor rights");break;
      case VIEW_POLLUTION:strcat(str,"pollution");break;
      case VIEW_CORPORATECULTURE:strcat(str,"corporations");break;
      case VIEW_CEOSALARY:strcat(str,"CEO compensation");break;
      case VIEW_WOMEN:strcat(str,"gender equality");break;
      case VIEW_CIVILRIGHTS:strcat(str,"racial equality");break;
      case VIEW_DRUGS:strcat(str,"drugs");break;
      case VIEW_IMMIGRATION:strcat(str,"immigration");break;
      case VIEW_MILITARY:strcat(str,"the military");break;
      case VIEW_AMRADIO:strcat(str,"AM radio");break;
      case VIEW_CABLENEWS:strcat(str,"cable news");break;
      //case VIEW_POLITICALVIOLENCE:strcat(str,"the need for action");break;
      case VIEW_LIBERALCRIMESQUAD:strcat(str,"the LCS");break;
      case VIEW_LIBERALCRIMESQUADPOS:strcat(str,"the LCS");break;
      case VIEW_CONSERVATIVECRIMESQUAD:strcat(str,"the CCS");break;
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
      case LAW_WOMEN:addstr("Women's Rights");break;
      case LAW_CIVILRIGHTS:addstr("Civil Rights");break;
      case LAW_DRUGS:addstr("Drug Law");break;
      case LAW_IMMIGRATION:addstr("Immigration");break;
      case LAW_ELECTIONS:addstr("Election Reform");break;
      case LAW_MILITARY:addstr("Military Spending");break;
      case LAW_TORTURE:addstr("Human Rights");break;
      case LAW_PRISONS:addstr("Prison Regulation");break;
   }
}

void cityname(char *story)
{
   switch(LCSrandom(42))
   {
      case 0:strcpy(story,"San Francisco, CA");break;
      case 1:strcpy(story,"Boston, MA");break;
      case 2:strcpy(story,"Los Angeles, CA");break;
      case 3:strcpy(story,"Detroit, MI");break;
      case 4:strcpy(story,"Cleveland, OH");break;
      case 5:strcpy(story,"Cincinnati, OH");break;
      case 6:strcpy(story,"New York, NY");break;
      case 7:strcpy(story,"Chicago, IL");break;
      case 8:strcpy(story,"Trenton, NJ");break;
      case 9:strcpy(story,"Denver, CO");break;
      case 10:strcpy(story,"Phoenix, AZ");break;
      case 11:strcpy(story,"Little Rock, AR");break;
      case 12:strcpy(story,"Houston, TX");break;
      case 13:strcpy(story,"Dallas, TX");break;
      case 14:strcpy(story,"Hartford, CT");break;
      case 15:strcpy(story,"Miami, FL");break;
      case 16:strcpy(story,"Baton Rouge, LA");break;
      case 17:strcpy(story,"Seattle, WA");break;
      case 18:strcpy(story,"Salt Lake City, UT");break;
      case 19:strcpy(story,"Philadelphia, PA");break;
      case 20:strcpy(story,"San Antonio, TX");break;
      case 21:strcpy(story,"Columbus, OH");break;
      case 22:strcpy(story,"Atlanta, GA");break;
      case 23:strcpy(story,"Buffalo, NY");break;
      case 24:strcpy(story,"Orlando, FL");break;
      case 25:strcpy(story,"Syracuse, NY");break;
      case 26:strcpy(story,"Baltimore, MD");break;
      case 27:strcpy(story,"Washington D.C.");break;
      case 28:strcpy(story,"Memphis, TN");break;
      case 29:strcpy(story,"Brooklyn, NY");break;
      case 30:strcpy(story,"New Orleans, LA");break;
      case 31:strcpy(story,"Albany, NY");break;
      case 32:strcpy(story,"Jackson, MS");break;
      case 33:strcpy(story,"Waco, TX");break;
      case 34:strcpy(story,"Oklahoma, OK");break;
      case 35:strcpy(story,"Austin, TX");break;
      case 36:strcpy(story,"Nashville, TN");break;
      case 37:strcpy(story,"Philadelphia, PA");break;
      case 38:strcpy(story,"Tampa, FL");break;
      case 39:strcpy(story,"San Diego, CA");break;
      case 40:strcpy(story,"El Paso, TX");break;
      case 41:strcpy(story,"Baton Rouge, LA");break;
      default:strcpy(story,"ERROR, AR");break;
   }
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

   if((defname!=NULL) && (strncmp(name,"",len-1)==0))
   {
      strncpy(name,defname,len-1);
   }
   name[len-1]='\0';
}

void getlawflag(char *str,int type)
{
   strcpy(str,"");

   switch(type)
   {
   case LAWFLAG_KIDNAPPING:strcat(str,"Kidnapping");break;
   case LAWFLAG_MURDER:strcat(str,"Murder");break;
   case LAWFLAG_THEFT:strcat(str,"Theft");break;
   case LAWFLAG_BREAKING:strcat(str,"Breaking and entering");break;
   case LAWFLAG_TERRORISM:strcat(str,"Terrorism");break;
   case LAWFLAG_JURY:strcat(str,"Jury tampering");break;
   case LAWFLAG_TREASON:strcat(str,"Treason");break;
   case LAWFLAG_ESCAPED:strcat(str,"Escaping prison");break;
   case LAWFLAG_HELPESCAPE:strcat(str,"Releasing prisoners");break;
   case LAWFLAG_RESIST:strcat(str,"Resisting arrest");break;
   case LAWFLAG_BURNFLAG:strcat(str,"Flag burning");break;
   case LAWFLAG_SPEECH:strcat(str,"Harmful speech");break;
   case LAWFLAG_VANDALISM:strcat(str,"Vandalism");break;
   case LAWFLAG_ASSAULT:strcat(str,"Assault");break;
   case LAWFLAG_ARMEDASSAULT:strcat(str,"Armed assault");break;
   case LAWFLAG_CARTHEFT:strcat(str,"Grand theft auto");break;
   case LAWFLAG_INFORMATION:strcat(str,"Hacking");break;
   case LAWFLAG_COMMERCE:strcat(str,"Electronic sabotage");break;
   case LAWFLAG_CCFRAUD:strcat(str,"Credit card fraud");break;
   case LAWFLAG_BROWNIES:strcat(str,"Drug dealing");break;
   case LAWFLAG_BURIAL:strcat(str,"Unlawful burial");break;
   case LAWFLAG_PROSTITUTION:strcat(str,"Prostitution");break;
   case LAWFLAG_DISTURBANCE:strcat(str,"Disturbing the peace");break;
   case LAWFLAG_HIREILLEGAL:strcat(str,"Hiring illegal aliens");break;
   case LAWFLAG_RACKETEERING:strcat(str,"Racketeering");break;
   case LAWFLAG_LOITERING:strcat(str,"Loitering");break;
   //case LAWFLAG_GUNCARRY:strcat(str,"Using illegal weapons");break;
   //case LAWFLAG_GUNUSE:strcat(str,"Firing illegal weapons");break;
   case LAWFLAG_ARSON:strcat(str,"Arson");break;
   case LAWFLAG_PUBLICNUDITY:strcat(str,"Public nudity");break;
   case LAWFLAG_EXTORTION:strcat(str,"Extortion");break;
   case LAWFLAG_BANKROBBERY:strcat(str,"Bank robbery");break;
   default:strcpy(str,"ERROR, AR");break;
   }
}
