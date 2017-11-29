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

#include "includes.h"
#include "common/interval.h"
#include "politics/alignment.h"

class CreatureType
{
  public:
    // Creates a creature type from xml.
    explicit CreatureType(std::string const& xmlstring);

    friend void makecreature(Creature &cr, short type);

    // Remakes the creature into the creature type. Depends on being called
    // from makecreature currently.
    void
    make_creature(Creature& cr) const;

    std::string const&
    get_idname() const
    { return idname_; }
    
    long
    get_id() const
    { return id_; }
    
    short
    get_type() const
    { return type_; }

    Alignment
    get_alignment() const;

    int
    roll_gender() const;
    
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

    static int number_of_creaturetypes;

  private:
    struct WeaponsAndClips
    {
      WeaponsAndClips(std::string const& weapon, int weapons, std::string const& clip, int clips);
      WeaponsAndClips(CMarkup& xml, string const& owner);

      std::string  weapontype;
      Interval     number_weapons;
      std::string  cliptype;
      Interval     number_clips;
    };

  private:
    std::string                  idname_;
    long                         id_;
    short                        type_; // This is a CreatureTypes enum value.
    std::vector<WeaponsAndClips> weapons_and_clips_;
    std::vector<std::string>     armortypes_;
    std::string                  type_name_;
    std::string                  encounter_name_;
    Interval                     age_{18, 37};
    Alignment                    alignment_;
    Interval                     attribute_points_{40};
    std::vector<Interval>        attributes_;
    int                          gender_liberal_;
    Interval                     infiltration_{0};
    Interval                     juice_{0};
    Interval                     money_{20, 40};
    Interval                     skills_[SKILLNUM];
};

#endif //CREATURE_TYPE_H
