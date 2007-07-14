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


/* hostage tending */
void tendhostage(creaturest *cr,char &clearformess)
{
   vector<creaturest *> temppool;
   int p;

   long hfunds=0;
   creaturest *a=NULL;

   for(p=0;p<pool.size();p++)
   {
      if(!pool[p]->alive)continue;
      if(pool[p]->activity.type==ACTIVITY_HOSTAGETENDING)
      {
         if(pool[p]->activity.arg==cr->id)
         {
            temppool.push_back(pool[p]);
         }
      }
   }

   if(temppool.size()==0)
   {
      //CHECK FOR HOSTAGE ESCAPE
      if(LCSrandom(200)<cr->attval(ATTRIBUTE_INTELLIGENCE)+
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
               refresh();
               getch();

               delete pool[p];
               pool.erase(pool.begin() + p);
               break;
            }
         }
      }

      return;
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
   move(1,0);
   addstr("The Automaton is ");
   printhealthstat(*cr,1,17,0);
   
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
      int gangsterism=0;

      int spiritcrush=cr->joindays;
      int healthcrush=0;

      for(p=0;p<temppool.size();p++)
      {
         if(temppool[p]!=NULL)
         {
            if(temppool[p]->alive)
            {
               if(temppool[p]->skill[SKILL_BUSINESS]>business)
                  business=temppool[p]->skill[SKILL_BUSINESS];
               if(temppool[p]->skill[SKILL_RELIGION]>religion)
                  religion=temppool[p]->skill[SKILL_RELIGION];
               if(temppool[p]->skill[SKILL_SCIENCE]>science)
                  science=temppool[p]->skill[SKILL_SCIENCE];
               if(temppool[p]->skill[SKILL_GANGSTERISM]>gangsterism)
                  gangsterism=temppool[p]->skill[SKILL_GANGSTERISM];

               if((temppool[p]->attval(ATTRIBUTE_CHARISMA)+
                  temppool[p]->attval(ATTRIBUTE_HEART)+
                  temppool[p]->skill[SKILL_INTERROGATION]*2)>maxattack)
               {
                  maxattack=temppool[p]->attval(ATTRIBUTE_CHARISMA)+
                     temppool[p]->attval(ATTRIBUTE_HEART)+
                     temppool[p]->skill[SKILL_INTERROGATION]*2;
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
               if((temppool[p]->attval(ATTRIBUTE_CHARISMA)+
                     temppool[p]->attval(ATTRIBUTE_HEART)+
                     temppool[p]->skill[SKILL_INTERROGATION]*2)==maxattack)
               {
                  goodp.push_back(p);
               }
            }
         }
      }
      creaturest *a=temppool[goodp[LCSrandom(goodp.size())]];

      maxattack+=temppool.size();

      if(cr->skill[SKILL_BUSINESS])maxattack+=business-cr->skill[SKILL_BUSINESS];
      if(cr->skill[SKILL_RELIGION])maxattack+=religion-cr->skill[SKILL_RELIGION];
      if(cr->skill[SKILL_SCIENCE])maxattack+=science-cr->skill[SKILL_SCIENCE];
      if(cr->skill[SKILL_GANGSTERISM])maxattack+=gangsterism-cr->skill[SKILL_GANGSTERISM];

      long aroll=LCSrandom(maxattack)+LCSrandom(10);
      long troll=LCSrandom(cr->attval(ATTRIBUTE_WISDOM)*2+
         cr->skill[SKILL_INTERROGATION]*2)+LCSrandom(15);

      bool techniques[9]={1,1,0};
      while(1)
      {
         y=3;
         if(techniques[8])
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            move(y,0);y+=2;addstr("The Execution of an Automoton         ");
         }
         else
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(y,0);y+=2;addstr("Selecting a Liberal Interrogation Plan");
         }
         if(techniques[8])set_color(COLOR_BLACK,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,techniques[0]);
         move(y,0);y+=1;addstr("A - Verbal Interrogation");
         if(!techniques[8])set_color(COLOR_WHITE,COLOR_BLACK,techniques[1]);
         move(y,0);y+=1;addstr("B - Physical Restraints");
         if(!techniques[8])set_color(COLOR_WHITE,COLOR_BLACK,techniques[2]);
         move(y,0);y+=1;addstr("C - Violent Beating");
         if(!techniques[8])set_color(COLOR_WHITE,COLOR_BLACK,techniques[3]);
         move(y,0);y+=1;addstr("D - No Sleep");
         if(!techniques[8])set_color(COLOR_WHITE,COLOR_BLACK,techniques[4]);
         move(y,0);y+=1;addstr("E - No Food");
         if(!techniques[8])set_color(COLOR_WHITE,COLOR_BLACK,techniques[5]);
         move(y,0);y+=1;addstr("F - No Water");
         if(!techniques[8])set_color(COLOR_WHITE,COLOR_BLACK,techniques[6]);
         move(y,0);y+=1;addstr("G - No Light");
         if(!techniques[8])set_color(COLOR_WHITE,COLOR_BLACK,techniques[7]);
         move(y,0);y+=2;addstr("H - Psychadelic Drugs");
         if(techniques[8])set_color(COLOR_RED,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);y+=2;addstr("K - Kill the Hostage");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);y+=1;addstr("Press Enter to Confirm the Plan");
         int c=getch();
         translategetch(c);
         if(c>='a'&&c<='h')techniques[c-'a']=!techniques[c-'a'];
         if(c=='k')techniques[8]=!techniques[8];
         if(c==10)break;
      }

      if(techniques[8]) // Kill the Hostage
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
            if(LCSrandom(10)<temppool[i]->juice&&
               LCSrandom(9)+1>=temppool[i]->attval(ATTRIBUTE_HEART,0))
            {
               a=temppool[i];
               break;
            }
         }

         if(a)
         {
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
         else
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,0);
            move(y,0);y++;
            addstr("There is no one able to get up the nerve to");
            move(y,0);y++;
            addstr("execute ");
            addstr(cr->name);
            addstr(" in cold blood.");
         }
         refresh();
         getch();

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(24,0);
         addstr("Press any key to reflect on this.");

         refresh();
         getch();

         return;
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
      if(techniques[1]) // Restraint
      {
         if(techniques[6]) // No light
         {
            addstr(" is tied hands and feet to a metal chair in a dark closet.");
            spiritcrush+=LCSrandom(3);
         }
         else
         {
            addstr(" is tied hands and feet to a metal chair");
            y++;move(y,0);
            addstr("in the middle of a back room.");
         }
         y++;

         spiritcrush+=LCSrandom(2);
      }
      else if(techniques[6]) // No light
      {
         addstr(" is locked in a dark room without any light.");
         y++;

         spiritcrush+=LCSrandom(3);
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
      if(techniques[4]) // no food
      {
         if(techniques[5])
         {
            addstr("It is left to hunger and thirst like a lost animal.");
            spiritcrush+=LCSrandom(2);
            healthcrush+=LCSrandom(3)+1;
            cr->blood-=20;
         }
         else
         {
            addstr("It is forced to beg for food and water, and never recieves any food.");
         }
         y++;
         spiritcrush+=LCSrandom(2);
         healthcrush+=LCSrandom(2)+1;
         cr->blood-=5;
         refresh();
         getch();
      }
      else if(techniques[5])
      {
         addstr("The only water it recieves comes in the scraps of food provided to sustain it.");
         spiritcrush+=LCSrandom(2);
         healthcrush+=LCSrandom(3)+1;
         cr->blood-=10;
         refresh();
         getch();
      }

      if(techniques[3]&&!turned&&cr->alive) // No sleep
      {
         move(y,0);
         addstr("A trance of Liberal tracts and endless protest music substitutes for sleep.");
         y++;

         spiritcrush+=LCSrandom(3);
         healthcrush+=2;

         refresh();
         getch();
      }
      
      if(techniques[7]) // Psychadelic drugs
      {
         move(y,0);
         addstr("It is forced to ingest psychadelic drugs...");
         y++;

         spiritcrush+=LCSrandom(5);
         healthcrush+=3;

         refresh();
         getch();
      }

      // Verbal Interrogation
      if(techniques[0])
      {
         y+=1;
         move(y,0);
         addstr(a->name);
         switch(LCSrandom(4))
         {
         case 0:addstr(" debates political issues with ");break;
         case 1:addstr(" argues about the LCS with ");break;
         case 2:addstr(" tries to expose the true Liberal side of ");break;
         case 3:addstr(" attempts to recruit ");break;
         }
         addstr(cr->name);
         addstr(".");
         y++;

         refresh();
         getch();

         if(cr->skill[SKILL_RELIGION]>religion+spiritcrush)
         {
            move(y,0);
            addstr(a->name);
            addstr(" is unable to shake ");
            addstr(cr->name);
            addstr("'s religious conviction.");
            y++;
         }
         else if(cr->skill[SKILL_BUSINESS]>business+spiritcrush)
         {
            move(y,0);
            addstr(cr->name);
            addstr(" just laughs at ");
            addstr(a->name);
            addstr("'s economic arguments.");
            y++;
         }
         else if(cr->skill[SKILL_SCIENCE]>science+spiritcrush)
         {
            move(y,0);
            addstr(cr->name);
            addstr(" merely pities ");
            addstr(a->name);
            addstr("'s ignorance of scientific truth.");
            y++;
         }
         else if(cr->skill[SKILL_GANGSTERISM]>gangsterism+spiritcrush)
         {
            move(y,0);
            addstr(cr->name);
            addstr(" falls back on gang lingo that confuses ");
            addstr(a->name);
            addstr(".");
            y++;
         }
         else if(LCSrandom(spiritcrush*2+1)+LCSrandom(healthcrush*2+1) >
            cr->att[ATTRIBUTE_WISDOM]+cr->att[ATTRIBUTE_HEART]+cr->att[ATTRIBUTE_HEALTH])
         {
            move(y,0);
            addstr(cr->name);
            switch(LCSrandom(3))
            {
            case 0:addstr(" barely responds.");break;
            case 1:addstr(" just whimpers.");break;
            case 2:addstr(" cries helplessly.");break;
            }
            if(cr->att[ATTRIBUTE_HEART]>1)cr->att[ATTRIBUTE_HEART]--;
            y++;
         }
         else if(aroll+spiritcrush>troll)
         {
            cr->att[ATTRIBUTE_WISDOM]--;
            if(cr->att[ATTRIBUTE_HEART]<5)cr->att[ATTRIBUTE_HEART]++;
            if(cr->att[ATTRIBUTE_WISDOM]<1)
            {
               turned=1;
               cr->att[ATTRIBUTE_WISDOM]=1;
            }
            else if(LCSrandom(11)>(cr->att[ATTRIBUTE_WISDOM]*5)-cr->att[ATTRIBUTE_HEART]-cr->joindays)turned=1;

            move(y,0);
            addstr(cr->name);
            addstr("'s resolve weakens...");
            y++;
         }
         else if(aroll+spiritcrush>(troll>>1))
         {
            move(y,0);
            addstr(cr->name);
            addstr(" holds firm.");
            y++;
         }
         else
         {
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

         a->skill_ip[SKILL_INTERROGATION]+=(troll>>1)+1;
         getch();
      }

      if(techniques[2]&&!turned) // Beating
      {
         y+=1;
         move(y,0);
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
         addstr(" the Automaton, yelling its");
         y++;
         move(y,0);
         addstr("favorite corporation's name.");
         y++;

         refresh();
         getch();

         long forceroll=LCSrandom(((long)temppool.size())*10+1);

         if(forceroll>=cr->attval(ATTRIBUTE_HEALTH)+cr->skill[SKILL_SURVIVAL])
         {
            if(cr->skill[SKILL_RELIGION]>spiritcrush)
            {
               move(y,0);
               addstr(cr->name);
               addstr(" prays...");
               y++;
            }
            else if(cr->skill[SKILL_GANGSTERISM]>spiritcrush)
            {
               move(y,0);
               addstr(cr->name);
               addstr(" defiantly threatens retribution from their gang.");
               y++;
            }
            else if(LCSrandom(spiritcrush+1)+LCSrandom(healthcrush+1) >
               cr->att[ATTRIBUTE_WISDOM]+cr->att[ATTRIBUTE_HEART]+cr->att[ATTRIBUTE_HEALTH])
            {
               move(y,0);
               addstr(cr->name);
               switch(LCSrandom(3))
               {
               case 0:addstr(" barely responds.");break;
               case 1:addstr(" just whimpers.");break;
               case 2:addstr(" cries helplessly.");break;
               }
               if(cr->att[ATTRIBUTE_HEART]>1)cr->att[ATTRIBUTE_HEART]--;
               y++;
            }
            else 
            {
               move(y,0);
               addstr(cr->name);
               addstr("'s resolve weakens...");
               y++;
               if(cr->att[ATTRIBUTE_WISDOM]>1)
               {
                  cr->att[ATTRIBUTE_WISDOM]--;
                  if(LCSrandom(11)>(cr->att[ATTRIBUTE_WISDOM]*5)-cr->joindays)turned=1;
               }
               else turned=1;
            }
            healthcrush+=forceroll/8;
            cr->blood-=forceroll/2;

            if(forceroll>=LCSrandom(cr->attval(ATTRIBUTE_HEALTH)*5+1))
            {
               refresh();
               getch();

               cr->att[ATTRIBUTE_HEALTH]--;
               move(y,0);
               addstr(cr->name);
               addstr(" is badly hurt.");
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

      cr->blood-=2*healthcrush/cr->attval(ATTRIBUTE_HEALTH);
      if(cr->alive==0||cr->blood<1)
      {
         cr->alive=0;

         stat_kills++;
         y++;
         move(y,0);
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr(cr->name);
         addstr(" is dead.");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         y++;
      }
   }

   #ifdef AUTOENLIGHTEN
      turned=1;
   #endif

   if(turned&&cr->alive)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      y++;
      move(y,0);
      addstr("The Automaton has been Enlightened!   Your Liberal ranks are swelling!");
      y+=2;
      cr->align=1;
      stat_recruits++;

      if(!(cr->flag & CREATUREFLAG_KIDNAPPED))
      {
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         move(y,0);y++;
         addstr(cr->name);
         addstr("'s disappearance has not yet been reported.");
         move(y,0);y+=2;
         addstr("You now have a sleeper infiltrating Conservative Society.");
         cr->flag&=~CREATUREFLAG_MISSING;
         cr->flag|=CREATUREFLAG_SLEEPER;
         cr->location=cr->worklocation;
         cr->base=cr->worklocation;
      }
      refresh();
      getch();
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



/* armor repair */
void repairarmor(creaturest &cr,char &clearformess)
{
   armorst *it=NULL;
   itemst *itm=NULL;
   vector<itemst *> *lar=NULL;

   if(cr.armor.type!=ARMOR_NONE&&
      ((cr.armor.flag & ARMORFLAG_DAMAGED)||
      (cr.armor.flag & ARMORFLAG_BLOODY)))
   {
      it=&cr.armor;
   }
   else if(cr.squadid!=-1)
   {
      int sq=getsquad(cr.squadid);
      for(int l=0;l<squad[sq]->loot.size();l++)
      {
         if(squad[sq]->loot[l]->type==ITEM_ARMOR&&
            ((squad[sq]->loot[l]->armor.flag & ARMORFLAG_DAMAGED)||
            (squad[sq]->loot[l]->armor.flag & ARMORFLAG_BLOODY)))
         {
            it=&squad[sq]->loot[l]->armor;
            itm=squad[sq]->loot[l];
            lar=&squad[sq]->loot;
            break;
         }
      }
   }
   if(it==NULL&&cr.location!=-1)
   {
      for(int l=0;l<location[cr.location]->loot.size();l++)
      {
         if(location[cr.location]->loot[l]->type==ITEM_ARMOR&&
            ((location[cr.location]->loot[l]->armor.flag & ARMORFLAG_DAMAGED)||
            (location[cr.location]->loot[l]->armor.flag & ARMORFLAG_BLOODY)))
         {
            it=&location[cr.location]->loot[l]->armor;
            itm=location[cr.location]->loot[l];
            lar=&location[cr.location]->loot;
            break;
         }
      }
   }

   if(it!=NULL)
   {
      if(clearformess)erase();
      else
      {
         makedelimiter(8,0);
      }

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(8,1);
      addstr(cr.name);
      if(it->flag & ARMORFLAG_DAMAGED)addstr(" repairs ");
      else addstr(" cleans ");
      char str[80];
      getarmorfull(str,it->type,it->subtype);
      addstr(str);

      if(itm!=NULL)
      {
         if(itm->number>1)
         {
            itemst *newi=new itemst;
               *newi=*itm;
            lar->push_back(newi);
            newi->number=itm->number-1;
            itm->number=1;
         }
      }

      it->flag&=~ARMORFLAG_BLOODY;
      it->flag&=~ARMORFLAG_DAMAGED;

      long dif=(armor_makedifficulty(it->type,&cr)>>1);
      cr.skill_ip[SKILL_GARMENTMAKING]+=dif+1;

      if((LCSrandom(10)<dif||LCSrandom(10)<dif)&&it->quality!='4'&&
         (it->flag & ARMORFLAG_DAMAGED))
      {
         addstr(" but it is not quite the same.");
         it->quality++;
         if((LCSrandom(10)<dif||LCSrandom(10)<dif)&&it->quality!='4')it->quality++;
         if((LCSrandom(10)<dif||LCSrandom(10)<dif)&&it->quality!='4')it->quality++;
      }
      else addstr(".");

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
               delete squad[sq]->loot[l];
               squad[sq]->loot.erase(squad[sq]->loot.begin() + l);
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
               delete location[cr.location]->loot[l];
               location[cr.location]->loot.erase(location[cr.location]->loot.begin() + l);
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
   int v;
   int creatureskill=cr->skill[SKILL_COMPUTERS]+cr->attval(ATTRIBUTE_INTELLIGENCE);
   int misschance=0,noise=2;
   if(creatureskill<3){noise=15;misschance=(30-creatureskill)*3;}
   else if(creatureskill<7){noise=7;misschance=(20-creatureskill)*3;}
   else if(creatureskill<10){noise=5;misschance=(20-creatureskill)*2;}
   else if(creatureskill<15){noise=4;misschance=20-creatureskill;}
   else if(creatureskill<20){noise=3;misschance=20-creatureskill;}

   int survey[VIEWNUM];

   for(v=0;v<VIEWNUM;v++)
   {
      survey[v]=attitude[v];

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
         if(LCSrandom(100)<misschance)survey[v]=-1;
      #endif

      if(v==VIEW_LIBERALCRIMESQUAD&&attitude[v]==0)survey[v]=0;
   }

   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   addstr("Survey of Public Opinion, According to Recent Polls");

   int y=2;

   char num[20];

   for(v=0;v<VIEWNUM;v++)
   {
      if(survey[VIEW_LIBERALCRIMESQUAD]==0&&
         v==VIEW_LIBERALCRIMESQUADPOS)continue;

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
         addstr("Unknown how many ");
      }
      else
      {
         itoa(survey[v],num,10);
         addstr(num);
         addstr("% ");
      }

      switch(v)
      {
         case VIEW_ABORTION:addstr("supported abortion.");break;
         case VIEW_GAY:addstr("were in favor of equal rights for homosexuals.");break;
         case VIEW_DEATHPENALTY:addstr("opposed the death penalty.");break;
			case VIEW_TAXES:addstr("were against cutting taxes.");break;
         case VIEW_NUCLEARPOWER:addstr("were terrified of nuclear power.");break;
         case VIEW_ANIMALRESEARCH:addstr("deplored animal research.");break;
         case VIEW_POLICEBEHAVIOR:addstr("were critical of the police.");break;
         case VIEW_INTELLIGENCE:addstr("thought the intelligence community invades privacy.");break;
         case VIEW_FREESPEECH:addstr("believed in unfettered free speech.");break;
         case VIEW_GENETICS:addstr("abhorred genetically altered food products.");break;
         case VIEW_JUSTICES:addstr("were for the appointment of Liberal justices.");break;
         case VIEW_SWEATSHOPS:addstr("would boycott companies that used sweatshops.");break;
         case VIEW_POLLUTION:addstr("thought industry should lower pollution.");break;
         case VIEW_CORPORATECULTURE:addstr("were disgusted by corporate malfeasance.");break;
         case VIEW_CEOSALARY:addstr("believed that CEO salaries are too great.");break;
         case VIEW_LIBERALCRIMESQUAD:addstr("had heard of the Liberal Crime Squad.");break;
         case VIEW_LIBERALCRIMESQUADPOS:addstr("of these held the Liberal Crime Squad in high regard.");break;
         case VIEW_PRISONS:addstr("think the prison system needs reform.");break;
         case VIEW_AMRADIO:addstr("do not like AM radio.");break;
         case VIEW_CABLENEWS:addstr("have a negative opinion of cable news programs.");break;
      }
      y++;
   }

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(23,0);
   addstr("Results are +/- ");
   itoa(noise,num,10);
   addstr(num);
   addstr(" Liberal percentage points.");
   move(24,0);
   addstr("Press any key to reflect on these results.");

   refresh();
   getch();
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
   vector<creaturest *> brownies;
   vector<creaturest *> prostitutes;

   for(int p=0;p<pool.size();p++)
   {
      if(!pool[p]->alive)continue;
      switch(pool[p]->activity.type)
      {
         case ACTIVITY_CCFRAUD:
         case ACTIVITY_DOS_RACKET:
         case ACTIVITY_DOS_ATTACKS:
            hack.push_back(pool[p]);
            break;
         case ACTIVITY_TROUBLE:
            trouble.push_back(pool[p]);
            break;
         case ACTIVITY_COMMUNITYSERVICE:
            addjuice(*pool[p],1,0);
            change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,1,0,70);
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
      money=LCSrandom(solicit[s]->skill[SKILL_PERSUASION]+
         solicit[s]->skill[SKILL_BUSINESS]+
         solicit[s]->attval(ATTRIBUTE_CHARISMA)+
         solicit[s]->attval(ATTRIBUTE_HEART)+1);
      if(LCSrandom(100)>attitude[VIEW_LIBERALCRIMESQUADPOS])money=0;
      funds+=money;
      stat_funds+=money;
      moneygained_donate+=money;
      if(solicit[s]->skill[SKILL_PERSUASION]<3)solicit[s]->skill_ip[SKILL_PERSUASION]+=LCSrandom(5)+2;
   }

   //BROWNIES
   long dodgelawroll;
   for(s=0;s<brownies.size();s++)
   {
      money=LCSrandom(brownies[s]->skill[SKILL_PERSUASION]*5+
         brownies[s]->attval(ATTRIBUTE_CHARISMA)*5+
         brownies[s]->attval(ATTRIBUTE_INTELLIGENCE)*5+
         brownies[s]->skill[SKILL_STREETSENSE]*5+
         brownies[s]->skill[SKILL_BUSINESS]*5+1);
      funds+=money;
      stat_funds+=money;
      moneygained_brownies+=money;
      brownies[s]->skill_ip[SKILL_PERSUASION]+=LCSrandom(5)+2;
      brownies[s]->skill_ip[SKILL_STREETSENSE]+=LCSrandom(5)+2;

      dodgelawroll=LCSrandom(brownies[s]->skill[SKILL_PERSUASION]+
         brownies[s]->skill[SKILL_DISGUISE]+
         brownies[s]->skill[SKILL_GANGSTERISM]+
         brownies[s]->skill[SKILL_STREETSENSE]+
         brownies[s]->attval(ATTRIBUTE_CHARISMA)+
         brownies[s]->attval(ATTRIBUTE_AGILITY)+
         brownies[s]->attval(ATTRIBUTE_INTELLIGENCE)+1);

      if(dodgelawroll==0)
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

         refresh();
         getch();

         removesquadinfo(*brownies[s]);
         brownies[s]->carid=-1;
         brownies[s]->location=ps;
         brownies[s]->weapon.type=WEAPON_NONE;
         brownies[s]->weapon.ammo=0;
         brownies[s]->activity.type=ACTIVITY_NONE;
         brownies[s]->lawflag[LAWFLAG_BROWNIES]++;
      }
   }

   //HACKING
   if(hack.size()>0)
   {
      long hskill=0;
      for(int h=0;h<hack.size();h++)
      {
         hskill+=hack[h]->skill[SKILL_COMPUTERS];
         hskill+=hack[h]->attval(ATTRIBUTE_INTELLIGENCE);
         hack[h]->skill_ip[SKILL_COMPUTERS]+=max(LCSrandom(6)-hack[h]->skill[SKILL_COMPUTERS]/4,0);
      }

      //MAJOR HACKING EVENT
      if(LCSrandom(10000)<=hskill*2) // *JDS* hacking skill multiplier dropped significantly on major events
      {
         if(clearformess)erase();
         else
         {
            makedelimiter(8,0);
         }

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);
         if(hack.size()>1)addstr("Your Hackers have ");
         else {addstr(hack[0]->name);addstr(" has ");}

         int trackdif=0;
         unsigned short crime=0;

         long juiceval=0;

         switch(LCSrandom(2))
         {
            case 0:
            {
               addstr("pilfered files from a Corporate server.");

               itemst *it=new itemst;
                  it->type=ITEM_LOOT;
                  it->loottype=LOOT_CORPFILES;
               location[hack[0]->location]->loot.push_back(it);

               trackdif=20;
               crime=LAWFLAG_INFORMATION;
               juiceval=10;
               break;
            }
            case 1: // *JDS* Penetrated government networks; don't get any loot, but do scare the info community
               addstr("caused a scare by breaking into a CIA network.");

               trackdif=30;
               crime=LAWFLAG_INFORMATION;
               juiceval=20;
               change_public_opinion(VIEW_INTELLIGENCE,3,0,75);
               break;
         }

         if(trackdif>LCSrandom(hskill+1)+LCSrandom(10))
         {
            for(int h=0;h<hack.size();h++)
            {
               hack[h]->lawflag[crime]++;
            }
         }

         for(int h=0;h<hack.size();h++)
         {
            addjuice(*hack[h],juiceval,20);
         }

         refresh();
         getch();
      }
      //MINOR HACKING EVENT
      else if(LCSrandom(1000)<=hskill*10)
      {
         if(clearformess)erase();
         else
         {
            makedelimiter(8,0);
         }

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(8,1);
         if(hack.size()>1)addstr("Your Hackers have ");
         else {addstr(hack[0]->name);addstr(" has ");}

         int trackdif=0;
         unsigned short crime=0;

         long juiceval=0;

         switch(LCSrandom(3))
         {
            case 0: // DDoS on corporate site
               addstr("launched a denial of service attack on a Corporate site.");

               trackdif=3;
               crime=LAWFLAG_COMMERCE;
               juiceval=2;
               break;
            case 1: // Credit Card fraud
            {
               addstr("stolen some credit card numbers, netting $");
               char num[20];
               // *JDS* You get between $1 and $500, plus an extra $1-250 every
               // time you pass a check against your hacking skill, where chance of
               // failure is one over the adjusted hackers' skill divided by four. Once
               // a check fails, no more money is gained. This check will be made
               // up to 20 times, at which point the loop breaks. The skill check
               // here doesn't take into account funding.
               long fundgain=LCSrandom(501);
               for(int i=0;i<20 && LCSrandom(hskill/4);i++)
               {
                  fundgain+=LCSrandom(251);
               }
               funds+=fundgain;
               stat_funds+=fundgain;
               moneygained_ccfraud+=fundgain;
               itoa(fundgain,num,10);
               addstr(num);
               addstr(".");

               trackdif=5;
               crime=LAWFLAG_CCFRAUD;
               break;
            }
            case 2: // *JDS* Website defacement... very small effect, but can hit any issue but media and LCS views
            {
               int issue=LCSrandom(VIEWNUM-4);
               addstr("defaced a prominent Conservative website.");
               // Maybe do a switch on issue here to specify which website it was, but I don't feel like
               // doing that right now
               juiceval=2;
               trackdif=3;
               crime=LAWFLAG_INFORMATION;
               change_public_opinion(issue,1,0,75);
               break;
            }
         }

         if(trackdif>LCSrandom(hskill+1)+LCSrandom(10))
         {
            for(int h=0;h<hack.size();h++)
            {
               hack[h]->lawflag[crime]++;
            }
         }

         for(int h=0;h<hack.size();h++)
         {
            addjuice(*hack[h],juiceval,20);
         }

         refresh();
         getch();
      }
   }

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
                                                                        20*prostitutes[p]->skill[SKILL_SEDUCTION];
      else fundgain=LCSrandom(21)+20;
      itoa(fundgain,num,10);
      addstr(num);
      addstr("!");

      addjuice(*prostitutes[p],-!LCSrandom(3));
      prostitutes[p]->skill_ip[SKILL_SEDUCTION]+=max(10-prostitutes[p]->skill[SKILL_SEDUCTION],0);

      refresh();
      getch();


      if(!LCSrandom((prostitutes[p]->skill[SKILL_SEDUCTION]+
                     prostitutes[p]->skill[SKILL_STREETSENSE]+
                     prostitutes[p]->skill[SKILL_GANGSTERISM]+
                     prostitutes[p]->attval(ATTRIBUTE_INTELLIGENCE))*3))
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
         prostitutes[p]->lawflag[LAWFLAG_PROSTITUTION]++;
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
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
               change_public_opinion(VIEW_ANIMALRESEARCH,mod,0,75);
               done=1;
               break;
            case 1:
            {
               if(law[LAW_GAY]<2)
               {
                  addstr("disrupted a traditional wedding at a church!");
                  change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
                  change_public_opinion(VIEW_GAY,mod,0,75);
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
                  change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
                  change_public_opinion(VIEW_ABORTION,mod,0,75);
                  juiceval=1;
                  done=1;
               }
               break;
            }
            case 3:
            {
               addstr("distributed fliers graphically illustrating prison life!");
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
               change_public_opinion(VIEW_PRISONS,mod,0,75);
               juiceval=1;
               done=1;
               break;
            }
            case 4:
            {
               if(law[LAW_POLICEBEHAVIOR]<2)
               {
                  addstr("gone downtown and reenacted a police beating!");
                  change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
                  change_public_opinion(VIEW_POLICEBEHAVIOR,mod,0,75);
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
                  change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
                  change_public_opinion(VIEW_NUCLEARPOWER,mod,0,75);
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
                  change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
                  change_public_opinion(VIEW_POLLUTION,mod,0,75);
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
                  change_public_opinion(VIEW_LIBERALCRIMESQUAD,mod,0);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,mod,0,70);
                  change_public_opinion(VIEW_DEATHPENALTY,mod,0,75);
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
            dodgelawroll=LCSrandom(trouble[t]->skill[SKILL_PERSUASION]+
               trouble[t]->skill[SKILL_DISGUISE]+
               trouble[t]->skill[SKILL_STREETSENSE]+
               trouble[t]->attval(ATTRIBUTE_CHARISMA)+
               trouble[t]->attval(ATTRIBUTE_AGILITY)+
               trouble[t]->attval(ATTRIBUTE_INTELLIGENCE)+1);

            if(dodgelawroll==0)
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
                  trouble[t]->lawflag[crime]++;
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
            bury[b]->lawflag[LAWFLAG_BURIAL]++;
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

            cr.skill_ip[SKILL_SECURITY]+=7;

            if(LCSrandom(11)<attack)
            {
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

            cr.skill_ip[SKILL_SECURITY]+=7;

            if(LCSrandom(11)<attack)
            {
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
                  case 1:addstr("under the front seat");break;
                  case 2:addstr("under the back seat");break;
                  case 3:addstr("above the pull-down sunblock thingy");break;
                  case 4:addstr("in the glove compartment.");break;
               }
               addstr("!");
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
      if(interface_pgup=='[')
      {
         addstr("[] to view other Liberal pages.");
      }
         else if(interface_pgup=='.')
         {
            addstr("; and : to view other Liberal pages.");
         }
      else
      {
         addstr("PGUP/PGDN to view other Liberal pages.");
      }

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if(c==interface_pgup&&page>0)page--;
      //PAGE DOWN
      if(c==interface_pgdn&&(page+1)*19<cart.size())page++;

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
