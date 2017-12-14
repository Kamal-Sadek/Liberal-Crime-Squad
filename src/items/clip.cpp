/**
 * Implementation of the Clip class.
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
#include "items/clip.h"
#include <externs.h>

Clip::
Clip(ClipType const& type, int number)
: Item(type, number)
{ }

Clip::
Clip(std::string const& inputXml)
: Item(inputXml)
{ }


std::string Clip::
item_class() const
{ return "class"; }


Clip* Clip::
split(int number)
{
   if(number>number_) number=number_;
   Clip* newi=this->clone();
   newi->number_=number;
   this->number_-=number;
   return newi;
}


bool Clip::
merge(Item& i)
{
   if(i.is_clip() && this->is_same_type(i))
   {
      this->increase_number(i.get_number());
      i.set_number(0);
      return true;
   }
   return false;
}


bool Clip::
sort_compare_special(Item* other) const
{
   if(other)
   {
      int thisi=getcliptype(itemtypename());
      int otheri=getcliptype(other->get_itemtypename());
      if(thisi<otheri||otheri==-1) return false;
      else if(thisi>otheri&&otheri!=-1) return true;
      else return false;
   }
   else return false;
}

string Clip::equip_title() const
{ return cliptype[getcliptype(itemtypename())]->get_name(); }

const string& Clip::get_name() const
{ return cliptype[getcliptype(itemtypename())]->get_name(); }

long Clip::get_fencevalue() const
{ return cliptype[getcliptype(itemtypename())]->get_fencevalue(); }

int Clip::get_ammoamount() const
{ return cliptype[getcliptype(itemtypename())]->get_ammoamount(); }
