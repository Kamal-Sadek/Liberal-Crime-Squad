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

#define DATERESULT_MEETTOMORROW 0
#define DATERESULT_BREAKUP      1
#define DATERESULT_JOINED       2
#define DATERESULT_ARRESTED     3


// Handles the result of a date or vacation
static int dateresult(int aroll,int troll,datest &d,int e,int p,int y)
{
   if(aroll>troll)
   {
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      move(y,0);y++;
      addstr(d.date[e]->name, gamelog);
      addstr(" is quite taken with ", gamelog);
      addstr(pool[p]->name, gamelog);
      addstr("'s unique life philosophy...", gamelog);
      gamelog.newline();
      refresh();
      getch();

      if(signed(LCSrandom(d.date[e]->get_attribute(ATTRIBUTE_HEART,false)+(aroll-troll)/2))>d.date[e]->get_attribute(ATTRIBUTE_WISDOM,false))
      {
         if(loveslavesleft(*pool[p]) <= 0)
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            
            move(y++,0);
            addstr("But when ", gamelog);
            addstr(pool[p]->name, gamelog);
            addstr(" mentions having other lovers, things go downhill fast.", gamelog);
            gamelog.newline();

            refresh();
            getch();

            move(y++,0);
            addstr("This relationship is over.", gamelog);
            gamelog.nextMessage();

            refresh();
            getch();

            delete d.date[e];
            d.date.erase(d.date.begin() + e);

            return DATERESULT_BREAKUP;
         }
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(y,0);y++;
         addstr("In fact, ", gamelog);
         addstr(d.date[e]->name, gamelog);
         addstr(" is ", gamelog);
         addstr(pool[p]->name, gamelog);
         addstr("'s totally unconditional love-slave!", gamelog);
         gamelog.nextMessage();
         //Get map of their workplace
         location[d.date[e]->worklocation]->mapped=1;
         location[d.date[e]->worklocation]->hidden=0;
         refresh();
         getch();

         d.date[e]->flag|=CREATUREFLAG_LOVESLAVE;
         d.date[e]->hireid=pool[p]->id;

         erase();

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(0,0);
         addstr("The Self-Nullifying Infatuation of ");
         addstr(d.date[e]->propername);

         move(2,0);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr("What name will you use for this ");
         char str[80];
         getrecruitcreature(str,d.date[e]->type);
         addstr(str);
         addstr(" in its presence?");
         move(3,0);
         addstr("If you do not enter anything, their real name will be used.");
         
         move(4,0);
         enter_name(d.date[e]->name,CREATURE_NAMELEN,d.date[e]->propername);

         sleeperize_prompt(*d.date[e],*pool[p],8);
         
         pool.push_back(d.date[e]);
         stat_recruits++;
         d.date.erase(d.date.begin() + e);

         return DATERESULT_JOINED;
      }
      else
      {
         if(d.date[e]->get_attribute(ATTRIBUTE_HEART,false)<pool[p]->get_attribute(ATTRIBUTE_HEART,false)-4)
         {
            d.date[e]->adjust_attribute(ATTRIBUTE_HEART,+1);
         }
         else
         {
            //Posibly date reveals map of location
            if(location[d.date[e]->worklocation]->mapped==0 && !LCSrandom(d.date[e]->get_attribute(ATTRIBUTE_WISDOM,false)))
            {
               y++;
               move(y++,0);
               addstr(d.date[e]->name, gamelog);
               addstr(" turns the topic of discussion to the ", gamelog);
               addstr(location[d.date[e]->worklocation]->name, gamelog);
               addstr(".", gamelog);
               gamelog.newline();
               move(y++,0);
               if(!(location[d.date[e]->worklocation]->type<=SITE_RESIDENTIAL_SHELTER))
               {
                  addstr(pool[p]->name, gamelog);
                  addstr(" was able to create a map of the site with this information.", gamelog);
                  gamelog.newline();
                  y++;                           
               }
               else
               {
                  addstr(pool[p]->name, gamelog);
                  addstr(" knows all about that already.", gamelog);
                  gamelog.newline();
                  y++;
               }
               location[d.date[e]->worklocation]->mapped=1;
               location[d.date[e]->worklocation]->hidden=0;
            }
         }

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);y++;
         addstr("They'll meet again tomorrow.", gamelog);
         gamelog.nextMessage();
         refresh();
         getch();
         
         return DATERESULT_MEETTOMORROW;
      }
   }
   else
   {
      //WISDOM POSSIBLE INCREASE
      if(d.date[e]->align==-1&&aroll<troll/2)
      {
                        set_color(COLOR_RED,COLOR_BLACK,1);
         move(y,0);y++;
         
         addstr("Talking with ", gamelog);
         addstr(d.date[e]->name, gamelog);
         addstr(" actually curses ", gamelog);
         addstr(pool[p]->name, gamelog);
         addstr("'s mind with wisdom!!!", gamelog);
         gamelog.newline();
         pool[p]->adjust_attribute(ATTRIBUTE_WISDOM,+1);

         if(d.date[e]->get_skill(SKILL_RELIGION)>pool[p]->get_skill(SKILL_RELIGION))
         {
            pool[p]->train(SKILL_RELIGION,20*(d.date[e]->get_skill(SKILL_RELIGION)-pool[p]->get_skill(SKILL_RELIGION)));
         }
         if(d.date[e]->get_skill(SKILL_SCIENCE)>pool[p]->get_skill(SKILL_SCIENCE))
         {
            pool[p]->train(SKILL_SCIENCE,20*(d.date[e]->get_skill(SKILL_SCIENCE)-pool[p]->get_skill(SKILL_SCIENCE)));
         }
         if(d.date[e]->get_skill(SKILL_BUSINESS)>pool[p]->get_skill(SKILL_BUSINESS))
         {
            pool[p]->train(SKILL_BUSINESS,20*(d.date[e]->get_skill(SKILL_BUSINESS)-pool[p]->get_skill(SKILL_BUSINESS)));
         }

         refresh();
         getch();
      }

      //BREAK UP
      
      // If your squad member is wanted by the police, a conservative who breaks up with
      // them has a 1 in 50 chance of ratting them out, unless the person being dated is law
      // enforcement, prison guard, or agent, in which case there is a 1 in 4 chance. -Fox
      if((iscriminal(*pool[p])) &&
         (!LCSrandom(50) ||(LCSrandom(2) && (d.date[e]->kidnap_resistant()))))
      {
         move(y,0);y++;
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr(d.date[e]->name, gamelog);
         addstr(" was leaking information to the police the whole time!", gamelog);
         refresh();
         getch();
         move(y,0);y++;
         // 3/4 chance of being arrested if less than 50 juice,
         // 1/2 chance of being arrested if more than 50 juice
         if((pool[p]->juice<50 && LCSrandom(2)) || LCSrandom(2))
         {
            // Find the police station
            long ps=find_police_station(*pool[p]);

            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            addstr(pool[p]->name, gamelog);
            addstr(" has been arrested.", gamelog);
            gamelog.nextMessage();
            
            removesquadinfo(*pool[p]);
            pool[p]->carid=-1;
            pool[p]->location=ps;
            pool[p]->drop_weapons_and_clips(NULL);
            pool[p]->activity.type=ACTIVITY_NONE;

            refresh();
            getch();

            delete d.date[e];
            d.date.erase(d.date.begin() + e);

            return DATERESULT_ARRESTED;
         }
         else
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            addstr("But ", gamelog);
            addstr(pool[p]->name, gamelog);
            addstr(" escapes the police ambush!", gamelog);
            gamelog.nextMessage();
         }
      }
      else
      {
         set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         move(y,0);y++;
         addstr(d.date[e]->name, gamelog);
         addstr(" can sense that things just aren't working out.", gamelog);
         gamelog.newline();
         move(y,0);y++;
         
         addstr("This relationship is over.", gamelog);
         gamelog.nextMessage();
      }
      refresh();
      getch();

      delete d.date[e];
      d.date.erase(d.date.begin() + e);

      return DATERESULT_BREAKUP;
   }
}

/* daily - date - dater p gets back from vacation */
char completevacation(datest &d,int p,char &clearformess)
{
   int e=0;

   clearformess=1;

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   addstr(pool[p]->name, gamelog);
   addstr(" is back from vacation.", gamelog);
   gamelog.nextMessage();

   short aroll=pool[p]->skill_roll(SKILL_SEDUCTION)*2;
   short troll=d.date[e]->attribute_roll(ATTRIBUTE_WISDOM);
   pool[p]->train(SKILL_SEDUCTION,LCSrandom(11)+15);
   
   pool[p]->train(SKILL_SCIENCE,
      max(d.date[e]->get_skill(SKILL_SCIENCE)-pool[p]->get_skill(SKILL_SCIENCE),0));
   pool[p]->train(SKILL_RELIGION,
      max(d.date[e]->get_skill(SKILL_RELIGION)-pool[p]->get_skill(SKILL_RELIGION),0));
   pool[p]->train(SKILL_BUSINESS,
      max(d.date[e]->get_skill(SKILL_BUSINESS)-pool[p]->get_skill(SKILL_BUSINESS),0));

   if(d.date[e]->skill_roll(SKILL_BUSINESS))
   {
      troll+=d.date[e]->skill_roll(SKILL_BUSINESS);
      aroll+=pool[p]->skill_roll(SKILL_BUSINESS);
   }
   if(d.date[e]->skill_roll(SKILL_RELIGION))
   {
      troll+=d.date[e]->skill_roll(SKILL_RELIGION);
      aroll+=pool[p]->skill_roll(SKILL_RELIGION);
   }
   if(d.date[e]->skill_roll(SKILL_SCIENCE))
   {
      troll+=d.date[e]->skill_roll(SKILL_SCIENCE);
      aroll+=pool[p]->skill_roll(SKILL_SCIENCE);
   }


   int y=2;
   switch(dateresult(aroll,troll,d,e,p,y))
   {
   default:
   case DATERESULT_MEETTOMORROW:return 0;
   case DATERESULT_BREAKUP:     return 1;
   case DATERESULT_JOINED:      return 1;
   case DATERESULT_ARRESTED:    return 1;
   }
}



/* daily - date - dater p goes on some dates */
char completedate(datest &d,int p,char &clearformess)
{
   int e;
   clearformess=1;

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   addstr(pool[p]->name, gamelog);
   addstr(" has ", gamelog);
   if(d.date.size()==1)
   {
      if(pool[p]->clinic)addstr("a \"hot\" date with ", gamelog);
      else addstr("a hot date with ", gamelog);
   }
   else addstr("dates to manage with ", gamelog);
   for(e=0;e<d.date.size();e++)
   {
      addstr(d.date[e]->name, gamelog);

      if(e<=d.date.size()-3)addstr(", ", gamelog);
      else if(e==d.date.size()-2)addstr(" and ", gamelog);
      else
      {
         if(pool[p]->clinic>0)
         {
            addstr(" at ", gamelog);
            addstr(location[pool[p]->location]->name, gamelog);
         }
         addstr(".", gamelog);
      }
   }

   gamelog.newline();

   refresh();
   getch();

   char datestr[128];
   if(d.date.size()>1&&
	   !LCSrandom( (d.date.size() > 2) ? 4 : 6))
   {
      switch (LCSrandom(3))
      {
         case 0:
            move(2,0);
            if (d.date.size()>2)
            {
               addstr("Unfortunately, they all know each other and had been discussing", gamelog);
            }
            else
            {
               addstr("Unfortunately, they know each other and had been discussing", gamelog);
            }
            move(3,0);
            addstr(pool[p]->name, gamelog);
            addstr(".  An ambush was set for the lying dog...", gamelog);
            gamelog.newline();
            refresh();
            getch();
            break;
         case 1:
            move(2,0);
            if (d.date.size()>2)
            {
               addstr("Unfortunately, they all turn up at the same time.", gamelog);
            }
            else
            {
               addstr("Unfortunately, they turn up at the same time.", gamelog);
            }
            gamelog.newline();
            move(3,0);
            addstr("Uh oh...", gamelog);
            gamelog.newline();
            refresh();
            getch();
            break;
         default:
            move(2,0);
            addstr(pool[p]->name, gamelog);
            if (d.date.size()>2)
            {
               snprintf(datestr, sizeof(char) * 128, " realizes %s has commited to eating %d meals at once.", pool[p]->heshe(), d.date.size());
               addstr(datestr, gamelog);
            }
            else
            {
               addstr(" mixes up the names of ", gamelog);
               addstr(d.date[0]->name, gamelog);
               addstr(" and ", gamelog);
               addstr(d.date[1]->name, gamelog);
               gamelog.newline();
            }
            move(3,0);
            addstr("Things go downhill fast.", gamelog);
            gamelog.newline();
            refresh();
            getch();
            break;
      }

      move(5,0);
      addstr(pool[p]->name, gamelog);
      switch (LCSrandom(3))
      {
         case 0:
            addstr(" is publicly humiliated.", gamelog);
            break;
         case 1:
            addstr(" runs away.", gamelog);
            break;
         case 2:
         default:
            addstr(" escapes through the bathroom window.", gamelog);
            break;
      }
      addjuice(*pool[p],-5,-50);
      refresh();
      getch();
      gamelog.nextMessage();

      return 1;
   }

   for(e=d.date.size()-1;e>=0;e--)
   {
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Seeing ");
      addstr(d.date[e]->name, gamelog);
      addstr(", ", gamelog);
      char str[75];
      getrecruitcreature(str,d.date[e]->type);
      addstr(str, gamelog);
      addstr(", ", gamelog);
      addstr(location[d.date[e]->worklocation]->name, gamelog);
      gamelog.newline();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      printfunds(0,1,"Money: ");

      //Others come to dates unarmed and wearing normal
      //clothing
      vector<Item*> temp;
      d.date[e]->drop_weapons_and_clips(&temp);
      Armor atmp(*armortype[getarmortype("ARMOR_CLOTHES")]);
      d.date[e]->give_armor(atmp,&temp);
      //Weapon* wtemp = &d.date[e]->get_weapon();
      //d.date[e]->weapon = NULL;
      //Armor* atemp = d.date[e]->armor;
      //d.date[e]->armor = new Armor(*armortype[getarmortype("ARMOR_CLOTHES")]);

      printcreatureinfo(d.date[e]);
      makedelimiter(8,0);

      while (!temp.empty())
      {
         if (temp.back()->is_weapon())
            d.date[e]->give_weapon(*(static_cast<Weapon*>(temp.back())),NULL); //casts -XML
         else if (temp.back()->is_armor())
            d.date[e]->give_armor(*(static_cast<Armor*>(temp.back())),NULL);
         else if (temp.back()->is_clip())
            d.date[e]->take_clips(*(static_cast<Clip*>(temp.back())),temp.back()->get_number());
         delete temp.back();
         temp.pop_back();
      }
      //d.date[e]->weapon = wtemp;
      //delete d.date[e]->armor;
      //d.date[e]->armor = atemp;


      move(10,0);
      addstr("How should ");
      addstr(pool[p]->name);
      addstr(" approach the situation?");

      if(ledger.get_funds()>=100&&!pool[p]->clinic)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(11,0);
      addstr("A - Spend a hundred bucks tonight to get the ball rolling.");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(12,0);
      addstr("B - Try to get through the evening without spending a penny.");
      if(!pool[p]->clinic&&pool[p]->blood==100)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(13,0);
      if(pool[p]->blood==100) addstr("C - Spend a week on a cheap vacation (stands up any other dates).");
      else addstr("C - Spend a week on a cheap vacation (must be uninjured).");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(14,0);
      addstr("D - Break it off.");
      if(d.date[e]->align==-1&&!pool[p]->clinic)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(15,0);
         addstr("E - Just kidnap the Conservative bitch.");
      }

      int thingsincommon = 0;
      for(int s=0;s<SKILLNUM;s++)
      {
         if(d.date[e]->get_skill(s)>=1 && pool[p]->get_skill(s)>=1)
         {
            //Has a skill that is between double and half the same skill of the other person on the date.
            if (d.date[e]->get_skill(s)<=pool[p]->get_skill(s)*2 && d.date[e]->get_skill(s)*2>=pool[p]->get_skill(s))
            {
               thingsincommon++;
            }
         }
      }
      do
      {
         refresh();
         int c=getch();
         translategetch(c);

         short aroll=pool[p]->skill_roll(SKILL_SEDUCTION);
         short troll=d.date[e]->attribute_roll(ATTRIBUTE_WISDOM);
         if(d.date[e]->align==ALIGN_CONSERVATIVE)
            troll+=troll*(d.date[e]->juice/100);

         char test=0;
		   aroll += thingsincommon * 3;
         if(c=='a'&&ledger.get_funds()>=100&&!pool[p]->clinic)
         {
            ledger.subtract_funds(100,EXPENSE_DATING);
            aroll+=LCSrandom(10);
            test=true;
         }
         else if(c=='b')
         {
            test=true;
         }

         if(test)
         {
            pool[p]->train(SKILL_SEDUCTION,LCSrandom(4)+5);
            pool[p]->train(SKILL_SCIENCE,
               max(d.date[e]->get_skill(SKILL_SCIENCE)-pool[p]->get_skill(SKILL_SCIENCE),0));
            pool[p]->train(SKILL_RELIGION,
               max(d.date[e]->get_skill(SKILL_RELIGION)-pool[p]->get_skill(SKILL_RELIGION),0));
            pool[p]->train(SKILL_BUSINESS,
               max(d.date[e]->get_skill(SKILL_BUSINESS)-pool[p]->get_skill(SKILL_BUSINESS),0));

            if(d.date[e]->get_skill(SKILL_BUSINESS))
            {
               troll+=d.date[e]->skill_roll(SKILL_BUSINESS);
               aroll+=pool[p]->skill_roll(SKILL_BUSINESS);
            }
            if(d.date[e]->get_skill(SKILL_RELIGION))
            {
               troll+=d.date[e]->skill_roll(SKILL_RELIGION);
               aroll+=pool[p]->skill_roll(SKILL_RELIGION);
            }
            if(d.date[e]->get_skill(SKILL_SCIENCE))
            {
               troll+=d.date[e]->skill_roll(SKILL_SCIENCE);
               aroll+=pool[p]->skill_roll(SKILL_SCIENCE);
            }

            int y=17;
            int result = dateresult(aroll,troll,d,e,p,y);
            if(result == DATERESULT_ARRESTED)return 1;
            break;
         }

         if(c=='c'&&!pool[p]->clinic&&pool[p]->blood==100)
         {
            for(int e2=d.date.size()-1;e2>=0;e2--)
            {
               if(e2==e)continue;
               delete d.date[e2];
               d.date.erase(d.date.begin() + e2);
               e=0;
            }
            d.timeleft=7;
            pool[p]->train(SKILL_SEDUCTION,LCSrandom(40)+15);
            pool[p]->train(SKILL_SCIENCE,
               max((d.date[e]->get_skill(SKILL_SCIENCE)-pool[p]->get_skill(SKILL_SCIENCE))*4,0));
            pool[p]->train(SKILL_RELIGION,
               max((d.date[e]->get_skill(SKILL_RELIGION)-pool[p]->get_skill(SKILL_RELIGION))*4,0));
            pool[p]->train(SKILL_BUSINESS,
               max((d.date[e]->get_skill(SKILL_BUSINESS)-pool[p]->get_skill(SKILL_BUSINESS))*4,0));
            return 0;
         }
         if(c=='d')
         {
            delete d.date[e];
            d.date.erase(d.date.begin() + e);
            break;
         }
         if(c=='e'&&d.date[e]->align==-1&&!pool[p]->clinic)
         {
            
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            int bonus=0;
            move(17,0);
            addstr(pool[p]->name, gamelog);

            if(pool[p]->get_weapon().is_ranged())
            {
               addstr(" comes back from the bathroom toting the ", gamelog);
               addstr(pool[p]->get_weapon().get_name(1).c_str(), gamelog);
               move(18,0);
               addstr("and threatens to blow the Conservative's brains out!", gamelog);
               gamelog.newline();
               bonus=5;
            }
            else if(pool[p]->is_armed())
            {
               addstr(" grabs the Conservative from behind, holding the ", gamelog);
               addstr(pool[p]->get_weapon().get_name(1).c_str(), gamelog);
               move(18,0);
               addstr("to the corporate slave's throat!", gamelog);
               gamelog.newline();
               if(pool[p]->get_weapon().can_take_hostages())
                  bonus=5;
               else
                  bonus=-1; // Conservative emboldened by the fact that you're trying
                           // to kidnap them with a gavel or some shit like that
            }
            else
            {
               addstr(" seizes the Conservative swine from behind and warns it", gamelog);
               move(18,0);
               if(law[LAW_FREESPEECH]!=-2)
                  addstr("not to fuck around!", gamelog);
               else
                  addstr("not to [resist]!", gamelog);
               gamelog.newline();

               bonus+=pool[p]->get_skill(SKILL_HANDTOHAND)-1;
            }

            refresh();
            getch();


            // Kidnap succeeds if the conservative isn't very dangerous,
            // but might fail if the conservative is tough stuff.
            if((d.date[e]->kidnap_resistant()&&
                LCSrandom(15))||
                LCSrandom(2+bonus))
            {
               
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(20,0);
               addstr(d.date[e]->name, gamelog);
               if(bonus)
               {
                  addstr(" doesn't resist.", gamelog);
                  gamelog.newline();
               }
               else
               {
                  addstr(" struggles and yells for help, but nobody comes.", gamelog);
                  gamelog.newline();
               }

               refresh();
               getch();
               move(22,0);
               addstr(pool[p]->name, gamelog);
               addstr(" kidnaps the Conservative!", gamelog);
               gamelog.nextMessage();
               refresh();
               getch();

               d.date[e]->namecreature();
               strcpy(d.date[e]->propername,d.date[e]->name);

               d.date[e]->location=pool[p]->location;
               d.date[e]->base=pool[p]->base;
               d.date[e]->flag|=CREATUREFLAG_MISSING;

               //Kidnapped wearing normal clothes and no weapon
               d.date[e]->drop_weapons_and_clips(NULL);
               Armor clothes(*armortype[getarmortype("ARMOR_CLOTHES")]);
               d.date[e]->give_armor(clothes,NULL);

               //Create interrogation data
               d.date[e]->activity.arg=reinterpret_cast<long>(new interrogation);

               erase();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(0,0);
               addstr("The Education of ");
               addstr(d.date[e]->propername);

               move(2,0);
               set_color(COLOR_WHITE,COLOR_BLACK,0);
               addstr("What name will you use for this ");
               char str[80];
               getrecruitcreature(str,d.date[e]->type);
               addstr(str);
               addstr(" in its presence?");

               move(3,0);
               addstr("If you do not enter anything, their real name will be used.");

               move(4,0);
               enter_name(d.date[e]->name,CREATURE_NAMELEN,d.date[e]->propername);

               pool.push_back(d.date[e]);
               stat_kidnappings++;
               d.date.erase(d.date.begin() + e);
               break;
            }
            else
            {
               int y=20;
               if(LCSrandom(2))
               {
                  set_color(COLOR_MAGENTA,COLOR_BLACK,1);
                  move(y,0);y++;
                  addstr(d.date[e]->name, gamelog);
                  addstr(" manages to get away on the way back to the safehouse!", gamelog);
                  gamelog.newline();
                  refresh();
                  getch();
                  move(++y,0);y++;
                  addstr(pool[p]->name, gamelog);
                  addstr(" has failed to kidnap the Conservative.", gamelog);
                  gamelog.nextMessage();

                  // Charge with kidnapping
                  criminalize(*pool[p],LAWFLAG_KIDNAPPING);

                  refresh();
                  getch();
                  
                  delete d.date[e];
                  d.date.erase(d.date.begin() + e);
                  break;
               }
               else
               {
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  move(y,0);y++;
                  addstr(d.date[e]->name, gamelog);
                  addstr("'s fist is the last thing ", gamelog);
                  addstr(pool[p]->name, gamelog);
                  addstr(" remembers seeing!", gamelog);
                  gamelog.newline();
                  refresh();
                  getch();
                  move(++y,0);y++;
                  addstr("The Liberal wakes up in the police station...", gamelog);
                  gamelog.nextMessage();

                  // Find the police station
                  int ps=find_police_station(*pool[p]);
                     
                  // Arrest the Liberal
                  removesquadinfo(*pool[p]);
                  pool[p]->carid=-1;
                  pool[p]->location=ps;
                  pool[p]->drop_weapons_and_clips(NULL);
                  pool[p]->activity.type=ACTIVITY_NONE;

                  // Charge with kidnapping
                  criminalize(*pool[p],LAWFLAG_KIDNAPPING);

                  refresh();
                  getch();
                  
                  delete d.date[e];
                  d.date.erase(d.date.begin() + e);
                  return 1;
               }
            }
         }
      }while(1);
   }

   if(d.date.size()>0)
   {
      d.timeleft=0;
      return 0;
   }
   else return 1;
}
