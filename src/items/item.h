/**
 * Interface for the Item base class.
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
#ifndef ITEM_H
#define ITEM_H

#include <memory>
#include <string>


class ItemType;


class Item
{
public:
  using OwningPtr = std::unique_ptr<Item>;

public:
  Item(): number_(1) { }
  explicit Item(const ItemType& seed, int number = 1);
  // (Sub-?)Constructor to create item from xml.
  explicit Item(std::string const& inputXml);
  virtual ~Item() = 0;

  // Returns xml describing the item. Used when saving.
  std::string
  showXml() const;

  /** Factory to create an item from some XML. */
  static OwningPtr
  create_from_xml(std::string const& xml);

  virtual std::string equip_title() const=0;

  // Create copy of item and return pointer.
  virtual Item* clone() const=0;
  // Creates a copy of the item and divides the number between them.
  virtual Item* split(int number)=0;
  // Use to merge two identical items. Leaves referenced item in parameter
  // empty, ie number_=0.
  virtual bool merge(Item& i)=0;

  static bool sort_compare(Item* a, Item* b);
  virtual bool sort_compare_special(Item* other) const=0;
  virtual bool is_same_type(const Item& i) const { return itemtypename_==i.itemtypename_; }

  virtual bool is_weapon() const { return false; }
  virtual bool is_armor() const { return false; }
  virtual bool is_clip() const { return false; }
  virtual bool is_loot() const { return false; }
  virtual bool is_money() const { return false; }

  virtual std::string const& get_name() const=0;
  const char* aan() const;
  virtual long get_fencevalue() const=0;
  virtual bool is_good_for_sale() const { return true; } //Have one that gives an error message when suitable? -XML

  void set_number(long number) { number_=number; }
  void increase_number(long increase) { number_+=increase; }
  void decrease_number(long decrease) { number_-=decrease; }
  long get_number() const { return number_; }
  bool empty() const { return number_<=0; }

  std::string const& get_itemtypename() const { return itemtypename_; }
  long get_itemtypeid() const { return itemtypeid_; }

protected:
  //string& itemtypename() { return itemtypename_; }
  std::string const& itemtypename() const { return itemtypename_; }
  //long& itemtypeid() { return itemtypeid_; }
  const long& itemtypeid() const { return itemtypeid_; }

  long number_;

private:
  /** Override in derived class to give item class name. */
  virtual std::string
  item_class() const = 0;

  /** Override in derived class to provide additional detail elements. */
  virtual std::string
  xml_details() const;

private:
  std::string itemtypename_;
  long        itemtypeid_;
};


#endif //ITEM_H
