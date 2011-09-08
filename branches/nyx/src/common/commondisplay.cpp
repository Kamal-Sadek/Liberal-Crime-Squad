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

// Sets the text color to the thematic color for the given alignment
// extended_range forces colors to be set on a 5 point scale instead
// of just basic liberal-moderate-conservative
void set_alignment_color(signed char alignment, bool extended_range)
{
   switch(alignment)
   {
   case ALIGN_ARCHCONSERVATIVE:
      set_color(COLOR_RED,COLOR_BLACK,1);
      break;
   case ALIGN_CONSERVATIVE:
      if(extended_range)
         set_color(COLOR_MAGENTA,COLOR_BLACK,1);
      else
         set_color(COLOR_RED,COLOR_BLACK,1);
      break;
   case ALIGN_MODERATE:
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      break;
   case ALIGN_LIBERAL:
      if(extended_range)
         set_color(COLOR_BLUE,COLOR_BLACK,1);
      else
         set_color(COLOR_GREEN,COLOR_BLACK,1);
      break;
   case ALIGN_ELITELIBERAL:
      set_color(COLOR_GREEN,COLOR_BLACK,1);
      break;
   }
}


// Sets the text color per activity type
void set_activity_color(long activity_type)
{
   switch(activity_type)
   {
      // Liberal actvism
   case ACTIVITY_SLEEPER_LIBERAL:
   case ACTIVITY_TROUBLE:
   case ACTIVITY_GRAFFITI:
   case ACTIVITY_DOS_ATTACKS:
   case ACTIVITY_HACKING:
   case ACTIVITY_WRITE_LETTERS:
   case ACTIVITY_WRITE_GUARDIAN:
      set_color(COLOR_GREEN,COLOR_BLACK,1);
      break;

      // Less exciting liberal activities
   case ACTIVITY_SLEEPER_SPY:
   case ACTIVITY_COMMUNITYSERVICE:
   case ACTIVITY_POLLS:
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      break;

      // Stealing things
   case ACTIVITY_SLEEPER_STEAL:
   case ACTIVITY_WHEELCHAIR:
   case ACTIVITY_STEALCARS:
      set_color(COLOR_CYAN,COLOR_BLACK,0);
      break;

      // Illegal fundraising
   case ACTIVITY_SLEEPER_EMBEZZLE:
   case ACTIVITY_SELL_DRUGS:
   case ACTIVITY_PROSTITUTION:
   case ACTIVITY_CCFRAUD:
   case ACTIVITY_PETTYTHEFT:
   case ACTIVITY_DOS_RACKET:
      set_color(COLOR_RED,COLOR_BLACK,1);
      break;

      // Legal fundraising
   case ACTIVITY_DONATIONS:
   case ACTIVITY_SELL_TSHIRTS:
   case ACTIVITY_SELL_ART:
   case ACTIVITY_SELL_MUSIC:
      set_color(COLOR_CYAN,COLOR_BLACK,1);
      break;

      // Clothing/garment stuff
   case ACTIVITY_REPAIR_ARMOR:
   case ACTIVITY_MAKE_ARMOR:
      set_color(COLOR_CYAN,COLOR_BLACK,0);
      break;

      // Teaching
   case ACTIVITY_TEACH_POLITICS:
   case ACTIVITY_TEACH_FIGHTING:
   case ACTIVITY_TEACH_COVERT:
   //and studying
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
      set_color(COLOR_MAGENTA,COLOR_BLACK,1);
      break;

      // Interrogating
   case ACTIVITY_HOSTAGETENDING:
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      break;

      // Dealing with your injuries
   case ACTIVITY_CLINIC:
      set_color(COLOR_RED,COLOR_BLACK,0);
      break;

      // Doing something Conservative
   case ACTIVITY_SLEEPER_CONSERVATIVE:
      set_color(COLOR_RED,COLOR_BLACK,1);
      break;

      // Dealing with the dead
   case ACTIVITY_BURY:
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      break;

      // Nothing terribly important
   case ACTIVITY_HEAL: // Identical to none in practice
   case ACTIVITY_NONE:
   case ACTIVITY_VISIT: // Shouldn't show on activate screens at all
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      break;
   }
}

/* location and squad header */
void locheader(void)
{
   char num[20];

   if(activesquad!=NULL && activesquad->squad[0]->location!=-1)
   {
      if(location[activesquad->squad[0]->location]->siege.siege)
      {
         if(location[activesquad->squad[0]->location]->siege.underattack)set_color(COLOR_RED,COLOR_BLACK,1);
         else set_color(COLOR_YELLOW,COLOR_BLACK,1);
      }
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
   }
   else if(selectedsiege!=-1)
   {
      if(location[selectedsiege]->siege.siege)
      {
         if(location[selectedsiege]->siege.underattack)set_color(COLOR_RED,COLOR_BLACK,1);
         else set_color(COLOR_YELLOW,COLOR_BLACK,1);
      }
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
   }
   else set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(0,0);

   if(activesquad!=NULL && activesquad->squad[0]->location!=-1)
   {
      addlocationname(location[activesquad->squad[0]->location]);
      addstr(", ");
   }
   else
   {
      if(selectedsiege==-1)
      {
         addstr("No Squad Selected");
         addstr(", ");
      }
      else
      {
         addlocationname(location[selectedsiege]);
         addstr(", ");
      }
   }

   switch(month)
   {
   case 1:addstr("Jan");break;
   case 2:addstr("Feb");break;
   case 3:addstr("Mar");break;
   case 4:addstr("Apr");break;
   case 5:addstr("May");break;
   case 6:addstr("Jun");break;
   case 7:addstr("Jul");break;
   case 8:addstr("Aug");break;
   case 9:addstr("Sep");break;
   case 10:addstr("Oct");break;
   case 11:addstr("Nov");break;
   case 12:addstr("Dec");break;
   }
   addstr(". ");
   itoa(day,num,10);
   addstr(num);
   addstr(", ");
   itoa(year,num,10);
   addstr(num);

   if(activesquad==NULL && selectedsiege==-1)
   {
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(3,6);
      addstr("To form a new squad:");
      move(4,6);
      addstr("1) R - Review Assets and Form Squads");
      move(5,6);
      addstr("2) Press Z to Assemble a New Squad");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
   }


   printfunds(0,1,"Money: ");

   if(activesquad!=NULL)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      char str[80];
      getactivity(str,activesquad->activity);

      if(activesquad->activity.type==ACTIVITY_NONE)
      {
         int count=0;char haveact=0;
         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]==NULL)continue;
            count++;
            if(activesquad->squad[p]->activity.type!=ACTIVITY_NONE)
            {
               getactivity(str,activesquad->squad[p]->activity);
               haveact=1;
            }
         }
         if(haveact&&count>1)strcpy(str,"Acting Individually");
      }

      move(0,46);
      addstr(str);
   }
}

/* party info at top of screen */
void printparty(void)
{
   Creature *party[6]={NULL,NULL,NULL,NULL,NULL,NULL};
   if(activesquad!=NULL)
   {
      for(int p=0;p<6;p++)party[p]=activesquad->squad[p];
   }

   set_color(COLOR_WHITE,COLOR_BLACK,0);

   for(int i=2;i<8;i++)
   {
      move(i,0);
      addstr("                                                                                ");
   }

   if(party_status!=-1&&party[party_status]==NULL)party_status=-1;

   if(party_status!=-1)
   {
      printcreatureinfo(party[party_status]);

      char num[20];
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      itoa(party_status+1,num,10);
      move(1,0);
      addstr(num);
   }
   else
   {
      char str[200];
      char num[20];

      move(1,0);
      addstr("#-CODE NAME------------SKILL---WEAPON---------ARMOR----------HEALTH---TRANSPORT-");

      for(int p=0;p<6;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(p+2,0);
         addstr("                                                                                          ");
         move(p+2,0);
         addch('1'+p);

         if(party[p]!=NULL)
         {
            if(party[p]->prisoner!=NULL)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(p+2,2);
            addstr(party[p]->name);
            if(party[p]->prisoner!=NULL)addstr("+H");

            int skill=0;
            char bright=0;
            for(int sk=0;sk<SKILLNUM;sk++)
            {
               skill+=(int)party[p]->get_skill(sk);
               if(party[p]->get_skill_ip(sk)>=100+(10*party[p]->get_skill(sk))&&
                  party[p]->get_skill(sk)<party[p]->skill_cap(sk,true))bright=1;
            }

            set_color(COLOR_WHITE,COLOR_BLACK,bright);
            move(p+2,23);
            itoa(skill,num,10);
            addstr(num);
            addstr("/");
            int wsk = SKILL_HANDTOHAND;
            if(party[p]->get_weapon().has_musical_attack())
               wsk=SKILL_MUSIC;
            else if (party[p]->has_thrown_weapon && !party[p]->extra_throwing_weapons.empty())
               wsk=party[p]->extra_throwing_weapons[0]->get_attack(false,false,false)->skill;
            else
               wsk=party[p]->get_weapon().get_attack(false,false,false)->skill;
            itoa(party[p]->get_skill(wsk),num,10);
            addstr(num);

            move(p+2,31);
            if(mode!=GAMEMODE_SITE)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else
               switch(weaponcheck(*party[p]))
            {
               case -1:
               case 0:set_color(COLOR_GREEN,COLOR_BLACK,1);break;
               case 1:set_color(COLOR_YELLOW,COLOR_BLACK,1);break;
               case 2:set_color(COLOR_RED,COLOR_BLACK,1);break;
            }
            if(party[p]->has_thrown_weapon && !party[p]->extra_throwing_weapons.empty())
               addstr(party[p]->extra_throwing_weapons[0]->get_shortname(0).c_str());
            else
               addstr(party[p]->get_weapon().get_shortname(0).c_str());
            //set_color(COLOR_WHITE,COLOR_BLACK,0);
            if(party[p]->get_weapon().get_ammoamount()>0)
            {
               //set_color(COLOR_WHITE,COLOR_BLACK,0);
               char num[20];
               itoa(party[p]->get_weapon().get_ammoamount(),num,10);
               addstr(" (");
               addstr(num);
               addstr(")");
            }
            else if(party[p]->get_weapon().uses_ammo())
            {
               set_color(COLOR_BLACK,COLOR_BLACK,1);
               if(!party[p]->clips.empty())
               {
                  char num[20];
                  itoa(party[p]->count_clips(),num,10);
                  addstr(" (");
                  addstr(num);
                  addstr(")");
               }
               else
               {
                  addstr(" (XX)");
               }
            }
            else if(party[p]->get_weapon().is_throwable() && !party[p]->has_thrown_weapon)
            {
               addstr(" (1)");
            }
            else if(party[p]->has_thrown_weapon && !party[p]->extra_throwing_weapons.empty())
            {
               set_color(COLOR_BLACK,COLOR_BLACK,1);
               int no_weapons = party[p]->count_weapons() - party[p]->is_armed();
               char num[20];
               itoa(no_weapons,num,10);
               addstr(" (");
               addstr(num);
               addstr(")");
            }

            if(mode!=GAMEMODE_SITE)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else
            {
               switch(hasdisguise(*party[p]))
               {
               case 1:set_color(COLOR_GREEN,COLOR_BLACK,1);break;
               case 2:set_color(COLOR_YELLOW,COLOR_BLACK,1);break;
               default:set_color(COLOR_RED,COLOR_BLACK,1);break;
               }
            }
            move(p+2,46);
            addstr(party[p]->get_armor().get_shortname().c_str());

            printhealthstat(*party[p],p+2,61,TRUE);

            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(p+2,70);

            long v=-1;
            if(showcarprefs==1)v=id_getcar(party[p]->pref_carid);
            else v=id_getcar(party[p]->carid);
            if(v!=-1&&showcarprefs!=-1)
            {
               strcpy(str,vehicle[v]->shortname().c_str());
               char d;
               if(showcarprefs==1)d=party[p]->pref_is_driver;
               else d=party[p]->is_driver;
               if(d)strcat(str,"-D");
            }
            else
            {
               int legok=2;
               if((party[p]->wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
                  (party[p]->wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
               if((party[p]->wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
                  (party[p]->wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;
               if(party[p]->flag & CREATUREFLAG_WHEELCHAIR)strcpy(str,"Wheelchair");
               else if(legok>=1)strcpy(str,"On Foot");
               else strcpy(str,"On \"Foot\"");
            }
            addstr(str);
         }
      }
   }

   makedelimiter(8,0);
}


/* location info at top of screen */
void printlocation(long loc)
{
   if(location[loc]->siege.siege)
   {
      if(!location[loc]->siege.underattack)
      {
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         move(2,1);
         addstr("The police have surrounded this location.");
      }
      else
      {
         set_color(COLOR_RED,COLOR_BLACK,1);
         move(2,1);
         switch(location[loc]->siege.siegetype)
         {
         case SIEGE_POLICE:
            addstr("The police are raiding this location!");break;
         case SIEGE_CIA:
            addstr("The CIA is raiding this location!");break;
         case SIEGE_HICKS:
            addstr("The masses are storming this location!");break;
         case SIEGE_CORPORATE:
            addstr("The Corporations are raiding this location!");break;
         case SIEGE_CCS:
            addstr("The CCS is raiding this location!");break;
         case SIEGE_FIREMEN:
            addstr("Firemen are raiding this location!");break;
         }
      }
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(2,1);
      addstr("You are not under siege...  yet.");
   }

   if(location[loc]->type==SITE_INDUSTRY_WAREHOUSE ||
      location[loc]->type==SITE_BUSINESS_CRACKHOUSE)
   {
      if(numbereating(loc)>0)
      {
         if(fooddaysleft(loc))
         {
            if(fooddaysleft(loc)<4)
            {
               if(!location[loc]->siege.siege)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_YELLOW,COLOR_BLACK,1);
               move(3,1);
               addstr("This location has food for only a few days.");
            }
         }
         else
         {
            if(!location[loc]->siege.siege)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_RED,COLOR_BLACK,1);
            move(3,1);
            addstr("This location has insufficient food stores.");
         }
      }

      if(location[loc]->compound_walls & COMPOUND_BASIC)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(4,1);
         addstr("FORTIFIED COMPOUND");
      }

      if(location[loc]->compound_walls & COMPOUND_PRINTINGPRESS)
      {
         set_color(COLOR_BLUE,COLOR_BLACK,1);
         move(4,31);
         addstr("PRINTING PRESS");
      }

      if(location[loc]->front_business!=-1)
      {
         set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         move(4,54);
         addstr("BUSINESS FRONT");
      }

      if(location[loc]->compound_walls & COMPOUND_CAMERAS)
      {
         if(location[loc]->siege.siege&&location[loc]->siege.cameras_off)
         {
            set_color(COLOR_RED,COLOR_BLACK,0);
            move(5,1);
            addstr("CAMERAS OFF");
         }
         else
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            move(5,1);
            addstr("CAMERAS ON");
         }
      }

      if(location[loc]->compound_walls & COMPOUND_TRAPS)
      {
         set_color(COLOR_RED,COLOR_BLACK,1);
         move(5,21);
         addstr("BOOBY TRAPS");
      }

      if(location[loc]->compound_walls & COMPOUND_TANKTRAPS)
      {
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         move(5,41);
         addstr("TANK TRAPS");
      }

      if(location[loc]->siege.siege&&location[loc]->siege.lights_off)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(5,60);
         addstr("LIGHTS OUT");
      }
      else if(location[loc]->compound_walls & COMPOUND_GENERATOR)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(5,61);
         addstr("GENERATOR");
      }

      int eaters=numbereating(loc);
      int days=fooddaysleft(loc);
      char num[20];

      if(eaters>0)
      {
         if(days>=1)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            itoa(days,num,10);
            move(6,50);
            addstr(num);
            addstr(" Day");
            if(days!=1)addstr("s");
            addstr(" of Food Left");
         }
         else if(days==0)
         {
            set_color(COLOR_RED,COLOR_BLACK,0);
            move(6,50);
            addstr("Not Enough Food");
         }
      }
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      itoa(location[loc]->compound_stores,num,10);
      move(6,1);
      addstr(num);
      addstr(" Daily Ration");
      if(location[loc]->compound_stores!=1)addstr("s");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      itoa(eaters,num,10);
      move(6,30);
      addstr(num);
      addstr(" Eating");
   }
}


/* character info at top of screen */
void printcreatureinfo(Creature *cr, unsigned char knowledge)
{
   char num[20],str[200];

   makedelimiter(1,0);

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(1,2);
   addstr(cr->name);
   addstr(", ");
   gettitle(str,*cr);
   addstr(str);

   if(cr->prisoner!=NULL)
   {
      addstr(", holding ");
      switch(cr->prisoner->type)
      {
      case CREATURE_CORPORATE_CEO:addstr("a squirming CEO");break;
      case CREATURE_RADIOPERSONALITY:addstr("a crying Radio Personality");break;
      case CREATURE_NEWSANCHOR:addstr("smarmy News Anchor");break;
      case CREATURE_SCIENTIST_EMINENT:addstr("a frightened Eminent Scientist");break;
      case CREATURE_JUDGE_CONSERVATIVE:addstr("an angry Hangin' Judge");break;
      default:
         addstr(cr->prisoner->name);
         break;
      }
   }

   move(2,0);addstr("Hrt:    ");
   if(knowledge>0)
   {
      itoa(cr->get_attribute(ATTRIBUTE_HEART,true),num,10);
      addstr(num);
   }
   else addstr("?");
   move(3,0);addstr("Int:    ");
   if(knowledge>0)
   {
      itoa(cr->get_attribute(ATTRIBUTE_INTELLIGENCE,true),num,10);
      addstr(num);
   }
   else addstr("?");
   move(4,0);addstr("Wis:    ");
   if(knowledge>0)
   {
      itoa(cr->get_attribute(ATTRIBUTE_WISDOM,true),num,10);
      addstr(num);
   }
   else addstr("?");
   move(5,0);addstr("Hlth:   ");
   if(knowledge>1)
   {
      itoa(cr->get_attribute(ATTRIBUTE_HEALTH,true),num,10);
      addstr(num);
   }
   else addstr("?");
   move(2,11);addstr("Agi:    ");
   if(knowledge>1)
   {
      itoa(cr->get_attribute(ATTRIBUTE_AGILITY,true),num,10);
      addstr(num);
   }
   else addstr("?");
   move(3,11);addstr("Str:    ");
   if(knowledge>1)
   {
      itoa(cr->get_attribute(ATTRIBUTE_STRENGTH,true),num,10);
      addstr(num);
   }
   else addstr("?");
   move(4,11);addstr("Char:   ");
   if(knowledge>0)
   {
      itoa(cr->get_attribute(ATTRIBUTE_CHARISMA,true),num,10);
      addstr(num);
   }
   else addstr("?");
   move(5,11);
   addstr("Trans: ");
   long v=-1;
   if(showcarprefs==1)v=id_getcar(cr->pref_carid);
   else v=id_getcar(cr->carid);
   if(v!=-1&&showcarprefs!=-1)
   {
      strcpy(str,vehicle[v]->shortname().c_str());
      char d;
      if(showcarprefs==1)d=cr->pref_is_driver;
      else d=cr->is_driver;
      if(d)strcat(str,"-D");
   }
   else
   {
      int legok=2;
      if((cr->wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
         (cr->wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
      if((cr->wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
         (cr->wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;
      if(cr->flag & CREATUREFLAG_WHEELCHAIR)strcpy(str,"Wheelchair");
      else if(legok>=1)strcpy(str,"On Foot");
      else strcpy(str,"On \"Foot\"");
   }
   addstr(str);
   move(6,0);
   if(mode!=GAMEMODE_SITE)set_color(COLOR_WHITE,COLOR_BLACK,0);
   else
      switch(weaponcheck(*cr))
   {
      case -1:
      case 0:set_color(COLOR_GREEN,COLOR_BLACK,1);break;
      case 1:set_color(COLOR_YELLOW,COLOR_BLACK,1);break;
      case 2:set_color(COLOR_RED,COLOR_BLACK,1);break;
   }
   addstr("Weapon: ");
   addstr(cr->get_weapon_string(1).c_str());
   

   if(mode!=GAMEMODE_SITE)set_color(COLOR_WHITE,COLOR_BLACK,0);
   else
   {
      switch(hasdisguise(*cr))
      {
      case 1:set_color(COLOR_GREEN,COLOR_BLACK,1);break;
      case 2:set_color(COLOR_YELLOW,COLOR_BLACK,1);break;
      default:set_color(COLOR_RED,COLOR_BLACK,1);break;
      }
   }
   move(7,0);
   addstr("Clothes: ");
   addstr(cr->get_armor_string(false).c_str());

   set_color(COLOR_WHITE,COLOR_BLACK,0);

   char used[SKILLNUM];
   memset(used,0,sizeof(char)*SKILLNUM);

   int snum=5;
   char printed=1;

   while(snum>0&&printed)
   {
      printed=0;

      int max=0;
      long maxs=-1;
      for(int s=0;s<SKILLNUM;s++)
      {
         if(cr->get_skill(s)>max && !used[s])
         {
            max=cr->get_skill(s);
            maxs=s;
         }
      }

      if(maxs!=-1)
      {
         used[maxs]=1;
         printed=1;

         if(cr->get_skill_ip(maxs)>=100+(10*cr->get_skill(maxs))&&
            cr->get_skill(maxs)<cr->skill_cap(maxs,true))set_color(COLOR_WHITE,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);

         move(3+5-snum,31);
         if(knowledge>5-snum)
            strcpy(str,Skill::get_name(maxs).c_str());
         else
            strcpy(str,"???????");
         strcat(str,": ");
         if(knowledge>7-snum)
            itoa(cr->get_skill(maxs),num,10);
         else
            strcpy(num,"?");
         strcat(str,num);
         addstr(str);

         if(snum==5&&printed)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(2,31);
            addstr("Top Skills:");
         }
      }

      snum--;
   }

   int woundsum=0;
   for(int w=0;w<BODYPARTNUM;w++)
   {
      if(cr->wound[w]!=0)woundsum++;
   }

   printhealthstat(*cr,1,49,FALSE);

   if(woundsum>0)
   {
      for(int w=0;w<BODYPARTNUM;w++)
      {
         if(cr->wound[w] & WOUND_BLEEDING)set_color(COLOR_RED,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);

         move(2+w,49);
         switch(w)
         {
         case BODYPART_HEAD:addstr("Head:");break;
         case BODYPART_BODY:addstr("Body:");break;
         case BODYPART_ARM_RIGHT:addstr("Right Arm:");break;
         case BODYPART_ARM_LEFT:addstr("Left Arm:");break;
         case BODYPART_LEG_RIGHT:addstr("Right Leg:");break;
         case BODYPART_LEG_LEFT:addstr("Left Leg:");break;
         }

         move(2+w,61);
         if(cr->wound[w] & WOUND_NASTYOFF)addstr("Ripped off");
         else if(cr->wound[w] & WOUND_CLEANOFF)addstr("Clean sever");
         else
         {
            int sum=0;

            if(cr->wound[w] & WOUND_SHOT)sum++;
            if(cr->wound[w] & WOUND_CUT)sum++;
            if(cr->wound[w] & WOUND_BRUISED)sum++;
            if(cr->wound[w] & WOUND_BURNED)sum++;
            if(cr->wound[w] & WOUND_TORN)sum++;

            if(sum==0)
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               if(cr->animalgloss==ANIMALGLOSS_ANIMAL)
                  addstr("Animal");
               else
                  addstr("Liberal");
            }

            if(cr->wound[w] & WOUND_SHOT){addstr("Sht");sum--;if(sum>0)addstr(",");}
            if(cr->wound[w] & WOUND_BRUISED){addstr("Brs");sum--;if(sum>0)addstr(",");}
            if(cr->wound[w] & WOUND_CUT){addstr("Cut");sum--;if(sum>0)addstr(",");}
            if(cr->wound[w] & WOUND_TORN){addstr("Trn");sum--;if(sum>0)addstr(",");}
            if(cr->wound[w] & WOUND_BURNED){addstr("Brn");sum--;if(sum>0)addstr(",");}
         }
      }
      set_color(COLOR_WHITE,COLOR_BLACK,0);
   }
}


/* full character sheet (with surrounding interface) */
void fullstatus(int p)
{
   if(activesquad==NULL)return;

   const int pagenum=3;
   int page=0;

   do
   {
      erase();

      set_color(COLOR_GREEN,COLOR_BLACK,1);
      move(0,0);
      addstr("Profile of a Liberal");

      if(page==0)
         printliberalstats(*activesquad->squad[p]);
      else if(page==1)
         printliberalskills(*activesquad->squad[p]);
      else if (page == 2)
         printliberalcrimes(*activesquad->squad[p]);

      move(23,0);
      addstr("N - Change Code Name      G - Fix Gender Label");
      if(activesquad->squad[1]!=NULL)
      {
         addstr("    LEFT/RIGHT - Other Liberals");
      }
      move(24,0);
      addstr("Press any other key to continue the Struggle");
      addstr("    UP/DOWN  - More Info");

      refresh();
      int c=getch();
      translategetch(c);

      if(activesquad->squad[1]!=NULL&&((c==KEY_LEFT)||(c==KEY_RIGHT)))
      {
         int sx=1;
         if((c==KEY_LEFT))sx=-1;
         do
         {
            p=(p+6+sx)%6;
         }while(activesquad->squad[p]==NULL);
         continue;
      }

      if(c==KEY_DOWN)
      {
         page++;
         page%=pagenum;
         continue;
      }

      if(c==KEY_UP)
      {
         page--;
         if(page<0)page=pagenum-1;
         page%=pagenum;
         continue;
      }

      if(c=='n')
      {
         move(23,0);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr("What is the new code name?                                       ");
         move(24,0);
         addstr("                                                                 ");

         keypad(stdscr,FALSE);
         raw_output(FALSE);
         echo();
         curs_set(1);
         move(24,0);
         enter_name(activesquad->squad[p]->name,CREATURE_NAMELEN);

         curs_set(0);
         noecho();
         raw_output(TRUE);
         keypad(stdscr,TRUE);
         continue;
      }
      else if(c=='g')
      {
         activesquad->squad[p]->gender_liberal++;
         if(activesquad->squad[p]->gender_liberal > 2)
            activesquad->squad[p]->gender_liberal = 0;
         continue;
      }
      break;
   }while(1);
}


/* Full screen character sheet, skills only edition */
void printliberalskills(Creature &cr)
{
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   char str[200];
   char num[5];

   // Add name
   move(2,0);
   if(strcmp(cr.propername,cr.name)!=0)
   {
      addstr("Code name: ");
   }
   else
   {
      addstr("Name: ");
   }
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   addstr(cr.name);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   addstr(", ");
   gettitle(str,cr);
   addstr(str);
   addstr(" (");
   getrecruitcreature(str,cr.type);
   addstr(str);
   addstr(")");

   // Add all skills
   for(int s=0;s<SKILLNUM;s++)
   {
      if(s%3==0 && s<9)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(4,27*(s/3));
         addstr("SKILL");
         move(4,15+27*(s/3));
         addstr("NOW   MAX");
      }

      // Maxed skills are green
      if(cr.skill_cap(s,true)!=0 && cr.get_skill(s)>=cr.skill_cap(s,true))set_color(COLOR_CYAN,COLOR_BLACK,1);
      // About to level up skills are white
      else if(cr.get_skill_ip(s)>=100+(10*cr.get_skill(s))&&
         cr.get_skill(s)<cr.skill_cap(s,true))set_color(COLOR_WHITE,COLOR_BLACK,1);
      // <1 skills are dark gray
      else if(cr.get_skill(s)<1)set_color(COLOR_BLACK,COLOR_BLACK,1);
      // >=1 skills are light gray
      else set_color(COLOR_WHITE,COLOR_BLACK,0);

      move(5+s/3,27*(s%3));
      strcpy(str,Skill::get_name(s).c_str());
      strcat(str,": ");
      addstr(str);
      move(5+s/3,14+27*(s%3));
      sprintf(num,"%2d.",cr.get_skill(s));
      addstr(num);
      if(cr.get_skill_ip(s)<100+(10*cr.get_skill(s)))
      {
         if ((cr.get_skill_ip(s)*100)/(100+(10*cr.get_skill(s)))!=0)
         {
            itoa((cr.get_skill_ip(s)*100)/(100+(10*cr.get_skill(s))),num,10);
            if ((cr.get_skill_ip(s)*100)/(100+(10*cr.get_skill(s)))<10)
            {
               addstr("0");
            }
            addstr(num);
         }
         else addstr("00");
      }
      else
      {
         addstr("99+");
      }

      if(cr.skill_cap(s,true)==0 || cr.get_skill(s)<cr.skill_cap(s,true))
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(5+s/3,20+27*(s%3));
      sprintf(str,"%2d.00",cr.skill_cap(s,true));
      addstr(str);
   }
   set_color(COLOR_WHITE,COLOR_BLACK,0);
}


/* full screen character sheet */
void printliberalstats(Creature &cr)
{
   set_color(COLOR_WHITE,COLOR_BLACK,0);

   char num[20],str[200];

   // Add name
   move(2,0);
   addstr("Name: ");
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   addstr(cr.name);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   if(strcmp(cr.propername,cr.name)!=0)
   {
      //The names do not match, print real name as well
      addstr(" (");
      addstr(cr.propername);
      addstr(")");
   }
   addstr(", ");
   gettitle(str,cr);
   addstr(str);
   addstr(" (");
   getrecruitcreature(str,cr.type);
   addstr(str);
   addstr(")");
   move(3,0);


   // Add birthdate
   addstr("Born ");
   switch(cr.birthday_month)
   {
   case 1:addstr("January");break;
   case 2:addstr("February");break;
   case 3:addstr("March");break;
   case 4:addstr("April");break;
   case 5:addstr("May");break;
   case 6:addstr("June");break;
   case 7:addstr("July");break;
   case 8:addstr("August");break;
   case 9:addstr("September");break;
   case 10:addstr("October");break;
   case 11:addstr("November");break;
   case 12:addstr("December");break;
   }
   addstr(" ");
   itoa(cr.birthday_day,num,10);
   addstr(num);
   addstr(", ");
   if(cr.birthday_month < month ||
      (cr.birthday_month == month && cr.birthday_day <= day))
   {
      itoa(year-cr.age,num,10);
   }
   else
   {
      itoa(year-1-cr.age,num,10);
   }
   addstr(num);
   // Add age
   addstr(" (Age ");
   itoa(cr.age,num,10);
   addstr(num);
   addstr(", ");
   // Add Liberal gender
   if(cr.gender_liberal == GENDER_FEMALE)
      addstr("Female");
   else if(cr.gender_liberal == GENDER_MALE)
      addstr("Male");
   else
      addstr("Androgynous");
   // Note if there's some conflict with Conservative society's perceptions
   //if(cr.gender_liberal != cr.gender_conservative)
   //   addstr("*");
   addstr(")");

   // Wanted for?
   move(3,46);
   statebrokenlaws(cr);

   // Current activity
   move(4,46);
   char str_activity[80];
   set_activity_color(cr.activity.type);
   getactivity(str_activity,cr.activity);
   addstr(str_activity);

   set_color(COLOR_WHITE,COLOR_BLACK,0);

   // Add juice
   move(10,16);
   addstr("Juice: ");
   itoa(cr.juice,num,10);
   addstr(num);
   if(cr.juice<1000)
   {
      move(11,16);addstr("Next:  ");
      if(cr.juice<0)addstr("0");
      else if(cr.juice<10)addstr("10");
      else if(cr.juice<50)addstr("50");
      else if(cr.juice<100)addstr("100");
      else if(cr.juice<200)addstr("200");
      else if(cr.juice<500)addstr("500");
      else addstr("1000");
   }
   // Add attributes
   move(5,0);addstr("Heart: ");
   itoa(cr.get_attribute(ATTRIBUTE_HEART,true),num,10);
   addstr(num);
   move(6,0);addstr("Intelligence: ");
   itoa(cr.get_attribute(ATTRIBUTE_INTELLIGENCE,true),num,10);
   addstr(num);
   move(7,0);addstr("Wisdom: ");
   itoa(cr.get_attribute(ATTRIBUTE_WISDOM,true),num,10);
   addstr(num);
   move(8,0);addstr("Health: ");
   itoa(cr.get_attribute(ATTRIBUTE_HEALTH,true),num,10);
   addstr(num);
   move(9,0);addstr("Agility: ");
   itoa(cr.get_attribute(ATTRIBUTE_AGILITY,true),num,10);
   addstr(num);
   move(10,0);addstr("Strength: ");
   itoa(cr.get_attribute(ATTRIBUTE_STRENGTH,true),num,10);
   addstr(num);
   move(11,0);addstr("Charisma: ");
   itoa(cr.get_attribute(ATTRIBUTE_CHARISMA,true),num,10);
   addstr(num);

   // Add highest skills
   char used[SKILLNUM];
   memset(used,0,sizeof(char)*SKILLNUM);

   int skills_max=16;
   char printed=1;

   move(5,28);
   addstr("SKILL");
   move(5,43);
   addstr("NOW   MAX");
   for(int skills_shown=0;skills_shown<skills_max&&printed;skills_shown++)
   {
      printed=0;

      int max=0;
      long maxs=-1;
      for(int s=0;s<SKILLNUM;s++)
      {
         if((cr.get_skill(s)*10000+cr.get_skill_ip(s))>max && !used[s])
         {
            max=(cr.get_skill(s)*10000+cr.get_skill_ip(s));
            maxs=s;
         }
      }

      if(maxs!=-1)
      {
         used[maxs]=1;
         printed=1;

         // Maxed skills are green
         if(cr.skill_cap(maxs,true)!=0 && cr.get_skill(maxs)>=cr.skill_cap(maxs,true))set_color(COLOR_CYAN,COLOR_BLACK,1);
         // About to level up skills are white
         else if(cr.get_skill_ip(maxs)>=100+(10*cr.get_skill(maxs))&&
            cr.get_skill(maxs)<cr.skill_cap(maxs,true))set_color(COLOR_WHITE,COLOR_BLACK,1);
         // <1 skills are dark gray
         else if(cr.get_skill(maxs)<1)set_color(COLOR_BLACK,COLOR_BLACK,1);
         // >=1 skills are light gray
         else set_color(COLOR_WHITE,COLOR_BLACK,0);

         move(6+skills_shown,28);
         strcpy(str,Skill::get_name(maxs).c_str());
         strcat(str,": ");
         addstr(str);
         move(6+skills_shown,42);
         sprintf(num,"%2d.",cr.get_skill(maxs));
         addstr(num);
         if(cr.get_skill_ip(maxs)<100+(10*cr.get_skill(maxs)))
         {
            if ((cr.get_skill_ip(maxs)*100)/(100+(10*cr.get_skill(maxs)))!=0)
            {
               itoa((cr.get_skill_ip(maxs)*100)/(100+(10*cr.get_skill(maxs))),num,10);
               if ((cr.get_skill_ip(maxs)*100)/(100+(10*cr.get_skill(maxs)))<10)
               {
                  addstr("0");
               }
               addstr(num);
            }
            else addstr("00");
         }
         else
         {
            addstr("99+");
         }

         if(cr.skill_cap(maxs,true)==0 || cr.get_skill(maxs)<cr.skill_cap(maxs,true))
            set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(6+skills_shown,48);
         sprintf(str,"%2d.00",cr.skill_cap(maxs,true));
         addstr(str);
      }
   }

   set_color(COLOR_WHITE,COLOR_BLACK,0);

   // Add weapon
   move(13,0);
   addstr("Weapon: ");
   addstr(cr.get_weapon_string(0).c_str());

   // Add clothing
   move(14,0);
   addstr("Clothes: ");
   addstr(cr.get_armor_string(true).c_str());

   // Add vehicle
   move(15,0);
   addstr("Car: ");
   long v=-1;
   if(showcarprefs==1)v=id_getcar(cr.pref_carid);
   else v=id_getcar(cr.carid);
   if(v!=-1&&showcarprefs!=-1)
   {
      strcpy(str,vehicle[v]->shortname().c_str());
      char d;
      if(showcarprefs==1)d=cr.pref_is_driver;
      else d=cr.is_driver;
      if(d)strcat(str,"-D");
   }
   else
   {
      int legok=2;
      if((cr.wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
         (cr.wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
      if((cr.wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
         (cr.wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;
      if(cr.flag & CREATUREFLAG_WHEELCHAIR)strcpy(str,"Wheelchair");
      else if(legok>=1)strcpy(str,"On Foot");
      else strcpy(str,"On \"Foot\"");
   }
   addstr(str);

   //Add Equip
   move(16,0);
   addstr("E - Equip   V - Vehicule");

   // Add recruit stats
   if(cr.flag!=CREATUREFLAG_BRAINWASHED)
   {
      move(18,0);
      itoa(maxsubordinates(cr)-subordinatesleft(cr),num,10);
      addstr(num);
      addstr(" Recruits / ");
      itoa(maxsubordinates(cr),num,10);
      addstr(num);
      addstr(" Max");
   }
   else
   {
      move(18,0);
      addstr("Enlightened ");
      addstr("Can't Recruit");
   }
   // Any meetings with potential recruits scheduled?
   int recruiting = scheduledmeetings(cr);
   if(recruiting)
   {
      move(18,55);
      itoa(recruiting,num,10);
      addstr("Scheduled Meetings: ");
      addstr(num);
   }
   // Add seduction stats
   move(19,0); 
   int lovers = loveslaves(cr);
   if(cr.flag & CREATUREFLAG_LOVESLAVE)
      lovers++;
   if(lovers)
   {
      itoa(lovers,num,10);
      addstr(num);
      addstr(" Romantic Interest");
      if(lovers>1)addstr("s");
   }
   // Any dates with potential love interests scheduled?
   int wooing = scheduleddates(cr);
   if(wooing)
   {
      move(19,55);
      itoa(wooing,num,10);
      addstr("Scheduled Dates:    ");
      addstr(num);
   }

   // Add wound status
   for(int w=0;w<BODYPARTNUM;w++)
   {
      if(cr.wound[w] & WOUND_BLEEDING)set_color(COLOR_RED,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);

      move(5+w,55);
      switch(w)
      {
      case BODYPART_HEAD:addstr("Head:");break;
      case BODYPART_BODY:addstr("Body:");break;
      case BODYPART_ARM_RIGHT:addstr("Right Arm:");break;
      case BODYPART_ARM_LEFT:addstr("Left Arm:");break;
      case BODYPART_LEG_RIGHT:addstr("Right Leg:");break;
      case BODYPART_LEG_LEFT:addstr("Left Leg:");break;
      }

      move(5+w,66);
      if(cr.wound[w] & WOUND_NASTYOFF)addstr("Ripped off");
      else if(cr.wound[w] & WOUND_CLEANOFF)addstr("Severed");
      else
      {
         int sum=0;

         if(cr.wound[w] & WOUND_SHOT)sum++;
         if(cr.wound[w] & WOUND_CUT)sum++;
         if(cr.wound[w] & WOUND_BRUISED)sum++;
         if(cr.wound[w] & WOUND_BURNED)sum++;
         if(cr.wound[w] & WOUND_TORN)sum++;

         if(sum==0)
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            if(cr.animalgloss==ANIMALGLOSS_ANIMAL)
               addstr("Animal");
            else
               addstr("Liberal");
         }

         if(cr.wound[w] & WOUND_SHOT){addstr("Shot");sum--;if(sum>0)addstr(",");}
         if(cr.wound[w] & WOUND_BRUISED){addstr("Bruised");sum--;if(sum>0)addstr(",");}
         if(cr.wound[w] & WOUND_CUT){addstr("Cut");sum--;if(sum>0)addstr(",");}
         if(cr.wound[w] & WOUND_TORN){addstr("Torn");sum--;if(sum>0)addstr(",");}
         if(cr.wound[w] & WOUND_BURNED){addstr("Burned");sum--;if(sum>0)addstr(",");}
      }
   }
   set_color(COLOR_WHITE,COLOR_BLACK,0);

   //SPECIAL WOUNDS
   set_color(COLOR_RED,COLOR_BLACK,0);

   int y=12;
   int x=55;
   if(cr.special[SPECIALWOUND_HEART]!=1)
   {
      move(y++,x);
      addstr("Heart Punctured");
   }
   if(cr.special[SPECIALWOUND_RIGHTLUNG]!=1)
   {
      move(y++,x);
      addstr("R. Lung Collapsed");
   }
   if(cr.special[SPECIALWOUND_LEFTLUNG]!=1)
   {
      move(y++,x);
      addstr("L. Lung Collapsed");
   }
   if(cr.special[SPECIALWOUND_NECK]!=1)
   {
      move(y++,x);
      addstr("Broken Neck");
   }
   if(cr.special[SPECIALWOUND_UPPERSPINE]!=1)
   {
      move(y++,x);
      addstr("Broken Up Spine");
   }
   if(cr.special[SPECIALWOUND_LOWERSPINE]!=1)
   {
      move(y++,x);
      addstr("Broken Lw Spine");
   }
   if(cr.special[SPECIALWOUND_RIGHTEYE]!=1)
   {
      move(y++,x);
      addstr("No Right Eye");
   }
   if(cr.special[SPECIALWOUND_LEFTEYE]!=1)
   {
      move(y++,x);
      addstr("No Left Eye");
   }
   if(cr.special[SPECIALWOUND_NOSE]!=1)
   {
      move(y++,x);
      addstr("No Nose");
   }
   if(cr.special[SPECIALWOUND_TONGUE]!=1)
   {
      move(y++,x);
      addstr("No Tongue");
   }
   if(cr.special[SPECIALWOUND_TEETH]!=TOOTHNUM)
   {
      move(y++,x);
      if(cr.special[SPECIALWOUND_TEETH]==0)addstr("No Teeth");
      else if(cr.special[SPECIALWOUND_TEETH]==TOOTHNUM-1)addstr("Missing a Tooth");
      else if(cr.special[SPECIALWOUND_TEETH]<TOOTHNUM)addstr("Missing Teeth");
   }
   if(cr.special[SPECIALWOUND_LIVER]!=1)
   {
      move(y++,x);
      addstr("Liver Damaged");
   }
   if(cr.special[SPECIALWOUND_RIGHTKIDNEY]!=1)
   {
      move(y++,x);
      addstr("R. Kidney Damaged");
   }
   if(cr.special[SPECIALWOUND_LEFTKIDNEY]!=1)
   {
      move(y++,x);
      addstr("L. Kidney Damaged");
   }
   if(cr.special[SPECIALWOUND_STOMACH]!=1)
   {
      move(y++,x);
      addstr("Stomach Injured");
   }
   if(cr.special[SPECIALWOUND_SPLEEN]!=1)
   {
      move(y++,x);
      addstr("Busted Spleen");
   }
   if(cr.special[SPECIALWOUND_RIBS]!=RIBNUM)
   {
      move(y++,x);
      if(cr.special[SPECIALWOUND_RIBS]==0)addstr("All Ribs Broken");
      else if(cr.special[SPECIALWOUND_RIBS]==RIBNUM-1)addstr("Broken Rib");
      else if(cr.special[SPECIALWOUND_RIBS]<RIBNUM)addstr("Broken Ribs");
   }

   set_color(COLOR_WHITE,COLOR_BLACK,0);
}

/* Full screen character sheet, crime sheet */
void printliberalcrimes(Creature &cr)
{
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   char str[200];
   char num[5];

   // Add name
   move(2,0);
   if(strcmp(cr.propername,cr.name)!=0)
   {
      addstr("Code name: ");
   }
   else
   {
      addstr("Name: ");
   }
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   addstr(cr.name);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   addstr(", ");
   gettitle(str,cr);
   addstr(str);
   addstr(" (");
   getrecruitcreature(str,cr.type);
   addstr(str);
   addstr(")");

   // Add all crimes
   for(int i = 0;i < LAWFLAGNUM; i++)
   {
      if(i%2==0 && i<4)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(4,40*(i/2));
         addstr("CRIME");
         move(4,30+40*(i/2));
         addstr("NUM");
      }

      // Commited crimes are yellow
      if(cr.crimes_suspected[i] > 0)
      {
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
      }
      else
      {
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      }

      move(5+i/2,40*(i%2));
      getlawflag(str,i);
      strcat(str,": ");
      addstr(str);
      move(5+i/2,30+40*(i%2));
      sprintf(num,"%2d",cr.crimes_suspected[i]);
      addstr(num);
   }

   set_color(COLOR_WHITE,COLOR_BLACK,0);
}

/* Full screen character sheet, relations sheet */
void printliberalrelations(Creature &cr)
{
  set_color(COLOR_WHITE,COLOR_BLACK,0);
     set_color(COLOR_WHITE,COLOR_BLACK,0);
   char str[200];
   char num[5];

   // Add name
   move(2,0);
   if(strcmp(cr.propername,cr.name)!=0)
   {
      addstr("Code name: ");
   }
   else
   {
      addstr("Name: ");
   }
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   addstr(cr.name);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   addstr(", ");
   gettitle(str,cr);
   addstr(str);
   addstr(" (");
   getrecruitcreature(str,cr.type);
   addstr(str);
   addstr(")");

   // Add superior
   set_color(COLOR_BLUE,COLOR_BLACK,1);
   move(4,0);
   if(cr.hireid == -1)
   {
       addstr("  <LCS Leader>");
   }
   else
   {
       int boss = getpoolcreature(cr.hireid);
       addstr("Recruiter: ");
       addstr(pool[boss]->name);
   }

   // Add all subordinates


//restore default color
     set_color(COLOR_WHITE,COLOR_BLACK,0);
}


/* draws a horizontal line across the screen */
void makedelimiter(int y,int x)
{
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(y,x);
   addstr("--------------------------------------------------------------------------------");
}



/* print location name (is aware of business fronts) */
void addlocationname(locationst *loc)
{
   if(loc->front_business!=-1)
   {
      addstr(loc->front_name);
   }
   else
   {
      addstr(loc->name);
   }
}

/* print location's shortname (is aware of business fronts) */
void addshortname (locationst *loc)
{
   if (loc->front_business!=-1) {
      addstr (loc->front_shortname);
   }
   else {
      addstr (loc->shortname);
   }
}

/* prints a character's health description (One Leg, Liberal, NearDETH...) */
void printhealthstat(Creature &g,int y,int x,char smll)
{
   short woundsum=0;
   char bleeding=0;
   for(int w=0;w<BODYPARTNUM;w++)
   {
      if(g.wound[w]!=0)woundsum++;
      if(g.wound[w] & WOUND_BLEEDING)bleeding=1;
   }

   int armok=2;
   int legok=2;
   if((g.wound[BODYPART_ARM_RIGHT] & WOUND_NASTYOFF)||
      (g.wound[BODYPART_ARM_RIGHT] & WOUND_CLEANOFF))armok--;
   if((g.wound[BODYPART_ARM_LEFT] & WOUND_NASTYOFF)||
      (g.wound[BODYPART_ARM_LEFT] & WOUND_CLEANOFF))armok--;
   if((g.wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
      (g.wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
   if((g.wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
      (g.wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;

   move(y,x);
   if(bleeding)set_color(COLOR_RED,COLOR_BLACK,1);
   else set_color(COLOR_WHITE,COLOR_BLACK,1);
   if(!g.alive)
   {
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr("Deceased");
   }
   else if(g.blood<=20)
   {
      if(smll)addstr("NearDETH");
      else addstr("Near Death");
   }
   else if(g.blood<=50)
   {
      if(smll)addstr("BadWound");
      else addstr("Badly Wounded");
   }
   else if(g.blood<=75)
   {
      addstr("Wounded");
   }
   else if(g.blood<100)
   {
      if(smll)addstr("LtWound");
      else addstr("Lightly Wounded");
   }
   else if(g.special[SPECIALWOUND_NECK]==2)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("NckBroke");
      else addstr("Neck Broken");
   }
   else if(g.special[SPECIALWOUND_UPPERSPINE]==2)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("Quadpleg");
      else addstr("Quadraplegic");
   }
   else if(g.special[SPECIALWOUND_LOWERSPINE]==2)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("Parapleg");
      else addstr("Paraplegic");
   }
   else if(g.special[SPECIALWOUND_RIGHTEYE]==0&&
      g.special[SPECIALWOUND_LEFTEYE]==0&&
      g.special[SPECIALWOUND_NOSE]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("FaceGone");
      else addstr("Face Gone");
   }
   else if(legok==0&&armok==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("No Limbs");
   }
   else if((legok==1&&armok==0)||(armok==1&&legok==0))
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("One Limb");
   }
   else if(legok==2&&armok==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("No Arms");
   }
   else if(legok==0&&armok==2)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("No Legs");
   }
   else if(legok==1&&armok==1)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("1Arm1Leg");
      else addstr("One Arm, One Leg");
   }
   else if(armok==1)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("One Arm");
   }
   else if(legok==1)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("One Leg");
   }
   else if(g.special[SPECIALWOUND_RIGHTEYE]==0&&
      g.special[SPECIALWOUND_LEFTEYE]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("Blind");
   }
   else if((g.special[SPECIALWOUND_RIGHTEYE]==0||
      g.special[SPECIALWOUND_LEFTEYE]==0)&&
      g.special[SPECIALWOUND_NOSE]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("FaceMutl");
      else addstr("Face Mutilated");
   }
   else if(g.special[SPECIALWOUND_NOSE]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("NoseGone");
      else addstr("Missing Nose");
   }
   else if(g.special[SPECIALWOUND_RIGHTEYE]==0||
      g.special[SPECIALWOUND_LEFTEYE]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("One Eye");
      else addstr("Missing Eye");
   }
   else if(g.special[SPECIALWOUND_TONGUE]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("NoTongue");
      else addstr("No Tongue");
   }
   else if(g.special[SPECIALWOUND_TEETH]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("No Teeth");
   }
   else if(g.special[SPECIALWOUND_TEETH]<TOOTHNUM)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("MisTeeth");
      else addstr("Missing Teeth");
   }
   else
   {
      if(g.align==-1)
      {
         set_color(COLOR_RED,COLOR_BLACK,1);
         if(smll)addstr("Consrvtv");
         else addstr("Conservative");
      }
      else if(g.align==0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         addstr("Moderate");
      }
      else
      {
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         if(g.animalgloss==ANIMALGLOSS_ANIMAL)
            addstr("Animal");
         else
            addstr("Liberal");
      }
   }
}

/*
This function prints the cash the player has with optional prefix as 
well as screen coordinates.

Please note that offsetx is the offset from the right of the screen, y is 
the offset from the top as always.
*/
void printfunds(int y, int offsetx, char* prefix)
{
   char moneystr[50];
   char prefixbuffer[50];

   if(prefix==NULL)
   {
      strncpy(prefixbuffer,"",50);
   }
   else
   {
      strncpy(prefixbuffer,prefix,50);
   }

   sprintf(moneystr,"$%d",ledger.get_funds());

   //Save screen coordinates for later.
   int begy,begx;
   getyx(stdscr,begy,begx);

   //Save color and brightness information for later.
   short colorpair;
   short front, back;
   char dim;

   attr_t attrs;
   attr_get(&attrs,&colorpair,NULL);
   if((attrs & WA_DIM)==0)
      dim=0;
   else
      dim=1;
   pair_content(colorpair,&front,&back);


   //Move, set color, and write.
   move(y,80-strlen(moneystr)-strlen(prefixbuffer)-offsetx);
   addstr(prefixbuffer);
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   addstr(moneystr);

   //Recover old settings
   move(begy,begx);
   set_color(front,back,dim);
}

/* prints a short blurb showing how to page forward */
void addnextpagestr()
{
   if(interface_pgup=='[')
      addstr("] - Next");
   else if(interface_pgup=='.')
      addstr(": - Next");
   else
      addstr("PGDN - Next");
}

/* prints a short blurb showing how to page back */
void addprevpagestr()
{
   if(interface_pgup=='[')
      addstr("[ - Previous");
   else if(interface_pgup=='.')
      addstr("; - Previous");
   else
      addstr("PGUP - Previous");
}

/* prints a long blurb showing how to page forward and back */
void addpagestr()
{
   if(interface_pgup=='[')
      addstr("[] to view other Liberal pages.");
   else if(interface_pgup=='.')
      addstr("; and : to view other Liberal pages.");
   else
      addstr("PGUP/PGDN to view other Liberal pages.");
}
