/**
 * Implementation of the LootType class.
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
#include "externs.h"
#include <sstream>
#include "tinyxml2.h"


namespace
{
  const std::string LOOTTYPE_XML_LOOTTYPE_ELEMENT{"loottype"};
} // anonymous namespace


LootType::
LootType(std::string const& xml)
: ItemType(xml)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    std::ostringstream ostr;
    ostr << "error " << doc.ErrorID() << " parsing LootType XML"
         << " at line " << doc.GetErrorLineNum() << ": "
         << doc.GetErrorStr1() << " / " << doc.GetErrorStr2();
    addstr(ostr.str(), xmllog);
    getkey();
    return;
  }

  auto toplevel = doc.FirstChildElement();
  if (toplevel != nullptr && toplevel->Name() == LOOTTYPE_XML_LOOTTYPE_ELEMENT)
  {
    for (auto element = toplevel->FirstChildElement(); element; element = element->NextSiblingElement())
    {
      std::string tag = element->Name();
      if (tag == "stackable")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->stackable_ = (stringtobool(val) == 1);
        }
      }
      else if (tag == "no_quick_fencing")
      {
        char const* val = element->GetText();
        if (val)
        {
          this->no_quick_fencing_ = (stringtobool(val) == 1);
        }
      }
      else if (tag == "cloth" )
      {
        char const* val = element->GetText();
        if (val)
        {
          this->cloth_ = (stringtobool(val) == 1);
        }
      }
    }
  }
}
