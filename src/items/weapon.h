/**
 * Implementation of the Weapon class.
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
#ifndef WEAPON_H
#define WEAPON_H

#include "includes.h"


struct attackst;


class Weapon
: public Item
{
public:
  explicit Weapon(const WeaponType& type, int number = 1);
  virtual Weapon* clone() const { return new Weapon(*this); }
  explicit Weapon(const std::string& inputXml);

  virtual string equip_title() const;

  virtual Weapon* split(int number);
  virtual bool merge(Item& i);
  virtual bool sort_compare_special(Item* other) const;

  const string& get_name() const;
  const string& get_name(unsigned subtype) const;
  const string& get_shortname(unsigned subtype = 0) const;
  long get_fencevalue() const;
  bool can_take_hostages() const;
  bool is_threatening() const;
  bool can_threaten_hostages() const;
  bool protects_against_kidnapping() const;
  bool has_musical_attack() const;
  bool is_instrument() const;
  int get_legality() const;
  float get_bashstrengthmod() const;
  bool is_suspicious() const;
  int get_size() const;
  bool can_graffiti() const;
  bool uses_ammo() const;
  bool acceptable_ammo(const Item& c) const;
  bool acceptable_ammo(const Clip& c) const;
  bool acceptable_ammo(const ClipType& c) const;
  bool is_ranged() const;
  bool is_throwable() const;
  bool auto_breaks_locks() const;
  bool is_legal() const;

  // Tries to reload the weapon with clip. If the weapon is reloaded, clip's
  // number is reduced by one.
  bool reload(Clip& clip);
  int get_ammoamount() const { return ammo_; }
  //void set_ammo(int b) { ammo_ = b; }
  void decrease_ammo(int d) { ammo_ -= d; }
  const string& get_loaded_cliptypename() const { return loaded_cliptype_; }
  const attackst* get_attack(bool force_ranged, bool force_melee, bool force_no_reload) const;

  virtual bool is_weapon() const { return true; }

private:
  std::string
  item_class() const override;

  std::string
  xml_details() const override;

private:
  string loaded_cliptype_{"NONE"};
  int    ammo_{0};
};


#endif //WEAPON_H
