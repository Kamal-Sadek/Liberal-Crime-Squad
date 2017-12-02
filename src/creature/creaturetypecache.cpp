/**
 * The CreatureType collection.
 */
/*
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
#include "creature/creaturetypecache.h"

#include "creature/creaturetype.h"
#include "tinyxml2.h"


void CreatureTypeCache::
load_from_xml_string(std::string const& xml)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    return;
  }

  for (auto node = doc.FirstChild(); node; node = node->NextSibling())
  {
    auto element = node->ToElement();
    if ((element != nullptr) && (element->Name() == "creatures"s))
    {
      for (auto def = element->FirstChildElement(); def; def = def->NextSiblingElement())
      {
        tinyxml2::XMLPrinter printer;
        def->Accept( &printer );
        this->creature_type_bag.push_back(std::make_unique<CreatureType>(printer.CStr()));
      }
    }
  }
}


CreatureType* CreatureTypeCache::
get_by_type(short type)
{
  for (auto const& ct: this->creature_type_bag)
  {
    if (ct->get_type() == type)
      return ct.get();
  }
  return nullptr;
}


CreatureType* CreatureTypeCache::
get_by_idname(std::string const& idname)
{
  for (auto const& ct: this->creature_type_bag)
  {
    if (ct->get_idname() == idname)
      return ct.get();
  }
  return nullptr;
}

