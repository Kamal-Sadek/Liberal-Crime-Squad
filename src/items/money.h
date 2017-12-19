/**
 * Interface for the Money class.
 */
/*
 * Copyright 2010, 2013 Carlos Gustavos  <blomkvist>
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
#ifndef MONEY_H
#define MONEY_H

#include "items/item.h"
#include <string>


class Money
: public Item
{
public:
  explicit Money(long amount) : amount_(amount) { }
  Money(std::string const& inputXml);

  virtual Money*
  clone() const
  { return new Money(*this); }

  virtual std::string
  equip_title() const;

  virtual std::string const&
  get_name() const
  { return itemtypename(); }

  virtual long
  get_fencevalue() const
  { return amount_ * number_; }

  virtual bool
  is_money() const
  { return true; }

  virtual Money*
  split(int number);

  virtual bool
  merge(Item& i);

  virtual bool
  sort_compare_special(Item* other) const;

  long
  get_amount() const
  { return amount_; }

  void
  set_amount(long amount)
  { amount_ = amount; }

  void
  add_amount(long amount)
  { amount_ += amount; }

  void
  take_all_from(Money& more)
  { amount_ += more.amount_; more.amount_ = 0; }

  void flatten()
  { amount_ = amount_ * number_; number_ = 1; }

private:
  std::string
  item_class() const override;

  std::string
  xml_details() const override;

private:
  long amount_;
};

#endif //MONEY_H
