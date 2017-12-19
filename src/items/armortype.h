/**
 * Interface for the ArmorType class.
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
#ifndef ARMORTYPE_H
#define ARMORTYPE_H

#include "items/itemtype.h"


class WeaponType;


class ArmorType
: public ItemType
{
   public:
      explicit ArmorType(std::string const& xmlstring);
      ArmorType(const ArmorType& base, std::string const& xmlstring);
      
      bool is_armor() const { return true; }
      
      int get_make_difficulty() const { return make_difficulty_; }
      int get_make_price() const { return make_price_; }
      int get_quality_levels() const { return quality_levels_; }
      int get_durability() const { return durability_; }
      // Returns if making the armor should only be possible when death penalty
      // and police behavior laws are arch-conservative, ie when death squads
      // roam.
      bool deathsquad_legality() const { return deathsquad_legality_; }
      
      bool can_get_bloody() const { return can_get_bloody_; }
      bool can_get_damaged() const { return can_get_damaged_; }

      // Returns armor (protection) provided by armor type on specified body
      // part. Use the bodyparts enum values for the parameter.
      int get_armor(int bodypart) const;
      bool has_fireprotection() const { return fireprotection_; }
      // Returns if the armor type covers the specified body part.
      // Use the bodyparts enum values for the parameter.
      bool covers(int bodypart) const;
      // Returns if the armor type conceals the wearer's face. (Not actually used anywhere.)
      bool conceals_face() const { return conceal_face_; }
      // Returns the stealth bonus given by the armor
      int get_stealth_value() const { return stealth_value_; }

      const std::string& get_shortname() const;

      int get_interrogation_basepower() const { return interrogation_basepower_; }
      int get_interrogation_assaultbonus() const { return interrogation_assaultbonus_; }
      int get_interrogation_drugbonus() const { return interrogation_drugbonus_; }

      // Returns value for professional appearance. Used when soliciting donations.
      int get_professionalism() const { return professionalism_; }

      // Returns the largest weapon size concealed by the armor type.
      int get_weaponsize_concealment() const { return conceal_weaponsize_; }
      // Checks if the armor type conceals the given weapon.
      bool conceals_weapon(const WeaponType& weapon) const;
      bool conceals_weaponsize(int weaponsize) const;
      /*const vector<string>& get_appropriate_weapons() const { return appropriate_weapon_; }
      const std::string& get_appropriate_weapon(int index) const;*/
      
      //For masks:
      bool is_mask() const { return mask_; }
      bool is_surprise_mask() const { return surprise_mask_; }
      const std::string& get_description() const { return description_; }
      
   private:
      void init(std::string const& xmlstring);
      
   private:
      int make_difficulty_;
      int make_price_;
      bool deathsquad_legality_;
      
      bool can_get_bloody_;
      bool can_get_damaged_;

      int stealth_value_;
      int armor_body_;
      int armor_head_;
      int armor_limbs_;
      bool fireprotection_;
      bool cover_head_;
      bool cover_body_;
      bool cover_arms_;
      bool cover_legs_;
      bool conceal_face_;

      std::string shortname_;
      std::string shortname_future_;
      bool shortname_defined_;
      bool shortname_future_defined_;
      

      int interrogation_basepower_;
      int interrogation_assaultbonus_;
      int interrogation_drugbonus_;

      int professionalism_;

      int conceal_weaponsize_;
      //vector<string> appropriate_weapon_;
      
      //For masks:
      bool mask_;
      bool surprise_mask_;
      std::string description_;
      
      int quality_levels_;
      int durability_;
};

#endif //ARMORTYPE_H
