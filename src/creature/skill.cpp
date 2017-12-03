/**
 * Implementation of the creature Skill class.
 */
/*
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
#include "creature/skill.h"

#include <algorithm>
#include <cmarkup/Markup.h>
#include <stdlib.h>


Skill::
Skill()
: value{0}
, associated_attribute{-1}
, skill{-1}
{ }


Skill::
Skill(const std::string& inputXml)
{
   CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   xml.IntoElem();

   while(xml.FindElem())
   {
      std::string tag = xml.GetTagName();

      if (tag == "associated_attribute")
         associated_attribute = std::stoi(xml.GetData());
      else if (tag == "skill")
         skill = std::stoi(xml.GetData());
      else if (tag == "value")
      {
        int v = std::stoi(xml.GetData());
        value = std::max(std::min(v, MAX_SKILL_LEVEL), 0);
      }
   }
}


std::string Skill::
showXml() const
{
   CMarkup xml;
   xml.AddElem("skill");
   xml.IntoElem();
   xml.AddElem("associated_attribute", associated_attribute);
   xml.AddElem("skill", skill);
   xml.AddElem("value", std::min(value, MAX_SKILL_LEVEL));

   return xml.GetDoc();
}


CreatureAttribute Skill::
get_associated_attribute(int skill_type)
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


std::string Skill::
get_name(int skill_type)
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


