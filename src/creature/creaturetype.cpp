/**
 * Implementation of the Creature Type class.
 */
/*
 * Copyright 2013 Carlos Gustavos  <blomkvist >
 * Copyright 2014 Rich McGrew (yetisyny)
 * Copyright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#include "creature/creaturetype.h"

#include <externs.h>
#include "politics/politics.h"
#include "tinyxml2.h"


namespace
{

// Assign a value to an Interval from a string or log error.
static void
assign_interval(Interval& i, std::string const& value,
                std::string const& owner, std::string const& element)
{
  try
  {
    i = Interval::from_string(value);
  }
  catch (std::exception& ex)
  {
    xmllog.log("Invalid interval for " + element + " in " + owner + ": " + value + "(" + ex.what() + ")");
  }
}

} // anonymous namespace

CreatureType::Id CreatureType::next_id_ = 1000;


CreatureType::WeaponsAndClips::
WeaponsAndClips(std::string const& weapon_type, Interval weapon_count,
                std::string const& clip_type,   Interval clip_count)
: weapontype(weapon_type)
, number_weapons(weapon_count)
, cliptype(clip_type)
, number_clips(clip_count)
{
  // Check values.
  if (weapontype != "CIVILIAN")
  {
    if (getweapontype(weapontype) == -1)
    {
       xmllog.log("Invalid weapon type for creature type: " + weapontype);
       weapontype = "WEAPON_NONE";
       cliptype = "NONE";
    }
    else
    {
       const vector<attackst*>& attacks = ::weapontype[getweapontype(weapontype)]->get_attacks();

       // Find a usable clip type for the weapon.
       if (cliptype == "APPROPRIATE")
       {
          cliptype = "NONE";
          for (auto const& attack: attacks)
          {
             if (attack->uses_ammo)
             {
                cliptype = attack->ammotype;
                break;
             }
          }
       }
       // Check clip is usable by the weapon.
       else if (getcliptype(cliptype) != -1) //Must be a clip type too.
       {
          int i;
          for (i=0; i<len(attacks) && cliptype != attacks[i]->ammotype; i++)
            ;

          if (i==len(attacks))
          {
             xmllog.log("Clip type " + cliptype +
                        "can not be used by " + weapontype + ".");
             cliptype = "NONE";
          }
       }
       // Undefined clip type.
       else
       {
          xmllog.log("Invalid clip type: " + cliptype);
          cliptype = "NONE";
       }
    }
  }
}


CreatureType::
CreatureType(std::string const& xml)
: id_(next_id_++)
, attributes_(ATTNUM, Interval(1, 10))
, gender_liberal_(GENDER_RANDOM)
, skills_(SKILLNUM, Interval(0))
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    std::cerr << "error parsing creature.\n";
    return;
  }

  for (auto node = doc.FirstChild(); node; node = node->NextSibling())
  {
    auto element = node->ToElement();
    if ((element != nullptr) && (element->Name() == "creaturetype"s))
    {
      auto attr_idname = element->Attribute("idname");
      if (attr_idname == nullptr)
      {
        this->idname_ = "UNKNOWN"s + std::to_string(id_);
        xmllog.log("Creature type " + std::to_string(id_)+" lacks idname.");
      }
      else
      {
        this->idname_ = attr_idname;
      }
      this->type_ = creaturetype_string_to_enum(this->idname_);

      for (auto e = element->FirstChildElement(); e; e = e->NextSiblingElement())
      {
        std::string ename = e->Name();
        if (ename == "alignment")
        {
          std::string alignment = e->GetText();
          if (!from_string(alignment, this->alignment_))
          {
           xmllog.log("Invalid alignment for " + this->idname_ + ": " + alignment);
          }
        }
        else if (ename == "age")
        {
          std::string age = e->GetText();
          if (age == "DOGYEARS")
            this->age_ = {2, 6};
          else if (age == "CHILD")
            this->age_ = {7, 10};
          else if (age == "TEENAGER")
            this->age_ = {14, 17};
          else if (age == "YOUNGADULT")
            this->age_ = {18,35};
          else if (age == "MATURE")
            this->age_ = {20, 59};
          else if (age == "GRADUATE")
            this->age_ = {26, 59};
          else if (age == "MIDDLEAGED")
            this->age_ = {35, 59};
          else if (age == "SENIOR")
            this->age_ = {65, 94};
          else
            assign_interval(this->age_, age, this->idname_, ename);
        }
        else if (ename == "attribute_points")
        {
          assign_interval(this->attribute_points_, e->GetText(), this->idname_, ename);
        }
        else if (ename == "attributes")
        {
          for (auto ae = e->FirstChildElement(); ae; ae = ae->NextSiblingElement())
          {
            int attribute = attribute_string_to_enum(ae->Name());
            if (attribute != -1)
               assign_interval(this->attributes_[attribute], ae->GetText(), this->idname_, ename);
            else
               xmllog.log("Unknown attribute in " + this->idname_ + ": " + ae->Name());
          }
        }
        else if (ename == "juice")
        {
          assign_interval(this->juice_, e->GetText(), this->idname_, ename);
        }
        else if (ename == "gender")
        {
          int gender = gender_string_to_enum(e->GetText());
          if (gender != -1 && gender != GENDER_WHITEMALEPATRIARCH)
            this->gender_liberal_ = gender;
          else
            xmllog.log("Invalid gender for " + this->idname_ + ": " + e->GetText());
        }
        else if (ename == "infiltration")
        {
          assign_interval(this->infiltration_, e->GetText(), this->idname_, ename);
        }
        else if (ename == "money")
        {
          assign_interval(this->money_, e->GetText(), this->idname_, ename);
        }
        else if (ename == "skills")
        {
          for (auto se = e->FirstChildElement(); se; se = se->NextSiblingElement())
          {
            int skill = skill_string_to_enum(se->Name());
            if (skill != -1)
               assign_interval(this->skills_[skill], se->GetText(), this->idname_, ename);
            else
               xmllog.log("Unknown skill for " + this->idname_ + ": " + se->Name());
          }
        }
        else if (ename == "armor")
        {
          if (getarmortype(e->GetText()) != -1)
            this->armortypes_.push_back(e->GetText());
          else
            xmllog.log("Invalid armor type for " + this->idname_ + ": " + e->GetText());;
        }
        else if (ename == "weapon")
        {
          const char*  weapon_type = e->GetText();
          Interval     weapon_count{1};
          const char*  clip_type{"APPROPRIATE"};
          Interval     clip_count{0};
          if (weapon_type == nullptr)
          {
            for (auto we = e->FirstChildElement(); we; we = we->NextSiblingElement())
            {
              std::string wname = we->Name();
              if (wname == "type")
              {
                weapon_type = we->GetText();
              }
              else if (wname == "number_weapons")
              {
                assign_interval(weapon_count, we->GetText(), this->idname_, ename);
              }
              else if (wname == "cliptype")
              {
                clip_type = we->GetText();
              }
              else if (wname == "number_clips")
              {
                assign_interval(clip_count, we->GetText(), this->idname_, ename);
              }
            }
          }
          weapons_and_clips_.emplace_back(weapon_type, weapon_count, clip_type, clip_count);
        }
        else if (ename == "encounter_name")
        {
          encounter_name_ = e->GetText();
        }
        else if (ename == "type_name")
        {
          type_name_ = e->GetText();
        }
        else
        {
          xmllog.log("Unknown element for " + this->idname_ + ": " + ename);
        }
      }
    }
  }

  if (!len(this->type_name_))
  {
    this->type_name_ = "UNDEFINED";
    xmllog.log("type_name not defined for " + this->idname_ + ".");
  }

  // If no weapon type has been given then use WEAPON_NONE.
  if (!len(this->weapons_and_clips_))
    this->weapons_and_clips_.emplace_back("WEAPON_NONE", 1, "NONE", 0);

  // If no armor type has been given then use ARMOR_NONE.
  if (!len(this->armortypes_))
    this->armortypes_.emplace_back("ARMOR_NONE");
}

void CreatureType::
make_creature(Creature& cr) const
{
   cr.type_idname=idname_;
   cr.align = get_alignment();
   cr.age=age_.roll();
   cr.juice=juice_.roll();
   cr.gender_liberal=cr.gender_conservative=roll_gender();
   cr.infiltration=roll_infiltration();
   cr.money=money_.roll();
   strcpy(cr.name,get_encounter_name());
   for(int i=0;i<SKILLNUM;i++) cr.set_skill(i,skills_[i].roll());
   give_armor(cr);
   give_weapon(cr);
}


Alignment
CreatureType::get_alignment() const
{
   if (this->alignment_ == Alignment::PUBLIC_MOOD)
   {
      Alignment a = Alignment::CONSERVATIVE;
      int mood = publicmood(-1);
      if (LCSrandom(100) < mood) a = Alignment::MODERATE;
      if (LCSrandom(100) < mood) a = Alignment::LIBERAL;
      return a;
   }
   return this->alignment_;
}


int CreatureType::
roll_gender() const
{
   int gender = LCSrandom(2)+1; // Male or female.
   switch(gender_liberal_)
   {
   case GENDER_NEUTRAL: return GENDER_NEUTRAL;
   case GENDER_MALE:    return GENDER_MALE;
   case GENDER_FEMALE:  return GENDER_FEMALE;
   case GENDER_MALE_BIAS:
      if(law[LAW_WOMEN] == Alignment::ARCH_CONSERVATIVE||
        (law[LAW_WOMEN] == Alignment::CONSERVATIVE&&LCSrandom(25))||
        (law[LAW_WOMEN] == Alignment::MODERATE&&LCSrandom(10))||
        (law[LAW_WOMEN] == Alignment::LIBERAL&&LCSrandom( 4)))
         return GENDER_MALE;
   case GENDER_FEMALE_BIAS:
      if(law[LAW_WOMEN] == Alignment::ARCH_CONSERVATIVE||
        (law[LAW_WOMEN] == Alignment::CONSERVATIVE&&LCSrandom(25))||
        (law[LAW_WOMEN] == Alignment::MODERATE&&LCSrandom(10))||
        (law[LAW_WOMEN] == Alignment::LIBERAL&&LCSrandom( 4)))
         return GENDER_FEMALE;
   }
   return gender;
}


float CreatureType::
roll_infiltration() const
{
   return infiltration_.roll()/100.0f;
}


std::string CreatureType::
get_encounter_name() const
{
   if (len(this->encounter_name_))
      return this->encounter_name_;
   return get_type_name();
}


std::string CreatureType::
get_type_name() const
{
   switch(type_) // Hardcoded special cases.
   {
   case CREATURE_WORKER_SERVANT:
      if(law[LAW_LABOR] == Alignment::ARCH_CONSERVATIVE&&law[LAW_CORPORATE] == Alignment::ARCH_CONSERVATIVE) return "Slave";
      break;
   case CREATURE_WORKER_JANITOR:
      if (law[LAW_LABOR] == Alignment::ELITE_LIBERAL) return "Custodian";
      break;
   case CREATURE_WORKER_SWEATSHOP:
      if(law[LAW_LABOR] == Alignment::ELITE_LIBERAL&&law[LAW_IMMIGRATION] == Alignment::ELITE_LIBERAL) return "Migrant Worker";
      break;
   case CREATURE_CARSALESMAN:
      if(law[LAW_WOMEN] == Alignment::ARCH_CONSERVATIVE) return "Car Salesman";
      break;
   case CREATURE_FIREFIGHTER:
      if(law[LAW_FREESPEECH] == Alignment::ARCH_CONSERVATIVE) return "Fireman";
      break;
   }
   return type_name_;
}


void CreatureType::
give_weapon(Creature& cr) const
{
   const WeaponsAndClips& wc = pickrandom(weapons_and_clips_);

   if (wc.weapontype == "CIVILIAN")
      give_weapon_civilian(cr);
   else
     if (wc.weapontype != "WEAPON_NONE")
     {
       Weapon w(*weapontype[getweapontype(wc.weapontype)], wc.number_weapons.roll());
       w.set_number(min(w.get_number(),10L));
       while(!w.empty()) cr.give_weapon(w,nullptr);
       if (wc.cliptype != "NONE")
       {
         int n = wc.number_clips.roll();
         cr.take_clips(*cliptype[getcliptype(wc.cliptype)], n);
         cr.reload(false);
       }
     }
}


void CreatureType::
give_weapon_civilian(Creature& cr) const
{
   if (law[LAW_GUNCONTROL] == Alignment::CONSERVATIVE && !LCSrandom(30))
   {
      cr.give_weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")], nullptr);
      cr.take_clips(*cliptype[getcliptype("CLIP_38")], 4);
      cr.reload(false);
   }
   else if (law[LAW_GUNCONTROL] == Alignment::ARCH_CONSERVATIVE)
   {
      if (!LCSrandom(10))
      {
         cr.give_weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")], nullptr);
         cr.take_clips(*cliptype[getcliptype("CLIP_9")], 4);
         cr.reload(false);
      }
      else if (!LCSrandom(9))
      {
         cr.give_weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_45")], nullptr);
         cr.take_clips(*cliptype[getcliptype("CLIP_45")], 4);
         cr.reload(false);
      }
   }
}

void CreatureType::
give_armor(Creature& cr) const
{
   const std::string str = pickrandom(armortypes_);
   if (str != "ARMOR_NONE")
      cr.give_armor(*armortype[getarmortype(str)], nullptr);
}
