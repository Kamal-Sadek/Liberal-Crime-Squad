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
#include "cmarkup/Markup.h"


Attribute::
Attribute()
: value(ATTR_DEFAULT_LEVEL)
, attribute(-1)
{ }


Attribute::
Attribute(std::string const& inputXml)
: Attribute()
{
   CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   xml.IntoElem();

   while(xml.FindElem())
   {
      std::string tag = xml.GetTagName();

      if (tag == "attribute")
         attribute = std::stoi(xml.GetData());
      else if (tag == "value")
         value = std::max(std::min(std::stoi(xml.GetData()), ATTR_MAX_LEVEL), ATTR_MIN_LEVEL);
   }
}


std::string Attribute::
showXml() const
{
   CMarkup xml;
   xml.AddElem("attribute");
   xml.IntoElem();
   xml.AddElem("attribute", attribute);
   xml.AddElem("value", std::min(value, ATTR_MAX_LEVEL));

   return xml.GetDoc();
}


std::string Attribute::
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

