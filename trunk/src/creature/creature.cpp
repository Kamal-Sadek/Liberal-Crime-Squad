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

Skill::Skill(const char* inputXml)
{
   CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   xml.IntoElem();
   
   while (xml.FindElem())
   {
      std::string tag = xml.GetTagName();
      
      if (tag == "associated_attribute")
         associated_attribute = atoi(xml.GetData().c_str());
      else if (tag == "skill")
         skill = atoi(xml.GetData().c_str());
      else if (tag == "value")
         value = atoi(xml.GetData().c_str());
   }
}

string Skill::showXml() const
{
   CMarkup xml;
   xml.AddElem("skill");
   xml.IntoElem();
   xml.AddElem("associated_attribute", associated_attribute);
   xml.AddElem("skill", skill);
   xml.AddElem("value", value);
   
   return xml.GetDoc();
}

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
   case SKILL_HEAVYWEAPONS:
      return ATTRIBUTE_STRENGTH;
   case SKILL_HANDTOHAND:
   case SKILL_KNIFE:
   case SKILL_SWORD:
   case SKILL_PISTOL:
   case SKILL_RIFLE:
   case SKILL_SMG:
   case SKILL_SHOTGUN:
   case SKILL_DRIVING:
   case SKILL_STEALTH:
   case SKILL_THROWING:
   case SKILL_DODGE:
      return ATTRIBUTE_AGILITY;
   case SKILL_DISGUISE:
   case SKILL_SEDUCTION:
   case SKILL_PERSUASION:
      return ATTRIBUTE_CHARISMA;
   case SKILL_ART:
   case SKILL_MUSIC:
      return ATTRIBUTE_HEART;
   case SKILL_RELIGION:
   case SKILL_BUSINESS:
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
   return get_attribute(Skill::get_associated_attribute(skill),use_juice);
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
   case SKILL_HEAVYWEAPONS:   return "Heavy Weapons";
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
   case SKILL_MUSIC:          return "Music";
   case SKILL_ART:            return "Art";
   case SKILL_RELIGION:       return "Religion";
   case SKILL_SCIENCE:        return "Science";
   case SKILL_BUSINESS:       return "Business";
   case SKILL_STEALTH:        return "Stealth";
   case SKILL_TEACHING:       return "Teaching";
   case SKILL_STREETSENSE:    return "Street Sense";
   case SKILL_SEDUCTION:      return "Seduction";
   case SKILL_FIRSTAID:       return "First Aid";
   case SKILL_DODGE:          return "Dodge";
   }
   return "Error Skill Name";
}

int Skill::get_attribute()
{
   return associated_attribute;
}

Attribute::Attribute(const char* inputXml)
{
   CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   xml.IntoElem();
   
   while (xml.FindElem())
   {
      std::string tag = xml.GetTagName();
      
      if (tag == "attribute")
         attribute = atoi(xml.GetData().c_str());
      else if (tag == "value")
         value = atoi(xml.GetData().c_str());
   }
}

string Attribute::showXml() const
{
   CMarkup xml;
   xml.AddElem("attribute");
   xml.IntoElem();
   xml.AddElem("attribute", attribute);
   xml.AddElem("value", value);
   
   return xml.GetDoc();
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

Creature::Creature(const Creature& org)
{
   copy(org);
}

Creature& Creature::operator=(const Creature& rhs)
{
   if (this != &rhs)
   {
      delete weapon;
      delete armor;
      for (int i = 0; i < clips.size(); ++i)
         delete clips[i];
      clips.clear();
      for (int i = 0; i < extra_throwing_weapons.size(); ++i)
         delete extra_throwing_weapons[i];
      extra_throwing_weapons.clear();
      
      copy(rhs);
   }
   return *this;
}

void Creature::copy(const Creature& org)
{
   for (int i = 0; i < ATTNUM; ++i)
      attributes[i] = org.attributes[i];
   for (int i = 0; i < SKILLNUM; ++i)
   {
      skills[i] = org.skills[i];
      skill_experience[i] = org.skill_experience[i];
   }
   for (int i = 0; i < BODYPARTNUM; ++i)
      wound[i] = org.wound[i];
   for (int i = 0; i < SPECIALWOUNDNUM; ++i)
      special[i] = org.special[i];
   for (int i = 0; i < LAWFLAGNUM; ++i)
      crimes_suspected[i] = org.crimes_suspected[i];

   if (org.weapon != NULL)
      weapon = new Weapon(*org.weapon);
   else
      weapon = NULL;
   if (org.armor != NULL)
      armor = new Armor(*org.armor);
   else
      armor = NULL;
   for (unsigned i = 0; i < org.extra_throwing_weapons.size(); ++i)
      extra_throwing_weapons.push_back(new Weapon(*org.extra_throwing_weapons[i]));
   for (unsigned i = 0; i < org.clips.size(); ++i)
      clips.push_back(new Clip(*org.clips[i]));

   strcpy(name, org.name);
   strcpy(propername, org.propername);
      
   gender_conservative = org.gender_conservative;
   gender_liberal = org.gender_liberal;
   squadid = org.squadid;
   age = org.age;
   birthday_month = org.birthday_month;
   birthday_day = org.birthday_day;
   exists = org.exists;
   align = org.align;
   alive = org.alive;
   type = org.type;
   infiltration = org.infiltration;
   animalgloss = org.animalgloss;
   specialattack = org.specialattack;
   clinic = org.clinic;
   dating = org.dating;
   hiding = org.hiding;
   trainingtime = org.trainingtime;
   trainingsubject = org.trainingsubject;
   sentence = org.sentence;
   confessions = org.confessions;
   deathpenalty = org.deathpenalty;
   joindays = org.joindays;
   deathdays = org.deathdays;
   id = org.id;
   hireid = org.hireid;
   meetings = org.meetings;
   forceinc = org.forceinc;
   stunned = org.stunned;
   has_thrown_weapon = org.has_thrown_weapon;
   money = org.money;
   juice = org.juice;
   income = org.income;
   blood = org.blood;
   heat = org.heat;
   location = org.location;
   worklocation = org.worklocation;
   cantbluff = org.cantbluff;
   base = org.base;
   activity = org.activity;
   carid = org.carid;
   is_driver = org.is_driver;
   pref_carid = org.pref_carid;
   pref_is_driver = org.pref_is_driver;
   flag = org.flag;
   dontname = org.dontname;
   prisoner = NULL; //Not copying prisoner.
}

Creature::~Creature()
{
   delete weapon;
   delete armor;
   while (!clips.empty())
   {
      delete clips.back();
      clips.pop_back();
   }
   while (!extra_throwing_weapons.empty())
   {
      delete extra_throwing_weapons.back();
      extra_throwing_weapons.pop_back();
   }
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
   weapon=NULL;
   has_thrown_weapon = false;
   armor=NULL;//new Armor(*armortype[getarmortype("ARMOR_CLOTHES")]); //Causes crash for global uniqueCreature -XML
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

Creature::Creature(const char* inputXml)
 : armor(NULL), weapon(NULL), prisoner(NULL)
{
   CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   xml.IntoElem();
   
   int attributesi = 0;
   int skillsi = 0;
   int skill_experiencei = 0;
   int woundi = 0;
   int speciali = 0;
   int crimesi = 0;
   while (xml.FindElem())
   {
      std::string tag = xml.GetTagName();
   
      if (tag == "attribute" && attributesi < ATTNUM)
         attributes[attributesi++] = Attribute(xml.GetSubDoc().c_str());
      else if (tag == "skill" && skillsi < SKILLNUM)
         skills[skillsi++] = Skill(xml.GetSubDoc().c_str());
      else if (tag == "skill_experience" && skill_experiencei < SKILLNUM)
         skill_experience[skill_experiencei++] = atoi(xml.GetData().c_str());
      else if (tag == "weapon")
      {
         Weapon w(xml.GetSubDoc().c_str());
         if (getweapontype(w.get_itemtypename()) != -1) //Check weapon is a valid type.
         {
            give_weapon(w,NULL);
         }
      }
      else if (tag == "armor")
      {
         armor = new Armor(xml.GetSubDoc().c_str());
         if (getarmortype(armor->get_itemtypename()) == -1) //Check armor is a valid type.
         {
            delete armor;
            armor = NULL;
         }
      }
      else if (tag == "name")
         strcpy(name,xml.GetData().c_str());
      else if (tag == "propername")
         strcpy(propername,xml.GetData().c_str());
      else if (tag == "gender_conservative")
         gender_conservative = atoi(xml.GetData().c_str());
      else if (tag == "gender_liberal")
         gender_liberal = atoi(xml.GetData().c_str());
      else if (tag == "squadid")
         squadid = atoi(xml.GetData().c_str());
      else if (tag == "age")
         age = atoi(xml.GetData().c_str());
      else if (tag == "birthday_month")
         birthday_month = atoi(xml.GetData().c_str());
      else if (tag == "birthday_day")
         birthday_day = atoi(xml.GetData().c_str());
      else if (tag == "exists")
         exists = atoi(xml.GetData().c_str());
      else if (tag == "align")
         align = atoi(xml.GetData().c_str());
      else if (tag == "alive")
         alive = atoi(xml.GetData().c_str());
      else if (tag == "type")
         type = atoi(xml.GetData().c_str());
      else if (tag == "infiltration")
         infiltration = atof(xml.GetData().c_str());
      else if (tag == "animalgloss")
         animalgloss = atoi(xml.GetData().c_str());
      else if (tag == "specialattack")
         specialattack = atoi(xml.GetData().c_str());
      else if (tag == "clinic")
         clinic = atoi(xml.GetData().c_str());
      else if (tag == "dating")
         dating = atoi(xml.GetData().c_str());
      else if (tag == "hiding")
         hiding = atoi(xml.GetData().c_str());
      else if (tag == "trainingtime")
         trainingtime = atoi(xml.GetData().c_str());
      else if (tag == "trainingsubject")
         trainingsubject = atoi(xml.GetData().c_str());
      else if (tag == "prisoner")
      {
         xml.IntoElem();
         prisoner = new Creature(xml.GetSubDoc().c_str());
         xml.OutOfElem();
      }
      else if (tag == "sentence")
         sentence = atoi(xml.GetData().c_str());
      else if (tag == "confessions")
         confessions = atoi(xml.GetData().c_str());
      else if (tag == "deathpenalty")
         deathpenalty = atoi(xml.GetData().c_str());
      else if (tag == "joindays")
         joindays = atoi(xml.GetData().c_str());
      else if (tag == "deathdays")
         deathdays = atoi(xml.GetData().c_str());
      else if (tag == "id")
         id = atoi(xml.GetData().c_str());
      else if (tag == "hireid")
         hireid = atoi(xml.GetData().c_str());
      else if (tag == "meetings")
         meetings = atoi(xml.GetData().c_str());
      else if (tag == "forceinc")
         forceinc = atoi(xml.GetData().c_str());
      else if (tag == "stunned")
         stunned = atoi(xml.GetData().c_str());
      else if (tag == "clip")
      {
         Clip* c = new Clip(xml.GetSubDoc().c_str());
         if (getcliptype(c->get_itemtypename()) != -1)
            clips.push_back(c);
         else
            delete c;
      }
      else if (tag == "has_thrown_weapon")
         has_thrown_weapon = atoi(xml.GetData().c_str());
      else if (tag == "money")
         money = atoi(xml.GetData().c_str());
      else if (tag == "juice")
         juice = atoi(xml.GetData().c_str());
      else if (tag == "income")
         income = atoi(xml.GetData().c_str());
      else if (tag == "wound" && woundi < BODYPARTNUM)
         wound[woundi++] = atoi(xml.GetData().c_str());
      else if (tag == "blood")
         blood = atoi(xml.GetData().c_str());
      else if (tag == "special" && speciali < SPECIALWOUNDNUM)
         special[speciali++] = atoi(xml.GetData().c_str());
      else if (tag == "crimes_suspected" && crimesi < LAWFLAGNUM)
         crimes_suspected[crimesi++] = atoi(xml.GetData().c_str());
      else if (tag == "heat")
         heat = atoi(xml.GetData().c_str());
      else if (tag == "location")
         location = atoi(xml.GetData().c_str());
      else if (tag == "worklocation")
         worklocation = atoi(xml.GetData().c_str());
      else if (tag == "cantbluff")
         cantbluff = atoi(xml.GetData().c_str());
      else if (tag == "base")
         base = atoi(xml.GetData().c_str());
      else if (tag == "activity")
      {
         xml.IntoElem();
         while (xml.FindElem())
         {
            tag = xml.GetTagName();
            if (tag == "type")
               activity.type = atoi(xml.GetData().c_str());
            else if (tag == "arg")
               activity.arg = atoi(xml.GetData().c_str());
            else if (tag == "arg2")
               activity.arg2 = atoi(xml.GetData().c_str());
         }
         xml.OutOfElem();
      }
      else if (tag == "carid")
         carid = atoi(xml.GetData().c_str());
      else if (tag == "is_driver")
         is_driver = atoi(xml.GetData().c_str());
      else if (tag == "pref_carid")
         pref_carid = atoi(xml.GetData().c_str());
      else if (tag == "pref_is_driver")
         pref_is_driver = atoi(xml.GetData().c_str());
      else if (tag == "flag")
         flag = atoi(xml.GetData().c_str());
      else if (tag == "dontname")
         dontname = atoi(xml.GetData().c_str());
   }
}

string Creature::showXml() const
{
   CMarkup xml;
   xml.AddElem("creature");
   xml.IntoElem();

   for (int i = 0; i < ATTNUM; ++i)
      xml.AddSubDoc(attributes[i].showXml());
   for (int i = 0; i < SKILLNUM; ++i)
      xml.AddSubDoc(skills[i].showXml());
   for (int i = 0; i < SKILLNUM; ++i)
      xml.AddElem("skill_experience", skill_experience[i]); //Bad, relies on their order in the xml file. -XML
   if (weapon != NULL)
      xml.AddSubDoc(weapon->showXml());
   if (armor != NULL)
      xml.AddSubDoc(armor->showXml());
   
   xml.AddElem("name", name);//string(name)?
   xml.AddElem("propername", propername);//string(propername)?
   xml.AddElem("gender_conservative", gender_conservative);
   xml.AddElem("gender_liberal", gender_liberal);
   xml.AddElem("squadid", squadid);
   xml.AddElem("age", age);
   xml.AddElem("birthday_month", birthday_month);
   xml.AddElem("birthday_day", birthday_day);
   xml.AddElem("exists", exists);
   xml.AddElem("align", align);
   xml.AddElem("alive", alive);
   xml.AddElem("type", type);

   char buf[256];
   snprintf (buf, 255, "%f", infiltration);
   xml.AddElem("infiltration", buf);

   xml.AddElem("animalgloss", animalgloss);
   xml.AddElem("specialattack", specialattack);
   xml.AddElem("clinic", clinic);
   xml.AddElem("dating", dating);
   xml.AddElem("hiding", hiding);
   xml.AddElem("trainingtime", trainingtime);
   xml.AddElem("trainingsubject", trainingsubject);
   if (prisoner != NULL) //Should never be true when saving.
   {
      xml.AddElem("prisoner");
      xml.IntoElem();
      xml.AddSubDoc(prisoner->showXml());
      xml.OutOfElem();
   }
   xml.AddElem("sentence", sentence);
   xml.AddElem("confessions", confessions);
   xml.AddElem("deathpenalty", deathpenalty);
   xml.AddElem("joindays", joindays);
   xml.AddElem("deathdays", deathdays);
   xml.AddElem("id", id);
   xml.AddElem("hireid", hireid);
   xml.AddElem("meetings", meetings);
   xml.AddElem("forceinc", forceinc);
   xml.AddElem("stunned", stunned);
   for (int i = 0; i < extra_throwing_weapons.size(); ++i)
   {
      xml.AddSubDoc(extra_throwing_weapons[i]->showXml());
   }
   for (int i = 0; i < clips.size(); ++i)
   {
      xml.AddSubDoc(clips[i]->showXml());
   }
   xml.AddElem("has_thrown_weapon", has_thrown_weapon);
   xml.AddElem("money", money);
   xml.AddElem("juice", juice);
   xml.AddElem("income", income);
   for (int i = 0; i < BODYPARTNUM; ++i) //Bad, relies on their order in the xml file. -XML
      xml.AddElem("wound",wound[i]);
   xml.AddElem("blood", blood);
   for (int i = 0; i < SPECIALWOUNDNUM; ++i) //Bad, relies on their order in the xml file. -XML
      xml.AddElem("special",special[i]);
   for (int i = 0; i < LAWFLAGNUM; ++i) //Bad, relies on their order in the xml file. -XML
      xml.AddElem("crimes_suspected",crimes_suspected[i]);
   xml.AddElem("heat", heat);
   xml.AddElem("location", location);
   xml.AddElem("worklocation", worklocation);
   xml.AddElem("cantbluff", cantbluff);
   xml.AddElem("base", base);
   
   xml.AddElem("activity");
   xml.IntoElem();
   xml.AddElem("type",activity.type);
   xml.AddElem("arg",activity.arg);
   xml.AddElem("arg2",activity.arg2);
   xml.OutOfElem();
   
   xml.AddElem("carid", carid);
   xml.AddElem("is_driver", is_driver);
   xml.AddElem("pref_carid", pref_carid);
   xml.AddElem("pref_is_driver", pref_is_driver);
   xml.AddElem("flag", flag);
   xml.AddElem("dontname", dontname);

   return xml.GetDoc();
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

   // Special modifications to attributes based on age
   switch(attribute)
   {
   case ATTRIBUTE_STRENGTH:
      if(age<11)ret>>=1;     // Strength is lowest at the beginning and end of life
      else if(age<16)ret-=1;
      else if(age>70)ret-=6;
      else if(age>52)ret-=3;
      else if(age>35)ret-=1;
      break;
   case ATTRIBUTE_AGILITY:
      if(age>70)ret-=6;      // Agility is weakened with age
      else if(age>52)ret-=3;
      else if(age>35)ret-=1;
      break;
   case ATTRIBUTE_HEALTH:
      if(age<11)ret-=2;
      else if(age<16)ret-=1; // Physical immaturity weakens health
                             // Aging actually damages base health and eventually kills, so no aging effects here
      break;
   case ATTRIBUTE_CHARISMA:
      if(age<11)ret+=2;      // Lots of folks like kids
      else if(age<16)ret-=1; // Teenagers have communication difficulties and image issues
      else if(age>70)ret+=3; // Authority and experience in life then enhance Charisma with age
      else if(age>52)ret+=2;
      else if(age>35)ret+=1;
      break;
   case ATTRIBUTE_INTELLIGENCE:
      if(age<11)ret-=3;      // Experience enhances Intelligence with age
      else if(age<16)ret-=1;
      else if(age>70)ret+=3;
      else if(age>52)ret+=2;
      else if(age>35)ret+=1;
      break;
   case ATTRIBUTE_WISDOM:
      if(age<11)ret-=2;      // Experience grants Wisdom with age
      else if(age<16)ret-=1;
      else if(age>70)ret+=2;
      else if(age>52)ret+=1;
      break;
   case ATTRIBUTE_HEART:
      if(age<11)ret+=2;      // Experience saps Heart with age due to cynicism
      else if(age<16)ret+=1; // No wonder it's typically the young who are most Liberal...
      else if(age>70)ret-=2;
      else if(age>52)ret-=1;
      break;
   }

   // Physical stats want to know: Are you paralyzed?
   if(attribute==ATTRIBUTE_STRENGTH||
      attribute==ATTRIBUTE_AGILITY||
      attribute==ATTRIBUTE_HEALTH)
   {
      if(special[SPECIALWOUND_NECK]!=1||
         special[SPECIALWOUND_UPPERSPINE]!=1)
      {
         ret=1;
      }
      else if(special[SPECIALWOUND_LOWERSPINE]!=1)ret>>=2;
   }

   // Agility wants to know: Do you have legs?
   if(attribute==ATTRIBUTE_AGILITY)
   {
      int legok=2;
      if((wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
          (wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
      if((wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
          (wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;

      if(legok==0)ret>>=2;
      else if(legok==1)ret>>=1;
   }

   // Charisma wants to know: How fucked up does your face look?
   if(attribute==ATTRIBUTE_CHARISMA)
   {
      long disfigs=0;
      if(special[SPECIALWOUND_TEETH]<TOOTHNUM)disfigs++;
      if(special[SPECIALWOUND_TEETH]<TOOTHNUM/2)disfigs++;
      if(special[SPECIALWOUND_TEETH]==0)disfigs++;
      if(special[SPECIALWOUND_RIGHTEYE]==0)disfigs+=2;
      if(special[SPECIALWOUND_LEFTEYE]==0)disfigs+=2;
      if(special[SPECIALWOUND_TONGUE]==0)disfigs+=3;
      if(special[SPECIALWOUND_NOSE]==0)disfigs+=3;

      ret-=disfigs;
   }

   // Never use juice to increase stats for the opposite ideology!
   if(attribute==ATTRIBUTE_WISDOM && align!=ALIGN_CONSERVATIVE)usejuice=false;
   if(attribute==ATTRIBUTE_HEART  && align!=ALIGN_LIBERAL)usejuice=false;

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
      
      // Debilitations for temporary injuries in attributes based
      // on physical appearance or performance, because people who
      // are bleeding all over are less strong, agile, and charismatic
      if(attribute==ATTRIBUTE_STRENGTH||
         attribute==ATTRIBUTE_AGILITY||
         attribute==ATTRIBUTE_CHARISMA)
      {
         if(blood<=20)ret>>=2;
         else if(blood<=50){ret>>=1;}
         else if(blood<=75){ret*=3;ret>>=2;}
      }
   }

   // Bounds check attributes
   if(ret<1)ret=1;
   //if(ret>20)ret=20;

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
   
   int adjusted_attribute_value;
   switch(skill)
   {
   // most attributes get halved when applied to skills, capped by relative skill level...
   default:
      adjusted_attribute_value = min(attribute_value/2, skill_value+3);
      break;
   // ...and some may be so specialized that they ignore attributes, instead counting skill double
   case SKILL_SECURITY:
      adjusted_attribute_value = skill_value;
      break;
   }
      
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
   case SKILL_FIRSTAID:
      if(skills[skill].value == 0)
      {
         return_value = 0; // Automatic failure
         break;
      }
      break;
   // Skills that should depend on clothing:
   case SKILL_STEALTH:
      {
         int stealth = get_armor().get_stealth_value();
         if(stealth == 0) return 0;

         return_value *= stealth;
         return_value /= 2;
         
      }
      break;
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
         if(get_armor().is_bloody()) { return_value>>=1; }
         if(get_armor().is_damaged()) { return_value>>=1; }

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
   if(cr.get_armor().get_itemtypename()=="ARMOR_CIVILLIANARMOR")
   {
      strcpy(cr.name,"Mercenary");
   }
   else if(cr.get_armor().get_itemtypename()=="ARMOR_ARMYARMOR")
   {
      strcpy(cr.name,"Soldier");
   }
   else if(cr.get_armor().get_itemtypename()=="ARMOR_HEAVYARMOR")
   {
      strcpy(cr.name,"Hardened Veteran");
   }
   else if(cr.get_weapon().get_itemtypename()=="WEAPON_SHOTGUN_PUMP"||LCSrandom(2))
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

UniqueCreatures::UniqueCreatures(const char * inputXml)
{
   CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   xml.IntoElem();

   while (xml.FindElem())
   {
      std::string tag = xml.GetTagName();
   
      if (tag == "CEO")
      {
         xml.IntoElem();
         
         xml.FindElem();
         //tag = xml.GetTagName();
         //if (tag == "creature")
         //{
            CEO_ = Creature(xml.GetSubDoc().c_str());
         //}
         
         xml.OutOfElem();
      }
      else if (tag == "CEO_ID")
      {
         CEO_ID = atoi(xml.GetData().c_str());
      }
      else if (tag == "CEO_state")
         CEO_state = atoi(xml.GetData().c_str());
   }
}

string UniqueCreatures::showXml() const
{
   CMarkup xml;
   xml.AddElem("uniquecreatures");
   xml.IntoElem();
   
   xml.AddElem("CEO_ID", CEO_ID);
   xml.AddElem("CEO_state", CEO_state);
   xml.AddElem("CEO");
   xml.IntoElem();
   xml.AddSubDoc(CEO_.showXml());
   
   return xml.GetDoc();
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

Weapon& Creature::weapon_none()
{
   static Weapon* unarmed = new Weapon(*weapontype[getweapontype("WEAPON_NONE")]);
   return *unarmed;
}

Armor& Creature::armor_none()
{
   static Armor* naked = new Armor(*armortype[getarmortype("ARMOR_NONE")]);
   return *naked;
}

Weapon& Creature::get_weapon()
{
   if (is_armed())
      return *weapon;
   else
      return weapon_none();
}
const Weapon& Creature::get_weapon() const
{
   if (is_armed())
      return *weapon;
   else
      return weapon_none();
}
Armor& Creature::get_armor()
{
   if (!is_naked())
      return *armor;
   else
      return armor_none();
}
const Armor& Creature::get_armor() const
{
   if (!is_naked())
      return *armor;
   else
      return armor_none();
}

bool Creature::will_do_ranged_attack(bool force_ranged,bool force_melee) const
{
   bool r;

   if (weapon != NULL) //Is the creature armed?
   {
      bool reload_allowed = can_reload();
      
      r = weapon->get_attack(force_ranged,force_melee,reload_allowed) != NULL //Any attacks possible under circumstances?
          && weapon->get_attack(force_ranged,force_melee,reload_allowed)->ranged //Is the attacked ranged?
          && (!weapon->get_attack(force_ranged,force_melee,reload_allowed)->uses_ammo //Does it not use ammo
              || weapon->get_ammoamount() != 0);                                      //or does it have ammo?

   }
   else
      r = false;

   return r;
}

bool Creature::can_reload() const
{
   //return !clips.empty(); //Can not be sure creature only has appropriate clips.
   bool can_reload = false;
   
   for (unsigned i = 0; i < clips.size() && !can_reload; ++i)
      can_reload = get_weapon().acceptable_ammo(*clips[i]);
   
   return can_reload;
}

bool Creature::will_reload(bool force_ranged, bool force_melee) const
{
   bool r;
   r = get_weapon().uses_ammo()              //Does it use ammo?
       && get_weapon().get_ammoamount() == 0 //Is it out of ammo?
       && can_reload()                       //Is reloading possible?
       && get_weapon().get_attack(force_ranged, force_melee, false) != NULL //Is there an appropriate attack for the situation?
       && get_weapon().get_attack(force_ranged, force_melee, false)->uses_ammo; //Does the attack need ammo?
   /*if (get_weapon().uses_ammo() && get_weapon().get_ammoamount() == 0)
   {
      if (can_reload() && get_weapon().get_attack(force_ranged, force_melee, false) != NULL)
      {
         if (get_weapon().get_attack(force_ranged, force_melee, false)->uses_ammo)
            r = true;
      }
   }*/
   return r;
}

bool Creature::reload(bool wasteful)
{
   bool r;
   if (get_weapon().uses_ammo() && !clips.empty()
       && (wasteful || get_weapon().get_ammoamount() == 0))
   {
      r = weapon->reload(*clips.front());
      if (clips.front()->empty())
      {
         delete clips.front();
         clips.pop_front();
      }
   }
   else
      r = false;
   
   return r;
}

bool Creature::ready_another_throwing_weapon()
{
   bool r = false;
   if (!extra_throwing_weapons.empty())
   {
      weapon = extra_throwing_weapons.front()->split(1);
      if (extra_throwing_weapons.front()->empty())
      {
         delete extra_throwing_weapons.front();
         extra_throwing_weapons.pop_front();
      }
      r = true;
   }
   has_thrown_weapon = false;
   return r;
}

int Creature::count_clips() const
{
   int sum = 0;
   for (deque<Clip*>::const_iterator i = clips.begin(); i != clips.end(); ++i)
      sum += (*i)->get_number();
   return sum;
}

bool Creature::take_clips(Item& clip, int number)
{
   bool r;
   if (clip.is_clip())
      r = take_clips(static_cast<Clip&>(clip),number); //cast -XML
   else
      r = false;
   
   return r;
}

bool Creature::take_clips(Clip& clip, int number)
{
   bool r;
   
   if (number + count_clips() >= 9)
      number = 9 - count_clips();
   if (number > clip.get_number())
      number = clip.get_number();
   
   if (number > 0 && get_weapon().acceptable_ammo(clip))
   {
      Clip* c = clip.split(number);
      clips.push_back(c);
      r = true;
   }
   else
      r = false;
   
   return r;
}

void Creature::give_weapon(Weapon& w, vector<Item*>* lootpile)
{
   if (weapon != NULL && !w.empty())
   {
      if (weapon->is_throwable() && weapon->is_same_type(w))
      {
         int take_number = 10 - count_weapons();
         if (take_number > 0)
            extra_throwing_weapons.push_back(w.split(1));
         
         //if (lootpile == NULL || w.empty())
         //   delete &w;
         //else
         //   lootpile.push_back(&w);
      }
      else
      {
         if (lootpile == NULL)
         {
            delete weapon;
            while (!extra_throwing_weapons.empty())
            {
               delete extra_throwing_weapons.back();
               extra_throwing_weapons.pop_back();
            }
         }
         else
         {
            lootpile->push_back(weapon);
            while (!extra_throwing_weapons.empty())
            {
               lootpile->push_back(extra_throwing_weapons.back());
               extra_throwing_weapons.pop_back();
            }
         }
         
         weapon = w.split(1);
         //if (weapon->is_throwable())
         //   give_weapon(w,lootpile);
         //else if (lootpile == NULL || w.empty())
         //   delete &w;
         //else
         //   lootpile->push_back(&w);

         if (lootpile == NULL)
         {
            for (int i = clips.size()-1; i >= 0; --i)
            {
               if (!weapon->acceptable_ammo(*clips[i]))
               {
                  delete clips[i];
                  clips.erase(clips.begin() + i);
               }
            }
         }
         else
         {
            for (int i = clips.size()-1; i >= 0; --i)
            {
               if (!weapon->acceptable_ammo(*clips[i]))
               {
                  lootpile->push_back(clips[i]);
                  clips.erase(clips.begin() + i);
               }
            }
         }
      }
   }
   else if (!w.empty())
   {
      drop_weapons_and_clips(lootpile);
      weapon = w.split(1);
      //if (weapon->is_throwable())
      //   give_weapon(w,lootpile);
      //else if (lootpile == NULL || w.empty())
      //   delete &w;
      //else
      //   lootpile->push_back(&w);
   }
   //else if (w.empty())
   //   delete &w;
}

void Creature::drop_weapons_and_clips(vector<Item*>* lootpile)
{
   has_thrown_weapon = false;
   if (weapon != NULL)
   {
      if (lootpile != NULL)
         lootpile->push_back(weapon);
      else
         delete weapon;
      weapon = NULL;
   }
   
   while (!extra_throwing_weapons.empty())
   {
      if (lootpile != NULL)
         lootpile->push_back(extra_throwing_weapons.back());
      else
         delete extra_throwing_weapons.back();
      extra_throwing_weapons.pop_back();
   }
  
   while (!clips.empty())
   {
      if (lootpile != NULL)
         lootpile->push_back(clips.back());
      else
         delete clips.back();
      clips.pop_back();
   }
}

void Creature::drop_weapon(vector<Item*>* lootpile)
{
   if (!extra_throwing_weapons.empty())
      has_thrown_weapon = true;

   if (weapon != NULL)
   {
      if (lootpile != NULL)
         lootpile->push_back(weapon);
      else
         delete weapon;
      weapon = NULL;
   }
}

int Creature::count_weapons() const
{
   int sum = 0;
   if (weapon !=NULL)
      ++sum;
   for (unsigned i = 0; i < extra_throwing_weapons.size(); ++i)
      sum += extra_throwing_weapons[i]->get_number();

   return sum;
}

void Creature::give_armor(Armor& a, vector<Item*>* lootpile)
{
   if (!a.empty())
   {
      strip(lootpile);
      armor = a.split(1);
   }
}

void Creature::strip(vector<Item*>* lootpile)
{
   if (armor != NULL)
   {
      if (lootpile == NULL)
         delete armor;
      else
         lootpile->push_back(armor);
      armor = NULL;
   }
}

bool Creature::weapon_is_concealed() const
{
   bool concealed = false;
   //if (is_armed() && is_naked())
   //   concealed = false;
   if (is_armed() && !is_naked())
   {
      concealed = armor->conceals_weapon(*weapon);
   }
   return concealed;
}

string Creature::get_weapon_string(int subtype) const
{
   string r;
   if(is_armed())
   {
      r = weapon->get_name(subtype);
      if(weapon->uses_ammo())
      {
         r += " (" + tostring(weapon->get_ammoamount()) + "/" + tostring(count_clips()) + ")";
      }
      else if(weapon->is_throwable())
      {
         r += " (1/" + tostring(count_weapons()-1) + ")"; // -1 so not to count weapon in hands.
      }
   }
   else if(!extra_throwing_weapons.empty())
   {
      r = extra_throwing_weapons[0]->get_name(subtype);
      r += " (0/" + tostring(count_weapons()) + ")";
   }
   else
      r = "None";
   
   return r;
}

string Creature::get_armor_string(bool fullname) const
{
   string r;
   if(is_naked())
      r = "Naked";
   else
      r = armor->equip_title(fullname);
   return r;
}
