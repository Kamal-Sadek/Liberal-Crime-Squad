/*
 * Copyright (c) 2002,2003,2004 by Tarn Adams
 * Copytright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This file is part of Liberal Crime Squad.
 *
 * Liberal Crime Squad is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/*
 * This file was created by Chris Johnson (grundee@users.sourceforge.net)
 * by copying code from game.cpp into monthly/endgame.cpp.
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
#include "politics/politics.h"

void doActivitySolicitDonations(vector<Creature *> &solicit, char &clearformess);
void doActivitySellTshirts(vector<Creature *> &tshirts, char &clearformess);
void doActivitySellArt(vector<Creature *> &art, char &clearformess);
void doActivitySellMusic(vector<Creature *> &music, char &clearformess);
void doActivitySellBrownies(vector<Creature *> &brownies, char &clearformess);
void doActivityHacking(vector<Creature *> &hack, char &clearformess);
void doActivityGraffiti(vector<Creature *> &graffiti, char &clearformess);
void doActivityProstitution(vector<Creature *> &prostitutes, char &clearformess);
void doActivityLearn(vector<Creature *> &students, char &clearformess);
void doActivityTrouble(vector<Creature *> &trouble, char &clearformess);
void doActivityTeach(vector<Creature *> &teachers, char &clearformess);
void doActivityBury(vector<Creature *> &bury, char &clearformess);


void adjustblogpower(int &power)
{
   if(power<20)
   {
      switch(LCSrandom(20))
      {
      case 0:addstr("a sub-Liberal");break;
      case 1:addstr("a really bad");break;
      case 2:addstr("a pathetic");break;
      case 3:addstr("a dreadful");break;
      case 4:addstr("a god-awful");break;
      case 5:addstr("a Conservative");break;
      case 6:addstr("a heinous");break;
      case 7:addstr("an embarrassing");break;
      case 8:addstr("a shameful");break;
      case 9:addstr("a counter-productive");break;
      case 10:addstr("a sad");break;
      case 11:addstr("a vapid");break;
      case 12:addstr("a weak");break;
      case 13:addstr("a brainless");break;
      case 14:addstr("a repellant");break;
      case 15:addstr("a nonsensical");break;
      case 16:addstr("a ludicrous");break;
      case 17:addstr("a half-baked");break;
      case 18:addstr("a laughable");break;
      case 19:addstr("an insane");break;
      }
      power = -signed(LCSrandom(2));
   }
   else if(power<35)
   {
      //switch(LCSrandom(1))
      //{
      //case 0:addstr("a fair");break;
      //case 1:addstr("a mainstream");break;
      //case 2:addstr("a mediocre");break;
      //case 3:addstr("a middling");break;
      //case 4:addstr("a passable");break;
      //case 5:addstr("a regular");break;
      //case 6:addstr("a normal");break;
      //case 7:addstr("a standard");break;
      //case 9:addstr("a reasonable");break;
      //case 0:addstr("a typical");break;
      //case 11:addstr("a vanilla");break;
      //case 12:addstr("a basic");break;
      //case 13:addstr("a plain");break;
      //case 14:addstr("a simple");break;
      /*case 0:*/addstr("a standard");//break;
      //case 16:addstr("an everyday");break;
      //case 17:addstr("a stock");break;
      //case 18:addstr("a stereotypical");break;
      //case 19:addstr("an okay");break;
      //case 8:addstr("a respectable");break;
      //}
      power = 1;
   }
   else if(power<50)
   {
      switch(LCSrandom(11))
      {
      //case 0:addstr("a solid");break;
      case 0:addstr("a good");break;
      //case 2:addstr("an intelligent");break;
      case 1:addstr("a satisfying");break;
      //case 4:addstr("an impressive");break;
      case 2:addstr("a sound");break;
      //case 6:addstr("a splendid");break;
      case 3:addstr("a competent");break;
      case 4:addstr("a clever");break;
      //case 10:addstr("a skillful");break;
      //case 11:addstr("a talented");break;
      case 5:addstr("a persuasive");break;
      case 6:addstr("a thorough");break;
      case 7:addstr("a bold");break;
      //case 15:addstr("a clever");break;
      case 8:addstr("a fresh");break;
      case 9:addstr("a pointed");break;
      case 10:addstr("a sassy");break;
      //case 19:addstr("a sharp");break;
      case 11:addstr("a Liberal");break;
      }
      power = 2;
   }
   else
   {
      switch(LCSrandom(10))
      {
      case 0:addstr("a great");break;
      //case 1:addstr("a masterful");break;
      //case 2:addstr("a penetrating");break;
      //case 3:addstr("a profound");break;
      case 1:addstr("a top-quality");break;
      //case 5:addstr("a very sound");break;
      case 2:addstr("an excellent");break;
      //case 7:addstr("a competent");break;
      case 3:addstr("a brilliant");break;
      case 4:addstr("a powerful");break;
      //case 10:addstr("a slick");break;
      //case 11:addstr("a winning");break;
      case 5:addstr("an inspiring");break;
      case 6:addstr("a touching");break;
      case 7:addstr("an eloquent");break;
      case 8:addstr("a forceful");break;
      case 9:addstr("a compelling");break;
      //case 17:addstr("an impelling");break;
      //case 18:addstr("a smooth");break;
      case 10:addstr("an Elite Liberal");break;
      }
      power = 3;
   }
}


/* armor repair */
void repairarmor(Creature &cr,char &clearformess)
{
   Armor *armor=NULL;
   Item *pile=NULL;
   vector<Item *> *pilelist=NULL;
   int pileindex = 0;

   // Clean yourself up first
   if(cr.get_armor().is_bloody() || cr.get_armor().is_damaged())
      armor=&cr.get_armor();
   else if(cr.squadid!=-1)
   {
      int sq=getsquad(cr.squadid);
      for(int l=0;l<len(squad[sq]->loot);l++)
         if(squad[sq]->loot[l]->is_armor())
         {
            Armor* a = static_cast<Armor*>(squad[sq]->loot[l]); //cast -XML
            if (a->is_bloody() || a->is_damaged())
            {
               armor=a;
               pile=squad[sq]->loot[l];
               pileindex=l;
               pilelist=&squad[sq]->loot;
               break;
            }
         }
   }
   // Multiple passes, to find the best item to work on
   bool dothis = false;
   for (int passnum=0; passnum < 3 && !dothis; passnum++)
      if(armor==NULL&&cr.location!=-1)
         for(int l=0;l<len(location[cr.location]->loot);l++)
            if(location[cr.location]->loot[l]->is_armor())
            {
               Armor* a = static_cast<Armor*>(location[cr.location]->loot[l]);//cast -XML
               switch (passnum)
               {
                  case 0: // Guaranteed to accomplish something
                     dothis = (a->is_bloody() && a->is_damaged());
                     break;
                  case 1: // Find something to clean if low skill, repair if high
                     dothis =  (a->is_bloody() && armor_makedifficulty(*a,&cr)>4)
                            || (a->is_damaged() && armor_makedifficulty(*a,&cr)<=4);
                     break;
                  case 2: // Anything that needs work
                     dothis = (a->is_bloody() || a->is_damaged());
                     break;
               }
               if(dothis)
               {
                  armor=a;
                  pile=location[cr.location]->loot[l];
                  pileindex=l;
                  pilelist=&location[cr.location]->loot;
                  break;
               }
            }

   if(clearformess) erase();
   else makedelimiter();

   if(armor==NULL)
   {
      move(8,1);
      addstr(cr.name, gamelog);
      switch (LCSrandom(4))
      {
         case 0: addstr(" tidies up the safehouse.", gamelog); break;
         case 1: addstr(" reorganizes the armor closet.", gamelog); break;
         case 2: addstr(" cleans the kitchen.", gamelog); break;
         case 3:
            addstr(" peruses some sewing magazines.", gamelog);
            cr.train(SKILL_TAILORING,1);
            break;
      }
      gamelog.nextMessage();

      getkey();
   }
   else
   {
      string armorname = armor->get_name();// Get name before we maybe destroy it
      bool repairfailed=false;
      bool qualityReduction=!LCSrandom(10);
      bool armorDestroyed=!armor->decrease_quality(0);

      if(armor->is_damaged())
      {
         long dif=armor_makedifficulty(*armor,&cr);
         dif>>=(armor->get_quality()-1);  // it is easy to patch up rags
         cr.train(SKILL_TAILORING,dif/2+1);

         if(LCSrandom(1+dif/2)) repairfailed=true;
      }else{
         repairfailed=true;
      }
      if (armorDestroyed)
         repairfailed = false;  // Its dead, Jim; stop trying to fix it
      if (repairfailed)
         qualityReduction = false; // Low skill repairers shredding your shirts seem too harsh

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);

      std::string result = "";
      result += cr.name;

      if (armorDestroyed)
      {
         set_color(COLOR_RED,COLOR_BLACK,1);
         result += " disposes of ";
      }else if(repairfailed && armor->is_bloody())
      {
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         result += " cleans ";
      }else if(repairfailed)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         result += " is working to repair ";
      }else
      {
         if(!qualityReduction)
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            result += " repairs ";
         }else
         {
            armorDestroyed = !armor->decrease_quality(1);
            if(armorDestroyed)
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               result += " finds there is no hope of repairing ";
            }else
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               result += " repairs what little can be fixed of ";
            }
         }
      }

      if (pile)
      {
         result += armor->aan();
      }
      else
         result += cr.hisher();

      if (armorDestroyed)
         result += " ruined";

      result += " " + armorname + ".";

      addstr(result,gamelog);
      gamelog.nextMessage();

      if(pile)
      {
         if(pile->get_number()>1)
         {
            Item *newpile=pile->split(pile->get_number()-1);
            pilelist->push_back(newpile);
         }
      }

      armor->set_bloody(false);
      if(!repairfailed)
      {
         armor->set_damaged(false);
      }
      if (armorDestroyed)
      {
         if (!pile) // repairer was wearing it
         {
            cr.strip(NULL);
         }
         else // scrap from stockpile
         {
            delete_and_remove(*pilelist, pileindex);
         }
      }

      getkey();
   }
}



/* armor manufacture */
void makearmor(Creature &cr,char &clearformess)
{
   int at=cr.activity.arg;

   int cost=armortype[at]->get_make_price();
   int hcost=(cost>>1)+1;
   int dif=armor_makedifficulty(*armortype[at],&cr);

   if(ledger.get_funds()<hcost)
   {
      if(clearformess) erase();
      else makedelimiter();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(cr.name, gamelog);
      addstr(" cannot afford material for clothing.", gamelog);
      gamelog.nextMessage();

      getkey();
      return;
   }
   else
   {
      char foundcloth=0;

      if(cr.squadid!=-1)
      {
         int sq=getsquad(cr.squadid);
         for(int l=0;l<len(squad[sq]->loot);l++)
            if(squad[sq]->loot[l]->is_loot()&&
               static_cast<Loot*>(squad[sq]->loot[l])->is_cloth()) //cast -XML
            {
               if(squad[sq]->loot[l]->get_number()==1)
                  delete_and_remove(squad[sq]->loot,l);
               else squad[sq]->loot[l]->decrease_number(1);
               foundcloth=1;
               break;
            }
      }
      if(!foundcloth) for(int l=0;l<len(location[cr.location]->loot);l++)
         if(location[cr.location]->loot[l]->is_loot()&&
            static_cast<Loot*>(location[cr.location]->loot[l])->is_cloth()) //cast -XML
         {
            if(location[cr.location]->loot[l]->get_number()==1)
               delete_and_remove(location[cr.location]->loot,l);
            else location[cr.location]->loot[l]->decrease_number(1);
            foundcloth=1;
            break;
         }

      if(!foundcloth&&ledger.get_funds()<cost)
      {
         if(clearformess) erase();
         else makedelimiter();

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);
         addstr(cr.name, gamelog);
         addstr(" cannot find enough cloth to reduce clothing costs.", gamelog);
         gamelog.nextMessage();

         getkey();
      }
      else
      {
         if(foundcloth) ledger.subtract_funds(hcost,EXPENSE_MANUFACTURE);
         else ledger.subtract_funds(cost,EXPENSE_MANUFACTURE);

         cr.train(SKILL_TAILORING,dif*2+1);

         int quality = 1;
         while (LCSrandom(10)<dif && quality <= armortype[at]->get_quality_levels())
            quality++;

         if(clearformess) erase();
         else makedelimiter();

         Item *it=new Armor(*armortype[at],quality);

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);
         addstr(cr.name, gamelog);
         if(quality <= ((Armor*)it)->get_quality_levels() )
         {
            addstr(" has made a ", gamelog);
            switch(quality)
            {
               case 1:addstr("first-rate", gamelog);break;
               case 2:addstr("second-rate", gamelog);break;
               case 3:addstr("third-rate", gamelog);break;
               case 4:addstr("fourth-rate", gamelog);break;
               default:addstr(quality,gamelog);addstr("th-rate", gamelog); break;
            }
            location[cr.location]->loot.push_back(it);
         }
         else
         {
            addstr(" wasted the materials for a", gamelog);
         }
         addstr(" ", gamelog);
         addstr(armortype[at]->get_name(), gamelog);
         addstr(".", gamelog);
         gamelog.nextMessage();

         getkey();
      }
   }
}



/* search for polls */
void survey(Creature *cr)
{
   music.play(MUSIC_ELECTIONS);
   static const char SURVEY_PAGE_SIZE=14;

   int v,creatureskill=cr->skill_roll(SKILL_COMPUTERS);
   int misschance=30-creatureskill,noise;
   if(misschance<5)misschance=5;
   if(creatureskill<1) noise=18+LCSrandom(3); // 18 to 20
   else if(creatureskill<2) noise=16+LCSrandom(2); // 16 or 17
   else if(creatureskill<3) noise=14+LCSrandom(2); // 14 or 15
   else if(creatureskill<4) noise=12+LCSrandom(2); // 12 or 13
   else if(creatureskill<5) noise=10+LCSrandom(2); // 10 or 11
   else if(creatureskill<6) noise=8+LCSrandom(2); // 8 or 9
   else if(creatureskill<7) noise=7;
   else if(creatureskill<9) noise=6;
   else if(creatureskill<11) noise=5;
   else if(creatureskill<14) noise=4;
   else if(creatureskill<18) noise=3;
   else noise=2;

   int survey[VIEWNUM];

   int maxview=-1;
   for(v=0;v<VIEWNUM;v++)
   {
      survey[v]=attitude[v];
      if(v!=VIEW_LIBERALCRIMESQUAD&&v!=VIEW_LIBERALCRIMESQUADPOS/*&&v!=VIEW_POLITICALVIOLENCE*/)
      {
         if(maxview!=-1) { if(public_interest[v]>public_interest[maxview]) maxview=v; }
         else { if(public_interest[v]>0) maxview=v; }
      }

      //MAKE SURVEY ACCURATE IF DEBUGGING
      #ifndef SHOWMECHANICS
      do { survey[v]+=LCSrandom(noise*2+1)-noise; } while(!LCSrandom(20));
      #endif

      if(survey[v]<0) survey[v]=0;
      if(survey[v]>100) survey[v]=100;

      #ifndef SHOWMECHANICS
      if(LCSrandom(public_interest[v]+100)<int(misschance)) survey[v]=-1;
      #endif

      if(v==VIEW_LIBERALCRIMESQUAD&&attitude[v]==0) survey[v]=-1;
      if(v==VIEW_LIBERALCRIMESQUADPOS&&survey[VIEW_LIBERALCRIMESQUAD]<=0) survey[v]=-1;
   }

   erase();

   //TODO: Sort out the gamelog for this.
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   addstr("Survey of Public Opinion, According to Recent Polls");

   int y=8,approval=presidentapproval();
   move(2,0);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   addstr(approval/10+(LCSrandom(noise*2+1)-noise), gamelog);
   addstr("% had a favorable opinion of ");
   set_alignment_color(exec[EXEC_PRESIDENT],true);
   addstr("President ");
   addstr(execname[EXEC_PRESIDENT]);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   addstr(".");

   //Top excitement issue
   if(maxview!=-1)
   {
      move(4,0);
      addstr("The people are most concerned about ");
      switch(maxview)
      {
         case VIEW_GAY:
            if(attitude[VIEW_GAY]>50) addstr("protecting gay rights.");
            else addstr("protecting the traditional family.");
            break;
         case VIEW_DEATHPENALTY:
            if (attitude[VIEW_DEATHPENALTY] > 50)
              addstr("the unjust death penalty.");
            else
            {
               if (law[LAW_DEATHPENALTY] == Alignment::ELITE_LIBERAL)
                 addstr("restoring the death penalty.");
               else
                 addstr("protecting the death penalty.");
            }
            break;
         case VIEW_TAXES:
            if(attitude[VIEW_TAXES]>50) addstr("the oppressive tax structure.");
            else addstr("the excessive tax burden.");
            break;
         case VIEW_NUCLEARPOWER:
            if (attitude[VIEW_NUCLEARPOWER] > 50)
              addstr("the dangers of nuclear power.");
            else
            {
               if (law[LAW_NUCLEARPOWER] == Alignment::ELITE_LIBERAL)
                 addstr("legalizing nuclear power.");
               else
                 addstr("threats to nuclear power.");
            }
            break;
         case VIEW_ANIMALRESEARCH:
            if(attitude[VIEW_ANIMALRESEARCH]>50) addstr("brutal animal research practices.");
            else addstr("excessive regulation of animal research.");
            break;
         case VIEW_POLICEBEHAVIOR:
            if(attitude[VIEW_POLICEBEHAVIOR]>50) addstr("preventing police brutality.");
            else addstr("expanding police powers.");
            break;
         case VIEW_INTELLIGENCE:
            if(attitude[VIEW_INTELLIGENCE]>50) addstr("civil liberties and personal privacy.");
            else addstr("national security and intelligence.");
            break;
         case VIEW_FREESPEECH:
            if(attitude[VIEW_FREESPEECH]>50) addstr("protecting free speech.");
            else addstr("ending hate speech.");
            break;
         case VIEW_GENETICS:
            if(attitude[VIEW_GENETICS]>50) addstr("the dangers of genetic engineering.");
            else addstr("excessive regulation of genetic research.");
            break;
         case VIEW_JUSTICES:
            if(attitude[VIEW_JUSTICES]>50) addstr("appointing proper Liberal justices.");
            else addstr("appointing proper Conservative justices.");
            break;
         case VIEW_SWEATSHOPS:
            if(attitude[VIEW_SWEATSHOPS]>50) addstr("threats to labor rights.");
            else addstr("corrupt union thugs.");
            break;
         case VIEW_POLLUTION:
            if(attitude[VIEW_POLLUTION]>50) addstr("threats to the environment.");
            else addstr("excessive regulation of industry.");
            break;
         case VIEW_CORPORATECULTURE:
            if(attitude[VIEW_CORPORATECULTURE]>50) addstr("corporate corruption.");
            else addstr("excessive regulation of corporations.");
            break;
         case VIEW_CEOSALARY:
            if(attitude[VIEW_CEOSALARY]>50) addstr("severe income inequality.");
            else addstr("resisting communist wage limits.");
            break;
         case VIEW_PRISONS:
            if(attitude[VIEW_PRISONS]>50) addstr("stopping the prisoners' suffering.");
            else addstr("putting the prisoners in line.");
            break;
         //case VIEW_POLITICALVIOLENCE:
         //   if(attitude[VIEW_POLITICALVIOLENCE]>50) addstr("taking strong action.");
         //   else addstr("political terrorism.");
         //   break;
         case VIEW_IMMIGRATION:
            if (attitude[VIEW_IMMIGRATION] > 50)
              addstr("immigrant rights.");
            else
            {
               if (law[LAW_IMMIGRATION] == Alignment::ELITE_LIBERAL)
                 addstr("wide-open immigration.");
               else
                 addstr("illegal immigration.");
            }
            break;
         case VIEW_DRUGS:
            if(attitude[VIEW_DRUGS]>50) addstr("drug rights.");
            else addstr("drug abuse.");
            break;
         case VIEW_WOMEN:
            if(attitude[VIEW_WOMEN]>50) addstr("women's equality.");
            else addstr("women.");
            break;
         case VIEW_CIVILRIGHTS:
            if(attitude[VIEW_CIVILRIGHTS]>50) addstr("civil rights.");
            else addstr("troublemaking minorities.");
            break;
         case VIEW_GUNCONTROL:
            if(attitude[VIEW_GUNCONTROL]>50) addstr("gun violence.");
            else addstr("protecting the Second Amendment.");
            break;
         case VIEW_MILITARY:
            if(attitude[VIEW_MILITARY]>50) addstr("military imperialism.");
            else addstr("defending the nation.");
            break;
         case VIEW_LIBERALCRIMESQUAD:
         case VIEW_LIBERALCRIMESQUADPOS:
            if(attitude[VIEW_LIBERALCRIMESQUAD]<50) addstr("activist political groups.");
            else
            {
               if(attitude[VIEW_LIBERALCRIMESQUADPOS]>50) addstr("the Liberal Crime Squad.");
               else addstr("the LCS terrorists.");
            }
            break;
         case VIEW_CONSERVATIVECRIMESQUAD:
            if(attitude[VIEW_CONSERVATIVECRIMESQUAD]<50) addstr("the Conservative Crime Squad.");
            else addstr("the CCS terrorists.");
            break;
         case VIEW_TORTURE:
            if(attitude[VIEW_TORTURE]>50)addstr("ending the use of torture.");
            else addstr("enhancing interrogations.");
            break;
         case VIEW_AMRADIO:
         case VIEW_CABLENEWS:
            if(attitude[VIEW_AMRADIO]+attitude[VIEW_CABLENEWS]>100) addstr("Conservative Media Bias.");
            else addstr("Liberal Media Bias.");
            break;
      }
   }
   else
   {
      move(4,0);
      addstr("The public is not concerned with politics right now.");
   }

   //Header for issue box
   move(6,0);
   addstr("Additional notable findings:");
   move(7,0);
   addstr("XX% Issue ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ Public Interest");

   //Footer
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(23,0);
   addstr("Results are +/- ");
   addstr(noise);
   addstr(" Liberal percentage points.");
   move(24,0);
   addstr("Enter - Done");
   move(24,40);
   addprevpagestr();
   addstr("    ");
   addnextpagestr();

   int page=0;
   const int maxpage=VIEWNUM/SURVEY_PAGE_SIZE;
   while(true)
   {
      //Keep pages within bounds
      if(page<0) page=maxpage;
      if(page>maxpage) page=0;
      //Start from the top
      y=8;
      //Draw each line
      for(v=page*SURVEY_PAGE_SIZE;v<(page+1)*SURVEY_PAGE_SIZE;v++,y++)
      {
         if(v>=VIEWNUM || (v==VIEW_CONSERVATIVECRIMESQUAD && (endgamestate>=ENDGAME_CCS_DEFEATED||newscherrybusted<2)))
         {
            move(y,0);
            addstr("                                                                                ");
            continue;
         }
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,4);
         addstr("........................................................");
         if(noise>=7||survey[v]==-1) addstr("Unknown  ");
         else if(noise>=4)
         {
            if(public_interest[v]>50) addstr("High     ");
            else addstr("Low      ");
         }
         else
         {
            if(public_interest[v]>100) addstr("Very High");
            else if(public_interest[v]>50) addstr("High     ");
            else if(public_interest[v]>10) addstr("Moderate ");
            else if(public_interest[v]) addstr("Low      ");
            else addstr("None     ");
         }

         if(survey[v]==-1)set_color(COLOR_BLACK,COLOR_BLACK,1);
         else if(survey[v]<10)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(survey[v]<30)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(survey[v]<50)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(survey[v]<70)set_color(COLOR_BLUE,COLOR_BLACK,1);
         else if(survey[v]<90)set_color(COLOR_CYAN,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);

         move(y,0);
         if(survey[v]==-1) addstr("??");
         else
         {
            if(survey[v]<10)addchar('0');
            addstr(survey[v]);
         }
         addstr("% ");

         switch(v)
         {
         case VIEW_GAY: addstr("were in favor of equal rights for homosexuals"); break;
         case VIEW_DEATHPENALTY: addstr("opposed the death penalty"); break;
         case VIEW_TAXES: addstr("were against cutting taxes"); break;
         case VIEW_NUCLEARPOWER: addstr("were terrified of nuclear power"); break;
         case VIEW_ANIMALRESEARCH: addstr("deplored animal research"); break;
         case VIEW_POLICEBEHAVIOR: addstr("were critical of the police"); break;
         case VIEW_TORTURE: addstr("wanted stronger measures to prevent torture"); break;
         case VIEW_INTELLIGENCE: addstr("thought the intelligence community invades privacy"); break;
         case VIEW_FREESPEECH: addstr("believed in unfettered free speech"); break;
         case VIEW_GENETICS: addstr("abhorred genetically altered food products"); break;
         case VIEW_JUSTICES: addstr("were for the appointment of Liberal justices"); break;
         case VIEW_SWEATSHOPS: addstr("would boycott companies that used sweatshops"); break;
         case VIEW_POLLUTION: addstr("thought industry should lower pollution"); break;
         case VIEW_CORPORATECULTURE: addstr("were disgusted by corporate malfeasance"); break;
         case VIEW_CEOSALARY: addstr("believed that CEO salaries are too great"); break;
         case VIEW_WOMEN: addstr("favored doing more for gender equality"); break;
         case VIEW_CIVILRIGHTS: addstr("felt more work was needed for racial equality"); break;
         case VIEW_GUNCONTROL: addstr("are concerned about gun violence"); break;
         case VIEW_DRUGS:
           if (law[LAW_DRUGS] == Alignment::LIBERAL
            || law[LAW_DRUGS] == Alignment::ELITE_LIBERAL)
             addstr("supported keeping marijuana legal");
           else
             addstr("believed in legalizing marijuana"); break;
         case VIEW_IMMIGRATION:
           if (law[LAW_IMMIGRATION] == Alignment::LIBERAL
            || law[LAW_IMMIGRATION] == Alignment::ELITE_LIBERAL)
             addstr("condemned unnecessary immigration regulations");
           else
             addstr("wanted amnesty for illegal immigrants"); break;
         case VIEW_MILITARY: addstr("opposed increasing military spending"); break;
         case VIEW_LIBERALCRIMESQUAD: addstr("respected the power of the Liberal Crime Squad"); break;
         case VIEW_LIBERALCRIMESQUADPOS: addstr("of these held the Liberal Crime Squad in high regard"); break;
         case VIEW_CONSERVATIVECRIMESQUAD: addstr("held the Conservative Crime Squad in contempt"); break;
         case VIEW_PRISONS: addstr("wanted to end prisoner abuse and torture"); break;
         case VIEW_AMRADIO: addstr("do not like AM radio"); break;
         case VIEW_CABLENEWS: addstr("have a negative opinion of cable news programs"); break;
         //case VIEW_POLITICALVIOLENCE: addstr("thought political violence was justified");break;
         }
      }

      while(true)
      {
         int c=getkey();

         if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) return;
         else if(c==interface_pgup||c==KEY_UP||c==KEY_LEFT) { page--; break; }
         else if(c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT) { page++; break; }
      }
   }


}

// Police accost your liberal!
void attemptarrest(Creature & liberal,const char* string,int clearformess)
{
   if(string)
   {
      if(clearformess) erase();
      else makedelimiter();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(liberal.name, gamelog);
      addstr(" is accosted by police while ", gamelog);
      addstr(string, gamelog);
      addstr("!", gamelog);
      gamelog.nextMessage();

      getkey();
   }

   // Chase sequence! Wee!
   makechasers(-1,5);

   if(!sitestory)
   {
      newsstoryst *ns=new newsstoryst;
      ns->type=NEWSSTORY_WANTEDARREST; // TODO: Make a more generic catch-all arrest story
      ns->loc=-1;
      newsstory.push_back(ns);
      sitestory=ns;
   }

   chaseseq.clean();
   chaseseq.location=location[liberal.location]->parent;
   footchase(liberal);
}

// While galavanting in public, your liberals may be ambushed by police
int checkforarrest(Creature & liberal,const char* string,int clearformess)
{
   bool arrest=false;

   if(!liberal.animalgloss && liberal.is_naked() && LCSrandom(2))
   {
      criminalize(liberal,LAWFLAG_DISTURBANCE);

      newsstoryst *ns=new newsstoryst;
      ns->type=NEWSSTORY_NUDITYARREST;
      ns->loc=-1;
      newsstory.push_back(ns);
      sitestory=ns;

      arrest=true;
   }
   else if(liberal.heat>liberal.get_skill(SKILL_STREETSENSE)*10)
   {
      if(!LCSrandom(50))
      {
         newsstoryst *ns=new newsstoryst;
         ns->type=NEWSSTORY_WANTEDARREST;
         ns->loc=-1;
         newsstory.push_back(ns);
         sitestory=ns;

         arrest=true;
      }
   }

   if(arrest) attemptarrest(liberal,string,clearformess);
   return arrest;
}



/* misc activation related things */
// *JDSRETURN*
void funds_and_trouble(char &clearformess)
{  //ACTIVITIES FOR INDIVIDUALS
   vector<Creature *> trouble,hack,bury,solicit,tshirts,art,music,graffiti,brownies,prostitutes,teachers,students;

   for(int p=0;p<len(pool);p++)
   {
      if(!pool[p]->alive) continue;
      if(pool[p]->location==-1)
      {
         pool[p]->activity.type=ACTIVITY_NONE;
         continue;
      }
      switch(pool[p]->activity.type)
      {
      case ACTIVITY_TEACH_FIGHTING:
      case ACTIVITY_TEACH_POLITICS:
      case ACTIVITY_TEACH_COVERT:
         teachers.push_back(pool[p]);
         break;
      case ACTIVITY_CCFRAUD:
      case ACTIVITY_DOS_RACKET:
      case ACTIVITY_DOS_ATTACKS:
      case ACTIVITY_HACKING:
         hack.push_back(pool[p]);
         break;
      case ACTIVITY_GRAFFITI:
         graffiti.push_back(pool[p]);
         break;
      case ACTIVITY_TROUBLE:
         trouble.push_back(pool[p]);
         break;
      case ACTIVITY_COMMUNITYSERVICE:
         addjuice(*pool[p],1,10);
         change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,1,0,80);
         break;
      case ACTIVITY_SELL_TSHIRTS:
         tshirts.push_back(pool[p]);
         break;
      case ACTIVITY_SELL_ART:
         art.push_back(pool[p]);
         break;
      case ACTIVITY_SELL_MUSIC:
         music.push_back(pool[p]);
         break;
      case ACTIVITY_DONATIONS:
         solicit.push_back(pool[p]);
         break;
      case ACTIVITY_SELL_DRUGS:
         brownies.push_back(pool[p]);
         break;
      case ACTIVITY_PROSTITUTION:
         prostitutes.push_back(pool[p]);
         break;
      case ACTIVITY_BURY:
         bury.push_back(pool[p]);
         pool[p]->activity.type=ACTIVITY_NONE;
         break;
      case ACTIVITY_CLINIC:
         hospitalize(find_clinic(*pool[p]),*pool[p]);
         pool[p]->activity.type=ACTIVITY_NONE;
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
      case ACTIVITY_STUDY_TEACHING:
      case ACTIVITY_STUDY_MUSIC:
      case ACTIVITY_STUDY_WRITING:
      case ACTIVITY_STUDY_LOCKSMITHING:
      case ACTIVITY_STUDY_COMPUTERS:
         students.push_back(pool[p]);
         break;
      case ACTIVITY_SLEEPER_JOINLCS:
         if(!location[find_homeless_shelter(*pool[p])]->siege.siege)
         {
            pool[p]->activity.type=ACTIVITY_NONE;
            pool[p]->flag &= ~CREATUREFLAG_SLEEPER;
            pool[p]->location = pool[p]->base = find_homeless_shelter(*pool[p]);
         }
         //Letters to the editor
      case ACTIVITY_WRITE_LETTERS:
         if(pool[p]->skill_check(SKILL_WRITING,DIFFICULTY_EASY))
            background_liberal_influence[randomissue()]+=5;
         pool[p]->train(SKILL_WRITING,LCSrandom(5)+1);
         break;
      //Guardian Essays
      //Basically letters to the editor, but thrice as potent, and can backfire
      case ACTIVITY_WRITE_GUARDIAN:
         if(pool[p]->skill_check(SKILL_WRITING,DIFFICULTY_EASY))
            background_liberal_influence[randomissue()]+=15;
         else
            background_liberal_influence[randomissue()]-=15;
         pool[p]->train(SKILL_WRITING,LCSrandom(5)+1);
         break;
      }
   }

	doActivitySolicitDonations(solicit, clearformess);
	doActivitySellTshirts(tshirts, clearformess);
	doActivitySellArt(art, clearformess);
	doActivitySellMusic(music, clearformess);
	doActivitySellBrownies(brownies, clearformess);
  	doActivityHacking(hack, clearformess);
	doActivityGraffiti(graffiti, clearformess);
  	doActivityProstitution(prostitutes, clearformess);
  	doActivityLearn(students, clearformess);
  	doActivityTrouble(trouble, clearformess);
  	doActivityTeach(teachers, clearformess);
  	doActivityBury(bury, clearformess);
}

void doActivitySolicitDonations(vector<Creature *> &solicit, char &clearformess)
{  //SOLICITORS
   long total_income=0;
   for(int s=0;s<len(solicit);s++)
   {
      if(!checkforarrest(*solicit[s],"soliciting donations",clearformess))
      {
         int income=solicit[s]->skill_roll(SKILL_PERSUASION) *
                    solicit[s]->get_armor().get_professionalism()+1;

         // Country's alignment dramatically affects effectiveness
         // The more conservative the country, the more effective
         if(publicmood(-1)>90) income/=2;
         if(publicmood(-1)>65) income/=2;
         if(publicmood(-1)>35) income/=2;
         if(publicmood(-1)>10) income/=2;

         solicit[s]->income=income;

         total_income += income;

         solicit[s]->train(SKILL_PERSUASION,max(5-solicit[s]->get_skill(SKILL_PERSUASION),2));
      }
   }
   ledger.add_funds(total_income,INCOME_DONATIONS);
}

void doActivitySellTshirts(vector<Creature *> &tshirts, char &clearformess)
{
   for(int s=0;s<len(tshirts);s++)
   {
      if(!checkforarrest(*tshirts[s],"selling shirts",clearformess))
      {
         long money = (tshirts[s]->skill_roll(SKILL_TAILORING) +
                       tshirts[s]->skill_roll(SKILL_BUSINESS))/2;

         // Country's alignment affects effectiveness
         // In a Liberal country, there are many competing vendors
         if(publicmood(-1)>65) money/=2;
         if(publicmood(-1)>35) money/=2;

         //If you're selling epic shirts enough they'll have some political impact
         if(tshirts[s]->skill_check(SKILL_TAILORING,DIFFICULTY_FORMIDABLE))
            background_liberal_influence[randomissue()]+=5;

         tshirts[s]->income=money;
         ledger.add_funds(money,INCOME_TSHIRTS);

         tshirts[s]->train(SKILL_TAILORING,max(7-tshirts[s]->get_skill(SKILL_TAILORING),2));
         tshirts[s]->train(SKILL_BUSINESS,max(7-tshirts[s]->get_skill(SKILL_BUSINESS),2));
      }
   }
}

void doActivitySellArt(vector<Creature *> &art, char &clearformess)
{
	for(int s=0;s<len(art);s++)
   {
      if(!checkforarrest(*art[s],"sketching portraits",clearformess))
      {
         long money = art[s]->skill_roll(SKILL_ART);

         // Country's alignment affects effectiveness
         // In a Liberal country, there are many competing vendors
         if(publicmood(-1)>65) money/=2;
         if(publicmood(-1)>35) money/=2;

         //Epic Liberal art may have positive political effect
         if(art[s]->skill_check(SKILL_ART,DIFFICULTY_FORMIDABLE))
            background_liberal_influence[randomissue()]+=5;

         art[s]->income=money;
         ledger.add_funds(money,INCOME_SKETCHES);

         art[s]->train(SKILL_ART,max(7-art[s]->get_skill(SKILL_ART),4));
      }
   }
}

void doActivitySellMusic(vector<Creature *> &music, char &clearformess)
{
   for(int s=0;s<len(music);s++)
   {
      if(!checkforarrest(*music[s],"playing music",clearformess))
      {
         long money = music[s]->skill_roll(SKILL_MUSIC)/2;
         bool has_instrument = music[s]->get_weapon().is_instrument();

         if(has_instrument) money*=4;

         // Country's alignment affects effectiveness
         // In a Liberal country, there are many competing vendors
         if(publicmood(-1)>65) money/=2;
         if(publicmood(-1)>35) money/=2;

         //Epic Liberal protest songs
         if(music[s]->skill_check(SKILL_MUSIC,DIFFICULTY_FORMIDABLE))
            background_liberal_influence[randomissue()]+=has_instrument ? 10 : 5;

         ledger.add_funds(money,INCOME_BUSKING);
         music[s]->income=money;

         if(has_instrument) music[s]->train(SKILL_MUSIC,max(7-music[s]->get_skill(SKILL_MUSIC),4));
         else music[s]->train(SKILL_MUSIC,max(5-music[s]->get_skill(SKILL_MUSIC),2));
      }
   }
}

void doActivitySellBrownies(vector<Creature *> &brownies, char &clearformess)
{
   for(int s=0;s<len(brownies);s++)
   {
      //Check for police search
      int dodgelawroll = LCSrandom(1 + 30*to_index(law[LAW_DRUGS]));

      //Saved by street sense?
      if(dodgelawroll==0)
         dodgelawroll=brownies[s]->skill_check(SKILL_STREETSENSE,DIFFICULTY_AVERAGE);

      if (dodgelawroll == 0
        && (law[LAW_DRUGS] != Alignment::LIBERAL && law[LAW_DRUGS] != Alignment::ELITE_LIBERAL)) // Busted!
      {
         newsstoryst *ns=new newsstoryst;
         ns->type=NEWSSTORY_DRUGARREST;
         ns->loc=-1;
         newsstory.push_back(ns);
         sitestory=ns;

         criminalize(*brownies[s],LAWFLAG_BROWNIES);
         attemptarrest(*brownies[s],"selling brownies",clearformess);
      }

      long money = brownies[s]->skill_roll(SKILL_PERSUASION) +
                   brownies[s]->skill_roll(SKILL_BUSINESS) +
                   brownies[s]->skill_roll(SKILL_STREETSENSE);

      // more money when more illegal
      if(law[LAW_DRUGS] == Alignment::ARCH_CONSERVATIVE) money*=4;
      if(law[LAW_DRUGS] == Alignment::CONSERVATIVE)      money*=2;
      if(law[LAW_DRUGS] == Alignment::LIBERAL)           money/=4;
      if(law[LAW_DRUGS] == Alignment::ELITE_LIBERAL)     money/=8;

      brownies[s]->income=money;
      ledger.add_funds(money,INCOME_BROWNIES);

      // Make the sale
      brownies[s]->train(SKILL_PERSUASION,max(4-brownies[s]->get_skill(SKILL_PERSUASION),1));
      // Know the streets
      brownies[s]->train(SKILL_STREETSENSE,max(7-brownies[s]->get_skill(SKILL_STREETSENSE),3));
      // Manage your money
      brownies[s]->train(SKILL_BUSINESS,max(10-brownies[s]->get_skill(SKILL_BUSINESS),3));
   }

}

void doActivityHacking(vector<Creature *> &hack, char &clearformess)
{
   if(len(hack))
   {
      vector<Creature *> cc,web,ddos,truehack;

      //First, do accounting to figure out who's doing what
      for(int h=0;h<len(hack);h++)
      {
         switch(hack[h]->activity.type)
         {
         case ACTIVITY_CCFRAUD:
            hack[h]->train(SKILL_COMPUTERS,2);
            cc.push_back(hack[h]);
            break;
         case ACTIVITY_DOS_ATTACKS:
            hack[h]->train(SKILL_COMPUTERS,2);
            web.push_back(hack[h]);
            break;
         case ACTIVITY_DOS_RACKET:
            hack[h]->train(SKILL_COMPUTERS,4);
            ddos.push_back(hack[h]);
            break;
         case ACTIVITY_HACKING:
            hack[h]->train(SKILL_COMPUTERS,4);
            truehack.push_back(hack[h]);
            break;
         }
      }

      char msg[200] = {0};

      //MAJOR HACKING
      int hack_skill=0;
      for(int h=0;h<len(truehack);h++)
         hack_skill=MAX(hack_skill,truehack[h]->skill_roll(SKILL_COMPUTERS));

      if(DIFFICULTY_HEROIC<=hack_skill+len(truehack)-1)
      {
         if(len(truehack)>1) strcpy(msg,"Your Hackers have ");
         else { strcpy(msg,truehack[0]->name); strcat(msg," has "); }

         int trackdif=0,juiceval=0;
         int short crime=0;

         switch(LCSrandom(11))
         {
            case 0:
            {
               strcat(msg,"pilfered files from a Corporate server.");

               Item *it=new Loot(*loottype[getloottype("LOOT_CORPFILES")]);
               location[hack[0]->location]->loot.push_back(it);

               trackdif=DIFFICULTY_SUPERHEROIC;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               break;
            }
            case 1: // *JDS* Penetrated government networks; don't get any loot, but do scare the info community
               strcat(msg,"caused a scare by breaking into a CIA network.");

               trackdif=DIFFICULTY_IMPOSSIBLE;
               crime=LAWFLAG_INFORMATION;
               juiceval=25;
               change_public_opinion(VIEW_INTELLIGENCE,10,0,75);
               break;
            case 2:
               strcat(msg,"sabotaged a genetics research company's network.");

               trackdif=DIFFICULTY_SUPERHEROIC;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               change_public_opinion(VIEW_GENETICS,2,0,75);
               break;
            case 3:
            {
               strcat(msg,"intercepted internal media emails.");

               Item *it;
               if(LCSrandom(2))it=new Loot(*loottype[getloottype("LOOT_CABLENEWSFILES")]);
               else it=new Loot(*loottype[getloottype("LOOT_AMRADIOFILES")]);
               location[hack[0]->location]->loot.push_back(it);

               trackdif=DIFFICULTY_SUPERHEROIC;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               break;
            }
            case 4:
               strcat(msg,"broke into military networks leaving LCS slogans.");

               trackdif=DIFFICULTY_IMPOSSIBLE;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,5,0,75);
               break;
            case 5:
            {
               strcat(msg,"uncovered information on dangerous research.");

               Item *it=new Loot(*loottype[getloottype("LOOT_RESEARCHFILES")]);
               location[hack[0]->location]->loot.push_back(it);

               trackdif=DIFFICULTY_SUPERHEROIC;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               break;
            }
            case 6:
            {
               strcat(msg,"discovered evidence of judicial corruption.");

               Item *it=new Loot(*loottype[getloottype("LOOT_JUDGEFILES")]);
               location[hack[0]->location]->loot.push_back(it);

               trackdif=DIFFICULTY_SUPERHEROIC;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               break;
            }
            case 7:
            {
               strcat(msg,"subverted a Conservative family forum.");

               trackdif=DIFFICULTY_SUPERHEROIC;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               change_public_opinion(VIEW_GAY,2,0,75);
               change_public_opinion(VIEW_WOMEN,2,0,75);
               break;
            }
            case 8:
            {
               strcat(msg,"spread videos of racist police brutality.");

               trackdif=DIFFICULTY_SUPERHEROIC;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               change_public_opinion(VIEW_POLICEBEHAVIOR,2,0,75);
               change_public_opinion(VIEW_CIVILRIGHTS,2,0,75);
               break;
            }
            case 9:
            {
               strcat(msg,"published emails revealing CEO tax evasion.");
               //Scambaiting, except you're baiting a CEO

               trackdif=DIFFICULTY_SUPERHEROIC;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               change_public_opinion(VIEW_CEOSALARY,2,0,75);
               change_public_opinion(VIEW_TAXES,2,0,75);
               break;
            }
            case 10:
            {
               strcat(msg,"revealed huge political bias in INS processes.");

               trackdif=DIFFICULTY_SUPERHEROIC;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               change_public_opinion(VIEW_IMMIGRATION,2,0,75);
               change_public_opinion(VIEW_FREESPEECH,2,0,75);
               break;
            }

         }

         if(trackdif>hack_skill + LCSrandom(5)-2)
            for(int h=0;h<len(truehack);h++)
               criminalize(*hack[h],crime);

         // Award juice to the hacking team for a job well done
         for(int h=0;h<len(truehack);h++)
            addjuice(*truehack[h],juiceval,200);
      }
      else if(DIFFICULTY_FORMIDABLE<=hack_skill+len(truehack)-1)
      {
         int issue=LCSrandom(VIEWNUM-5),crime=LAWFLAG_INFORMATION;

         // Maybe do a switch on issue here to specify which website it was, but I don't feel like
         // doing that right now

         if(len(truehack)>1) strcpy(msg,"Your hackers have ");
         else { strcpy(msg,truehack[0]->name); strcat(msg," has "); }

         switch(LCSrandom(4))
         {
         case 0: strcat(msg,"defaced");crime=LAWFLAG_INFORMATION; break;
         case 1: strcat(msg,"knocked out");crime=LAWFLAG_COMMERCE; break;
         case 2: strcat(msg,"threatened");crime=LAWFLAG_SPEECH; break;
         case 3: strcat(msg,"hacked");crime=LAWFLAG_INFORMATION; break;
         }
         strcat(msg," a ");
         switch(LCSrandom(5))
         {
         case 0: strcat(msg,"corporate website"); break;
         case 1: strcat(msg,"Conservative forum");break;
         case 2: strcat(msg,"Conservative blog"); break;
         case 3: strcat(msg,"news website"); break;
         case 4: strcat(msg,"government website"); break;
         }
         strcat(msg,".");

         change_public_opinion(issue,1);

         if(DIFFICULTY_HEROIC>hack_skill+LCSrandom(5)-2)
            for(int h=0;h<len(truehack);h++)
               criminalize(*truehack[h],crime);

         // Award juice to the hacking team for a job well done
         for(int h=0;h<len(truehack);h++)
            addjuice(*truehack[h],5,100);
      }

      if(msg[0])
      {
         if(clearformess) erase();
         else makedelimiter();

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);

         addstr(msg, gamelog);
         gamelog.nextMessage();
         msg[0]=0;

         getkey();
      }

      //CREDIT CARD FRAUD
      for(int h=0;h<len(cc);h++)
      {
         hack_skill = cc[h]->skill_roll(SKILL_COMPUTERS);
         int difficulty = DIFFICULTY_CHALLENGING;

         if(difficulty<=hack_skill)
         {
            // *JDS* You get between $1 and $100, plus an extra $1-50 every
            // time you pass a check against your hacking skill, where chance of
            // failure is one over the adjusted hackers' skill divided by four. Once
            // a check fails, no more money is gained. This check will be made
            // up to 20 times, at which point the loop breaks. The skill check
            // here doesn't take into account funding.
            int fundgain=LCSrandom(101);
            while(difficulty<hack_skill)
            {
               fundgain+=LCSrandom(51);
               difficulty+=2;
            }
            ledger.add_funds(fundgain,INCOME_CCFRAUD);

            cc[h]->income = fundgain / len(cc);

            if(fundgain/25>LCSrandom(hack_skill+1))
               criminalize(*cc[h],LAWFLAG_CCFRAUD);
         }

         if(msg[0])
         {
            if(clearformess) erase();
            else makedelimiter();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);

            addstr(msg, gamelog); //TODO: Log this?
            gamelog.nextMessage();
            msg[0]=0;

            getkey();
         }
      }
   }

}

void doActivityGraffiti(vector<Creature *> &graffiti, char &clearformess)
{
	int s;

   if(len(graffiti))
   {
      for(s=0;s<len(graffiti);s++)
      {
         if(!graffiti[s]->get_weapon().can_graffiti())
         {

            if(clearformess) erase();
            else makedelimiter();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(graffiti[s]->name, gamelog);

            //Check base inventory for a spraycan
            bool foundone=false;
            for(int i=0;i<len(location[graffiti[s]->base]->loot);i++)
            {
               if(location[graffiti[s]->base]->loot[i]->is_weapon())
               {
                  Weapon *w = static_cast<Weapon*>(location[graffiti[s]->base]->loot[i]); //cast -XML
                  if(w->can_graffiti())
                  {
                     addstr(" grabbed a ", gamelog);
                     addstr(w->get_name(), gamelog);
                     addstr(" from ", gamelog);
                     addstr(location[graffiti[s]->base]->getname()); //TODO: Explicitly log it, or will the game log it?
                     addstr(".", gamelog);

                     getkey();

                     graffiti[s]->give_weapon(*w,&(location[graffiti[s]->base]->loot));

                     if(location[graffiti[s]->base]->loot[i]->empty())
                        delete_and_remove(location[graffiti[s]->base]->loot,i);
                     foundone = true;
                     break;
                  }
               }
            }

            if(!foundone && ledger.get_funds()>=20)
            {
               ledger.subtract_funds(20,EXPENSE_SHOPPING);
               addstr(" bought spraypaint for graffiti.", gamelog);

               getkey();

               Weapon spray(*weapontype[getweapontype("WEAPON_SPRAYCAN")]);
               graffiti[s]->give_weapon(spray,&location[graffiti[s]->base]->loot);
            }
            else if (!foundone)
            {
               addstr(" needs a spraycan equipped to do graffiti.", gamelog);
               graffiti[s]->activity.type=ACTIVITY_NONE;

               getkey();
            }
            gamelog.nextMessage(); //Next message now so that we don't have to type it for every case.
         }

         int issue=VIEW_LIBERALCRIMESQUAD,power=1;

         if(clearformess) erase();
         else makedelimiter();

         if(!LCSrandom(10) &&
            !(graffiti[s]->skill_check(SKILL_STREETSENSE,DIFFICULTY_AVERAGE)))
         {
            if(clearformess) erase();
            else makedelimiter();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);

            addstr(graffiti[s]->name, gamelog);
            addstr(" was spotted by the police", gamelog);
            criminalize(*graffiti[s],LAWFLAG_VANDALISM);
            graffiti[s]->train(SKILL_STREETSENSE,20);

            if(graffiti[s]->activity.arg!=-1)
            {
               addstr(" while working on the mural!", gamelog);
               graffiti[s]->activity.arg=-1;
            }
            else addstr(" while spraying an LCS tag!", gamelog);
            gamelog.nextMessage();

            newsstoryst *ns=new newsstoryst;
            ns->type=NEWSSTORY_GRAFFITIARREST;
            ns->loc=-1;
            ns->positive=0;
            newsstory.push_back(ns);
            sitestory=ns;

            getkey();

            attemptarrest(*graffiti[s],NULL,clearformess);
         }
         else if(graffiti[s]->activity.arg!=-1)
         {
            power=0;
            if(!LCSrandom(3))
            {
               issue=graffiti[s]->activity.arg;
               power=graffiti[s]->skill_roll(SKILL_ART)/3;

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(graffiti[s]->name, gamelog);
               addstr(" has completed a", gamelog);
               if(power>3)addstr(" beautiful", gamelog);
               addstr(" mural about ", gamelog);
               addstr(getview(issue,false), gamelog);
               addstr(".", gamelog);
               gamelog.nextMessage();

               graffiti[s]->activity.arg=-1;
               addjuice(*graffiti[s],power,power*20);
               change_public_opinion(issue,power);
               graffiti[s]->train(SKILL_ART,MAX(10-graffiti[s]->get_skill(SKILL_ART)/2,1));

               getkey();
            }
            else
            {
               power=0;
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(graffiti[s]->name, gamelog);
               addstr(" works through the night on a large mural.", gamelog);
               gamelog.nextMessage();
               graffiti[s]->train(SKILL_ART,MAX(10-graffiti[s]->get_skill(SKILL_ART)/2,1));

               getkey();
            }
         }
         else if(!LCSrandom(MAX(30-graffiti[s]->get_skill(SKILL_ART)*2,5)))
         {
            issue=randomissue();
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(graffiti[s]->name, gamelog);
            addstr(" has begun work on a large mural about ", gamelog);
            addstr(getview(issue,false), gamelog);
            addstr(".", gamelog);
            gamelog.nextMessage();
            graffiti[s]->activity.arg=issue;
            power=0;
            graffiti[s]->train(SKILL_ART,MAX(10-graffiti[s]->get_skill(SKILL_ART)/2,1));

            getkey();
         }

         graffiti[s]->train(SKILL_ART,MAX(4-graffiti[s]->get_skill(SKILL_ART),0));
         if(issue==VIEW_LIBERALCRIMESQUAD)
         {
            change_public_opinion(VIEW_LIBERALCRIMESQUAD,LCSrandom(2),0,65);
            change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,!LCSrandom(8),0,65);
            public_interest[issue]+=power;
         }
         else
         {
            change_public_opinion(VIEW_LIBERALCRIMESQUAD,LCSrandom(2)+1,0,85);
            change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,!LCSrandom(4),0,65);
            public_interest[issue]+=power;
            background_liberal_influence[issue]+=power;
         }
      }
   }
}

void doActivityProstitution(vector<Creature *> &prostitutes, char &clearformess)
{
   for(int p=len(prostitutes)-1;p>=0;p--)
   {
      // Business once every three days or so
      if(LCSrandom(3)) continue;

      //char num[20];

      long fundgain=0;
      char caught=0;

      // Skill determies how much money you get
      int performance = prostitutes[p]->skill_roll(SKILL_SEDUCTION);
      if(performance>DIFFICULTY_HEROIC)
         fundgain=LCSrandom(201)+200;
      else
         fundgain=LCSrandom(10*performance)+10*performance;

      // Street sense check or deal with slimy people that reduce dignity and juice
      if(!LCSrandom(3) &&
         !(prostitutes[p]->skill_check(SKILL_STREETSENSE,DIFFICULTY_AVERAGE)))
      {
         addjuice(*prostitutes[p],-!LCSrandom(3), -20);
      }

      // Gain seduction and street sense
      prostitutes[p]->train(SKILL_SEDUCTION,MAX(10-prostitutes[p]->get_skill(SKILL_SEDUCTION),0));
      prostitutes[p]->train(SKILL_STREETSENSE,MAX(10-prostitutes[p]->get_skill(SKILL_STREETSENSE),0));


      if(!LCSrandom(50)) // Police sting?
      {
         if(!(prostitutes[p]->skill_check(SKILL_STREETSENSE,DIFFICULTY_AVERAGE))) // Street sense to avoid
         {
            if(clearformess) erase();
            else makedelimiter();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(prostitutes[p]->name, gamelog);
            addstr(" has been arrested in a prostitution sting.", gamelog);
            gamelog.nextMessage();

            addjuice(*prostitutes[p],-7,-30);

            getkey();

            caught=1;

            removesquadinfo(*prostitutes[p]);
            prostitutes[p]->carid=-1;
            prostitutes[p]->location=find_police_station(*prostitutes[p]);
            prostitutes[p]->drop_weapons_and_clips(NULL);
            prostitutes[p]->activity.type=ACTIVITY_NONE;
            criminalize(*prostitutes[p],LAWFLAG_PROSTITUTION);
         }
         else
         {
            if(clearformess) erase();
            else makedelimiter();

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(prostitutes[p]->name, gamelog);
            addstr(" was nearly caught in a prostitution sting.", gamelog);
            gamelog.nextMessage();

            addjuice(*prostitutes[p],5,0);

            getkey();
         }
      }

      if(!caught)
      {
         prostitutes[p]->train(SKILL_STREETSENSE,MAX(5-prostitutes[p]->get_skill(SKILL_STREETSENSE),0));
         ledger.add_funds(fundgain,INCOME_PROSTITUTION);
         prostitutes[p]->income = fundgain;
      }
   }
}

void doActivityLearn(vector<Creature *> &students, char &clearformess)
{
   for(int s=len(students)-1;s>=0;s--)
   {
	   if(ledger.get_funds()<60) break;
	   ledger.subtract_funds(60,EXPENSE_TRAINING);
	   int skill[2] = {-1, -1};
	   int effectiveness[2] = {20, 20};
	   switch(students[s]->activity.type)
	   {
		  case ACTIVITY_STUDY_DEBATING:
			  skill[0] = SKILL_PERSUASION;
			  break;
		  case ACTIVITY_STUDY_MARTIAL_ARTS:
			  skill[0] = SKILL_HANDTOHAND;
			  break;
		  case ACTIVITY_STUDY_DRIVING:
			  skill[0] = SKILL_DRIVING;
			  break;
		  case ACTIVITY_STUDY_PSYCHOLOGY:
           skill[0] = SKILL_PSYCHOLOGY;
			  break;
		  case ACTIVITY_STUDY_FIRST_AID:
           skill[0] = SKILL_FIRSTAID;
			  break;
		  case ACTIVITY_STUDY_LAW:
	        skill[0] = SKILL_LAW;
			  break;
		  case ACTIVITY_STUDY_DISGUISE:
           skill[0] = SKILL_DISGUISE;
			  break;
		  case ACTIVITY_STUDY_SCIENCE:
           skill[0] = SKILL_SCIENCE;
			  break;
		  case ACTIVITY_STUDY_BUSINESS:
           skill[0] = SKILL_BUSINESS;
			  break;
	//case ACTIVITY_STUDY_COOKING:
        //   skill[0] = SKILL_COOKING;
        //   break;
		  case ACTIVITY_STUDY_GYMNASTICS:
           skill[0] = SKILL_DODGE;
			  break;
        case ACTIVITY_STUDY_LOCKSMITHING:
           skill[0] = SKILL_SECURITY;
           break;
		  case ACTIVITY_STUDY_MUSIC:
           skill[0] = SKILL_MUSIC;
			  break;
		  case ACTIVITY_STUDY_ART:
           skill[0] = SKILL_ART;
			  break;
		  case ACTIVITY_STUDY_TEACHING:
           skill[0] = SKILL_TEACHING;
			  break;
		  case ACTIVITY_STUDY_WRITING:
           skill[0] = SKILL_WRITING;
			  break;
         case ACTIVITY_STUDY_COMPUTERS:
           skill[0] = SKILL_COMPUTERS;
           break; 
	   }
	   bool worthcontinuing=false;
	   for(int i=0; i<2; i++)
		   if(skill[i]!=-1)
		   {  // rapid decrease in effectiveness as your skill gets higher.
            effectiveness[i] /= (students[s]->get_skill(skill[i])+1);
			   if(effectiveness[i]<1)
               effectiveness[i]=1;
		      students[s]->train(skill[i],effectiveness[i]);
            if(students[s]->get_skill(skill[i]) < students[s]->skill_cap(skill[i],true))
				   worthcontinuing=true;
		   }
	   if (!worthcontinuing)
	   {
		   students[s]->activity.type=ACTIVITY_NONE;
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);
         addstr(students[s]->name, gamelog);
         addstr(" has learned as much as ", gamelog);
         addstr(students[s]->heshe(), gamelog);
         addstr(" can.", gamelog);
         gamelog.nextMessage();

         getkey();
	   }
   }
}

void doActivityTrouble(vector<Creature *> &trouble, char &clearformess)
{
   if(len(trouble))
   {
      long juiceval=0;
      char done=0;
      short crime=0;

      if(clearformess) erase();
      else makedelimiter();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      if(len(trouble)>1) addstr("Your Activists have ",gamelog);
      else { addstr(trouble[0]->name,gamelog); addstr(" has ",gamelog); }

      int power=0;
      for(int t=0;t<len(trouble);t++)
         power+=trouble[t]->skill_roll(SKILL_PERSUASION)+
                trouble[t]->skill_roll(SKILL_STREETSENSE);

      int mod=1;
      if(LCSrandom(10)<power) mod++;
      if(LCSrandom(20)<power) mod++;
      if(LCSrandom(40)<power) mod++;
      if(LCSrandom(60)<power) mod++;
      if(LCSrandom(80)<power) mod++;
      if(LCSrandom(100)<power) mod++;

      do
      {
         switch(LCSrandom(10))
         {
            case 0:
               addstr("run around uptown splashing paint on fur coats!", gamelog);
               juiceval=2;
               crime=LAWFLAG_ASSAULT;
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod>>1,0,70);
               public_interest[VIEW_ANIMALRESEARCH]+=mod;
               background_liberal_influence[VIEW_ANIMALRESEARCH]+=mod;
               done=1;
               break;
            case 1:
            {
               if (law[LAW_GAY] != Alignment::ELITE_LIBERAL)
               {
                  addstr("disrupted a traditional wedding at a church!", gamelog);
                  change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
                  public_interest[VIEW_GAY]+=mod;
                  background_liberal_influence[VIEW_GAY]+=mod;
                  juiceval=2;
                  crime=LAWFLAG_DISTURBANCE;
                  done=1;
               }
               break;
            }
            case 2:
            {
               if (law[LAW_ABORTION] != Alignment::ELITE_LIBERAL)
               {
                  addstr("posted horrifying dead abortion doctor pictures downtown!", gamelog);
                  change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
                  public_interest[VIEW_WOMEN]+=mod;
                  background_liberal_influence[VIEW_WOMEN]+=mod;
                  juiceval=1;
                  done=1;
               }
               break;
            }
            case 3:
            {
               if (law[LAW_POLICEBEHAVIOR] != Alignment::ELITE_LIBERAL)
               {
                  addstr("gone downtown and reenacted a police beating!", gamelog);
                  change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
                  public_interest[VIEW_POLICEBEHAVIOR]+=mod;
                  background_liberal_influence[VIEW_POLICEBEHAVIOR]+=mod;
                  juiceval=2;
                  crime=LAWFLAG_DISTURBANCE;
                  done=1;
               }
               break;
            }
            case 4:
            {
               if (law[LAW_NUCLEARPOWER] != Alignment::ELITE_LIBERAL)
               {
                  if(len(trouble)>1)addstr("dressed up and pretended to be radioactive mutants!", gamelog);
                  else addstr("dressed up and pretended to be a radioactive mutant!", gamelog);
                  change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
                  public_interest[VIEW_NUCLEARPOWER]+=mod;
                  background_liberal_influence[VIEW_NUCLEARPOWER]+=mod;
                  juiceval=2;
                  crime=LAWFLAG_DISTURBANCE;
                  done=1;
               }
               break;
            }
            case 5:
            {
               if (law[LAW_POLLUTION] != Alignment::ELITE_LIBERAL)
               {
                  addstr("squirted business people with fake polluted water!", gamelog);
                  change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
                  public_interest[VIEW_POLLUTION]+=mod;
                  background_liberal_influence[VIEW_POLLUTION]+=mod;
                  juiceval=2;
                  crime=LAWFLAG_DISTURBANCE;
                  done=1;
               }
               break;
            }
            case 6:
            {
               if (law[LAW_DEATHPENALTY] != Alignment::ELITE_LIBERAL)
               {
                  addstr("distributed fliers graphically illustrating executions!", gamelog);
                  change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
                  public_interest[VIEW_DEATHPENALTY]+=mod;
                  background_liberal_influence[VIEW_DEATHPENALTY]+=mod;
                  juiceval=1;
                  done=1;
               }
               break;
            }
            case 7:
            {
               addstr("distributed fliers graphically illustrating CIA torture!", gamelog);
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
               public_interest[VIEW_TORTURE]+=mod;
               background_liberal_influence[VIEW_TORTURE]+=mod;
               juiceval=1;
               done=1;
               break;
            }
            case 8:
            {
               addstr("burned a corporate symbol and denounced capitalism!", gamelog);
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
               public_interest[VIEW_CORPORATECULTURE]+=mod;
               background_liberal_influence[VIEW_CORPORATECULTURE]+=mod;
               if (law[LAW_CORPORATE] == Alignment::ARCH_CONSERVATIVE)
               {               // In extreme corporate culture cases this should give a flag burning charge! -- kviiri
                  juiceval=2;  // Done -- SlatersQuest
                  crime=LAWFLAG_BURNFLAG;
               }
               else juiceval=1;
               done=1;
               break;
            }
            case 9:
            {
               addstr("set up a mock sweatshop in the middle of the mall!", gamelog);
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
               public_interest[VIEW_SWEATSHOPS]+=mod;
               background_liberal_influence[VIEW_SWEATSHOPS]+=mod;
               juiceval+=1;
               done=1;
               break;
            }
         }
      } while(!done);

      gamelog.nextMessage(); //Do this now so that it doesn't have to be done in every case up there.

      getkey();

      if(crime!=0)
      {
         for(int t=0;t<len(trouble);t++)
         {
            if(!LCSrandom(30) &&
               !(trouble[t]->skill_check(SKILL_STREETSENSE,DIFFICULTY_AVERAGE)))
            {
               if(clearformess) erase();
               else makedelimiter();

               if(!LCSrandom(4))
               {
                  newsstoryst *ns=new newsstoryst;
                  ns->type=NEWSSTORY_WANTEDARREST; // should probably create a NEWSSTORY_TROUBLEARREST and implement it fully
                  ns->loc=-1;                      // but this will have to do for now
                  ns->positive=0;
                  newsstory.push_back(ns);
                  sitestory=ns;
                  attemptarrest(*trouble[t],"causing trouble",clearformess);
               }
               else
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(trouble[t]->name, gamelog);
                  addstr(" is cornered by a mob of angry rednecks.", gamelog);
                  gamelog.nextMessage();

                  getkey();

                  bool wonfight = false;

                  if(trouble[t]->get_weapon().is_threatening())
                  {
                     if(clearformess) erase();
                     else makedelimiter();

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(8,1);
                     addstr(trouble[t]->name, gamelog);
                     addstr(" brandishes the ", gamelog);
                     addstr(trouble[t]->get_weapon().get_name(), gamelog);
                     addstr("!", gamelog);
                     gamelog.nextMessage();

                     getkey();

                     if(clearformess) erase();
                     else makedelimiter();

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(8,1);
                     addstr("The mob scatters!", gamelog);
                     gamelog.nextMessage();

                     getkey();

                     addjuice(*trouble[t],5,20);

                     wonfight = true;
                  }
                  else
                  {
                     for(int count=0;count<=LCSrandom(5)+2;count++)
                     {
                        if(clearformess) erase();
                        else makedelimiter();
                        if(trouble[t]->skill_roll(SKILL_HANDTOHAND)>LCSrandom(6)+count)
                        {
                           set_color(COLOR_CYAN,COLOR_BLACK,1);
                           move(8,1);
                           addstr(trouble[t]->name, gamelog);
                           switch(LCSrandom(8))
                           {
                           case 0:addstr(" breaks the arm of the nearest person!", gamelog);break;
                           case 1:addstr(" knees a guy in the balls!", gamelog);break;
                           case 2:addstr(" knocks one out with a fist to the face!", gamelog);break;
                           case 3:addstr(" bites some hick's ear off!", gamelog);break;
                           case 4:addstr(" smashes one of them in the jaw!", gamelog);break;
                           case 5:addstr(" shakes off a grab from behind!", gamelog);break;
                           case 6:addstr(" yells the slogan!", gamelog);break;
                           case 7:addstr(" knocks two of their heads together!", gamelog);break;
                           }

                           gamelog.nextMessage();

                           getkey();

                           wonfight=true;
                        }
                        else
                        {
                           set_color(COLOR_YELLOW,COLOR_BLACK,1);
                           move(8,1);
                           addstr(trouble[t]->name, gamelog);
                           switch(LCSrandom(8))
                           {
                           case 0:addstr(" is held down and kicked by three guys!", gamelog);break;
                           case 1:addstr(" gets pummeled!", gamelog);break;
                           case 2:addstr(" gets hit by a sharp rock!", gamelog);break;
                           case 3:addstr(" is thrown against the sidewalk!", gamelog);break;
                           case 4:addstr(" is bashed in the face with a shovel!", gamelog);break;
                           case 5:addstr(" is forced into a headlock!", gamelog);break;
                           case 6:addstr(" crumples under a flurry of blows!", gamelog);break;
                           case 7:addstr(" is hit in the chest with a pipe!", gamelog);break;
                           }

                           gamelog.nextMessage();

                           getkey();

                           count++; // fight goes faster when you're losing

                           wonfight=false;
                        }
                     }

                     if(wonfight)
                     {
                        if(clearformess) erase();
                        else makedelimiter();
                        set_color(COLOR_GREEN,COLOR_BLACK,1);
                        move(8,1);
                        addstr(trouble[t]->name, gamelog);
                        addstr(" beat the ", gamelog);
                        if (law[LAW_FREESPEECH] == Alignment::ARCH_CONSERVATIVE)
                          addstr("[tar]", gamelog);
                        else
                          addstr("shit", gamelog);
                        addstr(" out of everyone who got close!", gamelog);

                        gamelog.nextMessage();

                        getkey();

                        addjuice(*trouble[t],30,300);
                        if(trouble[t]->blood>70)trouble[t]->blood=70;
                     }
                  }

                  if(!wonfight)
                  {
                     if(clearformess) erase();
                     else makedelimiter();
                     set_color(COLOR_RED,COLOR_BLACK,1);
                     move(8,1);
                     addstr(trouble[t]->name, gamelog);
                     addstr(" is severely beaten before the mob is broken up.", gamelog);
                     gamelog.nextMessage();

                     trouble[t]->activity.type=ACTIVITY_CLINIC;

                     getkey();

                     addjuice(*trouble[t],-10,-50);
                     if(trouble[t]->blood>10)trouble[t]->blood=10;

                     if(!LCSrandom(5))
                     {
                        if(clearformess) erase();
                        else makedelimiter();
                        switch(LCSrandom(10))
                        {
                           case 0:
                              if(trouble[t]->special[SPECIALWOUND_LOWERSPINE]==1)
                              {
                                 move(8,1);
                                 addstr(trouble[t]->name, gamelog);
                                 addstr("'s lower spine has been broken!", gamelog);
                                 gamelog.nextMessage();
                                 trouble[t]->special[SPECIALWOUND_LOWERSPINE]=0;

                                 getkey();
                              }
                              break;
                           case 1:
                              if(trouble[t]->special[SPECIALWOUND_UPPERSPINE]==1)
                              {
                                 move(8,1);
                                 addstr(trouble[t]->name, gamelog);
                                 addstr("'s upper spine has been broken!", gamelog);
                                 gamelog.nextMessage();
                                 trouble[t]->special[SPECIALWOUND_UPPERSPINE]=0;

                                 getkey();
                              }
                              break;
                           case 2:
                              if(trouble[t]->special[SPECIALWOUND_NECK]==1)
                              {
                                 move(8,1);
                                 addstr(trouble[t]->name, gamelog);
                                 addstr("'s neck has been broken!", gamelog);
                                 gamelog.nextMessage();
                                 trouble[t]->special[SPECIALWOUND_NECK]=0;

                                 getkey();
                              }
                              break;
                           case 3:
                              if(trouble[t]->special[SPECIALWOUND_TEETH]>0)
                              {
                                 move(8,1);
                                 addstr(trouble[t]->name);
                                 if(trouble[t]->special[SPECIALWOUND_TEETH]>1)addstr("'s teeth have been smashed out on the curb.", gamelog);
                                 else addstr("'s tooth has been pulled out with pliers!", gamelog);
                                 gamelog.nextMessage();
                                 trouble[t]->special[SPECIALWOUND_TEETH]=0;

                                 getkey();
                              }
                              break;
                           default:
                           {
                              if(trouble[t]->special[SPECIALWOUND_RIBS]>0)
                              {
                                 int ribminus=LCSrandom(RIBNUM)+1;
                                 if(ribminus>trouble[t]->special[SPECIALWOUND_RIBS])ribminus=trouble[t]->special[SPECIALWOUND_RIBS];

                                 move(8,1);
                                 if(ribminus>1)
                                 {
                                    if(ribminus==trouble[t]->special[SPECIALWOUND_RIBS])
                                       addstr("All ", gamelog);
                                    addstr(ribminus, gamelog);
                                    addstr(" of ", gamelog);
                                    addstr(trouble[t]->name, gamelog);
                                    addstr("'s ribs are ", gamelog);
                                 }
                                 else if(trouble[t]->special[SPECIALWOUND_RIBS]>1)
                                 {
                                    addstr("One of ", gamelog);
                                    addstr(trouble[t]->name, gamelog);
                                    addstr("'s rib is ", gamelog);
                                 }
                                 else
                                 {
                                    addstr(trouble[t]->name);
                                    addstr("'s last unbroken rib is ", gamelog);
                                 }

                                 addstr("broken!", gamelog);
                                 gamelog.nextMessage();

                                 getkey();

                                 trouble[t]->special[SPECIALWOUND_RIBS]-=ribminus;
                              }
                              break;
                           }
                        }
                     }
                  }
               }
            }
         }
      }

      for(int h=0;h<len(trouble);h++)
         addjuice(*trouble[h],juiceval,40);
   }
}

void doActivityTeach(vector<Creature *> &teachers, char &clearformess)
{
   for(int t=0;t<len(teachers);t++)
   {
      int skillarray[14];
      int cost=0, students=0;
      //Build a list of skills to train and determine the cost for running
      //a class depending on what the teacher is teaching
      switch(teachers[t]->activity.type)
      {
      case ACTIVITY_TEACH_POLITICS:
         cost=2;
         skillarray[0]=SKILL_LAW;
         skillarray[1]=SKILL_PERSUASION;
         skillarray[2]=SKILL_WRITING;
         skillarray[3]=SKILL_RELIGION;
         skillarray[4]=SKILL_BUSINESS;
         skillarray[5]=SKILL_SCIENCE;
         skillarray[6]=SKILL_STREETSENSE;
         skillarray[7]=SKILL_MUSIC;
         skillarray[8]=SKILL_ART;
         skillarray[9]=-1;
         break;
      case ACTIVITY_TEACH_COVERT:
         cost=6;
         skillarray[0]=SKILL_SECURITY;
         skillarray[1]=SKILL_COMPUTERS;
         skillarray[2]=SKILL_DISGUISE;
         skillarray[3]=SKILL_TAILORING;
         skillarray[4]=SKILL_STEALTH;
         skillarray[5]=SKILL_SEDUCTION;
         skillarray[6]=SKILL_PSYCHOLOGY;
         skillarray[7]=SKILL_DRIVING;
         skillarray[8]=-1;
         break;
      case ACTIVITY_TEACH_FIGHTING:
         cost=10;
         skillarray[0]=SKILL_KNIFE;
         skillarray[1]=SKILL_SWORD;
         skillarray[2]=SKILL_CLUB;
         skillarray[3]=SKILL_PISTOL;
         skillarray[4]=SKILL_RIFLE;
         skillarray[5]=SKILL_SHOTGUN;
         skillarray[6]=SKILL_HEAVYWEAPONS;
         skillarray[7]=SKILL_AXE;
         skillarray[8]=SKILL_SMG;
         skillarray[9]=SKILL_THROWING;
         skillarray[10]=SKILL_HANDTOHAND;
         skillarray[11]=SKILL_DODGE;
         skillarray[12]=SKILL_FIRSTAID;
         skillarray[13]=-1;
         break;
      }

      //Count potential students for this teacher to get an idea of efficiency
      for(int p=0;p<len(pool);p++)
      {
         //If they're at the location
         if(pool[p]->location==teachers[t]->location &&
            pool[p]->align==Alignment::LIBERAL &&
            pool[p]->alive)
         {
            //Step through the array of skills to train
            for(int i=0;i<13;i++)           //Any reason why we aren't using a while(true) loop or something even more dynamic? --kviiri
            {
               //If no more skills to train, stop
               if(skillarray[i]==-1) break;
               //Otherwise, if the student has less skill than the teacher, train the student
               //proportional to the difference in skill between teacher and student times the
               //teacher's ability at teaching
               if(pool[p]->get_skill(skillarray[i])<teachers[t]->get_skill(skillarray[i])-1&&
                  pool[p]->get_skill(skillarray[i])<teachers[t]->get_skill(SKILL_TEACHING)+2&&
                  pool[p]->get_skill(skillarray[i])<pool[p]->skill_cap(skillarray[i],true))
               {
                  students++;
               }
            }
         }
      }

      //Check funds.
      if(ledger.get_funds()<min(students,10)*cost)
         continue; //Can't afford to teach them. Continue with next teacher.

      //Walk through and train people
      for(int p=0;p<len(pool);p++)
      {
         //If they're at the location
         if(pool[p]->location==teachers[t]->location &&
            pool[p]->align==Alignment::LIBERAL &&
            pool[p]->alive)
         {
            //Step through the array of skills to train
            for(int i=0;i<13;i++)
            {
               //If no more skills to train, stop
               if(skillarray[i]==-1)break;
               //Otherwise, if the student has less skill than the teacher, train the student
               //proportional to the difference in skill between teacher and student times the
               //teacher's ability at teaching
               if(pool[p]->get_skill(skillarray[i])<teachers[t]->get_skill(skillarray[i])-1&&
                  pool[p]->get_skill(skillarray[i])<teachers[t]->get_skill(SKILL_TEACHING)+2&&
                  pool[p]->get_skill(skillarray[i])<pool[p]->skill_cap(skillarray[i],true))
               {
                  // Teach based on teacher's skill in the topic plus skill in teaching, minus
                  // student's skill in the topic
                  int teach=teachers[t]->get_skill(skillarray[i])+
                            teachers[t]->get_skill(SKILL_TEACHING)-
                            pool[p]->get_skill(skillarray[i]);
                  //at ten students, cost no longer goes up, but effectiveness goes down.
                  if (students > 10)
                  {
                     //teach = (teach * 10) / students; //teach at 50% speed with twice as many students.
                     teach = ((teach * 30 / students) + teach)/4; //62.5% speed with twice as many students.
                  }
                  if(teach<1)
                     teach=1;
                  // Cap at 10 points per day
                  if(teach>10)
                     teach=10;

                  pool[p]->train(skillarray[i],teach);

                  /*if(students<10)
                  {
                     students++;
                     ledger.subtract_funds(cost,EXPENSE_TRAINING);
                     if(students==10)cost=0;
                  }*/
               }
            }
         }
      }

      ledger.subtract_funds( cost*min(students, 10), EXPENSE_TRAINING );
      teachers[t]->train(SKILL_TEACHING,min(students, 10));
   }
}

void doActivityBury(vector<Creature *> &bury, char &clearformess)
{
   if(len(bury))
   {
      for(int p=len(pool)-1;p>=0;p--)
      {
         if(pool[p]->alive) continue;

         bool arrest_attempted = false;

         //MAKE BASE LOOT
         makeloot(*pool[p],location[bury[0]->base]->loot);

         for(int b=0;b<len(bury);b++)
         {
            if(!arrest_attempted && !(bury[b]->skill_check(SKILL_STREETSENSE,DIFFICULTY_EASY)))
            {
               arrest_attempted = true; // Only attempt one burial arrest per body

               newsstoryst *ns=new newsstoryst;
               ns->type=NEWSSTORY_BURIALARREST;
               ns->loc=-1;
               newsstory.push_back(ns);
               sitestory=ns;

               criminalize(*bury[b],LAWFLAG_BURIAL);
               char str[100] = "burying ";
               strcat(str, pool[p]->name);
               strcat(str, "'s body");
               attemptarrest(*bury[b],str,clearformess);

               // If a liberal is spotted they should not do more burials.
               bury.erase(bury.begin() + b--);

               break;
            }
         }

         //BURY (even if interrupted)
         delete_and_remove(pool,p);

         if(!len(bury)) break; //Stop burials if none are left doing them.
      }
   }

}

/* steal a car */
bool stealcar(Creature &cr,char &clearformess)
{
   music.play(MUSIC_CARTHEFT);
   clearformess=1;

   short cartype;

   if(carselect(cr,cartype))
   {
      int diff=vehicletype[cartype]->steal_difficultytofind()*2;

      Vehicle *v=NULL;

      int old=cartype;

      cr.train(SKILL_STREETSENSE,5);

      //THEFT SEQUENCE
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Adventures in Liberal Car Theft", gamelog);
      gamelog.nextMessage();
      printcreatureinfo(&cr);
      makedelimiter();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(10,0);
      addstr(cr.name, gamelog);
      addstr(" looks around for an accessible vehicle...", gamelog);

      getkey();

      //ROUGH DAY
      if(!cr.skill_check(SKILL_STREETSENSE,diff))
         do cartype=LCSrandom(len(vehicletype));
         while(cartype==old||LCSrandom(10)<vehicletype[cartype]->steal_difficultytofind());

      string carname=(v=new Vehicle(*vehicletype[cartype]))->fullname();

      move(11,0);
      addstr(cr.name, gamelog);
      if(old!=cartype)
      {
         addstr(" was unable to find a ", gamelog);
         addstr(vehicletype[old]->longname(), gamelog);
         addstr(" but did find a ", gamelog);
      }
      else addstr(" found a ", gamelog);
      addstr(v->longname(), gamelog);
      addstr(".", gamelog);
      gamelog.nextMessage();

      getkey();

      //APPROACH?
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Adventures in Liberal Car Theft");
      printcreatureinfo(&cr);
      makedelimiter();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(10,0);
      addstr(cr.name, gamelog);
      addstr(" looks from a distance at an empty ", gamelog);
      addstr(carname, gamelog);
      addstr(".", gamelog);

      gamelog.nextMessage();

      move(12,0);
      addstr("A - Approach the driver's side door.");
      move(13,0);
      addstr("Enter - Call it a day.");

      while(true)
      {
         int c=getkey();
         if(c=='a')break;
         if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR){delete v;return false;}
      }

      //SECURITY?
      bool alarmon=false,sensealarm=LCSrandom(100)<v->sensealarmchance(),
           touchalarm=LCSrandom(100)<v->touchalarmchance();
      char windowdamage=0;

      for(bool entered=false;!entered;)
      {
         erase();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(0,0);
         addstr("Adventures in Liberal Car Theft");
         printcreatureinfo(&cr);
         makedelimiter();

         if(alarmon)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(10,0);
            if(sensealarm)addstr("THE VIPER");
            else addstr(carname);
            addstr(":   ");
            set_color(COLOR_RED,COLOR_BLACK,1);
            if(sensealarm)addstr("STAND AWAY FROM THE VEHICLE!   <BEEP!!> <BEEP!!>");
            else addstr("<BEEP!!> <BEEP!!> <BEEP!!> <BEEP!!>");
         }
         else if(sensealarm)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(10,0);
            addstr("THE VIPER:   ");
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr("THIS IS THE VIPER!   STAND AWAY!", gamelog);
            gamelog.nextMessage();
         }
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(10,0);
            addstr(cr.name, gamelog);
            addstr(" stands by the ", gamelog);
            addstr(carname, gamelog);
            addstr(".", gamelog);
            gamelog.nextMessage();
         }

         move(12,0);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr("A - Pick the lock.");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(13,0);
         addstr("B - Break the window.");
         move(14,0);
         if(!sensealarm)addstr("Enter - Call it a day.");
         else
         {
            if(!alarmon)
            {
               addstr("Enter - The Viper?   ");
               addstr(cr.name);
               addstr(" is deterred.");
            }
            else
            {
               addstr("Enter - Yes, the Viper has deterred ");
               addstr(cr.name);
               addstr(".");
            }
         }

         char method=-1;
         while(method==-1)
         {
            int c=getkey();
            if(c=='a')method=0;
            if(c=='b')method=1;
            if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR){delete v;return false;} /* try again tomorrow */
         }

         //PICK LOCK
         if(method==0)
         {
            if(cr.skill_check(SKILL_SECURITY,DIFFICULTY_AVERAGE))
            {
               switch (fieldskillrate)
               {
                  case FIELDSKILLRATE_FAST:
                     cr.train(SKILL_SECURITY, 25);break;
                  case FIELDSKILLRATE_CLASSIC:
                     cr.train(SKILL_SECURITY, MAX(5 - cr.get_skill(SKILL_SECURITY), 0));break;
                  case FIELDSKILLRATE_HARD:
                     cr.train(SKILL_SECURITY, 0);break;
               }
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,0);
               addstr(cr.name, gamelog);
               addstr(" jimmies the car door open.", gamelog);
               gamelog.nextMessage();

               getkey();

               entered=true;
            }
            else
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,0);
               addstr(cr.name, gamelog);
               addstr(" fiddles with the lock with no luck.", gamelog);
               gamelog.nextMessage();

               getkey();
            }
         }
         //BREAK WINDOW
         if(method==1)
         {
            int difficulty = static_cast<int>(DIFFICULTY_EASY / cr.get_weapon().get_bashstrengthmod()) - windowdamage;

            if(cr.attribute_check(ATTRIBUTE_STRENGTH,difficulty))
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,0);
               addstr(cr.name, gamelog);
               addstr(" smashes the window", gamelog);
               if(cr.get_weapon().get_bashstrengthmod()>1)
               {
                  addstr(" with a ", gamelog);
                  addstr(cr.get_weapon().get_name(2), gamelog);
               }
               addstr(".", gamelog);
               gamelog.nextMessage();
               windowdamage=10;

               getkey();

               entered=true;
            }
            else
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,0);
               addstr(cr.name, gamelog);
               addstr(" cracks the window", gamelog);
               if(cr.get_weapon().get_bashstrengthmod()>1)
               {
                  addstr(" with a ", gamelog);
                  addstr(cr.get_weapon().get_name(2), gamelog);
               }
               addstr(" but it is still somewhat intact.", gamelog);
               gamelog.nextMessage();
               windowdamage++;

               getkey();
            }
         }

         //ALARM CHECK
         int y=17;

         if(touchalarm||sensealarm)
         {
            if(!alarmon)
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               move(y++,0);
               addstr("An alarm suddenly starts blaring!", gamelog);
               gamelog.nextMessage();

               getkey();

               alarmon=true;
            }
         }

         //NOTICE CHECK
         if(!LCSrandom(50)||(!LCSrandom(5)&&alarmon))
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            move(y++,0);
            addstr(cr.name, gamelog);
            addstr(" has been spotted by a passerby!", gamelog);
            gamelog.nextMessage();

            getkey();

            //FOOT CHASE
            chaseseq.clean();
            chaseseq.location=location[cr.location]->parent;
            newsstoryst *ns=new newsstoryst;
            ns->type=NEWSSTORY_CARTHEFT;
            newsstory.push_back(ns);
            sitestory=ns;
            makechasers(-1,5);

            if(footchase(cr)){
               mode=GAMEMODE_BASE;
               delete v;return false;} // Switched to return false; this will cause you to try again tomorrow
            else {
               mode=GAMEMODE_BASE;
               delete v;return false;}
         }
      }

      //START CAR
      char keys_in_car=LCSrandom(5)>0,key_search_total=0;
      int key_location=LCSrandom(5),nervous_counter=0;
      //char ignition_progress=0;

      for(bool started=false;!started;)
      {
         nervous_counter++;
         erase();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(0,0);
         addstr("Adventures in Liberal Car Theft");
         printcreatureinfo(&cr);
         makedelimiter();

         int y=10;

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y++,0);
         addstr(cr.name, gamelog);
         addstr(" is behind the wheel of a ", gamelog);
         addstr(carname, gamelog);
         addstr(".", gamelog);
         gamelog.nextMessage();

         if(alarmon)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(y++,0);
            if(sensealarm)addstr("THE VIPER");
            else addstr(carname);
            addstr(":   ");
            set_color(COLOR_RED,COLOR_BLACK,1);
            if(sensealarm)addstr("REMOVE YOURSELF FROM THE VEHICLE!   <BEEP!!> <BEEP!!>");
            else addstr("<BEEP!!> <BEEP!!> <BEEP!!> <BEEP!!>");
         }

         move((++y)++,0);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr("A - Hotwire the car.");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y++,0);
         addstr("B - Desperately search for keys.");
         move(y++,0);
         if(!sensealarm)addstr("Enter - Call it a day.");
         else {addstr("Enter - The Viper has finally deterred ");addstr(cr.name);addstr(".");}
         y++;

         char method=-1;
         while(method==-1)
         {
            int c=getkey();
            if(c=='a') method=0;
            if(c=='b') method=1;
            if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR){delete v;return false;} // Call it a day and try again tomorrow
         }

         //HOTWIRE CAR
         if(method==0)
         {
            if(cr.skill_check(SKILL_SECURITY,DIFFICULTY_CHALLENGING))
            {
               switch (fieldskillrate)
               {
                  case FIELDSKILLRATE_FAST:
                     cr.train(SKILL_SECURITY, 50);break;
                  case FIELDSKILLRATE_CLASSIC:
                     cr.train(SKILL_SECURITY, MAX(10 - cr.get_skill(SKILL_SECURITY), 0));break;
                  case FIELDSKILLRATE_HARD:
                     cr.train(SKILL_SECURITY, 0);break;
               }
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(y++,0);
               addstr(cr.name, gamelog);
               addstr(" hotwires the car!", gamelog);
               gamelog.nextMessage();

               getkey();

               started=true;
            }
            else
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(y++,0);
               addstr(cr.name, gamelog);
               int flavor_text;
               if(cr.get_skill(SKILL_SECURITY) < 4)
                  flavor_text = LCSrandom(3);
               else flavor_text = LCSrandom(5);
               switch(flavor_text)
               {
                  case 0:addstr(" fiddles with the ignition, but the car doesn't start.", gamelog);break;
                  case 1:addstr(" digs around in the steering column, but the car doesn't start.", gamelog);break;
                  case 2:addstr(" touches some wires together, but the car doesn't start.", gamelog);break;
                  case 3:addstr(" makes something in the engine click, but the car doesn't start.", gamelog);break;
                  case 4:addstr(" manages to turn on some dash lights, but the car doesn't start.", gamelog);break;
               }
               gamelog.nextMessage();

               getkey();
            }
         }
         //KEYS
         if(method==1)
         {
            int difficulty;
            const char * location;

            if(!keys_in_car)
            {
               difficulty = DIFFICULTY_IMPOSSIBLE;
               location   = "in SPACE. With ALIENS. Seriously.";
            }
            else switch(key_location)
            {
            case 0:
            default:
               difficulty = DIFFICULTY_AUTOMATIC;
               location   = "in the ignition.  Damn.";
               break;
            case 1:
               difficulty = DIFFICULTY_EASY;
               location   = "above the pull-down sunblock thingy!";
               break;
            case 2:
               difficulty = DIFFICULTY_EASY;
               location   = "in the glove compartment!";
               break;
            case 3:
               difficulty = DIFFICULTY_AVERAGE;
               location   = "under the front seat!";
               break;
            case 4:
               difficulty = DIFFICULTY_HARD;
               location   = "under the back seat!";
               break;
            }
            if(cr.attribute_check(ATTRIBUTE_INTELLIGENCE,difficulty))
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(y++,0);
               if (law[LAW_FREESPEECH] == Alignment::ARCH_CONSERVATIVE)
                 addstr("Holy [Car Keys]!  ", gamelog); // Holy car keys Batman!
               else addstr("Holy shit!  ", gamelog);
               addstr(cr.name, gamelog);
               addstr(" found the keys ", gamelog);
               addstr(location, gamelog);
               gamelog.nextMessage();

               getkey();

               started=true;
            }
            else
            {
               key_search_total++;
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(y++,0);
               addstr(cr.name, gamelog);
               addstr(": <rummaging> ", gamelog);
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               if(key_search_total==5)
                  addstr("Are they even in here?", gamelog);
               else if(key_search_total==10)
                  addstr("I don't think they're in here...", gamelog);
               else if(key_search_total==15)
                  addstr("If they were here, I'd have found them by now.", gamelog);
               else if (key_search_total>15)
               {
                  switch(LCSrandom(5))
                  {
                  case 0:addstr("This isn't working!", gamelog);break;
                  case 1:addstr("Why me?", gamelog);break;
                  case 2:addstr("What do I do now?", gamelog);break;
                  case 3:addstr("Oh no...", gamelog);break;
                  case 4:addstr("I'm going to get arrested, aren't I?", gamelog);break;
                  }
               }
               else
               {
                  switch(LCSrandom(5))
                  {
                     case 0:addstr("Please be in here somewhere...", gamelog);break;
                     case 1:
                        if (law[LAW_FREESPEECH] == Alignment::ARCH_CONSERVATIVE)
                          addstr("[Shoot]!  Where are they?!", gamelog);
                        else addstr("Fuck!  Where are they?!", gamelog);
                        break;
                     case 2:addstr("Come on, baby, come to me...", gamelog);break;
                     case 3:
                        if (law[LAW_FREESPEECH] == Alignment::ARCH_CONSERVATIVE)
                          addstr("[Darn] it...", gamelog);
                        else addstr("Dammit...", gamelog);
                        break;
                     case 4:addstr("I wish I could hotwire this thing...", gamelog);break;
                  }
               }
               gamelog.nextMessage();

               getkey();
            }
         }

         //NOTICE CHECK
         if(!started&&(!LCSrandom(50)||(!LCSrandom(5)&&alarmon)))
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            move(y++,0);
            addstr(cr.name, gamelog);
            addstr(" has been spotted by a passerby!", gamelog);
            gamelog.nextMessage();

            getkey();

            //FOOT CHASE
            chaseseq.clean();
            chaseseq.location=location[cr.location]->parent;
            newsstoryst *ns=new newsstoryst;
            ns->type=NEWSSTORY_CARTHEFT;
            newsstory.push_back(ns);
            sitestory=ns;
            makechasers(-1,5);

            if(footchase(cr)){
               mode=GAMEMODE_BASE;
               delete v;return 0;}
            else {
               mode=GAMEMODE_BASE;
               delete v;return 0;}
         }

         // Nervous message check
         else if (!started&&(LCSrandom(7)+5)<nervous_counter)
         {
            nervous_counter=0;
            move(++y,0);y++;
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            addstr(cr.name, gamelog);
            switch(LCSrandom(3))
            {
               case 0:addstr(" hears someone nearby making a phone call.", gamelog);break;
               case 1:addstr(" is getting nervous being out here this long.", gamelog);break;
               case 2:addstr(" sees a police car driving around a few blocks away.", gamelog);break;
            }
            gamelog.nextMessage();

            getkey();
         }
      }

      //CHASE SEQUENCE
         //CAR IS OFFICIAL, THOUGH CAN BE DELETE BY chasesequence()
      addjuice(cr,v->steal_juice(),100);

      vehicle.push_back(v);
      v->add_heat(14+v->steal_extraheat());
      v->set_location(cr.base);
      // Automatically assign this car to this driver, if no other one is present
      if(cr.pref_carid==-1)
      {
         cr.pref_carid = v->id();
         cr.pref_is_driver = true;
      }

      chaseseq.clean();
      chaseseq.location=location[cr.location]->parent;
      int chaselev=!LCSrandom(13-windowdamage);
      if(chaselev>0||(v->vtypeidname()=="POLICECAR"&&LCSrandom(2))) //Identify police cruiser. Temporary solution? -XML
      {
         v->add_heat(10);
         chaselev=1;
         newsstoryst *ns=new newsstoryst;
         ns->type=NEWSSTORY_CARTHEFT;
         newsstory.push_back(ns);
         sitestory=ns;
         makechasers(-1,chaselev);
         if(!chasesequence(cr,*v)) // Caught or killed in the chase. Do not need to delete vehicle.
            return 0;
      }

      return 1;
   }

   return 0;
}

bool carselect(Creature &cr,short &cartype)
{
   cartype=-1;

   vector<int> cart;
   for(int a=0;a<len(vehicletype);a++)
      if(vehicletype[a]->steal_difficultytofind()<10) cart.push_back(a);

   int page=0;

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("What type of car will ");
      addstr(cr.name);
      addstr(" try to find and steal today?");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(1,0);
      addstr("ÄÄÄÄTYPEÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄDIFFICULTY TO FIND UNATTENDEDÄÄ");

      int y=2,difficulty;
      for(int p=page*19;p<len(cart)&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addchar(y+'A'-2);addstr(" - ");
         addstr(vehicletype[cart[p]]->longname());

         move(y++,49);
         difficulty=vehicletype[cart[p]]->steal_difficultytofind();
         switch(difficulty)
         {
            case 0:
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               addstr("Simple");
               break;
            case 1:
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               addstr("Very Easy");
               break;
            case 2:
               set_color(COLOR_CYAN,COLOR_BLACK,0);
               addstr("Easy");
               break;
            case 3:
               set_color(COLOR_BLUE,COLOR_BLACK,1);
               addstr("Below Average");
               break;
            case 4:
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               addstr("Average");
               break;
            case 5:
               set_color(COLOR_WHITE,COLOR_BLACK,0);
               addstr("Above Average");
               break;
            case 6:
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               addstr("Hard");
               break;
            case 7:
               set_color(COLOR_MAGENTA,COLOR_BLACK,0);
               addstr("Very Hard");
               break;
            case 8:
               set_color(COLOR_MAGENTA,COLOR_BLACK,1);
               addstr("Extremely Difficult");
               break;
            case 9:
               set_color(COLOR_RED,COLOR_BLACK,0);
               addstr("Nearly Impossible");
               break;
            default:
               set_color(COLOR_RED,COLOR_BLACK,1);
               addstr("Impossible");
               break;
         }
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Type of Car");
      move(23,0);
      addpagestr();

      int c=getkey();

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0) page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<len(cart)) page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+c-'a';
         if(p<len(cart))
         {
            cartype=cart[p];
            return true;
         }
      }

      // Too easy to accidentally back out
      // Not a big problem if this page isn't skippable
      // (There's no immediate risk in picking a car)
      // - JDS
      //if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR)break;
   }

   return false;
}

/* get a wheelchair */
void getwheelchair(Creature &cr,char &clearformess)
{
   if(clearformess) erase();
   else makedelimiter();

   if(LCSrandom(2))
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(cr.name, gamelog);
      addstr(" has procured a wheelchair.", gamelog);

      cr.flag|=CREATUREFLAG_WHEELCHAIR;
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(cr.name, gamelog);
      addstr(" was unable to get a wheelchair.  Maybe tomorrow...", gamelog);
   }
   gamelog.nextMessage();

   getkey();
}
