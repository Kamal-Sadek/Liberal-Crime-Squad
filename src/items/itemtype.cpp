/**
 * Implementation of the base ItemType class.
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
#include "items/itemtype.h"

#include <externs.h>
#include <sstream>
#include "tinyxml2.h"



int ItemType::number_of_itemtypes = 0;


ItemType::ItemType(std::string const& xml)
: id_{number_of_itemtypes++}
, name_{"UNDEFINED"}
, name_future_defined_{false}
, fencevalue_{0}
{
   init(xml);
}


ItemType::
ItemType(const ItemType& base, std::string const& xml)
: idname_{base.idname_}
, id_{number_of_itemtypes++}
, name_{base.name_}
, name_future_{base.name_future_}
, name_future_defined_{base.name_future_defined_}
, fencevalue_{base.fencevalue_}
{
   init(xml);
}


void ItemType::
init(std::string const& xml)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    std::ostringstream ostr;
    ostr << "error " << doc.ErrorID() << " parsing itemType XML"
         << " at line " << doc.GetErrorLineNum() << ": "
         << doc.GetErrorStr1() << " / " << doc.GetErrorStr2();
    addstr(ostr.str(), xmllog);
    getkey();
    return;
  }

  auto toplevel = doc.FirstChildElement();
  if (toplevel != nullptr)
  {
    auto attr_idname = toplevel->Attribute("idname");
    if (attr_idname != nullptr)
    {
      this->idname_ = attr_idname;
    }

    for (auto element = toplevel->FirstChildElement(); element; element = element->NextSiblingElement())
    {
      std::string tag = element->Name();
      if (tag == "name")
      {
        char const* val = element->GetText();
        if (val)
          this->name_ = val;
      }
      else if (tag == "name_future")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->name_future_ = (stringtobool(val) == 1);
          name_future_defined_ = true;
        }
      }
      else if (tag == "fencevalue" )
      {
        char const* val = element->GetText();
        if (val)
          this->fencevalue_ = std::stoi(val);
      }
    }
  }
}


std::string const& ItemType::
get_name() const
{
   if (name_future_defined_ && year >= 2100)
      return name_future_;
   else
      return name_;
}
