/**
 * Interface for the LootType class.
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
#ifndef LOOTTYPE_H
#define LOOTTYPE_H

#include "items/itemtype.h"

class LootType
: public ItemType
{
public:
  explicit LootType(std::string const& xml);
  
  bool is_loot() const { return true; }
  
  // Returns if the loot type should be stacked or not.
  bool is_stackable() const { return stackable_; }
  // Returns if the loot should be skipped when quick fencing all loot.
  bool no_quick_fencing() const { return no_quick_fencing_; }
  // Returns if the loot is usable as cloth when making clothing.
  bool is_cloth() const { return cloth_; }
  

private:
  bool stackable_{true};
  bool no_quick_fencing_{false};
  bool cloth_{false};
};

#endif //LOOTTYPE_H
