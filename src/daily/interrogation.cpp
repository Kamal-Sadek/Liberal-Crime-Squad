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

#include <includeDefault.h>
//#include "configfile.h"
//#include "tinydir.h"
#include <includeEnum.h>
#include <includeCommon.h>

/*
consolesupport.cpp
*/
#include "common\\consolesupport.h"

//#include <includeNews.h>
#include <includeFunctions.h>
//#include <includeTitle.h>

#include <includeTalk.h>
extern vector<Location *> location;
//#include <includeExternDefault.h>
extern MusicClass music;
//#include <includeExternPolitics.h>
//#include <includeExternStat.h>

extern int stat_recruits;
extern int stat_kills;




extern vector<string> execution;
extern vector<string> feels_sick;
extern vector<string> low_heart_torture_props;
extern vector<string> screaming;
extern vector<string> beat_with_props;
extern vector<string> words_meaning_screaming;
extern vector<string> words_to_scream;
extern vector<string> prays;
extern vector<string> prays_on_drugs;
extern vector<string> use_props;
extern vector<string> resist_drugs;
extern vector<string> partial_conversion;
extern vector<string> broods_over_death;
extern vector<string> self_wounding;

// Macro dumps interrogation data to screen for debug
#ifdef INTERROGATION_DEBUG
   #define DEBUG_VARIABLE_DUMP(text,value) { mvaddstr(20,0,text); addstr(value); addchar(' '); }
#else
   #define DEBUG_VARIABLE_DUMP(text,value)
#endif

enum InterrogationTechnqiues
{
   TECHNIQUE_TALK,
   TECHNIQUE_RESTRAIN,
   TECHNIQUE_BEAT,
   TECHNIQUE_PROPS,
   TECHNIQUE_DRUGS,
   TECHNIQUE_KILL
};

// Clear sidebar
void clear_interrogation_sidebar()
{
   for(int i=4;i<23;i++)
      mvaddstr(i,40,"                                  "); // 34 spaces
}

// Shows the interrogation data at the right side of the screen
void show_interrogation_sidebar( Creature * cr, Creature * a )
{
   clear_interrogation_sidebar();
   int y=4;
   map<long,struct float_zero>& rapport = cr->activity.intr()->rapport;
   move(y,40);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   addstr("Prisoner: ");
   set_color(COLOR_RED,COLOR_BLACK,1);
   addstr(cr->name);
   move(y+=2,40);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   addstr("Health: ");
   printhealthstat(*cr,y,48,0);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(++y,40);
   addstr("Heart: ");
   addstr(cr->get_attribute(ATTRIBUTE_HEART,true));
   move(++y,40);
   addstr("Wisdom: ");
   addstr(cr->get_attribute(ATTRIBUTE_WISDOM,true));
   move(++y,40);
   addstr("Health: ");
   addstr(cr->get_attribute(ATTRIBUTE_HEALTH,true));

   move(y=13,40);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   addstr("Lead Interrogator: ");
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   addstr(a->name);
   move(y+=2,40);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   addstr("Health: ");
   printhealthstat(*a,y,48,0);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(++y,40);
   addstr("Psychology Skill: ");
   addstr(a->get_skill(SKILL_PSYCHOLOGY));
   move(++y,40);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   addstr("Heart: ");
   addstr(a->get_attribute(ATTRIBUTE_HEART,true));
   move(++y,40);
   addstr("Wisdom: ");
   addstr(a->get_attribute(ATTRIBUTE_WISDOM,true));
   move(++y,40);
   addstr("Outfit: ");
   addstr(a->get_armor().get_name());
   move(y+=2,40);

   // What would 4, 2, 0, -2, -3, and/or -5 mean? (Some of these may not exist) -- LK
   // These are greater than and less than comparisons, so they are testing ranges -Fox
   if(rapport[a->id]>3)
   {
      addstr("The Conservative clings helplessly ");
      move(++y,40);
      addstr("to ");
      addstr(a->name);
      addstr(" as its only friend.");
   }
   else if(rapport[a->id]>1)
   {
      addstr("The Conservative likes ");
      addstr(a->name);
      addstr(".");
   }
   else if(rapport[a->id]>-1)
   {
      addstr("The Conservative is uncooperative ");
      move(++y,40);
      addstr("toward ");
      addstr(a->name);
      addstr(".");
   }
   else if(rapport[a->id]>-4)
   {
      addstr("The Conservative hates ");
      addstr(a->name);
      addstr(".");
   }
   else
   {
      addstr("The Conservative would like to ");
      move(++y,40);
      addstr("murder ");
      addstr(a->name);
      addstr(".");
   }
}

/* hostage tending */
void tendhostage(Creature *cr,char &clearformess)
{
	static const int number_of_fall_in_love = 3;
	static const char *fall_in_love[number_of_fall_in_love][2] =
	{
		{" hallucinates and sees ", " as an angel."},
		{" realizes with joy that ", " is Ronald Reagan!"},
		{" begs ", " to let the colors stay forever."}
	};

	static const int number_of_bad_trip = 2;
	static const char *bad_trip[number_of_bad_trip][2] = 
	{
		{" screams in horror as "," turns into an alien."},
		{" watches ", " shift from one demonic form to another."}
	};

	static const int good_trip_number = 2;
	static const char *good_trip[good_trip_number] =
	{
		" can't stop looking at the moving colors.",
		" barks and woofs like a dog."
	};

	static const int good_trip_companion_number = 2;
	static const char *good_trip_companion[good_trip_companion_number][2] =
	{
		{" comments on the swirling light "," is radiating."},
		{" laughs hysterically at ","'s altered appearance."}
	};

	static const int smarter_than_you_one_line_number = 3;
	static const char *smarter_than_you_one_line[smarter_than_you_one_line_number] =
	{
		" knows how this works, and won't budge.",
		" asks if Liberal mothers would approve of this.",
		" seems resistant to this form of interrogation."
	};

	static const int smarter_than_you_number = 1;
	static const char *smarter_than_you[smarter_than_you_number][2] =
	{
		{" plays mind games with ", "."}
	};
	

	static const int develops_hatred_one_line_number = 6;
	static const char *develops_hatred_one_line[develops_hatred_one_line_number] =
	{
		" babbles mindlessly.",
		" just whimpers.",
		" cries helplessly.",
		" is losing faith in the world.",
		" only grows more distant.",
		" just hates the LCS even more."
	};

	static const int develops_hatred_number = 1;
	static const char *develops_hatred[develops_hatred_number][2] =
	{
		{" is too terrified to even speak to ", "."}
	};

	static const int number_of_compassion_one_line = 6;
	static const char *interrogater_shows_compassion_one_line[number_of_compassion_one_line] = 
	{
		" consoles the Conservative automaton.",
		" shares some chocolates.",
		" provides a shoulder to cry on.",
		"'s heart opens to the poor Conservative.",
		" helps the poor thing to come to terms with captivity.",
		"'s patience and kindness leaves the Conservative confused."
	};

	static const int number_of_compassion = 1;
	static const char *interrogater_shows_compassion[number_of_compassion][2] =
	{
		{" understands ", "'s pain."}
	};

	static const int number_of_clinging = 5;
	static const char *cling_to_interrogater[number_of_clinging][2] =
	{
		{" emotionally clings to ", "'s sympathy."},
		{" begs ", " for help."},
		{" thanks ", " for being merciful."},
		{" cries in ", "'s arms."},
		{" really likes ", "."}
	};

	static const int number_of_clinging_one_line = 2;
	static const char *clinging_one_line[number_of_clinging_one_line] =
	{
		" promises to be good.",
		" reveals childhood pains."
	};

	static const int number_of_clinging_religion = 2;
	static const char *cling_to_religion[number_of_clinging_religion][2] =
	{
		{" is unable to shake ","'s religious conviction."},
		{"'s efforts to question ","'s faith seem futile."}
	};

	static const int number_of_clinging_religion_one_line = 2;
	static const char *cling_to_religion_one_line[number_of_clinging_religion_one_line] =
	{
		" will never be broken so long as God grants it strength.",
		" calmly explains the Conservative tenets of its faith."
	};

	static const int discuss_number = 2;
	static const char *discuss[discuss_number][2] = 
	{
		{" talks about ", " with "},
		{" argues about ", " with "}
	};

	static const int vanilla_recruit_number = 2;
	static const char *vanilla_recruit[vanilla_recruit_number] = 
	{
		" tries to expose the true Liberal side of ",
		" attempts to recruit "
	};

	static const int number_of_clinging_business = 2;
	static const char *cling_to_business[number_of_clinging_business][2] =
	{
		{" will never be moved by ", "'s pathetic economic ideals."},
		{" explains to ", " why communism failed."}
	};

	static const int number_of_clinging_business_one_line = 2;
	static const char *cling_to_business_one_line[number_of_clinging_business_one_line] =
	{
		" wishes a big company would just buy the LCS and shut it down.",
		" mumbles incoherently about Reaganomics."
	};	

	static const int number_of_clinging_science = 3;
	static const char *cling_to_science[number_of_clinging_science][2] =
	{
		{" wonders what mental disease has possessed ", "."},
		{" makes Albert Einstein faces at ", "."},
		{" pities ", "'s blind ignorance of science."}
	};

	static const int number_of_clinging_science_one_line = 1;
	static const char *cling_to_science_one_line[number_of_clinging_science_one_line] =
	{
		" explains why nuclear energy is safe."
	};


   music.play(MUSIC_INTERROGATION);
   vector<Creature *> temppool;
   int p;
   Creature *a=NULL;

   interrogation* &intr=cr->activity.intr();
   bool (&techniques)[6]=intr->techniques;
   int& druguse = intr->druguse;
   map<long,struct float_zero>& rapport = intr->rapport;

   //Find all tenders who are set to this hostage
   for(p=0;p<len(pool);p++)
   {
      if(!pool[p]->alive) continue;
      if(pool[p]->activity.type==ACTIVITY_HOSTAGETENDING&&pool[p]->activity.arg==cr->id)
      {
         //If they're in the same location as the hostage,
         //include them in the interrogation
         if(pool[p]->location==cr->location&&pool[p]->location!=-1)
            temppool.push_back(pool[p]);
         //If they're someplace else, take them off the job
         else pool[p]->activity.type=ACTIVITY_NONE;
      }
   }

   if(cr->location==-1)
   {
      delete cr;
      return;
   }

   //possible hostage escape attempt if unattended or unrestrained
   if(!len(temppool)||!techniques[TECHNIQUE_RESTRAIN])
   {
      //CHECK FOR HOSTAGE ESCAPE
      if(LCSrandom(200)+25*len(temppool)<
         cr->get_attribute(ATTRIBUTE_INTELLIGENCE,true)+
         cr->get_attribute(ATTRIBUTE_AGILITY,true)+
         cr->get_attribute(ATTRIBUTE_STRENGTH,true)&&
         cr->joindays>=5)
      {
         for(int p=0;p<len(pool);p++)
         {
            if(pool[p]==cr)
            {
               if(clearformess) erase();
               else makedelimiter();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(cr->name, gamelog);
               addstr(" has escaped!", gamelog);
               gamelog.nextMessage();
               location[cr->location]->siege.timeuntillocated=3;

               getkey();

               //clear activities for tenders
               for(int i=0;i<len(pool);i++)
               {
                  if(!pool[i]->alive) continue;
                  if(pool[i]->activity.type==ACTIVITY_HOSTAGETENDING&&pool[i]->activity.arg==cr->id)
                     pool[i]->activity.type=ACTIVITY_NONE;
               }

               //delete interrogation data
               delete intr;
               delete_and_remove(pool,p);
               break;
            }
         }
         return;
      }

      if(!len(temppool)) return;
   }

   clearformess=1;

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   addstr("The Education of ");
   addstr(cr->name);
   addstr(": Day ");
   addstr(cr->joindays);

   getkey();

   set_color(COLOR_WHITE,COLOR_BLACK,0);

   char turned=0;

   int y=3;

   {
      //each day, the attack roll is initialized to the number of days of the stay with
      //the LCS -- they will eventually break, but also eventually become too traumatized
      //to continue
      int p,business=0,religion=0,science=0,attack=0;

      int* _attack = new int[len(temppool)];

      for(p=0;p<len(temppool);p++)
      {
         _attack[p] = 0;
         if(temppool[p])
            if(temppool[p]->alive)
            {
               if(temppool[p]->get_skill(SKILL_BUSINESS)>business)
                  business=temppool[p]->get_skill(SKILL_BUSINESS);
               if(temppool[p]->get_skill(SKILL_RELIGION)>religion)
                  religion=temppool[p]->get_skill(SKILL_RELIGION);
               if(temppool[p]->get_skill(SKILL_SCIENCE)>science)
                  science=temppool[p]->get_skill(SKILL_SCIENCE);

               _attack[p] = (temppool[p]->get_attribute(ATTRIBUTE_HEART,true)-
                             temppool[p]->get_attribute(ATTRIBUTE_WISDOM,true)+
                             temppool[p]->get_skill(SKILL_PSYCHOLOGY)*2);

               _attack[p] += temppool[p]->get_armor().get_interrogation_basepower();

               if(_attack[p]<0) _attack[p]=0;
               if(_attack[p]>attack) attack=_attack[p];
            }
      }

      vector<int> goodp;

      for(p=0;p<len(temppool);p++)
         if(temppool[p])
            if(temppool[p]->alive&&_attack[p]==attack)
               goodp.push_back(p);
      a=temppool[pickrandom(goodp)];

      attack+=len(temppool);
      attack+=cr->joindays;

      attack+=business-cr->get_skill(SKILL_BUSINESS);
      attack+=religion-cr->get_skill(SKILL_RELIGION);
      attack+=science-cr->get_skill(SKILL_SCIENCE);
      attack+=a->skill_roll(SKILL_PSYCHOLOGY)-cr->skill_roll(SKILL_PSYCHOLOGY);

      attack+=cr->attribute_roll(ATTRIBUTE_HEART);
      attack-=cr->attribute_roll(ATTRIBUTE_WISDOM)*2;

      while(true)
      {
         y=2;
         if(techniques[TECHNIQUE_KILL])
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            move(y,0);y+=2;addstr("The Execution of an Automaton         ");
         }
         else
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(y,0);y+=2;addstr("Selecting a Liberal Interrogation Plan");
         }
         if(techniques[TECHNIQUE_KILL])set_color(COLOR_BLACK,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,techniques[TECHNIQUE_TALK]);
         move(y++,0);addstr("A - ");
         if(!techniques[TECHNIQUE_TALK]) addstr("No Verbal Contact     ");
         else addstr("Attempt to Convert");
         if(!techniques[TECHNIQUE_KILL])set_color(COLOR_WHITE,COLOR_BLACK,techniques[TECHNIQUE_RESTRAIN]);
         move(y++,0);addstr("B - ");
         if(!techniques[TECHNIQUE_RESTRAIN]) addstr("No ");
         addstr("Physical Restraints   ");
         if(!techniques[TECHNIQUE_KILL])set_color(COLOR_WHITE,COLOR_BLACK,techniques[TECHNIQUE_BEAT]);
         move(y++,0);addstr("C - ");
         if(!techniques[TECHNIQUE_BEAT]) addstr("Not ");
         addstr("Violently Beaten    ");
         if(!techniques[TECHNIQUE_KILL])set_color(COLOR_WHITE,COLOR_BLACK,techniques[TECHNIQUE_PROPS]);
         move(y,0);addstr("D - ");
         if(!techniques[TECHNIQUE_PROPS])addstr("No ");
         addstr("Expensive Props     ");
         move(y++,27);
         addstr("($250)");
         if(!techniques[TECHNIQUE_KILL])set_color(COLOR_WHITE,COLOR_BLACK,techniques[TECHNIQUE_DRUGS]);
         move(y,0);addstr("E - ");
         if(!techniques[TECHNIQUE_DRUGS])addstr("No ");
         addstr("Hallucinogenic Drugs    ");
         move(y,28);y+=2;
         addstr("($50)");
         if(techniques[TECHNIQUE_KILL])set_color(COLOR_RED,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);y+=2;addstr("K - Kill the Hostage");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y++,0);addstr("Press Enter to Confirm the Plan");

         show_interrogation_sidebar(cr,a);

         int c=getkey();
         if(c>='a'&&c<='e') techniques[c-'a']=!techniques[c-'a'];
         if(c=='k') techniques[TECHNIQUE_KILL]=!techniques[TECHNIQUE_KILL];
         if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
      }

      if(techniques[TECHNIQUE_PROPS]&&ledger.get_funds()>=250)
         ledger.subtract_funds(250,EXPENSE_HOSTAGE);
      else techniques[TECHNIQUE_PROPS]=0;
      if(techniques[TECHNIQUE_DRUGS]&&ledger.get_funds()>=50)
         ledger.subtract_funds(50,EXPENSE_HOSTAGE);
      else techniques[TECHNIQUE_DRUGS]=0;

      if(techniques[TECHNIQUE_KILL]) // Kill the Hostage
      {
         erase();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(0,0);
         addstr("The Final Education of ", gamelog);
         addstr(cr->name, gamelog);
         addstr(": Day ", gamelog);
         addstr(cr->joindays, gamelog);
         gamelog.newline();

         a=NULL;

         for(int i=0;i<len(temppool);i++)
            if(LCSrandom(50)<temppool[i]->juice||
               LCSrandom(9)+1>=temppool[i]->get_attribute(ATTRIBUTE_HEART,0))
            {  a=temppool[i]; break; }

         if(a)
         {
            //delete interrogation information
            delete intr;
            set_color(COLOR_MAGENTA,COLOR_BLACK,0);
            cr->die();
            stat_kills++;
            move(y++,0);
            addstr(a->name, gamelog);
            addstr(" executes ", gamelog);
            addstr(cr->name, gamelog);
            addstr(" by ", gamelog);
            addstr(pickrandom(execution), gamelog);
            //show_interrogation_sidebar(cr,a);

            getkey();

            if(LCSrandom(a->get_attribute(ATTRIBUTE_HEART,false))>LCSrandom(3))
            {
               gamelog.newline();
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(y++,0);
               addstr(a->name, gamelog);
               addstr(" feels sick to the stomach afterward and ", gamelog);
               a->adjust_attribute(ATTRIBUTE_HEART,-1);
               move(y++,0);
               addstr(pickrandom(feels_sick), gamelog);
            }
            else if(!LCSrandom(3))
            {
               gamelog.newline();
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               move(y++,0);
               addstr(a->name, gamelog);
               addstr(" grows colder.", gamelog);
               a->adjust_attribute(ATTRIBUTE_WISDOM,+1);
            }
            gamelog.nextMessage();
         }
         else
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,0);
            move(y++,0);
            addstr("There is no one able to get up the nerve to ", gamelog);
            move(y++,0);
            addstr("execute ", gamelog);
            addstr(cr->name, gamelog);
            addstr(" in cold blood.", gamelog);
            gamelog.nextMessage();

            //Interrogation will continue as planned, with
            //these restrictions:
            techniques[TECHNIQUE_TALK]=0; //don't talk to them today
            techniques[TECHNIQUE_BEAT]=0; //don't beat them today
            techniques[TECHNIQUE_DRUGS]=0; //don't administer drugs

            //Food and restraint settings will be applied as normal
         }
         //show_interrogation_sidebar(cr,a);

         getkey();

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(24,0);
         addstr("Press any key to reflect on this.");

         getkey();

         if(!cr->alive)
         {
            for(int p=0;p<len(pool);p++)
            {
               if(!pool[p]->alive) continue;
               if(pool[p]->activity.type==ACTIVITY_HOSTAGETENDING&&pool[p]->activity.arg==cr->id)
                  pool[p]->activity.type=ACTIVITY_NONE;
            }
            delete[] _attack;
            return;
         }
      }

      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("The Education of ", gamelog);
      addstr(cr->name, gamelog);
      addstr(": Day ", gamelog);
      addstr(cr->joindays, gamelog);
      gamelog.newline();

      move((y=2)++,0);
      addstr("The Automaton", gamelog);
      if(techniques[TECHNIQUE_RESTRAIN]) // Restraint
      {
         addstr(" is tied hands and feet to a metal chair", gamelog);
         move(y++,0);
         addstr("in the middle of a back room.", gamelog);
         gamelog.newline();

         attack+=5;
      }
      else
      {
         addstr(" is locked in a back room ", gamelog);
         move(y++,0);
         addstr("converted into a makeshift cell.", gamelog);
         gamelog.newline();
      }
      //show_interrogation_sidebar(cr,a);

      getkey();

      if(techniques[TECHNIQUE_DRUGS]) // Hallucinogenic drugs
      {
         move(++y,0);

         addstr("It is subjected to dangerous hallucinogens.", gamelog);
         gamelog.newline();

         int drugbonus=10+a->get_armor().get_interrogation_drugbonus(); // we won't apply this JUST yet

         //Possible permanent health damage
         if(LCSrandom(50)<++druguse)
         {
            cr->adjust_attribute(ATTRIBUTE_HEALTH,-1);
            move(++y,0);

            getkey();

            addstr(cr->name, gamelog);
            addstr(" foams at the mouth and its eyes roll back in its skull.", gamelog);
            gamelog.newline();
            move(++y,0);

            getkey();

            Creature* doctor=a; // the lead interrogator is doctor by default
            int maxskill=doctor->get_skill(SKILL_FIRSTAID);
            for(int i=0;i<len(temppool);i++) // search for the best doctor
               if(temppool[i]->get_skill(SKILL_FIRSTAID)>maxskill)
                  maxskill=(doctor=temppool[i])->get_skill(SKILL_FIRSTAID); // we found a doctor

            if(cr->get_attribute(ATTRIBUTE_HEALTH,false)<=0 || !maxskill) // he's dead, Jim
            {
               if(maxskill)
               { // we have a real doctor but the patient is still dead anyway
                  addstr(doctor->name, gamelog);
                  addstr(" uses a defibrillator repeatedly but ", gamelog);
                  addstr(cr->name, gamelog);
                  addstr(" flatlines.", gamelog);
               }
               else
               {
                  addstr(doctor->name, gamelog);
                  if(law[LAW_FREESPEECH]==-2)
                     addstr(" has a panic attack and [makes a stinky].", gamelog);
                  else
                  {
                     addstr(" has a panic attack and shits ", gamelog);
                     addstr(doctor->hisher(), gamelog);
                     addstr(" pants.", gamelog);
                  }
               }
               gamelog.newline();
               move(++y,0);

               getkey();

               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               if(maxskill)
               {
                  addstr("It is a lethal overdose in ", gamelog);
                  addstr(cr->name, gamelog);
                  addstr("'s weakened state.", gamelog);
               }
               else
               {
                  addstr(cr->name, gamelog);
                  addstr(" dies due to ", gamelog);
                  addstr(doctor->name, gamelog);
                  addstr("'s incompetence at first aid.", gamelog);
               }
               gamelog.newline();
               cr->die();
            }
            else
            {
               addstr(doctor->name, gamelog);
               if(doctor->skill_check(SKILL_FIRSTAID,DIFFICULTY_CHALLENGING)) // is the doctor AWESOME?
               {
                  doctor->train(SKILL_FIRSTAID,5*max(10-doctor->get_skill(SKILL_FIRSTAID),0),10); // can train up to 10
                  addstr(" deftly rescues it from cardiac arrest with a defibrillator.", gamelog); // not long enough for near-death experience
                  gamelog.newline();
                  move(++y,0);

                  getkey();

                  addstr(doctor->name, gamelog);
                  addstr(" skillfully saves ", gamelog);
                  addstr(cr->name, gamelog);
                  addstr(" from any health damage.", gamelog);
                  cr->adjust_attribute(ATTRIBUTE_HEALTH,+1); // no permanent health damage from a skilled doctor
                  techniques[TECHNIQUE_DRUGS]=(druguse=drugbonus=0); // drugs eliminated from the system (zeroing out 3 variables with 1 line of code)
               }
               else
               {
                  doctor->train(SKILL_FIRSTAID,5*max(5-doctor->get_skill(SKILL_FIRSTAID),0),5); // can train up to 5
                  addstr(" clumsily rescues it from cardiac arrest with a defibrillator.", gamelog);
                  gamelog.newline();
                  move(++y,0);

                  getkey();

                  addstr(cr->name, gamelog);
                  if(cr->get_skill(SKILL_RELIGION)) // the patient was out long enough to have a near-death experience
                     addstr(" had a near-death experience and met God in heaven.", gamelog);
                  else addstr(" had a near-death experience and met John Lennon.", gamelog);
                  drugbonus*=2; // the near-death experience doubles the drug bonus, since the hostage is spaced out afterwards
               }
               rapport[doctor->id]+=0.5f; // rapport bonus for having life saved by doctor
               gamelog.newline();
            }
         }
         attack+=drugbonus; // now we finally apply the drug bonus
         move(++y,0);
         //show_interrogation_sidebar(cr,a);

         getkey();
      }

      if(techniques[TECHNIQUE_BEAT]&&!turned&&cr->alive) // Beating
      {
         move(++y,0);

         int forceroll=0;
         bool tortured=0;

         for(int i=0;i<len(temppool);i++)
         {
            //add interrogator's strength to beating strength
            forceroll+=temppool[i]->attribute_roll(ATTRIBUTE_STRENGTH);
            //reduce rapport with each interrogator
            rapport[temppool[i]->id]-=0.4f;
         }

         //Torture captive if lead interrogator has low heart
         //and you funded using extra supplies
         //
         //Yeah, you kinda don't want this to happen
         if(!(a->attribute_check(ATTRIBUTE_HEART,DIFFICULTY_EASY))&&techniques[TECHNIQUE_PROPS])
         {
            tortured = true;
            //Torture more devastating than normal beating
            forceroll*=5;
            //Extremely bad for rapport with lead interrogator
            rapport[a->id]-=3;

            addstr(a->name, gamelog);
			addstr(" ", gamelog);
            addstr(pickrandom(low_heart_torture_props), gamelog);
            addstr(", ", gamelog);
            move(++y,0);
            addstr("screaming \"", gamelog);
			addstr(pickrandom(screaming), gamelog);
			addstr("! ", gamelog);
			addstr(pickrandom(screaming), gamelog);
            addstr("!\" in its face.", gamelog);
            gamelog.newline();
            if(cr->get_attribute(ATTRIBUTE_HEART,true)>1) cr->adjust_attribute(ATTRIBUTE_HEART,-1);
            if(cr->get_attribute(ATTRIBUTE_WISDOM,true)>1) cr->adjust_attribute(ATTRIBUTE_WISDOM,-1);
         }
         else
         {
            if(len(temppool)==1)
            {
               addstr(temppool[0]->name, gamelog);
               addstr(" beats", gamelog);
            }
            else if(len(temppool)==2)
            {
               addstr(temppool[0]->name, gamelog);
               addstr(" and ", gamelog);
               addstr(temppool[1]->name, gamelog);
               addstr(" beat", gamelog);
            }
            else
            {
               addstr(cr->name);
               addstr("'s guards beat", gamelog);
            }
            addstr(" the Automaton", gamelog);
            if(techniques[TECHNIQUE_PROPS])
            {
				addstr(" ", gamelog);
				addstr(pickrandom(beat_with_props), gamelog);
            }
            addstr(", ", gamelog);
            move(++y,0);
            addstr(pickrandom(words_meaning_screaming), gamelog);
            for(int i=0;i<3;i++)
            {
               addstr(pickrandom(words_to_scream), gamelog);
               if(i<2) addstr("! ", gamelog);
            }
            addstr("!\" in its face.", gamelog);
            gamelog.newline();
         }
         y++;

         cr->blood-=(5 + LCSrandom(5)) * (1+techniques[TECHNIQUE_PROPS]);

         //show_interrogation_sidebar(cr,a);

         getkey();

         if(!(cr->attribute_check(ATTRIBUTE_HEALTH,forceroll)))
         {
            if(cr->skill_check(SKILL_RELIGION,forceroll))
            {
               move(y++,0);
               addstr(cr->name, gamelog);
               if(!techniques[TECHNIQUE_DRUGS])
               {
				   addstr(" ", gamelog);
                  addstr(pickrandom(prays), gamelog);
                  gamelog.newline();
               }
               else
               {
				   addstr(" ", gamelog);
                  addstr(pickrandom(prays_on_drugs), gamelog);
                  gamelog.newline();
               }
            }
            else if(forceroll >
                    cr->get_attribute(ATTRIBUTE_WISDOM,true)*3+
                    cr->get_attribute(ATTRIBUTE_HEART,true)*3+
                    cr->get_attribute(ATTRIBUTE_HEALTH,true)*3)
            {
               move(y++,0);
               addstr(cr->name, gamelog);
               switch(LCSrandom(4))
               {
               case 0:addstr(" screams helplessly for ", gamelog);
                  if(techniques[TECHNIQUE_DRUGS])addstr("John Lennon's mercy.", gamelog);
                  else if(cr->get_skill(SKILL_RELIGION))addstr("God's mercy.", gamelog);
                  else addstr("mommy.", gamelog);
                  break;
               case 1:
                  if(techniques[TECHNIQUE_RESTRAIN]) addstr(" goes limp in the restraints.", gamelog);
                  else addstr(" curls up in the corner and doesn't move.", gamelog);break;
               case 2:
                  if(techniques[TECHNIQUE_DRUGS] && !LCSrandom(5)) addstr(" barks helplessly.", gamelog);
                  else addstr(" cries helplessly.", gamelog);break;
               case 3:
                  if(techniques[TECHNIQUE_DRUGS] && !LCSrandom(3)) addstr(" wonders about apples.", gamelog);
                  else addstr(" wonders about death.", gamelog);
                  break;
               }
               gamelog.newline();
               if(cr->get_attribute(ATTRIBUTE_HEART,false)>1) cr->adjust_attribute(ATTRIBUTE_HEART,-1);

               if(LCSrandom(2)&&cr->juice>0) { if((cr->juice-=forceroll)<0) cr->juice=0; }
               else if(cr->get_attribute(ATTRIBUTE_WISDOM,false)>1)
               {
                  cr->set_attribute(ATTRIBUTE_WISDOM,cr->get_attribute(ATTRIBUTE_WISDOM,false)-(forceroll/10));
                  if(cr->get_attribute(ATTRIBUTE_WISDOM,false)<1) cr->set_attribute(ATTRIBUTE_WISDOM,1);
               }

               if(location[cr->worklocation]->mapped==0 && !LCSrandom(5))
               {
                  //show_interrogation_sidebar(cr,a);

                  getkey();

                  move(y++,0);
                  addstr(a->name, gamelog);
                  addstr(" beats information out of the pathetic thing.", gamelog);
                  gamelog.newline();
                  move(y++,0);

                  getkey();

                  if(location[cr->worklocation]->type<=SITE_RESIDENTIAL_SHELTER)
                     addstr("Unfortunately, none of it is useful to the LCS.", gamelog);
                  else
                  {
                     addstr("A detailed map has been created of ", gamelog);
                     addstr(location[cr->worklocation]->name, gamelog);
                     addstr(".", gamelog);
                  }
                  gamelog.newline();
                  location[cr->worklocation]->mapped=1;
                  location[cr->worklocation]->hidden=0;
               }
            }
            else
            {
               move(y++,0);
               addstr(cr->name, gamelog);
               addstr(" seems to be getting the message.", gamelog);
               gamelog.newline();

               if(cr->juice>0) if((cr->juice-=forceroll)<0) cr->juice=0;

               if(cr->get_attribute(ATTRIBUTE_WISDOM,false)>1)
               {
                  cr->set_attribute(ATTRIBUTE_WISDOM,cr->get_attribute(ATTRIBUTE_WISDOM,false)-(forceroll/10+1));
                  if(cr->get_attribute(ATTRIBUTE_WISDOM,false)<1) cr->set_attribute(ATTRIBUTE_WISDOM,1);
               }
            }

            if(!(cr->attribute_check(ATTRIBUTE_HEALTH,forceroll/3)))
            {
               //show_interrogation_sidebar(cr,a);

               getkey();

               move(y++,0);
               if(cr->get_attribute(ATTRIBUTE_HEALTH,false)>1)
               {
                  cr->adjust_attribute(ATTRIBUTE_HEALTH,-1);
                  addstr(cr->name, gamelog);
                  addstr(" is badly hurt.", gamelog);
                  gamelog.newline();
               }
               else
               {
                  cr->set_attribute(ATTRIBUTE_HEALTH,0);
                  addstr(cr->name, gamelog);
                  addstr("'s weakened body crumbles under the brutal assault.", gamelog);
                  gamelog.newline();
                  cr->die();
               }
            }
         }
         else
         {
            move(y++,0);
            addstr(cr->name, gamelog);
            addstr(" takes it well.", gamelog);
            gamelog.newline();
         }
         //show_interrogation_sidebar(cr,a);

         getkey();

         if(tortured && cr->alive)
         {
            if(LCSrandom(a->get_attribute(ATTRIBUTE_HEART,false))>LCSrandom(3))
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(y++,0);
               addstr(a->name, gamelog);
               addstr(" feels sick to the stomach afterward and ", gamelog);
               a->adjust_attribute(ATTRIBUTE_HEART,-1);
               move(y++,0);
               addstr(pickrandom(feels_sick), gamelog);
               gamelog.newline();
            }
            else if(!LCSrandom(3))
            {
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               move(y++,0);
               addstr(a->name, gamelog);
               addstr(" grows colder.", gamelog);
               gamelog.newline();
               a->adjust_attribute(ATTRIBUTE_WISDOM,+1);
            }
            set_color(COLOR_WHITE,COLOR_BLACK,1);
         }
      }

      // Verbal Interrogation
      if(techniques[TECHNIQUE_TALK]&&cr->alive)
      {
         float rapport_temp = rapport[a->id];

         if(!techniques[TECHNIQUE_RESTRAIN])attack += 5;
         attack += int(rapport[a->id] * 3);

         move((++y)++,0);
         addstr(a->name, gamelog);

         if(techniques[TECHNIQUE_PROPS])//props
         {
            attack += 10;
			addstr(" ", gamelog);
            addstr(pickrandom(use_props), gamelog);
            gamelog.newline();
         }
         else
         {
			int which_discuss = LCSrandom(discuss_number + vanilla_recruit_number);
			if(which_discuss < discuss_number){
				addstr(discuss[which_discuss][0], gamelog);
				addstr(getview(LCSrandom(VIEWNUM-3),true), gamelog);
				addstr(discuss[which_discuss][1], gamelog);
			}else{
				addstr(pickrandom(vanilla_recruit), gamelog);
			}
         }
         addstr(cr->name, gamelog);
         addstr(".", gamelog);
         gamelog.newline();

         //Hallucinogenic drugs:
         //Re-interprets lead interrogator
         if(techniques[TECHNIQUE_DRUGS])
         {
            //show_interrogation_sidebar(cr,a);

            getkey();

            move(y++,0);
            if(cr->skill_check(SKILL_PSYCHOLOGY,DIFFICULTY_CHALLENGING))
            {
				addstr(cr->name);
				addstr(" ", gamelog);
				addstr(pickrandom(resist_drugs), gamelog);
               gamelog.newline();
            }
            else if((rapport[a->id]>1 && !LCSrandom(3)) || !LCSrandom(10))
            {
               rapport_temp=10;
               switch(LCSrandom(number_of_fall_in_love + 1))
               {
               case 0:
                  addstr(cr->name, gamelog);
                  addstr(" stammers and ", gamelog);
                  techniques[TECHNIQUE_RESTRAIN] ? addstr("talks about hugging ", gamelog) : addstr("hugs ", gamelog);
                  addstr(a->name, gamelog);
                  addstr(".", gamelog);
                  break;
			   default:
				   int which_love = LCSrandom (number_of_fall_in_love);
				   addstr(cr->name, gamelog);
				   addstr(fall_in_love[which_love][0], gamelog);
				   addstr(a->name, gamelog);
				   addstr(fall_in_love[which_love][1], gamelog);
				   break;
               }
               gamelog.newline();
            }
            else if((rapport[a->id]<-1 && LCSrandom(3)) || !LCSrandom(5))
            {
               attack=0;
               switch(LCSrandom(number_of_bad_trip + 2))
               {
               case 0:
                  if(rapport[a->id]<-3)
                  {
                     addstr(cr->name, gamelog);
                     addstr(" begs Hitler to stay and kill ", gamelog);
                     addstr(a->name, gamelog);
                     addstr(".", gamelog);
                  }
                  else
                  {
                     addstr(cr->name, gamelog);
                     addstr(" screams for ", gamelog);
                     addstr(a->name, gamelog);
                     addstr(" to stop looking like Hitler.", gamelog);
                  }
                  break;
			   case 1:
                  addstr(cr->name, gamelog);
                  if(!techniques[TECHNIQUE_RESTRAIN]) addstr(" curls up and", gamelog);
                  addstr(" begs for the nightmare to end.", gamelog);
                  break;
			   default:
				   int which_trip = LCSrandom (number_of_bad_trip);
				   addstr(cr->name, gamelog);
				   addstr(pickrandom(bad_trip[which_trip][0]), gamelog);
				   addstr(a->name, gamelog);
				   addstr(pickrandom(bad_trip[which_trip][1]), gamelog);
				   break;
               }
               gamelog.newline();
            }
            else
            {
				int which_good_trip = LCSrandom(good_trip_number + good_trip_companion_number);
				addstr(cr->name, gamelog);
				if(which_good_trip < good_trip_companion_number){
					addstr(good_trip_companion[which_good_trip][0], gamelog);
					addstr(a->name, gamelog);
					addstr(good_trip_companion[which_good_trip][1], gamelog);
				} else {
					addstr(pickrandom(good_trip), gamelog);
				}
               gamelog.newline();
            }
         }

         //show_interrogation_sidebar(cr,a);

         getkey();

         if(cr->get_skill(SKILL_PSYCHOLOGY)>a->get_skill(SKILL_PSYCHOLOGY))
         {
            move(y++,0);
			addstr(cr->name, gamelog);
			int which_smarter = LCSrandom(smarter_than_you_number + smarter_than_you_one_line_number);
			if(which_smarter < smarter_than_you_number){
				addstr(smarter_than_you[which_smarter][0], gamelog);
				addstr(a->name, gamelog);
				addstr(smarter_than_you[which_smarter][1], gamelog);
			} else {
				addstr(pickrandom(smarter_than_you_one_line), gamelog);
			}
         }
         //Attempt to convert when the target is brutally treated will
         //just alienate them and make them cynical
         else if(techniques[TECHNIQUE_BEAT] || rapport_temp < -2)
         {
            move(y++,0);
            addstr(cr->name, gamelog);
            int which_hatred = LCSrandom(develops_hatred_number + develops_hatred_one_line_number);
            if(which_hatred < develops_hatred_number)
			{
				addstr(develops_hatred[which_hatred][0], gamelog);
				addstr(a->name);
				addstr(develops_hatred[which_hatred][1], gamelog);
			}else{
				addstr(pickrandom(develops_hatred_one_line), gamelog);
            }
            gamelog.newline();
            if(a->skill_check(SKILL_SEDUCTION,DIFFICULTY_CHALLENGING))
            {
               //show_interrogation_sidebar(cr,a);

               getkey();

               move(y++,0);
               addstr(a->name, gamelog);
			   int which_compassion = LCSrandom(number_of_compassion + number_of_compassion_one_line);
			   if(which_compassion < number_of_compassion){
				   addstr(interrogater_shows_compassion[which_compassion][0], gamelog);
				   addstr(cr->name, gamelog);
				   addstr(interrogater_shows_compassion[which_compassion][1], gamelog);
			   }else{
				   addstr(pickrandom(interrogater_shows_compassion_one_line), gamelog);
			   }

               gamelog.newline();
               rapport[a->id]+=0.7f;
               if(rapport[a->id]>3)
               {
                  //show_interrogation_sidebar(cr,a);

                  getkey();

                  move(y++,0);
                  addstr(cr->name, gamelog);
				  int which_cling = LCSrandom(number_of_clinging + number_of_clinging_one_line);
				  if(which_cling < number_of_clinging){
					  addstr(cling_to_interrogater[which_cling][0], gamelog);
					  addstr(a->name, gamelog);
					  addstr(cling_to_interrogater[which_cling][1], gamelog);
				  }else{
					  addstr(pickrandom(clinging_one_line), gamelog);
				  }
                  gamelog.newline();

                  if(rapport[a->id]>5) turned=1;
               }
            }

            if(cr->get_attribute(ATTRIBUTE_HEART,false)>1) cr->adjust_attribute(ATTRIBUTE_HEART,-1);
         }
         //Failure to break religious convictions
         else if(cr->get_skill(SKILL_RELIGION)>a->get_skill(SKILL_RELIGION)+a->get_skill(SKILL_PSYCHOLOGY) && !techniques[TECHNIQUE_DRUGS])
         {
            move(y++,0);

			int which_cling = LCSrandom(number_of_clinging_religion + number_of_clinging_religion_one_line);
			if(which_cling < number_of_clinging_religion){
				addstr(a->name, gamelog);
				addstr(cling_to_religion[which_cling][0], gamelog);
				addstr(cr->name, gamelog);
				addstr(cling_to_religion[which_cling][1], gamelog);
			}else{
				addstr(cr->name, gamelog);
				addstr(pickrandom(cling_to_religion_one_line), gamelog);
			}
            gamelog.newline();

            a->train(SKILL_RELIGION,cr->get_skill(SKILL_RELIGION)*4);
         }
         //Failure to persuade entrenched capitalists
         else if(cr->get_skill(SKILL_BUSINESS)>a->get_skill(SKILL_BUSINESS)+a->get_skill(SKILL_PSYCHOLOGY) && !techniques[TECHNIQUE_DRUGS])
         {
            move(y++,0);

			int which_cling = LCSrandom(number_of_clinging_business + number_of_clinging_business_one_line);
			if(which_cling < number_of_clinging_business){
				addstr(cr->name, gamelog);
				addstr(cling_to_business[which_cling][0], gamelog);
				addstr(a->name, gamelog);
				addstr(cling_to_business[which_cling][1], gamelog);
			}else{
				addstr(cr->name, gamelog);
				addstr(pickrandom(cling_to_business_one_line), gamelog);
			}
			gamelog.newline();

            a->train(SKILL_BUSINESS,cr->get_skill(SKILL_BUSINESS)*4);
         }
         //Failure to persuade scientific minds
         else if(cr->get_skill(SKILL_SCIENCE)>a->get_skill(SKILL_SCIENCE)+a->get_skill(SKILL_PSYCHOLOGY) && !techniques[TECHNIQUE_DRUGS])
         {
            move(y++,0);

			int which_cling = LCSrandom(number_of_clinging_science + number_of_clinging_science_one_line);
			if(which_cling < number_of_clinging_science){
				addstr(cr->name, gamelog);
				addstr(cling_to_science[which_cling][0], gamelog);
				addstr(a->name, gamelog);
				addstr(cling_to_science[which_cling][1], gamelog);
			}else{
				addstr(cr->name, gamelog);
				addstr(pickrandom(cling_to_science_one_line), gamelog);
			}

            gamelog.newline();

            a->train(SKILL_SCIENCE,cr->get_skill(SKILL_SCIENCE)*4);
         }
         //Target is swayed by Liberal Reason -- skilled interrogators, time held,
         //and rapport contribute to the likelihood of this
         else if(!(cr->attribute_check(ATTRIBUTE_WISDOM,attack/6)))
         {
            if(cr->juice>0)
            {
               cr->juice-=attack;
               if(cr->juice<0) cr->juice=0;
            }

            if(cr->get_attribute(ATTRIBUTE_HEART,false)<10)
               cr->adjust_attribute(ATTRIBUTE_HEART,+1);
            //Improve rapport with interrogator
            rapport[a->id]+=1.5;

            //Join LCS??
            //1) They were liberalized
            if(cr->get_attribute(ATTRIBUTE_HEART,true)>cr->get_attribute(ATTRIBUTE_WISDOM,true)+4) turned=1;
            //2) They were befriended
            if(rapport[a->id]>4) turned=1;

            move(y++,0);
            addstr(cr->name, gamelog);
			addstr(pickrandom(partial_conversion), gamelog);
            gamelog.newline();
            if(location[cr->worklocation]->mapped==0 && !LCSrandom(5))
            {
               move((++y)++,0);
               addstr(cr->name, gamelog);
               addstr(" reveals details about the ", gamelog);
               addstr(location[cr->worklocation]->name, gamelog);
               addstr(".", gamelog);
               gamelog.newline();
               move(y++,0);
               if(location[cr->worklocation]->type<=SITE_RESIDENTIAL_SHELTER)
               {
                  addstr("Unfortunately, none of it is useful to the LCS.", gamelog);
               }
               else
               {
                  addstr(a->name, gamelog);
                  addstr(" was able to create a map of the site with this information.", gamelog);
               }
               gamelog.newline();
               location[cr->worklocation]->mapped=1;
               location[cr->worklocation]->hidden=0;
            }
         }
         //Target is not sold on the LCS arguments and holds firm
         //This is the worst possible outcome if you use props
         else if(!(cr->skill_check(SKILL_PERSUASION,a->get_attribute(ATTRIBUTE_HEART, true))) || techniques[TECHNIQUE_PROPS])
         {
            //Not completely unproductive; builds rapport
            rapport[a->id]+=0.2f;

            move(y++,0);
            addstr(cr->name, gamelog);
            addstr(" holds firm.", gamelog);
            gamelog.newline();
         }
         //Target actually wins the argument so successfully that the Liberal
         //interrogator's convictions are the ones that are shaken
         else
         {
            //Consolation prize is that they end up liking the
            //liberal more
            rapport[a->id]+=0.5f;

            a->adjust_attribute(ATTRIBUTE_WISDOM,+1);

            move(y++,0);
            addstr(cr->name, gamelog);
            addstr(" turns the tables on ", gamelog);
            addstr(a->name, gamelog);
            addstr("!", gamelog);
            gamelog.newline();

            //show_interrogation_sidebar(cr,a);
            getkey();

            move(y++,0);
            addstr(a->name, gamelog);
            addstr(" has been tainted with wisdom!", gamelog);
            gamelog.newline();
         }

         //show_interrogation_sidebar(cr,a);
         getkey();
      }

      //Lead interrogator gets bonus experience
      if(!techniques[TECHNIQUE_KILL])
      {
         a->train(SKILL_PSYCHOLOGY,attack/2+1);
         //Others also get experience
         for(int i=0;i<len(temppool);i++) temppool[i]->train(SKILL_PSYCHOLOGY,attack/4+1);
      }

      //Possibly suicidal when heart is down to 1 and prisoner has already been
      //captive for a week without rescue
      if(!turned&&cr->alive&&cr->get_attribute(ATTRIBUTE_HEART,false)<=1&&LCSrandom(3)&&cr->joindays>6)
      {
         move(++y,0);

         //can't commit suicide if restrained
         if(LCSrandom(6)||techniques[TECHNIQUE_RESTRAIN])
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,0);
            addstr(cr->name, gamelog);
            //can't cut self if restrained
			if(! techniques[TECHNIQUE_RESTRAIN] && ! LCSrandom(5)){
				addstr(" " + pickrandom(self_wounding), gamelog); cr->blood-=LCSrandom(15)+10;
			}else{
				addstr(" " + pickrandom(broods_over_death), gamelog);
            }
            gamelog.newline();
         }
         else
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr(cr->name, gamelog);
            addstr(" has committed suicide.", gamelog);
            gamelog.newline(); //TODO: Next message?
            cr->die();
         }
         y++;
         //show_interrogation_sidebar(cr,a);

         getkey();
      }

      //Death
      if(cr->alive==0||cr->blood<1)
      {
         //delete interrogation information
         delete intr;
         cr->die();

         stat_kills++;
         move(++y,0);
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr(cr->name, gamelog);
         addstr(" is dead", gamelog);
         if(a)
         {
            addstr(" under ", gamelog);
            addstr(a->name, gamelog);
            addstr("'s interrogation.", gamelog);
         }
         else addstr(".", gamelog);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         y++;
         //show_interrogation_sidebar(cr,a);

         getkey();

         if(a)
         {
            if(LCSrandom(a->get_attribute(ATTRIBUTE_HEART,false)))
            {
               gamelog.newline();
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(++y,0);
               addstr(a->name, gamelog);
               addstr(" feels sick to the stomach afterward and ", gamelog);
               a->adjust_attribute(ATTRIBUTE_HEART,-1);
               move(++y,0);
               addstr(pickrandom(feels_sick), gamelog);
            }
            else if(!LCSrandom(3))
            {
               gamelog.newline();
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               move(++y,0);
               addstr(a->name, gamelog);
               addstr(" grows colder.", gamelog);
               a->adjust_attribute(ATTRIBUTE_WISDOM,+1);
            }
         }
      }
      gamelog.nextMessage();
      delete[] _attack;
   }
   #ifdef AUTOENLIGHTEN
   turned=1;// Lucky!
   #endif

   if(turned&&cr->alive)
   {
      //clear_interrogation_sidebar();
      //delete interrogation information
      delete intr;
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(++y,0);
      addstr("The Automaton has been Enlightened!   Your Liberal ranks are swelling!", gamelog);
      if(cr->get_attribute(ATTRIBUTE_HEART,true)>7 &&
         cr->get_attribute(ATTRIBUTE_WISDOM,true)>2 &&
        !LCSrandom(4) && (cr->flag & CREATUREFLAG_KIDNAPPED))
      {
         move(++y,0);
         gamelog.newline();
         addstr("The conversion is convincing enough that the police no longer consider it a kidnapping.", gamelog);
         //Actually liberalized -- they'll clean up the kidnapping story
         cr->flag&=~CREATUREFLAG_MISSING;
         cr->flag&=~CREATUREFLAG_KIDNAPPED;
      }
      cr->flag|=CREATUREFLAG_BRAINWASHED;

      for(int p=0;p<len(pool);p++)
         if(pool[p]->activity.type==ACTIVITY_HOSTAGETENDING&&pool[p]->activity.arg==cr->id)
            pool[p]->activity.type=ACTIVITY_NONE;

      y+=2;
      liberalize(*cr,false);
      cr->hireid=a->id;
      stat_recruits++;

      if(location[cr->worklocation]->mapped==0 || location[cr->worklocation]->hidden==1)
      {
         gamelog.newline();
         move(y,0);
         addstr(cr->name, gamelog);
         addstr(" reveals details about the ", gamelog);
         addstr(location[cr->worklocation]->name, gamelog);
         addstr(".", gamelog);
         gamelog.newline();
         move(++y,0);
         if(location[cr->worklocation]->type<=SITE_RESIDENTIAL_SHELTER)
            addstr("Unfortunately, none of it is useful to the LCS.", gamelog);
         else
         {
            addstr(a->name, gamelog);
            addstr(" was able to create a map of the site with this information.", gamelog);
         }
         location[cr->worklocation]->mapped=1;
         location[cr->worklocation]->hidden=0;
         y+=2;
      }

      if(cr->flag & CREATUREFLAG_MISSING && !(cr->flag & CREATUREFLAG_KIDNAPPED))
      {
         getkey();

         erase();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(y=1,0);
         gamelog.newline();
         addstr(cr->name, gamelog);
         addstr("'s disappearance has not yet been reported.", gamelog);
         sleeperize_prompt(*cr,*a,y+=2);
         cr->flag&=~CREATUREFLAG_MISSING;
         gamelog.nextMessage();

         return;
      }
   }

   if(cr->align==1||!cr->alive) for(int p=0;p<len(pool);p++)
   {
      if(!pool[p]->alive) continue;
      if(pool[p]->activity.type==ACTIVITY_HOSTAGETENDING&&pool[p]->activity.arg==cr->id)
         pool[p]->activity.type=ACTIVITY_NONE;
   }

   gamelog.nextMessage();

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(24,0);
   addstr("Press any key to reflect on this.");

   getkey();
}
