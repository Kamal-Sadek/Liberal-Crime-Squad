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

#include <includes.h>
#include <externs.h>

// Macro dumps interrogation data to screen for debug
#ifdef INTERROGATION_DEBUG
   #define DEBUG_VARIABLE_DUMP(text,value) { char c[5]; move(20,0); itoa(value,c,10); addstr(text); addstr(c); addstr(" "); }
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
   {
      move(i,40);
      addstr("                                  ");
   }
}

// Shows the interrogation data at the right side of the screen
void show_interrogation_sidebar( Creature * cr, Creature * a )
{
   clear_interrogation_sidebar();
   int y=4;
   char num2[10];
   map<long,struct float_zero>& rapport = reinterpret_cast<interrogation*>(cr->activity.arg)->rapport;
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
   itoa(cr->get_attribute(ATTRIBUTE_HEART,true),num2,10);
   addstr("Heart: ");
   addstr(num2);
   move(++y,40);
   itoa(cr->get_attribute(ATTRIBUTE_WISDOM,true),num2,10);
   addstr("Wisdom: ");
   addstr(num2);
   move(++y,40);
   itoa(cr->get_attribute(ATTRIBUTE_HEALTH,true),num2,10);
   addstr("Health: ");
   addstr(num2);

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
   itoa(a->get_skill(SKILL_PSYCHOLOGY),num2,10);
   addstr("Psychology Skill: ");
   addstr(num2);
   move(++y,40);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   itoa(a->get_attribute(ATTRIBUTE_HEART,true),num2,10);
   addstr("Heart: ");
   addstr(num2);
   move(++y,40);
   itoa(a->get_attribute(ATTRIBUTE_WISDOM,true),num2,10);
   addstr("Wisdom: ");
   addstr(num2);
   move(++y,40);
   itoa(a->get_attribute(ATTRIBUTE_WISDOM,true),num2,10);
   addstr("Outfit: ");
   //char str[40];
   addstr(a->get_armor().get_name().c_str());
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
   vector<Creature *> temppool;
   int p;

   Creature *a=NULL;



   //Find all tenders who are set to this hostage
   for(p=0;p<(int)pool.size();p++)
   {
      if(!pool[p]->alive)continue;
      if(pool[p]->activity.type==ACTIVITY_HOSTAGETENDING)
      {

         if(pool[p]->activity.arg==cr->id)
         {
            //If they're in the same location as the hostage,
            //include them in the interrogation
            if(pool[p]->location==cr->location)
            {
               if(pool[p]->location!=-1)
                  temppool.push_back(pool[p]);
            }
            //If they're someplace else, take them off the job
            else
               pool[p]->activity.type=ACTIVITY_NONE;
         }
      }
   }

   if(cr->location==-1)
   {
      delete cr;
      return;
   }

   //possible hostage escape attempt if unattended or unrestrained
   if(temppool.size()==0||
      !reinterpret_cast<interrogation*>(cr->activity.arg)->techniques[TECHNIQUE_RESTRAIN])
   {
      //CHECK FOR HOSTAGE ESCAPE
      if(LCSrandom(200)+25*(int)temppool.size()<
         cr->get_attribute(ATTRIBUTE_INTELLIGENCE,true)+
         cr->get_attribute(ATTRIBUTE_AGILITY,true)+
         cr->get_attribute(ATTRIBUTE_STRENGTH,true)&&
         cr->joindays>=5)
      {
         for(int p=0;p<(int)pool.size();p++)
         {
            if(pool[p]==cr)
            {
               if(clearformess)erase();
               else
               {
                  makedelimiter(8,0);
               }

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(cr->name, gamelog);
               addstr(" has escaped!", gamelog);
               gamelog.nextMessage();
               location[cr->location]->siege.timeuntillocated=3;
               refresh();
               getch();

               //clear activities for tenders
               for(int i=0;i<(int)pool.size();i++)
               {
                  if(!pool[i]->alive)continue;
                  if(pool[i]->activity.type==ACTIVITY_HOSTAGETENDING)
                  {
                     if(pool[i]->activity.arg==cr->id)
                     {
                        pool[i]->activity.type=ACTIVITY_NONE;
                     }
                  }
               }

               //delete interrogation data
               delete reinterpret_cast<interrogation*>(pool[p]->activity.arg);
               delete_and_remove(pool,p);
               break;
            }
         }
         return;
      }

      if(temppool.size()==0)return;
   }

   clearformess=1;

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   addstr("The Education of ");
   addstr(cr->name);
   addstr(": Day ");
   char num[20];
   itoa(cr->joindays,num,10);
   addstr(num);

   refresh();
   getch();

   set_color(COLOR_WHITE,COLOR_BLACK,0);

   char turned=0;

   int y=3;

   {
      int p;

      int business=0;
      int religion=0;
      int science=0;

      //each day, the attack roll is initialized to the number of days of the stay with
      //the LCS -- they will eventually break, but also eventually become too traumatized
      //to continue
      int attack = 0;
      int& druguse = reinterpret_cast<interrogation*>(cr->activity.arg)->druguse;

      map<long,struct float_zero>& rapport = reinterpret_cast<interrogation*>(cr->activity.arg)->rapport;

      int* _attack   = new int[temppool.size()];

      for(p=0;p<(int)temppool.size();p++)
      {
         _attack[p] = 0;
         if(temppool[p]!=NULL)
         {
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

               if(_attack[p] < 0)
                  _attack[p] = 0;

               if(_attack[p]>attack)
               {
                  attack=_attack[p];
               }
            }
         }
      }

      vector<int> goodp;

      for(p=0;p<(int)temppool.size();p++)
      {
         if(temppool[p]!=NULL)
         {
            if(temppool[p]->alive)
            {
               if(_attack[p]==attack)
               {
                  goodp.push_back(p);
               }
            }
         }
      }
      a=temppool[goodp[LCSrandom(goodp.size())]];

      attack+=temppool.size();
      attack+=cr->joindays;

      attack+=business-cr->get_skill(SKILL_BUSINESS);
      attack+=religion-cr->get_skill(SKILL_RELIGION);
      attack+=science-cr->get_skill(SKILL_SCIENCE);
      attack+=a->skill_roll(SKILL_PSYCHOLOGY)-cr->skill_roll(SKILL_PSYCHOLOGY);

      attack+=cr->attribute_roll(ATTRIBUTE_HEART);
      attack-=cr->attribute_roll(ATTRIBUTE_WISDOM)*2;

      bool techniques[6];
      //recall interrogation plan
      for(int i=0; i<6; i++)
      {
         techniques[i]=reinterpret_cast<interrogation*>(cr->activity.arg)->techniques[i];
      }

      while(1)
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
         move(y,0);y+=1;addstr("A - ");
         if(!techniques[TECHNIQUE_TALK]) addstr("No Verbal Contact     ");
         else addstr("Attempt to Convert");
         if(!techniques[TECHNIQUE_KILL])set_color(COLOR_WHITE,COLOR_BLACK,techniques[TECHNIQUE_RESTRAIN]);
         move(y,0);y+=1;addstr("B - ");
         if(!techniques[TECHNIQUE_RESTRAIN]) addstr("No ");
         addstr("Physical Restraints   ");
         if(!techniques[TECHNIQUE_KILL])set_color(COLOR_WHITE,COLOR_BLACK,techniques[TECHNIQUE_BEAT]);
         move(y,0);y+=1;addstr("C - ");
         if(!techniques[TECHNIQUE_BEAT]) addstr("Not ");
         addstr("Violently Beaten    ");
         if(!techniques[TECHNIQUE_KILL])set_color(COLOR_WHITE,COLOR_BLACK,techniques[TECHNIQUE_PROPS]);
         move(y,0);addstr("D - ");
         if(!techniques[TECHNIQUE_PROPS])addstr("No ");
         addstr("Expensive Props     ");
         move(y,27);y+=1;
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
         move(y,0);y+=1;addstr("Press Enter to Confirm the Plan");

         show_interrogation_sidebar(cr,a);

         int c=getch();
         translategetch(c);
         if(c>='a'&&c<='e')techniques[c-'a']=!techniques[c-'a'];
         if(c=='k')techniques[TECHNIQUE_KILL]=!techniques[TECHNIQUE_KILL];
         if(c==10||c==ESC)break;
      }

      if(techniques[TECHNIQUE_PROPS] && ledger.get_funds()>=250)
      { ledger.subtract_funds(250,EXPENSE_HOSTAGE); }
      else { techniques[TECHNIQUE_PROPS] = 0; }
      if(techniques[TECHNIQUE_DRUGS] && ledger.get_funds()>=50)
      { ledger.subtract_funds(50,EXPENSE_HOSTAGE); }
      else { techniques[TECHNIQUE_DRUGS] = 0; }

      //remember interrogation choices
      for(int i=0; i<6; i++)
      {
         reinterpret_cast<interrogation*>(cr->activity.arg)->techniques[i]=techniques[i];
      }

      if(techniques[TECHNIQUE_KILL]) // Kill the Hostage
      {
         erase();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(0,0);
         addstr("The Final Education of ", gamelog);
         addstr(cr->name, gamelog);
         addstr(": Day ", gamelog);
         char num[20];
         itoa(cr->joindays,num,10);
         addstr(num, gamelog);
         gamelog.newline();

         a=NULL;

         for(int i=0;i<(int)temppool.size();++i)
         {
            if((int)LCSrandom(50)<temppool[i]->juice||
               LCSrandom(9)+1>=temppool[i]->get_attribute(ATTRIBUTE_HEART,0))
            {
               a=temppool[i];
               break;
            }
         }

         if(a)
         {
            //delete interrogation information
            delete reinterpret_cast<interrogation*>(cr->activity.arg);
            set_color(COLOR_MAGENTA,COLOR_BLACK,0);
            cr->die();
            stat_kills++;
            move(y,0);y++;
            addstr(a->name, gamelog);
            addstr(" executes ", gamelog);
            addstr(cr->name, gamelog);
            addstr(" by ", gamelog);
            switch(LCSrandom(5))
            {
               case 0:addstr("strangling it to death.", gamelog);break;
               case 1:addstr("beating it to death.", gamelog);break;
               case 2:addstr("burning photos of Reagan in front of it.", gamelog);break;
               case 3:addstr("telling it that taxes have been increased.", gamelog);break;
               case 4:addstr("telling it its parents wanted to abort it.", gamelog);break;
            }
            //show_interrogation_sidebar(cr,a);
            refresh();getch();
            if(LCSrandom(a->get_attribute(ATTRIBUTE_HEART,false))>LCSrandom(3))
            {
               gamelog.newline();
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(y,0);y++;
               addstr(a->name, gamelog);
               addstr(" feels sick to the stomach afterward and ", gamelog);
               a->adjust_attribute(ATTRIBUTE_HEART,-1);
               move(y,0);y++;
               switch(LCSrandom(4))
               {
                  case 0:addstr("throws up in a trash can.", gamelog);break;
                  case 1:addstr("gets drunk, eventually falling asleep.", gamelog);break;
                  case 2:addstr("curls up in a ball, crying softly.", gamelog);break;
                  case 3:addstr("shoots up and collapses in a heap on the floor.", gamelog);break;
               }
            }
            else if(!LCSrandom(3))
            {
               gamelog.newline();
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               move(y,0);y++;
               addstr(a->name, gamelog);
               addstr(" grows colder.", gamelog);
               a->adjust_attribute(ATTRIBUTE_WISDOM,+1);
            }
            gamelog.nextMessage();
         }
         else
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,0);
            move(y,0);y++;
            addstr("There is no one able to get up the nerve to ", gamelog);
            move(y,0);y++;
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
         refresh();
         getch();

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(24,0);
         addstr("Press any key to reflect on this.");

         refresh();
         getch();

         if(cr->alive==0)
         {
            for(int p=0;p<(int)pool.size();p++)
            {
               if(!pool[p]->alive)continue;
               if(pool[p]->activity.type==ACTIVITY_HOSTAGETENDING)
               {
                  if(pool[p]->activity.arg==cr->id)
                  {
                     pool[p]->activity.type=ACTIVITY_NONE;
                  }
               }
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
      char num[20];
      itoa(cr->joindays,num,10);
      addstr(num, gamelog);
      gamelog.newline();
      y=2;

      move(y,0);
      addstr("The Automaton", gamelog);
      if(techniques[TECHNIQUE_RESTRAIN]) // Restraint
      {
         addstr(" is tied hands and feet to a metal chair", gamelog);
         y++;move(y,0);
         addstr("in the middle of a back room.", gamelog);
         y++;
         gamelog.newline();

         attack+=5;
      }
      else
      {
         addstr(" is locked in a back room ", gamelog);
         y++;move(y,0);
         addstr("converted into a makeshift cell.", gamelog);
         gamelog.newline();
         y++;
      }
      //show_interrogation_sidebar(cr,a);
      refresh();
      getch();

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
            refresh();
            getch();

            addstr(cr->name, gamelog);
            addstr(" foams at the mouth and its eyes roll back in its skull.", gamelog);
            gamelog.newline();
            move(++y,0);
            refresh();
            getch();

            Creature* doctor=a; // the lead interrogator is doctor by default
            int maxskill=doctor->get_skill(SKILL_FIRSTAID);
            for(int i=0;i<(int)temppool.size();++i) // search for the best doctor
               if(temppool[i]->get_skill(SKILL_FIRSTAID)>maxskill)
               {
                  doctor=temppool[i]; // we found a doctor
                  maxskill=doctor->get_skill(SKILL_FIRSTAID);
               }

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
               refresh();
               getch();
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
                  refresh();
                  getch();
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
                  refresh();
                  getch();
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
         refresh();
         getch();
      }

      if(techniques[TECHNIQUE_BEAT]&&!turned&&cr->alive) // Beating
      {
         y+=1;
         move(y,0);

         int forceroll=0;
         bool tortured=0;

         for(int i=0;i<(int)temppool.size();i++)
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
            switch(LCSrandom(6))
            {
            case 0:addstr(" reenacts scenes from Abu Ghraib", gamelog);break;
            case 1:addstr(" whips the Automaton with a steel cable", gamelog);break;
            case 2:addstr(" holds the hostage's head under water", gamelog);break;
            case 3:addstr(" pushes needles under the Automaton's fingernails", gamelog);break;
            case 4:addstr(" beats the hostage with a metal bat", gamelog);break;
            case 5:addstr(" beats the hostage with a belt", gamelog);break;
            }
            addstr(", ", gamelog);
            y++;
            move(y,0);
            addstr("screaming \"", gamelog);
            int i=0;
            while(i<2)
            {
               switch(LCSrandom(10))
               {
               case 0:addstr("I hate you", gamelog);break;
               case 1:addstr("Does it hurt?", gamelog);break;
               case 2:addstr("Nobody loves you", gamelog);break;
               case 3:addstr("God hates you", gamelog);break;
               case 4:addstr("Don't fuck with me", gamelog);break;
               case 5:addstr("This is Liberalism", gamelog);break;
               case 6:addstr("Convert, bitch", gamelog);break;
               case 7:addstr("I'm going to kill you", gamelog);break;
               case 8:addstr("Do you love me?", gamelog);break;
               case 9:addstr("I am your God", gamelog);break;
               }
               if(++i<2)
                  addstr("! ", gamelog);
            }
            addstr("!\" in its face.", gamelog);
            gamelog.newline();
            if(cr->get_attribute(ATTRIBUTE_HEART,true)>1)cr->adjust_attribute(ATTRIBUTE_HEART,-1);
            if(cr->get_attribute(ATTRIBUTE_WISDOM,true)>1)cr->adjust_attribute(ATTRIBUTE_WISDOM,-1);
         }
         else
         {
            if(temppool.size()==1)
            {
               addstr(temppool[0]->name, gamelog);
               addstr(" beats", gamelog);
            }
            else if(temppool.size()==2)
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
               switch(LCSrandom(6))
               {
               case 0:addstr(" with a giant stuffed elephant", gamelog);break;
               case 1:addstr(" while draped in a Confederate flag", gamelog);break;
               case 2:addstr(" with a cardboard cutout of Reagan", gamelog);break;
               case 3:addstr(" with a King James Bible", gamelog);break;
               case 4:addstr(" with fists full of money", gamelog);break;
               case 5:addstr(" with Conservative propaganda on the walls", gamelog);break;
               }
            }
            addstr(", ", gamelog);
            y++;
            move(y,0);
            switch(LCSrandom(4))
            {
            case 0:addstr("scream", gamelog);break;
            case 1:addstr("yell", gamelog);break;
            case 2:addstr("shout", gamelog);break;
            case 3:addstr("holler", gamelog);break;
            }
            addstr("ing \"", gamelog);
            int i=0;
            while(i<3)
            {
               switch(LCSrandom(20))
               {
               case 0:addstr("McDonalds", gamelog);break;
               case 1:addstr("Microsoft", gamelog);break;
               case 2:addstr("Bill Gates", gamelog);break;
               case 3:addstr("Wal-Mart", gamelog);break;
               case 4:addstr("George W. Bush", gamelog);break;
               case 5:addstr("ExxonMobil", gamelog);break;
               case 6:addstr("Trickle-down economics", gamelog);break;
               case 7:addstr("Family values", gamelog);break;
               case 8:addstr("Conservatism", gamelog);break;
               case 9:addstr("War on Drugs", gamelog);break;
               case 10:addstr("War on Terror", gamelog);break;
               case 11:addstr("Ronald Reagan", gamelog);break;
               case 12:addstr("Rush Limbaugh", gamelog);break;
               case 13:addstr("Tax cuts", gamelog);break;
               case 14:addstr("Military spending", gamelog);break;
               case 15:addstr("Ann Coulter", gamelog);break;
               case 16:addstr("Deregulation", gamelog);break;
               case 17:addstr("Police", gamelog);break;
               case 18:addstr("Corporations", gamelog);break;
               case 19:addstr("Wiretapping", gamelog);break;

               }
               if(++i<3)
                  addstr("! ", gamelog);
            }
            addstr("!\" in its face.", gamelog);
            gamelog.newline();
         }
         y++;

         cr->blood-=(5 + LCSrandom(5)) * (1+techniques[TECHNIQUE_PROPS]);

         //show_interrogation_sidebar(cr,a);
         refresh();
         getch();

         if(!(cr->attribute_check(ATTRIBUTE_HEALTH,forceroll)))
         {
            if(cr->skill_check(SKILL_RELIGION,forceroll))
            {
               move(y,0);
               addstr(cr->name, gamelog);
               if(!techniques[TECHNIQUE_DRUGS])
               {
                  switch(LCSrandom(2))
                  {
                  case 0:addstr(" prays...", gamelog);break;
                  case 1:addstr(" cries out for God.", gamelog);break;
                  }
                  gamelog.newline();
               }
               else
               {
                  switch(LCSrandom(2))
                  {
                  case 0:addstr(" takes solace in the personal appearance of God.", gamelog);break;
                  case 1:addstr(" appears to be having a religious experience.", gamelog);break;
                  }
                  gamelog.newline();
               }
               y++;
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
                  if(techniques[TECHNIQUE_RESTRAIN])addstr(" goes limp in the restraints.", gamelog);
                  else addstr(" curls up in the corner and doesn't move.", gamelog);break;
               case 2:
                  if(techniques[TECHNIQUE_DRUGS] && !LCSrandom(5))addstr(" barks helplessly.", gamelog);
                  else addstr(" cries helplessly.", gamelog);break;
               case 3:
                  if(techniques[TECHNIQUE_DRUGS] && !LCSrandom(3))addstr(" wonders about apples.", gamelog);
                  else addstr(" wonders about death.", gamelog);
                  break;
               }
               gamelog.newline();
               if(cr->get_attribute(ATTRIBUTE_HEART,false)>1)cr->adjust_attribute(ATTRIBUTE_HEART,-1);

               if(LCSrandom(2) && cr->juice>0)
               {
                  cr->juice-=forceroll;
                  if(cr->juice<0)cr->juice=0;
               }
               else if(cr->get_attribute(ATTRIBUTE_WISDOM,false)>1)
               {
                  cr->set_attribute(ATTRIBUTE_WISDOM,cr->get_attribute(ATTRIBUTE_WISDOM,false)-(forceroll/10));
                  if(cr->get_attribute(ATTRIBUTE_WISDOM,false)<1)cr->set_attribute(ATTRIBUTE_WISDOM,1);
               }

               if(location[cr->worklocation]->mapped==0 && !LCSrandom(5))
               {
                  //show_interrogation_sidebar(cr,a);
                  refresh();
                  getch();
                  move(y++,0);
                  addstr(a->name, gamelog);
                  addstr(" beats information out of the pathetic thing.", gamelog);
                  gamelog.newline();
                  move(y++,0);
                  refresh();
                  getch();
                  if(location[cr->worklocation]->type<=SITE_RESIDENTIAL_SHELTER)
                  {
                     addstr("Unfortunately, none of it is useful to the LCS.", gamelog);
                  }
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
               move(y,0);
               addstr(cr->name, gamelog);
               addstr(" seems to be getting the message.", gamelog);
               gamelog.newline();
               y++;

               if(cr->juice>0)
               {
                  cr->juice-=forceroll;
                  if(cr->juice<0)cr->juice=0;
               }

               if(cr->get_attribute(ATTRIBUTE_WISDOM,false)>1)
               {
                  cr->set_attribute(ATTRIBUTE_WISDOM,cr->get_attribute(ATTRIBUTE_WISDOM,false)-(forceroll/10+1));
                  if(cr->get_attribute(ATTRIBUTE_WISDOM,false)<1)cr->set_attribute(ATTRIBUTE_WISDOM,1);
               }
            }

            if(!(cr->attribute_check(ATTRIBUTE_HEALTH,forceroll/3)))
            {
               //show_interrogation_sidebar(cr,a);
               refresh();
               getch();

               move(y,0);
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
               y++;
            }
         }
         else
         {
            move(y,0);
            addstr(cr->name, gamelog);
            addstr(" takes it well.", gamelog);
            gamelog.newline();
            y++;
         }
         //show_interrogation_sidebar(cr,a);
         getch();

         if(tortured && cr->alive)
         {
            if(LCSrandom(a->get_attribute(ATTRIBUTE_HEART,false))>LCSrandom(3))
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(y,0);y++;
               addstr(a->name, gamelog);
               addstr(" feels sick to the stomach afterward and ", gamelog);
               a->adjust_attribute(ATTRIBUTE_HEART,-1);
               move(y,0);y++;
               switch(LCSrandom(4))
               {
                  case 0:addstr("throws up in a trash can.", gamelog);break;
                  case 1:addstr("gets drunk, eventually falling asleep.", gamelog);break;
                  case 2:addstr("curls up in a ball, crying softly.", gamelog);break;
                  case 3:addstr("shoots up and collapses in a heap on the floor.", gamelog);break;
               }
               gamelog.newline();
            }
            else if(!LCSrandom(3))
            {
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               move(y,0);y++;
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

         y+=1;
         move(y,0);
         addstr(a->name, gamelog);

         if(techniques[TECHNIQUE_PROPS])//props
         {
            attack += 10;
            switch(LCSrandom(9))
            {
            case 0:addstr(" plays violent video games with ", gamelog);break;
            case 1:addstr(" reads Origin of the Species to ", gamelog);break;
            case 2:addstr(" burns flags in front of ", gamelog);break;
            case 3:addstr(" explores an elaborate political fantasy with ", gamelog);break;
            case 4:addstr(" watches controversial avant-garde films with ", gamelog);break;
            case 5:addstr(" plays the anime film Bible Black for ", gamelog);break;// Yes, this is a porno.
            case 6:addstr(" watches a documentary about Emmett Till with ", gamelog);break;
            case 7:addstr(" watches Michael Moore films with ", gamelog);break;
            case 8:addstr(" listens to Liberal radio shows with ", gamelog);break;
            }
            gamelog.newline();
         }
         else
         {
            char str[75];
            switch(LCSrandom(4))
            {
            case 0:addstr(" talks about ", gamelog);
                   getviewsmall(str,LCSrandom(VIEWNUM-3));
                   addstr(str, gamelog);
                   addstr(" with ", gamelog);break;
            case 1:addstr(" argues about ", gamelog);
                   getviewsmall(str,LCSrandom(VIEWNUM-3));
                   addstr(str, gamelog);
                   addstr(" with ", gamelog);break;
            case 2:addstr(" tries to expose the true Liberal side of ", gamelog);break;
            case 3:addstr(" attempts to recruit ", gamelog);break;
            }
         }
         addstr(cr->name, gamelog);
         addstr(".", gamelog);
         gamelog.newline();
         y++;

         //Hallucinogenic drugs:
         //Re-interprets lead interrogator
         if(techniques[TECHNIQUE_DRUGS])
         {
            //show_interrogation_sidebar(cr,a);
            refresh();
            getch();

            move(y++,0);
            if(cr->skill_check(SKILL_PSYCHOLOGY,DIFFICULTY_CHALLENGING))
            {
               switch(LCSrandom(4))
               {
               case 0:addstr(cr->name);
                     addstr(" takes the drug-induced hallucinations with stoicism.", gamelog);
                     break;
               case 1:addstr(cr->name);
                     addstr(" mutters its initials over and over again.", gamelog);
                     break;
               case 2:addstr(cr->name);
                     addstr(" babbles continuous numerical sequences.", gamelog);
                     break;
               case 3:addstr(cr->name);
                     addstr(" manages to remain grounded through the hallucinations.", gamelog);
                     break;
               }
               gamelog.newline();
            }
            else if((rapport[a->id]>1 && !LCSrandom(3)) || !LCSrandom(10))
            {
               rapport_temp=10;
               switch(LCSrandom(4))
               {
               case 0:
                  addstr(cr->name, gamelog);
                  addstr(" hallucinates and sees ", gamelog);
                  addstr(a->name, gamelog);
                  addstr(" as an angel.", gamelog);
                  break;
               case 1:
                  addstr(cr->name);
                  addstr(" realizes with joy that ", gamelog);
                  addstr(a->name, gamelog);
                  addstr(" is Ronald Reagan!", gamelog);
                  break;
               case 2:
                  addstr(cr->name, gamelog);
                  addstr(" stammers and ", gamelog);
                  techniques[TECHNIQUE_RESTRAIN] ? addstr("talks about hugging ", gamelog) : addstr("hugs ", gamelog);
                  addstr(a->name, gamelog);
                  addstr(".", gamelog);
                  break;
               case 3:
                  addstr(cr->name, gamelog);
                  addstr(" begs ", gamelog);
                  addstr(a->name, gamelog);
                  addstr(" to let the colors stay forever.", gamelog);
                  break;
               }
               gamelog.newline();
            }
            else if((rapport[a->id]<-1 && LCSrandom(3)) || !LCSrandom(5))
            {
               attack=0;
               switch(LCSrandom(4))
               {
               case 0:
                  addstr(cr->name, gamelog);
                  addstr(" screams in horror as ", gamelog);
                  addstr(a->name, gamelog);
                  addstr(" turns into an alien.", gamelog);
                  break;
               case 1:
                  addstr(cr->name, gamelog);
                  if(!techniques[TECHNIQUE_RESTRAIN])addstr(" curls up and", gamelog);
                  addstr(" begs for the nightmare to end.", gamelog);
                  break;
               case 2:
                  addstr(cr->name, gamelog);
                  addstr(" watches ", gamelog);
                  addstr(a->name, gamelog);
                  addstr(" shift from one demonic form to another.", gamelog);
                  break;
               case 3:
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
               }
               gamelog.newline();
            }
            else
            {
               switch(LCSrandom(4))
               {
               case 0:
                  addstr(cr->name, gamelog);
                  addstr(" comments on the swirling light ", gamelog);
                  addstr(a->name, gamelog);
                  addstr(" is radiating.", gamelog);
                  break;
               case 1:
                  addstr(cr->name, gamelog);
                  addstr(" can't stop looking at the moving colors.", gamelog);
                  break;
               case 2:
                  addstr(cr->name, gamelog);
                  addstr(" laughs hysterically at ", gamelog);
                  addstr(a->name, gamelog);
                  addstr("'s altered appearance.", gamelog);
                  break;
               case 3:
                  addstr(cr->name, gamelog);
                  addstr(" barks and woofs like a dog.", gamelog);
                  break;
               }
               gamelog.newline();
            }
         }

         //show_interrogation_sidebar(cr,a);
         refresh();
         getch();

         if(cr->get_skill(SKILL_PSYCHOLOGY)>a->get_skill(SKILL_PSYCHOLOGY))
         {
            move(y,0);
            switch(LCSrandom(4))
            {
            case 0:addstr(cr->name, gamelog);
                  addstr(" plays mind games with ", gamelog);
                  addstr(a->name, gamelog);
                  addstr(".", gamelog);
                  break;
            case 1:addstr(cr->name, gamelog);
                  addstr(" knows how this works, and won't budge.", gamelog);
                  break;
            case 2:addstr(cr->name, gamelog);
                  addstr(" asks if Liberal mothers would approve of this.", gamelog);
                  break;
            case 3:addstr(cr->name, gamelog);
                  addstr(" seems resistant to this form of interrogation.", gamelog);
                  break;
            }
            y++;
         }
         //Attempt to convert when the target is brutally treated will
         //just alienate them and make them cynical
         else if(techniques[TECHNIQUE_BEAT] || rapport_temp < -2)
         {
            move(y,0);
            addstr(cr->name, gamelog);
            switch(LCSrandom(7))
            {
            case 0:addstr(" babbles mindlessly.", gamelog);break;
            case 1:addstr(" just whimpers.", gamelog);break;
            case 2:addstr(" cries helplessly.", gamelog);break;
            case 3:addstr(" is losing faith in the world.", gamelog);break;
            case 4:addstr(" only grows more distant.", gamelog);break;
            case 5:addstr(" is too terrified to even speak to ", gamelog);addstr(a->name);addstr(".");break;
            case 6:addstr(" just hates the LCS even more.", gamelog);break;
            }
            gamelog.newline();
            if(a->skill_check(SKILL_SEDUCTION,DIFFICULTY_CHALLENGING))
            {
               //show_interrogation_sidebar(cr,a);
               getch();
               move(++y,0);
               addstr(a->name, gamelog);
               switch(LCSrandom(7))
               {
               case 0:addstr(" consoles the Conservative automaton.", gamelog);break;
               case 1:addstr(" shares some chocolates.", gamelog);break;
               case 2:addstr(" provides a shoulder to cry on.", gamelog);break;
               case 3:addstr(" understands ", gamelog);addstr(cr->name, gamelog);addstr("'s pain.", gamelog);break;
               case 4:addstr("'s heart opens to the poor Conservative.", gamelog);break;
               case 5:addstr(" helps the poor thing to come to terms with captivity.", gamelog);break;
               case 6:addstr("'s patience and kindness leaves the Conservative confused.", gamelog);break;
               }
               gamelog.newline();
               rapport[a->id]+=0.7f;
               if(rapport[a->id]>3)
               {
                  //show_interrogation_sidebar(cr,a);
                  getch();
                  move(++y,0);
                  addstr(cr->name, gamelog);
                  switch(LCSrandom(7))
                  {
                  case 0:addstr(" emotionally clings to ", gamelog);
                     addstr(a->name, gamelog);
                     addstr("'s sympathy.", gamelog);break;
                  case 1:addstr(" begs ", gamelog);
                     addstr(a->name, gamelog);
                     addstr(" for help.", gamelog);break;
                  case 2:addstr(" promises to be good.", gamelog);break;
                  case 3:addstr(" reveals childhood pains.", gamelog);break;
                  case 4:addstr(" thanks ", gamelog);
                     addstr(a->name, gamelog);
                     addstr(" for being merciful.", gamelog);break;
                  case 5:addstr(" cries in ", gamelog);
                     addstr(a->name, gamelog);
                     addstr("'s arms.", gamelog);break;
                  case 6:addstr(" really likes ", gamelog);
                     addstr(a->name, gamelog);
                     addstr(".", gamelog);break;
                  }
                  gamelog.newline();

                  if(rapport[a->id]>5)turned=1;
               }
            }

            if(cr->get_attribute(ATTRIBUTE_HEART,false)>1)cr->adjust_attribute(ATTRIBUTE_HEART,-1);
            y++;
         }
         //Failure to break religious convictions
         else if(cr->get_skill(SKILL_RELIGION)>a->get_skill(SKILL_RELIGION)+a->get_skill(SKILL_PSYCHOLOGY) && !techniques[TECHNIQUE_DRUGS])
         {
            move(y,0);
            switch(LCSrandom(4))
            {
            case 0:addstr(a->name);
                  addstr(" is unable to shake ", gamelog);
                  addstr(cr->name, gamelog);
                  addstr("'s religious conviction.", gamelog);
                  break;
            case 1:addstr(cr->name, gamelog);
                  addstr(" will never be broken so long as God grants it strength.", gamelog);
                  break;
            case 2:addstr(a->name, gamelog);
                  addstr("'s efforts to question ", gamelog);
                  addstr(cr->name, gamelog);
                  addstr("'s faith seem futile.", gamelog);
                  break;
            case 3:addstr(cr->name, gamelog);
                  addstr(" calmly explains the Conservative tenets of its faith.", gamelog);
                  break;
            }
            gamelog.newline();

            a->train(SKILL_RELIGION,cr->get_skill(SKILL_RELIGION)*4);
            y++;
         }
         //Failure to persuade entrenched capitalists
         else if(cr->get_skill(SKILL_BUSINESS)>a->get_skill(SKILL_BUSINESS)+a->get_skill(SKILL_PSYCHOLOGY) && !techniques[TECHNIQUE_DRUGS])
         {
            move(y,0);
            switch(LCSrandom(4))
            {
            case 0:addstr(cr->name, gamelog);
                  addstr(" will never be moved by ", gamelog);
                  addstr(a->name, gamelog);
                  addstr("'s pathetic economic ideals.", gamelog);
                  break;
            case 1:addstr(cr->name, gamelog);
                  addstr(" wishes a big company would just buy the LCS and shut it down.", gamelog);
                  break;
            case 2:addstr(cr->name, gamelog);
                  addstr(" explains to ", gamelog);
                  addstr(a->name, gamelog);
                  addstr(" why communism failed.", gamelog);
                  break;
            case 3:addstr(cr->name, gamelog);
                  addstr(" mumbles incoherently about Reaganomics.", gamelog);
                  break;
            }
            gamelog.newline();

            a->train(SKILL_BUSINESS,cr->get_skill(SKILL_BUSINESS)*4);
            y++;
         }
         //Failure to persuade scientific minds
         else if(cr->get_skill(SKILL_SCIENCE)>a->get_skill(SKILL_SCIENCE)+a->get_skill(SKILL_PSYCHOLOGY) && !techniques[TECHNIQUE_DRUGS])
         {
            move(y,0);
            switch(LCSrandom(4))
            {
            case 0:addstr(cr->name, gamelog);
                  addstr(" wonders what mental disease has possessed ", gamelog);
                  addstr(a->name, gamelog);
                  addstr(".", gamelog);
                  break;
            case 1:addstr(cr->name, gamelog);
                  addstr(" explains why nuclear energy is safe.", gamelog);
                  break;
            case 2:addstr(cr->name, gamelog);
                  addstr(" makes Albert Einstein faces at ", gamelog);
                  addstr(a->name, gamelog);
                  addstr(".", gamelog);
                  break;
            case 3:addstr(cr->name, gamelog);
                  addstr(" pities ", gamelog);
                  addstr(a->name, gamelog);
                  addstr("'s blind ignorance of science.", gamelog);
                  break;
            }
            gamelog.newline();

            a->train(SKILL_SCIENCE,cr->get_skill(SKILL_SCIENCE)*4);
            y++;
         }
         //Target is swayed by Liberal Reason -- skilled interrogators, time held,
         //and rapport contribute to the likelihood of this
         else if(!(cr->attribute_check(ATTRIBUTE_WISDOM,attack/6)))
         {
            if(cr->juice>0)
            {
               cr->juice-=attack;
               if(cr->juice<0)cr->juice=0;
            }

            if(cr->get_attribute(ATTRIBUTE_HEART,false)<10)
            {
               cr->adjust_attribute(ATTRIBUTE_HEART,+1);
            }
            //Improve rapport with interrogator
            rapport[a->id]+=1.5;

            //Join LCS??
            //1) They were liberalized
            if(cr->get_attribute(ATTRIBUTE_HEART,true)>cr->get_attribute(ATTRIBUTE_WISDOM,true)+4)turned=1;
            //2) They were befriended
            if(rapport[a->id]>4)turned=1;

            move(y,0);
            addstr(cr->name, gamelog);
            switch(LCSrandom(5))
            {
            case 0:addstr("'s Conservative beliefs are shaken.", gamelog);break;
            case 1:addstr(" quietly considers these ideas.", gamelog);break;
            case 2:addstr(" is beginning to see Liberal reason.", gamelog);break;
            case 3:addstr(" has a revelation of understanding.", gamelog);break;
            case 4:addstr(" grudgingly admits sympathy for LCS ideals.", gamelog);break;
            }
            gamelog.newline();
            y++;
            if(location[cr->worklocation]->mapped==0 && !LCSrandom(5))
            {
               y++;
               move(y++,0);
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

            move(y,0);
            addstr(cr->name, gamelog);
            addstr(" holds firm.", gamelog);
            gamelog.newline();
            y++;
         }
         //Target actually wins the argument so successfully that the Liberal
         //interrogator's convictions are the ones that are shaken
         else
         {
            //Consolation prize is that they end up liking the
            //liberal more
            rapport[a->id]+=0.5f;

            a->adjust_attribute(ATTRIBUTE_WISDOM,+1);

            move(y,0);
            addstr(cr->name, gamelog);
            addstr(" turns the tables on ", gamelog);
            addstr(a->name, gamelog);
            addstr("!", gamelog);
            gamelog.newline();
            y++;

            //show_interrogation_sidebar(cr,a);
            refresh();
            getch();

            move(y,0);
            addstr(a->name, gamelog);
            addstr(" has been tainted with wisdom!", gamelog);
            gamelog.newline();
            y++;
         }

         //show_interrogation_sidebar(cr,a);
         getch();
      }

      //Lead interrogator gets bonus experience
      if(!techniques[TECHNIQUE_KILL])a->train(SKILL_PSYCHOLOGY,attack/2+1);
      //Others also get experience
      for(int i=0;i<(int)temppool.size();i++)
         temppool[i]->train(SKILL_PSYCHOLOGY,(attack/4)+1);



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
            switch(LCSrandom(5-techniques[TECHNIQUE_RESTRAIN]))
            {
            case 0:addstr(" mutters about death.", gamelog);break;
            case 1:addstr(" broods darkly.", gamelog);break;
            case 2:addstr(" has lost hope of rescue.", gamelog);break;
            case 3:addstr(" is making peace with God.", gamelog);break;
            case 4:addstr(" is bleeding from self-inflicted wounds.", gamelog);cr->blood-=LCSrandom(15)+10;break;
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
         refresh();
         getch();
      }

      //Death
      if(cr->alive==0||cr->blood<1)
      {
         //delete interrogation information
         delete reinterpret_cast<interrogation*>(cr->activity.arg);
         cr->die();

         stat_kills++;
         y++;
         move(y,0);
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr(cr->name, gamelog);
         addstr(" is dead", gamelog);
         if(a)
         {
            addstr(" under ", gamelog);
            addstr(a->name, gamelog);
            addstr("'s interrogation.", gamelog);
         }
         else
         {
            addstr(".", gamelog);
         }
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         y++;
         //show_interrogation_sidebar(cr,a);
         refresh();
         getch();
         if(a)
         {
            if(LCSrandom(a->get_attribute(ATTRIBUTE_HEART,false)))
            {
               gamelog.newline();
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(y,0);y++;
               addstr(a->name, gamelog);
               addstr(" feels sick to the stomach afterward and ", gamelog);
               a->adjust_attribute(ATTRIBUTE_HEART,-1);
               move(y,0);y++;
               switch(LCSrandom(4))
               {
                  case 0:addstr("throws up in a trash can.", gamelog);break;
                  case 1:addstr("gets drunk, eventually falling asleep.", gamelog);break;
                  case 2:addstr("curls up in a ball, crying softly.", gamelog);break;
                  case 3:addstr("shoots up and collapses in a heap on the floor.", gamelog);break;
               }
            }
            else if(!LCSrandom(3))
            {
               gamelog.newline();
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               move(y,0);y++;
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
      delete reinterpret_cast<interrogation*>(cr->activity.arg);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      y++;
      move(y,0);
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

      for(int p=0;p<(int)pool.size();p++)
      {
         if(pool[p]->activity.type==ACTIVITY_HOSTAGETENDING &&
            pool[p]->activity.arg==cr->id)
         {
            pool[p]->activity.type=ACTIVITY_NONE;
         }
      }

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
         {
            addstr("Unfortunately, none of it is useful to the LCS.", gamelog);
         }
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
         refresh();
         getch();

         erase();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         y=1;
         move(y,0);
         gamelog.newline();
         addstr(cr->name, gamelog);
         addstr("'s disappearance has not yet been reported.", gamelog);
         y+=2;

         sleeperize_prompt(*cr,*a,y);

         cr->flag&=~CREATUREFLAG_MISSING;

         gamelog.nextMessage();

         return;
      }
   }

   if(cr->align==1||!cr->alive)
   {
      for(int p=0;p<(int)pool.size();p++)
      {
         if(!pool[p]->alive)continue;
         if(pool[p]->activity.type==ACTIVITY_HOSTAGETENDING)
         {
            if(pool[p]->activity.arg==cr->id)
            {
               pool[p]->activity.type=ACTIVITY_NONE;
            }
         }
      }
   }

   gamelog.nextMessage();

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(24,0);
   addstr("Press any key to reflect on this.");

   refresh();
   getch();
}
