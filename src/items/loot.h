/**
 * Interface for the Loot class.
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
#ifndef LOOT_H
#define LOOT_H

#include "items/item.h"
#include <string>

class LootType;


class Loot
: public Item
{
public:
  explicit Loot(const LootType& seed, int number = 1);

  Loot(std::string const& inputXml);

  virtual Loot*
  clone() const
  { return new Loot(*this); }

  virtual bool
  is_loot() const
  { return true; }

  virtual Loot*
  split(int number);

  virtual bool
  merge(Item& i);

  virtual bool
  sort_compare_special(Item* other) const;

  virtual std::string
  equip_title() const;

  virtual std::string const&
  get_name() const;

  virtual long
  get_fencevalue() const;

  bool
  is_stackable() const;

  bool
  no_quick_fencing() const;

  bool
  is_cloth() const;

private:
  std::string
  item_class() const override;

};

#endif //LOOT_H
