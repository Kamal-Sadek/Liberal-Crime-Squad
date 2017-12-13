/**
 * Interface for the Clip class.
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
#ifndef CLIP_H
#define CLIP_H

#include "items/item.h"
#include <string>


class ClipType;

class Clip
: public Item
{
public:
  Clip(const ClipType& type, int number = 1);

  Clip(std::string const& inputXml);

  virtual Clip*
  clone() const
  { return new Clip(*this); }
  
  bool
  is_clip() const
  { return true; }

  virtual Clip*
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

  int
  get_ammoamount() const;

private:
  std::string
  item_class() const override;
};

#endif //CLIP_H
