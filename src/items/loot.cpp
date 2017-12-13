/**
 * Implementation of the Loot class.
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
#include "items/loot.h"
#include <externs.h>

Loot::Loot(const LootType& seed, int number) : Item(seed,number)
{ }

Loot::Loot(std::string const& xml)
: Item(xml)
{
}


std::string Loot::
item_class() const
{ return "loot"; }


Loot* Loot::split(int number)
{
   if(number>number_) number=number_;
   Loot* newi=clone();
   newi->number_=number;
   number_-=number;
   return newi;
}

bool Loot::merge(Item& i)
{
   if(is_stackable()&&i.is_loot()&&is_same_type(i))
   {
      increase_number(i.get_number());
      i.set_number(0);
      return true;
   }
   return false;
}

bool Loot::sort_compare_special(Item* other) const
{
   if(other)
   {
      int thisi=getloottype(itemtypename());
      int otheri=getloottype(other->get_itemtypename());
      if(thisi<otheri||otheri==-1) return false;
      else if(thisi>otheri&&otheri!=-1) return true;
      else return false;
   }
   else return false;
}

string Loot::equip_title() const
{ return loottype[getloottype(itemtypename())]->get_name(); }

const string& Loot::get_name() const
{ return loottype[getloottype(itemtypename())]->get_name(); }

long Loot::get_fencevalue() const
{ return loottype[getloottype(itemtypename())]->get_fencevalue(); }

bool Loot::is_stackable() const
{ return loottype[getloottype(itemtypename())]->is_stackable(); }

bool Loot::no_quick_fencing() const
{ return loottype[getloottype(itemtypename())]->no_quick_fencing(); }

bool Loot::is_cloth() const
{ return loottype[getloottype(itemtypename())]->is_cloth(); }
