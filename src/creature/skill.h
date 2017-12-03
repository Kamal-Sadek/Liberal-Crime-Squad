/**
 * Interface for the creature Skill class.
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
#ifndef LCS_CREATURE_SKILL_H_H
#define LCS_CREATURE_SKILL_H_H

#include "creature/attributes.h"
#include <string>


enum CreatureSkill
{
   PSEUDOSKILL_ESCAPEDRIVE = -2,
   PSEUDOSKILL_DODGEDRIVE,
   SKILL_ART = 0,
   SKILL_AXE,
   SKILL_BUSINESS,
   SKILL_CLUB,
   SKILL_COMPUTERS,
   SKILL_DISGUISE,
   SKILL_DODGE,
   SKILL_DRIVING,
   SKILL_FIRSTAID,
   SKILL_HEAVYWEAPONS,
   SKILL_KNIFE,
   SKILL_LAW,
   SKILL_HANDTOHAND, // actually this is called "Martial Arts"
   SKILL_MUSIC,
   SKILL_PERSUASION,
   SKILL_PISTOL,
   SKILL_PSYCHOLOGY,
   SKILL_RELIGION,
   SKILL_RIFLE,
   SKILL_SCIENCE,
   SKILL_SECURITY,
   SKILL_SEDUCTION,
   SKILL_SHOTGUN,
   SKILL_SMG,
   SKILL_STEALTH,
   SKILL_STREETSENSE,
   SKILL_SWORD,
   SKILL_TAILORING,
   SKILL_TEACHING,
   SKILL_THROWING,
   SKILL_WRITING,
   SKILLNUM
};

/**
 * The maximum allowable level that can be achieved in any skill.
 *
 * The value '99' was chosen to limit the display width to two digits.
 */
constexpr int MAX_SKILL_LEVEL = 99;

/**
 * The definition of a creature skill.
 *
 * A skill has an ID (currently from a hard-coded enumeration called CreatureSkill),
 * a name (magically associated with the hard-coded enumeration through the
 * static member function get_name()) and the ID of an associated character
 * attribute (mostly hard-coded through the static member function get_associated_attribute()).
 *
 * Use of the @p skill and @p associated_value members is unclear.
 *
 * Ultimately, this should morph into a loadable SkillType, which can be stashed
 * in a SkillTypeCache as stored in Pandora's box, and each creature should have
 * a SkillSet (of arbitrary size) associating a SkillType and a numeric value.
 */
class Skill
{
public:
  Skill() { }
  Skill(const std::string& inputXml);

  std::string
  showXml() const;

  int
  get_attribute() const
  { return associated_attribute; }

  void
  set_type(int skill_type)
  { skill=skill_type; associated_attribute=get_associated_attribute(skill); }

  static std::string get_name(int skill_type);
  static CreatureAttribute get_associated_attribute(int skill_type);

public:
  int value;

private:
  int associated_attribute;
  int skill;
};

#endif /* LCS_CREATURE_SKILL_H_H */
