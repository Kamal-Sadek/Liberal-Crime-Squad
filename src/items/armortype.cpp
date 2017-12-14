/**
 * Implementation of the ArmorType class.
 */
/*
 * Copyright 2010 Carlos Gustavos  <blomkvist>
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
#include "items/armortype.h"

#include "externs.h"
#include <sstream>
#include "tinyxml2.h"


namespace
{
  const std::string ARMORTYPE_XML_ARMORTYPE_ELEMENT{"armortype"};
} // anonymous namespace


ArmorType::
ArmorType(std::string const& xmlstring)
: ItemType(xmlstring),
   make_difficulty_(0), make_price_(0), deathsquad_legality_(false),
   can_get_bloody_(true), can_get_damaged_(true),
   stealth_value_(0), armor_body_(0), armor_head_(0), armor_limbs_(0), fireprotection_(false),
   cover_head_(false), cover_body_(true), cover_arms_(true), cover_legs_(true), conceal_face_(false),
   shortname_("UNDEF"), shortname_future_("UNDEF"), shortname_defined_(false), shortname_future_defined_(false),
   interrogation_basepower_(0), interrogation_assaultbonus_(0), interrogation_drugbonus_(0),
   professionalism_(2), conceal_weaponsize_(5),
   mask_(false), surprise_mask_(false), description_("UNDEF"),
   quality_levels_(4), durability_(10)
{
   init(xmlstring);
}


ArmorType::
ArmorType(const ArmorType& base, std::string const& xmlstring)
: ItemType(base, xmlstring),
   make_difficulty_(base.make_difficulty_), make_price_(base.make_price_), deathsquad_legality_(base.deathsquad_legality_),
   can_get_bloody_(base.can_get_bloody_), can_get_damaged_(base.can_get_damaged_),
   stealth_value_(base.stealth_value_), armor_body_(base.armor_body_), armor_head_(base.armor_head_), armor_limbs_(base.armor_limbs_), fireprotection_(base.fireprotection_),
   cover_head_(base.cover_head_), cover_body_(base.cover_body_), cover_arms_(base.cover_arms_), cover_legs_(base.cover_legs_), conceal_face_(base.conceal_face_),
   shortname_(base.shortname_), shortname_future_(base.shortname_future_), shortname_defined_(base.shortname_defined_), shortname_future_defined_(base.shortname_future_defined_),
   interrogation_basepower_(base.interrogation_basepower_), interrogation_assaultbonus_(base.interrogation_assaultbonus_), interrogation_drugbonus_(base.interrogation_drugbonus_),
   professionalism_(base.professionalism_), conceal_weaponsize_(base.conceal_weaponsize_),
   mask_(base.mask_), surprise_mask_(base.surprise_mask_), description_(base.description_),
   quality_levels_(base.quality_levels_), durability_(base.durability_)

{
   init(xmlstring);
}


void ArmorType::
init(std::string const& xml)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    std::ostringstream ostr;
    ostr << "error " << doc.ErrorID() << " parsing ArmorType XML"
         << " at line " << doc.GetErrorLineNum() << ": "
         << doc.GetErrorStr1() << " / " << doc.GetErrorStr2();
    addstr(ostr.str(), xmllog);
    getkey();
    return;
  }

  auto toplevel = doc.FirstChildElement();
  if (toplevel != nullptr && toplevel->Name() == ARMORTYPE_XML_ARMORTYPE_ELEMENT)
  {
    for (auto element = toplevel->FirstChildElement(); element; element = element->NextSiblingElement())
    {
      std::string tag = element->Name();
      if (tag == "make_difficulty")
      {
        char const* val = element->GetText();
        if (val)
          this->make_difficulty_ = std::stoi(val);
      }
      else if (tag == "make_price")
      {
        char const* val = element->GetText();
        if (val)
          this->make_price_ = std::stoi(val);
      }
      else if (tag == "deathsquad_legality")
      {
        char const* val = element->GetText();
        if (val)
          this->deathsquad_legality_ = (stringtobool(val)==1);
      }
      else if (tag == "can_get_bloody")
      {
        char const* val = element->GetText();
        if (val)
          this->can_get_bloody_ = (stringtobool(val)==1);
      }
      else if (tag == "can_get_damaged")
      {
        char const* val = element->GetText();
        if (val)
          this->can_get_damaged_ = (stringtobool(val)==1);
      }
      else if (tag == "armor")
      {
        for (auto e = element->FirstChildElement(); e; e = e->NextSiblingElement())
        {
          std::string tag = e->Name();
          if (tag == "body")
          {
            char const* val = e->GetText();
            if (val)
              this->armor_body_ = std::stoi(val);
          }
          else if (tag == "head")
          {
            char const* val = e->GetText();
            if (val)
              this->armor_head_ = std::stoi(val);
          }
          else if (tag == "limbs")
          {
            char const* val = e->GetText();
            if (val)
              this->armor_limbs_ = std::stoi(val);
          }
          else if (tag == "fireprotection")
          {
            char const* val = e->GetText();
            if (val)
              this->fireprotection_ = (stringtobool(val)==1);
          }
        }
      }
      else if (tag == "body_covering")
      {
        for (auto e = element->FirstChildElement(); e; e = e->NextSiblingElement())
        {
          std::string tag = e->Name();
          if (tag == "body")
          {
            char const* val = e->GetText();
            if (val)
              this->cover_body_ = (stringtobool(val)==1);
          }
          else if (tag == "head")
          {
            char const* val = e->GetText();
            if (val)
              this->cover_head_ = (stringtobool(val)==1);
          }
          else if (tag == "arms")
          {
            char const* val = e->GetText();
            if (val)
              this->cover_arms_ = (stringtobool(val)==1);
          }
          else if (tag == "legs")
          {
            char const* val = e->GetText();
            if (val)
              this->cover_legs_ = (stringtobool(val)==1);
          }
          else if (tag == "conceals_face")
          {
            char const* val = e->GetText();
            if (val)
              this->conceal_face_ = (stringtobool(val)==1);
          }
        }
      }
      else if (tag == "shortname")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->shortname_ = val;
          if (this->shortname_.length() > 14)
            this->shortname_.resize(14);
          this->shortname_defined_ = true;
        }
      }
      else if (tag == "interrogation")
      {
        for (auto e = element->FirstChildElement(); e; e = e->NextSiblingElement())
        {
          std::string tag = e->Name();
          if (tag == "basepower")
          {
            char const* val = e->GetText();
            if (val)
              this->interrogation_basepower_ = std::stoi(val);
          }
          else if (tag == "assaultbonus")
          {
            char const* val = e->GetText();
            if (val)
              this->interrogation_assaultbonus_ = std::stoi(val);
          }
          else if (tag == "drugbonus")
          {
            char const* val = e->GetText();
            if (val)
              this->interrogation_drugbonus_ = std::stoi(val);
          }
        }
      }
      else if (tag == "professionalism")
      {
        char const* val = element->GetText();
        if (val)
          this->professionalism_ = std::stoi(val);
      }
      else if (tag == "conceal_weapon_size")
      {
        char const* val = element->GetText();
        if (val)
          this->conceal_weaponsize_ = std::stoi(val);
      }
      else if (tag == "stealth_value")
      {
        char const* val = element->GetText();
        if (val)
          this->stealth_value_ = std::stoi(val);
      }
      else if (tag == "mask")
      {
        char const* val = element->GetText();
        if (val)
          this->mask_ = (stringtobool(val)==1);
      }
      else if (tag == "surprise")
      {
        char const* val = element->GetText();
        if (val)
          this->surprise_mask_ = (stringtobool(val)==1);
      }
      else if (tag == "description")
      {
        char const* val = element->GetText();
        if (val)
          this->description_ = val;
      }
      else if (tag == "qualitylevels")
      {
        char const* val = element->GetText();
        if (val)
          this->quality_levels_ = std::max(1, std::stoi(val));
      }
      else if (tag == "durability")
      {
        char const* val = element->GetText();
        if (val)
          this->durability_ = std::max(0, std::stoi(val));
      }
    }
  }

  if (!shortname_defined_ && len(name()) <= 14)
    shortname_ = name();
}


int ArmorType::
get_armor(int bodypart) const
{
   if(covers(bodypart))
   {
      switch(bodypart)
      {
         case BODYPART_HEAD:
            return armor_head_;

         case BODYPART_BODY:
            return armor_body_;

         case BODYPART_ARM_RIGHT:
         case BODYPART_ARM_LEFT:
            return armor_limbs_;

         case BODYPART_LEG_RIGHT:
         case BODYPART_LEG_LEFT:
            return armor_limbs_;
      }
   }

   return 0;
}


bool ArmorType::
covers(int bodypart) const
{
   switch(bodypart)
   {
      case BODYPART_HEAD: return cover_head_;
      case BODYPART_BODY: return cover_body_;
      case BODYPART_ARM_RIGHT:
      case BODYPART_ARM_LEFT: return cover_arms_;
      case BODYPART_LEG_RIGHT:
      case BODYPART_LEG_LEFT: return cover_legs_;
   }

   return false;
}


string const& ArmorType::
get_shortname() const
{
   if (shortname_future_defined_ && year >= 2100)
      return shortname_future_;
   else if (year >= 2100 && name_future_defined() && len(name_future()) <= 14)
      return name_future();
   else //if (shortname_defined_)
      return shortname_;
   /*else if (len(name()) <= 14)
      return name();
   else
      return "UNDEF";*/
}


bool ArmorType::
conceals_weapon(const WeaponType& weapon) const
{ return conceals_weaponsize(weapon.get_size()); }


bool ArmorType::
conceals_weaponsize(int weaponsize) const
{ return (conceal_weaponsize_ >= weaponsize); }

/*const string& get_appropriate_weapon(int index) const
{
   if (index < len(appropriate_weapon_))
      return appropriate_weapon_[index];
   else
      return "";
}*/
