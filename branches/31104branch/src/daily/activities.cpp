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

#include <includes.h>
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
      case 8:addstr("a shameful");break;
      }
      power = -LCSrandom(2);
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
void repairarmor(creaturest &cr,char &clearformess)
{
   armorst *armor=NULL;
   itemst *pile=NULL;
   vector<itemst *> *pilelist=NULL;

   if(cr.armor.type!=ARMOR_NONE&&
      cr.armor.flag & (ARMORFLAG_DAMAGED | ARMORFLAG_BLOODY))
   {
      armor=&cr.armor;
   }
   else if(cr.squadid!=-1)
   {
      int sq=getsquad(cr.squadid);
      for(int l=0;l<squad[sq]->loot.size();l++)
      {
         if(squad[sq]->loot[l]->type==ITEM_ARMOR&&
            squad[sq]->loot[l]->armor.flag & (ARMORFLAG_DAMAGED | ARMORFLAG_BLOODY))
         {
            armor=&squad[sq]->loot[l]->armor;
            pile=squad[sq]->loot[l];
            pilelist=&squad[sq]->loot;
            break;
         }
      }
   }
   if(armor==NULL&&cr.location!=-1)
   {
      for(int l=0;l<location[cr.location]->loot.size();l++)
      {
         if(location[cr.location]->loot[l]->type==ITEM_ARMOR&&
            location[cr.location]->loot[l]->armor.flag & (ARMORFLAG_DAMAGED | ARMORFLAG_BLOODY))
         {
            armor=&location[cr.location]->loot[l]->armor;
            pile=location[cr.location]->loot[l];
            pilelist=&location[cr.location]->loot;
            break;
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

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(cr.name);
      if(armor->flag & ARMORFLAG_DAMAGED)addstr(" repairs ");
      else addstr(" cleans ");
      char str[80];
      getarmorfull(str,armor->type,armor->subtype);
      addstr(str);

      if(pile!=NULL)
      {
         if(pile->number>1)
         {
            itemst *newpile=new itemst;
               *newpile=*pile;
            pilelist->push_back(newpile);
            newpile->number=pile->number-1;
            pile->number=1;
         }
      }

      armor->flag&=~ARMORFLAG_BLOODY;
      armor->flag&=~ARMORFLAG_DAMAGED;

      if(armor->flag & ARMORFLAG_DAMAGED)
      {
         long dif=(armor_makedifficulty(armor->type,&cr)>>1);
         cr.skill_ip[SKILL_GARMENTMAKING]+=dif+1;

         if((LCSrandom(3+cr.skill[SKILL_GARMENTMAKING])<dif)&&armor->quality!='4')
         {
            addstr(" but it is not quite the same");
            armor->quality++;
            if(LCSrandom(3+cr.skill[SKILL_GARMENTMAKING])<dif && armor->quality!='4')armor->quality++;
            if(LCSrandom(3+cr.skill[SKILL_GARMENTMAKING])<dif && armor->quality!='4')armor->quality++;
         }
      }
      
      addstr(".");

      refresh();
      getch();
   }
}



/* armor manufacture */
void makearmor(creaturest &cr,char &clearformess)
{
   long at=cr.activity.arg;

   long cost=armor_makeprice(at);
   long hcost=(armor_makeprice(at)>>1)+1;
   long dif=armor_makedifficulty(at,&cr);

   if(funds<hcost)
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
            if(squad[sq]->loot[l]->type==ITEM_LOOT&&
               squad[sq]->loot[l]->loottype==LOOT_FINECLOTH)
            {
               if(squad[sq]->loot[l]->number==1)
               {
                  delete squad[sq]->loot[l];
                  squad[sq]->loot.erase(squad[sq]->loot.begin() + l);
               }
               else squad[sq]->loot[l]->number--;
               foundcloth=1;
               break;
            }
         }
      }
      if(!foundcloth)
      {
         for(int l=0;l<location[cr.location]->loot.size();l++)
         {
            if(location[cr.location]->loot[l]->type==ITEM_LOOT&&
               location[cr.location]->loot[l]->loottype==LOOT_FINECLOTH)
            {
               if(location[cr.location]->loot[l]->number==1)
               {
                  delete location[cr.location]->loot[l];
                  location[cr.location]->loot.erase(location[cr.location]->loot.begin() + l);
               }
               else location[cr.location]->loot[l]->number--;
               foundcloth=1;
               break;
            }
         }
      }

      if(!foundcloth&&funds<cost)
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
            funds-=hcost;
            stat_spent+=hcost;
            moneylost_manufacture+=hcost;
         }
         else
         {
            funds-=cost;
            stat_spent+=cost;
            moneylost_manufacture+=cost;
         }

         cr.skill_ip[SKILL_GARMENTMAKING]+=dif*2+1;

         itemst *it=new itemst;
            it->type=ITEM_ARMOR;
            it->armor.type=at;
            it->armor.quality='1';
            it->armor.flag=0;
         location[cr.location]->loot.push_back(it);

         if(LCSrandom(10)<dif||LCSrandom(10)<dif)
         {
            it->armor.quality='2';
            if(LCSrandom(10)<dif||LCSrandom(10)<dif)
            {
               it->armor.quality='3';
               if(LCSrandom(10)<dif||LCSrandom(10)<dif)
               {
                  it->armor.quality='4';
               }
            }
         }

         if(clearformess)erase();
         else
         {
            makedelimiter(8,0);
         }

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);
         addstr(cr.name);
         addstr(" has made a ");
         switch(it->armor.quality)
         {
            case '1':addstr("first-rate");break;
            case '2':addstr("second-rate");break;
            case '3':addstr("third-rate");break;
            case '4':addstr("fourth-rate");break;
         }
         addstr(" ");
         char str[80];
         getarmorfull(str,at);
         addstr(str);
         addstr(".");

         refresh();
         getch();
      }
   }
}



/* search for polls */
void survey(creaturest *cr)
{
   static const char SURVEY_PAGE_SIZE=14;

   int v;
   int creatureskill=cr->skill[SKILL_COMPUTERS]+cr->attval(ATTRIBUTE_INTELLIGENCE);
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
      if(maxview!=-1)
      {
         if(public_interest[v]>public_interest[maxview])maxview=v;
      }
      else
      {
         if(public_interest[v]>0)maxview=v;
      }

      //MAKE SURVEY ACCURATE IF DEBUGGING
      #ifdef NDEBUG
         survey[v]+=LCSrandom(noise*2+1)-noise;

         if(!LCSrandom(20))
         {
            survey[v]+=LCSrandom(noise*2+1)-noise;
            if(!LCSrandom(20))
            {
               survey[v]+=LCSrandom(noise*2+1)-noise;
               if(!LCSrandom(20))
               {
                  survey[v]+=LCSrandom(noise*2+1)-noise;
                  if(!LCSrandom(20))
                  {
                     survey[v]+=LCSrandom(noise*2+1)-noise;
                  }
               }
            }
         }
      #endif

      if(survey[v]<0)survey[v]=0;
      if(survey[v]>100)survey[v]=100;

      #ifdef NDEBUG
         if(LCSrandom(public_interest[v]+50)<misschance)survey[v]=-1;
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
      set_color(COLOR_BLUE,COLOR_BLACK,1);
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
         case VIEW_ABORTION:
            if(attitude[VIEW_ABORTION]>50)addstr("protecting abortion rights.");
            else addstr("protecting the unborn child.");
            break;
         case VIEW_GAY:
            if(attitude[VIEW_GAY]>50)addstr("protecting gay rights.");
            else addstr("protecting the traditional family.");
            break;
         case VIEW_DEATHPENALTY:
            if(attitude[VIEW_DEATHPENALTY]>50)addstr("the unjust death penalty.");
            else addstr("protecting the death penalty.");
            break;
			case VIEW_TAXES:
            if(attitude[VIEW_TAXES]>50)addstr("the oppressive tax structure.");
            else addstr("the excessive tax burden.");
            break;
         case VIEW_NUCLEARPOWER:
            if(attitude[VIEW_NUCLEARPOWER]>50)addstr("the dangers of nuclear power.");
            else addstr("threats to nuclear power.");
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
         case VIEW_LIBERALCRIMESQUAD:
         case VIEW_LIBERALCRIMESQUADPOS:
            if(attitude[VIEW_LIBERALCRIMESQUAD]<50)
            {
               addstr("activist political groups.");
               break;
            }
            if(attitude[VIEW_LIBERALCRIMESQUADPOS]>50)addstr("the Liberal Crime Squad.");
            else addstr("the left-wing terrorist threat.");
            break;
         case VIEW_PRISONS:
            if(attitude[VIEW_PRISONS]>50)addstr("horrific prison conditions.");
            else addstr("lax prison conditions.");
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
         if(v>=VIEWNUM)
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
            case VIEW_ABORTION:addstr("supported abortion");break;
            case VIEW_GAY:addstr("were in favor of equal rights for homosexuals");break;
            case VIEW_DEATHPENALTY:addstr("opposed the death penalty");break;
			   case VIEW_TAXES:addstr("were against cutting taxes");break;
            case VIEW_NUCLEARPOWER:addstr("were terrified of nuclear power");break;
            case VIEW_ANIMALRESEARCH:addstr("deplored animal research");break;
            case VIEW_POLICEBEHAVIOR:addstr("were critical of the police");break;
            case VIEW_INTELLIGENCE:addstr("thought the intelligence community invades privacy");break;
            case VIEW_FREESPEECH:addstr("believed in unfettered free speech");break;
            case VIEW_GENETICS:addstr("abhorred genetically altered food products");break;
            case VIEW_JUSTICES:addstr("were for the appointment of Liberal justices");break;
            case VIEW_SWEATSHOPS:addstr("would boycott companies that used sweatshops");break;
            case VIEW_POLLUTION:addstr("thought industry should lower pollution");break;
            case VIEW_CORPORATECULTURE:addstr("were disgusted by corporate malfeasance");break;
            case VIEW_CEOSALARY:addstr("believed that CEO salaries are too great");break;
            case VIEW_LIBERALCRIMESQUAD:addstr("respected the power of the Liberal Crime Squad");break;
            case VIEW_LIBERALCRIMESQUADPOS:addstr("of these held the Liberal Crime Squad in high regard");break;
            case VIEW_PRISONS:addstr("think the prison system needs reform");break;
            case VIEW_AMRADIO:addstr("do not like AM radio");break;
            case VIEW_CABLENEWS:addstr("have a negative opinion of cable news programs");break;
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


int checkforarrest(creaturest & liberal,const char* string,long ps,int clearformess)
{
   if(liberal.heat>liberal.skill[SKILL_STREETSENSE]*10)
   {
      if(!LCSrandom(50))
      {
         if(clearformess)erase();
         else
         {
            makedelimiter(8,0);
         }

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);
         addstr(liberal.name);
         addstr(" has been arrested while ");
         addstr(string);
         addstr(".");

         refresh();
         getch();

         removesquadinfo(liberal);
         liberal.carid=-1;
         liberal.location=ps;
         liberal.weapon.type=WEAPON_NONE;
         liberal.weapon.ammo=0;
         liberal.activity.type=ACTIVITY_NONE;

         return true;
      }
   }
   return false;
}



/* misc activation related things */
// *JDSRETURN*
void funds_and_trouble(char &clearformess)
{
   int s;
   //FIND A POLICE STATION
   //and a clinic too
   long ps=-1;
   long clinic=-1;
   for(long l=0;l<location.size();l++)
   {
      if(location[l]->type==SITE_GOVERNMENT_POLICESTATION)
      {
         ps=l;
      }
      if(location[l]->type==SITE_HOSPITAL_CLINIC)
      {
         clinic=l;
      }
   }

   //ACTIVITIES FOR INDIVIDUALS
   vector<creaturest *> trouble;
   vector<creaturest *> hack;
   vector<creaturest *> bury;
   vector<creaturest *> solicit;
   vector<creaturest *> tshirts;
   vector<creaturest *> art;
   vector<creaturest *> music;
   vector<creaturest *> graffiti;
   vector<creaturest *> brownies;
   vector<creaturest *> prostitutes;
   vector<creaturest *> teachers;

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
         case ACTIVITY_TEACH_GENERALED:
         case ACTIVITY_TEACH_FIGHTING:
         case ACTIVITY_TEACH_SURVIVAL:
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
            if(pool[p]->heat && !LCSrandom(20))pool[p]->heat--;
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
      }
   }

   long money;

   //SOLICITORS
   for(s=0;s<solicit.size();s++)
   {
      if(!checkforarrest(*solicit[s],"soliciting donations",ps,clearformess))
      {
         money=LCSrandom(((solicit[s]->skill[SKILL_PERSUASION]+
         solicit[s]->skill[SKILL_BUSINESS]+
         solicit[s]->attval(ATTRIBUTE_CHARISMA)+
         solicit[s]->attval(ATTRIBUTE_HEART))*
         attitude[VIEW_LIBERALCRIMESQUADPOS]*
         attitude[VIEW_LIBERALCRIMESQUAD])/10000+1);

         funds+=money;
         stat_funds+=money;
         moneygained_donate+=money;
         if(solicit[s]->skill[SKILL_PERSUASION]<3)
            solicit[s]->skill_ip[SKILL_PERSUASION]+=LCSrandom(5)+2;
      }
   }

   //TSHIRTS
   int mood=publicmood(-1);
   for(s=0;s<tshirts.size();s++)
   {
      if(!checkforarrest(*tshirts[s],"selling shirts",ps,clearformess))
      {
         money=LCSrandom((tshirts[s]->skill[SKILL_GARMENTMAKING]*4*mood)/50+1);
         funds+=money;
         stat_funds+=money;
         moneygained_goods+=money;

         if(tshirts[s]->skill[SKILL_GARMENTMAKING]<4)
            tshirts[s]->skill_ip[SKILL_GARMENTMAKING]+=LCSrandom(5)+2;
      }
   }

   //ART
   for(s=0;s<art.size();s++)
   {
      if(!checkforarrest(*art[s],"sketching potraits",ps,clearformess))
      {
         money=LCSrandom((art[s]->skill[SKILL_ART]*4*mood)/50+1);
         funds+=money;
         stat_funds+=money;
         moneygained_goods+=money;

         if(art[s]->skill[SKILL_ART]<5)
            art[s]->skill_ip[SKILL_ART]+=LCSrandom(5)+2;
      }
   }

   //MUSIC
   for(s=0;s<music.size();s++)
   {
      if(!checkforarrest(*music[s],"playing music",ps,clearformess))
      {
         money=LCSrandom((music[s]->skill[SKILL_MUSIC]*4*mood)/50+1);
         funds+=money;
         stat_funds+=money;
         moneygained_goods+=money;

         if(music[s]->skill[SKILL_MUSIC]<5)
            music[s]->skill_ip[SKILL_MUSIC]+=LCSrandom(5)+2;
      }
   }

   //BROWNIES
   long dodgelawroll;
   for(s=0;s<brownies.size();s++)
   {
      money=LCSrandom(brownies[s]->skill[SKILL_PERSUASION]*2+
                      brownies[s]->attval(ATTRIBUTE_CHARISMA)*2+
                      brownies[s]->attval(ATTRIBUTE_INTELLIGENCE)*2+
                      brownies[s]->skill[SKILL_STREETSENSE]*2+
                      brownies[s]->skill[SKILL_BUSINESS]*2+1);
      funds+=money;
      stat_funds+=money;
      moneygained_brownies+=money;
      if(brownies[s]->skill[SKILL_PERSUASION]<5)
         brownies[s]->skill_ip[SKILL_PERSUASION]+=LCSrandom(5)+2;
      if(brownies[s]->skill[SKILL_STREETSENSE]<5)
         brownies[s]->skill_ip[SKILL_STREETSENSE]+=LCSrandom(5)+2;

      //Check for police search
      dodgelawroll=LCSrandom(100);

      //Saved by street sense?
      if(dodgelawroll==0)
         dodgelawroll=LCSrandom(brownies[s]->skill[SKILL_STREETSENSE]+1);

      if(dodgelawroll==0) // Busted! Sort of
      {
         criminalize(*brownies[s],LAWFLAG_BROWNIES);

         // Check if an immediate arrest is made, or they're
         // just quietly watched
         if(brownies[s]->heat > 5 || LCSrandom(2))
         {
            if(clearformess)erase();
            else
            {
               makedelimiter(8,0);
            }

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(brownies[s]->name);
            addstr(" has been arrested while selling brownies.");

            brownies[s]->lawflag[LAWFLAG_BROWNIES]=1;

            refresh();
            getch();

            removesquadinfo(*brownies[s]);
            brownies[s]->carid=-1;
            brownies[s]->location=ps;
            brownies[s]->weapon.type=WEAPON_NONE;
            brownies[s]->weapon.ammo=0;
            brownies[s]->activity.type=ACTIVITY_NONE;
         }         
      }
   }

   //HACKING
   if(hack.size()>0)
   {
      int h;
      vector<creaturest *> cc;
      vector<creaturest *> web;
      vector<creaturest *> ddos;
      vector<creaturest *> truehack;

      int cc_skill=0,cc_succeeded=0;
      int web_skill=0,web_succeeded=0;
      int ddos_skill=0,ddos_succeeded=0;
      int hack_skill=0,hack_succeeded=0;

      //First, do accounting to figure out who's doing what
      for(h=0;h<hack.size();h++)
      {
         switch(hack[h]->activity.type)
         {
         case ACTIVITY_CCFRAUD:
            if(hack[h]->skill[SKILL_COMPUTERS]>1)
            {
               cc_skill+=hack[h]->skill[SKILL_COMPUTERS]-1;
               cc_succeeded=1;
            }
            cc.push_back(hack[h]);
            break;
         case ACTIVITY_DOS_ATTACKS:
            if(hack[h]->skill[SKILL_COMPUTERS]>1)
            {
               web_skill+=hack[h]->skill[SKILL_COMPUTERS]-1;
               web_succeeded=1;
            }
            web.push_back(hack[h]);
            break;
         case ACTIVITY_DOS_RACKET:
            if(hack[h]->skill[SKILL_COMPUTERS]>5)
            {
               ddos_skill+=hack[h]->skill[SKILL_COMPUTERS]-4;
               ddos_skill+=hack[h]->attval(ATTRIBUTE_INTELLIGENCE)-7;
               ddos_succeeded=1;
            }
            ddos.push_back(hack[h]);
            break;
         case ACTIVITY_HACKING:
            if(hack[h]->skill[SKILL_COMPUTERS]>5)
            {
               hack_skill+=hack[h]->skill[SKILL_COMPUTERS]-4;
               hack_skill+=hack[h]->attval(ATTRIBUTE_INTELLIGENCE)-7;
               hack_succeeded=1;
            }
            truehack.push_back(hack[h]);
            break;
         }
      }
      //Now that we know what's being tried and what's being pulled off,
      //loop through again to assign experince
      for(h=0;h<hack.size();h++)
      {
         switch(hack[h]->activity.type)
         {
         case ACTIVITY_CCFRAUD:
            if(cc_succeeded&&hack[h]->skill[SKILL_COMPUTERS]<7)
               hack[h]->skill_ip[SKILL_COMPUTERS]+=LCSrandom(5);
            else
               hack[h]->skill_ip[SKILL_COMPUTERS]+=LCSrandom(2);
            break;
         case ACTIVITY_DOS_ATTACKS:
            if(web_succeeded&&hack[h]->skill[SKILL_COMPUTERS]<7)
               hack[h]->skill_ip[SKILL_COMPUTERS]+=LCSrandom(5);
            else
               hack[h]->skill_ip[SKILL_COMPUTERS]+=LCSrandom(2);
            break;
         case ACTIVITY_DOS_RACKET:
            if(ddos_succeeded&&hack[h]->skill[SKILL_COMPUTERS]<14)
               hack[h]->skill_ip[SKILL_COMPUTERS]+=LCSrandom(8);
            else
               hack[h]->skill_ip[SKILL_COMPUTERS]+=LCSrandom(2);
            break;
         case ACTIVITY_HACKING:
            if(hack_succeeded&&hack[h]->skill[SKILL_COMPUTERS]<18)
               hack[h]->skill_ip[SKILL_COMPUTERS]+=LCSrandom(10);
            else
               hack[h]->skill_ip[SKILL_COMPUTERS]+=LCSrandom(2);
            break;
         }
      }

      char msg[80] = {0};

      //MAJOR HACKING
      if(truehack.size())
      {
         if(!hack_succeeded)
         {
            switch(LCSrandom(16))
            {
            case 0:strcpy(msg,"Your hackers have no idea how to do what you've asked them to do.");break;
            case 1:strcpy(msg,"Your hackers can't even figure out where to start.");break;
            case 2:strcpy(msg,"Your hackers need to get their feet wet on something less ambitious.");break;
            case 3:strcpy(msg,"Your hackers are just playing video games.");break;
            default:strcpy(msg,"");break;
            }
         }
         else
         {
            if(LCSrandom(150)<=hack_skill)
            {
               if(truehack.size()>1)strcpy(msg,"Your Hackers have ");
               else {strcpy(msg,truehack[0]->name);strcat(msg," has ");}

               int trackdif=0;
               unsigned short crime=0;

               long juiceval=0;

               switch(LCSrandom(5))
               {
                  case 0:
                  {
                     strcat(msg,"pilfered files from a Corporate server.");

                     itemst *it=new itemst;
                        it->type=ITEM_LOOT;
                        it->loottype=LOOT_CORPFILES;
                     location[hack[0]->location]->loot.push_back(it);

                     trackdif=20;
                     crime=LAWFLAG_INFORMATION;
                     juiceval=5;
                     break;
                  }
                  case 1: // *JDS* Penetrated government networks; don't get any loot, but do scare the info community
                     strcat(msg,"caused a scare by breaking into a CIA network.");

                     trackdif=30;
                     crime=LAWFLAG_INFORMATION;
                     juiceval=20;
                     change_public_opinion(VIEW_INTELLIGENCE,10,0,75);
                     break;
                  case 2:
                     strcat(msg,"sabotaged a genetics research company's network.");

                     trackdif=20;
                     crime=LAWFLAG_INFORMATION;
                     juiceval=5;
                     change_public_opinion(VIEW_GENETICS,2,0,75);
                     break;
                  case 3:
                     strcat(msg,"intercepted and published biased internal media emails.");

                     trackdif=20;
                     crime=LAWFLAG_INFORMATION;
                     juiceval=5;
                     change_public_opinion(VIEW_CABLENEWS,5,0,75);
                     break;
                  case 4:
                     strcat(msg,"broke into military networks leaving LCS slogans.");

                     trackdif=30;
                     crime=LAWFLAG_INFORMATION;
                     juiceval=5;
                     change_public_opinion(VIEW_LIBERALCRIMESQUAD,5,0,75);
                     break;
               }

               if(trackdif>LCSrandom(hack_skill+1)+LCSrandom(10))
               {
                  for(int h=0;h<truehack.size();h++)
                  {
                     criminalize(*hack[h],crime);
                  }
               }

               for(int h=0;h<truehack.size();h++)
               {
                  addjuice(*truehack[h],juiceval,50);
               }               
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

      //CREDIT CARD FRAUD
      if(cc.size())
      {
         if(!cc_succeeded)
         {
            switch(LCSrandom(16))
            {
            case 0:strcpy(msg,"Nobody on your credit card fraud team knows what to do.");break;
            case 1:strcpy(msg,"Your credit card fraud team needs skilled hackers.");break;
            case 2:strcpy(msg,"Your credit card fraud team needs a programmer.");break;
            case 3:strcpy(msg,"Your credit card fraud team is just playing video games.");break;
            default:strcpy(msg,"");break;
            }
         }
         else if(LCSrandom(15)<=cc_skill)
         {
            if(cc.size()>1)strcpy(msg,"Your credit card fraud team has ");
            else {strcpy(msg,cc[0]->name);strcat(msg," has ");}

            strcat(msg,"run some numbers, netting $");
            char num[20];
            // *JDS* You get between $1 and $100, plus an extra $1-50 every
            // time you pass a check against your hacking skill, where chance of
            // failure is one over the adjusted hackers' skill divided by four. Once
            // a check fails, no more money is gained. This check will be made
            // up to 20 times, at which point the loop breaks. The skill check
            // here doesn't take into account funding.
            long fundgain=LCSrandom(101);
            for(int i=0;i<20 && LCSrandom(cc_skill/4);i++)
            {
               fundgain+=LCSrandom(51);
            }
            funds+=fundgain;
            stat_funds+=fundgain;
            moneygained_ccfraud+=fundgain;
            itoa(fundgain,num,10);
            strcat(msg,num);
            strcat(msg,".");

            if(fundgain/100>LCSrandom(cc_skill+1))
            {
               for(int h=0;h<cc.size();h++)
               {
                  criminalize(*cc[h],LAWFLAG_CCFRAUD);
               }
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

      //DENIAL OF SERVICE RACKET
      if(ddos.size())
      {
         if(!ddos_succeeded)
         {
            switch(LCSrandom(16))
            {
            case 0:strcpy(msg,"Nobody on your denial of service racket team knows what to do.");break;
            case 1:strcpy(msg,"Your denial of service racket team needs a skilled hacker.");break;
            case 2:strcpy(msg,"Your denial of service racket team isn't scaring anyone.");break;
            case 3:strcpy(msg,"Your denial of service racket team needs a programmer.");break;
            default:strcpy(msg,"");break;
            }
         }
         else if(LCSrandom(100)<=ddos_skill)
         {
            if(ddos.size()>1)strcpy(msg,"Your DoS team has ");
            else {strcpy(msg,ddos[0]->name);strcat(msg," has ");}

            int trackdif=0;
            unsigned short crime=0;

            // hit means they had to take the site down;
            // no hit means the target surrendered before
            // the attack was launched
            bool hit=LCSrandom(100)>ddos_skill;
            
            if(hit) strcat(msg,"hit a ");
            else strcat(msg,"extorted a ");

            switch(LCSrandom(5))
            {
            case 0:strcat(msg,"large corporation");break;
            case 1:strcat(msg,"betting company");break;
            case 2:strcat(msg,"political website");break;
            case 3:strcat(msg,"news agency");break;
            case 4:strcat(msg,"government agency");break;
            }
            strcat(msg,", netting $");
            char num[20];

            long fundgain=200;
            for(int i=0;i<19 && LCSrandom(ddos_skill/(2+i));i++)
            {
               fundgain+=200;
            }
            funds+=fundgain;
            stat_funds+=fundgain;
            moneygained_extortion+=fundgain;
            itoa(fundgain,num,10);
            strcat(msg,num);
            strcat(msg,".");

            if(hit)trackdif=fundgain/1000; // Easier to track if they fight
            else trackdif=!LCSrandom(3); // Harder to track if they give in

            if(trackdif>LCSrandom(ddos_skill+1))
            {
               for(int h=0;h<ddos.size();h++)
               {
                  criminalize(*ddos[h],LAWFLAG_RACKETEERING);
               }
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
      
      //HARASS WEBSITES
      if(web.size())
      {
         if(!web_succeeded)
         {
            switch(LCSrandom(16))
            {
            case 0:strcpy(msg,"Your website harassment team trolls some forums.");break;
            case 1:strcpy(msg,"Your website harassment team sends spam emails.");break;
            case 2:strcpy(msg,"Your website harassment team posts mean comments.");break;
            case 3:strcpy(msg,"Your website harassment team bad-mouths a website.");break;
            default:strcpy(msg,"");break;
            }
         }
         else if(LCSrandom(10)<=web_skill)
         {
            int issue=LCSrandom(VIEWNUM-4); 
            int crime;

            // Maybe do a switch on issue here to specify which website it was, but I don't feel like
            // doing that right now

            if(web.size()>1)strcpy(msg,"Your website harassment team has ");
            else {strcpy(msg,web[0]->name);strcat(msg," has ");}

            switch(LCSrandom(5))
            {
            case 0:strcat(msg,"defaced");crime=LAWFLAG_INFORMATION;break;
            case 1:strcat(msg,"knocked out");crime=LAWFLAG_COMMERCE;break;
            case 2:strcat(msg,"slandered");crime=LAWFLAG_SPEECH;break;
            case 3:strcat(msg,"threatened");crime=LAWFLAG_SPEECH;break;
            case 4:strcat(msg,"hacked");crime=LAWFLAG_INFORMATION;break;
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

            if(!LCSrandom(web_skill+1))
            {
               for(int h=0;h<web.size();h++)
               {
                  criminalize(*web[h],crime);
               }
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
         int issue=VIEW_LIBERALCRIMESQUAD;
         int power=1;
         int caught=0;

         if(clearformess)erase();
         else
         {
            makedelimiter(8,0);
         }

         if(graffiti[s]->activity.arg)
         {
            power=0;
            if(!LCSrandom(3))
            {
               char issuestr[60];
               getview(issuestr,graffiti[s]->activity.arg);
               issue=graffiti[s]->activity.arg;
               power=LCSrandom((graffiti[s]->skill[SKILL_ART]+graffiti[s]->attval(ATTRIBUTE_HEART))/5+1)+1;

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(graffiti[s]->name);
               addstr(" has completed a");
               if(power>3)addstr(" beautiful");
               addstr(" mural about ");
               addstr(issuestr);
               addstr(".");
               
               graffiti[s]->activity.arg=0;
               addjuice(*graffiti[s],power,power*20);
               change_public_opinion(issue,power);
               graffiti[s]->skill_ip[SKILL_ART]+=max(10-graffiti[s]->skill[SKILL_ART]/2,1);
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
               graffiti[s]->skill_ip[SKILL_ART]+=max(10-graffiti[s]->skill[SKILL_ART]/2,1);
               refresh();
               getch();
            }
         }
         else if(!LCSrandom(max(30-graffiti[s]->skill[SKILL_ART]*2,5)))
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
            graffiti[s]->skill_ip[SKILL_ART]+=max(10-graffiti[s]->skill[SKILL_ART]/2,1);
            refresh();
            getch();
         }
         


         if(!LCSrandom((graffiti[s]->skill[SKILL_STREETSENSE]*3+
                        graffiti[s]->attval(ATTRIBUTE_INTELLIGENCE))*3))
         {
            if(clearformess)erase();
            else
            {
               makedelimiter(8,0);
            }

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            if(LCSrandom(graffiti[s]->skill[SKILL_STREETSENSE]*3+
                         graffiti[s]->attval(ATTRIBUTE_AGILITY)*3))
            {
               addstr(graffiti[s]->name);
               addstr(" was spotted by the police");
               criminalize(*graffiti[s],LAWFLAG_VANDALISM);
               graffiti[s]->skill_ip[SKILL_STREETSENSE]+=20;

               if(graffiti[s]->activity.arg)
               {
                  addstr(" and forced to abandon the mural.");
                  graffiti[s]->activity.arg=0;
               }
               else addstr(" while spraying an LCS tag.");

               refresh();
               getch();
            }
            else
            {
               addstr(graffiti[s]->name);
               addstr(" has been arrested while spraying graffiti.");

               refresh();
               getch();

               caught=1;

               removesquadinfo(*graffiti[s]);
               graffiti[s]->carid=-1;
               graffiti[s]->location=ps;
               graffiti[s]->weapon.type=WEAPON_NONE;
               graffiti[s]->weapon.ammo=0;
               graffiti[s]->activity.type=ACTIVITY_NONE;
               criminalize(*graffiti[s],LAWFLAG_VANDALISM);
            }
         }

         graffiti[s]->skill_ip[SKILL_ART]+=max(4-graffiti[s]->skill[SKILL_ART],0);
         if(issue=VIEW_LIBERALCRIMESQUAD)
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
      if(LCSrandom(3))continue;
      char num[20];

      long fundgain=0;
      char caught=0;

      if(clearformess)erase();
      else
      {
         makedelimiter(8,0);
      }

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(prostitutes[p]->name);
      addstr(" gives it up for $");
      if(prostitutes[p]->skill[SKILL_SEDUCTION]>9)fundgain=LCSrandom(201)+200;
      else if(prostitutes[p]->skill[SKILL_SEDUCTION])fundgain=LCSrandom(20*prostitutes[p]->skill[SKILL_SEDUCTION]+1)+
                                                                        5*prostitutes[p]->skill[SKILL_SEDUCTION];
      else fundgain=LCSrandom(21)+20;
      itoa(fundgain,num,10);
      addstr(num);
      addstr("!");

      addjuice(*prostitutes[p],-!LCSrandom(3));
      prostitutes[p]->skill_ip[SKILL_SEDUCTION]+=max(10-prostitutes[p]->skill[SKILL_SEDUCTION],0);

      refresh();
      getch();


      if(!LCSrandom(100) &&                                    // Police sting?
         !LCSrandom(prostitutes[p]->skill[SKILL_STREETSENSE])) // Street sense to avoid
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

         addjuice(*prostitutes[p],-7);

         refresh();
         getch();

         caught=1;

         removesquadinfo(*prostitutes[p]);
         prostitutes[p]->carid=-1;
         prostitutes[p]->location=ps;
         prostitutes[p]->weapon.type=WEAPON_NONE;
         prostitutes[p]->weapon.ammo=0;
         prostitutes[p]->activity.type=ACTIVITY_NONE;
         criminalize(*prostitutes[p],LAWFLAG_PROSTITUTION);
      }
      else prostitutes[p]->skill_ip[SKILL_STREETSENSE]+=max(5-prostitutes[p]->skill[SKILL_STREETSENSE],0);

      /*{ //HUSTLING CODE NOT CURRENTLY USED
         if(clearformess)erase();
         else
         {
            makedelimiter(8,0);
         }

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);
         addstr(trouble[t]->name);
         switch(LCSrandom(2))
         {
            case 0:addstr(" made some money playing shell games.");break;
            case 1:addstr(" found some money on the ground.");break;
         }

         refresh();
         getch();

         fundgain=LCSrandom(11)+10;
      }*/

      if(!caught)
      {
         funds+=fundgain;
         stat_funds+=fundgain;
         moneygained_hustling+=fundgain;
      }
   }

   //TROUBLE
   if(trouble.size()>0)
   {
      char num[20];

      long juiceval=0;
      char done=0;
      unsigned short crime=0;

      if(clearformess)erase();
      else
      {
         makedelimiter(8,0);
      }

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      if(trouble.size()>1)addstr("Your Activists have ");
      else {addstr(trouble[0]->name);addstr(" has ");}

      long power=0;
      for(int t=0;t<trouble.size();t++)
      {
         power+=trouble[t]->skill[SKILL_PERSUASION]+
                  trouble[t]->attval(ATTRIBUTE_CHARISMA)+
                  trouble[t]->attval(ATTRIBUTE_AGILITY)+
                  trouble[t]->attval(ATTRIBUTE_HEART)+
                  trouble[t]->attval(ATTRIBUTE_STRENGTH)+
                  trouble[t]->attval(ATTRIBUTE_INTELLIGENCE);
      }

      long mod=1;
      if(LCSrandom(100)<power)mod++;
      if(LCSrandom(100)<power)mod++;
      if(LCSrandom(1000)<power)mod++;
      if(LCSrandom(1000)<power)mod++;
      if(LCSrandom(10000)<power)mod++;
      if(LCSrandom(10000)<power)mod++;

      do
      {
         switch(LCSrandom(8))
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
                  public_interest[VIEW_ABORTION]+=mod;
                  background_liberal_influence[VIEW_ABORTION]+=mod;
                  juiceval=1;
                  done=1;
               }
               break;
            }
            case 3:
            {
               addstr("distributed fliers graphically illustrating prison life!");
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
               public_interest[VIEW_PRISONS]+=mod;
               background_liberal_influence[VIEW_PRISONS]+=mod;
               juiceval=1;
               done=1;
               break;
            }
            case 4:
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
            case 5:
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
            case 6:
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
            case 7:
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
         }
      }while(!done);

      refresh();
      getch();

      if(crime!=0)
      {
         for(int t=0;t<trouble.size();t++)
         {
            if(!LCSrandom(10) &&                                         // Police called?
               !LCSrandom(trouble[t]->attval(ATTRIBUTE_AGILITY)+
                          trouble[t]->attval(ATTRIBUTE_INTELLIGENCE)+    // Use your instincts...
                          trouble[t]->skill[SKILL_DISGUISE]+
                          trouble[t]->skill[SKILL_STREETSENSE]))         // ...and your skills!
            {
               if(clearformess)erase();
               else
               {
                  makedelimiter(8,0);
               }

               if(LCSrandom(2))
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(trouble[t]->name);
                  addstr(" has been arrested.");

                  refresh();
                  getch();

                  removesquadinfo(*trouble[t]);
                  trouble[t]->carid=-1;
                  trouble[t]->location=ps;
                  trouble[t]->weapon.type=WEAPON_NONE;
                  trouble[t]->weapon.ammo=0;
                  trouble[t]->activity.type=ACTIVITY_NONE;
                  criminalize(*trouble[t],crime);
               }
               else if(trouble[t]->weapon.type==WEAPON_NONE)
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(trouble[t]->name);
                  addstr(" is cornered by a mob of angry rednecks.");

                  refresh();
                  getch();

                  trouble[t]->activity.type=ACTIVITY_CLINIC;

                  if(clearformess)erase();
                  else
                  {
                     makedelimiter(8,0);
                  }

                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(trouble[t]->name);
                  addstr(" is severely beaten before the mob is broken up.");

                  refresh();
                  getch();

                  if(trouble[t]->juice>=100||LCSrandom(100)<trouble[t]->juice)
                  {
                     addjuice(*trouble[t],-5);
                     if(trouble[t]->blood>50)trouble[t]->blood=50;
                  }
                  else
                  {
                     addjuice(*trouble[t],-10);
                     if(trouble[t]->blood>10)trouble[t]->blood=10;

                     if(!LCSrandom(5))
                     {
                        switch(LCSrandom(15))
                        {
                           case 0:
                              if(trouble[t]->special[SPECIALWOUND_LOWERSPINE]==1)
                              {
                                 set_color(COLOR_WHITE,COLOR_BLACK,1);
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
                                 set_color(COLOR_WHITE,COLOR_BLACK,1);
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
                                 set_color(COLOR_WHITE,COLOR_BLACK,1);
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
                                 set_color(COLOR_WHITE,COLOR_BLACK,1);
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
         addjuice(*trouble[h],juiceval,20);
      }
   }

   //Teaching
   for(int t=0;t<teachers.size();t++)
   {
      int skillarray[10];
      int cost, students=0;
      //Build a list of skills to train and determine the cost for running
      //a class depending on what the teacher is teaching
      switch(teachers[t]->activity.type)
      {
      case ACTIVITY_TEACH_GENERALED:
         cost=50;
         skillarray[0]=SKILL_COMPUTERS;
         skillarray[1]=SKILL_WRITING;
         skillarray[2]=SKILL_MUSIC;
         skillarray[3]=SKILL_ART;
         skillarray[4]=SKILL_RELIGION;
         skillarray[5]=SKILL_BUSINESS;
         skillarray[6]=SKILL_SCIENCE;
         skillarray[7]=-1;
         break;
      case ACTIVITY_TEACH_POLITICS:
         cost=50;
         skillarray[0]=SKILL_LAW;
         skillarray[1]=SKILL_PERSUASION;
         skillarray[2]=SKILL_LEADERSHIP;
         skillarray[3]=-1;
         break;
      case ACTIVITY_TEACH_SURVIVAL:
         cost=50;
         skillarray[0]=SKILL_DRIVING;
         skillarray[1]=SKILL_MEDICAL;
         skillarray[2]=SKILL_SURVIVAL;
         skillarray[3]=SKILL_STREETSENSE;
         skillarray[4]=SKILL_GARMENTMAKING;
         skillarray[5]=SKILL_HANDTOHAND;
         skillarray[6]=SKILL_IMPROVISED;
         skillarray[7]=SKILL_COOKING;
         skillarray[8]=-1;
         break;
      case ACTIVITY_TEACH_FIGHTING:
         cost=50;
         skillarray[0]=SKILL_KNIFE;
         skillarray[1]=SKILL_SWORD;
         skillarray[2]=SKILL_CLUB;
         skillarray[3]=SKILL_PISTOL;
         skillarray[4]=SKILL_RIFLE;
         skillarray[5]=SKILL_SMG;
         skillarray[6]=SKILL_SHOTGUN;
         skillarray[7]=-1;
         break;
      case ACTIVITY_TEACH_COVERT:
         cost=50;
         skillarray[0]=SKILL_PERSUASION;
         skillarray[1]=SKILL_SECURITY;
         skillarray[2]=SKILL_DISGUISE;
         skillarray[3]=SKILL_STEALTH;
         skillarray[4]=SKILL_SEDUCTION;
         skillarray[5]=SKILL_INTERROGATION;
         skillarray[6]=-1;
         break;
      }

      //Walk through and train people
      for(int p=0;p<pool.size();p++)
      {
         //If they're at the location
         if(pool[p]->location==teachers[t]->location &&
            pool[p]->align==ALIGN_LIBERAL)
         {
            //Step through the array of skills to train
            for(int i=0;i<10;i++)
            {
               //If no more skills to train, stop
               if(skillarray[i]==-1)break;
               //Otherwise, if the student has less skill than the teacher, train the student
               //proportional to the difference in skill between teacher and student times the
               //teacher's ability at teaching
               if(pool[p]->skill[skillarray[i]]<teachers[t]->skill[skillarray[i]]-1&&
                  pool[p]->skill[skillarray[i]]<teachers[t]->skill[SKILL_TEACHING]+2&&
                  funds>cost&&
                  pool[p]->skill[skillarray[i]]<maxskill(skillarray[i],*pool[p]))
               {
                  int teach=teachers[t]->skill[skillarray[i]]-pool[p]->skill[skillarray[i]]+
                                                    teachers[t]->skill[SKILL_TEACHING];
                  if(teach>10)teach=10;
                  pool[p]->skill_ip[skillarray[i]]+=teach;

                  if(students<10)
                  {
                     students++;
                     funds-=cost;
                     moneylost_training+=cost;
                     if(students==10)cost=0;
                  }
                  //No crime for now
                  /*if(pool[p]->heat>teachers[t]->heat)
                  {
                     if(teachers[t]->lawflag[LAWFLAG_RACKETEERING]==0)
                        teachers[t]->lawflag[LAWFLAG_RACKETEERING]=1;
                  }*/
               }
            }
         }
      }

      teachers[t]->skill_ip[SKILL_TEACHING]+=students;
   }

   //BURY THE DEAD
   if(bury.size()>0)
   {
      char haveburied=0;
      char caught=0;

      for(int p=pool.size()-1;p>=0;p--)
      {
         if(pool[p]->alive)continue;

         //MAKE BASE LOOT
         makeloot(*pool[p],location[bury[0]->base]->loot);

         //BURY
         delete pool[p];
         pool.erase(pool.begin() + p);

         if(!caught)
         {
            long dodgelawroll=0,ndodgelawroll;

            for(int b=0;b<bury.size();b++)
            {
               ndodgelawroll=LCSrandom(bury[b]->skill[SKILL_PERSUASION]+
                  bury[b]->skill[SKILL_DISGUISE]+
                  bury[b]->skill[SKILL_STREETSENSE]+
                  bury[b]->attval(ATTRIBUTE_CHARISMA)+
                  bury[b]->attval(ATTRIBUTE_AGILITY)+
                  bury[b]->attval(ATTRIBUTE_INTELLIGENCE)+1);
               if(ndodgelawroll>dodgelawroll)dodgelawroll=ndodgelawroll;
            }
            if(dodgelawroll==0)caught=1;
         }
      }

      if(caught)
      {
         if(clearformess)erase();
         else
         {
            makedelimiter(8,0);
         }

         if(bury.size()>0)addstr("Some Liberals have");
         else {addstr(bury[0]->name);addstr(" has");}
         addstr(" been while disposing of bodies.");

         refresh();
         getch();

         for(int b=0;b<bury.size();b++)
         {
            removesquadinfo(*bury[b]);
            bury[b]->carid=-1;
            bury[b]->location=ps;
            bury[b]->weapon.type=WEAPON_NONE;
            bury[b]->weapon.ammo=0;
            bury[b]->activity.type=ACTIVITY_NONE;
            criminalize(*bury[b],LAWFLAG_BURIAL);

         }
      }
   }
}



/* steal a car */
char stealcar(creaturest &cr,char &clearformess)
{
   clearformess=1;

   short cartype;
   char str[80];

   if(carselect(cr,cartype))
   {
      long diff=difficulty_carfind(cartype);

      vehiclest *v=new vehiclest;

      int old=cartype;

      //ROUGH DAY
      if(LCSrandom(10)<diff-cr.skill[SKILL_STREETSENSE])
      {
         do
         {
            cartype=LCSrandom(VEHICLENUM);
            if(LCSrandom(10)<difficulty_carfind(cartype))cartype=old;
         }while(cartype==old);
      }

      v->init(cartype);

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

      if(old!=cartype)
      {
         move(11,0);
         addstr(cr.name);
         addstr(" was unable to find a ");
         getcarfull(str,old);
         addstr(str);
         addstr(" but did find a ");
         getcarfull(str,cartype);
         addstr(str);
         addstr(".");

         refresh();
         getch();
      }
      else
      {
         move(11,0);
         addstr(cr.name);
         addstr(" found a ");
         getcarfull(str,cartype);
         addstr(str);
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
      getcarfull(str,*v);
      addstr(str);
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
      if(LCSrandom(100)<sensealarmchance(cartype))sensealarm=1;
      char touchalarm=0;
      if(LCSrandom(100)<touchalarmchance(cartype))touchalarm=1;
      char windowdamage=0;

      do
      {
         erase();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(0,0);
         addstr("Adventures in Liberal Car Theft");
         printcreatureinfo(&cr);
         makedelimiter(8,0);

         getcarfull(str,*v);

         if(alarmon)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(10,0);
            if(sensealarm)addstr("THE VIPER");
            else addstr(str);
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
            addstr(str);
            addstr(".");
         }

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(12,0);
         addstr("A - Pick the lock.");
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
            if(c=='x'){delete v;return 1;}
         }while(1);

         char entered=0;

         //PICK LOCK
         if(method==0)
         {
            int attack=cr.skill[SKILL_SECURITY];
            //if(!LCSrandom(20))attack++;

            if(LCSrandom(6)<attack)
            {
               cr.skill_ip[SKILL_SECURITY]+=max(5-cr.skill[SKILL_SECURITY],0);
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
            int attack=cr.attval(ATTRIBUTE_STRENGTH)+
               bashstrengthmod(cr.weapon.type);

            if(LCSrandom(11)<attack+windowdamage)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,0);
               addstr(cr.name);
               addstr(" smashes the window");
               if(bashstrengthmod(cr.weapon.type)>0)
               {
                  addstr(" with a ");
                  char str[80];
                  getweaponfull(str,cr.weapon.type,2);
                  addstr(str);
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
               addstr(" wacks the window");
               if(bashstrengthmod(cr.weapon.type)>0)
               {
                  addstr(" with a ");
                  char str[80];
                  getweaponfull(str,cr.weapon.type,2);
                  addstr(str);
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
               delete v;return 1;}
            else {
               mode=GAMEMODE_BASE;
               delete v;return 0;}
         }

         if(entered)break;
      }while(1);

      //START CAR
      char keys_in_car=0;
      if(!LCSrandom(5))keys_in_car=1;

      do
      {
         erase();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(0,0);
         addstr("Adventures in Liberal Car Theft");
         printcreatureinfo(&cr);
         makedelimiter(8,0);

         getcarfull(str,*v);

         int y=10;

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);y++;
         addstr(cr.name);
         addstr(" is behind the wheel of a ");
         addstr(str);
         addstr(".");

         if(alarmon)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(y,0);y++;
            if(sensealarm)addstr("THE VIPER");
            else addstr(str);
            addstr(":   ");
            set_color(COLOR_RED,COLOR_BLACK,1);
            if(sensealarm)addstr("REMOVE YOURSELF FROM THE VEHICLE!   <BEEP!!> <BEEP!!>");
            else addstr("<BEEP!!> <BEEP!!> <BEEP!!> <BEEP!!>");
         }

         y++;

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);y++;
         addstr("A - Hotwire the car.");
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
            if(c=='x'){delete v;return 1;}
         }while(1);

         char started=0;

         //HOTWIRE CAR
         if(method==0)
         {
            int attack=cr.skill[SKILL_SECURITY];
            if(!LCSrandom(20))attack++;

            if(LCSrandom(11)<attack)
            {
               cr.skill_ip[SKILL_SECURITY]+=max(7-cr.skill[SKILL_SECURITY],0);
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
               addstr(" fiddles with ignition, but the car doesn't start.");
               refresh();getch();
            }
         }
         //KEYS
         if(method==1)
         {
            int attack=cr.attval(ATTRIBUTE_INTELLIGENCE);

            if(LCSrandom(11)<attack&&keys_in_car)
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(y,0);y++;
               if(law[LAW_FREESPEECH]==-2)addstr("Holy [Car Keys]!  ");
               else addstr("Holy shit!  ");
               addstr(cr.name);
               addstr(" found the keys ");
               switch(LCSrandom(5))
               {
                  case 0:addstr("in the ignition.  Damn.");break;
                  case 1:addstr("under the front seat!");break;
                  case 2:addstr("under the back seat!");break;
                  case 3:addstr("above the pull-down sunblock thingy!");break;
                  case 4:addstr("in the glove compartment!");break;
               }
               refresh();getch();

               started=1;
            }
            else
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(y,0);y++;
               addstr(cr.name);
               addstr(": <rummaging> ");
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               switch(LCSrandom(5))
               {
                  case 0:addstr("They've gotta be in here somewhere...");break;
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
               delete v;return 1;}
            else {
               mode=GAMEMODE_BASE;
               delete v;return 0;}
         }

         if(started)break;
      }while(1);

      //CHASE SEQUENCE
         //CAR IS OFFICIAL, THOUGH CAN BE DELETE BY chasesequence()
      if(v->type==VEHICLE_SUV||
         v->type==VEHICLE_POLICECAR)addjuice(cr,2,50);

      chaseseq.clean();
      chaseseq.location=0;
      int chaselev=!LCSrandom(13-windowdamage);
      if(chaselev>0||(v->type==VEHICLE_POLICECAR&&LCSrandom(2)))
      {
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
         v->location=cr.base;
         return 1;
      }
      else return 0;//do not need to delete vehicle
   }

   return 0;
}



long difficulty_carfind(int type)
{
   switch(type)
   {
      case VEHICLE_VAN:return 2;
      case VEHICLE_STATIONWAGON:return 1;
      case VEHICLE_SPORTSCAR:return 3;
      case VEHICLE_BUG:return 2;
      case VEHICLE_PICKUP:return 1;
      case VEHICLE_POLICECAR:return 7;
      case VEHICLE_TAXICAB:return 8;
      case VEHICLE_SUV:return 1;
      case VEHICLE_JEEP:return 6;
   }

   return 10;
}



char carselect(creaturest &cr,short &cartype)
{
   cartype=-1;

   vector<int> cart;
   for(int a=0;a<VEHICLENUM;a++)
   {
      if(difficulty_carfind(a)<10)cart.push_back(a);
   }

   short page=0;

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
         getcarfull(str,cart[p]);
         addstr(str);

         move(y,49);
         difficulty=difficulty_carfind(cart[p]);
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

      if(c==10)break;
   }while(1);

   return 0;
}



long sensealarmchance(int ct)
{
   switch(ct)
   {
      case VEHICLE_SPORTSCAR:
         return 15;
      case VEHICLE_SUV:
         return 4;
      case VEHICLE_JEEP:
      case VEHICLE_VAN:
      case VEHICLE_STATIONWAGON:
      case VEHICLE_BUG:
      case VEHICLE_PICKUP:
         return 1;
      default:
         return 0;
   }
}


long touchalarmchance(int ct)
{
   switch(ct)
   {
      case VEHICLE_SPORTSCAR:
         return 95;
      case VEHICLE_SUV:
         return 85;
      case VEHICLE_JEEP:
      case VEHICLE_VAN:
      case VEHICLE_STATIONWAGON:
      case VEHICLE_BUG:
      case VEHICLE_PICKUP:
         return 10;
      default:
         return 0;
   }
}



/* get a wheelchair */
void getwheelchair(creaturest &cr,char &clearformess)
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
