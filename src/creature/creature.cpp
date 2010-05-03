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

void Skill::set_type(int skill_type)
{
   skill = skill_type;
   // Initialize associated attribute
   associated_attribute = get_associated_attribute(skill);
}

CreatureAttribute Skill::get_associated_attribute(int skill_type)
{
   // Initialize associated attribute
   switch(skill_type)
   {
   case SKILL_CLUB:
   case SKILL_AXE:
   case SKILL_FLAMETHROWER:
      return ATTRIBUTE_STRENGTH;
   case SKILL_HANDTOHAND:
   case SKILL_KNIFE:
   case SKILL_SWORD:
   case SKILL_PISTOL:
   case SKILL_RIFLE:
   case SKILL_SMG:
   case SKILL_SHOTGUN:
   case SKILL_DRIVING:
   //case SKILL_THEFT:
   case SKILL_STEALTH:
   case SKILL_THROWING:
   case SKILL_DODGE:
      return ATTRIBUTE_AGILITY;
   case SKILL_DISGUISE:
   case SKILL_SEDUCTION:
   case SKILL_LEADERSHIP:
      return ATTRIBUTE_CHARISMA;
   case SKILL_PERSUASION:
   case SKILL_ART:
   case SKILL_MUSIC:
   case SKILL_COOKING:
      return ATTRIBUTE_HEART;
   case SKILL_RELIGION:
   case SKILL_BUSINESS:
      return ATTRIBUTE_WISDOM;
   //case SKILL_SURVIVAL:
   //   return ATTRIBUTE_HEALTH;
   case SKILL_WRITING:
   case SKILL_PSYCHOLOGY:
   case SKILL_SECURITY:
   case SKILL_TAILORING:
   case SKILL_TEACHING:
   case SKILL_FIRSTAID:
   case SKILL_SCIENCE:
   case SKILL_LAW:
   case SKILL_COMPUTERS:
   case SKILL_STREETSENSE:
   default:
      return ATTRIBUTE_INTELLIGENCE;
   }
}

/* returns the creature's maximum level in the given skill */
int Creature::skill_cap(int skill,bool use_juice)
{
   if(skill==SKILL_LEADERSHIP) // Special leadership skill cap
   {
      if(juice<10)return 0;
      if(juice<50)return 1;
      if(juice<100)return 2;
      if(juice<200)return 4;
      if(juice<500)return 7;
      if(juice<1000)return 10;
      return 14;
   }
   else
   {
      return get_attribute(Skill::get_associated_attribute(skill),use_juice);
   }
}

std::string Skill::get_name(int skill_type)
{
   switch(skill_type)
   {
   
   case SKILL_HANDTOHAND:     return "Martial Arts";
   case SKILL_KNIFE:          return "Knife";
   case SKILL_SWORD:          return "Sword";
   case SKILL_THROWING:       return "Throwing";
   case SKILL_CLUB:           return "Club";
   case SKILL_AXE:            return "Axe";
   case SKILL_PISTOL:         return "Pistol";
   case SKILL_RIFLE:          return "Rifle";
   case SKILL_FLAMETHROWER:   return "Flamethrower";
   case SKILL_SHOTGUN:        return "Shotgun";
   case SKILL_SMG:            return "SMG";
   case SKILL_PERSUASION:     return "Persuasion";
   case SKILL_PSYCHOLOGY:     return "Psychology";
   case SKILL_SECURITY:       return "Security";
   case SKILL_DISGUISE:       return "Disguise";
   case SKILL_COMPUTERS:      return "Computers";
   case SKILL_LAW:            return "Law";
   case SKILL_TAILORING:      return "Tailoring";
   case SKILL_DRIVING:        return "Driving";
   case SKILL_WRITING:        return "Writing";
   case SKILL_COOKING:        return "Cooking";
   case SKILL_MUSIC:          return "Music";
   case SKILL_ART:            return "Art";
   case SKILL_RELIGION:       return "Religion";
   case SKILL_SCIENCE:        return "Science";
   case SKILL_BUSINESS:       return "Business";
   //case SKILL_SURVIVAL:       return "Survival";
   case SKILL_STEALTH:        return "Stealth";
   //case SKILL_THEFT:          return "Theft";
   case SKILL_TEACHING:       return "Teaching";
   case SKILL_STREETSENSE:    return "Street Sense";
   case SKILL_SEDUCTION:      return "Seduction";
   case SKILL_LEADERSHIP:     return "Leadership";
   case SKILL_FIRSTAID:       return "First Aid";
   case SKILL_DODGE:          return "Dodge";
   }
   return "Error Skill Name";
}

int Skill::get_attribute()
{
   return associated_attribute;
}

std::string Attribute::get_name(int attribute_type)
{
   switch(attribute_type)
   {
   case ATTRIBUTE_STRENGTH:      return "STR";
   case ATTRIBUTE_AGILITY:       return "AGI";
   case ATTRIBUTE_WISDOM:        return "WIS";
   case ATTRIBUTE_INTELLIGENCE:  return "INT";
   case ATTRIBUTE_HEART:         return "HRT";
   case ATTRIBUTE_HEALTH:        return "HLTH";
   case ATTRIBUTE_CHARISMA:      return "CHA";
   }
   return "Error Attribute Name";
}

Creature::Creature()
{
   creatureinit();
}

Creature::~Creature()
{
   if(prisoner!=NULL)
   {
      int p;
      for(p=0;p<pool.size();p++)
      {
         if(prisoner==pool[p])
            break;
      }
      if(p != pool.size())
         delete prisoner;
   }
   // Clean up hostage situation
   stop_hauling_me();
}

void Creature::creatureinit(void)
{
   dontname=false;
   hireid=-1;
   worklocation=0;
   juice=0;
   flag=0;
   age=18+LCSrandom(40);
   gender_liberal = gender_conservative = LCSrandom(2) + 1;
   birthday_month=LCSrandom(12)+1;
   if(birthday_month==4 || birthday_month==6 ||
      birthday_month==9 || birthday_month==11)
   {
      birthday_day=LCSrandom(30)+1;
   }
   else if(birthday_month==2)
   {
      birthday_day=LCSrandom(28)+1;
   }
   else
   {
      birthday_day=LCSrandom(31)+1;
   }
   carid=-1;
   is_driver=0;
   pref_carid=-1;
   pref_is_driver=0;
   id=curcreatureid;
      curcreatureid++;
   joindays=0;
   deathdays=0;
   squadid=-1;
   cantbluff=0;
   location=0;
   base=0;
   activity.type=ACTIVITY_NONE;
   for(int i=0;i<LAWFLAGNUM;i++)
      crimes_suspected[i]=0;
   heat=0;
   confessions=0;
   clinic=0;
   dating=0;
   hiding=0;
   trainingtime=0;
   trainingsubject=-1;
   specialattack=-1;
   animalgloss=0;
   prisoner=NULL;
   alive=1;
   blood=100;
   stunned=0;
   for(int w=0;w<BODYPARTNUM;w++)wound[w]=0;
   weapon.type=WEAPON_NONE;
   weapon.ammo=0;
   armor.type=ARMOR_CLOTHES;
   armor.quality='1';
   armor.flag=0;
   for(int a=0;a<ATTNUM;a++)
   {
      //attributes[a].set_type(a);
      attributes[a].value=1;
   }
   int attnum=32;
   while(attnum>0)
   {
      int a=LCSrandom(ATTNUM);
      if(attributes[a].value<10)
      {
         attributes[a].value++;
         attnum--;
      }
   }
   for(int s=0;s<SKILLNUM;s++)
   {
      skills[s].set_type(s);
      skills[s].value=0;
      skill_experience[s]=0;
   }
   for(int c=0;c<CLIPNUM;c++)clip[c]=0;
   special[SPECIALWOUND_TEETH]=TOOTHNUM;
   special[SPECIALWOUND_RIGHTEYE]=1;
   special[SPECIALWOUND_LEFTEYE]=1;
   special[SPECIALWOUND_NOSE]=1;
   special[SPECIALWOUND_TONGUE]=1;
   special[SPECIALWOUND_RIGHTLUNG]=1;
   special[SPECIALWOUND_LEFTLUNG]=1;
   special[SPECIALWOUND_HEART]=1;
   special[SPECIALWOUND_LIVER]=1;
   special[SPECIALWOUND_STOMACH]=1;
   special[SPECIALWOUND_RIGHTKIDNEY]=1;
   special[SPECIALWOUND_LEFTKIDNEY]=1;
   special[SPECIALWOUND_SPLEEN]=1;
   special[SPECIALWOUND_RIBS]=RIBNUM;
   special[SPECIALWOUND_NECK]=1;
   special[SPECIALWOUND_UPPERSPINE]=1;
   special[SPECIALWOUND_LOWERSPINE]=1;
   forceinc=0;
   sentence=0;
   deathpenalty=0;
}

void Creature::set_attribute(int attribute, int amount)
{
   attributes[attribute].value=amount;
}
void Creature::set_skill(int skill, int amount) { skills[skill].value=amount; }
int Creature::get_skill(int skill) const { return skills[skill].value; }

void Creature::adjust_attribute(int attribute, int amount)
{
   attributes[attribute].value += amount;
}

int Creature::get_attribute(int attribute, bool usejuice) const
{
   int ret=attributes[attribute].value;
   
   if(attribute==ATTRIBUTE_WISDOM && align!=ALIGN_CONSERVATIVE)usejuice=false;
   if(attribute==ATTRIBUTE_HEART  && align!=ALIGN_LIBERAL)usejuice=false;

   if(!usejuice) return ret;

   // Effects of juice on the character's attributes
   if(usejuice)
   {
      if(juice<=-50)ret=1; // Damn worthless
      else if(juice<=-10)ret=static_cast<int>(ret*0.6); // Society's dregs
      else if(juice<0)ret=static_cast<int>(ret*0.8);    // Punk
      else if(juice>=10)
      {
         if(juice<50)ret=static_cast<int>(ret+=1); // Activist
         else if(juice<100)ret=static_cast<int>(ret*1.1+2); // Socialist Threat
         else if(juice<200)ret=static_cast<int>(ret*1.2+3); // Revolutionary
         else if(juice<500)ret=static_cast<int>(ret*1.3+4); // Urban Guerrilla
         else if(juice<1000)ret=static_cast<int>(ret*1.4+5); // Liberal Guardian
         else ret=static_cast<int>(ret*1.5+6); // Elite Liberal
      }
   }

   // How disfigured does the character's face look?
   // This is important for Charisma
   long disfigs=0;
   if(special[SPECIALWOUND_TEETH]<TOOTHNUM)disfigs++;
   if(special[SPECIALWOUND_TEETH]<TOOTHNUM/2)disfigs++;
   if(special[SPECIALWOUND_TEETH]==0)disfigs++;
   if(special[SPECIALWOUND_RIGHTEYE]==0)disfigs+=2;
   if(special[SPECIALWOUND_LEFTEYE]==0)disfigs+=2;
   if(special[SPECIALWOUND_TONGUE]==0)disfigs+=3;
   if(special[SPECIALWOUND_NOSE]==0)disfigs+=3;

   // Does the character have both legs intact?
   // Even one leg?
   // Important for Agility
   int legok=2;
   if((wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
       (wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
   if((wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
       (wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;

   // Special modifications to attributes based on circumstances,
   // especially aging effects and unusual injuries like paralysis
   switch(attribute)
   {
   case ATTRIBUTE_STRENGTH:
      if(special[SPECIALWOUND_NECK]!=1||
         special[SPECIALWOUND_UPPERSPINE]!=1)
      {
         ret=1;
      }
      else if(special[SPECIALWOUND_LOWERSPINE]!=1)ret>>=2;

      if(age<11)ret>>=1;     // Strength is lowest at the beginning and end of life
      else if(age<16)ret-=1;
      else if(age>35)ret-=1;
      else if(age>52)ret-=3;
      else if(age>70)ret-=6;
      break;
   case ATTRIBUTE_AGILITY:
      if(special[SPECIALWOUND_NECK]!=1||
         special[SPECIALWOUND_UPPERSPINE]!=1)
      {
         ret=1;
      }
      else if(special[SPECIALWOUND_LOWERSPINE]!=1)ret>>=2;
      else if(legok==0)ret>>=2;
      else if(legok==1)ret>>=1;

      if(age>35)ret-=1;      // Agility is weakened with age
      else if(age>52)ret-=3;
      else if(age>70)ret-=6;
      break;
   case ATTRIBUTE_HEALTH:
      if(special[SPECIALWOUND_NECK]!=1||
         special[SPECIALWOUND_UPPERSPINE]!=1)
      {
         ret=1;
      }
      else if(special[SPECIALWOUND_LOWERSPINE]!=1)ret>>=2;

      if(age<11)ret-=2;
      else if(age<16)ret-=1; // Physical immaturity weakens health
                             // Aging actually damages base health and eventually kills, so no aging effects here
      break;
   case ATTRIBUTE_CHARISMA:
      ret-=disfigs;

      if(age<11)ret+=2;      // Lots of folks like kids
      else if(age<16)ret-=1; // Teenagers have communication difficulties and image issues
      else if(age>35)ret+=1; // Authority and experience in life then enhance Charisma with age
      else if(age>52)ret+=2;
      else if(age>70)ret+=3;
      break;
   case ATTRIBUTE_INTELLIGENCE:
      if(age<11)ret-=3;      // Experience enhances Intelligence with age
      else if(age<16)ret-=1;
      else if(age>35)ret+=1;
      else if(age>52)ret+=2;
      else if(age>70)ret+=3;
      break;
   case ATTRIBUTE_WISDOM:
      if(age<11)ret-=2;      // Experience grants Wisdom with age
      else if(age<16)ret-=1;
      else if(age>52)ret+=1;
      else if(age>70)ret+=2;
      break;
   case ATTRIBUTE_HEART:
      if(age<11)ret+=2;      // Experience saps Heart with age due to cynicism
      else if(age<16)ret+=1; // No wonder it's typically the young who are most Liberal...
      else if(age>52)ret-=1;
      else if(age>70)ret-=2;
      break;
   }

   // Debilitations for temporary injuries in attributes based
   // on physical appearance or performance, because people who
   // are bleeding all over are less strong, agile, and charismatic
   if(attribute==ATTRIBUTE_STRENGTH||
      attribute==ATTRIBUTE_AGILITY||
      attribute==ATTRIBUTE_CHARISMA)
   {
      if(blood<=20)ret>>=2;
      else if(blood<=50){ret>>=1;}
      else if(blood<=75){ret*=3;ret>>=1;}
   }

   // Bounds check attributes
   if(ret<1)ret=1;
   if(ret>20)ret=20;

   return ret;
}

int Creature::roll_check(int skill)
{
   // This die rolling system (and the associated difficulty
   // ratings) is adapted from EABA, which uses a system of
   // rolling a number of six-sided dice equal to the ability
   // score divided by three. The top three dice are used,
   // the rest discarded. Finally, any additional points that
   // didn't divide evenly into creating a die contribute to
   // odd-shaped dice that don't exist in the real world. This
   // system gives diminishing returns for high skill levels.
   // EABA actually just adds the remainder to the die total,
   // but there are some statistical problems with that system.

   // It is not possible to roll above an 18 using this system.

   // It is possible to roll below a 3, if you don't have at
   // least 9 skill.

   int dice    = skill/3;
   int total   = 0;
   int roll[3] = {0,0,0};

   for(int i=0;i<dice+1;i++)
   {
      int newroll = 0;

      // Roll d6 for every three skill
      if(i<dice)
         newroll = LCSrandom(6)+1;
      // Partial dice for the remainder -- d3 for +1, d5 for +2
      else if(skill%3)
         newroll = LCSrandom((skill%3)*2+1)+1;

      // Keep the top three dice
      if(i<3)
         roll[i]=newroll;
      else
      {
         for(int j=0;j<3;j++)
         {
            if(newroll>roll[j])
            {
               roll[j]=newroll;
               break;
            }
         }
      }
   }

   for(int i=0;i<3;i++)
      total += roll[i];

   return total;
}

int Creature::attribute_roll(int attribute)
{
   int return_value = roll_check(get_attribute(attribute,true));
   #ifdef SHOWMECHANICS
   {
      move(8,1);
      char str[10];
      addstr(" AttributeRoll(");
      addstr(Attribute::get_name(attribute).c_str());
      addstr(", Attribute Level ");
      itoa(get_attribute(attribute,true),str,10);
      addstr(str);
      addstr(", Outcome of ");
      itoa(return_value,str,10);
      addstr(str);
      addstr(")");
      getch();
   }
   #endif
   // Roll on the attribute value
   return return_value;
}

bool Creature::attribute_check(int attribute, int difficulty)
{
   #ifdef SHOWMECHANICS
   {
      move(8,1);
      char str[10];
      addstr(" AttributeCheck(");
      addstr(Attribute::get_name(attribute).c_str());
      if(difficulty<21)
      {
         addstr(", Difficulty ");
         itoa(difficulty,str,10);
         addstr(str);
      }
      else addstr(", IMPOSSIBLE");
      addstr(")");
      getch();
   }
   #endif
   if(attribute_roll(attribute) >= difficulty)
      return true;
   else
      return false;
}

int Creature::skill_roll(int skill)
{
   // Take skill strength
   int skill_value = skills[skill].value;
   // plus the skill's associate attribute
   int attribute_value = get_attribute(skills[skill].get_attribute(),true);
   // most attributes get quartered when applied to skills, capped by relative skill level...
   int adjusted_attribute_value = min(attribute_value / 2,(skill_value+1) * 3);
   // ...but some skills might support full attribute use
   if(skill == SKILL_DISGUISE)
      adjusted_attribute_value = attribute_value;
   // add the adjusted attribute and skill to get the adjusted skill total
   // that will be rolled on
   int return_value = roll_check(skill_value + adjusted_attribute_value);

   // Special skill handling
   switch(skill)
   {
   // Skills that cannot be used if zero skill:
   case SKILL_PSYCHOLOGY:
   case SKILL_LAW:
   case SKILL_SECURITY:
   case SKILL_COMPUTERS:
   case SKILL_MUSIC:
   case SKILL_ART:
   case SKILL_RELIGION:
   case SKILL_SCIENCE:
   case SKILL_BUSINESS:
   case SKILL_TEACHING:
   case SKILL_LEADERSHIP:
   case SKILL_FIRSTAID:
      if(skills[skill].value == 0)
      {
         return_value = 0; // Automatic failure
         break;
      }
      break;
   // Skills that should depend on clothing:
   case SKILL_STEALTH:
   case SKILL_SEDUCTION:
   case SKILL_PERSUASION:
      break;
   // Unique disguise handling
   case SKILL_DISGUISE:
      {
         // Check for appropriate uniform
         char uniformed = hasdisguise(*this);

         // Ununiformed disguise checks automatically fail
         if(!uniformed) { return_value = 0; break; }
         // reduce effectiveness for 'partial' uniforms (police uniforms when trespassing)
         else { if(uniformed==2) return_value>>=1; }

         // Bloody, damaged clothing hurts disguise check
         if(armor.flag & ARMORFLAG_BLOODY) { return_value>>=1; }
         if(armor.flag & ARMORFLAG_DAMAGED) { return_value>>=1; }

         // Carrying corpses or having hostages is very bad for disguise
         if(prisoner!=NULL) { return_value>>=2; break; }
      }
   }
   #ifdef SHOWMECHANICS
   {
      move(8,1);
      char str[10];
      addstr(" SkillRoll(");
      addstr(Skill::get_name(skill).c_str());
      addstr(", Skill Value ");
      itoa(skills[skill].value,str,10);
      addstr(str);
      addstr(", ");
      if(return_value==0)
      {
         addstr("automatic failure");
      }
      else
      {
         addstr("Adjusted Attribute Value ");
         itoa(adjusted_attribute_value,str,10);
         addstr(str);
         addstr(", Outcome of ");
         itoa(return_value,str,10);
         addstr(str);
      }
      addstr(")");
      getch();
   }
   #endif
   return return_value;
}

bool Creature::skill_check(int skill, int difficulty)
{
   #ifdef SHOWMECHANICS
   {
      move(8,1);
      char str[10];
      addstr(" SkillCheck(");
      addstr(Skill::get_name(skill).c_str());
      if(difficulty<21)
      {
         addstr(", Difficulty ");
         itoa(difficulty,str,10);
         addstr(str);
      }
      else addstr(", IMPOSSIBLE");
      addstr(")");
      getch();
   }
   #endif
   if(skill_roll(skill) >= difficulty)
      return true;
   else
      return false;
}

void Creature::stop_hauling_me()
{
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->prisoner == this)
      {
         pool[p]->prisoner = NULL;
      }
   }
}

void Creature::train(int trainedskill, int experience, int upto)
{
   // Do we allow animals to gain skills? Right now, yes
   //if(animalgloss==ANIMALGLOSS_ANIMAL)return;

   // Don't give experience if already maxed out
   if(skill_cap(trainedskill,true)<=skills[trainedskill].value || upto<=skills[trainedskill].value)
      return;
   // Don't give experience if requested to give none
   if(experience==0)
      return;
   // Skill gain scaled by ability in the area
   skill_experience[trainedskill]+=max(1,static_cast<int>(experience * skill_cap(trainedskill,false) / 6.0));

   int abovenextlevel;
   // only allow gaining experience on the new level if it doesn't put us over a level limit
   if (skills[trainedskill].value >= (upto - 1) || 
       skills[trainedskill].value >= (skill_cap(trainedskill,true) - 1))
     abovenextlevel = 0;
   else
     abovenextlevel = 50 + 5*(1+skills[trainedskill].value); // enough skill points to get halfway through the next skill level

   skill_experience[trainedskill] = min(skill_experience[trainedskill], 100 + 10*skills[trainedskill].value + abovenextlevel);

}

void Creature::train(int trainedskill, int experience)
{
   return this->train (trainedskill, experience, 20);
}

void Creature::skill_up()
{
   for(int s=0;s<SKILLNUM;s++)
   {
      while(skill_experience[s]>=100+10*skills[s].value&&
            skills[s].value<skill_cap(s,true))
      {
         skill_experience[s]-=100+10*skills[s].value;
         skills[s].value++;
      }
      if(skills[s].value==skill_cap(s,true))
         skill_experience[s]=0;
   }
}

int Creature::get_skill_ip(int skill)
{
   return skill_experience[skill];
}

bool Creature::enemy()
{
   if(align==ALIGN_CONSERVATIVE)
   {
      return true;
   }
   else
   {
      if(type==CREATURE_COP && align==ALIGN_MODERATE)
      {
         for(int i=0;i<pool.size();i++)
         {
            if(pool[i]==this)
            {
               return false;
            }
         }
         return true;
      }
   }
   return false;
}


/* turns a creature into a conservative */
void conservatise(Creature &cr)
{
   if(cr.align==ALIGN_LIBERAL && cr.juice>0)cr.juice=0;
   
   cr.align=ALIGN_CONSERVATIVE;

   switch(cr.type)
   {
      case CREATURE_WORKER_FACTORY_UNION:
         strcpy(cr.name,"Ex-Union Worker");
         break;
      case CREATURE_JUDGE_LIBERAL:
         strcpy(cr.name,"Jaded Liberal Judge");
         break;
   }
}



/* turns a creature into a liberal */
void liberalize(Creature &cr,bool rename)
{
   if(cr.align==ALIGN_CONSERVATIVE && cr.juice>0)cr.juice=0;

   cr.align=ALIGN_LIBERAL;

   if(cr.type == CREATURE_CORPORATE_CEO)
      uniqueCreatures.CEO_state = UNIQUECREATURE_LIBERAL;

   if(rename)
   {
      switch(cr.type)
      {
         case CREATURE_WORKER_FACTORY_NONUNION:
            strcpy(cr.name,"New Union Worker");
            break;
//       case CREATURE_JUDGE_CONSERVATIVE:
//          strcpy(cr.name,"Enlightened Judge");
//          break;
      }
   }
}

/* gives a CCS member a cover name */
void nameCCSMember(Creature &cr)
{
   if(cr.armor.type==ARMOR_CIVILLIANARMOR)
   {
      strcpy(cr.name,"Mercenary");
   }
   else if(cr.armor.type==ARMOR_ARMYARMOR)
   {
      strcpy(cr.name,"Soldier");
   }
   else if(cr.armor.type==ARMOR_HEAVYARMOR)
   {
      strcpy(cr.name,"Hardened Veteran");
   }
   else if(cr.weapon.type==WEAPON_SHOTGUN_PUMP||LCSrandom(2))
   {
      switch(LCSrandom(5))
      {
         case 0:strcpy(cr.name,"Country Boy");break;
         case 1:strcpy(cr.name,"Hick");break;
         case 2:strcpy(cr.name,"Redneck");break;
         case 3:strcpy(cr.name,"Rube");break;
         case 4:strcpy(cr.name,"Yokel");break;
      }
   }
   else
   {
      switch(LCSrandom(9))
      {
      case 0: strcpy(cr.name,"Biker");break;
      case 1: strcpy(cr.name,"Transient");break;
      case 2: strcpy(cr.name,"Crackhead");break;
      case 3: strcpy(cr.name,"Fast Food Worker");break;
      case 4: strcpy(cr.name,"Telemarketer");break;
      case 5: strcpy(cr.name,"Office Worker");break;
      case 6: strcpy(cr.name,"Mailman");break;
      case 7: strcpy(cr.name,"Musician");break;
      case 8: strcpy(cr.name,"Hairstylist");break;
      }
   }
}

/* are they interested in talking about the issues? */
bool Creature::talkreceptive()
{
   if(enemy())return false;

   switch(type)
   {
      case CREATURE_WORKER_SERVANT:
      case CREATURE_WORKER_JANITOR:
      case CREATURE_WORKER_SWEATSHOP:
      case CREATURE_WORKER_FACTORY_CHILD:
      case CREATURE_TEENAGER:
      case CREATURE_SEWERWORKER:
      case CREATURE_COLLEGESTUDENT:
      case CREATURE_MUSICIAN:
      case CREATURE_MATHEMATICIAN:
      case CREATURE_TEACHER:
      case CREATURE_HSDROPOUT:
      case CREATURE_BUM:
      case CREATURE_POLITICALACTIVIST:
      case CREATURE_GANGMEMBER:
      case CREATURE_CRACKHEAD:
      case CREATURE_FASTFOODWORKER:
      case CREATURE_TELEMARKETER:
      case CREATURE_PROSTITUTE:
      case CREATURE_GARBAGEMAN:
      case CREATURE_PLUMBER:
      case CREATURE_AMATEURMAGICIAN:
      case CREATURE_HIPPIE:
      case CREATURE_RETIREE:
      case CREATURE_HAIRSTYLIST:
      case CREATURE_CLERK:
      case CREATURE_MUTANT: // What? -- LK
         return true;
   }

   return false;
}

#define ABS(x) ((x)<0)?(-x):(x)

/* are the characters close enough in age to date? */
bool Creature::can_date(Creature &a)
{
   if(age<11 || a.age<11) return false;
   if(age<16 || a.age<16)
   {
      if(ABS(age-a.age)<5)
         return true;
      else
         return false;
   }
   else return true;
}

void Creature::die()
{
   alive = 0;
   blood = 0;
   if(type==CREATURE_CORPORATE_CEO)
      uniqueCreatures.CEO_state = UNIQUECREATURE_DEAD;
}

void UniqueCreatures::initialize()
{
   makecreature(CEO_,CREATURE_CORPORATE_CEO);
   CEO_ID = CEO_.id;
   CEO_state = UNIQUECREATURE_ALIVE;
}

Creature& UniqueCreatures::CEO()
{
   if(CEO_ID==-1)
      makecreature(CEO_,CREATURE_CORPORATE_CEO);
   return CEO_;
}

const char *Creature::heshe()
{
	if(gender_liberal == GENDER_MALE)
	{
		return "he";
	}
	if(gender_liberal == GENDER_FEMALE)
	{
		return "she";
	}
	return "they";
}