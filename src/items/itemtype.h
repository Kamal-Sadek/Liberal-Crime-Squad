/**
 * Interface for the base ItemType class.
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
#ifndef ITEMTYPE_H
#define ITEMTYPE_H

#include <string>


class ItemType
{
public:
  //ItemType() { }
  // (Sub-?)Constructor for creating an item type from xml.
  explicit ItemType(std::string const& xml);

  // (Sub-?)Constructor for creating an item type from xml using another
  // item type for default values.
  ItemType(const ItemType& base, std::string const& xml);

  virtual ~ItemType() { }

  // Returns the unique (unless someone messed up an xml-file) identifying
  // name of the item type.
  std::string const& get_idname() const { return idname_; }
  // Return the id number. Id number may not be consistent between a
  // save'n'load if an xml-file has been changed.
  long get_id() const { return id_; }

  // Functions to identify the derived classes.
  virtual bool is_weapon() const { return false; }
  virtual bool is_armor() const { return false; }
  virtual bool is_clip() const { return false; }
  virtual bool is_loot() const { return false; }
  virtual bool is_money() const { return false; }

  // Returns the name of item type, dependent on the year.
  virtual std::string const& get_name() const;
  // Returns the fencevalue.
  virtual long get_fencevalue() const { return fencevalue_; }


  // Keeps count of the number of item types. Used for assigning id number.
  static int number_of_itemtypes;

protected:
  std::string const& idname() const { return idname_; }
  const long& id() const { return id_; }
  std::string const& name() const { return name_; }
  std::string const& name_future() const { return name_future_; }
  const bool& name_future_defined() const { return name_future_defined_; }
  const long& fencevalue() const { return fencevalue_; }

private:

  // Used by constructors to initialize values from xml.
  void init(std::string const& xml);

  //short type;
  std::string idname_;
  long id_;

  std::string name_;
  std::string name_future_;
  bool name_future_defined_;
  long fencevalue_;
};


#endif //ITEMTYPE_H
