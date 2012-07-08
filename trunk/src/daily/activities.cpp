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
      case 7:addstr("an embarrasing");break;
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
      switch(LCSrandom(1))
      {
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
      case 0:addstr("a standard");break;
      //case 16:addstr("an everyday");break;
      //case 17:addstr("a stock");break;
      //case 18:addstr("a stereotypical");break;
      //case 19:addstr("an okay");break;
      //case 8:addstr("a respectable");break;
      }
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

   if(cr.get_armor().is_bloody() || cr.get_armor().is_damaged())
   {
      armor=&cr.get_armor();
   }
   else if(cr.squadid!=-1)
   {
      int sq=getsquad(cr.squadid);
      for(int l=0;l<squad[sq]->loot.size();l++)
      {
         if(squad[sq]->loot[l]->is_armor())
         {
            Armor* a = static_cast<Armor*>(squad[sq]->loot[l]); //cast -XML
            if (a->is_bloody() || a->is_damaged())
            {
               armor=a;
               pile=squad[sq]->loot[l];
               pilelist=&squad[sq]->loot;
               break;
            }
         }
      }
   }
   if(armor==NULL&&cr.location!=-1)
   {
      for(int l=0;l<location[cr.location]->loot.size();l++)
      {
         if(location[cr.location]->loot[l]->is_armor())
         {
            Armor* a = static_cast<Armor*>(location[cr.location]->loot[l]);//cast -XML
            if(a->is_bloody() || a->is_damaged())
            {
               armor=a;
               pile=location[cr.location]->loot[l];
               pilelist=&location[cr.location]->loot;
               break;
            }
         }
      }
   }

   if(armor!=NULL)
   {
      if(clearformess)erase();
      else
      {
         makedelimiter(8,0);
      }

      bool repairfailed=false;

      if(armor->is_damaged())
      {
         long dif=armor_makedifficulty(*armor,&cr);
         dif>>=1;
         cr.train(SKILL_TAILORING,dif+1);

         if(LCSrandom(1+dif/2))
         {
            repairfailed=true;
         }
      }

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(cr.name);
      if(armor->is_damaged())
      {
         if(repairfailed)
         {
            addstr(" is working to repair ");
         }
         else addstr(" repairs ");
      }
      else addstr(" cleans ");
      char str[80];

      addstr(armor->get_name().c_str());
      addstr(".");

      if(pile!=NULL)
      {
         if(pile->get_number()>1)
         {
            Item *newpile=pile->split(pile->get_number()-1);
            pilelist->push_back(newpile);
         }
      }

      armor->set_bloody(false);
      if(!repairfailed)armor->set_damaged(false);

      refresh();
      getch();
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
      if(clearformess)erase();
      else
      {
         makedelimiter(8,0);
      }

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(cr.name);
      addstr(" cannot afford material for clothing.");

      refresh();
      getch();
      return;
   }
   else
   {
      char foundcloth=0;

      if(cr.squadid!=-1)
      {
         int sq=getsquad(cr.squadid);
         for(int l=0;l<squad[sq]->loot.size();l++)
         {
            if(squad[sq]->loot[l]->is_loot()&&
               static_cast<Loot*>(squad[sq]->loot[l])->is_cloth()) //cast -XML
            {
               if(squad[sq]->loot[l]->get_number()==1)
               {
                  delete squad[sq]->loot[l];
                  squad[sq]->loot.erase(squad[sq]->loot.begin() + l);
               }
               else squad[sq]->loot[l]->decrease_number(1);
               foundcloth=1;
               break;
            }
         }
      }
      if(!foundcloth)
      {
         for(int l=0;l<location[cr.location]->loot.size();l++)
         {
            if(location[cr.location]->loot[l]->is_loot()&&
               static_cast<Loot*>(location[cr.location]->loot[l])->is_cloth()) //cast -XML
            {
               if(location[cr.location]->loot[l]->get_number()==1)
               {
                  delete location[cr.location]->loot[l];
                  location[cr.location]->loot.erase(location[cr.location]->loot.begin() + l);
               }
               else location[cr.location]->loot[l]->decrease_number(1);
               foundcloth=1;
               break;
            }
         }
      }

      if(!foundcloth&&ledger.get_funds()<cost)
      {
         if(clearformess)erase();
         else
         {
            makedelimiter(8,0);
         }

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);
         addstr(cr.name);
         addstr(" cannot find enough cloth to reduce clothing costs.");

         refresh();
         getch();
      }
      else
      {
         if(foundcloth)
         {
            ledger.subtract_funds(hcost,EXPENSE_MANUFACTURE);
         }
         else
         {
            ledger.subtract_funds(cost,EXPENSE_MANUFACTURE);
         }

         cr.train(SKILL_TAILORING,dif*2+1);

         int quality = 1;
         if(!cr.skill_check(SKILL_TAILORING,dif))
         {
            quality=2;
            if(!cr.skill_check(SKILL_TAILORING,dif))
            {
               quality=3;
               if(!cr.skill_check(SKILL_TAILORING,dif))
               {
                  quality=4;
               }
            }
         }

         Item *it=new Armor(*armortype[at],quality);
         location[cr.location]->loot.push_back(it);


         if(clearformess)erase();
         else
         {
            makedelimiter(8,0);
         }

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);
         addstr(cr.name);
         addstr(" has made a ");
         switch(quality)
         {
            case 1:addstr("first-rate");break;
            case 2:addstr("second-rate");break;
            case 3:addstr("third-rate");break;
            case 4:addstr("fourth-rate");break;
         }
         addstr(" ");
         addstr(it->get_name().c_str());
         addstr(".");

         refresh();
         getch();
      }
   }
}



/* search for polls */
void survey(Creature *cr)
{
   static const char SURVEY_PAGE_SIZE=14;

   int v;
   int creatureskill=cr->skill_roll(SKILL_COMPUTERS);
   int misschance=30-creatureskill,noise=2;
   if(misschance<5)misschance=5;
   if(creatureskill<3){noise=15;}
   else if(creatureskill<7){noise=7;}
   else if(creatureskill<10){noise=5;}
   else if(creatureskill<15){noise=4;}
   else if(creatureskill<20){noise=3;}

   int survey[VIEWNUM];

   int maxview=-1;
   for(v=0;v<VIEWNUM;v++)
   {
      survey[v]=attitude[v];
      if(v!=VIEW_LIBERALCRIMESQUAD&&v!=VIEW_LIBERALCRIMESQUADPOS/*&&v!=VIEW_POLITICALVIOLENCE*/)
      {
         if(maxview!=-1)
         {
            if(public_interest[v]>public_interest[maxview])maxview=v;
         }
         else
         {
            if(public_interest[v]>0)maxview=v;
         }
      }

      //MAKE SURVEY ACCURATE IF DEBUGGING
      #ifndef SHOWMECHANICS
      do{ survey[v]+=LCSrandom(noise*2+1)-noise; }while(!LCSrandom(20));
      #endif

      if(survey[v]<0)survey[v]=0;
      if(survey[v]>100)survey[v]=100;

      #ifndef SHOWMECHANICS
      if(LCSrandom(public_interest[v]+100)<int(misschance))survey[v]=-1;
      #endif

      if(v==VIEW_LIBERALCRIMESQUAD&&attitude[v]==0)survey[v]=-1;
      if(v==VIEW_LIBERALCRIMESQUADPOS&&survey[VIEW_LIBERALCRIMESQUAD]<=0)survey[v]=-1;
   }

   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   addstr("Survey of Public Opinion, According to Recent Polls");

   int y=8;

   char num[20];

   int approval=presidentapproval();
   move(2,0);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   itoa(approval/10+(LCSrandom(noise*2+1)-noise),num,10);
   addstr(num);
   addstr("% had a favorable opinion of ");
   switch(exec[EXEC_PRESIDENT])
   {
   case -2:
      set_color(COLOR_RED,COLOR_BLACK,1);
      break;
   case -1:
      set_color(COLOR_MAGENTA,COLOR_BLACK,1);
      break;
   case 0:
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      break;
   case 1:
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      break;
   case 2:
      set_color(COLOR_GREEN,COLOR_BLACK,1);
      break;
   }
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
            if(attitude[VIEW_GAY]>50)addstr("protecting gay rights.");
            else addstr("protecting the traditional family.");
            break;
         case VIEW_DEATHPENALTY:
            if(attitude[VIEW_DEATHPENALTY]>50)addstr("the unjust death penalty.");
            else
            {
               if(law[LAW_DEATHPENALTY]==2)
               {
                  addstr("restoring the death penalty.");
               }
               else
               {
                  addstr("protecting the death penalty.");
               }
            }
            break;
         case VIEW_TAXES:
            if(attitude[VIEW_TAXES]>50)addstr("the oppressive tax structure.");
            else addstr("the excessive tax burden.");
            break;
         case VIEW_NUCLEARPOWER:
            if(attitude[VIEW_NUCLEARPOWER]>50)addstr("the dangers of nuclear power.");
            else
            {
               if(law[LAW_NUCLEARPOWER]==2)
               {
                  addstr("legalizing nuclear power.");
               }
               else
               {
                  addstr("threats to nuclear power.");
               }
            }
            break;
         case VIEW_ANIMALRESEARCH:
            if(attitude[VIEW_ANIMALRESEARCH]>50)addstr("brutal animal research practices.");
            else addstr("excessive regulation of animal research.");
            break;
         case VIEW_POLICEBEHAVIOR:
            if(attitude[VIEW_POLICEBEHAVIOR]>50)addstr("preventing police brutality.");
            else addstr("expanding police powers.");
            break;
         case VIEW_INTELLIGENCE:
            if(attitude[VIEW_INTELLIGENCE]>50)addstr("civil liberties and personal privacy.");
            else addstr("national security and intelligence.");
            break;
         case VIEW_FREESPEECH:
            if(attitude[VIEW_FREESPEECH]>50)addstr("protecting free speech.");
            else addstr("ending hate speech.");
            break;
         case VIEW_GENETICS:
            if(attitude[VIEW_GENETICS]>50)addstr("the dangers of genetic engineering.");
            else addstr("excessive regulation of genetic research.");
            break;
         case VIEW_JUSTICES:
            if(attitude[VIEW_JUSTICES]>50)addstr("appointing proper Liberal justices.");
            else addstr("appointing proper Conservative justices.");
            break;
         case VIEW_SWEATSHOPS:
            if(attitude[VIEW_SWEATSHOPS]>50)addstr("threats to labor rights.");
            else addstr("excessive regulation of labor practices.");
            break;
         case VIEW_POLLUTION:
            if(attitude[VIEW_POLLUTION]>50)addstr("threats to the environment.");
            else addstr("excessive regulation of industry.");
            break;
         case VIEW_CORPORATECULTURE:
            if(attitude[VIEW_CORPORATECULTURE]>50)addstr("corporate corruption.");
            else addstr("excessive regulation of corporations.");
            break;
         case VIEW_CEOSALARY:
            if(attitude[VIEW_CEOSALARY]>50)addstr("severe income inequality.");
            else addstr("resisting communist wage limits.");
            break;
         //case VIEW_POLITICALVIOLENCE:
         //   if(attitude[VIEW_POLITICALVIOLENCE]>50)addstr("taking strong action.");
         //   else addstr("political terrorism.");
         //   break;
         case VIEW_IMMIGRATION:
            if(attitude[VIEW_IMMIGRATION]>50)addstr("immigrant rights.");
                        else
                        {
                                if(law[LAW_IMMIGRATION]>=1)
                                {
                                        addstr("uncontrolled immigration.");
                                }
                                else
                                {
                                        addstr("illegal immigration.");
                                }
                        }
            break;
         case VIEW_DRUGS:
            if(attitude[VIEW_DRUGS]>50)addstr("drug rights.");
            else addstr("drug abuse.");
            break;
         case VIEW_WOMEN:
            if(attitude[VIEW_WOMEN]>50)addstr("women's equality.");
            else addstr("women.");
            break;
         case VIEW_CIVILRIGHTS:
            if(attitude[VIEW_CIVILRIGHTS]>50)addstr("civil rights.");
            else addstr("troublemaking minorities.");
            break;
                 case VIEW_GUNCONTROL:
            if(attitude[VIEW_GUNCONTROL]>50)addstr("gun violence.");
            else addstr("protecting the Second Amendment.");
            break;
         case VIEW_MILITARY:
            if(attitude[VIEW_MILITARY]>50)addstr("the large military.");
            else addstr("strengthening the military.");
            break;
         case VIEW_LIBERALCRIMESQUAD:
         case VIEW_LIBERALCRIMESQUADPOS:
            if(attitude[VIEW_LIBERALCRIMESQUAD]<50)
            {
               addstr("activist political groups.");
               break;
            }
            if(attitude[VIEW_LIBERALCRIMESQUADPOS]>50)addstr("the Liberal Crime Squad.");
            else addstr("the LCS terrorists.");
            break;
         case VIEW_CONSERVATIVECRIMESQUAD:
            if(attitude[VIEW_CONSERVATIVECRIMESQUAD]<50)
            {
               addstr("the Conservative Crime Squad.");
               break;
            }
            else addstr("the CCS terrorists.");
            break;
         case VIEW_TORTURE:
            if(attitude[VIEW_TORTURE]>50)addstr("ending the use of torture.");
            else addstr("enhancing interrogations.");
            break;
         case VIEW_AMRADIO:
         case VIEW_CABLENEWS:
            if(attitude[VIEW_AMRADIO]+attitude[VIEW_CABLENEWS]>100)addstr("Conservative Media Bias.");
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
   addstr("XX% Issue ------------------------------------------------- Public Interest");

   //Footer
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(23,0);
   addstr("Results are +/- ");
   itoa(noise,num,10);
   addstr(num);
   addstr(" Liberal percentage points.");
   move(24,0);
   addstr("Enter - Done");
   move(24,40);
   addprevpagestr();
   addstr("    ");
   addnextpagestr();

   int page=0;
   const int maxpage=VIEWNUM/SURVEY_PAGE_SIZE;
   while(1)
   {
      //Keep pages within bounds
      if(page<0)page=maxpage;
      if(page>maxpage)page=0;
      //Start from the top
      y=8;
      //Draw each line
      for(v=page*SURVEY_PAGE_SIZE;v<(page+1)*SURVEY_PAGE_SIZE;v++)
      {
         if(v>=VIEWNUM || (v==VIEW_CONSERVATIVECRIMESQUAD && (endgamestate>=ENDGAME_CCS_DEFEATED||newscherrybusted<2)))
         {
            move(y++,0);
            addstr("                                                                                ");
            continue;
         }
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,4);
         addstr("........................................................");
         if(noise>=7||survey[v]==-1)
         {
            addstr("Unknown  ");
         }
         else if(noise>=4)
         {
            if(public_interest[v]>50)
            {
               addstr("High     ");
            }
            else
            {
               addstr("Low      ");
            }
         }
         else
         {
            if(public_interest[v]>100)
            {
               addstr("Very High");
            }
            else if(public_interest[v]>50)
            {
               addstr("High     ");
            }
            else if(public_interest[v]>10)
            {
               addstr("Moderate ");
            }
            else if(public_interest[v])
            {
               addstr("Low      ");
            }
            else
            {
               addstr("None     ");
            }
         }

         if(survey[v]==-1)set_color(COLOR_BLACK,COLOR_BLACK,1);
         else if(survey[v]<10)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(survey[v]<30)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(survey[v]<50)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(survey[v]<70)set_color(COLOR_BLUE,COLOR_BLACK,1);
         else if(survey[v]<90)set_color(COLOR_CYAN,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);

         move(y,0);
         if(survey[v]==-1)
         {
            addstr("??");
         }
         else
         {
            itoa(survey[v],num,10);
            if(strlen(num)==1)addstr("0");
            addstr(num);
         }
         addstr("% ");

         switch(v)
         {
            case VIEW_GAY:addstr("were in favor of equal rights for homosexuals");break;
            case VIEW_DEATHPENALTY:addstr("opposed the death penalty");break;
            case VIEW_TAXES:addstr("were against cutting taxes");break;
            case VIEW_NUCLEARPOWER:addstr("were terrified of nuclear power");break;
            case VIEW_ANIMALRESEARCH:addstr("deplored animal research");break;
            case VIEW_POLICEBEHAVIOR:addstr("were critical of the police");break;
            case VIEW_TORTURE:addstr("wanted stronger measures to prevent torture");break;
            case VIEW_INTELLIGENCE:addstr("thought the intelligence community invades privacy");break;
            case VIEW_FREESPEECH:addstr("believed in unfettered free speech");break;
            case VIEW_GENETICS:addstr("abhorred genetically altered food products");break;
            case VIEW_JUSTICES:addstr("were for the appointment of Liberal justices");break;
            case VIEW_SWEATSHOPS:addstr("would boycott companies that used sweatshops");break;
            case VIEW_POLLUTION:addstr("thought industry should lower pollution");break;
            case VIEW_CORPORATECULTURE:addstr("were disgusted by corporate malfeasance");break;
            case VIEW_CEOSALARY:addstr("believed that CEO salaries are too great");break;
            case VIEW_WOMEN:addstr("favored doing more for gender equality");break;
            case VIEW_CIVILRIGHTS:addstr("felt more work was needed for racial equality");break;
            case VIEW_GUNCONTROL:addstr("are concerned about gun violence");break;
            case VIEW_DRUGS:
                                        if(law[LAW_DRUGS]>=1)addstr("supported keeping marijuana legal");
                                        else addstr("believed in legalizing marijuana");
                                        break;
            case VIEW_IMMIGRATION:
                                        if(law[LAW_IMMIGRATION]>=1)addstr("condemned unnecessary immigration regulations");
                                        else addstr("wanted amnesty for illegal immigrants");
                                        break;
            case VIEW_MILITARY:addstr("opposed increasing military spending");break;
            case VIEW_LIBERALCRIMESQUAD:addstr("respected the power of the Liberal Crime Squad");break;
            case VIEW_LIBERALCRIMESQUADPOS:addstr("of these held the Liberal Crime Squad in high regard");break;
            case VIEW_CONSERVATIVECRIMESQUAD:addstr("held the Conservative Crime Squad in contempt");break;
            //case VIEW_PRISONS:addstr("wanted to end prisoner abuse and torture");break;
            case VIEW_AMRADIO:addstr("do not like AM radio");break;
            case VIEW_CABLENEWS:addstr("have a negative opinion of cable news programs");break;
            //case VIEW_POLITICALVIOLENCE:addstr("thought political violence was justified");break;
         }
         y++;
      }

      while(1)
      {
         refresh();
         char key=getch();
         if(key==10)return;
         else if(key==interface_pgup)
         {
            page--;
            break;
         }
         else if(key==interface_pgdn)
         {
            page++;
            break;
         }
      }
   }


}

// Police accost your liberal!
void attemptarrest(Creature & liberal,const char* string,int clearformess)
{
   if(string)
   {
      if(clearformess)erase();
      else
      {
         makedelimiter(8,0);
      }

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr("Police are attempting to arrest ");
      addstr(liberal.name);
      addstr(" while ");
      addstr(string);
      addstr(".");

      refresh();
      getch();
   }

   // Chase sequence! Wee!
   makechasers(-1,5);

   chaseseq.clean();
   chaseseq.location=0;
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

   if(arrest)
   {
      attemptarrest(liberal,string,clearformess);
   }
   return arrest;
}



/* misc activation related things */
// *JDSRETURN*
void funds_and_trouble(char &clearformess)
{
   int s;
   //FIND A POLICE STATION
   //and a clinic too
   //and a homeless shelter three!
   int ps=-1;
   int clinic=-1;
   int shelter=-1;
   for(int l=0;l<location.size();l++)
   {
      if(location[l]->type==SITE_GOVERNMENT_POLICESTATION)
      {
         ps=l;
      }
      if(location[l]->type==SITE_HOSPITAL_CLINIC)
      {
         clinic=l;
      }
      if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
      {
         shelter=l;
      }
   }

   //ACTIVITIES FOR INDIVIDUALS
   vector<Creature *> trouble;
   vector<Creature *> hack;
   vector<Creature *> bury;
   vector<Creature *> solicit;
   vector<Creature *> tshirts;
   vector<Creature *> art;
   vector<Creature *> music;
   vector<Creature *> graffiti;
   vector<Creature *> brownies;
   vector<Creature *> prostitutes;
   vector<Creature *> teachers;
   vector<Creature *> students;

   for(int p=0;p<pool.size();p++)
   {
      if(!pool[p]->alive)continue;
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
            addjuice(*pool[p],1,0);
            if(pool[p]->heat && !LCSrandom(3))pool[p]->heat--;
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
            hospitalize(clinic,*pool[p]);
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
            students.push_back(pool[p]);
            break;
         case ACTIVITY_SLEEPER_JOINLCS:
            if(!location[shelter]->siege.siege)
            {
               pool[p]->activity.type=ACTIVITY_NONE;
               pool[p]->flag &= ~CREATUREFLAG_SLEEPER;
               pool[p]->location = pool[p]->base = shelter;
            }
      }
   }

   long money;

   //SOLICITORS
   int total_income=0;
   for(s=0;s<solicit.size();s++)
   {
      if(!checkforarrest(*solicit[s],"soliciting donations",clearformess))
      {
         int income=solicit[s]->skill_roll(SKILL_PERSUASION) *
                    solicit[s]->get_armor().get_professionalism()+1;
         
         // Country's alignment dramatically affects effectiveness
         // The more conservative the country, the more effective
         if(publicmood(-1) > 90)
            income /= 2;
         if(publicmood(-1) > 65)
            income /= 2;
         if(publicmood(-1) > 35)
            income /= 2;
         if(publicmood(-1) > 10)
            income /= 2;

         solicit[s]->income=income;

         total_income += income;

         solicit[s]->train(SKILL_PERSUASION,max(5-solicit[s]->get_skill(SKILL_PERSUASION),2));
      }
   }
   ledger.add_funds(total_income,INCOME_DONATIONS);

   //TSHIRTS
   int mood=publicmood(-1);
   for(s=0;s<tshirts.size();s++)
   {
      if(!checkforarrest(*tshirts[s],"selling shirts",clearformess))
      {
         money = (tshirts[s]->skill_roll(SKILL_TAILORING) +
                  tshirts[s]->skill_roll(SKILL_BUSINESS))/2;

         // Country's alignment affects effectiveness
         // In a Liberal country, there are many competing vendors
         if(publicmood(-1) > 65)
            money /= 2;
         if(publicmood(-1) > 35)
            money /= 2;

         //If you're selling epic shirts enough they'll have some political impact

         if(tshirts[s]->skill_check(SKILL_TAILORING,DIFFICULTY_FORMIDABLE))
            background_liberal_influence[randomissue()]+=5;



         tshirts[s]->income=money;
         ledger.add_funds(money,INCOME_TSHIRTS);

         tshirts[s]->train(SKILL_TAILORING,max(7-tshirts[s]->get_skill(SKILL_TAILORING),2));
         tshirts[s]->train(SKILL_BUSINESS,max(7-tshirts[s]->get_skill(SKILL_BUSINESS),2));
      }
   }

   //ART
   for(s=0;s<art.size();s++)
   {
      if(!checkforarrest(*art[s],"sketching portraits",clearformess))
      {
         money = art[s]->skill_roll(SKILL_ART);

         // Country's alignment affects effectiveness
         // In a Liberal country, there are many competing vendors
         if(publicmood(-1) > 65)
            money /= 2;
         if(publicmood(-1) > 35)
            money /= 2;

         //Epic Liberal art may have positive political effect
         if(art[s]->skill_check(SKILL_ART,DIFFICULTY_FORMIDABLE))
            background_liberal_influence[randomissue()]+=5;

         art[s]->income=money;
         ledger.add_funds(money,INCOME_SKETCHES);

         art[s]->train(SKILL_ART,max(7-art[s]->get_skill(SKILL_ART),4));
      }
   }

   //MUSIC
   for(s=0;s<music.size();s++)
   {
      if(!checkforarrest(*music[s],"playing music",clearformess))
      {
         money = music[s]->skill_roll(SKILL_MUSIC) / 2;

         bool has_instrument = music[s]->get_weapon().is_instrument();

         if(has_instrument)
            money *= 4;

         // Country's alignment affects effectiveness
         // In a Liberal country, there are many competing vendors
         if(publicmood(-1) > 65)
            money /= 2;
         if(publicmood(-1) > 35)
            money /= 2;

         //Epic Liberal protest songs
         if(music[s]->skill_check(SKILL_MUSIC,DIFFICULTY_FORMIDABLE))
            background_liberal_influence[randomissue()]+=has_instrument ? 10 : 5;

         ledger.add_funds(money,INCOME_BUSKING);
         music[s]->income=money;

         if(has_instrument)
            music[s]->train(SKILL_MUSIC,max(7-music[s]->get_skill(SKILL_MUSIC),4));
         else
            music[s]->train(SKILL_MUSIC,max(5-music[s]->get_skill(SKILL_MUSIC),2));
      }
   }

   //BROWNIES
   int dodgelawroll;
   for(s=0;s<brownies.size();s++)
   {
      //Check for police search
      dodgelawroll=LCSrandom(1+30*law[LAW_DRUGS]+3);

      //Saved by street sense?
      if(dodgelawroll==0)
         dodgelawroll=brownies[s]->skill_check(SKILL_STREETSENSE,DIFFICULTY_AVERAGE);

      if(dodgelawroll==0 && law[LAW_DRUGS]<=0) // Busted!
      {
         newsstoryst *ns=new newsstoryst;
            ns->type=NEWSSTORY_DRUGARREST;
            ns->loc=-1;
         newsstory.push_back(ns);
         sitestory=ns;

         criminalize(*brownies[s],LAWFLAG_BROWNIES);
         attemptarrest(*brownies[s],"selling brownies",clearformess);
      }

      money = brownies[s]->skill_roll(SKILL_PERSUASION) +
              brownies[s]->skill_roll(SKILL_BUSINESS) +
              brownies[s]->skill_roll(SKILL_STREETSENSE);

      // more money when more illegal
      if(law[LAW_DRUGS]==-2)
         money*=4;
      if(law[LAW_DRUGS]==-1)
         money*=2;
      if(law[LAW_DRUGS]==1)
         money/=4;
      if(law[LAW_DRUGS]==2)
         money/=8;

      brownies[s]->income=money;
      ledger.add_funds(money,INCOME_BROWNIES);

      // Make the sale
      brownies[s]->train(SKILL_PERSUASION,max(4-brownies[s]->get_skill(SKILL_PERSUASION),1));
      // Know the streets
      brownies[s]->train(SKILL_STREETSENSE,max(7-brownies[s]->get_skill(SKILL_STREETSENSE),3));
      // Manage your money
      brownies[s]->train(SKILL_BUSINESS,max(10-brownies[s]->get_skill(SKILL_BUSINESS),3));
   }

   //HACKING
   if(hack.size()>0)
   {
      int h;
      vector<Creature *> cc;
      vector<Creature *> web;
      vector<Creature *> ddos;
      vector<Creature *> truehack;

      //First, do accounting to figure out who's doing what
      for(h=0;h<hack.size();h++)
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

      char msg[80] = {0};

      //MAJOR HACKING
      int hack_skill=0;
      for(int h=0;h<truehack.size();h++)
      {
         hack_skill = MAX(hack_skill,truehack[h]->skill_roll(SKILL_COMPUTERS));
      }

      if(DIFFICULTY_HEROIC<=hack_skill+static_cast<int>(truehack.size())-1)
      {
         if(truehack.size()>1)strcpy(msg,"Your Hackers have ");
         else {strcpy(msg,truehack[0]->name);strcat(msg," has ");}

         int trackdif=0;
         int short crime=0;

         int juiceval=0;

         switch(LCSrandom(11))
         {
            case 0:
            {
               strcat(msg,"pilfered files from a Corporate server.");

               Item *it=new Loot(*loottype[getloottype("LOOT_CORPFILES")]);
               location[hack[0]->location]->loot.push_back(it);

               trackdif=DIFFICULTY_FORMIDABLE;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               break;
            }
            case 1: // *JDS* Penetrated government networks; don't get any loot, but do scare the info community
               strcat(msg,"caused a scare by breaking into a CIA network.");

               trackdif=DIFFICULTY_SUPERHEROIC;
               crime=LAWFLAG_INFORMATION;
               juiceval=25;
               change_public_opinion(VIEW_INTELLIGENCE,10,0,75);
               break;
            case 2:
               strcat(msg,"sabotaged a genetics research company's network.");

               trackdif=DIFFICULTY_FORMIDABLE;
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

               trackdif=DIFFICULTY_FORMIDABLE;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               break;
            }
            case 4:
               strcat(msg,"broke into military networks leaving LCS slogans.");

               trackdif=DIFFICULTY_SUPERHEROIC;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,5,0,75);
               break;
            case 5:
            {
               strcat(msg,"uncovered information on dangerous research.");

               Item *it=new Loot(*loottype[getloottype("LOOT_RESEARCHFILES")]);
               location[hack[0]->location]->loot.push_back(it);

               trackdif=DIFFICULTY_FORMIDABLE;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               break;
            }
            case 6:
            {
               strcat(msg,"discovered evidence of judicial corruption.");

               Item *it=new Loot(*loottype[getloottype("LOOT_JUDGEFILES")]);
               location[hack[0]->location]->loot.push_back(it);

               trackdif=DIFFICULTY_FORMIDABLE;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               break;
            }
            case 7:
            {
               strcat(msg,"subverted a Conservative family forum.");

               trackdif=DIFFICULTY_FORMIDABLE;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               change_public_opinion(VIEW_GAY,2,0,75);
               change_public_opinion(VIEW_WOMEN,2,0,75);
               break;
            }
            case 8:
            {
               strcat(msg,"spread videos of racist police brutality.");

               trackdif=DIFFICULTY_FORMIDABLE;
               juiceval=10;
               change_public_opinion(VIEW_POLICEBEHAVIOR,2,0,75);
               change_public_opinion(VIEW_CIVILRIGHTS,2,0,75);
               break;
            }
            case 9:
            {
               strcat(msg,"published emails revealing CEO tax evasion.");
               //Scambaiting, except you're baiting a CEO

               trackdif=DIFFICULTY_FORMIDABLE;
               juiceval=10;
               change_public_opinion(VIEW_CEOSALARY,2,0,75);
               change_public_opinion(VIEW_TAXES,2,0,75);
               break;
            }
            case 10:
            {
               strcat(msg,"revealed huge political bias in INS processes.");

               trackdif=DIFFICULTY_FORMIDABLE;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               change_public_opinion(VIEW_IMMIGRATION,2,0,75);
               change_public_opinion(VIEW_FREESPEECH,2,0,75);
               break;
            }

         }

         if(trackdif>hack_skill + LCSrandom(5)-2)
         {
            for(int h=0;h<truehack.size();h++)
            {
               criminalize(*hack[h],crime);
            }
         }

         // Award juice to the hacking team for a job well done
         for(int h=0;h<truehack.size();h++)
            addjuice(*truehack[h],juiceval,200);
      }
      else if(DIFFICULTY_FORMIDABLE<=hack_skill+static_cast<int>(truehack.size())-1)
      {
         int issue=LCSrandom(VIEWNUM-5);
         int crime;

         // Maybe do a switch on issue here to specify which website it was, but I don't feel like
         // doing that right now

         if(truehack.size()>1)strcpy(msg,"Your hackers have ");
         else {strcpy(msg,truehack[0]->name);strcat(msg," has ");}

         switch(LCSrandom(4))
         {
         case 0:strcat(msg,"defaced");crime=LAWFLAG_INFORMATION;break;
         case 1:strcat(msg,"knocked out");crime=LAWFLAG_COMMERCE;break;
         case 2:strcat(msg,"threatened");crime=LAWFLAG_SPEECH;break;
         case 3:strcat(msg,"hacked");crime=LAWFLAG_INFORMATION;break;
         }
         strcat(msg," a ");
         switch(LCSrandom(5))
         {
         case 0:strcat(msg,"corporate website");break;
         case 1:strcat(msg,"Conservative forum");break;
         case 2:strcat(msg,"Conservative blog");break;
         case 3:strcat(msg,"news website");break;
         case 4:strcat(msg,"government website");break;
         }
         strcat(msg,".");

         change_public_opinion(issue,1);

         if(DIFFICULTY_FORMIDABLE>hack_skill+LCSrandom(5)-2)
         {
            for(int h=0;h<truehack.size();h++)
            {
               criminalize(*truehack[h],crime);
            }
         }

         // Award juice to the hacking team for a job well done
         for(int h=0;h<truehack.size();h++)
            addjuice(*truehack[h],5,100);
      }

      if(msg[0])
      {
         if(clearformess)erase();
         else
         {
            makedelimiter(8,0);
         }

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);

         addstr(msg);
         msg[0]=0;

         refresh();
         getch();
      }

      //CREDIT CARD FRAUD
      for(int h=0;h<cc.size();h++)
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

			   cc[h]->income = fundgain / cc.size();

            if(fundgain/25>LCSrandom(hack_skill+1))
            {
               criminalize(*cc[h],LAWFLAG_CCFRAUD);
            }
         }

         if(msg[0])
         {
            if(clearformess)erase();
            else
            {
               makedelimiter(8,0);
            }

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);

            addstr(msg);
            msg[0]=0;

            refresh();
            getch();
         }
      }
   }

   //GRAFFITI
   if(graffiti.size()>=0)
   {
      for(s=0;s<graffiti.size();++s)
      {
         if(!graffiti[s]->get_weapon().can_graffiti())
         {

            if(clearformess)erase();
            else
            {
               makedelimiter(8,0);
            }

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(graffiti[s]->name);

            //Check base inventory for a spraycan
            bool foundone = false;
            for(int i=0; i<location[graffiti[s]->base]->loot.size(); ++i)
            {
               if(location[graffiti[s]->base]->loot[i]->is_weapon())
               {
                  Weapon *w = static_cast<Weapon*>(location[graffiti[s]->base]->loot[i]); //cast -XML
                  if(w->can_graffiti())
                  {
                     addstr(" grabbed a ");
                     addstr(w->get_name().c_str());
                     addstr(" from ");
                     addlocationname(location[graffiti[s]->base]);
                     addstr(".");
                     refresh();
                     getch();

                     graffiti[s]->give_weapon(*w,&(location[graffiti[s]->base]->loot));

                     if(location[graffiti[s]->base]->loot[i]->empty())
                     {
                        delete location[graffiti[s]->base]->loot[i];
                        location[graffiti[s]->base]->loot.erase(location[graffiti[s]->base]->loot.begin()+i);
                     }
                     foundone = true;
                     break;
                  }
               }
            }

            if(!foundone && ledger.get_funds()>=20)
            {
               ledger.subtract_funds(20,EXPENSE_SHOPPING);
               addstr(" bought spraypaint for graffiti.");
               refresh();
               getch();

               Weapon spray(*weapontype[getweapontype("WEAPON_SPRAYCAN")]);
               graffiti[s]->give_weapon(spray,&location[graffiti[s]->base]->loot);
            }
            else if (!foundone)
            {
               addstr(" needs a spraycan equipped to do graffiti.");
               graffiti[s]->activity.type=ACTIVITY_NONE;
               refresh();
               getch();
            }
         }

         int issue=VIEW_LIBERALCRIMESQUAD;
         int power=1;
         int caught=0;

         if(clearformess)erase();
         else
         {
            makedelimiter(8,0);
         }

         if(!LCSrandom(10) &&
            !(graffiti[s]->skill_check(SKILL_STREETSENSE,DIFFICULTY_AVERAGE)))
         {
            if(clearformess)erase();
            else
            {
               makedelimiter(8,0);
            }

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);

            addstr(graffiti[s]->name);
            addstr(" was spotted by the police");
            criminalize(*graffiti[s],LAWFLAG_VANDALISM);
            graffiti[s]->train(SKILL_STREETSENSE,20);

            if(graffiti[s]->activity.arg!=-1)
            {
               addstr(" while working on the mural!");
               graffiti[s]->activity.arg=-1;
            }
            else addstr(" while spraying an LCS tag!");

            newsstoryst *ns=new newsstoryst;
               ns->type=NEWSSTORY_GRAFFITIARREST;
               ns->loc=-1;
               ns->positive=0;
            newsstory.push_back(ns);
            sitestory=ns;

            refresh();
            getch();

            attemptarrest(*graffiti[s],NULL,clearformess);
         }
         else if(graffiti[s]->activity.arg!=-1)
         {
            power=0;
            if(!LCSrandom(3))
            {
               char issuestr[60];
               getview(issuestr,graffiti[s]->activity.arg);
               issue=graffiti[s]->activity.arg;
               power=graffiti[s]->skill_roll(SKILL_ART)/3;

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(graffiti[s]->name);
               addstr(" has completed a");
               if(power>3)addstr(" beautiful");
               addstr(" mural about ");
               addstr(issuestr);
               addstr(".");

               graffiti[s]->activity.arg=-1;
               addjuice(*graffiti[s],power,power*20);
               change_public_opinion(issue,power);
               graffiti[s]->train(SKILL_ART,MAX(10-graffiti[s]->get_skill(SKILL_ART)/2,1));
               refresh();
               getch();
            }
            else
            {
               power=0;
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(graffiti[s]->name);
               addstr(" works through the night on a large mural.");
               graffiti[s]->train(SKILL_ART,MAX(10-graffiti[s]->get_skill(SKILL_ART)/2,1));
               refresh();
               getch();
            }
         }
         else if(!LCSrandom(MAX(30-graffiti[s]->get_skill(SKILL_ART)*2,5)))
         {
            issue=randomissue();
            char issuestr[60];
            getview(issuestr,issue);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(graffiti[s]->name);
            addstr(" has begun work on a large mural about ");
            addstr(issuestr);
            addstr(".");
            graffiti[s]->activity.arg=issue;
            power=0;
            graffiti[s]->train(SKILL_ART,MAX(10-graffiti[s]->get_skill(SKILL_ART)/2,1));
            refresh();
            getch();
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

   //PROSTITUTION
   for(int p=prostitutes.size()-1;p>=0;p--)
   {
      // Business once every three days or so
      if(LCSrandom(3))continue;

      char num[20];

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
            if(clearformess)erase();
            else
            {
               makedelimiter(8,0);
            }

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(prostitutes[p]->name);
            addstr(" has been arrested in a prostitution sting.");

            addjuice(*prostitutes[p],-7,-30);

            refresh();
            getch();

            caught=1;

            removesquadinfo(*prostitutes[p]);
            prostitutes[p]->carid=-1;
            prostitutes[p]->location=ps;
            prostitutes[p]->drop_weapons_and_clips(NULL);
            prostitutes[p]->activity.type=ACTIVITY_NONE;
            criminalize(*prostitutes[p],LAWFLAG_PROSTITUTION);
         }
         else
         {
            if(clearformess)erase();
            else
            {
               makedelimiter(8,0);
            }

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(prostitutes[p]->name);
            addstr(" was nearly caught in a prostitution sting.");

            addjuice(*prostitutes[p],5,0);

            refresh();
            getch();
         }
      }

      if(!caught)
      {
         prostitutes[p]->train(SKILL_STREETSENSE,MAX(5-prostitutes[p]->get_skill(SKILL_STREETSENSE),0));
         ledger.add_funds(fundgain,INCOME_PROSTITUTION);
         prostitutes[p]->income = fundgain;
      }
   }

   for(int s=students.size()-1;s>=0;s--)
   {
	   if (ledger.get_funds()<60)
	   {
		   break;
	   }
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
	   }
	   bool worthcontinuing = false;
	   for (int i = 0; i < 2; i++)
	   {
		   if (skill[i] != -1)
		   {
			   // rapid decrease in effectiveness as your skill gets higher.
            effectiveness[i] /= (students[s]->get_skill(skill[i])+1);
			   if (effectiveness[i]<1)
			   {
               effectiveness[i]=1;
			   }
		      students[s]->train(skill[i],effectiveness[i]);
            if(students[s]->get_skill(skill[i]) < students[s]->skill_cap(skill[i],true))
			   {
				   worthcontinuing=true;
			   }
		   }
	   }
	   if (!worthcontinuing)
	   {
		   students[s]->activity.type=ACTIVITY_NONE;
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);
         addstr(students[s]->name);
         addstr(" has learned as much as ");
         addstr(students[s]->heshe());
         addstr(" can.");
         refresh();
         getch();
	   }
   }

   //TROUBLE
   if(trouble.size()>0)
   {
      long juiceval=0;
      char done=0;
      short crime=0;

      if(clearformess)erase();
      else
      {
         makedelimiter(8,0);
      }

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      if(trouble.size()>1)addstr("Your Activists have ");
      else {addstr(trouble[0]->name);addstr(" has ");}

      int power=0;
      for(int t=0;t<trouble.size();t++)
      {
         power+=trouble[t]->skill_roll(SKILL_PERSUASION)+
                trouble[t]->skill_roll(SKILL_STREETSENSE);
      }

      int mod=1;
      if(LCSrandom(10)<power)mod++;
      if(LCSrandom(20)<power)mod++;
      if(LCSrandom(40)<power)mod++;
      if(LCSrandom(60)<power)mod++;
      if(LCSrandom(80)<power)mod++;
      if(LCSrandom(100)<power)mod++;

      do
      {
         switch(LCSrandom(10))
         {
            case 0:
               addstr("run around uptown splashing paint on fur coats!");
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
               if(law[LAW_GAY]<2)
               {
                  addstr("disrupted a traditional wedding at a church!");
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
               if(law[LAW_ABORTION]<2)
               {
                  addstr("posted horrifying dead abortion doctor pictures downtown!");
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
               if(law[LAW_POLICEBEHAVIOR]<2)
               {
                  addstr("gone downtown and reenacted a police beating!");
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
               if(law[LAW_NUCLEARPOWER]<2)
               {
                  if(trouble.size()>1)addstr("dressed up and pretended to be radioactive mutants!");
                  else addstr("dressed up and pretended to be a radioactive mutant!");
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
               if(law[LAW_POLLUTION]<2)
               {
                  addstr("squirted business people with fake polluted water!");
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
               if(law[LAW_DEATHPENALTY]<2)
               {
                  addstr("distributed fliers graphically illustrating executions!");
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
               addstr("distributed fliers graphically illustrating CIA torture!");
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
               //In extreme corporate culture cases this should give a flag burning charge! -- kviiri
               addstr("burned a corporate symbol and denounced capitalism!");
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
               public_interest[VIEW_CORPORATECULTURE]+=mod;
               background_liberal_influence[VIEW_CORPORATECULTURE]+=mod;
               juiceval=1;
               done=1;
               break;
            }
            case 9:
            {
               addstr("set up a mock sweatshop in the middle of the mall!");
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
               public_interest[VIEW_SWEATSHOPS]+=mod;
               background_liberal_influence[VIEW_SWEATSHOPS]+=mod;
               juiceval+=1;
               done=1;
               break;
            }
         }
      }while(!done);

      refresh();
      getch();

      if(crime!=0)
      {
         for(int t=0;t<trouble.size();t++)
         {
            if(!LCSrandom(30) && 
               !(trouble[t]->skill_check(SKILL_STREETSENSE,DIFFICULTY_AVERAGE)))
            {
               if(clearformess)erase();
               else
               {
                  makedelimiter(8,0);
               }

               if(!LCSrandom(4))
               {
                  attemptarrest(*trouble[t],"causing trouble",clearformess);
               }
               else
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(trouble[t]->name);
                  addstr(" is cornered by a mob of angry rednecks.");

                  refresh();
                  getch();

                  bool wonfight = false;

                  if(trouble[t]->get_weapon().is_threatening())
                  {
                     if(clearformess)erase();
                     else makedelimiter(8,0);

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(8,1);
                     addstr(trouble[t]->name);
                     addstr(" brandishes the ");
                     addstr(trouble[t]->get_weapon().get_name().c_str());
                     addstr("!");
                     
                     refresh();
                     getch();
                     
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(8,1);
                     addstr("The mob scatters!");

                     refresh();
                     getch();

                     addjuice(*trouble[t],5,20);

                     wonfight = true;
                  }
                  else
                  {
                     int count = 0;

                     while(count <= LCSrandom(5)+2)
                     {
                        if(clearformess)erase();
                        else
                        {
                           makedelimiter(8,0);
                        }
                        if(trouble[t]->skill_roll(SKILL_HANDTOHAND)>LCSrandom(6)+count)
                        {
                           set_color(COLOR_CYAN,COLOR_BLACK,1);
                           move(8,1);
                           addstr(trouble[t]->name);
                           switch(LCSrandom(8))
                           {
                           case 0:addstr(" breaks the arm of the nearest person!");break;
                           case 1:addstr(" knees a guy in the balls!");break;
                           case 2:addstr(" knocks one out with a fist to the face!");break;
                           case 3:addstr(" bites some hick's ear off!");break;
                           case 4:addstr(" smashes one of them in the jaw!");break;
                           case 5:addstr(" shakes off a grab from behind!");break;
                           case 6:addstr(" yells the slogan!");break;
                           case 7:addstr(" knocks two of their heads together!");break;
                           }

                           refresh();
                           getch();

                           wonfight=true;
                        }
                        else
                        {
                           set_color(COLOR_YELLOW,COLOR_BLACK,1);
                           move(8,1);
                           addstr(trouble[t]->name);
                           switch(LCSrandom(8))
                           {
                           case 0:addstr(" is held down and kicked by three guys!");break;
                           case 1:addstr(" gets pummelled!");break;
                           case 2:addstr(" gets hit by a sharp rock!");break;
                           case 3:addstr(" is thrown against the sidewalk!");break;
                           case 4:addstr(" is bashed in the face with a shovel!");break;
                           case 5:addstr(" is forced into a headlock!");break;
                           case 6:addstr(" crumples under a flurry of blows!");break;
                           case 7:addstr(" is hit in the chest with a pipe!");break;
                           }

                           refresh();
                           getch();

                           count++; // fight goes faster when you're losing

                           wonfight=false;
                        }
                        count++;
                     }

                     if(wonfight)
                     {
                        if(clearformess)erase();
                        else
                        {
                           makedelimiter(8,0);
                        }
                        set_color(COLOR_GREEN,COLOR_BLACK,1);
                        move(8,1);
                        addstr(trouble[t]->name);
                        addstr(" beat the ");
                        if(law[LAW_FREESPEECH]==-2)
                           addstr("[tar]");
                        else
                           addstr("shit");
                        addstr(" out of everyone who got close!");

                        refresh();
                        getch();
                        
                        addjuice(*trouble[t],30,300);
                        if(trouble[t]->blood>70)trouble[t]->blood=70;
                     }
                  }

                  if(!wonfight)
                  {
                     if(clearformess)erase();
                     else
                     {
                        makedelimiter(8,0);
                     }
                     set_color(COLOR_RED,COLOR_BLACK,1);
                     move(8,1);
                     addstr(trouble[t]->name);
                     addstr(" is severely beaten before the mob is broken up.");

                     trouble[t]->activity.type=ACTIVITY_CLINIC;

                     refresh();
                     getch();

                     addjuice(*trouble[t],-10,-50);
                     if(trouble[t]->blood>10)trouble[t]->blood=10;

                     if(!LCSrandom(5))
                     {
                        if(clearformess)erase();
                        else
                        {
                           makedelimiter(8,0);
                        }
                        switch(LCSrandom(10))
                        {
                           case 0:
                              if(trouble[t]->special[SPECIALWOUND_LOWERSPINE]==1)
                              {
                                 move(8,1);
                                 addstr(trouble[t]->name);
                                 addstr("'s lower spine has been broken!");
                                 trouble[t]->special[SPECIALWOUND_LOWERSPINE]=0;
                                 refresh();
                                 getch();
                              }
                              break;
                           case 1:
                              if(trouble[t]->special[SPECIALWOUND_UPPERSPINE]==1)
                              {
                                 move(8,1);
                                 addstr(trouble[t]->name);
                                 addstr("'s upper spine has been broken!");
                                 trouble[t]->special[SPECIALWOUND_UPPERSPINE]=0;
                                 refresh();
                                 getch();
                              }
                              break;
                           case 2:
                              if(trouble[t]->special[SPECIALWOUND_NECK]==1)
                              {
                                 move(8,1);
                                 addstr(trouble[t]->name);
                                 addstr("'s neck has been broken!");
                                 trouble[t]->special[SPECIALWOUND_NECK]=0;
                                 refresh();
                                 getch();
                              }
                              break;
                           case 3:
                              if(trouble[t]->special[SPECIALWOUND_TEETH]>0)
                              {
                                 move(8,1);
                                 addstr(trouble[t]->name);
                                 if(trouble[t]->special[SPECIALWOUND_TEETH]>1)addstr("'s teeth have been smashed out on the curb.");
                                 else addstr("'s tooth has been pulled out with pliers!");
                                 trouble[t]->special[SPECIALWOUND_TEETH]=0;
                                 refresh();
                                 getch();
                              }
                              break;
                           default:
                           {
                              if(trouble[t]->special[SPECIALWOUND_RIBS]>0)
                              {
                                 int ribminus=LCSrandom(RIBNUM)+1;
                                 if(ribminus>trouble[t]->special[SPECIALWOUND_RIBS])ribminus=trouble[t]->special[SPECIALWOUND_RIBS];
                                 char num[20];
                                 itoa(ribminus,num,10);

                                 move(8,1);
                                 if(ribminus>1)
                                 {
                                    addstr(num);
                                    addstr(" of ");
                                    addstr(trouble[t]->name);
                                    addstr("'s ribs are ");
                                 }
                                 else if(trouble[t]->special[SPECIALWOUND_RIBS]>1)
                                 {
                                    addstr("One of ");
                                    addstr(trouble[t]->name);
                                    addstr("'s rib is ");
                                 }
                                 else
                                 {
                                    addstr(trouble[t]->name);
                                    addstr("'s last unbroken rib is ");
                                 }

                                 addstr("broken!");
                                 refresh();
                                 getch();

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

      for(int h=0;h<trouble.size();h++)
      {
         addjuice(*trouble[h],juiceval,40);
      }
   }

   //Teaching
   for(int t=0;t<teachers.size();t++)
   {
      int skillarray[14];
      int cost, students=0;
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
      for(int p=0;p<pool.size();p++)
      {
         //If they're at the location
         if(pool[p]->location==teachers[t]->location &&
            pool[p]->align==ALIGN_LIBERAL)
         {
            //Step through the array of skills to train
            for(int i=0;i<13;i++)           //Any reason why we aren't using a while(true) loop or something even more dynamic? --kviiri
            {
               //If no more skills to train, stop
               if(skillarray[i]==-1)break;
               //Otherwise, if the student has less skill than the teacher, train the student
               //proportional to the difference in skill between teacher and student times the
               //teacher's ability at teaching
               if(pool[p]->get_skill(skillarray[i])<teachers[t]->get_skill(skillarray[i])-1&&
                  pool[p]->get_skill(skillarray[i])<teachers[t]->get_skill(SKILL_TEACHING)+2&&
                  ledger.get_funds()>cost&&
                  pool[p]->get_skill(skillarray[i])<pool[p]->skill_cap(skillarray[i],true))
               {
                  students++;
               }
            }
         }
      }

      //Walk through and train people
      for(int p=0;p<pool.size();p++)
      {
         //If they're at the location
         if(pool[p]->location==teachers[t]->location &&
            pool[p]->align==ALIGN_LIBERAL)
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
                  ledger.get_funds()>cost&&
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

   //BURY THE DEAD
   if(bury.size()>0)
   {
      for(int p=pool.size()-1;p>=0;p--)
      {
         if(pool[p]->alive)continue;

         //MAKE BASE LOOT
         makeloot(*pool[p],location[bury[0]->base]->loot);

         //BURY
         delete pool[p];
         pool.erase(pool.begin() + p);

         for(int b=0;b<bury.size();b++)
         {
            if(!(bury[b]->skill_check(SKILL_STREETSENSE,DIFFICULTY_AVERAGE)))
            {
               newsstoryst *ns=new newsstoryst;
                  ns->type=NEWSSTORY_BURIALARREST;
                  ns->loc=-1;
               newsstory.push_back(ns);
               sitestory=ns;

               criminalize(*bury[b],LAWFLAG_BURIAL);
               attemptarrest(*bury[b],"disposing of bodies",clearformess);

               //If a liberal has been killed or arrested they should not do more burials.
               if (!(bury[b]->alive)
                   || location[bury[b]->location]->type == SITE_GOVERNMENT_POLICESTATION)
               {
                  bury.erase(bury.begin() + b);
                  --b;
               }
            }
         }
         if (bury.size() == 0) //Stop burials if none are left doing them.
            break;
      }
   }
}



/* steal a car */
char stealcar(Creature &cr,char &clearformess)
{
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
      addstr("Adventures in Liberal Car Theft");
      printcreatureinfo(&cr);
      makedelimiter(8,0);

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(10,0);
      addstr(cr.name);
      addstr(" looks around for an accessible vehicle...");

      refresh();
      getch();

      //ROUGH DAY
      if(!cr.skill_check(SKILL_STREETSENSE,diff))
      {
         do
         {
            cartype=LCSrandom(vehicletype.size());
            if(LCSrandom(10)<vehicletype[cartype]->steal_difficultytofind())cartype=old;
         }while(cartype==old);
      }

      v = new Vehicle(*vehicletype[cartype]);
      string carname = v->fullname();

      if(old!=cartype)
      {
         move(11,0);
         addstr(cr.name);
         addstr(" was unable to find a ");
         addstr(vehicletype[old]->longname().c_str());
         addstr(" but did find a ");
         addstr(v->longname().c_str());
         addstr(".");

         refresh();
         getch();
      }
      else
      {
         move(11,0);
         addstr(cr.name);
         addstr(" found a ");
         addstr(v->longname().c_str());
         addstr(".");

         refresh();
         getch();
      }

      //APPROACH?
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Adventures in Liberal Car Theft");
      printcreatureinfo(&cr);
      makedelimiter(8,0);

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(10,0);
      addstr(cr.name);
      addstr(" looks from a distance at an empty ");
      addstr(carname.c_str());
      addstr(".");

      move(12,0);
      addstr("A - Approach the driver's side door.");
      move(13,0);
      addstr("X - Call it a day.");

      refresh();

      int c;

      do
      {
         c=getch();
         translategetch(c);
         if(c=='a')break;
         if(c=='x'){delete v;return 1;}
      }while(1);

      //SECURITY?
      char alarmon=0;
      char sensealarm=0;
      if(LCSrandom(100)<v->sensealarmchance()) sensealarm=1;
      char touchalarm=0;
      if(LCSrandom(100)<v->touchalarmchance()) touchalarm=1;
      char windowdamage=0;

      do
      {
         erase();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(0,0);
         addstr("Adventures in Liberal Car Theft");
         printcreatureinfo(&cr);
         makedelimiter(8,0);

         if(alarmon)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(10,0);
            if(sensealarm)addstr("THE VIPER");
            else addstr(carname.c_str());
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
            addstr("THIS IS THE VIPER!   STAND AWAY!");
         }
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(10,0);
            addstr(cr.name);
            addstr(" stands by the ");
            addstr(carname.c_str());
            addstr(".");
         }

         move(12,0);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr("A - Pick the lock.");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(13,0);
         addstr("B - Break the window.");
         move(14,0);
         if(!sensealarm)addstr("X - Call it a day.");
         else
         {
            if(!alarmon)
            {
               addstr("X - The Viper?   ");
               addstr(cr.name);
               addstr(" is deterred.");
            }
            else
            {
               addstr("X - Yes, the Viper has deterred ");
               addstr(cr.name);
               addstr(".");
            }
         }

         refresh();

         char method=0;

         do
         {
            c=getch();
            translategetch(c);
            if(c=='a')break;
            if(c=='b'){method=1;break;}
            if(c=='x'){delete v;return 0;} /* try again tomorrow */
         }while(1);

         char entered=0;

         //PICK LOCK
         if(method==0)
         {
            if(cr.skill_check(SKILL_SECURITY,DIFFICULTY_AVERAGE))
            {
               cr.train(SKILL_SECURITY,MAX(5-cr.get_skill(SKILL_SECURITY),0));
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,0);
               addstr(cr.name);
               addstr(" jimmies the car door open.");
               refresh();getch();

               entered=1;
            }
            else
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,0);
               addstr(cr.name);
               addstr(" fiddles with the lock with no luck.");
               refresh();getch();
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
               addstr(cr.name);
               addstr(" smashes the window");
               if(cr.get_weapon().get_bashstrengthmod()>1)
               {
                  addstr(" with a ");
                  addstr(cr.get_weapon().get_name(2).c_str());
               }
               addstr(".");
               refresh();getch();

               windowdamage=10;

               entered=1;
            }
            else
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,0);
               addstr(cr.name);
               addstr(" cracks the window");
               if(cr.get_weapon().get_bashstrengthmod()>1)
               {
                  addstr(" with a ");
                  addstr(cr.get_weapon().get_name(2).c_str());
               }
               addstr(" but it is still somewhat intact.");

               windowdamage+=1;

               refresh();getch();
            }
         }

         //ALARM CHECK
         int y=17;

         if(touchalarm||sensealarm)
         {
            if(!alarmon)
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               move(y,0);y++;
               addstr("An alarm suddenly starts blaring!");
               refresh();
               getch();
               alarmon=1;
            }
         }

         //NOTICE CHECK
         if(!LCSrandom(50)||(!LCSrandom(5)&&alarmon))
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            move(y,0);y++;
            addstr(cr.name);
            addstr(" has been spotted by a passerby!");
            refresh();
            getch();

            //FOOT CHASE
            chaseseq.clean();
            chaseseq.location=0;
            newsstoryst *ns=new newsstoryst;
               ns->type=NEWSSTORY_CARTHEFT;
            newsstory.push_back(ns);
            sitestory=ns;
            makechasers(-1,5);

            if(footchase(cr)){
               mode=GAMEMODE_BASE;
               delete v;return 0;} // Switched to return 0; this will cause you to try again tomorrow
            else {
               mode=GAMEMODE_BASE;
               delete v;return 0;}
         }

         if(entered)break;
      }while(1);

      //START CAR
      char keys_in_car=0;
      int key_location = LCSrandom(5);
      char ignition_progress=0;
      char key_search_total=0;
      int nervous_counter=0;
      if(!LCSrandom(5))keys_in_car=1;

      do
      {
         nervous_counter++;
         erase();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(0,0);
         addstr("Adventures in Liberal Car Theft");
         printcreatureinfo(&cr);
         makedelimiter(8,0);

         int y=10;

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);y++;
         addstr(cr.name);
         addstr(" is behind the wheel of a ");
         addstr(carname.c_str());
         addstr(".");

         if(alarmon)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(y,0);y++;
            if(sensealarm)addstr("THE VIPER");
            else addstr(carname.c_str());
            addstr(":   ");
            set_color(COLOR_RED,COLOR_BLACK,1);
            if(sensealarm)addstr("REMOVE YOURSELF FROM THE VEHICLE!   <BEEP!!> <BEEP!!>");
            else addstr("<BEEP!!> <BEEP!!> <BEEP!!> <BEEP!!>");
         }

         y++;


         move(y,0);y++;
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr("A - Hotwire the car.");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);y++;
         addstr("B - Desperately search for keys.");
         move(y,0);y++;
         if(!sensealarm)addstr("X - Call it a day.");
         else {addstr("X - The Viper has finally deterred ");addstr(cr.name);addstr(".");}

         refresh();

         y++;

         char method=0;

         do
         {
            c=getch();
            translategetch(c);
            if(c=='a')break;
            if(c=='b'){method=1;break;}
            if(c=='x'){delete v;return 0;} // Call it a day and try again tomorrow
         }while(1);

         char started=0;

         //HOTWIRE CAR
         if(method==0)
         {
            if(cr.skill_check(SKILL_SECURITY,DIFFICULTY_CHALLENGING))
            {
               cr.train(SKILL_SECURITY,MAX(10-cr.get_skill(SKILL_SECURITY),0));
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(y,0);y++;
               addstr(cr.name);
               addstr(" hotwires the car!");
               refresh();getch();

               started=1;
            }
            else
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(y,0);y++;
               addstr(cr.name);
               int flavor_text;
               if(cr.get_skill(SKILL_SECURITY) < 4)
                  flavor_text = LCSrandom(3);
               else
                  flavor_text = LCSrandom(5);
               switch(flavor_text)
               {
                  case 0:addstr(" fiddles with the ignition, but the car doesn't start.");break;
                  case 1:addstr(" digs around in the steering column, but the car doesn't start.");break;
                  case 2:addstr(" touches some wires together, but the car doesn't start.");break;
                  case 3:addstr(" makes something in the engine click, but the car doesn't start.");break;
                  case 4:addstr(" manages to turn on some dash lights, but the car doesn't start.");break;
               }
               refresh();getch();
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
               move(y,0);y++;
               if(law[LAW_FREESPEECH]==-2)addstr("Holy [Car Keys]!  "); // Holy car keys Batman!
               else addstr("Holy shit!  ");
               addstr(cr.name);
               addstr(" found the keys ");
               addstr(location);

               refresh();getch();

               started=1;
            }
            else
            {
               key_search_total++;
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(y,0);y++;
               addstr(cr.name);
               addstr(": <rummaging> ");
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               if (key_search_total==5)
               {
                  addstr("Are they even in here?");
               }
               else if (key_search_total==10)
               {
                  addstr("I don't think they're in here...");
               }
               else if (key_search_total==15)
               {
                  addstr("If they were here, I'd have found them by now.");
               }
               else if (key_search_total>15)
               {
                  switch(LCSrandom(5))
                  {
                  case 0:addstr("This isn't working!");break;
                  case 1:addstr("Why me?");break;
                  case 2:addstr("What do I do now?");break;
                  case 3:addstr("Oh no...");break;
                  case 4:addstr("I'm going to get arrested, aren't I?");break;
                  }
               }
               else
               {
                  switch(LCSrandom(5))
                  {
                     case 0:addstr("Please be in here somewhere...");break;
                     case 1:
                        if(law[LAW_FREESPEECH]==-2)addstr("[Shoot]!  Where are they?!");
                        else addstr("Fuck!  Where are they?!");
                        break;
                     case 2:addstr("Come on, baby, come to me...");break;
                     case 3:
                        if(law[LAW_FREESPEECH]==-2)addstr("[Darn] it...");
                        else addstr("Dammit...");
                        break;
                     case 4:addstr("I wish I could hotwire this thing...");break;
                  }
               }
               refresh();getch();
            }
         }

         //NOTICE CHECK
         if(!started&&(!LCSrandom(50)||(!LCSrandom(5)&&alarmon)))
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            move(y,0);y++;
            addstr(cr.name);
            addstr(" has been spotted by a passerby!");
            refresh();
            getch();

            //FOOT CHASE
            chaseseq.clean();
            chaseseq.location=0;
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
            addstr(cr.name);
            switch(LCSrandom(3))
            {
               case 0:addstr(" hears someone nearby making a phone call.");break;
               case 1:addstr(" is getting nervous being out here this long.");break;
               case 2:addstr(" sees a police car driving around a few blocks away.");break;
            }
            refresh();
            getch();
         }

         if(started)break;
      }while(1);

      //CHASE SEQUENCE
         //CAR IS OFFICIAL, THOUGH CAN BE DELETE BY chasesequence()
      addjuice(cr,v->steal_juice(),100);

      v->add_heat(14+v->steal_extraheat());

      chaseseq.clean();
      chaseseq.location=0;
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
      }

      vehicle.push_back(v);
      if(chasesequence(cr,*v))
      {
         v->set_location(cr.base);
         // Automatically assign this car to this driver, if no other one is present
         if(cr.pref_carid==-1)
         {
            cr.pref_carid = v->id();
            cr.pref_is_driver = true;
         }
         return 1;
      }
      else
      {

         return 0;//do not need to delete vehicle
      }
   }

   return 0;
}

char carselect(Creature &cr,short &cartype)
{
   cartype=-1;

   vector<int> cart;
   for(unsigned a=0;a<vehicletype.size();a++)
   {
      if(vehicletype[a]->steal_difficultytofind()<10) cart.push_back(a);
   }

   int page=0;

   char str[200];

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("What type of car will ");
      addstr(cr.name);
      addstr(" try to find and steal today?");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(1,0);
      addstr("----TYPE-----------------------------------------DIFFICULTY TO FIND UNATTENDED--");

      int y=2,difficulty;
      for(int p=page*19;p<cart.size()&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         addstr(vehicletype[cart[p]]->longname().c_str());

         move(y,49);
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

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Type of Car");
      move(23,0);
      addpagestr();

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<cart.size())page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<cart.size())
         {
            cartype=cart[p];
            return 1;
         }
      }

      // Too easy to accidentally back out
      // Not a big problem if this page isn't skippable
      // (There's no immediate risk in picking a car)
      // - JDS
      //if(c==10)break;
   }while(1);

   return 0;
}

/* get a wheelchair */
void getwheelchair(Creature &cr,char &clearformess)
{
   if(clearformess)
   {
      erase();
   }
   else
   {
      makedelimiter(8,0);
   }

   if(LCSrandom(2))
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(cr.name);
      addstr(" has procured a wheelchair.");

      cr.flag|=CREATUREFLAG_WHEELCHAIR;
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(cr.name);
      addstr(" was unable to get a wheelchair.  Maybe tomorrow...");
   }

   refresh();
   getch();
}
