/**
 * Interface for the base WeaponType class.
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
#ifndef WEAPONTYPE_H
#define WEAPONTYPE_H

#include "includes.h"

// Struct to hold all values related to an attack performable by the weapon type.
struct attackst
{
   attackst(std::string const& xmlstring);
   int priority;
   bool ranged;
   bool thrown;
   string ammotype;
   bool uses_ammo;
   string attack_description;
   string hit_description;
   bool always_describe_hit;
   bool can_backstab;
   string hit_punctuation;
   int skill;
   int accuracy_bonus;
   int number_attacks;
   int successive_attacks_difficulty;
   int strength_min;
   int strength_max;
   int random_damage;
   int fixed_damage;
   bool bruises;
   bool tears;
   bool cuts;
   bool burns;
   bool shoots;
   bool bleeding;
   int severtype;
   bool damages_armor;
   int armorpiercing;
   int no_damage_reduction_for_limbs_chance;
  
   struct criticalst
   {
      criticalst();
      int chance;
      int hits_required;
      int random_damage;
      bool random_damage_defined;
      int fixed_damage;
      bool fixed_damage_defined;
      int severtype;
      bool severtype_defined;
   } critical;
   
   struct firest
   {
      firest();   
      int chance;
      int chance_causes_debris;
   } fire;
};

class WeaponType : public ItemType
{
   public:
      // Constructor to create a weapon type from xml.
      explicit WeaponType(std::string const& xmlstring);
      ~WeaponType();
      
      bool is_weapon() const { return true; }
   
      using ItemType::get_name;
      // Returns the name of the weapon type of given subtype, dependent on year.
      // Subtype may be 0, 1 or 2. Any other values will be the same as 0. If
      // subtypes 1 or 2 are not defined, subtype 0 will be used instead.
      // Subtype 0 is equivalent to ItemType::get_name() (actually just calls that).
      const string& get_name(unsigned subtype) const;
      // Return the shorter name.
      const string& get_shortname(unsigned subtype = 0) const;   
      
      // Returns if the weapon type can be used to take hostages without causing
      // alarm. 
      bool can_take_hostages() const { return can_take_hostages_; }
      // Returns if the weapon type can be used to threaten landlords.
      bool is_threatening() const { return threatening_; }
      
      bool can_threaten_hostages() const { return can_threaten_hostages_; }
      
      bool protects_against_kidnapping() const { return protects_against_kidnapping_; }
      // Returns if the weapon will use a musical attack in combat.
      bool has_musical_attack() const { return musical_attack_; }
      // Returns whether the weapon type is considered an instrument when fund
      // raising with music.
      bool is_instrument() const { return instrument_; }
      // Returns the most liberal gun control law for the weapon to be legal.
      // -2, -1, 0, 1 and 2 for C+, C, M, L and L+ respectively. -3 always illegal.
      int get_legality() const { return legality_; }
      // Checks if the weapon type is legal.
      bool is_legal() const;
      // Returns the bash bonus provided by the weapon type.
      float get_bashstrengthmod() const { return bashstrengthmod_; }
      // 
      bool is_suspicious() const { return suspicious_; }
      // Returns the size of the weapon type. Used for concealment under clothes.
      int get_size() const { return size_; }
      // Gives a reference to the vector of all possible attacks made by the
      // weapon type.
      const vector<attackst*>& get_attacks() const { return attacks_; }
      // Checks if the weapon uses ammo in any of its attacks.
      bool uses_ammo() const;
      // Checks if the given clip type is used by any of the weapon's attacks.
      bool acceptable_ammo(const ClipType& c) const { return acceptable_ammo(c.get_idname()); }
      bool acceptable_ammo(const string& clipname) const;
      // Checks if any of the weapon's attacks are ranged.
      bool is_ranged() const;
      // Checks if any of the weapon's attacks involve throwing the weapon.
      bool is_throwable() const;
      // Returns if the weapon type should always succeed breaking locks.
      bool auto_breaks_locks() const { return auto_break_lock_; }
      // Returns if the weapon type can be used to make graffiti.
      bool can_graffiti() const { return can_graffiti_; }
      
   
   private:
      string name_sub_1_;
      string name_sub_2_;
      bool name_sub_1_defined_;
      bool name_sub_2_defined_;
      string name_future_sub_1_;
      string name_future_sub_2_;
      bool name_future_sub_1_defined_;
      bool name_future_sub_2_defined_;
      
      string shortname_;
      string shortname_future_;
      bool shortname_defined_;
      bool shortname_future_defined_;
      string shortname_sub_1_;
      string shortname_sub_2_;
      bool shortname_sub_1_defined_;
      bool shortname_sub_2_defined_;
      string shortname_future_sub_1_;
      string shortname_future_sub_2_;
      bool shortname_future_sub_1_defined_;
      bool shortname_future_sub_2_defined_;
        
      bool can_take_hostages_;
      bool threatening_;
      bool can_threaten_hostages_;
      bool protects_against_kidnapping_;
      bool musical_attack_;
      bool instrument_;
      int legality_;
      float bashstrengthmod_;
      bool suspicious_;
      int size_;
      bool can_graffiti_;
      bool auto_break_lock_;
      
      vector<attackst*> attacks_;
      
};

#endif //WEAPONTYPE_H
