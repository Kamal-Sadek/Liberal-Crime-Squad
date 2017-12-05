/**
 * Implementation of the creature Attributes submodule.
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
#include "creature/attributes.h"

#include <algorithm>
#include "tinyxml2.h"

using std::to_string;
using std::string;


namespace
{
  const string ATTR_XML_ATTRIBUTE_TAG{"attribute"};
  const string ATTR_XML_VALUE_TAG{"value"};
} // anonymous namespace


Attribute::
Attribute()
: value(ATTR_DEFAULT_LEVEL)
, attribute(-1)
{ }


Attribute::
Attribute(string const& xml)
: Attribute()
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    doc.PrintError();
    return;
  }

  auto e = doc.FirstChildElement();
  if ((e != nullptr) && (e->Name() == ATTR_XML_ATTRIBUTE_TAG))
  {
    for (auto element = e->FirstChildElement(); element; element = element->NextSiblingElement())
    {
      string tag = element->Name();
      if (tag == ATTR_XML_ATTRIBUTE_TAG)
      {
        this->attribute = std::stoi(element->GetText());
      }
      else if (tag == ATTR_XML_VALUE_TAG)
      {
        string v = element->GetText();
        this->value = std::max(std::min(std::stoi(v), ATTR_MAX_LEVEL), ATTR_MIN_LEVEL);
      }
    }
  }
}


string Attribute::
showXml() const
{
   return string{
     "<" + ATTR_XML_ATTRIBUTE_TAG + ">"
       "<" + ATTR_XML_ATTRIBUTE_TAG + ">" + to_string(this->attribute) + "</" + ATTR_XML_ATTRIBUTE_TAG +">"
       "<" + ATTR_XML_VALUE_TAG + ">" + to_string(this->value) + "</" + ATTR_XML_VALUE_TAG +">"
     "</" + ATTR_XML_ATTRIBUTE_TAG +">"
   };
}


string Attribute::
get_name(int attribute_type)
{
   switch(attribute_type)
   {
   case ATTRIBUTE_STRENGTH:      return "STR";
   case ATTRIBUTE_AGILITY:       return "AGI";
   case ATTRIBUTE_WISDOM:        return "WIS";
   case ATTRIBUTE_INTELLIGENCE:  return "INT";
   case ATTRIBUTE_HEART:         return "HRT";
   case ATTRIBUTE_HEALTH:        return "HLTH";
   case ATTRIBUTE_CHARISMA:      return "CHA";
   }
   return "Error Attribute Name";
}

