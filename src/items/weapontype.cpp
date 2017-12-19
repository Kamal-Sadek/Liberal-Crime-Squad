/**
 * Implementation of the WeaponType class.
 */
/*
 * Copyright 2010, 2013 Carlos Gustavos  <blomkvist>
 * Copyright 2013, 2014 Rich McGrew (yetisyny)
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
#include "items/weapontype.h"

#include "externs.h"
#include <sstream>
#include "tinyxml2.h"


namespace
{
  const std::string WEAPONTYPE_XML_WEAPONTYPE_ELEMENT{"weapontype"};
  const std::string WEAPONTYPE_XML_ATTACK_ELEMENT{"attack"};
} // anonymous namespace


WeaponType::
WeaponType(std::string const& xml)
: ItemType(xml)
, name_sub_1_defined_(false)
, name_sub_2_defined_(false)
, name_future_sub_1_defined_(false)
, name_future_sub_2_defined_(false)
, shortname_("UNDEF")
, shortname_defined_(false)
, shortname_future_defined_(false)
, shortname_sub_1_defined_(false)
, shortname_sub_2_defined_(false)
, shortname_future_sub_1_defined_(false)
, shortname_future_sub_2_defined_(false)
, can_take_hostages_(false)
, threatening_(false)
, can_threaten_hostages_(true)
, protects_against_kidnapping_(true)
, musical_attack_(false)
, instrument_(false)
, legality_(2)
, bashstrengthmod_(1)
, suspicious_(true)
, size_(15)
, can_graffiti_(false)
, auto_break_lock_(false)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    std::ostringstream ostr;
    ostr << "error " << doc.ErrorID() << " parsing WeaponType XML"
         << " at line " << doc.GetErrorLineNum() << ": "
         << doc.GetErrorStr1() << " / " << doc.GetErrorStr2();
    addstr(ostr.str(), xmllog);
    getkey();
    return;
  }

  auto toplevel = doc.FirstChildElement();
  if (toplevel != nullptr && toplevel->Name() == WEAPONTYPE_XML_WEAPONTYPE_ELEMENT)
  {
    for (auto element = toplevel->FirstChildElement(); element; element = element->NextSiblingElement())
    {
      std::string tag = element->Name();
      if (tag == "shortname")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->shortname_ = val;
          this->shortname_defined_ = true;
        }
      }
      if (tag == "shortname_future")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->shortname_future_ = val;
          this->shortname_future_defined_ = true;
        }
      }
      if (tag == "name_sub_1")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->name_sub_1_ = val;
          this->name_sub_1_defined_ = true;
        }
      }
      if (tag == "name_sub_2")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->name_sub_2_ = val;
          this->name_sub_2_defined_ = true;
        }
      }
      if (tag == "name_future_sub_1")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->name_future_sub_1_ = val;
          this->name_future_sub_1_defined_ = true;
        }
      }
      if (tag == "shortname_sub_1")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->shortname_sub_1_ = val;
          this->shortname_sub_1_defined_ = true;
        }
      }
      if (tag == "shortname_sub_2")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->shortname_sub_2_ = val;
          this->shortname_sub_2_defined_ = true;
        }
      }
      if (tag == "shortname_future_sub_1")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->shortname_future_sub_1_ = val;
          this->shortname_future_sub_1_defined_ = true;
        }
      }
      if (tag == "shortname_future_sub_2")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->shortname_future_sub_2_ = val;
          this->shortname_future_sub_2_defined_ = true;
        }
      }
      else if (tag == "can_take_hostages")
      {
        char const* val = element->GetText();
        if (val)
          this->can_take_hostages_ = (stringtobool(val)==1);
      }
      else if (tag == "threatening")
      {
        char const* val = element->GetText();
        if (val)
          this->threatening_ = (stringtobool(val)==1);
      }
      else if (tag == "can_threaten_hostages")
      {
        char const* val = element->GetText();
        if (val)
          this->can_threaten_hostages_ = (stringtobool(val)==1);
      }
      else if (tag == "protects_against_kidnapping")
      {
        char const* val = element->GetText();
        if (val)
          this->protects_against_kidnapping_ = (stringtobool(val)==1);
      }
      else if (tag == "musical_attack")
      {
        char const* val = element->GetText();
        if (val)
          this->musical_attack_ = (stringtobool(val)==1);
      }
      else if (tag == "instrument")
      {
        char const* val = element->GetText();
        if (val)
          this->instrument_ = (stringtobool(val)==1);
      }
      else if (tag == "graffiti")
      {
        char const* val = element->GetText();
        if (val)
          this->can_graffiti_ = (stringtobool(val)==1);
      }
      else if (tag == "legality")
      {
        char const* val = element->GetText();
        if (val)
          this->legality_ = std::stoi(val);
      }
      else if (tag == "bashstrengthmod")
      {
        char const* val = element->GetText();
        if (val)
          this->bashstrengthmod_ = std::stoi(val) / 100.0;
      }
      else if (tag == "auto_break_locks")
      {
        char const* val = element->GetText();
        if (val)
          this->auto_break_lock_ = (stringtobool(val)==1);
      }
      else if (tag == "suspicious")
      {
        char const* val = element->GetText();
        if (val)
          this->suspicious_ = (stringtobool(val)==1);
      }
      else if (tag == "size")
      {
        char const* val = element->GetText();
        if (val)
          this->size_ = std::stoi(val);
      }
      else if (tag == "attack")
      {
        tinyxml2::XMLPrinter printer;
        element->Accept(&printer);
        attackst* attack = new attackst(printer.CStr());
        int i;
        for (i=0; i<len(this->attacks_) && attack->priority >= this->attacks_[i]->priority; i++)
          ;
        this->attacks_.insert(this->attacks_.begin()+i, attack);
      }
    }
  }
   
  if (!shortname_defined_)
  {
    if ((uses_ammo() && len(name()) <= 9) || len(name()) <= 14)
      shortname_ = name();
  }
  else
  {
    if (len(shortname_) > 9 && uses_ammo())
      shortname_.resize(9);
    else if (len(shortname_) > 14)
      shortname_.resize(14);
  }

}

attackst::
attackst(std::string const& xml)
: priority(1)
, ranged(false)
, thrown(false)
, ammotype("UNDEF")
, uses_ammo(false)
, attack_description("assaults")
, hit_description("striking")
, always_describe_hit(false)
, can_backstab(false)
, hit_punctuation(".")
, skill(SKILL_CLUB)
, accuracy_bonus(0)
, number_attacks(1)
, successive_attacks_difficulty(0)
, strength_min(5)
, strength_max(10)
, random_damage(1)
, fixed_damage(1)
, bruises(false)
, tears(false)
, cuts(false)
, burns(false)
, shoots(false)
, bleeding(false)
, severtype(0)
, damages_armor(false)
, armorpiercing(0)
, no_damage_reduction_for_limbs_chance(0)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    std::ostringstream ostr;
    ostr << "error " << doc.ErrorID() << " parsing attackst XML"
         << " at line " << doc.GetErrorLineNum() << ": "
         << doc.GetErrorStr1() << " / " << doc.GetErrorStr2();
    addstr(ostr.str(), xmllog);
    getkey();
    return;
  }

  auto toplevel = doc.FirstChildElement();
  if (toplevel != nullptr && toplevel->Name() == WEAPONTYPE_XML_ATTACK_ELEMENT)
  {
    for (auto element = toplevel->FirstChildElement(); element; element = element->NextSiblingElement())
    {
      std::string tag = element->Name();
      if (tag == "priority")
      {
        char const* val = element->GetText();
        if (val)
          this->priority = std::stoi(val);
      }
      else if (tag == "ranged")
      {
        char const* val = element->GetText();
        if (val)
          this->ranged = (stringtobool(val)==1);
      }
      else if (tag == "thrown")
      {
        char const* val = element->GetText();
        if (val)
          this->thrown = (stringtobool(val)==1);
      }
      else if (tag == "can_backstab")
      {
        char const* val = element->GetText();
        if (val)
          this->can_backstab = (stringtobool(val)==1);
      }
      else if (tag == "ammotype")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->ammotype = val;
          this->uses_ammo = true;
        }
      }
      else if (tag == "attack_description")
      {
        char const* val = element->GetText();
        if (val)
          this->attack_description = val;
      }
      else if (tag == "hit_description")
      {
        char const* val = element->GetText();
        if (val)
          this->hit_description = val;
      }
      else if (tag == "always_describe_hit")
      {
        char const* val = element->GetText();
        if (val)
          this->always_describe_hit = (stringtobool(val)==1);
      }
      else if (tag == "hit_punctuation")
      {
        char const* val = element->GetText();
        if (val)
          this->hit_punctuation = val;
      }
      else if (tag == "skill")
      {
        char const* val = element->GetText();
        if (val)
        {
          int s = skill_string_to_enum(val);
          if (s != -1)
            this->skill = s;
        }
      }
      else if (tag == "accuracy_bonus")
      {
        char const* val = element->GetText();
        if (val)
          this->accuracy_bonus = std::stoi(val);
      }
      else if (tag == "number_attacks")
      {
        char const* val = element->GetText();
        if (val)
          this->number_attacks = std::stoi(val);
      }
      else if (tag == "successive_attacks_difficulty")
      {
        char const* val = element->GetText();
        if (val)
          this->successive_attacks_difficulty = std::stoi(val);
      }
      else if (tag == "strength_min")
      {
        char const* val = element->GetText();
        if (val)
          this->strength_min = std::stoi(val);
      }
      else if (tag == "strength_max")
      {
        char const* val = element->GetText();
        if (val)
          this->strength_max = std::stoi(val);
      }
      else if (tag == "random_damage")
      {
        char const* val = element->GetText();
        if (val)
          this->random_damage = std::stoi(val);
      }
      else if (tag == "fixed_damage")
      {
        char const* val = element->GetText();
        if (val)
          this->fixed_damage = std::stoi(val);
      }
      else if (tag == "bruises")
      {
        char const* val = element->GetText();
        if (val)
          this->bruises = (stringtobool(val)==1);
      }
      else if (tag == "tears")
      {
        char const* val = element->GetText();
        if (val)
          this->tears = (stringtobool(val)==1);
      }
      else if (tag == "cuts")
      {
        char const* val = element->GetText();
        if (val)
          this->cuts = (stringtobool(val)==1);
      }
      else if (tag == "burns")
      {
        char const* val = element->GetText();
        if (val)
          this->burns = (stringtobool(val)==1);
      }
      else if (tag == "shoots")
      {
        char const* val = element->GetText();
        if (val)
          this->shoots = (stringtobool(val)==1);
      }
      else if (tag == "bleeding")
      {
        char const* val = element->GetText();
        if (val)
          this->bleeding = (stringtobool(val)==1);
      }
      else if (tag == "severtype")
      {
        char const* val = element->GetText();
        if (val)
        {
          int s = severtype_string_to_enum(val);
          if (s != -1)
            this->severtype = s;
        }
      }
      else if (tag == "damages_armor")
      {
        char const* val = element->GetText();
        if (val)
          this->damages_armor = (stringtobool(val)==1);
      }
      else if (tag == "armorpiercing")
      {
        char const* val = element->GetText();
        if (val)
          this->armorpiercing = std::stoi(val);
      }
      else if (tag == "no_damage_reduction_for_limbs_chance")
      {
        char const* val = element->GetText();
        if (val)
          this->no_damage_reduction_for_limbs_chance = std::stoi(val);
      }
      else if (tag == "critical")
      {
        for (auto e = element->FirstChildElement(); e; e = e->NextSiblingElement())
        {
          std::string tag = e->Name();
          if (tag == "chance")
          {
            char const* val = e->GetText();
            if (val)
              this->critical.chance = std::stoi(val);
          }
          else if (tag == "hits_required")
          {
            char const* val = e->GetText();
            if (val)
              this->critical.hits_required = std::stoi(val);
          }
          else if (tag == "random_damage")
          {
            char const* val = e->GetText();
            if (val)
            {
              this->critical.random_damage = std::stoi(val);
              this->critical.random_damage_defined = true;
            }
          }
          else if (tag == "fixed_damage")
          {
            char const* val = e->GetText();
            if (val)
            {
              this->critical.fixed_damage = std::stoi(val);
              this->critical.fixed_damage_defined = true;
            }
          }
          else if (tag == "severtype")
          {
            char const* val = element->GetText();
            if (val)
            {
              int s = severtype_string_to_enum(val);
              if (s != -1)
              {
                this->critical.severtype = s;
                this->critical.severtype_defined = true;
              }
            }
          }
        }
      }
      else if (tag == "fire")
      {
        for (auto e = element->FirstChildElement(); e; e = e->NextSiblingElement())
        {
          std::string tag = e->Name();
          if (tag == "chance")
          {
            char const* val = e->GetText();
            if (val)
              this->fire.chance = std::stoi(val);
          }
          else if (tag == "chance_causes_debris")
          {
            char const* val = e->GetText();
            if (val)
              this->fire.chance_causes_debris = std::stoi(val);
          }
        }
      }
    }
  }

  if (!bruises && !tears && !cuts && !burns && !shoots)
    bruises = true; //If no type specified, then bruise.
}


attackst::criticalst::criticalst()
 : chance(0), hits_required(1), random_damage(1), random_damage_defined(false),
   fixed_damage(1), fixed_damage_defined(false), severtype(0), severtype_defined(false)
{

}

attackst::firest::firest()
 : chance(0), chance_causes_debris(0)
{

}

WeaponType::~WeaponType()
{
   delete_and_clear(attacks_);
}

const string& WeaponType::get_name(unsigned subtype) const
{
   if (subtype == 0)
      return get_name();
   else if (subtype == 1)
   {
      if (name_future_sub_1_defined_ && year >= 2100)
         return name_future_sub_1_;
      else if (name_sub_1_defined_)
         return name_sub_1_;
      else
         return get_name();
   }
   else if (subtype == 2)
   {
      if (name_future_sub_2_defined_ && year >= 2100)
         return name_future_sub_2_;
      else if (name_sub_2_defined_)
         return name_sub_2_;
      else
         return get_name();
   }
   else
      return get_name(); //return "INVALID SUBTYPE"; //Reference to temporary. -XML
}

const string& WeaponType::get_shortname(unsigned subtype) const
{
   if (subtype > 2)
      return shortname_; //return "INVALID SUBTYPE"; //Reference to temporary. -XML
   else if (subtype == 1)
   {
      if (shortname_future_sub_1_defined_ && year >= 2100)
         return shortname_future_sub_1_;
      else if (year >= 2100 && name_future_sub_1_defined_ && len(name_future_sub_1_) <= 14) //Too long for ammo using weapons. -XML
         return name_future_sub_1_;
      else if (shortname_sub_1_defined_)
         return shortname_sub_1_;
      else if (name_sub_1_defined_ && len(name_sub_1_) <= 14) //Too long for ammo using weapons. -XML
         return name_sub_1_;
   }
   else if (subtype == 2)
   {
      if (shortname_future_sub_2_defined_ && year >= 2100)
         return shortname_future_sub_2_;
      else if (year >= 2100 && name_future_sub_2_defined_ && len(name_future_sub_2_) <= 14) //Too long for ammo using weapons. -XML
         return name_future_sub_2_;
      else if (shortname_sub_2_defined_)
         return shortname_sub_2_;
      else if (name_sub_2_defined_ && len(name_sub_2_) <= 14) //Too long for ammo using weapons. -XML
         return name_sub_2_;
   }

   if (shortname_future_defined_ && year >= 2100)
      return shortname_future_;
   else if (year >= 2100 && name_future_defined() && len(name_future()) <= 14) //Too long for ammo using weapons. -XML
      return name_future();
   else //if (shortname_defined_)
      return shortname_;
   /*else if (len(name()) <= 14) //Too long for ammo using weapons. -XML
      return name();
   else
      return "UNDEF";*/
}

bool WeaponType::uses_ammo() const
{
   for(int i=0;i<len(attacks_);i++)
      if(attacks_[i]->uses_ammo)
        return true;
   return false;
}

bool WeaponType::acceptable_ammo(const string& clipname) const
{
   for(int i=0;i<len(attacks_);i++)
      if(attacks_[i]->ammotype==clipname)
        return true;
   return false;
}

bool WeaponType::is_ranged() const
{
   for(int i=0;i<len(attacks_);i++)
      if(attacks_[i]->ranged)
        return true;
   return false;
}

bool WeaponType::is_throwable() const
{
   for(int i=0;i<len(attacks_);i++)
      if(attacks_[i]->thrown)
        return true;
   return false;
}

bool WeaponType::is_legal() const
{
   return legality_ >= to_index(law[LAW_GUNCONTROL])-2;
}
