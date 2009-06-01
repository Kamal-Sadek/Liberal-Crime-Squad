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

enum InterrogationTechnqiues
{
   TECHNIQUE_TALK,
   TECHNIQUE_RESTRAIN,
   TECHNIQUE_BEAT,
   TECHNIQUE_NOSLEEP,
   //TECHNIQUE_NOLIGHTS, XXX
   TECHNIQUE_GIVEFOOD,
   TECHNIQUE_GIVEWATER,
   TECHNIQUE_PROPS,
   TECHNIQUE_DRUGS,
   TECHNIQUE_KILL
};

/* hostage tending */
void tendhostage(Creature *cr,char &clearformess)
{
   vector<Creature *> temppool;
   int p;

   Creature *a=NULL;

   //Find all tenders who are set to this hostage
   for(p=0;p<pool.size();p++)
   {
      if(!pool[p]->alive)continue;
      if(pool[p]->activity.type==ACTIVITY_HOSTAGETENDING)
      {
         if(pool[p]->activity.arg==cr->id)
         {
            //If they're in the same location as the hostage,
            //include them in the interrogation
            if(pool[p]->location==cr->location)
               temppool.push_back(pool[p]);
            //If they're someplace else, take them off the job
            else
               pool[p]->activity.type=ACTIVITY_NONE;
         }
      }
   }

   //possible hostage escape attempt if unattended or unrestrained
   if(temppool.size()==0||
      !reinterpret_cast<interrogation*>(cr->activity.arg)->techniques[TECHNIQUE_RESTRAIN])
   {
      //CHECK FOR HOSTAGE ESCAPE
      if((int)(LCSrandom(200)+10*temppool.size())<
         cr->attval(ATTRIBUTE_INTELLIGENCE)+
         cr->attval(ATTRIBUTE_AGILITY)+
         cr->attval(ATTRIBUTE_STRENGTH)&&
         cr->joindays>=2)
      {
         for(int p=0;p<pool.size();p++)
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
               addstr(cr->name);
               addstr(" has escaped!");
               location[cr->location]->siege.timeuntillocated=3;
               refresh();
               getch();
               
               //clear activities for tenders
               for(int i=0;i<pool.size();i++)
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
               delete pool[p];

               pool.erase(pool.begin() + p);
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
      int maxattack=0;

      int business=0;
      int religion=0;
      int science=0;

      //each day, spiritcrush is initialized to the average spiritcrush of
      //the entire stay with the LCS -- continued pressure just compounds
      //the trauma, while letting up takes a while to get them calmed down
      //and really brutal treatment early on will never be completely
      //forgotten
      int spiritcrush;
      if(cr->joindays)spiritcrush=reinterpret_cast<interrogation*>(cr->activity.arg)->totalspiritcrush/cr->joindays;
      else spiritcrush=0;
      int healthcrush=0;

      //get short references to the hostage's status
      int& nofood = reinterpret_cast<interrogation*>(cr->activity.arg)->nofood;
      int& nowater = reinterpret_cast<interrogation*>(cr->activity.arg)->nowater;
      int& nosleep = reinterpret_cast<interrogation*>(cr->activity.arg)->nosleep;
      //int& nolight = reinterpret_cast<interrogation*>(cr->activity.arg)->nolight;
      int& druguse = reinterpret_cast<interrogation*>(cr->activity.arg)->druguse;
      map<long,struct float_zero>& rapport = reinterpret_cast<interrogation*>(cr->activity.arg)->rapport;

      int * attack = new int[temppool.size()];

      for(p=0;p<temppool.size();p++)
      {
         attack[p] = 0;
         if(temppool[p]!=NULL)
         {
            if(temppool[p]->alive)
            {
               if(temppool[p]->skillval(SKILL_BUSINESS)>business)
                  business=temppool[p]->skillval(SKILL_BUSINESS);
               if(temppool[p]->skillval(SKILL_RELIGION)>religion)
                  religion=temppool[p]->skillval(SKILL_RELIGION);
               if(temppool[p]->skillval(SKILL_SCIENCE)>science)
                  science=temppool[p]->skillval(SKILL_SCIENCE);

               attack[p] = (temppool[p]->attval(ATTRIBUTE_HEART)-
                            temppool[p]->attval(ATTRIBUTE_WISDOM)+
                            temppool[p]->skillval(SKILL_PSYCHOLOGY)*2);

               attack[p] += temppool[p]->armor.interrogation_basepower();

               if(attack[p] < 0)
                  attack[p] = 0;

               if(attack[p]>maxattack)
               {
                  maxattack=attack[p];
               }
            }
         }
      }

      vector<int> goodp;

      for(p=0;p<temppool.size();p++)
      {
         if(temppool[p]!=NULL)
         {
            if(temppool[p]->alive)
            {
               if(attack[p]==maxattack)
               {
                  goodp.push_back(p);
               }
            }
         }
      }
      a=temppool[goodp[LCSrandom(goodp.size())]];

      maxattack+=temppool.size();

      maxattack+=business-cr->skillval(SKILL_BUSINESS);
      maxattack+=religion-cr->skillval(SKILL_RELIGION);
      maxattack+=science-cr->skillval(SKILL_SCIENCE);

      int aroll=maxattack+
                         LCSrandom(20)+1;
      int troll=cr->attval(ATTRIBUTE_WISDOM)*2-
                         cr->attval(ATTRIBUTE_HEART)+
                         LCSrandom(20)+1;

      bool techniques[9];
      //recall interrogation plan
      for(int i=0; i<9; i++)
      {
         techniques[i]=reinterpret_cast<interrogation*>(cr->activity.arg)->techniques[i];
      }

      while(1)
      {
         y=2;
         char num2[20];
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
         if(!techniques[TECHNIQUE_KILL])set_color(COLOR_WHITE,COLOR_BLACK,techniques[TECHNIQUE_NOSLEEP]);
         move(y,0);y+=1;addstr("D - ");
         if(!techniques[TECHNIQUE_NOSLEEP])addstr("No ");
         addstr("Sleep Deprivation   ");
         if(!techniques[TECHNIQUE_KILL])set_color(COLOR_WHITE,COLOR_BLACK,!techniques[TECHNIQUE_GIVEFOOD]);
         move(y,0);y+=1;addstr("E - ");
         if(techniques[TECHNIQUE_GIVEFOOD])addstr("Not ");
         addstr("Given Food    ");
         if(!techniques[TECHNIQUE_KILL])set_color(COLOR_WHITE,COLOR_BLACK,!techniques[TECHNIQUE_GIVEWATER]);
         move(y,0);y+=1;addstr("F - ");
         if(techniques[TECHNIQUE_GIVEWATER])addstr("Not ");
         addstr("Given Water    ");
         if(!techniques[TECHNIQUE_KILL])set_color(COLOR_WHITE,COLOR_BLACK,techniques[TECHNIQUE_PROPS]);
         move(y,0);addstr("G - ");
         if(!techniques[TECHNIQUE_PROPS])addstr("No ");
         addstr("Expensive Props     ");
         move(y,27);y+=1;
         addstr("($100)");
         if(!techniques[TECHNIQUE_KILL])set_color(COLOR_WHITE,COLOR_BLACK,techniques[TECHNIQUE_DRUGS]);
         move(y,0);addstr("H - ");
         if(!techniques[TECHNIQUE_DRUGS])addstr("No ");
         addstr("Hallucinogenic Drugs    ");
         move(y,28);y+=2;
         addstr("($50)");
         if(techniques[TECHNIQUE_KILL])set_color(COLOR_RED,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);y+=2;addstr("K - Kill the Hostage");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);y+=1;addstr("Press Enter to Confirm the Plan");

         y=4;
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
         itoa(cr->attval(ATTRIBUTE_HEART),num2,10);
         addstr("Heart: ");
         addstr(num2);
         move(++y,40);
         itoa(cr->attval(ATTRIBUTE_WISDOM),num2,10);
         addstr("Wisdom: ");
         addstr(num2);
         move(++y,40);
         itoa(cr->attval(ATTRIBUTE_HEALTH),num2,10);
         addstr("Health: ");
         addstr(num2);
         if(nofood)
         {
            move(++y,40);
            itoa(nofood,num2,10);
            addstr("No Food: ");
            addstr(num2);
            addstr(" days");
         }
         if(nowater)
         {
            move(++y,40);
            itoa(nowater,num2,10);
            addstr("No Water: ");
            addstr(num2);
            addstr(" days");
         }

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
         itoa(a->skillval(SKILL_PSYCHOLOGY),num2,10);
         addstr("Psychology Skill: ");
         addstr(num2);
         move(++y,40);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         itoa(a->attval(ATTRIBUTE_HEART),num2,10);
         addstr("Heart: ");
         addstr(num2);
         move(++y,40);
         itoa(a->attval(ATTRIBUTE_WISDOM),num2,10);
         addstr("Wisdom: ");
         addstr(num2);
         move(++y,40);
         itoa(a->attval(ATTRIBUTE_WISDOM),num2,10);
         addstr("Outfit: ");
         char str[40];
         getarmorfull(str,a->armor.type);
         addstr(str);
         move(y+=2,40);
         if(rapport[a->id]>3)
         {
            addstr("The Conservative clings helplessly ");
            move(++y,40);
            addstr("to ");
            addstr(a->name);
            addstr(" as its only friend.");
         } //XXX: What would 4, 2, 0, -2, -3, and/or -5 mean? (Some of these may not exist)
         else if(rapport[a->id]>1) //                   -- LK
         {
            addstr("The Conservative likes ");
            addstr(a->name);
            addstr(".");
         }
         else if(rapport[a->id]>0)
         {
            addstr("The Conservative is indifferent");
            move(++y,40);
            addstr("toward ");
            addstr(a->name);
            addstr(".");
         }
         else if(rapport[a->id]>-1)
         {
            addstr("The Conservative is uncooperative");
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
         

         
         int c=getch();
         translategetch(c);
         if(c>='a'&&c<='h')techniques[c-'a']=!techniques[c-'a'];
         if(c=='k')techniques[TECHNIQUE_KILL]=!techniques[TECHNIQUE_KILL];
         if(c==10)break;
      }

      if(techniques[TECHNIQUE_PROPS] && funds>=100)
      { funds-=100; moneylost_hostage+=100; }
      else { techniques[TECHNIQUE_PROPS] = 0; }
      if(techniques[TECHNIQUE_DRUGS] && funds>=50)
      { funds-=50; moneylost_hostage+=50; }
      else { techniques[TECHNIQUE_DRUGS] = 0; }

      //remember interrogation choices
      for(int i=0; i<9; i++)
      {
         reinterpret_cast<interrogation*>(cr->activity.arg)->techniques[i]=techniques[i];
      }

      if(techniques[TECHNIQUE_KILL]) // Kill the Hostage
      {
         erase();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(0,0);
         addstr("The Final Education of ");
         addstr(cr->name);
         addstr(": Day ");
         char num[20];
         itoa(cr->joindays,num,10);
         addstr(num);

         a=NULL;

         for(int i=0;i<temppool.size();++i)
         {
            if((int)LCSrandom(50)<temppool[i]->juice||
               LCSrandom(9)+1>=temppool[i]->attval(ATTRIBUTE_HEART,0))
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
            cr->alive=0;
            stat_kills++;
            move(y,0);y++;
            addstr(a->name);
            addstr(" executes ");
            addstr(cr->name);
            addstr(" by ");
            switch(LCSrandom(5))
            {
               case 0:addstr("strangling it to death.");break;
               case 1:addstr("beating it to death.");break;
               case 2:addstr("burning photos of Reagan in front of it.");break;
               case 3:addstr("telling it that taxes have been increased.");break;
               case 4:addstr("telling it its parents wanted to abort it.");break;
            }
            refresh();getch();
            if(LCSrandom(a->att[ATTRIBUTE_HEART])>LCSrandom(3))
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(y,0);y++;
               addstr(a->name);
               addstr(" feels sick to the stomach afterward and ");
               a->att[ATTRIBUTE_HEART]--;
               move(y,0);y++;
               switch(LCSrandom(4))
               {
                  case 0:addstr("throws up in a trash can.");break;
                  case 1:addstr("gets drunk, eventually falling asleep.");break;
                  case 2:addstr("curls up in a ball, crying softly.");break;
                  case 3:addstr("shoots up and collapses in a heap on the floor.");break;
               }
            }
            else if(!LCSrandom(3))
            {
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               move(y,0);y++;
               addstr(a->name);
               addstr(" grows colder.");
               a->att[ATTRIBUTE_WISDOM]++;
            }
         }
         else
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,0);
            move(y,0);y++;
            addstr("There is no one able to get up the nerve to");
            move(y,0);y++;
            addstr("execute ");
            addstr(cr->name);
            addstr(" in cold blood.");

            //Interrogation will continue as planned, with
            //these restrictions:
            techniques[TECHNIQUE_TALK]=0; //don't talk to them today
            techniques[TECHNIQUE_BEAT]=0; //don't beat them today
            techniques[TECHNIQUE_NOSLEEP]=0; //don't keep them from sleeping
            techniques[TECHNIQUE_DRUGS]=0; //don't administer drugs

            //Food, water, light, and restraint settings will be applied as
            //normal, of course, at the moment, light is not implemented
         }
         refresh();
         getch();

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(24,0);
         addstr("Press any key to reflect on this.");

         refresh();
         getch();

         if(cr->alive==0)
         {
            for(int p=0;p<pool.size();p++)
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
            delete[] attack;
            return;
         }
      }

      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("The Education of ");
      addstr(cr->name);
      addstr(": Day ");
      char num[20];
      itoa(cr->joindays,num,10);
      addstr(num);
      y=2;

      move(y,0);
      addstr("The Automaton");
      if(techniques[TECHNIQUE_RESTRAIN]) // Restraint
      {
         addstr(" is tied hands and feet to a metal chair");
         y++;move(y,0);
         addstr("in the middle of a back room.");
         y++;

         spiritcrush+=LCSrandom(5);
      }
      else
      {
         addstr(" is locked in a back room");
         y++;move(y,0);
         addstr("converted into a makeshift cell.");
         y++;
      }
      refresh();
      getch();

      move(y,0);
      if(techniques[TECHNIQUE_GIVEFOOD]) // no food
      {
         if(techniques[TECHNIQUE_GIVEWATER]) // no food AND no water
         {
            addstr("It is left to hunger and thirst like a lost animal.");
            spiritcrush+=LCSrandom(2*(nowater+1)+1);
            healthcrush+=LCSrandom(3*(nowater+1)+1)+1;
            if(nowater++>2)cr->att[ATTRIBUTE_HEALTH]--;
         }
         else
         {
            addstr("It is forced to beg for food and water, and never receives any food.");
            if(nowater>3)cr->att[ATTRIBUTE_HEALTH]++;
            nowater=0;
         }
         y++;
         spiritcrush+=LCSrandom(2*(nofood+1)+1);
         healthcrush+=LCSrandom(2*(nofood+1)+1)+1;
         if(nofood++>5)cr->att[ATTRIBUTE_HEALTH]--;
         refresh();
         getch();
      }
      else if(techniques[TECHNIQUE_GIVEWATER])
      {
         addstr("The only water it recieves comes in the scraps of food provided to sustain it.");
         spiritcrush+=LCSrandom(2*(nowater+1));
         healthcrush+=LCSrandom(3*(nowater+1))+1;
         if(nowater++>2)cr->att[ATTRIBUTE_HEALTH]--;
         if(nofood>6)cr->att[ATTRIBUTE_HEALTH]++;
         nofood=0;

         y++;
         refresh();
         getch();
      }
      else 
      {
         if(nowater>3)cr->att[ATTRIBUTE_HEALTH]++;
         if(nofood>6)cr->att[ATTRIBUTE_HEALTH]++;
         cr->blood+=nofood+nowater;
         nofood=0;
         nowater=0;
      }

      if(techniques[TECHNIQUE_NOSLEEP]&&!turned&&cr->alive) // No sleep
      {
         move(y,0);
         addstr("It is forced to ");
         switch(LCSrandom(2+techniques[TECHNIQUE_PROPS]*2))
         {
         case 0:addstr("listen to protest music");break;
         case 1:addstr("listen to public radio");break;
         case 2:addstr("watch network news tapes");break;
         case 3:addstr("watch revealing documentaries");break;
         }
         addstr(" in lieu of sleep.");
         y++;

         spiritcrush+=LCSrandom(3*(++nosleep));
         healthcrush+=2*nosleep;

         refresh();
         getch();
      }
      else
      {
         nosleep=0;
      }
      
      if(techniques[TECHNIQUE_DRUGS]) // Hallucinogenic drugs
      {
         move(y,0);
         
         y++;

         addstr("It is subjected to dangerous hallucinogens.");

         spiritcrush+=LCSrandom(5)+a->armor.interrogation_drugbonus();
         healthcrush+=3;

         //Possible permanent health damage
         if(LCSrandom(50)<++druguse)cr->att[ATTRIBUTE_HEALTH]--;
         
         if(cr->att[ATTRIBUTE_HEALTH]==0)
         {
            y++;
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            refresh();
            getch();
            move(y++,0);
            addstr("It is a lethal overdose in ");
            addstr(cr->name);
            addstr("'s weakened state.");
            cr->alive=0;
         }

         refresh();
         getch();
      }
      if(cr->att[ATTRIBUTE_HEALTH]<=0)cr->alive=0;

      if(techniques[TECHNIQUE_BEAT]&&!turned&&cr->alive) // Beating
      {
         y+=1;
         move(y,0);

         int forceroll=0;

         for(int i=0;i<temppool.size();i++)
         {
            //add interrogator's strength to beating strength
            forceroll+=temppool[i]->attval(ATTRIBUTE_STRENGTH);
            //reduce rapport with each interrogator
            rapport[temppool[i]->id]-=0.4f;
         }

         forceroll=LCSrandom(forceroll)+1;
         
         //Torture captive if lead interrogator has low heart
         //and you funded using extra supplies
         //
         //Yeah, you kinda don't want this to happen
         if(a->attval(ATTRIBUTE_HEART)==1&&techniques[TECHNIQUE_PROPS])
         {
            //Torture more devastating than normal beating
            forceroll*=2;
            //Extremely bad for rapport with lead interrogator
            rapport[a->id]-=3;

            addstr(a->name);
            switch(LCSrandom(6))
            {
            case 0:addstr(" recreates scenes from Abu Ghraib");break;//lol, sodomy
            case 1:addstr(" whips the Automaton with a steel cable");break;
            case 2:addstr(" holds the hostage's head under water");break;
            case 3:addstr(" peels back the Automaton's fingernails");break;//XXX: but shouldn't this only happen once?
            case 4:addstr(" beats the hostage with a metal bat");break;
            case 5:addstr(" beats the hostage with a belt");break;
//            case 6:addstr(" ");break;
            }
            addstr(",");
            y++;
            move(y,0);
            addstr("screaming \"");
            int i=0;
            while(i<2)
            {
               switch(LCSrandom(10))
               {
               case 0:addstr("I hate you");break;
               case 1:addstr("Does it hurt?");break;
               case 2:addstr("Nobody loves you");break;
               case 3:addstr("God hates you");break;
               case 4:
                      if(law[LAW_FREESPEECH]==-2)addstr("Don't [mess] with me");
                      else addstr("Don't fuck with me");break;
               case 5:addstr("This is Liberalism");break;
               case 6:
                      if(law[LAW_FREESPEECH]==-2)addstr("Convert, [you]");
                      else addstr("Convert, bitch");break;
               case 7:addstr("I'm going to kill you");break;
               case 8:addstr("Do you love me?");break;
               case 9:addstr("I am your God");break;
               }
               if(++i<2)
                  addstr("! ");
            }
            addstr("!\" in its face.");
            if(a->attval(ATTRIBUTE_HEART)>1)a->att[ATTRIBUTE_HEART]--;
            if(cr->attval(ATTRIBUTE_HEART)>1)cr->att[ATTRIBUTE_HEART]--;
         }
         else
         {
            if(temppool.size()==1)
            {
               addstr(temppool[0]->name);
               addstr(" beats");
            }
            else if(temppool.size()==2)
            {
               addstr(temppool[0]->name);
               addstr(" and ");
               addstr(temppool[1]->name);
               addstr(" beat");
            }
            else
            {
               addstr(cr->name);
               addstr("'s guards beat");
            }
            addstr(" the Automaton");
            if(techniques[TECHNIQUE_PROPS])//props
            {
               switch(LCSrandom(5))
               {
               case 0:addstr(" with a flagpole");break;
               case 1:addstr(" with a flag");break;
               case 2:addstr(" with a bible");break;//XXX: Effect on religion/science? XXX: Maybe if(law[LAW_RELIGION]==-2) it's *the* bible?
               case 3:
                      if(law[LAW_FREESPEECH]==-2)addstr(" with a [pink rod]");// Pervert!
                      else addstr(" with a dildo");break;
               case 4:addstr(" with a book");break;
               }
            }
            addstr(",");
            y++;
            move(y,0);
            switch(LCSrandom(4))
            {
            case 0:addstr("scream");break;
            case 1:addstr("yell");break;
            case 2:addstr("shout");break;
            case 3:addstr("holler");break;
//            case 4:addstr("");break;
            }
            addstr("ing \"");
            int i=0;
            while(i<3)
            {
               switch(LCSrandom(23))
               {
               case 0:addstr("McDonalds");break;
               case 1:addstr("Microsoft");break;
               case 2:addstr("Bill Gates");break;
               case 3:addstr("Wal-Mart");break;
               case 4:addstr("George Bush");break;//XXX: Dubya?
               case 5:addstr("ExxonMobil");break;
               case 6:addstr("Trickle-down economics");break;
               case 7:addstr("Family values");break;
               case 8:addstr("Conservatism");break;
               case 9:addstr("War on Drugs");break;
               case 10:addstr("War on Terror");break;
               case 11:addstr("Ronald Reagan");break;
               case 12:addstr("Rush Limbaugh");break;
               case 13:addstr("Tax cuts");break;
               case 14:addstr("Military spending");break;
               case 15:addstr("Ann Coulter");break;
               case 16:addstr("Deregulation");break;
               case 17:addstr("Gang Violence");break;
               case 18:addstr("Police Brutality");break;
               case 19:addstr("Corporate Corruption");break;
               case 20:addstr("Wiretapping");break;
               case 22:addstr("Reaganomics");break;

               }
               if(++i<3)
                  addstr("! ");
            }
            addstr("!\" in its face.");
         }            }
         y++;

         spiritcrush+=forceroll/2+a->armor.interrogation_assaultbonus();
         if(techniques[TECHNIQUE_PROPS])spiritcrush+=forceroll/2;
         healthcrush+=forceroll/2;
         cr->blood-=forceroll/2;

         refresh();
         getch();

         if(forceroll>=cr->attval(ATTRIBUTE_HEALTH))
         {
            if(cr->skillval(SKILL_RELIGION)>spiritcrush)
            {
               move(y,0);
               addstr(cr->name);
               if(!techniques[TECHNIQUE_DRUGS])
               {
                  switch(LCSrandom(2))
                  {
                  case 0:addstr(" prays...");break;
                  case 1:addstr(" cries out for God.");break;
                  }
               }
               else
               {
                  switch(LCSrandom(2))
                  {
                  case 0:addstr(" takes solace in the personal appearance of God.");break;
                  case 1:addstr(" appears to be having a religious experience.");break;
                  }
               }
               y++;
            }
            else if(signed(spiritcrush+healthcrush) >
               cr->att[ATTRIBUTE_WISDOM]*3+cr->att[ATTRIBUTE_HEART]*3+cr->att[ATTRIBUTE_HEALTH]*3)
            {
               move(y++,0);
               addstr(cr->name);
               switch(LCSrandom(4))
               {
               case 0:addstr(" screams helplessly for ");
                  if(techniques[TECHNIQUE_DRUGS])addstr("John Lennon's mercy.");
                  else if(cr->skillval(SKILL_RELIGION))addstr("God's mercy.");
                  else addstr("mommy.");
                  break;
               case 1:
                  if(techniques[TECHNIQUE_RESTRAIN])addstr(" goes limp in the restraints.");
                  else addstr(" curls up in the corner and doesn't move.");break;
               case 2:
                  if(techniques[TECHNIQUE_DRUGS] && !LCSrandom(5))addstr(" barks helplessly.");
                  else addstr(" cries helplessly.");break;
               case 3:
                  if(techniques[TECHNIQUE_DRUGS] && !LCSrandom(3))addstr(" wonders about apples.");
                  else addstr(" wonders about death.");
                  break;
               }
               if(cr->att[ATTRIBUTE_HEART]>1)cr->att[ATTRIBUTE_HEART]--;
               if(location[cr->worklocation]->interrogated==0 && !LCSrandom(5))
               {
                  refresh();
                  getch();
                  move(y++,0);
                  addstr(a->name);
                  addstr(" beats information out of the pathetic thing.");
                  move(y++,0);
                  refresh();
                  getch();
                  if(location[cr->worklocation]->type<=SITE_RESIDENTIAL_SHELTER)
                  {
                     addstr("Unfortunately, none of it is useful to the LCS.");
                  }
                  else
                  {
                     addstr("A detailed map has been created of ");
                     addstr(location[cr->worklocation]->name);
                     addstr(".");
                  }
                  location[cr->worklocation]->interrogated=1;
                  location[cr->worklocation]->hidden=0;
               }
            }
            else 
            {
               move(y,0);
               addstr(cr->name);
               addstr(" seems to be getting the message.");
               y++;
               if(cr->juice>1)
               {
                  cr->juice-=forceroll;
                  if(cr->juice<0)
                     cr->juice=0;
               }
               else if(cr->att[ATTRIBUTE_WISDOM]>1)
               {
                  cr->att[ATTRIBUTE_WISDOM]-=forceroll/10+1;
                  if(cr->att[ATTRIBUTE_WISDOM]<1)cr->att[ATTRIBUTE_WISDOM]=1;
               }
            }

            if(forceroll>=LCSrandom(cr->attval(ATTRIBUTE_HEALTH)*5+1))
            {
               refresh();
               getch();

               move(y,0);
               if(--cr->att[ATTRIBUTE_HEALTH]>0)
               {
                  addstr(cr->name);
                  addstr(" is badly hurt.");
               }
               else
               {
                  addstr(cr->name);
                  addstr("'s weakened body crumbles under the brutal assault.");
                  cr->alive=0;
               }
               y++;
            }
         }
         else
         {
            move(y,0);
            addstr(cr->name);
            addstr(" takes it well.");
            y++;
         }
         getch();
      }

      // Verbal Interrogation
      if(techniques[TECHNIQUE_TALK]&&cr->alive)
      {
         float rapport_temp = rapport[a->id];
         rapport_temp += static_cast<float>(a->attval(ATTRIBUTE_CHARISMA)+a->skillval(SKILL_PSYCHOLOGY)-5)/10.0f;
         if(techniques[TECHNIQUE_RESTRAIN])rapport_temp -= 1;
         y+=1;
         move(y,0);
         addstr(a->name);
         if(techniques[TECHNIQUE_PROPS])//props
         {
            switch(LCSrandom(9))
            {
            case 0:addstr(" plays violent video games with ");break;
            case 1:addstr(" reads Origin of the Species to ");break;
            case 2:addstr(" burns flags in front of ");break;
            case 3:addstr(" explores an elaborate political fantasy with ");break;
            case 4:addstr(" watches controversial avant-garde films with ");break;
            case 5:addstr(" watches the anime film Bible Black with ");break;//XXX: Wasn't this basically a porno?
            case 6:addstr(" watches a documentary about Emmett Till with ");break;
            case 7:addstr(" watches Michael Moore films with ");break;
            case 8:addstr(" listens to Liberal radio shows with ");break;
            }
         }
         else
         {
            char str[75];
            switch(LCSrandom(4))
            {
            case 0:addstr(" talks about ");
                   getviewsmall(str,LCSrandom(VIEWNUM-3));
                   addstr(str);
                   addstr(" with ");break;
            case 1:addstr(" argues about ");
                   getviewsmall(str,LCSrandom(VIEWNUM-3));
                   addstr(str);
                   addstr(" with ");break;
            case 2:addstr(" tries to expose the true Liberal side of ");break;
            case 3:addstr(" attempts to recruit ");break;
            }
         }
         addstr(cr->name);
         addstr(".");
         y++;

         //Hallucinogenic drugs:
         //Re-interprets lead interrogator
         if(techniques[TECHNIQUE_DRUGS])
         {
            refresh();
            getch();

            move(y++,0);
            if(cr->skillval(SKILL_PSYCHOLOGY)*3>spiritcrush)
            {
               switch(LCSrandom(4))
               {
               case 0:addstr(cr->name);
                     addstr(" takes the drug-induced hallucinations with stoicism.");
                     break;
               case 1:addstr(cr->name);
                     addstr(" mutters its initials over and over again.");
                     break;
               case 2:addstr(cr->name);
                     addstr(" babbles continuous numerical sequences.");
                     break;
               case 3:addstr(cr->name);
                     addstr(" manages to remain grounded through the hallucinations.");
                     break;
               }
            }
            else if((rapport[a->id]>1 && !LCSrandom(3)) || !LCSrandom(10))
            {
               rapport_temp=5;
               switch(LCSrandom(4))
               {
               case 0:
                  addstr(cr->name);
                  addstr(" hallucinates and sees ");
                  addstr(a->name);
                  addstr(" as an angel.");
                  break;
               case 1:
                  addstr(cr->name);
                  addstr(" realizes with joy that ");
                  addstr(a->name);
                  addstr(" is Ronald Reagan.");
                  break;
               case 2:
                  addstr(cr->name);
                  addstr(" stammers and ");
                  techniques[TECHNIQUE_RESTRAIN] ? addstr("talks about hugging ") : addstr("hugs ");
                  addstr(a->name);
                  addstr(".");
                  break;
               case 3:
                  addstr(cr->name);
                  addstr(" salutes ");
                  addstr(a->name);
                  addstr(" as the Knight Guardian of Humanity.");
                  break;
               }
            }
            else if((rapport[a->id]<-1 && LCSrandom(3)) || !LCSrandom(5))
            {
               rapport_temp=-5;
               switch(LCSrandom(4))
               {
               case 0:
                  addstr(cr->name);
                  addstr(" shouts in numb terror at the sight of ");
                  addstr(a->name);
                  addstr(".");
                  break;
               case 1:
                  addstr(cr->name);
                  if(!techniques[TECHNIQUE_RESTRAIN])addstr(" curls into a ball and");
                  addstr(" squeals in fear.");
                  break;
               case 2:
                  addstr(cr->name);
                  addstr(" watches ");
                  addstr(a->name);
                  addstr(" shift from one monstrous form to another.");
                  break;
               case 3:
                  addstr(cr->name);
                  addstr(" gasps and demands that ");
                  //addstr("Hitler tell them where ");
                  //addstr(a->name);
                  //addstr(" went.");
                  addstr(a->name);
                  addstr(" stop looking like Hitler.");
                  break;
               }
            }
            else
            {
               rapport_temp+=0.5f*(LCSrandom(3)-1);
               switch(LCSrandom(4))
               {
               case 0:
                  addstr(cr->name);
                  addstr(" comments on the shimmering clothing ");
                  addstr(a->name);
                  addstr(" is wearing.");
                  break;
               case 1:
                  addstr(cr->name);
                  addstr(" can't stop looking at the movie playing on the door.");
                  break;
               case 2:
                  addstr(cr->name);
                  addstr(" laughs hysterically at the sight of ");
                  addstr(a->name);
                  addstr(".");
                  break;
               case 3:
                  addstr(cr->name);
                  addstr(" barks like a dog.");
                  break;
               }
            }
         }

         refresh();
         getch();

         if(troll<aroll&&troll+cr->skillval(SKILL_PSYCHOLOGY)*2>aroll)
         {
            move(y,0);
            switch(LCSrandom(4))
            {
            case 0:addstr(cr->name);
                  addstr(" plays mind games with ");
                  addstr(a->name);
                  addstr(".");
                  break;
            case 1:addstr(cr->name);
                  addstr(" knows how this works, and won't budge.");
                  break;
            case 2:addstr(cr->name);
                  addstr(" asks if Liberal mothers would approve of this.");
                  break;
            case 3:addstr(cr->name);
                  addstr(" seems resistant to this form of interrogation.");
                  break;
            }
            y++;
         }
         //Failure to break religious convictions
         else if(cr->skillval(SKILL_RELIGION)>religion+spiritcrush && !techniques[TECHNIQUE_DRUGS])
         {
            move(y,0);
            switch(LCSrandom(4))
            {
            case 0:addstr(a->name);
                  addstr(" is unable to shake ");
                  addstr(cr->name);
                  addstr("'s religious conviction.");
                  break;
            case 1:addstr(cr->name);
                  addstr(" will never be broken so long as God grants it strength.");
                  break;
            case 2:addstr(a->name);
                  addstr("'s efforts to question ");
                  addstr(cr->name);
                  addstr("'s faith seem futile.");
                  break;
            case 3:addstr(cr->name);
                  addstr(" calmly explains the Conservative tenets of its faith.");
                  break;
            }
            
            a->train(SKILL_RELIGION,cr->skillval(SKILL_RELIGION)*4);
            y++;
         }
         //Failure to persuade entrenched capitalists
         else if(cr->skillval(SKILL_BUSINESS)>business+spiritcrush && !techniques[TECHNIQUE_DRUGS])
         {
            move(y,0);
            switch(LCSrandom(4))
            {
            case 0:addstr(cr->name);
                  addstr(" will never be moved by ");
                  addstr(a->name);
                  addstr("'s pathetic economic ideals.");
                  break;
            case 1:addstr(cr->name);
                  addstr(" is pretty sure LCS actions hurt businesses.");
                  break;
            case 2:addstr(cr->name);
                  addstr(" explains to ");
                  addstr(a->name);
                  addstr(" why communism failed.");
                  break;
            case 3:addstr(cr->name);
                  addstr(" mumbles incoherently about Reaganomics.");
                  break;
            }
            
            a->train(SKILL_BUSINESS,cr->skillval(SKILL_BUSINESS)*4);
            y++;
         }
         //Failure to persuade scientific minds
         else if(cr->skillval(SKILL_SCIENCE)>science+spiritcrush && !techniques[TECHNIQUE_DRUGS])
         {
            move(y,0);
            switch(LCSrandom(4))
            {
            case 0:addstr(cr->name);
                  addstr(" wonders what mental disease has possessed ");
                  addstr(a->name);
                  addstr(".");
                  break;
            case 1:addstr(cr->name);
                  addstr(" explains why nuclear energy is safe.");
                  break;
            case 2:addstr(cr->name);
                  addstr(" makes Albert Einstein faces at ");
                  addstr(a->name);
                  addstr(".");
                  break;
            case 3:addstr(cr->name);
                  addstr(" pities ");
                  addstr(a->name);
                  addstr("'s blind ignorance of science.");
                  break;
            }

            a->train(SKILL_SCIENCE,cr->skillval(SKILL_SCIENCE)*4);
            y++;
         }
         //Attempt to convert when the target is brutally treated will
         //just alienate them and make them cynical - targets with poor
         //health and few convictions are more suceptible to succumbing
         //to this, also targets who have been held a long time are
         //also likely to do this, or if you're interrogating with someone
         //who has frequently abused this target in the past
         else if((int)(LCSrandom(spiritcrush*2+1)+LCSrandom(healthcrush*2+1)) >
            cr->attval(ATTRIBUTE_WISDOM)+cr->attval(ATTRIBUTE_HEART)+cr->attval(ATTRIBUTE_HEALTH))
         {
            move(y,0);
            addstr(cr->name);
            switch(LCSrandom(7))
            {
            case 0:addstr(" babbles mindlessly.");break;
            case 1:addstr(" just whimpers.");break;
            case 2:addstr(" cries helplessly.");break;
            case 3:addstr(" is losing faith in the world.");break;
            case 4:addstr(" only grows more distant.");break;
            case 5:addstr(" is too terrified to even speak to ");addstr(a->name);addstr(".");break;
            case 6:addstr(" just hates the LCS even more.");break;
            }
            if(a->attval(ATTRIBUTE_HEART)+
               a->skillval(SKILL_PSYCHOLOGY)+
               a->attval(ATTRIBUTE_CHARISMA)>16+LCSrandom(10))
            {
               getch();
               move(++y,0);
               addstr(a->name);
               switch(LCSrandom(7))
               {
               case 0:addstr(" consoles the Conservative automaton.");break;
               case 1:addstr(" shares some chocolates.");break;
               case 2:addstr(" provides a shoulder to cry on.");break;
               case 3:addstr(" understands ");addstr(cr->name);addstr("'s pain.");break;
               case 4:addstr("'s heart opens to the poor Conservative.");break;
               case 5:addstr(" helps the poor thing to come to terms with captivity.");break;
               case 6:addstr("'s patience and kindness leaves the Conservative confused.");break;
               }
               rapport[a->id]+=0.7f;
               if(rapport[a->id]>3)
               {
                  getch();
                  move(++y,0);
                  addstr(cr->name);
                  switch(LCSrandom(7))
                  {
                  case 0:addstr(" emotionally clings to ");
                     addstr(a->name);
                     addstr("'s sympathy.");break;
                  case 1:addstr(" begs ");
                     addstr(a->name);
                     addstr(" for help.");break;
                  case 2:addstr(" promises to be good.");break;
                  case 3:addstr(" reveals childhood pains.");break;
                  case 4:addstr(" thanks ");
                     addstr(a->name);
                     addstr(" for being merciful.");break;
                  case 5:addstr(" cries in ");
                     addstr(a->name);
                     addstr("'s arms.");break;
                  case 6:addstr(" really likes ");
                     addstr(a->name);
                     addstr(".");break;
                  }
                  
                  if(rapport[a->id]>5)turned=1;
               }
            }
            
            if(cr->att[ATTRIBUTE_HEART]>1)cr->att[ATTRIBUTE_HEART]--;
            y++;
         }
         //Target is swayed by Liberal Reason -- skilled interrogators, time held,
         //and rapport contribute to the likelihood of this
         else if(aroll*rapport_temp+spiritcrush>troll*2)
         {
            //Improve rapport with interrogator
            rapport[a->id]+=0.6f;

            if(cr->juice>0)
            {
               cr->juice-=aroll;
               if(cr->juice<0)cr->juice=0;
            }
            else
            {
               //Reduce wisdom!
               if(cr->attval(ATTRIBUTE_WISDOM)>a->attval(ATTRIBUTE_WISDOM))
               {
                  int change = LCSrandom(cr->attval(ATTRIBUTE_WISDOM)-a->attval(ATTRIBUTE_WISDOM)+1);
                  if(change > a->skillval(SKILL_PSYCHOLOGY)/2 + 1)change = a->skillval(SKILL_PSYCHOLOGY)/2 + 1;
                  cr->att[ATTRIBUTE_WISDOM]-=change;
               }
               //Increase heart
               if(cr->attval(ATTRIBUTE_HEART)<a->attval(ATTRIBUTE_HEART))
               {
                  int change = LCSrandom(a->attval(ATTRIBUTE_HEART)-cr->attval(ATTRIBUTE_HEART)+1);
                  if(change > a->skillval(SKILL_PSYCHOLOGY)/2 + 1)change = a->skillval(SKILL_PSYCHOLOGY)/2 + 1;
                  cr->att[ATTRIBUTE_HEART]+=change;
               }
            }

            //Join LCS??
            //1) They were liberalized
            if(cr->attval(ATTRIBUTE_HEART)>cr->attval(ATTRIBUTE_WISDOM)+4)turned=1;
            //2) They were befriended
            if(rapport[a->id]>4)turned=1;

            move(y,0);
            addstr(cr->name);
            switch(LCSrandom(5))
            {
            case 0:addstr("'s Conservative beliefs are shaken.");break;
            case 1:addstr(" quietly considers these ideas.");break;
            case 2:addstr(" is beginning to see Liberal reason.");break;
            case 3:addstr(" has a revelation of understanding.");break;
            case 4:addstr(" grudgingly admits sympathy for LCS ideals.");break;
            }
            y++;
            if(location[cr->worklocation]->interrogated==0 && !LCSrandom(5))
            {
               y++;
               move(y++,0);
               addstr(cr->name);
               addstr(" reveals details about the ");
               addstr(location[cr->worklocation]->name);
               addstr(".");
               move(y++,0);
               if(location[cr->worklocation]->type<=SITE_RESIDENTIAL_SHELTER)
               {
                  addstr("Unfortunately, none of it is useful to the LCS.");
               }
               else
               {
                  addstr(a->name);
                  addstr(" was able to create a map of the site with this information.");
               }
               location[cr->worklocation]->interrogated=1;
               location[cr->worklocation]->hidden=0;
            }
         }
         //Target is not sold on the LCS arguments and holds firm
         //This is the worst possible outcome if you use props
         else if(aroll+spiritcrush>(troll>>2)||a->attval(ATTRIBUTE_WISDOM)>=cr->attval(ATTRIBUTE_WISDOM)||
                  techniques[TECHNIQUE_PROPS])
         {
            //Not completely unproductive; builds rapport
            rapport[a->id]+=0.2f;

            move(y,0);
            addstr(cr->name);
            addstr(" holds firm.");
            y++;
         }
         //Target actually wins the argument so successfully that the Liberal
         //interrogator's convictions are the ones that are shaken
         else
         {
            //Consolation prize is that they end up liking the
            //liberal more
            rapport[a->id]+=0.5f;

            a->att[ATTRIBUTE_WISDOM]++;

            move(y,0);
            addstr(cr->name);
            addstr(" turns the tables on ");
            addstr(a->name);
            addstr("!");
            y++;

            refresh();
            getch();
      
            move(y,0);
            addstr(a->name);
            addstr(" has gained wisdom!");
            y++;
         }

         getch();
      }

      //Health degraded when brutally treated
      cr->blood-=2*healthcrush/cr->attval(ATTRIBUTE_HEALTH);
      //Resolve (and desire to join) weakens when brutally treated
      cr->att[ATTRIBUTE_WISDOM]-=spiritcrush>>(2+LCSrandom(3));
      if(cr->att[ATTRIBUTE_WISDOM]<1)cr->att[ATTRIBUTE_WISDOM]=1;
      cr->att[ATTRIBUTE_HEART]-=spiritcrush>>(3+LCSrandom(3));
      if(cr->att[ATTRIBUTE_HEART]<1)cr->att[ATTRIBUTE_HEART]=1;

      //Lead interrogator gets bonus experience
      if(!techniques[TECHNIQUE_KILL])a->train(SKILL_PSYCHOLOGY,spiritcrush);
      //Others also get experience
      for(int i=0;i<temppool.size();i++)
         temppool[i]->train(SKILL_PSYCHOLOGY,(spiritcrush>>1)+1);

      reinterpret_cast<interrogation*>(cr->activity.arg)->totalspiritcrush+=spiritcrush;



      //Possibly suicidal when heart is down to 1 and prisoner has already been
      //captive for a week without rescue
      if(cr->alive&&cr->att[ATTRIBUTE_HEART]==1&&LCSrandom(3)&&cr->joindays>6)
      {
         move(++y,0);
         
         //can't commit suicide if restrained
         if(LCSrandom(6)||techniques[TECHNIQUE_RESTRAIN])
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,0);
            addstr(cr->name);
            //can't cut self if restrained
            switch(LCSrandom(5-techniques[TECHNIQUE_RESTRAIN]))
            {
            case 0:addstr(" mutters about death.");break;
            case 1:addstr(" broods darkly.");break;
            case 2:addstr(" has lost hope of rescue.");break;
            case 3:addstr(" is making peace with God.");break;
            case 4:addstr(" is bleeding from self-inflicted wounds.");cr->blood-=25;break;//XXX: should be cr->blood-=LCSrandom(15)+10 or something... I mean, blood loss isn't the same *every* time.
            }
         }
         else
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr(cr->name);
            addstr(" has committed suicide.");
            cr->alive=0;
         }
         y++;
         refresh();
         getch();
      }

      //Death
      if(cr->alive==0||cr->blood<1)
      {
         //delete interrogation information
         delete reinterpret_cast<interrogation*>(cr->activity.arg);
         cr->alive=0;

         stat_kills++;
         y++;
         move(y,0);
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr(cr->name);
         addstr(" is dead");
         if(a)
         {
            addstr(" under ");
            addstr(a->name);
            addstr("'s interrogation.");
         }
         else
         {
            addstr(".");
         }
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         y++;
         refresh();
         getch();
         if(a)
         {
            if(LCSrandom(a->att[ATTRIBUTE_HEART]))
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(y,0);y++;
               addstr(a->name);
               addstr(" feels sick to the stomach afterward and ");
               a->att[ATTRIBUTE_HEART]--;
               move(y,0);y++;
               switch(LCSrandom(3))
               {
                  case 0:addstr("throws up in a trash can.");break;
                  case 1:addstr("gets drunk, eventually falling asleep.");break;
                  case 2:addstr("curls up in a ball, crying softly.");break;
               }
            }
            else if(!LCSrandom(3))
            {
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               move(y,0);y++;
               addstr(a->name);
               addstr(" grows colder.");
               a->att[ATTRIBUTE_WISDOM]++;
            }
         }
      }
      delete[] attack;
   }
   #ifdef AUTOENLIGHTEN
      turned=1;// Lucky!
   #endif

   if(turned&&cr->alive)
   {
      //delete interrogation information
      delete reinterpret_cast<interrogation*>(cr->activity.arg);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      y++;
      move(y,0);
      addstr("The Automaton has been Enlightened!   Your Liberal ranks are swelling!");
      if(cr->attval(ATTRIBUTE_HEART)>7 &&
         cr->attval(ATTRIBUTE_WISDOM)>2 &&
         !LCSrandom(4) && (cr->flag & CREATUREFLAG_KIDNAPPED))
      {
         move(++y,0);
         addstr("The conversion is convincing enough that the police no longer consider it a kidnapping.");
         //Actually liberalized -- they'll clean up the kidnapping story
         cr->flag&=~CREATUREFLAG_MISSING;
         cr->flag&=~CREATUREFLAG_KIDNAPPED;
      }
      cr->flag|=CREATUREFLAG_BRAINWASHED;

      for(int p=0;p<pool.size();p++)
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

      if(location[cr->worklocation]->interrogated==0 || location[cr->worklocation]->hidden==1)
      {
         move(y,0);
         addstr(cr->name);
         addstr(" reveals details about the ");
         addstr(location[cr->worklocation]->name);
         addstr(".");
         move(++y,0);
         if(location[cr->worklocation]->type<=SITE_RESIDENTIAL_SHELTER)
         {
            addstr("Unfortunately, none of it is useful to the LCS.");
         }
         else
         {
            addstr(a->name);
            addstr(" was able to create a map of the site with this information.");
         }
         location[cr->worklocation]->interrogated=1;
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
         addstr(cr->name);
         addstr("'s disappearance has not yet been reported.");
         y+=2;
         move(y++,0);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr("In what capacity will ");
         addstr(cr->name);
         addstr(" best serve the Liberal cause?");
         move(y++,0);
         addstr("1) Stay at ");
         addstr(location[cr->worklocation]->name);
         addstr(" as a ");
         set_color(COLOR_CYAN,COLOR_BLACK,0);
         addstr("sleeper agent");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr(".");
         move(y++,0);
         addstr("2) Come to ");
         addstr(location[a->location]->name);
         addstr(" as a ");
         set_color(COLOR_GREEN,COLOR_BLACK,0);
         addstr("regular member");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr(".");

         
         while(1)
         {
            char keystroke = getch();
            if(keystroke == '1')
            {
               cr->flag |= CREATUREFLAG_SLEEPER;
               liberalize(*cr,false);
               cr->location = cr->worklocation;
               location[cr->worklocation]->interrogated=1;
               location[cr->worklocation]->hidden=0;
               cr->base = cr->worklocation;
               break;
            }
            else if(keystroke == '2')
            {
               cr->location=a->location;
               cr->base=a->base;
               liberalize(*cr,false);
               break;
            }
         }

         cr->flag&=~CREATUREFLAG_MISSING;

         return;
      }
   }

   if(cr->align==1||!cr->alive)
   {
      for(int p=0;p<pool.size();p++)
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

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(24,0);
   addstr("Press any key to reflect on this.");

   refresh();
   getch();
}
