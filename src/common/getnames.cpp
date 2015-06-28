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

// Note: this file is encoded in the PC-8 / Code Page 437 / OEM-US character set
// (The same character set used by Liberal Crime Squad when it is running)
// Certain special characters won't display correctly unless your text editor is
// set to use that character set, such as this e with an accent: ‚

// In Windows Notepad with the Terminal font, OEM/DOS encoding it should work fine.
// You can set this in Notepad by going to Format->Font and choosing the Terminal font,
// then choosing OEM/DOS in the Script dropdown box.

// In Notepad++ go to the Encoding menu, Character sets, Western European, OEM-US... easy!

// In Code::Blocks's editor go to Settings->Editor->the Other Settings tab and
// then pick WINDOWS-437 from the dropdown box and then choose the radio button
// to make this the default encoding and disable auto-detection of the encoding.
// Then close the file and reopen it (since Code::Blocks detects the encoding
// when it opens the file and it can't be changed after that; what we changed was
// how it detects encoding for files it opens in the future, not files already open).

// In Microsoft Visual C++, right-click the file in the Solution Explorer,
// select "Open With...", choose "C++ Source Code Editor (with encoding)",
// then choose "OEM United States - Codepage 437".

// In MS-DOS Editor (included with Windows as EDIT.COM in your system32 directory),
// the codepage will be correct already since it's running in a console window just
// like Liberal Crime Squad. Well OK, the encoding might be wrong, but then it's wrong
// in Liberal Crime Squad TOO, and to fix it, go to Control Panel, Regional and Language Settings,
// Advanced tab, and choose English (United States) from the dropdown box as the encoding
// for non-Unicode applications, then press OK.

// If you have a Linux or other UNIX-based system you are obviously smart enough
// to figure out for yourself how to open a file in OEM-US PC-8 codepage 437 in
// your favorite text editor. If you're on Mac OS X, well that's UNIX-based, figure
// it out for yourself.

#include <externs.h>

std::string getactivity(activityst &act)
{
   switch(act.type)
   {
   case ACTIVITY_HOSTAGETENDING:
      {
         std::string str="Tending to ";
         int pl=getpoolcreature(act.arg);
         if(pl!=-1) return str+pool[pl]->name;
         else return str+"a bug";
      }
   case ACTIVITY_RECRUITING:
      return "Recruiting";
   case ACTIVITY_REPAIR_ARMOR:
      return "Repairing Clothing";
   case ACTIVITY_WHEELCHAIR:
      return "Procuring a Wheelchair";
   case ACTIVITY_STEALCARS:
      return "Stealing a Car";
   case ACTIVITY_POLLS:
      return "Gathering Opinion Info";
   case ACTIVITY_MAKE_ARMOR:
      return "Making "+armortype[act.arg]->get_shortname();
   case ACTIVITY_TROUBLE:
      return "Causing Trouble";
   case ACTIVITY_PROSTITUTION:
      return "Prostituting";
   case ACTIVITY_COMMUNITYSERVICE:
      return "Volunteering";
   case ACTIVITY_GRAFFITI:
      return "Making Graffiti";
   case ACTIVITY_CCFRAUD:
      return "Credit Card Fraud";
   case ACTIVITY_DOS_RACKET:
      return "Extorting Websites";
   case ACTIVITY_DOS_ATTACKS:
      return "Attacking Websites";
   case ACTIVITY_HACKING:
      return "Hacking Networks";
   case ACTIVITY_SELL_TSHIRTS:
      return "Selling T-Shirts";
   case ACTIVITY_SELL_ART:
      return "Selling Art";
   case ACTIVITY_TEACH_POLITICS:
      return "Teaching Politics";
   case ACTIVITY_TEACH_FIGHTING:
      return "Teaching Fighting";
   case ACTIVITY_TEACH_COVERT:
      return "Teaching Covert Ops";
   case ACTIVITY_SELL_MUSIC:
      return "Selling Music";
   case ACTIVITY_BURY:
      return "Disposing of Bodies";
   case ACTIVITY_DONATIONS:
      return "Soliciting Donations";
   case ACTIVITY_SELL_DRUGS:
      return "Selling Brownies";
   case ACTIVITY_VISIT:
      return "Going to "+location[act.arg]->getname(!location[act.arg]->is_city());
   case ACTIVITY_HEAL:
      return "Tending to Injuries";
   case ACTIVITY_NONE:
      return "Laying Low";
   case ACTIVITY_WRITE_LETTERS:
      return "Writing letters";
   case ACTIVITY_WRITE_GUARDIAN:
      return "Writing news";
   case ACTIVITY_CLINIC:
      return "Going to Free CLINIC";
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
   case ACTIVITY_STUDY_COMPUTERS:
      return "Attending Classes";
   case ACTIVITY_SLEEPER_LIBERAL:
      return "Promoting Liberalism";
   case ACTIVITY_SLEEPER_CONSERVATIVE:
      return "Spouting Conservatism";
   case ACTIVITY_SLEEPER_SPY:
      return "Snooping Around";
   case ACTIVITY_SLEEPER_RECRUIT:
      return "Recruiting Sleepers";
   case ACTIVITY_SLEEPER_JOINLCS:
      return "Quitting Job";
   case ACTIVITY_SLEEPER_SCANDAL:
      return "Creating a Scandal";
   case ACTIVITY_SLEEPER_EMBEZZLE:
      return "Embezzling Funds";
   case ACTIVITY_SLEEPER_STEAL:
      return "Stealing Equipment";
   case ACTIVITY_AUGMENTING:
      return "Augmenting Liberal";
   default:
      return "Reporting Bugs to the Dev Team";
   }
}

std::string gettitle(Creature &cr)
{
   if(cr.align==-1)
   {
      if(cr.juice<=-50)
      {
         if(law[LAW_FREESPEECH]==-2) return "[Darn] Worthless";
         else return "Damn Worthless";
      }
      else if(cr.juice<=-10) return "Conservative Dregs";
      else if(cr.juice<0) return "Conservative Punk";
      else if(cr.juice<10) return "Mindless Conservative";
      else if(cr.juice<50) return "Wrong-Thinker";
      else if(cr.juice<100)
      {
         if(law[LAW_FREESPEECH]==-2) return "Stubborn as [Heck]";
         else return "Stubborn as Hell";
      }
      else if(cr.juice<200)
      {
         if(law[LAW_FREESPEECH]==-2) return "Heartless [Jerk]";
         else return "Heartless Bastard";
      }
      else if(cr.juice<500) return "Insane Vigilante";
      else if(cr.juice<1000) return "Arch-Conservative";
      else return "Evil Incarnate";
   }
   else if(cr.align==0)
   {
      if(cr.juice<=-50)
      {
         if(law[LAW_FREESPEECH]==-2) return "[Darn] Worthless";
         else return "Damn Worthless";
      }
      else if(cr.juice<=-10) return "Society's Dregs";
      else if(cr.juice<0) return "Non-Liberal Punk";
      else if(cr.juice<10) return "Non-Liberal";
      else if(cr.juice<50) return "Hard Working";
      else if(cr.juice<100) return "Respected";
      else if(cr.juice<200) return "Upstanding Citizen";
      else if(cr.juice<500) return "Great Person";
      else if(cr.juice<1000) return "Peacemaker";
      else return "Peace Prize Winner";
   }
   else
   {
      if(cr.juice<=-50)
      {
         if(law[LAW_FREESPEECH]==-2) return "[Darn] Worthless";
         else return "Damn Worthless";
      }
      else if(cr.juice<=-10) return "Society's Dregs";
      else if(cr.juice<0) return "Punk";
      else if(cr.juice<10) return "Civilian";
      else if(cr.juice<50) return "Activist";
      else if(cr.juice<100) return "Socialist Threat";
      else if(cr.juice<200) return "Revolutionary";
      else if(cr.juice<500) return "Urban Commando";
      else if(cr.juice<1000) return "Liberal Guardian";
      else return "Elite Liberal";
   }
}

std::string getview(short view,bool shortname)
{
   if(shortname) switch(view)
   {
   case VIEW_STALIN: return "Stalinism";
   case VIEW_MOOD: return "public mood";
   case VIEW_GAY: return "LGBTQ rights";
   case VIEW_DEATHPENALTY: return "the death penalty";
   case VIEW_TAXES: return "taxes";
   case VIEW_NUCLEARPOWER: return "nuclear power";
   case VIEW_ANIMALRESEARCH: return "animal cruelty";
   case VIEW_POLICEBEHAVIOR: return "cops";
   case VIEW_TORTURE: return "torture";
   case VIEW_PRISONS: return "the prison system";
   case VIEW_INTELLIGENCE: return "privacy";
   case VIEW_FREESPEECH: return "free speech";
   case VIEW_GENETICS: return "genetic research";
   case VIEW_JUSTICES: return "judges";
   case VIEW_GUNCONTROL: return "gun control";
   case VIEW_SWEATSHOPS: return "labor unions";
   case VIEW_POLLUTION: return "pollution";
   case VIEW_CORPORATECULTURE: return "corporations";
   case VIEW_CEOSALARY: return "CEO compensation";
   case VIEW_WOMEN: return "women's rights";
   case VIEW_CIVILRIGHTS: return "civil rights";
   case VIEW_DRUGS: return "drugs";
   case VIEW_IMMIGRATION: return "immigration";
   case VIEW_MILITARY: return "the military";
   case VIEW_AMRADIO: return "AM radio";
   case VIEW_CABLENEWS: return "cable news";
   //case VIEW_POLITICALVIOLENCE: return "political violence";
   case VIEW_LIBERALCRIMESQUAD:
   case VIEW_LIBERALCRIMESQUADPOS: return "the LCS";
   case VIEW_CONSERVATIVECRIMESQUAD: return "the CCS";
   default: return "bugs";
   }
   else switch(view)
   {
   case VIEW_STALIN: return "Stalinism";
   case VIEW_MOOD: return "Public Mood";
   case VIEW_GAY: return "LGBTQ Rights";
   case VIEW_DEATHPENALTY: return "Barbaric Executions";
   case VIEW_TAXES: return "The Tax Structure";
   case VIEW_NUCLEARPOWER: return "Nuclear Meltdowns";
   case VIEW_ANIMALRESEARCH: return "Animal Cruelty";
   case VIEW_POLICEBEHAVIOR: return "Police Misconduct";
   case VIEW_TORTURE: return "Torture";
   case VIEW_PRISONS: return "The Prison System";
   case VIEW_INTELLIGENCE: return "Privacy Rights";
   case VIEW_FREESPEECH: return "Freedom of Speech";
   case VIEW_GENETICS: return "Dangerous GMOs";
   case VIEW_JUSTICES: return "The Judiciary";
   case VIEW_GUNCONTROL: return "Mass Shootings";
   case VIEW_SWEATSHOPS: return "Workers' Rights";
   case VIEW_POLLUTION: return "Pollution";
   case VIEW_CORPORATECULTURE: return "Corporate Corruption";
   case VIEW_CEOSALARY: return "CEO Compensation";
   case VIEW_WOMEN: return "Gender Equality";
   case VIEW_CIVILRIGHTS: return "Racial Equality";
   case VIEW_DRUGS: return "Oppressive Drug Laws";
   case VIEW_IMMIGRATION: return "Immigrant Rights";
   case VIEW_MILITARY: return "Military Spending";
   case VIEW_AMRADIO: return "AM Radio Propaganda";
   case VIEW_CABLENEWS: return "Cable \"News\" Lies";
   //case VIEW_POLITICALVIOLENCE: return "The Need For Action";
   case VIEW_LIBERALCRIMESQUAD: return "Who We Are";
   case VIEW_LIBERALCRIMESQUADPOS: return "Why We Rock";
   case VIEW_CONSERVATIVECRIMESQUAD: return "The CCS Terrorists";
   default: return "Buggy Software";
   }
}



std::string getlaw(int l)
{
   switch(l)
   {
   case LAW_ABORTION:return "Abortion Rights";
   case LAW_ANIMALRESEARCH:return "Animal Rights"; // a.k.a. "Animal Research"
   case LAW_POLICEBEHAVIOR:return "Police Regulation"; // a.k.a. "Police Behavior"
   case LAW_PRIVACY:return "Privacy Rights"; // a.k.a. "Privacy"
   case LAW_DEATHPENALTY:return "Death Penalty"; // a.k.a. "Capital Punishment"
   case LAW_NUCLEARPOWER:return "Nuclear Power";
   case LAW_POLLUTION:return "Pollution";
   case LAW_LABOR:return "Labor Laws"; // a.k.a. "Workers' Rights"
   case LAW_GAY:return "Gay Rights"; // a.k.a. "Homosexual Rights"
   case LAW_CORPORATE:return "Corporate Law"; // a.k.a. "Corporate Ethics"
   case LAW_FREESPEECH:return "Free Speech"; // a.k.a. "Freedom of Speech"
   case LAW_FLAGBURNING:return "Flag Burning";
   case LAW_GUNCONTROL:return "Gun Control";
   case LAW_TAX:return "Tax Structure"; // a.k.a. "Tax Law"
   case LAW_WOMEN:return "Women's Rights";
   case LAW_CIVILRIGHTS:return "Civil Rights";
   case LAW_DRUGS:return "Drug Laws"; // a.k.a. "Drug Law"
   case LAW_IMMIGRATION:return "Immigration";
   case LAW_ELECTIONS:return "Election Reform";
   case LAW_MILITARY:return "Military Spending";
   case LAW_PRISONS:return "Prison Regulation";
   case LAW_TORTURE:return "Human Rights";
   default:return "Software Bugs";
   }
}

std::string cityname()
{
   static const char *city_names[] =
   {
      /* City population < 100,000 = listed once if the city is somehow important
         (i.e., biggest city in a U.S. state or territory, or most important city in a
          metropolitan area with over 200,000 people)
         100,000 < City population <   200,000 = listed once
         200,000 < City population <   400,000 = listed twice
         400,000 < City population <   600,000 = listed 3 times
         600,000 < City population < 1,000,000 = listed 4 times
       1,000,000 < City population < 5,000,000 = listed 5 times
       5,000,000 < City population = city listed 6 times and each of its 5 its boroughs listed 1-2 times
       Anyway, for states/territories without big cities their biggest city is listed once.  */
      "Abilene, TX",
      "Aguadilla, PR", // city in Puerto Rico
      "Akron, OH",
      "Albany, NY",
      "Albuquerque, NM", "Albuquerque, NM", "Albuquerque, NM",
      "Alexandria, VA",
      "Allentown, PA",
      "Amarillo, TX",
      "Anaheim, CA", "Anaheim, CA", "Anaheim, CA",
      "Anchorage, AK", "Anchorage, AK", "Anchorage, AK",
      "Ann Arbor, MI",
      "Antioch, CA",
      "Appleton, WI",
      "Arlington, TX", "Arlington, TX", "Arlington, TX",
      "Arvada, CO",
      "Asheville, NC",
      "Atlantic City, NJ",
      "Athens, GA",
      "Atlanta, GA", "Atlanta, GA", "Atlanta, GA",
      "Augusta, GA",
      "Aurora, CO", "Aurora, CO", "Aurora, CO",
      "Aurora, IL",
      "Austin, TX", "Austin, TX", "Austin, TX", "Austin, TX",
      "Bakersfield, CA", "Bakersfield, CA", "Bakersfield, CA",
      "Baltimore, MD", "Baltimore, MD", "Baltimore, MD", "Baltimore, MD",
      "Barnstable, MA",
      "Baton Rouge, LA", "Baton Rouge, LA",
      "Bayam¢n, PR", "Bayam¢n, PR", // city in Puerto Rico
      "Bellevue, WA",
      "Bellingham, WA",
      "Beaumont, TX",
      "Berkeley, CA",
      "Billings, MT",
      "Biloxi, MS",
      "Binghamton, NY",
      "Birmingham, AL", "Birmingham, AL",
      "Boise, ID", "Boise, ID",
      "Boston, MA", "Boston, MA", "Boston, MA", "Boston, MA",
      "Boulder, CO",
      "Bremerton, WA",
      "Bridgeport, CT",
      "Broken Arrow, OK",
      "Brooklyn, NY", "Brooklyn, NY",
      "Brownsville, TX",
      "Buffalo, NY", "Buffalo, NY",
      "Burbank, CA",
      "Burlington, VT",
      "Cambridge, MA",
      "Caguas, PR", // city in Puerto Rico
      "Canton, OH",
      "Cape Coral, FL",
      "Carlsbad, CA",
      "Carolina, PR", // city in Puerto Rico
      "Carrollton, TX",
      "Cary, NC",
      "Cedar Rapids, IA",
      "Centennial, CO",
      "Champaign, IL",
      "Chandler, AZ", "Chandler, AZ",
      "Charleston, SC",
      "Charleston, WV",
      "Charlotte, NC", "Charlotte, NC", "Charlotte, NC", "Charlotte, NC",
      "Charlotte Amalie, VI", // biggest city in U.S. Virgin Islands
      "Charlottesville, VA",
      "Chattanooga, TN",
      "Chesapeake, VA", "Chesapeake, VA",
      "Cheyenne, WY",
      "Chicago, IL", "Chicago, IL", "Chicago, IL", "Chicago, IL", "Chicago, IL",
      "Chico, CA",
      "Chula Vista, CA", "Chula Vista, CA",
      "Cincinnati, OH", "Cincinnati, OH",
      "Clarksville, TN",
      "Clearwater, FL",
      "Cleveland, OH", "Cleveland, OH", "Cleveland, OH",
      "College Station, TX",
      "Colorado Springs, CO", "Colorado Springs, CO", "Colorado Springs, CO",
      "Columbia, MO",
      "Columbia, SC",
      "Columbus, GA", "Columbus, GA",
      "Columbus, OH", "Columbus, OH", "Columbus, OH", "Columbus, OH",
      "Concord, CA",
      "Coral Springs, FL",
      "Corona, CA",
      "Corpus Christi, TX", "Corpus Christi, TX", "Corpus Christi, TX",
      "Costa Mesa, CA",
      "Crestview, FL",
      "Dallas, TX", "Dallas, TX", "Dallas, TX", "Dallas, TX", "Dallas, TX",
      "Daly City, CA",
      "Davenport, IA",
      "Dayton, OH",
      "Daytona Beach, FL",
      "Dededo, GU", // biggest city in Guam
      "Denton, TX",
      "Denver, CO", "Denver, CO", "Denver, CO", "Denver, CO",
      "Des Moines, IA", "Des Moines, IA",
      "Detroit, MI", "Detroit, MI", "Detroit, MI", "Detroit, MI",
      "Downey, CA",
      "Duluth, MN",
      "Durham, NC", "Durham, NC",
      "El Cajon, CA",
      "El Monte, CA",
      "El Paso, TX", "El Paso, TX", "El Paso, TX", "El Paso, TX",
      "Elgin, IL",
      "Elk Grove, CA",
      "Elkhart, IN",
      "Elizabeth, NJ",
      "Erie, PA",
      "Escondido, CA",
      "Eugene, OR",
      "Evansville, IN",
      "Everett, WA",
      "Fairfield, CA",
      "Fargo, ND",
      "Fayetteville, AR",
      "Fayetteville, NC", "Fayetteville, NC",
      "Flint, MI",
      "Florence, SC",
      "Fontana, CA", "Fontana, CA",
      "Fort Collins, CO",
      "Fort Lauderdale, FL",
      "Fort Smith, AR",
      "Fort Wayne, IN", "Fort Wayne, IN",
      "Fort Worth, TX", "Fort Worth, TX", "Fort Worth, TX", "Fort Worth, TX",
      "Fremont, CA", "Fremont, CA",
      "Fresno, CA", "Fresno, CA", "Fresno, CA",
      "Frisco, TX",
      "Fullerton, TX",
      "Gainesville, FL",
      "Garden Grove, CA",
      "Garland, TX", "Garland, TX",
      "Gilbert, AZ", "Gilbert, AZ",
      "Glendale, AZ", "Glendale, AZ",
      "Glendale, CA",
      "Grand Prairie, TX",
      "Grand Rapids, MI",
      "Greeley, CO",
      "Green Bay, WI",
      "Greensboro, NC", "Greensboro, NC",
      "Greenville, SC",
      "Gresham, OR",
      "Hagerstown, MD",
      "Hampton, VA",
      "Harrisburg, PA",
      "Hartford, CT",
      "Hayward, CA",
      "Hempstead, NY",
      "Henderson, NV", "Henderson, NV",
      "Hialeah, FL", "Hialeah, FL",
      "Hickory, NC",
      "High Point, NC",
      "Hollywood, CA",
      "Hollywood, FL",
      "Honolulu, HI", "Honolulu, HI", "Honolulu, HI",
      "Houma, LA",
      "Houston, TX", "Houston, TX", "Houston, TX", "Houston, TX", "Houston, TX",
      "Huntington, WV",
      "Huntington Beach, CA",
      "Huntsville, AL",
      "Independence, MO",
      "Indianapolis, IN", "Indianapolis, IN", "Indianapolis, IN", "Indianapolis, IN",
      "Inglewood, CA",
      "Irvine, CA", "Irvine, CA",
      "Irving, TX", "Irving, TX",
      "Jackson, MS",
      "Jacksonville, FL", "Jacksonville, FL", "Jacksonville, FL", "Jacksonville, FL",
      "Jersey City, NJ", "Jersey City, NJ",
      "Johnson City, TN",
      "Joliet, IL",
      "Kalamazoo, MI",
      "Kansas City, KS",
      "Kansas City, MO", "Kansas City, MO", "Kansas City, MO",
      "Kent, WA",
      "Killeen, TX",
      "Kingsport, TN",
      "Knoxville, TN",
      "Lafayette, IN",
      "Lafayette, LA",
      "Lake Charles, LA",
      "Lake Havasu City, AZ",
      "Lakeland, FL",
      "Lakewood, CO",
      "Lancaster, CA",
      "Lancaster, PA",
      "Lansing, MI",
      "Laredo, TX", "Laredo, TX",
      "Las Cruces, NM",
      "Las Vegas, NV", "Las Vegas, NV", "Las Vegas, NV", "Las Vegas, NV",
      "Lewisville, TX",
      "Lexington, KY", "Lexington, KY", "Lexington, KY",
      "Lincoln, NE", "Lincoln, NE",
      "Little Rock, AR",
      "Long Beach, CA", "Long Beach, CA", "Long Beach, CA",
      "Long Island, NY", "Long Island, NY", "Long Island, NY", "Long Island, NY",
      "Longview, TX",
      "Los Angeles, CA", "Los Angeles, CA", "Los Angeles, CA", "Los Angeles, CA", "Los Angeles, CA",
      "Louisville, KY", "Louisville, KY", "Louisville, KY", "Louisville, KY",
      "Lowell, MA",
      "Lubbock, TX", "Lubbock, TX",
      "Lynchburg, VA",
      "Macon, GA",
      "Madison, WI", "Madison, WI",
      "Manchester, NH",
      "Manhattan, NY", "Manhattan, NY",
      "Mayagez, PR", // city in Puerto Rico
      "McAllen, TX",
      "McKinney, TX",
      "Medford, OR",
      "Memphis, TN", "Memphis, TN", "Memphis, TN", "Memphis, TN",
      "Merced, CA",
      "Mesa, AZ", "Mesa, AZ", "Mesa, AZ",
      "Mesquite, TX",
      "Miami, FL", "Miami, FL", "Miami, FL",
      "Miami Gardens, FL",
      "Midland, TX",
      "Milwaukee, WI", "Milwaukee, WI", "Milwaukee, WI",
      "Minneapolis, MN", "Minneapolis, MN", "Minneapolis, MN",
      "Miramar, FL",
      "Mobile, AL",
      "Modesto, CA", "Modesto, CA",
      "Montgomery, AL", "Montgomery, AL",
      "Moreno Valley, CA", "Moreno Valley, CA",
      "Murfreesboro, TN",
      "Murrieta, CA",
      "Myrtle Beach, SC",
      "Naperville, IL",
      "Naples, FL",
      "Nashville, TN", "Nashville, TN", "Nashville, TN", "Nashville, TN",
      "New Haven, CT",
      "New London, CT",
      "New Orleans, LA", "New Orleans, LA", "New Orleans, LA",
      "New York, NY", "New York, NY", "New York, NY", "New York, NY", "New York, NY", "New York, NY",
      "Newark, NJ", "Newark, NJ",
      "Newport, RI",
      "Newport News, VA",
      "Norfolk, VA", "Norfolk, VA",
      "Norman, OK",
      "North Charleston, SC",
      "North Las Vegas, NV", "North Las Vegas, NV",
      "Norwalk, CA",
      "Oakland, CA", "Oakland, CA", "Oakland, CA",
      "Ocala, FL",
      "Oceanside, CA",
      "Odessa, TX",
      "Ogden, UT",
      "Oklahoma City, OK", "Oklahoma City, OK", "Oklahoma City, OK", "Oklahoma City, OK",
      "Olathe, KS",
      "Olympia, WA",
      "Omaha, NE", "Omaha, NE", "Omaha, NE",
      "Ontario, CA",
      "Orange, CA",
      "Orlando, FL", "Orlando, FL",
      "Overland Park, KS",
      "Oxnard, CA", "Oxnard, CA",
      "Palm Bay, FL",
      "Palmdale, CA",
      "Pasadena, CA",
      "Pasadena, TX",
      "Paterson, NJ",
      "Pearland, TX",
      "Pembroke Pines, FL",
      "Pensacola, FL",
      "Peoria, IL",
      "Peoria, AZ",
      "Philadelphia, PA", "Philadelphia, PA", "Philadelphia, PA", "Philadelphia, PA", "Philadelphia, PA",
      "Phoenix, AZ", "Phoenix, AZ", "Phoenix, AZ", "Phoenix, AZ", "Phoenix, AZ",
      "Pittsburgh, PA", "Pittsburgh, PA", "Pittsburgh, PA",
      "Plano, TX", "Plano, TX",
      "Pomona, CA",
      "Pompano Beach, FL",
      "Ponce, PR", // city in Puerto Rico
      "Port St. Lucie, FL",
      "Portland, ME",
      "Portland, OR", "Portland, OR", "Portland, OR", "Portland, OR",
      "Poughkeepsie, NY",
      "Prescott, AZ",
      "Providence, RI",
      "Provo, UT",
      "Pueblo, CO",
      "Queens, NY", "Queens, NY",
      "Raleigh, NC", "Raleigh, NC", "Raleigh, NC",
      "Rancho Cucamonga, CA",
      "Reading, PA",
      "Reno, NV", "Reno, NV",
      "Rialto, CA",
      "Richardson, TX",
      "Richland, WA",
      "Richmond, CA",
      "Richmond, VA", "Richmond, VA",
      "Riverside, CA", "Riverside, CA", "Riverside, CA",
      "Roanoke, VA",
      "Rochester, MN",
      "Rochester, NY", "Rochester, NY",
      "Rockford, IL",
      "Roseville, CA",
      "Round Rock, TX",
      "Sacramento, CA", "Sacramento, CA", "Sacramento, CA",
      "Saipan, MP", // biggest city in Northern Mariana Islands
      "Salem, OR",
      "Salinas, CA",
      "Salisbury, MD",
      "Salt Lake City, UT",
      "San Antonio, TX", "San Antonio, TX", "San Antonio, TX", "San Antonio, TX", "San Antonio, TX",
      "San Bernadino, CA", "San Bernadino, CA",
      "San Diego, CA", "San Diego, CA", "San Diego, CA", "San Diego, CA", "San Diego, CA",
      "San Francisco, CA", "San Francisco, CA", "San Francisco, CA", "San Francisco, CA",
      "San Jose, CA", "San Jose, CA", "San Jose, CA", "San Jose, CA",
      "San Juan, PR", "San Juan, PR", // biggest city in Puerto Rico
      "San Luis Obispo, CA",
      "San Mateo, CA",
      "Santa Ana, CA", "Santa Ana, CA", "Santa Ana, CA",
      "Santa Clara, CA",
      "Santa Clarita, CA",
      "Santa Cruz, CA",
      "Santa Maria, CA",
      "Santa Rosa, CA",
      "Sarasota, FL",
      "Savannah, GA",
      "Scottsdale, AZ", "Scottsdale, AZ",
      "Scranton, PA",
      "Seattle, WA", "Seattle, WA", "Seattle, WA", "Seattle, WA",
      "Shreveport, LA", "Shreveport, LA",
      "Sioux Falls, SD",
      "Simi Valley, CA",
      "South Bend, IN",
      "Spartanburg, SC",
      "Spokane, WA", "Spokane, WA",
      "Springfield, IL",
      "Springfield, MA",
      "Springfield, MO",
      "St. Louis, MO", "St. Louis, MO", "St. Louis, MO",
      "St. Paul, MN", "St. Paul, MN",
      "St. Petersburg, FL", "St. Petersburg, FL",
      "Stamford, CT",
      "Staten Island, NY",
      "Sterling Heights, MI",
      "Stockton, CA", "Stockton, CA",
      "Sunnyvale, CA",
      "Surprise, AZ",
      "Syracuse, NY",
      "Tacoma, WA", "Tacoma, WA",
      "Tafuna, AS", // biggest city in American Samoa
      "Tallahassee, FL",
      "Tampa, FL", "Tampa, FL", "Tampa, FL",
      "Temecula, CA",
      "Tempe, AZ",
      "The Bronx, NY", "The Bronx, NY",
      "Thornton, CO",
      "Thousand Oaks, CA",
      "Toledo, OH", "Toledo, OH",
      "Topeka, KS",
      "Torrance, CA",
      "Trenton, NJ",
      "Tuscaloosa, AL",
      "Tucson, AZ", "Tucson, AZ", "Tucson, AZ",
      "Tulsa, OK", "Tulsa, OK", "Tulsa, OK",
      "Tyler, TX",
      "Utica, NY",
      "Vallejo, CA",
      "Vancouver, WA",
      "Ventura, CA",
      "Victorville, CA",
      "Virginia Beach, VA", "Virginia Beach, VA", "Virginia Beach, VA",
      "Visalia, CA",
      "Waco, TX",
      "Warren, MI",
      "Washington, DC", "Washington, DC", "Washington, DC", "Washington, DC",
      "Waterbury, CT",
      "West Covina, CA",
      "West Jordan, UT",
      "West Palm Beach, FL",
      "West Valley City, UT",
      "Westminster, CO",
      "White Plains, NY",
      "Wichita, KS", "Wichita, KS", "Wichita, KS",
      "Wichita Falls, TX",
      "Wilmington, DE",
      "Wilmington, NC",
      "Winston-Salem, NC", "Winston-Salem, NC",
      "Worcester, MA",
      "Yakima, WA",
      "Yonkers, NY",
      "York, PA",
      "Youngstown, OH",
      "Yuma, AZ"
   };
   return pickrandom(city_names);
}

/* Allow the player to enter a name with an optional default */
void enter_name(int y,int x,char *name,int len,const char* defname)
{
   refresh();
   keypad(stdscr,FALSE);
   raw_output(FALSE);
   echo();
   curs_set(1);
   mvgetnstr(y,x,name,len-1); //-1 because 'len' is normally the full space available and we need one for a terminator.
   curs_set(0);
   noecho();
   raw_output(TRUE);
   keypad(stdscr,TRUE);
   if((defname!=NULL)&&(strncmp(name,"",len-1)==0)) strncpy(name,defname,len-1);
   name[len-1]='\0';
}

std::string getlawflag(int type)
{
   switch(type)
   {
   case LAWFLAG_TREASON:return "Treason";
   case LAWFLAG_TERRORISM:return "Terrorism";
   case LAWFLAG_MURDER:return "Murder";
   case LAWFLAG_KIDNAPPING:return "Kidnapping";
   case LAWFLAG_BANKROBBERY:return "Bank robbery";
   case LAWFLAG_ARSON:return "Arson";
   case LAWFLAG_BURNFLAG:return(law[LAW_FLAGBURNING]==-2?"Flag Murder":"Flag burning");
   case LAWFLAG_SPEECH:return "Harmful speech";
   case LAWFLAG_BROWNIES:return "Drug dealing";
   case LAWFLAG_ESCAPED:return "Escaping prison";
   case LAWFLAG_HELPESCAPE:return "Releasing prisoners";
   case LAWFLAG_JURY:return "Jury tampering";
   case LAWFLAG_RACKETEERING:return "Racketeering";
   case LAWFLAG_EXTORTION:return "Extortion";
   case LAWFLAG_ARMEDASSAULT:return "Armed assault";
   case LAWFLAG_ASSAULT:return "Assault";
   case LAWFLAG_CARTHEFT:return "Grand theft auto";
   case LAWFLAG_CCFRAUD:return "Credit card fraud";
   case LAWFLAG_THEFT:return "Theft";
   case LAWFLAG_PROSTITUTION:return "Prostitution";
   case LAWFLAG_HIREILLEGAL:return(law[LAW_IMMIGRATION]<1?"Hiring illegal aliens":"Hiring undocumented workers");
   //case LAWFLAG_GUNUSE:return "Firing illegal weapons";
   //case LAWFLAG_GUNCARRY:return "Carrying illegal weapons";
   case LAWFLAG_COMMERCE:return "Electronic sabotage";
   case LAWFLAG_INFORMATION:return "Hacking";
   case LAWFLAG_BURIAL:return "Unlawful burial";
   case LAWFLAG_BREAKING:return "Breaking and entering";
   case LAWFLAG_VANDALISM:return "Vandalism";
   case LAWFLAG_RESIST:return "Resisting arrest";
   case LAWFLAG_DISTURBANCE:return "Disturbing the peace";
   case LAWFLAG_PUBLICNUDITY:return "Public nudity";
   case LAWFLAG_LOITERING:return "Loitering";
   default:return "Finding bugs";
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

std::string getalign(signed char alignment,bool capitalize)
{
   switch(alignment)
   {
   case ALIGN_ARCHCONSERVATIVE: return "Arch-Conservative";
   case ALIGN_CONSERVATIVE: return "Conservative";
   case ALIGN_MODERATE: return (capitalize?"Moderate":"moderate");
   case ALIGN_LIBERAL: return "Liberal";
   case ALIGN_ELITELIBERAL: return "Elite Liberal";
   case ALIGN_STALINIST: return "Stalinist";
   default: return "Buggy";
   }
}
