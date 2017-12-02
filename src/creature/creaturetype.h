/**
 * Interface for the Creature Type class.
 */
/*
 * Copyright 2013 Carlos Gustavos  <blomkvist >
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
#ifndef CREATURE_TYPE_H
#define CREATURE_TYPE_H

#include "common/interval.h"
#include "politics/alignment.h"
#include <string>
#include <vector>


class Creature;


/**
 * A template for creating creatures.
 */
class CreatureType
{
  using Id = std::size_t;

  public:
    /** Construct a creature type from an XML string. */
    explicit CreatureType(std::string const& xmlstring);

    friend void makecreature(Creature &cr, short type);

    // Remakes the creature into the creature type. Depends on being called
    // from makecreature currently.
    void
    make_creature(Creature& cr) const;

    /** Get the ID tag of this creature type. */
    std::string const&
    get_idname() const
    { return idname_; }
    
    /** Get the internal ID of this creature type. */
    Id
    get_id() const
    { return id_; }
    
    /** Get the internal type enum of this creature type. */
    short
    get_type() const
    { return type_; }

    /** Get the age range rule. */
    Interval
    age_range() const
    { return this->age_; }

    int
    roll_age() const
    { return this->age_.roll(); }

    /** Get the alignment rules for this creature type. */
    Alignment
    alignment() const
    { return this->alignment_; }

    /** Roll an alignment based on the alignment rules for this creature type.  */
    Alignment
    roll_alignment() const;

    /** Roll a gender based on the gender selection rules for this creature type. */
    int
    roll_gender() const;

    /** Roll a number of allocatable attribute points. */
    int
    roll_available_attribute_points() const
    { return attribute_points_.roll(); }
    
    float
    roll_infiltration() const;
    
    std::string
    get_encounter_name() const;
    
    std::string
    get_type_name() const;

    // Gives the creature one of the creature type's possible weapons,
    // with clips if so defined.
    void
    give_weapon(Creature& cr) const;

    // May give a 9mm, .45 or .38 depending on gun control law and chance.
    void
    give_weapon_civilian(Creature& cr) const;

    // Gives the creature one of the creature type's possible armors.
    void
    give_armor(Creature& cr) const;

    /** Get the initial juice rule. */
    Interval
    juice() const
    { return this->juice_; }

    /** Roll starting juice based on the rules for this creature type. */
    int
    roll_juice() const
    { return this->juice_.roll(); }

    /** Get the initial money rule. */
    Interval
    money() const
    { return this->money_; }

    /** Roll starting money based on the rules for this creature type. */
    int
    roll_money() const
    { return this->money_.roll(); }

  private:
    struct WeaponsAndClips
    {
      WeaponsAndClips(std::string const& weapon, Interval weapons, std::string const& clip, Interval clips);

      std::string  weapontype;
      Interval     number_weapons;
      std::string  cliptype;
      Interval     number_clips;
    };
    using WeaponAndClipSelections = std::vector<WeaponsAndClips>;
    using ArmorTypeSelections     = std::vector<std::string>;
    using AttributeRanges         = std::vector<Interval>;
    using SkillRanges             = std::vector<Interval>;

  private:
    Id                           id_;
    std::string                  idname_;
    short                        type_; // This is a CreatureTypes enum value.
    std::string                  type_name_;
    WeaponAndClipSelections      weapons_and_clips_;
    ArmorTypeSelections          armortypes_;
    std::string                  encounter_name_;
    Interval                     age_{18, 57};
    Alignment                    alignment_{Alignment::PUBLIC_MOOD};
    Interval                     attribute_points_{40};
    AttributeRanges              attributes_;
    int                          gender_liberal_;
    Interval                     infiltration_{0};
    Interval                     juice_{0};
    Interval                     money_{20, 40};
    SkillRanges                  skills_;

    static Id next_id_;
};

#endif //CREATURE_TYPE_H
